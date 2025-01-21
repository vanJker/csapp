#include "sim.h"

static void test(void);

Machine machine = {
    .cpu = &cpu,
    .memory = &memory,
    .disk = &disk,
};

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

int main(void)
{
    cpu = (Cpu){
        .regs =
            {
                .rax = 0x1234,
                .rdx = 0xabcd0000,
                .rbp = (uint64_t) (memory.ram + MEM_CAPACITY),
                .rsp = (uint64_t) (memory.ram + MEM_CAPACITY),
                .rip = (uint64_t) (((Inst *) &memory.ram) + 11),
            },
    };

    // printf("%lx\n", sizeof(Inst));
    cpu_dump(&cpu);
    stack_dump(&machine);

    load_program_from_disk(&machine);
    instruction_cycle(&machine);

    cpu_dump(&cpu);
    stack_dump(&machine);

    test();

    return 0;
}

static void test(void)
{
    assert(cpu.regs.rax == 0x0);
    assert(cpu.regs.rbx == 0x0);
    assert(cpu.regs.rcx == 0x0);
    assert(cpu.regs.rdx == 0x1234);
    assert(cpu.regs.rsi == 0xabcd0000);
    assert(cpu.regs.rdi == 0x1234);
    assert(cpu.regs.rbp == (uint64_t) (memory.ram + MEM_CAPACITY));
    assert(cpu.regs.rsp == (uint64_t) (memory.ram + MEM_CAPACITY));
    assert(cpu.regs.rip ==
           (uint64_t) (((Inst *) &memory.ram) + disk.program_size));

    assert(read64bits(&memory, cpu.regs.rsp - 8) == 0xabcd1234);
    assert(read64bits(&memory, cpu.regs.rsp - 16) == cpu.regs.rsp);
    assert(read64bits(&memory, cpu.regs.rsp - 24) == 0xabcd1234);
    assert(read64bits(&memory, cpu.regs.rsp - 32) == 0x0);
    assert(read64bits(&memory, cpu.regs.rsp - 40) == 0x1234);
    assert(read64bits(&memory, cpu.regs.rsp - 48) == 0xabcd0000);

    printf("===TEST PASS===\n");
}
