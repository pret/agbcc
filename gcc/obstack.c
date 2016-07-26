/* obstack.c - subroutines used implicitly by object stack macros
   Copyright (C) 1988,89,90,91,92,93,94,96,97 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#include <stdio.h>
#include <stdlib.h>
#include "obstack.h"


#if defined (__STDC__) && __STDC__
#define POINTER void *
#else
#define POINTER char *
#endif

/* Determine default alignment.  */
struct fooalign {char x; double d;};
#define DEFAULT_ALIGNMENT  \
  ((ptrdiff_t) ((char *) &((struct fooalign *) 0)->d - (char *) 0))
/* If malloc were really smart, it would round addresses to DEFAULT_ALIGNMENT.
   But in fact it might be less smart and round addresses to as much as
   DEFAULT_ROUNDING.  So we prepare for it to do that.  */
union fooround {long x; double d;};
#define DEFAULT_ROUNDING (sizeof (union fooround))

/* When we copy a long block of data, this is the unit to do it with.
   On some machines, copying successive ints does not work;
   in such a case, redefine COPYING_UNIT to `long' (if that works)
   or `char' as a last resort.  */
#ifndef COPYING_UNIT
#define COPYING_UNIT int
#endif


/* The functions allocating more room by calling `obstack_chunk_alloc'
   jump to the handler pointed to by `obstack_alloc_failed_handler'.
   This variable by default points to the internal function
   `print_and_abort'.  */
#if defined (__STDC__) && __STDC__
static void print_and_abort (void);
void (*obstack_alloc_failed_handler) (void) = print_and_abort;
#else
static void print_and_abort ();
void (*obstack_alloc_failed_handler) () = print_and_abort;
#endif

/* Exit value used when `print_and_abort' is used.  */
#if defined __GNU_LIBRARY__ || defined HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
int obstack_exit_failure = EXIT_FAILURE;

/* The non-GNU-C macros copy the obstack into this global variable
   to avoid multiple evaluation.  */

struct obstack *_obstack;

/* Define a macro that either calls functions with the traditional malloc/free
   calling interface, or calls functions with the mmalloc/mfree interface
   (that adds an extra first argument), based on the state of use_extra_arg.
   For free, do not use ?:, since some compilers, like the MIPS compilers,
   do not allow (expr) ? void : void.  */

#if defined (__STDC__) && __STDC__
#define CALL_CHUNKFUN(h, size) \
  (((h) -> use_extra_arg) \
   ? (*(h)->chunkfun) ((h)->extra_arg, (size)) \
   : (*(struct _obstack_chunk *(*) (long)) (h)->chunkfun) ((size)))

#define CALL_FREEFUN(h, old_chunk) \
  do { \
    if ((h) -> use_extra_arg) \
      (*(h)->freefun) ((h)->extra_arg, (old_chunk)); \
    else \
      (*(void (*) (void *)) (h)->freefun) ((old_chunk)); \
  } while (0)
#else
#define CALL_CHUNKFUN(h, size) \
  (((h) -> use_extra_arg) \
   ? (*(h)->chunkfun) ((h)->extra_arg, (size)) \
   : (*(struct _obstack_chunk *(*) ()) (h)->chunkfun) ((size)))

#define CALL_FREEFUN(h, old_chunk) \
  do { \
    if ((h) -> use_extra_arg) \
      (*(h)->freefun) ((h)->extra_arg, (old_chunk)); \
    else \
      (*(void (*) ()) (h)->freefun) ((old_chunk)); \
  } while (0)
#endif


/* Initialize an obstack H for use.  Specify chunk size SIZE (0 means default).
   Objects start on multiples of ALIGNMENT (0 means use default).
   CHUNKFUN is the function to use to allocate chunks,
   and FREEFUN the function to free them.

   Return nonzero if successful, zero if out of memory.
   To recover from an out of memory error,
   free up some memory, then call this again.  */

int
_obstack_begin (h, size, alignment, chunkfun, freefun)
     struct obstack *h;
     int size;
     int alignment;
#if defined (__STDC__) && __STDC__
     POINTER (*chunkfun) (long);
     void (*freefun) (void *);
#else
     POINTER (*chunkfun) ();
     void (*freefun) ();
