// Part 3 Tests — Debt Simplification
// Tests greedy debt minimization algorithm

#include "solution.cpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <set>
using namespace std;

int part3_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: Simple two-person debt simplification
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addExpense("E1", "alice", 200.0, {"alice", "bob"});
        // Bob owes Alice 100
        auto txns = mgr.simplifyDebts();
        assert(txns.size() == 1);
        assert(get<0>(txns[0]) == "bob");
        assert(get<1>(txns[0]) == "alice");
        assert(fabs(get<2>(txns[0]) - 100.0) < 1e-9);
        cout << "PASS test_simple_two_person" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_simple_two_person" << endl;
        failed++;
    }

    // Test 2: Three-person chain simplification
    try {
        ExpenseManager mgr;
        mgr.addUser("A", "A");
        mgr.addUser("B", "B");
        mgr.addUser("C", "C");
        // A pays $300 for A,B,C => B owes A 100, C owes A 100
        mgr.addExpense("E1", "A", 300.0, {"A", "B", "C"});
        auto txns = mgr.simplifyDebts();
        // Net: A is owed 200, B owes 100, C owes 100
        // Simplified: B->A 100, C->A 100 (already minimal)
        assert(txns.size() == 2);
        // Verify total flow to A is 200
        double totalToA = 0;
        for (auto& t : txns) {
            if (get<1>(t) == "A") totalToA += get<2>(t);
        }
        assert(fabs(totalToA - 200.0) < 1e-9);
        cout << "PASS test_three_person_simplification" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_three_person_simplification" << endl;
        failed++;
    }

    // Test 3: Circular debts simplify to fewer transactions
    try {
        ExpenseManager mgr;
        mgr.addUser("A", "A");
        mgr.addUser("B", "B");
        mgr.addUser("C", "C");
        // A pays $300 for A,B,C => B owes A 100, C owes A 100
        mgr.addExpense("E1", "A", 300.0, {"A", "B", "C"});
        // B pays $300 for A,B,C => A owes B 100, C owes B 100
        mgr.addExpense("E2", "B", 300.0, {"A", "B", "C"});
        // Net: A owed 200, owes 100 => net +100
        //      B owed 200, owes 100 => net +100
        //      C owes 200 => net -200
        // Simplified: C pays A 100, C pays B 100
        auto txns = mgr.simplifyDebts();
        assert(txns.size() == 2);
        double totalFromC = 0;
        for (auto& t : txns) {
            if (get<0>(t) == "C") totalFromC += get<2>(t);
        }
        assert(fabs(totalFromC - 200.0) < 1e-9);
        cout << "PASS test_circular_debt_simplification" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_circular_debt_simplification" << endl;
        failed++;
    }

    // Test 4: No debts — returns empty
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        auto txns = mgr.simplifyDebts();
        assert(txns.empty());
        cout << "PASS test_no_debts_empty" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_debts_empty" << endl;
        failed++;
    }

    // Test 5: All debts cancel out — returns empty
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addExpense("E1", "alice", 200.0, {"alice", "bob"});
        mgr.addExpense("E2", "bob", 200.0, {"alice", "bob"});
        // Bob owes Alice 100, Alice owes Bob 100 => net zero
        auto txns = mgr.simplifyDebts();
        assert(txns.empty());
        cout << "PASS test_cancelling_debts" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_cancelling_debts" << endl;
        failed++;
    }

    // Test 6: Four-person simplification
    try {
        ExpenseManager mgr;
        mgr.addUser("A", "A");
        mgr.addUser("B", "B");
        mgr.addUser("C", "C");
        mgr.addUser("D", "D");
        // A pays 400 for all => B,C,D each owe A 100
        mgr.addExpense("E1", "A", 400.0, {"A", "B", "C", "D"});
        // Net: A=+300, B=-100, C=-100, D=-100
        auto txns = mgr.simplifyDebts();
        assert(txns.size() == 3);
        double totalToA = 0;
        for (auto& t : txns) {
            assert(get<1>(t) == "A");  // All payments go to A
            totalToA += get<2>(t);
        }
        assert(fabs(totalToA - 300.0) < 1e-9);
        cout << "PASS test_four_person_simplification" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_four_person_simplification" << endl;
        failed++;
    }

    // Test 7: Simplification preserves net amounts
    try {
        ExpenseManager mgr;
        mgr.addUser("A", "A");
        mgr.addUser("B", "B");
        mgr.addUser("C", "C");
        mgr.addExpense("E1", "A", 600.0, {"A", "B", "C"});
        mgr.addExpense("E2", "B", 300.0, {"A", "B", "C"});
        // After E1: B->A:200, C->A:200
        // After E2: A->B:100 (netted: B->A:100), C->B:100
        // Net: A=+300, B=-100+200=+100, hmm let me recalc
        // E1: A pays 600, split 200 each. B owes A 200, C owes A 200.
        // E2: B pays 300, split 100 each. A owes B 100, C owes B 100.
        // After netting: B owes A 100, C owes A 200, C owes B 100
        // Net: A=+300, B=+0(owes100 to A, owed 100 by C), C=-300
        // Actually: Net A = +200-100=+100+200 = let me use formula
        // A net = (200+200) - (100) = +300
        // B net = (100) - (200) = actually...
        // B net = (owed to B: 100 from C) - (B owes A: 100) = 0
        // C net = 0 - (200 to A + 100 to B) = -300
        // Simplified: C->A:300 (if B net is 0)
        // But wait, B might have some transactions. Let me check the greedy.
        // creditors: A(+300), debtors: C(300). B is zero.
        // One transaction: C->A 300
        auto txns = mgr.simplifyDebts();
        // B's net should be zero after netting
        double totalFromC = 0, totalToA = 0;
        for (auto& t : txns) {
            if (get<0>(t) == "C") totalFromC += get<2>(t);
            if (get<1>(t) == "A") totalToA += get<2>(t);
        }
        assert(fabs(totalFromC - 300.0) < 1e-9);
        assert(fabs(totalToA - 300.0) < 1e-9);
        cout << "PASS test_simplification_preserves_net" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_simplification_preserves_net" << endl;
        failed++;
    }

    cout << "PART3_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
