// Phase 1, Exercise 2 — Two-file project
// main.cpp only needs to include the header — it doesn't care how greet() is
// implemented. The linker resolves the call at link time.

#include <iostream>
#include "greet.h"

int main() {
    // The compiler sees the declaration of greet() from greet.h and generates
    // a call instruction. The linker later fills in the real address from greet.o.
    std::cout << greet("World") << '\n';
    std::cout << greet("C++ Learner") << '\n';
    return 0;
}
