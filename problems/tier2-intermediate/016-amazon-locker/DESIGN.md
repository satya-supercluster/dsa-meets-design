# Design Walkthrough — Amazon Locker System

> Only read after attempting the problem.

---

## How Strategy + Factory + State compose

```
LockerSystem
    ├── LockerAllocationStrategy* (swappable — SmallestFit / NearestLocker / SecureFirst)
    ├── map<LockerSize, queue<string>> availableLockers  (queues per size)
    ├── map<string, Locker> allLockers                   (lockerId → Locker)
    ├── map<string, DepositRecord> activeDeposits        (pickupCode → record)
    └── vector<NotificationChannel*> channels

depositPackage(packageId, size):
    lockerId = strategy->allocate(size)    // Strategy picks the locker
    if lockerId empty → return ""
    code = generateCode()
    activeDeposits[code] = {lockerId, packageId, timestamp}
    notify all channels
    return code
```

The system orchestrates three patterns:
- **Strategy** decides *which* locker to allocate
- **Factory** encapsulates locker creation (adding XL lockers requires no core changes)
- **State** governs locker lifecycle transitions (Available ↔ Occupied)

---

## Smallest-fit allocation with queues

```cpp
class SmallestFitStrategy : public LockerAllocationStrategy {
    map<LockerSize, queue<string>>& available;
public:
    string allocate(LockerSize packageSize) override {
        // Try sizes in order: packageSize, then larger sizes
        vector<LockerSize> tryOrder;
        if (packageSize == LockerSize::SMALL)
            tryOrder = {LockerSize::SMALL, LockerSize::MEDIUM, LockerSize::LARGE};
        else if (packageSize == LockerSize::MEDIUM)
            tryOrder = {LockerSize::MEDIUM, LockerSize::LARGE};
        else
            tryOrder = {LockerSize::LARGE};

        for (auto sz : tryOrder) {
            if (!available[sz].empty()) {
                string id = available[sz].front();
                available[sz].pop();
                return id;
            }
        }
        return "";  // no locker available
    }
};
```

**Why queues?** O(1) allocation and O(1) return. No need to scan all lockers each time.

**Why HashMap for codes?** O(1) lookup on retrieval — critical when the system has thousands of active deposits.

---

## Code expiry design

```cpp
struct DepositRecord {
    string lockerId;
    string packageId;
    string pickupCode;
    long   depositTime;
};

vector<string> checkExpired(long currentTime) {
    vector<string> expired;
    for (auto it = activeDeposits.begin(); it != activeDeposits.end(); ) {
        if (currentTime - it->second.depositTime > expiryHours * 3600) {
            // Free the locker
            freeLocker(it->second.lockerId);
            expired.push_back(it->second.packageId);
            notify("Package " + it->second.packageId + " expired.");
            it = activeDeposits.erase(it);
        } else {
            ++it;
        }
    }
    return expired;
}
```

**Key insight:** Expiry is a *scan* operation, not a timer. In a real system you'd use a min-heap sorted by expiry time for O(log n) processing, but the scan approach is simpler and correct for interview scope.

---

## Interview follow-ups

- *"How would you handle concurrent deposits?"* → Mutex per locker size queue. The pickup code generation must be atomic.
- *"What if you need to support locker reservations?"* → Add a RESERVED state between AVAILABLE and OCCUPIED. The State pattern handles this cleanly.
- *"How would you prioritize Prime members?"* → A new allocation strategy (PrimePriorityStrategy) that checks membership before allocating. Zero changes to the core system.
- *"How would you scale this to 10,000 locations?"* → Each location is an independent LockerSystem instance. A location router (Factory) creates the right one per request.
