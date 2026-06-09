// Phase 3, Exercise 4 — Matrix operator overloading

#include "matrix.h"

#include <iomanip>   // std::setw, std::fixed, std::setprecision
#include <stdexcept> // std::out_of_range

Matrix::Matrix(int rows, int cols, double fill)
    : rows_(rows)
    , cols_(cols)
    , data_(static_cast<std::size_t>(rows * cols), fill)
{
    if (rows <= 0 || cols <= 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
}

double& Matrix::at(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        throw std::out_of_range("Matrix::at out of range");
    return data_[static_cast<std::size_t>(row * cols_ + col)];
}

const double& Matrix::at(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        throw std::out_of_range("Matrix::at out of range");
    return data_[static_cast<std::size_t>(row * cols_ + col)];
}

// ── operator<< ────────────────────────────────────────────────────────────────
// The return type is std::ostream& so that chaining works: cout << a << b.
// Returning by value would copy the stream (impossible — streams aren't copyable).
std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    os << '[' << m.rows_ << 'x' << m.cols_ << "]\n";
    for (int r = 0; r < m.rows_; ++r) {
        os << "  [";
        for (int c = 0; c < m.cols_; ++c) {
            os << std::fixed << std::setprecision(2)
               << std::setw(7) << m.data_[static_cast<std::size_t>(r * m.cols_ + c)];
            if (c + 1 < m.cols_) os << ',';
        }
        os << " ]\n";
    }
    return os;
}

// ── operator< (lexicographic) ─────────────────────────────────────────────────
// We compare (rows, cols) first for shape ordering, then element-by-element.
// std::vector already implements lexicographic comparison, so we delegate.
bool operator<(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.rows_ != rhs.rows_) return lhs.rows_ < rhs.rows_;
    if (lhs.cols_ != rhs.cols_) return lhs.cols_ < rhs.cols_;
    return lhs.data_ < rhs.data_;  // std::vector<double> lexicographic <
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.rows_ == rhs.rows_
        && lhs.cols_ == rhs.cols_
        && lhs.data_ == rhs.data_;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs) {
    return !(lhs == rhs);
}
