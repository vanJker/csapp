#include "sim.h"

Memory memory = {0};

static void push_reg(Machine *m, uint64_t src, uint64_t dst)
{
    // TODO
}

static void pop_reg(Machine *m, uint64_t src, uint64_t dst)
{
    // TODO
}

static void call(Machine *m, uint64_t src, uint64_t dst)
{
    (void) dst;

    // push return address
    m->cpu->regs.rsp -= sizeof(uint64_t);
    *(uint64_t *) m->cpu->regs.rsp = m->cpu->regs.rip + sizeof(Inst);

    // jump to callee
    m->cpu->regs.rip = src;
}

static void ret(void)
{
    // TODO
}

static void mov_src2dst(Machine *m, uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = *(uint64_t *) src;
    m->cpu->regs.rip += sizeof(Inst);
}

static void add_src2dst(Machine *m, uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = *(uint64_t *) src + *(uint64_t *) dst;
    m->cpu->regs.rip += sizeof(Inst);
}

Handler handler_table[INST_CNT] = {
    [MOV_REG2REG] = &mov_src2dst,
    [MOV_REG2MEM] = &mov_src2dst,
    [MOV_MEM2REG] = &mov_src2dst,
    [ADD_REG2REG] = &add_src2dst,
    [CALL] = &call,
};

uint64_t decode_operand(Operand operand)
{
    switch (operand.type) {
    case EMPTY:
        return 0;
    case IMM:
        return *(uint64_t *) &operand.imm;
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

#define CACHE_SET 0

uint64_t read64bits(Memory *memory, uint64_t paddr)
{
#if CACHE_SET
    return 0;
#endif

    return *(uint64_t *) (&memory->ram[paddr]);
}

void write64bits(Memory *memory, uint64_t paddr, uint64_t data)
{
#if CACHE_SET
    return;
#endif

    *(uint64_t *) (&memory->ram[paddr]) = data;
}
