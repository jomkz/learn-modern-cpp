// Phase 7, Exercise 4 — C++20 Modules
// Concept: Replacing #include with import for faster, isolated compilation.
// Demonstrates: export module, export namespace, module interface unit.
//
// Why Modules?
//   #include pastes text verbatim into every translation unit that uses it.
//   For a header included by 1000 files: the header is parsed 1000 times.
//   Modules are parsed ONCE, serialised to a binary interface file (.pcm, .ifc),
//   and imported from that cache — often 10x–100x faster for large projects.
//
//   Additionally, modules don't suffer from include-order bugs: macros defined
//   before an #include can't pollute a module's contents.
//
// File extension conventions (not standardised, but common):
//   .cppm — Clang/GCC module interface unit
//   .ixx  — MSVC module interface unit
//   .cpp  — Can also be used with proper CMake FILE_SET CXX_MODULES

// 'export module myutil' declares this file as the primary module interface
// for the module named 'myutil'. There can only be ONE primary interface unit
// per named module.
export module myutil;

// Standard library headers must be #included or imported.
// Most implementations do not yet support 'import std;' (C++23) fully.
// For now we use #include inside a module to import the standard library.
#include <string>
#include <sstream>
#include <iomanip>

// 'export' makes a declaration visible to code that does 'import myutil;'.
// Non-exported declarations are private to this module unit.

// ── Exported function: square ─────────────────────────────────────────────────
// Callers write: import myutil; ... myutil::square(5);
export namespace myutil {

    // Returns x squared.
    int square(int x) {
        return x * x;
    }

    // Converts an unsigned integer to its hexadecimal string representation.
    // e.g., to_hex(255) == "0xff"
    std::string to_hex(unsigned int n) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::setfill('0') << std::setw(2) << n;
        return oss.str();
    }

} // namespace myutil
