// Phase 5, Exercise 5 — if constexpr process tests

#include <catch2/catch_test_macros.hpp>
#include "process.h"

TEST_CASE("process doubles int", "[if_constexpr]") {
    REQUIRE(process(5)   == 10);
    REQUIRE(process(-3)  == -6);
    REQUIRE(process(0)   == 0);
}

TEST_CASE("process doubles double", "[if_constexpr]") {
    REQUIRE(process(3.14) == 6.28);
    REQUIRE(process(-1.5) == -3.0);
}

TEST_CASE("process reverses std::string", "[if_constexpr]") {
    REQUIRE(process(std::string("hello"))   == "olleh");
    REQUIRE(process(std::string("abc"))     == "cba");
    REQUIRE(process(std::string(""))        == "");
    REQUIRE(process(std::string("a"))       == "a");
}

TEST_CASE("process does not modify the input", "[if_constexpr]") {
    // process takes by value; the original should be unchanged.
    std::string s = "test";
    auto result = process(s);
    REQUIRE(s == "test");        // original unchanged
    REQUIRE(result == "tset");   // reversed copy returned
}

TEST_CASE("is_arithmetic type trait checks", "[if_constexpr]") {
    static_assert(std::is_arithmetic_v<int>);
    static_assert(std::is_arithmetic_v<double>);
    static_assert(!std::is_arithmetic_v<std::string>);
    SUCCEED("static_asserts passed at compile time");
}
