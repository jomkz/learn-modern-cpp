// Phase 3, Exercise 3 — Copy-and-Swap Idiom

#include "buffer.h"

#include <algorithm>   // std::copy_n
#include <cstring>
#include <ostream>
#include <utility>     // std::exchange, std::swap

Buffer::Buffer(std::size_t size)
    : data_(size > 0 ? new char[size] : nullptr)
    , size_(size)
{
    if (data_) std::memset(data_, 0, size_);
}

Buffer::~Buffer() {
    delete[] data_;
}

Buffer::Buffer(const Buffer& other)
    : data_(other.size_ > 0 ? new char[other.size_] : nullptr)
    , size_(other.size_)
{
    if (data_) std::copy_n(other.data_, size_, data_);
}

Buffer::Buffer(Buffer&& other) noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
{}

// ── swap ─────────────────────────────────────────────────────────────────────
// Simply swap each member. No allocation, no exception possible.
void swap(Buffer& a, Buffer& b) noexcept {
    std::swap(a.data_, b.data_);
    std::swap(a.size_, b.size_);
}

// ── Copy Assignment via copy-and-swap ─────────────────────────────────────────
// How this achieves the strong exception guarantee:
//
//   Step 1: 'rhs' is constructed (copy or move) from the caller's argument.
//           If the copy constructor throws here, our object (*this) is
//           UNCHANGED — the exception escapes before we touch *this at all.
//           This is the key to the strong guarantee.
//
//   Step 2: swap(*this, rhs) — no-throw by design. After the swap,
//           *this holds the new data, and 'rhs' holds our old data.
//
//   Step 3: 'rhs' goes out of scope. Its destructor frees our old data.
//
// Self-assignment is handled correctly for free: if 'this == &caller_obj',
// then 'rhs' is a copy of *this (or a no-op for moved objects). The swap
// exchanges identical data, then 'rhs' destroys the copy. *this is unchanged.
Buffer& Buffer::operator=(Buffer rhs) noexcept {
    swap(*this, rhs);
    return *this;
}

void Buffer::print(std::ostream& os) const {
    os << "Buffer{data=" << static_cast<const void*>(data_)
       << ", size=" << size_ << "}\n";
}
