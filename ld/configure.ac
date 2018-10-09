dnl Process this file with autoconf to produce a configure script
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
AC_INIT([ld],[BFD_VERSION])
AC_CONFIG_AUX_DIR([../aux])
AC_CONFIG_SRCDIR(ldmain.c)
m4_include([../config/lean.m4])

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_SEARCH_LIBS([strerror],[cposix])

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_INSTALL

ACX_LARGEFILE

AC_ARG_WITH(lib-path, [  --with-lib-path=dir1:dir2...  set default LIB_PATH],LIB_PATH=$withval)
AC_ARG_ENABLE(64-bit-bfd,
[  --enable-64-bit-bfd     64-bit support (on hosts with narrower word sizes)],
[case "${enableval}" in
  yes)  want64=true  ;;
  no)   want64=false ;;
  *)    AC_MSG_ERROR(bad value ${enableval} for 64-bit-bfd option) ;;
esac],[want64=false])dnl

AC_ARG_WITH(sysroot,
[  --with-sysroot[=DIR] Search for usr/lib et al within DIR.],
[
 case ${with_sysroot} in
 yes) TARGET_SYSTEM_ROOT='${exec_prefix}/${target_alias}/sys-root' ;;
 *) TARGET_SYSTEM_ROOT=$with_sysroot ;;
 esac

 TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"$(TARGET_SYSTEM_ROOT)\"'
 use_sysroot=yes

 if test "x$prefix" = xNONE; then
  test_prefix=/usr/local
 else
  test_prefix=$prefix
 fi
 if test "x$exec_prefix" = xNONE; then
  test_exec_prefix=$test_prefix
 else
  test_exec_prefix=$exec_prefix
 fi
 case ${TARGET_SYSTEM_ROOT} in
 "${test_prefix}"|"${test_prefix}/"*|\
 "${test_exec_prefix}"|"${test_exec_prefix}/"*|\
 '${prefix}'|'${prefix}/'*|\
 '${exec_prefix}'|'${exec_prefix}/'*)
   t="$TARGET_SYSTEM_ROOT_DEFINE -DTARGET_SYSTEM_ROOT_RELOCATABLE"
   TARGET_SYSTEM_ROOT_DEFINE="$t"
   ;;
 esac
], [
 use_sysroot=no
 TARGET_SYSTEM_ROOT=
 TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"\"'
])
AC_SUBST(use_sysroot)
AC_SUBST(TARGET_SYSTEM_ROOT)
AC_SUBST(TARGET_SYSTEM_ROOT_DEFINE)

dnl Use --enable-gold to decide if this linker should be the default.
dnl "install_as_default" is set to false if gold is the default linker.
dnl "installed_linker" is the installed BFD linker name.
AC_ARG_ENABLE(gold,
[[  --enable-gold[=ARG]     build gold [ARG={default,yes,no}]]],
[case "${enableval}" in
 default)
   install_as_default=no
   installed_linker=ld.bfd
   ;;
 yes|no)
   install_as_default=yes
   installed_linker=ld.bfd
   ;;
 *)
   AC_MSG_ERROR([invalid --enable-gold argument])
   ;;
 esac],
[install_as_default=yes
 installed_linker=ld.bfd])
AC_SUBST(install_as_default)
AC_SUBST(installed_linker)

AC_ARG_ENABLE([got],
AS_HELP_STRING([--enable-got=<type>],
	       [GOT handling scheme (target, single, negative, multigot)]),
[case "${enableval}" in
  target | single | negative | multigot)  got_handling=$enableval ;;
  *)  AC_MSG_ERROR(bad value ${enableval} for --enable-got option) ;;
esac],
[got_handling=target])

case "${got_handling}" in
  target)
    AC_DEFINE([GOT_HANDLING_DEFAULT], [GOT_HANDLING_TARGET_DEFAULT],
	      [Define to choose default GOT handling scheme]) ;;
  single)
    AC_DEFINE([GOT_HANDLING_DEFAULT], [GOT_HANDLING_SINGLE],
	      [Define to choose default GOT handling scheme]) ;;
  negative)
    AC_DEFINE([GOT_HANDLING_DEFAULT], [GOT_HANDLING_NEGATIVE],
	      [Define to choose default GOT handling scheme]) ;;
  multigot)
    AC_DEFINE([GOT_HANDLING_DEFAULT], [GOT_HANDLING_MULTIGOT],
	      [Define to choose default GOT handling scheme]) ;;
  *)  AC_MSG_ERROR(bad value ${got_handling} for --enable-got option) ;;
esac

# PR gas/19109
# Decide the default method for compressing debug sections.
ac_default_compressed_debug_sections=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(compressed_debug_sections,
	      AS_HELP_STRING([--enable-compressed-debug-sections={all,ld,none}],
	      [compress debug sections by default])],
[case ,"${enableval}", in
  ,yes, | ,all, | *,ld,*) ac_default_compressed_debug_sections=yes ;;
  ,no, | ,none,)  ac_default_compressed_debug_sections=no ;;
esac])dnl

