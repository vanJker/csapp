#ifndef _SIM_H
#define _SIM_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MEM_CAPACITY (1024 * 8)
#define PROGRAM_CAPACITY 128

#define array_siaze(a) (sizeof(a) / sizeof(a[0]))

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
    PUSH_REG,
    POP_REG,
    MOV_REG2REG,
    MOV_REG2MEM,
    MOV_MEM2REG,
    ADD_REG2REG,
    CALL,
    RET,

    INST_CNT,
} Inst_Type;

typedef enum {
    EMPTY = 0,
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
    const char *code;
} Inst;

typedef struct {
    uint8_t ram[MEM_CAPACITY];
    uint8_t *stack;
} Memory;

uint64_t read64bits(Memory *memory, uint64_t paddr);
void write64bits(Memory *memory, uint64_t paddr, uint64_t data);

/* Disk */
typedef struct {
    Inst program[PROGRAM_CAPACITY];
    size_t program_size;
} Disk;

/* Machine */
typedef struct Machine Machine;
typedef void (*Handler)(Machine *, uint64_t, uint64_t);

struct Machine {
    Cpu *cpu;
    Memory *memory;
    Disk *disk;
};

extern Machine machine;
extern Cpu cpu;
extern Memory memory;
extern Disk disk;
extern Handler handler_table[INST_CNT];

void instruction_cycle(Machine *m);
void load_program_from_disk(Machine *m);
void stack_dump(Machine *m);

#endif
