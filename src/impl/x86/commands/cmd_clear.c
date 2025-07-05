#include "../../../intf/commands.h"
#include "../../../intf/terminal.h"
#include "../../../intf/stdio.h"

int cmd_clear(int argc, char **argv) {
    if (argc >= 2) {
        printf("Usage: clear\n\n");
        return -1;
    }

    terminal_clear();
    return 0;
}