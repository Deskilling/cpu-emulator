#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

#include "../hardware/cpu.h"
#include "../hardware/mem.h"

#define REGISTER_COUNT 16
#define DATA_MEM_SIZE 256
#define PROGRAM_MEM_SIZE 256

typedef enum {
    MOV_RN_ADDR = 0,
    MOV_ADDR_RN = 1,
    MOV_RN_READ_RM = 2,
    MOV_READ_RN_RM = 3,
    MOV_RN_LITERAL = 4,
    MOV_RN_RM = 5,
    ADD = 6,
    SUB = 7,
    MUL = 8,
    DIV = 9,
    AND = 10,
    OR = 11,
    JZ = 12,
    CMP = 13,
    LESS = 14,
    NOP = 15
} e_opcode;

typedef struct {
    s_cpu cpu;
    s_mem mem;
}s_emulator;

void init_emulator(s_emulator* emu);
void free_emulator(s_emulator* emu);
void exec_emulator(s_emulator* emu, uint16_t instr);

uint8_t load_hex(const char* filename, s_emulator* emu);
void run_emulator(s_emulator* emu, uint8_t instr_count);

#endif
