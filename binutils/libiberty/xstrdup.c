/* xstrdup.c -- Duplicate a string in memory, using malloc.
   This trivial function is in the public domain.
   Ian Lance Taylor, Cygnus Support, December 1995.  */

/*

@deftypefn Replacement char* xstrdup (const char *@var{s})

Duplicates a character string without fail, using @code{malloc} to
obtain memory.

@end deftypefn

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sys/types.h>
#include <string.h>
#include "ansidecl.h"
#include "libiberty.h"

char *
xstrdup (const char *s)
{
  register size_t len = strlen (s) + 1;
  register char *ret = XNEWVEC (char, len);
  return (char *) memcpy (ret, s, len);
}
