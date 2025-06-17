#include <string.h>

void *memset(void* dest, int c, size_t count) {
    unsigned char *ptr = (unsigned char*) dest;
    unsigned char byte_val = (unsigned char) c;
    for (size_t i = 0; i < count; i++) {
        ptr[i] = byte_val;
    }
    return dest;
}