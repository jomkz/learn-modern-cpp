// Phase 3, Exercise 1 — RAII FileHandle
// Demonstrates that the file is closed even when an exception propagates
// out of the scope containing the FileHandle.

#include <iostream>
#include <stdexcept>
#include "file_handle.h"

// This function deliberately throws after writing to demonstrate that
// the FileHandle destructor still fires on the exception path.
static void write_and_throw(const std::string& path) {
    // ── RAII object created: file opened ─────────────────────────────────────
    FileHandle fh(path, "w");
    std::cout << "File opened. Writing data...\n";

    const char* data = "Hello from RAII FileHandle\n";
    fh.write(data, std::char_traits<char>::length(data));

    std::cout << "About to throw — watch whether the file gets closed.\n";
    // The exception causes the stack to unwind. Before control leaves this
    // scope, all local objects with automatic storage duration are destroyed —
    // including 'fh'. Its destructor calls fclose(), closing the file cleanly.
    throw std::runtime_error("simulated error after write");

    // If we didn't have RAII and relied on manual fclose() here, the line
    // below would never execute and the file would leak.
    // fclose(raw_file); // ← this would be skipped by the exception above
}

int main() {
    const std::string path = "/tmp/raii_test.txt";

    try {
        write_and_throw(path);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
        std::cout << "File was closed by RAII destructor before exception propagated.\n";
    }

    // Verify the file was written and closed by re-opening it for reading.
    try {
        FileHandle reader(path, "r");
        char buf[128] = {};
        reader.read(buf, sizeof(buf) - 1);
        std::cout << "File contents: " << buf;
        std::cout << "Success: file was properly flushed and closed despite exception.\n";
    } catch (const std::exception& e) {
        std::cerr << "Could not read file: " << e.what() << '\n';
    }

    return 0;
}
