// Phase 6, Exercise 4 — Perfect Forwarding tests

#include <catch2/catch_test_macros.hpp>
#include <string>
#include "invoke.h"

// ── Test helpers ──────────────────────────────────────────────────────────────
static int add(int a, int b) { return a + b; }
static std::string concat(std::string a, std::string b) { return a + b; }

// Counts how many copies and moves have occurred in an object's history.
// Unlike a simple bool flag, counters propagate through the entire chain:
//   copy: copies++, moves carried over
//   move: moves++,  copies carried over
// This lets us verify the total number of each operation, regardless of how
// many subsequent moves the compiler applies (return-value optimisation, etc.).
struct MoveTracker {
    int copies = 0;
    int moves  = 0;
    MoveTracker() = default;
    MoveTracker(const MoveTracker& o) : copies(o.copies + 1), moves(o.moves) {}
    MoveTracker(MoveTracker&& o) noexcept : copies(o.copies), moves(o.moves + 1) {}
};

// Takes MoveTracker by value so a copy (lvalue arg) or move (rvalue arg) occurs
// at the call site. The return may add one more move; we test copies not moves.
static MoveTracker passthrough(MoveTracker t) { return t; }

// Returns a reference to a global int (tests decltype(auto) return).
static int g_value = 42;
static int& get_ref() { return g_value; }

// ── Tests ─────────────────────────────────────────────────────────────────────
TEST_CASE("invoke with regular function", "[forwarding]") {
    REQUIRE(invoke(add, 3, 4) == 7);
    REQUIRE(invoke(concat, std::string("hello"), std::string(" world")) == "hello world");
}

TEST_CASE("invoke with lambda", "[forwarding]") {
    auto square = [](int x) { return x * x; };
    REQUIRE(invoke(square, 5) == 25);
}

TEST_CASE("invoke preserves move semantics (rvalue forwarded, not copied)", "[forwarding]") {
    MoveTracker t;
    // std::move(t) creates an rvalue. Perfect forwarding passes it as rvalue ref,
    // so the move constructor is called inside passthrough — no copy occurs.
    auto result = invoke(passthrough, std::move(t));
    REQUIRE(result.copies == 0);   // no copy anywhere in the chain
}

TEST_CASE("invoke preserves copy semantics (lvalue forwarded as copy)", "[forwarding]") {
    MoveTracker t;
    // Passing t as an lvalue: perfect forwarding keeps it as an lvalue ref,
    // so the copy constructor fires when constructing the by-value parameter.
    // The return may add a move (compiler decides), but copies == 1.
    auto result = invoke(passthrough, t);
    REQUIRE(result.copies == 1);   // exactly one copy, at the call site
}

TEST_CASE("invoke preserves reference return type (decltype(auto))", "[forwarding]") {
    // get_ref() returns int& (a reference to g_value). If invoke returned 'auto'
    // instead of 'decltype(auto)', the reference would be stripped — we'd get
    // a copy, and modifying 'ref' wouldn't affect g_value.
    decltype(auto) ref = invoke(get_ref);
    ref = 99;  // modify through the reference
    REQUIRE(g_value == 99);  // confirms ref IS a reference, not a copy
}

TEST_CASE("invoke with zero arguments", "[forwarding]") {
    auto get_42 = []() { return 42; };
    REQUIRE(invoke(get_42) == 42);
}
