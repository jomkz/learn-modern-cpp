// Phase 1, Exercise 1 — Hello World
// Concept: The translation unit, the preprocessor, and manual compilation.
// Demonstrates: #include, main(), std::cout, manual g++ invocation.
//
// Compile manually (do NOT use an IDE's hidden build button for this exercise):
//   g++ -std=c++20 hello.cpp -o hello
//   ./hello
//
// Then inspect the binary:
//   file hello          — shows ELF/Mach-O type and architecture
//   nm hello | grep main  — shows the 'main' symbol in the symbol table
//   objdump -d hello | head -40  — disassembles the first few instructions

// #include is a preprocessor directive. It literally copies the contents of
// <iostream> into this file before compilation begins. That expanded file is
// called a "translation unit" — the unit the compiler actually sees.
#include <iostream>

// Every C++ program starts execution at main(). The OS calls main() after
// loading the binary. Returning 0 signals success to the shell; any non-zero
// value signals failure (try: ./hello; echo $?).
int main() {
    // std::cout is the standard character output stream, declared in <iostream>.
    // The << operator sends data to the stream.
    // std::endl flushes the buffer AND appends a newline; '\n' only appends a
    // newline without flushing. Prefer '\n' in loops — flushing on every line
    // is expensive. Here it doesn't matter, but the habit is worth building.
    std::cout << "Hello, World!\n";

    return 0;
}
