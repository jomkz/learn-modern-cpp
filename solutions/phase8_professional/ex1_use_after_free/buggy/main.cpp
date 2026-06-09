// Phase 8, Exercise 1 — Use-After-Free (BUGGY version)
// Concept: Detecting UAF with GDB and AddressSanitizer.
// Demonstrates: deliberate UAF bug, GDB workflow, ASan output.
//
// A use-after-free (UAF) is a form of undefined behaviour where a program
// continues to access memory after it has been freed (delete[]'d or free()'d).
// The memory may be reallocated to another object, corrupt heap metadata, or
// appear to work until production — making UAF one of the hardest bugs to find
// without tools.
//
// To find this bug with GDB:
//   1. Build with -g3 -O0 (debug symbols, no optimisation):
//        cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build
//   2. Run under GDB:
//        gdb ./build/phase8_professional/ex1_use_after_free/ph8_ex1_buggy
//   3. In GDB: run
//   4. The program crashes (SIGSEGV or similar). Type:
//        backtrace      (or bt) — shows the call stack
//        up             — move to the calling frame
//        info locals    — print local variables in this frame
//        print ptr      — shows the dangling pointer value
//
// To find this bug with AddressSanitizer (faster, better stack trace):
//   cmake -B build-asan -DENABLE_ASAN=ON && cmake --build build-asan
//   ./build-asan/phase8_professional/ex1_use_after_free/ph8_ex1_buggy
//   ASan output: "==ERROR: AddressSanitizer: heap-use-after-free"
//   with the exact line number.
//
// Fix: see fixed/main.cpp — uses std::unique_ptr.

#include <iostream>

struct Foo {
    int value;
    explicit Foo(int v) : value(v) {}
};

static void use_foo(Foo* foo) {
    // UAF HERE: foo points to freed memory. The value may look correct
    // (memory not yet overwritten) or may be garbage — undefined behaviour.
    // With ASan enabled, this line produces the definitive error report.
    std::cout << "value = " << foo->value << '\n';  // ← UAF
}

int main() {
    Foo* ptr = new Foo(42);
    std::cout << "Allocated Foo at " << ptr << "\n";

    // Explicitly delete the object.
    delete ptr;
    std::cout << "Freed Foo\n";

    // BUG: use 'ptr' after it has been freed.
    // Setting ptr = nullptr after delete would make this a null dereference
    // (also wrong, but caught differently). The real danger is when the
    // stale pointer is used without realising it was freed.
    use_foo(ptr);  // ← undefined behaviour: accessing freed memory

    return 0;
}
