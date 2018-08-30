sinclude(../config/acx.m4)
sinclude(../config/no-executables.m4)
sinclude(../config/override.m4)
sinclude(../config/picflag.m4)
sinclude(../config/warnings.m4)


dnl See if errno must be declared even when <errno.h> is included.
AC_DEFUN(libiberty_AC_DECLARE_ERRNO,
[AC_CACHE_CHECK(whether errno must be declared, libiberty_cv_declare_errno,
[AC_TRY_COMPILE(
[#include <errno.h>],
[int x = errno;],
libiberty_cv_declare_errno=no,
libiberty_cv_declare_errno=yes)])
if test $libiberty_cv_declare_errno = yes
then AC_DEFINE(NEED_DECLARATION_ERRNO, 1,
  [Define if errno must be declared even when <errno.h> is included.])
fi
])

dnl See whether we need a declaration for a function.
AC_DEFUN(libiberty_NEED_DECLARATION,
[AC_MSG_CHECKING([whether $1 must be declared])
AC_CACHE_VAL(libiberty_cv_decl_needed_$1,
[AC_TRY_COMPILE([
#include "confdefs.h"
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif],
[char *(*pfn) = (char *(*)) $1],
libiberty_cv_decl_needed_$1=no, libiberty_cv_decl_needed_$1=yes)])
AC_MSG_RESULT($libiberty_cv_decl_needed_$1)
if test $libiberty_cv_decl_needed_$1 = yes; then
  AC_DEFINE([NEED_DECLARATION_]translit($1, [a-z], [A-Z]), 1,
            [Define if $1 is not declared in system header files.])
fi
])dnl

# We always want a C version of alloca() compiled into libiberty,
# because native-compiler support for the real alloca is so !@#$%
# unreliable that GCC has decided to use it only when being compiled
# by GCC.  This is the part of AC_FUNC_ALLOCA that calculates the
# information alloca.c needs.
AC_DEFUN(libiberty_AC_FUNC_C_ALLOCA,
[AC_CACHE_CHECK(whether alloca needs Cray hooks, ac_cv_os_cray,
[AC_EGREP_CPP(webecray,
[#if defined(CRAY) && ! defined(CRAY2)
webecray
#else
wenotbecray
#endif
], ac_cv_os_cray=yes, ac_cv_os_cray=no)])
if test $ac_cv_os_cray = yes; then
  for ac_func in _getb67 GETB67 getb67; do
    AC_CHECK_FUNC($ac_func, 
      [AC_DEFINE_UNQUOTED(CRAY_STACKSEG_END, $ac_func, 
  [Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP
   systems. This function is required for alloca.c support on those
   systems.])  break])
  done
fi

AC_CACHE_CHECK(stack direction for C alloca, ac_cv_c_stack_direction,
[AC_TRY_RUN([find_stack_direction ()
{
  static char *addr = 0;
  auto char dummy;
  if (addr == 0)
    {
      addr = &dummy;
      return find_stack_direction ();
    }
  else
    return (&dummy > addr) ? 1 : -1;
}
main ()
{
  exit (find_stack_direction() < 0);
}], 
  ac_cv_c_stack_direction=1,
  ac_cv_c_stack_direction=-1,
  ac_cv_c_stack_direction=0)])
AC_DEFINE_UNQUOTED(STACK_DIRECTION, $ac_cv_c_stack_direction,
  [Define if you know the direction of stack growth for your system;
   otherwise it will be automatically deduced at run-time.
        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown])
])
