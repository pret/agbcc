#   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
#   2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
#   2014, 2015, 2016 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.

##############################################################################
### WARNING: this file contains embedded tabs.  Do not run untabify on this file.

m4_include(config/acx.m4)
m4_include(config/override.m4)
m4_include(config/proginstall.m4)
m4_include(config/elf.m4)
m4_include([config/isl.m4])
m4_include([bfd/version.m4])
AC_INIT([move-if-change],[BFD_VERSION])

AC_CONFIG_SRCDIR([move-if-change])
m4_include([config/lean.m4])

ACX_LEAN_FORCE_CACHE
AC_DISABLE_OPTION_CHECKING

progname=$0
# if PWD already has a value, it is probably wrong.
if test -n "$PWD" ; then PWD=`${PWDCMD-pwd}`; fi

# Export original configure arguments for use by sub-configures.
# Quote arguments with shell meta charatcers.
TOPLEVEL_CONFIGURE_ARGUMENTS=
set -- "$progname" "$@"
for ac_arg
do
  case "$ac_arg" in
  *" "*|*"	"*|*[[\[\]\~\#\$\^\&\*\(\)\{\}\\\|\;\<\>\?\']]*)
    ac_arg=`echo "$ac_arg" | sed "s/'/'\\\\\\\\''/g"`
    # if the argument is of the form -foo=baz, quote the baz part only
    ac_arg=`echo "'$ac_arg'" | sed "s/^'\([[-a-zA-Z0-9]]*=\)/\\1'/"` ;;
  *) ;;
  esac
  # Add the quoted argument to the list.
  TOPLEVEL_CONFIGURE_ARGUMENTS="$TOPLEVEL_CONFIGURE_ARGUMENTS $ac_arg"
done
if test "$silent" = yes; then
  TOPLEVEL_CONFIGURE_ARGUMENTS="$TOPLEVEL_CONFIGURE_ARGUMENTS --silent"
fi
# Remove the initial space we just introduced and, as these will be
# expanded by make, quote '$'.
TOPLEVEL_CONFIGURE_ARGUMENTS=`echo "x$TOPLEVEL_CONFIGURE_ARGUMENTS" | sed -e 's/^x *//' -e 's,\\$,$$,g'`
AC_SUBST(TOPLEVEL_CONFIGURE_ARGUMENTS)

# Find the build, host, and target systems.
ACX_NONCANONICAL_BUILD
ACX_NONCANONICAL_HOST
ACX_NONCANONICAL_TARGET

dnl Autoconf 2.5x and later will set a default program prefix if
dnl --target was used, even if it was the same as --host.  Disable
dnl that behavior.  This must be done before AC CANONICAL TARGET
dnl to take effect.
test "$host_noncanonical" = "$target_noncanonical" &&
  test "$program_prefix$program_suffix$program_transform_name" = \
    NONENONEs,x,x, &&
  program_transform_name=s,y,y,

AC_CANONICAL_TARGET

AC_ARG_PROGRAM

m4_pattern_allow([^AS_FOR_TARGET$])dnl
m4_pattern_allow([^AS_FOR_BUILD$])dnl

# Get 'install' or 'install-sh' and its variants.
AC_PROG_INSTALL
ACX_PROG_LN
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_AWK

srcpwd=`cd ${srcdir} ; ${PWDCMD-pwd}`

# We pass INSTALL explicitly to sub-makes.  Make sure that it is not
# a relative path.
if test "$INSTALL" = "${srcdir}/install-sh -c"; then
  INSTALL="${srcpwd}/install-sh -c"
fi

# Set srcdir to "." if that's what it is.
# This is important for multilib support.
pwd=`${PWDCMD-pwd}`
if test "${pwd}" = "${srcpwd}" ; then
  srcdir=.
fi

topsrcdir=$srcpwd

extra_host_args=

### To add a new directory to the tree, first choose whether it is a target
### or a host dependent tool.  Then put it into the appropriate list
### (library or tools, host or target), doing a dependency sort.

# Subdirs will be configured in the order listed in build_configdirs, 
# configdirs, or target_configdirs; see the serialization section below.

# Dependency sorting is only needed when *configuration* must be done in 
# a particular order.  In all cases a dependency should be specified in 
# the Makefile, whether or not it's implicitly specified here.

# Double entries in build_configdirs, configdirs, or target_configdirs may
# cause circular dependencies and break everything horribly.

# these library is used by various programs built for the build
# environment
#
build_libs="build-libiberty build-libcpp"

# these tools are built for the build environment
build_tools="build-m4"

# these libraries are used by various programs built for the host environment
#f
host_libs="libiberty bfd libcpp  libelf"

# these tools are built for the host environment
# Note, the powerpc-eabi build depends on sim occurring before gdb in order to
# know that we are building the simulator.
# binutils, gas and ld appear in that order because it makes sense to run
# "make check" in that particular order.
# If --enable-gold is used, "gold" may replace "ld".
host_tools="binutils gas ld m4"

# these libraries are built for the target environment, and are built after
# the host libraries and the host tools (which may be a cross compiler)
# Note that libiberty is not a target library.
target_libraries=""

# these tools are built using the target libraries, and are intended to
# run only in the target environment
#
# note: any program that *uses* libraries that are in the "target_libraries"
# list belongs in this list.
#
target_tools="target-rda"

################################################################################

## All tools belong in one of the four categories, and are assigned above
## We assign ${configdirs} this way to remove all embedded newlines.  This
## is important because configure will choke if they ever get through.
## ${configdirs} is directories we build using the host tools.
## ${target_configdirs} is directories we build using the target tools.
configdirs=`echo ${host_libs} ${host_tools}`
target_configdirs=`echo ${target_libraries} ${target_tools}`
build_configdirs=`echo ${build_libs} ${build_tools}`

m4_divert_text([PARSE_ARGS],
[case $srcdir in
  *" "*)
m4_pushdef([AS_MESSAGE_LOG_FD], [])dnl
    AC_MSG_ERROR([path to source, $srcdir, contains spaces])
m4_popdef([AS_MESSAGE_LOG_FD])dnl
    ;;
esac
ac_subdirs_all=`cd $srcdir && echo */configure | sed 's,/configure,,g'`
])

################################################################################

srcname="gnu development package"

# This gets set non-empty for some net releases of packages.
appdirs=""

# Define is_cross_compiler to save on calls to 'test'.
is_cross_compiler=
if test x"${host}" = x"${target}" ; then
  is_cross_compiler=no
else
  is_cross_compiler=yes
fi	

# Find the build and target subdir names.
GCC_TOPLEV_SUBDIRS
# Be sure to cover against remnants of an in-tree build.
if test $srcdir != .  && test -d $srcdir/host-${host_noncanonical}; then
  AC_MSG_ERROR([building out of tree but $srcdir contains host-${host_noncanonical}.
Use a pristine source tree when building in a separate tree])
fi

# Skipdirs are removed silently.
skipdirs=
# Noconfigdirs are removed loudly.
noconfigdirs=""

use_gnu_ld=
# Make sure we don't let GNU ld be added if we didn't want it.
if test x$with_gnu_ld = xno ; then
  use_gnu_ld=no
  noconfigdirs="$noconfigdirs ld gold"
fi

use_gnu_as=
# Make sure we don't let GNU as be added if we didn't want it.
if test x$with_gnu_as = xno ; then
  use_gnu_as=no
  noconfigdirs="$noconfigdirs gas"
fi

use_included_zlib=
AC_ARG_WITH(system-zlib,
[AS_HELP_STRING([--with-system-zlib], [use installed libz])])
# Make sure we don't let ZLIB be added if we didn't want it.
if test x$with_system_zlib = xyes ; then
  use_included_zlib=no
  noconfigdirs="$noconfigdirs zlib"
fi

# Some are only suitable for cross toolchains.
# Remove these if host=target.
cross_only="target-opcodes"

case $is_cross_compiler in
  no) skipdirs="${skipdirs} ${cross_only}" ;;
esac


skipdirs="${skipdirs} target-newlib"

with_as_accelerator_for=

enable_offload_targets=

# Handle --enable-gold, --enable-ld.
# --disable-gold [--enable-ld]
#     Build only ld.  Default option.
# --enable-gold [--enable-ld]
#     Build both gold and ld.  Install gold as "ld.gold", install ld
#     as "ld.bfd" and "ld".
# --enable-gold=default [--enable-ld]
#     Build both gold and ld.  Install gold as "ld.gold" and "ld",
#     install ld as "ld.bfd".
# --enable-gold[=default] --disable-ld
#     Build only gold, which is then installed as both "ld.gold" and "ld".
# --enable-gold --enable-ld=default
#     Build both gold (installed as "ld.gold") and ld (installed as "ld"
#     and ld.bfd).
#     In other words, ld is default
# --enable-gold=default --enable-ld=default
#     Error.

AC_ARG_ENABLE(ld,
[AS_HELP_STRING([[--enable-ld[=ARG]]],
		[build ld @<:@ARG={default,yes,no}@:>@])],
ENABLE_LD=$enableval,
ENABLE_LD=yes)

case "${ENABLE_LD}" in
  default)
    if test x${default_ld} != x; then
      AC_MSG_ERROR([either gold or ld can be the default ld])
    fi
    ;;
  yes)
    ;;
  no)
    if test x${ENABLE_GOLD} != xyes; then
      AC_MSG_WARN([neither ld nor gold are enabled])
    fi
    configdirs=`echo " ${configdirs} " | sed -e 's/ ld / /'`
    ;;
  *)
    AC_MSG_ERROR([invalid --enable-ld argument])
    ;;
esac

# PR gas/19109
# Decide the default method for compressing debug sections.
# Provide a configure time option to override our default.
AC_ARG_ENABLE(compressed_debug_sections,
[AS_HELP_STRING([--enable-compressed-debug-sections={all,gas,gold,ld,none}],
		[Enable compressed debug sections for gas, gold or ld by
		 default])],
[
  if test x"$enable_compressed_debug_sections" = xyes; then
    AC_MSG_ERROR([no program with compressed debug sections specified])
  fi
], [enable_compressed_debug_sections=])

