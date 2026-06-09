# phase3_oop/ex3_copy_and_swap/CMakeLists.txt

add_library(ph3_buffer_cas STATIC buffer.cpp)
target_include_directories(ph3_buffer_cas PUBLIC .)
target_enable_warnings(ph3_buffer_cas)

add_executable(ph3_ex3_tests test_copy_swap.cpp)
target_link_libraries(ph3_ex3_tests PRIVATE ph3_buffer_cas Catch2::Catch2WithMain)
target_enable_warnings(ph3_ex3_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph3_ex3_tests)
