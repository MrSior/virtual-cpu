#include "../mystack/src/mystack.h"
#include "criterionlib/include/criterion/assert.h"
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
    cr_expect(stack.top() == 4, "hello world");
}

Test(stack_methods, pop) {
    container::Stack<int> stack;
    for (int ind = 0; ind < 5; ++ind) {
        stack.push(ind);
    }

    for (int ind = 4; ind >= 0; --ind) {
        cr_expect(stack.pop() == ind, "pushed not correct value");
    }
    cr_expect(stack.empty(), "pop does not delete all data");
}
