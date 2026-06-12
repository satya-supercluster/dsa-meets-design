// Part 1 Tests — Theater, Show, and Booking Management
// Tests theater/show creation, movie search, seat availability, booking, double-booking

#include "solution.cpp"
#include <cassert>
#include <iostream>
#include <set>
using namespace std;

int part1_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: Add theater and show, search movies by city
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR Phoenix", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        auto movies = sys.searchMovies("Mumbai");
        assert(movies.size() == 1);
        assert(movies[0] == "Inception");
        cout << "PASS test_search_movies_by_city" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_search_movies_by_city" << endl;
        failed++;
    }

    // Test 2: Multiple movies in same city
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR Phoenix", "Mumbai");
        sys.addTheater("T2", "INOX", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        sys.addShow("S2", "T2", "Interstellar", "20:00", 5, 10);
        auto movies = sys.searchMovies("Mumbai");
        assert(movies.size() == 2);
        // set<string> ensures sorted order
        set<string> movieSet(movies.begin(), movies.end());
        assert(movieSet.count("Inception"));
        assert(movieSet.count("Interstellar"));
        cout << "PASS test_multiple_movies_same_city" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_multiple_movies_same_city" << endl;
        failed++;
    }

    // Test 3: Search movies in city with no theaters
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        auto movies = sys.searchMovies("Delhi");
        assert(movies.empty());
        cout << "PASS test_no_movies_in_city" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_movies_in_city" << endl;
        failed++;
    }

    // Test 4: Get available seats — all seats free initially
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 3, 4);
        auto seats = sys.getAvailableSeats("S1");
        assert(seats.size() == 12);  // 3 rows x 4 cols
        cout << "PASS test_all_seats_available" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_all_seats_available" << endl;
        failed++;
    }

    // Test 5: Book seats successfully
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        bool ok = sys.bookSeats("B1", "S1", {{0,0}, {0,1}, {0,2}}, "user1");
        assert(ok == true);
        auto seats = sys.getAvailableSeats("S1");
        assert(seats.size() == 47);  // 50 - 3 booked
        cout << "PASS test_book_seats_success" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_book_seats_success" << endl;
        failed++;
    }

    // Test 6: Double-booking prevention — same seats
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        sys.bookSeats("B1", "S1", {{0,0}, {0,1}}, "user1");
        // Try to book same seat (0,0)
        bool ok = sys.bookSeats("B2", "S1", {{0,0}, {0,2}}, "user2");
        assert(ok == false);
        cout << "PASS test_double_booking_prevention" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_double_booking_prevention" << endl;
        failed++;
    }

    // Test 7: Atomic booking — partial overlap fails entirely
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 5, 10);
        sys.bookSeats("B1", "S1", {{0,0}}, "user1");
        // Try to book {0,0} (taken) and {0,1} (free) — should fail entirely
        bool ok = sys.bookSeats("B2", "S1", {{0,0}, {0,1}}, "user2");
        assert(ok == false);
        // Verify {0,1} is still available (atomic — nothing was booked)
        auto seats = sys.getAvailableSeats("S1");
        bool found01 = false;
        for (auto& [r, c] : seats) {
            if (r == 0 && c == 1) found01 = true;
        }
        assert(found01);
        cout << "PASS test_atomic_booking" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_atomic_booking" << endl;
        failed++;
    }

    // Test 8: Invalid seat coordinates
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "18:00", 3, 4);
        // Row 5 doesn't exist in a 3-row theater
        bool ok = sys.bookSeats("B1", "S1", {{5, 0}}, "user1");
        assert(ok == false);
        cout << "PASS test_invalid_seat_coordinates" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_invalid_seat_coordinates" << endl;
        failed++;
    }

    // Test 9: Booking on nonexistent show fails
    try {
        BookingSystem sys;
        bool ok = sys.bookSeats("B1", "NONEXISTENT", {{0,0}}, "user1");
        assert(ok == false);
        cout << "PASS test_nonexistent_show" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_nonexistent_show" << endl;
        failed++;
    }

    // Test 10: Duplicate movie in same city appears only once
    try {
        BookingSystem sys;
        sys.addTheater("T1", "PVR Phoenix", "Mumbai");
        sys.addTheater("T2", "INOX", "Mumbai");
        sys.addShow("S1", "T1", "Inception", "15:00", 5, 10);
        sys.addShow("S2", "T2", "Inception", "18:00", 5, 10);
        auto movies = sys.searchMovies("Mumbai");
        assert(movies.size() == 1);  // "Inception" only once
        cout << "PASS test_duplicate_movie_deduplicated" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_duplicate_movie_deduplicated" << endl;
        failed++;
    }

    cout << "PART1_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