# Decide setting DT_RUNPATH instead of DT_RPATH by default
ac_default_new_dtags=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(new_dtags,
	      AS_HELP_STRING([--enable-new-dtags],
	      [set DT_RUNPATH instead of DT_RPATH by default])],
[case "${enableval}" in
  yes) ac_default_new_dtags=1 ;;
  no) ac_default_new_dtags=0 ;;
esac])dnl

# Decide if -z relro should be enabled in ELF linker by default.
ac_default_ld_z_relro=unset
# Provide a configure time option to override our default.
AC_ARG_ENABLE(relro,
	      AS_HELP_STRING([--enable-relro],
	      [enable -z relro in ELF linker by default]),
[case "${enableval}" in
  yes)  ac_default_ld_z_relro=1 ;;
  no)  ac_default_ld_z_relro=0 ;;
esac])dnl

# Decide if -z separate-code should be enabled in ELF linker by default.
ac_default_ld_z_separate_code=unset
AC_ARG_ENABLE(separate-code,
	      AS_HELP_STRING([--enable-separate-code],
	      [enable -z separate-code in ELF linker by default]),
[case "${enableval}" in
  yes) ac_default_ld_z_separate_code=1 ;;
  no) ac_default_ld_z_separate_code=0 ;;
esac])

# Decide which "--hash-style" to use by default
# Provide a configure time option to override our default.
AC_ARG_ENABLE([default-hash-style],
AS_HELP_STRING([--enable-default-hash-style={sysv,gnu,both}],
	       [use this default hash style]),
[case "${enable_default_hash_style}" in
  sysv | gnu | both) ;;
  *) AC_MSG_ERROR([bad value ${enable_default_hash_style} for enable-default-hash-style option]) ;;
esac],
[enable_default_hash_style=sysv])

case "${enable_default_hash_style}" in
  sysv | both) ac_default_emit_sysv_hash=1 ;;
  *) ac_default_emit_sysv_hash=0 ;;
esac

case "${enable_default_hash_style}" in
  gnu | both) ac_default_emit_gnu_hash=1 ;;
  *) ac_default_emit_gnu_hash=0 ;;
esac

AM_BINUTILS_WARNINGS

AC_CONFIG_HEADERS([config.h:config.in])

if test -z "$host" ; then
    AC_MSG_ERROR(Unrecognized host system type; please check config.sub.)
fi

# host-specific stuff:

AC_EXEEXT

AM_CONDITIONAL(GENINSRC_NEVER, false)

. ${srcdir}/configure.host

AC_SUBST(HDEFINES)
AC_SUBST(NATIVE_LIB_DIRS)

AC_CHECK_HEADERS_ONCE([fcntl.h locale.h malloc.h strings.h sys/file.h unistd.h])

ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([mmap setlocale glob strcasecmp strcspn])

AC_SEARCH_LIBS([dlopen], [dl])
AM_CONDITIONAL([ENABLE_PLUGINS], [test x$plugins = xyes])

AC_ARG_ENABLE(initfini-array,
[  --disable-initfini-array do not use .init_array/.fini_array sections],
[case "${enableval}" in
 yes|no) ;;
 *) AC_MSG_ERROR([invalid --enable-initfini-array argument]) ;;
 esac], [enable_initfini_array=yes])
AC_SUBST(enable_initfini_array)
if test $enable_initfini_array = yes; then
  AC_DEFINE(HAVE_INITFINI_ARRAY, 1,
    [Define .init_array/.fini_array sections are available and working.])
fi

AC_CHECK_DECLS([environ])

# When converting linker scripts into strings for use in emulation
# files, use astring.sed if the compiler supports ANSI string
# concatenation, or ostring.sed otherwise.  This is to support the
# broken Microsoft MSVC compiler, which limits the length of string
# constants, while still supporting pre-ANSI compilers which do not
# support string concatenation.
STRINGIFY=astring.sed
AC_SUBST(STRINGIFY)

# target-specific stuff:

all_targets=
EMUL=
all_emuls=
all_emul_extras=
all_libpath=

dnl We need to get an arbitrary number of tdir definitions into
dnl Makefile.  We can't do it using AC_SUBST, because autoconf does
dnl not permit literal newlines in an AC_SUBST variables.  So we use a
dnl file.
rm -f tdirs

# If the host is 64-bit, then we enable 64-bit targets by default.
# This is consistent with what ../bfd/configure.ac does.
if test x${want64} = xfalse; then
  AC_CHECK_SIZEOF(void *)
  if test "x${ac_cv_sizeof_void_p}" = "x8"; then
    want64=true
  fi
fi

