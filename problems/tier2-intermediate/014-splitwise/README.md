# Problem 014 — Splitwise Expense-Sharing System

**Tier:** 2 (Intermediate) | **Pattern:** Strategy + Factory + Observer | **DSA:** HashMap + Graph + Greedy
**Companies:** ShareChat, Razorpay, Flipkart, Paytm | **Time:** 45 minutes

---

## Problem Statement

You're building an expense-sharing system similar to Splitwise. Users create shared expenses (one person pays, multiple people split the cost), and the system tracks who owes whom. The system must support multiple split strategies and optimize debt settlement.

**Your task:** Design and implement an `ExpenseManager` that tracks users, creates expenses with configurable split strategies, computes net balances, and simplifies debts to minimize transactions.

---

## Before You Code

> Read this section carefully. This is where the design thinking happens.

**Ask yourself:**
1. When a user pays for a group expense, how do you represent who owes whom? A single expense creates multiple debts — what data structure captures this efficiently?
2. If the product manager asks you to add a new split type (e.g., split by shares, split by weight), should you modify the expense creation logic? Or should the split calculation be a separate, swappable component?
3. After many expenses, user A might owe B $10 and B might owe A $5. How do you simplify this to "A owes B $5"? What if there are circular debts among 5 users?

**The key insight:** The split calculation varies by type (equal, exact, percentage) — this is the **Strategy** pattern. Each strategy takes the total amount and participants, then returns how much each person owes. The core `ExpenseManager` delegates to the strategy without knowing the split logic. For debt simplification, **net out** all credits and debits per user, then greedily match the largest creditor with the largest debtor.

---

## Data Structures

```cpp
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
    vector<Split> splits;  // how the expense is divided
};
```

---

## Part 1

**Base requirement — Equal expense splitting and balance tracking**

Implement an `ExpenseManager` that manages users and expenses. When an expense is created, the payer pays the full amount and it is split equally among all participants (including the payer).

**Balance rule:** If Alice pays $300 for Alice, Bob, and Charlie, then Bob owes Alice $100 and Charlie owes Alice $100. Alice's share cancels out since she paid.

**Entry points (tests will call these):**
```cpp
void add_user(const string& userId, const string& name);
void add_expense(const string& expenseId, const string& paidBy,
                 double amount, const vector<string>& participants);
unordered_map<string, unordered_map<string, double>> get_balances();
// Returns balances[A][B] = amount A owes B (positive means A owes B)
```

**What to implement:**
```cpp
class ExpenseManager {
    unordered_map<string, User> users;
    vector<Expense> expenses;
    // balances[A][B] > 0 means A owes B that amount
    unordered_map<string, unordered_map<string, double>> balances;
public:
    void addUser(const string& userId, const string& name);
    void addExpense(const string& expenseId, const string& paidBy,
                    double amount, const vector<string>& participants);
    unordered_map<string, unordered_map<string, double>> getBalances() const;
};
```

**Design goal:** Balance tracking must correctly net out mutual debts. If A owes B $50 and B owes A $30, the result should show A owes B $20 (not both directions).

---

## Part 2

**Extension 1 — Multiple split strategies**

The product manager wants different ways to split expenses. Instead of always splitting equally, the system should support swappable strategies:

| Strategy | Rule |
|----------|------|
| EqualSplit | Divide total equally among all participants |
| ExactSplit | Each participant's share is specified explicitly (amounts must sum to total) |
| PercentSplit | Each participant's share is a percentage of the total (percentages must sum to 100) |

**Design challenge:** How do you add a new split strategy **without modifying** the ExpenseManager?

**New entry point:**
```cpp
void add_expense_with_strategy(const string& expenseId, const string& paidBy,
                               double amount, const vector<string>& participants,
                               SplitStrategy* strategy,
                               const vector<double>& strategyParams);
// strategyParams: for ExactSplit = exact amounts; for PercentSplit = percentages; for EqualSplit = empty
```

**What to implement:**
```cpp
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

class EqualSplit  : public SplitStrategy { ... };
class ExactSplit  : public SplitStrategy { ... };
class PercentSplit : public SplitStrategy { ... };
```

**Hint:** Each strategy validates its inputs (exact amounts sum to total, percentages sum to 100), then returns a vector of Splits.

---

## Part 3

**Extension 2 — Debt simplification**

After many expenses, the balance graph can have many edges. Simplify all debts to minimize the number of transactions needed to settle up.

**Algorithm:**
1. Compute net balance for each user (total owed to them minus total they owe).
2. Separate users into creditors (net positive) and debtors (net negative).
3. Greedily match the largest creditor with the largest debtor. The transaction amount is the minimum of the two absolute values.
4. Repeat until all debts are settled.

**Example:** If net balances are A=+50, B=-30, C=-20, then: B pays A $30, C pays A $20. Two transactions instead of potentially many more.

**New entry point:**
```cpp
vector<tuple<string, string, double>> simplify_debts();
// Returns list of (from, to, amount) — minimized transactions
```

**Design challenge:** The greedy approach works because we only care about net positions, not the original expense graph. Think of it as netting out a directed graph.

---

## Running Tests

```bash
./run-tests.sh 014-splitwise cpp
```
