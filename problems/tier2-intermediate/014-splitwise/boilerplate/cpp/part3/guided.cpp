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

// ─── Split Interface (from Part 2) ─────────────────────────────────────────

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

// ─── Debt Simplification ───────────────────────────────────────────────────
// HINT: Compute net balance per user: net[user] = (owed to user) - (user owes)
// HINT: Positive net = creditor. Negative net = debtor.
// HINT: Sort creditors descending, debtors descending by absolute value.
// HINT: Use two pointers — match largest creditor with largest debtor.
//       Transaction = min(credit, |debt|). Advance pointer when one reaches zero.

// vector<tuple<string, string, double>> simplifyDebts() {
//     // Step 1: compute net from balances map
//     // Step 2: separate into creditors and debtors
//     // Step 3: sort both descending
//     // Step 4: two-pointer greedy matching
// }

// ─── Test Entry Points (must exist for tests to compile) ────────────────────
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   void add_expense_with_strategy(const string& expenseId, const string& paidBy,
//                                  double amount, const vector<string>& participants,
//                                  SplitStrategy* strategy,
//                                  const vector<double>& params);
//   unordered_map<string, unordered_map<string, double>> get_balances();
//   vector<tuple<string, string, double>> simplify_debts();
// ─────────────────────────────────────────────────────────────────────────────


