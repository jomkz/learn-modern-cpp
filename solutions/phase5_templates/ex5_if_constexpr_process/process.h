// Phase 5, Exercise 5 — if constexpr and type traits
// Concept: Compile-time branching inside templates.
// Demonstrates: if constexpr, std::is_arithmetic_v, type traits.
//
// Problem: We want a function that doubles numbers and reverses strings.
// Both branches must compile for EVERY type, which is impossible without
// compile-time branching.
//
// Why regular 'if' doesn't work in a template:
//   template<typename T>
//   T process(T x) {
//       if (std::is_arithmetic_v<T>) return x * 2;   // fine for numbers
//       else return std::string(x.rbegin(), x.rend()); // ← compile error for int!
//   }
// The compiler type-checks BOTH branches for every T. Even if the false
// branch can never execute, it must be valid code.
//
// if constexpr: the untaken branch is not instantiated, eliminating the error.

#pragma once

#include <algorithm>   // std::reverse
#include <string>
#include <type_traits> // std::is_arithmetic_v

template<typename T>
T process(T x) {
    if constexpr (std::is_arithmetic_v<T>) {
        // This branch is compiled only when T is an arithmetic type
        // (int, double, float, etc.). The else branch is discarded.
        return x * 2;
    } else {
        // This branch is compiled only when T is NOT arithmetic.
        // The x * 2 in the if branch is never checked for std::string.
        // We return a reversed copy.
        T result = x;
        std::reverse(result.begin(), result.end());
        return result;
    }
}

// ── Type traits mini-tutorial ─────────────────────────────────────────────────
// std::is_arithmetic_v<T> is true for:
//   integral types:  bool, char, short, int, long, long long (and unsigned variants)
//   floating-point:  float, double, long double
// It's false for: pointers, references, class types, enum, std::string, etc.
//
// Other useful type traits:
//   std::is_integral_v<T>        — integers only (not float/double)
//   std::is_floating_point_v<T>  — float, double, long double only
//   std::is_same_v<T, U>         — true if T and U are the same type
//   std::is_class_v<T>           — true for class/struct types
//   std::is_pointer_v<T>         — true for pointer types
