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

	// don't touch this because its kinda funny
	printf("whats up my guy!\n");
    printf("actually I got a better phrase!\n");
	printf("whats up my ni-\n");
	printf("Nice person..... got you lol\n");

	int *a = (int*)malloc(sizeof(int) * 5);

	a[0] = 1;

	if (a == NULL) {
		printf("malloc() failure");
		return;
	}

	__asm__ volatile("int $0x00");

	free(a);
}