// Phase 8, Exercise 2 — clang-tidy (BEFORE: legacy code with violations)
// This file intentionally contains patterns that clang-tidy's modernize-*,
// cppcoreguidelines-*, and readability-* checks flag as violations.
// See after/modern_code.cpp for the fixed version.
//
// Run clang-tidy on this file to see all warnings:
//   clang-tidy before/legacy_code.cpp \
//     --checks='modernize-*,cppcoreguidelines-*,readability-*' \
//     -- -std=c++20

#include <cstdio>
#include <string>
#include <vector>

// VIOLATION: C-style cast
// modernize-use-auto / cppcoreguidelines-pro-type-cstyle-cast
double ratio(int a, int b) {
    return (double)a / (double)b;  // NOLINT is NOT used — fix it instead
}

// VIOLATION: raw pointer / naked new
// cppcoreguidelines-owning-memory, modernize-make-unique
void process_raw() {
    int* data = new int[10];
    for (int i = 0; i < 10; ++i) {
        data[i] = i * 2;
    }
    // VIOLATION: no delete — memory leak
    // (deliberately missing to show what clang-tidy catches)
    delete[] data;
}

// VIOLATION: printf instead of std::cout
// modernize-use-default-member-init, readability-identifier-naming
void print_value(int x) {
    printf("value = %d\n", x);  // cppcoreguidelines-pro-type-vararg
}

// VIOLATION: no override keyword
// modernize-use-override
class Base {
public:
    virtual ~Base() = default;
    virtual void do_something() = 0;
};

class Derived : public Base {
public:
    void do_something() {  // missing 'override'
        printf("derived\n");  // also: printf
    }
};

// VIOLATION: NULL instead of nullptr
// modernize-use-nullptr
void check_null(int* p) {
    if (p == NULL) {  // use nullptr instead
        printf("null\n");
    }
}

// VIOLATION: auto not used for obvious types
// modernize-use-auto
void demo_auto() {
    std::vector<int> v = {1, 2, 3};
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        printf("%d\n", *it);  // also: printf
    }
}

int main() {
    printf("ratio = %f\n", ratio(1, 3));
    process_raw();
    print_value(42);
    check_null(NULL);
    demo_auto();
    return 0;
}
