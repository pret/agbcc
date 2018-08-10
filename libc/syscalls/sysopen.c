/* connector for open */

#include <reent.h>
#include <fcntl.h>

/* The prototype in <fcntl.h> uses ..., so we must correspond.  */

#include <stdarg.h>

int open(const char *file, int flags, ...)
{
    va_list ap;
    int ret;

    va_start(ap, flags);
#ifdef REENTRANT_SYSCALLS_PROVIDED
    ret = _open_r(_REENT, file, flags, va_arg(ap, int));
#else
    ret = _open(file, flags, va_arg(ap, int));
#endif
    va_end(ap);
    return ret;
}
