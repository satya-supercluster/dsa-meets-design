# Design Walkthrough — BookMyShow Ticket Booking System

> This file is the answer guide. Only read after you've attempted the problem.

---

## The Core Design Decision

Three concerns compose here:

1. **Seat state management** — Seats transition between AVAILABLE, LOCKED, and BOOKED. The system must handle expired locks gracefully by checking timestamps before trusting status.
2. **Atomic booking** — Either all seats in a booking request are reserved, or none are. This prevents partial bookings that leave users with non-adjacent seats.
3. **TTL-based locking** — Locks have expiry timestamps. Instead of a background timer, we use **lazy expiration** — check if a lock is expired whenever we query seat status.

```
BookingSystem
    ├── unordered_map<string, Theater>        (theater registry)
    ├── unordered_map<string, Show>           (shows with 2D seat grids)
    ├── unordered_map<string, Booking>        (confirmed bookings)
    ├── unordered_map<string, SeatLock>       (active locks)
    └── unordered_map<string, set<string>>    (city -> movies index)

bookSeats(showId, seats, userId):
    for each seat:
        if seat.status != AVAILABLE: return false  // atomic check
    for each seat:
        seat.status = BOOKED
        seat.bookedBy = userId

lockSeats(showId, seats, userId, ttl, currentTime):
    for each seat:
        if !isSeatAvailable(seat, currentTime): return ""
    expiry = currentTime + ttl * 60
    for each seat:
        seat.status = LOCKED
        seat.lockedBy = userId
        seat.lockExpiry = expiry
```

---

## Why Lazy Expiration?

A background timer that auto-releases locks is complex and error-prone. Instead, we check the timestamp whenever we need to know a seat's true status:

```cpp
bool isSeatAvailable(const Seat& seat, long currentTime) {
    if (seat.status == SeatStatus::AVAILABLE) return true;
    if (seat.status == SeatStatus::LOCKED && currentTime >= seat.lockExpiry) {
        // Lock has expired — seat is effectively available
        return true;
    }
    return false;
}
```

This approach is simpler, thread-safe (no background threads), and correct. The seat's status field might say LOCKED, but if the lock has expired, we treat it as AVAILABLE. When we actually use the seat (lock or book it), we update the status.

---

## 2D Seat Grid — Why a Matrix?

A flat list of seats would work, but a 2D grid (`vector<vector<Seat>>`) maps naturally to a theater's physical layout:

```
Row 0: [ A1  A2  A3  A4  A5 ]
Row 1: [ B1  B2  B3  B4  B5 ]
Row 2: [ C1  C2  C3  C4  C5 ]
```

This makes it easy to:
- Display the seating map to users
- Find adjacent seats in a row
- Support different row sizes (e.g., premium rows with fewer seats)

---

## Reference Implementation

