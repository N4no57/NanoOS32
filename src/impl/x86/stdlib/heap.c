#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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
    printf("heap contains, %d bytes\n", heap.avail);
}

static bool debug_malloc = false;

void* malloc(size_t size) {
    struct heapchunk_t *chunk = heap.start;
    struct heapchunk_t *prev = NULL;

    //check if the heap has enough space
    while (chunk != NULL) {
        if (!chunk->inuse && chunk->size >= size) {
            break;
        }
        prev = chunk;
        chunk = chunk->next;
    }

    if (chunk == NULL) {
        printf("No suitable chunk\ntoo bad, so sad, get the FUCK OUTTA HERE\n");
        return NULL;
    }

    if (debug_malloc) printf("Found Chunk: %p\n", chunk);
    if (debug_malloc) printf("Initial chunk size: %d\n", chunk->size);

    size_t leftover = chunk->size - size;
    if (debug_malloc) printf("Leftover heap: %d\n", leftover);
    if (leftover > sizeof(struct heapchunk_t) + 4) {  // leave room for a new chunk
        struct heapchunk_t *new_chunk = (struct heapchunk_t *)((char *)chunk + sizeof(struct heapchunk_t) + size);
        new_chunk->size = leftover - sizeof(struct heapchunk_t);
        new_chunk->inuse = false;
        new_chunk->next = chunk->next;

        chunk->next = new_chunk;
        chunk->size = size;
    }
    if (debug_malloc) printf("chunk->size: %d\n", chunk->size);
    if (debug_malloc) printf("new chunk size: %d\n", chunk->next->size);

    chunk->inuse = true;

    return (char *)chunk + sizeof(struct heapchunk_t);
}

void free(void* ptr) {
    if (!ptr) return;

    struct heapchunk_t *chunk = (struct heapchunk_t *)((char *)ptr - sizeof(struct heapchunk_t));
    chunk->inuse = false;

    return;
}