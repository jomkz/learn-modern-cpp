# phase7_advanced/ex5_parallel_transform/CMakeLists.txt
# Requires TBB on Linux; included only when TBB is found (see parent CMakeLists).

add_executable(ph7_ex5_parallel_bench parallel_bench.cpp)
target_enable_warnings(ph7_ex5_parallel_bench)

# -O3: full optimisation so the benchmark reflects realistic performance.
target_compile_options(ph7_ex5_parallel_bench PRIVATE -O3)

# Link TBB on Linux (required for libstdc++ parallel algorithms).
# On macOS and Windows the runtime is built-in.
if(TBB_FOUND)
    target_link_libraries(ph7_ex5_parallel_bench PRIVATE TBB::tbb)
endif()
