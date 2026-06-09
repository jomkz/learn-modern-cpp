// Phase 8, Exercise 4 — Thread-Safe Producer-Consumer Queue
// Concept: std::jthread, std::mutex, std::condition_variable, std::stop_token.
// Demonstrates: producer-consumer pattern, cooperative cancellation, bounded queue.
//
// The Bounded Queue:
//   - Fixed capacity N prevents unbounded memory growth.
//   - Producers block when full; consumers block when empty.
//   - std::stop_token allows any thread to signal "done" and unblock waiters.
//
// std::jthread (C++20) vs std::thread:
//   std::thread:  must call join() or detach() manually; no built-in cancellation.
//   std::jthread: auto-joins in the destructor; constructor accepts a stop-token
//                 parameter for cooperative cancellation.
//
// Synchronisation:
//   std::mutex:               mutual exclusion; only one thread holds the lock.
//   std::condition_variable:  allows threads to sleep until a condition changes.
//   std::stop_token:          a thread-safe flag that signals "time to stop."

#pragma once

#include <array>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <stop_token>   // std::stop_token, std::stop_source (C++20)

template<typename T, std::size_t Capacity>
class BoundedQueue {
    static_assert(Capacity > 0, "BoundedQueue requires Capacity > 0");

public:
    BoundedQueue() = default;

    // Disable copy and move — the mutex and cv are not copyable.
    BoundedQueue(const BoundedQueue&)            = delete;
    BoundedQueue& operator=(const BoundedQueue&) = delete;

    // ── push ─────────────────────────────────────────────────────────────────
    // Blocks if the queue is full. Returns false if the stop token fired.
    bool push(T value, std::stop_token stop = {}) {
        std::unique_lock lock(mutex_);

        // wait() atomically releases the lock and sleeps. It re-acquires the
        // lock before returning. The predicate is re-checked to handle spurious wakeups.
        cv_not_full_.wait(lock, [&] {
            return count_ < Capacity || stop.stop_requested();
        });

        if (stop.stop_requested()) return false;

        buffer_[tail_] = std::move(value);
        tail_ = (tail_ + 1) % Capacity;
        ++count_;

        // Notify one blocked consumer that an item is available.
        cv_not_empty_.notify_one();
        return true;
    }

    // ── pop ──────────────────────────────────────────────────────────────────
    // Blocks if the queue is empty. Returns nullopt if the stop token fired.
    std::optional<T> pop(std::stop_token stop = {}) {
        std::unique_lock lock(mutex_);

        cv_not_empty_.wait(lock, [&] {
            return count_ > 0 || stop.stop_requested();
        });

        if (count_ == 0 && stop.stop_requested()) return std::nullopt;

        T value = std::move(buffer_[head_]);
        head_ = (head_ + 1) % Capacity;
        --count_;

        // Notify one blocked producer that space is available.
        cv_not_full_.notify_one();
        return value;
    }

    // ── Shutdown ─────────────────────────────────────────────────────────────
    // Wake up all waiting threads so they can check stop_requested() and exit.
    void notify_all_waiters() {
        cv_not_full_.notify_all();
        cv_not_empty_.notify_all();
    }

    std::size_t size() const {
        std::scoped_lock lock(mutex_);
        return count_;
    }

    bool empty() const {
        std::scoped_lock lock(mutex_);
        return count_ == 0;
    }

private:
    std::array<T, Capacity> buffer_{};
    std::size_t head_  = 0;
    std::size_t tail_  = 0;
    std::size_t count_ = 0;

    mutable std::mutex              mutex_;
    std::condition_variable         cv_not_full_;
    std::condition_variable         cv_not_empty_;
};
