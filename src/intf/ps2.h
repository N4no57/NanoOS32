#ifndef PS2_H
#define PS2_H

extern volatile unsigned char input_buff[256];
extern volatile unsigned char read_ptr;
extern volatile unsigned char write_ptr;

void keyboard_interrupt_handler();
void parse_input_buffer();

#endif