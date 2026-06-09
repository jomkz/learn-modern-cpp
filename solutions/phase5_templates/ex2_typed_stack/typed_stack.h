// Phase 5, Exercise 2 — TypedStack with bool partial specialisation
// Concept: Class template partial specialisation, bitfield packing.
// Demonstrates: template<typename T>, template<> (full/partial spec),
//               uint64_t bitfield, space efficiency.
//
// std::vector<bool> is the infamous "broken" STL specialisation that stores
// bits instead of bytes, breaking iterator semantics. This exercise builds a
// correct version of the idea explicitly, making the bitfield mechanics visible.
//
// Primary template: generic stack using std::vector<T>.
// bool specialisation: packs up to 64 booleans into a single uint64_t word.
//   sizeof(TypedStack<bool>) << sizeof(TypedStack<int>) for typical sizes.

#pragma once

#include <cstddef>    // std::size_t
#include <cstdint>    // uint64_t
#include <stdexcept>  // std::underflow_error, std::overflow_error
#include <vector>

// ── Primary template ─────────────────────────────────────────────────────────
template<typename T>
class TypedStack {
public:
    void push(const T& value) { data_.push_back(value); }
    void push(T&& value)      { data_.push_back(std::move(value)); }

    void pop() {
        if (data_.empty()) throw std::underflow_error("TypedStack: pop on empty stack");
        data_.pop_back();
    }

    T& top() {
        if (data_.empty()) throw std::underflow_error("TypedStack: top on empty stack");
        return data_.back();
    }
    const T& top() const {
        if (data_.empty()) throw std::underflow_error("TypedStack: top on empty stack");
        return data_.back();
    }

    std::size_t size()  const noexcept { return data_.size(); }
    bool        empty() const noexcept { return data_.empty(); }

private:
    std::vector<T> data_;
};

// ── Partial specialisation for bool ──────────────────────────────────────────
// Stores up to 64 booleans in a single uint64_t, using bitwise operations.
// Each bit at position i corresponds to the i-th element from the bottom.
//
// Why uint64_t?
//   A std::vector<bool> using one byte per bool wastes 7 bits per element.
//   Packing 64 bools into a uint64_t uses 1/8th the memory.
//   For small stacks this can mean fitting in a CPU register.
template<>
class TypedStack<bool> {
public:
    static constexpr std::size_t CAPACITY = 64;  // one uint64_t holds 64 bits

    void push(bool value) {
        if (size_ >= CAPACITY)
            throw std::overflow_error("TypedStack<bool>: capacity is 64 bits");
        if (value) {
            // Set bit at position size_.
            // (1ULL << n) creates a mask with only bit n set; |= sets that bit.
            bits_ |= (1ULL << size_);
        } else {
            // Clear bit at position size_.
            // ~(1ULL << n) has all bits set except bit n; &= clears that bit.
            bits_ &= ~(1ULL << size_);
        }
        ++size_;
    }

    void pop() {
        if (size_ == 0) throw std::underflow_error("TypedStack<bool>: pop on empty");
        --size_;
        // Optionally clear the vacated bit (not strictly necessary but clean).
        bits_ &= ~(1ULL << size_);
    }

    bool top() const {
        if (size_ == 0) throw std::underflow_error("TypedStack<bool>: top on empty");
        // Check if bit (size_ - 1) is set.
        return (bits_ >> (size_ - 1)) & 1ULL;
    }

    std::size_t size()  const noexcept { return size_; }
    bool        empty() const noexcept { return size_ == 0; }

private:
    uint64_t    bits_ = 0;  // packed booleans
    std::size_t size_ = 0;  // number of elements currently in the stack
};
