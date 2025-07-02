#include <ps2.h>
#include <terminal.h>
#include <stdio.h>

// raw scancode buffer
volatile unsigned char input_buff[256];
volatile unsigned char read_ptr = 0;
volatile unsigned char write_ptr = 0;

// parsed input buffer
volatile unsigned char parsed_buff[256];
volatile unsigned char parsed_read_ptr = 0;
volatile unsigned char parsed_write_ptr = 0;

// lookup tables for parser
const unsigned char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // 0x0–0xE
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // 0xF–0x1C
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',          // 0x1D–0x29
    0,  '#','z','x','c','v','b','n','m',',','.','/',0,             // 0x2A–0x35
    '*', 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                             // 0x36–0x3F
};

const unsigned char scancode_shift_table[128] = {
    0,  27, '!','"','£','$','%','^','&','*','(',')','_','+', '\b', // 0x0–0xE
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',     // 0xF–0x1C
    0,  'A','S','D','F','G','H','J','K','L',':','@','¬',           // 0x1D–0x29
    0,  '~','Z','X','C','V','B','N','M','<','>','?',0,             // 0x2A–0x35
    0, 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                               // 0x36–0x3F
};

unsigned char shift_pressed = 0;

void parse_input_buffer() {
    
}

// input raw scancode to an input buffer
void keyboard_interrupt_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0xE0) { // extended scancodes
        input_buff[write_ptr] = 0xE0;
        write_ptr++;
        scancode = inb(0x60);
        input_buff[write_ptr] = scancode;
        write_ptr++;
    }

    // normal scancodes
    input_buff[write_ptr] = scancode;
    write_ptr++;
}