// Phase 1, Exercise 3 — Catch2 testing
// Concept: Unit testing with Catch2 v3.
// Demonstrates: FetchContent dependency management, TEST_CASE, SECTION, REQUIRE, CHECK.

#pragma once

// These are the functions we will test. They're intentionally simple so the
// focus is on the testing infrastructure, not the logic.
int add(int a, int b);
int multiply(int a, int b);
double divide(double numerator, double denominator);
