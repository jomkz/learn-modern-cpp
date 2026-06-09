// Phase 7, Exercise 3 — Coroutine Generator
// Concept: C++20 coroutines, promise_type, coroutine_handle.
// Demonstrates: co_yield, suspend_always, coroutine lifecycle.
//
// A coroutine is a function that can suspend execution and be resumed later.
// C++ coroutines are stackless (no separate stack frame per coroutine — they
// heap-allocate a small "coroutine frame" instead).
//
// The low-level machinery requires three types:
//   Generator<T>   — the return type the caller receives
//   promise_type   — nested in Generator<T>; controls the coroutine lifecycle
//   coroutine_handle — an opaque handle that can resume/destroy the coroutine
//
// NOTE: C++23 introduces std::generator<T> in <generator>, which replaces
// this entire hand-rolled implementation. This exercise exists to understand
// the mechanism; in production, use std::generator.
//
// Lifecycle diagram:
//   caller:      Generator<T> gen = fibonacci();  <- creates coroutine frame
//   coroutine:   suspended at initial_suspend
//   caller:      auto it = gen.begin();           <- first resume
//   coroutine:   runs until co_yield value; suspends
//   caller:      *it == value
//   caller:      ++it                             <- resumes coroutine
//   coroutine:   runs until next co_yield; suspends
//   ...
//   caller:      it == gen.end()                  <- coroutine reached end
//   coroutine:   at final_suspend (still alive until Generator destructs)

#pragma once

#include <coroutine>  // std::coroutine_handle, std::suspend_always, std::suspend_never
#include <cstddef>    // std::ptrdiff_t
#include <exception>  // std::rethrow_exception
#include <iterator>   // std::input_iterator_tag
#include <optional>   // std::optional
#include <utility>    // std::exchange

template<typename T>
class Generator {
public:
    // ── promise_type ──────────────────────────────────────────────────────────
    // The compiler looks for a nested 'promise_type' (or a specialisation of
    // std::coroutine_traits) when the function return type is Generator<T>.
    struct promise_type {
        std::optional<T> current_value;  // the yielded value (if any)
        std::exception_ptr exception;    // captured exception (if any)

        // get_return_object: called when the coroutine frame is created.
        // Returns the Generator object that the caller receives.
        Generator<T> get_return_object() {
            return Generator<T>{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        // initial_suspend: called before the coroutine body starts.
        // suspend_always means the coroutine starts SUSPENDED — it won't run
        // until the first time the caller resumes it (via begin()).
        std::suspend_always initial_suspend() noexcept { return {}; }

        // final_suspend: called when the coroutine body returns.
        // suspend_always keeps the coroutine frame alive until the caller's
        // Generator destructor destroys it. Without this, destroying a handle
        // after final_suspend would be a double-free.
        std::suspend_always final_suspend() noexcept { return {}; }

        // yield_value: called by co_yield <expr>.
        // Stores the yielded value and suspends the coroutine.
        std::suspend_always yield_value(T value) noexcept {
            current_value = std::move(value);
            return {};
        }

        // return_void: called when the coroutine function returns normally.
        void return_void() noexcept {}

        // unhandled_exception: called if an exception escapes the coroutine body.
        // We capture it to rethrow on the caller's side.
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    // ── Iterator ──────────────────────────────────────────────────────────────
    struct Iterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;

        std::coroutine_handle<promise_type> handle;

        // operator++ resumes the coroutine, causing it to run until the next
        // co_yield or return.
        Iterator& operator++() {
            handle.resume();
            return *this;
        }

        reference operator*()  const { return *handle.promise().current_value; }
        pointer   operator->() const { return &*handle.promise().current_value; }

        bool operator==(std::default_sentinel_t) const {
            // The coroutine is "done" (at final_suspend) when done() is true.
            return handle.done();
        }
    };

    // ── Generator itself ──────────────────────────────────────────────────────
    explicit Generator(std::coroutine_handle<promise_type> h) : handle_(h) {}

    // Non-copyable: the handle is unique.
    Generator(const Generator&)            = delete;
    Generator& operator=(const Generator&) = delete;

    // Movable.
    Generator(Generator&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}

    ~Generator() {
        if (handle_) handle_.destroy();
    }

    // begin() resumes the coroutine to get the first value.
    Iterator begin() {
        handle_.resume();
        if (handle_.promise().exception)
            std::rethrow_exception(handle_.promise().exception);
        return Iterator{handle_};
    }

    std::default_sentinel_t end() { return {}; }

private:
    std::coroutine_handle<promise_type> handle_;
};
