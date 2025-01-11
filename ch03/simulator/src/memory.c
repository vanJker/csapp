#include "sim.h"

Memory mem = {0};

static void add_reg_reg(uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = *(uint64_t *) src + *(uint64_t *) dst;
}

Handler handler_table[INST_CNT] = {
    [ADD_REG_REG] = &add_reg_reg,
};

uint64_t decode_operand(Operand operand)
{
    switch (operand.type) {
    case IMM:
        return operand.imm;
    case REG:
        return (uint64_t) operand.reg1;
    case MEM_IMM:
        return operand.imm;
    case MEM_REG:
        return *operand.reg1;
    case MEM_IMM_REG:
        return operand.imm + *operand.reg1;
    case MEM_REG1_REG2:
        return *operand.reg1 + *operand.reg2;
    case MEM_IMM_REG1_REG2:
        return operand.imm + *operand.reg1 + *operand.reg2;
    case MEM_REG_S:
        return *operand.reg1 * operand.scale;
    case MEM_IMM_REG_S:
        return operand.imm + *operand.reg1 * operand.scale;
    case MEM_REG1_REG2_S:
        return *operand.reg1 + *operand.reg2 * operand.scale;
    case MEM_IMM_REG1_REG2_S:
        return operand.imm + *operand.reg1 + *operand.reg2 * operand.scale;
    default:
        assert(0 && "Unsupport operand type");
    }
}
