#include "heap.h"
#include <stdio.h>

uintptr_t heap[HEAP_CAP_WORDS] = {0};

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {
    .count = 1,
    .chunks = {
        [0] = {.start = heap, .size = sizeof(heap)},
    }
};

Chunk_List tmp_chunks = {0};

void chunk_list_insert(Chunk_List *list, void *start, size_t size)
{
    assert(list->count < CHUNK_LIST_CAP);
    list->chunks[list->count].start = start;
    list->chunks[list->count].size = size;

    for(size_t i = list->count; i > 0 && list->chunks[i].start < list->chunks[i-1].start; --i) {
        Chunk t = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i-1] = t;
    }

    list->count ++;
}

void chunk_list_merge(Chunk_List *dst, const Chunk_List *src) 
{
    dst->count = 0;
    for(size_t i = 0; i < src->count; ++i) {
        const Chunk chunk = src->chunks[i];
        if(dst->count > 0) {
            Chunk *top_chunk = &dst->chunks[dst->count - 1];

            if (top_chunk->start + top_chunk->size == chunk.start) {
                top_chunk->size += chunk.size;
            } else {
                chunk_list_insert(dst, chunk.start, chunk.size);
            }
        } else {
            chunk_list_insert(dst, chunk.start, chunk.size);
        }
    }
}

void chunk_list_dump(const Chunk_List *list)
{   
    printf("Chunks (%zu):\n", list->count);
    for(size_t i = 0; i < list->count; ++i) {
        printf("    start: %p, size: %zu\n", (void *) list->chunks[i].start, list->chunks[i].size);
    }
}

int chunk_list_find(const Chunk_List *list, uintptr_t *ptr)
{
    for(size_t i = 0; i < list->count; ++i) {
        if(list->chunks[i].start == ptr) {
            return (int) i;
        }
    }

    return -1;
}

void chunk_list_remove(Chunk_List *list, size_t index)
{
    assert(index < list->count);
    for(size_t i = index; i < list->count - 1; ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }

    list->count --;
}

void *heap_alloc(size_t size_bytes) 
{
    const size_t size_words = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    if(size_words <= 0) return NULL;

    chunk_list_merge(&tmp_chunks, &freed_chunks);
    freed_chunks = tmp_chunks;

    for(size_t i = 0; i < freed_chunks.count; ++i) {
        const Chunk chunk = freed_chunks.chunks[i];
        if(chunk.size >= size_words) {
            chunk_list_remove(&freed_chunks, i);

            const size_t tail_size_words = chunk.size - size_words;
            chunk_list_insert(&alloced_chunks, chunk.start, size_words);

            if(tail_size_words > 0) {
                chunk_list_insert(&freed_chunks, chunk.start + size_words, tail_size_words);
            }

            return chunk.start;
        }
    }

    return NULL;
}

void heap_free(void *ptr) 
{
    if(ptr == NULL) return;

    const int index = chunk_list_find(&alloced_chunks, ptr);
    
    assert(index >= 0);
    assert(ptr == alloced_chunks.chunks[index].start);

    chunk_list_insert(&freed_chunks, alloced_chunks.chunks[index].start, alloced_chunks.chunks[index].size);
    chunk_list_remove(&alloced_chunks, (size_t) index);
}

// TODO: garbage collector
void heap_collect(void)
{
    assert(false && "heap_collect is not implemented");
}
