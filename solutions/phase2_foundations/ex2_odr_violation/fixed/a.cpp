// Phase 2, Exercise 2 — ODR Violation (FIXED version)
// a.cpp uses helper() via its declaration in helper.h. No definition here.

#include "helper.h"

void do_a_thing() {
    helper();  // linker resolves this to the definition in helper.cpp
}
