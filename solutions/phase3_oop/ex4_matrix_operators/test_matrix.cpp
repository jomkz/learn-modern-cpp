// Phase 3, Exercise 4 — Matrix operator tests

#include <catch2/catch_test_macros.hpp>
#include <algorithm>  // std::sort, std::is_sorted
#include <sstream>    // std::ostringstream
#include <vector>
#include "matrix.h"

TEST_CASE("operator<< produces output", "[matrix]") {
    Matrix m(2, 3, 1.0);
    m.at(0, 1) = 2.0;
    m.at(1, 2) = 3.0;

    std::ostringstream oss;
    oss << m;

    const std::string out = oss.str();
    // Should contain the dimensions and some values.
    REQUIRE(out.find("2x3") != std::string::npos);
    REQUIRE(out.find("2.00") != std::string::npos);
    REQUIRE(out.find("3.00") != std::string::npos);
}

TEST_CASE("operator< gives strict weak ordering", "[matrix]") {
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 2.0);
    Matrix c(3, 3, 0.5);

    // Smaller values come first; smaller shapes come before larger shapes.
    REQUIRE(a < b);
    REQUIRE(!(b < a));
    REQUIRE(b < c);  // 2x2 shape < 3x3 shape
    REQUIRE(!(a < a));  // irreflexivity
}

TEST_CASE("std::sort works on vector<Matrix>", "[matrix]") {
    std::vector<Matrix> matrices;
    matrices.emplace_back(2, 2, 5.0);
    matrices.emplace_back(2, 2, 1.0);
    matrices.emplace_back(2, 2, 3.0);
    matrices.emplace_back(1, 1, 0.0);

    // std::sort uses operator< by default via std::less<Matrix>.
    std::sort(matrices.begin(), matrices.end());

    REQUIRE(std::is_sorted(matrices.begin(), matrices.end()));

    // 1x1 matrix should come first (smallest shape).
    REQUIRE(matrices.front().rows() == 1);
}

TEST_CASE("operator== and operator!=", "[matrix]") {
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 1.0);
    Matrix c(2, 2, 2.0);

    REQUIRE(a == b);
    REQUIRE(!(a == c));
    REQUIRE(a != c);
}

TEST_CASE("chained operator<< works", "[matrix]") {
    Matrix a(1, 1, 1.0);
    Matrix b(1, 1, 2.0);

    std::ostringstream oss;
    // This tests that operator<< returns ostream& (enabling chaining).
    oss << a << b;

    REQUIRE(!oss.str().empty());
}
