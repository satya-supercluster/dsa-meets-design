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

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;

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
        // TODO: Return true if node has a TTL and currentTime >= expiresAt
        //       Return false if expiresAt == 0 (no TTL)
        return false;
    }

    void evictNode(Node* node) {
        // TODO: Remove node from list, erase from cache map, delete node
    }

    void evictLRU() {
        // TODO: Evict the node just before tail sentinel
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
        // TODO: Calculate expiresAt: if ttl > 0 then currentTime + ttl, else 0
        //       If key exists in cache:
        //         If expired, remove it first
        //         Else update value, expiresAt, and move to front; return
        //       If at capacity, evict LRU
        //       Create new node with expiresAt, add to front, insert into map
    }

    int get(int key, long currentTime) {
        // TODO: If key not in cache, return -1
        //       If key is expired, evict it and return -1
        //       Otherwise move to front and return value
        return -1;
    }

    bool deleteKey(int key) {
        // TODO: If key not in cache, return false
        //       Remove node from list and map, delete node, return true
        return false;
    }

    int size() {
        // TODO: Return the number of entries in the cache map
        //       Note: this is approximate — may include expired entries not yet cleaned
        return 0;
    }
};

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache Part 2 — implement the TODO methods above." << endl;
    return 0;
}
#endif
