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
AC_CONFIG_AUX_DIR([../gnu_aux])
AC_CONFIG_SRCDIR(ldmain.c)
m4_include([../config/lean.m4])

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_INSTALL

AC_SYS_LARGEFILE

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

AC_CONFIG_HEADERS([config.h:config.in])

if test -z "$host" ; then
    AC_MSG_ERROR(Unrecognized host system type; please check config.sub.)
fi

# host-specific stuff:

AC_EXEEXT

. ${srcdir}/configure.host

AC_SUBST(HDEFINES)
AC_SUBST(NATIVE_LIB_DIRS)

AC_CHECK_HEADERS_ONCE([fcntl.h malloc.h strings.h sys/file.h unistd.h])

ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([mmap strcasecmp])

# target-specific stuff:

all_targets=
EMUL=
all_emuls=
all_emul_extras=
all_libpath=

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

      . ${srcdir}/../bfd/config.bfd

    for i in $targ_emul $targ_extra_emuls $targ_extra_libpath; do
	case " $all_emuls " in
	*" e${i}.o "*) ;;
	*)
	  all_emuls="$all_emuls e${i}.o"
	  test -z "$result" && result=$targ_alias
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

AC_SUBST(elf_list_options)
AC_SUBST(elf_shlib_list_options)
AC_SUBST(elf_plt_unwind_list_options)
AC_SUBST(EMUL)

  EMULATION_OFILES=$all_emuls
  EMUL_EXTRA_OFILES=$all_emul_extras
AC_SUBST(EMULATION_OFILES)
AC_SUBST(EMUL_EXTRA_OFILES)

EMULATION_LIBPATH=$all_libpath
AC_SUBST(EMULATION_LIBPATH)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