elf_list_options=FALSE
elf_shlib_list_options=FALSE
elf_plt_unwind_list_options=FALSE
for targ_alias in `echo $target_alias | sed 's/,/ /g'`
do
    # Canonicalize the secondary target names.
    result=`$ac_config_sub $targ_alias 2>/dev/null`
    if test -n "$result"; then
	targ=$result
    else
	targ=$targ_alias
    fi

    targ_emul=armelf

    if test "$targ" = "$target"; then
      EMUL=$targ_emul
    fi

    if test x${want64} = xfalse; then
      . ${srcdir}/../bfd/config.bfd
    fi

    if test x${want64} = xtrue; then
      targ_extra_emuls="$targ_extra_emuls $targ64_extra_emuls"
      targ_extra_libpath="$targ_extra_libpath $targ64_extra_libpath"
    fi

    for i in $targ_emul $targ_extra_emuls $targ_extra_libpath; do
	case " $all_emuls " in
	*" e${i}.o "*) ;;
	*)
	  all_emuls="$all_emuls e${i}.o"
	  eval result=\$tdir_$i
	  test -z "$result" && result=$targ_alias
	  echo tdir_$i=$result >> tdirs
	  case "${i}" in
	  *elf*)
	    elf_list_options=TRUE
	    ;;
	  *)
	    if grep "TEMPLATE_NAME=elf32" ${srcdir}/emulparams/${i}.sh >/dev/null 2>/dev/null; then
	      elf_list_options=TRUE
	    fi
	    ;;
	  esac
	  if test "$elf_list_options" = "TRUE"; then
	    . ${srcdir}/emulparams/${i}.sh
	    if test x${GENERATE_SHLIB_SCRIPT} = xyes; then
	      elf_shlib_list_options=TRUE
	    fi
	    if test x${PLT_UNWIND} = xyes; then
	      elf_plt_unwind_list_options=TRUE
	    fi
	  fi
	  ;;
	esac
    done

    for i in $targ_emul $targ_extra_libpath; do
	case " $all_libpath " in
	*" ${i} "*) ;;
	*)
	  if test -z "$all_libpath"; then
	    all_libpath=${i}
	  else
	    all_libpath="$all_libpath ${i}"
	  fi
	  ;;
	esac
    done

    for i in $targ_extra_ofiles; do
	case " $all_emul_extras " in
	*" ${i} "*) ;;
	*)
	  all_emul_extras="$all_emul_extras ${i}"
	  ;;
	esac
    done
done

if test x$ac_default_compressed_debug_sections = xyes ; then
  AC_DEFINE(DEFAULT_FLAG_COMPRESS_DEBUG, 1, [Define if you want compressed debug sections by default.])
fi

if test "${ac_default_new_dtags}" = unset; then
  ac_default_new_dtags=0
fi
AC_DEFINE_UNQUOTED(DEFAULT_NEW_DTAGS,
  $ac_default_new_dtags,
  [Define to 1 if you want to set DT_RUNPATH instead of DT_RPATH by default.])

if test "${ac_default_ld_z_relro}" = unset; then
  ac_default_ld_z_relro=0
fi
AC_DEFINE_UNQUOTED(DEFAULT_LD_Z_RELRO,
  $ac_default_ld_z_relro,
  [Define to 1 if you want to enable -z relro in ELF linker by default.])

if test "${ac_default_ld_z_separate_code}" = unset; then
  ac_default_ld_z_separate_code=0
fi
AC_DEFINE_UNQUOTED(DEFAULT_LD_Z_SEPARATE_CODE,
  $ac_default_ld_z_separate_code,
  [Define to 1 if you want to enable -z separate-code in ELF linker by default.])

AC_DEFINE_UNQUOTED([DEFAULT_EMIT_SYSV_HASH],
  [$ac_default_emit_sysv_hash],
  [Define to 1 if you want to emit sysv hash in the ELF linker by default.])

AC_DEFINE_UNQUOTED([DEFAULT_EMIT_GNU_HASH],
  [$ac_default_emit_gnu_hash],
  [Define to 1 if you want to emit gnu hash in the ELF linker by default.])

AC_SUBST(elf_list_options)
AC_SUBST(elf_shlib_list_options)
AC_SUBST(elf_plt_unwind_list_options)
AC_SUBST(EMUL)

TDIRS=tdirs
AC_SUBST_FILE(TDIRS)

if test x${all_targets} = xtrue; then
  if test x${want64} = xtrue; then
    EMULATION_OFILES='$(ALL_EMULATIONS) $(ALL_64_EMULATIONS)'
    EMUL_EXTRA_OFILES='$(ALL_EMUL_EXTRA_OFILES) $(ALL_64_EMUL_EXTRA_OFILES)'
  else
    EMULATION_OFILES='$(ALL_EMULATIONS)'
    EMUL_EXTRA_OFILES='$(ALL_EMUL_EXTRA_OFILES)'
  fi
else
  EMULATION_OFILES=$all_emuls
  EMUL_EXTRA_OFILES=$all_emul_extras
fi
AC_SUBST(EMULATION_OFILES)
AC_SUBST(EMUL_EXTRA_OFILES)
AC_SUBST(LIB_PATH)

EMULATION_LIBPATH=$all_libpath
AC_SUBST(EMULATION_LIBPATH)

if test x${enable_static} = xno; then
  TESTBFDLIB="-Wl,--rpath,../bfd/.libs ../bfd/.libs/libbfd.so"
else
  TESTBFDLIB="../bfd/.libs/libbfd.a"
fi
AC_SUBST(TESTBFDLIB)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
