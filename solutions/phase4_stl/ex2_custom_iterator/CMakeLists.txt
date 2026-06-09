# phase4_stl/ex2_custom_iterator/CMakeLists.txt
# Header-only implementation; no library needed.

add_executable(ph4_ex2_tests test_iterator.cpp)
target_include_directories(ph4_ex2_tests PRIVATE .)
target_link_libraries(ph4_ex2_tests PRIVATE Catch2::Catch2WithMain)
target_enable_warnings(ph4_ex2_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph4_ex2_tests)
