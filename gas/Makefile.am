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

AUTOMAKE_OPTIONS = foreign no-dist subdir-objects
ACLOCAL_AMFLAGS = -I .. -I ../config -I ../bfd

tooldir = $(exec_prefix)/$(target_alias)

WARN_CFLAGS = -Wall -Wwrite-strings
NO_WERROR =
AM_CFLAGS = $(WARN_CFLAGS)

TARG_CPU = arm
TARG_CPU_C = $(srcdir)/config/tc-arm.c
TARG_CPU_O = config/tc-arm.@OBJEXT@
TARG_CPU_H = $(srcdir)/config/tc-arm.h
OBJ_FORMAT_C = $(srcdir)/config/obj-elf.c
OBJ_FORMAT_O = config/obj-elf.@OBJEXT@
OBJ_FORMAT_H = $(srcdir)/config/obj-elf.h
TARG_ENV_H = $(srcdir)/config/te-armeabi.h
ATOF_TARG_C = $(srcdir)/config/atof-ieee.c
ATOF_TARG_O = config/atof-ieee.@OBJEXT@

# use @target_cpu_type@ for refering to configured target name


# Regular source files.

GAS_CFILES = \
	app.c \
	as.c \
	atof-generic.c \
	compress-debug.c \
	cond.c \
	depend.c \
	dwarf2dbg.c \
	dw2gencfi.c \
	ehopt.c \
	expr.c \
	flonum-copy.c \
	flonum-konst.c \
	flonum-mult.c \
	frags.c \
	hash.c \
	input-file.c \
	input-scrub.c \
	listing.c \
	macro.c \
	messages.c \
	output-file.c \
	read.c \
	remap.c \
	sb.c \
	subsegs.c \
	symbols.c \
	write.c

CFILES = $(GAS_CFILES)

HFILES = \
	as.h \
	asintl.h \
	bignum.h \
	bit_fix.h \
	compress-debug.h \
	dwarf2dbg.h \
	dw2gencfi.h \
	emul-target.h \
	expr.h \
	flonum.h \
	frags.h \
	hash.h \
	input-file.h \
	listing.h \
	macro.h \
	obj.h \
	output-file.h \
	read.h \
	sb.h \
	struc-symbol.h \
	subsegs.h \
	symbols.h \
	tc.h \
	write.h

# CPU files in config.

TARGET_CPU_CFILES = \
	config/tc-arm.c

TARGET_CPU_HFILES = \
	config/tc-arm.h

# OBJ files in config

OBJ_FORMAT_CFILES = \
	config/obj-elf.c

OBJ_FORMAT_HFILES = \
	config/obj-elf.h \
	config/obj-fdpicelf.h

# Emulation header files in config

TARG_ENV_HFILES = \
	config/te-armeabi.h \
	config/te-generic.h

TARG_ENV_CFILES =

# Multi files in config

MULTI_CFILES =

CONFIG_ATOF_CFILES = \
	config/atof-ieee.c

# Note: GASP is now deprecated and has been removed.  It is still
# available in the CVS archive or older binutils releases if it is needed.
noinst_PROGRAMS = as-new
noinst_SCRIPTS =
EXTRA_SCRIPTS =


diststuff: info

DISTCLEANFILES = targ-cpu.h obj-format.h targ-env.h

# Now figure out from those variables how to compile and link.

BASEDIR = $(srcdir)/..
BFDDIR = $(BASEDIR)/bfd
INCDIR = $(BASEDIR)/include

# This is the variable actually used when we compile.
# Specify the directories to be searched for header files.
# Both . and srcdir are used, in that order,
# so that tm.h and config.h will be found in the compilation
# subdirectory rather than in the source directory.
AM_CPPFLAGS = -I. -I$(srcdir) -I../bfd -I$(srcdir)/config \
	-I$(INCDIR) -I$(srcdir)/.. -I$(BFDDIR)

# How to link with both our special library facilities
# and the system's installed libraries.

GASLIBS = ../bfd/libbfd.a ../libiberty/libiberty.a

# Files to be copied away after each stage in building.
STAGESTUFF = *.@OBJEXT@ $(noinst_PROGRAMS)

as_new_SOURCES = $(GAS_CFILES)
as_new_LDADD = $(TARG_CPU_O) $(OBJ_FORMAT_O) $(ATOF_TARG_O) \
	$(extra_objects) $(GASLIBS) $(LIBM) -lz
as_new_DEPENDENCIES = $(TARG_CPU_O) $(OBJ_FORMAT_O) $(ATOF_TARG_O) \
	$(extra_objects) $(GASLIBS)
EXTRA_as_new_SOURCES = $(CFILES) $(HFILES) $(TARGET_CPU_CFILES) \
	$(TARGET_CPU_HFILES) $(OBJ_FORMAT_CFILES) $(OBJ_FORMAT_HFILES) \
	$(TARG_ENV_CFILES) $(CONFIG_ATOF_CFILES) $(MULTI_CFILES)

# Remake the info files.

MOSTLYCLEANFILES = $(STAGESTUFF) core \
	testsuite/*.@OBJEXT@ testsuite/*.out testsuite/gas.log testsuite/gas.sum \
	testsuite/site.exp site.bak site.exp stage stage1 stage2

.PHONY: install-exec-local install-data-local
.PHONY: install-exec-bindir install-exec-tooldir

install-exec-local: install-exec-bindir @install_tooldir@

install-exec-bindir: $(noinst_PROGRAMS)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(noinst_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "$(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(bindir)/`echo $$p|sed -e 's/$(EXEEXT)$$//' -e 's/-new//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	    $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed -e 's/$(EXEEXT)$$//' -e 's/-new//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

install-exec-tooldir: install-exec-bindir $(noinst_PROGRAMS)
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/bin
	n=`echo as | sed '$(transform)'`; \
	if [ "$(bindir)/$$n$(EXEEXT)" != "$(tooldir)/bin/as$(EXEEXT)" ]; then \
	  rm -f $(DESTDIR)$(tooldir)/bin/as$(EXEEXT); \
	  ln -s $(DESTDIR)$(bindir)/$$n$(EXEEXT) $(DESTDIR)$(tooldir)/bin/as$(EXEEXT) >/dev/null 2>/dev/null \
	    || $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) as-new$(EXEEXT) $(DESTDIR)$(tooldir)/bin/as$(EXEEXT); \
	else \
	  true ; \
	fi
# Reconfigure if configure.tgt changes.
# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = $(BFDDIR)/development.sh

objs: $(TARG_CPU_O) $(OBJ_FORMAT_O) $(ATOF_TARG_O) $(as_new_OBJECTS)
.PHONY: objs

