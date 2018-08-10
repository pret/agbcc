/* mtrim.c -- a wrapper for malloc_trim.  */


#include <reent.h>
#include <stdlib.h>
#include <malloc.h>

#ifndef _REENT_ONLY

int malloc_trim(size_t pad)
{
    return _malloc_trim_r(_REENT, pad);
}

#endif
