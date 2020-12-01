# Makefile for GNU C compiler.
#   Copyright (C) 1987, 88, 90-98, 1999 Free Software Foundation, Inc.

#This file is part of GNU CC.

#GNU CC is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2, or (at your option)
#any later version.

#GNU CC is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with GNU CC; see the file COPYING.  If not, write to
#the Free Software Foundation, 59 Temple Place - Suite 330,
#Boston MA 02111-1307, USA.

# Directory where sources are, from where we are.
srcdir ?= .
VPATH = $(srcdir)

CC = gcc

BASE_CFLAGS = -g -std=gnu11

INCLUDES = -I. -I$(srcdir)

md_file=$(srcdir)/thumb.md

RTL_SRCS = rtl.c bitmap.c obstack.c
RTL_OBJS = rtl.o bitmap.o obstack.o
RTLANAL_SRCS = rtlanal.c
RTLANAL_OBJS = rtlanal.o
PRINT_SRCS = print-rtl.c
PRINT_OBJS = print-rtl.o

SRCS = toplev.c version.c tree.c print-tree.c stor-layout.c fold-const.c \
function.c stmt.c except.c expr.c calls.c expmed.c explow.c optabs.c \
varasm.c emit-rtl.c genrtl.c real.c regmove.c dwarf2out.c alias.c integrate.c \
jump.c cse.c loop.c unroll.c flow.c stupid.c combine.c varray.c regclass.c \
local-alloc.c global.c reload.c reload1.c caller-save.c gcse.c insn-peep.c \
final.c recog.c insn-opinit.c insn-recog.c insn-extract.c insn-output.c \
insn-emit.c lcm.c insn-attrtab.c thumb.c getpwd.c convert.c dyn-string.c \
splay-tree.c graph.c sbitmap.c resource.c c-parse.c c-lex.c c-decl.c \
c-typeck.c c-convert.c c-aux-info.c c-common.c c-iterate.c

OBJS = $(SRCS:.c=.o)

GENERATED = genrtl.c genrtl.h \
 insn-peep.c insn-opinit.c insn-recog.c insn-extract.c insn-output.c \
 insn-emit.c insn-attrtab.c \
 tree-check.h \
 insn-config.h insn-flags.h insn-codes.h insn-attr.h

.PHONY: normal old clean

# The usual compiler.
normal: CFLAGS = $(BASE_CFLAGS)
normal: $(OBJS)
	$(CC) $(CFLAGS) -o agbcc $(OBJS) $(RTL_OBJS) $(PRINT_OBJS) $(RTLANAL_OBJS)

# The old version of the compiler, used for building libgcc.
old: CFLAGS = $(BASE_CFLAGS) -DOLD_COMPILER
old: $(OBJS)
	$(CC) $(CFLAGS) -o old_agbcc $(OBJS) $(RTL_OBJS) $(PRINT_OBJS) $(RTLANAL_OBJS)

# Clear suffixes.
.SUFFIXES:

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(OBJS): %.o: %.c $(DEPDIR)/%.d | $(GENERATED)
	$(COMPILE) $<
	$(POSTCOMPILE)

$(RTL_OBJS) $(RTLANAL_OBJS) $(PRINT_OBJS): %.o: %.c $(DEPDIR)/%.d genrtl.h
	$(COMPILE) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

GEN = genemit genoutput genrecog genextract genflags gencodes genconfig \
genpeep gengenrtl gencheck genattr genattrtab genopinit

clean:
	$(RM) agbcc agbcc.exe old_agbcc old_agbcc.exe
	$(RM) $(OBJS) $(RTL_OBJS) $(RTLANAL_OBJS) $(PRINT_OBJS)
	$(RM) $(GENERATED)
	$(RM) $(GEN) $(addsuffix .exe,$(GEN)) $(addsuffix .o,$(GEN))
	$(RM) s-config s-flags s-codes s-emit s-recog s-opinit s-extract s-peep \
