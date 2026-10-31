#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "cpu.h"
#include "bus.h"

// Function prototypes
int load_rom(struct Bus *bus);

// iNES Header
typedef struct iNES_Header{
    uint8_t magic_numbers[4]; // Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
    uint8_t prg_rom_16Kb;     // Size of PRG ROM in 16 KB units
    uint8_t chr_rom_8Kb;      // Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
    uint8_t flags6;           // Flags 6 – Mapper, mirroring, battery, trainer
    uint8_t flags7;           // Flags 7 – Mapper, VS/Playchoice, NES 2.0
    uint8_t flags8;           // Flags 8 – PRG-RAM size (rarely used extension)
    uint8_t flags9;           // Flags 9 – TV system (rarely used extension)
    uint8_t flags10;          // Flags 10 – TV system, PRG-RAM presence (unofficial, rarely used extension)
    uint8_t padding[5];       // Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
} iNES_Header;

int main(void) {
    // 1. First we zero out and initialize the BUS. This holds all our memory
    Bus bus;
    bus_init(&bus);

    // 2. Second, we read our ROM and initialize the bus
    load_rom(&bus);

    // 3. Finally, now that we have our PRG
    CPU cpu;
    cpu_init(&cpu, &bus);

    cpu_print(&cpu);

    return 0;
}

int load_rom(Bus *bus) {
    const char* filename = "nestest.nes";
    FILE *nes_file = fopen(filename, "rb");

    // Checks that fopen() actually returned a valid stream handle
    if (nes_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    printf("The file is not null\n");

    struct stat file_info; // Built in structure for file information
    int result = stat(filename, &file_info);
    if (result == -1) {
        perror("stat");
        fclose(nes_file);
        exit(EXIT_FAILURE);
    }
    printf("The files stats have been verified\n");

    // Allocate dynamic uint8_t buffer for raw binary data
    uint8_t *file_contents = malloc(file_info.st_size);
    if (file_contents == NULL) {
        perror("Malloc Failed");
        fclose(nes_file);
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(file_contents, 1, file_info.st_size, nes_file);
    if (bytes_read != file_info.st_size) { // Technically not the same data type, C *should* handle it but
        free(file_contents);
        fclose(nes_file);
        exit(EXIT_FAILURE);
    }
    printf("The bytes read is equal to the file contents!\n");

    iNES_Header header;
    // File Header Size
    if (file_info.st_size >= sizeof(iNES_Header)) {
        memcpy(&header, file_contents, sizeof(struct iNES_Header));
        printf("First 16 Bytes of file have been copied\n");
    } else {
        free(file_contents);
        fclose(nes_file);
        printf("The file is not large enough to fill the NES header. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    // 16 Bytes WILL at least have been copied though it could still be an invalid file so for now we do at least one genuine check.
    // File Check
    if (header.magic_numbers[0] == 'N' && 
        header.magic_numbers[1] == 'E' && 
        header.magic_numbers[2] == 'S' && 
        header.magic_numbers[3] == 0x1A) {
        printf("The file is a valid NES File!\n");
    } else {
        free(file_contents);
        fclose(nes_file);
        printf("This file is NOT a valid NES file!\n");
        exit(EXIT_FAILURE);
    }

    // First we skip the 16 byte header
    uint8_t *prg_rom_start = file_contents + 16;

    // Second we check if bit 2 of flags6 is 4. If it's 4, we skip 512
    // Remember the AND just makes every bit BUT bit 2, 0.
    int bit2 = header.flags6 & 0x04;

    size_t required_size = 0;

    // Now we finally read 32k bytes (if it's 1 we just do 16 twice)
    if (header.prg_rom_16Kb == 0) {
        printf("No program ROM? Well that certainly makes my job easier");
        free(file_contents);
        fclose(nes_file);
        exit(EXIT_FAILURE);
    } else if (header.prg_rom_16Kb >= 3) {
        printf("Mmm it'd be nice if we could handle more ROM. Turns out we cant't. Maybe shrink the program lol. Or maybe this is where mappers come in");
        free(file_contents);
        fclose(nes_file);
        exit(EXIT_FAILURE);
    } else {
        // We KNOW it's either 1 or 2.
        if (bit2 == 4) {
            required_size += 512; // Add the trainer shit
        }
        required_size += (16 + header.prg_rom_16Kb * 16384);

        // Note that this doesn't account for CHR ROM.
        // We're basically saying if it's not at LEAST this big, it can't work.
        if (file_info.st_size < required_size) {
            printf("Your file does not mean the minimum size requirements lmao.\n");
            free(file_contents);
            fclose(nes_file);
            exit(EXIT_FAILURE);
        } else {
            printf("File size successfully verified!\n");
        }
    }

    // Remember it's left to right NOT right to left
    // So it's (0000 0100)
    if (bit2 == 4) {
        printf("We are skipping 512 bytes!\n");
        prg_rom_start += 512;
    } else {
        printf("We are NOT skipping 512 bytes!\n");
    }
    
    if (header.prg_rom_16Kb == 2) {
        printf("We are copying 32K into PRG ROM\n");
        memcpy(bus->prg_rom, prg_rom_start, 32768);
    } else {
        printf("We are copying 16K twice into PRG ROM\n");
        memcpy(bus->prg_rom, prg_rom_start, 16384);
        memcpy(bus->prg_rom + 16384, prg_rom_start, 16384);
    }

    // Clean up our resources!
    free(file_contents);
    fclose(nes_file);

    printf("File read successfully!\n");
    return 0;
}