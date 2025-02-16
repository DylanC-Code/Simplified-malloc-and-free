#include <unistd.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <errno.h>

#define MINIMUM_BLOCK_SIZE 4096

struct Block
{
    size_t size;
    int free;
    struct Block *next;
};

static struct Block first_heap_block;
static void *heap_address;

int calculate_block_size(size_t size);
void create_block(size_t size, struct Block *p_block);
int check_block(struct Block *block, size_t size);
void initialize_heap();

int calculate_block_size(size_t size)
{
    int block_size = MINIMUM_BLOCK_SIZE;

    while (block_size < size)
    {
        block_size += MINIMUM_BLOCK_SIZE;
    }

    return block_size;
}

void create_block(size_t size, struct Block *p_block)
{
    p_block->size = calculate_block_size(size);
    p_block->free = 1;
    p_block->next = NULL;
}

int check_block(struct Block *block, size_t size)
{
    int is_free = block->free;
    int is_fitting_size = block->size >= size;

    if (is_free && is_fitting_size)
    {
        return 1;
    }

    return 0;
}

void *s_malloc(size_t size)
{
    if (heap_address == NULL)
        initialize_heap();

    int heap_offset = 0;
    struct Block *block = &first_heap_block;

    int is_valid_block = check_block(block, size);

    while (!is_valid_block)
    {
        heap_offset += block->size;

        int is_next_block = block->next != NULL;

        if (is_next_block)
        {
            block = block->next;
        }
        else
        {
            struct Block new_block;
            block = block->next = &new_block;
            create_block(size, &new_block);
        }

        is_valid_block = check_block(block, size);
    }

    block->free = 0;
    return heap_address + heap_offset;
}

void initialize_heap()
{
    sbrk(0);
    sbrk(0);
    heap_address = sbrk(0);
    create_block(0, &first_heap_block);
    printf("Heap initialized with address %p\n", heap_address);
}