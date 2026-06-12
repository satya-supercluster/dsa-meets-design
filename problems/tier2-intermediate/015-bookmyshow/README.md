# Problem 015 — BookMyShow Ticket Booking System

**Tier:** 2 (Intermediate) | **Pattern:** Strategy + Observer + State | **DSA:** HashMap + Matrix + Queue
**Companies:** DoorDash, BookMyShow, Swiggy, Paytm | **Time:** 50 minutes

---

## Problem Statement

You're building a movie ticket booking system like BookMyShow. Theaters have screens with 2D seating grids. Users search for movies by city, select a show, view available seats, and book them. The system must prevent double-booking and support temporary seat locking with auto-expiry.

**Your task:** Design and implement a `BookingSystem` that manages theaters, shows, seat availability, bookings, and seat locks with TTL-based auto-release.

---

## Before You Code

> Read this section carefully. This is where the design thinking happens.

**Ask yourself:**
1. How do you represent a theater screen's seating arrangement? A flat list, or a 2D grid (rows x columns)? How does this affect seat selection UX?
2. When two users try to book the same seat simultaneously, how do you prevent double-booking? Is checking availability and booking a single atomic operation, or two separate steps?
3. When a user starts selecting seats, should those seats be immediately reserved? What happens if the user abandons the session? How long should a lock last?

**The key insight:** Seats transition through states: AVAILABLE -> LOCKED -> BOOKED (or LOCKED -> AVAILABLE on timeout). This is a **State** pattern at the seat level. The lock-with-TTL mechanism uses timestamps — when checking seat status, compare current time against lock expiry to determine if a lock is still active. The booking flow is: search shows -> view seats -> lock seats -> confirm booking.

---

## Data Structures

```cpp
enum class SeatStatus { AVAILABLE, LOCKED, BOOKED };

struct Seat {
    int row;
    int col;
    SeatStatus status;
    string lockedBy;   // userId who locked this seat
    long lockExpiry;   // timestamp when lock expires (0 if not locked)
    string bookedBy;   // userId who booked this seat
};

struct Show {
    string id;
    string theaterId;
    string movie;
    string time;       // e.g., "18:00"
    int rows;
    int cols;
    vector<vector<Seat>> seats;  // 2D seating grid
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
    vector<pair<int,int>> seatPositions;  // (row, col) pairs
};
```

---

## Part 1

**Base requirement — Theater, show, and booking management**

Implement a `BookingSystem` that manages theaters, shows, and seat bookings. Seats are represented as a 2D grid. Users can view available seats and book them. Double-booking must be prevented.

**Seat ID convention:** Seats are identified by `(row, col)` pairs, 0-indexed.

**Entry points (tests will call these):**
```cpp
void add_theater(const string& theaterId, const string& name, const string& city);
void add_show(const string& showId, const string& theaterId,
              const string& movie, const string& time, int rows, int cols);
vector<string> search_movies(const string& city);
vector<pair<int,int>> get_available_seats(const string& showId);
bool book_seats(const string& bookingId, const string& showId,
                const vector<pair<int,int>>& seatPositions, const string& userId);
```

**What to implement:**
```cpp
class BookingSystem {
    unordered_map<string, Theater> theaters;
    unordered_map<string, Show> shows;
    unordered_map<string, Booking> bookings;
    // city -> set of movie names
    unordered_map<string, set<string>> cityMovies;
public:
    void addTheater(const string& theaterId, const string& name, const string& city);
    void addShow(const string& showId, const string& theaterId,
                 const string& movie, const string& time, int rows, int cols);
    vector<string> searchMovies(const string& city);
    vector<pair<int,int>> getAvailableSeats(const string& showId);
    bool bookSeats(const string& bookingId, const string& showId,
                   const vector<pair<int,int>>& seatPositions, const string& userId);
};
```

**Design goal:** Booking must be atomic — either all requested seats are booked, or none are (if any seat is already taken). The seat grid must correctly reflect availability after each booking.

---

## Part 2

**Extension 1 — Seat locking with TTL**

When a user selects seats, the system should **lock** them temporarily. Other users cannot book locked seats. If the user doesn't confirm within the TTL, the lock auto-releases.

**Lock lifecycle:**
1. User calls `lockSeats(showId, seatIds, userId, ttlMinutes)` — seats become LOCKED
2. User calls `confirmBooking(lockId)` — seats become BOOKED (only if lock hasn't expired)
3. If TTL expires before confirmation, seats return to AVAILABLE
4. User can manually call `releaseLock(lockId)` to cancel

**Time model:** Use a `currentTime` variable (long, seconds since epoch) that tests can control. When checking if a lock is active: `currentTime < lockExpiry`.

**New entry points:**
```cpp
string lock_seats(const string& showId, const vector<pair<int,int>>& seatPositions,
                  const string& userId, int ttlMinutes, long currentTime);
// Returns a lockId, or "" if any seat is not available

bool confirm_booking(const string& lockId, long currentTime);
// Converts locked seats to booked. Fails if lock expired.

bool release_lock(const string& lockId, long currentTime);
// Manually releases a lock. Seats return to AVAILABLE.
```

**What to implement:**
```cpp
struct SeatLock {
    string id;
    string showId;
    string userId;
    vector<pair<int,int>> seatPositions;
    long expiry;  // timestamp when lock expires
    bool confirmed;
    bool released;
};
```

**Design challenge:** When checking seat availability, you must account for expired locks — an expired lock means the seat is actually AVAILABLE, even though its status might still say LOCKED. Always check `currentTime >= lockExpiry` before trusting the lock status.

---

## Running Tests

```bash
./run-tests.sh 015-bookmyshow cpp
```
