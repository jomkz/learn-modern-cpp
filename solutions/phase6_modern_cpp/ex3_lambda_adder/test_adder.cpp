// Phase 6, Exercise 3 — Lambda adder tests

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <vector>
#include "adder.h"

TEST_CASE("make_adder returns correct adder", "[lambda]") {
    auto add5  = make_adder(5);
    auto add10 = make_adder(10);

    REQUIRE(add5(3)   == 8);
    REQUIRE(add5(0)   == 5);
    REQUIRE(add5(-5)  == 0);
    REQUIRE(add10(7)  == 17);
    REQUIRE(add10(-3) == 7);
}

TEST_CASE("adders are independent (each captures its own n)", "[lambda]") {
    // Each lambda has its own copy of n inside its closure.
    auto add1 = make_adder(1);
    auto add2 = make_adder(2);
    auto add3 = make_adder(3);

    REQUIRE(add1(10) == 11);
    REQUIRE(add2(10) == 12);
    REQUIRE(add3(10) == 13);

    // Calling add1 doesn't affect add2 or add3.
    (void)add1(999);
    REQUIRE(add2(10) == 12);
}

TEST_CASE("adders stored in vector<function<int(int)>>", "[lambda]") {
    // std::function<int(int)> provides a uniform type for heterogeneous
    // callables (different lambdas capturing different values of n).
    std::vector<std::function<int(int)>> adders;
    adders.push_back(make_adder(1));
    adders.push_back(make_adder(10));
    adders.push_back(make_adder(100));

    REQUIRE(adders[0](5) == 6);
    REQUIRE(adders[1](5) == 15);
    REQUIRE(adders[2](5) == 105);
}

TEST_CASE("adders compose correctly", "[lambda]") {
    // Apply a chain of adders: +1, +10, +100.
    std::vector<std::function<int(int)>> pipeline;
    pipeline.push_back(make_adder(1));
    pipeline.push_back(make_adder(10));
    pipeline.push_back(make_adder(100));

    int result = 0;
    for (const auto& f : pipeline) {
        result = f(result);
    }
    REQUIRE(result == 111);  // 0 + 1 + 10 + 100
}
