// Part 2 Tests — LRU Cache with TTL
// Tests TTL expiry, delete, and size operations

#include <cassert>
#include <iostream>
using namespace std;

int part2_tests() {
    int passed = 0;
    int failed = 0;

    // Test 1: TTL entry expires after time
    try {
        LRUCache cache(5);
        cache.put(1, 10, 1000, 60);  // expires at 1060
        assert(cache.get(1, 1030) == 10);  // still valid
        assert(cache.get(1, 1061) == -1);  // expired
        cout << "PASS test_ttl_expiry" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_ttl_expiry" << endl;
        failed++;
    }

    // Test 2: no TTL means never expires
    try {
        LRUCache cache(5);
        cache.put(1, 10, 1000, 0);  // no TTL
        assert(cache.get(1, 999999) == 10);  // never expires
        cout << "PASS test_no_ttl_never_expires" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_no_ttl_never_expires" << endl;
        failed++;
    }

    // Test 3: delete existing key
    try {
        LRUCache cache(5);
        cache.put(1, 10, 1000);
        assert(cache.deleteKey(1) == true);
        assert(cache.get(1, 1001) == -1);
        cout << "PASS test_delete_existing" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_delete_existing" << endl;
        failed++;
    }

    // Test 4: delete non-existent key
    try {
        LRUCache cache(5);
        assert(cache.deleteKey(99) == false);
        cout << "PASS test_delete_nonexistent" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_delete_nonexistent" << endl;
        failed++;
    }

    // Test 5: size tracks entries
    try {
        LRUCache cache(5);
        assert(cache.size() == 0);
        cache.put(1, 10, 1000);
        cache.put(2, 20, 1000);
        assert(cache.size() == 2);
        cache.deleteKey(1);
        assert(cache.size() == 1);
        cout << "PASS test_size_tracking" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_size_tracking" << endl;
        failed++;
    }

    // Test 6: expired entry is lazily removed on get
    try {
        LRUCache cache(5);
        cache.put(1, 10, 1000, 30);  // expires at 1030
        cache.put(2, 20, 1000, 60);  // expires at 1060
        // Access after key 1 expired but before key 2
        assert(cache.get(1, 1035) == -1);  // expired, removed
        assert(cache.get(2, 1035) == 20);  // still valid
        cout << "PASS test_lazy_expiry_on_get" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_lazy_expiry_on_get" << endl;
        failed++;
    }

    // Test 7: put with same key refreshes TTL
    try {
        LRUCache cache(5);
        cache.put(1, 10, 1000, 30);  // expires at 1030
        cache.put(1, 20, 1020, 30);  // new TTL, expires at 1050
        assert(cache.get(1, 1035) == 20);  // still valid (new TTL)
        assert(cache.get(1, 1051) == -1);  // now expired
        cout << "PASS test_put_refreshes_ttl" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_put_refreshes_ttl" << endl;
        failed++;
    }

    // Test 8: expired entries don't block capacity eviction
    try {
        LRUCache cache(2);
        cache.put(1, 10, 1000, 10);  // expires at 1010
        cache.put(2, 20, 1000, 10);  // expires at 1010
        // Both expired — put new entries
        cache.put(3, 30, 1020);  // should evict expired key 1
        cache.put(4, 40, 1020);  // should evict expired key 2
        assert(cache.get(3, 1020) == 30);
        assert(cache.get(4, 1020) == 40);
        cout << "PASS test_capacity_with_expired" << endl;
        passed++;
    } catch (...) {
        cout << "FAIL test_capacity_with_expired" << endl;
        failed++;
    }

    cout << "PART2_SUMMARY " << passed << "/" << (passed + failed) << endl;
    return failed;
}
