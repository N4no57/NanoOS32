#include <stdio.h>
#include <stdint.h>

int printf(char const* const _Format, ...) {
    uint32_t* arg = &_Format + 1;
    char out_str = "";
    int arg_idx = 0;

    for (int i = 0; i < strlen(_Format); i++) {
        if (_Format[i] != '%') {
            out_str += _Format[i];
        } else {
            i++;
            char specifier = _Format[i];

            switch(specifier) {
                case 'd':
                    int val = *args;
                    args++;
                    break;
            }
        }
    }
}