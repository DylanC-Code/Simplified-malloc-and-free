#include <unistd.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <errno.h>

#define MINIMUM_BLOCK_SIZE 4096

typedef struct Block
{
    size_t size;
    int free;
    struct Block *next;
} Block;

static Block *first_heap_block = NULL;

Block *find_free_block(Block **last, size_t size);
Block *extend_heap(Block *last, size_t size);
void merge_free_blocks();

void *s_malloc(size_t size)
{
    sbrk(0);
    sbrk(0);

    Block *block;
    Block *last = first_heap_block;

    block = find_free_block(&last, size);

    if (!block)
    {
        block = extend_heap(last, size);
    }
    else
    {
        block->free = 0;
    }

    if (!first_heap_block)
    {
        first_heap_block = block;
    }

    return (void *)(block + 1);
}

void s_free(void *p_address)
{
    if (!p_address)
        return;

    Block *block = (Block *)p_address - 1;
    block->free = 1;

    merge_free_blocks();
}

Block *find_free_block(Block **last, size_t size)
{
    Block *current = first_heap_block;

    while (current && !(current->size >= size && current->free))
    {
        *last = current;
        current = current->next;
    }

    return current;
}

Block *extend_heap(Block *last, size_t size)
{
    Block *new_block = sbrk(sizeof(Block) + size);

    new_block->free = 0;
    new_block->size = size;
    new_block->next = NULL;

    if (last)
    {
        last->next = new_block;
    }

    return new_block;
}

void merge_free_blocks()
{
    Block *current = first_heap_block;

    while (current && current->next)
    {
        if (current->free && current->next->free)
        {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}
