#include "mystack.h"
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stack>
#include <type_traits>

class C {
    int cl;

  public:
    C() { cl = 3; }
    C(int x) {
        cl = x;
        return;
    }
};

class A {
  public:
    int a;
    int b;

    A() = default;

    A(int a, int b) {
        this->a = a;
        this->b = b;
    }
    A(A &&t) {
        a = t.a;
        b = t.b;
    }

    A &operator=(A &&t) {
        int a = 0;
        return *this;
    }

    ~A() { std::cout << "destuctor call\n"; }
};

int main() {

    container::Stack<int> a;

    for (int i = 0; i < 4; ++i) {
        a.push(i);
    }

    while (!a.empty()) {
        // std::cout << a.pop() << '\n';
        std::cout << a.pop() << '\n';
    }

    return 0;
}
