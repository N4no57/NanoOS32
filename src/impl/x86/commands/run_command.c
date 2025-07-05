#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"

int run_command(int argc, char **argv) {
    char name[] = argv[0];
    for (int i = 0; i < command_count; i++) {
        if (strcmp(name, commands[i].name) == 0) {
            return commands[i].func(argc, argv);
        }
    }

    return -1;
}