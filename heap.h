#ifndef HEAP_H
#define HEAP_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define CHUNK_LIST_CAP 1024
#define HEAP_CAP_BYTES 640000
static_assert(HEAP_CAP_BYTES % sizeof(uintptr_t) == 0, 
    "The heap capacity is not divisible by the size of a word of the OS (platform)");
#define HEAP_CAP_WORDS (HEAP_CAP_BYTES / sizeof(uintptr_t))

extern uintptr_t heap[HEAP_CAP_WORDS];

typedef struct Chunk {
    uintptr_t *start;
    size_t size;
} Chunk;

typedef struct {
    size_t count;
    Chunk chunks[CHUNK_LIST_CAP];
} Chunk_List;

extern Chunk_List alloced_chunks;
extern Chunk_List freed_chunks;
extern Chunk_List tmp_chunks;

void *heap_alloc(size_t size_bytes);
void heap_free(void *ptr);
void heap_collect(void);

void chunk_list_insert(Chunk_List *list, void *start, size_t size);
void chunk_list_merge(Chunk_List *dst, const Chunk_List *src);
void chunk_list_dump(const Chunk_List *list);
int chunk_list_find(const Chunk_List *list, uintptr_t *ptr);
void chunk_list_remove(Chunk_List *list, size_t index);

#endif // HEAP_H
