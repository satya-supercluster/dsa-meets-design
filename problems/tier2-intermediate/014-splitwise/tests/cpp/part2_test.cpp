// Part 2 Tests — Multiple Split Strategies
// Tests EqualSplit, ExactSplit, PercentSplit, and validation

#include "solution.cpp"
#include <cassert>
#include <iostream>
#include <cmath>
using namespace std;

int part2_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: EqualSplit via strategy interface
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addUser("charlie", "Charlie");
        EqualSplit eq;
        mgr.addExpenseWithStrategy("E1", "alice", 300.0,
            {"alice", "bob", "charlie"}, &eq, {});
        auto bal = mgr.getBalances();
        assert(fabs(bal["bob"]["alice"] - 100.0) < 1e-9);
        assert(fabs(bal["charlie"]["alice"] - 100.0) < 1e-9);
        cout << "PASS test_equal_split_strategy" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_equal_split_strategy" << endl;
        failed++;
    }

    // Test 2: ExactSplit — specified amounts per person
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addUser("charlie", "Charlie");
        ExactSplit exact;
        mgr.addExpenseWithStrategy("E1", "alice", 300.0,
            {"alice", "bob", "charlie"}, &exact, {100.0, 150.0, 50.0});
        auto bal = mgr.getBalances();
        // Bob owes Alice 150, Charlie owes Alice 50
        assert(fabs(bal["bob"]["alice"] - 150.0) < 1e-9);
        assert(fabs(bal["charlie"]["alice"] - 50.0) < 1e-9);
        cout << "PASS test_exact_split_strategy" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_exact_split_strategy" << endl;
        failed++;
    }

    // Test 3: PercentSplit — percentage-based amounts
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        mgr.addUser("charlie", "Charlie");
        PercentSplit pct;
        mgr.addExpenseWithStrategy("E1", "alice", 1000.0,
            {"alice", "bob", "charlie"}, &pct, {50.0, 30.0, 20.0});
        auto bal = mgr.getBalances();
        // Alice's share: 500 (cancels), Bob: 300, Charlie: 200
        assert(fabs(bal["bob"]["alice"] - 300.0) < 1e-9);
        assert(fabs(bal["charlie"]["alice"] - 200.0) < 1e-9);
        cout << "PASS test_percent_split_strategy" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_percent_split_strategy" << endl;
        failed++;
    }

    // Test 4: ExactSplit validation — amounts don't sum to total
    try {
        ExactSplit exact;
        bool valid = exact.validate(300.0, {"a", "b", "c"}, {100.0, 100.0, 50.0});
        assert(valid == false);  // 250 != 300
        cout << "PASS test_exact_split_invalid_sum" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_exact_split_invalid_sum" << endl;
        failed++;
    }

    // Test 5: PercentSplit validation — percentages don't sum to 100
    try {
        PercentSplit pct;
        bool valid = pct.validate(1000.0, {"a", "b"}, {60.0, 60.0});
        assert(valid == false);  // 120 != 100
        cout << "PASS test_percent_split_invalid_sum" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_percent_split_invalid_sum" << endl;
        failed++;
    }

    // Test 6: ExactSplit validation — wrong number of params
    try {
        ExactSplit exact;
        bool valid = exact.validate(300.0, {"a", "b", "c"}, {150.0, 150.0});
        assert(valid == false);  // 2 params for 3 participants
        cout << "PASS test_exact_split_wrong_param_count" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_exact_split_wrong_param_count" << endl;
        failed++;
    }

    // Test 7: Mixed strategies — equal then exact on same users
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        EqualSplit eq;
        ExactSplit exact;
        mgr.addExpenseWithStrategy("E1", "alice", 200.0,
            {"alice", "bob"}, &eq, {});
        // Bob owes Alice 100
        mgr.addExpenseWithStrategy("E2", "bob", 150.0,
            {"alice", "bob"}, &exact, {90.0, 60.0});
        // Alice owes Bob 90, but Bob owes Alice 100 => net Bob owes Alice 10
        auto bal = mgr.getBalances();
        assert(fabs(bal["bob"]["alice"] - 10.0) < 1e-9);
        cout << "PASS test_mixed_strategies" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_mixed_strategies" << endl;
        failed++;
    }

    // Test 8: Invalid strategy does not create expense
    try {
        ExpenseManager mgr;
        mgr.addUser("alice", "Alice");
        mgr.addUser("bob", "Bob");
        ExactSplit exact;
        mgr.addExpenseWithStrategy("E1", "alice", 100.0,
            {"alice", "bob"}, &exact, {60.0, 60.0});  // sum=120, invalid
        auto bal = mgr.getBalances();
        bool empty = true;
        for (auto& [k, v] : bal) {
            for (auto& [k2, amt] : v) {
                if (amt > 1e-9) empty = false;
            }
        }
        assert(empty);
        cout << "PASS test_invalid_strategy_no_expense" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_invalid_strategy_no_expense" << endl;
        failed++;
    }

    cout << "PART2_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
