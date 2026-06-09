# phase1_tooling/ex2_two_file_project/CMakeLists.txt

# add_executable lists every .cpp file that should be compiled and linked
# together. greet.cpp defines greet(); main.cpp calls it.
add_executable(ph1_ex2_greet
    greet.cpp
    main.cpp
)

# target_include_directories(PRIVATE .) makes the current directory visible
# to the compiler when it processes this target's source files.
# PRIVATE means this include path is NOT propagated to targets that link
# against ph1_ex2_greet. (INTERFACE = propagated only to dependents;
# PUBLIC = both. For an executable, PRIVATE is always correct.)
target_include_directories(ph1_ex2_greet PRIVATE .)

target_enable_warnings(ph1_ex2_greet)
