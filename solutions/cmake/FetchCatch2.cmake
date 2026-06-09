# cmake/FetchCatch2.cmake
# Downloads Catch2 v3 at configure time using CMake's FetchContent module.
#
# Why FetchContent instead of vcpkg/Conan?
#   FetchContent is built into CMake — no extra tooling required. It clones
#   the exact tagged version into the build tree, so the project is fully
#   self-contained. For a learning project this is the simplest setup.
#
# Usage in exercise CMakeLists.txt:
#   target_link_libraries(my_tests PRIVATE Catch2::Catch2WithMain)
#   include(CTest)
#   include(Catch)          # provided by Catch2; auto-discovers TEST_CASEs
#   catch_discover_tests(my_tests)
#
# Catch2::Catch2WithMain supplies its own main() that parses CLI options
# (e.g., --reporter, --filter) so test files don't need a main().

include(FetchContent)

# Use a local Catch2 source if network is unavailable, otherwise fetch from GitHub.
# The SOURCE_DIR approach avoids network access and is reproducible offline.
set(_CATCH2_LOCAL_SRC "" CACHE PATH "Optional path to a local Catch2 source tree")

# Try known local locations first (e.g., an existing project in the same workspace).
if(NOT _CATCH2_LOCAL_SRC)
    foreach(_candidate
        "${CMAKE_CURRENT_LIST_DIR}/../../../fighters-legacy/build/coverage/_deps/catch2-src"
    )
        if(EXISTS "${_candidate}/CMakeLists.txt")
            set(_CATCH2_LOCAL_SRC "${_candidate}")
            break()
        endif()
    endforeach()
endif()

if(_CATCH2_LOCAL_SRC)
    message(STATUS "Catch2: using local source at ${_CATCH2_LOCAL_SRC}")
    FetchContent_Declare(
        Catch2
        SOURCE_DIR "${_CATCH2_LOCAL_SRC}"
    )
else()
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.3   # pin to a stable release for reproducibility
        GIT_SHALLOW    TRUE     # fetch only the tagged commit, not full history
    )
endif()

# FetchContent_MakeAvailable downloads (if not cached) and adds the
# subdirectory, making Catch2::Catch2WithMain available as a CMake target.
FetchContent_MakeAvailable(Catch2)

# Add Catch2's cmake/ directory to the module path so include(Catch) works.
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
