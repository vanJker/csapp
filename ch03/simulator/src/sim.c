#include "sim.h"

#define DEBUG 0

static void test(void);

int main(void)
{
    test();

    return 0;
}

static void test_add(void)
{
    reset_machine(&machine);

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

#if DEBUG
    cpu_dump(&cpu);
    stack_dump(&machine);
#endif

    extern Inst add_program[];
    extern size_t add_program_size;
    write_program_to_disk(machine.disk, add_program, add_program_size);
    load_program_from_disk(&machine);
    instruction_cycle(&machine);

#if DEBUG
    cpu_dump(&cpu);
    stack_dump(&machine);
#endif

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

static void test_sum(void)
{
    reset_machine(&machine);

    cpu = (Cpu){
        .regs =
            {
                .rax = 0xa,
                .rbp = (uint64_t) (memory.ram + MEM_CAPACITY),
                .rsp = (uint64_t) (memory.ram + MEM_CAPACITY),
                .rip = (uint64_t) (((Inst *) &memory.ram) + 0),
            },
    };

#if DEBUG
    cpu_dump(&cpu);
    stack_dump(&machine);
#endif

    extern Inst sum_program[];
    extern size_t sum_program_size;
    write_program_to_disk(machine.disk, sum_program, sum_program_size);
    load_program_from_disk(&machine);
    instruction_cycle(&machine);

#if DEBUG
    cpu_dump(&cpu);
    stack_dump(&machine);
#endif

    printf("===TEST PASS===\n");
}

static void test(void)
{
    test_add();
    test_sum();
}
