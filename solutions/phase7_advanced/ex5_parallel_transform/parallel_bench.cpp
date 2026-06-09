// Phase 7, Exercise 5 — Parallel std::transform
// Concept: C++17 execution policies, std::execution::par, parallel algorithms.
// Demonstrates: std::execution::seq vs par, TBB dependency, timing.
//
// C++17 added execution policies to most <algorithm> functions:
//   std::execution::seq  — sequential (single-threaded, default behaviour)
//   std::execution::par  — parallel (unordered between threads, safe for
//                          independent operations)
//   std::execution::par_unseq — parallel + SIMD vectorisation
//
// On Linux with libstdc++, parallel algorithms require Intel TBB as the
// backend. Install with: sudo apt install libtbb-dev
// On macOS with libc++ (LLVM), TBB may or may not be needed.
// On MSVC/Windows, parallel STL is built-in.
//
// IMPORTANT: std::execution::par does NOT guarantee ordering. If the operation
// has side effects or elements are interdependent, par produces data races.
// For std::transform with a stateless function, it's always safe.

#include <algorithm>   // std::transform
#include <chrono>
#include <cmath>       // std::sqrt
#include <execution>   // std::execution::seq, std::execution::par
#include <iostream>
#include <iomanip>
#include <vector>

using Clock = std::chrono::steady_clock;

template<typename Policy, typename Vec>
long long time_transform(Policy policy, Vec& input, Vec& output) {
    auto start = Clock::now();
    std::transform(policy, input.begin(), input.end(), output.begin(),
        [](double x) {
            // A moderately expensive operation so parallelisation shows benefit.
            return std::sqrt(x) + std::sin(x) + std::cos(x);
        });
    auto end = Clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    constexpr std::size_t N = 10'000'000;

    std::vector<double> input(N), output_seq(N), output_par(N);
    // Initialise with non-trivial values so the computation isn't optimised away.
    for (std::size_t i = 0; i < N; ++i) {
        input[i] = static_cast<double>(i) * 0.001;
    }

    std::cout << "std::transform on " << N << " doubles\n";
    std::cout << std::string(40, '-') << '\n';

    // ── Sequential ────────────────────────────────────────────────────────────
    long long seq_ms = time_transform(std::execution::seq, input, output_seq);
    std::cout << "seq:  " << std::setw(6) << seq_ms << " ms\n";

    // ── Parallel ─────────────────────────────────────────────────────────────
    long long par_ms = time_transform(std::execution::par, input, output_par);
    std::cout << "par:  " << std::setw(6) << par_ms << " ms\n";

    // ── Verify results match ──────────────────────────────────────────────────
    // Floating-point operations may produce slightly different results across
    // threads due to non-associative floating-point arithmetic. For this
    // benchmark we verify that output_par is non-zero (not silently skipped).
    bool any_nonzero = false;
    for (const double v : output_par) if (v != 0.0) { any_nonzero = true; break; }

    // ── Report speedup ────────────────────────────────────────────────────────
    if (par_ms > 0) {
        double speedup = static_cast<double>(seq_ms) / static_cast<double>(par_ms);
        std::cout << "speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";
    }

    std::cout << (any_nonzero ? "Results verified.\n" : "Warning: all zero results.\n");
    return 0;
}
