// Phase 8, Exercise 3 — Matrix Multiplication Benchmark
// Measures naive ijk vs ikj vs tiled and reports speedup.

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>  // std::iota
#include <vector>

// Declarations of the C-linked functions in naive/ and optimized/.
extern "C" void run_naive(double* A, double* B, double* C, int N);
extern "C" void run_ikj  (double* A, double* B, double* C, int N);
extern "C" void run_tiled(double* A, double* B, double* C, int N);

using Clock = std::chrono::steady_clock;

static double time_ms(auto fn) {
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    const int N = 512;  // 512×512 doubles = 2 MB per matrix; fits in L3 cache
    const std::size_t NN = static_cast<std::size_t>(N * N);

    std::vector<double> A(NN), B(NN), C(NN, 0.0);
    // Fill with small values to avoid overflow.
    for (std::size_t i = 0; i < NN; ++i) {
        A[i] = static_cast<double>(i % 17) * 0.01;
        B[i] = static_cast<double>(i % 13) * 0.01;
    }

    std::cout << "Matrix multiplication benchmark (N=" << N << ")\n";
    std::cout << std::string(50, '-') << '\n';

    // Warm up the cache with a dummy run.
    run_naive(A.data(), B.data(), C.data(), N);

    std::fill(C.begin(), C.end(), 0.0);
    double naive_ms = time_ms([&]{ run_naive(A.data(), B.data(), C.data(), N); });
    std::cout << "naive (ijk): " << std::setw(8) << std::fixed << std::setprecision(1)
              << naive_ms << " ms\n";

    std::fill(C.begin(), C.end(), 0.0);
    double ikj_ms = time_ms([&]{ run_ikj(A.data(), B.data(), C.data(), N); });
    std::cout << "ikj reorder: " << std::setw(8) << ikj_ms << " ms"
              << "  speedup vs naive: "
              << std::setprecision(2) << (naive_ms / ikj_ms) << "x\n";

    std::fill(C.begin(), C.end(), 0.0);
    double tiled_ms = time_ms([&]{ run_tiled(A.data(), B.data(), C.data(), N); });
    std::cout << "tiled:       " << std::setw(8) << tiled_ms << " ms"
              << "  speedup vs naive: "
              << std::setprecision(2) << (naive_ms / tiled_ms) << "x\n";

    return 0;
}
