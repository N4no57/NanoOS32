#include "privateFloppyDriver.h"
#include "../../../intf/stdio.h"

bool FDC_init() {
    outb(DIGITAL_OUTPUT_REGISTER, 0x00);
    // sleep(10); // small delay
    outb(DIGITAL_OUTPUT_REGISTER, 0x1C);
    // sleep(500) // wait for drive to spool up
    
    wait_for_irq_6();
    send_command(0x08); // tell FDC to send interrupt status
    uint8_t st0 = read_data(); // status byte 0
    uint8_t cy1 = read_data(); // current cylinder (after seek/recall)

    wait_for_irq_6();
    send_command(0x08); // SENSE INTERRUPT
    st0 = read_data();
    cy1 = read_data();

    outb(CONFIGURATION_CONTROL_REGISTER, 0x00); // 500 kbps

    int retries = 5;
    do {
        send_command(0x03); // SPECIFY
        send_command(0xDF); // SRT (Step Rate Time) = 3 ms, HUT (Head Unload Time) = 240 ms
        send_command(0x02); // HLT (Head Load Time) = 16 ms, ND = 0 (use DMA)

        send_command(0x07); // Recalibrate
        send_command(0x00); // Drive 0

        wait_for_irq_6();
        send_command(0x08); // SENSE INTERRUPT
        st0 = read_data();
        cy1 = read_data();
    } while (cy1 != 0 && retries-- > 0);

    return (cy1 == 0);
}