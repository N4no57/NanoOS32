#include <ps2.h>
#include <terminal.h>
#include <stdio.h>

void keyboard_interrupt_handler() {
    unsigned char input = inb(0x60);
    switch (input) {
        case 0x10: terminal_putchar('q'); break;
        case 0x11: terminal_putchar('w'); break;
        case 0x12: terminal_putchar('e'); break;
        case 0x13: terminal_putchar('r'); break;
        case 0x14: terminal_putchar('t'); break;
        case 0x15: terminal_putchar('y'); break;
        case 0x16: terminal_putchar('u'); break;
        case 0x17: terminal_putchar('i'); break;
        case 0x18: terminal_putchar('o'); break;
        case 0x19: terminal_putchar('p'); break;
        case 0x1C: terminal_putchar('\n'); break;
        case 0x1E: terminal_putchar('a'); break;
    }
    return;
}