# Problem 017 — LRU Cache

**Tier:** 2 (Intermediate) | **Patterns:** Singleton, Observer, Strategy | **DSA:** HashMap, Doubly Linked List
**Companies:** Kutumb | **Time:** 60 minutes

---

## Problem Statement

Build a Least Recently Used (LRU) Cache with O(1) time complexity for both `get` and `put` operations. The system must:

1. Store key-value pairs with a fixed capacity
2. Evict the **least recently used** entry when capacity is exceeded
3. Support TTL (time-to-live) per entry
4. Notify registered listeners when entries are evicted

---

## Before You Code

**The naive approach:** Use a vector of key-value pairs. On every `get`, scan the vector to find the key (O(n)). On every `put`, scan to check if key exists, then shift elements to maintain order (O(n)). Eviction is O(1) from the front, but lookups kill performance.

**The pattern approach:**
- **HashMap + Doubly Linked List**: The core DSA insight. HashMap gives O(1) lookup by key. Doubly Linked List gives O(1) move-to-front and O(1) eviction from the tail. Together, every operation is O(1).
- **Singleton**: The cache is a shared resource — in production, you want exactly one instance per cache namespace. The Singleton pattern ensures this.
- **Observer**: Eviction events (due to capacity overflow or TTL expiry) need to reach multiple listeners — logging, metrics, backup systems. Observer decouples the cache from its consumers.
- **Strategy**: The eviction policy itself is a strategy. LRU is one policy; LFU, FIFO, or random eviction are others. Making the policy swappable keeps the cache core generic.

**The DSA angle:** The HashMap stores `key → Node*` pointers into the doubly linked list. Moving a node to the front is O(1) pointer manipulation. Evicting the tail node is O(1). This is the classic interview data structure combination.

---

## Data Structures

```cpp
struct Node {
    int key;
    int value;
    long expiresAt;  // 0 means no TTL
    Node* prev;
    Node* next;
};
```

---

## Part 1

**Base requirement — LRU Cache with O(1) operations**

Implement an LRU Cache that supports `get` and `put` in O(1) time.

**Rules:**
- `get(key)` — Return the value if the key exists, otherwise return -1. Mark the key as recently used.
- `put(key, value)` — Insert or update the key-value pair. If the cache exceeds capacity, evict the least recently used entry.
- Both operations must be O(1) average time.

**Entry points (tests will call these):**
```cpp
LRUCache(int capacity);
int get(int key);
void put(int key, int value);
```

**What to implement:**
```cpp
class LRUCache {
    int capacity;
    unordered_map<int, Node*> cache;  // key → Node*
    Node* head;  // most recently used (sentinel)
    Node* tail;  // least recently used (sentinel)

    void addToFront(Node* node);
    void removeNode(Node* node);
    void moveToFront(Node* node);
    void evictLRU();
public:
    LRUCache(int capacity);
    int get(int key);
    void put(int key, int value);
};
```

**Key insight:** Use sentinel head and tail nodes to avoid null-pointer edge cases in linked list operations.

---

## Part 2

**Extension 1 — TTL support**

Each entry can now have an optional time-to-live (TTL). Expired entries are treated as non-existent.

**Rules:**
- `put(key, value, ttl)` — Set TTL in seconds. The entry expires at `currentTime + ttl`. If `ttl` is 0, the entry never expires.
- `get(key)` now takes a `currentTime` parameter. If the entry has expired, remove it and return -1.
- `delete(key)` — Explicitly remove an entry. Return true if the key existed, false otherwise.
- `size()` — Return the number of non-expired entries.

**New entry points:**
```cpp
void put(int key, int value, long currentTime, int ttl = 0);
int get(int key, long currentTime);
bool deleteKey(int key);
int size();
```

**Design challenge:** Expired entries are lazily evicted (only on access). This means `size()` must either eagerly clean up or accept approximate counts. Choose a strategy and justify it.

---

## Part 3

**Extension 2 — Eviction listeners**

Register callback listeners that are notified whenever an entry is evicted — whether due to capacity overflow, TTL expiry, or explicit deletion.

**Listener interface:**
```cpp
enum class EvictionReason { CAPACITY, TTL_EXPIRED, EXPLICIT_DELETE };

class EvictionListener {
public:
    virtual void onEviction(int key, int value, EvictionReason reason) = 0;
    virtual ~EvictionListener() = default;
};
```

**New entry points:**
```cpp
void addEvictionListener(EvictionListener* listener);
void removeEvictionListener(EvictionListener* listener);
```

**Rules:**
- Listeners are notified synchronously in registration order
- Capacity eviction: notified with `EvictionReason::CAPACITY`
- TTL expiry: notified with `EvictionReason::TTL_EXPIRED`
- Explicit delete: notified with `EvictionReason::EXPLICIT_DELETE`

**Design challenge:** Listeners should not be able to modify the cache during notification (no re-entrant calls). How do you prevent or handle this?

---

## Running Tests

```bash
./run-tests.sh 017-lru-cache cpp
```