s-attr s-attrtab s-output s-genrtl s-check
	$(RM) -r $(DEPDIR)

# The files that "belong" in CONFIG_H are deliberately omitted
# because having them there would not be useful in actual practice.
# All they would do is cause complete recompilation every time
# one of the machine description files is edited.
# That may or may not be what one wants to do.
# If it is, rm *.o is an easy way to do it.
# CONFIG_H = $(xm_file) $(tm_file)
CONFIG_H =
RTL_BASE_H = rtl.h rtl.def machmode.h machmode.def
RTL_H = $(RTL_BASE_H) genrtl.h
TREE_H = tree.h real.h tree.def machmode.h machmode.def tree-check.h
BASIC_BLOCK_H = basic-block.h bitmap.h sbitmap.h
RECOG_H = recog.h
EXPR_H = expr.h insn-codes.h
REGS_H = regs.h varray.h machmode.h machmode.def

%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c $<

tree-check.h: s-check
s-check : gencheck $(srcdir)/move-if-change
	./gencheck > tmp-check.h
	$(srcdir)/move-if-change tmp-check.h tree-check.h
	touch s-check

gencheck : gencheck.o tree.def
	$(CC) $(CFLAGS) -o $@ gencheck.o

gencheck.o : gencheck.c config.h system.h

# Generate header and source files from the machine description,
# and compile them.

.PRECIOUS: insn-config.h insn-flags.h insn-codes.h \
  insn-emit.c insn-recog.c insn-extract.c insn-output.c insn-peep.c \
  insn-attr.h insn-attrtab.c

# The following pair of rules has this effect:
# genconfig is run only if the md has changed since genconfig was last run;
# but the file insn-config.h is touched only when its contents actually change.

# Each of the other insn-* files is handled by a similar pair of rules.

# This causes an anomaly in the results of make -n
# because insn-* is older than s-*
# and thus make -n thinks that insn-* will be updated
# and force recompilation of things that depend on it.
# We use move-if-change precisely to avoid such recompilation.
# But there is no way to teach make -n that it will be avoided.

insn-config.h: s-config
s-config : $(md_file) genconfig $(srcdir)/move-if-change
	./genconfig $(md_file) > tmp-config.h
	$(srcdir)/move-if-change tmp-config.h insn-config.h
	touch s-config

insn-flags.h: s-flags
s-flags : $(md_file) genflags $(srcdir)/move-if-change
	./genflags $(md_file) > tmp-flags.h
	$(srcdir)/move-if-change tmp-flags.h insn-flags.h
	touch s-flags

insn-codes.h: s-codes
s-codes : $(md_file) gencodes $(srcdir)/move-if-change
	./gencodes $(md_file) > tmp-codes.h
	$(srcdir)/move-if-change tmp-codes.h insn-codes.h
	touch s-codes

insn-emit.c: s-emit
s-emit : $(md_file) genemit $(srcdir)/move-if-change
	./genemit $(md_file) > tmp-emit.c
	$(srcdir)/move-if-change tmp-emit.c insn-emit.c
	touch s-emit

insn-recog.c: s-recog
s-recog : $(md_file) genrecog $(srcdir)/move-if-change
	./genrecog $(md_file) > tmp-recog.c
	$(srcdir)/move-if-change tmp-recog.c insn-recog.c
	touch s-recog

insn-opinit.c: s-opinit
s-opinit : $(md_file) genopinit $(srcdir)/move-if-change
	./genopinit $(md_file) > tmp-opinit.c
	$(srcdir)/move-if-change tmp-opinit.c insn-opinit.c
	touch s-opinit

insn-extract.c: s-extract
s-extract : $(md_file) genextract $(srcdir)/move-if-change
	./genextract $(md_file) > tmp-extract.c
	$(srcdir)/move-if-change tmp-extract.c insn-extract.c
	touch s-extract

insn-peep.c: s-peep
s-peep : $(md_file) genpeep $(srcdir)/move-if-change
	./genpeep $(md_file) > tmp-peep.c
	$(srcdir)/move-if-change tmp-peep.c insn-peep.c
	touch s-peep

