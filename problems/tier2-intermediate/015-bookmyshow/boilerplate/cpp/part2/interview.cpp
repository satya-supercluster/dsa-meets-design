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

// ─── Your Design Starts Here ────────────────────────────────────────────────
//
// Extend the BookingSystem with seat locking and TTL:
//   1. lockSeats(showId, seats, userId, ttlMinutes, currentTime)
//      — Temporarily lock seats. Returns a lockId.
//   2. confirmBooking(lockId, currentTime)
//      — Converts locked seats to BOOKED. Fails if lock expired.
//   3. releaseLock(lockId, currentTime)
//      — Manually releases a lock. Seats return to AVAILABLE.
//
// Think about:
//   - Seats transition: AVAILABLE -> LOCKED -> BOOKED (or LOCKED -> AVAILABLE)
//   - Use lazy expiration: when checking availability, compare currentTime
//     against lockExpiry. If expired, treat as AVAILABLE.
//   - No background threads — the currentTime parameter simulates time.
//   - A locked seat blocks other users from booking/locking it.
//   - What happens if confirmBooking is called after expiry?
//
// Entry points (must exist for tests — includes all Part 1 entry points):
//   void add_theater(...);
//   void add_show(...);
//   vector<string> search_movies(const string& city);
//   vector<pair<int,int>> get_available_seats(const string& showId, long currentTime);
//   bool book_seats(const string& bookingId, const string& showId,
//                   const vector<pair<int,int>>& seatPositions,
//                   const string& userId, long currentTime);
//   string lock_seats(const string& showId, const vector<pair<int,int>>& seatPositions,
//                     const string& userId, int ttlMinutes, long currentTime);
//   bool confirm_booking(const string& lockId, long currentTime);
//   bool release_lock(const string& lockId, long currentTime);
//
// ─────────────────────────────────────────────────────────────────────────────


