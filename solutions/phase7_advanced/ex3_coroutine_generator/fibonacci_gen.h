// Phase 7, Exercise 3 — Fibonacci coroutine generator

#pragma once

#include <cstdint>
#include "generator.h"

// fibonacci() is a coroutine: it contains co_yield, which transforms the
// function into a coroutine function. The return type Generator<uint64_t>
// tells the compiler which promise_type to use.
//
// The coroutine body runs until the first co_yield, then suspends.
// Each call to ++iterator resumes it from the last suspension point.
// The coroutine runs forever (infinite sequence); the caller decides when to stop.
inline Generator<uint64_t> fibonacci() {
    uint64_t a = 0, b = 1;

    while (true) {
        // co_yield suspends the coroutine and sends 'a' to the caller.
        // When the coroutine is resumed (via ++iterator), execution continues
        // at the line AFTER co_yield.
        co_yield a;

        // Advance the sequence: a, b → b, a+b.
        uint64_t next = a + b;
        a = b;
        b = next;
    }
    // This while(true) never returns, which is fine: the caller stops
    // iterating when it has enough values. The coroutine frame is destroyed
    // by Generator's destructor.
}
