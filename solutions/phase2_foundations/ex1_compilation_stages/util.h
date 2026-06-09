// Phase 2, Exercise 1 — Compilation Stages
// Concept: The four stages every C++ source file goes through.
// Demonstrates: preprocessing, compilation, assembly, linking.
//
// This header intentionally uses macros so the preprocessor output is
// interesting to inspect.

#pragma once

// A preprocessor macro — the preprocessor replaces every occurrence of
// MAX_VALUE with the literal token 100 before the compiler sees the file.
#define MAX_VALUE 100

// A declaration. The preprocessor doesn't know about types or functions;
// it just does text substitution. The compiler handles this line.
int clamp_to_max(int value);
