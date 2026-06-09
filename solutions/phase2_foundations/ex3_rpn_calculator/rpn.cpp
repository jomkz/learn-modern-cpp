// Phase 2, Exercise 3 — RPN Calculator

#include "rpn.h"

#include <functional>   // std::function
#include <map>          // std::map for operator dispatch
#include <sstream>      // std::istringstream for token splitting
#include <stack>        // std::stack for the evaluation stack
#include <stdexcept>    // std::runtime_error, std::invalid_argument
#include <string>

// ── Operator table ────────────────────────────────────────────────────────────
// Using a map<string, function<double(double,double)>> for operator dispatch
// instead of a long if-else or switch. This makes adding new operators trivial:
// just insert a new entry. The lambda captures nothing and is stored by value.
//
// Why std::function here? We need to store heterogeneous callables (lambdas,
// function pointers) in a uniform container. std::function is slightly slower
// than a direct call due to type erasure, but the code is in a parsing loop —
// not a hot inner loop — so clarity wins over micro-optimisation.
static const std::map<std::string, std::function<double(double, double)>> operators = {
    {"+", [](double a, double b) { return a + b; }},
    {"-", [](double a, double b) { return a - b; }},
    {"*", [](double a, double b) { return a * b; }},
    {"/", [](double a, double b) {
        // Check for division by zero before dividing. IEEE 754 would produce
        // inf or NaN silently; we prefer an explicit error for a calculator.
        if (b == 0.0) throw std::runtime_error("division by zero");
        return a / b;
    }},
};

// ── Helper: pop the top of the stack or throw ─────────────────────────────────
// std::stack::top() + pop() is a two-step operation. If we call top() on an
// empty stack we get undefined behaviour; pop() on empty also UB. This helper
// wraps both steps with a bounds check.
static double pop(std::stack<double>& stk) {
    if (stk.empty()) {
        throw std::runtime_error("not enough operands on stack");
    }
    double val = stk.top();
    stk.pop();
    return val;
}

// ── Evaluator ────────────────────────────────────────────────────────────────
double evaluate(const std::string& expression) {
    std::stack<double> stk;

    // std::istringstream wraps a string and provides stream-like tokenisation.
    // The >> operator skips whitespace and reads the next whitespace-delimited
    // token — exactly what we need to split an RPN expression.
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        // Is this token a known operator?
        auto it = operators.find(token);
        if (it != operators.end()) {
            // Pop right operand first, then left. Stack is LIFO, so the last
            // number pushed is the right operand of the operator.
            double rhs = pop(stk);
            double lhs = pop(stk);
            stk.push(it->second(lhs, rhs));
        } else {
            // Try to parse as a floating-point number.
            // std::stod throws std::invalid_argument if the string is not numeric.
            std::size_t parsed = 0;
            double value = 0.0;
            try {
                value = std::stod(token, &parsed);
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("unknown token: '" + token + "'");
            }
            // parsed == token.size() ensures the entire token was consumed
            // (e.g., "3.14xyz" would not parse cleanly).
            if (parsed != token.size()) {
                throw std::runtime_error("invalid token: '" + token + "'");
            }
            stk.push(value);
        }
    }

    // After processing all tokens, exactly one value must remain.
    if (stk.size() != 1) {
        throw std::runtime_error(
            "expression error: " + std::to_string(stk.size()) +
            " value(s) left on stack (expected 1)");
    }

    return stk.top();
}
