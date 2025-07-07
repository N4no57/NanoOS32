#include <pit.h>
#include <stdio.h>

volatile unsigned int tick = 0;

void pit_interrupt_handler() {
    tick++;
    return;
}

void set_frequency(uint16_t divisor) {
    // Sets PIT frequency by programming channel 0 with the given divisor.
    // Example: divisor = 11932 → ~100 Hz (frequency = 1193182 / divisor)
    outb(COMMAND_REG, 0x36); // set channel 0, read/write LSB then MSB, set square wave mode
    
    outb(CHANNEL_0, divisor & 0xFF); // send LSB of divisor
    outb(CHANNEL_0, (divisor >> 8) & 0xFF); // send MSB of divisor
    return;
}