```cpp
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
using namespace std;

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

class BookingSystem {
    unordered_map<string, Theater> theaters;
    unordered_map<string, Show> shows;
    unordered_map<string, Booking> bookings;
    unordered_map<string, SeatLock> locks;
    unordered_map<string, set<string>> cityMovies;
    int lockCounter = 0;

    bool isSeatAvailable(const Seat& seat, long currentTime) const {
        if (seat.status == SeatStatus::AVAILABLE) return true;
        if (seat.status == SeatStatus::LOCKED && currentTime >= seat.lockExpiry)
            return true;
        return false;
    }

    void expireSeat(Seat& seat, long currentTime) {
        if (seat.status == SeatStatus::LOCKED && currentTime >= seat.lockExpiry) {
            seat.status = SeatStatus::AVAILABLE;
            seat.lockedBy = "";
            seat.lockExpiry = 0;
        }
    }

public:
    void addTheater(const string& theaterId, const string& name,
                    const string& city) {
        theaters[theaterId] = {theaterId, name, city};
    }

    void addShow(const string& showId, const string& theaterId,
                 const string& movie, const string& time, int rows, int cols) {
        Show show{showId, theaterId, movie, time, rows, cols, {}};
        show.seats.resize(rows, vector<Seat>(cols));
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                show.seats[r][c] = {r, c, SeatStatus::AVAILABLE, "", 0, ""};
        shows[showId] = show;
        if (theaters.count(theaterId))
            cityMovies[theaters[theaterId].city].insert(movie);
    }

    vector<string> searchMovies(const string& city) {
        if (!cityMovies.count(city)) return {};
        return vector<string>(cityMovies[city].begin(), cityMovies[city].end());
    }

    vector<pair<int,int>> getAvailableSeats(const string& showId,
                                            long currentTime = 0) {
        if (!shows.count(showId)) return {};
        auto& show = shows[showId];
        vector<pair<int,int>> result;
        for (int r = 0; r < show.rows; r++)
            for (int c = 0; c < show.cols; c++) {
                expireSeat(show.seats[r][c], currentTime);
                if (show.seats[r][c].status == SeatStatus::AVAILABLE)
                    result.push_back({r, c});
            }
        return result;
    }

    bool bookSeats(const string& bookingId, const string& showId,
                   const vector<pair<int,int>>& seatPositions,
                   const string& userId, long currentTime = 0) {
        if (!shows.count(showId)) return false;
        auto& show = shows[showId];
        // Check all seats are available
        for (auto& [r, c] : seatPositions) {
            if (r < 0 || r >= show.rows || c < 0 || c >= show.cols)
                return false;
            expireSeat(show.seats[r][c], currentTime);
            if (show.seats[r][c].status != SeatStatus::AVAILABLE)
                return false;
        }
        // Book all seats
        for (auto& [r, c] : seatPositions) {
            show.seats[r][c].status = SeatStatus::BOOKED;
            show.seats[r][c].bookedBy = userId;
        }
        bookings[bookingId] = {bookingId, showId, userId, seatPositions};
        return true;
    }

    string lockSeats(const string& showId,
                     const vector<pair<int,int>>& seatPositions,
                     const string& userId, int ttlMinutes, long currentTime) {
        if (!shows.count(showId)) return "";
        auto& show = shows[showId];
        // Check all seats are available
        for (auto& [r, c] : seatPositions) {
            if (r < 0 || r >= show.rows || c < 0 || c >= show.cols)
                return "";
            expireSeat(show.seats[r][c], currentTime);
            if (show.seats[r][c].status != SeatStatus::AVAILABLE)
                return "";
        }
        string lockId = "LOCK_" + to_string(++lockCounter);
        long expiry = currentTime + ttlMinutes * 60;
        // Lock all seats
        for (auto& [r, c] : seatPositions) {
            show.seats[r][c].status = SeatStatus::LOCKED;
            show.seats[r][c].lockedBy = userId;
            show.seats[r][c].lockExpiry = expiry;
        }
        locks[lockId] = {lockId, showId, userId, seatPositions,
                         expiry, false, false};
        return lockId;
    }

    bool confirmBooking(const string& lockId, long currentTime) {
        if (!locks.count(lockId)) return false;
        auto& lock = locks[lockId];
        if (lock.confirmed || lock.released) return false;
        if (currentTime >= lock.expiry) {
            // Lock expired — release seats
            if (shows.count(lock.showId)) {
                auto& show = shows[lock.showId];
                for (auto& [r, c] : lock.seatPositions) {
                    expireSeat(show.seats[r][c], currentTime);
                }
            }
            lock.released = true;
            return false;
        }
        // Confirm booking
        if (!shows.count(lock.showId)) return false;
        auto& show = shows[lock.showId];
        for (auto& [r, c] : lock.seatPositions) {
            show.seats[r][c].status = SeatStatus::BOOKED;
            show.seats[r][c].bookedBy = lock.userId;
            show.seats[r][c].lockedBy = "";
            show.seats[r][c].lockExpiry = 0;
        }
        string bookingId = "BK_" + lockId;
        bookings[bookingId] = {bookingId, lock.showId, lock.userId,
                               lock.seatPositions};
        lock.confirmed = true;
        return true;
    }

    bool releaseLock(const string& lockId, long currentTime) {
        if (!locks.count(lockId)) return false;
        auto& lock = locks[lockId];
        if (lock.confirmed || lock.released) return false;
        if (shows.count(lock.showId)) {
            auto& show = shows[lock.showId];
            for (auto& [r, c] : lock.seatPositions) {
                show.seats[r][c].status = SeatStatus::AVAILABLE;
                show.seats[r][c].lockedBy = "";
                show.seats[r][c].lockExpiry = 0;
            }
        }
        lock.released = true;
        return true;
    }
};
```

---

## What interviewers look for

1. **Atomic booking**: Did you check all seats before booking any? If one seat is taken, the whole request should fail — no partial bookings.
2. **Seat state transitions**: Can you articulate the AVAILABLE -> LOCKED -> BOOKED lifecycle? What about LOCKED -> AVAILABLE on expiry?
3. **Lazy expiration**: Did you check timestamps when querying availability, or did you rely on a background thread?
4. **2D grid modeling**: Did you use a matrix for seats, or a flat list? The matrix is more natural for theater seating.

---

## Common interview follow-ups

- *"How would you handle concurrent booking requests?"* — Use a mutex per show or per seat row. Optimistic locking with version numbers also works.
- *"How would you add seat categories (Regular, Premium, VIP)?"* — Add a `category` field to Seat. Pricing depends on category — a Strategy pattern for pricing.
- *"What if you need to support different theater layouts?"* — Allow irregular grids (different column counts per row). Store layout as configuration.
- *"How would you scale to millions of concurrent users?"* — Partition by city/theater. Use Redis for seat locks with TTL. Database for bookings.
