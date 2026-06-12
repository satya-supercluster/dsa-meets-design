# Design Walkthrough — LRU Cache

> Only read after attempting the problem.

---

## HashMap + Doubly Linked List: the classic O(1) combo

```
LRUCache
    ├── unordered_map<int, Node*> cache     (O(1) key lookup)
    └── Doubly Linked List: head ↔ ... ↔ tail  (O(1) reorder + evict)

get(key):
    if key not in cache → return -1
    move node to front of list (most recently used)
    return node->value

put(key, value):
    if key in cache → update value, move to front
    else:
        if at capacity → evict tail node, remove from map
        create new node, add to front, insert into map
```

**Why this works:** The HashMap gives O(1) access to any node by key. The doubly linked list maintains recency order. Moving a node to the front or removing the tail are both O(1) pointer operations because each node has `prev` and `next` pointers.

**Sentinel nodes:** Using dummy head and tail nodes simplifies edge cases. You never need to check for null — every real node is always between head and tail.

```cpp
void addToFront(Node* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

void removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void moveToFront(Node* node) {
    removeNode(node);
    addToFront(node);
}
```

---

## TTL with lazy eviction

```cpp
int get(int key, long currentTime) {
    if (cache.find(key) == cache.end()) return -1;
    Node* node = cache[key];
    if (node->expiresAt > 0 && currentTime >= node->expiresAt) {
        // Expired — treat as miss
        removeNode(node);
        cache.erase(key);
        notifyListeners(key, node->value, EvictionReason::TTL_EXPIRED);
        delete node;
        return -1;
    }
    moveToFront(node);
    return node->value;
}
```

**Lazy vs eager eviction:** Lazy eviction (check on access) is simpler and avoids background threads. The tradeoff is that `size()` may overcount unless you eagerly scan — but in an interview, lazy is the right default.

---

## Eviction listener design

```cpp
void evictNode(Node* node, EvictionReason reason) {
    removeNode(node);
    cache.erase(node->key);
    for (auto* listener : listeners) {
        listener->onEviction(node->key, node->value, reason);
    }
    delete node;
}
```

**Re-entrancy guard:** If a listener calls `put()` or `get()` during notification, it could invalidate iterators or cause infinite recursion. The simplest guard is a boolean flag:

```cpp
bool notifying = false;

void evictNode(Node* node, EvictionReason reason) {
    // ... remove node ...
    notifying = true;
    for (auto* listener : listeners) {
        listener->onEviction(node->key, node->value, reason);
    }
    notifying = false;
}
```

Any cache-modifying operation checks `notifying` and either queues the operation or throws.

---

## Interview follow-ups

- *"How would you make this thread-safe?"* → Reader-writer lock: multiple concurrent `get()` calls with shared lock, exclusive lock for `put()` and eviction. Or use a concurrent hash map with per-bucket locks.
- *"How would you switch to LFU eviction?"* → Strategy pattern: extract the eviction policy into an interface. LRU uses a linked list; LFU uses a frequency map + min-heap.
- *"How would you handle distributed caching?"* → Consistent hashing to shard keys across nodes. Each node runs an independent LRU cache. Invalidation via pub/sub.
- *"Why not just use a sorted map by access time?"* → O(log n) operations instead of O(1). The linked list is specifically chosen because reordering is O(1).
