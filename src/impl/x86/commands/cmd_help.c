#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

int cmd_help(int argc, char **argv) {
    if (argc >= 2) {
        printf("idk what your mentioning here\n");
        return 0;
    }

    printf("I don't do anything\nHow am I supposed to help?\n");
    return 0;
}