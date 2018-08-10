/*
 * gmtime_r.c
 */

#include <time.h>

#define _GMT_OFFSET 0

struct tm *gmtime_r(const time_t *tim_p, struct tm *res)
{
    time_t tim = *tim_p + _GMT_OFFSET;

    return (localtime_r(&tim, res));
}
