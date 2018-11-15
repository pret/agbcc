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
ACLOCAL_AMFLAGS = -I .. -I ../config -I ../bfd

tooldir = $(exec_prefix)/$(target_alias)

## These aren't set by automake, because they appear in
## bfd/acinclude.m4, which is included by binutils/acinclude.m4, and
## thus is not seen by automake.
CC_FOR_BUILD = @CC_FOR_BUILD@
EXEEXT_FOR_BUILD = @EXEEXT_FOR_BUILD@

WARN_CFLAGS = -Wall -Wwrite-strings
NO_WERROR =
AM_CFLAGS = $(WARN_CFLAGS)
AM_CFLAGS_FOR_BUILD = -Wall

# these two are almost the same program
AR_PROG=ar
RANLIB_PROG=ranlib

# objcopy and strip should be the same program
OBJCOPY_PROG=objcopy

# These should all be the same program too.
OBJDUMP_PROG=objdump

bin_PROGRAMS = $(OBJDUMP_PROG) $(AR_PROG) $(RANLIB_PROG) $(OBJCOPY_PROG)

bin_SCRIPTS = @BUILD_INSTALL_MISC@
EXTRA_SCRIPTS = embedspu


## We need a special rule to install the programs which are built with
## -new, and to rename cxxfilt to c++filt.

# Stuff that goes in tooldir/ if appropriate.
TOOL_PROGS = nm-new ar ranlib objdump objcopy

BASEDIR = $(srcdir)/..
BFDDIR = $(BASEDIR)/bfd
INCDIR	= $(BASEDIR)/include

AM_CPPFLAGS = -I. -I$(srcdir) -I../bfd -I$(BFDDIR) -I$(INCDIR) \
	 @HDEFINES@ \
	 -DLOCALEDIR="\"$(datadir)/locale\"" \
	 -Dbin_dummy_emulation=$(EMULATION_VECTOR)

HFILES = \
	arsup.h binemul.h bucomm.h budbg.h \
	coffgrok.h debug.h dwarf.h elfcomm.h \
	objdump.h sysdep.h

GENERATED_HFILES = arparse.h sysroff.h sysinfo.h defparse.h
BUILT_SOURCES = $(GENERATED_HFILES)

CFILES = \
	ar.c arsup.c bin2c.c binemul.c bucomm.c \
	coffdump.c coffgrok.c  \
	dwarf.c  \
	elfcomm.c filemode.c \
	is-ranlib.c is-strip.c maybe-ranlib.c maybe-strip.c \
	not-ranlib.c not-strip.c \
	objcopy.c objdump.c  \
	rename.c \
	syslex_wrap.c version.c mclex.c

GENERATED_CFILES = \
	arparse.c arlex.c sysroff.c sysinfo.c syslex.c \
	defparse.c deflex.c mcparse.c

DEBUG_SRCS =
WRITE_DEBUG_SRCS = $(DEBUG_SRCS)

# Extra object files for objdump
OBJDUMP_PRIVATE_OFILES = @OBJDUMP_PRIVATE_OFILES@

# Code shared by all the binutils.
BULIBS = bucomm.c version.c filemode.c

# Code shared by the ELF related programs.
ELFLIBS = elfcomm.c

BFDLIB = ../bfd/libbfd.a

LIBIBERTY = ../libiberty/libiberty.a

EXPECT = expect

installcheck-local:
	/bin/sh $(srcdir)/sanity.sh $(bindir)

objdump_DEPENDENCIES =   $(LIBIBERTY) $(BFDLIB) $(OBJDUMP_PRIVATE_OFILES)
ar_DEPENDENCIES =        $(LIBIBERTY) $(BFDLIB)
ranlib_DEPENDENCIES =    $(LIBIBERTY) $(BFDLIB)
objcopy_DEPENDENCIES =   $(LIBIBERTY) $(BFDLIB)

LDADD = $(BFDLIB) $(LIBIBERTY) -lz

