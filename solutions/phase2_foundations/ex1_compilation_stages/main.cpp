// Phase 2, Exercise 1 — Compilation Stages
// main.cpp — calls clamp_to_max() defined in util.cpp.
// At compile time the compiler only needs the *declaration* from util.h.
// At link time the linker resolves the call to the *definition* in util.o.

#include <iostream>
#include "util.h"

int main() {
    std::cout << clamp_to_max(50)  << '\n';  // prints 50
    std::cout << clamp_to_max(150) << '\n';  // prints 100 (clamped)
    return 0;
}
