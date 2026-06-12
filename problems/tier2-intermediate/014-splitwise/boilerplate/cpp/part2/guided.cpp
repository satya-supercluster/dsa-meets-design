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

// ─── Split Interface ───────────────────────────────────────────────────────
// HINT: Define an abstract base class with split() and validate() methods.
// Each concrete class implements its own split logic.

// class SplitStrategy {
// public:
//     virtual vector<Split> split(double totalAmount,
//                                 const vector<string>& participants,
//                                 const vector<double>& params) = 0;
//     virtual bool validate(double totalAmount,
//                           const vector<string>& participants,
//                           const vector<double>& params) = 0;
//     virtual ~SplitStrategy() = default;
// };

// HINT: EqualSplit — share = totalAmount / participants.size(), no params needed
// HINT: ExactSplit — params[i] is exact amount for participants[i], sum must equal total
// HINT: PercentSplit — params[i] is percentage for participants[i], sum must equal 100

// ─── Expense Manager ───────────────────────────────────────────────────────
// HINT: The manager calls strategy->validate(), then strategy->split(),
// then updates balances. It never knows which strategy is being used.

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
//     void addExpenseWithStrategy(const string& expenseId, const string& paidBy,
//                                 double amount, const vector<string>& participants,
//                                 SplitStrategy* strategy,
//                                 const vector<double>& params);
//     unordered_map<string, unordered_map<string, double>> getBalances() const;
// };

// ─── Test Entry Points (must exist for tests to compile) ────────────────────
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   void add_expense_with_strategy(const string& expenseId, const string& paidBy,
//                                  double amount, const vector<string>& participants,
//                                  SplitStrategy* strategy,
//                                  const vector<double>& params);
//   unordered_map<string, unordered_map<string, double>> get_balances();
// ─────────────────────────────────────────────────────────────────────────────


