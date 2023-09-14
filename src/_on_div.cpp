#include "bbult.interp.hpp"

/**
 * SUB (DEST, A, B)
 * 
 *  DESTination can be REG(recommended) or STACK(not recommended)
 *  A, B can be REG or STACKPTR/BASE, or IMM value
*/
void op_impl::_on_div(op_impl_args_named(dst, a, b)) {
    int64_t val_a = 0;
    int64_t val_b = 0;
    
    // get a value
    if (a.is_stack_reg()) {
        val_a = stack[regs[a.reg] + a.offset];
    }
    else if (a.is_reg) {
        val_a = regs[a.reg];
    }
    else {
        val_a = a.value;
    }
    // get b value
    if (b.is_stack_reg()) {
        val_b = stack[regs[b.reg] + b.offset];
    }
    else if (b.is_reg) {
        val_b = regs[b.reg];
    }
    else {
        val_b = b.value;
    }

    if (dst.is_stack_reg()) {
        stack[regs[dst.reg] + dst.offset] = val_a / val_b;
    }
    else {
        regs[dst.reg] = val_a / val_b;
    }
}
