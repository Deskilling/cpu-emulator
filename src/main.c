#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator.h"
#include "file.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "expected 1 argument (example.hex)\n");
		return -1;
	}

	s_emulator emu = {0};
	init_emulator(&emu);

	char* filename = argv[1];
	char* ext = strrchr(filename, '.');
	printf("extension: %s\n", ext);

	if (ext != NULL) {
		if (strcmp(ext, ".hex") == 0) {
			load_hex(argv[1], &emu);
		} else if (strcmp(ext, ".asm") == 0) {
			load_asm(argv[1], &emu);
		} else {
			printf("unknown\n");
			exit(-1);
		}
	} else {
		printf("no extension\n");
		exit(-1);
	}

	if (emu.instrCnt <= 0) {
		fprintf(stderr, "failed to load program\n");
		free_emulator(&emu);
		return -1;
	}

	// print_program(&emu);
	run_emulator(&emu);

	save_reg(&emu);
	save_mem(&emu);
	// print_reg(&emu);
	// print_mem(&emu);
	free_emulator(&emu);
	return 0;
}
