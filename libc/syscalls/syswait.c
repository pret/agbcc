/* connector for wait */

#include <reent.h>

int wait(int *status)
{
#ifdef REENTRANT_SYSCALLS_PROVIDED
    return _wait_r(_REENT, status);
#else
    return _wait(status);
#endif
}
