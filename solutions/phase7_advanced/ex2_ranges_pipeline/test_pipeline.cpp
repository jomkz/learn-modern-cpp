// Phase 7, Exercise 2 — Ranges pipeline tests

#include <catch2/catch_test_macros.hpp>
#include "pipeline.h"

TEST_CASE("pipeline filters blank lines", "[ranges]") {
    std::vector<std::string> input = {"hello", "", "world", "", "foo"};
    auto result = process_lines(input, 10);
    for (const auto& s : result) {
        REQUIRE(!s.empty());
    }
}

TEST_CASE("pipeline transforms to uppercase", "[ranges]") {
    std::vector<std::string> input = {"hello", "world"};
    auto result = process_lines(input, 10);
    REQUIRE(result[0] == "HELLO");
    REQUIRE(result[1] == "WORLD");
}

TEST_CASE("pipeline takes at most limit results", "[ranges]") {
    std::vector<std::string> input;
    for (int i = 0; i < 50; ++i) input.push_back("line" + std::to_string(i));
    auto result = process_lines(input, 10);
    REQUIRE(result.size() == 10);
}

TEST_CASE("pipeline skips blanks then takes limit from remaining", "[ranges]") {
    // 5 blanks then 15 real lines: take(10) should give 10 results.
    std::vector<std::string> input;
    for (int i = 0; i < 5; ++i) input.push_back("");
    for (int i = 0; i < 15; ++i) input.push_back("line" + std::to_string(i));
    auto result = process_lines(input, 10);
    REQUIRE(result.size() == 10);
}

TEST_CASE("pipeline handles all-blank input", "[ranges]") {
    std::vector<std::string> input = {"", "", ""};
    auto result = process_lines(input, 10);
    REQUIRE(result.empty());
}

TEST_CASE("pipeline handles empty input", "[ranges]") {
    std::vector<std::string> input;
    auto result = process_lines(input, 10);
    REQUIRE(result.empty());
}
