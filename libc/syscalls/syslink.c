/* connector for link */

#include <reent.h>

int link(char *old, char *new)
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _link_r(_REENT, old, new);
#else
    return _link(old, new);
#endif
}
