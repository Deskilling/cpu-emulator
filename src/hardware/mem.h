#ifndef MEM_H
#define MEM_H

#include <stdint.h>

typedef struct {
	// Datenspeicher
	uint8_t* data;
	// Programmspeicher
	uint16_t* program;
} s_mem;

int init_mem(s_mem* mem);
void free_mem(s_mem* mem);

#endif
