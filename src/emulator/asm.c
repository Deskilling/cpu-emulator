#include "assembler.h"
#include "emulator.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>

void load_asm(const char* filename, s_emulator* emu) {
	if (!emu) {
		fprintf(stderr, "invalid emulator pointer in load_asm");
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

	s_token* tokens = lexer(f.ptr);
	assemble(tokens, "test.hex");
}
