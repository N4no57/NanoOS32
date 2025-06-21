#include <stdint.h>
#include <stdbool.h>

struct heapinfo_t {
    struct heapchunk_t *start;
    uint32_t avail;
};

struct heapchunk_t {
    uint32_t size;
    bool inuse;
    struct heapchunk_t *next;
};