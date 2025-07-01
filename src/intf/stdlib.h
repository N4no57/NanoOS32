#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void itoa(int value, char* str);

void heap_init(void);
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t new_size);

#endif