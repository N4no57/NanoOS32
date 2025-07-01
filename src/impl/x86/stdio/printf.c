#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <terminal.h>
#include <stdint.h>

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    
    int buf_size = 64;
    char* out_str = malloc(buf_size);
    if (!out_str) return -1;

    int out_idx = 0;
    int i = 0;

    #define APPEND_CHAR(c) \
        do { \
            if (out_idx + 1 >= buf_size) { \
                buf_size *= 2; \
                char* temp = realloc(out_str, buf_size); \
                if (!temp) { free(out_str); return -1; } \
                out_str = temp; \
            } \
            out_str[out_idx++] = (c); \
        } while (0)

    do {
        if(_Format[i] != '%') { // check if the current character is not the format specifier
            out_str[out_idx++] = _Format[i];
        } else { // go through format specifier logic
            i++;
            char specifier = _Format[i];
            int val;
            char* string;

            switch(specifier) {
                case 'd': // integer
                    val = va_arg(args, int); // get the number
                    char num_buf[20]; // create a 20 digit buffer (big enough to handle 64-bit mode)
                    int num_idx = 0;
                    itoa(val, num_buf); // convert the integer to its ascii counterpart
                    for (int j = 0; num_buf[j] != '\0'; ++j) {
                        APPEND_CHAR(num_buf[j]);
                    }
                    break;
                case 'c': // character
                    val = va_arg(args, int);
                    APPEND_CHAR(val);
                    break;
                case 's': // string
                    string = va_arg(args, char*); // get the string
                    for (int j = 0; string[j] != '\0'; ++j) {
                        APPEND_CHAR(string[j]);
                    }
                    break;
                case 'p': // pointer
                    void* ptr = va_arg(args, void*);
                    uintptr_t addr = (uintptr_t)ptr;
                    char ptr_buf[20];
                    itoa(addr, ptr_buf); // TODO: convert to hex format
                    for (int j = 0; ptr_buf[j] != '\0'; ++j) {
                        APPEND_CHAR(ptr_buf[j]);
                    }
                    break;
                default:
                    free(out_str);
                    return 1; // throw error, dealing with unknow specifier
            }
        }
    } while (_Format[i++] != '\0');
    APPEND_CHAR('\0'); // Null-terminate the string
    terminal_writestring(out_str);

    free(out_str);
    return 0;
}