insn-attr.h: s-attr
s-attr : $(md_file) genattr $(srcdir)/move-if-change
	./genattr $(md_file) > tmp-attr.h
	$(srcdir)/move-if-change tmp-attr.h insn-attr.h
	touch s-attr

insn-attrtab.c: s-attrtab
s-attrtab : $(md_file) genattrtab $(srcdir)/move-if-change
	./genattrtab $(md_file) > tmp-attrtab.c;
	$(srcdir)/move-if-change tmp-attrtab.c insn-attrtab.c
	touch s-attrtab

insn-output.c: s-output
s-output : $(md_file) genoutput $(srcdir)/move-if-change
	./genoutput $(md_file) > tmp-output.c
	$(srcdir)/move-if-change tmp-output.c insn-output.c
	touch s-output

genrtl.c genrtl.h : s-genrtl
s-genrtl: gengenrtl $(srcdir)/move-if-change $(RTL_BASE_H)
	./gengenrtl tmp-genrtl.h tmp-genrtl.c
	$(srcdir)/move-if-change tmp-genrtl.h genrtl.h
	$(srcdir)/move-if-change tmp-genrtl.c genrtl.c
	touch s-genrtl

# Compile the programs that generate insn-* from the machine description.

# $(CONFIG_H) is omitted from the deps of the gen*.o
# because these programs don't really depend on anything
# about the target machine.  They do depend on config.h itself,
# since that describes the host machine.

genconfig : genconfig.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genconfig.o $(RTL_OBJS) $(PRINT_OBJS)

genconfig.o : genconfig.c $(RTL_H) config.h system.h

genflags : genflags.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genflags.o $(RTL_OBJS) $(PRINT_OBJS)

genflags.o : genflags.c $(RTL_H) config.h system.h

gencodes : gencodes.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ gencodes.o $(RTL_OBJS) $(PRINT_OBJS)

gencodes.o : gencodes.c $(RTL_H) config.h system.h

genemit : genemit.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genemit.o $(RTL_OBJS) $(PRINT_OBJS)

genemit.o : genemit.c $(RTL_H) config.h system.h

genopinit : genopinit.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genopinit.o $(RTL_OBJS) $(PRINT_OBJS)

genopinit.o : genopinit.c $(RTL_H) config.h system.h

genrecog : genrecog.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genrecog.o $(RTL_OBJS) $(PRINT_OBJS)

genrecog.o : genrecog.c $(RTL_H) config.h system.h

genextract : genextract.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genextract.o $(RTL_OBJS) $(PRINT_OBJS)

genextract.o : genextract.c $(RTL_H) config.h system.h insn-config.h

genpeep : genpeep.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genpeep.o $(RTL_OBJS) $(PRINT_OBJS)

genpeep.o : genpeep.c $(RTL_H) config.h system.h

genattr : genattr.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genattr.o $(RTL_OBJS) $(PRINT_OBJS)

genattr.o : genattr.c $(RTL_H) config.h system.h

genattrtab : genattrtab.o $(RTL_OBJS) $(PRINT_OBJS) $(RTLANAL_OBJS)
	$(CC) $(CFLAGS) -o $@ genattrtab.o $(RTL_OBJS) $(PRINT_OBJS) $(RTLANAL_OBJS)

genattrtab.o : genattrtab.c $(RTL_H)  config.h system.h insn-config.h

genoutput : genoutput.o $(RTL_OBJS) $(PRINT_OBJS)
	$(CC) $(CFLAGS) -o $@ genoutput.o $(RTL_OBJS) $(PRINT_OBJS)

genoutput.o : genoutput.c $(RTL_H) config.h system.h

gengenrtl : gengenrtl.o
	$(CC) $(CFLAGS) -o $@ gengenrtl.o

gengenrtl.o : gengenrtl.c $(RTL_BASE_H) config.h system.h
