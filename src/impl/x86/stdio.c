#include <stdio.h>
#include <stdint.h>
#include <terminal.h>
#include <string.h>
#include <stdarg.h>

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    char out_str[256];
    int out_idx = 0;

    for (int i = 0; i < strlen(_Format); i++) {
        if (_Format[i] != '%') {
            out_str[out_idx] = _Format[i];
            out_idx++;
        } else {
            i++;
            char specifier = _Format[i];
            int val;

            switch(specifier) {
                case 'd':
                    val = va_arg(args, int);
                    out_str[out_idx] = (char)val;
                    out_idx++;
                    break;
                case 'c':
                    val = va_arg(args, int);
                    out_str[out_idx] = (char)val;
                    out_idx++;
                    break;
            }
        }
    }
    terminal_writestring(out_str);

    return 0;
}