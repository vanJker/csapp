#include "sim.h"

Disk disk = {
    .program =
        {// <add>:
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
         }},
    .program_size = 16,
};
