#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

enum class SeatStatus { AVAILABLE, LOCKED, BOOKED };

struct Seat {
    int row;
    int col;
    SeatStatus status;
    string lockedBy;
    long lockExpiry;
    string bookedBy;
};

struct Show {
    string id;
    string theaterId;
    string movie;
    string time;
    int rows;
    int cols;
    vector<vector<Seat>> seats;
};

struct Theater {
    string id;
    string name;
    string city;
};

struct Booking {
    string id;
    string showId;
    string userId;
    vector<pair<int,int>> seatPositions;
};

// ─── Your Design Starts Here ────────────────────────────────────────────────
//
// Design and implement a BookingSystem that:
//   1. Manages theaters (id, name, city) and shows (movie, time, seat grid)
//   2. Allows searching movies by city
//   3. Shows available seats for a given show (2D grid)
//   4. Books seats atomically — all-or-nothing (no partial bookings)
//   5. Prevents double-booking
//
// Think about:
//   - How do you represent a screen's seating? A 2D vector of Seat objects.
//   - Booking must be atomic: check ALL seats are free, then book ALL.
//     If any seat is taken, the entire request fails.
//   - How do you index movies by city for efficient search?
//   - How will this extend to support seat locking with TTL later?
//
// Entry points (must exist for tests):
//   void add_theater(const string& theaterId, const string& name, const string& city);
//   void add_show(const string& showId, const string& theaterId,
//                 const string& movie, const string& time, int rows, int cols);
//   vector<string> search_movies(const string& city);
//   vector<pair<int,int>> get_available_seats(const string& showId);
//   bool book_seats(const string& bookingId, const string& showId,
//                   const vector<pair<int,int>>& seatPositions, const string& userId);
//
// ─────────────────────────────────────────────────────────────────────────────