# Configure extra directories which are host specific

case "${host}" in
  *-cygwin*)
    configdirs="$configdirs libtermcap" ;;
esac

# A target can indicate whether a language isn't supported for some reason.
# Only spaces may be used in this macro; not newlines or tabs.
unsupported_languages=

# Remove more programs from consideration, based on the host or 
# target this usually means that a port of the program doesn't
# exist yet.

case "${host}" in
  i[[3456789]]86-*-msdosdjgpp*)
    noconfigdirs="$noconfigdirs tcl tk itcl"
    ;;
esac



ENABLE_LIBSSP=no


# If this is accelerator compiler and its target is intelmic we enable
# target liboffloadmic by default.  If this is compiler with offloading
# for intelmic we enable host liboffloadmic by default.  Otherwise
# liboffloadmic is disabled by default.
enable_liboffloadmic=
AC_SUBST(extra_liboffloadmic_configure_flags)

noconfigdirs="$noconfigdirs target-libgomp"


# Disable libssp for some systems.
case "${target}" in
  avr-*-*)
    # No hosted I/O support.
    noconfigdirs="$noconfigdirs target-libssp"
    ;;
  powerpc-*-aix* | rs6000-*-aix*)
    noconfigdirs="$noconfigdirs target-libssp"
    ;;
  rl78-*-*)
    # libssp uses a misaligned load to trigger a fault, but the RL78
    # doesn't fault for those - instead, it gives a build-time error
    # for explicit misaligned loads.
    noconfigdirs="$noconfigdirs target-libssp"
    ;;
  visium-*-*)
    # No hosted I/O support.
    noconfigdirs="$noconfigdirs target-libssp"
    ;;
esac


# Default libgloss CPU subdirectory.
libgloss_dir="$target_cpu"

case "${target}" in
  arm*-*-*)
    libgloss_dir=arm
    ;;
esac

# Work in distributions that contain no compiler tools, like Autoconf.
host_makefile_frag=/dev/null
if test -d ${srcdir}/config ; then
case "${host}" in
  i[[3456789]]86-*-msdosdjgpp*)
    host_makefile_frag="config/mh-djgpp"
    ;;
  *-cygwin*)
    ACX_CHECK_CYGWIN_CAT_WORKS
    host_makefile_frag="config/mh-cygwin"
    ;;
  *-mingw*)
    host_makefile_frag="config/mh-mingw"
    ;;
  alpha*-linux*)
    host_makefile_frag="config/mh-alpha-linux"
    ;;
  hppa*-hp-hpux10*)
    host_makefile_frag="config/mh-pa-hpux10"
    ;;
  hppa*-hp-hpux*)
    host_makefile_frag="config/mh-pa"
    ;;
  hppa*-*)	
    host_makefile_frag="config/mh-pa"
    ;;
  *-*-darwin*)
    host_makefile_frag="config/mh-darwin"
    ;;
  powerpc-*-aix*)
    host_makefile_frag="config/mh-ppc-aix"
    ;;
  rs6000-*-aix*)
    host_makefile_frag="config/mh-ppc-aix"
    ;;
esac
fi

if test "${build}" != "${host}" ; then
  AR_FOR_BUILD=${AR_FOR_BUILD-ar}
  AS_FOR_BUILD=${AS_FOR_BUILD-as}
  CC_FOR_BUILD=${CC_FOR_BUILD-gcc}
  DLLTOOL_FOR_BUILD=${DLLTOOL_FOR_BUILD-dlltool}
  LD_FOR_BUILD=${LD_FOR_BUILD-ld}
  NM_FOR_BUILD=${NM_FOR_BUILD-nm}
  RANLIB_FOR_BUILD=${RANLIB_FOR_BUILD-ranlib}
else
  AR_FOR_BUILD="\$(AR)"
  AS_FOR_BUILD="\$(AS)"
  CC_FOR_BUILD="\$(CC)"
  DLLTOOL_FOR_BUILD="\$(DLLTOOL)"
  LD_FOR_BUILD="\$(LD)"
  NM_FOR_BUILD="\$(NM)"
  RANLIB_FOR_BUILD="\$(RANLIB)"
fi

AC_PROG_CC

# We must set the default linker to the linker used by gcc for the correct
# operation of libtool.  If LD is not defined and we are using gcc, try to
# set the LD default to the ld used by gcc.
if test -z "$LD"; then
  if test "$GCC" = yes; then
    case $build in
    *-*-mingw*)
      gcc_prog_ld=`$CC -print-prog-name=ld 2>&1 | tr -d '\015'` ;;
    *)
      gcc_prog_ld=`$CC -print-prog-name=ld 2>&1` ;;
    esac
    case $gcc_prog_ld in
    # Accept absolute paths.
    [[\\/]* | [A-Za-z]:[\\/]*)]
      LD="$gcc_prog_ld" ;;
    esac
  fi
fi

AC_ARG_ENABLE([bootstrap],
[AS_HELP_STRING([--enable-bootstrap],
		[enable bootstrapping @<:@yes if native build@:>@])],,
enable_bootstrap=default)

# Issue errors and warnings for invalid/strange bootstrap combinations.
  have_compiler=no

case "$have_compiler:$host:$target:$enable_bootstrap" in
  *:*:*:no) ;;

  # Default behavior.  Enable bootstrap if we have a compiler
  # and we are in a native configuration.
  yes:$build:$build:default)
    enable_bootstrap=yes ;;

  *:*:*:default)
    enable_bootstrap=no ;;

  # We have a compiler and we are in a native configuration, bootstrap is ok
  yes:$build:$build:yes)
    ;;

  # Other configurations, but we have a compiler.  Assume the user knows
  # what he's doing.
  yes:*:*:yes)
    AC_MSG_WARN([trying to bootstrap a cross compiler])
    ;;

  # No compiler: if they passed --enable-bootstrap explicitly, fail
  no:*:*:yes)
    AC_MSG_ERROR([cannot bootstrap without a compiler]) ;;

  # Fail if wrong command line
  *)
    AC_MSG_ERROR([invalid option for --enable-bootstrap])
    ;;
esac

# Check for GMP, MPFR and MPC
gmplibs="-lmpc -lmpfr -lgmp"
gmpinc=
have_gmp=no

# Specify a location for mpc
# check for this first so it ends up on the link line before mpfr.
with_mpc=
with_mpc_include=
with_mpc_lib=

# Specify a location for mpfr
# check for this first so it ends up on the link line before gmp.
with_mpfr=
with_mpfr_include=
with_mpfr_lib=

with_gmp=
with_gmp_include=
with_gmp_lib=

# Flags needed for both GMP, MPFR and/or MPC.
AC_SUBST(gmplibs)
AC_SUBST(gmpinc)
AC_SUBST(extra_mpfr_configure_flags)
AC_SUBST(extra_mpc_gmp_configure_flags)
AC_SUBST(extra_mpc_mpfr_configure_flags)
AC_SUBST(extra_isl_gmp_configure_flags)

# Libraries to use for stage1 or when not bootstrapping.
AC_ARG_WITH(stage1-libs,
[AS_HELP_STRING([--with-stage1-libs=LIBS], [libraries for stage1])],
[if test "$withval" = "no" -o "$withval" = "yes"; then
   stage1_libs=
 else
   stage1_libs=$withval
 fi],
[stage1_libs=])
AC_SUBST(stage1_libs)

# Linker flags to use for stage1 or when not bootstrapping.
AC_ARG_WITH(stage1-ldflags,
[AS_HELP_STRING([--with-stage1-ldflags=FLAGS], [linker flags for stage1])],
[if test "$withval" = "no" -o "$withval" = "yes"; then
   stage1_ldflags=
 else
   stage1_ldflags=$withval
 fi],
[stage1_ldflags=
 # In stage 1, default to linking libstdc++ and libgcc statically with GCC
 # if supported.  But if the user explicitly specified the libraries to use,
 # trust that they are doing what they want.
 if test "$stage1_libs" = "" -a "$have_static_libs" = yes; then
   stage1_ldflags="-static-libgcc"
 fi])
AC_SUBST(stage1_ldflags)

# Libraries to use for stage2 and later builds.
AC_ARG_WITH(boot-libs,
[AS_HELP_STRING([--with-boot-libs=LIBS], [libraries for stage2 and later])],
[if test "$withval" = "no" -o "$withval" = "yes"; then
   poststage1_libs=
 else
   poststage1_libs=$withval
 fi],
[poststage1_libs=])
AC_SUBST(poststage1_libs)

# Linker flags to use for stage2 and later builds.
AC_ARG_WITH(boot-ldflags,
[AS_HELP_STRING([--with-boot-ldflags=FLAGS],
		[linker flags for stage2 and later])],
[if test "$withval" = "no" -o "$withval" = "yes"; then
   poststage1_ldflags=
 else
   poststage1_ldflags=$withval
 fi],
[poststage1_ldflags=
 # In stages 2 and 3, default to linking libstdc++ and libgcc
 # statically.  But if the user explicitly specified the libraries to
 # use, trust that they are doing what they want.
 if test "$poststage1_libs" = ""; then
   poststage1_ldflags="-static-libgcc"
 fi])
AC_SUBST(poststage1_ldflags)

# GCC GRAPHITE dependency isl.
# Basic setup is inlined here, actual checks are in config/isl.m4

AC_ARG_WITH(isl,
  [AS_HELP_STRING(
   [--with-isl=PATH],
   [Specify prefix directory for the installed isl package.
    Equivalent to --with-isl-include=PATH/include
    plus --with-isl-lib=PATH/lib])])

# Treat --without-isl as a request to disable
# GRAPHITE support and skip all following checks.
if test "x$with_isl" != "xno"; then
  # Check for isl
  dnl Provide configure switches and initialize islinc & isllibs
  dnl with user input.
  ISL_INIT_FLAGS
  dnl The versions of isl that work for Graphite
  ISL_CHECK_VERSION()
  dnl Only execute fail-action, if isl has been requested.
  ISL_IF_FAILED([
    AC_MSG_ERROR([Unable to find a usable isl.  See config.log for details.])])
