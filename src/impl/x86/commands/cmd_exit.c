#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

#define SLP_TYP   0x2000
#define SLP_EN    0x2000

int cmd_exit(int argc, char **argv) {
    printf("Shutting down...");

    for (;;) asm volatile ("hlt");
}