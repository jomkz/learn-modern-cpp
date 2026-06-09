// Phase 7, Exercise 4 — C++20 Modules consumer
// 'import myutil' replaces '#include "myutil.h"'. The compiler loads
// the precompiled module interface rather than re-parsing a header.

import myutil;

#include <iostream>

int main() {
    // Use the exported namespace.
    std::cout << "square(7) = " << myutil::square(7) << '\n';
    std::cout << "to_hex(255) = " << myutil::to_hex(255) << '\n';
    std::cout << "to_hex(16)  = " << myutil::to_hex(16)  << '\n';
    return 0;
}
