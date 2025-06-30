#include <stdio.h>
#include <terminal.h>
#include <stdlib.h>
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

	while (1) {}
}