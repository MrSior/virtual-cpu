#include "../mystack/src/mystack.h"
#include "criterionlib/include/criterion/criterion.h"
#include <cstddef>
#include <stack>

Test(stack_methods, empty_pop) {
    container::Stack<int> a;
    cr_expect_any_throw(a.pop(), "expected error throw of pop of empty atack");
}

Test(stack_methods, push) {
    container::Stack<int> stack;
    for (int ind = 0; ind < 5; ++ind) {
        stack.push(ind);
    }
    cr_expect(1, "hello world");
}
