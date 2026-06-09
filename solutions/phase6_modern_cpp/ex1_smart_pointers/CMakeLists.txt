# phase6_modern_cpp/ex1_smart_pointers/CMakeLists.txt
# Build with ASan to confirm the raw version leaks and the smart version doesn't:
#   cmake -B build-asan -DENABLE_ASAN=ON && cmake --build build-asan
#   ./build-asan/phase6_modern_cpp/ex1_smart_pointers/ph6_ex1_raw_demo
#   ./build-asan/phase6_modern_cpp/ex1_smart_pointers/ph6_ex1_smart_demo

add_executable(ph6_ex1_raw_demo raw_version/main_raw.cpp)
target_include_directories(ph6_ex1_raw_demo PRIVATE .)
target_enable_warnings(ph6_ex1_raw_demo)
target_enable_asan(ph6_ex1_raw_demo)
# Suppress the -Werror for this file since it intentionally leaks (for demo purposes).
target_compile_options(ph6_ex1_raw_demo PRIVATE -Wno-error)

add_executable(ph6_ex1_smart_demo smart_version/main_smart.cpp)
target_include_directories(ph6_ex1_smart_demo PRIVATE .)
target_enable_warnings(ph6_ex1_smart_demo)
target_enable_asan(ph6_ex1_smart_demo)
