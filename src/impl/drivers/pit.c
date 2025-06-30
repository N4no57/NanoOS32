#include <pit.h>
#include <stdio.h>

volatile unsigned int tick = 0;

void pit_interrupt_handler() {
    tick++;
    return;
}

void set_frequency(uint16_t divisor) { // 11932 for 100 Hz
    outb(COMMAND_REG, 0x36);
    
    outb(CHANNEL_0, divisor & 0xFF);
    outb(CHANNEL_0, (divisor >> 8) & 0xFF);
    return;
}