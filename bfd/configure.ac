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

m4_include([version.m4])
AC_INIT([bfd], [BFD_VERSION])
AC_CONFIG_AUX_DIR([../aux])
AC_CONFIG_SRCDIR([libbfd.c])
m4_include([../config/lean.m4])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

dnl## These must be called before Libtool, because it may want
dnl## to call AC_CHECK_Prog
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_PROG_SED

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

ACX_LARGEFILE

AC_ARG_ENABLE(64-bit-bfd,
[  --enable-64-bit-bfd     64-bit support (on hosts with narrower word sizes)],
[case "${enableval}" in
  yes)  want64=true  ;;
  no)   want64=false ;;
  *)    AC_MSG_ERROR(bad value ${enableval} for 64-bit-bfd option) ;;
esac],[want64=false])dnl

enable_targets=

AC_ARG_ENABLE(64_bit_archive,
	      AS_HELP_STRING([--enable-64-bit-archive],
	      [force 64-bit archives]),
[case "${enableval}" in
  yes)  want_64_bit_archive=true  ;;
  no)   want_64_bit_archive=false ;;
  *)    AC_MSG_ERROR(bad value ${enableval} for 64-bit-archive option) ;;
esac],[want_64_bit_archive=unset])dnl

AC_ARG_WITH(mmap,
[  --with-mmap             try using mmap for BFD input files if available],
[case "${withval}" in
  yes)  want_mmap=true ;;
  no)   want_mmap=false ;;
  *)    AC_MSG_ERROR(bad value ${withval} for BFD with-mmap option) ;;
esac],[want_mmap=false])dnl

DEBUGDIR=${libdir}/debug
AC_ARG_WITH(separate-debug-dir,
  AS_HELP_STRING([--with-separate-debug-dir=DIR],
                 [Look for global separate debug info in DIR [[default=LIBDIR/debug]]]),
[DEBUGDIR="${withval}"])
AC_SUBST(DEBUGDIR)

ACX_PKGVERSION([GNU Binutils])
ACX_BUGURL([http://www.sourceware.org/bugzilla/])

AM_BINUTILS_WARNINGS

AC_CONFIG_HEADERS(config.h:config.in)

if test -z "$target" ; then
    AC_MSG_ERROR(Unrecognized target system type; please check config.sub.)
fi

AM_CONDITIONAL(GENINSRC_NEVER, false)
AC_EXEEXT

host64=false
target64=false
bfd_default_target_size=32

# host stuff:

# Permit host specific settings.
. ${srcdir}/configure.host

AC_SUBST(HDEFINES)
AC_PROG_INSTALL

BFD_HOST_64BIT_LONG=0
BFD_HOST_64BIT_LONG_LONG=0
BFD_HOST_64_BIT_DEFINED=0
BFD_HOST_64_BIT=
BFD_HOST_U_64_BIT=
BFD_HOSTPTR_T="unsigned long"

AC_TYPE_LONG_DOUBLE
AC_CHECK_SIZEOF(void *)

if test "x${ac_cv_sizeof_void_p}" = "x8"; then
  host64=true
fi


  BFD_HOST_64BIT_LONG_LONG=1
  test -n "${HOST_64BIT_TYPE}" || HOST_64BIT_TYPE="long long"
  test -n "${HOST_U_64BIT_TYPE}" || HOST_U_64BIT_TYPE="unsigned long long"
  if test "x${ac_cv_sizeof_void_p}" = "x8"; then
    BFD_HOSTPTR_T="unsigned long long"
  fi

if test -n "${HOST_64BIT_TYPE}" -a -n "${HOST_U_64BIT_TYPE}"; then
  BFD_HOST_64_BIT_DEFINED=1
  BFD_HOST_64_BIT="${HOST_64BIT_TYPE}"
  BFD_HOST_U_64_BIT="${HOST_U_64BIT_TYPE}"
fi

AC_SUBST(BFD_HOST_64BIT_LONG_LONG)
AC_SUBST(BFD_HOST_64_BIT_DEFINED)
AC_SUBST(BFD_HOST_64_BIT)
AC_SUBST(BFD_HOST_U_64_BIT)
AC_SUBST(BFD_HOSTPTR_T)

BFD_CC_FOR_BUILD

AC_CHECK_HEADERS_ONCE([fcntl.h limits.h strings.h sys/file.h sys/time.h unistd.h])

ACX_HEADER_STRING
AC_CHECK_FUNCS_ONCE([getpagesize mmap munmap strcasecmp strdup strncasecmp ])
AC_FUNC_FSEEKO

AC_CHECK_DECLS(ftello)
AC_CHECK_DECLS(ftello64)

AC_CHECK_DECLS(ffs)
AC_CHECK_DECLS(asprintf)
AC_CHECK_DECLS(vasprintf)

save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Werror"
AC_CACHE_CHECK([compiler support for hidden visibility], bfd_cv_hidden,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
const char hw[] __attribute__ ((__visibility__ ("hidden"))) = "Hello, World\n";
extern void print (const char *) __attribute__ ((__visibility__ ("hidden")));]],
	[[print (hw);]])],
  [bfd_cv_hidden=yes], [bfd_cv_hidden=no])])
CFLAGS="$save_CFLAGS"
if test $bfd_cv_hidden = yes; then
  AC_DEFINE(HAVE_HIDDEN, 1,
	    [Define if your compiler supports hidden visibility.])
fi
##LT_LIB_M

