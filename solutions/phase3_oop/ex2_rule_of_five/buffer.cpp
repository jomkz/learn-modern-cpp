// Phase 3, Exercise 2 — Rule of Five

#include "buffer.h"

#include <algorithm>   // std::copy_n
#include <cstring>     // std::memset
#include <iostream>    // std::cerr (debug print)
#include <ostream>
#include <stdexcept>   // std::invalid_argument
#include <utility>     // std::exchange

Buffer::Buffer(std::size_t size)
    : data_(size > 0 ? new char[size] : nullptr)
    , size_(size)
{
    // Zero-initialise so we don't expose uninitialised memory to callers.
    if (data_) std::memset(data_, 0, size_);
}

// ── 1/5 Destructor ────────────────────────────────────────────────────────────
Buffer::~Buffer() {
    // delete[] on nullptr is a no-op, so we don't need a null check.
    // delete[] matches new[] — mixing new/delete[] or new[]/delete is UB.
    delete[] data_;
}

// ── 2/5 Copy Constructor ──────────────────────────────────────────────────────
Buffer::Buffer(const Buffer& other)
    : data_(other.size_ > 0 ? new char[other.size_] : nullptr)
    , size_(other.size_)
{
    // Deep copy — we own a fresh allocation, independent of 'other'.
    if (data_) std::copy_n(other.data_, size_, data_);
}

// ── 3/5 Copy Assignment ───────────────────────────────────────────────────────
Buffer& Buffer::operator=(const Buffer& other) {
    // Self-assignment guard: if someone writes 'b = b', do nothing.
    // Without this check we'd free our own data before copying from it.
    if (this == &other) return *this;

    // Free old allocation before acquiring new one.
    delete[] data_;

    size_ = other.size_;
    data_ = (size_ > 0) ? new char[size_] : nullptr;
    if (data_) std::copy_n(other.data_, size_, data_);

    return *this;
    // PITFALL: This implementation is NOT exception-safe. If new[] throws,
    // data_ is now a dangling pointer (we deleted it but haven't replaced it).
    // See Phase 3 Ex3 (copy-and-swap) for the correct approach.
}

// ── 4/5 Move Constructor ──────────────────────────────────────────────────────
Buffer::Buffer(Buffer&& other) noexcept
    // std::exchange(old, new): atomically reads old value and replaces it.
    // other.data_ becomes nullptr (preventing double-free in other's dtor),
    // and we receive the original pointer. More concise than two lines.
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
{
    // No allocation: O(1). The moved-from object 'other' is now in a valid
    // but empty state — it's safe to destroy (dtor will delete[] nullptr).
}

// ── 5/5 Move Assignment ───────────────────────────────────────────────────────
Buffer& Buffer::operator=(Buffer&& other) noexcept {
    // Self-move guard (rare but possible with std::move(b) = b).
    if (this == &other) return *this;

    // Release current resource before stealing 'other's.
    delete[] data_;

    data_ = std::exchange(other.data_, nullptr);
    size_ = std::exchange(other.size_, 0);
    return *this;
}

void Buffer::print(std::ostream& os) const {
    os << "Buffer{data=" << static_cast<const void*>(data_)
       << ", size=" << size_ << "}\n";
}
