/*
FUNCTION
    <<bcopy>>---copy memory regions

ANSI_SYNOPSIS
    #include <string.h>
    void bcopy(const char *<[in]>, char  *<[out]>, size_t <[n]>);

TRAD_SYNOPSIS
    void bcopy(<[in]>, <[out]>, <[n]>
    char *<[in]>;
    char *<[out]>;
    size_t <[n]>;

DESCRIPTION
    This function copies <[n]> bytes from the memory region
    pointed to by <[in]> to the memory region pointed to by
    <[out]>.

    This function is implemented in term of <<memmove>>.

PORTABILITY
<<bcopy>> requires no supporting OS subroutines.

QUICKREF
    bcopy - pure
*/

#include <string.h>

void bcopy(const char *b1, char *b2, size_t length)
{
    memmove((void *)b2, (void *)b1, length);
}
