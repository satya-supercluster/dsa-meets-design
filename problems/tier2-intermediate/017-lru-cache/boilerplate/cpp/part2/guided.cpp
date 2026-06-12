#include <iostream>
#include <unordered_map>
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

// ─── LRU Cache with TTL ─────────────────────────────────────────────────────
// HINT: Extend your Part 1 cache with expiration tracking.

class LRUCache {
    // HINT: Same HashMap + Doubly Linked List as Part 1
    // HINT: Each node now has an expiresAt field
    // HINT: On get(), check if the node has expired before returning

public:
    // TODO: Constructor — same as Part 1
    LRUCache(int capacity) {
        // TODO: Initialize
    }

    // TODO: put(key, value, currentTime, ttl)
    //       HINT: If ttl > 0, set expiresAt = currentTime + ttl
    //       HINT: If ttl == 0, set expiresAt = 0 (never expires)
    //       HINT: If key exists but is expired, treat as new insertion
    void put(int key, int value, long currentTime, int ttl = 0) {
        // TODO: Implement
    }

    // TODO: get(key, currentTime)
    //       HINT: If key exists but expired, remove it and return -1
    //       HINT: If key exists and valid, move to front and return value
    int get(int key, long currentTime) {
        return -1;
    }

    // TODO: deleteKey(key)
    //       HINT: Remove from both map and list. Return true if key existed.
    bool deleteKey(int key) {
        return false;
    }

    // TODO: size()
    //       HINT: Return cache.size() — note this may include expired entries
    //       that haven't been lazily cleaned up yet. For exact count, you'd
    //       need to scan. Choose one approach and justify it.
    int size() {
        return 0;
    }
};

// ─── Test Entry Points ───────────────────────────────────────────────────────
//   LRUCache(int capacity);
//   void put(int key, int value, long currentTime, int ttl = 0);
//   int get(int key, long currentTime);
//   bool deleteKey(int key);
//   int size();
// ─────────────────────────────────────────────────────────────────────────────

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 2 — implement the methods above." << endl;
    return 0;
}
#endif

