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
AC_INIT([opcodes], BFD_VERSION)
m4_include([../config/lean.m4])

AC_CANONICAL_TARGET
AC_ISC_POSIX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_GNU_SOURCE
AC_USE_SYSTEM_EXTENSIONS

dnl These must be called before LT_INIT, because it may want
dnl to call AC_CHECK_PROG.
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib, :)

dnl Default to a non shared library.  This may be overridden by the
dnl configure option --enable-shared.
#AC_DISABLE_SHARED

#LT_INIT

AC_ARG_ENABLE(targets,
[  --enable-targets        alternative target configurations],
[case "${enableval}" in
  yes | "") AC_MSG_ERROR([enable-targets option must specify target names or 'all'])
            ;;
  no)       enable_targets= ;;
  *)        enable_targets=$enableval ;;
esac])dnl

AM_BINUTILS_WARNINGS
ACX_PROG_CC_WARNING_OPTS([-Wno-missing-field-initializers],
			 [NO_WMISSING_FIELD_INITIALIZERS])

AC_CONFIG_HEADERS(config.h:config.in)

# PR 14072
AH_VERBATIM([00_CONFIG_H_CHECK],
[/* Check that config.h is #included before system headers
   (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
#  error config.h must be #included before system headers
#endif
#define __CONFIG_H__ 1])

if test -z "$target" ; then
    AC_MSG_ERROR(Unrecognized target system type; please check config.sub.)
fi

AM_MAINTAINER_MODE
AM_INSTALL_LIBBFD
AC_EXEEXT

. ${srcdir}/../bfd/configure.host

BFD_CC_FOR_BUILD

AC_SUBST(HDEFINES)
AC_PROG_INSTALL

ACX_HEADER_STRING

AC_CHECK_DECLS([basename, stpcpy])

# Check if sigsetjmp is available.  Using AC_CHECK_FUNCS won't do
# since sigsetjmp might only be defined as a macro.
AC_CACHE_CHECK([for sigsetjmp], gdb_cv_func_sigsetjmp,
[AC_TRY_COMPILE([
#include <setjmp.h>
], [sigjmp_buf env; while (! sigsetjmp (env, 1)) siglongjmp (env, 1);],
bfd_cv_func_sigsetjmp=yes, bfd_cv_func_sigsetjmp=no)])
if test $bfd_cv_func_sigsetjmp = yes; then
  AC_DEFINE(HAVE_SIGSETJMP, 1, [Define if sigsetjmp is available. ])
fi

cgen_maint=no
cgendir='$(srcdir)/../cgen'

AC_ARG_ENABLE(cgen-maint,
[  --enable-cgen-maint[=dir]    build cgen generated files],
[case "${enableval}" in
  yes)	cgen_maint=yes ;;
  no)	cgen_maint=no ;;
  *)
	# argument is cgen install directory (not implemented yet).
	# Having a `share' directory might be more appropriate for the .scm,
	# .cpu, etc. files.
	cgen_maint=yes
	cgendir=${cgen_maint}/lib/cgen
	;;
esac])dnl
AM_CONDITIONAL(CGEN_MAINT, test x${cgen_maint} = xyes)
AC_SUBST(cgendir)

using_cgen=no

# Check if linker supports --as-needed and --no-as-needed options
AC_CACHE_CHECK(linker --as-needed support, bfd_cv_ld_as_needed,
	[bfd_cv_ld_as_needed=no
	if $LD --help 2>/dev/null | grep as-needed > /dev/null; then
		bfd_cv_ld_as_needed=yes
	fi
	])

#LT_LIB_M

#Libs for generator progs
if test "x$cross_compiling" = "xno"; then
  BUILD_LIBS=../libiberty/libiberty.a
  BUILD_LIB_DEPS=$BUILD_LIBS
else
  # if cross-compiling, assume that the system provides -liberty
  # and that the version is compatible with new headers.
  BUILD_LIBS=-liberty
  BUILD_LIB_DEPS=
fi
BUILD_LIBS="$BUILD_LIBS $LIBINTL"
BUILD_LIB_DEPS="$BUILD_LIB_DEPS $LIBINTL_DEP"

AC_SUBST(BUILD_LIBS)
AC_SUBST(BUILD_LIB_DEPS)

# Horrible hacks to build DLLs on Windows and a shared library elsewhere.
SHARED_LDFLAGS=
SHARED_LIBADD=
SHARED_DEPENDENCIES=
if test "$enable_shared" = "yes"; then
# When building a shared libopcodes, link against the pic version of libiberty
# so that apps that use libopcodes won't need libiberty just to satisfy any
# libopcodes references.
# We can't do that if a pic libiberty is unavailable since including non-pic
# code would insert text relocations into libopcodes.
# Note that linking against libbfd as we do here, which is itself linked
# against libiberty, may not satisfy all the libopcodes libiberty references
# since libbfd may not pull in the entirety of libiberty.
changequote(,)dnl
  x=`sed -n -e 's/^[ 	]*PICFLAG[ 	]*=[ 	]*//p' < ../libiberty/Makefile | sed -n '$p'`
changequote([,])dnl
  if test -n "$x"; then
    SHARED_LIBADD="-L`pwd`/../libiberty/pic -liberty"
  fi

  case "${host}" in
    *-*-cygwin*)
      SHARED_LDFLAGS="-no-undefined"
      SHARED_LIBADD="-L`pwd`/../bfd -lbfd -L`pwd`/../libiberty -liberty -L`pwd`/../intl -lintl -lcygwin"
      ;;
   *-*-darwin*)
     SHARED_LIBADD="-Wl,`pwd`/../bfd/.libs/libbfd.dylib ${SHARED_LIBADD}"
     SHARED_DEPENDENCIES="../bfd/libbfd.la"
     ;;
    *)
      case "$host_vendor" in
        hp)
          SHARED_LIBADD="-Wl,`pwd`/../bfd/.libs/libbfd.sl ${SHARED_LIBADD}"
	  ;;
	*)
          SHARED_LIBADD="-Wl,`pwd`/../bfd/.libs/libbfd.so ${SHARED_LIBADD}"
	  ;;
      esac
      SHARED_DEPENDENCIES="../bfd/libbfd.la"
      ;;
  esac

  if test -n "$SHARED_LIBADD"; then
    if test -n "$LIBM"; then
      if test x"$bfd_cv_ld_as_needed" = xyes; then
	# Link against libm only when needed.  Put -lc, -lm inside -Wl
	# to stop libtool reordering these options.
	SHARED_LIBADD="$SHARED_LIBADD -Wl,-lc,--as-needed,`echo $LIBM | sed 's/ /,/g'`,--no-as-needed"
      else
	SHARED_LIBADD="$SHARED_LIBADD $LIBM"
      fi
    fi
  fi
