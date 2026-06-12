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

class NotificationChannel {
public:
    virtual void notify(const string& packageId, const string& message) = 0;
    virtual ~NotificationChannel() = default;
};

// ─── Your Design Starts Here ─────────────────────────────────────────────────
//
// Extend your locker system from Part 1 with:
//   1. Pickup code expiry — codes expire after N hours
//   2. Expired package detection — checkExpired() scans and frees lockers
//   3. Notification channels — notify on deposit and on expiry
//
// Think about:
//   - How do you add expiry without modifying core deposit/retrieve logic?
//   - How do you support multiple notification channels (email, SMS, push)?
//   - What data do you need to track per deposit for expiry to work?
//
// Entry points (in addition to Part 1):
//   void setCodeExpiry(int hours);
//   vector<string> checkExpired(long currentTime);
//   void addNotificationChannel(NotificationChannel* channel);
//
// ─────────────────────────────────────────────────────────────────────────────

