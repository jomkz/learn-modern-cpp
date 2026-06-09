// Phase 2, Exercise 2 — ODR Violation (BROKEN version)
// b.cpp also defines helper() — this is the second definition that causes the
// linker error. In a real project this often happens when:
//   - A function definition is placed in a .h file that is #included by
//     more than one .cpp file.
//   - Two developers independently write the same utility function.

#include <iostream>

// Second definition — violates the ODR.
void helper() {
    std::cout << "helper from b.cpp\n";
}

void do_b_thing() {
    helper();
}
