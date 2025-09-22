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

Node *generate_tree(size_t level_cur, size_t level_max)
{
    if (level_cur < level_max) {
        Node *root = heap_alloc(sizeof(*root));
        assert((char) level_cur - 'a' <= 'z');
        root->x = level_cur + 'a';
        root->left = generate_tree(level_cur + 1, level_max);
        root->right = generate_tree(level_cur + 1, level_max);
        return root;
    } else {
        return NULL;
    }
}


int main(void) {
    stack_base = (const uintptr_t*)__builtin_frame_address(0);

    // unreachable garbage (allocate garbage without saving the pointers)
    for(size_t i = 0; i < 10; ++i) {
        heap_alloc(i);
    }

    // reachable garbage (allocate binary tree)
    Node *root = generate_tree(0, 3);

    // collect garbage from 10 heap_allocs
    heap_collect();
    chunk_list_dump(&alloced_chunks, "Alloced");
    chunk_list_dump(&freed_chunks, "Freed");

    printf("\n-----------------------\n");
    
    // make binary tree garbage and collect it
    root = NULL;
    heap_collect();
    chunk_list_dump(&alloced_chunks, "Alloced");
    chunk_list_dump(&freed_chunks, "Freed");

    return 0;
}
