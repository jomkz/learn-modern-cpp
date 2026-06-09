// Phase 2, Exercise 2 — ODR Violation (FIXED version)
// The single definition of helper(). Exactly one .cpp file may define it.
// Any .cpp that wants to call helper() includes helper.h for the declaration.

#include "helper.h"
#include <iostream>

void helper() {
    std::cout << "helper (single definition in helper.cpp)\n";
}
