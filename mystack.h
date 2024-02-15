#ifndef VIRTUALCPU_MYSTACK_H
#define VIRTUALCPU_MYSTACK_H

#include <cassert>
#include <concepts>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

// template <typename T>
// concept Number = std::integral<T> || std::floating_point<T>;

namespace container {
template <typename T>
concept StackType = (std::default_initializable<T>)&&(
    std::is_move_assignable_v<T>)&&(std::is_move_constructible_v<T>);

template <StackType T> class Stack;

template <StackType T> void swap(Stack<T> &a, Stack<T> &b);

template <StackType T> class Stack {
  public:
    Stack() {
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    Stack(Stack<T> &other) : size_(other.size_) {
        auto other_iter = other.head_;
        while (other_iter != nullptr) {
            Push(other_iter->data);
            other_iter = other_iter->next;
        }
    }

    Stack(Stack<T> &&other) noexcept
        : head_(std::exchange(other.head_, nullptr)),
          tail_(std::exchange(other.tail_, nullptr)),
          size_(std::exchange(other.size__, 0)) {}

    Stack &operator=(Stack<T> other) {
        swap(*this, other);
        return *this;
    }

    Stack &operator=(Stack<T> &&other) noexcept {
        if (this != &other) {
            delete head_;

            head_ = std::exchange(other.head_, nullptr);
            tail_ = std::exchange(other.tail_, nullptr);
            size_ = std::exchange(other.size_, nullptr);
        }

        return *this;
    }

    void push(const T &data) {
        if (tail_ == nullptr) {
            head_ = tail_ = new Node(data);
        } else {
            tail_->next = new Node(data);

            tail_->next->prev = tail_;
            tail_ = tail_->next;
        }
    }

    void push(T &&data) { push(static_cast<const T &>(data)); }

    T pop() {
        if (empty()) {
            throw std::runtime_error("can not pop from an empty stack");
            return 0;
        }
        T ret_value(std::move(tail_->data));

        if (tail_ == head_) {
            delete tail_;
            head_ = tail_ = nullptr;
        } else {
            auto new_tail = tail_->prev;
            delete tail_;
            tail_ = new_tail;
            tail_->next = nullptr;
        }
        return ret_value;
    }

    T top() {
        if (tail_ != nullptr) {
            return tail_->data;
        }
        return T();
    }

    bool empty() { return head_ == nullptr; }

    friend void swap(Stack<T> &a, Stack<T> &b);

  private:
    class Node {
      public:
        Node *next;
        Node *prev;
        T data;

        Node() {
            next = prev = nullptr;
            data = T();
        }

        Node(const T &data) {
            next = prev = nullptr;
            this->data = data;
        }

        Node(T &&data) {
            next = prev = nullptr;
            this->data = std::move(data);
        }

        ~Node() { delete next; }
    };
    Node *tail_;
    Node *head_;

    size_t size_;
};

template <StackType T> void swap(Stack<T> &a, Stack<T> &b) {
    auto tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

} // namespace container

#endif // VIRTUALCPU_MYSTACK_H
