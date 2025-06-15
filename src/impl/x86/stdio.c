#include <stdio.h>
#include <terminal.h>
#include <string.h>
#include <stdarg.h>

void itoa(int value, char* str) {
    char temp[16];
    int i = 0, j = 0;
    int is_negative = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value != 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (is_negative)
        temp[i++] = '-';

    while (i--)
        str[j++] = temp[i];
    str[j] = '\0';
}

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    char out_str[256];
    int out_idx = 0;
    int i = 0;

    do {
        if(_Format[i] != '%') {
            out_str[out_idx] = _Format[i];
            out_idx++;
        } else {
            i++;
            char specifier = _Format[i];
            int val;
            char* string;

            switch(specifier) {
                case 'd':
                    val = va_arg(args, int);
                    char num_buf[16];
                    int num_idx = 0;
                    itoa(val, num_buf);
                    do {
                        out_str[out_idx++] = num_buf[num_idx];
                    } while (num_buf[num_idx++] != '\0');
                    break;
                case 'c':
                    val = va_arg(args, int);
                    out_str[out_idx] = (char)val;
                    out_idx++;
                    break;
                case 's':
                    string = va_arg(args, char*);
                    int string_idx = 0;
                    do {
                        out_str[out_idx] = string[string_idx];
                    } while (string[string_idx++] != '\0');
                    break;
            }
        }
    } while (_Format[i++] != '\0');
    terminal_writestring(out_str);

    return 0;
}