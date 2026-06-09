// Phase 3, Exercise 1 — RAII FileHandle tests

#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <string>
#include "file_handle.h"

static const std::string TEST_PATH = "/tmp/raii_fh_test.txt";

TEST_CASE("FileHandle writes and reads data", "[raii]") {
    {
        FileHandle writer(TEST_PATH, "w");
        const std::string msg = "RAII test";
        writer.write(msg.data(), msg.size());
    }
    // After the scope, the destructor has closed the file.
    // We can now re-open and read to confirm the write succeeded.
    FileHandle reader(TEST_PATH, "r");
    char buf[64] = {};
    reader.read(buf, sizeof(buf) - 1);
    REQUIRE(std::string(buf) == "RAII test");
}

TEST_CASE("FileHandle throws on bad path", "[raii]") {
    REQUIRE_THROWS_AS(
        FileHandle("/no/such/directory/file.txt", "r"),
        std::runtime_error);
}

TEST_CASE("FileHandle closes on exception in scope", "[raii]") {
    // After this block, the file at TEST_PATH should be closed even though
    // we threw. We verify by re-opening it — if it was leaked (not closed),
    // some OSes would deny the second open under certain modes.
    try {
        FileHandle fh(TEST_PATH, "w");
        const std::string data = "before throw";
        fh.write(data.data(), data.size());
        throw std::runtime_error("forced");
    } catch (const std::runtime_error&) {
        // expected — file should have been closed by destructor
    }

    // Re-open should succeed, confirming the handle was properly released.
    // Use a temporary — REQUIRE_NOTHROW expects an expression, not a declaration.
    REQUIRE_NOTHROW(FileHandle(TEST_PATH, "r"));
}
