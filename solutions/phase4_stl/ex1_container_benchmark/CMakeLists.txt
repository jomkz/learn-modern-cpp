# phase4_stl/ex1_container_benchmark/CMakeLists.txt
# Build with optimisations so the benchmark reflects realistic performance.
# Debug builds (-O0) mask the cache-locality differences between containers.

add_executable(ph4_ex1_bench bench.cpp)
target_enable_warnings(ph4_ex1_bench)

# Apply -O2 even in Debug builds so the benchmark is meaningful.
target_compile_options(ph4_ex1_bench PRIVATE
    $<$<NOT:$<CONFIG:Release>>:-O2>
)
