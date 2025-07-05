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

int tok_idx = 0;
char* get_tok(char *buff, char terminator) { // returns the next token in an item
    if (!buff) {
        return NULL;
    }
    if (tok_idx >= strlen(buff)-1) {
        return NULL;
    }

    size_t buff_size = 32;
    uint32_t buff_idx = 0;
    char *return_buff = malloc(buff_size*sizeof(char));
    do {
        if (buff[tok_idx] == '\n') {
            return_buff[buff_idx] = '\0';
            return return_buff;
        }

        if (buff[tok_idx] == ' ') {
            continue;
        }

        if (tok_idx+1 > buff_size) {
            return_buff = realloc(return_buff, buff_size*=2);
        }

        return_buff[buff_idx++] = buff[tok_idx];
    } while(buff[++tok_idx] != terminator);

    return_buff[buff_idx] = '\0';
    return return_buff;
}

void parse_command(char command[], int *out_argc, char ***out_argv) {
    int32_t argc = 0;
    size_t argv_capacity = ARGV_BASE_CAPACITY;
    char **argv = malloc((argv_capacity) * sizeof(char *));

    char *token = get_tok(command, ' ');
    while (token != NULL) {
        if (argc >= argv_capacity - 1) {
            argv = realloc(argv, (argv_capacity*=2) * sizeof(char *));
        }

        argv[argc++] = token;
        token = get_tok(command, ' ');
    }
    argv[argc] = NULL;

    *out_argc = argc;
    *out_argv = argv;
}

void execute_command(int argc, char **argv) {
    
}

void kernel_main(void) {
    kernel_init();

	printf("Welcome to ShitOS\n");

	while (1) {
        tok_idx = 0;
        int argc;
        char **argv;

        printf("> ");

        char line[128];
        
        readline(line, 128);

        parse_command(line, &argc, &argv);

        execute_command(argc, argv);

        // free argv (all the tokens)
        for (int i = 0; i < argc; i++) {
            free(argv[i]);
        }
        free(argv);
    }
}