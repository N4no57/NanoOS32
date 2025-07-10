#include "privateTime.h"

void sleep(uint32_t milliseconds) {
    uint32_t target = tick + (milliseconds / 10);
    while (tick < target) {
        __asm__ volatile ("hlt"); // low power mode until next interrupt
    }
}