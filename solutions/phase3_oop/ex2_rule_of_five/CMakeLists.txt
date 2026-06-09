# phase3_oop/ex2_rule_of_five/CMakeLists.txt
# Build with ASan to verify no double-free or leak:
#   cmake -B build-asan -DENABLE_ASAN=ON && cmake --build build-asan

add_library(ph3_buffer_rof STATIC buffer.cpp)
target_include_directories(ph3_buffer_rof PUBLIC .)
target_enable_warnings(ph3_buffer_rof)
target_enable_asan(ph3_buffer_rof)

add_executable(ph3_ex2_rule_of_five main.cpp)
target_link_libraries(ph3_ex2_rule_of_five PRIVATE ph3_buffer_rof)
target_enable_warnings(ph3_ex2_rule_of_five)
target_enable_asan(ph3_ex2_rule_of_five)
