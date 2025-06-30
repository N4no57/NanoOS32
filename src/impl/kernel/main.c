#include <stdio.h>
#include <terminal.h>
#include <stdlib.h>
#include <idt.h>
#include <pic.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif


void kernel_main(void) {
	/* Initialize kernel code */
	terminal_initialize();
	heap_init();
	idt_init();
	PIC_remap(PIC1, PIC2);

	while (1) {}
}