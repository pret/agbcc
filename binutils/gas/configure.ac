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
AC_INIT([gas], BFD_VERSION)
AC_CONFIG_SRCDIR(as.h)
m4_include([../config/lean.m4])

dnl Autoconf 2.57 will find the aux dir without this.  However, unless
dnl we specify this explicitly, automake-1.7 will assume that ylwrap is in
dnl gas/ instead of gas/../.
AC_CONFIG_AUX_DIR(..)
AC_CANONICAL_TARGET
AC_ISC_POSIX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_GNU_SOURCE
AC_USE_SYSTEM_EXTENSIONS

#LT_INIT
ACX_LARGEFILE
ACX_PROG_CMP_IGNORE_INITIAL

AC_ARG_ENABLE(targets,
[  --enable-targets        alternative target configurations besides the primary],
[case "${enableval}" in
  yes | "") AC_MSG_ERROR(enable-targets option must specify target names or 'all')
	    ;;
  no)	    enable_targets= ;;
  *)	    enable_targets=$enableval ;;
esac])dnl

ac_checking=yes
if grep '^RELEASE=y' ${srcdir}/../bfd/Makefile.am >/dev/null 2>/dev/null ; then
  ac_checking=
fi
AC_ARG_ENABLE(checking,
[  --enable-checking       enable run-time checks],
[case "${enableval}" in
  no|none)  ac_checking= ;;
  *)	    ac_checking=yes ;;
esac])dnl
if test x$ac_checking != x ; then
  AC_DEFINE(ENABLE_CHECKING, 1, [Define if you want run-time sanity checks.])
fi

# PR gas/19109
# Decide the default method for compressing debug sections.
ac_default_compressed_debug_sections=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(compressed_debug_sections,
	      AS_HELP_STRING([--enable-compressed-debug-sections={all,gas,none}],
	      [compress debug sections by default]),
[case ,"${enableval}", in
  ,yes, | ,all, | *,gas,*) ac_default_compressed_debug_sections=yes ;;
  ,no, | ,none,)  ac_default_compressed_debug_sections=no ;;
  *)   ac_default_compressed_debug_sections=unset ;;
esac])dnl

# PR gas/19520
# Decide if x86 assembler should generate relax relocations.
ac_default_x86_relax_relocations=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(x86_relax_relocations,
	      AS_HELP_STRING([--enable-x86-relax-relocations],
	      [generate x86 relax relocations by default]),
[case "${enableval}" in
  no)  ac_default_x86_relax_relocations=0 ;;
esac])dnl

# Decide if ELF assembler should generate common symbols with the
# STT_COMMON type.
ac_default_elf_stt_common=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(elf_stt_common,
	      AS_HELP_STRING([--enable-elf-stt-common],
	      [generate ELF common symbols with STT_COMMON type by default]),
[case "${enableval}" in
  yes)  ac_default_elf_stt_common=1 ;;
esac])dnl


# Decide if the ELF assembler should default to generating
# GNU Build notes if none are provided by the input.
ac_default_generate_build_notes=0
# Provide a configuration option to override the default.
AC_ARG_ENABLE(generate_build_notes,
	      AS_HELP_STRING([--enable-generate-build-notes],
	      [generate GNU Build notes if none are provided by the input]),
[case "${enableval}" in
  yes)  ac_default_generate_build_notes=1 ;;
  no)   ac_default_generate_build_notes=0 ;;
esac])dnl


using_cgen=no

AM_BINUTILS_WARNINGS

# Generate a header file
AC_CONFIG_HEADERS(config.h:config.in)

dnl Option --with-cpu=TYPE allows configure type control of the default
dnl cpu type within the assembler.  Currently only the ARC target
dnl supports this feature, but others may be added in the future.
AC_ARG_WITH(cpu,
            AS_HELP_STRING([--with-cpu=CPU],
            [default cpu variant is CPU (currently only supported on ARC)]),
            [AC_DEFINE_UNQUOTED(TARGET_WITH_CPU,
                                "${with_cpu}",
                                [Target specific CPU.])],
            [])

