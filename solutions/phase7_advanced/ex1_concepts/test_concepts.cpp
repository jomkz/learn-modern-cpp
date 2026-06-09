// Phase 7, Exercise 1 — Concepts tests
// To see the readable concept error, uncomment the static_assert at the bottom.

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "statistics.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("statistics<double> mean", "[concepts]") {
    statistics<double> s({1.0, 2.0, 3.0, 4.0, 5.0});
    REQUIRE_THAT(s.mean(), WithinAbs(3.0, 1e-9));
}

TEST_CASE("statistics<int> mean", "[concepts]") {
    statistics<int> s({10, 20, 30});
    REQUIRE_THAT(s.mean(), WithinAbs(20.0, 1e-9));
}

TEST_CASE("statistics variance and stddev", "[concepts]") {
    // For {2, 4, 4, 4, 5, 5, 7, 9}, sample variance = 4.571...
    statistics<double> s({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0});
    REQUIRE_THAT(s.variance(), WithinAbs(4.571428, 1e-4));
    REQUIRE_THAT(s.stddev(),   WithinAbs(2.13809, 1e-4));
}

TEST_CASE("statistics min/max", "[concepts]") {
    statistics<int> s({5, 1, 9, 3, 7});
    REQUIRE(s.min() == 1);
    REQUIRE(s.max() == 9);
    REQUIRE(s.count() == 5);
}

TEST_CASE("Arithmetic concept rejects non-arithmetic types at compile time", "[concepts]") {
    // Verify the concept check:
    static_assert(!Arithmetic<std::string>,
                  "std::string should not satisfy Arithmetic");
    static_assert(!Arithmetic<std::vector<int>>,
                  "vector should not satisfy Arithmetic");

    // To deliberately trigger the concept error and see the readable message,
    // uncomment the line below and recompile. The error will name the concept:
    //   statistics<std::string> bad({"a"});  // "constraint not satisfied: Arithmetic<std::string>"

    SUCCEED("Concept correctly rejects non-arithmetic types");
}
