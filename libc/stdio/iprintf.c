/*
FUNCTION
        <<iprintf>>---write formatted output (integer only)
INDEX
    iprintf

ANSI_SYNOPSIS
        #include <stdio.h>

        int iprintf(const char *<[format]>, ...);

TRAD_SYNOPSIS
    #include <stdio.h>

    int iprintf(<[format]> [, <[arg]>, ...])
    char *<[format]>;

DESCRIPTION
<<iprintf>> is a restricted version of <<printf>>: it has the same
arguments and behavior, save that it cannot perform any floating-point
formatting: the <<f>>, <<g>>, <<G>>, <<e>>, and <<F>> type specifiers
are not recognized.

RETURNS
        <<iprintf>> returns the number of bytes in the output string,
        save that the concluding <<NULL>> is not counted.
        <<iprintf>> returns when the end of the format string is
        encountered.  If an error occurs, <<iprintf>>
        returns <<EOF>>.

PORTABILITY
<<iprintf>> is not required by ANSI C.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/


#include <stdio.h>

#ifndef _REENT_ONLY
#include <stdarg.h>

int iprintf(const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    _stdout_r(_REENT)->_data = _REENT;
    ret = vfiprintf(stdout, fmt, ap);
    va_end(ap);
    return ret;
}

#endif /* ! _REENT_ONLY */

#include <stdarg.h>

int _iprintf_r(struct _reent *ptr, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = vfiprintf(_stdout_r(ptr), fmt, ap);
    va_end(ap);
    return ret;
}
