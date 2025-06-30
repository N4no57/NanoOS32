#include <stdio.h>
#include <terminal.h>
#include <stdlib.h>
#include <ps2.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif


void kernel_main(void) {
	/* Initialise kernel code */
	terminal_initialize();
	heap_init();
	set_frequency(11932);
	idt_init();
	PIC_remap(PIC1, PIC2);

	while (1) {
		if (read_ptr != write_ptr) {
        	char c = input_buff[read_ptr++];
			
			terminal_putchar(c);
		}
	}
}