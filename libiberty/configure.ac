dnl Process this file with autoconf to produce a configure script

AC_INIT
AC_CONFIG_AUX_DIR([../gnu_aux])
AC_CONFIG_SRCDIR([vprintf-support.c])
m4_include([../config/lean.m4])

# This works around the fact that libtool configuration may change LD
# for this particular configuration, but some shells, instead of
# keeping the changes in LD private, export them just because LD is
# exported.  We don't use libtool yet, but some day we might, so...
ORIGINAL_LD_FOR_MULTILIBS=$LD

if test "${srcdir}" = "."; then
  if test -n "${with_build_subdir}"; then
    libiberty_topdir="${srcdir}/../.."
    with_target_subdir=
  elif test -z "${with_target_subdir}"; then
    libiberty_topdir="${srcdir}/.."
  else
    if test "${with_target_subdir}" != "."; then
      libiberty_topdir="${srcdir}/${with_multisrctop}../.."
    else
      libiberty_topdir="${srcdir}/${with_multisrctop}.."
    fi
  fi
else
  libiberty_topdir="${srcdir}/.."
fi
AC_SUBST(libiberty_topdir)

AC_CANONICAL_HOST

dnl## These must be called before libtool because it may want
dnl## to call AC_ CHECK_PROG.
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib, :)

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

AC_C_BIGENDIAN


dnl When we start using automake:
AC_CONFIG_HEADER(config.h:config.in)

AC_EXEEXT

dnl When we start using automake:
AC_PROG_INSTALL

# Don't build the shared library for build.
if [[ -n "${with_build_subdir}" ]]; then
  enable_shared=no
fi

frag=
case "${host}" in
  *-*-freebsd2.1.*)	frag=mh-fbsd21 ;;
  *-*-freebsd2.2.[[012]])	frag=mh-fbsd21 ;;
  i370-*-opened*)       frag=mh-openedition ;;
  i[[34567]]86-*-windows*)	frag=mh-windows ;;
esac

if [[ -n "${frag}" ]]; then
  frag=${libiberty_topdir}/libiberty/config/$frag
fi

echo "# Warning: this fragment is automatically generated" > temp-frag

if [[ -n "${frag}" ]] && [[ -f "${frag}" ]]; then
  echo "Appending ${frag} to xhost-mkfrag"
  echo "# Following fragment copied from ${frag}" >> temp-frag
  cat ${frag} >> temp-frag
fi

echo enable_shared = no >> temp-frag

frag=xhost-mkfrag
${CONFIG_SHELL-/bin/sh} ${libiberty_topdir}/gnu_aux/move-if-change temp-frag xhost-mkfrag

host_makefile_frag=${frag}
AC_SUBST_FILE(host_makefile_frag)

# It's OK to check for header files.  Although the compiler may not be
# able to link anything, it had better be able to at least compile
# something.
AC_CHECK_HEADERS([fcntl.h malloc.h stdio_ext.h sys/file.h sys/param.h sys/time.h unistd.h])
AC_HEADER_SYS_WAIT
AC_HEADER_TIME

# Determine sizes of some types.
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([size_t])

# This is the list of functions which libiberty will provide if they
# are not available on the host.

funcs="asprintf atexit basename bzero getcwd getpagesize \
gettimeofday index mkstemps random rename  \
sigsetmask stpcpy strdup  \
vasprintf"
AC_CHECK_FUNCS_ONCE([atexit bzero getcwd getpagesize gettimeofday mempcpy pathconf pstat_getdynamic realpath stpcpy strdup strerror psignal strsignal])

# Also in the old function.def file: alloca, vfork, getopt.

vars="sys_errlist sys_nerr sys_siglist"

# These are neither executed nor required, but they help keep
# autoheader happy without adding a bunch of text to acconfig.h.
if test "x" = "y"; then
  AC_CHECK_DECLS([basename(char *), asprintf, vasprintf, ntf])
  AC_DEFINE(HAVE_SYS_ERRLIST, 1, [Define if you have the sys_errlist variable.])
  AC_DEFINE(HAVE_SYS_NERR,    1, [Define if you have the sys_nerr variable.])
  AC_DEFINE(HAVE_SYS_SIGLIST, 1, [Define if you have the sys_siglist variable.])
fi

