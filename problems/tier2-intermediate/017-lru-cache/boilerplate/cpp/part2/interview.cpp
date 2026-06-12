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

// ─── Your Design Starts Here ─────────────────────────────────────────────────
//
// Extend your LRU Cache from Part 1 with:
//   1. TTL (time-to-live) per entry — expired entries treated as non-existent
//   2. delete(key) — explicitly remove an entry
//   3. size() — return number of non-expired entries
//
// Think about:
//   - Should expired entries be evicted eagerly or lazily?
//   - How does TTL interact with LRU eviction order?
//   - What happens when put() is called with a key that has expired?
//
// Entry points:
//   LRUCache(int capacity);
//   void put(int key, int value, long currentTime, int ttl = 0);
//   int get(int key, long currentTime);  // returns -1 if not found or expired
//   bool deleteKey(int key);
//   int size();
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
};

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 2 — design and implement the class above." << endl;
    return 0;
}
#endif

