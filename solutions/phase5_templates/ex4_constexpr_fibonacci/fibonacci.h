// Phase 5, Exercise 4 — constexpr Fibonacci
// Concept: constexpr functions, compile-time computation, static_assert.
// Demonstrates: constexpr, consteval, static_assert, compile vs runtime eval.
//
// constexpr (C++11): "can be evaluated at compile time if called in a
//   constant expression context." May also be evaluated at runtime.
// consteval (C++20): "MUST be evaluated at compile time." Calling at runtime
//   is a compile error.
//
// static_assert is evaluated at compile time and fails the build if false.
// It's the compile-time counterpart of REQUIRE in tests.

#pragma once

#include <cstdint>    // uint64_t

// ── Iterative implementation ──────────────────────────────────────────────────
// We use an iterative loop rather than the classic recursive definition
//   fib(n) = fib(n-1) + fib(n-2)
// because the recursive version has exponential time complexity and quickly
// overflows the constexpr evaluation depth limit for large n.
// The iterative version is O(n) and evaluates correctly at compile time.
constexpr uint64_t fibonacci(int n) {
    if (n < 0) return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        uint64_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// ── Compile-time verification with static_assert ──────────────────────────────
// These fire during compilation, not at runtime. If any value is wrong,
// the build fails with a clear message like:
//   static_assert failed: "fibonacci(10) must be 55"
// This zero-cost "always-on" invariant checking is one of constexpr's superpowers.
static_assert(fibonacci(0)  ==          0, "fibonacci(0) must be 0");
static_assert(fibonacci(1)  ==          1, "fibonacci(1) must be 1");
static_assert(fibonacci(10) ==         55, "fibonacci(10) must be 55");
static_assert(fibonacci(20) ==       6765, "fibonacci(20) must be 6765");
static_assert(fibonacci(50) == 12586269025ULL, "fibonacci(50) must be 12586269025");

// ── consteval variant (C++20) ─────────────────────────────────────────────────
// fibonacci_ct is guaranteed to run at compile time. You cannot call it with
// a runtime variable — the compiler will reject it. Useful for template
// parameters, array sizes, and switch case values.
consteval uint64_t fibonacci_ct(int n) {
    return fibonacci(n);  // reuses the constexpr implementation
}
