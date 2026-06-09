// Phase 6, Exercise 2 — Move Semantics Buffer

#include "buffer.h"
#include <algorithm>   // std::copy_n
#include <cstring>     // std::memset
#include <utility>     // std::exchange

Buffer::Buffer(std::size_t size)
    : data_(size > 0 ? new std::byte[size] : nullptr)
    , size_(size)
{
    if (data_) std::memset(data_, 0, size_);
}

Buffer::~Buffer() {
    delete[] data_;
}

Buffer::Buffer(const Buffer& other)
    : data_(other.size_ > 0 ? new std::byte[other.size_] : nullptr)
    , size_(other.size_)
{
    if (data_) std::copy_n(other.data_, size_, data_);
}

Buffer& Buffer::operator=(const Buffer& other) {
    if (this == &other) return *this;
    delete[] data_;
    size_ = other.size_;
    data_ = (size_ > 0) ? new std::byte[size_] : nullptr;
    if (data_) std::copy_n(other.data_, size_, data_);
    return *this;
}

// ── Move Constructor ──────────────────────────────────────────────────────────
Buffer::Buffer(Buffer&& other) noexcept
    // std::exchange(old, new) reads 'other.data_', replaces it with nullptr,
    // and returns the original value — all in one atomic step.
    // This is why we don't write two separate lines: thread safety aside,
    // the one-liner expresses intent more clearly.
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
{
    // After this constructor:
    //   this->data_ = old other.data_  (we own the allocation)
    //   this->size_ = old other.size_
    //   other.data_ = nullptr           (moved-from: safe to destroy)
    //   other.size_ = 0
    //
    // PITFALL: If you forget to null out other.data_, both *this and 'other'
    // will call delete[] on the same pointer when they're destroyed — double-free.
    // ASan will immediately report this as "heap-use-after-free" or
    // "attempting double-free".
}

// ── Move Assignment ───────────────────────────────────────────────────────────
Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this == &other) return *this;

    // Release our current resource before stealing other's.
    // If we stole first and then freed, we'd free the stolen resource!
    delete[] data_;

    data_ = std::exchange(other.data_, nullptr);
    size_ = std::exchange(other.size_, 0);
    return *this;
}
