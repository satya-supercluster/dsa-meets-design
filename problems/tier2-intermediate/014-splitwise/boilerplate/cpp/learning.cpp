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

// ─── Expense Manager ───────────────────────────────────────────────────────

class ExpenseManager {
    unordered_map<string, User> users;
    vector<Expense> expenses;
    unordered_map<string, unordered_map<string, double>> balances;

    void updateBalance(const string& debtor, const string& creditor, double amount) {
        if (debtor == creditor) return;
        // TODO: If creditor already owes debtor, reduce that amount first (netting)
        // HINT: check balances[creditor][debtor], offset = min(existing, amount)
        // TODO: If remaining amount > 0, add to balances[debtor][creditor]
    }

public:
    void addUser(const string& userId, const string& name) {
        users[userId] = {userId, name};
    }

    void addExpense(const string& expenseId, const string& paidBy,
                    double amount, const vector<string>& participants) {
        // TODO: Calculate equal share = amount / participants.size()
        // TODO: Create Split for each participant
        // TODO: Store the Expense
        // TODO: For each split, call updateBalance(split.userId, paidBy, split.amount)
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

unordered_map<string, unordered_map<string, double>> get_balances() {
    return manager.getBalances();
}

#ifndef RUNNING_TESTS
int main() {
    cout << "Splitwise — implement the TODO methods above, then run tests." << endl;
    return 0;
}
#endif
