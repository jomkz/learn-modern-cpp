// Phase 7, Exercise 2 — Ranges pipeline
// Concept: C++20 Ranges library, lazy views, pipe composition.
// Demonstrates: views::filter, views::transform, views::take, ranges::to.
//
// The Ranges library replaces the old iterator-pair based algorithms with
// range-based ones that compose lazily via the | pipe operator.
//
// Key properties:
//   LAZY: No work is done until the range is iterated. A filter|transform|take
//         chain does not materialise any intermediate containers.
//   COMPOSABLE: Each view adaptor returns a new view; they stack cleanly.
//   ZERO ALLOCATION: No heap allocation for the pipeline itself.
//
// Compare with the old approach:
//   vector<string> step1;
//   for (auto& s : lines) if (!s.empty()) step1.push_back(s);
//   vector<string> step2;
//   for (auto& s : step1) step2.push_back(to_uppercase(s));
//   vector<string> result(step2.begin(), step2.begin() + 10);
// Three intermediate vectors, three full copies.
//
// With ranges: zero intermediate vectors, evaluation stops as soon as
// take(10) has consumed its quota.

#pragma once

#include <algorithm>   // std::transform
#include <cctype>      // std::toupper
#include <ranges>      // std::views::*, std::ranges::to (C++23)
#include <string>
#include <vector>

// Converts a string to uppercase in-place (returns a copy).
inline std::string to_uppercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return s;
}

// Applies the lazy pipeline:
//   1. Filter out empty strings (views::filter)
//   2. Transform each string to uppercase (views::transform)
//   3. Take at most 'limit' results (views::take)
// Returns a materialised std::vector<std::string>.
inline std::vector<std::string>
process_lines(const std::vector<std::string>& lines, std::size_t limit = 10) {
    // Build the lazy pipeline. Nothing executes here — this creates a view
    // description, not a container.
    auto pipeline = lines
        | std::views::filter([](const std::string& s) { return !s.empty(); })
        | std::views::transform(to_uppercase)
        | std::views::take(static_cast<std::ranges::range_difference_t<decltype(lines)>>(limit));

    // Materialise into a vector.
    // C++23: std::ranges::to<std::vector>()
    // C++20 fallback: construct from iterator pair.
#if __cpp_lib_ranges_to_container >= 202202L
    return pipeline | std::ranges::to<std::vector>();
#else
    std::vector<std::string> result;
    for (auto&& s : pipeline) result.push_back(std::string(s));
    return result;
#endif
}
