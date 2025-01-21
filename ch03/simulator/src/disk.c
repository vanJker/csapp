#include "sim.h"

Disk disk = {0};

void write_program_to_disk(Disk *disk, Inst *program, size_t program_size)
{
    memcpy(&disk->program, program, program_size * sizeof(program[0]));
    disk->program_size = program_size;
}

void reset_disk(Disk *disk)
{
    assert(disk != NULL);
    memset(disk, 0, sizeof(Disk));
}

Inst add_program[] = {
    // <add>:
    {
        .type = PUSH_REG,
        .src = {REG, 0, &cpu.regs.rbp, NULL, 0},
        .dst = {EMPTY, 0, NULL, NULL, 0},
        .code = "push   %rbp",
    },
    {
        .type = MOV_REG2REG,
        .src = {REG, 0, &cpu.regs.rsp, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rbp, NULL, 0},
        .code = "mov    %rsp,%rbp",
    },
    {
        .type = MOV_REG2MEM,
        .src = {REG, 0, &cpu.regs.rdi, NULL, 0},
        .dst = {MEM_IMM_REG, -0X18, &cpu.regs.rbp, NULL, 0},
        .code = "mov    %rdi,-0x18(%rbp)",
    },
    {
        .type = MOV_REG2MEM,
        .src = {REG, 0, &cpu.regs.rsi, NULL, 0},
        .dst = {MEM_IMM_REG, -0x20, &cpu.regs.rbp, NULL, 0},
        .code = "mov    %rsi,-0x20(%rbp)",
    },
    {
        .type = MOV_MEM2REG,
        .src = {MEM_IMM_REG, -0x18, &cpu.regs.rbp, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rdx, NULL, 0},
        .code = "mov    -0x18(%rbp),%rdx",
    },
    {
        .type = MOV_MEM2REG,
        .src = {MEM_IMM_REG, -0x20, &cpu.regs.rbp, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rax, NULL, 0},
        .code = "mov    -0x20(%rbp),%rax",
    },
    {
        .type = ADD_REG2REG,
        .src = {REG, 0, &cpu.regs.rdx, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rax, NULL, 0},
        .code = "add    %rdx,%rax",
    },
    {
        .type = MOV_REG2MEM,
        .src = {REG, 0, &cpu.regs.rax, NULL, 0},
        .dst = {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
        .code = "mov    %rax,-0x8(%rbp)",
    },
    {
        .type = MOV_MEM2REG,
        .src = {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rax, NULL, 0},
        .code = "mov    -0x8(%rbp),%rax",
    },
    {
        .type = POP_REG,
        .src = {REG, 0, &cpu.regs.rbp, NULL, 0},
        .dst = {EMPTY, 0, NULL, NULL, 0},
        .code = "pop    %rbp",
    },
    {
        .type = RET,
        .src = {EMPTY, 0, NULL, NULL, 0},
        .dst = {EMPTY, 0, NULL, NULL, 0},
        .code = "ret",
    },
    // <main>:
    {
        .type = MOV_REG2REG,
        .src = {REG, 0, &cpu.regs.rdx, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rsi, NULL, 0},
        .code = "mov    %rdx,%rsi",
    },
    {
        .type = MOV_REG2REG,
        .src = {REG, 0, &cpu.regs.rax, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rdi, NULL, 0},
        .code = "mov    %rax,%rdi",
    },
    {
        .type = CALL,
        .src = {IMM, (uint64_t) disk.program, NULL, NULL, 0},
        .dst = {EMPTY, 0, NULL, NULL, 0},
        .code = "call   1129 <add>",
    },
    {
        .type = MOV_REG2MEM,
        .src = {REG, 0, &cpu.regs.rax, NULL, 0},
        .dst = {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
        .code = "mov    %rax,-0x8(%rbp)",
    },
    {
        .type = MOV_IMM2REG,
        .src = {IMM, 0, NULL, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rax, NULL, 0},
        .code = "mov    $0x0,%eax",
    }};
size_t add_program_size = ARRAY_SIZE(add_program);

Inst sum_program[] = {
    // <main>:
    {
        .type = MOV_REG2REG,
        .src = {REG, 0, &cpu.regs.rax, NULL, 0},
        .dst = {REG, 0, &cpu.regs.rdi, NULL, 0},
        .code = "mov    %rax,%rdi",
    }};
size_t sum_program_size = ARRAY_SIZE(sum_program);

// <sum>:
// push   %rbp
// mov    %rsp,%rbp
// sub    $0x10,%rsp
// mov    %rdi,-0x8(%rbp)
// cmpq   $0x0,-0x8(%rbp)
// jne    1147 <sum+0x1e>
// mov    $0x0,%eax
// jmp    115e <sum+0x35>
// mov    -0x8(%rbp),%rax
// sub    $0x1,%rax
// mov    %rax,%rdi
// call   1129 <sum>
// mov    -0x8(%rbp),%rdx
// add    %rdx,%rax
// leave
// ret
// <main>:
// mov    %rax,%rdi
// call   1129 <sum>
// mov    %rax,-0x8(%rbp)
// mov    $0x0,%eax
// leave
// ret
