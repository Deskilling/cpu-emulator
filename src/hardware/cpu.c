#include "cpu.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_cpu(s_cpu* cpu) {
	if (!cpu) {
		fprintf(stderr, "invalid cpu pointer in init\n");
		return 0;
	}

	cpu->pc = 0;

	cpu->reg = malloc(REGISTER_COUNT * sizeof(*cpu->reg));
	if (!cpu->reg) {
		fprintf(stderr, "failed allocating cpu->reg\n");
		return 0;
	}

	memset(cpu->reg, 0, REGISTER_COUNT * sizeof(*cpu->reg));

	return 1;
}

void free_cpu(s_cpu* cpu) {
	if (!cpu) {
		fprintf(stderr, "invalid cpu pointer in free\n");
		return;
	}

	cpu->pc = 0;

	if (cpu->reg) {
		free(cpu->reg);
		cpu->reg = NULL;
	}
}
