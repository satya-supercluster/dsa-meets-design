// Part 1 Tests — Equal Expense Splitting and Balance Tracking
// Tests user management, equal splitting, and balance netting

#include "solution.cpp"
#include <cassert>
#include <iostream>
#include <cmath>
using namespace std;

int part1_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: Simple equal split — Alice pays $300 for Alice, Bob, Charlie
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addUser("charlie", "Charlie");
        mgr.addExpense("E1", "alice", 300.0, {"alice", "bob", "charlie"});
        auto bal = mgr.getBalances();
        assert(fabs(bal["bob"]["alice"] - 100.0) < 1e-9);
        assert(fabs(bal["charlie"]["alice"] - 100.0) < 1e-9);
        // Alice should not owe herself
        assert(bal["alice"].find("alice") == bal["alice"].end() ||
               bal["alice"]["alice"] < 1e-9);
        cout << "PASS test_simple_equal_split" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_simple_equal_split" << endl;
        failed++;
    }

    // Test 2: Two expenses — balances accumulate
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addExpense("E1", "alice", 200.0, {"alice", "bob"});
        mgr.addExpense("E2", "alice", 100.0, {"alice", "bob"});
        auto bal = mgr.getBalances();
        // Bob owes Alice: 100 + 50 = 150
        assert(fabs(bal["bob"]["alice"] - 150.0) < 1e-9);
        cout << "PASS test_accumulated_balances" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_accumulated_balances" << endl;
        failed++;
    }

    // Test 3: Mutual debts net out — Alice pays for both, then Bob pays for both
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addExpense("E1", "alice", 200.0, {"alice", "bob"});
        // Bob owes Alice $100
        mgr.addExpense("E2", "bob", 60.0, {"alice", "bob"});
        // Alice owes Bob $30, but Bob already owes Alice $100
        // Net: Bob owes Alice $70
        auto bal = mgr.getBalances();
        assert(fabs(bal["bob"]["alice"] - 70.0) < 1e-9);
        // Alice should NOT owe Bob anything
        assert(bal["alice"].find("bob") == bal["alice"].end() ||
               bal["alice"]["bob"] < 1e-9);
        cout << "PASS test_mutual_debt_netting" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_mutual_debt_netting" << endl;
        failed++;
    }

    // Test 4: Payer not in participants — all participants owe payer
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addUser("charlie", "Charlie");
        mgr.addExpense("E1", "alice", 200.0, {"bob", "charlie"});
        auto bal = mgr.getBalances();
        assert(fabs(bal["bob"]["alice"] - 100.0) < 1e-9);
        assert(fabs(bal["charlie"]["alice"] - 100.0) < 1e-9);
        cout << "PASS test_payer_not_in_participants" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_payer_not_in_participants" << endl;
        failed++;
    }

    // Test 5: Single participant is the payer — no debts created
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addExpense("E1", "alice", 100.0, {"alice"});
        auto bal = mgr.getBalances();
        bool noDebts = true;
        for (auto& [debtor, creditors] : bal) {
            for (auto& [creditor, amount] : creditors) {
                if (amount > 1e-9) noDebts = false;
            }
        }
        assert(noDebts);
        cout << "PASS test_self_expense_no_debt" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_self_expense_no_debt" << endl;
        failed++;
    }

    // Test 6: Multiple expenses among multiple users
    try {
        ExpenseManager mgr;
        mgr.addUser("A", "A");
        mgr.addUser("B", "B");
        mgr.addUser("C", "C");
        mgr.addUser("D", "D");
        mgr.addExpense("E1", "A", 400.0, {"A", "B", "C", "D"});
        // Each owes A: $100. B->A:100, C->A:100, D->A:100
        mgr.addExpense("E2", "B", 200.0, {"A", "B", "C", "D"});
        // Each owes B: $50. A->B:50, C->B:50, D->B:50
        // But B owes A 100, and A owes B 50 => net: B owes A 50
        auto bal = mgr.getBalances();
        assert(fabs(bal["B"]["A"] - 50.0) < 1e-9);
        assert(fabs(bal["C"]["A"] - 100.0) < 1e-9);
        assert(fabs(bal["C"]["B"] - 50.0) < 1e-9);
        assert(fabs(bal["D"]["A"] - 100.0) < 1e-9);
        assert(fabs(bal["D"]["B"] - 50.0) < 1e-9);
        cout << "PASS test_multi_user_multi_expense" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_multi_user_multi_expense" << endl;
        failed++;
    }

    // Test 7: Empty balances when no expenses
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        auto bal = mgr.getBalances();
        bool empty = true;
        for (auto& [k, v] : bal) {
            for (auto& [k2, amt] : v) {
                if (amt > 1e-9) empty = false;
            }
        }
        assert(empty);
        cout << "PASS test_empty_balances" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_empty_balances" << endl;
        failed++;
    }

    cout << "PART1_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
