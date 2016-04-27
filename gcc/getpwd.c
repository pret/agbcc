/* getpwd.c - get the working directory */

#include "config.h"
#include "system.h"

#if !( defined(_WIN32) && !defined(__CYGWIN__))

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Get the working directory.  Use the PWD environment variable if it's
   set correctly, since this is faster and gives more uniform answers
   to the user.  Yield the working directory if successful; otherwise,
   yield 0 and set errno.  */

char *
getpwd ()
{
  static char *pwd;
  static int failure_errno;

  char *p = pwd;
  size_t guessed_len;
  struct stat dotstat, pwdstat;

  if (!p && !(errno = failure_errno))
    {
      int env_ok = ((p = getenv ("PWD")) != 0
	     && *p == '/'
	     && stat (p, &pwdstat) == 0
	     && stat (".", &dotstat) == 0
	     && dotstat.st_ino == pwdstat.st_ino
	     && dotstat.st_dev == pwdstat.st_dev);

	if (!env_ok)
    {
        /* The shortcut didn't work.  Try the slow, ``sure'' way.  */
	for (guessed_len = 256; !getcwd(p = xmalloc (guessed_len), guessed_len); guessed_len *= 2)
	  {
	    int e = errno;
	    free (p);
	    if (e != ERANGE)
	      {
		errno = failure_errno = e;
		p = 0;
		break;
	      }
	  }
    }

      /* Cache the result.  This assumes that the program does
	 not invoke chdir between calls to getpwd.  */
      pwd = p;
    }
  return p;
}

#else	/* _WIN32 && !__CYGWIN__ */

#include <direct.h>

char *
getpwd ()
{
  static char *pwd = 0;

  if (!pwd)
    pwd = _getcwd(NULL, 0);
  return pwd;
}

#endif	/* _WIN32 && !__CYGWIN__ */
