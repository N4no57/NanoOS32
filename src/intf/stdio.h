#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

// Legacy functions
int legacy_printf(const char* _Format, ...);
int getchar();
char *fgets(char *buffer, size_t max_len);

// maintained functions
int printf(const char* _Format, ...);

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif