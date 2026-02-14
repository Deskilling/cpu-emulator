#include <stdio.h>
#include <string.h>

#include "emulator/emulator.h"
#include "util/file.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "expected 1 argument (example.hex)");
		return -1;
	}

	s_emulator emu = {0};
	init_emulator(&emu);

	int instr_count = load_hex(argv[1], &emu);
	if (instr_count <= 0) {
		fprintf(stderr, "failed to load program\n");
		return -1;
	}

	// print_program(&emu);

	run_emulator(&emu, instr_count);

	// print_reg(&emu);
	// print_mem(&emu);

	save_reg(&emu);
	save_mem(&emu);

	free_emulator(&emu);
	return 0;
}
