# phase1_tooling/ex3_catch2/CMakeLists.txt

# Build a static library for math_utils so both the test executable and a
# future main executable can link against it without compiling it twice.
add_library(ph1_math_utils STATIC math_utils.cpp)
target_include_directories(ph1_math_utils PUBLIC .)
target_enable_warnings(ph1_math_utils)

# Test executable. Catch2::Catch2WithMain provides main(); our test file only
# needs to include catch2/catch_test_macros.hpp and define TEST_CASEs.
add_executable(ph1_ex3_tests test_math_utils.cpp)
target_link_libraries(ph1_ex3_tests
    PRIVATE
        ph1_math_utils
        Catch2::Catch2WithMain  # from cmake/FetchCatch2.cmake
)
target_enable_warnings(ph1_ex3_tests)

# catch_discover_tests scans the binary for Catch2 TEST_CASEs and registers
# each one as a separate CTest test, so `ctest --output-on-failure` shows
# individual test results rather than a single pass/fail for the whole binary.
include(CTest)
include(Catch)
catch_discover_tests(ph1_ex3_tests)
