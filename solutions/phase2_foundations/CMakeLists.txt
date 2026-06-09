# phase2_foundations/CMakeLists.txt
# Ex1 (compilation stages) is a shell script exercise — no CMake target needed.
# Ex2 ODR broken build is intentionally excluded from the build (it links with
# an error). Only the fixed version and ex3 are added as CMake targets.

add_subdirectory(ex2_odr_violation/fixed)
add_subdirectory(ex3_rpn_calculator)
