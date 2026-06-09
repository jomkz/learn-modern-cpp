# phase3_oop/ex1_raii_filehandle/CMakeLists.txt

add_library(ph3_file_handle STATIC file_handle.cpp)
target_include_directories(ph3_file_handle PUBLIC .)
target_enable_warnings(ph3_file_handle)

add_executable(ph3_ex1_raii_demo main.cpp)
target_link_libraries(ph3_ex1_raii_demo PRIVATE ph3_file_handle)
target_enable_warnings(ph3_ex1_raii_demo)

add_executable(ph3_ex1_tests test_file_handle.cpp)
target_link_libraries(ph3_ex1_tests PRIVATE ph3_file_handle Catch2::Catch2WithMain)
target_enable_warnings(ph3_ex1_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph3_ex1_tests)
