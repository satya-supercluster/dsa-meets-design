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
// HINT: Use unordered_map<string, set<string>> to index city -> movies.
// HINT: Initialize seats as a 2D grid: vector<vector<Seat>>(rows, vector<Seat>(cols)).
// HINT: For atomic booking — first loop checks ALL seats, second loop books ALL.

// class BookingSystem {
//     unordered_map<string, Theater> theaters;
//     unordered_map<string, Show> shows;
//     unordered_map<string, Booking> bookings;
//     unordered_map<string, set<string>> cityMovies;
// public:
//     void addTheater(const string& theaterId, const string& name, const string& city);
//     void addShow(const string& showId, const string& theaterId,
//                  const string& movie, const string& time, int rows, int cols);
//     vector<string> searchMovies(const string& city);
//     vector<pair<int,int>> getAvailableSeats(const string& showId);
//     bool bookSeats(const string& bookingId, const string& showId,
//                    const vector<pair<int,int>>& seatPositions, const string& userId);
// };

// ─── Test Entry Points (must exist for tests to compile) ────────────────────
// Your solution must provide these functions:
//
//   void add_theater(const string& theaterId, const string& name, const string& city);
//   void add_show(const string& showId, const string& theaterId,
//                 const string& movie, const string& time, int rows, int cols);
//   vector<string> search_movies(const string& city);
//   vector<pair<int,int>> get_available_seats(const string& showId);
//   bool book_seats(const string& bookingId, const string& showId,
//                   const vector<pair<int,int>>& seatPositions, const string& userId);
// ─────────────────────────────────────────────────────────────────────────────


