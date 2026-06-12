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

// ─── LRU Cache ──────────────────────────────────────────────────────────────

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;  // key → Node*
    Node* head;  // sentinel — most recently used side
    Node* tail;  // sentinel — least recently used side

    void addToFront(Node* node) {
        // TODO: Insert node right after head sentinel
        //       node->next = head->next
        //       node->prev = head
        //       head->next->prev = node
        //       head->next = node
    }

    void removeNode(Node* node) {
        // TODO: Unlink node from its current position
        //       node->prev->next = node->next
        //       node->next->prev = node->prev
    }

    void moveToFront(Node* node) {
        // TODO: Remove from current position, then add to front
    }

    void evictLRU() {
        // TODO: The LRU node is tail->prev (just before the tail sentinel)
        //       Remove it from the list
        //       Remove it from the cache map
        //       Delete the node
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

    int get(int key) {
        // TODO: If key not in cache, return -1
        //       Otherwise, move node to front and return its value
        return -1;
    }

    void put(int key, int value) {
        // TODO: If key already in cache, update value and move to front
        //       If at capacity, evict LRU entry
        //       Create new node, add to front, insert into cache map
    }
};

#ifndef RUNNING_TESTS
int main() {
    cout << "LRU Cache — implement the TODO methods above." << endl;
    return 0;
}
#endif
