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
char *readline(char *buffer, size_t max_len);

static inline uint8_t inb(uint16_t port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint32_t inl(uint16_t port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

#endif