# PR 14072
AH_VERBATIM([00_CONFIG_H_CHECK],
[/* Check that config.h is #included before system headers
   (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
#  error config.h must be #included before system headers
#endif
#define __CONFIG_H__ 1])

# If we are on a DOS filesystem, we must use gdb.ini rather than
# .gdbinit.
case "${host}" in
  *-*-msdos* | *-*-go32* | *-*-mingw32* | *-*-windows*)
    GDBINIT="gdb.ini"
    AC_CONFIG_FILES(gdb.ini:gdbinit.in)
    ;;
  *)
    GDBINIT=".gdbinit"
    AC_CONFIG_FILES(.gdbinit:gdbinit.in)
    ;;
esac
AC_SUBST(GDBINIT)

#We need this for the host.
AC_C_BIGENDIAN

te_file=generic

# Makefile target for installing gas in $(tooldir)/bin.
install_tooldir=install-exec-tooldir

canon_targets=""
all_targets=no
if test -n "$enable_targets" ; then
  for t in `echo $enable_targets | sed 's/,/ /g'`; do
    if test $t = "all"; then
      all_targets=yes
      continue
    fi
    result=`$ac_config_sub $t 2>/dev/null`
    if test -n "$result" ; then
      canon_targets="$canon_targets $result"
#    else
#      # Permit "all", etc.  We don't support it yet though.
#      canon_targets="$canon_targets $t"
    fi
  done
  GAS_UNIQ(canon_targets)
fi

emulations=""

for this_target in $target $canon_targets ; do

    targ=${this_target}
    . ${srcdir}/configure.tgt

    case ${target_cpu} in
      crisv32)
	AC_DEFINE_UNQUOTED(DEFAULT_CRIS_ARCH, $arch,
			   [Default CRIS architecture.])
	;;
    esac

    if test ${this_target} = $target ; then
      target_cpu_type=${cpu_type}
    elif test ${target_cpu_type} != ${cpu_type} ; then
      continue
    fi

    generic_target=${cpu_type}-${target_vendor}-${target_os}
    case ${generic_target} in
      i386-*-msdosdjgpp* \
      | i386-*-go32* \
      | i386-go32-rtems*)
	AC_DEFINE(STRICTCOFF, 1, [Using strict COFF?])
	;;

      i386-*-solaris2 \
      | x86_64-*-solaris2 \
      | i386-*-solaris2.[[0-9]] \
      | i386-*-solaris2.1[[01]] \
      | x86_64-*-solaris2.1[[01]])
	if test ${this_target} = $target \
	   && test ${ac_default_x86_relax_relocations} = unset; then
	  ac_default_x86_relax_relocations=0
	fi
	;;

    esac

    if test ${this_target} = $target ; then
      endian_def=
      if test x${endian} = xbig; then
	endian_def=1
      elif test x${endian} = xlittle; then
	endian_def=0
      fi
      if test x${endian_def} != x; then
	AC_DEFINE_UNQUOTED(TARGET_BYTES_BIG_ENDIAN, $endian_def,
			   [Define as 1 if big endian.])
      fi
    fi

    if test $using_cgen = yes ; then
	f=cgen.o
	case " $extra_objects " in
	  *" $f "*) ;;
	  *) extra_objects="$extra_objects $f" ;;
	esac
    fi

# See if we really can support this configuration with the emulation code.

    if test $this_target = $target ; then
      obj_format=$fmt
      te_file=$em
    fi

    case ${te_file} in
      vms)
	f=config/te-vms.o
	case " $extra_objects " in
	  *" $f "*) ;;
	  *) extra_objects="$extra_objects $f" ;;
	esac ;;
    esac

# From target name and format, produce a list of supported emulations.

    emulations="$emulations $emulation"

done

if test ${ac_default_x86_relax_relocations} = unset; then
  ac_default_x86_relax_relocations=1
fi
AC_DEFINE_UNQUOTED(DEFAULT_GENERATE_X86_RELAX_RELOCATIONS,
  $ac_default_x86_relax_relocations,
  [Define to 1 if you want to generate x86 relax relocations by default.])

if test ${ac_default_elf_stt_common} = unset; then
  ac_default_elf_stt_common=0
fi
AC_DEFINE_UNQUOTED(DEFAULT_GENERATE_ELF_STT_COMMON,
  $ac_default_elf_stt_common,
  [Define to 1 if you want to generate ELF common symbols with the
   STT_COMMON type by default.])

AC_DEFINE_UNQUOTED(DEFAULT_GENERATE_BUILD_NOTES,
  $ac_default_generate_build_notes,
  [Define to 1 if you want to generate GNU Build attribute notes
   by default, if none are contained in the input.])

if test x$ac_default_compressed_debug_sections = xyes ; then
  AC_DEFINE(DEFAULT_FLAG_COMPRESS_DEBUG, 1, [Define if you want compressed debug sections by default.])
fi

# Turn on all targets if possible
if test ${all_targets} = "yes"; then
  case ${target_cpu_type} in
  i386)
    case ${obj_format} in
    aout)
      emulations="$emulations i386coff i386elf"
      ;;
    coff)
      emulations="$emulations i386aout i386elf"
    ;;
    elf)
      emulations="$emulations i386aout i386coff"
      ;;
    esac
  ;;
  x86_64)
    case ${obj_format} in
    aout)
      emulations="$emulations i386coff i386elf"
      ;;
    coff)
      emulations="$emulations i386aout i386elf"
    ;;
    elf)
      emulations="$emulations i386aout i386coff"
      ;;
    esac
  ;;
  esac
fi

# PE code has way too many macros tweaking behaviour
case ${te_file} in
  pe*) emulations="" ;;
esac

# Assign floating point type.  Most processors with FP support
# IEEE FP.  On those that don't support FP at all, usually IEEE
# is emulated.
case ${target_cpu} in
  vax | pdp11 )	atof=vax ;;
  *)		atof=ieee ;;
esac

case "${obj_format}" in
  "") AC_MSG_ERROR(GAS does not know what format to use for target ${target}) ;;
esac

# Unfortunately the cpu in cpu-opc.h file isn't always $(TARGET_CPU).
cgen_cpu_prefix=""
if test $using_cgen = yes ; then
  case ${target_cpu} in
    or1knd)
       cgen_cpu_prefix=or1k ;;
    *) cgen_cpu_prefix=${target_cpu} ;;
  esac
  AC_SUBST(cgen_cpu_prefix)
  AC_DEFINE(USING_CGEN, 1, [Using cgen code?])
fi

dnl
dnl Make sure the desired support files exist.
dnl

if test ! -r ${srcdir}/config/tc-${target_cpu_type}.c; then
  AC_MSG_ERROR(GAS does not support target CPU ${target_cpu_type})
fi

if test ! -r ${srcdir}/config/obj-${obj_format}.c; then
  AC_MSG_ERROR(GAS does not have support for object file format ${obj_format})
fi

# Some COFF configurations want these random other flags set.
case ${obj_format} in
  coff)
    case ${target_cpu_type} in
      i386) AC_DEFINE(I386COFF, 1, [Using i386 COFF?]) ;;
      x86_64) AC_DEFINE(I386COFF, 1, [Using i386 COFF?]) ;;
    esac
    ;;
esac

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
  case $em in
    .)	continue ;;
    mipsbelf | mipslelf | mipself)
	fmt=elf   file=mipself ;;
    *coff)
	fmt=coff  file=$em ;;
    *aout)
	fmt=aout file=$em ;;
    *elf)
	fmt=elf file=$em ;;
  esac
  formats="$formats $fmt"
  emfiles="$emfiles config/e-$file.o"
  EMULATIONS="$EMULATIONS &$em,"
done
GAS_UNIQ(formats)
GAS_UNIQ(emfiles)
if test `set . $formats ; shift ; echo $#` -gt 1 ; then
  for fmt in $formats ; do
    case $fmt in
      aout)	AC_DEFINE(OBJ_MAYBE_AOUT, 1,    [a.out support?])   ;;
      coff)	AC_DEFINE(OBJ_MAYBE_COFF, 1,    [COFF support?])    ;;
      ecoff)	AC_DEFINE(OBJ_MAYBE_ECOFF, 1,   [ECOFF support?])   ;;
      elf)	AC_DEFINE(OBJ_MAYBE_ELF, 1,     [ELF support?])     ;;
      generic)	AC_DEFINE(OBJ_MAYBE_GENERIC, 1, [generic support?]) ;;
      som)	AC_DEFINE(OBJ_MAYBE_SOM, 1,     [SOM support?])     ;;
    esac
    extra_objects="$extra_objects config/obj-$fmt.o"
  done
  obj_format=multi
fi
if test `set . $emfiles ; shift ; echo $#` -gt 0 ; then
  DEFAULT_EMULATION=`set . $emulations ; echo $2`
  # e-mipself has more than one emulation per file, e-i386* has just one at the
  # moment.  If only one emulation is specified, then don't define
  # USE_EMULATIONS or include any of the e-files as they will only be bloat.
  case "${obj_format}${emfiles}" in
    multi* | *mipself*)
      extra_objects="$extra_objects $emfiles"
      AC_DEFINE(USE_EMULATIONS, 1, [Use emulation support?]) ;;
  esac
fi
AC_SUBST(extra_objects)
AC_DEFINE_UNQUOTED(EMULATIONS, $EMULATIONS, [Supported emulations.])
AC_DEFINE_UNQUOTED(DEFAULT_EMULATION, "$DEFAULT_EMULATION",
		   [Default emulation.])

reject_dev_configs=yes

case ${reject_dev_configs}-${dev} in
  yes-yes) # Oops.
    AC_MSG_ERROR(GAS does not support the ${generic_target} configuration.)
    ;;
esac

AC_SUBST(target_cpu_type)
AC_SUBST(obj_format)
AC_SUBST(te_file)
AC_SUBST(install_tooldir)
AC_SUBST(atof)
dnl AC_SUBST(emulation)

AC_DEFINE_UNQUOTED(TARGET_ALIAS,	"${target_alias}", [Target alias.])
AC_DEFINE_UNQUOTED(TARGET_CANONICAL,	"${target}",       [Canonical target.])
AC_DEFINE_UNQUOTED(TARGET_CPU,		"${target_cpu}",   [Target CPU.])
AC_DEFINE_UNQUOTED(TARGET_VENDOR,	"${target_vendor}", [Target vendor.])
AC_DEFINE_UNQUOTED(TARGET_OS,		"${target_os}",    [Target OS.])

ALL_LINGUAS="es fi fr id ja ru rw sv tr uk zh_CN"
ZW_GNU_GETTEXT_SISTER_DIR
AM_PO_SUBDIRS

AM_MAINTAINER_MODE
AM_CONDITIONAL(GENINSRC_NEVER, false)
AC_EXEEXT

AC_CHECK_HEADERS(memory.h strings.h unistd.h errno.h sys/types.h locale.h time.h sys/stat.h)
ACX_HEADER_STRING

# Put this here so that autoconf's "cross-compiling" message doesn't confuse
# people who are not cross-compiling but are compiling cross-assemblers.
AC_MSG_CHECKING(whether compiling a cross-assembler)
if test "${host}" = "${target}"; then
  cross_gas=no
else
  cross_gas=yes
  AC_DEFINE(CROSS_COMPILE, 1, [Compiling cross-assembler?])
fi
AC_MSG_RESULT($cross_gas)

dnl ansidecl.h will deal with const
dnl AC_C_CONST
AC_FUNC_ALLOCA

# VMS doesn't have unlink.
AC_CHECK_FUNCS(unlink remove, break)
AC_CHECK_FUNCS(setlocale)
AC_CHECK_FUNCS(strsignal)

AM_LC_MESSAGES

# do we need the math library?
case "${need_libm}" in
yes)
  LT_LIB_M
  AC_SUBST(LIBM)
  ;;
esac

# Some non-ANSI preprocessors botch requoting inside strings.  That's bad
# enough, but on some of those systems, the assert macro relies on requoting
# working properly!
GAS_WORKING_ASSERT

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

# Does errno.h declare errno, or do we have to add a separate declaration
# for it?
GAS_CHECK_DECL_NEEDED(errno, f, int f, [
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
])

AC_MSG_CHECKING(for a known getopt prototype in unistd.h)
AC_CACHE_VAL(gas_cv_decl_getopt_unistd_h,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <unistd.h>], [extern int getopt (int, char *const*, const char *);])],
gas_cv_decl_getopt_unistd_h=yes, gas_cv_decl_getopt_unistd_h=no)])
AC_MSG_RESULT($gas_cv_decl_getopt_unistd_h)
if test $gas_cv_decl_getopt_unistd_h = yes; then
  AC_DEFINE([HAVE_DECL_GETOPT], 1,
	    [Is the prototype for getopt in <unistd.h> in the expected format?])
fi

GAS_CHECK_DECL_NEEDED(environ, f, char **f, $gas_test_headers)
GAS_CHECK_DECL_NEEDED(ffs, f, int (*f)(int), $gas_test_headers)

AC_CHECK_DECLS([getenv, mempcpy, stpcpy, asprintf])

BFD_BINARY_FOPEN

# Link in zlib if we can.  This allows us to write compressed debug sections.
AM_ZLIB

# Support for VMS timestamps via cross compile

if test "$ac_cv_header_time_h" = yes; then
  GAS_HAVE_TIME_TYPE_MEMBER(struct tm, tm_gmtoff)
fi

if test "$ac_cv_header_sys_stat_h" = yes; then
    GAS_HAVE_SYS_STAT_TYPE_MEMBER(struct stat, st_mtim, tv_sec)
    GAS_HAVE_SYS_STAT_TYPE_MEMBER(struct stat, st_mtim, tv_nsec)
fi


dnl Required for html, pdf, install-pdf and install-html targets.
AC_SUBST(datarootdir)
AC_SUBST(docdir)
AC_SUBST(htmldir)
AC_SUBST(pdfdir)

dnl This must come last.

dnl We used to make symlinks to files in the source directory, but now
dnl we just use the right name for .c files, and create .h files in
dnl the build directory which include the right .h file.  Make sure
dnl the old symlinks don't exist, so that a reconfigure in an existing
dnl directory behaves reasonably.

AC_CONFIG_FILES(Makefile)
AC_CONFIG_COMMANDS([default],
[rm -f targ-cpu.c targ-cpu.h obj-format.h obj-format.c targ-env.h atof-targ.c itbl-cpu.h
 echo '#include "tc-'"${target_cpu_type}"'.h"' > targ-cpu.h
 echo '#include "obj-'"${obj_format}"'.h"' > obj-format.h
 echo '#include "te-'"${te_file}"'.h"' > targ-env.h
 echo '#include "itbl-'"${target_cpu_type}"'.h"' > itbl-cpu.h
 if test "x$cgen_cpu_prefix" != x ; then
   echo '#include "opcodes/'"${cgen_cpu_prefix}"'-desc.h"' > cgen-desc.h
 fi],
[target_cpu_type=${target_cpu_type}
 cgen_cpu_prefix=${cgen_cpu_prefix}
 obj_format=${obj_format}
 te_file=${te_file}])

AC_OUTPUT
