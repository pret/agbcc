#include <ctype.h>

#undef _toupper
int _toupper(int c)
{
    return islower(c) ? c - 'a' + 'A' : c;
}
