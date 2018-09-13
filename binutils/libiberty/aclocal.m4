sinclude(../config/acx.m4)
sinclude(../config/no-executables.m4)
sinclude(../config/override.m4)
dnl sinclude(../config/picflag.m4)
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
