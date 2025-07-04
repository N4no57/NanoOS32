#include "../../../intf/stdio.h"
#include "../../../intf/terminal.h"
#include "../../../intf/ps2.h"

char *readline(char *buffer, size_t max_len) {
    size_t len = 0;
    KeyEvent ev;

    while (1) {
        do {
            parse_input_buffer();
            ev = get_key_event();
        } while (ev.type == ERROR);

        if (ev.type == KEY_ASCII) {
            char c = ev.ascii;
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
                for (int i = 0; i < TAB_SIZE && len < max_len; i++) {
                    buffer[len++] = ' ';
                }
                terminal_putchar('\t');
            } else if (c >= 32 && c < 127) { // printable chars
                buffer[len++] = c;
                terminal_putchar(c);
            }
        } else if (ev.type == KEY_SPECIAL) {
            switch (ev.code) {
                case KEY_PGUP: terminal_scroll_up(); break;
                case KEY_PGDN: terminal_scroll_down(); break;
            }
            
        }

        if (len >= max_len - 1) break;

        terminal_update_cursor();
    }

    buffer[len] = '\0';
    return buffer;
}