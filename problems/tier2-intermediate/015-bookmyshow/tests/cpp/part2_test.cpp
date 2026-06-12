// Part 2 Tests — Seat Locking with TTL
// Tests lock creation, confirmation, expiration, and release

#include "solution.cpp"
#include <cassert>
#include <iostream>
using namespace std;

int part2_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: Lock seats successfully
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}, {0,1}}, "user1", 5, now);
        assert(!lockId.empty());
        // Locked seats should not be available
        auto seats = sys.getAvailableSeats("S1", now);
        assert(seats.size() == 48);  // 50 - 2 locked
        cout << "PASS test_lock_seats_success" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_lock_seats_success" << endl;
        failed++;
    }

    // Test 2: Cannot lock already locked seats
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        sys.lockSeats("S1", {{0,0}, {0,1}}, "user1", 5, now);
        // Another user tries to lock same seats
        string lockId2 = sys.lockSeats("S1", {{0,0}}, "user2", 5, now);
        assert(lockId2.empty());
        cout << "PASS test_cannot_lock_locked_seats" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_cannot_lock_locked_seats" << endl;
        failed++;
    }

    // Test 3: Confirm booking within TTL
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}, {0,1}}, "user1", 5, now);
        // Confirm within TTL (5 min = 300 sec)
        bool ok = sys.confirmBooking(lockId, now + 200);
        assert(ok == true);
        // Seats should now be BOOKED, not available
        auto seats = sys.getAvailableSeats("S1", now + 200);
        assert(seats.size() == 48);
        cout << "PASS test_confirm_within_ttl" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_confirm_within_ttl" << endl;
        failed++;
    }

    // Test 4: Confirm booking AFTER TTL expires — fails
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}, {0,1}}, "user1", 5, now);
        // Try to confirm after TTL (5 min = 300 sec, try at +400)
        bool ok = sys.confirmBooking(lockId, now + 400);
        assert(ok == false);
        // Seats should be available again (expired)
        auto seats = sys.getAvailableSeats("S1", now + 400);
        assert(seats.size() == 50);  // All seats free
        cout << "PASS test_confirm_after_ttl_expires" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_confirm_after_ttl_expires" << endl;
        failed++;
    }

    // Test 5: Expired lock releases seats for other users
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        sys.lockSeats("S1", {{0,0}}, "user1", 5, now);
        // After TTL, another user can lock the same seat
        long afterExpiry = now + 400;
        string lockId2 = sys.lockSeats("S1", {{0,0}}, "user2", 5, afterExpiry);
        assert(!lockId2.empty());
        cout << "PASS test_expired_lock_releases_seats" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_expired_lock_releases_seats" << endl;
        failed++;
    }

    // Test 6: Manual lock release
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}, {0,1}}, "user1", 5, now);
        bool released = sys.releaseLock(lockId, now + 60);
        assert(released == true);
        // Seats should be available again
        auto seats = sys.getAvailableSeats("S1", now + 60);
        assert(seats.size() == 50);
        cout << "PASS test_manual_release" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_manual_release" << endl;
        failed++;
    }

    // Test 7: Cannot confirm after manual release
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}}, "user1", 5, now);
        sys.releaseLock(lockId, now + 60);
        bool ok = sys.confirmBooking(lockId, now + 120);
        assert(ok == false);
        cout << "PASS test_no_confirm_after_release" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_confirm_after_release" << endl;
        failed++;
    }

    // Test 8: Cannot double-confirm a lock
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        string lockId = sys.lockSeats("S1", {{0,0}}, "user1", 5, now);
        bool ok1 = sys.confirmBooking(lockId, now + 100);
        assert(ok1 == true);
        bool ok2 = sys.confirmBooking(lockId, now + 150);
        assert(ok2 == false);
        cout << "PASS test_no_double_confirm" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_double_confirm" << endl;
        failed++;
    }

    // Test 9: Cannot book already locked seats (not expired)
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        long now = 1000;
        sys.lockSeats("S1", {{0,0}}, "user1", 5, now);
        // Try direct booking of locked seat
        bool ok = sys.bookSeats("B1", "S1", {{0,0}}, "user2", now + 60);
        assert(ok == false);
        cout << "PASS test_cannot_book_locked_seat" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_cannot_book_locked_seat" << endl;
        failed++;
    }

    // Test 10: Release nonexistent lock fails
    try {
        BookingSystem sys;
        bool ok = sys.releaseLock("NONEXISTENT", 1000);
        assert(ok == false);
        cout << "PASS test_release_nonexistent_lock" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_release_nonexistent_lock" << endl;
        failed++;
    }

    cout << "PART2_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
