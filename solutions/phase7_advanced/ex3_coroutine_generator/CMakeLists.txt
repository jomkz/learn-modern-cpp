add_executable(ph7_ex3_tests test_generator.cpp)
target_include_directories(ph7_ex3_tests PRIVATE .)
target_link_libraries(ph7_ex3_tests PRIVATE Catch2::Catch2WithMain)
target_enable_warnings(ph7_ex3_tests)

# Coroutines require explicit opt-in on some older compilers.
# GCC: -fcoroutines (needed for GCC < 14 with -std=c++20)
# Clang: coroutines are always on with -std=c++20 (no extra flag needed)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14")
    target_compile_options(ph7_ex3_tests PRIVATE -fcoroutines)
endif()

include(CTest)
include(Catch)
catch_discover_tests(ph7_ex3_tests)
