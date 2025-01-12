#include "sim.h"

Machine machine = {
    .cpu = &cpu,
    .memory = &memory,
    .disk = &disk,
};

void instruction_cycle(Machine *m)
{
    size_t n = 15;
    for (size_t i = 0; i < n; i++) {
        Inst *inst = (Inst *) m->cpu->regs.rip;

        uint64_t src = decode_operand(inst->src);
        uint64_t dst = decode_operand(inst->dst);
        handler_table[inst->type](m, src, dst);

        m->cpu->regs.rip += sizeof(Inst);
    }
}

void load_program_from_disk(Machine *m)
{
    memcpy(&m->memory->ram, &m->disk->program,
           m->disk->program_size * sizeof(m->disk->program[0]));
}

int main(void)
{
    memory.stack = memory.ram + MEM_CAPACITY - 1;
    cpu = (Cpu){
        .regs =
            {
                .rax = 0x1234,
                .rdx = 0xabcd0000,
                .rbp = (uint64_t) memory.stack,
                .rsp = (uint64_t) memory.stack,
                .rip = (uint64_t) ((Inst *) &memory.ram + 11),
            },
    };
    cpu_dump(&cpu);
    load_program_from_disk(&machine);
    // instruction_cycle(&cpu);
    // cpu_dump(&cpu);

    // assert(cpu.regs.rax == 0xabcd1234);

    return 0;
}
