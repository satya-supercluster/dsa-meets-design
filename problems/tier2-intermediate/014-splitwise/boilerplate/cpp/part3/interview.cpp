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

// ─── Your Design Starts Here ────────────────────────────────────────────────
//
// Extend the system with debt simplification:
//   Given all current balances, compute the minimum set of transactions
//   needed to settle all debts.
//
// Think about:
//   - Individual expense history doesn't matter — only each user's NET
//     position matters (total owed to them minus total they owe)
//   - Separate users into creditors (net positive) and debtors (net negative)
//   - Greedily match largest creditor with largest debtor
//   - Transaction amount = min(credit, |debt|)
//
// Entry points (must exist for tests — includes all prior parts):
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   void add_expense_with_strategy(const string& expenseId, const string& paidBy,
//                                  double amount, const vector<string>& participants,
//                                  SplitStrategy* strategy,
//                                  const vector<double>& params);
//   unordered_map<string, unordered_map<string, double>> get_balances();
//   vector<tuple<string, string, double>> simplify_debts();
//     // Returns list of (from, to, amount) — minimized transactions
//
// ─────────────────────────────────────────────────────────────────────────────


