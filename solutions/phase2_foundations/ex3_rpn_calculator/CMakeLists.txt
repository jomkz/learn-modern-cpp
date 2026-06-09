# phase2_foundations/ex3_rpn_calculator/CMakeLists.txt

add_library(ph2_rpn_lib STATIC rpn.cpp)
target_include_directories(ph2_rpn_lib PUBLIC .)
target_enable_warnings(ph2_rpn_lib)

add_executable(ph2_ex3_rpn main.cpp)
target_link_libraries(ph2_ex3_rpn PRIVATE ph2_rpn_lib)
target_enable_warnings(ph2_ex3_rpn)

add_executable(ph2_ex3_rpn_tests test_rpn.cpp)
target_link_libraries(ph2_ex3_rpn_tests
    PRIVATE ph2_rpn_lib Catch2::Catch2WithMain)
target_enable_warnings(ph2_ex3_rpn_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph2_ex3_rpn_tests)
