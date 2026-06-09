// Phase 1, Exercise 2 — Two-file project
// Concept: Separating declaration (header) from definition (source file).
// Demonstrates: header guards, #pragma once, forward declarations.

// ── Header Guard ──────────────────────────────────────────────────────────────
// A header guard prevents this file's contents from being pasted twice into
// the same translation unit if it is #include-d multiple times (directly or
// transitively). The compiler would otherwise see duplicate declarations and
// error on duplicate type definitions.
//
// Two equivalent approaches:
//
// Approach A — #pragma once (non-standard but universally supported, simpler):
#pragma once
//
// Approach B — Traditional include guard (standard, works everywhere):
//   #ifndef GREET_H
//   #define GREET_H
//   ... header contents ...
//   #endif  // GREET_H
//
// Both prevent double-inclusion. #pragma once is shorter and can't be
// accidentally misspelled, but is technically an extension. Either is fine;
// pick one and be consistent across a project.

#include <string>

// This is a *declaration* — it tells the compiler that greet() exists and
// what its signature is, without providing the function body. The body
// (the *definition*) lives in greet.cpp.
//
// Why separate them?
//   - Multiple .cpp files can #include this header and call greet().
//   - Only one .cpp file contains the definition, satisfying the One Definition Rule.
//   - Recompiling greet.cpp doesn't force recompilation of every file that
//     merely includes the header (faster incremental builds).
std::string greet(const std::string& name);
