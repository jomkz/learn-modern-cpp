#!/usr/bin/env bash
# Phase 2, Exercise 1 — The Four Compilation Stages
#
# Run from the directory containing this script:
#   cd solutions/phase2_foundations/ex1_compilation_stages
#   chmod +x build_stages.sh && ./build_stages.sh
#
# The four stages:
#   1. Preprocessing  (-E) : expands macros, processes #include, strips comments
#   2. Compilation    (-S) : translates C++ to assembly
#   3. Assembly       (-c) : translates assembly to machine code (object file)
#   4. Linking            : combines object files into an executable

set -e  # exit on first error

echo "══ Stage 1: Preprocessing ══════════════════════════════════════════"
# -E: stop after preprocessing; output the expanded translation unit to stdout.
# -I. adds the current directory to the include search path.
g++ -std=c++20 -E -I. main.cpp -o main.i
g++ -std=c++20 -E -I. util.cpp -o util.i

echo "Preprocessed line counts (large because <iostream> expands to ~30K lines):"
wc -l main.i util.i

echo ""
echo "See how MAX_VALUE was replaced with 100 in util.i:"
grep -n "MAX_VALUE\|100" util.i | head -10

echo ""
echo "══ Stage 2: Compilation (C++ → Assembly) ═══════════════════════════"
# -S: stop after producing assembly; output .s file.
# -O0: no optimisation so the assembly closely mirrors the source.
# -g: include debug info in assembly (labels, .file, .loc directives).
g++ -std=c++20 -S -O0 -g -I. main.cpp -o main.s
g++ -std=c++20 -S -O0 -g -I. util.cpp -o util.s

echo "Assembly files created: main.s, util.s"
echo "First 20 lines of util.s (look for clamp_to_max label):"
head -20 util.s

echo ""
echo "══ Stage 3: Assembly (.s → Object file .o) ══════════════════════════"
# -c: compile AND assemble, but do NOT link. Produces an object file.
# At this point undefined symbols (like cout) are recorded but not resolved.
g++ -std=c++20 -c main.s -o main.o
g++ -std=c++20 -c util.s -o util.o

echo "Object files created. Undefined symbols in main.o:"
nm main.o | grep ' U '  # 'U' = undefined; will be resolved at link time

echo ""
echo "══ Stage 4: Linking (.o → executable) ══════════════════════════════"
# Link both object files. The linker resolves clamp_to_max from util.o
# and the C++ standard library from the system's libstdc++ / libc++.
g++ main.o util.o -o app

echo "Executable 'app' created."
echo "Running: ./app"
./app

echo ""
echo "Inspect with 'file app' to see the binary format:"
file app
