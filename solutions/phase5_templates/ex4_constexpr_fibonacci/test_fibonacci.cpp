// Phase 5, Exercise 4 — constexpr fibonacci tests
// The static_asserts in fibonacci.h already verify values at COMPILE TIME.
// These tests verify the same values at RUNTIME for a complete test suite.

#include <catch2/catch_test_macros.hpp>
#include "fibonacci.h"

TEST_CASE("fibonacci base cases", "[fibonacci]") {
    REQUIRE(fibonacci(0) == 0);
    REQUIRE(fibonacci(1) == 1);
    REQUIRE(fibonacci(2) == 1);
}

TEST_CASE("fibonacci known values", "[fibonacci]") {
    REQUIRE(fibonacci(5)  == 5);
    REQUIRE(fibonacci(10) == 55);
    REQUIRE(fibonacci(15) == 610);
    REQUIRE(fibonacci(20) == 6765);
}

TEST_CASE("fibonacci negative returns 0", "[fibonacci]") {
    REQUIRE(fibonacci(-1) == 0);
    REQUIRE(fibonacci(-100) == 0);
}

TEST_CASE("fibonacci can be used as template argument", "[fibonacci]") {
    // fibonacci_ct forces compile-time evaluation. Using its result as a
    // template non-type parameter proves it's a constant expression.
    std::array<int, fibonacci_ct(7)> arr{};  // array of 13 elements
    REQUIRE(arr.size() == 13);
}
