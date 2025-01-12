#include "sim.h"

Disk disk = {
    .program =
        {
            // <add>:
            {
                // push   %rbp
                PUSH_REG,
                {REG, 0, &cpu.regs.rbp, NULL, 0},
                {EMPTY, 0, NULL, NULL, 0},
            },
            {
                // mov    %rsp,%rbp
                MOV_REG2REG,
                {REG, 0, &cpu.regs.rsp, NULL, 0},
                {REG, 0, &cpu.regs.rbp, NULL, 0},
            },
            {
                // mov    %rdi,-0x18(%rbp)
                MOV_REG2MEM,
                {REG, 0, &cpu.regs.rdi, NULL, 0},
                {MEM_IMM_REG, -0X18, &cpu.regs.rbp, NULL, 0},
            },
            {
                // mov    %rsi,-0x20(%rbp)
                MOV_REG2MEM,
                {REG, 0, &cpu.regs.rsi, NULL, 0},
                {MEM_IMM_REG, -0x20, &cpu.regs.rbp, NULL, 0},
            },
            {
                // mov    -0x18(%rbp),%rdx
                MOV_MEM2REG,
                {MEM_IMM_REG, -0x18, &cpu.regs.rbp, NULL, 0},
                {REG, 0, &cpu.regs.rdx, NULL, 0},
            },
            {
                // mov    -0x20(%rbp),%rax
                MOV_MEM2REG,
                {MEM_IMM_REG, -0x20, &cpu.regs.rbp, NULL, 0},
                {REG, 0, &cpu.regs.rax, NULL, 0},
            },
            {
                // add    %rdx,%rax
                ADD_REG2REG,
                {REG, 0, &cpu.regs.rdx, NULL, 0},
                {REG, 0, &cpu.regs.rax, NULL, 0},
            },
            {
                // mov    %rax,-0x8(%rbp)
                MOV_REG2MEM,
                {REG, 0, &cpu.regs.rax, NULL, 0},
                {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
            },
            {
                // mov    -0x8(%rbp),%rax
                MOV_MEM2REG,
                {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
                {REG, 0, &cpu.regs.rax, NULL, 0},
            },
            {
                // pop    %rbp
                POP_REG,
                {REG, 0, &cpu.regs.rbp, NULL, 0},
                {EMPTY, 0, NULL, NULL, 0},
            },
            {
                // ret
                RET,
                {EMPTY, 0, NULL, NULL, 0},
                {EMPTY, 0, NULL, NULL, 0},
            },
            // <main>:
            {
                // mov    %rdx,%rsi
                MOV_REG2REG,
                {REG, 0, &cpu.regs.rdx, NULL, 0},
                {REG, 0, &cpu.regs.rsi, NULL, 0},
            },
            {
                // mov    %rax,%rdi
                MOV_REG2REG,
                {REG, 0, &cpu.regs.rax, NULL, 0},
                {REG, 0, &cpu.regs.rdi, NULL, 0},
            },
            {
                // call   1129 <add>
                CALL,
                {EMPTY, (uint64_t) disk.program, NULL, NULL, 0},
                {EMPTY, 0, NULL, NULL, 0},
            },
            {
                // mov    %rax,-0x8(%rbp)
                MOV_REG2MEM,
                {REG, 0, &cpu.regs.rax, NULL, 0},
                {MEM_IMM_REG, -0x8, &cpu.regs.rbp, NULL, 0},
            },
        },
    .program_size = 15,
};
