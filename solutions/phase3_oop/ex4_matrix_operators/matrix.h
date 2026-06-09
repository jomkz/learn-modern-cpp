// Phase 3, Exercise 4 — Matrix operator overloading
// Concept: Operator overloading to give types natural value semantics.
// Demonstrates: operator<<, operator<, friend vs member, std::sort compatibility.
//
// Design decisions:
//   - operator<< must be a NON-MEMBER function: the left operand is std::ostream,
//     not Matrix. Member functions always have *this as the left operand.
//   - operator< is also a non-member for symmetry. Both are declared 'friend'
//     so they can access private members without a public accessor.
//   - operator== is needed for full equality semantics (tests use it).

#pragma once

#include <ostream>
#include <vector>

class Matrix {
public:
    // Construct an rows×cols matrix filled with 'fill' (default 0).
    Matrix(int rows, int cols, double fill = 0.0);

    // Element access (non-const and const overloads).
    double&       at(int row, int col);
    const double& at(int row, int col) const;

    int rows() const noexcept { return rows_; }
    int cols() const noexcept { return cols_; }

    // ── operator<< ────────────────────────────────────────────────────────────
    // Prints the matrix in a human-readable grid form.
    // 'friend' grants access to private members AND declares a non-member
    // function — the function is NOT a member of Matrix; it's a free function
    // that happens to have privileged access. This is the correct pattern for
    // symmetric operators where *this is NOT the left operand.
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

    // ── operator< ────────────────────────────────────────────────────────────
    // Defines a strict weak ordering so Matrix can be stored in std::set
    // and sorted with std::sort. We use lexicographic comparison of elements.
    // A strict weak ordering requires: irreflexivity, asymmetry, transitivity.
    friend bool operator<(const Matrix& lhs, const Matrix& rhs);

    // ── operator== ───────────────────────────────────────────────────────────
    friend bool operator==(const Matrix& lhs, const Matrix& rhs);
    friend bool operator!=(const Matrix& lhs, const Matrix& rhs);

private:
    int rows_;
    int cols_;
    // Flat row-major storage: element [r][c] is at data_[r * cols_ + c].
    // Using a flat vector avoids the overhead and indirection of
    // vector<vector<double>> while keeping bounds-checked access.
    std::vector<double> data_;
};
