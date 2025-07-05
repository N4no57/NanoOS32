#include "../../../intf/commands.h"

__attribute__((NORETURN))
void cmd_halt() {
    for (;;) __asm__ volatile("hlt");
}