/*
 * isinff(x) returns 1 if x is infinity, else 0;
 * no branching!
 * Added by Cygnus Support.
 */

#include "fdlibm.h"

int isinff(float x)
{
	__int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	ix = 0x7f800000 - ix;
	return 1 - (int)((__uint32_t)(ix|(-ix))>>31);
}

#ifdef _DOUBLE_IS_32BITS

int isinf(double x)
{
	return isinff((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
