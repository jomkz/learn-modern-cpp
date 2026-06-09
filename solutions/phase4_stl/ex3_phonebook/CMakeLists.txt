# phase4_stl/ex3_phonebook/CMakeLists.txt

add_library(ph4_phonebook STATIC phonebook.cpp)
target_include_directories(ph4_phonebook PUBLIC .)
target_enable_warnings(ph4_phonebook)

add_executable(ph4_ex3_tests test_phonebook.cpp)
target_link_libraries(ph4_ex3_tests PRIVATE ph4_phonebook Catch2::Catch2WithMain)
target_enable_warnings(ph4_ex3_tests)

include(CTest)
include(Catch)
catch_discover_tests(ph4_ex3_tests)
