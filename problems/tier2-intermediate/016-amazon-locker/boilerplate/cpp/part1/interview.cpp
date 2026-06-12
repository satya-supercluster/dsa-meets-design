#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

enum class LockerSize { SMALL, MEDIUM, LARGE };

struct Package {
    string packageId;
    LockerSize size;
};

struct Locker {
    string lockerId;
    LockerSize size;
    bool occupied;
};

// ─── Your Design Starts Here ─────────────────────────────────────────────────
//
// Design and implement an Amazon Locker System that:
//   1. Manages lockers of different sizes (Small, Medium, Large)
//   2. Allocates the smallest fitting locker for a package
//   3. Generates a unique pickup code on deposit
//   4. Allows retrieval using the pickup code, freeing the locker
//
// Allocation rule:
//   SMALL packages → try SMALL, then MEDIUM, then LARGE
//   MEDIUM packages → try MEDIUM, then LARGE
//   LARGE packages → try LARGE only
//   Return "" if no locker is available.
//
// Think about:
//   - How do you make locker allocation swappable (e.g., smallest-fit vs nearest)?
//   - How do you ensure O(1) allocation per size?
//   - How would you add a new locker size (XL) with zero changes to existing code?
//
// Entry points:
//   void addLocker(const string& lockerId, LockerSize size);
//   string depositPackage(const string& packageId, LockerSize size);
//   bool retrievePackage(const string& code);
//
// ─────────────────────────────────────────────────────────────────────────────

