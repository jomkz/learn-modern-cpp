// Phase 5, Exercise 3 — Variadic print with fold expression
// Concept: Variadic templates, parameter packs, fold expressions (C++17).
// Demonstrates: template<typename... Args>, (expr op ...), separator injection.
//
// Fold expressions (C++17) allow applying a binary operator across all elements
// of a parameter pack in a single expression, replacing recursive templates.
//
// Fold forms:
//   (pack op ...)       — left fold:  ((a op b) op c) op d
//   (... op pack)       — right fold: a op (b op (c op d))
//   (init op ... op pack) — left fold with initial value
//   (pack op ... op init) — right fold with initial value

#pragma once

#include <iostream>
#include <ostream>

// ── Simple version (no separator) ────────────────────────────────────────────
// Prints all arguments concatenated, then a newline.
// The fold expression (std::cout << ... << args) expands to:
//   std::cout << arg0 << arg1 << arg2 << ...
// This is a left fold: the leftmost << is evaluated first.
template<typename... Args>
void print_no_sep(Args&&... args) {
    (std::cout << ... << args);
    std::cout << '\n';
}

// ── Full version (space-separated) ───────────────────────────────────────────
// We can't fold with a space between each arg using a simple unary fold.
// Instead, we use a comma-fold trick: print each arg, then print a space,
// except for the last one. This version uses a two-arg helper approach.
//
// Helper that prints one value then a space:
template<typename T>
void print_with_space(std::ostream& os, const T& val, bool last) {
    os << val;
    if (!last) os << ' ';
}

// Variadic print to a stream with space separator and newline.
// We convert the pack to an index-counted approach via a fold over a comma
// expression that counts elements.
template<typename... Args>
void print(Args&&... args) {
    std::size_t idx = 0;
    const std::size_t last = sizeof...(args) - 1;
    // Comma-fold: (expr, ...) evaluates each expr left-to-right.
    // The comma operator ensures each element is printed before advancing idx.
    ((print_with_space(std::cout, args, idx++ == last)), ...);
    std::cout << '\n';
}

// ── Overload for arbitrary ostream ───────────────────────────────────────────
template<typename... Args>
void print_to(std::ostream& os, Args&&... args) {
    std::size_t idx = 0;
    const std::size_t last = sizeof...(args) - 1;
    ((print_with_space(os, args, idx++ == last)), ...);
    os << '\n';
}
