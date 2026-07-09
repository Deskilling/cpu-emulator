#include "mem.h"
#include "../settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_mem(s_mem* mem) {
	if (!mem) {
		fprintf(stderr, "invalid mem pointer in init");
		return 0;
	}

	mem->data = malloc(DATA_MEM_SIZE * sizeof(uint8_t));
	if (!mem->data) {
		fprintf(stderr, "failed allocating mem->data");
		return 0;
	}

	mem->program = malloc(PROGRAM_MEM_SIZE * sizeof(uint16_t));
	if (!mem->program) {
		fprintf(stderr, "failed allocating mem->program");
		return 0;
	}

	memset(mem->data, 0, DATA_MEM_SIZE * sizeof(uint8_t));
	memset(mem->program, 0, PROGRAM_MEM_SIZE * sizeof(uint16_t));

	return 1;
}

void free_mem(s_mem* mem) {
	if (!mem) {
		fprintf(stderr, "invalid mem pointer in free");
		return;
	}

	if (mem->data) {
		free(mem->data);
		mem->data = NULL;
	}

	if (mem->program) {
		free(mem->program);
		mem->program = NULL;
	}
}
