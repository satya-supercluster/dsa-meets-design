#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <cmath>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

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
        // TODO: divide equally
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: validate non-empty participants
        return false;
    }
};

class ExactSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        // TODO: use exact amounts from params
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: validate sizes match and sum equals total
        return false;
    }
};

class PercentSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        // TODO: compute percentage-based amounts
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: validate sizes match and percentages sum to 100
        return false;
    }
};

// ─── Expense Manager ───────────────────────────────────────────────────────

class ExpenseManager {
    unordered_map<string, User> users;
    vector<Expense> expenses;
    unordered_map<string, unordered_map<string, double>> balances;

    void updateBalance(const string& debtor, const string& creditor, double amount) {
        if (debtor == creditor) return;
        // TODO: Net out mutual debts, then add remaining
    }

public:
    void addUser(const string& userId, const string& name) {
        users[userId] = {userId, name};
    }

    void addExpense(const string& expenseId, const string& paidBy,
                    double amount, const vector<string>& participants) {
        // TODO: Use EqualSplit, compute splits, update balances
    }

    void addExpenseWithStrategy(const string& expenseId, const string& paidBy,
                                double amount, const vector<string>& participants,
                                SplitStrategy* strategy,
                                const vector<double>& params) {
        // TODO: Validate, compute splits, update balances
    }

    unordered_map<string, unordered_map<string, double>> getBalances() const {
        return balances;
    }

    vector<tuple<string, string, double>> simplifyDebts() {
        // TODO: Step 1 — Compute net balance per user from balances map
        //   net[user] = sum of (amounts owed TO user) - sum of (amounts user OWES)

        // TODO: Step 2 — Separate into creditors (net > 0) and debtors (net < 0)

        // TODO: Step 3 — Sort both vectors descending by amount

        // TODO: Step 4 — Two-pointer greedy matching
        //   settle = min(creditors[i].second, debtors[j].second)
        //   Add transaction (debtor, creditor, settle)
        //   Advance pointer when amount reaches zero

        return {};
    }
};

// ─── Test Entry Points ─────────────────────────────────────────────────────

ExpenseManager manager;

void add_user(const string& userId, const string& name) {
    manager.addUser(userId, name);
}

void add_expense(const string& expenseId, const string& paidBy,
                 double amount, const vector<string>& participants) {
    manager.addExpense(expenseId, paidBy, amount, participants);
}

void add_expense_with_strategy(const string& expenseId, const string& paidBy,
                               double amount, const vector<string>& participants,
                               SplitStrategy* strategy,
                               const vector<double>& params) {
    manager.addExpenseWithStrategy(expenseId, paidBy, amount, participants,
                                  strategy, params);
}

unordered_map<string, unordered_map<string, double>> get_balances() {
    return manager.getBalances();
}

vector<tuple<string, string, double>> simplify_debts() {
    return manager.simplifyDebts();
}

#ifndef RUNNING_TESTS
int main() {
    cout << "Splitwise Part 3 — implement the TODO methods above, then run tests." << endl;
    return 0;
}
#endif