#endif
{
  register struct _obstack_chunk *chunk; /* points to new chunk */

  if (alignment == 0)
    alignment = (int) DEFAULT_ALIGNMENT;
  if (size == 0)
    /* Default size is what GNU malloc can fit in a 4096-byte block.  */
    {
      /* 12 is sizeof (mhead) and 4 is EXTRA from GNU malloc.
	 Use the values for range checking, because if range checking is off,
	 the extra bytes won't be missed terribly, but if range checking is on
	 and we used a larger request, a whole extra 4096 bytes would be
	 allocated.

	 These number are irrelevant to the new GNU malloc.  I suspect it is
	 less sensitive to the size of the request.  */
      int extra = ((((12 + DEFAULT_ROUNDING - 1) & ~(DEFAULT_ROUNDING - 1))
		    + 4 + DEFAULT_ROUNDING - 1)
		   & ~(DEFAULT_ROUNDING - 1));
      size = 4096 - extra;
    }

#if defined (__STDC__) && __STDC__
  h->chunkfun = (struct _obstack_chunk * (*)(void *, long)) chunkfun;
  h->freefun = (void (*) (void *, struct _obstack_chunk *)) freefun;
#else
  h->chunkfun = (struct _obstack_chunk * (*)()) chunkfun;
  h->freefun = freefun;
#endif
  h->chunk_size = size;
  h->alignment_mask = alignment - 1;
  h->use_extra_arg = 0;

  chunk = h->chunk = CALL_CHUNKFUN (h, h -> chunk_size);
  if (!chunk)
    (*obstack_alloc_failed_handler) ();
  h->next_free = h->object_base = chunk->contents;
  h->chunk_limit = chunk->limit
    = (char *) chunk + h->chunk_size;
  chunk->prev = 0;
  /* The initial chunk now contains no empty object.  */
  h->maybe_empty_object = 0;
  h->alloc_failed = 0;
  return 1;
}

int
_obstack_begin_1 (h, size, alignment, chunkfun, freefun, arg)
     struct obstack *h;
     int size;
     int alignment;
#if defined (__STDC__) && __STDC__
     POINTER (*chunkfun) (POINTER, long);
     void (*freefun) (POINTER, POINTER);
#else
     POINTER (*chunkfun) ();
     void (*freefun) ();
#endif
     POINTER arg;
{
  register struct _obstack_chunk *chunk; /* points to new chunk */

  if (alignment == 0)
    alignment = (int) DEFAULT_ALIGNMENT;
  if (size == 0)
    /* Default size is what GNU malloc can fit in a 4096-byte block.  */
    {
      /* 12 is sizeof (mhead) and 4 is EXTRA from GNU malloc.
	 Use the values for range checking, because if range checking is off,
	 the extra bytes won't be missed terribly, but if range checking is on
	 and we used a larger request, a whole extra 4096 bytes would be
	 allocated.

	 These number are irrelevant to the new GNU malloc.  I suspect it is
	 less sensitive to the size of the request.  */
      int extra = ((((12 + DEFAULT_ROUNDING - 1) & ~(DEFAULT_ROUNDING - 1))
		    + 4 + DEFAULT_ROUNDING - 1)
		   & ~(DEFAULT_ROUNDING - 1));
      size = 4096 - extra;
    }

#if defined(__STDC__) && __STDC__
  h->chunkfun = (struct _obstack_chunk * (*)(void *,long)) chunkfun;
  h->freefun = (void (*) (void *, struct _obstack_chunk *)) freefun;
#else
  h->chunkfun = (struct _obstack_chunk * (*)()) chunkfun;
  h->freefun = freefun;
#endif
  h->chunk_size = size;
  h->alignment_mask = alignment - 1;
  h->extra_arg = arg;
  h->use_extra_arg = 1;

  chunk = h->chunk = CALL_CHUNKFUN (h, h -> chunk_size);
  if (!chunk)
    (*obstack_alloc_failed_handler) ();
  h->next_free = h->object_base = chunk->contents;
  h->chunk_limit = chunk->limit
    = (char *) chunk + h->chunk_size;
  chunk->prev = 0;
  /* The initial chunk now contains no empty object.  */
  h->maybe_empty_object = 0;
  h->alloc_failed = 0;
  return 1;
}

/* Allocate a new current chunk for the obstack *H
   on the assumption that LENGTH bytes need to be added
   to the current object, or a new object of length LENGTH allocated.
   Copies any partial object from the end of the old chunk
   to the beginning of the new one.  */

void
_obstack_newchunk (h, length)
     struct obstack *h;
     int length;
{
  register struct _obstack_chunk *old_chunk = h->chunk;
  register struct _obstack_chunk *new_chunk;
  register long	new_size;
  register long obj_size = h->next_free - h->object_base;
  register long i;
  long already;

  /* Compute size for new chunk.  */
  new_size = (obj_size + length) + (obj_size >> 3) + 100;
  if (new_size < h->chunk_size)
    new_size = h->chunk_size;

  /* Allocate and initialize the new chunk.  */
  new_chunk = CALL_CHUNKFUN (h, new_size);
  if (!new_chunk)
    (*obstack_alloc_failed_handler) ();
  h->chunk = new_chunk;
  new_chunk->prev = old_chunk;
  new_chunk->limit = h->chunk_limit = (char *) new_chunk + new_size;

  /* Move the existing object to the new chunk.
     Word at a time is fast and is safe if the object
     is sufficiently aligned.  */
  if (h->alignment_mask + 1 >= DEFAULT_ALIGNMENT)
    {
      for (i = obj_size / sizeof (COPYING_UNIT) - 1;
	   i >= 0; i--)
	((COPYING_UNIT *)new_chunk->contents)[i]
	  = ((COPYING_UNIT *)h->object_base)[i];
      /* We used to copy the odd few remaining bytes as one extra COPYING_UNIT,
	 but that can cross a page boundary on a machine
	 which does not do strict alignment for COPYING_UNITS.  */
      already = obj_size / sizeof (COPYING_UNIT) * sizeof (COPYING_UNIT);
    }
  else
    already = 0;
  /* Copy remaining bytes one by one.  */
  for (i = already; i < obj_size; i++)
    new_chunk->contents[i] = h->object_base[i];

  /* If the object just copied was the only data in OLD_CHUNK,
     free that chunk and remove it from the chain.
     But not if that chunk might contain an empty object.  */
  if (h->object_base == old_chunk->contents && ! h->maybe_empty_object)
    {
      new_chunk->prev = old_chunk->prev;
      CALL_FREEFUN (h, old_chunk);
    }

  h->object_base = new_chunk->contents;
  h->next_free = h->object_base + obj_size;
  /* The new chunk certainly contains no empty object yet.  */
  h->maybe_empty_object = 0;
}

