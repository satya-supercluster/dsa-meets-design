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
// HINT: Use unordered_map<string, unordered_map<string, double>> for balances.
// balances[A][B] > 0 means A owes B that amount.
// HINT: When updating balances, net out mutual debts. If B already owes A,
// reduce that before adding to A's debt to B.

// class ExpenseManager {
//     unordered_map<string, User> users;
//     vector<Expense> expenses;
//     unordered_map<string, unordered_map<string, double>> balances;
//
//     void updateBalance(const string& debtor, const string& creditor, double amount);
// public:
//     void addUser(const string& userId, const string& name);
//     void addExpense(const string& expenseId, const string& paidBy,
//                     double amount, const vector<string>& participants);
//     unordered_map<string, unordered_map<string, double>> getBalances() const;
// };

// ─── Test Entry Points (must exist for tests to compile) ────────────────────
// Your solution must provide these functions:
//
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   unordered_map<string, unordered_map<string, double>> get_balances();
//
// How you implement them internally is up to you.
// ─────────────────────────────────────────────────────────────────────────────


