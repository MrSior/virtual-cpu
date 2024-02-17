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

namespace container {
template <typename T>
concept StackType = (std::default_initializable<T>)&&(
    std::is_move_assignable_v<T>)&&(std::is_move_constructible_v<T>);

template <StackType T> class Stack {
  public:
    Stack() = default;

    Stack(const Stack<T> &other) : size_(other.size_) {
        auto other_iter = other.head_;
        while (other_iter != nullptr) {
            push(other_iter->data);
            other_iter = other_iter->next;
        }
    }

    Stack(Stack<T> &&other) noexcept
        : head_(std::exchange(other.head_, nullptr)),
          size_(std::exchange(other.size_, 0)) {}

    Stack &operator=(Stack<T> other) {
        swap(*this, other);
        return *this;
    }

    Stack &operator=(Stack<T> &&other) noexcept {
        if (this != &other) {
            Stack<T> tmp(std::move(other));
            swap(tmp, *this);
        }

        return *this;
    }

    template <typename U>
    void push(U &&data)
        requires std::same_as<T, std::remove_reference_t<U>>
    {
        Node *new_node = new Node(std::forward<U>(data));
        new_node->next = head_;
        head_ = new_node;
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("can not pop from an empty stack");
        }
        T ret_value(std::move(head_->data));
        Node *new_head = head_->next;
        delete head_;
        head_ = new_head;

        return ret_value;
    }

    T top() {
        if (head_ == nullptr) {
            throw std::runtime_error(
                "can not get top element from an empty stack");
        }
        return head_->data;
    }

    bool empty() { return head_ == nullptr; }

    friend void swap(Stack<T> &a, Stack<T> &b);

  private:
    class Node {
      public:
        Node *next = nullptr;
        T data = T();

        Node() = default;

        Node(const T &data) : data(data) {}

        Node(T &&data) : data(std::move(data)) {}

        ~Node() { delete next; }
    };
    Node *head_ = nullptr;

    size_t size_ = 0;
};

template <StackType T> void swap(Stack<T> &a, Stack<T> &b) {
    auto tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

} // namespace container

#endif // VIRTUALCPU_MYSTACK_H
