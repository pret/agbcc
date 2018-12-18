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
AC_CONFIG_AUX_DIR([../gnu_aux])
m4_include([../config/lean.m4])

AC_CONFIG_SRCDIR(ar.c)

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_SYS_LARGEFILE

AC_CONFIG_HEADERS(config.h:config.in)

if test -z "$target" ; then
    AC_MSG_ERROR(Unrecognized target system type; please check config.sub.)
fi
if test -z "$host" ; then
    AC_MSG_ERROR(Unrecognized host system type; please check config.sub.)
fi

AC_EXEEXT

# host-specific stuff:

HDEFINES=

. ${srcdir}/../bfd/configure.host

AC_SUBST(HDEFINES)
AC_PROG_INSTALL

BFD_CC_FOR_BUILD

AC_CHECK_SIZEOF([long])
AC_CHECK_HEADERS_ONCE([fcntl.h malloc.h strings.h sys/param.h sys/time.h unistd.h utime.h])

ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([getpagesize mkdir rmdir stpcpy utime])

AC_CHECK_FUNC([mkstemp],
	      AC_DEFINE([HAVE_MKSTEMP], 1,
	      [Define to 1 if you have the `mkstemp' function.]))
AC_CHECK_FUNC([mkdtemp],
              AC_DEFINE([HAVE_MKDTEMP], 1,
              [Define to 1 if you have the `mkdtemp' function.]))

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

AC_CHECK_DECLS([stpcpy])


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

AC_SUBST(BUILD_INSTALL_MISC)
AC_SUBST(OBJDUMP_DEFS)
AC_SUBST(OBJDUMP_PRIVATE_OFILES)

AC_DEFINE_UNQUOTED(TARGET, "${target}", [Configured target name.])

targ=$target
. $srcdir/../bfd/config.bfd

# Emulation
targ=$target
. ${srcdir}/configure.tgt
EMULATION=$targ_emul
EMULATION_VECTOR=$targ_emul_vector

AC_SUBST(EMULATION)
AC_SUBST(EMULATION_VECTOR)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
