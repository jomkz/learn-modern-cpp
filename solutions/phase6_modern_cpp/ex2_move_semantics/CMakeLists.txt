add_library(ph6_buffer STATIC buffer.cpp)
target_include_directories(ph6_buffer PUBLIC .)
target_enable_warnings(ph6_buffer)

add_executable(ph6_ex2_tests test_move.cpp)
target_link_libraries(ph6_ex2_tests PRIVATE ph6_buffer Catch2::Catch2WithMain)
target_enable_warnings(ph6_ex2_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph6_ex2_tests)
