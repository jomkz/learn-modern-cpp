// Phase 5, Exercise 2 — TypedStack tests

#include <catch2/catch_test_macros.hpp>
#include "typed_stack.h"

// ── Generic template (int) ────────────────────────────────────────────────────
TEST_CASE("TypedStack<int> push/pop/top", "[typed_stack]") {
    TypedStack<int> s;
    REQUIRE(s.empty());

    s.push(10);
    s.push(20);
    s.push(30);

    REQUIRE(s.size() == 3);
    REQUIRE(s.top() == 30);

    s.pop();
    REQUIRE(s.top() == 20);

    s.pop();
    s.pop();
    REQUIRE(s.empty());
}

TEST_CASE("TypedStack<int> throws on underflow", "[typed_stack]") {
    TypedStack<int> s;
    REQUIRE_THROWS_AS(s.pop(), std::underflow_error);
    REQUIRE_THROWS_AS(s.top(), std::underflow_error);
}

// ── bool specialisation ───────────────────────────────────────────────────────
TEST_CASE("TypedStack<bool> push/pop/top", "[typed_stack][bool]") {
    TypedStack<bool> s;
    REQUIRE(s.empty());

    s.push(true);
    s.push(false);
    s.push(true);
    s.push(true);

    REQUIRE(s.size() == 4);
    REQUIRE(s.top() == true);

    s.pop();
    REQUIRE(s.top() == true);

    s.pop();
    REQUIRE(s.top() == false);
}

TEST_CASE("TypedStack<bool> alternating values", "[typed_stack][bool]") {
    TypedStack<bool> s;
    const bool sequence[] = {true, false, true, false, true};
    for (bool v : sequence) s.push(v);

    // Pop in reverse order and verify.
    for (int i = 4; i >= 0; --i) {
        REQUIRE(s.top() == sequence[i]);
        s.pop();
    }
    REQUIRE(s.empty());
}

TEST_CASE("TypedStack<bool> is smaller than TypedStack<int>", "[typed_stack][bool]") {
    // The whole point: bool specialisation is space-efficient.
    // TypedStack<bool> holds up to 64 booleans in a single uint64_t (8 bytes).
    // TypedStack<int> holds 64 ints in a vector (64*4 = 256 bytes + overhead).
    REQUIRE(sizeof(TypedStack<bool>) < sizeof(TypedStack<int>));
}

TEST_CASE("TypedStack<bool> throws on overflow at capacity", "[typed_stack][bool]") {
    TypedStack<bool> s;
    for (std::size_t i = 0; i < TypedStack<bool>::CAPACITY; ++i) {
        s.push(true);
    }
    REQUIRE_THROWS_AS(s.push(true), std::overflow_error);
}
