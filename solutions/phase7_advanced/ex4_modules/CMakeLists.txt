# phase7_advanced/ex4_modules/CMakeLists.txt
# C++20 Modules require CMake 3.28+ and a modern compiler.
# This file is only included when the version guard in the parent CMakeLists.txt
# passes. If your build fails here, check the README.md in this directory.

cmake_minimum_required(VERSION 3.28)

# The module interface unit (myutil.cppm) must be declared in a FILE_SET
# of type CXX_MODULES so CMake can track the module dependency and build
# the Binary Module Interface (.pcm / .ifc) before compiling consumers.
add_library(ph7_myutil)
target_sources(ph7_myutil
    PUBLIC
    FILE_SET CXX_MODULES FILES myutil.cppm
)
set_target_properties(ph7_myutil PROPERTIES CXX_STANDARD 20)

add_executable(ph7_ex4_modules_main main.cpp)
target_link_libraries(ph7_ex4_modules_main PRIVATE ph7_myutil)
target_enable_warnings(ph7_ex4_modules_main)
set_target_properties(ph7_ex4_modules_main PROPERTIES CXX_STANDARD 20)
