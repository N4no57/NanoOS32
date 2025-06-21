#include <stdint.h>
#include <stdbool.h>

extern uint8_t heap_start;
extern uint8_t heap_end;

struct heapinfo_t {
    struct heapchunk_t *start;
    uint32_t avail;
};

struct heapchunk_t {
    uint32_t size;
    bool inuse;
    struct heapchunk_t *next;
};

static struct heapinfo_t heap;

void heap_init(void) {
    heap.start = (struct heapchunk_t*)(uintptr_t)&heap_start;
    heap.avail = (uintptr_t)&heap_end - (uintptr_t)&heap_start;

    heap.start->size = heap.avail - sizeof(struct heapchunk_t);
    heap.start->inuse = false;
    heap.start->next = NULL;
}

void* malloc(size_t size) {
    
}

void* free(void* ptr) {

}