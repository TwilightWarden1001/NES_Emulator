#include "cpu.h"
#include "bus.h"
#include <assert.h>

// Function prototypes
int run_tests();
int test_LDA(struct CPU *cpu); // LDA Zero Page

int main(void) {
    return run_tests();
}

int run_tests() {
    Bus bus;
    bus_init(&bus);
    CPU cpu;
    cpu_init(&cpu, &bus);
    int code = -1; // If it's -1 at the end, we failed. If it's 0, we passed.
    code = test_LDA(&cpu);
    return code;
}

// COMMENT FOR OTHER TESTS. Remember the flags are stored in an 8 BIT VARIABLE
// IF YOU GET CONFUSED WHY IT'S NOT THE NUMBER YOU THINK, CHECK THE POSITION.
// IF IT'S ON IT WILL BE 2^POSITION. REMEMBER YOUR BINARY TRAINING. 1,2,4,8,16,32,64,128
int test_LDA(struct CPU *cpu) {
    // Normal Case. Neither Flag Set. A = 66
    printf("Testing LDA Zero Page, Should be A=66, Zero Flag Off, Negative Flag Off");
    bus_write(cpu->bus, 0x0200, 0xA5);
    bus_write(cpu->bus, 0x0201, 0x10);
    bus_write(cpu->bus, 0x0010, 0x42);
    cpu->program_counter = 0x0200;
    cpu_step(cpu);
    assert((cpu->accumulator == 0x42));
    assert((cpu->status_register & FLAG_ZERO) == 0);
    assert((cpu->status_register & FLAG_NEGATIVE) == 0);
    printf("Test #1 Passed");

    // Zero Flag Check. A = 0
    printf("Testing LDA Zero Page, Should be A=0, Zero Flag On, Negative Flag Off");
    bus_write(cpu->bus, 0x0201, 0x10);
    bus_write(cpu->bus, 0x0010, 0x00);
    cpu->program_counter = 0x0200;
    cpu_step(cpu);
    assert(cpu->accumulator == 0x00);
    assert((cpu->status_register & FLAG_ZERO) == 2);     // Remember 2^1 = 2
    assert((cpu->status_register & FLAG_NEGATIVE) == 0);
    printf("Test #2 Passed");

    // Negative Flag Check. A = -1
    printf("Testing LDA Zero Page, Should be A=-1, Zero Flag Off, Negative Flag On");
    bus_write(cpu->bus, 0x0200, 0xA5);
    bus_write(cpu->bus, 0x0201, 0x10);
    bus_write(cpu->bus, 0x0010, 0xFF); // Should be -1
    cpu->program_counter = 0x0200;
    cpu_step(cpu);
    assert((cpu->accumulator == 0xFF));
    assert((cpu->status_register & FLAG_ZERO) == 0);
    assert((cpu->status_register & FLAG_NEGATIVE) == 128); // Remember 2^7 = 128
    printf("Test #3 Passed");
    return 0;
}