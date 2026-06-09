# phase8_professional/ex2_clang_tidy/CMakeLists.txt
# The "after" version should compile cleanly with clang-tidy enabled.
# The "before" version is not built by CMake (it's for manual inspection).

add_executable(ph8_ex2_modern after/modern_code.cpp)
target_enable_warnings(ph8_ex2_modern)

# Enable clang-tidy on the modern version to verify it's actually clean.
# To run: cmake -DCMAKE_CXX_CLANG_TIDY="clang-tidy" .. && cmake --build .
# To check manually: clang-tidy after/modern_code.cpp -- -std=c++20
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set_target_properties(ph8_ex2_modern PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy"
    )
endif()
