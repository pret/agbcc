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

AUTOMAKE_OPTIONS = foreign no-dist
ACLOCAL_AMFLAGS = -I .. -I ../config -I ../bfd

# Build '.' first so all generated files exist.
SUBDIRS = .

INCDIR = $(srcdir)/../include
BFDDIR = $(srcdir)/../bfd

WARN_CFLAGS = @WARN_CFLAGS@
NO_WERROR = @NO_WERROR@
NO_WMISSING_FIELD_INITIALIZERS = @NO_WMISSING_FIELD_INITIALIZERS@
AM_CFLAGS = $(WARN_CFLAGS)

COMPILE_FOR_BUILD = $(CC_FOR_BUILD) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CFLAGS_FOR_BUILD)
LINK_FOR_BUILD = $(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) \
	$(LDFLAGS_FOR_BUILD) -o $@

libopcodes_a_LDFLAGS =
if INSTALL_LIBBFD
bfdlibdir = @bfdlibdir@
bfdincludedir = @bfdincludedir@
bfdinclude_DATA = $(INCDIR)/dis-asm.h
else
# Empty these so that the respective installation directories will not be created.
bfdlibdir =
bfdincludedir =
rpath_bfdlibdir = @bfdlibdir@
libopcodes_a_LDFLAGS += -rpath $(rpath_bfdlibdir)
endif

# This is where bfd.h lives.
BFD_H = ../bfd/bfd.h

BUILD_LIBS = @BUILD_LIBS@
BUILD_LIB_DEPS = @BUILD_LIB_DEPS@

# Header files.
HFILES = \
	sysdep.h

# C source files that correspond to .o's ending up in libopcodes
# for all machines.
TARGET_LIBOPCODES_CFILES = \
	arm-dis.c

# C source files that correspond to .o's ending up in libopcodes.
LIBOPCODES_CFILES = \
	$(TARGET_LIBOPCODES_CFILES) \
	dis-buf.c \
	dis-init.c \
	disassemble.c

# C source files that correspond to .o's.
CFILES = \
	$(LIBOPCODES_CFILES)

ALL_MACHINES = $(TARGET_LIBOPCODES_CFILES:.c=.o)

OFILES = @BFD_MACHINES@

# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = $(BFDDIR)/development.sh

AM_CPPFLAGS = -I. -I$(srcdir) -I../bfd -I$(INCDIR) -I$(BFDDIR) @HDEFINES@

disassemble.o: disassemble.c
if am__fastdepCC
	$(COMPILE_FOR_BUILD) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ @archdefs@ $(srcdir)/disassemble.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='disassemble.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(COMPILE_FOR_BUILD) -c -o $@ @archdefs@ $(srcdir)/disassemble.c
endif

libopcodes_a_SOURCES =  dis-buf.c disassemble.c dis-init.c
# It's desirable to list ../bfd/libbfd.la in DEPENDENCIES and LIBADD.
# Unfortunately this causes libtool to add -L$(libdir), referring to the
# planned install directory of libbfd.  This can cause us to pick up an
# old version of libbfd, or to pick up libbfd for the wrong architecture
# if host != build. So for building with shared libraries we use a
# hardcoded path to libbfd.so instead of relying on the entries in libbfd.la.
libopcodes_a_DEPENDENCIES = $(OFILES) @SHARED_DEPENDENCIES@
libopcodes_a_LIBADD = $(OFILES) @SHARED_LIBADD@
libopcodes_a_LDFLAGS += -release `cat ../bfd/libtool-soversion` @SHARED_LDFLAGS@
# Allow dependency tracking to work on all the source files.
EXTRA_libopcodes_a_SOURCES = $(LIBOPCODES_CFILES)

# libtool will build .libs/libopcodes.a.  We create libopcodes.a in
# the build directory so that we don't have to convert all the
# programs that use libopcodes.a simultaneously.  This is a hack which
# should be removed if everything else starts using libtool.  FIXME.

noinst_LIBRARIES = libopcodes.a
#libopcodes_a_SOURCES =
#
#stamp-lib: libopcodes.la
#	libtooldir=`$(LIBTOOL) --config | sed -n -e 's/^objdir=//p'`; \
#	if [ -f $$libtooldir/libopcodes.a ]; then \
#	  cp $$libtooldir/libopcodes.a libopcodes.tmp; \
#	  $(RANLIB) libopcodes.tmp; \
#	  $(SHELL) $(srcdir)/../move-if-change libopcodes.tmp libopcodes.a; \
#	else true; fi
#	touch stamp-lib
#
#libopcodes.a: stamp-lib ; @true

POTFILES = $(HFILES) $(CFILES)
po/POTFILES.in: @MAINT@ Makefile
	for f in $(POTFILES); do echo $$f; done | LC_ALL=C sort > tmp \
	  && mv tmp $(srcdir)/po/POTFILES.in

CLEANFILES = \
	libopcodes.a stamp-lib


CGENDIR = @cgendir@
CPUDIR = $(srcdir)/../cpu
CGEN = "`if test -f ../guile/libguile/guile ; then echo ../guile/libguile/guile; else echo guile ; fi` -l ${cgendir}/guile.scm -s"
CGENFLAGS = -v

CGENDEPS = \
	$(CGENDIR)/desc.scm $(CGENDIR)/desc-cpu.scm \
	$(CGENDIR)/opcodes.scm $(CGENDIR)/opc-asmdis.scm \
	$(CGENDIR)/opc-ibld.scm $(CGENDIR)/opc-itab.scm \
	$(CGENDIR)/opc-opinst.scm \
	cgen-asm.in cgen-dis.in cgen-ibld.in

CGEN_CPUS =

EPIPHANY_DEPS =
FR30_DEPS =
FRV_DEPS =
IP2K_DEPS =
IQ2000_DEPS =
LM32_DEPS =
M32C_DEPS =
M32R_DEPS =
MEP_DEPS =
MT_DEPS =
OR1K_DEPS =
XC16X_DEPS =
XSTORMY16_DEPS =

run-cgen:
	$(SHELL) $(srcdir)/cgen.sh opcodes $(srcdir) $(CGEN) \
	  $(CGENDIR) "$(CGENFLAGS)" $(arch) $(prefix) $(archfile) $(opcfile) \
	  "$(options)" "$(extrafiles)"
	touch stamp-${prefix}
.PHONY: run-cgen

# Maintainer utility rule to regenerate all cgen files.
run-cgen-all:
	for c in $(CGEN_CPUS) ; \
	do \
		$(MAKE) stamp-$$c || exit 1 ; \
	done
.PHONY: run-cgen-all

MOSTLYCLEANFILES =

MAINTAINERCLEANFILES =

opc2c$(EXEEXT_FOR_BUILD): opc2c.o $(BUILD_LIBS_DEPS)
	$(LINK_FOR_BUILD) opc2c.o $(BUILD_LIBS)

opc2c.o: opc2c.c $(INCDIR)/libiberty.h
	$(COMPILE_FOR_BUILD) -c $(srcdir)/opc2c.c
