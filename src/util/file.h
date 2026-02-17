#ifndef FILE_H
#define FILE_H

#include <stdio.h>

#include "../emulator/emulator.h"

#define FILE_BUFFER_SIZE 1024

typedef struct {
    FILE* ptr;
    uint8_t size;
    uint8_t cnt;
    char*  buffer;
    uint8_t bufferIdx;
} s_file;

void save_reg(s_emulator* emu);
void save_mem(s_emulator* emu);
void init_file(s_file* file, uint8_t buffer_size);

#endif
