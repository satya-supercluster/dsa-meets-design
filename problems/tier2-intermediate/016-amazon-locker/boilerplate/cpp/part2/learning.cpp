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

// ─── Allocation Strategy Interface ───────────────────────────────────────────

class LockerAllocationStrategy {
public:
    virtual string allocate(LockerSize packageSize,
                            map<LockerSize, queue<string>>& available) = 0;
    virtual ~LockerAllocationStrategy() = default;
};

class SmallestFitStrategy : public LockerAllocationStrategy {
public:
    string allocate(LockerSize packageSize,
                    map<LockerSize, queue<string>>& available) override {
        // TODO: Try sizes from packageSize up to LARGE
        //       Pop and return first available locker ID, or ""
        return "";
    }
};

// ─── Notification Channel Interface ─────────────────────────────────────────

class NotificationChannel {
public:
    virtual void notify(const string& packageId, const string& message) = 0;
    virtual ~NotificationChannel() = default;
};

// ─── Deposit Record (with timestamp) ────────────────────────────────────────

struct DepositRecord {
    string lockerId;
    string packageId;
    string pickupCode;
    long   depositTime;
};

// ─── Locker System ──────────────────────────────────────────────────────────

class LockerSystem {
private:
    map<string, Locker> lockers;
    map<LockerSize, queue<string>> available;
    map<string, DepositRecord> activeDeposits;   // pickupCode → record
    LockerAllocationStrategy* strategy;
    vector<NotificationChannel*> channels;
    int codeCounter = 0;
    int expiryHours = 0;  // 0 means no expiry

    string generateCode() {
        return "CODE-" + to_string(++codeCounter);
    }

    void notifyAll(const string& packageId, const string& message) {
        // TODO: Iterate over all channels and call notify()
    }

    void freeLocker(const string& lockerId) {
        // TODO: Mark locker as not occupied
        //       Add locker back to the available queue for its size
    }

public:
    LockerSystem() {
        strategy = new SmallestFitStrategy();
    }

    ~LockerSystem() {
        delete strategy;
    }

    void addLocker(const string& lockerId, LockerSize size) {
        // TODO: Create locker, store in map, add to available queue
    }

    string depositPackage(const string& packageId, LockerSize size,
                          long depositTime = 0) {
        // TODO: Allocate locker using strategy
        //       If failed, return ""
        //       Mark locker occupied, generate code, store DepositRecord with timestamp
        //       Notify all channels: "Package <id> deposited. Code: <code>"
        //       Return the code
        return "";
    }

    bool retrievePackage(const string& code) {
        // TODO: Look up code in activeDeposits
        //       If not found, return false
        //       Free the locker, remove deposit record
        //       Return true
        return false;
    }

    void setCodeExpiry(int hours) {
        // TODO: Set the expiry duration
    }

    vector<string> checkExpired(long currentTime) {
        // TODO: Scan all activeDeposits
        //       For each deposit where (currentTime - depositTime) > expiryHours * 3600
        //       Free the locker, notify channels, collect packageId
        //       Remove expired entries from activeDeposits
        //       Return list of expired packageIds
        return {};
    }

    void addNotificationChannel(NotificationChannel* channel) {
        // TODO: Add channel to the channels list
    }
};

// ─── Global Entry Points ─────────────────────────────────────────────────────

static LockerSystem* g_system = nullptr;

void initLockerSystem() {
    delete g_system;
    g_system = new LockerSystem();
}

void addLocker(const string& lockerId, LockerSize size) {
    if (!g_system) initLockerSystem();
    g_system->addLocker(lockerId, size);
}

string depositPackage(const string& packageId, LockerSize size,
                      long depositTime = 0) {
    if (!g_system) initLockerSystem();
    return g_system->depositPackage(packageId, size, depositTime);
}

bool retrievePackage(const string& code) {
    if (!g_system) return false;
    return g_system->retrievePackage(code);
}

void setCodeExpiry(int hours) {
    if (!g_system) initLockerSystem();
    g_system->setCodeExpiry(hours);
}

vector<string> checkExpired(long currentTime) {
    if (!g_system) return {};
    return g_system->checkExpired(currentTime);
}

void addNotificationChannel(NotificationChannel* channel) {
    if (!g_system) initLockerSystem();
    g_system->addNotificationChannel(channel);
}

#ifndef RUNNING_TESTS
int main() {
    cout << "Amazon Locker System Part 2 — implement the TODO methods above." << endl;
    return 0;
}
#endif
