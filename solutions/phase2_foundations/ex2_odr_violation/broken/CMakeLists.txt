# phase2_foundations/ex2_odr_violation/broken/CMakeLists.txt
#
# This target deliberately FAILS to link. Run cmake --build . and observe:
#
#   ld: error: duplicate symbol: helper()
#   >>> defined in a.cpp.o
#   >>> defined in b.cpp.o
#
# The two .cpp files both define helper(), which violates the One Definition Rule.
# Each file compiles successfully; the error only appears during linking.
#
# To reproduce the error:
#   cmake -B build_broken -S broken
#   cmake --build build_broken   ← this will fail with "multiple definition"
#
# This target is NOT included in the parent CMakeLists.txt to keep the normal
# build passing. Uncomment the add_subdirectory in phase2's CMakeLists.txt to
# deliberately observe the linker error.

add_executable(ph2_ex2_broken_odr
    a.cpp
    b.cpp
    main.cpp
)
target_enable_warnings(ph2_ex2_broken_odr)