fi
AC_SUBST(SHARED_LDFLAGS)
AC_SUBST(SHARED_LIBADD)
AC_SUBST(SHARED_DEPENDENCIES)

# target-specific stuff:

# Canonicalize the secondary target names.
if test -n "$enable_targets" ; then
    for targ in `echo $enable_targets | sed 's/,/ /g'`
    do
	result=`$ac_config_sub $targ 2>/dev/null`
	if test -n "$result" ; then
	    canon_targets="$canon_targets $result"
	else
	    # Allow targets that config.sub doesn't recognize, like "all".
	    canon_targets="$canon_targets $targ"
	fi
    done
fi

all_targets=false
selarchs=
for targ in $target $canon_targets
do
    if test "x$targ" = "xall" ; then
        all_targets=true
    else
	. $srcdir/../bfd/config.bfd
	selarchs="$selarchs $targ_archs"
    fi
done

# Utility var, documents generic cgen support files.

cgen_files="cgen-opc.o cgen-asm.o cgen-dis.o cgen-bitset.o"

# We don't do any links based on the target system, just makefile config.

if test x${all_targets} = xfalse ; then

    # Target architecture .o files.
    ta=

    for arch in $selarchs
    do
	ad=`echo $arch | sed -e s/bfd_//g -e s/_arch//g`
	archdefs="$archdefs -DARCH_$ad"
	case "$arch" in
	bfd_aarch64_arch)	ta="$ta aarch64-asm.o aarch64-dis.o aarch64-opc.o aarch64-asm-2.o aarch64-dis-2.o aarch64-opc-2.o" ;;
	bfd_alpha_arch)		ta="$ta alpha-dis.o alpha-opc.o" ;;
	bfd_arc_arch)		ta="$ta arc-dis.o arc-opc.o arc-ext.o" ;;
	bfd_arm_arch)		ta="$ta arm-dis.o" ;;
	bfd_avr_arch)		ta="$ta avr-dis.o" ;;
	bfd_bfin_arch)		ta="$ta bfin-dis.o" ;;
	bfd_cr16_arch)		ta="$ta cr16-dis.o cr16-opc.o" ;;
	bfd_cris_arch)		ta="$ta cris-dis.o cris-opc.o cgen-bitset.o" ;;
	bfd_crx_arch)		ta="$ta crx-dis.o crx-opc.o" ;;
	bfd_d10v_arch)		ta="$ta d10v-dis.o d10v-opc.o" ;;
	bfd_d30v_arch)		ta="$ta d30v-dis.o d30v-opc.o" ;;
	bfd_dlx_arch)		ta="$ta dlx-dis.o" ;;
	bfd_fr30_arch)		ta="$ta fr30-asm.o fr30-desc.o fr30-dis.o fr30-ibld.o fr30-opc.o" using_cgen=yes ;;
	bfd_frv_arch)		ta="$ta frv-asm.o frv-desc.o frv-dis.o frv-ibld.o frv-opc.o" using_cgen=yes ;;
	bfd_ft32_arch)		ta="$ta ft32-opc.o ft32-dis.o" ;;
	bfd_moxie_arch)		ta="$ta moxie-dis.o moxie-opc.o" ;;
	bfd_h8300_arch)		ta="$ta h8300-dis.o" ;;
	bfd_hppa_arch)		ta="$ta hppa-dis.o" ;;
	bfd_i386_arch|bfd_iamcu_arch|bfd_l1om_arch|bfd_k1om_arch)
				ta="$ta i386-dis.o i386-opc.o" ;;
	bfd_ia64_arch)		ta="$ta ia64-dis.o ia64-opc.o" ;;
	bfd_ip2k_arch)		ta="$ta ip2k-asm.o ip2k-desc.o ip2k-dis.o ip2k-ibld.o ip2k-opc.o" using_cgen=yes ;;
	bfd_epiphany_arch)	ta="$ta epiphany-asm.o epiphany-desc.o epiphany-dis.o epiphany-ibld.o epiphany-opc.o" using_cgen=yes ;;
	bfd_iq2000_arch)	ta="$ta iq2000-asm.o iq2000-desc.o iq2000-dis.o iq2000-ibld.o iq2000-opc.o" using_cgen=yes ;;
	bfd_lm32_arch)		ta="$ta lm32-asm.o lm32-desc.o lm32-dis.o lm32-ibld.o lm32-opc.o lm32-opinst.o" using_cgen=yes ;;
	bfd_m32c_arch)		ta="$ta m32c-asm.o m32c-desc.o m32c-dis.o m32c-ibld.o m32c-opc.o" using_cgen=yes ;;
	bfd_m32r_arch)		ta="$ta m32r-asm.o m32r-desc.o m32r-dis.o m32r-ibld.o m32r-opc.o m32r-opinst.o" using_cgen=yes ;;
	bfd_m68hc11_arch)	ta="$ta m68hc11-dis.o m68hc11-opc.o" ;;
	bfd_m68hc12_arch)	ta="$ta m68hc11-dis.o m68hc11-opc.o" ;;
	bfd_m9s12x_arch)	ta="$ta m68hc11-dis.o m68hc11-opc.o" ;;
	bfd_m9s12xg_arch)	ta="$ta m68hc11-dis.o m68hc11-opc.o" ;;
	bfd_s12z_arch)	ta="$ta s12z-dis.o m68hc11-opc.o" ;;
	bfd_m68k_arch)		ta="$ta m68k-dis.o m68k-opc.o" ;;
	bfd_mcore_arch)		ta="$ta mcore-dis.o" ;;
	bfd_mep_arch)		ta="$ta mep-asm.o mep-desc.o mep-dis.o mep-ibld.o mep-opc.o" using_cgen=yes ;;
	bfd_metag_arch)		ta="$ta metag-dis.o" ;;
	bfd_microblaze_arch)	ta="$ta microblaze-dis.o" ;;
	bfd_mips_arch)		ta="$ta mips-dis.o mips-opc.o mips16-opc.o micromips-opc.o" ;;
	bfd_mmix_arch)		ta="$ta mmix-dis.o mmix-opc.o" ;;
	bfd_mn10200_arch)	ta="$ta m10200-dis.o m10200-opc.o" ;;
	bfd_mn10300_arch)	ta="$ta m10300-dis.o m10300-opc.o" ;;
	bfd_mt_arch)		ta="$ta mt-asm.o mt-desc.o mt-dis.o mt-ibld.o mt-opc.o" using_cgen=yes ;;
	bfd_msp430_arch)	ta="$ta msp430-dis.o msp430-decode.o" ;;
	bfd_nds32_arch)		ta="$ta nds32-asm.o nds32-dis.o" ;;
	bfd_nfp_arch)		ta="$ta nfp-dis.o" ;;
	bfd_nios2_arch)		ta="$ta nios2-dis.o nios2-opc.o" ;;
	bfd_ns32k_arch)		ta="$ta ns32k-dis.o" ;;
	bfd_or1k_arch)		ta="$ta or1k-asm.o or1k-desc.o or1k-dis.o or1k-ibld.o or1k-opc.o" using_cgen=yes ;;
	bfd_pdp11_arch)		ta="$ta pdp11-dis.o pdp11-opc.o" ;;
	bfd_pj_arch)		ta="$ta pj-dis.o pj-opc.o" ;;
	bfd_powerpc_arch)	ta="$ta ppc-dis.o ppc-opc.o" ;;
	bfd_powerpc_64_arch)	ta="$ta ppc-dis.o ppc-opc.o" ;;
	bfd_pru_arch)		ta="$ta pru-dis.o pru-opc.o" ;;
	bfd_pyramid_arch)	;;
	bfd_romp_arch)		;;
	bfd_riscv_arch)         ta="$ta riscv-dis.o riscv-opc.o" ;;
	bfd_rs6000_arch)	ta="$ta ppc-dis.o ppc-opc.o" ;;
	bfd_rl78_arch)		ta="$ta rl78-dis.o rl78-decode.o";;
	bfd_rx_arch)		ta="$ta rx-dis.o rx-decode.o";;
	bfd_s390_arch)		ta="$ta s390-dis.o s390-opc.o" ;;
	bfd_score_arch)		ta="$ta score-dis.o score7-dis.o" ;;
	bfd_sh_arch)		ta="$ta sh-dis.o cgen-bitset.o" ;;
	bfd_sparc_arch)		ta="$ta sparc-dis.o sparc-opc.o" ;;
	bfd_spu_arch)		ta="$ta spu-dis.o spu-opc.o" ;;
	bfd_tic30_arch)		ta="$ta tic30-dis.o" ;;
	bfd_tic4x_arch)		ta="$ta tic4x-dis.o" ;;
	bfd_tic54x_arch)	ta="$ta tic54x-dis.o tic54x-opc.o" ;;
	bfd_tic6x_arch)		ta="$ta tic6x-dis.o" ;;
	bfd_tic80_arch)		ta="$ta tic80-dis.o tic80-opc.o" ;;
	bfd_tilegx_arch)	ta="$ta tilegx-dis.o tilegx-opc.o" ;;
	bfd_tilepro_arch)	ta="$ta tilepro-dis.o tilepro-opc.o" ;;
	bfd_v850_arch)		ta="$ta v850-opc.o v850-dis.o" ;;
	bfd_v850e_arch)		ta="$ta v850-opc.o v850-dis.o" ;;
	bfd_v850ea_arch)	ta="$ta v850-opc.o v850-dis.o" ;;
	bfd_v850_rh850_arch)	ta="$ta v850-opc.o v850-dis.o" ;;
	bfd_vax_arch)		ta="$ta vax-dis.o" ;;
	bfd_visium_arch)	ta="$ta visium-dis.o visium-opc.o" ;;
        bfd_wasm32_arch)        ta="$ta wasm32-dis.o" ;;
	bfd_xc16x_arch)		ta="$ta xc16x-asm.o xc16x-desc.o xc16x-dis.o xc16x-ibld.o xc16x-opc.o" using_cgen=yes ;;
	bfd_xgate_arch)		ta="$ta xgate-dis.o xgate-opc.o" ;;
	bfd_xstormy16_arch)	ta="$ta xstormy16-asm.o xstormy16-desc.o xstormy16-dis.o xstormy16-ibld.o xstormy16-opc.o" using_cgen=yes ;;
	bfd_xtensa_arch)	ta="$ta xtensa-dis.o" ;;
	bfd_z80_arch)		ta="$ta z80-dis.o" ;;
	bfd_z8k_arch)		ta="$ta z8k-dis.o" ;;

	"")			;;
	*)		AC_MSG_ERROR(*** unknown target architecture $arch) ;;
	esac
    done

    if test $using_cgen = yes ; then
	ta="$ta $cgen_files"
    fi

    # Weed out duplicate .o files.
    f=""
    for i in $ta ; do
	case " $f " in
	*" $i "*) ;;
	*) f="$f $i" ;;
	esac
    done
    ta="$f"

    # And duplicate -D flags.
    f=""
    for i in $archdefs ; do
	case " $f " in
	*" $i "*) ;;
	*) f="$f $i" ;;
	esac
    done
    archdefs="$f"

    BFD_MACHINES="$ta"

else	# all_targets is true
    archdefs=-DARCH_all
    BFD_MACHINES='$(ALL_MACHINES)'
fi

AC_SUBST(archdefs)
AC_SUBST(BFD_MACHINES)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