fi

# If the isl check failed, disable builds of in-tree variant of isl
if test "x$with_isl" = xno ||
   test "x$gcc_cv_isl" = xno; then
  noconfigdirs="$noconfigdirs isl"
  islinc=
fi

AC_SUBST(isllibs)
AC_SUBST(islinc)

# Check for LTO support.
AC_ARG_ENABLE(lto,
[AS_HELP_STRING([--enable-lto], [enable link time optimization support])],
enable_lto=$enableval,
enable_lto=yes; default_enable_lto=yes)

ACX_ELF_TARGET_IFELSE([# ELF platforms build the lto-plugin always.
  build_lto_plugin=yes
],[if test x"$default_enable_lto" = x"yes" ; then
    case $target in
      *-apple-darwin[[912]]* | *-cygwin* | *-mingw* | *djgpp*) ;;
      # On other non-ELF platforms, LTO has yet to be validated.
      *) enable_lto=no ;;
    esac
  else
  # Apart from ELF platforms, only Windows and Darwin support LTO so far.
  # It would also be nice to check the binutils support, but we don't
  # have gcc_GAS_CHECK_FEATURE available here.  For now, we'll just
  # warn during gcc/ subconfigure; unless you're bootstrapping with
  # -flto it won't be needed until after installation anyway.
    case $target in
      *-cygwin* | *-mingw* | *-apple-darwin* | *djgpp*) ;;
      *) if test x"$enable_lto" = x"yes"; then
	AC_MSG_ERROR([LTO support is not enabled for this target.])
        fi
      ;;
    esac
  fi
  # Among non-ELF, only Windows platforms support the lto-plugin so far.
  # Build it unless LTO was explicitly disabled.
  case $target in
    *-cygwin* | *-mingw*) build_lto_plugin=$enable_lto ;;
    *) ;;
  esac
])

AC_ARG_ENABLE(linker-plugin-configure-flags,
  [AS_HELP_STRING([[--enable-linker-plugin-configure-flags=FLAGS]],
    [additional flags for configuring linker plugins @<:@none@:>@])],
  extra_linker_plugin_configure_flags=$enableval,
  extra_linker_plugin_configure_flags=)
AC_SUBST(extra_linker_plugin_configure_flags)
AC_ARG_ENABLE(linker-plugin-flags,
  [AS_HELP_STRING([[--enable-linker-plugin-flags=FLAGS]],
    [additional flags for configuring and building linker plugins @<:@none@:>@])],
  extra_linker_plugin_flags=$enableval,
  extra_linker_plugin_flags=)
AC_SUBST(extra_linker_plugin_flags)

# Enable --enable-host-shared.
# Checked early to determine whether jit is an 'all' language
AC_ARG_ENABLE(host-shared,
[AS_HELP_STRING([--enable-host-shared],
		[build host code as shared libraries])],
[host_shared=$enableval], [host_shared=no])
AC_SUBST(host_shared)

# By default, C and C++ are the only stage 1 languages.
stage1_languages=,c,

# Target libraries that we bootstrap.
bootstrap_target_libs=,target-libgcc,

# Figure out what language subdirectories are present.
# Look if the user specified --enable-languages="..."; if not, use
# the environment variable $LANGUAGES if defined. $LANGUAGES might
# go away some day.
# NB:  embedded tabs in this IF block -- do not untabify
if test -d ${srcdir}/gcc; then
  if test x"${enable_languages+set}" != xset; then
    if test x"${LANGUAGES+set}" = xset; then
      enable_languages="${LANGUAGES}"
        echo configure.ac: warning: setting LANGUAGES is deprecated, use --enable-languages instead 1>&2
    else
      enable_languages=default
    fi
  else
    if test x"${enable_languages}" = x ||
       test x"${enable_languages}" = xyes;
       then
      echo configure.ac: --enable-languages needs at least one language argument 1>&2
      exit 1
    fi
  fi
  enable_languages=`echo "${enable_languages}" | sed -e 's/[[ 	,]][[ 	,]]*/,/g' -e 's/,$//'`

  # First scan to see if an enabled language requires some other language.
  # We assume that a given config-lang.in will list all the language
  # front ends it requires, even if some are required indirectly.
  for lang_frag in ${srcdir}/gcc/*/config-lang.in .. ; do
    case ${lang_frag} in
      ..) ;;
      # The odd quoting in the next line works around
      # an apparent bug in bash 1.12 on linux.
      ${srcdir}/gcc/[[*]]/config-lang.in) ;;
      *)
        # From the config-lang.in, get $language, $lang_requires, and
        # $lang_requires_boot_languages.
        language=
        lang_requires=
        lang_requires_boot_languages=
        # set srcdir during sourcing lang_frag to the gcc dir.
        # Sadly overriding srcdir on the . line doesn't work in plain sh as it
        # polutes this shell
        saved_srcdir=${srcdir}
        srcdir=${srcdir}/gcc . ${lang_frag}
        srcdir=${saved_srcdir}
        for other in ${lang_requires} ${lang_requires_boot_languages}; do
          case ,${enable_languages}, in
	    *,$other,*) ;;
	    *,default,*) ;;
	    *,all,*) ;;
	    *,$language,*)
	      echo " \`$other' language required by \`$language'; enabling" 1>&2
	      enable_languages="${enable_languages},${other}"
	      ;;
	  esac
        done
	for other in ${lang_requires_boot_languages} ; do
	  if test "$other" != "c"; then
	    case ,${enable_stage1_languages}, in
	      *,$other,*) ;;
	      *,default,*) ;;
	      *,all,*) ;;
	      *)
		case ,${enable_languages}, in
		  *,$language,*)
		    echo " '$other' language required by '$language' in stage 1; enabling" 1>&2
		    enable_stage1_languages="$enable_stage1_languages,${other}"
		    ;;
		esac
		;;
	    esac
          fi
        done
        ;;
    esac
  done

  new_enable_languages=,c,

  # If LTO is enabled, add the LTO front end.
  if test "$enable_lto" = "yes" ; then
    case ,${enable_languages}, in
      *,lto,*) ;;
      *) enable_languages="${enable_languages},lto" ;;
    esac
    if test "${build_lto_plugin}" = "yes" ; then
      configdirs="$configdirs lto-plugin"
    fi
  fi

  # If we're building an offloading compiler, add the LTO front end.
  if test x"$enable_as_accelerator_for" != x ; then
    case ,${enable_languages}, in
      *,lto,*) ;;
      *) enable_languages="${enable_languages},lto" ;;
    esac
  fi

  missing_languages=`echo ",$enable_languages," | sed -e s/,default,/,/ -e s/,all,/,/ -e s/,c,/,/ `
  potential_languages=,c,

  enabled_target_libs=
  disabled_target_libs=

  for lang_frag in ${srcdir}/gcc/*/config-lang.in .. ; do
    case ${lang_frag} in
      ..) ;;
      # The odd quoting in the next line works around
      # an apparent bug in bash 1.12 on linux.
      ${srcdir}/gcc/[[*]]/config-lang.in) ;;
      *)
        # From the config-lang.in, get $language, $target_libs, 
        # $lang_dirs, $boot_language, and $build_by_default
        language=
        target_libs=
        lang_dirs=
        subdir_requires=
        boot_language=no
        build_by_default=yes
        # set srcdir during sourcing.  See above about save & restore
        saved_srcdir=${srcdir}
        srcdir=${srcdir}/gcc . ${lang_frag}
        srcdir=${saved_srcdir}
        if test x${language} = x; then
          echo "${lang_frag} doesn't set \$language." 1>&2
          exit 1
        fi

        add_this_lang=no
        # C is always enabled, so no need to add it again
        if test "$language" != "c"; then
          case ,${enable_languages}, in
            *,${language},*)
              # Language was explicitly selected; include it
	      add_this_lang=yes
              ;;
	    *,all,*)
	      # All languages are enabled
	      add_this_lang=all
              ;;
            *,default,*)
              # 'default' was selected, select it if it is a default language
	      add_this_lang=${build_by_default}
              ;;
          esac
        fi

        # Disable languages that need other directories if these aren't available.
	for i in $subdir_requires; do
	  test -f "$srcdir/gcc/$i/config-lang.in" && continue
	  case ${add_this_lang} in
	    yes)
              # Specifically requested language; tell them.
              AC_MSG_ERROR([The gcc/$i directory contains parts of $language but is missing])
              ;;
            all)
              AC_MSG_WARN([The gcc/$i directory contains parts of $language but is missing])
              add_this_lang=unsupported
              ;;
            *)
              # Silently disable.
              add_this_lang=unsupported
              ;;
          esac
	done

        # Disable Ada if no preexisting GNAT is available.
        case ${add_this_lang}:${language}:${have_gnat} in
          yes:ada:no)
            # Specifically requested language; tell them.
            AC_MSG_ERROR([GNAT is required to build $language])
            ;;
          all:ada:no)
            AC_MSG_WARN([GNAT is required to build $language])
            add_this_lang=unsupported
            ;;
          *:ada:no)
            # Silently disable.
            add_this_lang=unsupported
            ;;
        esac

        # Disable jit if -enable-host-shared not specified
        case ${add_this_lang}:${language}:${host_shared} in
          yes:jit:no)
	    # PR jit/64780: explicitly specify --enable-host-shared
	    AC_MSG_ERROR([
Enabling language "jit" requires --enable-host-shared.

--enable-host-shared typically slows the rest of the compiler down by
a few %, so you must explicitly enable it.

If you want to build both the jit and the regular compiler, it is often
best to do this via two separate configure/builds, in separate
directories, to avoid imposing the performance cost of
--enable-host-shared on the regular compiler.])
	    ;;
          all:jit:no)
	    AC_MSG_WARN([--enable-host-shared required to build $language])
            add_this_lang=unsupported
            ;;
          *:jit:no)
            # Silently disable.
            add_this_lang=unsupported
            ;;
	esac

        # Disable a language that is unsupported by the target.
	case "${add_this_lang}: $unsupported_languages " in
	  no:*) ;;
	  unsupported:*) ;;
	  *:*" $language "*)
	    AC_MSG_WARN([${language} not supported for this target])
	    add_this_lang=unsupported
	    ;;
	esac

	case $add_this_lang in
	  unsupported)
            # Remove language-dependent dirs.
	    disabled_target_libs="$disabled_target_libs $target_libs"
	    noconfigdirs="$noconfigdirs $lang_dirs"
	    ;;
	  no)
            # Remove language-dependent dirs; still show language as supported.
	    disabled_target_libs="$disabled_target_libs $target_libs"
	    noconfigdirs="$noconfigdirs $lang_dirs"
            potential_languages="${potential_languages}${language},"
	    ;;
          all|yes)
	    new_enable_languages="${new_enable_languages}${language},"
            potential_languages="${potential_languages}${language},"
	    missing_languages=`echo "$missing_languages" | sed "s/,$language,/,/"`
	    enabled_target_libs="$enabled_target_libs $target_libs"
	    case "${boot_language}:,$enable_stage1_languages," in
	      yes:* | *:*,$language,* | *:*,yes, | *:*,all,)
		# Add to (comma-separated) list of stage 1 languages.
		case ",$stage1_languages," in
		  *,$language,* | ,yes, | ,all,) ;;
		  *) stage1_languages="${stage1_languages}${language}," ;;
		esac
		# We need to bootstrap any supporting libraries.
		bootstrap_target_libs="${bootstrap_target_libs}${target_libs},"
		;;
	    esac
	    ;;
        esac
        ;;
    esac
  done

  # Add target libraries which are only needed for disabled languages
  # to noconfigdirs.
  if test -n "$disabled_target_libs"; then
    for dir in $disabled_target_libs; do
      case " $enabled_target_libs " in
      *" ${dir} "*) ;;
      *) noconfigdirs="$noconfigdirs $dir" ;;
      esac
    done
  fi

  AC_ARG_ENABLE(stage1-languages,
  [AS_HELP_STRING([[--enable-stage1-languages[=all]]],
		  [choose additional languages to build during
		  stage1.  Mostly useful for compiler development])],
  [case ,${enable_stage1_languages}, in
    ,no,|,,)
      # Set it to something that will have no effect in the loop below
      enable_stage1_languages=c ;;
    ,yes,)
      enable_stage1_languages=`echo $new_enable_languages | \
	sed -e "s/^,//" -e "s/,$//" ` ;;
    *,all,*)
      enable_stage1_languages=`echo ,$enable_stage1_languages, | \
	sed -e "s/,all,/$new_enable_languages/" -e "s/^,//" -e "s/,$//" ` ;;
  esac

  # Add "good" languages from enable_stage1_languages to stage1_languages,
  # while "bad" languages go in missing_languages.  Leave no duplicates.
  for i in `echo $enable_stage1_languages | sed 's/,/ /g' `; do
    case $potential_languages in
      *,$i,*)
        case $stage1_languages in
          *,$i,*) ;;
          *) stage1_languages="$stage1_languages$i," ;;
        esac ;;
      *)
        case $missing_languages in
          *,$i,*) ;;
          *) missing_languages="$missing_languages$i," ;;
        esac ;;
     esac
  done])

  # Remove leading/trailing commas that were added for simplicity
  potential_languages=`echo "$potential_languages" | sed -e "s/^,//" -e "s/,$//"`
  missing_languages=`echo "$missing_languages" | sed -e "s/^,//" -e "s/,$//"`
  stage1_languages=`echo "$stage1_languages" | sed -e "s/^,//" -e "s/,$//"`
  new_enable_languages=`echo "$new_enable_languages" | sed -e "s/^,//" -e "s/,$//"`

  if test "x$missing_languages" != x; then
    AC_MSG_ERROR([
The following requested languages could not be built: ${missing_languages}
Supported languages are: ${potential_languages}])
  fi
  if test "x$new_enable_languages" != "x$enable_languages"; then
    echo The following languages will be built: ${new_enable_languages}
    enable_languages="$new_enable_languages"
  fi

  AC_SUBST(stage1_languages)
  ac_configure_args=`echo " $ac_configure_args" | sed -e "s/ '--enable-languages=[[^ ]]*'//g" -e "s/$/ '--enable-languages="$enable_languages"'/" `
