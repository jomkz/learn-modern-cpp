// Phase 3, Exercise 2 — Rule of Five
// Exercises each of the five special member functions to confirm they work
// correctly. Run with -DENABLE_ASAN=ON to let AddressSanitizer catch any
// double-free or memory leak.

#include <cstring>
#include <iostream>
#include "buffer.h"

int main() {
    std::cout << "── 1. Constructor ──────────────────────────────────────\n";
    Buffer b1(16);
    std::memcpy(b1.data(), "Hello, Buffer!\0", 15);
    b1.print(std::cout);

    std::cout << "\n── 2. Copy Constructor ─────────────────────────────────\n";
    Buffer b2(b1);  // invokes Buffer(const Buffer&)
    b2.print(std::cout);
    // Verify deep copy: modifying b2 must not affect b1.
    b2.data()[0] = 'J';
    std::cout << "b1[0] = '" << b1.data()[0] << "' (should be 'H')\n";
    std::cout << "b2[0] = '" << b2.data()[0] << "' (should be 'J')\n";

    std::cout << "\n── 3. Copy Assignment ──────────────────────────────────\n";
    Buffer b3(8);
    b3 = b1;  // invokes operator=(const Buffer&)
    b3.print(std::cout);

    std::cout << "\n── 4. Move Constructor ─────────────────────────────────\n";
    Buffer b4(std::move(b1));  // invokes Buffer(Buffer&&)
    b4.print(std::cout);
    // b1 is now in the moved-from (empty) state — its data_ is nullptr.
    std::cout << "b1 after move: size=" << b1.size()
              << " (should be 0; safe to destroy)\n";

    std::cout << "\n── 5. Move Assignment ──────────────────────────────────\n";
    Buffer b5(4);
    b5 = std::move(b4);  // invokes operator=(Buffer&&)
    b5.print(std::cout);

    std::cout << "\n── 6. Self-assignment ──────────────────────────────────\n";
    b3 = b3;  // should be a no-op
    b3.print(std::cout);

    std::cout << "\nAll buffers destroyed at end of scope.\n"
              << "Run with ASan to confirm no double-free or leak.\n";

    return 0;
}
