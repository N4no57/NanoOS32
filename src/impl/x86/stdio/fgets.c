#include <stdio.h>
#include <terminal.h>

char *fgets(char *buffer, size_t max_len) {
    size_t len = 0;

    while (len < max_len) {
        char c = getchar();

        if (c == '\n') {
            terminal_putchar(c);
            buffer[len++] = c;
            break;
        } else if (c == '\b') {
            if (len > 0) {
                len--;
                terminal_putchar(c);
            }
        } else if (c == '\t') {
            if (len + TAB_SIZE < max_len) {
                for (int i = 0; i < TAB_SIZE; i++) {
                    buffer[len++] = ' ';
                }
                terminal_putchar('\t');
            }
        } else if (c >= 32 && c < 127) { // printable chars
            buffer[len++] = c;
            terminal_putchar(c);
        }
        terminal_update_cursor();
    }
    buffer[len] = '\0';
    return buffer;
}