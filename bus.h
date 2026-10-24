#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <string.h>

// Minimal Bus representation with 2KB internal CPU RAM
typedef struct Bus {
    uint8_t ram[2048];
    uint8_t prg_rom[32768];
} Bus;

// Function prototypes for Bus R/W
void bus_init(struct Bus *bus);
void bus_write(struct Bus *bus, uint16_t address, uint8_t value);
uint8_t bus_read(struct Bus *bus, uint16_t address);

#endif // BUS_H