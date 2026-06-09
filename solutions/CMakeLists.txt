# solutions/CMakeLists.txt
# Root build file for all learn-modern-cpp exercise solutions.
#
# Build:
#   cmake -B build -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build -j$(nproc)
#   cd build && ctest --output-on-failure
#
# Build with AddressSanitizer (recommended for Phases 3, 6, 8):
#   cmake -B build-asan -DENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build-asan
#
# Note: Phase 7 Ex4 (modules) requires CMake 3.28+ and GCC 14+ / Clang 17+.
#       Phase 7 Ex5 (parallel) requires libtbb-dev on Linux.

cmake_minimum_required(VERSION 3.25)
project(LearnModernCpp LANGUAGES CXX)

# ── Language standard ─────────────────────────────────────────────────────────
# C++20 gives us concepts, ranges, coroutines, jthread, span, and format.
# EXTENSIONS OFF keeps us strictly conformant — no GNU extensions.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json so clang-tidy and clangd can find include paths.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Shared helpers ────────────────────────────────────────────────────────────
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/FetchCatch2.cmake)

# ── Enable CTest at the root so each phase can register tests ─────────────────
enable_testing()

# ── Exercise phases ───────────────────────────────────────────────────────────
add_subdirectory(phase1_tooling)
add_subdirectory(phase2_foundations)
add_subdirectory(phase3_oop)
add_subdirectory(phase4_stl)
add_subdirectory(phase5_templates)
add_subdirectory(phase6_modern_cpp)
add_subdirectory(phase7_advanced)
add_subdirectory(phase8_professional)
