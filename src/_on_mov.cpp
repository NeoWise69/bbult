#include "bbult.interp.hpp"

void op_impl::_on_mov(op_impl_args_named(kind, src, dst)) {
    if (not(src.is_stack_reg() || dst.is_stack_reg())) {
        // simplest case
        switch (kind.value) {
            default:
            case MOD_TOREG_IMM: {
                regs[dst.reg] = src.value;
            } return;
            case MOD_TOREG_REG: {
                regs[dst.reg] = regs[src.reg];
            } return;
        }
    }
    else if (src.is_stack_reg() && dst.is_stack_reg()) {
        if (stack.size() > src.offset || src.offset < 0) {
            fprintf(stdout, "[WARN]: out of stack memory bounds! (pc=%d, offset=%d, stack_size=%d)", regs[REGPC], src.offset, stack.size());
            regs[dst.reg] = 0;
        }
        if (stack.size() > dst.offset || dst.offset < 0) {
            fprintf(stdout, "[WARN]: out of stack memory bounds! (pc=%d, offset=%d, stack_size=%d)", regs[REGPC], src.offset, stack.size());
            regs[dst.reg] = 0;
        }
        {
            stack[regs[dst.reg] + dst.offset] = stack[regs[src.reg] + src.offset];
        }
    }
    else if (src.is_stack_reg()) {
        // retrieve a value from the stack, or warn with 'out_of_stack_bounds' message!
        if (stack.size() > src.offset || src.offset < 0) {
            fprintf(stdout, "[WARN]: out of stack memory bounds! (pc=%d, offset=%d, stack_size=%d)", regs[REGPC], src.offset, stack.size());
            regs[dst.reg] = 0;
            return;
        }
        const auto val = stack[regs[src.reg] + src.offset];
        regs[dst.reg] = val;
        return;
    }
    else if (dst.is_stack_reg()) {
        // set a value to stack
        if (stack.size() > dst.offset || dst.offset < 0) {
            fprintf(stdout, "[WARN]: out of stack memory bounds! (pc=%d, offset=%d, stack_size=%d)", regs[REGPC], src.offset, stack.size());
            regs[dst.reg] = 0;
            return;
        }
        auto& set = stack[regs[dst.reg] + dst.offset];
        switch (kind.value) {
            default:
            case MOD_TOREG_IMM: {
                set = src.value;
            } return;
            case MOD_TOREG_REG: {
                set = regs[src.reg];
            } return;
        }
    }
}
