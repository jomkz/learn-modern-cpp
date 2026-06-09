// Phase 4, Exercise 3 — Phonebook with std::map and STL algorithms
// Concept: std::map, iterator-based operations, no hand-written loops.
// Demonstrates: std::find_if, std::for_each, std::erase_if (C++20).
//
// All operations use STL algorithms rather than raw for-loops to demonstrate
// idiomatic STL usage. Note that std::map::find() is O(log n) and preferable
// for production code; std::find_if on a map's range is O(n) and is used here
// purely to exercise the algorithm API.

#pragma once

#include <algorithm>   // std::find_if, std::for_each
#include <functional>  // std::function
#include <map>
#include <optional>    // std::optional (C++17)
#include <string>

class Phonebook {
public:
    // Insert or update an entry. std::map::operator[] creates a default entry
    // if the key doesn't exist, so one line handles both insert and update.
    void insert(const std::string& name, const std::string& number);

    // Remove the entry with the given name. Returns true if found and removed.
    bool erase(const std::string& name);

    // Search by name — exact O(log n) lookup using std::map::find.
    // Returns std::optional to express "might not exist" without using
    // null pointers or sentinel values.
    std::optional<std::string> find_by_name(const std::string& name) const;

    // Search by number — linear scan using std::find_if, since std::map
    // is keyed by name, not number.
    std::optional<std::string> find_by_number(const std::string& number) const;

    // Print all entries in sorted key order (guaranteed by std::map).
    // Uses std::for_each over the map's range — equivalent to a range-based
    // for loop but more explicitly algorithmic.
    void print_sorted(std::ostream& os) const;

    std::size_t size() const noexcept { return entries_.size(); }
    bool empty() const noexcept { return entries_.empty(); }

private:
    // std::map<Key, Value>: red-black tree, O(log n) insert/find/erase,
    // always iterated in sorted key order.
    std::map<std::string, std::string> entries_;
};
