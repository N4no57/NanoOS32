#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"
#include "../../../intf/string.h"

int cmd_echo(int argc, char **argv) {
    int total_len = 0;
    
    for (int i = 0; argv[i] != NULL; i++) {
        total_len += strlen(argv[i]);
    }
    total_len += (argc - 1) - 1; // for space between the words
    total_len += 1; // for null terminator

    char out_str[total_len];
    out_str[0] = '\0';

    printf("%s\n", out_str);
}