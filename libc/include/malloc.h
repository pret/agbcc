/* malloc.h -- header file for memory routines.  */

#ifndef _INCLUDE_MALLOC_H_
#define _INCLUDE_MALLOC_H_


#include <sys/reent.h>

#define __need_size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This version of struct mallinfo must match the one in
   libc/stdlib/mallocr.c.  */

struct mallinfo
{
    int arena;    /* total space allocated from system */
    int ordblks;  /* number of non-inuse chunks */
    int smblks;   /* unused -- always zero */
    int hblks;    /* number of mmapped regions */
    int hblkhd;   /* total space in mmapped regions */
    int usmblks;  /* unused -- always zero */
    int fsmblks;  /* unused -- always zero */
    int uordblks; /* total allocated space */
    int fordblks; /* total non-inuse space */
    int keepcost; /* top-most, releasable (via malloc_trim) space */
};

/* The routines.  */

extern void *malloc(size_t);
extern void *_malloc_r(struct _reent *, size_t);

extern void free(void *);
extern void _free_r(struct _reent *, void *);

extern void *realloc(void *, size_t);
extern void *_realloc_r(struct _reent *, void *, size_t);

extern void *calloc(size_t, size_t);
extern void *_calloc_r(struct _reent *, size_t, size_t);

extern void *memalign(size_t, size_t);
extern void *_memalign_r(struct _reent *, size_t, size_t);

extern struct mallinfo mallinfo(void);
extern struct mallinfo _mallinfo_r(struct _reent *);

extern void malloc_stats(void);
extern void _malloc_stats_r(struct _reent *);

extern int mallopt(int, int);
extern int _mallopt_r(struct _reent *, int, int);

extern size_t malloc_usable_size(void *);
extern size_t _malloc_usable_size_r(struct _reent *, void *);

/* These aren't too useful on an embedded system, but we define them
   anyhow.  */

extern void *valloc(size_t);
extern void *_valloc_r(struct _reent *, size_t);

extern void *pvalloc(size_t);
extern void *_pvalloc_r(struct _reent *, size_t);

extern int malloc_trim(size_t);
extern int _malloc_trim_r(struct _reent *, size_t);

/* Some systems provide this, so do too for compatibility.  */

extern void cfree(void *);

/* A compatibility routine for an earlier version of the allocator.  */

extern void mstats(char *);
extern void _mstats_r(struct _reent *, char *);

#ifdef __CYGWIN32__

/* Cygwin32 needs to be able to copy all the malloc information from
   the parent to the child.  However, cygwin32 does not normally copy
   any data in the DLL data section.  This routine handles copying
   that information.  */

extern int __malloc_copy(int (*)(void *, void *, void *, int), void *, int);
#endif /* __CYGWIN32 */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_MALLOC_H_ */
