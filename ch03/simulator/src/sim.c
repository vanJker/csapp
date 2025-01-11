#include "sim.h"

void instruction_cycle(Cpu *cpu)
{
    int i = 1;
    while (i--) {
        Inst *inst = (Inst *) cpu->regs.rip;

        uint64_t src = decode_operand(inst->src);
        uint64_t dst = decode_operand(inst->dst);
        handler_table[inst->type](src, dst);

        cpu->regs.rip += sizeof(Inst);
    }
}

int main(void)
{
    cpu = (Cpu){
        .regs =
            {
                .rax = 0x0123456789abcdef,
                .rip = (uint64_t) &program,
            },
    };
    cpu_dump(&cpu);
    instruction_cycle(&cpu);
    cpu_dump(&cpu);

    return 0;
}
