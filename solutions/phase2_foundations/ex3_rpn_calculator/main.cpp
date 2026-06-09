// Phase 2, Exercise 3 — RPN Calculator
// Interactive driver: reads one RPN expression per line from stdin,
// prints the result, and reports errors without crashing.
//
// Usage:
//   echo "3 4 + 2 *" | ./ph2_ex3_rpn
//   ./ph2_ex3_rpn
//   > 15 7 1 1 + - / 3 * 2 1 1 + + -
//   5
//   > ^D  (Ctrl+D to exit)

#include <iostream>
#include <stdexcept>
#include <string>
#include "rpn.h"

int main() {
    std::string line;

    // std::getline reads a full line including spaces (unlike >> which stops
    // at whitespace). We loop until EOF (Ctrl+D on Linux/macOS, Ctrl+Z on Windows).
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        try {
            double result = evaluate(line);
            std::cout << result << '\n';
        } catch (const std::exception& e) {
            // Write errors to stderr so they don't pollute stdout when the
            // program is used in a pipeline (e.g., piping output to another tool).
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}
