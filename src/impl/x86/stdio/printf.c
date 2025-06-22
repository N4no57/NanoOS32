#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <terminal.h>

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    char out_str[256];
    int out_idx = 0;
    int i = 0;

    do {
        if(_Format[i] != '%') {
            out_str[out_idx++] = _Format[i];
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
                        if (num_buf[num_idx] == '\0')
                            break;
                        out_str[out_idx++] = num_buf[num_idx];
                    } while (num_buf[num_idx++] != '\0');
                    break;
                case 'c':
                    val = va_arg(args, int);
                    out_str[out_idx++] = (char)val;
                    break;
                case 's':
                    string = va_arg(args, char*);
                    int string_idx = 0;
                    do {
                        if (string[string_idx] == '\0')
                            break;
                        out_str[out_idx++] = string[string_idx];
                    } while (string[string_idx++] != '\0');
                    break;
                default:
                    return 1; // throw error, dealing with unknow specifier
            }
        }
    } while (_Format[i++] != '\0');
    terminal_writestring(out_str);

    return 0;
}