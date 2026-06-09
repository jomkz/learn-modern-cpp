# cmake/Sanitizers.cmake
# Provides:
#   option ENABLE_ASAN  — toggle AddressSanitizer + UBSan at configure time
#   function target_enable_asan(target) — apply sanitizer flags to a target
#
# Usage:
#   cmake -B build -DENABLE_ASAN=ON
#   cmake --build build
#
# ASan detects: heap/stack/global buffer overflows, use-after-free,
#               use-after-return, double-free, memory leaks.
# UBSan detects: signed integer overflow, null dereference, misaligned access,
#                invalid enum value, and many other forms of undefined behavior.
#
# Note: sanitizers require a debug build (-g) to produce useful stack traces.
#       They are not supported by MSVC; use the /analyze flag there instead.

option(ENABLE_ASAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

function(target_enable_asan target)
    if(ENABLE_ASAN)
        if(MSVC)
            message(WARNING "ASan is not supported via this helper on MSVC. "
                            "Use /fsanitize=address in your project properties.")
            return()
        endif()

        # -fsanitize=address,undefined   — enable both ASan and UBSan
        # -fno-omit-frame-pointer        — keep frame pointers for readable stack traces
        # -g                             — include debug symbols
        # These flags must appear in BOTH compile and link options.
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
        message(STATUS "ASan+UBSan enabled for target: ${target}")
    endif()
endfunction()
