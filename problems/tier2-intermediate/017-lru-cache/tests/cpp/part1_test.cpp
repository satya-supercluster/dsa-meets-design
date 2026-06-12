// Part 1 Tests — LRU Cache
// Tests basic get/put operations and LRU eviction

#include "solution.cpp"
#include <cassert>
#include <iostream>
using namespace std;

int part1_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: basic put and get
    try {
        LRUCache cache(2);
        cache.put(1, 10);
        cache.put(2, 20);
        assert(cache.get(1) == 10);
        assert(cache.get(2) == 20);
        cout << "PASS test_basic_put_get" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_basic_put_get" << endl;
        failed++;
    }

    // Test 2: get non-existent key returns -1
    try {
        LRUCache cache(2);
        assert(cache.get(99) == -1);
        cache.put(1, 10);
        assert(cache.get(2) == -1);
        cout << "PASS test_get_nonexistent" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_get_nonexistent" << endl;
        failed++;
    }

    // Test 3: evicts least recently used when capacity exceeded
    try {
        LRUCache cache(2);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(3, 30);  // evicts key 1 (LRU)
        assert(cache.get(1) == -1);  // evicted
        assert(cache.get(2) == 20);
        assert(cache.get(3) == 30);
        cout << "PASS test_eviction_on_capacity" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_eviction_on_capacity" << endl;
        failed++;
    }

    // Test 4: get() updates recency — prevents eviction
    try {
        LRUCache cache(2);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.get(1);       // key 1 is now most recent
        cache.put(3, 30);   // evicts key 2 (now LRU)
        assert(cache.get(1) == 10);  // still present
        assert(cache.get(2) == -1);  // evicted
        assert(cache.get(3) == 30);
        cout << "PASS test_get_updates_recency" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_get_updates_recency" << endl;
        failed++;
    }

    // Test 5: put() updates existing key value
    try {
        LRUCache cache(2);
        cache.put(1, 10);
        cache.put(1, 100);  // update value
        assert(cache.get(1) == 100);
        cout << "PASS test_put_updates_value" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_put_updates_value" << endl;
        failed++;
    }

    // Test 6: put() on existing key updates recency
    try {
        LRUCache cache(2);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(1, 100);  // key 1 updated — now most recent
        cache.put(3, 30);   // evicts key 2 (now LRU)
        assert(cache.get(1) == 100);
        assert(cache.get(2) == -1);  // evicted
        assert(cache.get(3) == 30);
        cout << "PASS test_put_updates_recency" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_put_updates_recency" << endl;
        failed++;
    }

    // Test 7: capacity-1 cache
    try {
        LRUCache cache(1);
        cache.put(1, 10);
        assert(cache.get(1) == 10);
        cache.put(2, 20);   // evicts key 1
        assert(cache.get(1) == -1);
        assert(cache.get(2) == 20);
        cout << "PASS test_capacity_one" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_capacity_one" << endl;
        failed++;
    }

    // Test 8: multiple evictions in sequence
    try {
        LRUCache cache(3);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(3, 30);
        cache.put(4, 40);  // evicts 1
        cache.put(5, 50);  // evicts 2
        assert(cache.get(1) == -1);
        assert(cache.get(2) == -1);
        assert(cache.get(3) == 30);
        assert(cache.get(4) == 40);
        assert(cache.get(5) == 50);
        cout << "PASS test_multiple_evictions" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_multiple_evictions" << endl;
        failed++;
    }

    cout << "PART1_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
