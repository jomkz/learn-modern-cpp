// Phase 7, Exercise 1 — C++20 Concepts
// A statistics class constrained to arithmetic types only.
// Demonstrates: template<Arithmetic T>, concept in class template.

#pragma once

#include <cmath>        // std::sqrt
#include <numeric>      // std::accumulate
#include <stdexcept>    // std::invalid_argument
#include <vector>
#include "arithmetic_concept.h"

// The concept constraint appears directly in the template parameter list.
// 'template<Arithmetic T>' is shorthand for 'template<typename T> requires Arithmetic<T>'.
// If you try 'statistics<std::string>', the compiler immediately says:
//   "template constraint not satisfied: Arithmetic<std::string>"
template<Arithmetic T>
class statistics {
public:
    explicit statistics(std::vector<T> data) : data_(std::move(data)) {
        if (data_.empty())
            throw std::invalid_argument("statistics: empty data set");
    }

    // Arithmetic mean: sum / n
    double mean() const {
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
        return sum / static_cast<double>(data_.size());
    }

    // Sample variance: sum((x - mean)²) / (n - 1)
    // Using n-1 (Bessel's correction) gives an unbiased estimator.
    double variance() const {
        if (data_.size() < 2)
            throw std::invalid_argument("statistics: need at least 2 samples for variance");
        double m = mean();
        double sq_sum = 0.0;
        for (const T& x : data_) {
            double diff = static_cast<double>(x) - m;
            sq_sum += diff * diff;
        }
        return sq_sum / static_cast<double>(data_.size() - 1);
    }

    double stddev() const { return std::sqrt(variance()); }

    T min() const { return *std::min_element(data_.begin(), data_.end()); }
    T max() const { return *std::max_element(data_.begin(), data_.end()); }

    std::size_t count() const { return data_.size(); }

private:
    std::vector<T> data_;
};
