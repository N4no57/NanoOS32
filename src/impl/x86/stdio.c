#include <stdio.h>
#include <stdint.h>
#include <terminal.h>
#include <string.h>
#include <stdarg.h>

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    char* out_str = "";

    for (int i = 0; i < strlen(_Format); i++) {
        if (_Format[i] != '%') {
            out_str += _Format[i];
        } else {
            i++;
            char specifier = _Format[i];

            switch(specifier) {
                case 'd':
                    int val = va_arg(args, int);
                    out_str += val;
                    break;
                case 'c':
                    char c = va_arg(args, char);
                    out_str += c;
                    break;
            }
        }
    }
    terminal_writestring(out_str);

    return 0;
}