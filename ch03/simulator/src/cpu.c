#include "sim.h"

Cpu cpu = {0};

void cpu_dump(Cpu *cpu)
{
    printf("Registers:\n");
    Reg *regs = &cpu->regs;

    printf("rax =\t0x%016lx\n", regs->rax);
    printf("eax =\t0x%08x\n", regs->eax);
    printf("ax  =\t0x%04x\n", regs->ax);
    printf("al  =\t0x%02x\n", regs->al);
    printf("ah  =\t0x%02x\n", regs->ah);

    printf("rbx =\t0x%016lx\n", regs->rbx);
    printf("rcx =\t0x%016lx\n", regs->rcx);
    printf("rdx =\t0x%016lx\n", regs->rdx);
    printf("rsi =\t0x%016lx\n", regs->rsi);
    printf("rdi =\t0x%016lx\n", regs->rdi);
    printf("rbp =\t0x%016lx\n", regs->rbp);
    printf("rsp =\t0x%016lx\n", regs->rsp);

    printf("rip =\t0x%016lx\n", regs->rip);
}
