#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    uint8_t pc;
    uint8_t *reg;
} s_cpu;

#endif
