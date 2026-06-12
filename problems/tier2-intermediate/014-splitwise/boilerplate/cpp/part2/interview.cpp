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

// ─── Your Design Starts Here ────────────────────────────────────────────────
//
// Extend the ExpenseManager to support multiple split strategies:
//   - EqualSplit: divide total equally among participants
//   - ExactSplit: each participant's share is specified explicitly
//     (amounts must sum to total)
//   - PercentSplit: each participant's share is a percentage
//     (percentages must sum to 100)
//
// Think about:
//   - How do you add a new split type without modifying ExpenseManager?
//   - Each strategy needs validation (e.g., exact amounts sum correctly)
//   - The manager should delegate split calculation to the strategy
//
// Entry points (must exist for tests):
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   void add_expense_with_strategy(const string& expenseId, const string& paidBy,
//                                  double amount, const vector<string>& participants,
//                                  SplitStrategy* strategy,
//                                  const vector<double>& params);
//   unordered_map<string, unordered_map<string, double>> get_balances();
//
// ─────────────────────────────────────────────────────────────────────────────


