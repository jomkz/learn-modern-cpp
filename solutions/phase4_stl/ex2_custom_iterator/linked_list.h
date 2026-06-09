// Phase 4, Exercise 2 — Custom Bidirectional Iterator
// Concept: Iterator categories, concept satisfaction, STL algorithm compatibility.
// Demonstrates: std::bidirectional_iterator concept, iterator traits, operator++/--.
//
// An iterator is a generalisation of a pointer. The STL defines five categories:
//   input      — read once, forward only (e.g., std::istream_iterator)
//   output     — write once, forward only (e.g., std::ostream_iterator)
//   forward    — read multiple times, forward only (e.g., std::forward_list)
//   bidirectional — forward AND backward (std::list, std::map)
//   random-access — O(1) jump by n (std::vector, std::deque, raw array)
//
// Each STL algorithm requires a minimum iterator category. std::sort needs
// random-access; std::reverse needs bidirectional; std::find needs forward.
// By satisfying std::bidirectional_iterator, our iterator works with std::reverse,
// std::distance, std::prev, std::next, and any algorithm requiring ≤ bidirectional.
//
// Sentinel node design (same as std::list internally):
//   head_ -> n1 -> n2 -> ... -> tail_ -> sentinel_
//                                         ^
//                                     end() points here
//
// The sentinel node is a dummy node that is always present. It has no value; only
// its prev/next pointers are used. This lets operator--() decrement from end()
// to reach the last real element — without it, decrementing from nullptr crashes.

#pragma once

#include <cstddef>   // std::ptrdiff_t
#include <iterator>  // std::bidirectional_iterator_tag, std::reverse_iterator

// ── Node ─────────────────────────────────────────────────────────────────────
template<typename T>
struct Node {
    T     value{};   // default-init allows sentinel construction without a value
    Node* prev = nullptr;
    Node* next = nullptr;

    Node() = default;                           // for the sentinel node
    explicit Node(const T& v) : value(v) {}
    explicit Node(T&& v) : value(std::move(v)) {}
};

// ── DoublyLinkedList ─────────────────────────────────────────────────────────
template<typename T>
class DoublyLinkedList {
public:
    // ── Iterator ─────────────────────────────────────────────────────────────
    // The iterator is defined as a nested class so it can access the list's
    // private nodes. It stores a plain pointer to the current node.
    class Iterator {
    public:
        // These type aliases are required by iterator traits (std::iterator_traits).
        // Without them, algorithms can't query the iterator's properties.
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        // Default constructor required by std::bidirectional_iterator (via std::semiregular).
        Iterator() : node_(nullptr) {}
        explicit Iterator(Node<T>* node) : node_(node) {}

        // ── Dereference ──────────────────────────────────────────────────────
        reference operator*()  const { return  node_->value; }
        pointer   operator->() const { return &node_->value; }

        // ── Pre-increment (++it): advance forward ────────────────────────────
        Iterator& operator++() {
            node_ = node_->next;
            return *this;
        }
        // Post-increment (it++): return copy before advancing.
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        // ── Pre-decrement (--it): move backward ──────────────────────────────
        // This works because even end() (the sentinel) has a valid prev pointer.
        Iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return node_ == other.node_; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }

    private:
        Node<T>* node_;
    };

    // ── Concept check ────────────────────────────────────────────────────────
    // static_assert at class scope fires at compile time if Iterator fails to
    // satisfy the std::bidirectional_iterator concept. This gives a clear error
    // message rather than a cryptic template substitution failure later.
    static_assert(std::bidirectional_iterator<Iterator>,
                  "Iterator must satisfy std::bidirectional_iterator");

    using iterator = Iterator;
    using reverse_iterator = std::reverse_iterator<Iterator>;

    // Allocate the sentinel on construction. The sentinel is a dummy node that
    // serves as end() — its 'prev' always points to the last real element.
    DoublyLinkedList() : sentinel_(new Node<T>{}) {}

    ~DoublyLinkedList() {
        clear();
        delete sentinel_;
    }

    // No copy for simplicity; focus is on the iterator.
    DoublyLinkedList(const DoublyLinkedList&)            = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_back(const T& value) {
        auto* n = new Node<T>(value);
        // Always link the new node before the sentinel.
        n->next = sentinel_;
        sentinel_->prev = n;
        if (tail_) {
            tail_->next = n;
            n->prev = tail_;
        } else {
            head_ = n;
            n->prev = nullptr;
        }
        tail_ = n;
        ++size_;
    }

    void clear() {
        Node<T>* cur = head_;
        while (cur && cur != sentinel_) {
            Node<T>* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head_ = nullptr;
        tail_ = nullptr;
        sentinel_->prev = nullptr;
        size_ = 0;
    }

    std::size_t size() const noexcept { return size_; }

    // begin() points to the first real node; end() points to the sentinel.
    // When the list is empty, head_ is nullptr and begin() == end().
    Iterator begin() { return Iterator(head_ ? head_ : sentinel_); }
    Iterator end()   { return Iterator(sentinel_); }

    // rbegin/rend allow std::reverse to work. std::reverse_iterator<Iterator>
    // wraps our Iterator and reverses ++ into --.
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }

private:
    Node<T>*    sentinel_;       // always-present dummy end node
    Node<T>*    head_ = nullptr;
    Node<T>*    tail_ = nullptr;
    std::size_t size_ = 0;
};
