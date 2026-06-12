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

class EvictionListener {
public:
    virtual void onEviction(int key, int value, EvictionReason reason) = 0;
    virtual ~EvictionListener() = default;
};

// ─── LRU Cache with TTL and Eviction Listeners ─────────────────────────────

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;
    vector<EvictionListener*> listeners;
    bool notifying = false;  // re-entrancy guard

    void addToFront(Node* node) {
        // TODO: Insert node right after head sentinel
    }

    void removeNode(Node* node) {
        // TODO: Unlink node from its current position
    }

    void moveToFront(Node* node) {
        // TODO: removeNode + addToFront
    }

    bool isExpired(Node* node, long currentTime) {
        // TODO: Check if node has expired (expiresAt > 0 && currentTime >= expiresAt)
        return false;
    }

    void notifyListeners(int key, int value, EvictionReason reason) {
        // TODO: Set notifying = true
        //       Iterate over all listeners and call onEviction()
        //       Set notifying = false
    }

    void evictNode(Node* node, EvictionReason reason) {
        // TODO: Remove node from list
        //       Remove from cache map
        //       Notify listeners (BEFORE deleting the node)
        //       Delete node
    }

    void evictLRU() {
        // TODO: Get tail->prev, evict with reason CAPACITY
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void put(int key, int value, long currentTime, int ttl = 0) {
        // TODO: Calculate expiresAt
        //       If key exists and not expired, update and move to front
        //       If key exists but expired, evict with TTL_EXPIRED reason
        //       If at capacity, evict LRU
        //       Create new node, add to front, insert into map
    }

    int get(int key, long currentTime) {
        // TODO: If not found, return -1
        //       If expired, evict with TTL_EXPIRED and return -1
        //       Move to front, return value
        return -1;
    }

    bool deleteKey(int key) {
        // TODO: If not found, return false
        //       Evict with EXPLICIT_DELETE reason, return true
        return false;
    }

    int size() {
        // TODO: Return cache.size()
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

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 3 — implement the TODO methods above." << endl;
    return 0;
}
#endif
