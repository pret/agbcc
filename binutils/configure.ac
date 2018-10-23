dnl Process this file with autoconf to produce a configure script.
dnl
dnl   Copyright (C) 2012-2018 Free Software Foundation, Inc.
dnl
dnl This file is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; see the file COPYING3.  If not see
dnl <http://www.gnu.org/licenses/>.
dnl

m4_include([../bfd/version.m4])
AC_INIT([binutils], BFD_VERSION)
AC_CONFIG_AUX_DIR([../aux])
m4_include([../config/lean.m4])

AC_CONFIG_SRCDIR(ar.c)

AC_CANONICAL_TARGET
AC_ISC_POSIX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

ACX_LARGEFILE

AC_CONFIG_HEADERS(config.h:config.in)

if test -z "$target" ; then
    AC_MSG_ERROR(Unrecognized target system type; please check config.sub.)
fi
if test -z "$host" ; then
    AC_MSG_ERROR(Unrecognized host system type; please check config.sub.)
fi

AM_CONDITIONAL(GENINSRC_NEVER, false)
AC_EXEEXT
if test -n "$EXEEXT"; then
  AC_DEFINE(HAVE_EXECUTABLE_SUFFIX, 1,
	    [Does the platform use an executable suffix?])
fi
AC_DEFINE_UNQUOTED(EXECUTABLE_SUFFIX, "${EXEEXT}",
		   [Suffix used for executables, if any.])

# host-specific stuff:

HDEFINES=

. ${srcdir}/../bfd/configure.host

AC_SUBST(HDEFINES)
AR=${AR-ar}
AC_SUBST(AR)
AC_PROG_INSTALL

BFD_CC_FOR_BUILD

AC_CHECK_SIZEOF([long])
AC_CHECK_HEADERS_ONCE([fcntl.h malloc.h strings.h sys/file.h sys/param.h sys/time.h unistd.h utime.h])

AC_HEADER_SYS_WAIT
ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([getpagesize mkdir rmdir stpcpy strcasecmp strncasecmp utime])

AC_CHECK_FUNC([mkstemp],
	      AC_DEFINE([HAVE_MKSTEMP], 1,
	      [Define to 1 if you have the `mkstemp' function.]))
AC_CHECK_FUNC([mkdtemp],
              AC_DEFINE([HAVE_MKDTEMP], 1,
              [Define to 1 if you have the `mkdtemp' function.]))

AC_MSG_CHECKING(for time_t in time.h)
AC_CACHE_VAL(bu_cv_decl_time_t_time_h,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <time.h>], [time_t i;])],
bu_cv_decl_time_t_time_h=yes, bu_cv_decl_time_t_time_h=no)])
AC_MSG_RESULT($bu_cv_decl_time_t_time_h)
if test $bu_cv_decl_time_t_time_h = yes; then
  AC_DEFINE([HAVE_TIME_T_IN_TIME_H], 1,
	    [Is the type time_t defined in <time.h>?])
else
  AC_MSG_CHECKING(for time_t in sys/types.h)
  AC_CACHE_VAL(bu_cv_decl_time_t_types_h,
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <sys/types.h>], [time_t i;])],
  bu_cv_decl_time_t_types_h=yes, bu_cv_decl_time_t_types_h=no)])
  AC_MSG_RESULT($bu_cv_decl_time_t_types_h)
  if test $bu_cv_decl_time_t_types_h = yes; then
    AC_DEFINE([HAVE_TIME_T_IN_TYPES_H], 1,
	      [Is the type time_t defined in <sys/types.h>?])
  fi
fi

AC_MSG_CHECKING(for a known getopt prototype in unistd.h)
AC_CACHE_VAL(bu_cv_decl_getopt_unistd_h,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <unistd.h>], [extern int getopt (int, char *const*, const char *);])],
bu_cv_decl_getopt_unistd_h=yes, bu_cv_decl_getopt_unistd_h=no)])
AC_MSG_RESULT($bu_cv_decl_getopt_unistd_h)
if test $bu_cv_decl_getopt_unistd_h = yes; then
  AC_DEFINE([HAVE_DECL_GETOPT], 1,
	    [Is the prototype for getopt in <unistd.h> in the expected format?])
fi

# Under Next 3.2 <utime.h> apparently does not define struct utimbuf
# by default.
AC_MSG_CHECKING([for utime.h])
AC_CACHE_VAL(bu_cv_header_utime_h,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <sys/types.h>
#include <time.h>
#include <utime.h>],
[struct utimbuf s;])],
bu_cv_header_utime_h=yes, bu_cv_header_utime_h=no)])
AC_MSG_RESULT($bu_cv_header_utime_h)
if test $bu_cv_header_utime_h = yes; then
  AC_DEFINE(HAVE_GOOD_UTIME_H, 1, [Does <utime.h> define struct utimbuf?])
fi

AC_CHECK_DECLS([asprintf, environ, getc_unlocked, stpcpy])


all_targets=false
OBJDUMP_DEFS=
OBJDUMP_PRIVATE_VECTORS=
OBJDUMP_PRIVATE_OFILES=
od_vectors=

# Uniq objdump private vector, build objdump target ofiles.
od_files=
f=""
for i in $od_vectors ; do
    case " $f " in
    *" $i "*) ;;
    *)
	f="$f $i"
	OBJDUMP_PRIVATE_VECTORS="$OBJDUMP_PRIVATE_VECTORS &$i,"

	AC_MSG_ERROR(*** unknown private vector $i)
	;;
    esac
done

# Uniq objdump target ofiles
f=""
for i in $od_files ; do
    case " $f " in
    *" $i "*) ;;
    *)
	f="$f $i"
	OBJDUMP_PRIVATE_OFILES="$OBJDUMP_PRIVATE_OFILES $i.$objext"
	;;
    esac
done


OBJDUMP_DEFS="${OBJDUMP_DEFS} -DOBJDUMP_PRIVATE_VECTORS=\"${OBJDUMP_PRIVATE_VECTORS}\""

AC_SUBST(BUILD_MISC)
AC_SUBST(BUILD_INSTALL_MISC)
AC_SUBST(OBJDUMP_DEFS)
AC_SUBST(OBJDUMP_PRIVATE_OFILES)

AC_DEFINE_UNQUOTED(TARGET, "${target}", [Configured target name.])

targ=$target
. $srcdir/../bfd/config.bfd
if test "x$targ_underscore" = "xyes"; then
    UNDERSCORE=1
else
    UNDERSCORE=0
fi
AC_DEFINE_UNQUOTED(TARGET_PREPENDS_UNDERSCORE, $UNDERSCORE,
 [Define to 1 if user symbol names have a leading underscore, 0 if not.])

# Emulation
targ=$target
. ${srcdir}/configure.tgt
EMULATION=$targ_emul
EMULATION_VECTOR=$targ_emul_vector

AC_SUBST(EMULATION)
AC_SUBST(EMULATION_VECTOR)

# Required for html and install-html
AC_SUBST(datarootdir)
AC_SUBST(docdir)
AC_SUBST(htmldir)
AC_SUBST(pdfdir)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
