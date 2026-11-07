#include "bus.h"

void bus_init(struct Bus *bus) {
    // Zero out Memory on initialization
    memset(bus->ram, 0, sizeof(bus->ram));
    memset(bus->prg_rom, 0, sizeof(bus->prg_rom));
}

void bus_write(struct Bus *bus, uint16_t address, uint8_t value) {
    // 0x0000 - 0x1FFF: 2KB Internal RAM mirrored 4 times
    if (address < 0x2000) {
        bus->ram[address & 0x07FF] = value;
    }
    // TODO: Add PPU (0x2000-0x3FFF), APU/IO (0x4000-0x4017), Mapper (0x4020-0xFFFF)
}

uint8_t bus_read(struct Bus *bus, uint16_t address) {
    // 0x0000 - 0x1FFF: 2KB Internal RAM mirrored 4 times
    if (address < 0x2000) {
        return bus->ram[address & 0x07FF];
    }
    // TODO: Add PPU, APU/IO, Mapper reads

    // No need for <= FFFF, it's a 16 bit value, it cannot exceed 0xFFFF by definition
    if (address >= 0x8000) {
        return bus->prg_rom[address - 0x8000];
    }
    
    return 0x00; // Unmapped default
}