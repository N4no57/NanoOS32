#include "privateTime.h"

uint32_t get_uptime() { // ticks are counted every 100 Hz so 1/100 seconds, so dividing by 100 gives you a second
    return tick / 100;
}