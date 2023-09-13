#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <sstream>
#include <map>
#include <memory>

#include "bbult.interp.hpp"

constexpr const char* welcome = {
    "\nBBULT\n"
    "bbult will be probably interpreter with simplest human readable language\n"
};

/**
 * STACK 
 * stack is a little bit tricky, because in memory it's
 * just an ingrowing memory array. and starts with 0.
 * this means that stack base is always ZERO, but
 * stack pointer always at the last element.
 * 0 ~~~~~~~~~~~~~ N
 * ^- BASE         ^- POINTER
*/

auto main(int argc, char** argv) -> int32_t {

    fprintf(stdout, "%s", welcome);

    std::vector<op_t> test_ops = {
        mov(REGC, 12),
        store(REGC),
        mov(REGC, 23),
        load(REGD),
        store(REGC),
        store(REGD),
        adds(),
        load(REGC),
    };
    execute_program(test_ops);
}
