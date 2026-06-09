// Phase 2, Exercise 3 — RPN Calculator tests

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "rpn.h"

using Catch::Matchers::WithinRel;

TEST_CASE("basic arithmetic", "[rpn]") {
    REQUIRE_THAT(evaluate("3 4 +"),   WithinRel(7.0, 1e-9));
    REQUIRE_THAT(evaluate("10 3 -"),  WithinRel(7.0, 1e-9));
    REQUIRE_THAT(evaluate("3 4 *"),   WithinRel(12.0, 1e-9));
    REQUIRE_THAT(evaluate("10 4 /"),  WithinRel(2.5, 1e-9));
}

TEST_CASE("chained operations", "[rpn]") {
    // (3 + 4) * 2 = 14
    REQUIRE_THAT(evaluate("3 4 + 2 *"), WithinRel(14.0, 1e-9));
    // 15 7 1 1 + - / 3 * 2 1 1 + + - = 5
    REQUIRE_THAT(evaluate("15 7 1 1 + - / 3 * 2 1 1 + + -"), WithinRel(5.0, 1e-9));
}

TEST_CASE("negative numbers", "[rpn]") {
    REQUIRE_THAT(evaluate("-3 4 +"), WithinRel(1.0, 1e-9));
    REQUIRE_THAT(evaluate("5 -2 *"), WithinRel(-10.0, 1e-9));
}

TEST_CASE("floating point", "[rpn]") {
    REQUIRE_THAT(evaluate("1.5 2.5 +"), WithinRel(4.0, 1e-9));
    REQUIRE_THAT(evaluate("1 3 /"), WithinRel(0.333333, 1e-5));
}

TEST_CASE("error: division by zero", "[rpn][errors]") {
    REQUIRE_THROWS_AS(evaluate("5 0 /"), std::runtime_error);
}

TEST_CASE("error: unknown token", "[rpn][errors]") {
    REQUIRE_THROWS_AS(evaluate("3 4 %"), std::runtime_error);
}

TEST_CASE("error: not enough operands", "[rpn][errors]") {
    REQUIRE_THROWS_AS(evaluate("3 +"), std::runtime_error);
}

TEST_CASE("error: too many values", "[rpn][errors]") {
    REQUIRE_THROWS_AS(evaluate("3 4"), std::runtime_error);
}
