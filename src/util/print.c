#include <stdio.h>

#include "print.h"

void print_program(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\n");
		return;
	}

	for (int i = 0; i < PROGRAM_MEM_SIZE; i++) {
		printf("%04X", emu->mem.program[i]);

		if ((i + 1) % 16 == 0) {
			printf("\n");
		} else {
			printf(" ");
		}
	}

	printf("\n");
}

void print_reg(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\n");
		return;
	}

	for (int i = 0; i < REGISTER_COUNT; i++) {
		printf("%02X", emu->cpu.reg[i]);
		if (i < REGISTER_COUNT - 1) {
			printf(" ");
		}
	}
	printf("\n\n");
}

void print_mem(s_emulator* emu) {
	for (int i = 0; i < DATA_MEM_SIZE; i++) {
		printf("%02X", emu->mem.data[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
		else
			printf(" ");
	}
}
