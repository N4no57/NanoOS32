#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

#define PM1a_CNT  0x404  // Common port (but ideally should be read from FADT)
#define SLP_TYP   0x2000
#define SLP_EN    0x2000

int cmd_exit(int argc, char **argv) {
    printf("Shutting down...");
    
    outw(PM1a_CNT, SLP_TYP | SLP_EN);

    outw(0x604, 0x2000);

    for (;;) asm volatile ("hlt");
}