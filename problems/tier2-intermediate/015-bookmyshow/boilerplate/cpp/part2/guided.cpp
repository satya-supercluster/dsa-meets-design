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

struct SeatLock {
    string id;
    string showId;
    string userId;
    vector<pair<int,int>> seatPositions;
    long expiry;
    bool confirmed;
    bool released;
};

// ─── Booking System ────────────────────────────────────────────────────────
// HINT: Add an expireSeat(seat, currentTime) helper that resets LOCKED seats
//       whose lockExpiry <= currentTime back to AVAILABLE.
// HINT: Call expireSeat before checking any seat's status.
// HINT: For lockSeats — check all seats available, generate a lockId,
//       set each seat to LOCKED with lockedBy and lockExpiry.
// HINT: For confirmBooking — check lock exists, not expired, not already
//       confirmed/released. Then set seats to BOOKED.
// HINT: For releaseLock — set seats back to AVAILABLE, mark lock as released.

// class BookingSystem {
//     unordered_map<string, Theater> theaters;
//     unordered_map<string, Show> shows;
//     unordered_map<string, Booking> bookings;
//     unordered_map<string, SeatLock> locks;
//     unordered_map<string, set<string>> cityMovies;
//     int lockCounter = 0;
//
//     void expireSeat(Seat& seat, long currentTime);
// public:
//     void addTheater(...);
//     void addShow(...);
//     vector<string> searchMovies(const string& city);
//     vector<pair<int,int>> getAvailableSeats(const string& showId, long currentTime);
//     bool bookSeats(const string& bookingId, const string& showId,
//                    const vector<pair<int,int>>& seatPositions,
//                    const string& userId, long currentTime);
//     string lockSeats(const string& showId,
//                      const vector<pair<int,int>>& seatPositions,
//                      const string& userId, int ttlMinutes, long currentTime);
//     bool confirmBooking(const string& lockId, long currentTime);
//     bool releaseLock(const string& lockId, long currentTime);
// };

// ─── Test Entry Points (must exist for tests to compile) ────────────────────
//   void add_theater(...);
//   void add_show(...);
//   vector<string> search_movies(const string& city);
//   vector<pair<int,int>> get_available_seats(const string& showId, long currentTime);
//   bool book_seats(..., long currentTime);
//   string lock_seats(const string& showId, ..., int ttlMinutes, long currentTime);
//   bool confirm_booking(const string& lockId, long currentTime);
//   bool release_lock(const string& lockId, long currentTime);
// ─────────────────────────────────────────────────────────────────────────────


