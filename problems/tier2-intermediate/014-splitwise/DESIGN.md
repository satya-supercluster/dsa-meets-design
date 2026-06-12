# Design Walkthrough — Splitwise Expense-Sharing System

> This file is the answer guide. Only read after you've attempted the problem.

---

## The Core Design Decision

Three concerns compose in this problem:

1. **How to split?** — The split calculation varies (equal, exact, percentage). This is the Strategy pattern. Each strategy encapsulates its own math and validation.
2. **How to track debts?** — A HashMap of HashMaps (adjacency map) stores who owes whom. When new expenses come in, balances are updated and mutual debts are netted out.
3. **How to simplify?** — Debt simplification is a greedy algorithm on net balances. Compute each user's net position, then greedily match creditors with debtors.

```
ExpenseManager
    ├── unordered_map<userId, User>                    (user registry)
    ├── unordered_map<userId, unordered_map<userId, double>>  (balance graph)
    └── SplitStrategy* (swappable — EqualSplit / ExactSplit / PercentSplit)

addExpenseWithStrategy(paidBy, amount, participants, strategy, params):
    splits = strategy->split(amount, participants, params)
    for each split:
        if split.userId != paidBy:
            balances[split.userId][paidBy] += split.amount
            // Net out: if paidBy also owes split.userId, reduce both
```

---

## Why Strategy Pattern?

Without Strategy, you'd write something like:

```cpp
if (type == "EQUAL") {
    // equal split logic
} else if (type == "EXACT") {
    // exact split logic
} else if (type == "PERCENT") {
    // percent split logic
}
```

Every new split type means modifying the ExpenseManager. With Strategy:

```cpp
vector<Split> splits = strategy->split(amount, participants, params);
```

The ExpenseManager doesn't know or care how the split is calculated. Adding a new strategy (e.g., SplitByShares) requires zero changes to existing code.

---

## Balance Netting — The Key Data Structure Insight

A naive approach stores every individual debt separately. But if A owes B $50 and B owes A $30, we should store only "A owes B $20." This is called **netting**.

When updating balances after a split:
```cpp
void updateBalance(const string& debtor, const string& creditor, double amount) {
    // If creditor already owes debtor, reduce that first
    if (balances[creditor][debtor] > 0) {
        double offset = min(balances[creditor][debtor], amount);
        balances[creditor][debtor] -= offset;
        amount -= offset;
        if (balances[creditor][debtor] < 1e-9)
            balances[creditor].erase(debtor);
    }
    if (amount > 1e-9) {
        balances[debtor][creditor] += amount;
    }
}
```

This ensures the balance graph never has both A->B and B->A edges simultaneously.

---

## Debt Simplification — Greedy Algorithm

The insight: individual expense history doesn't matter for settlement. Only each user's **net position** matters.

1. Compute net balance: `net[user] = (total owed to user) - (total user owes)`
2. Positive net = creditor, negative net = debtor
3. Sort creditors descending, debtors by absolute value descending
4. Match largest creditor with largest debtor; transaction = min(credit, |debt|)
5. Reduce both; if either reaches zero, move to next

```cpp
vector<tuple<string, string, double>> simplifyDebts() {
    // Step 1: Compute net balances
    unordered_map<string, double> net;
    for (auto& [debtor, creditors] : balances) {
        for (auto& [creditor, amount] : creditors) {
            net[debtor] -= amount;
            net[creditor] += amount;
        }
    }

    // Step 2: Separate into creditors and debtors
    vector<pair<string, double>> creditors, debtors;
    for (auto& [user, amount] : net) {
        if (amount > 1e-9) creditors.push_back({user, amount});
        else if (amount < -1e-9) debtors.push_back({user, -amount});
    }

    // Step 3: Sort descending
    sort(creditors.begin(), creditors.end(),
         [](auto& a, auto& b) { return a.second > b.second; });
    sort(debtors.begin(), debtors.end(),
         [](auto& a, auto& b) { return a.second > b.second; });

    // Step 4: Greedy matching
    vector<tuple<string, string, double>> transactions;
    int i = 0, j = 0;
    while (i < creditors.size() && j < debtors.size()) {
        double settle = min(creditors[i].second, debtors[j].second);
        transactions.push_back({debtors[j].first, creditors[i].first, settle});
        creditors[i].second -= settle;
        debtors[j].second -= settle;
        if (creditors[i].second < 1e-9) i++;
        if (debtors[j].second < 1e-9) j++;
    }
    return transactions;
}
```

This greedy approach guarantees the minimum number of transactions when there is a single creditor or single debtor. For the general case, it produces at most `n-1` transactions (where `n` is the number of users with non-zero net balance), which is optimal.

---

## Reference Implementation

