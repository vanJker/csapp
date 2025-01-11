#include "sim.h"

Inst program[PROGRAM_CAPACITY] = {
    {
        ADD_REG_REG,
        {REG, 0, &cpu.regs.rax, NULL, 0},
        {REG, 0, &cpu.regs.rbx, NULL, 0},
    },
};
