// Phase 8, Exercise 1 — Use-After-Free (FIXED version)
// Fix: use std::unique_ptr. The object is automatically destroyed when the
// unique_ptr goes out of scope. There is no raw pointer to accidentally reuse.
//
// Why unique_ptr prevents UAF:
//   1. The raw pointer is encapsulated — callers work through the smart pointer.
//   2. When unique_ptr is destroyed (scope exit or explicit reset), it deletes
//      the object AND the internal raw pointer becomes inaccessible.
//   3. If the callee needs a raw pointer (e.g., for a C API), it calls .get()
//      explicitly, which makes the access visible in code review.

#include <iostream>
#include <memory>   // std::unique_ptr, std::make_unique

struct Foo {
    int value;
    explicit Foo(int v) : value(v) {}
};

// Accept by raw pointer (from .get()) to match a C API signature.
// The function does NOT own the pointer — it only borrows it.
// The Foo is guaranteed to be alive for the duration of this call.
static void use_foo(const Foo* foo) {
    std::cout << "value = " << foo->value << '\n';
}

int main() {
    // std::make_unique<Foo>(42): allocates and constructs Foo in one step.
    // The unique_ptr owns the Foo; no raw new, no raw delete.
    auto ptr = std::make_unique<Foo>(42);
    std::cout << "Allocated Foo at " << ptr.get() << "\n";

    // Pass the raw pointer via .get(). The unique_ptr retains ownership.
    // 'use_foo' cannot accidentally free or outlive 'ptr'.
    use_foo(ptr.get());

    // 'ptr' goes out of scope here. Its destructor calls delete automatically.
    // There is no stale raw pointer left behind — UAF is structurally impossible.
    std::cout << "Foo destroyed when ptr leaves scope.\n";
    return 0;
}
