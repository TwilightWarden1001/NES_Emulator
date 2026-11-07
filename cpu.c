#include "cpu.h"

void cpu_init(struct CPU *cpu, struct Bus *bus) {
    cpu->bus = bus;
    cpu_reset(cpu);
}

void cpu_reset(struct CPU *cpu) {
    cpu->accumulator = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->stack_pointer = 0xFD;   // Default 6502 stack offset
    cpu->status_register = 0x24; // Unused flag (0x20) + Interrupt Disable (0x04)

    uint8_t low_byte = bus_read(cpu->bus, 0xFFFC);
    uint8_t high_byte = bus_read(cpu->bus, 0xFFFD);
    uint16_t reset_vector = (high_byte << 8) | low_byte;

    printf("Low Byte: %d  High Byte: %d, Reset Vector: %d\n", low_byte, high_byte, reset_vector);

    cpu->program_counter = reset_vector;
    cpu->cycle_counter = 0;
}

void cpu_print(struct CPU *cpu) {
    // 1. Print the registers in standard Hexadecimal format
    printf("PC: $%04X  A: $%02X  X: $%02X  Y: $%02X  SP: $%02X  P: $%02X [",
           cpu->program_counter, 
           cpu->accumulator, 
           cpu->x, 
           cpu->y, 
           cpu->stack_pointer, 
           cpu->status_register);
    
    // 2. Decode the Status Register flags individually
    printf("%c", (cpu->status_register & FLAG_NEGATIVE)          ? 'N' : '-');
    printf("%c", (cpu->status_register & FLAG_OVERFLOW)          ? 'V' : '-');
    printf("%c", (cpu->status_register & FLAG_UNUSED)            ? 'U' : '-');
    printf("%c", (cpu->status_register & FLAG_BREAK)             ? 'B' : '-');
    printf("%c", (cpu->status_register & FLAG_DECIMAL)           ? 'D' : '-');
    printf("%c", (cpu->status_register & FLAG_INTERRUPT_DISABLE) ? 'I' : '-');
    printf("%c", (cpu->status_register & FLAG_ZERO)              ? 'Z' : '-');
    printf("%c", (cpu->status_register & FLAG_CARRY)             ? 'C' : '-');
    
    // 3. Print the absolute cycle count
    printf("] CYC: %llu\n", (unsigned long long)cpu->cycle_counter);
}

// LDA - Complete
void LDA(struct CPU *cpu, uint8_t value) {
    // Load the value into the accumulator
    cpu->accumulator = value;

    // ZERO Check
    if (cpu->accumulator == 0x00) {
        cpu->status_register |= FLAG_ZERO;
    } else {
        cpu->status_register &= ~FLAG_ZERO;
    }

    // NEGATIVE Check
    if ((cpu->accumulator & 0x80) != 0) {
        cpu->status_register |= FLAG_NEGATIVE;
    } else {
        cpu->status_register &= ~FLAG_NEGATIVE;
    }
}

uint16_t addressing_zero_page(struct CPU *cpu) {
    uint16_t operand = bus_read(cpu->bus, cpu->program_counter);
    cpu->program_counter++;
    return operand;
}

void cpu_step(struct CPU *cpu) {
    // The FIRST read is just to get a special "opcode" so we know what function to call
    uint8_t opcode = bus_read(cpu->bus, cpu->program_counter);
    cpu->program_counter++;

    switch (opcode) {
            case 0xA5:
            {
                // THIS is to get the ADDRESS of where the value is stored. This is NOT where the value is
                uint16_t address = addressing_zero_page(cpu);
                LDA(cpu, bus_read(cpu->bus, address));  // This reads whatever is stored at the ADDRESS and passes it to LDA
                break;
            }
            default:
                printf("Unimplemented Opcode");

    }
}