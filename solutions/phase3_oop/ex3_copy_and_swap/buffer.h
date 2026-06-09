// Phase 3, Exercise 3 — Copy-and-Swap Idiom
// Concept: Implementing exception-safe copy assignment via copy-and-swap.
// Demonstrates: the swap() pattern, strong exception guarantee, self-assignment
//               handling for free.
//
// The copy-and-swap idiom is the standard recipe for writing a correct,
// exception-safe, self-assignment-safe copy assignment operator using only
// tools you already have (copy constructor + destructor).
//
// Cross-reference: Phase 6 Ex1 shows how std::unique_ptr (Rule of Zero)
//                  makes ALL of this unnecessary.

#pragma once

#include <cstddef>
#include <ostream>

class Buffer {
public:
    explicit Buffer(std::size_t size);
    ~Buffer();

    // Copy constructor — used by copy-and-swap to build the temporary.
    Buffer(const Buffer& other);

    // ── Copy Assignment via copy-and-swap ─────────────────────────────────────
    // Takes 'rhs' by VALUE (not const-reference). The compiler will either:
    //   a) copy-construct 'rhs' from the argument (if called with lvalue), or
    //   b) move-construct 'rhs' from the argument (if called with rvalue).
    // Then we swap our internals with 'rhs', and 'rhs' destructs the old data.
    // One operator= handles both copy-assign and move-assign!
    Buffer& operator=(Buffer rhs) noexcept;

    // Move constructor (still needed for move-assign path).
    Buffer(Buffer&& other) noexcept;

    // ── swap ─────────────────────────────────────────────────────────────────
    // Non-throwing swap that exchanges all member variables. Declared as a
    // friend so ADL (Argument-Dependent Lookup) finds it when callers write:
    //   using std::swap; swap(a, b);
    // noexcept is critical — if swap could throw, the strong guarantee breaks.
    friend void swap(Buffer& a, Buffer& b) noexcept;

    char*       data()  noexcept { return data_; }
    const char* data()  const noexcept { return data_; }
    std::size_t size()  const noexcept { return size_; }

    void print(std::ostream& os) const;

private:
    char*       data_;
    std::size_t size_;
};
