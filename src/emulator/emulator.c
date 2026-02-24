#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator.h"
#include "../util/file.h"
#include "../log/log.h"

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

void load_hex(const char* filename, s_emulator* emu) {
	s_file f;
	init_file(&f, (uint8_t)FILE_BUFFER_SIZE);

	f.ptr = fopen(filename, "r");
	if (f.ptr == NULL) {
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	char c;
	f.size = 0;
	while ((c = fgetc(f.ptr)) != EOF) {
		if (c != ';') {
			while ((c = fgetc(f.ptr)) != '\n' && c != EOF);
		} else if (c != ' ' && c != '\n') {
			f.size++;
		}
	}
	rewind(f.ptr);

	unsigned int maxChars = PROGRAM_MEM_SIZE * 4;
	if (f.size > maxChars) {
		fprintf(stderr, "Hex file is too large (max %u hex chars)\n", maxChars);
		fclose(f.ptr);
		return;
	}

	f.bufferIdx = 0;
	while ((c = fgetc(f.ptr)) != EOF) {
		if (c == ';') {
			while ((c = fgetc(f.ptr)) != '\n' && c != EOF);
		} else if (c != ' ' && c != '\n') {
			f.buffer[f.bufferIdx++] = c;
		}
	}
	f.buffer[f.bufferIdx] = '\0';
	fclose(f.ptr);

	char hexStr[5] = {0};
	uint8_t hexStrIdx = 0;

	for (uint8_t i = 0; i < f.bufferIdx; i++) {
		if (hexStrIdx < 4) {
			hexStr[hexStrIdx++] = f.buffer[i];
		}
		if (hexStrIdx == 4) {
			emu->mem.program[emu->instrCnt++] = (uint16_t)strtol(hexStr, NULL, 16);
			hexStrIdx = 0;
		}
	}

	if (f.bufferIdx > f.size) {
		return;
	}

	f.buffer = NULL;

	if (hexStrIdx != 0) {
		fprintf(stderr, "Incomplete instruction at end of file\n");
		return;
	}
}

void run_emulator(s_emulator* emu) {
	while (emu->cpu.pc < emu->instrCnt) {
		exec_emulator(emu, emu->mem.program[emu->cpu.pc]);
	}
}
