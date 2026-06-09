# phase7_advanced/CMakeLists.txt

add_subdirectory(ex1_concepts)
add_subdirectory(ex2_ranges_pipeline)
add_subdirectory(ex3_coroutine_generator)

# Ex4 (modules) requires CMake 3.28+ and GCC 14+ / Clang 17+.
# We guard it so older toolchains don't fail the build.
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.28" AND
   CMAKE_GENERATOR MATCHES "Ninja" AND
   ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"   AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "14") OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "17") OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")))
    add_subdirectory(ex4_modules)
else()
    message(STATUS "Phase 7 Ex4 (C++20 Modules): skipped. "
                   "Requires CMake 3.28+, Ninja generator, GCC 14+, Clang 17+, or MSVC 2022. "
                   "Run: cmake -G Ninja ...")
endif()

# Ex5 (parallel) requires TBB on Linux.
find_package(TBB QUIET)
if(TBB_FOUND OR APPLE OR WIN32)
    add_subdirectory(ex5_parallel_transform)
else()
    message(STATUS "Phase 7 Ex5 (parallel): TBB not found. "
                   "Install libtbb-dev and reconfigure, or run on macOS/Windows.")
endif()
