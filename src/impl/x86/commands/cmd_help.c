#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

int cmd_help(int argc, char **argv) {
    if (argc >= 2) {
        printf("idk what your mentioning here\n");
        return 0;
    }

    printf("Built-in functions:\n\n");
    printf("clear\n");
    printf("echo [arg ...]\n");
    printf("halt\n");
    printf("exit\n");
    printf("reboot\n");
    return 0;
}