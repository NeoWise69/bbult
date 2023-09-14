#include "bbult.interp.hpp"

void op_impl::_on_muls(op_impl_args_named(_1, _2, _3)) {
    if (stack.size() < 2) {
        // stack is REQUIRED to hold at least 2 values!
        fprintf(stderr, "WARN: an attempt to SUBS with stack size < 2!(pc=%d)\n", regs[REGPC]);
    }
    else {
        const auto b = stack.back();
        stack.pop_back();
        stack.back() *= b;
    }

    regs[REGSP] = stack.size();
}
