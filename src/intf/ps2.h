#ifndef PS2_H
#define PS2_H

#define KEY_UP     0x80
#define KEY_DOWN   0x81
#define KEY_LEFT   0x82
#define KEY_RIGHT  0x83
#define KEY_PGUP   0x84
#define KEY_PGDN   0x85
#define KEY_HOME   0x86
#define KEY_END    0x87

extern volatile unsigned char parsed_buff[256];
extern volatile unsigned char parsed_read_ptr;
extern volatile unsigned char parsed_write_ptr;

extern volatile unsigned char key_event[256];
extern volatile unsigned char key_event_read_ptr;
extern volatile unsigned char key_event_write_ptr;

void keyboard_interrupt_handler();
void parse_input_buffer();

#endif