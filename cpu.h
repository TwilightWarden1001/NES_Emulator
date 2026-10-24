#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>
#include "bus.h"

// 6502 Status Register Flags
#define FLAG_CARRY             (1 << 0) // C
#define FLAG_ZERO              (1 << 1) // Z
#define FLAG_INTERRUPT_DISABLE (1 << 2) // I
#define FLAG_DECIMAL           (1 << 3) // D (Ignored on NES/2A03)
#define FLAG_BREAK             (1 << 4) // B
#define FLAG_UNUSED            (1 << 5) // 1 Always 1 when pushed
#define FLAG_OVERFLOW          (1 << 6) // V
#define FLAG_NEGATIVE          (1 << 7) // N

// 6502 Registers
typedef struct CPU {
    uint8_t accumulator;
    uint8_t x;
    uint8_t y;
    uint16_t program_counter;
    uint8_t stack_pointer;
    uint8_t status_register;
    struct Bus *bus;
    uint64_t cycle_counter;
} CPU;

// Core CPU Functions
void cpu_init(struct CPU *cpu, struct Bus *bus);
void cpu_reset(struct CPU *cpu);
void cpu_print(struct CPU *cpu);

#endif