// Phase 2, Exercise 2 — ODR Violation (BROKEN version)
//
// The One Definition Rule (ODR): every function, variable, and class must have
// exactly ONE definition in the entire program (across all translation units).
//
// This file defines helper(). So does b.cpp. When the linker tries to combine
// a.o and b.o it will see two definitions of the same symbol and error:
//
//   b.cpp:(.text+0x...): multiple definition of `helper()'
//   a.cpp:(.text+0x...): first defined here
//
// PITFALL: The error is a *linker* error, not a compiler error. Each
// translation unit compiles successfully in isolation — the problem only
// surfaces when combining them.

#include <iostream>

// This is a DEFINITION (has a body). Both a.cpp and b.cpp define it.
void helper() {
    std::cout << "helper from a.cpp\n";
}

void do_a_thing() {
    helper();
}
