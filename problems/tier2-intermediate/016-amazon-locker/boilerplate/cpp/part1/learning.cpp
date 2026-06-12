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

// ─── Smallest-Fit Allocation Strategy ────────────────────────────────────────

class SmallestFitStrategy : public LockerAllocationStrategy {
public:
    string allocate(LockerSize packageSize,
                    map<LockerSize, queue<string>>& available) override {
        // TODO: Build a list of sizes to try, starting from packageSize up to LARGE
        //       For each size, check if the queue has an available locker
        //       If found, pop from queue and return the locker ID
        //       If none found, return ""
        return "";
    }
};

// ─── Deposit Record ─────────────────────────────────────────────────────────

struct DepositRecord {
    string lockerId;
    string packageId;
    string pickupCode;
};

// ─── Locker System ──────────────────────────────────────────────────────────

class LockerSystem {
private:
    map<string, Locker> lockers;                    // lockerId → Locker
    map<LockerSize, queue<string>> available;        // size → queue of available locker IDs
    map<string, DepositRecord> activeDeposits;       // pickupCode → DepositRecord
    LockerAllocationStrategy* strategy;
    int codeCounter = 0;

    string generateCode() {
        return "CODE-" + to_string(++codeCounter);
    }

public:
    LockerSystem() {
        strategy = new SmallestFitStrategy();
    }

    ~LockerSystem() {
        delete strategy;
    }

    void addLocker(const string& lockerId, LockerSize size) {
        // TODO: Create a Locker, store in lockers map, add to available queue
    }

    string depositPackage(const string& packageId, LockerSize size) {
        // TODO: Use strategy to allocate a locker
        //       If allocation fails, return ""
        //       Mark locker as occupied
        //       Generate pickup code, create DepositRecord, store in activeDeposits
        //       Return the pickup code
        return "";
    }

    bool retrievePackage(const string& code) {
        // TODO: Look up code in activeDeposits
        //       If not found, return false
        //       Mark locker as not occupied
        //       Add locker back to available queue
        //       Remove from activeDeposits
        //       Return true
        return false;
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

string depositPackage(const string& packageId, LockerSize size) {
    if (!g_system) initLockerSystem();
    return g_system->depositPackage(packageId, size);
}

bool retrievePackage(const string& code) {
    if (!g_system) return false;
    return g_system->retrievePackage(code);
}

#ifndef RUNNING_TESTS
int main() {
    cout << "Amazon Locker System — implement the TODO methods above." << endl;
    return 0;
}
#endif
