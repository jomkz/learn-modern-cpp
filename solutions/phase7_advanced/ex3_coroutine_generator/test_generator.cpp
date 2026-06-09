// Phase 7, Exercise 3 — Coroutine Generator tests

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include "fibonacci_gen.h"

// First 20 Fibonacci numbers (0-indexed).
static constexpr uint64_t EXPECTED[] = {
    0, 1, 1, 2, 3, 5, 8, 13, 21, 34,
    55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181
};

TEST_CASE("fibonacci generator yields first 20 values", "[coroutine]") {
    auto gen = fibonacci();

    std::vector<uint64_t> values;
    // Range-based for uses begin()/end() — begin() resumes the coroutine
    // to get the first value; ++it resumes for each subsequent value.
    // We manually break after 20 to avoid infinite iteration.
    int count = 0;
    for (uint64_t v : gen) {
        values.push_back(v);
        if (++count == 20) break;
    }

    REQUIRE(values.size() == 20);
    for (int i = 0; i < 20; ++i) {
        REQUIRE(values[static_cast<std::size_t>(i)] == EXPECTED[i]);
    }
}

TEST_CASE("fibonacci generator can be restarted", "[coroutine]") {
    // Each call to fibonacci() creates a NEW coroutine frame — independent state.
    auto gen1 = fibonacci();
    auto gen2 = fibonacci();

    auto it1 = gen1.begin();
    auto it2 = gen2.begin();

    // Both should start at 0.
    REQUIRE(*it1 == 0);
    REQUIRE(*it2 == 0);

    // Advance gen1 three times; gen2 stays at beginning.
    ++it1; ++it1; ++it1;  // gen1 is now at index 3 (value 2)
    REQUIRE(*it1 == 2);
    REQUIRE(*it2 == 0);   // gen2 is unaffected
}

TEST_CASE("generator is move-only (no copy)", "[coroutine]") {
    // Confirm at compile time that Generator is not copyable.
    static_assert(!std::is_copy_constructible_v<Generator<uint64_t>>,
                  "Generator should not be copyable");
    static_assert(std::is_move_constructible_v<Generator<uint64_t>>,
                  "Generator should be movable");
    SUCCEED();
}
