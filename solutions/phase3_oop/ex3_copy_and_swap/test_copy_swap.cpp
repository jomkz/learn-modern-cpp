// Phase 3, Exercise 3 — Copy-and-Swap tests

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "buffer.h"

TEST_CASE("copy assignment creates independent copy", "[copy-and-swap]") {
    Buffer a(8);
    std::memcpy(a.data(), "AAAAAAAA", 8);

    Buffer b(8);
    b = a;  // copy-and-swap path (copy ctor builds temp, then swap)

    // Verify deep copy: modifying b doesn't affect a.
    b.data()[0] = 'Z';
    REQUIRE(a.data()[0] == 'A');
    REQUIRE(b.data()[0] == 'Z');
}

TEST_CASE("self-assignment is safe", "[copy-and-swap]") {
    Buffer a(4);
    std::memcpy(a.data(), "TEST", 4);
    const void* original_ptr = a.data();

    // Self-assignment: copy-and-swap makes a copy then swaps.
    // The copy is destroyed, leaving the original intact.
    // No crash, no corruption.
    a = a;  // NOLINT(clang-diagnostic-self-assign-overloaded)

    REQUIRE(a.size() == 4);
    REQUIRE(std::memcmp(a.data(), "TEST", 4) == 0);
    // After self-copy-assign the pointer may differ (new allocation was made).
    // That's acceptable — the data is intact.
    (void)original_ptr;
}

TEST_CASE("move assignment transfers ownership", "[copy-and-swap]") {
    Buffer a(16);
    std::memcpy(a.data(), "move-assign-test", 16);
    const void* original_data = a.data();

    Buffer b(4);
    b = std::move(a);  // move-and-swap path (move ctor builds temp, then swap)

    REQUIRE(b.size() == 16);
    REQUIRE(b.data() == original_data);  // same heap block, no copy
    REQUIRE(a.size() == 0);              // moved-from is empty
}

TEST_CASE("swap exchanges contents", "[copy-and-swap]") {
    Buffer a(4);
    Buffer b(8);
    std::memcpy(a.data(), "AAAA", 4);
    std::memcpy(b.data(), "BBBBBBBB", 8);

    swap(a, b);  // ADL finds our friend swap()

    REQUIRE(a.size() == 8);
    REQUIRE(b.size() == 4);
    REQUIRE(std::memcmp(a.data(), "BBBBBBBB", 8) == 0);
    REQUIRE(std::memcmp(b.data(), "AAAA", 4) == 0);
}
