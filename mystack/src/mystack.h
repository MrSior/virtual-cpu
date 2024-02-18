#ifndef VIRTUALCPU_MYSTACK_H
#define VIRTUALCPU_MYSTACK_H

#include <cassert>
#include <concepts>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace container {
template <typename T>
concept StackType = (std::default_initializable<T>)&&(
    std::is_move_assignable_v<T>)&&(std::is_move_constructible_v<T>);

template <StackType T, typename Alloc = std::allocator<T>> class Stack {
  public:
    Stack(const Alloc &alloc = Alloc()) : alloc_(alloc) {}

    Stack(const Stack<T> &other) : size_(other.size_) {
        Node *other_iter = other.head_;
        Node *iter = nullptr;
        while (other_iter != nullptr) {
            if (iter == nullptr) {
                head_ = iter = node_alloc_traits_::allocate(node_alloc_, 1);
                node_alloc_traits_::construct(node_alloc_, iter,
                                              other_iter->data);
            } else {
                iter->next = node_alloc_traits_::allocate(node_alloc_, 1);
                node_alloc_traits_::construct(node_alloc_, iter->next,
                                              other_iter->data);
                iter = iter->next;
            }
            other_iter = other_iter->next;
        }
    }

    Stack(Stack<T> &&other) noexcept
        : head_(std::exchange(other.head_, nullptr)),
          size_(std::exchange(other.size_, 0)) {
        if (node_alloc_traits_::propagate_on_container_move_assignment::value ==
            true) {
            alloc_ = std::move(other.alloc_);
        } else {
            alloc_ = Alloc();
        }
    }

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
        // Node *new_node = new Node(std::forward<U>(data));
        Node *new_node = node_alloc_traits_::allocate(node_alloc_, 1);
        node_alloc_traits_::construct(node_alloc_, new_node,
                                      std::forward<U>(data));
        new_node->next = head_;
        head_ = new_node;
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("can not pop from an empty stack");
        }

        T ret_value(std::move(head_->data));
        Node *new_head = head_->next;
        node_alloc_traits_::deallocate(node_alloc_, head_, 1);
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

    // using node_alloc_ = typename Alloc::template rebind<Node>::other;

    Alloc alloc_;
    using alloc_traits_ = std::allocator_traits<Alloc>;

    using node_alloc_t_ = typename alloc_traits_::template rebind_alloc<Node>;
    node_alloc_t_ node_alloc_;
    using node_alloc_traits_ =
        typename alloc_traits_::template rebind_traits<Node>;

    // using node_alloc_t_ = typename alloc_traits_::template
    // rebind_alloc<Node>; using node_alloc_traits_t_ =
    //     typename alloc_traits_::template rebind_traits<Node>;
    // node_alloc_t_ node_alloc_;
    // node_alloc_traits_t_ node_alloc_traits_;
};

template <StackType T> void swap(Stack<T> &a, Stack<T> &b) {
    auto tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

} // namespace container

#endif // VIRTUALCPU_MYSTACK_H
