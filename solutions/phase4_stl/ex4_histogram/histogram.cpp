// Phase 4, Exercise 4 — Random Histogram
// Concept: <random> library, std::mt19937, std::uniform_int_distribution.
// Demonstrates: Why rand() is deprecated, seeding with random_device, bucketing.
//
// Why NOT use rand()?
//   - rand() uses global state — not thread-safe.
//   - Its period (number of unique values before cycling) is often just 32767.
//   - The lower bits of rand() tend to have poor randomness patterns.
//   - std::mt19937 has a period of 2^19937 - 1 and passes rigorous statistical tests.

#include <algorithm> // std::max_element
#include <array>
#include <iomanip>
#include <iostream>
#include <random>    // std::mt19937, std::random_device, std::uniform_int_distribution

int main() {
    // ── Setup: RNG and distribution ───────────────────────────────────────────
    // std::random_device is a hardware entropy source (reads /dev/urandom on Linux).
    // We use it to seed std::mt19937 so the sequence is different each run.
    // PITFALL: Calling operator()() on random_device repeatedly is expensive;
    //          only use it to seed the PRNG, not as the PRNG itself.
    std::mt19937 rng(std::random_device{}());

    // Produce integers uniformly distributed in [0, 999999].
    // std::uniform_int_distribution guarantees uniformity across the range,
    // unlike rand() % N which introduces modulo bias when N doesn't divide
    // RAND_MAX+1 evenly.
    std::uniform_int_distribution<int> dist(0, 999999);

    // ── Generate 1 million random integers ────────────────────────────────────
    constexpr int NUM_SAMPLES = 1'000'000;  // digit separators (C++14) for readability
    constexpr int NUM_BINS    = 10;
    constexpr int BIN_SIZE    = 1'000'000 / NUM_BINS;  // 100000 per bin

    std::array<int, NUM_BINS> buckets{};  // zero-initialised
    buckets.fill(0);

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        int value = dist(rng);
        // Integer division maps value to bin: 0–99999 → bin 0, 100000–199999 → bin 1, …
        int bin = value / BIN_SIZE;
        if (bin >= NUM_BINS) bin = NUM_BINS - 1;  // clamp edge: value == 999999
        ++buckets[static_cast<std::size_t>(bin)];
    }

    // ── Print ASCII histogram ─────────────────────────────────────────────────
    std::cout << "Histogram of " << NUM_SAMPLES << " uniform random integers\n";
    std::cout << "Expected count per bin: ~" << NUM_SAMPLES / NUM_BINS << '\n';
    std::cout << std::string(50, '-') << '\n';

    // Find max bucket count to scale the bar width.
    int max_count = *std::max_element(buckets.begin(), buckets.end());
    constexpr int BAR_WIDTH = 40;

    for (int b = 0; b < NUM_BINS; ++b) {
        int lo = b * BIN_SIZE;
        int hi = lo + BIN_SIZE - 1;
        int count = buckets[static_cast<std::size_t>(b)];

        // Scale bar length proportionally to the max bucket.
        int bar_len = static_cast<int>(
            static_cast<long long>(count) * BAR_WIDTH / max_count);

        std::cout << std::setw(7) << lo << '-' << std::setw(7) << hi
                  << " [" << std::string(static_cast<std::size_t>(bar_len), '#')
                  << std::string(static_cast<std::size_t>(BAR_WIDTH - bar_len), ' ')
                  << "] " << count << '\n';
    }

    return 0;
}
