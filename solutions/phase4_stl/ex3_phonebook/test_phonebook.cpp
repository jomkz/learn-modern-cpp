// Phase 4, Exercise 3 — Phonebook tests

#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "phonebook.h"

TEST_CASE("insert and find by name", "[phonebook]") {
    Phonebook pb;
    pb.insert("Alice", "555-0100");
    pb.insert("Bob",   "555-0200");

    REQUIRE(pb.find_by_name("Alice") == std::optional<std::string>("555-0100"));
    REQUIRE(pb.find_by_name("Bob")   == std::optional<std::string>("555-0200"));
    REQUIRE(pb.find_by_name("Charlie") == std::nullopt);
}

TEST_CASE("insert updates existing entry", "[phonebook]") {
    Phonebook pb;
    pb.insert("Alice", "555-0100");
    pb.insert("Alice", "555-9999");  // update

    REQUIRE(pb.find_by_name("Alice") == std::optional<std::string>("555-9999"));
    REQUIRE(pb.size() == 1);  // no duplicate entries
}

TEST_CASE("erase removes entry", "[phonebook]") {
    Phonebook pb;
    pb.insert("Alice", "555-0100");
    pb.insert("Bob",   "555-0200");

    REQUIRE(pb.erase("Alice") == true);
    REQUIRE(pb.find_by_name("Alice") == std::nullopt);
    REQUIRE(pb.size() == 1);
}

TEST_CASE("erase returns false for missing name", "[phonebook]") {
    Phonebook pb;
    REQUIRE(pb.erase("Nobody") == false);
}

TEST_CASE("find by number (reverse lookup)", "[phonebook]") {
    Phonebook pb;
    pb.insert("Alice", "555-0100");
    pb.insert("Bob",   "555-0200");

    REQUIRE(pb.find_by_number("555-0200") == std::optional<std::string>("Bob"));
    REQUIRE(pb.find_by_number("999-9999") == std::nullopt);
}

TEST_CASE("print_sorted outputs in alphabetical order", "[phonebook]") {
    Phonebook pb;
    pb.insert("Zara",    "555-0300");
    pb.insert("Alice",   "555-0100");
    pb.insert("Charlie", "555-0200");

    std::ostringstream oss;
    pb.print_sorted(oss);
    const std::string out = oss.str();

    // Alice must appear before Charlie, and Charlie before Zara.
    const auto alice_pos   = out.find("Alice");
    const auto charlie_pos = out.find("Charlie");
    const auto zara_pos    = out.find("Zara");

    REQUIRE(alice_pos   != std::string::npos);
    REQUIRE(charlie_pos != std::string::npos);
    REQUIRE(zara_pos    != std::string::npos);
    REQUIRE(alice_pos   < charlie_pos);
    REQUIRE(charlie_pos < zara_pos);
}
