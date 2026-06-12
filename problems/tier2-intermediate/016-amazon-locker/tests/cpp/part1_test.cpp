// Part 1 Tests — Amazon Locker System
// Tests core locker allocation, deposit, and retrieval

#include "solution.cpp"
#include <cassert>
#include <iostream>
using namespace std;

int part1_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: deposit into exact-size locker
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        addLocker("M1", LockerSize::MEDIUM);
        addLocker("L1", LockerSize::LARGE);
        string code = depositPackage("pkg1", LockerSize::SMALL);
        assert(!code.empty());
        cout << "PASS test_deposit_exact_size" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_deposit_exact_size" << endl;
        failed++;
    }

    // Test 2: retrieve with valid code
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        string code = depositPackage("pkg1", LockerSize::SMALL);
        assert(!code.empty());
        bool ok = retrievePackage(code);
        assert(ok == true);
        cout << "PASS test_retrieve_valid_code" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_retrieve_valid_code" << endl;
        failed++;
    }

    // Test 3: retrieve with invalid code fails
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        depositPackage("pkg1", LockerSize::SMALL);
        bool ok = retrievePackage("INVALID-CODE");
        assert(ok == false);
        cout << "PASS test_retrieve_invalid_code" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_retrieve_invalid_code" << endl;
        failed++;
    }

    // Test 4: smallest-fit allocation (small package uses small locker first)
    try {
        initLockerSystem();
        addLocker("M1", LockerSize::MEDIUM);
        addLocker("S1", LockerSize::SMALL);
        addLocker("L1", LockerSize::LARGE);
        // Small package should go to S1 (smallest fit)
        string code1 = depositPackage("pkg1", LockerSize::SMALL);
        assert(!code1.empty());
        // Another small package should go to M1 (next smallest available)
        string code2 = depositPackage("pkg2", LockerSize::SMALL);
        assert(!code2.empty());
        // Another small package should go to L1
        string code3 = depositPackage("pkg3", LockerSize::SMALL);
        assert(!code3.empty());
        // No more lockers — should fail
        string code4 = depositPackage("pkg4", LockerSize::SMALL);
        assert(code4.empty());
        cout << "PASS test_smallest_fit_allocation" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_smallest_fit_allocation" << endl;
        failed++;
    }

    // Test 5: large package cannot use small locker
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        addLocker("S2", LockerSize::SMALL);
        string code = depositPackage("bigpkg", LockerSize::LARGE);
        assert(code.empty()); // no large lockers available
        cout << "PASS test_large_package_no_small_locker" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_large_package_no_small_locker" << endl;
        failed++;
    }

    // Test 6: retrieve frees locker for reuse
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        string code1 = depositPackage("pkg1", LockerSize::SMALL);
        assert(!code1.empty());
        // Locker is full — next deposit fails
        string code2 = depositPackage("pkg2", LockerSize::SMALL);
        assert(code2.empty());
        // Retrieve pkg1 — locker freed
        assert(retrievePackage(code1) == true);
        // Now deposit should succeed again
        string code3 = depositPackage("pkg3", LockerSize::SMALL);
        assert(!code3.empty());
        cout << "PASS test_retrieve_frees_locker" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_retrieve_frees_locker" << endl;
        failed++;
    }

    // Test 7: same code cannot be used twice
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        string code = depositPackage("pkg1", LockerSize::SMALL);
        assert(retrievePackage(code) == true);
        assert(retrievePackage(code) == false); // already retrieved
        cout << "PASS test_code_single_use" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_code_single_use" << endl;
        failed++;
    }

    cout << "PART1_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
