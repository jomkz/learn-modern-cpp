# phase4_stl/ex4_histogram/CMakeLists.txt

add_executable(ph4_ex4_histogram histogram.cpp)
target_enable_warnings(ph4_ex4_histogram)

add_executable(ph4_ex4_tests test_histogram.cpp)
target_link_libraries(ph4_ex4_tests PRIVATE Catch2::Catch2WithMain)
target_enable_warnings(ph4_ex4_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph4_ex4_tests)
