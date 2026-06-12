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

// ─── Booking System ────────────────────────────────────────────────────────

class BookingSystem {
    unordered_map<string, Theater> theaters;
    unordered_map<string, Show> shows;
    unordered_map<string, Booking> bookings;
    unordered_map<string, set<string>> cityMovies;

public:
    void addTheater(const string& theaterId, const string& name,
                    const string& city) {
        theaters[theaterId] = {theaterId, name, city};
    }

    void addShow(const string& showId, const string& theaterId,
                 const string& movie, const string& time, int rows, int cols) {
        Show show{showId, theaterId, movie, time, rows, cols, {}};
        // TODO: Initialize seats grid — resize to rows x cols
        // TODO: Set each seat to {r, c, SeatStatus::AVAILABLE, "", 0, ""}
        shows[showId] = show;
        // TODO: Add movie to cityMovies index using theaters[theaterId].city
    }

    vector<string> searchMovies(const string& city) {
        // TODO: Return sorted list of movie names showing in this city
        // HINT: cityMovies[city] is a set<string>, convert to vector
        return {};
    }

    vector<pair<int,int>> getAvailableSeats(const string& showId) {
        if (!shows.count(showId)) return {};
        auto& show = shows[showId];
        vector<pair<int,int>> result;
        // TODO: Iterate all seats in the 2D grid
        // TODO: Add (row, col) to result if seat.status == SeatStatus::AVAILABLE
        return result;
    }

    bool bookSeats(const string& bookingId, const string& showId,
                   const vector<pair<int,int>>& seatPositions,
                   const string& userId) {
        if (!shows.count(showId)) return false;
        auto& show = shows[showId];
        // TODO: First pass — check ALL seats are valid and AVAILABLE
        // TODO: Second pass — book ALL seats
        // TODO: Store the Booking
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

vector<pair<int,int>> get_available_seats(const string& showId) {
    return system_instance.getAvailableSeats(showId);
}

bool book_seats(const string& bookingId, const string& showId,
                const vector<pair<int,int>>& seatPositions,
                const string& userId) {
    return system_instance.bookSeats(bookingId, showId, seatPositions, userId);
}

#ifndef RUNNING_TESTS
int main() {
    cout << "BookMyShow — implement the TODO methods above, then run tests." << endl;
    return 0;
}
#endif
