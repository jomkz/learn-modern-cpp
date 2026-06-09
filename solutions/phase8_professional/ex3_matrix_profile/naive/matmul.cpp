// Phase 8, Exercise 3 — Matrix Multiplication (NAIVE: cache-unfriendly)
// Concept: Cache locality, loop order optimisation, profiling with perf.
// Demonstrates: why ijk loop order causes cache misses.
//
// Matrix multiplication: C[i][j] = sum_k(A[i][k] * B[k][j])
//
// Memory layout: row-major (C/C++ default).
//   A[i][k] — advancing k steps through row i: sequential, cache-friendly.
//   B[k][j] — advancing k steps through COLUMN j: stride = N elements.
//              Each access jumps N doubles (N*8 bytes) in memory.
//              For N=512: stride = 4096 bytes. A 64-byte cache line holds 8 doubles.
//              We use 1 out of 8 loaded doubles → 87.5% cache miss rate.
//
// This is the "naive" or "textbook" implementation of matrix multiplication.
// It's correct but slow for large matrices due to B's column-access pattern.

#include <cstddef>
#include <vector>

// Square N×N matrix stored as a flat row-major vector.
// Element [i][j] is at data[i * N + j].
struct Matrix {
    std::size_t N;
    std::vector<double> data;
    Matrix(std::size_t n, double fill = 0.0) : N(n), data(n * n, fill) {}
    double& at(std::size_t i, std::size_t j) { return data[i * N + j]; }
    const double& at(std::size_t i, std::size_t j) const { return data[i * N + j]; }
};

// Naive ijk implementation.
// Inner loop: k iterates over B's column (stride-N access — cache thrashing).
void matmul_naive(const Matrix& A, const Matrix& B, Matrix& C) {
    const std::size_t N = A.N;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            for (std::size_t k = 0; k < N; ++k) {
                // B.at(k, j): k advances row-by-row through column j.
                // This is the cache-unfriendly access.
                C.at(i, j) += A.at(i, k) * B.at(k, j);
            }
        }
    }
}

// Expose for bench.cpp
extern "C" void run_naive(double* A, double* B, double* C, int N) {
    Matrix ma(static_cast<std::size_t>(N)), mb(static_cast<std::size_t>(N)),
           mc(static_cast<std::size_t>(N));
    ma.data.assign(A, A + N * N);
    mb.data.assign(B, B + N * N);
    matmul_naive(ma, mb, mc);
    for (int i = 0; i < N * N; ++i) C[i] = mc.data[static_cast<std::size_t>(i)];
}
