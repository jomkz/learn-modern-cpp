# phase8_professional/ex3_matrix_profile/CMakeLists.txt
# Build with -O3 so the benchmark reflects realistic performance.
# Use -O0 -g to build a debug version for perf/Instruments profiling.

add_library(ph8_naive_lib STATIC naive/matmul.cpp)
add_library(ph8_optimized_lib STATIC optimized/matmul.cpp)

target_compile_options(ph8_naive_lib     PRIVATE -O3)
target_compile_options(ph8_optimized_lib PRIVATE -O3)

add_executable(ph8_ex3_matmul_bench bench.cpp)
target_link_libraries(ph8_ex3_matmul_bench
    PRIVATE ph8_naive_lib ph8_optimized_lib)
target_compile_options(ph8_ex3_matmul_bench PRIVATE -O3)
