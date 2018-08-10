

#include <stdio.h>
#include <stdarg.h>

int _printf_r(struct _reent *ptr, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = _vfprintf_r(ptr, _stdout_r(ptr), fmt, ap);
    va_end(ap);
    return ret;
}
#ifndef _REENT_ONLY
#include <stdarg.h>

int printf(const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    _stdout_r(_REENT)->_data = _REENT;
    ret = vfprintf(_stdout_r(_REENT), fmt, ap);
    va_end(ap);
    return ret;
}
#endif /* ! _REENT_ONLY */
