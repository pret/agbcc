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

AUTOMAKE_OPTIONS = no-dist foreign
ACLOCAL_AMFLAGS = -I . -I .. -I ../config

INCDIR = $(srcdir)/../include
CSEARCH = -I. -I$(srcdir) -I$(INCDIR)

noinst_LIBRARIES = libbfd.a

bfddocdir =

libbfd_a_LDFLAGS =
if INSTALL_LIBBFD
bfdlibdir = @bfdlibdir@
bfdincludedir = @bfdincludedir@
bfdinclude_HEADERS = $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h \
		     $(INCDIR)/bfdlink.h $(INCDIR)/diagnostics.h
else !INSTALL_LIBBFD
# Empty these so that the respective installation directories will not be created.
bfdlibdir =
bfdincludedir =
bfdinclude_HEADERS =
rpath_bfdlibdir = @bfdlibdir@
libbfd_a_LDFLAGS += -rpath $(rpath_bfdlibdir)
endif

# This is where we get zlib from.  zlibdir is -L../zlib and zlibinc is
# -I../zlib, unless we were configured with --with-system-zlib, in which
# case both are empty.
ZLIB = @zlibdir@
ZLIBINC = @zlibinc@

WARN_CFLAGS = @WARN_CFLAGS@
NO_WERROR = @NO_WERROR@
AM_CFLAGS = $(WARN_CFLAGS) $(ZLIBINC)
AM_CPPFLAGS = -DBINDIR='"$(bindir)"'
LTCOMPILE = $(CC) $(CFLAGS) $(AM_CPPFLAGS) $(AM_CFLAGS)

# bfd.h goes here, for now
BFD_H = bfd.h

# Jim Kingdon notes:
# Writing S-records should be included in all (or at least most)
# *-*-coff, *-*-aout, etc., configurations, because people will want to
# be able to use objcopy to create S-records.  (S-records are not useful
# for the debugger, so if you are downloading things as S-records you
# need two copies of the executable, one to download and one for the
# debugger).
BFD32_LIBS = \
	archive.o archures.o bfd.o bfdio.o bfdwin.o \
	cache.o compress.o corefile.o format.o hash.o \
	init.o libbfd.o linker.o merge.o opncls.o reloc.o \
	section.o simple.o stab-syms.o stabs.o syms.o targets.o \
	binary.o ihex.o srec.o tekhex.o verilog.o

BFD64_LIBS = archive64.o

BFD32_LIBS_CFILES = \
	archive.c archures.c bfd.c bfdio.c bfdwin.c \
	cache.c  compress.c corefile.c format.c hash.c \
	init.c libbfd.c linker.c merge.c opncls.c reloc.c \
	section.c simple.c stab-syms.c stabs.c syms.c targets.c \
	binary.c ihex.c srec.c tekhex.c verilog.c

BFD64_LIBS_CFILES = archive64.c

# This list is alphabetized to make it easier to keep in sync
# with the decls and initializer in archures.c.
ALL_MACHINES = \
	cpu-aarch64.o \
	cpu-alpha.o \
	cpu-arc.o \
	cpu-arm.o \
	cpu-avr.o \
	cpu-bfin.o \
	cpu-cr16.o \
	cpu-cr16c.o \
	cpu-cris.o \
	cpu-crx.o \
	cpu-d10v.o \
	cpu-d30v.o \
	cpu-dlx.o \
	cpu-epiphany.o \
	cpu-fr30.o \
	cpu-frv.o \
	cpu-ft32.o \
	cpu-h8300.o \
	cpu-hppa.o \
	cpu-i386.o \
	cpu-iamcu.o \
	cpu-l1om.o \
	cpu-k1om.o \
	cpu-ia64.o \
	cpu-ip2k.o \
	cpu-iq2000.o \
	cpu-lm32.o \
	cpu-m10200.o \
	cpu-m10300.o \
	cpu-m32c.o \
	cpu-m32r.o \
	cpu-m68hc11.o \
	cpu-m68hc12.o \
	cpu-m9s12x.o \
	cpu-s12z.o \
	cpu-m9s12xg.o \
	cpu-m68k.o \
	cpu-mcore.o \
	cpu-mep.o \
	cpu-metag.o \
	cpu-microblaze.o \
	cpu-mips.o \
	cpu-mmix.o \
	cpu-moxie.o \
	cpu-msp430.o \
	cpu-mt.o \
	cpu-nds32.o \
	cpu-nfp.o \
	cpu-nios2.o \
	cpu-ns32k.o \
	cpu-or1k.o \
	cpu-pdp11.o \
	cpu-pj.o \
	cpu-plugin.o \
	cpu-powerpc.o \
	cpu-pru.o \
	cpu-rs6000.o \
	cpu-riscv.o \
	cpu-rl78.o \
	cpu-rx.o \
	cpu-s390.o \
	cpu-score.o \
	cpu-sh.o \
	cpu-sparc.o \
	cpu-spu.o \
	cpu-tic30.o \
	cpu-tic4x.o \
	cpu-tic54x.o \
	cpu-tic6x.o \
	cpu-tic80.o \
	cpu-tilegx.o \
	cpu-tilepro.o \
	cpu-v850.o \
	cpu-v850_rh850.o \
	cpu-vax.o \
	cpu-visium.o \
	cpu-wasm32.o \
	cpu-xc16x.o \
	cpu-xgate.o \
	cpu-xstormy16.o \
	cpu-xtensa.o \
	cpu-z80.o \
	cpu-z8k.o

ALL_MACHINES_CFILES = \
	cpu-aarch64.c \
	cpu-alpha.c \
	cpu-arc.c \
	cpu-arm.c \
	cpu-avr.c \
	cpu-bfin.c \
	cpu-cr16.c \
	cpu-cr16c.c \
	cpu-cris.c \
	cpu-crx.c \
	cpu-d10v.c \
	cpu-d30v.c \
	cpu-dlx.c \
	cpu-epiphany.c \
	cpu-fr30.c \
	cpu-frv.c \
	cpu-ft32.c \
	cpu-h8300.c \
	cpu-hppa.c \
	cpu-i386.c \
	cpu-iamcu.c \
	cpu-l1om.c \
	cpu-k1om.c \
	cpu-ia64.c \
	cpu-ip2k.c \
	cpu-iq2000.c \
	cpu-lm32.c \
	cpu-m10200.c \
	cpu-m10300.c \
	cpu-m32c.c \
	cpu-m32r.c \
	cpu-m68hc11.c \
	cpu-m68hc12.c \
	cpu-m9s12x.c \
	cpu-s12z.c \
	cpu-m9s12xg.c \
	cpu-m68k.c \
	cpu-mcore.c \
	cpu-mep.c \
	cpu-metag.c \
	cpu-microblaze.c \
	cpu-mips.c \
	cpu-mmix.c \
	cpu-moxie.c \
	cpu-msp430.c \
	cpu-mt.c \
	cpu-nds32.c \
	cpu-nfp.c \
	cpu-ns32k.c \
	cpu-nios2.c \
	cpu-or1k.c \
	cpu-pdp11.c \
	cpu-pj.c \
	cpu-plugin.c \
	cpu-powerpc.c \
	cpu-pru.c \
	cpu-rs6000.c \
	cpu-riscv.c \
	cpu-rl78.c \
	cpu-rx.c \
	cpu-s390.c \
	cpu-score.c \
	cpu-sh.c \
	cpu-sparc.c \
	cpu-spu.c \
	cpu-tic30.c \
	cpu-tic4x.c \
	cpu-tic54x.c \
	cpu-tic6x.c \
	cpu-tic80.c \
	cpu-tilegx.c \
	cpu-tilepro.c \
	cpu-v850.c \
	cpu-v850_rh850.c \
	cpu-vax.c \
	cpu-visium.c \
	cpu-wasm32.c \
	cpu-xc16x.c \
	cpu-xgate.c \
	cpu-xstormy16.c \
	cpu-xtensa.c \
	cpu-z80.c \
	cpu-z8k.c

# The .o files needed by all of the 32 bit vectors that are configured into
# target_vector in targets.c if configured with --enable-targets=all.
BFD32_BACKENDS = \
	cf-i386lynx.o \
	dwarf1.o \
	dwarf2.o \
	elf-attrs.o \
	elf-eh-frame.o \
	elf-nacl.o \
	elf-properties.o \
	elf-strtab.o \
	elf-vxworks.o \
	elf.o \
	elf32-am33lin.o \
	elf32-arc.o \
	elf32-arm.o \
	elf32.o \
	elflink.o \
	elfxx-sparc.o \
	elfxx-tilegx.o \
	reloc16.o \
	som.o \
	vax1knetbsd.o \
	vaxnetbsd.o \
	vms-lib.o \
	vms-misc.o \
	wasm-module.o

BFD32_BACKENDS_CFILES = \
	dwarf1.c \
	dwarf2.c \
	elf-attrs.c \
	elf-eh-frame.c \
	elf-nacl.c \
	elf-properties.c \
	elf-strtab.c \
	elf-vxworks.c \
	elf.c \
	elf32-arm.c \
	elf32.c \
	elflink.c \
	elfxx-sparc.c \
	elfxx-tilegx.c \
	reloc16.c \
	vms-lib.c \
	vms-misc.c \
	wasm-module.c

# The .o files needed by all of the 64 bit vectors that are configured into
# target_vector in targets.c if configured with --enable-targets=all
# and --enable-64-bit-bfd.
# elf32-ia64.c requires a 64-bit bfd_vma, and hence can not be put in
# BFD32_BACKENDS.
BFD64_BACKENDS = \
	elf32-aarch64.o \
	elf64-aarch64.o \
	elfxx-aarch64.o \
	aix5ppc-core.o \
	aout64.o \
	coff-alpha.o \
	coff-x86_64.o \
	coff64-rs6000.o \
	elf32-ia64.o \
	elf32-mips.o \
	elf32-score.o \
	elf32-score7.o \
	elf64-alpha.o \
	elf64-gen.o \
	elf64-hppa.o \
	elf64-ia64.o \
	elf64-ia64-vms.o \
	elfxx-ia64.o \
	elfn32-mips.o \
	elf64-mips.o \
	elfxx-mips.o \
	elf64-mmix.o \
	elf64-nfp.o \
	elf64-ppc.o \
	elf32-riscv.o \
	elf64-riscv.o \
	elfxx-riscv.o \
	elf64-s390.o \
	elf64-sparc.o \
	elf64-tilegx.o \
	elf64-x86-64.o \
	elfxx-x86.o \
	elf64.o \
	mach-o-aarch64.o \
	mach-o-x86-64.o \
	mmo.o \
	pe-x86_64.o \
	pei-ia64.o \
	pei-x86_64.o \
	pepigen.o \
	pex64igen.o \
	vms-alpha.o

BFD64_BACKENDS_CFILES = \
	aix5ppc-core.c \
	aout64.c \
	coff-alpha.c \
	coff-x86_64.c \
	coff64-rs6000.c \
	elf32-mips.c \
	elf32-score.c \
	elf32-score7.c \
	elf64-alpha.c \
	elf64-gen.c \
	elf64-hppa.c \
	elf64-ia64-vms.c \
	elf64-mips.c \
	elf64-mmix.c \
	elf64-nfp.c \
	elf64-ppc.c \
	elf64-s390.c \
	elf64-sparc.c \
	elf64-tilegx.c \
	elf64-x86-64.c \
	elfxx-x86.c \
	elf64.c \
	elfn32-mips.c \
	elfxx-aarch64.c \
	elfxx-ia64.c \
	elfxx-mips.c \
	elfxx-riscv.c \
	mach-o-aarch64.c \
	mach-o-x86-64.c \
	mmo.c \
	pe-x86_64.c \
	pei-ia64.c \
	pei-x86_64.c \
	vms-alpha.c

OPTIONAL_BACKENDS = \
	aix386-core.o \
	cisco-core.o \
	hpux-core.o \
	irix-core.o \
	lynx-core.o \
	netbsd-core.o \
	osf-core.o \
	rs6000-core.o \
	sco5-core.o \
	trad-core.o

OPTIONAL_BACKENDS_CFILES = \
	aix386-core.c \
	cisco-core.c \
	hpux-core.c \
	irix-core.c \
	lynx-core.c \
	netbsd-core.c \
	osf-core.c \
	rs6000-core.c \
	sco5-core.c \
	trad-core.c

# Reconfigure if config.bfd or configure.host changes.
# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = \
	$(srcdir)/config.bfd \
	$(srcdir)/configure.host \
	$(srcdir)/development.sh

# These are defined by configure:
WORDSIZE = @wordsize@
ALL_BACKENDS = @all_backends@
BFD_BACKENDS = @bfd_backends@
BFD_MACHINES = @bfd_machines@
TDEFAULTS = @tdefaults@
HAVEVECS = @havevecs@

AM_CPPFLAGS += @HDEFINES@ @COREFLAG@ @TDEFINES@ $(CSEARCH) $(CSWITCHES) \
	$(HAVEVECS)

# C source files that correspond to .o's.
SOURCE_CFILES = \
	$(BFD32_LIBS_CFILES) \
	$(BFD64_LIBS_CFILES) \
	$(ALL_MACHINES_CFILES) \
	$(BFD32_BACKENDS_CFILES) \
	$(BFD64_BACKENDS_CFILES) \
	$(OPTIONAL_BACKENDS_CFILES)

BUILD_CFILES = \
	elf32-aarch64.c elf64-aarch64.c \
	elf32-ia64.c elf64-ia64.c \
	elf32-riscv.c elf64-riscv.c \
	peigen.c pepigen.c pex64igen.c

CFILES = $(SOURCE_CFILES) $(BUILD_CFILES)

## This is a list of all .h files which are in the source tree.
SOURCE_HFILES = \
	elf-bfd.h  \
	elfcode.h elfcore.h \
	genlink.h \
	libaout.h libbfd.h \
	som.h version.h

## ... and all .h files which are in the build tree.
BUILD_HFILES = \
	bfdver.h elf32-target.h elf64-target.h targmatch.h bfd_stdint.h

# Ensure they are built early:
BUILT_SOURCES = $(BUILD_HFILES)

HFILES = $(SOURCE_HFILES) $(BUILD_HFILES)

SRC_POTFILES = $(SOURCE_CFILES) $(SOURCE_HFILES)
BLD_POTFILES = $(BUILD_CFILES) $(BUILD_HFILES)

po/SRC-POTFILES.in: @MAINT@ Makefile $(SRC_POTFILES)
	for file in $(SRC_POTFILES); do echo $$file; done \
	  | LC_ALL=C sort > tmp.src \
	  && mv tmp.src $(srcdir)/po/SRC-POTFILES.in

po/BLD-POTFILES.in: @MAINT@ Makefile $(BLD_POTFILES)
	for file in $(BLD_POTFILES); do echo $$file; done \
	  | LC_ALL=C sort > tmp.bld \
	  && mv tmp.bld $(srcdir)/po/BLD-POTFILES.in

all diststuff: info

# Various kinds of .o files to put in libbfd.a:
# BFD_BACKENDS	Routines the configured targets need.
# BFD_MACHINES	Architecture-specific routines the configured targets need.
# COREFILE	Core file routines for a native configuration
# bfd64_libs	Routines for 64bit support
OFILES = $(BFD_BACKENDS) $(BFD_MACHINES) @COREFILE@ @bfd64_libs@

stamp-ofiles: Makefile
	rm -f tofiles
	f=""; \
	for i in $(OFILES) ; do \
	  case " $$f " in \
	    *" $$i "*) ;; \
	    *) f="$$f $$i" ;; \
	  esac ; \
	done ; \
	echo $$f > tofiles
	$(SHELL) $(srcdir)/../move-if-change tofiles ofiles
	touch stamp-ofiles

ofiles: stamp-ofiles ; @true

# Since wBFD64_LIBS is optional and we can't have substitution in
# libbfd_la_SOURCES, we put BFD64_LIBS in OFILES instead.
# However, list all sources in EXTRA_libbfd_la_SOURCES so the
# dependency tracking fragments are picked up in the Makefile.
libbfd_a_SOURCES = $(BFD32_LIBS_CFILES)
EXTRA_libbfd_a_SOURCES = $(CFILES)
libbfd_a_DEPENDENCIES = $(OFILES) ofiles
libbfd_a_LIBADD = `cat ofiles` @SHARED_LIBADD@ $(LIBDL) $(ZLIB)
libbfd_a_LDFLAGS += -release `cat libtool-soversion` @SHARED_LDFLAGS@

# libtool will build .libs/libbfd.a.  We create libbfd.a in the build
# directory so that we don't have to convert all the programs that use
# libbfd.a simultaneously.  This is a hack which should be removed if
# everything else starts using libtool.  FIXME.

#libbfd_a_SOURCES =s



#libbfd.a: $(ofiles)
#	$(AR) $(ARFLAGS) $@ $(ofiles)

# This file holds an array associating configuration triplets and
# vector names.  It is built from config.bfd.  It is not compiled by
# itself, but is included by targets.c.
targmatch.h: config.bfd targmatch.sed
	rm -f targmatch.h
	$(SED) -f $(srcdir)/targmatch.sed < $(srcdir)/config.bfd > targmatch.new
	mv -f targmatch.new targmatch.h

# When compiling archures.c and targets.c, supply the default target
# info from configure.

targets.o: targets.c Makefile
if am__fastdepCC
	$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(TDEFAULTS) $(srcdir)/targets.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='targets.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(LTCOMPILE) -c -o $@ $(TDEFAULTS) $(srcdir)/targets.c
endif

archures.o: archures.c Makefile
if am__fastdepCC
	$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(TDEFAULTS) $(srcdir)/archures.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='archures.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(LTCOMPILE) -c -o $@ $(TDEFAULTS) $(srcdir)/archures.c
endif

$(ofiles): %.o: %.c Makefile
dwarf2.o: dwarf2.c Makefile
if am__fastdepCC
	$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ -DDEBUGDIR=\"$(DEBUGDIR)\" $(srcdir)/dwarf2.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='dwarf2.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(LTCOMPILE) -c -o $@ -DDEBUGDIR=\"$(DEBUGDIR)\" $(srcdir)/dwarf2.c
endif

elf32-target.h : elfxx-target.h
	rm -f elf32-target.h
	$(SED) -e s/NN/32/g < $(srcdir)/elfxx-target.h > elf32-target.new
	mv -f elf32-target.new elf32-target.h

elf64-target.h : elfxx-target.h
	rm -f elf64-target.h
	$(SED) -e s/NN/64/g < $(srcdir)/elfxx-target.h > elf64-target.new
	mv -f elf64-target.new elf64-target.h

elf32-aarch64.c : elfnn-aarch64.c
	rm -f elf32-aarch64.c
	echo "#line 1 \"$(srcdir)/elfnn-aarch64.c\"" > elf32-aarch64.new
	$(SED) -e s/NN/32/g < $(srcdir)/elfnn-aarch64.c >> elf32-aarch64.new
	mv -f elf32-aarch64.new elf32-aarch64.c

elf64-aarch64.c : elfnn-aarch64.c
	rm -f elf64-aarch64.c
	echo "#line 1 \"$(srcdir)/elfnn-aarch64.c\"" > elf64-aarch64.new
	$(SED) -e s/NN/64/g < $(srcdir)/elfnn-aarch64.c >> elf64-aarch64.new
	mv -f elf64-aarch64.new elf64-aarch64.c

elf32-ia64.c : elfnn-ia64.c
	rm -f elf32-ia64.c
	$(SED) -e s/NN/32/g < $(srcdir)/elfnn-ia64.c > elf32-ia64.new
	mv -f elf32-ia64.new elf32-ia64.c

elf64-ia64.c : elfnn-ia64.c
	rm -f elf64-ia64.c
	$(SED) -e s/NN/64/g < $(srcdir)/elfnn-ia64.c > elf64-ia64.new
	mv -f elf64-ia64.new elf64-ia64.c

elf32-riscv.c : elfnn-riscv.c
	rm -f elf32-riscv.c
	echo "#line 1 \"$(srcdir)/elfnn-riscv.c\"" > elf32-riscv.new
	sed -e s/NN/32/g < $(srcdir)/elfnn-riscv.c >> elf32-riscv.new
	mv -f elf32-riscv.new elf32-riscv.c

elf64-riscv.c : elfnn-riscv.c
	rm -f elf64-riscv.c
	echo "#line 1 \"$(srcdir)/elfnn-riscv.c\"" > elf64-riscv.new
	sed -e s/NN/64/g < $(srcdir)/elfnn-riscv.c >> elf64-riscv.new
	mv -f elf64-riscv.new elf64-riscv.c

peigen.c : peXXigen.c
	rm -f peigen.c
	$(SED) -e s/XX/pe/g < $(srcdir)/peXXigen.c > peigen.new
	mv -f peigen.new peigen.c

pepigen.c : peXXigen.c
	rm -f pepigen.c
	$(SED) -e s/XX/pep/g < $(srcdir)/peXXigen.c > pepigen.new
	mv -f pepigen.new pepigen.c

pex64igen.c: peXXigen.c
	rm -f pex64igen.c
	$(SED) -e s/XX/pex64/g < $(srcdir)/peXXigen.c > pex64igen.new
	mv -f pex64igen.new pex64igen.c

BFD_H_DEPS= $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
LOCAL_H_DEPS= libbfd.h sysdep.h config.h
$(BFD32_LIBS) \
 $(BFD64_LIBS) \
 $(ALL_MACHINES) \
 $(BFD32_BACKENDS) \
 $(BFD64_BACKENDS) \
 $(OPTIONAL_BACKENDS): $(BFD_H) $(BFD_H_DEPS) $(LOCAL_H_DEPS)

host-aout.o: Makefile

# The following program can be used to generate a simple config file
# which can be folded into an h-XXX file for a new host, with some editing.
aout-params.h: gen-aout
	./gen-aout host > aout-params.h
gen-aout: $(srcdir)/gen-aout.c Makefile
	$(CC) -o gen-aout $(CFLAGS) $(LFLAGS) $(srcdir)/gen-aout.c

$(BFD_H): stmp-bfd-h ; @true

stmp-bfd-h: bfd-in3.h
	rm -f bfd-tmp.h
	cp bfd-in3.h bfd-tmp.h
	$(SHELL) $(srcdir)/../move-if-change bfd-tmp.h $(BFD_H)
	rm -f bfd-tmp.h
	touch stmp-bfd-h

BFD_H_FILES = bfd-in.h init.c opncls.c libbfd.c \
	bfdio.c bfdwin.c section.c archures.c reloc.c \
	syms.c bfd.c archive.c corefile.c targets.c format.c \
	linker.c simple.c compress.c
BFD64_H_FILES = archive64.c
LIBBFD_H_FILES = libbfd-in.h libbfd.c bfdio.c bfdwin.c \
	cache.c reloc.c archures.c linker.c

# Could really use a "copy-if-change"...
headers:
	(cd $(bfddocdir); $(MAKE) protos $(FLAGS_TO_PASS))
	cp $(bfddocdir)/bfd.h bfd-in2.h-new
	$(SHELL) $(srcdir)/../move-if-change bfd-in2.h-new $(srcdir)/bfd-in2.h
	cp $(bfddocdir)/libbfd.h libbfd.h-new
	$(SHELL) $(srcdir)/../move-if-change libbfd.h-new $(srcdir)/libbfd.h

# We only rebuild the header files automatically if we have been
# configured with --enable-maintainer-mode.

$(srcdir)/bfd-in2.h: @MAINT@ stmp-bin2-h ; @true
stmp-bin2-h: $(BFD_H_FILES) $(BFD64_H_FILES)
	(cd $(bfddocdir); $(MAKE) $(FLAGS_TO_PASS) bfd.h)
	cp $(bfddocdir)/bfd.h bfd-in2.h-new
	$(SHELL) $(srcdir)/../move-if-change bfd-in2.h-new $(srcdir)/bfd-in2.h
	touch stmp-bin2-h

$(srcdir)/libbfd.h: @MAINT@ stmp-lbfd-h ; @true
stmp-lbfd-h: $(LIBBFD_H_FILES)
	(cd $(bfddocdir); $(MAKE) $(FLAGS_TO_PASS) libbfd.h)
	cp $(bfddocdir)/libbfd.h libbfd.h-new
	$(SHELL) $(srcdir)/../move-if-change libbfd.h-new $(srcdir)/libbfd.h
	touch stmp-lbfd-h

MOSTLYCLEANFILES = ofiles stamp-ofiles

CLEANFILES = bfd.h dep.sed stmp-bfd-h DEP DEPA DEP1 DEP2 libbfd.a stamp-lib \
	stmp-bin2-h stmp-lbfd-h

DISTCLEANFILES = $(BUILD_CFILES) $(BUILD_HFILES) libtool-soversion

bfdver.h: $(srcdir)/version.h $(srcdir)/development.sh $(srcdir)/Makefile.in
	@echo "creating $@"
	@bfd_version=`echo "$(VERSION)" | $(SED) -e 's/\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\).*/\1.00\2.00\3.00\4.00\5/' -e 's/\([^\.]*\)\..*\(..\)\..*\(..\)\..*\(..\)\..*\(..\)$$/\1\2\3\4\5/'` ;\
	bfd_version_string="\"$(VERSION)\"" ;\
	bfd_soversion="$(VERSION)" ;\
	bfd_version_package="\"$(PKGVERSION)\"" ;\
	report_bugs_to="\"$(REPORT_BUGS_TO)\"" ;\
	. $(srcdir)/development.sh ;\
	if test "$$development" = true ; then \
	  bfd_version_date=`$(SED) -n -e 's/.*DATE //p' < $(srcdir)/version.h` ;\
	  bfd_version_string="\"$(VERSION).$${bfd_version_date}\"" ;\
	  bfd_soversion="$(VERSION).$${bfd_version_date}" ;\
	fi ;\
	$(SED) -e "s,@bfd_version@,$$bfd_version," \
	    -e "s,@bfd_version_string@,$$bfd_version_string," \
	    -e "s,@bfd_version_package@,$$bfd_version_package," \
	    -e "s,@report_bugs_to@,$$report_bugs_to," \
	    < $(srcdir)/version.h > $@; \
	echo "$${bfd_soversion}" > libtool-soversion
