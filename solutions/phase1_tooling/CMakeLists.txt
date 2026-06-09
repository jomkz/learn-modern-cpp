# phase1_tooling/CMakeLists.txt
# Phase 1 has no compiled library targets of its own — ex1 is compiled manually
# from the command line (the whole point of the exercise). Only ex2 and ex3
# need CMake targets.

add_subdirectory(ex2_two_file_project)
add_subdirectory(ex3_catch2)
