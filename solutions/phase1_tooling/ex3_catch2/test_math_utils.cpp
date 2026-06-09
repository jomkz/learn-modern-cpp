// Phase 1, Exercise 3 — Catch2 testing
// Concept: Structure of Catch2 v3 tests.
// Demonstrates: TEST_CASE, SECTION, REQUIRE, CHECK, REQUIRE_THROWS_AS.
//
// Build: cmake --build build --target ph1_ex3_tests
// Run:   ./build/phase1_tooling/ex3_catch2/ph1_ex3_tests
// Run (verbose): ./build/.../ph1_ex3_tests --reporter console --success

// Catch2::Catch2WithMain provides the main() entry point that parses
// command-line options (--filter, --reporter, --list-tests, etc.).
// We only need to include this single header.
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "math_utils.h"

// ── TEST_CASE ─────────────────────────────────────────────────────────────────
// TEST_CASE("name", "[tags]") registers a test. Tags allow selective running:
//   ./tests --filter "[arithmetic]"   runs only tagged tests.

TEST_CASE("add returns correct sums", "[arithmetic]") {
    // SECTION creates a sub-scope within a TEST_CASE. Each SECTION runs
    // independently from the top of the TEST_CASE — setup code above the first
    // SECTION runs before EACH SECTION. This avoids copy-paste of setup code.
    SECTION("positive numbers") {
        // REQUIRE: if this assertion fails, the test STOPS immediately.
        // Use REQUIRE for preconditions or when later assertions are meaningless
        // if this one fails.
        REQUIRE(add(2, 3) == 5);
    }

    SECTION("negative numbers") {
        REQUIRE(add(-4, -6) == -10);
    }

    SECTION("mixed sign") {
        REQUIRE(add(10, -3) == 7);
    }

    SECTION("identity: adding zero") {
        // CHECK: if this assertion fails, the test CONTINUES. Use CHECK when
        // you want to see all failures in one run, not just the first.
        CHECK(add(42, 0) == 42);
        CHECK(add(0, 42) == 42);
    }
}

TEST_CASE("multiply returns correct products", "[arithmetic]") {
    REQUIRE(multiply(3, 4) == 12);
    REQUIRE(multiply(-2, 5) == -10);
    REQUIRE(multiply(0, 99) == 0);
}

TEST_CASE("divide handles normal cases", "[arithmetic][floating-point]") {
    // WithinRel matcher checks floating-point equality within a relative
    // tolerance (1e-5 here). Never compare doubles with ==; rounding error
    // makes exact equality fragile.
    using Catch::Matchers::WithinRel;

    REQUIRE_THAT(divide(10.0, 4.0), WithinRel(2.5, 1e-9));
    REQUIRE_THAT(divide(1.0, 3.0), WithinRel(0.333333333, 1e-5));
}

TEST_CASE("divide throws on zero denominator", "[arithmetic][exceptions]") {
    // REQUIRE_THROWS_AS verifies that the expression throws a specific exception
    // type. If it throws the wrong type, or doesn't throw at all, the test fails.
    REQUIRE_THROWS_AS(divide(1.0, 0.0), std::invalid_argument);
}
