// Phase 8, Exercise 3 — Matrix Multiplication (OPTIMIZED: cache-friendly)
// Demonstrates: ikj loop reorder, loop tiling (blocking), speedup measurement.
//
// Optimisation 1: Swap j and k loops (ijk → ikj).
//   In the ikj order, for a fixed i and k:
//     A.at(i, k): fixed — same value throughout the inner loop (register).
//     B.at(k, j): j advances along ROW k — sequential, cache-friendly!
//     C.at(i, j): j advances along ROW i — sequential, cache-friendly!
//   All three accesses are now sequential → maximises cache line utilisation.
//
// Optimisation 2: Loop tiling (blocking).
//   Even with ikj, for very large N, the working set of B's rows may exceed
//   the L1/L2 cache. Tiling divides the matrix into BLOCK_SIZE × BLOCK_SIZE
//   tiles and processes one tile at a time, keeping it in cache.
//   BLOCK_SIZE should be chosen so that 3 tiles fit in L1 cache:
//     3 * BLOCK_SIZE² * 8 bytes ≤ L1 cache size (typically 32–64 KB).
//     For L1 = 32KB: BLOCK_SIZE ≤ sqrt(32768/(3*8)) ≈ 37. Use 32 or 64.

#include <algorithm>  // std::min
#include <cstddef>
#include <vector>

struct Matrix {
    std::size_t N;
    std::vector<double> data;
    Matrix(std::size_t n, double fill = 0.0) : N(n), data(n * n, fill) {}
    double& at(std::size_t i, std::size_t j) { return data[i * N + j]; }
    const double& at(std::size_t i, std::size_t j) const { return data[i * N + j]; }
};

// ── Optimisation 1: ikj loop order ───────────────────────────────────────────
void matmul_ikj(const Matrix& A, const Matrix& B, Matrix& C) {
    const std::size_t N = A.N;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t k = 0; k < N; ++k) {
            // A[i][k] is constant for this inner loop — can be kept in a register.
            const double aik = A.at(i, k);
            for (std::size_t j = 0; j < N; ++j) {
                // Both B[k][j] and C[i][j] advance sequentially — cache-friendly.
                C.at(i, j) += aik * B.at(k, j);
            }
        }
    }
}

// ── Optimisation 2: tiled / blocked multiplication ───────────────────────────
constexpr std::size_t BLOCK_SIZE = 64;  // tune for your L1 cache size

void matmul_tiled(const Matrix& A, const Matrix& B, Matrix& C) {
    const std::size_t N = A.N;
    for (std::size_t i0 = 0; i0 < N; i0 += BLOCK_SIZE) {
        for (std::size_t k0 = 0; k0 < N; k0 += BLOCK_SIZE) {
            for (std::size_t j0 = 0; j0 < N; j0 += BLOCK_SIZE) {
                // Process the tile [i0, i0+B) × [k0, k0+B) × [j0, j0+B).
                const std::size_t i_end = std::min(i0 + BLOCK_SIZE, N);
                const std::size_t k_end = std::min(k0 + BLOCK_SIZE, N);
                const std::size_t j_end = std::min(j0 + BLOCK_SIZE, N);
                for (std::size_t i = i0; i < i_end; ++i) {
                    for (std::size_t k = k0; k < k_end; ++k) {
                        const double aik = A.at(i, k);
                        for (std::size_t j = j0; j < j_end; ++j) {
                            C.at(i, j) += aik * B.at(k, j);
                        }
                    }
                }
            }
        }
    }
}

extern "C" void run_ikj(double* A, double* B, double* C, int N) {
    Matrix ma(static_cast<std::size_t>(N)), mb(static_cast<std::size_t>(N)),
           mc(static_cast<std::size_t>(N));
    ma.data.assign(A, A + N * N);
    mb.data.assign(B, B + N * N);
    matmul_ikj(ma, mb, mc);
    for (int i = 0; i < N * N; ++i) C[i] = mc.data[static_cast<std::size_t>(i)];
}

extern "C" void run_tiled(double* A, double* B, double* C, int N) {
    Matrix ma(static_cast<std::size_t>(N)), mb(static_cast<std::size_t>(N)),
           mc(static_cast<std::size_t>(N));
    ma.data.assign(A, A + N * N);
    mb.data.assign(B, B + N * N);
    matmul_tiled(ma, mb, mc);
    for (int i = 0; i < N * N; ++i) C[i] = mc.data[static_cast<std::size_t>(i)];
}