fi

# Handle --disable-<component> generically.
for dir in $configdirs $build_configdirs $target_configdirs ; do
  dirname=`echo $dir | sed -e s/target-//g -e s/build-//g -e s/-/_/g`
  varname=`echo $dirname | sed -e s/+/_/g`
  if eval test x\${enable_${varname}} "=" xno ; then
    noconfigdirs="$noconfigdirs $dir"
  fi
done

# Check for Boehm's garbage collector
AC_ARG_ENABLE(objc-gc,
[AS_HELP_STRING([--enable-objc-gc],
		[enable use of Boehm's garbage collector with the
		 GNU Objective-C runtime])])
AC_ARG_WITH([target-bdw-gc],
[AS_HELP_STRING([--with-target-bdw-gc=PATHLIST],
		[specify prefix directory for installed bdw-gc package.
		 Equivalent to --with-target-bdw-gc-include=PATH/include
		 plus --with-target-bdw-gc-lib=PATH/lib])])
AC_ARG_WITH([target-bdw-gc-include],
[AS_HELP_STRING([--with-target-bdw-gc-include=PATHLIST],
		[specify directories for installed bdw-gc include files])])
AC_ARG_WITH([target-bdw-gc-lib],
[AS_HELP_STRING([--with-target-bdw-gc-lib=PATHLIST],
		[specify directories for installed bdw-gc library])])
		  
case ,${enable_languages},:${enable_objc_gc} in *,objc,*:yes|*,objc,*:auto)
  AC_MSG_CHECKING([for bdw garbage collector])
  if test "x$with_target_bdw_gc$with_target_bdw_gc_include$with_target_bdw_gc_lib" = x; then
    dnl no bdw-gw options, assume default locations
    AC_MSG_RESULT([using bdw-gc in default locations])
  else
    dnl bdw-gw options, first error checking, complete checking in libobjc
    if test "x$with_target_bdw_gc_include" = x && test "x$with_target_bdw_gc_lib" != x; then
      AC_MSG_ERROR([found --with-target-bdw-gc-lib but --with-target-bdw-gc-include missing])
    elif test "x$with_target_bdw_gc_include" != x && test "x$with_target_bdw_gc_lib" = x; then
      AC_MSG_ERROR([found --with-target-bdw-gc-include but --with-target-bdw-gc-lib missing])
    else
      AC_MSG_RESULT([using paths configured with --with-target-bdw-gc options])
    fi
  fi
esac

# Disable libitm, libsanitizer, libvtv, liboffloadmic if we're not building C++
    noconfigdirs="$noconfigdirs target-liboffloadmic target-libitm target-libsanitizer target-libvtv"

# Remove the entries in $skipdirs and $noconfigdirs from $configdirs,
# $build_configdirs and $target_configdirs.
# If we have the source for $noconfigdirs entries, add them to $notsupp.

notsupp=""
for dir in . $skipdirs $noconfigdirs ; do
  dirname=`echo $dir | sed -e s/target-//g -e s/build-//g`
  if test $dir != .  && echo " ${configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    configdirs=`echo " ${configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r $srcdir/$dirname/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="$notsupp $dir"
      fi
    fi
  fi
  if test $dir != .  && echo " ${build_configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    build_configdirs=`echo " ${build_configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r $srcdir/$dirname/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="$notsupp $dir"
      fi
    fi
  fi
  if test $dir != . && echo " ${target_configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    target_configdirs=`echo " ${target_configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r $srcdir/$dirname/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="$notsupp $dir"
      fi
    fi
  fi
done

# Quietly strip out all directories which aren't configurable in this tree.
# This relies on all configurable subdirectories being autoconfiscated, which
# is now the case.
build_configdirs_all="$build_configdirs"
build_configdirs=
for i in ${build_configdirs_all} ; do
  j=`echo $i | sed -e s/build-//g` 
  if test -f ${srcdir}/$j/configure ; then
    build_configdirs="${build_configdirs} $i"
  fi
done

configdirs_all="$configdirs"
configdirs=
for i in ${configdirs_all} ; do
  if test -f ${srcdir}/$i/configure ; then
    configdirs="${configdirs} $i"
  fi
done

target_configdirs_all="$target_configdirs"
target_configdirs=
for i in ${target_configdirs_all} ; do
  j=`echo $i | sed -e s/target-//g` 
  if test -f ${srcdir}/$j/configure ; then
    target_configdirs="${target_configdirs} $i"
  fi
done

# libiberty-linker-plugin is special: it doesn't have its own source directory,
# so we have to add it after the preceding checks.
if test x"$extra_linker_plugin_flags$extra_linker_plugin_configure_flags" != x
then
  case " $configdirs " in
    *" libiberty "*)
      # If we can build libiberty, we can also build libiberty-linker-plugin.
      configdirs="$configdirs libiberty-linker-plugin"
      extra_linker_plugin_configure_flags="$extra_linker_plugin_configure_flags \
        --with-libiberty=../libiberty-linker-plugin";;
    *)
      AC_MSG_ERROR([libiberty missing]);;
  esac
fi

# Sometimes we have special requirements for the host libiberty.
extra_host_libiberty_configure_flags=
extra_host_zlib_configure_flags=
case " $configdirs " in
  *" lto-plugin "* | *" libcc1 "*)
    # When these are to be built as shared libraries, the same applies to
    # libiberty.
    extra_host_libiberty_configure_flags=--enable-shared
    ;;
  *" bfd "*)
    # When bfd is to be built as a shared library, the same applies to
    # zlib.
    if test "$enable_shared" = "yes"; then
      extra_host_zlib_configure_flags=--enable-host-shared
    fi
    ;;
esac
AC_SUBST(extra_host_libiberty_configure_flags)
AC_SUBST(extra_host_zlib_configure_flags)

# Produce a warning message for the subdirs we can't configure.
# This isn't especially interesting in the Cygnus tree, but in the individual
# FSF releases, it's important to let people know when their machine isn't
# supported by the one or two programs in a package.

