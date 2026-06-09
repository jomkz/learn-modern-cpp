// Phase 6, Exercise 2 — Move Semantics tests

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <vector>
#include "buffer.h"

TEST_CASE("move constructor transfers ownership", "[move]") {
    Buffer a(64);
    std::memset(a.data(), 0xAB, 64);
    const std::byte* original_ptr = a.data();

    // std::move is a cast to rvalue reference — the move constructor then
    // "steals" the pointer. No allocation occurs.
    Buffer b(std::move(a));

    REQUIRE(b.data() == original_ptr);  // same heap block, no copy
    REQUIRE(b.size() == 64);

    // a is in the moved-from state: safe to destroy, data_ is nullptr.
    REQUIRE(a.is_moved_from());
    REQUIRE(a.size() == 0);
}

TEST_CASE("move assignment transfers ownership", "[move]") {
    Buffer a(32);
    const std::byte* ptr = a.data();

    Buffer b(16);
    b = std::move(a);

    REQUIRE(b.data() == ptr);
    REQUIRE(b.size() == 32);
    REQUIRE(a.is_moved_from());
}

TEST_CASE("moved-from buffer is destructible", "[move]") {
    // A moved-from buffer must be safe to destroy. Its destructor does
    // delete[] nullptr, which is a no-op. We call the destructor explicitly
    // via placement-destroy to verify the moved-from state is safe to clean up.
    Buffer a(8);
    Buffer b(std::move(a));
    // Destroying the moved-from 'a' must not throw or crash.
    REQUIRE_NOTHROW(a.~Buffer());
    (void)b;
}

TEST_CASE("std::vector move-reallocation doesn't double-free", "[move]") {
    // When a std::vector grows beyond its capacity, it must move (or copy)
    // all existing elements into the new allocation. With noexcept move, it
    // moves; without, it copies. Run with ASan to confirm no double-free.
    std::vector<Buffer> v;
    for (int i = 1; i <= 5; ++i) {
        v.emplace_back(static_cast<std::size_t>(i * 64));
    }
    REQUIRE(v.size() == 5);
    for (const auto& buf : v) {
        REQUIRE(!buf.is_moved_from());
    }
}
