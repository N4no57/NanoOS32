#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <terminal.h>
#include <stdint.h>

int printf(const char* _Format, ...) {
    va_list args;
    va_start(args, _Format);
    char out_str[256]; // ! 256 character size, may cause problems down the line for bigger print jobs
    int out_idx = 0;
    int i = 0;

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
                    do { // loop through the ascii integer to append to the out_str
                        if (num_buf[num_idx] == '\0')
                            break;
                        out_str[out_idx++] = num_buf[num_idx];
                    } while (num_buf[num_idx++] != '\0');
                    break;
                case 'c': // character
                    val = va_arg(args, int);
                    out_str[out_idx++] = (char)val;
                    break;
                case 's': // string
                    string = va_arg(args, char*); // get the string
                    int string_idx = 0;
                    do { // loop through string to append it to the out_str
                        if (string[string_idx] == '\0')
                            break;
                        out_str[out_idx++] = string[string_idx];
                    } while (string[string_idx++] != '\0');
                    break;
                case 'p': // pointer
                    void* ptr = va_arg(args, void*);
                    //out_str[out_idx++] = "0"; // hardcoded "0x" for hex specification
                    //out_str[out_idx++] = "x";
                    // current non hex version
                    // TODO: make this hex
                    char ptr_buff[20]; // create a 20 digit buffer (big enough to handle 64-bit mode)
                    int ptr_idx = 0;
                    itoa((uintptr_t)ptr, ptr_buff); // convert the integer to its ascii counterpart
                    do { // loop through the ascii integer to append to the out_str
                        if (ptr_buff[ptr_idx] == '\0')
                            break;
                        out_str[out_idx++] = ptr_buff[ptr_idx];
                    } while (ptr_buff[ptr_idx++] != '\0');
                    break;
                default:
                    return 1; // throw error, dealing with unknow specifier
            }
        }
    } while (_Format[i++] != '\0');
    terminal_writestring(out_str);

    return 0;
}