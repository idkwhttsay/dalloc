#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

typedef struct Node Node;

struct Node {
    char x;
    Node *left;
    Node *right;
};

#define N 10
void *ptrs[N] = {0};

int main(void) {
    size_t heap_ptrs_count = 0;
    for(size_t i = 0; i < alloced_chunks.count; ++i) {
        for(size_t j = 0; j < alloced_chunks.chunks[i].size; ++j) {
            uintptr_t *p = (uintptr_t*) alloced_chunks.chunks[i].start[j];
            if(heap <= p && p < heap + HEAP_CAP_WORDS) {
                printf("DETECTED HEAP POINTER: %p\n", (void*) p);
                heap_ptrs_count ++;
            }
        }
    }

    printf("Detected %zu heap pointers\n", heap_ptrs_count);

    return 0;
}
