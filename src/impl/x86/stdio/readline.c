#include "../../../intf/stdio.h"

void readline(char *buffer, size_t max_len) {
    size_t len = 0;

    while (1) {
        KeyEvent ev = get_key_event();
        
        if (ev.type == ERROR) {
            return;
        }

        if (ev.type == KEY_ASCII) {

        } else if (ev.type == KEY_SPECIAL) {
            
        }
    }
}