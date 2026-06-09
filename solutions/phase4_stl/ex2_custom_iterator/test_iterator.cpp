// Phase 4, Exercise 2 — Custom Bidirectional Iterator tests

#include <catch2/catch_test_macros.hpp>
#include <algorithm>  // std::reverse, std::find, std::distance
#include <numeric>    // std::accumulate
#include <vector>
#include "linked_list.h"

TEST_CASE("forward iteration", "[iterator]") {
    DoublyLinkedList<int> list;
    for (int i = 1; i <= 5; ++i) list.push_back(i);

    std::vector<int> result;
    for (auto v : list) result.push_back(v);  // range-based for uses begin/end

    REQUIRE(result == std::vector<int>{1, 2, 3, 4, 5});
}

TEST_CASE("std::distance works (counts nodes)", "[iterator]") {
    DoublyLinkedList<int> list;
    for (int i = 0; i < 10; ++i) list.push_back(i);

    // std::distance advances begin to end, counting steps.
    // For bidirectional iterators it runs in O(n); for random-access O(1).
    REQUIRE(std::distance(list.begin(), list.end()) == 10);
}

TEST_CASE("std::accumulate sums values", "[iterator]") {
    DoublyLinkedList<int> list;
    for (int i = 1; i <= 5; ++i) list.push_back(i);

    int sum = std::accumulate(list.begin(), list.end(), 0);
    REQUIRE(sum == 15);
}

TEST_CASE("std::find locates element", "[iterator]") {
    DoublyLinkedList<int> list;
    for (int i = 1; i <= 5; ++i) list.push_back(i);

    auto it = std::find(list.begin(), list.end(), 3);
    REQUIRE(it != list.end());
    REQUIRE(*it == 3);
}

TEST_CASE("std::reverse reverses the list in-place", "[iterator]") {
    // std::reverse requires bidirectional iterators and uses swap through them.
    // If our iterator doesn't satisfy std::bidirectional_iterator, this fails
    // to compile (or the static_assert in linked_list.h fires first).
    DoublyLinkedList<int> list;
    for (int i = 1; i <= 5; ++i) list.push_back(i);

    std::reverse(list.begin(), list.end());

    std::vector<int> result;
    for (auto v : list) result.push_back(v);
    REQUIRE(result == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("iterator concept is satisfied", "[iterator]") {
    // Verify at runtime (and compile time via static_assert in the header).
    REQUIRE(std::bidirectional_iterator<DoublyLinkedList<int>::Iterator>);
}
