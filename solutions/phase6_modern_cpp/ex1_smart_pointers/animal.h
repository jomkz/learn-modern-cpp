// Phase 6, Exercise 1 — Smart Pointers
// Shared base class used by both raw_version and smart_version.

#pragma once

#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(const std::string& name) : name_(name) {}

    // ── Virtual destructor is mandatory for base classes ──────────────────────
    // Without it, deleting a Derived* through a Base* only calls Base::~Base(),
    // leaking the Derived part. The compiler warning -Wnon-virtual-dtor catches
    // this. Always declare a base class destructor as virtual.
    virtual ~Animal() = default;

    virtual void speak() const = 0;  // pure virtual — Animal is abstract

    const std::string& name() const { return name_; }

protected:
    std::string name_;
};

class Dog : public Animal {
public:
    explicit Dog(const std::string& name) : Animal(name) {}
    void speak() const override { std::cout << name_ << " says: Woof!\n"; }
};

class Cat : public Animal {
public:
    explicit Cat(const std::string& name) : Animal(name) {}
    void speak() const override { std::cout << name_ << " says: Meow!\n"; }
};
