/* Concatenate variable number of strings.
   Copyright (C) 1991-2018 Free Software Foundation, Inc.
   Written by Fred Fish @ Cygnus Support

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */


/*

@deftypefn Extension char* concat (const char *@var{s1}, const char *@var{s2}, @
  @dots{}, @code{NULL})

Concatenate zero or more of strings and return the result in freshly
@code{malloc}ed memory.  The argument list is terminated by the first
@code{NULL} pointer encountered.  Pointers to empty strings are ignored.

@end deftypefn

*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "ansidecl.h"
#include "libiberty.h"
#include <sys/types.h>		/* size_t */

#include <stdarg.h>

#include <string.h>

#include <stdlib.h>

static inline unsigned long vconcat_length (const char *, va_list);
static inline unsigned long
vconcat_length (const char *first, va_list args)
{
  unsigned long length = 0;
  const char *arg;

  for (arg = first; arg ; arg = va_arg (args, const char *))
    length += strlen (arg);

  return length;
}

static inline char *
vconcat_copy (char *dst, const char *first, va_list args)
{
  char *end = dst;
  const char *arg;

  for (arg = first; arg ; arg = va_arg (args, const char *))
    {
      unsigned long length = strlen (arg);
      memcpy (end, arg, length);
      end += length;
    }
  *end = '\000';

  return dst;
}

/* @undocumented concat_copy */

char *
concat_copy (char *dst, const char *first, ...)
{
  char *save_dst;
  va_list args;

  va_start (args, first);
  vconcat_copy (dst, first, args);
  save_dst = dst; /* With K&R C, dst goes out of scope here.  */
  va_end (args);

  return save_dst;
}

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
char *libiberty_concat_ptr;
#ifdef __cplusplus
}
#endif /* __cplusplus */


char *
concat (const char *first, ...)
{
  char *newstr;
  va_list args;

  /* First compute the size of the result and get sufficient memory.  */
  va_start (args, first);
  newstr = XNEWVEC (char, vconcat_length (first, args) + 1);
  va_end (args);

  /* Now copy the individual pieces to the result string. */
  va_start (args, first);
  vconcat_copy (newstr, first, args);
  va_end (args);

  return newstr;
}
