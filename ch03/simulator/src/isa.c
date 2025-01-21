#include "sim.h"

static void push_reg(Machine *, uint64_t, uint64_t);
static void pop_reg(Machine *, uint64_t, uint64_t);
static void mov_imm2reg(Machine *, uint64_t, uint64_t);
static void mov_reg2reg(Machine *, uint64_t, uint64_t);
static void mov_reg2mem(Machine *, uint64_t, uint64_t);
static void mov_mem2reg(Machine *, uint64_t, uint64_t);
static void add_reg2reg(Machine *, uint64_t, uint64_t);
static void call(Machine *, uint64_t, uint64_t);
static void ret(Machine *, uint64_t, uint64_t);

Handler handler_table[INST_CNT] = {
    [PUSH_REG] = &push_reg,
    [POP_REG] = &pop_reg,
    [MOV_IMM2REG] = &mov_imm2reg,
    [MOV_REG2REG] = &mov_reg2reg,
    [MOV_REG2MEM] = &mov_reg2mem,
    [MOV_MEM2REG] = &mov_mem2reg,
    [ADD_REG2REG] = &add_reg2reg,
    [CALL] = &call,
    [RET] = &ret,
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

static void push_reg(Machine *m, uint64_t src, uint64_t dst)
{
    (void) dst;

    m->cpu->regs.rsp -= sizeof(uint64_t);
    write64bits(m->memory, m->cpu->regs.rsp, *(uint64_t *) src);

    m->cpu->regs.rip += sizeof(Inst);
}

static void pop_reg(Machine *m, uint64_t src, uint64_t dst)
{
    (void) dst;

    *(uint64_t *) src = read64bits(m->memory, m->cpu->regs.rsp);
    m->cpu->regs.rsp += sizeof(uint64_t);

    m->cpu->regs.rip += sizeof(Inst);
}

static void mov_imm2reg(Machine *m, uint64_t src, uint64_t dst)
{
    *(int64_t *) dst = (int64_t) src;
    m->cpu->regs.rip += sizeof(Inst);
}

static void mov_reg2reg(Machine *m, uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = *(uint64_t *) src;
    m->cpu->regs.rip += sizeof(Inst);
}

static void mov_reg2mem(Machine *m, uint64_t src, uint64_t dst)
{
    write64bits(m->memory, dst, *(uint64_t *) src);
    m->cpu->regs.rip += sizeof(Inst);
}

static void mov_mem2reg(Machine *m, uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = read64bits(m->memory, src);
    m->cpu->regs.rip += sizeof(Inst);
}

static void add_reg2reg(Machine *m, uint64_t src, uint64_t dst)
{
    *(uint64_t *) dst = *(uint64_t *) src + *(uint64_t *) dst;
    m->cpu->regs.rip += sizeof(Inst);
}

static void call(Machine *m, uint64_t src, uint64_t dst)
{
    (void) dst;

    // push return address
    m->cpu->regs.rsp -= sizeof(uint64_t);
    write64bits(m->memory, m->cpu->regs.rsp, m->cpu->regs.rip + sizeof(Inst));

    // jump to callee
    m->cpu->regs.rip = src;
}

static void ret(Machine *m, uint64_t src, uint64_t dst)
{
    (void) src;
    (void) dst;

    // pop return address
    uint64_t addr = read64bits(m->memory, m->cpu->regs.rsp);
    m->cpu->regs.rsp += sizeof(uint64_t);

    // jump to caller
    m->cpu->regs.rip = addr;
}
