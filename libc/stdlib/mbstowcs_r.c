#include <stdlib.h>

size_t _mbstowcs_r(struct _reent *r, wchar_t *pwcs, const char *s, size_t n, int *state)
{
    wchar_t *ptr = pwcs;
    size_t max = n;
    char *t = (char *)s;
    int bytes;

    while (n > 0)
    {
        bytes = _mbtowc_r(r, ptr, t, MB_CUR_MAX, state);
        if (bytes == -1)
            return -1;
        else if (bytes == 0)
            return ptr - pwcs;
        t += bytes;
        ++ptr;
        --n;
    }

    return max;
}
