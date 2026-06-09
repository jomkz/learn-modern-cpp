// Phase 5, Exercise 1 — clamp tests

#include <catch2/catch_test_macros.hpp>
#include "clamp.h"

TEST_CASE("clamp with int", "[clamp]") {
    REQUIRE(clamp(5,  0, 10) == 5);   // inside range
    REQUIRE(clamp(-1, 0, 10) == 0);   // below lo
    REQUIRE(clamp(15, 0, 10) == 10);  // above hi
    REQUIRE(clamp(0,  0, 10) == 0);   // at lo boundary
    REQUIRE(clamp(10, 0, 10) == 10);  // at hi boundary
}

TEST_CASE("clamp with double", "[clamp]") {
    REQUIRE(clamp(3.14, 0.0, 5.0) == 3.14);
    REQUIRE(clamp(-1.5, 0.0, 5.0) == 0.0);
    REQUIRE(clamp(7.0,  0.0, 5.0) == 5.0);
}

TEST_CASE("clamp with custom Duration type", "[clamp]") {
    // Demonstrates the template works for any std::totally_ordered type.
    Duration lo(100), hi(500), val_inside(300), val_below(50), val_above(600);

    REQUIRE(clamp(val_inside, lo, hi) == val_inside);
    REQUIRE(clamp(val_below,  lo, hi) == lo);
    REQUIRE(clamp(val_above,  lo, hi) == hi);
}

TEST_CASE("clamp at boundary is idempotent", "[clamp]") {
    // clamp(clamp(x, lo, hi), lo, hi) == clamp(x, lo, hi)
    for (int x : {-5, 0, 3, 10, 15}) {
        REQUIRE(clamp(clamp(x, 0, 10), 0, 10) == clamp(x, 0, 10));
    }
}
