#include <ps2.h>
#include <terminal.h>
#include <stdio.h>

volatile unsigned char input_buff[256];
volatile unsigned char read_ptr = 0;
volatile unsigned char write_ptr = 0;

const unsigned char scancode_table[256] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', /* 0x0–0xE */
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     /* 0xF–0x1C */
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',         /* 0x1D–0x29 */
    0,  '\\','z','x','c','v','b','n','m',',','.','/',0,           /* 0x2A–0x35 */
    '*', 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                             /* 0x36–0x3F */
};

void keyboard_interrupt_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode > 127) return;

    input_buff[write_ptr] = scancode_table[scancode];
    write_ptr++;
}