#include "mystack/src/mystack.h"
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

    A() {
        a = b = 0;
        std::cout << "LOG: default cstr call" << std::endl;
    };

    A(int a, int b) {
        this->a = a;
        this->b = b;
        std::cout << "LOG: cstr from data call" << std::endl;
    }

    A(const A &t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: copy cstr call" << std::endl;
    }

    A(A &&t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: move cstr call" << std::endl;
    }

    A &operator=(A &&t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: move assignment operator call" << std::endl;
        return *this;
    }

    A &operator=(const A &t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: assignment operator call" << std::endl;
        return *this;
    }

    ~A() { std::cout << "LOG: destuctor call\n"; }
};

int main() {

    // container::Stack<A> a;

    // for (int i = 0; i < 3; ++i) {
    //     a.push(A({i, i}));
    // }

    // std::cout << "_________pop()_________\n";
    // A elem = a.pop();
    // std::cout << elem.a << " " << elem.b << '\n';
    // std::cout << "_______________________\n";

    return 0;
}
