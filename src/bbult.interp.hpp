#pragma once

#include <iostream>
#include <vector>
#include <span>
#include <cstdint>
#include <cstdlib>

enum : int64_t {
    OP_MOV, // copy value from reg/imm to reg/mem
    OP_STORE, // store value from src to stack
    OP_LOAD, // load value from stack to reg
    OP_ADD, // sum values
    OP_ADDS, // sum values on the stack

    _OP_NUM,
    MOD_TOREG_IMM,
    MOD_TOREG_REG,
};


#define op_impl_args_named(a, b, c) REG_ARG a, REG_ARG b, REG_ARG c
#define op_impl_args op_impl_args_named(a, b, c)

inline std::vector<int64_t> stack = {};
enum REG : int64_t {
    REGA,   // general purpose register 
    REGB,   // general purpose register 
    REGC,   // general purpose register 
    REGD,   // general purpose register 
    REGSP,  // stack pointer register
    REGBP,  // stack base register
    REGPC,  // program counter register

    _REGS_COUNT,
};
inline int64_t regs[_REGS_COUNT] = {};

struct REG_ARG {
    union {
        struct {
            REG reg : 4;
            int64_t offset : 4;
        };
        int64_t value = {};
    };
    bool is_reg = false;

    constexpr REG_ARG(int64_t v = 0) noexcept : value(v) {}
    constexpr REG_ARG(REG r) noexcept : reg(r), is_reg(true) {}
    constexpr REG_ARG(REG r, int64_t off) noexcept : reg(r), offset(off), is_reg(true) {}

    inline auto is_stack_reg() {
        return reg == REGBP || reg == REGSP;
    }

    inline operator int64_t() const noexcept {
        return reg;
    }
};

typedef void(*pfn_op_impl)(op_impl_args);

namespace op_impl {
    void _on_mov(op_impl_args);
    void _on_store(op_impl_args);
    void _on_load(op_impl_args);
    void _on_add(op_impl_args);
    void _on_adds(op_impl_args);
}

const pfn_op_impl _op_impl[_OP_NUM] = {
    &op_impl::_on_mov,
    &op_impl::_on_store,
    &op_impl::_on_load,
    &op_impl::_on_add,
    &op_impl::_on_adds,
};

struct op_t {
    int64_t op = {};
    REG_ARG val0 = {};
    REG_ARG val1 = {};
    REG_ARG val2 = {};
};

/**
 * MOV (TO, VAL)
*/
inline auto mov(REG_ARG reg, int64_t val) {
    return op_t{
        .op = OP_MOV,
        .val0 = MOD_TOREG_IMM,
        .val1 = val,
        .val2 = reg
    };
}

/**
 * MOV (TO, FROM)
*/
inline auto mov(REG_ARG reg_dst, REG_ARG reg_src) {
    return op_t{
        .op = OP_MOV,
        .val0 = MOD_TOREG_REG,
        .val1 = reg_src,
        .val2 = reg_dst,
    };
}

inline auto store(REG_ARG reg_src) {
    return op_t{
        .op = OP_STORE,
        .val0 = reg_src,
    };
}

inline auto load(REG_ARG reg_dst) {
    return op_t{
        .op = OP_LOAD,
        .val0 = reg_dst
    };
}

inline auto add(REG_ARG reg_dst, REG_ARG a, REG_ARG b) {
    return op_t{
        .op = OP_ADD,
        .val0 = reg_dst,
        .val1 = a,
        .val2 = b
    };
}

inline auto adds() {
    return op_t{
        .op = OP_ADDS,
    };
}

inline auto execute_single(const op_t& op) {
    if (const auto fn = _op_impl[op.op]) {
        fn(op.val0, op.val1, op.val2);
    }
}

inline auto dump_state() {
    fprintf(stdout, "RA=%d\n", regs[REGA]);
    fprintf(stdout, "RB=%d\n", regs[REGB]);
    fprintf(stdout, "RC=%d\n", regs[REGC]);
    fprintf(stdout, "RD=%d\n", regs[REGD]);
    fprintf(stdout, "SP=%d\n", regs[REGSP]);
    fprintf(stdout, "BP=%d\n", regs[REGBP]);
    fprintf(stdout, "PC=%d\n", regs[REGPC]);

    fprintf(stdout, "stack: \noff | val\n");
    if (not stack.empty()) {
        for (int64_t i = stack.size() - 1; i >= 0; i--) {
            fprintf(stdout,
                        " %d    %d \n", stack.size() - i, stack.at(i));
        }
    }
}

inline auto execute_program(const std::span<op_t>& code) {
    int64_t& pc = regs[REGPC];
    for (; pc < code.size(); pc++) {
        execute_single(code[pc]);
    }

    dump_state();
}
