// Phase 6, Exercise 3 — Lambda factory and std::function
// Concept: Lambdas, capture clauses, std::function, type erasure.
// Demonstrates: [n] capture by value, std::function<int(int)>, closure lifetime.
//
// A lambda is a syntactic shorthand for an anonymous function object (functor).
// Capture clauses copy or reference outer variables into the lambda's closure:
//   [=]  — capture all visible variables by value (snapshot at creation time)
//   [&]  — capture all by reference (DANGEROUS for returned lambdas — dangling ref)
//   [n]  — capture only 'n' by value
//   [&n] — capture only 'n' by reference
//
// For lambdas returned from functions or stored in containers, ALWAYS capture
// by value (or use smart pointers). Capturing by reference in a returned lambda
// leaves a dangling reference when the enclosing scope ends.
//
// std::function<Ret(Args...)> is a type-erased callable wrapper. It can hold:
//   - Lambdas
//   - Function pointers
//   - Member function pointers (via std::bind or a capturing lambda)
//   - Functors (objects with operator())
//
// Cost: std::function involves a virtual-dispatch-like indirection and may
// heap-allocate for large captures. For performance-critical code, prefer
// template parameters over std::function to avoid this overhead.

#pragma once

#include <functional>  // std::function

// Returns a lambda that adds 'n' to its argument.
// The lambda captures 'n' BY VALUE — it gets its own copy of n.
// This is safe to return and store because the closure owns its data.
inline std::function<int(int)> make_adder(int n) {
    // [n]: capture n by value. The lambda stores n as a member variable
    // of its anonymous closure type.
    return [n](int x) { return x + n; };
}

// ── Why [n] instead of [&n]? ──────────────────────────────────────────────────
// If we wrote [&n], the lambda would hold a reference to the local variable 'n'
// in make_adder's stack frame. After make_adder returns, that frame is gone —
// accessing the reference in the returned lambda is undefined behaviour.
//
// With [n] (by value), the lambda gets its own copy of n at construction time.
// The copy lives inside the closure object, which lives wherever the lambda is
// stored — as long as the caller keeps it alive.
