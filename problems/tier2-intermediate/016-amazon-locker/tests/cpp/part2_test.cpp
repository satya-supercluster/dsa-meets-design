// Part 2 Tests — Code Expiry + Notifications
// Tests pickup code expiry and notification channels

#include <cassert>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ─── Notification spy for testing ────────────────────────────────────────────

static vector<string> notification_log;

class TestNotificationChannel : public NotificationChannel {
public:
    void notify(const string& packageId, const string& message) override {
        notification_log.push_back(packageId + ": " + message);
    }
};

int part2_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: deposit with notification
    try {
        initLockerSystem();
        notification_log.clear();
        TestNotificationChannel channel;
        addNotificationChannel(&channel);
        addLocker("S1", LockerSize::SMALL);
        string code = depositPackage("pkg1", LockerSize::SMALL, 1000);
        assert(!code.empty());
        assert(notification_log.size() >= 1); // should have been notified
        cout << "PASS test_deposit_notification" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_deposit_notification" << endl;
        failed++;
    }

    // Test 2: code expiry frees locker
    try {
        initLockerSystem();
        notification_log.clear();
        addLocker("S1", LockerSize::SMALL);
        setCodeExpiry(2); // 2 hours = 7200 seconds
        string code = depositPackage("pkg1", LockerSize::SMALL, 1000);
        assert(!code.empty());
        // Check at 1000 + 7201 = 8201 (past expiry)
        vector<string> expired = checkExpired(8201);
        assert(expired.size() == 1);
        assert(expired[0] == "pkg1");
        cout << "PASS test_code_expiry" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_code_expiry" << endl;
        failed++;
    }

    // Test 3: non-expired code still works
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        setCodeExpiry(2);
        string code = depositPackage("pkg1", LockerSize::SMALL, 1000);
        // Check at 1000 + 3600 = 4600 (before expiry)
        vector<string> expired = checkExpired(4600);
        assert(expired.empty());
        // Code should still work
        assert(retrievePackage(code) == true);
        cout << "PASS test_non_expired_code" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_non_expired_code" << endl;
        failed++;
    }

    // Test 4: expired locker becomes available again
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        setCodeExpiry(1); // 1 hour = 3600 seconds
        depositPackage("pkg1", LockerSize::SMALL, 1000);
        // Can't deposit — locker is full
        string code2 = depositPackage("pkg2", LockerSize::SMALL, 1500);
        assert(code2.empty());
        // Expire the first package
        checkExpired(5000); // 1000 + 3600 < 5000
        // Now locker should be free
        string code3 = depositPackage("pkg3", LockerSize::SMALL, 5001);
        assert(!code3.empty());
        cout << "PASS test_expired_locker_reuse" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_expired_locker_reuse" << endl;
        failed++;
    }

    // Test 5: expiry triggers notification
    try {
        initLockerSystem();
        notification_log.clear();
        TestNotificationChannel channel;
        addNotificationChannel(&channel);
        addLocker("S1", LockerSize::SMALL);
        setCodeExpiry(1);
        depositPackage("pkg1", LockerSize::SMALL, 1000);
        size_t before = notification_log.size();
        checkExpired(5000);
        assert(notification_log.size() > before); // expiry notification sent
        cout << "PASS test_expiry_notification" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_expiry_notification" << endl;
        failed++;
    }

    // Test 6: multiple deposits, only expired ones freed
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        addLocker("S2", LockerSize::SMALL);
        setCodeExpiry(2);
        string code1 = depositPackage("pkg1", LockerSize::SMALL, 1000);
        string code2 = depositPackage("pkg2", LockerSize::SMALL, 5000);
        assert(!code1.empty());
        assert(!code2.empty());
        // At time 8201: pkg1 expired (1000+7200<8201), pkg2 not (5000+7200>8201)
        vector<string> expired = checkExpired(8201);
        assert(expired.size() == 1);
        assert(expired[0] == "pkg1");
        // pkg2's code should still work
        assert(retrievePackage(code2) == true);
        // pkg1's code should NOT work (already expired)
        assert(retrievePackage(code1) == false);
        cout << "PASS test_selective_expiry" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_selective_expiry" << endl;
        failed++;
    }

    // Test 7: no expiry set means codes never expire
    try {
        initLockerSystem();
        addLocker("S1", LockerSize::SMALL);
        // Don't call setCodeExpiry — default is no expiry
        string code = depositPackage("pkg1", LockerSize::SMALL, 1000);
        vector<string> expired = checkExpired(999999999);
        assert(expired.empty());
        assert(retrievePackage(code) == true);
        cout << "PASS test_no_expiry_default" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_expiry_default" << endl;
        failed++;
    }

    cout << "PART2_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
