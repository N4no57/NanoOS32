#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

size_t strlen(const char* str);
bool strcmp(const char* a, const char* b);
void strcpy(char* dest, const char* src);

void memset(void* dest, int c, size_t n);
void memcpy(void* dest, const void* src, size_t count);

#endif //STRING_H
