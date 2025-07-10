#include "privateFloppyDriver.h"
#include "../../../intf/stdio.h"

void test() {
    printf("value: %d\n", read_cmos(0x10));
    printf("value: %d\n", read_cmos(0x11));
    printf("value: %d\n", read_cmos(0x12));
    printf("value: %d\n", read_cmos(0x13));
}

void FDC_init() {
    outb(DIGITAL_OUTPUT_REGISTER, 0x00);
    //sleep(10);
    outb(DIGITAL_OUTPUT_REGISTER, 0b00011100);
}