#include "privateFloppyDriver.h"

extern volatile bool irq6;

void wait_for_irq_6() {
    while (!irq6);
    irq6 = false;
}