if test -n "${notsupp}" && test -z "${norecursion}" ; then
  # If $appdirs is non-empty, at least one of those directories must still
  # be configured, or we error out.  (E.g., if the gas release supports a
  # specified target in some subdirs but not the gas subdir, we shouldn't
  # pretend that all is well.)
  if test -n "$appdirs" ; then
    for dir in $appdirs ; do
      if test -r $dir/Makefile.in ; then
	if echo " ${configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	  appdirs=""
	  break
	fi
	if echo " ${target_configdirs} " | grep " target-${dir} " >/dev/null 2>&1; then
	  appdirs=""
	  break
	fi
      fi
    done
    if test -n "$appdirs" ; then
      echo "*** This configuration is not supported by this package." 1>&2
      exit 1
    fi
  fi
  # Okay, some application will build, or we don't care to check.  Still
  # notify of subdirs not getting built.
  echo "*** This configuration is not supported in the following subdirectories:" 1>&2
  echo "    ${notsupp}" 1>&2
  echo "    (Any other directories should still work fine.)" 1>&2
fi

case "$host" in
  *msdosdjgpp*)
    enable_gdbtk=no ;;
esac

# To find our prefix, in gcc_cv_tool_prefix.
#ACX_TOOL_DIRS

copy_dirs=

AC_ARG_WITH([build-sysroot], 
  [AS_HELP_STRING([--with-build-sysroot=SYSROOT],
		  [use sysroot as the system root during the build])],
  [if test x"$withval" != x ; then
     SYSROOT_CFLAGS_FOR_TARGET="--sysroot=$withval"
   fi],
  [SYSROOT_CFLAGS_FOR_TARGET=])
AC_SUBST(SYSROOT_CFLAGS_FOR_TARGET)

AC_ARG_WITH([debug-prefix-map],
  [AS_HELP_STRING([--with-debug-prefix-map='A=B C=D ...'],
		  [map A to B, C to D ... in debug information])],
  [if test x"$withval" != x; then
     DEBUG_PREFIX_CFLAGS_FOR_TARGET=
     for debug_map in $withval; do
       DEBUG_PREFIX_CFLAGS_FOR_TARGET="$DEBUG_PREFIX_CFLAGS_FOR_TARGET -fdebug-prefix-map=$debug_map"
     done
   fi],
  [DEBUG_PREFIX_CFLAGS_FOR_TARGET=])
AC_SUBST(DEBUG_PREFIX_CFLAGS_FOR_TARGET)

# During gcc bootstrap, if we use some random cc for stage1 then CFLAGS
# might be empty or "-g".  We don't require a C++ compiler, so CXXFLAGS
# might also be empty (or "-g", if a non-GCC C++ compiler is in the path).
# We want to ensure that TARGET libraries (which we know are built with
# gcc) are built with "-O2 -g", so include those options when setting
# CFLAGS_FOR_TARGET and CXXFLAGS_FOR_TARGET.
if test "x$CFLAGS_FOR_TARGET" = x; then
  if test "x${is_cross_compiler}" = xyes; then
    CFLAGS_FOR_TARGET="-g -O2"
  else
    CFLAGS_FOR_TARGET=$CFLAGS
    case " $CFLAGS " in
      *" -O2 "*) ;;
      *) CFLAGS_FOR_TARGET="-O2 $CFLAGS_FOR_TARGET" ;;
    esac
    case " $CFLAGS " in
      *" -g "* | *" -g3 "*) ;;
      *) CFLAGS_FOR_TARGET="-g $CFLAGS_FOR_TARGET" ;;
    esac
  fi
fi
AC_SUBST(CFLAGS_FOR_TARGET)

AC_SUBST(LDFLAGS_FOR_TARGET)

# Handle --with-headers=XXX.  If the value is not "yes", the contents of
# the named directory are copied to $(tooldir)/sys-include.
if test x"${with_headers}" != x && test x"${with_headers}" != xno ; then
  if test x${is_cross_compiler} = xno ; then
    echo 1>&2 '***' --with-headers is only supported when cross compiling
    exit 1
  fi
  if test x"${with_headers}" != xyes ; then
    x=${gcc_cv_tool_prefix}
    copy_dirs="${copy_dirs} ${with_headers} $x/${target_noncanonical}/sys-include"
  fi
fi

# Handle --with-libs=XXX.  If the value is not "yes", the contents of
# the name directories are copied to $(tooldir)/lib.  Multiple directories
# are permitted.
if test x"${with_libs}" != x && test x"${with_libs}" != xno ; then
  if test x${is_cross_compiler} = xno ; then
    echo 1>&2 '***' --with-libs is only supported when cross compiling
    exit 1
  fi
  if test x"${with_libs}" != xyes ; then
    # Copy the libraries in reverse order, so that files in the first named
    # library override files in subsequent libraries.
    x=${gcc_cv_tool_prefix}
    for l in ${with_libs}; do
      copy_dirs="$l $x/${target_noncanonical}/lib ${copy_dirs}"
    done
  fi
fi

# Set with_gnu_as, with_gnu_ld, and with_system_zlib as appropriate.
#
# This is done by determining whether or not the appropriate directory
# is available, and by checking whether or not specific configurations
# have requested that this magic not happen.
# 
# The command line options always override the explicit settings in 
# configure.ac, and the settings in configure.ac override this magic.
#
# If the default for a toolchain is to use GNU as and ld, and you don't 
# want to do that, then you should use the --without-gnu-as and
# --without-gnu-ld options for the configure script.  Similarly, if
# the default is to use the included zlib and you don't want to do that,
# you should use the --with-system-zlib option for the configure script.

if test x${use_gnu_as} = x &&
   echo " ${configdirs} " | grep " gas " > /dev/null 2>&1 ; then
  with_gnu_as=yes
  extra_host_args="$extra_host_args --with-gnu-as"
fi

if test x${use_gnu_ld} = x &&
   echo " ${configdirs} " | egrep " (go)?ld " > /dev/null 2>&1 ; then
  with_gnu_ld=yes
  extra_host_args="$extra_host_args --with-gnu-ld"
fi

if test x${use_included_zlib} = x &&
   echo " ${configdirs} " | grep " zlib " > /dev/null 2>&1 ; then
  :
else
  with_system_zlib=yes
  extra_host_args="$extra_host_args --with-system-zlib"
fi

# Handle ${copy_dirs}
set fnord ${copy_dirs}
shift
while test $# != 0 ; do
  if test -f $2/COPIED && test x"`cat $2/COPIED`" = x"$1" ; then
    :
  else
    echo Copying $1 to $2

    # Use the install script to create the directory and all required
    # parent directories.
    if test -d $2 ; then
      :
    else
      echo >config.temp
      ${srcdir}/install-sh -c -m 644 config.temp $2/COPIED
    fi

    # Copy the directory, assuming we have tar.
    # FIXME: Should we use B in the second tar?  Not all systems support it.
    (cd $1; tar -cf - .) | (cd $2; tar -xpf -)

    # It is the responsibility of the user to correctly adjust all
    # symlinks.  If somebody can figure out how to handle them correctly
    # here, feel free to add the code.

    echo $1 > $2/COPIED
  fi
  shift; shift
done

# Determine a target-dependent exec_prefix that the installed
# gcc will search in.  Keep this list sorted by triplet, with
# the *-*-osname triplets last.
md_exec_prefix=
case "${target}" in
  i[[34567]]86-pc-msdosdjgpp*)
    md_exec_prefix=/dev/env/DJDIR/bin
    ;;
  *-*-hpux* | \
  *-*-nto-qnx* | \
  *-*-solaris2*)
    md_exec_prefix=/usr/ccs/bin
    ;;
esac

extra_arflags_for_target=
extra_nmflags_for_target=
extra_ranlibflags_for_target=
target_makefile_frag=/dev/null
alphaieee_frag=/dev/null

# If --enable-target-optspace always use -Os instead of -O2 to build
# the target libraries, similarly if it is not specified, use -Os
# on selected platforms.
ospace_frag=/dev/null

# Some systems (e.g., one of the i386-aix systems the gas testers are
# using) don't handle "\$" correctly, so don't use it here.
tooldir='${exec_prefix}'/${target_noncanonical}
build_tooldir=${tooldir}

# Create a .gdbinit file which runs the one in srcdir
# and tells GDB to look there for source files.

if test -r ${srcdir}/.gdbinit ; then
  case ${srcdir} in
    .) ;;
    *) cat > ./.gdbinit <<EOF
# ${NO_EDIT}
dir ${srcdir}
dir .
source ${srcdir}/.gdbinit
EOF
    ;;
  esac
fi

# Make sure that the compiler is able to generate an executable.  If it
# can't, we are probably in trouble.  We don't care whether we can run the
# executable--we might be using a cross compiler--we only care whether it
# can be created.  At this point the main configure script has set CC.
we_are_ok=no
echo "int main () { return 0; }" > conftest.c
${CC} -o conftest ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} conftest.c
if test $? = 0 ; then
  if test -s conftest || test -s conftest.exe ; then
    we_are_ok=yes
  fi
fi 
case $we_are_ok in
  no)
    echo 1>&2 "*** The command '${CC} -o conftest ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} conftest.c' failed."
    echo 1>&2 "*** You must set the environment variable CC to a working compiler."
    rm -f conftest*
    exit 1
    ;;
esac
rm -f conftest*

# Decide which environment variable is used to find dynamic libraries.
case "${host}" in
  *-*-hpux*) RPATH_ENVVAR=SHLIB_PATH ;;
  *-*-darwin*) RPATH_ENVVAR=DYLD_LIBRARY_PATH ;;
  *-*-mingw* | *-*-cygwin ) RPATH_ENVVAR=PATH ;;
  *) RPATH_ENVVAR=LD_LIBRARY_PATH ;;
esac

# On systems where the dynamic library environment variable is PATH,
# gcc/ will put dynamic libraries into a subdirectory to avoid adding
# built executables to PATH.
if test "$RPATH_ENVVAR" = PATH; then
  GCC_SHLIB_SUBDIR=/shlib
