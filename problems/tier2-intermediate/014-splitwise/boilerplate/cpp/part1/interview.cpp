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
    double amount;  // how much this user owes from a single expense
};

struct Expense {
    string id;
    string paidBy;      // userId of the person who paid
    double totalAmount;
    vector<Split> splits;
};

// ─── Your Design Starts Here ────────────────────────────────────────────────
//
// Design and implement an ExpenseManager that:
//   1. Manages users
//   2. Creates expenses where one user pays and the cost is split equally
//      among all participants (including the payer)
//   3. Tracks balances — who owes whom and how much
//
// Think about:
//   - If Alice pays $300 split among Alice, Bob, Charlie, each share is $100.
//     Bob owes Alice $100, Charlie owes Alice $100. Alice's share cancels out.
//   - If A owes B $50 and later B owes A $30, the net should be A owes B $20.
//   - What data structure represents the balance graph efficiently?
//   - How will this extend to support different split strategies later?
//
// Entry points (must exist for tests):
//   void add_user(const string& userId, const string& name);
//   void add_expense(const string& expenseId, const string& paidBy,
//                    double amount, const vector<string>& participants);
//   unordered_map<string, unordered_map<string, double>> get_balances();
//
// ─────────────────────────────────────────────────────────────────────────────


