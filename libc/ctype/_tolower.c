#include <ctype.h>

#undef _tolower
int _tolower(int c)
{
    return isupper(c) ? (c) - 'A' + 'a' : c;
}
