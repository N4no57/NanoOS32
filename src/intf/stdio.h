#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

#define STD_BUF_SIZE 256

typedef struct {
    char buffer[STD_BUF_SIZE];
    size_t write_ptr;
    size_t read_ptr;

    // Methods
    // TODO

    // Additional fields
    // TODO
} File;

extern volatile File* stdin;

int printf(const char* _Format, ...);
int legacy_printf(const char* _Format, ...);
int getchar(File file);
char *fgets(char *buffer, size_t max_len, File file);

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif