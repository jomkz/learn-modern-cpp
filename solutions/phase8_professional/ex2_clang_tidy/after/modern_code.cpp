// Phase 8, Exercise 2 — clang-tidy (AFTER: all violations fixed)
// Every warning from the 'before' version has been resolved.
// Each fix is annotated with the clang-tidy check that flagged it.
//
// This file is compiled with clang-tidy enabled (see CMakeLists.txt) so any
// regression immediately breaks the build — the "after" state is verified by CI.

#include <iostream>   // FIXED: use cout, not printf
#include <vector>     // FIXED: use std::vector instead of owning raw array

// FIXED: cppcoreguidelines-pro-type-cstyle-cast
// Use static_cast<double> instead of a C-style (double) cast.
// C-style casts silently apply reinterpret_cast if needed, bypassing type safety.
//
// FIXED: readability-identifier-length
// Parameter names must be descriptive, not single letters — this makes call sites
// self-documenting and avoids ambiguity (is 'a' the numerator or denominator?).
double ratio(int numerator, int denominator) {
    return static_cast<double>(numerator) / static_cast<double>(denominator);
}

// FIXED: cppcoreguidelines-owning-memory, modernize-make-unique
// Use std::vector instead of make_unique<int[]>: vector is bounds-safe, resizable,
// and passes the cppcoreguidelines-pro-bounds-avoid-unchecked-container-access check
// because .at() or range-based-for is used rather than raw operator[].
void process_buffer() {
    std::vector<int> data(10);
    for (std::size_t idx = 0; idx < data.size(); ++idx) {
        data.at(idx) = static_cast<int>(idx) * 2;
    }
    // No delete — vector goes out of scope here and cleans up.
}

// FIXED: cppcoreguidelines-pro-type-vararg
// std::cout is type-safe; printf uses ... (varargs) which bypasses type checking.
//
// FIXED: readability-identifier-length — renamed 'x' to 'val'
void print_value(int val) {
    std::cout << "value = " << val << '\n';
}

// FIXED: modernize-use-override
// 'override' makes the compiler verify the signature matches the base class.
// Without it, a typo in the signature creates a new virtual function instead of
// overriding — a silent bug.
//
// FIXED: cppcoreguidelines-special-member-functions
// If a class declares any special member, it should declare all five (Rule of Five)
// or none (Rule of Zero). Here we use = default / = delete explicitly so the
// compiler enforces the correct semantics and clang-tidy is satisfied.
class Base {
public:
    Base() = default;
    Base(const Base&) = default;
    Base& operator=(const Base&) = default;
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;
    virtual ~Base() = default;

    virtual void do_something() = 0;
};

class Derived : public Base {
public:
    void do_something() override {
        // FIXED: printf → std::cout
        std::cout << "derived\n";
    }
};

// FIXED: modernize-use-nullptr
// nullptr is a strongly-typed null pointer constant (type std::nullptr_t).
// NULL is typically #defined as 0 (int), which causes ambiguity with integer overloads.
//
// FIXED: readability-non-const-parameter — pointer should be const since we don't
// modify the pointed-to value.
// FIXED: readability-identifier-length — renamed 'p' to 'ptr'
void check_null(const int* ptr) {
    if (ptr == nullptr) {
        std::cout << "null\n";
    }
}

// FIXED: modernize-use-auto, readability-qualified-auto, modernize-loop-convert
// Range-based for is preferred over manual iterator loops: simpler, less error-prone,
// and the iterator type is inferred automatically.
// FIXED: readability-identifier-length — renamed 'v' to 'vec'
void demo_auto() {
    std::vector<int> vec = {1, 2, 3};
    for (int value : vec) {
        std::cout << value << '\n';
    }
}

int main() {
    std::cout << "ratio = " << ratio(1, 3) << '\n';
    process_buffer();
    print_value(42);
    check_null(nullptr);
    demo_auto();
    return 0;
}
