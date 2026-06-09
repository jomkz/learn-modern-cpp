// Phase 4, Exercise 3 — Phonebook

#include "phonebook.h"
#include <iostream>
#include <ostream>

void Phonebook::insert(const std::string& name, const std::string& number) {
    // operator[] inserts a default-constructed value if key is absent,
    // then we assign. This is one idiomatic way; another is insert_or_assign.
    entries_[name] = number;
}

bool Phonebook::erase(const std::string& name) {
    // std::erase_if (C++20) removes all elements matching a predicate.
    // For std::map it returns the number of elements removed.
    // This is the algorithm-based approach requested by the exercise.
    // The O(log n) alternative would be: entries_.erase(name)
    std::size_t removed = std::erase_if(entries_,
        [&name](const auto& kv) { return kv.first == name; });
    return removed > 0;
}

std::optional<std::string> Phonebook::find_by_name(const std::string& name) const {
    // std::map::find is O(log n) — the right tool for key-based lookup.
    // Returns std::optional so the caller can check with if (opt).
    auto it = entries_.find(name);
    if (it == entries_.end()) return std::nullopt;
    return it->second;
}

std::optional<std::string> Phonebook::find_by_number(const std::string& number) const {
    // std::find_if scans linearly. The lambda receives a std::pair<const string, string>.
    // We use a structured binding auto& [k, v] (C++17) for readability.
    auto it = std::find_if(entries_.begin(), entries_.end(),
        [&number](const auto& kv) { return kv.second == number; });
    if (it == entries_.end()) return std::nullopt;
    return it->first;  // return the name, not the number
}

void Phonebook::print_sorted(std::ostream& os) const {
    // std::for_each applies a callable to each element in range.
    // std::map iterates in sorted key order, so the output is automatically sorted.
    std::for_each(entries_.begin(), entries_.end(),
        [&os](const auto& kv) {
            os << kv.first << ": " << kv.second << '\n';
        });
}
