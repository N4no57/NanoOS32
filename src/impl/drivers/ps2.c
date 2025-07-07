#include <ps2.h>
#include <terminal.h>
#include <stdio.h>

#define CIRCULAR_BUFFER_SIZE 256

// Circular buffer for raw keyboard scancodes (written by ISR, read by parser)
unsigned char input_buff[CIRCULAR_BUFFER_SIZE];
unsigned char read_ptr = 0; // Read by parser
unsigned char write_ptr = 0; // Written by keyboard ISR

// Circular buffer for parsed ASCII characters (used by higher-level input consumers)
volatile unsigned char parsed_buff[CIRCULAR_BUFFER_SIZE];
volatile unsigned char parsed_read_ptr = 0; // Read by shell or input reader
volatile unsigned char parsed_write_ptr = 0; // Written by parser

// Circular buffer for non-ASCII key events (e.g., arrow keys, Home, End)
volatile unsigned char key_event[CIRCULAR_BUFFER_SIZE];
volatile unsigned char key_event_read_ptr = 0; // Read by input consumer
volatile unsigned char key_event_write_ptr = 0; // Written by parser

// Lookup table for scancode → ASCII mapping (unshifted)
const unsigned char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // 0x0–0xE
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // 0xF–0x1C
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',          // 0x1D–0x29
    0,  '#','z','x','c','v','b','n','m',',','.','/',0,             // 0x2A–0x35
    '*', 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                             // 0x36–0x3F
};

// Lookup table for scancode → ASCII mapping (shifted)
const unsigned char scancode_shift_table[128] = {
    0,  27, '!','"','£','$','%','^','&','*','(',')','_','+', '\b', // 0x0–0xE
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',     // 0xF–0x1C
    0,  'A','S','D','F','G','H','J','K','L',':','@','¬',           // 0x1D–0x29
    0,  '~','Z','X','C','V','B','N','M','<','>','?',0,             // 0x2A–0x35
    0, 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                               // 0x36–0x3F
};

unsigned char input_avail() {
    return read_ptr != write_ptr;
}

// State flags used during keyboard input parsing
unsigned char shift_pressed = 0; // Non-zero if shift pressed
unsigned char extended = 0; // Non-zero if next scancode is extended (startswith 0xE0)

void parse_input_buffer() {
    while (input_avail()) {
        unsigned char scancode = input_buff[read_ptr++];

        // Detect start of extended scancode (0xE0 prefix)
        if (scancode == 0xE0) {
            extended = 1;
            continue;
        }

        // Handle key release (only tracking shift keys for now)
        if (!extended) {
            if (scancode == 0xAA || scancode == 0xB6) { // Left/Right shift
                shift_pressed = 0;
            }
        }

        // Handle key press (e.g. navigation keys)
        if (extended) { // extended scan codes
            switch (scancode) {
                case 0x47: key_event[key_event_write_ptr++] = KEY_HOME; break;
                case 0x4F: key_event[key_event_write_ptr++] = KEY_END;  break;
                case 0x49: key_event[key_event_write_ptr++] = KEY_PGUP; break;
                case 0x51: key_event[key_event_write_ptr++] = KEY_PGDN; break;
                default: break; // unhandled extended key - ignore
            }
            extended = 0;
            continue;
        }

        // Handle non-extended key press
        if (scancode == 0x2A || scancode == 0x36) { // Shift press
            shift_pressed = 1;
            continue;
        }

        if (scancode > 128) continue; // Ignore key release (break codes)

        // Translate scancode to ASCII using shift state
        unsigned char ascii = shift_pressed ? scancode_shift_table[scancode] : scancode_table[scancode];
        if (ascii) {
            parsed_buff[parsed_write_ptr++] = ascii;
        }
    }
}

// Keyboard interrupt handler: reads scancodes from PS/2 port (0x60) into input buffer.
// Handles both single-byte and extended two-byte scancodes.
void keyboard_interrupt_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0xE0) { // Start of extended scancode (two-byte sequence)
        input_buff[write_ptr++] = scancode;
        scancode = inb(0x60); // Read second byte of extended scancode
        input_buff[write_ptr++] = scancode;
        return;
    }

    // Normal (single-byte) scancodes
    input_buff[write_ptr++] = scancode;
}