objcopy_SOURCES = objcopy.c not-strip.c rename.c $(WRITE_DEBUG_SRCS) $(BULIBS)

objdump_SOURCES = objdump.c dwarf.c  $(DEBUG_SRCS) $(BULIBS) $(ELFLIBS)
EXTRA_objdump_SOURCES = od-xcoff.c
objdump_LDADD = $(OBJDUMP_PRIVATE_OFILES) $(BFDLIB) $(LIBIBERTY) -lz

objdump.@OBJEXT@:objdump.c
	$(COMPILE) -c $(OBJDUMP_DEFS) $(srcdir)/objdump.c

ar_SOURCES = arparse.c arlex.c ar.c not-ranlib.c arsup.c rename.c binemul.c \
	emul_$(EMULATION).c $(BULIBS)
EXTRA_ar_SOURCES = $(CFILES)
ar_LDADD = $(BFDLIB) $(LIBIBERTY) $(LEXLIB) -lz

ranlib_SOURCES = ar.c is-ranlib.c arparse.c arlex.c arsup.c rename.c \
	binemul.c emul_$(EMULATION).c $(BULIBS)
ranlib_LDADD = $(BFDLIB) $(LIBIBERTY) $(LEXLIB) -lz


# The following is commented out for the conversion to automake.
# This rule creates a single binary that switches between ar and ranlib
# by looking at argv[0].  Use this kludge to save some disk space.
# However, you have to install things by hand.
# (That is after 'make install', replace the installed ranlib by a link to ar.)
# Alternatively, you can install ranlib.sh as ranlib.
# ar_with_ranlib: $(ADDL_DEPS) ar.o maybe-ranlib.o
#	$(HLDENV) $(CC) $(HLDFLAGS) $(CFLAGS) $(LDFLAGS) -o $(AR_PROG) ar.o maybe-ranlib.o $(ADDL_LIBS) $(EXTRALIBS)
#	-rm -f $(RANLIB_PROG)
#	-ln $(AR_PROG) $(RANLIB_PROG)
#
# objcopy and strip in one binary that uses argv[0] to decide its action.
#
#objcopy_with_strip: $(ADDL_DEPS) objcopy.o maybe-strip.o
#	$(HLDENV) $(CC) $(HLDFLAGS) $(CFLAGS) $(LDFLAGS) -o $(OBJCOPY_PROG) objcopy.o maybe-strip.o $(ADDL_LIBS) $(EXTRALIBS)
#	-rm -f $(STRIP_PROG)
#	-ln $(OBJCOPY_PROG) $(STRIP_PROG)

sysroff.c: sysinfo$(EXEEXT_FOR_BUILD) sysroff.info
	./sysinfo$(EXEEXT_FOR_BUILD) -c <$(srcdir)/sysroff.info >sysroff.c
	./sysinfo$(EXEEXT_FOR_BUILD) -i <$(srcdir)/sysroff.info >>sysroff.c
	./sysinfo$(EXEEXT_FOR_BUILD) -g <$(srcdir)/sysroff.info >>sysroff.c

sysroff.h: sysinfo$(EXEEXT_FOR_BUILD) sysroff.info
	./sysinfo$(EXEEXT_FOR_BUILD) -d <$(srcdir)/sysroff.info >sysroff.h

