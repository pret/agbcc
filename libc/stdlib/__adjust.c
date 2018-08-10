/*
 * return (*acc) scaled by 10**dexp.
 */


#include <reent.h>
#include "std.h"

#define abs(x) (((x) < 0) ? -(x) : (x))

double __adjust(struct _reent *ptr, double *acc, int dexp, int sign)
/* *acc	the 64 bit accumulator */
/* dexp	decimal exponent       */
/* sign	sign flag   	       */
{
    double r;

    if (dexp > MAXE)
    {
        ptr->_errno = ERANGE;
        return (sign) ? -HUGE_VAL : HUGE_VAL;
    }
    else if (dexp < MINE)
    {
        ptr->_errno = ERANGE;
        return 0.0;
    }

    r = *acc;
    if (sign)
        r = -r;
    if (dexp == 0)
        return r;

    if (dexp < 0)
        return r / __exp10(abs(dexp));
    else
        return r * __exp10(dexp);
}
