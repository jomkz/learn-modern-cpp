# phase8_professional/ex1_use_after_free/CMakeLists.txt

# Buggy version — debug symbols, no optimisation so GDB gives readable output.
add_executable(ph8_ex1_buggy buggy/main.cpp)
target_compile_options(ph8_ex1_buggy PRIVATE -g3 -O0)
# Intentionally do NOT enable ASan here so learners can first use GDB.
# To run with ASan: cmake -DENABLE_ASAN=ON
target_enable_asan(ph8_ex1_buggy)
# Don't make warnings fatal for the buggy demo (it contains intentional issues).
if(NOT MSVC)
    target_compile_options(ph8_ex1_buggy PRIVATE -w)
endif()

# Fixed version — clean code, ASan confirms no issues.
add_executable(ph8_ex1_fixed fixed/main.cpp)
target_compile_options(ph8_ex1_fixed PRIVATE -g3 -O0)
target_enable_warnings(ph8_ex1_fixed)
target_enable_asan(ph8_ex1_fixed)
