#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <bits/mman-linux.h>

void *s_malloc(size_t size);
int s_free(void *p_addr);

int main()
{

    char *my_string = s_malloc(3 * sizeof(char));
    my_string[0] = 'J';
    my_string[1] = 'a';
    my_string[2] = 'm';
    printf("My string equal %s\n", my_string);
    printf("First char address %ld\n", &my_string[0]);
    s_free(my_string);

    return 0;
}

#define MAX_ALLOCATION 1000

struct Alloc
{
    void *addr;
    size_t size;
};

static struct Alloc allocations[MAX_ALLOCATION];
static int allocations_length = 0;

void *s_malloc(size_t size)
{
    if (allocations_length >= MAX_ALLOCATION)
    {
        printf("Maximum allocation '%d' reached. You must released to be able reallocatting.\n", MAX_ALLOCATION);
        return (void *)-1;
    }
    uintptr_t *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (addr != MAP_FAILED)
    {
        struct Alloc *alloc = &allocations[allocations_length++];
        alloc->addr = addr;
        alloc->size = size;
    }
    else
    {
        perror("mmap failed.\n");
    }

    return addr;
}

int s_free(void *p_addr)
{
    for (int i = 0; i < allocations_length; i++)
    {
        if (allocations[i].addr == p_addr)
        {
            struct Alloc *p_alloc = &allocations[i];

            int result = munmap(p_alloc->addr, p_alloc->size);
            allocations[i] = allocations[--allocations_length];
            return result;
        }
    }

    return -1;
}