# For each of these functions, if the host does not provide the
# function we want to put FN.o in LIBOBJS, and if the host does
# provide the function, we want to define HAVE_FN in config.h.

setobjs=
CHECK=
if test -n "${with_target_subdir}"; then

  # If we are being configured for Mingw, we know which functions
  # Mingw provides and which ones we will be expected to provide.

  case "${host}" in
  *-*-mingw*)
    AC_LIBOBJ([asprintf])
    AC_LIBOBJ([basename])
    AC_LIBOBJ([bzero])
    AC_LIBOBJ([getpagesize])
    AC_LIBOBJ([index])
    AC_LIBOBJ([mkstemps])
    AC_LIBOBJ([random])
    AC_LIBOBJ([sigsetmask])
    AC_LIBOBJ([stpcpy])
    AC_LIBOBJ([vasprintf])

    for f in $funcs; do
      case "$f" in
	asprintf | basename | bzero | getpagesize | index | mkstemps | random | sigsetmask | stpcpy | strdup | vasprintf)
	  ;;
	*)
	  n=HAVE_`echo $f | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
	  AC_DEFINE_UNQUOTED($n)
	  ;;
      esac
    done

    # Mingw doesnt provide any of the variables in $vars, so we
    # dont have to check them here.

    setobjs=yes
    ;;

  esac

else

   # Not a target library, so we set things up to run the test suite.
   CHECK=really-check

fi

AC_SUBST(CHECK)
AC_SUBST(target_header_dir)

case "${host}" in
  *-*-cygwin* | *-*-mingw*)
    AC_DEFINE(HAVE_SYS_ERRLIST)
    AC_DEFINE(HAVE_SYS_NERR)
    ;;
esac

if test -z "${setobjs}"; then

  case "${host}" in

  *-*-android*)
    ## On android, getpagesize is defined in unistd.h as a static inline
    ## function, which AC_CHECK_FUNCS does not handle properly.
    ac_cv_func_getpagesize=yes
    ;;

  *-*-mingw32*)
    ## Under mingw32, sys_nerr and sys_errlist exist, but they are
    ## macros, so the test below won't find them.
    libiberty_cv_var_sys_nerr=yes
    libiberty_cv_var_sys_errlist=yes
    ;;
  esac

  # We haven't set the list of objects yet.  Use the standard autoconf
  # tests.  This will only work if the compiler works.
  AC_REPLACE_FUNCS($funcs)

  for v in $vars; do
    AC_MSG_CHECKING([for $v])
    AC_CACHE_VAL(libiberty_cv_var_$v,
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[int *p;]],[[extern int $v []; p = $v;]])],
		      [eval "libiberty_cv_var_$v=yes"],
		      [eval "libiberty_cv_var_$v=no"])])
    if eval "test \"`echo '$libiberty_cv_var_'$v`\" = yes"; then
      AC_MSG_RESULT(yes)
      n=HAVE_`echo $v | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
      AC_DEFINE_UNQUOTED($n)
    else
      AC_MSG_RESULT(no)
    fi
  done

  AC_CHECK_DECLS([basename(char *), asprintf, vasprintf, getopt, canonicalize_file_name])
fi

  INSTALL_DEST=libdir
AC_SUBST(INSTALL_DEST)

m4_pattern_allow(LIBOBJS)
L=""
for l in x $LIBOBJS; do
  case $l in
    x) ;;
    *) L="$L ./$l" ;;
  esac
done
LIBOBJS="$L"

# We need multilib support, but only if configuring for the target.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],
  [[test -z "$CONFIG_HEADERS" || echo timestamp > stamp-h
if test -n "$CONFIG_FILES"; then
  if test -n "${with_target_subdir}"; then
    # FIXME: We shouldn't need to set ac_file
    ac_file=Makefile
    LD="${ORIGINAL_LD_FOR_MULTILIBS}"
    . ${libiberty_topdir}/config-ml.in
  fi
fi]],
[[srcdir=${srcdir}
host=${host}
target=${target}
with_target_subdir=${with_target_subdir}
with_multisubdir=${with_multisubdir}
ac_configure_args="--enable-multilib ${ac_configure_args}"
CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
ORIGINAL_LD_FOR_MULTILIBS="${ORIGINAL_LD_FOR_MULTILIBS}"
libiberty_topdir=${libiberty_topdir}
]])
AC_OUTPUT
