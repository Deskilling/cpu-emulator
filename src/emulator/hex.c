#include "emulator.h"
#include "settings.h"
#include "file.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void load_hex(const char* filename, s_emulator* emu) {
	if (!emu) {
		fprintf(stderr, "invalid emulator pointer in load_hex");
		exit(-1);
	}

	s_file f;
	init_file(&f, FILE_BUFFER_SIZE);

	f.ptr = fopen(filename, "r");
	if (!f.ptr) {
		fprintf(stderr, "Failed to open file\n");
		if (f.buffer) {
			free(f.buffer);
		}

		return;
	}

	char c;
	while ((c = fgetc(f.ptr)) != EOF) {
		f.size++;
		if (c == ';') {
			while ((c = fgetc(f.ptr)) != '\n' && c != EOF);
		} else if (c != ' ' && c != '\n') {
			f.valid_size++;
		}
	}
	rewind(f.ptr);

	printf("program size: %u\n", f.valid_size);
	printf("full size: %u\n", f.size);

	unsigned int maxChars = PROGRAM_MEM_SIZE * 8;
	if (f.valid_size > maxChars) {
		fprintf(stderr, "Hex file is too large (max %u hex chars)\n", maxChars);
		fclose(f.ptr);

		if (f.buffer) {
			free(f.buffer);
		}

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

	char hexStr[9] = {0};
	uint16_t hexStrIdx = 0;

	for (uint16_t i = 0; i < f.bufferIdx; i++) {
		if (hexStrIdx < 8) {
			hexStr[hexStrIdx++] = f.buffer[i];
		}
		if (hexStrIdx == 8) {
			emu->mem->program[emu->instrCnt++] = (uint32_t)strtoul(hexStr, NULL, 16) >> 8;
			hexStrIdx = 0;
		}
	}

	if (f.buffer) {
		free(f.buffer);
	}
	f.buffer = NULL;

	if (hexStrIdx != 0) {
		fprintf(stderr, "Incomplete instruction at end of file\n");
		return;
	}
}
