#include <iostream>
#include <unordered_map>
using namespace std;

// ─── Data Model (given — do not modify) ─────────────────────────────────────

struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// ─── Cache Interface ─────────────────────────────────────────────────────────
// HINT: You need two data structures working together:
//   - One for O(1) key lookup
//   - One for O(1) ordering (move-to-front, remove-from-tail)

class LRUCache {
    // HINT: Store key → Node* mapping for O(1) lookup
    // HINT: Use a doubly linked list for O(1) reordering
    // HINT: Sentinel head/tail nodes eliminate null-pointer edge cases

public:
    // TODO: Constructor — initialize with capacity, set up sentinel nodes
    LRUCache(int capacity) {
        // TODO: Initialize
    }

    // TODO: get(key) — return value if found, -1 otherwise
    //       HINT: On hit, move the node to the front (most recently used)
    int get(int key) {
        return -1;
    }

    // TODO: put(key, value) — insert or update
    //       HINT: If key exists, update value and move to front
    //       HINT: If at capacity, evict the node just before tail (least recently used)
    //       HINT: Always add new nodes right after head
    void put(int key, int value) {
        // TODO: Implement
    }

    // HINT: Helper methods you'll need:
    //   void addToFront(Node* node)  — insert right after head sentinel
    //   void removeNode(Node* node)  — unlink from current position
    //   void moveToFront(Node* node) — removeNode + addToFront
};

// ─── Test Entry Points ───────────────────────────────────────────────────────
//   LRUCache(int capacity);
//   int get(int key);
//   void put(int key, int value);
// ─────────────────────────────────────────────────────────────────────────────

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache — implement the methods above." << endl;
    return 0;
}
#endif

