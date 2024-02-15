#include "../mystack/src/mystack.h"
#include "criterionlib/include/criterion/criterion.h"

Test(misc, failing) { cr_assert(1, "hello world"); }

Test(misc, passing) { cr_assert(1, "hello world"); }