else
  GCC_SHLIB_SUBDIR=
fi

# Adjust the toplevel makefile according to whether bootstrap was selected.
case $enable_bootstrap in
  yes)
    bootstrap_suffix=bootstrap
    BUILD_CONFIG=bootstrap-debug
    ;;
  no)
    bootstrap_suffix=no-bootstrap
    BUILD_CONFIG=
    ;;
esac

AC_MSG_CHECKING(for default BUILD_CONFIG)

AC_ARG_WITH([build-config],
  [AS_HELP_STRING([--with-build-config='NAME NAME2...'],
		  [use config/NAME.mk build configuration])],
  [case $with_build_config in
   yes) with_build_config= ;;
   no) with_build_config= BUILD_CONFIG= ;;
   esac])

if test "x${with_build_config}" != x; then
  BUILD_CONFIG=$with_build_config
else
  case $BUILD_CONFIG in
  bootstrap-debug)
    if echo "int f (void) { return 0; }" > conftest.c &&
       ${CC} -c conftest.c &&
       mv conftest.o conftest.o.g0 &&
       ${CC} -c -g conftest.c &&
       mv conftest.o conftest.o.g &&
       ${srcdir}/contrib/compare-debug conftest.o.g0 conftest.o.g > /dev/null 2>&1; then
      :
    else
      BUILD_CONFIG=
    fi
    rm -f conftest.c conftest.o conftest.o.g0 conftest.o.g
    ;;
  esac
fi
AC_MSG_RESULT($BUILD_CONFIG)
AC_SUBST(BUILD_CONFIG)

# Use same top-level configure hooks in libgcc/libstdc++/libvtv.
AC_MSG_CHECKING([for --enable-vtable-verify])
AC_ARG_ENABLE(vtable-verify,
[AS_HELP_STRING([--enable-vtable-verify],
		[Enable vtable verification feature])],
[case "$enableval" in
 yes) enable_vtable_verify=yes ;;
 no)  enable_vtable_verify=no ;;
 *)   enable_vtable_verify=no;;
 esac],
[enable_vtable_verify=no])
AC_MSG_RESULT($enable_vtable_verify)

# Record target_configdirs and the configure arguments for target and
# build configuration in Makefile.
target_configdirs=`echo "${target_configdirs}" | sed -e 's/target-//g'`
build_configdirs=`echo "${build_configdirs}" | sed -e 's/build-//g'`
bootstrap_fixincludes=no

# If we are building libgomp, bootstrap it.
if echo " ${target_configdirs} " | grep " libgomp " > /dev/null 2>&1 ; then
  bootstrap_target_libs=${bootstrap_target_libs}target-libgomp,
fi

# If we are building libsanitizer and $BUILD_CONFIG contains bootstrap-asan
# or bootstrap-ubsan, bootstrap it.
if echo " ${target_configdirs} " | grep " libsanitizer " > /dev/null 2>&1; then
  case "$BUILD_CONFIG" in
    *bootstrap-asan* | *bootstrap-ubsan* )
      bootstrap_target_libs=${bootstrap_target_libs}target-libsanitizer,
      bootstrap_fixincludes=yes
      ;;
  esac
fi

# If we are building libvtv and --enable-vtable-verify, bootstrap it.
if echo " ${target_configdirs} " | grep " libvtv " > /dev/null 2>&1 &&
   test "$enable_vtable_verify" != no; then
  bootstrap_target_libs=${bootstrap_target_libs}target-libvtv,
fi

# Determine whether gdb needs tk/tcl or not.
# Use 'maybe' since enable_gdbtk might be true even if tk isn't available
# and in that case we want gdb to be built without tk.  Ugh!
# In fact I believe gdb is the *only* package directly dependent on tk,
# so we should be able to put the 'maybe's in unconditionally and
# leave out the maybe dependencies when enable_gdbtk is false.  I'm not
# 100% sure that that's safe though.

gdb_tk="maybe-all-tcl maybe-all-tk maybe-all-itcl maybe-all-libgui"
case "$enable_gdbtk" in
  no)
    GDB_TK="" ;;
  yes)
    GDB_TK="${gdb_tk}" ;;
  *)
    # Only add the dependency on gdbtk when GDBtk is part of the gdb
    # distro.  Eventually someone will fix this and move Insight, nee
    # gdbtk to a separate directory.
    if test -d ${srcdir}/gdb/gdbtk ; then
      GDB_TK="${gdb_tk}"
    else
      GDB_TK=""
    fi
    ;;
esac
CONFIGURE_GDB_TK=`echo ${GDB_TK} | sed s/-all-/-configure-/g`
INSTALL_GDB_TK=`echo ${GDB_TK} | sed s/-all-/-install-/g`

# Strip out unwanted targets.

# While at that, we remove Makefiles if we were started for recursive
# configuration, so that the top-level Makefile reconfigures them,
# like we used to do when configure itself was recursive.

# Loop over modules.  We used to use the "$extrasub" feature from Autoconf
# but now we're fixing up the Makefile ourselves with the additional
# commands passed to AC_CONFIG_FILES.  Use separate variables
# extrasub-{build,host,target} not because there is any reason to split
# the substitutions up that way, but only to remain below the limit of
# 99 commands in a script, for HP-UX sed.
# Do not nest @if/@endif pairs, because configure will not warn you at all.


extrasub_build=
for module in ${build_configdirs} ; do
  if test -z "${no_recursion}" \
     && test -f ${build_subdir}/${module}/Makefile; then
    echo 1>&2 "*** removing ${build_subdir}/${module}/Makefile to force reconfigure"
    rm -f ${build_subdir}/${module}/Makefile
  fi
  extrasub_build="$extrasub_build
/^@if build-$module\$/d
/^@endif build-$module\$/d
/^@if build-$module-$bootstrap_suffix\$/d
/^@endif build-$module-$bootstrap_suffix\$/d"
done
extrasub_host=
for module in ${configdirs} ; do
  if test -z "${no_recursion}"; then
    for file in stage*-${module}/Makefile prev-${module}/Makefile ${module}/Makefile; do
      if test -f ${file}; then
	echo 1>&2 "*** removing ${file} to force reconfigure"
	rm -f ${file}
      fi
    done
  fi
  case ${module},${bootstrap_fixincludes} in
    fixincludes,no) host_bootstrap_suffix=no-bootstrap ;;
    *) host_bootstrap_suffix=$bootstrap_suffix ;;
  esac
  extrasub_host="$extrasub_host
/^@if $module\$/d
/^@endif $module\$/d
/^@if $module-$host_bootstrap_suffix\$/d
/^@endif $module-$host_bootstrap_suffix\$/d"
done
extrasub_target=
for module in ${target_configdirs} ; do
  if test -z "${no_recursion}" \
     && test -f ${target_subdir}/${module}/Makefile; then
    echo 1>&2 "*** removing ${target_subdir}/${module}/Makefile to force reconfigure"
    rm -f ${target_subdir}/${module}/Makefile
  fi

  # We only bootstrap target libraries listed in bootstrap_target_libs.
  case $bootstrap_target_libs in
    *,target-$module,*) target_bootstrap_suffix=$bootstrap_suffix ;;
    *) target_bootstrap_suffix=no-bootstrap ;;
  esac

  extrasub_target="$extrasub_target
/^@if target-$module\$/d
/^@endif target-$module\$/d
/^@if target-$module-$target_bootstrap_suffix\$/d
/^@endif target-$module-$target_bootstrap_suffix\$/d"
done

# Do the final fixup along with target modules.
extrasub_target="$extrasub_target
/^@if /,/^@endif /d"

# Create the serialization dependencies.  This uses a temporary file.

AC_ARG_ENABLE([serial-configure],
[AS_HELP_STRING([[--enable-serial-[{host,target,build}-]configure]],
		[force sequential configuration of
		 sub-packages for the host, target or build
		 machine, or all sub-packages])])

case ${enable_serial_configure} in
  yes)
    enable_serial_build_configure=yes
    enable_serial_host_configure=yes
    enable_serial_target_configure=yes
    ;;
esac

# These force 'configure's to be done one at a time, to avoid problems
# with contention over a shared config.cache.
rm -f serdep.tmp
echo '# serdep.tmp' > serdep.tmp
olditem=
test "x${enable_serial_build_configure}" = xyes &&
for item in ${build_configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-build-${item}: configure-build-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
olditem=
test "x${enable_serial_host_configure}" = xyes &&
for item in ${configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-${item}: configure-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
olditem=
test "x${enable_serial_target_configure}" = xyes &&
for item in ${target_configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-target-${item}: configure-target-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
serialization_dependencies=serdep.tmp
AC_SUBST_FILE(serialization_dependencies)

# Base args.  Strip norecursion, cache-file, srcdir, host, build,
# target, nonopt, and variable assignments.  These are the ones we
# might not want to pass down to subconfigures.  The exception being
# --cache-file=/dev/null, which is used to turn off the use of cache
# files altogether, and which should be passed on to subconfigures.
# Also strip program-prefix, program-suffix, and program-transform-name,
# so that we can pass down a consistent program-transform-name.
baseargs=
tbaseargs=
keep_next=no
skip_next=no
eval "set -- $ac_configure_args"
for ac_arg
do
  if test X"$skip_next" = X"yes"; then
    skip_next=no
    continue
  fi
  if test X"$keep_next" = X"yes"; then
    case $ac_arg in
      *\'*)
	ac_arg=`echo "$ac_arg" | sed "s/'/'\\\\\\\\''/g"` ;;
    esac
    baseargs="$baseargs '$ac_arg'"
    tbaseargs="$tbaseargs '$ac_arg'"
    keep_next=no
    continue
  fi

  # Handle separated arguments.  Based on the logic generated by
  # autoconf 2.59.
  case $ac_arg in
    *=* | --config-cache | -C | -disable-* | --disable-* \
      | -enable-* | --enable-* | -gas | --g* | -nfp | --nf* \
      | -q | -quiet | --q* | -silent | --sil* | -v | -verb* \
      | -with-* | --with-* | -without-* | --without-* | --x)
      separate_arg=no
      ;;
    -*)
      separate_arg=yes
      ;;
    *)
      separate_arg=no
      ;;
  esac

  skip_targ=no
  case $ac_arg in
