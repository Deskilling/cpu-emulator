#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "emulator.h"

void init_emulator(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\nquitting program");
		exit(-1);
	}

	emu->cpu.pc = 0;

	emu->cpu.reg = malloc(REGISTER_COUNT * sizeof(uint8_t));
	emu->mem.data = malloc(DATA_MEM_SIZE * sizeof(uint8_t));
	emu->mem.program = malloc(PROGRAM_MEM_SIZE * sizeof(uint16_t));

	if (emu->cpu.reg == NULL || emu->mem.data == NULL || emu->mem.program == NULL) {
		fprintf(stderr, "failed allocating memory to emulator\nquitting program");
		exit(-1);
		return;
	}

	memset(emu->cpu.reg, 0, REGISTER_COUNT * sizeof(uint8_t));
	memset(emu->mem.data, 0, DATA_MEM_SIZE * sizeof(uint8_t));
	memset(emu->mem.program, 0, PROGRAM_MEM_SIZE * sizeof(uint16_t));
}

void free_emulator(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\nquitting program");
		exit(-1);
	}

	free(emu->cpu.reg);
	free(emu->mem.data);
	free(emu->mem.program);

	emu->cpu.reg = NULL;
	emu->mem.data = NULL;
	emu->mem.program = NULL;
}

void exec_emulator(s_emulator* emu, uint16_t instr) {
	if (emu == NULL) {
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
		emu->cpu.reg[rn] = emu->mem.data[rm_addr];
		break;
	case MOV_ADDR_RN:
		emu->mem.data[rm_addr] = emu->cpu.reg[rn];
		break;
	case MOV_RN_READ_RM:
		emu->cpu.reg[rn] = emu->mem.data[emu->cpu.reg[rm_reg % REGISTER_COUNT]];
		break;
	case MOV_READ_RN_RM:
		emu->mem.data[emu->cpu.reg[rn] % 256] = emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case MOV_RN_LITERAL:
		emu->cpu.reg[rn] = rm_addr;
		break;
	case MOV_RN_RM:
		emu->cpu.reg[rn] = emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case ADD:
		emu->cpu.reg[rn] += emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case SUB:
		emu->cpu.reg[rn] -= emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case MUL:
		emu->cpu.reg[rn] *= emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case DIV:
		if (emu->cpu.reg[rm_reg % REGISTER_COUNT] != 0) {
			emu->cpu.reg[rn] /= emu->cpu.reg[rm_reg % REGISTER_COUNT];
		}
		break;
	case AND:
		emu->cpu.reg[rn] &= emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case OR:
		emu->cpu.reg[rn] |= emu->cpu.reg[rm_reg % REGISTER_COUNT];
		break;
	case JZ:
		if (emu->cpu.reg[rn] == 0) {
			emu->cpu.pc += rm_addr;
		}
		break;
	case CMP:
		emu->cpu.reg[rn] = (emu->cpu.reg[rn] == emu->cpu.reg[rm_reg % REGISTER_COUNT]);
		break;
	case LESS:
		emu->cpu.reg[rn] = (emu->cpu.reg[rn] < emu->cpu.reg[rm_reg % REGISTER_COUNT]);
		break;
	case NOP:
		break;
	default:
		fprintf(stderr, "opcode %u not found\nquitting program", opcode);
		exit(-1);
	}

	emu->cpu.pc++;
}

uint8_t load_hex(const char* filename, s_emulator* emu) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		return -1;
	}

	char buffer[1024];
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		fclose(file);
		fprintf(stderr, "failed to open file");
		return -1;
	}

	fclose(file);

	uint8_t len = strlen(buffer);
	uint8_t count = 0;

	for (int i = 0; i + 3 < len; i += 4) {
		char instr_str[5] = {buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3], '\0'};
		emu->mem.program[count++] = (uint16_t)strtol(instr_str, NULL, 16);
	}

	return count;
}

void run_emulator(s_emulator* emu, uint8_t instr_count) {
	while (emu->cpu.pc < instr_count) {
		// printf("pc=%u instr=%04X\n", emu->cpu.pc, emu->mem.program[emu->cpu.pc]);
		exec_emulator(emu, emu->mem.program[emu->cpu.pc]);
		// print_reg(emu);
		// print_mem(emu);
	}
}
