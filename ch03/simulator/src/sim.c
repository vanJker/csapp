#include "sim.h"

Machine machine = {
    .cpu = &cpu,
    .memory = &memory,
    .disk = &disk,
};

void instruction_cycle(Machine *m)
{
    size_t n = 15;
    for (size_t i = 0; i < n - 12; i++) {
        Inst *inst = (Inst *) m->cpu->regs.rip;

        uint64_t src = decode_operand(inst->src);
        uint64_t dst = decode_operand(inst->dst);
        handler_table[inst->type](m, src, dst);
        printf("    %s\n", inst->code);

        cpu_dump(m->cpu);
        stack_dump(m);
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

int main(void)
{
    memory.stack = memory.ram + MEM_CAPACITY;
    cpu = (Cpu){
        .regs =
            {
                .rax = 0x1234,
                .rdx = 0xabcd0000,
                .rbp = (uint64_t) memory.stack,
                .rsp = (uint64_t) memory.stack,
                .rip = (uint64_t) (((Inst *) &memory.ram) + 11),
            },
    };

    // printf("%lx\n", sizeof(Inst));
    cpu_dump(&cpu);
    stack_dump(&machine);

    load_program_from_disk(&machine);
    instruction_cycle(&machine);

    // cpu_dump(&cpu);
    // stack_dump(&machine);

    // assert(cpu.regs.rax == 0xabcd1234);

    return 0;
}
