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

class BookingSystem {
    unordered_map<string, Theater> theaters;
    unordered_map<string, Show> shows;
    unordered_map<string, Booking> bookings;
    unordered_map<string, SeatLock> locks;
    unordered_map<string, set<string>> cityMovies;
    int lockCounter = 0;

    void expireSeat(Seat& seat, long currentTime) {
        // TODO: If seat is LOCKED and currentTime >= lockExpiry,
        //       reset to AVAILABLE and clear lockedBy/lockExpiry
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
                                            long currentTime) {
        if (!shows.count(showId)) return {};
        auto& show = shows[showId];
        vector<pair<int,int>> result;
        for (int r = 0; r < show.rows; r++)
            for (int c = 0; c < show.cols; c++) {
                // TODO: Call expireSeat first, then check if AVAILABLE
            }
        return result;
    }

    bool bookSeats(const string& bookingId, const string& showId,
                   const vector<pair<int,int>>& seatPositions,
                   const string& userId, long currentTime) {
        if (!shows.count(showId)) return false;
        auto& show = shows[showId];
        // TODO: First pass — expire and check ALL seats are AVAILABLE
        // TODO: Second pass — set all to BOOKED with bookedBy = userId
        // TODO: Store booking
        return false;
    }

    string lockSeats(const string& showId,
                     const vector<pair<int,int>>& seatPositions,
                     const string& userId, int ttlMinutes, long currentTime) {
        if (!shows.count(showId)) return "";
        auto& show = shows[showId];
        // TODO: Check all seats are available (after expiring)
        // TODO: Generate lockId = "LOCK_" + to_string(++lockCounter)
        // TODO: Compute expiry = currentTime + ttlMinutes * 60
        // TODO: Set each seat to LOCKED with lockedBy and lockExpiry
        // TODO: Store SeatLock in locks map
        return "";
    }

    bool confirmBooking(const string& lockId, long currentTime) {
        // TODO: Find lock, check not confirmed/released
        // TODO: If currentTime >= expiry, expire seats and return false
        // TODO: Set all locked seats to BOOKED
        // TODO: Create booking, mark lock as confirmed
        return false;
    }

    bool releaseLock(const string& lockId, long currentTime) {
        // TODO: Find lock, check not confirmed/released
        // TODO: Set all seats back to AVAILABLE
        // TODO: Mark lock as released
        return false;
    }
};

// ─── Test Entry Points ─────────────────────────────────────────────────────

BookingSystem system_instance;

void add_theater(const string& theaterId, const string& name,
                 const string& city) {
    system_instance.addTheater(theaterId, name, city);
}

void add_show(const string& showId, const string& theaterId,
              const string& movie, const string& time, int rows, int cols) {
    system_instance.addShow(showId, theaterId, movie, time, rows, cols);
}

vector<string> search_movies(const string& city) {
    return system_instance.searchMovies(city);
}

vector<pair<int,int>> get_available_seats(const string& showId,
                                          long currentTime) {
    return system_instance.getAvailableSeats(showId, currentTime);
}

bool book_seats(const string& bookingId, const string& showId,
                const vector<pair<int,int>>& seatPositions,
                const string& userId, long currentTime) {
    return system_instance.bookSeats(bookingId, showId, seatPositions,
                                     userId, currentTime);
}

string lock_seats(const string& showId,
                  const vector<pair<int,int>>& seatPositions,
                  const string& userId, int ttlMinutes, long currentTime) {
    return system_instance.lockSeats(showId, seatPositions, userId,
                                     ttlMinutes, currentTime);
}

bool confirm_booking(const string& lockId, long currentTime) {
    return system_instance.confirmBooking(lockId, currentTime);
}

bool release_lock(const string& lockId, long currentTime) {
    return system_instance.releaseLock(lockId, currentTime);
}

#ifndef RUNNING_TESTS
int main() {
    cout << "BookMyShow Part 2 — implement the TODO methods above, then run tests." << endl;
    return 0;
}
#endif
