// Phase 3, Exercise 1 — RAII FileHandle
// Concept: RAII (Resource Acquisition Is Initialization)
// Demonstrates: constructor acquires, destructor releases, deleted copy,
//               noexcept destructor, exception-safety through RAII.
//
// RAII is the most important C++ idiom. Every resource — file handles, memory,
// mutex locks, network sockets — should be owned by an object whose destructor
// releases it. Because destructors run even when exceptions propagate, RAII
// gives automatic exception-safety with no try/finally boilerplate.

#pragma once

#include <cstdio>    // std::FILE, std::fopen, std::fclose, std::fwrite
#include <stdexcept> // std::runtime_error
#include <string>

class FileHandle {
public:
    // ── Constructor: ACQUIRE ─────────────────────────────────────────────────
    // Opens the file at 'path' with the given mode (e.g., "w", "r", "a").
    // Throws std::runtime_error if the file cannot be opened — never silently
    // constructs a FileHandle in a broken state.
    //
    // Why not accept std::string_view? std::fopen requires a null-terminated
    // char*, which std::string guarantees via .c_str(), but string_view does not.
    explicit FileHandle(const std::string& path, const std::string& mode);

    // ── Destructor: RELEASE ───────────────────────────────────────────────────
    // Closes the file. noexcept is critical here: if a destructor throws while
    // another exception is propagating, std::terminate is called. fclose()
    // returns EOF on error, but we cannot throw from a destructor — we silently
    // ignore fclose failures (log in production code).
    ~FileHandle() noexcept;

    // ── Copy: DISABLED ────────────────────────────────────────────────────────
    // A file handle represents exclusive ownership of an OS resource. Copying
    // would mean two objects try to close the same file — double-close is UB.
    // The correct choices for exclusive resources are: delete copy, allow move.
    // (We omit move for simplicity; Phase 6 covers move semantics fully.)
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // ── Operations ────────────────────────────────────────────────────────────
    // Write bytes to the file. Returns the number of bytes written.
    std::size_t write(const void* data, std::size_t size);

    // Read up to 'size' bytes into 'buf'. Returns actual bytes read.
    std::size_t read(void* buf, std::size_t size);

    // Access the raw FILE* for use with C APIs (fgets, fprintf, etc.).
    // Returns nullptr if the file is not open (should never happen post-ctor).
    std::FILE* get() const noexcept { return file_; }

private:
    std::FILE* file_;   // the OS file handle; owned exclusively by this object
};
