#include "bbult.interp.hpp"

void op_impl::_on_rest(op_impl_args_named(reg, _1, _2)) {
    if (reg.is_readonly_reg()) {
        fprintf(stderr, "WARN: an attempt to reset readonly register!(pc=%d)\n", regs[REGPC]);
    }
    else {
        regs[reg.reg] = 0;
    }
}
