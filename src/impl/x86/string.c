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

char *strcpy(char* dest, const char* src) {
    int i = 0;
    do {
        dest[i] = src[i];
    } while (src[i++] != '\0');
    return dest;
}

void *memset(void* dest, int c, size_t n) {
    unsigned char *ptr = (unsigned char) dest;
    unsigned char byte_val = (unsigned char) c;
    for (size_t i = 0; i < n; i++) {
        ptr[i] = byte_val;
    }
    return dest;
}