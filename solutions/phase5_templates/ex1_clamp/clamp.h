// Phase 5, Exercise 1 — Generic clamp function template
// Concept: Function templates, concept constraints, std::totally_ordered.
// Demonstrates: template<typename T>, requires clause, type constraints.
//
// std::clamp already exists in <algorithm> (C++17). This exercise is about
// understanding how to write it yourself, not about novelty.
//
// Two versions are shown:
//   1. Unconstrained (C++11 style) — compiles for any T but gives unhelpful
//      errors if T lacks operator<.
//   2. Constrained (C++20 Concepts) — immediately names the unsatisfied
//      requirement in the error message.

#pragma once

#include <concepts>    // std::totally_ordered

// ── Version 1: Unconstrained (pre-C++20 style) ───────────────────────────────
// Works for any type that has operator< and operator>. If T lacks these,
// the error message is a multi-line substitution failure pointing deep into
// the template. Hard to debug.
template<typename T>
T clamp_unconstrained(const T& value, const T& lo, const T& hi) {
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

// ── Version 2: Constrained with C++20 Concepts ───────────────────────────────
// The 'requires std::totally_ordered<T>' clause checks at the call site
// that T supports ==, !=, <, >, <=, >= with the expected semantics.
// If T doesn't satisfy the concept, the error says:
//   "constraint not satisfied: std::totally_ordered<MyType>"
// instead of a pages-long substitution trace.
template<typename T>
    requires std::totally_ordered<T>
T clamp(const T& value, const T& lo, const T& hi) {
    // Precondition: lo <= hi. In production, assert or throw if violated.
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

// ── Test type: custom Duration ────────────────────────────────────────────────
// Demonstrates that clamp works for any type satisfying std::totally_ordered,
// not just built-in types. Duration wraps milliseconds.
struct Duration {
    long long ms;
    explicit Duration(long long millis) : ms(millis) {}

    bool operator< (const Duration& o) const { return ms <  o.ms; }
    bool operator> (const Duration& o) const { return ms >  o.ms; }
    bool operator<=(const Duration& o) const { return ms <= o.ms; }
    bool operator>=(const Duration& o) const { return ms >= o.ms; }
    bool operator==(const Duration& o) const { return ms == o.ms; }
    bool operator!=(const Duration& o) const { return ms != o.ms; }
};
