#include <stdlib.h>

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