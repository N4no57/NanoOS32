#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

__attribute__((NORETURN))
int cmd_halt(int argc, char **argv) {
    printf("Halting...\n");
    for (;;) __asm__ volatile("hlt");
}