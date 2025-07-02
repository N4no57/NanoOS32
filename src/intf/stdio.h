#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    enum { KEY_ASCII, KEY_SPECIAL, ERROR } type;
    union {
        uint8_t ascii;
        uint8_t code; // e.g., KEY_LEFT, KEY_PGUP
    };
} KeyEvent;

// Legacy functions
int legacy_printf(const char* _Format, ...);
int getchar();
char *fgets(char *buffer, size_t max_len);

// maintained functions
int printf(const char* _Format, ...);
KeyEvent get_key_event();

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif