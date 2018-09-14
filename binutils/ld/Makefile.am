## Process this file with automake to generate Makefile.in
#
#   Copyright (C) 2012-2018 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.
#

AUTOMAKE_OPTIONS = no-texinfo.tex no-dist foreign info-in-builddir
ACLOCAL_AMFLAGS = -I .. -I ../config -I ../bfd

tooldir = $(exec_prefix)/$(target_alias)

ELF_CLFAGS=-DELF_LIST_OPTIONS=@elf_list_options@ \
	   -DELF_SHLIB_LIST_OPTIONS=@elf_shlib_list_options@ \
	   -DELF_PLT_UNWIND_LIST_OPTIONS=@elf_plt_unwind_list_options@
WARN_CFLAGS = @WARN_CFLAGS@
NO_WERROR = @NO_WERROR@
AM_CFLAGS = $(WARN_CFLAGS) $(ELF_CLFAGS)

PLUGIN_C =
PLUGIN_H =
PLUGIN_OBJECT =
PLUGIN_CFLAGS =

# We put the scripts in the directory $(scriptdir)/ldscripts.
# We can't put the scripts in $(datadir) because the SEARCH_DIR
# directives need to be different for native and cross linkers.
scriptdir = $(tooldir)/lib

EMUL = @EMUL@
EMULATION_OFILES = @EMULATION_OFILES@
EMUL_EXTRA_OFILES = @EMUL_EXTRA_OFILES@


# Search path to override the default search path for -lfoo libraries.
# If LIB_PATH is empty, the ones in the script (if any) are left alone.
# (The default is usually /lib:/usr/lib:/usr/local/lib, unless building
# a cross-linker, in which case the default is empty.  See genscripts.sh.)
# Otherwise, they are replaced with the ones given in LIB_PATH,
# which may have the form: LIB_PATH=/lib:/usr/local/lib.  This can be set
# when the linker is configured via the --with-lib-path configure switch.
LIB_PATH = @LIB_PATH@

BASEDIR = $(srcdir)/..
BFDDIR = $(BASEDIR)/bfd
INCDIR	= $(BASEDIR)/include

# What version of the manual to build
DOCVER = gen

CC_FOR_TARGET = ` \
  if [ -f $$r/../gcc/xgcc ] ; then \
    if [ -f $$r/../newlib/Makefile ] ; then \
      echo $$r/../gcc/xgcc -B$$r/../gcc/ -idirafter $$r/../newlib/targ-include -idirafter $${srcroot}/../newlib/libc/include -nostdinc; \
    else \
      echo $$r/../gcc/xgcc -B$$r/../gcc/; \
    fi; \
  else \
    if [ "@host@" = "@target@" ] ; then \
      echo $(CC); \
    else \
      echo gcc | sed '$(transform)'; \
    fi; \
  fi`

# Strip out sanitization options as we want to test building binaries without any extra paraphernalia
CFLAGS_FOR_TARGET = `echo $(CFLAGS) | sed -e 's/-fsanitize=address//g' -e 's/-fsanitize=undefined//g'`

transform = s/^ld-new$$/$(installed_linker)/;@program_transform_name@
bin_PROGRAMS = ld-new
info_TEXINFOS =
man_MANS =

AM_MAKEINFOFLAGS = -I $(srcdir) -I $(BFDDIR)/doc -I ../bfd/doc \
		   -I $(top_srcdir)/../libiberty

AM_CPPFLAGS = -I. -I$(srcdir) -I../bfd -I$(BFDDIR) -I$(INCDIR) \
	$(HDEFINES) $(CFLAGS) $(PLUGIN_CFLAGS) \
	-DLOCALEDIR="\"$(datadir)/locale\""

BFDLIB = ../bfd/libbfd.a
LIBIBERTY = ../libiberty/libiberty.a

ALL_EMULATION_SOURCES = \
	earmelf.c \
	evanilla.c

ALL_EMULATIONS = $(ALL_EMULATION_SOURCES:.c=.@OBJEXT@)

ALL_64_EMULATION_SOURCES = \
	emmo.c

ALL_64_EMULATIONS = $(ALL_64_EMULATION_SOURCES:.c=.@OBJEXT@)

ALL_EMUL_EXTRA_OFILES = \
	deffilep.@OBJEXT@ \
	pe-dll.@OBJEXT@

ALL_64_EMUL_EXTRA_OFILES = \
	pep-dll.@OBJEXT@

CFILES = ldctor.c ldemul.c ldexp.c ldfile.c ldlang.c \
	ldmain.c ldmisc.c ldver.c ldwrite.c lexsup.c \
	mri.c ldcref.c pe-dll.c pep-dll.c ldlex-wrapper.c \
	$(PLUGIN_C)

HFILES = ld.h ldctor.h ldemul.h ldexp.h ldfile.h \
	ldlang.h ldlex.h ldmain.h ldmisc.h ldver.h \
	ldwrite.h mri.h deffile.h pe-dll.h pep-dll.h \
	elf-hints-local.h $(PLUGIN_H)

GENERATED_CFILES = ldgram.c ldlex.c deffilep.c
GENERATED_HFILES = ldgram.h ldemul-list.h deffilep.h

# Require an early dependency on the generated headers, as the dependency
# tracking will not cause them to be built beforehand.
BUILT_SOURCES = $(GENERATED_HFILES)

OFILES = ldgram.@OBJEXT@ ldlex-wrapper.@OBJEXT@ lexsup.@OBJEXT@ ldlang.@OBJEXT@ \
	mri.@OBJEXT@ ldctor.@OBJEXT@ ldmain.@OBJEXT@ $(PLUGIN_OBJECT) \
	ldwrite.@OBJEXT@ ldexp.@OBJEXT@  ldemul.@OBJEXT@ ldver.@OBJEXT@ ldmisc.@OBJEXT@ \
	ldfile.@OBJEXT@ ldcref.@OBJEXT@ ${EMULATION_OFILES} ${EMUL_EXTRA_OFILES}

STAGESTUFF = *.@OBJEXT@ ldscripts/* e*.c

# Disable -Werror, if it has been enabled, since old versions of bison/
# yacc will produce working code which contain compile time warnings.
ldgram.@OBJEXT@: ldgram.c
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f ldgram.c || echo $(srcdir)/`ldgram.c $(NO_WERROR)
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='ldgram.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c `test -f ldgram.c || echo $(srcdir)/`ldgram.c $(NO_WERROR)
endif

ldlex-wrapper.@OBJEXT@: ldlex-wrapper.c ldlex.c
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(srcdir)/ldlex-wrapper.c $(NO_WERROR)
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='ldlex-wrapper.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c $(srcdir)/ldlex-wrapper.c $(NO_WERROR)
endif

deffilep.@OBJEXT@: deffilep.c
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f deffilep.c || echo $(srcdir)/`deffilep.c $(NO_WERROR)
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='deffilep.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c `test -f deffilep.c || echo $(srcdir)/`deffilep.c $(NO_WERROR)
endif

SRC_POTFILES = $(CFILES) $(HFILES)
BLD_POTFILES = $(ALL_EMULATION_SOURCES) $(ALL_64_EMULATION_SOURCES)

ldmain.@OBJEXT@: ldmain.c config.status
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ \
	  -DDEFAULT_EMULATION='"$(EMUL)"' \
	  -DBINDIR='"$(bindir)"' -DTOOLBINDIR='"$(tooldir)/bin"' \
	  -DTARGET='"@target@"' @TARGET_SYSTEM_ROOT_DEFINE@ \
	  $(srcdir)/ldmain.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='ldmain.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c -DDEFAULT_EMULATION='"$(EMUL)"' \
	  -DBINDIR='"$(bindir)"' -DTOOLBINDIR='"$(tooldir)/bin"' \
	  -DTARGET='"@target@"' @TARGET_SYSTEM_ROOT_DEFINE@ \
	  $(srcdir)/ldmain.c
endif

ldfile.@OBJEXT@: ldfile.c config.status
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ \
	-DSCRIPTDIR='"$(scriptdir)"' -DBINDIR='"$(bindir)"' -DTOOLBINDIR='"$(tooldir)/bin"' \
	 $(srcdir)/ldfile.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='ldfile.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c -DSCRIPTDIR='"$(scriptdir)"' -DBINDIR='"$(bindir)"' \
	  -DTOOLBINDIR='"$(tooldir)/bin"' \
	  $(srcdir)/ldfile.c
endif

eelf32_spu.@OBJEXT@: eelf32_spu.c
if am__fastdepCC
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ \
	  -DEMBEDSPU="\"`echo embedspu | sed '$(transform)'`\"" eelf32_spu.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	source='eelf32_spu.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE) -c -DEMBEDSPU="\"`echo embedspu | sed '$(transform)'`\"" \
	  eelf32_spu.c
endif

ldemul-list.h: Makefile
	(echo "/* This file is automatically generated.  DO NOT EDIT! */";\
	for f in `echo " " ${EMULATION_OFILES} "" \
	 | sed -e 's/ e/ ld/g' -e 's/ ld/ /g' -e 's/[.]o//g'`; do \
	    echo "extern ld_emulation_xfer_type ld_$${f}_emulation;"; \
	done;\
	echo "";\
	echo "#define EMULATION_LIST \\";\
	for f in `echo " " ${EMULATION_OFILES} "" \
	 | sed -e 's/ e/ ld/g' -e 's/ ld/ /g' -e 's/[.]o//g'`; do \
	    echo "  &ld_$${f}_emulation, \\"; \
	done;\
	echo "  0") >ldemul-tmp.h
	mv ldemul-tmp.h ldemul-list.h

stringify.sed: ${srcdir}/emultempl/$(STRINGIFY)
	  cp ${srcdir}/emultempl/$(STRINGIFY) stringify.sed

# These all start with e so 'make clean' can find them.

GENSCRIPTS = LIB_PATH='${LIB_PATH}' $(SHELL) $(srcdir)/genscripts.sh "${srcdir}" "${libdir}" "${prefix}" "${exec_prefix}" @host@ @target@ @target_alias@ "@EMULATION_LIBPATH@" "@NATIVE_LIB_DIRS@" @use_sysroot@ @enable_initfini_array@
GEN_DEPENDS = $(srcdir)/genscripts.sh stringify.sed
ELF_DEPS = $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/elf-generic.em $(srcdir)/scripttempl/DWARF.sc
ELF_GEN_DEPS = $(srcdir)/emultempl/generic.em $(srcdir)/emultempl/elf-generic.em $(srcdir)/emultempl/genelf.em $(srcdir)/scripttempl/DWARF.sc
ELF_X86_DEPS = $(ELF_DEPS) $(srcdir)/emulparams/plt_unwind.sh \
	       $(srcdir)/emulparams/extern_protected_data.sh \
	       $(srcdir)/emulparams/dynamic_undefined_weak.sh \
	       $(srcdir)/emulparams/reloc_overflow.sh \
	       $(srcdir)/emulparams/call_nop.sh \
	       $(srcdir)/emulparams/cet.sh

@TDIRS@

# We can't use pattern rules as we don't want to depend on GNU
# make, or else these rules could have been expressed in one
# two-liner: 'e%.c:' and '	${GENSCRIPTS} $* "$(tdir_$*)"'.
# (The recursive variable expansion is portable.)

run-genscripts:
	${GENSCRIPTS} $(script_target) "$($(script_tdirname))"

.PHONY: run-genscripts

$(ALL_EMULATION_SOURCES) $(ALL_64_EMULATION_SOURCES):
	base=`echo $@ | sed -e 's,e\(.*\).c,\1,'`; \
	$(MAKE) run-genscripts "script_target=$$base" "script_tdirname=tdir_$$base"

earmelf.c: $(srcdir)/emulparams/armelf.sh \
  $(ELF_DEPS) $(srcdir)/emultempl/armelf.em \
  $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}

evanilla.c: $(srcdir)/emulparams/vanilla.sh \
  $(srcdir)/emultempl/vanilla.em $(srcdir)/scripttempl/vanilla.sc ${GEN_DEPENDS}

emmo.c: $(srcdir)/emulparams/mmo.sh $(srcdir)/emultempl/mmix-elfnmmo.em \
  $(srcdir)/emultempl/generic.em $(srcdir)/emultempl/elf-generic.em \
  $(srcdir)/emultempl/mmo.em $(srcdir)/scripttempl/DWARF.sc \
  $(srcdir)/scripttempl/mmo.sc ${GEN_DEPENDS}


# We need this for automake to use YLWRAP.
EXTRA_ld_new_SOURCES = deffilep.c ldlex.c
# Allow dependency tracking to work for these files, too.
EXTRA_ld_new_SOURCES += pep-dll.c pe-dll.c

ld_new_SOURCES = ldgram.c ldlex-wrapper.c lexsup.c ldlang.c mri.c ldctor.c ldmain.c \
	ldwrite.c ldexp.c ldemul.c ldver.c ldmisc.c ldfile.c ldcref.c $(PLUGIN_C)

ld_new_DEPENDENCIES = $(EMULATION_OFILES) $(EMUL_EXTRA_OFILES) \
		      $(BFDLIB) $(LIBIBERTY) $(LIBINTL_DEP)
ld_new_LDADD = $(EMULATION_OFILES) $(EMUL_EXTRA_OFILES) $(BFDLIB) $(LIBIBERTY) $(LIBINTL) -lz

# Dependency tracking for the generated emulation files.
EXTRA_ld_new_SOURCES += $(ALL_EMULATION_SOURCES) $(ALL_64_EMULATION_SOURCES)

# This is the real libbfd.a created by libtool.
TESTBFDLIB = @TESTBFDLIB@

# We want to reconfigure if configure.host or configure.tgt changes.
# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = $(srcdir)/configure.host $(srcdir)/configure.tgt \
	$(BFDDIR)/development.sh

MOSTLYCLEANFILES = $(STAGESTUFF) ld1$(EXEEXT) ld2$(EXEEXT) ld3$(EXEEXT) \
	ldemul-list.h crtbegin.@OBJEXT@ crtend.@OBJEXT@ ld.log ld.sum
mostlyclean-local:
	-rm -rf tmpdir
CLEANFILES = dep.sed DEP DEPA DEP1 DEP2 spu_ovl.s spu_ovl.@OBJEXT@ spu_icache.s spu_icache.@OBJEXT@

.PHONY: install-exec-local install-data-local

install-exec-local: ld-new$(EXEEXT) install-binPROGRAMS
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/bin
	n=`echo $(installed_linker) | sed '$(transform)'`; \
	if test "$(bindir)" != "$(tooldir)/bin"; then \
	  rm -f $(DESTDIR)$(tooldir)/bin/$(installed_linker)$(EXEEXT); \
	  ln -s $(DESTDIR)$(bindir)/$$n$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$(installed_linker)$(EXEEXT) >/dev/null 2>/dev/null \
	  || $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) ld-new$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$(installed_linker)$(EXEEXT); \
	fi; \
	if test "x$(install_as_default)" = "xyes"; then \
	  ld=`echo ld | sed '$(transform)'`; \
	  rm -f $(DESTDIR)$(bindir)/$$ld$(EXEEXT); \
	  ln -s $(DESTDIR)$(bindir)/$$n$(EXEEXT) $(DESTDIR)$(bindir)/$$ld$(EXEEXT) >/dev/null 2>/dev/null \
	  || $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) ld-new$(EXEEXT) $(DESTDIR)$(bindir)/$$ld$(EXEEXT); \
	  if test "$(bindir)" != "$(tooldir)/bin"; then \
	    rm -f $(DESTDIR)$(tooldir)/bin/ld$(EXEEXT); \
	    ln -s $(DESTDIR)$(bindir)/$$n$(EXEEXT) $(DESTDIR)$(tooldir)/bin/ld$(EXEEXT) >/dev/null 2>/dev/null \
	    || $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) ld-new$(EXEEXT) $(DESTDIR)$(tooldir)/bin/ld$(EXEEXT); \
	  fi; \
	fi

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(scriptdir)/ldscripts
	for f in ldscripts/* ; do \
	  $(INSTALL_DATA) $$f $(DESTDIR)$(scriptdir)/$$f ; \
	done

# Stuff that should be included in a distribution.  The diststuff
# target is run by the taz target in ../Makefile.in.
EXTRA_DIST = ldgram.c ldgram.h ldlex.c emultempl/spu_ovl.@OBJEXT@_c \
	     emultempl/spu_icache.@OBJEXT@_c deffilep.c deffilep.h $(man_MANS)
diststuff: info $(EXTRA_DIST)

# Both info (ld.info) and ld.1 depend on configdoc.texi.
# But info isn't a direct target. Make info-recursive to depend on
# ld.1 to support parallel build.
info-recursive:

DISTCLEANFILES = tdirs site.exp site.bak stringify.sed
distclean-local:
	rm -rf ldscripts
