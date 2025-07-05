#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

int cmd_help(int argc, char **argv) {
    if (argc >= 2) {
        printf("help: no help topics match '%s'. Try 'help help'\n", argv[1]);
        return 0;
    }

    printf("Built-in functions:\n\n");
    printf("clear\n");
    printf("echo [arg ...]\n");
    printf("exit\n");
    printf("halt\n");
    printf("help [-dms] [pattern ...]\n");
    printf("reboot\n");
    return 0;
}