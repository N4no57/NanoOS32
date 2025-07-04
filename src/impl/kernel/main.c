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

#define ARGV_BASE_CAPACITY 4

void kernel_init(void) {
    terminal_initialize();
	heap_init();
	set_frequency(11932);
	PIC_remap(PIC1, PIC2);
	idt_init();
}

void kernel_main(void) {
    kernel_init();

	printf("Welcome to ShitOS\n");

	while (1) {
        printf("> ");

        char line[128];
        
        readline(line, 128);

        // process command
        if (strcmp(line, "help\n") == 0) {
            printf("What do you mean help?\nI don't do anything.\n");
		} else if (strcmp(line, "clear\n") == 0) { 
            terminal_clear();
        } else {
            printf("Unknown command\n");
        }
    }
}