# When building a shared libbfd, link against the pic version of libiberty
# so that apps that use libbfd won't need libiberty just to satisfy any
# libbfd references.
# We can't do that if a pic libiberty is unavailable since including non-pic
# code would insert text relocations into libbfd.
SHARED_LIBADD=
SHARED_LDFLAGS=
AC_SUBST(SHARED_LDFLAGS)
AC_SUBST(SHARED_LIBADD)

all_targets=false
defvec=
selvecs=
assocvecs=
selarchs=
TDEFINES=
for targ in $target $canon_targets
do
	. $srcdir/config.bfd
	if test "x$targ" = "x$target"; then
	    defvec=$targ_defvec
	fi
	selvecs="$selvecs $targ_defvec $targ_selvecs"
	selarchs="$selarchs $targ_archs"
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


# uniq the architectures in all the configured targets.
f=""
for i in $selarchs ; do
    case " $f " in
    *" $i "*) ;;
    *) f="$f $i" ;;
    esac
done
selarchs="$f"

# Target backend .o files.
tb=

elf="elf.o elflink.o elf-attrs.o elf-strtab.o elf-properties.o
     elf-eh-frame.o dwarf1.o dwarf2.o"
coffgen="coffgen.o dwarf2.o"

for vec in $selvecs
do
    target_size=32
    case "$vec" in
    # This list is alphabetized to make it easy to compare
    # with the two vector lists in targets.c.  For the same reason,
    # use one entry per line, even though this leads to long lines.
    arm_elf32_le_vec)		 tb="$tb elf32-arm.o elf32.o $elf" ;;
    arm_elf32_fdpic_le_vec)     tb="$tb elf32-arm.o elf32.o $elf" ;;
    elf32_le_vec)		 tb="$tb elf32-gen.o elf32.o $elf" ;;

    "")				 ;;
    *) AC_MSG_ERROR(*** unknown target vector $vec) ;;
    esac

    if test ${target_size} = 64; then
	target64=true
    fi
    if test x"${vec}" = x"${defvec}"; then
	bfd_default_target_size=${target_size}
    fi
done

# Target architecture .o files.
# A couple of CPUs use shorter file names to avoid problems on DOS
# filesystems.
ta=`echo $selarchs | sed -e s/bfd_/cpu-/g -e s/_arch/.o/g -e s/mn10200/m10200/ -e s/mn10300/m10300/`

# Weed out duplicate .o files.
f=""
for i in $tb ; do
    case " $f " in
    *" $i "*) ;;
    *) f="$f $i" ;;
    esac
done
tb="$f"

f=""
for i in $ta ; do
    case " $f " in
    *" $i "*) ;;
    *) f="$f $i" ;;
    esac
done
ta="$f"

bfd_backends="$tb"
bfd_machines="$ta"

  # Only set these if they will be nonempty, for the clever echo.
  havevecs=
  assocvecs=
  test -n "$selvecs" &&
    havevecs=`echo $selvecs | sed -e 's/^/-DHAVE_/' -e 's/ \(.\)/ -DHAVE_\1/g'`
  test -n "$selvecs" &&
    selvecs=`echo $selvecs | sed -e 's/^/\&/' -e 's/ \(.\)/,\&\1/g'`
  test -n "$selarchs" &&
    selarchs=`echo $selarchs | sed -e 's/^/\&/' -e 's/ \(.\)/,\&\1/g'`

# 64-bit archives need a 64-bit bfd_vma.
if test "x$want_64_bit_archive" = xtrue; then
  want64=true
  AC_DEFINE(USE_64_BIT_ARCHIVE, 1,
    [Define if 64-bit archives should always be used.])
fi

case ${host64}-${target64}-${want64} in
  *true*)
    wordsize=64
    bfd64_libs='$(BFD64_LIBS)'
    all_backends='$(BFD64_BACKENDS) $(BFD32_BACKENDS)'
    if test $BFD_HOST_64_BIT_DEFINED = 0; then
      AC_MSG_WARN([You have requested a 64 bit BFD configuration, but])
      AC_MSG_WARN([your compiler may not have a 64 bit integral type])
    fi
    ;;
  false-false-false)
    wordsize=32
    all_backends='$(BFD32_BACKENDS)'
    ;;
esac

tdefaults=""
test -n "${defvec}" && tdefaults="${tdefaults} -DDEFAULT_VECTOR=${defvec}"
test -n "${selvecs}" && tdefaults="${tdefaults} -DSELECT_VECS='${selvecs}'"
test -n "${assocvecs}" && tdefaults="${tdefaults} -DASSOCIATED_VECS='${assocvecs}'"
test -n "${selarchs}" && tdefaults="${tdefaults} -DSELECT_ARCHITECTURES='${selarchs}'"

AC_SUBST(wordsize)
AC_SUBST(bfd64_libs)
AC_SUBST(all_backends)
AC_SUBST(bfd_backends)
AC_SUBST(bfd_machines)
AC_SUBST(bfd_default_target_size)
AC_SUBST(tdefaults)
AC_SUBST(havevecs)

AC_SUBST(COREFILE)
AC_SUBST(COREFLAG)
if test -n "$CORE_HEADER"; then
  AC_DEFINE_UNQUOTED(CORE_HEADER, $CORE_HEADER,
    [Name of host specific core header file to include in elf.c.])
fi
  supports_plugins=0
AC_SUBST(lt_cv_dlopen_libs)

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

dnl Required by html, pdf, install-pdf and install-html
AC_SUBST(datarootdir)
AC_SUBST(docdir)
AC_SUBST(htmldir)
AC_SUBST(pdfdir)

AC_OUTPUT
