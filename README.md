# NES Emulator

A Nintendo Entertainment System (NES) emulator written from scratch in C.

The goal of the project is to build a **semi cycle-perfect NES emulator**, with a prioritization on understanding the original hardware and implementing it with that goal in mind. The loader, however, will be looser since we cannot accurately implement it. The loader will focus on validation and

## Technology

- Language: C
- Graphics/Windowing: SDL2
- Platform: Windows (Development Environment)

## Project Status

**Early Development - not yet playable**

The current development is focused on establishing the emulator's foundations:

- iNES ROM loading and header validation.
- PRG ROM loading and mirroring
- CPU and system bus foundations
- SDL2 development environment setup

## Planned Components

The emulator will eventually implement:

- **CPU:** NES 6502-derived processor and instruction execution
- **PPU:** Picture Processing Unit for background and sprite rendering
- **APU:** Audio Processing Unit
- **Bus:** Communication between the CPU, memory, and hardware components.
- **Cartridge Support:** ROM loading, memory mapping, and mapping support.
- **Timing:** CPU/PPU synchronization and cycle-accurate hardware behavior.

## License

License information will be added later.
