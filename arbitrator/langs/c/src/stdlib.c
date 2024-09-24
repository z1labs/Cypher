#include <stdlib.h>

#define PAGE_SIZE 65536

static size_t next_free;
static size_t heap_end;

void *malloc(size_t size) {
    if (size > heap_end - next_free) {
        size_t pages_required = (size - (heap_end - next_free) + PAGE_SIZE - 1) / PAGE_SIZE;
        size_t prev_pages = __builtin_wasm_memory_grow(0, pages_required);
        if (next_free == 0) {
            next_free = prev_pages * PAGE_SIZE;
        }
        heap_end = (prev_pages + pages_required) * PAGE_SIZE;
    }
    void *retval = (void *)next_free;
    next_free += size;
    return retval;
}

// free does nothing
void free(void *ptr) {}