sysinfo$(EXEEXT_FOR_BUILD): sysinfo.@OBJEXT@ syslex_wrap.@OBJEXT@
	$(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(LDFLAGS_FOR_BUILD) -o $@ sysinfo.@OBJEXT@ syslex_wrap.@OBJEXT@

syslex_wrap.@OBJEXT@: syslex_wrap.c syslex.c sysinfo.h config.h
	$(CC_FOR_BUILD) -c -I. -I$(srcdir) $(AM_CFLAGS_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(NO_WERROR) $(srcdir)/syslex_wrap.c

sysinfo.@OBJEXT@: sysinfo.c
	if [ -r sysinfo.c ]; then \
	  $(CC_FOR_BUILD) -c -I. $(AM_CFLAGS_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(NO_WERROR) sysinfo.c ; \
	else \
	  $(CC_FOR_BUILD) -c -I. $(AM_CFLAGS_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(NO_WERROR) $(srcdir)/sysinfo.c ; \
	fi

bin2c$(EXEEXT_FOR_BUILD): bin2c.c
	$(CC_FOR_BUILD) -o $@ $(AM_CPPFLAGS) $(AM_CFLAGS_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(LDFLAGS_FOR_BUILD) $(srcdir)/bin2c.c

embedspu: embedspu.sh Makefile
	awk '/^program_transform_name=/ {print "program_transform_name=\"$(program_transform_name)\""; next} {print}' < $< > $@
	chmod a+x $@

# We need these for parallel make.
sysinfo.h: sysinfo.c

# Disable -Werror, if it has been enabled, since old versions of bison/
# yacc will produce working code which contain compile time warnings.
arparse.@OBJEXT@: arparse.c
	$(COMPILE) -c `test -f arparse.c || echo $(srcdir)/`arparse.c $(NO_WERROR)

arlex.@OBJEXT@: arlex.c
	$(COMPILE) -c `test -f arlex.c || echo $(srcdir)/`arlex.c $(NO_WERROR)

sysroff.@OBJEXT@: sysroff.c
	$(COMPILE) -c `test -f sysroff.c || echo $(srcdir)/`sysroff.c $(NO_WERROR)

defparse.@OBJEXT@: defparse.c
	$(COMPILE) -c `test -f defparse.c || echo $(srcdir)/`defparse.c $(NO_WERROR)

deflex.@OBJEXT@: deflex.c
	$(COMPILE) -c `test -f deflex.c || echo $(srcdir)/`deflex.c $(NO_WERROR)

EXTRA_DIST = arparse.c arparse.h arlex.c sysinfo.c sysinfo.h \
	syslex.c deflex.c defparse.h defparse.c \
	mcparse.h mcparse.c embedspu.sh

diststuff: $(EXTRA_DIST) info
all: info

# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = $(BFDDIR)/development.sh

DISTCLEANFILES = sysroff.c sysroff.h site.exp site.bak embedspu

MOSTLYCLEANFILES = sysinfo$(EXEEXT_FOR_BUILD) bin2c$(EXEEXT_FOR_BUILD) \
  binutils.log binutils.sum abcdefgh*
mostlyclean-local:
	-rm -rf tmpdir

.PHONY: install-exec-local

install-exec-local: install-binPROGRAMS $(bin_PROGRAMS)
	@list='$(RENAMED_PROGS)'; for p in $$list; do \
	  if test -f $$p$(EXEEXT); then \
	    echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p$(EXEEXT) $(bindir)/`echo $$p|sed -e 's/-new//' |sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	    $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p$(EXEEXT) $(DESTDIR)$(bindir)/`echo $$p|sed -e 's/-new//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/bin
	for i in $(TOOL_PROGS); do \
	  if [ -f $$i$(EXEEXT) ]; then \
	    j=`echo $$i | sed -e 's/-new//'`; \
	    k=`echo $$j | sed '$(transform)'`; \
	    if [ "$(bindir)/$$k" != "$(tooldir)/bin/$$j" ]; then \
	      rm -f $(DESTDIR)$(tooldir)/bin/$$j$(EXEEXT); \
	      ln -s $(DESTDIR)$(bindir)/$$k$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$$j$(EXEEXT) >/dev/null 2>/dev/null \
		|| $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$i$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$$j$(EXEEXT); \
	    fi; \
	  else true; \
	  fi; \
	done

# Helper target that builds objects but doesn't link them
objs: $(ar_OBJECTS) $(objcopy_OBJECTS) $(objdump_OBJECTS) $(ranlib_OBJECTS)
.PHONY: objs
