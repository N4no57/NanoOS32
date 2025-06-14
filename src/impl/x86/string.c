#include "string.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int strcmp(const char* a, const char* b) {
    if (strlen(a) != strlen(b)) {
        return 0;
    }
}
