/*
 * Andy Wilson, 2-Oct-89.
 */

#include <stdlib.h>

long atol(const char *s)
{
    return strtol(s, NULL, 10);
}
