# cmake/CompilerWarnings.cmake
# Provides target_enable_warnings(target) — a function that applies a
# comprehensive, compiler-portable set of warning flags to any CMake target.
#
# Why use this function instead of setting CMAKE_CXX_FLAGS globally?
#   - target_*() commands apply only to the named target; global variables
#     affect every target in the build tree, including third-party libraries
#     that may not compile cleanly under -Wpedantic.
#   - This keeps warning policy explicit and opt-in.

function(target_enable_warnings target)
    if(MSVC)
        # /W4  — warning level 4 (comprehensive)
        # /WX  — treat warnings as errors (forces you to fix them)
        # /permissive- — strict standards conformance; disables MSVC extensions
        target_compile_options(${target} PRIVATE
            /W4
            /WX
            /permissive-
        )
    else()
        # -Wall        — most common correctness warnings
        # -Wextra      — extra warnings not enabled by -Wall
        # -Wpedantic   — flag extensions beyond ISO C++
        # -Wshadow     — warn when a local variable shadows an outer variable
        # -Wnon-virtual-dtor — base class with virtual functions but non-virtual dtor
        # -Wold-style-cast   — flag C-style casts; use static_cast etc. instead
        # -Wconversion       — flag implicit narrowing conversions
        # -Wsign-conversion  — flag signed/unsigned implicit conversions
        # -Werror      — treat warnings as errors so they can't be ignored
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wconversion
            -Wsign-conversion
            -Werror
        )
    endif()
endfunction()
