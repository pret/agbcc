#include <stdlib.h>

int rand_r(unsigned int *seed)
{
    return (((*seed) = (*seed) * 1103515245 + 12345) & RAND_MAX);
}
