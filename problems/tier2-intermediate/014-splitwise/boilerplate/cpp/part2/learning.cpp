#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
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

// ─── Concrete Strategies ───────────────────────────────────────────────────

class EqualSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        // TODO: divide totalAmount equally among participants
        // Return a vector of Split with each participant's share
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: validate that participants is not empty
        return false;
    }
};

class ExactSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        // TODO: params[i] is the exact amount for participants[i]
        // Return a vector of Split with specified amounts
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: check params.size() == participants.size()
        // TODO: check sum of params == totalAmount (use fabs < 1e-9)
        return false;
    }
};

class PercentSplit : public SplitStrategy {
public:
    vector<Split> split(double totalAmount,
                        const vector<string>& participants,
                        const vector<double>& params) override {
        // TODO: params[i] is the percentage for participants[i]
        // amount = totalAmount * params[i] / 100.0
        return {};
    }
    bool validate(double totalAmount, const vector<string>& participants,
                  const vector<double>& params) override {
        // TODO: check params.size() == participants.size()
        // TODO: check sum of params == 100.0 (use fabs < 1e-9)
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
        // TODO: Net out mutual debts before adding new debt
        // HINT: if balances[creditor][debtor] > 0, offset that first
        // Then add remaining to balances[debtor][creditor]
    }

public:
    void addUser(const string& userId, const string& name) {
        users[userId] = {userId, name};
    }

    void addExpense(const string& expenseId, const string& paidBy,
                    double amount, const vector<string>& participants) {
        // TODO: Use EqualSplit strategy to compute splits
        // TODO: Store expense and update balances
    }

    void addExpenseWithStrategy(const string& expenseId, const string& paidBy,
                                double amount, const vector<string>& participants,
                                SplitStrategy* strategy,
                                const vector<double>& params) {
        // TODO: Validate using strategy->validate()
        // TODO: Compute splits using strategy->split()
        // TODO: Store expense and update balances
    }

    unordered_map<string, unordered_map<string, double>> getBalances() const {
        return balances;
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

#ifndef RUNNING_TESTS
int main() {
    cout << "Splitwise Part 2 — implement the TODO methods above, then run tests." << endl;
    return 0;
}
#endif
