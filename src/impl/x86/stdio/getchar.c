#include <stdio.h>
#include <ps2.h>

int getchar(void) {
    while (write_ptr == read_ptr);

    char c = input_buff[read_ptr++];
    return c;
}