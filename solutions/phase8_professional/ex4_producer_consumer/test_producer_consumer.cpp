// Phase 8, Exercise 4 — Producer-Consumer Queue tests

#include <catch2/catch_test_macros.hpp>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include "bounded_queue.h"

using namespace std::chrono_literals;

TEST_CASE("single-threaded push/pop", "[producer_consumer]") {
    BoundedQueue<int, 8> q;

    REQUIRE(q.push(1));
    REQUIRE(q.push(2));
    REQUIRE(q.push(3));
    REQUIRE(q.size() == 3);

    REQUIRE(q.pop().value() == 1);
    REQUIRE(q.pop().value() == 2);
    REQUIRE(q.pop().value() == 3);
    REQUIRE(q.empty());
}

TEST_CASE("FIFO ordering", "[producer_consumer]") {
    BoundedQueue<int, 16> q;
    for (int i = 0; i < 10; ++i) q.push(i);
    for (int i = 0; i < 10; ++i) {
        REQUIRE(q.pop().value() == i);
    }
}

TEST_CASE("single producer, single consumer", "[producer_consumer]") {
    BoundedQueue<int, 16> q;
    constexpr int N = 100;
    std::atomic<int> consumed_sum = 0;

    // jthread auto-joins when it goes out of scope.
    std::jthread producer([&](std::stop_token stop) {
        for (int i = 0; i < N; ++i) {
            q.push(i, stop);
        }
    });

    std::jthread consumer([&](std::stop_token stop) {
        for (int i = 0; i < N; ++i) {
            if (auto v = q.pop(stop)) {
                consumed_sum += *v;
            }
        }
    });

    // jthread destructor auto-joins both threads here.
    producer.join();
    consumer.join();

    // sum(0..99) = 4950
    REQUIRE(consumed_sum == (N - 1) * N / 2);
}

TEST_CASE("multiple producers, one consumer", "[producer_consumer]") {
    BoundedQueue<int, 32> q;
    constexpr int ITEMS_PER_PRODUCER = 50;
    constexpr int NUM_PRODUCERS = 3;
    std::atomic<int> total_consumed = 0;

    std::stop_source stop_src;
    std::vector<std::jthread> producers;
    for (int p = 0; p < NUM_PRODUCERS; ++p) {
        producers.emplace_back([&, p](std::stop_token stop) {
            for (int i = 0; i < ITEMS_PER_PRODUCER; ++i) {
                q.push(p * 1000 + i, stop);
            }
        });
    }

    std::jthread consumer([&](std::stop_token stop) {
        const int total = NUM_PRODUCERS * ITEMS_PER_PRODUCER;
        for (int i = 0; i < total; ++i) {
            if (auto v = q.pop(stop)) {
                ++total_consumed;
                (void)*v;
            }
        }
    });

    for (auto& t : producers) t.join();
    consumer.join();

    REQUIRE(total_consumed == NUM_PRODUCERS * ITEMS_PER_PRODUCER);
}

TEST_CASE("stop_token cancels blocked pop", "[producer_consumer]") {
    BoundedQueue<int, 8> q;  // empty queue
    std::stop_source stop_src;

    std::jthread consumer([&](std::stop_token stop) {
        // Will block because the queue is empty. We expect it to return nullopt
        // when stop is requested.
        auto v = q.pop(stop);
        REQUIRE(!v.has_value());  // nullopt — stopped, not a real value
    });

    // Request stop after a brief delay.
    std::this_thread::sleep_for(50ms);
    stop_src.request_stop();
    consumer.request_stop();  // jthread's internal stop_source
    q.notify_all_waiters();

    consumer.join();
    SUCCEED("Consumer exited cleanly on stop_token");
}
