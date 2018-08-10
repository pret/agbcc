/* connector for sbrk */

#include <reent.h>
#include <unistd.h>

extern void *_sbrk_r(struct _reent *, size_t);
extern void *_sbrk(size_t);

void *sbrk(size_t incr)
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _sbrk_r(_REENT, incr);
#else
    return _sbrk(incr);
#endif
}
