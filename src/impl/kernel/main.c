#include <stdio.h>
#include <terminal.h>
#include <stdlib.h>
#include <ps2.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>
#include <string.h>

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

	printf("Welcome to ShitOS\n");

	while (1) {
        terminal_writestring("> ");

        char line[128];
        
        fgets(line, 128);

        // process command
        if (strcmp(line, "help\n") == 0) {
            terminal_writestring("What do you mean help?\nI don't do anything.\n");
		} else {
            terminal_writestring("Unknown command\n");
        }
    }
}