#include "privateFloppyDriver.h"

void send_command(uint8_t command) {
    // Wait until the FDC is ready to receive data
    while ((inb(MAIN_STATUS_REGISTER) & 0xC0) != 0x80); // MSR: RQM=1, DIO=0
    outb(DATA_FIFO, command);                           // Send command byte
}