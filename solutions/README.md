# Solutions — learn-modern-cpp

This directory contains complete, heavily-commented solutions for every exercise in the guide.
Each file is written as a **teaching artifact**: comments explain *why* code is written a certain
way, flag common pitfalls, and cross-reference related concepts.

96 Catch2 tests cover all phases and pass cleanly on GCC 16 / Clang 22.

---

## Prerequisites

| Tool | Minimum | Notes |
|---|---|---|
| CMake | 3.25 | 3.28+ required for Phase 7 Ex4 (modules) |
| GCC | 13+ | 14+ required for modules; or Clang 16+ |
| Ninja | any | required for Phase 7 Ex4 (C++20 modules) |
| libtbb-dev | any | Linux only; needed for Phase 7 Ex5 (parallel STL) |

```sh
# Ubuntu / Debian
sudo apt install cmake ninja-build g++ libtbb-dev

# macOS (Homebrew LLVM — Apple Clang lags C++20/23 support)
brew install cmake ninja llvm tbb

# Windows: Visual Studio 2022 includes CMake, Ninja, and the parallel STL backend
```

---

## Build

```sh
cd solutions

# Standard debug build (all phases, all tests)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

---

## AddressSanitizer Build

Several exercises (Phase 3 Ex2, Phase 6 Ex1, Phase 8 Ex1) are designed to be
run under ASan. Enable it with:

```sh
cmake -B build-asan -DENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-asan -j$(nproc)
ctest --test-dir build-asan --output-on-failure
```

ASan output appears on stderr with a `==ERROR: AddressSanitizer==` header and
a full stack trace pointing to the exact source line.

---

## Phase 7 Ex4 — C++20 Modules

Modules require CMake 3.28+, GCC 14+ (or Clang 17+), **and the Ninja generator**
(CMake's module dependency scanning is only implemented for Ninja and Visual Studio):

```sh
cmake -B build-modules -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug
cmake --build build-modules --target ph7_ex4_modules_main
```

If the toolchain or generator doesn't qualify, CMake prints a status message and
skips the exercise — the rest of the build is unaffected.

---

## Phase 7 Ex5 — Parallel Algorithms

`std::execution::par` on Linux requires Intel TBB as the parallel backend for
libstdc++. Install it before configuring:

```sh
sudo apt install libtbb-dev          # Ubuntu
brew install tbb                     # macOS
# Windows: TBB is bundled with MSVC's parallel STL
```

If TBB is not found, CMake skips the parallel benchmark with a status message.

---

## Offline / Airgapped Builds

Catch2 is fetched at configure time via CMake's `FetchContent`. If the network is
unavailable, point it at a local Catch2 source tree:

```sh
cmake -B build \
      -D_CATCH2_LOCAL_SRC=/path/to/catch2-src \
      -DCMAKE_BUILD_TYPE=Debug
```

The path should be the root of a Catch2 v3 checkout (it must contain a
`CMakeLists.txt`). No internet access is required after that.

---

## IDE Integration

CMake writes a `compile_commands.json` to the build directory. Point your editor
at it so clangd has accurate include paths and flags:

```sh
# One-time symlink (or configure your editor to look in build/)
ln -s build/compile_commands.json compile_commands.json
```

A `.clangd` file in this directory suppresses a Clang 22 false-positive
(`-Wc2y-extensions`) triggered by `__COUNTER__` inside Catch2's `TEST_CASE`
macros. The actual build (GCC) is unaffected.

---

## Directory Structure

```
solutions/
├── .clangd              IDE/clangd configuration (suppresses Catch2 macro warnings)
├── cmake/               Shared CMake helpers (warnings, sanitizers, Catch2)
├── phase1_tooling/      Ex 1–3: hello world, two-file project, Catch2
├── phase2_foundations/  Ex 1–3: compilation stages, ODR violation, RPN calculator
├── phase3_oop/          Ex 1–4: RAII, Rule of Five, copy-and-swap, Matrix operators
├── phase4_stl/          Ex 1–4: container benchmarks, custom iterator, phonebook, histogram
├── phase5_templates/    Ex 1–5: clamp, TypedStack, variadic print, constexpr fib, if constexpr
├── phase6_modern_cpp/   Ex 1–4: smart pointers, move semantics, lambdas, perfect forwarding
├── phase7_advanced/     Ex 1–5: concepts, ranges, coroutines, modules, parallel algorithms
└── phase8_professional/ Ex 1–5: UAF/GDB, clang-tidy, matrix optimisation, producer-consumer, CI
```
