
#include "sim.h"

Machine machine = {
    .cpu = &cpu,
    .memory = &memory,
    .disk = &disk,
};

void reset_machine(Machine *m)
{
    assert(m != NULL);
    reset_cpu(m->cpu);
    reset_memory(m->memory);
    reset_disk(m->disk);
}

void instruction_cycle(Machine *m)
{
    size_t n = m->disk->program_size;
    for (size_t i = 0; i < n; i++) {
        Inst *inst = (Inst *) m->cpu->regs.rip;

        uint64_t src = decode_operand(inst->src);
        uint64_t dst = decode_operand(inst->dst);
        handler_table[inst->type](m, src, dst);

        // printf("\n");
        printf("    %p    %s\n", (void *) inst, inst->code);
        // cpu_dump(m->cpu);
        // stack_dump(m);
    }
}

void load_program_from_disk(Machine *m)
{
    memcpy(&m->memory->ram, &m->disk->program,
           m->disk->program_size * sizeof(m->disk->program[0]));
}

void stack_dump(Machine *m)
{
    printf("Stack:\n");

    size_t n = 8;
    uint64_t *sp = (uint64_t *) m->cpu->regs.rsp;
    uint64_t *addr = ((uint64_t *) m->cpu->regs.rsp) + n;
    for (size_t i = 0; i < 2 * n; i++) {
        printf("%p: %016lx", (void *) addr, *addr);
        if (addr == sp) {
            printf(" <== rsp");
        }
        printf("\n");

        addr--;
    }
}
