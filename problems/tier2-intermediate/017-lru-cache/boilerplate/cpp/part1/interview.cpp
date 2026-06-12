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

// ─── Your Design Starts Here ─────────────────────────────────────────────────
//
// Design and implement an LRU Cache that:
//   1. Stores key-value pairs with a fixed capacity
//   2. Supports get(key) and put(key, value) in O(1) time
//   3. Evicts the least recently used entry when capacity is exceeded
//
// Think about:
//   - What combination of data structures gives O(1) for both lookup and reorder?
//   - How do you avoid null-pointer edge cases in linked list operations?
//   - How would you swap LRU for a different eviction policy (LFU, FIFO)?
//
// Entry points:
//   LRUCache(int capacity);
//   int get(int key);         // returns -1 if not found
//   void put(int key, int value);
//
// ─────────────────────────────────────────────────────────────────────────────

class LRUCache {
public:
    LRUCache(int capacity) {
        // TODO: Implement
    }

    int get(int key) {
        // TODO: Implement
        return -1;
    }

    void put(int key, int value) {
        // TODO: Implement
    }
};

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache — design and implement the class above." << endl;
    return 0;
}
#endif

