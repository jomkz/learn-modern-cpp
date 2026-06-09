# Phase 7, Exercise 4 — C++20 Modules

## Toolchain Requirements

| Tool | Minimum Version | Notes |
|---|---|---|
| CMake | **3.28** | `FILE_SET CXX_MODULES` support |
| GCC | **14** | Modules enabled by default with `-std=c++20` |
| Clang | **17** | Use with `-fmodules` flag |
| MSVC | **2022 (19.34+)** | `/std:c++20` + Ninja generator |

**Recommended generator:** Ninja (`cmake -G Ninja`). Make has limited module dependency tracking.

## Build Commands

```sh
# Check your cmake version first:
cmake --version   # must be 3.28+

cmake -B build-modules -G Ninja \
      -DCMAKE_CXX_STANDARD=20 \
      -DCMAKE_BUILD_TYPE=Debug
cmake --build build-modules --target ph7_ex4_modules_main
./build-modules/phase7_advanced/ex4_modules/ph7_ex4_modules_main
```

## How Module Compilation Works

1. CMake detects `myutil.cppm` in `FILE_SET CXX_MODULES`.
2. It compiles the module interface into a Binary Module Interface (BMI):
   - GCC:   `myutil.gcm` in the build dir
   - Clang: `myutil.pcm` in the build dir
   - MSVC:  `myutil.ifc` in the build dir
3. `main.cpp` is compiled with a dependency on the BMI.
4. The linker links both object files normally.

## Why Modules Are Faster Than Headers

| | Headers | Modules |
|---|---|---|
| Parsed | Once per TU that includes them | Once total |
| Macro leakage | Yes — macros defined before #include affect the header | No — module interface is a clean snapshot |
| Include order bugs | Yes | No |
| Build time (large projects) | Baseline | 5x–100x faster in practice |

## Current Limitations (as of 2025)

- Standard library modules (`import std;`) are supported on MSVC 2022 but
  still experimental on GCC/Clang. Use `#include` inside module units for now.
- Mix of headers and modules in the same project is supported but can be tricky
  with macros crossing the boundary.
