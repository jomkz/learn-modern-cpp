// Phase 6, Exercise 1 — Smart Pointers (AFTER: unique_ptr and shared_ptr)
// Demonstrates how smart pointers eliminate every problem of the raw version.
// Run with ASan to confirm zero leaks.

#include <iostream>
#include <memory>   // std::unique_ptr, std::shared_ptr, std::make_unique, std::make_shared
#include <vector>
#include "../animal.h"

// ── unique_ptr: exclusive ownership ──────────────────────────────────────────
// unique_ptr<T> is a zero-overhead RAII wrapper around a T*. It:
//   - Deletes the T when it goes out of scope (even on exception/early return).
//   - Cannot be copied (exclusive ownership), only moved.
//   - Has no runtime overhead over a raw pointer (-O2 generates identical asm).
//
// make_unique<T>(args...) is preferred over new:
//   raw:    Animal* a = new Dog("x");  // allocation and assignment are separate
//   smart:  auto a = std::make_unique<Dog>("x");  // atomic, exception-safe

static void early_return_demo() {
    // No leak possible: unique_ptr destructor runs even on early return.
    auto a = std::make_unique<Dog>("Buddy");
    a->speak();
    if (true) return;  // ← NOT a leak: unique_ptr destructs automatically
}

// ── shared_ptr: shared reference-counted ownership ────────────────────────────
// shared_ptr<T> allows multiple owners. The T is destroyed when the last
// shared_ptr to it is destroyed. Each copy increments a reference counter;
// each destruction decrements it.
//
// Use case: objects that need to outlive their creator and be shared between
// components (e.g., a resource cached in multiple places).

static void shared_ownership_demo() {
    // One animal shared between two "shelters".
    auto chief = std::make_shared<Dog>("Chief");
    std::cout << "Chief shared: use_count=" << chief.use_count() << '\n';  // 1

    {
        // weak_ptr observes a shared_ptr without extending lifetime.
        // Used to break reference cycles (e.g., parent/child with back-pointers).
        std::weak_ptr<Dog> observer = chief;
        std::cout << "After weak_ptr: use_count=" << chief.use_count() << '\n';  // still 1

        // To access the object, lock() promotes weak_ptr to shared_ptr temporarily.
        if (auto locked = observer.lock()) {
            locked->speak();
        }
    }
    // observer is gone; chief's use_count is still 1.
    // When 'chief' goes out of scope, the Dog is destroyed.
}

int main() {
    std::cout << "── unique_ptr collection ───────────────────────────────\n";

    // Polymorphic collection using unique_ptr. The vector owns the objects.
    // No manual delete needed — the vector's destructor calls each unique_ptr's
    // destructor, which calls delete on the underlying Animal*.
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Rex"));
    animals.push_back(std::make_unique<Cat>("Whiskers"));
    animals.push_back(std::make_unique<Dog>("Fido"));

    for (const auto& a : animals) {
        a->speak();
    }

    std::cout << "\n── early return (no leak) ───────────────────────────────\n";
    early_return_demo();

    std::cout << "\n── shared_ptr and weak_ptr ─────────────────────────────\n";
    shared_ownership_demo();

    std::cout << "\nAll objects destroyed automatically. Run with ASan to confirm.\n";
    return 0;
}
