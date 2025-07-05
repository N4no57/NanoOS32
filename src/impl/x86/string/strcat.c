#include "../../../intf/stdio.h"

char *strcat(char *destination, const char *source) {
    int dest_str_end = strlen(destination);
    for (int i = dest_str_end; i < strlen(source); i++) {
        destination[dest_str_end++] = source[i];
    }
    destination[dest_str_end] = '\0';
    return destination;
}