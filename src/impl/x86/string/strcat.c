#include "../../../intf/string.h"

char *strcat(char *destination, const char *source) {
    int dest_len = strlen(destination);
    int source_len = strlen(source);
    for (int i = 0; i < source_len; i++) {
        destination[dest_len + i] = source[i];
    }
    destination[dest_len + source_len] = '\0';
    return destination;
}