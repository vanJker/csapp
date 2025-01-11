#ifndef _SIM_H
#define _SIM_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MEM_CAPACITY 1024
#define PROGRAM_CAPACITY 128

/* CPU */
typedef struct {
    union {
        struct {
            uint8_t al;
            uint8_t ah;
        };
        uint16_t ax;
        uint32_t eax;
        uint64_t rax;
    };
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;

    uint64_t rip;
} Reg;

typedef struct {
    Reg regs;
} Cpu;

void cpu_dump(Cpu *cpu);

/* Memory */
typedef enum {
    NOP = 0,
    MOV,
    PUSH,
    CALL,
    ADD_REG_REG,
    INST_CNT,
} Inst_Type;

typedef enum {
    IMM,
    REG,
    MEM_IMM,
    MEM_REG,
    MEM_IMM_REG,
    MEM_REG1_REG2,
    MEM_IMM_REG1_REG2,
    MEM_REG_S,
    MEM_IMM_REG_S,
    MEM_REG1_REG2_S,
    MEM_IMM_REG1_REG2_S,
} Operand_Type;

typedef struct {
    Operand_Type type;

    int64_t imm;
    uint64_t *reg1;
    uint64_t *reg2;
    int64_t scale;
} Operand;

uint64_t decode_operand(Operand operand);

typedef struct {
    Inst_Type type;
    Operand src;
    Operand dst;
} Inst;

typedef void (*Handler)(uint64_t, uint64_t);

typedef struct {
    uint8_t data[MEM_CAPACITY];
} Memory;

/* Machine */
extern Cpu cpu;
extern Memory mem;
extern Inst program[PROGRAM_CAPACITY];
extern Handler handler_table[INST_CNT];

void instruction_cycle(Cpu *cpu);

#endif
