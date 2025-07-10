#include "privateFloppyDriver.h"

uint8_t read_data() {
    while ((inb(MAIN_STATUS_REGISTER) & 0xC0) != 0xC0); // RQM=1, DIO=1 (ready to read)
    return inb(DATA_FIFO);
}