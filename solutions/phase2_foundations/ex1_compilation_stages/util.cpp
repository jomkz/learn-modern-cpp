// Phase 2, Exercise 1 — Compilation Stages
// The definition of clamp_to_max(). This file is a separate translation unit:
// it is preprocessed, compiled, and assembled independently of main.cpp.
// The linker joins the two resulting object files.

#include "util.h"

int clamp_to_max(int value) {
    // After preprocessing, MAX_VALUE here becomes the literal 100.
    // You can see this in the preprocessed output (stage 1 in build_stages.sh).
    return (value > MAX_VALUE) ? MAX_VALUE : value;
}
