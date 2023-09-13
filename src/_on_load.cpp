#include "bbult.interp.hpp"

void op_impl::_on_load(op_impl_args_named(dst, _1, _2)) {
    if (dst.is_stack_reg()) {
        // WE CANT LOAD A VALUE FROM STACK ONTO THE STACK!!!
        fprintf(stderr, "WARN: attempt to load a value from the stack back into the stack!(pc=%d)\n", regs[REGPC]);
    }
    else {
        regs[dst.reg] = stack.back();
        stack.pop_back();
    }

    regs[REGSP] = stack.size();
}
