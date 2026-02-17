#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "../emulator/emulator.h"

void init_file(s_file* file, uint8_t buffer_size) {
	if (file == NULL) {
		fprintf(stderr, "invalid file pointer\nquitting program");
		exit(-1);
	}

	file->ptr = NULL;
	file->size = buffer_size;
	file->cnt = 0;

	file->buffer = malloc(buffer_size * sizeof(char));
	if (file->buffer == NULL) {
		fprintf(stderr, "failed allocating memory to file buffer\nquitting program");
		exit(-1);
	}

	memset(file->buffer, 0, buffer_size * sizeof(char));
}
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
