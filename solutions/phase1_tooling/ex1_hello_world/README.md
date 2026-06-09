# Phase 1, Exercise 1 — Hello World

**Exercise:** Write a `hello_world` program and compile it manually from the
command line with `g++ -std=c++20 hello.cpp -o hello`. Inspect the binary with
`file hello`.

---

## Step-by-step commands

```sh
# 1. Compile
g++ -std=c++20 hello.cpp -o hello

# 2. Run
./hello
# Expected output: Hello, World!

# 3. Inspect the binary type (ELF on Linux, Mach-O on macOS)
file hello
# Linux example:   hello: ELF 64-bit LSB pie executable, x86-64, ...
# macOS example:   hello: Mach-O 64-bit executable arm64

# 4. List symbols — find main() and the C++ standard library references
nm hello | grep -E '(main|cout)'
# Output includes:
#   ... T main         — 'T' means defined in the text (code) section
#   ... U _ZNSo...     — 'U' means undefined (resolved from libstdc++/libc++)

# 5. Disassemble — see what assembly the compiler generated for main()
objdump -d hello | grep -A 30 '<main>'
# You'll see the function prologue (push rbp; mov rbp,rsp) and the
# call to operator<< on std::cout.
```

## What -std=c++20 does

The `-std=c++20` flag tells the compiler to parse the source as C++20, enabling
modern features like concepts, ranges, and `std::format`. Without it, the compiler
defaults to an older standard (often C++14 or C++17) and may reject valid C++20 code.

Always pass `-std=c++20` (or `-std=c++23`) explicitly rather than relying on the default.
