// Part 3 Tests — Eviction Listeners
// Tests that eviction events fire with correct reason and data

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

// ─── Listener spy for testing ────────────────────────────────────────────────

struct EvictionEvent {
    int key;
    int value;
    EvictionReason reason;
};

class TestEvictionListener : public EvictionListener {
public:
    vector<EvictionEvent> events;

    void onEviction(int key, int value, EvictionReason reason) override {
        events.push_back({key, value, reason});
    }

    void clear() { events.clear(); }
};

int part3_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: capacity eviction fires listener
    try {
        LRUCache cache(2);
        TestEvictionListener listener;
        cache.addEvictionListener(&listener);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);
        cache.put(3, 30, 1000);  // evicts key 1
        assert(listener.events.size() == 1);
        assert(listener.events[0].key == 1);
        assert(listener.events[0].value == 10);
        assert(listener.events[0].reason == EvictionReason::CAPACITY);
        cout << "PASS test_capacity_eviction_listener" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_capacity_eviction_listener" << endl;
        failed++;
    }

    // Test 2: TTL expiry fires listener
    try {
        LRUCache cache(5);
        TestEvictionListener listener;
        cache.addEvictionListener(&listener);
        cache.put(1, 10, 1000, 30);  // expires at 1030
        cache.get(1, 1035);           // triggers TTL eviction
        assert(listener.events.size() == 1);
        assert(listener.events[0].key == 1);
        assert(listener.events[0].reason == EvictionReason::TTL_EXPIRED);
        cout << "PASS test_ttl_eviction_listener" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_ttl_eviction_listener" << endl;
        failed++;
    }

    // Test 3: explicit delete fires listener
    try {
        LRUCache cache(5);
        TestEvictionListener listener;
        cache.addEvictionListener(&listener);
        cache.put(1, 10, 1000);
        cache.deleteKey(1);
        assert(listener.events.size() == 1);
        assert(listener.events[0].key == 1);
        assert(listener.events[0].value == 10);
        assert(listener.events[0].reason == EvictionReason::EXPLICIT_DELETE);
        cout << "PASS test_explicit_delete_listener" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_explicit_delete_listener" << endl;
        failed++;
    }

    // Test 4: multiple listeners all notified
    try {
        LRUCache cache(2);
        TestEvictionListener listener1;
        TestEvictionListener listener2;
        cache.addEvictionListener(&listener1);
        cache.addEvictionListener(&listener2);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);
        cache.put(3, 30, 1000);  // evicts key 1
        assert(listener1.events.size() == 1);
        assert(listener2.events.size() == 1);
        assert(listener1.events[0].key == 1);
        assert(listener2.events[0].key == 1);
        cout << "PASS test_multiple_listeners" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_multiple_listeners" << endl;
        failed++;
    }

    // Test 5: remove listener stops notifications
    try {
        LRUCache cache(2);
        TestEvictionListener listener;
        cache.addEvictionListener(&listener);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);
        cache.put(3, 30, 1000);  // evicts key 1, listener notified
        assert(listener.events.size() == 1);
        cache.removeEvictionListener(&listener);
        cache.put(4, 40, 1000);  // evicts key 2, listener NOT notified
        assert(listener.events.size() == 1);  // still 1, not 2
        cout << "PASS test_remove_listener" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_remove_listener" << endl;
        failed++;
    }

    // Test 6: no listeners registered — no crash
    try {
        LRUCache cache(1);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);  // evicts key 1, no listeners
        assert(cache.get(2, 1000) == 20);
        cout << "PASS test_no_listeners_no_crash" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_listeners_no_crash" << endl;
        failed++;
    }

    // Test 7: listener receives correct values on multiple evictions
    try {
        LRUCache cache(2);
        TestEvictionListener listener;
        cache.addEvictionListener(&listener);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);
        cache.put(3, 30, 1000);  // evicts 1
        cache.put(4, 40, 1000);  // evicts 2
        assert(listener.events.size() == 2);
        assert(listener.events[0].key == 1);
        assert(listener.events[0].value == 10);
        assert(listener.events[1].key == 2);
        assert(listener.events[1].value == 20);
        cout << "PASS test_sequential_evictions" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_sequential_evictions" << endl;
        failed++;
    }

    cout << "PART3_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
