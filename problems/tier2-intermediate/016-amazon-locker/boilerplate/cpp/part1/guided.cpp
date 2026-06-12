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

// ─── Allocation Interface ────────────────────────────────────────────────────
// HINT: The allocation policy is separate from the locker system itself.
// One policy tries the smallest fitting locker first, but others are possible.

class AllocationPolicy {
public:
    virtual string allocate(LockerSize packageSize) = 0;
    virtual ~AllocationPolicy() = default;
};

// TODO: Implement a concrete allocation policy that:
//   - Tries the exact size first, then larger sizes in order
//   - Returns the locker ID if found, "" if none available
// HINT: Use a queue per locker size for O(1) allocation.

// ─── Locker System ──────────────────────────────────────────────────────────
// HINT: The system needs to track:
//   - All lockers by ID (for state changes)
//   - Available lockers per size (for fast allocation)
//   - Active deposits by pickup code (for retrieval)

// TODO: Implement the locker system with these entry points:
//   void addLocker(const string& lockerId, LockerSize size);
//   string depositPackage(const string& packageId, LockerSize size);
//   bool retrievePackage(const string& code);

// HINT: Pickup codes must be unique. Consider using a counter or combining
// package ID with a timestamp.

// ─── Test Entry Points ───────────────────────────────────────────────────────
//   void addLocker(const string& lockerId, LockerSize size);
//   string depositPackage(const string& packageId, LockerSize size);
//   bool retrievePackage(const string& code);
// ─────────────────────────────────────────────────────────────────────────────

