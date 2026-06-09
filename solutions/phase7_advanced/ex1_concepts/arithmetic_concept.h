// Phase 7, Exercise 1 — C++20 Concepts
// Concept: Named constraints on template parameters.
// Demonstrates: concept keyword, requires clause, readable error messages.
//
// BEFORE concepts, constraining a template to numeric types required SFINAE:
//   template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
//   class statistics { ... };
// When instantiated with a wrong type, SFINAE produces a multi-page substitution
// failure that doesn't mention 'arithmetic' anywhere.
//
// WITH concepts, the error immediately reads:
//   "constraint not satisfied: Arithmetic<std::string>"
// This is the primary user-facing benefit: readable diagnostics.

#pragma once

#include <type_traits>  // std::is_arithmetic_v

// ── Define the concept ────────────────────────────────────────────────────────
// A concept is a compile-time Boolean predicate on template parameters.
// 'concept Arithmetic = ...' names the constraint so it appears in diagnostics.
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

// Verify our concept makes sense at definition time.
static_assert(Arithmetic<int>);
static_assert(Arithmetic<double>);
static_assert(Arithmetic<float>);
static_assert(Arithmetic<unsigned long long>);
static_assert(!Arithmetic<std::string>);
static_assert(!Arithmetic<void*>);
