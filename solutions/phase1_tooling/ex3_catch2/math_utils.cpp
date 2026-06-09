// Phase 1, Exercise 3 — Catch2 testing

#include "math_utils.h"
#include <stdexcept>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(double numerator, double denominator) {
    // Throwing a standard exception from library code is correct. The caller
    // decides whether to catch it — the library shouldn't swallow it silently.
    if (denominator == 0.0) {
        throw std::invalid_argument("division by zero");
    }
    return numerator / denominator;
}