changequote(,)
  --with-* | --without-*)
    libopt=`echo "$ac_arg" | sed -e 's,^--[^-_]*[-_],,' -e 's,=.*$,,'`

    case $libopt in
    *[-_]include)
      lib=`echo "$libopt" | sed 's,[-_]include$,,'`
      ;;
    *[-_]lib)
      lib=`echo "$libopt" | sed 's,[-_]lib$,,'`
      ;;
    *)
      lib=$libopt
      ;;
    esac
changequote([,])

    case $lib in
    mpc | mpfr | gmp | isl)
      # If we're processing --with-$lib, --with-$lib-include or
      # --with-$lib-lib, for one of the libs above, and target is
      # different from host, don't pass the current argument to any
      # target library's configure.
      if test x$is_cross_compiler = xyes; then
        skip_targ=yes
      fi
      ;;
    esac
    ;;
  esac

  case "$ac_arg" in
    --cache-file=/dev/null | \
    -cache-file=/dev/null )
      # Handled here to avoid the test to skip args below.
      baseargs="$baseargs '$ac_arg'"
      tbaseargs="$tbaseargs '$ac_arg'"
      # Assert: $separate_arg should always be no.
      keep_next=$separate_arg
      ;;
    --no*)
      continue
      ;;
    --c* | \
    --sr* | \
    --ho* | \
    --bu* | \
    --t* | \
    --program-* | \
    -cache_file* | \
    -srcdir* | \
    -host* | \
    -build* | \
    -target* | \
    -program-prefix* | \
    -program-suffix* | \
    -program-transform-name* )
      skip_next=$separate_arg
      continue
      ;;
    -*)
      # An option.  Add it.
      case $ac_arg in
	*\'*)
	  ac_arg=`echo "$ac_arg" | sed "s/'/'\\\\\\\\''/g"` ;;
      esac
      baseargs="$baseargs '$ac_arg'"
      if test X"$skip_targ" = Xno; then
        tbaseargs="$tbaseargs '$ac_arg'"
      fi
      keep_next=$separate_arg
      ;;
    *)
      # Either a variable assignment, or a nonopt (triplet).  Don't
      # pass it down; let the Makefile handle this.
      continue
      ;;
  esac
done
# Remove the initial space we just introduced and, as these will be
# expanded by make, quote '$'.
baseargs=`echo "x$baseargs" | sed -e 's/^x *//' -e 's,\\$,$$,g'`

# Add in --program-transform-name, after --program-prefix and
# --program-suffix have been applied to it.  Autoconf has already
# doubled dollar signs and backslashes in program_transform_name; we want
# the backslashes un-doubled, and then the entire thing wrapped in single
# quotes, because this will be expanded first by make and then by the shell. 
# Also, because we want to override the logic in subdir configure scripts to
# choose program_transform_name, replace any s,x,x, with s,y,y,.
sed -e "s,\\\\\\\\,\\\\,g; s,','\\\\'',g; s/s,x,x,/s,y,y,/" <<EOF_SED > conftestsed.out
${program_transform_name}
EOF_SED
gcc_transform_name=`cat conftestsed.out`
rm -f conftestsed.out
baseargs="$baseargs --program-transform-name='${gcc_transform_name}'"
tbaseargs="$tbaseargs --program-transform-name='${gcc_transform_name}'"
if test "$silent" = yes; then
  baseargs="$baseargs --silent"
  tbaseargs="$tbaseargs --silent"
fi
baseargs="$baseargs --disable-option-checking"
tbaseargs="$tbaseargs --disable-option-checking"

# Record and document user additions to sub configure arguments.
AC_ARG_VAR([build_configargs],
	   [additional configure arguments for build directories])
AC_ARG_VAR([host_configargs],
	   [additional configure arguments for host directories])
AC_ARG_VAR([target_configargs],
	   [additional configure arguments for target directories])

# For the build-side libraries, we just need to pretend we're native,
# and not use the same cache file.  Multilibs are neither needed nor
# desired.  We can't even use the same cache file for all build-side
# libraries, as they're compiled differently; some with C, some with
# C++ or with different feature-enabling options.
build_configargs="$build_configargs --cache-file=./config.cache ${baseargs}"

# For host modules, accept cache file option, or specification as blank.
case "${cache_file}" in
"") # empty
  cache_file_option="" ;;
