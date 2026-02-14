#include <stdio.h>

#include "file.h"
#include "../emulator/emulator.h"

void save_reg(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\n");
		return;
	}

	FILE* file = fopen(".reg_dump.reg", "w");
	if (file == NULL) {
		fprintf(stderr, "failed to open file");
		return;
	}

	for (int i = 0; i < REGISTER_COUNT; i++) {
		fprintf(file, "%02X", emu->cpu.reg[i]);
		if (i < REGISTER_COUNT - 1) {
			fprintf(file, " ");
		}
	}

	fprintf(file, "\n");
	fclose(file);
}

void save_mem(s_emulator* emu) {
	if (emu == NULL) {
		fprintf(stderr, "invalid emulator pointer\n");
		return;
	}

	FILE* file = fopen(".mem_dump.mem", "w");
	if (file == NULL) {
		fprintf(stderr, "failed to open file");
		return;
	}

	for (int i = 0; i < DATA_MEM_SIZE; i++) {
		fprintf(file, "%02X", emu->mem.data[i]);

		if ((i + 1) % 16 == 0)
			fprintf(file, "\n");
		else
			fprintf(file, " ");
	}

	fclose(file);
}
