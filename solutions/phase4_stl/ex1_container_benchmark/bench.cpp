// Phase 4, Exercise 1 — Container Benchmark
// Concept: Performance characteristics of std::vector, std::list, std::deque.
// Demonstrates: std::chrono::steady_clock, template benchmarking, cache effects.
//
// Expected results (x86-64, Linux, -O2):
//   vector front-insert: VERY SLOW (O(n) shift of all elements)
//   list   front-insert: fast     (O(1) pointer manipulation)
//   deque  front-insert: fast     (O(1) amortised, block-based)
//
//   vector back-insert:  fast     (O(1) amortised with reserve)
//   list   back-insert:  fast     (O(1) but slow due to allocations)
//   deque  back-insert:  fast     (O(1) amortised)
//
//   vector iterate:      FASTEST  (cache-friendly contiguous memory)
//   list   iterate:      SLOWEST  (pointer chasing = cache misses)
//   deque  iterate:      medium   (block-based, reasonably cache-friendly)
//
// Key insight: std::vector wins sequential iteration by a wide margin because
// its elements are contiguous in memory. The CPU prefetcher can pipeline loads.
// std::list forces a pointer dereference per element (cache miss every time).

#include <chrono>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

using Clock = std::chrono::steady_clock;
using Micros = std::chrono::microseconds;

// Returns elapsed microseconds for a callable.
template<typename Fn>
long long time_us(Fn&& fn) {
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return std::chrono::duration_cast<Micros>(end - start).count();
}

// Benchmark a container type C for a given size n.
// We test: front-insert, back-insert, sequential-iterate (sum).
template<typename C>
void benchmark(const std::string& name, int n) {
    // ── Front insert ──────────────────────────────────────────────────────────
    // For std::vector this shifts every element right on each insert: O(n²) total.
    // For std::list and std::deque this is O(n) total.
    long long front_us = time_us([&] {
        C c;
        for (int i = 0; i < n; ++i) {
            c.insert(c.begin(), i);  // works for all three container types
        }
    });

    // ── Back insert ───────────────────────────────────────────────────────────
    long long back_us = time_us([&] {
        C c;
        for (int i = 0; i < n; ++i) {
            c.push_back(i);  // O(1) amortised for all three
        }
    });

    // ── Sequential iterate (sum) ───────────────────────────────────────────────
    // First build the container, then time only the iteration.
    C container;
    for (int i = 0; i < n; ++i) container.push_back(i);

    long long iter_us = time_us([&] {
        volatile long long sum = 0;  // volatile prevents the loop being optimised away
        for (const auto& v : container) sum += v;
        (void)sum;
    });

    std::cout << std::setw(8) << name
              << "  n=" << std::setw(8) << n
              << "  front=" << std::setw(8) << front_us << "µs"
              << "  back=" << std::setw(8) << back_us << "µs"
              << "  iter=" << std::setw(8) << iter_us << "µs\n";
}

int main() {
    std::cout << "Container benchmark (all times in microseconds)\n";
    std::cout << std::string(75, '-') << '\n';

    for (int n : {1000, 100000, 1000000}) {
        benchmark<std::vector<int>>("vector", n);
        benchmark<std::list<int>>  ("list",   n);
        benchmark<std::deque<int>> ("deque",  n);
        std::cout << '\n';
    }

    return 0;
}
