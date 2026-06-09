// Phase 2, Exercise 3 — RPN Calculator
// Concept: std::string, std::stack, std::istringstream, exception handling.
// Demonstrates: token parsing, operator dispatch, error propagation.
//
// Reverse Polish Notation (RPN) eliminates parentheses by placing operators
// after their operands. Example:
//   Infix:  (3 + 4) * 2
//   RPN:    3 4 + 2 *
//
// Evaluation algorithm (stack-based):
//   - If token is a number: push onto stack.
//   - If token is an operator: pop two operands, apply, push result.
//   - At end: the stack should contain exactly one value (the result).

#pragma once

#include <string>

// Evaluates a single RPN expression string.
// Returns the computed result.
// Throws std::runtime_error on invalid input (unknown token, insufficient
// operands, division by zero, unbalanced expression).
double evaluate(const std::string& expression);
