dnl Process this file with autoconf to produce a configure script.
dnl
dnl And be careful when changing it!  If you must add tests with square
dnl brackets, be sure changequote invocations surround it.
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
dnl v2.5 needed for --bindir et al
m4_include([../bfd/version.m4])
AC_INIT([gas],[BFD_VERSION])
AC_CONFIG_AUX_DIR([../gnu_aux])
AC_CONFIG_SRCDIR(as.h)
m4_include([../config/lean.m4])

AC_CANONICAL_TARGET

AC_CONFIG_MACRO_DIRS([../config])
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

AC_SYS_LARGEFILE

# Generate a header file
AC_CONFIG_HEADERS(config.h:config.in)

#We need this for the host.
AC_C_BIGENDIAN

te_file=generic

# Makefile target for installing gas in $(tooldir)/bin.
install_tooldir=install-exec-tooldir

all_targets=no

targ=armv4tl-none-eabi

cpu=armv4tl
cpu_type=arm
em=armeabi
endian=little
fmt=elf
generic_target=arm-none-eabi
os=eabi
vendor=none
obj_format=elf
te_file=armeabi

# Getting this done right is going to be a bitch.  Each configuration specified
# with --enable-targets=... should be checked for environment, format, cpu
# setting.
#
# For each configuration, the necessary object file support code must be linked
# in.  This might be only one, it might be up to four.  The necessary emulation
# code needs to be provided, too.
#
# And then there's "--enable-targets=all"....
#
# For now, just always do it for MIPS ELF configurations.  Sigh.

formats="${obj_format}"
emfiles=""
EMULATIONS=""
GAS_UNIQ(emulations)
for em in . $emulations ; do
  fmt=elf
  file=$em
  formats="$formats $fmt"
  emfiles="$emfiles config/e-$file.o"
  EMULATIONS="$EMULATIONS &$em,"
done
GAS_UNIQ(formats)
GAS_UNIQ(emfiles)
if test `set . $formats ; shift ; echo $#` -gt 1 ; then
  for fmt in $formats ; do
    case $fmt in
      elf)	AC_DEFINE(OBJ_MAYBE_ELF, 1,     [ELF support?])     ;;
      generic)	AC_DEFINE(OBJ_MAYBE_GENERIC, 1, [generic support?]) ;;
    esac
    extra_objects="$extra_objects config/obj-$fmt.o"
  done
  obj_format=multi
fi
if test `set . $emfiles ; shift ; echo $#` -gt 0 ; then
  DEFAULT_EMULATION=`set . $emulations ; echo $2`
fi
AC_SUBST(extra_objects)
AC_DEFINE_UNQUOTED(EMULATIONS, $EMULATIONS, [Supported emulations.])

AC_SUBST(te_file)
AC_SUBST(install_tooldir)

AC_DEFINE_UNQUOTED(TARGET_CPU,		"${target_cpu}",   [Target CPU.])
AC_DEFINE_UNQUOTED(TARGET_VENDOR,	"${target_vendor}", [Target vendor.])
AC_DEFINE_UNQUOTED(TARGET_OS,		"${target_os}",    [Target OS.])

AC_EXEEXT

AC_CHECK_HEADERS_ONCE(memory.h strings.h unistd.h sys/types.h sys/stat.h)
ACX_HEADER_STRING


# VMS doesn't have unlink.
AC_CHECK_FUNCS(unlink remove, break)

# On some systems, the system header files may not declare malloc, realloc,
# and free.  There are places where gas needs these functions to have been
# declared -- such as when taking their addresses.
gas_test_headers="
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#include <string.h>
#endif
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
"

dnl This must come last.

dnl We used to make symlinks to files in the source directory, but now
dnl we just use the right name for .c files, and create .h files in
dnl the build directory which include the right .h file.  Make sure
dnl the old symlinks don't exist, so that a reconfigure in an existing
dnl directory behaves reasonably.

AC_CONFIG_FILES(Makefile)
AC_CONFIG_COMMANDS([default],
[rm -f targ-cpu.c targ-cpu.h obj-format.h obj-format.c targ-env.h atof-targ.c],
[])

AC_OUTPUT
