/* connector for getpid */

#include <reent.h>

int getpid(void)
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _getpid_r(_REENT);
#else
    return _getpid();
#endif
}
