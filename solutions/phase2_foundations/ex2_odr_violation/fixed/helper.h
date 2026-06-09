// Phase 2, Exercise 2 — ODR Violation (FIXED version)
// Fix: move helper()'s DEFINITION to helper.cpp; keep only a DECLARATION here.
//
// Key distinction:
//   Declaration: "A function named helper() exists, takes no args, returns void."
//               The compiler trusts you; no code is emitted.
//   Definition:  "Here is the body of helper()."
//               The compiler emits machine code into the object file.
//
// Multiple declarations of the same function are allowed (and sometimes
// necessary). Only ONE definition may exist across the entire program.

#pragma once

// Declaration only — no body, just a prototype.
// a.cpp and b.cpp can both include this and call helper() without
// creating two definitions.
void helper();
