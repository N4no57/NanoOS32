#include <stdio.h>
#include <terminal.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif


void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	printf("Hello, kernel World!\n");
    printf("Are you ready to rumble!!\n");
}