#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

struct Node {
    int key;
    int value;
    long expiresAt;
    Node* prev;
    Node* next;
    Node(int k, int v, long exp = 0)
        : key(k), value(v), expiresAt(exp), prev(nullptr), next(nullptr) {}
};

enum class EvictionReason { CAPACITY, TTL_EXPIRED, EXPLICIT_DELETE };

// ─── Eviction Listener Interface ─────────────────────────────────────────────
// HINT: Listeners are notified whenever an entry is evicted, regardless of reason.
// They receive the key, value, and why the eviction happened.

class EvictionListener {
public:
    virtual void onEviction(int key, int value, EvictionReason reason) = 0;
    virtual ~EvictionListener() = default;
};

// ─── LRU Cache with Listeners ───────────────────────────────────────────────
// HINT: Extend your Part 2 cache with a list of eviction listeners.

class LRUCache {
    // TODO: Your cache class should:
    //   - Store a vector of listener pointers
    //   - Call all listeners when evicting (capacity, TTL, explicit delete)
    //   - Support addEvictionListener() and removeEvictionListener()

    // HINT: When evicting a node, notify listeners BEFORE deleting the node
    //       so they can read the key and value.

    // HINT: To prevent re-entrancy issues, consider adding a boolean guard
    //       that blocks cache modifications during listener notification.

public:
    LRUCache(int capacity) {
        // TODO: Initialize
    }

    void put(int key, int value, long currentTime, int ttl = 0) {
        // TODO: Implement
    }

    int get(int key, long currentTime) {
        return -1;
    }

    bool deleteKey(int key) {
        return false;
    }

    int size() {
        return 0;
    }

    void addEvictionListener(EvictionListener* listener) {
        // TODO: Add listener to the list
    }

    void removeEvictionListener(EvictionListener* listener) {
        // TODO: Find and remove listener from the list
        //       Use std::find + erase
    }
};

// ─── Test Entry Points ───────────────────────────────────────────────────────
//   LRUCache(int capacity);
//   void put(int key, int value, long currentTime, int ttl = 0);
//   int get(int key, long currentTime);
//   bool deleteKey(int key);
//   int size();
//   void addEvictionListener(EvictionListener* listener);
//   void removeEvictionListener(EvictionListener* listener);
// ─────────────────────────────────────────────────────────────────────────────

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 3 — implement the methods above." << endl;
    return 0;
}
#endif

