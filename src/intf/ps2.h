#ifndef PS2_H
#define PS2_H

extern volatile unsigned char parsed_buff[256];
extern volatile unsigned char parsed_read_ptr;
extern volatile unsigned char parsed_write_ptr;

extern volatile unsigned char key_event[256];
extern volatile unsigned char key_event_read_ptr;
extern volatile unsigned char key_event_write_ptr;

void keyboard_interrupt_handler();
void parse_input_buffer();

#endif