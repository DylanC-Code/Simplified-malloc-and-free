#include <unistd.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <errno.h>

#include "heap.h"

int main()
{
    char *m1 = s_malloc(10 * sizeof(char));
    char *m2 = s_malloc(8000 * sizeof(char));
    char *m3 = s_malloc(1000 * sizeof(char));
    char *m4 = s_malloc(300 * sizeof(int));
    char *m5 = s_malloc(3 * sizeof(unsigned long));

    s_free(m2);
    char *m6 = s_malloc(4000 * sizeof(char));

    return 0;
}
