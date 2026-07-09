#include "emulator.h"
#include "../settings.h"
#include "src/hardware/cpu.h"
#include "src/hardware/mem.h"
#include "../log/log.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_emulator(s_emulator* emu) {
	if (!emu) {
		fprintf(stderr, "invalid emulator pointer\nquitting program");
		exit(-1);
	}

	emu->cpu = malloc(sizeof(s_cpu));
	if (!init_cpu(emu->cpu)) {
		if (emu->cpu) {
			free(emu->cpu);
			emu->cpu = NULL;
		}
		exit(-1);
	}

	emu->mem = malloc(sizeof(s_mem));
	if (!init_mem(emu->mem)) {
		if (emu->mem) {
			free(emu->mem);
			emu->mem = NULL;
		}
		exit(-1);
	}
}

void free_emulator(s_emulator* emu) {
	if (!emu) {
		fprintf(stderr, "invalid emulator pointer\nquitting program");
		exit(-1);
	}

	free_cpu(emu->cpu);
	emu->cpu = NULL;

	free_mem(emu->mem);
	emu->mem = NULL;
}

void exec_emulator(s_emulator* emu, uint16_t instr) {
	if (!emu) {
		fprintf(stderr, "invalid emulator pointer\nquitting program");
		exit(-1);
	}

	e_opcode opcode = (instr >> 12) & 0xF;
	uint8_t rn = (instr >> 8) & 0xF;
	uint8_t rm = instr & 0xFF;
	uint8_t rm_reg = rm >> 4;
	uint8_t rm_addr = rm & 0xFF;

	switch (opcode) {
	case MOV_RN_ADDR:
		emu->cpu->reg[rn] = emu->mem->data[rm_addr];
		break;
	case MOV_ADDR_RN:
		emu->mem->data[rm_addr] = emu->cpu->reg[rn];
		break;
	case MOV_RN_READ_RM:
		emu->cpu->reg[rn] = emu->mem->data[emu->cpu->reg[rm_reg % REGISTER_COUNT]];
		break;
	case MOV_READ_RN_RM:
		emu->mem->data[emu->cpu->reg[rn] % 256] = emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case MOV_RN_LITERAL:
		emu->cpu->reg[rn] = rm_addr;
		break;
	case MOV_RN_RM:
		emu->cpu->reg[rn] = emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case ADD:
		emu->cpu->reg[rn] += emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case SUB:
		emu->cpu->reg[rn] -= emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case MUL:
		emu->cpu->reg[rn] *= emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case DIV:
		if (emu->cpu->reg[rm_reg % REGISTER_COUNT] != 0) {
			emu->cpu->reg[rn] /= emu->cpu->reg[rm_reg % REGISTER_COUNT];
		}
		break;
	case AND:
		emu->cpu->reg[rn] &= emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case OR:
		emu->cpu->reg[rn] |= emu->cpu->reg[rm_reg % REGISTER_COUNT];
		break;
	case JZ:
		if (emu->cpu->reg[rn] == 0) {
			emu->cpu->pc += rm_addr;
		}
		break;
	case CMP:
		emu->cpu->reg[rn] = (emu->cpu->reg[rn] == emu->cpu->reg[rm_reg % REGISTER_COUNT]);
		break;
	case LESS:
		emu->cpu->reg[rn] = (emu->cpu->reg[rn] < emu->cpu->reg[rm_reg % REGISTER_COUNT]);
		break;
	case NOP:
		break;
	default:
		fprintf(stderr, "opcode %u not found\nquitting program", opcode);
		exit(-1);
	}

	emu->cpu->pc++;
}

void run_emulator(s_emulator* emu) {
	while (emu->cpu->pc < emu->instrCnt) {
#ifdef DEBUG
		printf("pc=%u instr=%02X\n", emu->cpu->pc, emu->mem->program[emu->cpu->pc]);
#endif
		exec_emulator(emu, emu->mem->program[emu->cpu->pc]);
	}
}
