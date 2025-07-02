#include "../../../Intf/stdio.h"
#include "../../../intf/ps2.h"

KeyEvent get_key_event() {
    KeyEvent event;
    if (key_event_read_ptr != key_event_write_ptr) {
        event.type = KEY_SPECIAL;
        event.code = key_event[key_event_read_ptr++];
    } else if (parsed_read_ptr != parsed_write_ptr) {
        event.type = KEY_ASCII;
        event.ascii = parsed_buff[parsed_read_ptr++];
    } else {
        event.type = ERROR;
    }
    return event;
}