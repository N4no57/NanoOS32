#include "privateFloppyDriver.h"

bool irq6 = false;

void floppy_interrupt_handler() {
    irq6 = true;
    return;
}