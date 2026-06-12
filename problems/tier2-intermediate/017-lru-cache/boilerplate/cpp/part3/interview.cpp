#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

struct Node {
    int key;
    int value;
    long expiresAt;  // 0 means no TTL
    Node* prev;
    Node* next;
    Node(int k, int v, long exp = 0)
        : key(k), value(v), expiresAt(exp), prev(nullptr), next(nullptr) {}
};

enum class EvictionReason { CAPACITY, TTL_EXPIRED, EXPLICIT_DELETE };

// ─── Eviction Listener Interface ─────────────────────────────────────────────

class EvictionListener {
public:
    virtual void onEviction(int key, int value, EvictionReason reason) = 0;
    virtual ~EvictionListener() = default;
};

// ─── Your Design Starts Here ─────────────────────────────────────────────────
//
// Extend your LRU Cache from Parts 1-2 with eviction listeners:
//   1. Register callbacks notified on every eviction
//   2. Each notification includes the key, value, and reason for eviction
//   3. Support adding and removing listeners
//
// Eviction reasons:
//   CAPACITY       — evicted because cache was full
//   TTL_EXPIRED    — evicted because the entry's TTL expired
//   EXPLICIT_DELETE — evicted because deleteKey() was called
//
// Think about:
//   - Should listeners be notified before or after the entry is removed?
//   - What happens if a listener tries to call put() or get() during notification?
//   - How do you safely remove a listener while iterating?
//
// Entry points (in addition to Parts 1-2):
//   void addEvictionListener(EvictionListener* listener);
//   void removeEvictionListener(EvictionListener* listener);
//
// ─────────────────────────────────────────────────────────────────────────────

class LRUCache {
public:
    LRUCache(int capacity) {
        // TODO: Implement
    }

    void put(int key, int value, long currentTime, int ttl = 0) {
        // TODO: Implement
    }

    int get(int key, long currentTime) {
        // TODO: Implement
        return -1;
    }

    bool deleteKey(int key) {
        // TODO: Implement
        return false;
    }

    int size() {
        // TODO: Implement
        return 0;
    }

    void addEvictionListener(EvictionListener* listener) {
        // TODO: Implement
    }

    void removeEvictionListener(EvictionListener* listener) {
        // TODO: Implement
    }
};

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 3 — design and implement the class above." << endl;
    return 0;
}
#endif

