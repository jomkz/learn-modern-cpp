# phase8_professional/ex4_producer_consumer/CMakeLists.txt

add_executable(ph8_ex4_tests test_producer_consumer.cpp)
target_include_directories(ph8_ex4_tests PRIVATE .)
target_link_libraries(ph8_ex4_tests PRIVATE Catch2::Catch2WithMain)
target_enable_warnings(ph8_ex4_tests)

# Link pthreads on Linux — required for std::jthread and condition_variable.
find_package(Threads REQUIRED)
target_link_libraries(ph8_ex4_tests PRIVATE Threads::Threads)

include(CTest)
include(Catch)
catch_discover_tests(ph8_ex4_tests)
