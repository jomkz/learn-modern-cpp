// Phase 6, Exercise 1 — Smart Pointers (BEFORE: raw new/delete)
// This is the PROBLEMATIC version that smart pointers replace.
// PITFALL: Run with ASan to see the leak when early_return() is called:
//   cmake -B build -DENABLE_ASAN=ON && cmake --build build
//   ./build/phase6_modern_cpp/ex1_smart_pointers/ph6_ex1_raw_demo
//
// Problems with raw new/delete:
//   1. Every new must be paired with exactly one delete — easy to forget.
//   2. If an exception is thrown (or early return taken) before delete,
//      the memory leaks. There is no automatic cleanup.
//   3. Transfer of ownership is implicit — it's unclear who is responsible
//      for deletion.

#include <iostream>
#include <vector>
#include "../animal.h"

// This function leaks if we return early.
static void early_return_demo() {
    Animal* a = new Dog("Buddy");  // allocated on heap
    a->speak();

    // Imagine an early return here (error check, branch, etc.).
    // The delete below is never reached.
    if (true) return;  // ← LEAK: delete never called

    delete a;  // only reached if no early return
}

int main() {
    // ── Case 1: Correct cleanup (fragile) ─────────────────────────────────────
    std::vector<Animal*> animals;
    animals.push_back(new Dog("Rex"));
    animals.push_back(new Cat("Whiskers"));
    animals.push_back(new Dog("Fido"));

    for (Animal* a : animals) {
        a->speak();
    }

    // Manual deletion — must remember to do this for EVERY pointer.
    // If we throw before this loop, all three objects leak.
    for (Animal* a : animals) {
        delete a;
    }
    animals.clear();

    // ── Case 2: Leak via early return ─────────────────────────────────────────
    std::cout << "\nDemonstrating early-return leak (see ASan output):\n";
    early_return_demo();

    return 0;
}
