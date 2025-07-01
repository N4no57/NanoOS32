#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    //printf("heap contains, %d bytes\n", heap.avail);
}

static bool debug_malloc = false;

void* malloc(size_t size) {
    struct heapchunk_t *chunk = heap.start;
    struct heapchunk_t *prev = NULL;

    if (debug_malloc) {
        legacy_printf("[malloc]     Requested size: %d\n", size);
    }

    //check if the heap has enough space
    while (chunk != NULL) {
        if (!chunk->inuse && chunk->size >= size) {
            break;
        }
        prev = chunk;
        chunk = chunk->next;
    }

    if (chunk == NULL) {
        if (debug_malloc) {
            legacy_printf("[malloc] No suitable chunk found. Heap exhausted.\n");
            legacy_printf("[malloc] too bad, so sad, get the FUCK OUTTA HERE\n");
        }
        return NULL;
    }

    if (debug_malloc) {
        legacy_printf("[malloc]     Found suitable chunk at %p\n", (void*)chunk);
        legacy_printf("[malloc]     Chunk size: %d\n", chunk->size);
        legacy_printf("[malloc]     In use?     %s\n", chunk->inuse ? "yes" : "no");
    }

    size_t leftover = chunk->size - size;
    if (debug_malloc) {
        legacy_printf("[malloc]     Leftover space after allocation: %d\n", leftover);
    }

    if (leftover > sizeof(struct heapchunk_t) + 4) {  // leave room for a new chunk
        struct heapchunk_t *new_chunk = (struct heapchunk_t *)((char *)chunk + sizeof(struct heapchunk_t) + size);
        new_chunk->size = leftover - sizeof(struct heapchunk_t);
        new_chunk->inuse = false;
        new_chunk->next = chunk->next;

        chunk->next = new_chunk;
        chunk->size = size;

        if (debug_malloc) {
            legacy_printf("[malloc]     Splitting chunk:\n");
            legacy_printf("           - Allocated chunk size: %d\n", chunk->size);
            legacy_printf("           - New free chunk at %p, size: %d\n", (void*)new_chunk, new_chunk->size);
        }
    }

    chunk->inuse = true;

    if (debug_malloc) {
        legacy_printf("[malloc]     Allocation successful! Returning user pointer: %p\n\n", (char *)chunk + sizeof(struct heapchunk_t));
    }

    return (char *)chunk + sizeof(struct heapchunk_t);
}

void coalesce_chunk(void) {
    struct heapchunk_t *chunk = heap.start;
    while (chunk && chunk->next) {
        if (!chunk->inuse && !chunk->next->inuse) {
            chunk->size += sizeof(struct heapchunk_t) + chunk->next->size;
            chunk->next = chunk->next->next;
        } else {
            chunk = chunk->next;
        }
    }
}

void free(void* ptr) {
    if (!ptr) return;

    struct heapchunk_t *chunk = (struct heapchunk_t *)((char *)ptr - sizeof(struct heapchunk_t));
    chunk->inuse = false;

    coalesce_chunk();

    return;
}

static bool debug_realloc = false;

void* extendChunkInPlace(void* ptr, size_t new_size, struct heapchunk_t *old_chunk) {
    if (debug_realloc) {
        legacy_printf("[extendChunkInPlace] Attempting in-place extension...\n");
        legacy_printf("[extendChunkInPlace] Current size: %d, Requested size: %d\n", old_chunk->size, new_size);
    }

    if (old_chunk->next->inuse == false) {
        size_t extra_space_required = new_size-old_chunk->size; //* get the space the chunk needs to be extended by

        if (debug_realloc) {
            legacy_printf("[extendChunkInPlace] Next chunk is free. Extra space required: %d\n", extra_space_required);
            legacy_printf("[extendChunkInPlace] Next chunk size: %d\n", old_chunk->next->size);
        }

        if (old_chunk->next->size >= extra_space_required + sizeof(struct heapchunk_t)) { //* check if free chunk has enough space
            struct heapchunk_t* next = old_chunk->next;
            struct heapchunk_t* split = (struct heapchunk_t*)((char *)next + extra_space_required);

            split->size = next->size - extra_space_required - sizeof(struct heapchunk_t);
            split->inuse = false;
            split->next = next->next;

            old_chunk->size += extra_space_required;
            old_chunk->next = split;

            if (debug_realloc) {
                legacy_printf("[extendChunkInPlace] In-place extension successful.\n");
                legacy_printf("[extendChunkInPlace] New size: %d\n", old_chunk->size);
            }

            return (char *)old_chunk + sizeof(struct heapchunk_t);
        }
    }

    if (debug_realloc) {
        legacy_printf("[extendChunkInPlace] In-place extension failed.\n");
    }

    return NULL;
}

void* realloc(void *ptr, size_t new_size) {
    //* if ptr == NULL malloc() new block and return
    if (!ptr) {
        if (debug_realloc) {
            legacy_printf("[realloc] NULL ptr passed in, calling malloc(%d).\n", new_size);
        }
        return malloc(new_size);
    }

    struct heapchunk_t *old_chunk = (struct heapchunk_t *)((char *)ptr - sizeof(struct heapchunk_t));
    //* if new size is larger the current size
    if (old_chunk->size < new_size) {
        if (debug_realloc) {
            legacy_printf("[realloc] Requesting growth: current size %d, new size %d.\n", old_chunk->size, new_size);
        }

        //* attempt to extend in place
        if (extendChunkInPlace(ptr, new_size, old_chunk)) {
            if (debug_realloc) {
                legacy_printf("[realloc] Extended in place.\n");
            }

            return ptr; // In-place extension successful
        }

        if (debug_realloc) {
            legacy_printf("[realloc] Could not extend in place. Allocating new block.\n");
        }

        //* otherwise allocate completely new block
        void* new_ptr = malloc(new_size);
        if (!new_ptr) {
            if (debug_realloc) {
                legacy_printf("[realloc] malloc() failed for size %d.\n", new_size);
            }
            return NULL;
        }
        memcpy(new_ptr, ptr, (old_chunk->size < new_size ? old_chunk->size : new_size));
        free(ptr); //* free old block and coalesce heap

        if (debug_realloc) {
            legacy_printf("[realloc] Copied data to new block and freed old one.\n");
        }

        return new_ptr; //* return pointer to new block
    }

    // TODO
    //* if new size is smaller
    //* shrink block in place

    if (debug_realloc) {
        legacy_printf("[realloc] Shrinking not implemented yet.\n");
    }

    //* placeholder return until chunk shrinking implemented
    return NULL;
}