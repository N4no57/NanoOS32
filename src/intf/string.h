#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* a, const char* b);
char* strcpy(char* dest, const char* src);

void* memset(void* dest, int c, size_t count);
void* memcpy(void* dest, const void* src, size_t count);

#endif //STRING_H
