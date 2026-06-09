Mastering modern C++ is a marathon, not a sprint. The language has evolved massively from its origins, moving away from manual memory management and raw pointers toward safer, highly expressive, and fiercely fast paradigms.

Here is a detailed, sequential learning path to take you from the fundamentals to advanced modern C++ (covering C++11 through C++23).

---

## How to Use This Guide

This guide is structured for **all experience levels**:

- **Complete beginners:** Follow every phase in order.
- **Programmers new to C++** (Python, Java, Go, Rust, etc.): Skim Phase 2 quickly — focus on the C++-specific differences (pointers, `const`, UB, casts). You can move fast.
- **C++98/03 developers:** Phases 2–4 will be largely review. Start paying close attention from Phase 5 (Templates) onward.

**Estimated total time:** 6–12 months at a consistent pace of a few hours per week.

**How to learn from it:** Read a section, then do the exercises before moving on. Passive reading is not enough for C++.

**Reference solutions:** Worked solutions for every exercise live in the [`solutions/`](solutions/) directory, with comments explaining the *why* behind each design choice. Attempt the exercise first — then use the solutions to check your work or get unstuck.

**Three sites to bookmark right now — use them throughout every phase:**

- [learncpp.com](https://www.learncpp.com) — Best free tutorial; modern C++ from the start
- [cppreference.com](https://en.cppreference.com) — Authoritative language and library reference
- [isocpp.org](https://isocpp.org) — Official C++ foundation; Core Guidelines and FAQ

---

## Phase 1: Environment Setup and Tooling

*Estimated time: ~1 week*

Before writing a single line of code, you need a robust modern development environment.

- **Compiler:** Install a modern compiler that supports C++20/C++23. Choose GCC (Linux/Windows via WSL2 or MinGW), Clang (macOS/Linux), or MSVC (Windows).
- **IDE/Editor:** Set up Visual Studio, CLion, or VS Code with the C++ extension pack.
- **Build System:** Learn the absolute basics of **CMake**. It is the industry standard for building C++ projects. Do not rely solely on your IDE's hidden build mechanics.
- **Package Manager:** Use **vcpkg** (Microsoft, widely adopted, integrates directly with CMake) or **Conan** (JFrog, mature binary management) to handle third-party library dependencies rather than installing them manually.
- **Debugger:** Install **GDB** (Linux/Windows/WSL2) or **LLDB** (macOS/Clang). You will learn to use it hands-on in Phase 8, but install it now.
- **Version Control:** Initialize a Git repository for your learning projects.

> ⚠️ **Platform Notes**
>
> | Platform | Recommended Compiler | Debugger | Profiler | Key Note |
> |---|---|---|---|---|
> | **Linux** | GCC or Clang via package manager | GDB | `perf` / Valgrind | Most C++ tooling assumes Linux. Best first environment. |
> | **macOS** | Homebrew LLVM (`brew install llvm`), **not** Apple Clang | LLDB | Instruments (Xcode) | Apple's bundled Clang lags C++23 support by years. Install Homebrew LLVM for full standard coverage. |
> | **Windows** | MSVC (Visual Studio) or **WSL2** for Linux toolchain | VS Debugger or GDB inside WSL2 | Visual Studio Profiler / vcperf+WPA | WSL2 is recommended for access to Linux-native tools (`perf`, Valgrind, GDB). Valgrind does **not** run natively on Windows or macOS. |
>
> For cross-platform code, use `#ifdef _WIN32`, `#ifdef __APPLE__`, and `#ifdef __linux__` to conditionally compile platform-specific sections. This is covered in depth in Phase 2.

> **Resources**
> - [learncpp.com — Chapter 0: Getting Started](https://www.learncpp.com/cpp-tutorial/introduction-to-these-tutorials/)
> - [CMake Official Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
> - [vcpkg Quickstart](https://vcpkg.io/en/getting-started.html)
> - [Conan Getting Started Guide](https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html)

### Exercises

1. Write a `hello_world` program and compile it manually from the command line with `g++ -std=c++20 hello.cpp -o hello`. Inspect the binary with `file hello`.
2. Create a two-file project (a `.h` header and a `.cpp` source file) and build it with a `CMakeLists.txt` using `add_executable`.
3. Add the `Catch2` testing library to your CMake project using vcpkg and write one passing test.

---

## Phase 2: C++ Foundations

*Estimated time: ~3–4 weeks*

⏩ *Experienced programmers: skim quickly — focus on `std::string`, `const` correctness, the C++ cast operators, the compilation pipeline, and the UB callout. These are the sharpest differences from other languages.*

Focus on the core mechanics of the language before touching object-oriented concepts or modern features.

- **Syntax and Structure:** Understand `#include`, the `main()` function, and basic compilation.
- **Data Types and Variables:** Learn about integers, floating-point numbers, booleans, and characters. Understand signed vs. unsigned types and their pitfalls.
- **`std::string`:** The fundamental string type. Learn key methods: `find`, `substr`, `replace`, `+` concatenation, `size()`, and `c_str()`. Preview `std::string_view` (C++17) — a zero-copy, non-owning view into a string for read-only operations.
- **Control Flow:** Master `if/else` statements, `switch` cases, `for` loops, and `while` loops.
- **Functions:** Understand function declaration, definition, parameters, return types, and function overloading.
- **Compilation Pipeline:** Understand the four stages — *preprocessing*, *compilation*, *assembly*, and *linking*. Learn what header guards (`#ifndef/#define/#endif`) and `#pragma once` do, how **forward declarations** reduce compilation dependencies and break circular includes, and what the **One Definition Rule (ODR)** requires. This knowledge is what lets you diagnose linker errors.
- **Namespaces:** Understand `namespace`, `using` declarations, anonymous namespaces (internal linkage), and `inline namespace`. Always qualify names explicitly (e.g., `std::cout` rather than `using namespace std;` in headers).
- **`enum class`:** C++11 scoped enumerations. Prefer `enum class` over plain `enum` — no implicit integer conversions and no name pollution into the enclosing scope.
- **`const` Correctness:** Apply `const` to variables, function parameters, pointers (`const int*` vs. `int* const`), and member functions (`void foo() const`). Treat `const` as a design tool — it communicates intent and prevents bugs.
- **C++ Cast Operators:** Use `static_cast<T>()`, `dynamic_cast<T*>()`, `const_cast<T>()`, and `reinterpret_cast<T>()` instead of C-style `(int)x` casts. C-style casts silently combine all four C++ casts and bypass type safety; the C++ operators make intent explicit and catch many misuses at compile time.
- **Pointers and References:** This is the most critical hurdle. Learn memory addresses, pointer dereferencing, passing by value versus passing by reference, and the stack versus the heap.
- **I/O Basics:** Use `std::cin`/`std::cout` from `<iostream>`, `std::fstream` for files, and `std::stringstream` for string streams.

> ⚠️ **Undefined Behavior (UB)**
>
> C++ has a concept unique among mainstream languages: *undefined behavior*. When code invokes UB — signed integer overflow, reading past the end of an array, using a pointer after its object has been destroyed, uninitialized reads — the C++ standard places **no requirement whatsoever** on the compiler's output. The program may appear to work correctly on your machine and crash in production, or silently corrupt data. UB is why tools like AddressSanitizer and Valgrind exist (Phase 8). Learn to recognize common UB early, and enable compiler warnings (`-Wall -Wextra -Wpedantic`) to catch many of them at compile time.

> **Resources**
> - [learncpp.com — Chapters 1–7](https://www.learncpp.com) *(syntax, variables, control flow, functions, pointers)*
> - [cppreference.com — Fundamental types](https://en.cppreference.com/w/cpp/language/types)
> - 📖 *C++ Primer* (Lippman, Lajoie, Moo) — Chapters 1–6

### Exercises

1. Manually run all four compilation stages for a two-file program: `g++ -E` (preprocessing), `g++ -S` (to assembly), `g++ -c` (to object file), then link with `g++ main.o util.o -o app`. Inspect each output file.
2. Create an ODR violation by defining the same non-inline function in two translation units and observe the linker error. Fix it with a forward declaration.
3. Implement a command-line RPN (Reverse Polish Notation) calculator that reads expressions token by token from `std::cin` using `std::string`.

---

## Phase 3: Object-Oriented Programming (OOP)

*Estimated time: ~3–4 weeks*

C++ is a multi-paradigm language, but OOP remains heavily utilized.

- **Classes and Objects:** Understand the difference between a blueprint (class) and an instance (object).
- **Constructors, Destructors, and Member Initializer Lists:** Learn how objects are initialized and destroyed. Use the `: member(value)` member initializer list syntax in constructors — it is required for `const` members, reference members, and base class subobjects, and avoids the overhead of default-constructing then assigning.
- **Static Members and Methods:** `static` member variables are shared across all instances of a class (not per-object). `static` member functions have no implicit `this` pointer and are callable without an instance. Common uses: instance counters, factory methods, Singleton.
- **Encapsulation:** Practice hiding internal state using `public`, `private`, and `protected` access modifiers.
- **Inheritance:** Learn how to create derived classes to reuse and extend code.
- **Polymorphism:** Master virtual functions, pure virtual functions, and abstract classes to design flexible interfaces. Always use `override` on functions that override a base-class virtual — it prevents silent bugs when the base class signature changes. Use `final` to mark a class or function as non-derivable or non-overridable.
- **Operator Overloading:** Overload `<<`, `>>`, `==`, `!=`, `<`, `[]`, `()`, and arithmetic operators to give your types natural value semantics. Use `friend` functions for symmetric operators like `operator==`; use member functions for `operator[]` and `operator()`.
- **RAII (Resource Acquisition Is Initialization):** The single most important C++ idiom. Bind every resource — memory, file handles, mutex locks, network sockets — to the lifetime of an object. The constructor acquires the resource; the destructor releases it. Because destructors run even when exceptions propagate, RAII makes code automatically exception-safe.
- **Exception Handling:** Use `try`, `catch`, and `throw`. Mark functions `noexcept` when they cannot throw (destructors should almost always be `noexcept`). Learn the three exception safety guarantees: *basic* (no resource leaks, invariants intact), *strong* (operation either fully succeeds or has no effect — commit-or-rollback), and *no-throw* (operation never throws).
- **Rule of Three / Five / Zero:** If your class manually manages a resource, you must define the destructor, copy constructor, and copy assignment operator (Rule of Three), and in C++11 also the move constructor and move assignment operator (Rule of Five). Prefer the **Rule of Zero**: design classes so that member smart pointers and STL containers own all resources, making all five special functions unnecessary.

> **Resources**
> - [learncpp.com — Chapters 13–17](https://www.learncpp.com) *(classes, inheritance, virtual functions, operator overloading)*
> - [C++ Core Guidelines — Class section](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
> - 📖 *Effective C++* (Scott Meyers, 3rd Ed.) — Items on constructors, RAII, and polymorphism

### Exercises

1. Write an RAII `FileHandle` wrapper class. Verify it correctly closes the file even when an exception propagates out of the scope.
2. Implement the Rule of Five for a `Buffer` class that owns a heap-allocated `char[]` array. Use AddressSanitizer to confirm no double-free or leak.
3. Implement exception-safe copy assignment for `Buffer` using the copy-and-swap idiom.
4. Overload `operator<<` for a `Matrix` class and `operator<` so a `std::vector<Matrix>` can be sorted with `std::sort`.

---

## Phase 4: The Standard Template Library (STL)

*Estimated time: ~3–4 weeks*

Do not reinvent the wheel. The STL provides highly optimized, template-based data structures and algorithms.

- **Sequential Containers:** Master `std::vector` (your default array), `std::array`, `std::deque`, and `std::list`. Use `std::vector` by default; switch to another container only when its specific complexity guarantees are genuinely needed.
- **Associative Containers:** Learn `std::map`, `std::unordered_map`, `std::set`, and `std::unordered_set`. The ordered variants (`map`/`set`) use a red-black tree (O(log n)). The unordered variants are hash-based (average O(1) but with worse cache behavior).
- **Container Adaptors:** `std::stack`, `std::queue`, and `std::priority_queue` wrap the sequence containers with a restricted interface.
- **`std::pair` and `std::tuple`:** `std::map::value_type` is `std::pair<const Key, Value>`. Use `std::tie` and C++17 structured bindings (`auto [key, val] = *it;`) to unpack them cleanly.
- **Iterators:** Understand how to traverse containers using iterators rather than raw index loops. Learn the five iterator categories (input, output, forward, bidirectional, random-access) and when each applies. Use range-based `for` by default.
- **Algorithms:** Learn `<algorithm>` functions: `std::sort`, `std::find`, `std::find_if`, `std::transform`, `std::accumulate`, `std::count_if`, `std::copy_if`, `std::remove_if`, and `std::partition`. Prefer algorithms over hand-written loops.
- **`<random>` Library:** Replace `rand()` (deprecated, statistically biased, global state) with `std::mt19937` paired with a distribution object like `std::uniform_int_distribution<int>` or `std::normal_distribution<double>`.

> ⚠️ **Iterator Invalidation**
>
> Modifying a container while holding iterators into it — or inserting past the end of a `vector` when it reallocates — silently invalidates those iterators, producing UB. Quick reference:
>
> | Container | Operations that invalidate iterators |
> |---|---|
> | `std::vector` | Any reallocation (`push_back` beyond capacity invalidates **all** iterators and pointers) |
> | `std::deque` | Insertions at front/back invalidate all iterators; middle insertions invalidate all |
> | `std::list` / `std::forward_list` | Only the erased element's iterator is invalidated |
> | `std::map` / `std::set` | Only the erased element's iterator is invalidated |
> | `std::unordered_map` / `std::unordered_set` | Rehashing (triggered by insert beyond load factor) invalidates **all** iterators |

> **Resources**
> - [learncpp.com — Chapters 16–17, 23](https://www.learncpp.com) *(arrays, STL, algorithms)*
> - 📖 *Effective STL* (Scott Meyers) — 50 specific ways to improve STL usage
> - [cppreference.com — Algorithms library](https://en.cppreference.com/w/cpp/algorithm)
> - [cppreference.com — Containers library](https://en.cppreference.com/w/cpp/container)

### Exercises

1. Benchmark `std::vector`, `std::list`, and `std::deque` for front-insert, back-insert, and sequential-iterate at sizes 1K, 100K, and 1M. Measure with `std::chrono::steady_clock`.
2. Write a custom bidirectional iterator for a doubly-linked list node class that satisfies the `std::bidirectional_iterator` concept.
3. Implement a phonebook using `std::map<std::string, std::string>` with search, sorted-print, and erase operations — all implemented using STL algorithms, not hand-written loops.
4. Generate 1 million random integers with `std::mt19937` + `std::uniform_int_distribution<int>` and bucket them into a 10-bin histogram.

---

## Phase 5: Templates & Generic Programming

*Estimated time: ~3–4 weeks*

Generic programming allows you to write code that works for any type satisfying a set of requirements — without sacrificing performance. Every major component of the STL is built on templates.

- **Function Templates:** Write a single function definition that the compiler instantiates into a concrete version for each type it is called with.
- **Class Templates:** Parameterize entire classes by type. Learn **partial specialization** to provide a different implementation for a specific family of types (e.g., specializing `Container<T*>` for all pointer types).
- **Non-Type Template Parameters:** Templates can be parameterized by values, not just types — e.g., `template<typename T, std::size_t N>`, which is exactly how `std::array<int, 5>` is declared.
- **Variadic Templates:** Accept any number of template arguments using `...Args`. Combined with **fold expressions** (`(f(args), ...)`, C++17), they replace recursive template metaprogramming for most common patterns.
- **SFINAE (Substitution Failure Is Not An Error):** When the compiler substitutes a type into a template and the result is an invalid expression, that specialization is silently discarded rather than causing a compile error. The mechanism behind `std::enable_if` and conditional template overloading. (C++20 Concepts, covered in Phase 7, largely supersede SFINAE for new code.)
- **`decltype` and `decltype(auto)`:** `decltype(expr)` queries the declared type of an expression without evaluating it. `decltype(auto)` deduces a return type that preserves reference and cv-qualifiers — essential for perfect-forwarding return types. Completes the type-deduction picture alongside `auto`.
- **Compile-Time Programming:** Write `constexpr` functions evaluated at compile time when possible. Use `consteval` (C++20) to *require* compile-time evaluation. Use `static_assert` to embed compile-time correctness checks directly in the code. Explore type traits (`std::is_arithmetic_v`, `std::is_same_v`, `std::conditional_t`, `std::enable_if_t`) and `if constexpr` for compile-time branching inside template bodies.

> **Resources**
> - 📖 *Effective Modern C++* (Scott Meyers) — Items 1–3 *(type deduction for templates and `auto`)*
> - [learncpp.com — Chapters 11–13](https://www.learncpp.com) *(function templates, class templates)*
> - [cppreference.com — Templates](https://en.cppreference.com/w/cpp/language/templates)
> - [cppreference.com — Type traits](https://en.cppreference.com/w/cpp/header/type_traits)

### Exercises

1. Write a generic `clamp(value, lo, hi)` function template and verify it compiles and behaves correctly for `int`, `double`, and a custom `Duration` type.
2. Implement a class template `TypedStack<T>` with a partial specialization for `bool` that packs booleans into a `uint64_t` bitfield.
3. Write a variadic `print(args...)` function using a fold expression that prints all arguments separated by spaces, followed by a newline.
4. Implement `constexpr uint64_t fibonacci(int n)` and verify three values at compile time using `static_assert`.
5. Write a template function `process(x)` that uses `if constexpr` and `std::is_arithmetic_v` to double numeric arguments and reverse string arguments.

---

## Phase 6: Modern C++ Core (C++11 & C++14)

*Estimated time: ~3–4 weeks*

This phase represents the shift from "Old C++" to "Modern C++."

⏩ *C++98/03 developers: this is the most important phase for you — every topic replaces a dangerous or verbose old pattern.*

- **Smart Pointers:** Stop using raw `new` and `delete`. Master `std::unique_ptr` for exclusive ownership (zero overhead abstraction), `std::shared_ptr` for shared reference-counted ownership, and `std::weak_ptr` to observe a `shared_ptr`-managed object without extending its lifetime or creating reference cycles.
- **Type Deduction:** Use `auto` to let the compiler deduce types — especially with iterator types and complex template return types. Use `decltype` and `decltype(auto)` to preserve value category in forwarding code.
- **Lambdas:** Write anonymous, inline functions for use with STL algorithms. Understand capture clauses: `[=]` (capture all by value), `[&]` (capture all by reference), and `[x, &y]` (mixed). Use generic lambdas with `auto` parameters (C++14).
- **Move Semantics:** The key to C++ performance. Learn rvalue references (`&&`), `std::move` (an unconditional cast to rvalue — it does not itself move anything; the move constructor or assignment does), and `std::forward<T>` (a conditional cast used in **forwarding references** `T&&` to preserve the argument's original lvalue or rvalue category). Together these enable zero-cost resource transfer and are essential for writing generic code that doesn't unnecessarily copy.
- **`constexpr`:** Use `constexpr` to force the compiler to evaluate functions and variables at compile time.
- **`std::function` and Callable Wrappers:** `std::function<Ret(Args...)>` is a type-erased wrapper that stores any callable — lambdas, function pointers, bound member functions, or functors. Use it when you need to store a callback in a non-template context (class member, heterogeneous container, runtime dispatch). For performance-critical paths, prefer template parameters over `std::function` to avoid the virtual-dispatch overhead.

> **Resources**
> - 📖 *Effective Modern C++* (Scott Meyers) — Items 18–22 *(smart pointers)*, 23–29 *(rvalue references and move semantics)*, 31–34 *(lambdas)*
> - [learncpp.com — Chapters 20–22](https://www.learncpp.com) *(move semantics, smart pointers, lambdas)*
> - [CppCon "Back to Basics: Smart Pointers" on YouTube](https://www.youtube.com/@CppCon)
> - [CppCon "Back to Basics: Move Semantics" on YouTube](https://www.youtube.com/@CppCon)

### Exercises

1. Refactor a class hierarchy that uses raw `new`/`delete` to use `std::unique_ptr` and `std::shared_ptr`. Confirm no leaks with AddressSanitizer (`-fsanitize=address`).
2. Implement a move constructor and move assignment operator for a `Buffer` class that owns a heap-allocated `std::byte[]` array. Use a `moved-from` state check to verify correctness.
3. Write a `make_adder(n)` factory that returns a lambda capturing `n` by value, and store several adders in a `std::vector<std::function<int(int)>>`.
4. Implement a perfect-forwarding `invoke(f, args...)` wrapper that calls `f` with `args...`, preserving their value categories, and returns the result via `decltype(auto)`.

---

## Phase 7: Advanced Modern C++ (C++17, C++20, & C++23)

*Estimated time: ~4–6 weeks*

This phase explores the cutting edge of the language, focusing on safer abstractions and highly expressive syntax.

### Modern C++ Feature Evolution

| Version | Key Features Introduced | Primary Use Case |
| --- | --- | --- |
| **C++17** | `std::optional`, `std::variant`, Structured Bindings | Safer return types (no more returning null pointers or sentinel values); cleaner variable unpacking with `auto [a, b] = pair;` |
| **C++17** | `std::string_view`, `std::filesystem` | Zero-allocation string parsing; cross-platform file and directory manipulation |
| **C++17** | `std::execution` policies | Trivially parallelise STL algorithms: `std::sort(std::execution::par, v.begin(), v.end())` |
| **C++20** | Concepts | Add named constraints to templates: `template<std::integral T>` produces human-readable errors instead of multi-page substitution failures |
| **C++20** | Ranges | Functional-style composition: `v \| views::filter(pred) \| views::transform(f) \| views::take(10)` — lazy, no intermediate allocations |
| **C++20** | Modules | Replace `#include` with `import` for faster, more isolated compilation with no header-order bugs |
| **C++20** | Coroutines | `co_await`, `co_yield`, `co_return` — the building blocks for lazy generators and async programming without callback pyramids |
| **C++20** | `std::span` | Non-owning view over contiguous memory; replaces `T*` + size pairs in function signatures |
| **C++23** | `std::expected`, `mdspan`, Deducing `this` | Rust-like error handling without exceptions; multi-dimensional array views; simplified CRTP pattern |

**Concepts in depth:** A concept is a named Boolean predicate on template parameters checked at compile time. Constraining a template with `requires std::regular<T>` produces a diagnostic that names the unsatisfied concept rather than a pages-long substitution chain.

**Ranges in depth:** The Ranges library (introduced in C++20) replaces iterator-pair based algorithms with range-based ones that compose with the `|` pipe operator. Views are lazy — no intermediate containers are allocated during composition.

**Coroutines in depth:** A coroutine is a function that can suspend (`co_yield`, `co_await`) and be resumed later. They are the foundation for C++ generators, async I/O, and cooperative multitasking. The low-level machinery (`promise_type`, `coroutine_handle`) is complex; use a library abstraction (e.g., `cppcoro` or C++23's `std::generator`) in practice.

> **Resources**
> - [CppCon YouTube — "Back to Basics" and feature talks (2020–2024)](https://www.youtube.com/@CppCon)
> - [cppreference.com — Ranges library](https://en.cppreference.com/w/cpp/ranges)
> - [cppreference.com — Coroutines](https://en.cppreference.com/w/cpp/language/coroutines)
> - [cppreference.com — Concepts library](https://en.cppreference.com/w/cpp/concepts)
> - 📖 *C++ Crash Course* (Josh Lospinoso, 2020) — C++17 focused, fast-paced

### Exercises

1. Write an `Arithmetic` concept that accepts only arithmetic types and use it to constrain a `statistics<T>` class template. Confirm that instantiating with `std::string` produces a readable error.
2. Build a lazy pipeline using `std::views` that reads a `std::vector<std::string>`, filters out blank lines, transforms each to uppercase, and collects the first 10 results — without allocating intermediate vectors.
3. Implement a `Generator<uint64_t>` coroutine that yields an infinite Fibonacci sequence. Consume the first 20 values using a range-based `for`.
4. Convert a small utility header into a named C++20 module (`export module myutil;`) and update its `CMakeLists.txt` to use `target_sources(... FILE_SET CXX_MODULES ...)`.
5. Parallelise a `std::transform` over a 10M-element `std::vector<double>` using `std::execution::par`. Benchmark against `std::execution::seq` using `std::chrono::steady_clock` and report the speedup.

---

## Phase 8: Professional Practices & Project Architecture

*Estimated time: ~6–8 weeks*

To transition from a learner to a software engineer, you must master the ecosystem surrounding the language.

- **Debugging:**
  - **GDB** (Linux/macOS/WSL2): set breakpoints (`break`), step through code (`next`, `step`, `finish`), inspect state (`print`, `info locals`, `backtrace`), and perform post-mortem analysis with core dumps (`gdb ./app core`).
  - **LLDB** (macOS/Clang): the native debugger for LLVM-built programs; equivalent capabilities with a slightly different command syntax.
  - Both integrate with VS Code (via the C/C++ extension) and CLion.

- **Memory Safety & Tooling:**
  - **AddressSanitizer** (ASan, `-fsanitize=address`) and **UndefinedBehaviorSanitizer** (UBSan, `-fsanitize=undefined`) are compiler-injected runtime checks available on GCC and Clang on all platforms.
  - **Valgrind** (Linux only — does not run natively on macOS or Windows; use WSL2) provides deeper heap analysis and the Callgrind profiler.
  - On macOS, use ASan + LeakSanitizer or Instruments instead of Valgrind.

- **Static Analysis:** Run **clang-tidy** (integrates with CMake via `CMAKE_CXX_CLANG_TIDY`) and **cppcheck** on every CI build. **include-what-you-use** finds unnecessary `#include` directives and speeds compilation.

- **Unit Testing:** Write tests using **Google Test** (GTest) or **Catch2**. Structure tests into fixtures, use `ASSERT_*` (abort test on failure) vs. `EXPECT_*` (continue test on failure) correctly, and ensure tests are independent, fast, and deterministic.

- **`std::chrono`:** The portable, type-safe timing library for benchmarking. Use `std::chrono::steady_clock::now()` to measure wall-clock intervals. Prefer this over `clock()` or OS-specific APIs.

- **Profiling:** Measure before you optimize.
  - **Linux:** `perf stat`, `perf record` + `perf report`, Valgrind/Callgrind
  - **macOS:** Instruments (Xcode), `sample` command
  - **Windows:** Visual Studio Profiler, vcperf + Windows Performance Analyzer
  - **Cross-platform:** [Compiler Explorer (godbolt.org)](https://godbolt.org) — inspect generated assembly and compare codegen across compilers and optimization flags

- **Multithreading & Concurrency:** Learn `<thread>`, `<mutex>`, `<condition_variable>`, `<future>`, and `<atomic>`. Understand the C++ memory model: acquire/release semantics and `std::memory_order` for lock-free programming. In C++20, prefer `std::jthread` (auto-joins on destruction, preventing detached-thread footguns) with `std::stop_token` for cooperative cancellation. Use `std::latch`, `std::barrier`, and `std::counting_semaphore` (C++20) for synchronization.

- **Networking:** POSIX socket APIs (Linux/macOS) provide the foundation. For practical async networking, use **Asio** (standalone, header-only, no Boost required) — the de-facto standard and the basis for `std::net` (targeted for C++26). Asio works with C++20 coroutines for clean async code.

- **Design Patterns in Modern C++:** Study how Singleton, Factory, Observer, Strategy, CRTP, PIMPL (Pointer to Implementation for ABI stability), and Type Erasure are implemented safely using modern C++ idioms.

- **Data-Oriented Design (DOD) & Optimization:** Prefer structs-of-arrays over arrays-of-structs for cache performance on hot paths. Profile first, then optimize. Understand what the compiler generates before assuming something is slow.

- **Continuous Integration:** Set up a **GitHub Actions** workflow that builds your project with CMake, runs your tests, runs clang-tidy, and runs ASan — on Linux, macOS, and Windows matrix runners simultaneously.

> **Resources**
> - [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — 16 categories of best practices by Stroustrup and Herb Sutter
> - [Google Test documentation](https://google.github.io/googletest/)
> - [Catch2 documentation](https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md)
> - [Asio documentation](https://think-async.com/Asio/)
> - [CppCon "Back to Basics: Concurrency" on YouTube](https://www.youtube.com/@CppCon)

### Exercises

1. Introduce a deliberate use-after-free bug in a test program, locate it using GDB (`backtrace` + `info locals`), and confirm the precise line with AddressSanitizer.
2. Run clang-tidy on a project with `--checks='modernize-*,cppcoreguidelines-*,readability-*'` and fix every warning without suppressing any with `NOLINT`.
3. Profile a naïve matrix-multiplication function with `perf`/Instruments, measure baseline with `std::chrono`, optimize for cache locality (loop order, tiling), and report the speedup.
4. Write a thread-safe bounded producer-consumer queue using `std::jthread`, `std::mutex`, and `std::condition_variable`. Add a `std::stop_token` cancellation path that allows clean shutdown.
5. Set up a GitHub Actions workflow with a build matrix (GCC on Linux, Clang on macOS, MSVC on Windows) that compiles with CMake, runs unit tests, and runs ASan on every push to `main`.

---

## Capstone Projects

Apply everything by building increasingly ambitious projects. Each will expose gaps in your knowledge — that is the point.

1. **CLI Task Manager** *(Phases 2–6)*
   A command-line todo application: add, remove, list, and persist tasks to a file. Focus on `std::string` parsing, STL containers, RAII file handling, smart pointers, and unit testing with Catch2.

2. **Generic Data Structures Library** *(Phases 4, 5, 6)*
   Implement your own `DynArray<T>`, `HashMap<K,V>`, and `LinkedList<T>` with STL-compatible iterators, exception safety guarantees, unit tests, and benchmarks against their STL counterparts.

3. **Multithreaded Log Aggregator** *(Phase 8 — Concurrency)*
   A thread-safe log ingestion service: multiple `std::jthread` producers write entries to a shared queue; a consumer thread batches and flushes them to disk. Profile throughput with `std::chrono` and optimize for concurrency.

4. **Async HTTP Server** *(Phases 7–8 — Coroutines + Networking)*
   A minimal HTTP/1.1 server using **Asio** for async I/O and C++20 coroutines for request handling. Implement `GET` and `POST` endpoints serving a simple JSON API.

5. **Custom STL-Compatible Allocator + Container** *(All Phases)*
   Implement a memory arena allocator and a `vector`-like container that uses it. Implement all five iterator categories, the strong exception safety guarantee, and benchmark allocation performance against the default allocator using `std::chrono`.
