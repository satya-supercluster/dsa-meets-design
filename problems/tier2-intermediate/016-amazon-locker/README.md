# Problem 016 — Amazon Locker System

**Tier:** 2 (Intermediate) | **Patterns:** Strategy, Factory, State | **DSA:** HashMap, Queue
**Companies:** Amazon | **Time:** 25 minutes

---

## Problem Statement

Build an Amazon Locker package management system. The system must:

1. Manage lockers of different sizes (Small, Medium, Large)
2. Allocate the **smallest fitting locker** for a given package
3. Generate a unique pickup code upon deposit
4. Allow retrieval using the pickup code, freeing the locker

---

## Before You Code

**The naive approach:** A single function with nested if-else to check locker sizes, generate codes, handle state transitions, and manage expiry. This becomes unmaintainable the moment you add new locker types, new notification channels, or code expiry logic.

**The pattern approach:**
- **Strategy**: Locker allocation is a strategy — "find the smallest fitting locker" is one policy, but the system should support swapping to "find the nearest locker" or "find the most secure locker" without rewriting allocation logic.
- **Factory**: Creating lockers of different sizes with their constraints is a factory concern. Adding a new size (e.g., XL) should require zero changes to the allocation engine.
- **State**: Each locker transitions through states (Available → Occupied → Available). In Part 2, packages transition through (Deposited → PickedUp / Expired → Returned). State machines prevent invalid transitions.

**The DSA angle:** HashMap gives O(1) lookup for pickup codes and locker IDs. A Queue per locker size allows O(1) allocation of the next available locker of a given size.

---

## Data Structures

```cpp
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
```

---

## Part 1

**Base requirement — Core locker allocation and retrieval**

Implement a locker management system that allocates the smallest fitting locker for a package and supports retrieval via pickup code.

| Locker Size | Can Fit |
|-------------|---------|
| **SMALL**   | SMALL packages only |
| **MEDIUM**  | SMALL or MEDIUM packages |
| **LARGE**   | SMALL, MEDIUM, or LARGE packages |

**Allocation rule:** Always allocate the **smallest available locker** that fits the package. If a SMALL package arrives and no SMALL lockers are free, try MEDIUM, then LARGE. If no locker fits, return an empty string (allocation failed).

**Pickup code:** A unique string returned on successful deposit. The same code is used for retrieval.

**Entry points (tests will call these):**
```cpp
void addLocker(const string& lockerId, LockerSize size);
string depositPackage(const string& packageId, LockerSize size);
    // Returns pickup code on success, "" on failure
bool retrievePackage(const string& code);
    // Returns true if retrieval succeeded, false if code is invalid
```

**What to implement:**
```cpp
class LockerAllocationStrategy {
public:
    virtual string allocate(LockerSize packageSize) = 0;
    virtual ~LockerAllocationStrategy() = default;
};

class SmallestFitStrategy : public LockerAllocationStrategy { ... };

class LockerSystem {
    // HashMap: lockerId → Locker
    // Queue per size: available lockers
    // HashMap: pickupCode → (lockerId, packageId)
public:
    void addLocker(const string& lockerId, LockerSize size);
    string depositPackage(const string& packageId, LockerSize size);
    bool retrievePackage(const string& code);
};
```

---

## Part 2

**Extension — Code expiry and notifications**

Pickup codes now expire after a configurable number of hours. Expired packages are marked for return and their locker is freed. The system supports notification channels that fire on deposit and before expiry.

**Expiry rules:**
- Each pickup code has a creation timestamp
- After `N` hours, the code expires
- Calling `checkExpired(currentTime)` scans all active codes, expires any that exceeded the time limit, frees the locker, and returns the list of expired package IDs

**Notification interface:**
```cpp
class NotificationChannel {
public:
    virtual void notify(const string& packageId, const string& message) = 0;
    virtual ~NotificationChannel() = default;
};
```

- On successful deposit: notify all channels with `"Package <id> deposited. Code: <code>"`
- On expiry: notify all channels with `"Package <id> expired. Locker freed."`

**New entry points:**
```cpp
void setCodeExpiry(int hours);
vector<string> checkExpired(long currentTime);
    // Returns list of expired packageIds, frees their lockers
void addNotificationChannel(NotificationChannel* channel);
```

**Design challenge:** How do you add expiry and notifications without modifying the core deposit/retrieve logic from Part 1? The State pattern models locker lifecycle; Strategy still governs allocation; Observer handles notifications.

---

## Running Tests

```bash
./run-tests.sh 016-amazon-locker cpp
```
