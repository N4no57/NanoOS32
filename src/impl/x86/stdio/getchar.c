#include <stdio.h>
#include <ps2.h>

int getchar(void) {
    while (parsed_write_ptr == parsed_read_ptr);

    char c = parsed_buff[parsed_read_ptr++];
    return c;
}