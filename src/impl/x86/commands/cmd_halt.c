#include "../../../intf/commands.h"

__attribute__((NORETURN))
int cmd_halt(int argc, char **argv) {
    for (;;) __asm__ volatile("hlt");
}