```cpp
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <cmath>
using namespace std;

struct User {
    string id;
    string name;
};

struct Split {
    string userId;
    double amount;
};

struct Expense {
    string id;
    string paidBy;
    double totalAmount;
    vector<Split> splits;
};

// ─── Strategy Interface ────────────────────────────────────────────────────

class SplitStrategy {
public:
    virtual vector<Split> split(double totalAmount,
                                const vector<string>& participants,
                                const vector<double>& params) = 0;
    virtual bool validate(double totalAmount,
                          const vector<string>& participants,
                          const vector<double>& params) = 0;
    virtual ~SplitStrategy() = default;
};

class EqualSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        vector<Split> result;
        double share = totalAmount / participants.size();
        for (auto& p : participants)
            result.push_back({p, share});
        return result;
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        return !participants.empty();
    }
};

class ExactSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        vector<Split> result;
        for (size_t i = 0; i < participants.size(); i++)
            result.push_back({participants[i], params[i]});
        return result;
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        if (params.size() != participants.size()) return false;
        double sum = 0;
        for (double v : params) sum += v;
        return fabs(sum - totalAmount) < 1e-9;
    }
};

class PercentSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        vector<Split> result;
        for (size_t i = 0; i < participants.size(); i++)
            result.push_back({participants[i], totalAmount * params[i] / 100.0});
        return result;
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        if (params.size() != participants.size()) return false;
        double sum = 0;
        for (double v : params) sum += v;
        return fabs(sum - 100.0) < 1e-9;
    }
};

// ─── Expense Manager ───────────────────────────────────────────────────────

class ExpenseManager {
    unordered_map<string, User> users;
    vector<Expense> expenses;
    unordered_map<string, unordered_map<string, double>> balances;

    void updateBalance(const string& debtor, const string& creditor, double amount) {
        if (debtor == creditor) return;
        if (balances[creditor].count(debtor) && balances[creditor][debtor] > 0) {
            double offset = min(balances[creditor][debtor], amount);
            balances[creditor][debtor] -= offset;
            amount -= offset;
            if (balances[creditor][debtor] < 1e-9)
                balances[creditor].erase(debtor);
        }
        if (amount > 1e-9)
            balances[debtor][creditor] += amount;
    }

public:
    void addUser(const string& userId, const string& name) {
        users[userId] = {userId, name};
    }

    void addExpense(const string& expenseId, const string& paidBy,
                    double amount, const vector<string>& participants) {
        EqualSplit strategy;
        vector<Split> splits = strategy.split(amount, participants, {});
        Expense expense{expenseId, paidBy, amount, splits};
        expenses.push_back(expense);
        for (auto& s : splits) {
            updateBalance(s.userId, paidBy, s.amount);
        }
    }

    void addExpenseWithStrategy(const string& expenseId, const string& paidBy,
                                double amount, const vector<string>& participants,
                                SplitStrategy* strategy,
                                const vector<double>& params) {
        if (!strategy->validate(amount, participants, params)) return;
        vector<Split> splits = strategy->split(amount, participants, params);
        Expense expense{expenseId, paidBy, amount, splits};
        expenses.push_back(expense);
        for (auto& s : splits) {
            updateBalance(s.userId, paidBy, s.amount);
        }
    }

    unordered_map<string, unordered_map<string, double>> getBalances() const {
        return balances;
    }

    vector<tuple<string, string, double>> simplifyDebts() {
        unordered_map<string, double> net;
        for (auto& [debtor, creditors] : balances) {
            for (auto& [creditor, amount] : creditors) {
                net[debtor] -= amount;
                net[creditor] += amount;
            }
        }
        vector<pair<string, double>> creditors, debtors;
        for (auto& [user, amount] : net) {
            if (amount > 1e-9) creditors.push_back({user, amount});
            else if (amount < -1e-9) debtors.push_back({user, -amount});
        }
        sort(creditors.begin(), creditors.end(),
             [](auto& a, auto& b) { return a.second > b.second; });
        sort(debtors.begin(), debtors.end(),
             [](auto& a, auto& b) { return a.second > b.second; });

        vector<tuple<string, string, double>> transactions;
        int i = 0, j = 0;
        while (i < (int)creditors.size() && j < (int)debtors.size()) {
            double settle = min(creditors[i].second, debtors[j].second);
            transactions.push_back({debtors[j].first, creditors[i].first, settle});
            creditors[i].second -= settle;
            debtors[j].second -= settle;
            if (creditors[i].second < 1e-9) i++;
            if (debtors[j].second < 1e-9) j++;
        }
        return transactions;
    }
};
```

---

## What interviewers look for

1. **Balance netting**: Can you correctly handle bidirectional debts? Many candidates store both A->B and B->A instead of netting.
2. **Strategy pattern**: Did you isolate the split logic behind an interface? Can you add PercentSplit without touching ExpenseManager?
3. **Debt simplification**: Can you articulate why net balances are sufficient? Can you implement the greedy matching correctly?
4. **Floating-point handling**: Do you use epsilon comparisons (1e-9) instead of exact equality for doubles?

---

## Common interview follow-ups

- *"How would you handle group expenses where the payer is not a participant?"* — The payer's share is zero; all participants are debtors to the payer.
- *"What if two users settle a debt outside the app?"* — Add a `recordPayment(from, to, amount)` that directly adjusts balances.
- *"How would you persist this?"* — Store expenses as an event log (append-only). Balances can be recomputed from the log or cached.
- *"How would you handle currencies?"* — Store amounts in cents (integer), add a currency field, and convert at settlement time.
