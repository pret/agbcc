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
target=armv4tl-none-eabi
target_alias=armv4tl-none-eabi
ac_cv_target=armv4tl-none-eabi
ac_cv_target_alias=armv4tl-none-eabi

m4_include([version.m4])
AC_INIT([bfd], [BFD_VERSION])
m4_include([../config/lean.m4])
AC_CONFIG_AUX_DIR([../gnu_aux])
AC_CONFIG_SRCDIR([libbfd.c])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

dnl## These must be called before Libtool, because it may want
dnl## to call AC_CHECK_Prog
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_PROG_SED

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_SYS_LARGEFILE

enable_targets=

AC_ARG_WITH(mmap,
[  --with-mmap             try using mmap for BFD input files if available],
[case "${withval}" in
  yes)  want_mmap=true ;;
  no)   want_mmap=false ;;
  *)    AC_MSG_ERROR(bad value ${withval} for BFD with-mmap option) ;;
esac],[want_mmap=false])dnl

ACX_PKGVERSION([GNU Binutils])
ACX_BUGURL([http://www.sourceware.org/bugzilla/])

AC_CONFIG_HEADERS(config.h:config.in)

if test -z "$target" ; then
    AC_MSG_ERROR(Unrecognized target system type; please check config.sub.)
fi

AC_EXEEXT

host64=false
target64=false
bfd_default_target_size=32

# host stuff:

# Permit host specific settings.
. ${srcdir}/configure.host

AC_SUBST(HDEFINES)
AC_PROG_INSTALL

BFD_HOST_64_BIT_DEFINED=0
BFD_HOST_64_BIT=
BFD_HOST_U_64_BIT=
BFD_HOSTPTR_T="unsigned long"

AC_TYPE_LONG_DOUBLE
#AC_CHECK_SIZEOF(void *)

#if test "x${ac_cv_sizeof_void_p}" = "x8"; then
#  host64=true
#  BFD_HOST_64_BIT_DEFINED=1
#  BFD_HOSTPTR_T="unsigned long long"
#fi

test -n "${HOST_64BIT_TYPE}" || HOST_64BIT_TYPE="long long"
test -n "${HOST_U_64BIT_TYPE}" || HOST_U_64BIT_TYPE="unsigned long long"

AC_SUBST(BFD_HOST_64_BIT_DEFINED)
AC_SUBST(BFD_HOST_64_BIT)
AC_SUBST(BFD_HOST_U_64_BIT)
AC_SUBST(BFD_HOSTPTR_T)

BFD_CC_FOR_BUILD

AC_CHECK_HEADERS_ONCE([fcntl.h strings.h sys/file.h sys/time.h unistd.h])

ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([getpagesize mmap munmap strcasecmp strncasecmp ])
AC_FUNC_FSEEKO

AC_CHECK_DECLS(ftello)
AC_CHECK_DECLS(ftello64)

AC_CHECK_DECLS(asprintf)

# When building a shared libbfd, link against the pic version of libiberty
# so that apps that use libbfd won't need libiberty just to satisfy any
# libbfd references.
# We can't do that if a pic libiberty is unavailable since including non-pic
# code would insert text relocations into libbfd.

all_targets=false
defvec=
selvecs=
assocvecs=
TDEFINES=
for targ in $target $canon_targets
do
	. $srcdir/config.bfd
	if test "x$targ" = "x$target"; then
	    defvec=$targ_defvec
	fi
	selvecs="$selvecs $targ_defvec $targ_selvecs"
	TDEFINES="$TDEFINES $targ_cflags"
done
AC_SUBST(TDEFINES)

# This processing still needs to be done if we're to decide properly whether
# 64-bit support needs to be compiled in.  Currently, it will be included if
# the default or any other explicitly requested target requires it; it
# will not be included on a 32-bit host if no 64-bit target is requested, and
# no "--with-64-bit-bfd" option is given, even if "--enable-targets=all" is
# used.

# uniq the default and selected vectors in all the configured targets.
f=""
for i in $selvecs ; do
    case " $f " in
    *" $i "*) ;;
    *) f="$f $i" ;;
    esac
done
selvecs="$f"


# uniq the associated vectors in all the configured targets.
f=""
for i in $assocvecs ; do
    case " $f " in
    *" $i "*) ;;
    *) f="$f $i" ;;
    esac
done
assocvecs="$f"

elf="elf.o elflink.o elf-attrs.o elf-strtab.o elf-properties.o
     elf-eh-frame.o dwarf1.o dwarf2.o"


# Target architecture .o files.
# A couple of CPUs use shorter file names to avoid problems on DOS
# filesystems.

  # Only set these if they will be nonempty, for the clever echo.

AC_SUBST(bfd_default_target_size)

# Determine the host dependent file_ptr a.k.a. off_t type.  In order
# prefer: off64_t - if ftello64 and fseeko64, off_t - if ftello and
# fseeko, long.  This assumes that sizeof off_t is .ge. sizeof long.
# Hopefully a reasonable assumption since fseeko et.al. should be
# upward compatible.
if test x"$ac_cv_func_ftello" = xyes -a x"$ac_cv_func_fseeko" = xyes; then
    AC_CHECK_SIZEOF(off_t)
fi
AC_MSG_CHECKING([file_ptr type])
bfd_file_ptr="long"
bfd_ufile_ptr="unsigned long"
if test x"$ac_cv_func_ftello64" = xyes -a x"$ac_cv_func_fseeko64" = xyes \
	-o x"${ac_cv_sizeof_off_t}" = x8; then
    bfd_file_ptr=BFD_HOST_64_BIT
    bfd_ufile_ptr=BFD_HOST_U_64_BIT
fi
AC_MSG_RESULT($bfd_file_ptr)
AC_SUBST(bfd_file_ptr)
AC_SUBST(bfd_ufile_ptr)

if test x"$ac_cv_func_mmap" = xyes; then
  AC_DEFINE(USE_MMAP, 1, [Use mmap if it's available?])
fi

rm -f doc/config.status
AC_CONFIG_FILES([Makefile bfd-in3.h:bfd-in2.h])

AC_OUTPUT
