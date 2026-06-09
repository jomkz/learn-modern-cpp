// Phase 4, Exercise 4 — Histogram tests
// Tests the bucketing logic and statistical properties.

#include <catch2/catch_test_macros.hpp>
#include <array>
#include <random>

// Extracted bucketing logic so it can be unit tested.
static std::array<int, 10> build_histogram(int n_samples, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 999999);
    std::array<int, 10> buckets{};
    buckets.fill(0);
    for (int i = 0; i < n_samples; ++i) {
        int bin = dist(rng) / 100000;
        if (bin >= 10) bin = 9;
        ++buckets[static_cast<std::size_t>(bin)];
    }
    return buckets;
}

TEST_CASE("total sample count sums to n", "[histogram]") {
    auto buckets = build_histogram(1'000'000, 42u);
    int total = 0;
    for (int c : buckets) total += c;
    REQUIRE(total == 1'000'000);
}

TEST_CASE("all 10 bins are populated", "[histogram]") {
    auto buckets = build_histogram(1'000'000, 42u);
    for (int c : buckets) {
        REQUIRE(c > 0);
    }
}

TEST_CASE("bins are roughly uniform (within 5%)", "[histogram]") {
    // With 1M samples the expected count per bin is 100000. A 5% tolerance
    // gives [95000, 105000]. The probability of any bin falling outside this
    // range with a good PRNG is astronomically small.
    auto buckets = build_histogram(1'000'000, 42u);
    for (int c : buckets) {
        REQUIRE(c > 95000);
        REQUIRE(c < 105000);
    }
}
