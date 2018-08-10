#include <stdlib.h>

size_t _wcstombs_r(struct _reent *r, char *s, const wchar_t *pwcs, size_t n, int *state)
{
    char *ptr = s;
    size_t max = n;
    char buff[8];
    int i, num_to_copy;

    while (n > 0)
    {
        int bytes = _wctomb_r(r, buff, *pwcs, state);
        if (bytes == -1)
            return -1;
        num_to_copy = (n > bytes ? bytes : (int)n);
        for (i = 0; i < num_to_copy; ++i)
            *ptr++ = buff[i];

        if (*pwcs == 0x00)
            return ptr - s - (n >= bytes);
        ++pwcs;
        n -= num_to_copy;
    }

    return max;
}
