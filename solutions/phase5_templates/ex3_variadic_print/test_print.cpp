// Phase 5, Exercise 3 — variadic print tests

#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "print.h"

TEST_CASE("print single argument", "[print]") {
    std::ostringstream oss;
    print_to(oss, 42);
    REQUIRE(oss.str() == "42\n");
}

TEST_CASE("print multiple arguments separated by spaces", "[print]") {
    std::ostringstream oss;
    print_to(oss, 1, 2, 3);
    REQUIRE(oss.str() == "1 2 3\n");
}

TEST_CASE("print mixed types", "[print]") {
    std::ostringstream oss;
    print_to(oss, "hello", 42, 3.14);
    REQUIRE(oss.str() == "hello 42 3.14\n");
}

TEST_CASE("print two arguments", "[print]") {
    std::ostringstream oss;
    print_to(oss, "a", "b");
    REQUIRE(oss.str() == "a b\n");
}

TEST_CASE("print no trailing space before newline", "[print]") {
    std::ostringstream oss;
    print_to(oss, 1, 2, 3);
    const std::string out = oss.str();
    // Last char before \n should not be a space.
    REQUIRE(out.size() >= 2);
    REQUIRE(out[out.size() - 2] != ' ');
}
