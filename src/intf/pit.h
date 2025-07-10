#ifndef PIT_H
#define PIT_H

#define CHANNEL_0 0x40
#define CHANNEL_1 0x41
#define CHANNEL_2 0x42
#define COMMAND_REG 0x43

#include <stdint.h>

extern volatile uint32_t tick;

void set_frequency(uint16_t divisor);
void pit_interrupt_handler();

#endif