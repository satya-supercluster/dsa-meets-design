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

// ─── Notification Interface ─────────────────────────────────────────────────
// HINT: Multiple channels (email, SMS, push) can be registered.
// They are all notified on deposit and on expiry.

class NotificationChannel {
public:
    virtual void notify(const string& packageId, const string& message) = 0;
    virtual ~NotificationChannel() = default;
};

// TODO: Implement concrete notification channels (e.g., EmailChannel, SMSChannel)
//       for testing purposes, a simple channel that logs messages is sufficient.

// ─── Deposit Record ─────────────────────────────────────────────────────────
// HINT: Each deposit now needs a timestamp to support expiry.
//       Store the deposit time alongside the locker ID and package ID.

// ─── Locker System with Expiry ──────────────────────────────────────────────
// HINT: Extend your Part 1 system with:
//   - A configurable expiry duration (in hours)
//   - A depositPackage() that records the deposit time
//   - A checkExpired(currentTime) that scans active deposits
//   - Notification dispatch on deposit and expiry events

// TODO: Entry points:
//   void setCodeExpiry(int hours);
//   vector<string> checkExpired(long currentTime);
//   void addNotificationChannel(/* channel */);

// HINT: depositPackage() now takes an additional timestamp parameter:
//   string depositPackage(const string& packageId, LockerSize size, long depositTime);

// ─── Test Entry Points ───────────────────────────────────────────────────────
//   void addLocker(const string& lockerId, LockerSize size);
//   string depositPackage(const string& packageId, LockerSize size, long depositTime);
//   bool retrievePackage(const string& code);
//   void setCodeExpiry(int hours);
//   vector<string> checkExpired(long currentTime);
//   void addNotificationChannel(/* channel */);
// ─────────────────────────────────────────────────────────────────────────────

