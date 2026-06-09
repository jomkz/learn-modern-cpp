// Phase 1, Exercise 2 — Two-file project
// This is the *definition* of greet(). The compiler emits machine code for
// this function into greet.o; the linker later combines greet.o with main.o
// to produce the final executable.

#include "greet.h"  // include our own header to verify the signatures match

// Why #include "greet.h" here too?
//   If we accidentally change the signature in greet.cpp but forget to update
//   greet.h, the compiler will catch the mismatch with a "conflicting declaration"
//   error. Without this self-include, the mismatch would only be caught at link
//   time — or worse, silently produce wrong behavior.

std::string greet(const std::string& name) {
    // std::string supports operator+ for concatenation.
    // For more complex formatting prefer std::format (C++20).
    return "Hello, " + name + "!";
}
