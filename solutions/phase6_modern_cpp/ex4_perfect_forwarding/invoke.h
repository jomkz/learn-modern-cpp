// Phase 6, Exercise 4 — Perfect Forwarding
// Concept: Forwarding references, std::forward, decltype(auto).
// Demonstrates: T&&, value category preservation, universal reference pattern.
//
// The Problem:
//   When you write a wrapper function that passes arguments to another function,
//   you want to preserve whether each argument is an lvalue (named variable,
//   can be assigned to) or rvalue (temporary, std::move result). Without
//   std::forward, all named parameters are lvalues — you lose the rvalue-ness
//   of temporaries, forcing copies instead of moves.
//
// The Solution — std::forward<T>:
//   - If T was deduced from an lvalue (T = X&), std::forward<T>(x) returns X&.
//   - If T was deduced from an rvalue (T = X), std::forward<T>(x) returns X&&.
//   - This is called "perfect forwarding" — value categories are preserved.
//
// Forwarding reference vs rvalue reference:
//   T&&  in a DEDUCED context (template parameter) = forwarding reference
//   T&&  in a NON-deduced context (specific type) = rvalue reference
//   These look identical but behave differently. The difference is whether T
//   is being deduced from the argument at the call site.

#pragma once

#include <utility>  // std::forward

// ── invoke: perfect-forwarding wrapper ───────────────────────────────────────
// This is essentially std::invoke (C++17) written from scratch for learning.
//
// F&& f      — forwarding reference to the callable
// Args&&...  — forwarding references to each argument (parameter pack)
// decltype(auto) — deduces the return type including reference and cv-qualifiers.
//                  If f(args...) returns int&, decltype(auto) deduces int& (not int).
//                  Plain 'auto' would strip the reference, returning int instead.
template<typename F, typename... Args>
decltype(auto) invoke(F&& f, Args&&... args) {
    // std::forward<F>(f)         — forward the callable itself
    // std::forward<Args>(args)... — forward each argument preserving value category
    //
    // Why not just call f(args...) directly?
    //   Without std::forward, 'args' are named variables, so they're always lvalues.
    //   std::forward<Args>(args) restores the original rvalue-ness if Args was
    //   deduced as a non-reference type (indicating the argument was an rvalue).
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
