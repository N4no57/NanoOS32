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

void *memset(void* dest, int c, size_t count) {
    unsigned char *ptr = (unsigned char*) dest;
    unsigned char byte_val = (unsigned char) c;
    for (size_t i = 0; i < count; i++) {
        ptr[i] = byte_val;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t count) {
    char* d = dest;
    const char* s = src;
    for (int i = 0; i < count; i++) {
        d[i] = s[i];
    }
    return dest;
}