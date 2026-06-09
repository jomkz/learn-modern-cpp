# phase2_foundations/ex2_odr_violation/fixed/CMakeLists.txt
# This target links successfully because helper() has exactly one definition.

add_executable(ph2_ex2_fixed_odr
    helper.cpp   # the one and only definition of helper()
    a.cpp
    b.cpp
    main.cpp
)
target_include_directories(ph2_ex2_fixed_odr PRIVATE .)
target_enable_warnings(ph2_ex2_fixed_odr)
