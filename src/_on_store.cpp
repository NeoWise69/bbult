#include "bbult.interp.hpp"

void op_impl::_on_store(op_impl_args_named(a, _1, _2)) {
    if (a.is_reg) {
        // store register value
        if (a.is_stack_reg()) {
            const auto val = stack[regs[a.reg] + a.offset];
            stack.push_back(val);
        }
        else {
            stack.push_back(regs[a.reg]);
        }
    } 
    else {
        stack.push_back(a.value);
    }
    regs[REGSP] = stack.size();
}
