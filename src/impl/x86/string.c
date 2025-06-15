#include <string.h>
#include <stdio.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

void strcpy(char* dest, const char* src) {
    for (int i = 0; i < strlen(src); i++) {
        dest[i] = src[i];
    }
}