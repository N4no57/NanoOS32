#include "privateFloppyDriver.h"
#include "../../../intf/stdio.h"

void FDC_init() {
    outb(DIGITAL_OUTPUT_REGISTER, 0x00);
    //sleep(10);
    outb(DIGITAL_OUTPUT_REGISTER, 0b00011100);
}