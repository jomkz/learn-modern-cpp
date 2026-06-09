// Phase 3, Exercise 2 — Rule of Five
// Concept: When a class manually manages a resource, it must define all five
//          special member functions (Rule of Five).
// Demonstrates: destructor, copy constructor, copy assignment operator,
//               move constructor, move assignment operator.
//
// The Rule of Five (C++11+):
//   If you define ANY of: destructor, copy ctor, copy assign, move ctor,
//   move assign — define ALL five. The compiler-generated defaults are wrong
//   for a resource-owning class (shallow copy leads to double-free).
//
// Run with ASan to confirm no double-free or leak:
//   cmake -B build -DENABLE_ASAN=ON && cmake --build build
//
// Cross-reference: Phase 6 Ex2 shows how unique_ptr/Rule of Zero eliminates
//                  the need for all this manual work.

#pragma once

#include <cstddef>   // std::size_t
#include <ostream>   // std::ostream for debug printing

class Buffer {
public:
    // ── Constructor ───────────────────────────────────────────────────────────
    // Allocates 'size' bytes on the heap. Throws std::bad_alloc if out of memory.
    explicit Buffer(std::size_t size);

    // ── 1/5 Destructor ────────────────────────────────────────────────────────
    // Releases the heap memory. Without this, every Buffer constructed on the
    // heap would leak. The compiler-generated destructor just destroys members
    // (the pointer and size), but does NOT call delete[] on what they point to.
    ~Buffer();

    // ── 2/5 Copy Constructor ──────────────────────────────────────────────────
    // Creates a new Buffer with its own copy of 'other's data (deep copy).
    // The compiler-generated version copies the pointer — then two Buffer
    // objects point to the same heap block. When one is destroyed, the pointer
    // in the other becomes dangling — a classic use-after-free.
    Buffer(const Buffer& other);

    // ── 3/5 Copy Assignment Operator ─────────────────────────────────────────
    // Replaces this Buffer's content with a copy of 'other's content.
    // Must handle self-assignment (buf = buf) without corrupting state.
    // See Phase 3 Ex3 for the preferred copy-and-swap implementation.
    Buffer& operator=(const Buffer& other);

    // ── 4/5 Move Constructor ──────────────────────────────────────────────────
    // "Steals" other's heap block — O(1) regardless of buffer size.
    // After the move, 'other' must be in a valid but unspecified state:
    // it must be safe to destroy, but its content is unspecified.
    // We set other.data_ = nullptr so its destructor doesn't double-free.
    Buffer(Buffer&& other) noexcept;

    // ── 5/5 Move Assignment Operator ─────────────────────────────────────────
    // Releases our current allocation, then steals 'other's allocation.
    Buffer& operator=(Buffer&& other) noexcept;

    // ── Accessors ─────────────────────────────────────────────────────────────
    char*       data()  noexcept { return data_; }
    const char* data()  const noexcept { return data_; }
    std::size_t size()  const noexcept { return size_; }
    bool        empty() const noexcept { return size_ == 0; }

    // Debug helper — prints the buffer address and content.
    void print(std::ostream& os) const;

private:
    char*       data_;  // heap-allocated array of 'size_' bytes
    std::size_t size_;  // number of bytes allocated
};
