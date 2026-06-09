# phase3_oop/ex4_matrix_operators/CMakeLists.txt

add_library(ph3_matrix STATIC matrix.cpp)
target_include_directories(ph3_matrix PUBLIC .)
target_enable_warnings(ph3_matrix)

add_executable(ph3_ex4_tests test_matrix.cpp)
target_link_libraries(ph3_ex4_tests PRIVATE ph3_matrix Catch2::Catch2WithMain)
target_enable_warnings(ph3_ex4_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph3_ex4_tests)