/* | [[A-Za-z]]:[[\\/]]* ) # absolute path
  cache_file_option="--cache-file=${cache_file}" ;;
*) # relative path
  cache_file_option="--cache-file=../${cache_file}" ;;
esac

# Host dirs don't like to share a cache file either, horribly enough.
# This seems to be due to autoconf 2.5x stupidity.
host_configargs="$host_configargs --cache-file=./config.cache ${extra_host_args} ${baseargs}"

target_configargs="$target_configargs ${tbaseargs}"

# Passing a --with-cross-host argument lets the target libraries know
# whether they are being built with a cross-compiler or being built
# native.  However, it would be better to use other mechanisms to make the
# sorts of decisions they want to make on this basis.  Please consider
# this option to be deprecated.  FIXME.
if test x${is_cross_compiler} = xyes ; then
  target_configargs="--with-cross-host=${host_noncanonical} ${target_configargs}"
fi

# Special user-friendly check for native x86_64-linux build, if
# multilib is not explicitly enabled.
case "$target:$have_compiler:$host:$target:$enable_multilib" in
  x86_64-*linux*:yes:$build:$build:)
    # Make sure we have a development environment that handles 32-bit
    dev64=no
    echo "int main () { return 0; }" > conftest.c
    ${CC} -m32 -o conftest ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} conftest.c
    if test $? = 0 ; then
      if test -s conftest || test -s conftest.exe ; then
	dev64=yes
      fi
    fi 
    rm -f conftest*
    if test x${dev64} != xyes ; then
      AC_MSG_ERROR([I suspect your system does not have 32-bit development libraries (libc and headers). If you have them, rerun configure with --enable-multilib. If you do not have them, and want to build a 64-bit-only compiler, rerun configure with --disable-multilib.])
    fi
    ;;
esac

# Default to --enable-multilib.
if test x${enable_multilib} = x ; then
  target_configargs="--enable-multilib ${target_configargs}"
fi

# Different target subdirs use different values of certain variables
# (notably CXX).  Worse, multilibs use *lots* of different values.
# Worse yet, autoconf 2.5x makes some of these 'precious', meaning that
# it doesn't automatically accept command-line overrides of them.
# This means it's not safe for target subdirs to share a cache file,
# which is disgusting, but there you have it.  Hopefully this can be
# fixed in future.  It's still worthwhile to use a cache file for each
# directory.  I think.

# Pass the appropriate --build, --host, --target and --cache-file arguments.
# We need to pass --target, as newer autoconf's requires consistency
# for target_alias and gcc doesn't manage it consistently.
target_configargs="--cache-file=./config.cache ${target_configargs}"

FLAGS_FOR_TARGET=

case "$target" in
  x86_64-*mingw* | *-w64-mingw*)
  # MinGW-w64 does not use newlib, nor does it use winsup. It may,
  # however, use a symlink named 'mingw' in ${prefix} .
    FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -L${prefix}/${target}/lib -L${prefix}/mingw/lib -isystem ${prefix}/${target}/include -isystem ${prefix}/mingw/include'
    ;;
  *-mingw*)
  # MinGW can't be handled as Cygwin above since it does not use newlib.
    FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -L$$r/$(TARGET_SUBDIR)/winsup/mingw -L$$r/$(TARGET_SUBDIR)/winsup/w32api/lib -isystem $$s/winsup/mingw/include -isystem $$s/winsup/w32api/include'
    ;;
esac

# Allow the user to override the flags for
# our build compiler if desired.
if test x"${build}" = x"${host}" ; then
  CFLAGS_FOR_BUILD=${CFLAGS_FOR_BUILD-${CFLAGS}}
  LDFLAGS_FOR_BUILD=${LDFLAGS_FOR_BUILD-${LDFLAGS}}
fi

# On Canadian crosses, we'll be searching the right directories for
# the previously-installed cross compiler, so don't bother to add
# flags for directories within the install tree of the compiler
# being built; programs in there won't even run.
if test "${build}" = "${host}" && test -d ${srcdir}/gcc; then
  # Search for pre-installed headers if nothing else fits.
  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$(build_tooldir)/bin/ -B$(build_tooldir)/lib/ -isystem $(build_tooldir)/include -isystem $(build_tooldir)/sys-include'
fi

if test "x${use_gnu_ld}" = x &&
   echo " ${configdirs} " | grep " ld " > /dev/null ; then
  # Arrange for us to find uninstalled linker scripts.
  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -L$$r/$(HOST_SUBDIR)/ld'
fi

# Search for other target-specific linker scripts and such.
case "${target}" in
  mep*)
    FLAGS_FOR_TARGET="$FLAGS_FOR_TARGET -mlibrary"
    ;;
esac

# Makefile fragments.
for frag in host_makefile_frag target_makefile_frag alphaieee_frag ospace_frag;
do
  eval fragval=\$$frag
  if test $fragval != /dev/null; then
    eval $frag=${srcdir}/$fragval
  fi
done
AC_SUBST_FILE(host_makefile_frag)
AC_SUBST_FILE(target_makefile_frag)
AC_SUBST_FILE(alphaieee_frag)
AC_SUBST_FILE(ospace_frag)

# Miscellanea: directories, flags, etc.
AC_SUBST(RPATH_ENVVAR)
AC_SUBST(GCC_SHLIB_SUBDIR)
AC_SUBST(tooldir)
AC_SUBST(build_tooldir)

# Build module lists & subconfigure args.
AC_SUBST(build_configargs)
AC_SUBST(build_configdirs)

# Host module lists & subconfigure args.
AC_SUBST(host_configargs)
AC_SUBST(configdirs)
AC_SUBST(target_configdirs)

# Target module lists & subconfigure args.
AC_SUBST(target_configargs)


# Build tools.
AC_SUBST(AR_FOR_BUILD)
AC_SUBST(AS_FOR_BUILD)
AC_SUBST(CC_FOR_BUILD)
AC_SUBST(CFLAGS_FOR_BUILD)
AC_SUBST(DLLTOOL_FOR_BUILD)
AC_SUBST(LDFLAGS_FOR_BUILD)
AC_SUBST(LD_FOR_BUILD)
AC_SUBST(NM_FOR_BUILD)
AC_SUBST(RANLIB_FOR_BUILD)

# Generate default definitions for YACC, M4, LEX and other programs that run
# on the build machine.  These are used if the Makefile can't locate these
# programs in objdir.
MISSING=`cd $ac_aux_dir && ${PWDCMD-pwd}`/missing


AC_CHECK_PROGS([M4], [gm4 gnum4 m4], [$MISSING m4])
case " $build_configdirs " in
  *" m4 "*) M4='$$r/$(BUILD_SUBDIR)/m4/m4' ;;
esac

# Host tools.
NCN_STRICT_CHECK_TOOLS(AR, ar)
NCN_STRICT_CHECK_TOOLS(AS, as)
NCN_STRICT_CHECK_TOOLS(DLLTOOL, dlltool)
NCN_STRICT_CHECK_TOOLS(LD, ld)
NCN_STRICT_CHECK_TOOLS(LIPO, lipo)
NCN_STRICT_CHECK_TOOLS(NM, nm)
NCN_STRICT_CHECK_TOOLS(RANLIB, ranlib, true)
NCN_STRICT_CHECK_TOOLS(STRIP, strip, true)
NCN_STRICT_CHECK_TOOLS(OBJCOPY, objcopy)
NCN_STRICT_CHECK_TOOLS(OBJDUMP, objdump)
NCN_STRICT_CHECK_TOOLS(READELF, readelf)
AC_SUBST(CC)
AC_SUBST(CFLAGS)

# Target tools.
AC_ARG_WITH([build-time-tools], 
  [AS_HELP_STRING([--with-build-time-tools=PATH],
		  [use given path to find target tools during the build])],
  [case x"$withval" in
     x/*) ;;
     *)
       with_build_time_tools=
       AC_MSG_WARN([argument to --with-build-time-tools must be an absolute path])
       ;;
   esac],
  [with_build_time_tools=])

NCN_STRICT_CHECK_TARGET_TOOLS(CC_FOR_TARGET, cc gcc)
NCN_STRICT_CHECK_TARGET_TOOLS(GCC_FOR_TARGET, gcc, ${CC_FOR_TARGET})

ACX_CHECK_INSTALLED_TARGET_TOOL(AR_FOR_TARGET, ar)
ACX_CHECK_INSTALLED_TARGET_TOOL(AS_FOR_TARGET, as)
ACX_CHECK_INSTALLED_TARGET_TOOL(DLLTOOL_FOR_TARGET, dlltool)
ACX_CHECK_INSTALLED_TARGET_TOOL(LD_FOR_TARGET, ld)
ACX_CHECK_INSTALLED_TARGET_TOOL(LIPO_FOR_TARGET, lipo)
ACX_CHECK_INSTALLED_TARGET_TOOL(NM_FOR_TARGET, nm)
ACX_CHECK_INSTALLED_TARGET_TOOL(OBJCOPY_FOR_TARGET, objcopy)
ACX_CHECK_INSTALLED_TARGET_TOOL(OBJDUMP_FOR_TARGET, objdump)
ACX_CHECK_INSTALLED_TARGET_TOOL(RANLIB_FOR_TARGET, ranlib)
ACX_CHECK_INSTALLED_TARGET_TOOL(READELF_FOR_TARGET, readelf)
ACX_CHECK_INSTALLED_TARGET_TOOL(STRIP_FOR_TARGET, strip)

GCC_TARGET_TOOL(ar, AR_FOR_TARGET, AR, [binutils/ar])
GCC_TARGET_TOOL(as, AS_FOR_TARGET, AS, [gas/as-new])
GCC_TARGET_TOOL(cc, CC_FOR_TARGET, CC, [gcc/xgcc -B$$r/$(HOST_SUBDIR)/gcc/])
GCC_TARGET_TOOL(dlltool, DLLTOOL_FOR_TARGET, DLLTOOL, [binutils/dlltool])
GCC_TARGET_TOOL(gcc, GCC_FOR_TARGET, , [gcc/xgcc -B$$r/$(HOST_SUBDIR)/gcc/])
GCC_TARGET_TOOL(ld, LD_FOR_TARGET, LD, [ld/ld-new])
GCC_TARGET_TOOL(lipo, LIPO_FOR_TARGET, LIPO)
GCC_TARGET_TOOL(nm, NM_FOR_TARGET, NM, [binutils/nm-new])
GCC_TARGET_TOOL(objcopy, OBJCOPY_FOR_TARGET, OBJCOPY, [binutils/objcopy])
GCC_TARGET_TOOL(objdump, OBJDUMP_FOR_TARGET, OBJDUMP, [binutils/objdump])
GCC_TARGET_TOOL(ranlib, RANLIB_FOR_TARGET, RANLIB, [binutils/ranlib])
GCC_TARGET_TOOL(readelf, READELF_FOR_TARGET, READELF, [binutils/readelf])
GCC_TARGET_TOOL(strip, STRIP_FOR_TARGET, STRIP, [binutils/strip-new])

AC_SUBST(FLAGS_FOR_TARGET)

# Certain tools may need extra flags.
AR_FOR_TARGET=${AR_FOR_TARGET}${extra_arflags_for_target}
RANLIB_FOR_TARGET=${RANLIB_FOR_TARGET}${extra_ranlibflags_for_target}
NM_FOR_TARGET=${NM_FOR_TARGET}${extra_nmflags_for_target}

# When building target libraries, except in a Canadian cross, we use
# the same toolchain as the compiler we just built.
COMPILER_AS_FOR_TARGET='$(AS_FOR_TARGET)'
COMPILER_LD_FOR_TARGET='$(LD_FOR_TARGET)'
COMPILER_NM_FOR_TARGET='$(NM_FOR_TARGET)'
if test $host = $build; then
  case " $configdirs " in
    *" gcc "*)
      COMPILER_AS_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/as'
      COMPILER_LD_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/collect-ld'
      COMPILER_NM_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/nm'${extra_nmflags_for_target}
      ;;
  esac
fi

AC_SUBST(COMPILER_AS_FOR_TARGET)
AC_SUBST(COMPILER_LD_FOR_TARGET)
AC_SUBST(COMPILER_NM_FOR_TARGET)

  MAINTAINER_MODE_TRUE='#'
  MAINTAINER_MODE_FALSE=
MAINT=$MAINTAINER_MODE_TRUE
AC_SUBST(MAINT)dnl

# ---------------------
# GCC bootstrap support
# ---------------------

# Stage specific cflags for build.
stage1_cflags="-g"
case $build in
  vax-*-*)
    case ${GCC} in
      yes) stage1_cflags="-g -Wa,-J" ;;
      *) stage1_cflags="-g -J" ;;
    esac ;;
esac

AC_SUBST(stage1_cflags)

# Enable --enable-checking in stage1 of the compiler.
AC_ARG_ENABLE(stage1-checking,
[AS_HELP_STRING([[--enable-stage1-checking[=all]]],
		[choose additional checking for stage1 of the compiler])],
[stage1_checking=--enable-checking=${enable_stage1_checking}],
[if test "x$enable_checking" = xno || test "x$enable_checking" = x; then
  # For --disable-checking or implicit --enable-checking=release, avoid
  # setting --enable-checking=gc in the default stage1 checking for LTO
  # bootstraps.  See PR62077.
  case $BUILD_CONFIG in
    *lto*)
      stage1_checking=--enable-checking=release,misc,gimple,rtlflag,tree,types;;
    *)
      stage1_checking=--enable-checking=yes,types;;
  esac
  if test "x$enable_checking" = x && \
     test -d ${srcdir}/gcc && \
     test x"`cat ${srcdir}/gcc/DEV-PHASE`" = xexperimental; then
    stage1_checking=--enable-checking=yes,types,extra
  fi
else
  stage1_checking=--enable-checking=$enable_checking,types
fi])
AC_SUBST(stage1_checking)

# Enable -Werror in bootstrap stage2 and later.
AC_ARG_ENABLE(werror,
[AS_HELP_STRING([--enable-werror],
		[enable -Werror in bootstrap stage2 and later])],
[
case ${enable_werror} in
  yes) stage2_werror_flag="--enable-werror-always" ;;
  *) stage2_werror_flag="" ;;
esac
],
[
if test -d ${srcdir}/gcc && test x"`cat $srcdir/gcc/DEV-PHASE`" = xexperimental; then
  case $BUILD_CONFIG in
  bootstrap-debug)
      stage2_werror_flag="--enable-werror-always" ;;
  "")
      stage2_werror_flag="--enable-werror-always" ;;
  esac
fi
])

AC_SUBST(stage2_werror_flag)

# Specify what files to not compare during bootstrap.

compare_exclusions="gcc/cc*-checksum\$(objext) | gcc/ada/*tools/*"
case "$target" in
  hppa*64*-*-hpux*) ;;
  hppa*-*-hpux*) compare_exclusions="gcc/cc*-checksum\$(objext) | */libgcc/lib2funcs* | gcc/ada/*tools/* | gcc/function-tests.o" ;;
  powerpc*-ibm-aix*) compare_exclusions="gcc/cc*-checksum\$(objext) | gcc/ada/*tools/* | *libgomp*\$(objext)" ;;
esac
AC_SUBST(compare_exclusions)

AC_CONFIG_FILES([Makefile],
  [sed "$extrasub_build" Makefile |
   sed "$extrasub_host" |
   sed "$extrasub_target" > mf$$
   mv -f mf$$ Makefile],
  [extrasub_build="$extrasub_build"
   extrasub_host="$extrasub_host"
   extrasub_target="$extrasub_target"])
AC_OUTPUT