/* Return nonzero if object OBJ has been allocated from obstack H.
   This is here for debugging.
   If you use it in a program, you are probably losing.  */

#if defined (__STDC__) && __STDC__
/* Suppress -Wmissing-prototypes warning.  We don't want to declare this in
   obstack.h because it is just for debugging.  */
int _obstack_allocated_p (struct obstack *h, POINTER obj);
#endif

int
_obstack_allocated_p (h, obj)
     struct obstack *h;
     POINTER obj;
{
  register struct _obstack_chunk *lp;	/* below addr of any objects in this chunk */
  register struct _obstack_chunk *plp;	/* point to previous chunk if any */

  lp = (h)->chunk;
  /* We use >= rather than > since the object cannot be exactly at
     the beginning of the chunk but might be an empty object exactly
     at the end of an adjacent chunk.  */
  while (lp != 0 && ((POINTER) lp >= obj || (POINTER) (lp)->limit < obj))
    {
      plp = lp->prev;
      lp = plp;
    }
  return lp != 0;
}

/* Free objects in obstack H, including OBJ and everything allocate
   more recently than OBJ.  If OBJ is zero, free everything in H.  */

#undef obstack_free

/* This function has two names with identical definitions.
   This is the first one, called from non-ANSI code.  */

void
_obstack_free (h, obj)
     struct obstack *h;
     POINTER obj;
{
  register struct _obstack_chunk *lp;	/* below addr of any objects in this chunk */
  register struct _obstack_chunk *plp;	/* point to previous chunk if any */

  lp = h->chunk;
  /* We use >= because there cannot be an object at the beginning of a chunk.
     But there can be an empty object at that address
     at the end of another chunk.  */
  while (lp != 0 && ((POINTER) lp >= obj || (POINTER) (lp)->limit < obj))
    {
      plp = lp->prev;
      CALL_FREEFUN (h, lp);
      lp = plp;
      /* If we switch chunks, we can't tell whether the new current
	 chunk contains an empty object, so assume that it may.  */
      h->maybe_empty_object = 1;
    }
  if (lp)
    {
      h->object_base = h->next_free = (char *) (obj);
      h->chunk_limit = lp->limit;
      h->chunk = lp;
    }
  else if (obj != 0)
    /* obj is not in any of the chunks! */
    abort ();
}

/* This function is used from ANSI code.  */

void
obstack_free (h, obj)
     struct obstack *h;
     POINTER obj;
{
  register struct _obstack_chunk *lp;	/* below addr of any objects in this chunk */
  register struct _obstack_chunk *plp;	/* point to previous chunk if any */

  lp = h->chunk;
  /* We use >= because there cannot be an object at the beginning of a chunk.
     But there can be an empty object at that address
     at the end of another chunk.  */
  while (lp != 0 && ((POINTER) lp >= obj || (POINTER) (lp)->limit < obj))
    {
      plp = lp->prev;
      CALL_FREEFUN (h, lp);
      lp = plp;
      /* If we switch chunks, we can't tell whether the new current
	 chunk contains an empty object, so assume that it may.  */
      h->maybe_empty_object = 1;
    }
  if (lp)
    {
      h->object_base = h->next_free = (char *) (obj);
      h->chunk_limit = lp->limit;
      h->chunk = lp;
    }
  else if (obj != 0)
    /* obj is not in any of the chunks! */
    abort ();
}

int
_obstack_memory_used (h)
     struct obstack *h;
{
  register struct _obstack_chunk* lp;
  register int nbytes = 0;

  for (lp = h->chunk; lp != 0; lp = lp->prev)
    {
      nbytes += lp->limit - (char *) lp;
    }
  return nbytes;
}

/* Define the error handler.  */
static void
print_and_abort ()
{
  fputs ("memory exhausted\n", stderr);
  exit (obstack_exit_failure);
}
