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
srcdir = .
VPATH = $(srcdir)

CFLAGS = -g -std=gnu11 -Wunused-function

CC = gcc

md_file=$(srcdir)/thumb.md

# End of variables for you to override.

# This tells GNU Make version 3 not to put all variables in the environment.
.NOEXPORT:

HOST_RTL = rtl.o bitmap.o obstack.o
HOST_RTLANAL = rtlanal.o
HOST_PRINT = print-rtl.o

# Specify the directories to be searched for header files.
INCLUDES = -I. -I$(srcdir)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

# This tells GNU make version 3 not to export all the variables
# defined in this file into the environment.
.NOEXPORT:

# Lists of files for various purposes.

OBJS = toplev.o version.o tree.o print-tree.o stor-layout.o fold-const.o \
 function.o stmt.o except.o expr.o calls.o expmed.o explow.o optabs.o \
 varasm.o rtl.o print-rtl.o rtlanal.o emit-rtl.o genrtl.o real.o regmove.o \
 dwarf2out.o bitmap.o alias.o \
 integrate.o jump.o cse.o loop.o unroll.o flow.o stupid.o combine.o varray.o \
 regclass.o local-alloc.o global.o reload.o reload1.o caller-save.o gcse.o \
 insn-peep.o final.o recog.o \
 insn-opinit.o insn-recog.o insn-extract.o insn-output.o insn-emit.o \
 lcm.o \
 insn-attrtab.o thumb.o getpwd.o convert.o \
 dyn-string.o splay-tree.o graph.o sbitmap.o resource.o \
 c-parse.o c-lex.o c-decl.o c-typeck.o c-convert.o c-aux-info.o c-common.o \
 c-iterate.o obstack.o

GEN = genemit genoutput genrecog genextract genflags gencodes genconfig \
 genpeep gengenrtl gencheck genattr

# The files that "belong" in CONFIG_H are deliberately omitted
# because having them there would not be useful in actual practice.
# All they would do is cause complete recompilation every time
# one of the machine description files is edited.
# That may or may not be what one wants to do.
# If it is, rm *.o is an easy way to do it.
# CONFIG_H = $(host_xm_file) $(tm_file)
CONFIG_H =
RTL_BASE_H = rtl.h rtl.def machmode.h machmode.def
RTL_H = $(RTL_BASE_H) genrtl.h
TREE_H = tree.h real.h tree.def machmode.h machmode.def tree-check.h
BASIC_BLOCK_H = basic-block.h bitmap.h sbitmap.h
RECOG_H = recog.h
EXPR_H = expr.h insn-codes.h
REGS_H = regs.h varray.h machmode.h machmode.def

# The only suffixes we want for implicit rules are .c and .o, so clear
# the list and add them.  This speeds up GNU Make, and allows -r to work.
.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: clean

agbcc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(GEN) $(OBJS)

# Compiling object files from source files.

# C language specific files.

# CYGNUS LOCAL: built in build directory
c-parse.o : $(srcdir)/c-parse.c $(CONFIG_H) $(TREE_H) c-lex.h c-parse.h \
    c-tree.h input.h flags.h system.h toplev.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)/c-parse.c

# CYGNUS LOCAL: c-gperf.h really depends on c-parse.gperf.
$(srcdir)/c-gperf.h:
	gperf -L KR-C -F ', 0, 0' -p -j1 -i 1 -g -o -t -G -N is_reserved_word \
	   -k1,3,$$ $(srcdir)/c-parse.gperf >tmp-gperf.h
	 $(srcdir)/move-if-change tmp-gperf.h $(srcdir)/c-gperf.h

c-decl.o : c-decl.c $(CONFIG_H) system.h $(TREE_H) c-tree.h c-lex.h flags.h \
    output.h toplev.h
c-typeck.o : c-typeck.c $(CONFIG_H) system.h $(TREE_H) c-tree.h flags.h \
    output.h $(EXPR_H) $(RTL_H) toplev.h
# CYGNUS LOCAL: built in build directory
c-lex.o : c-lex.c $(CONFIG_H) system.h $(TREE_H) $(RTL_H) c-lex.h c-tree.h \
    c-parse.h input.h flags.h c-gperf.h toplev.h output.h
c-aux-info.o : c-aux-info.c  $(CONFIG_H) system.h $(TREE_H) c-tree.h flags.h
c-convert.o : c-convert.c $(CONFIG_H) system.h $(TREE_H) flags.h toplev.h
c-iterate.o: c-iterate.c $(CONFIG_H) system.h $(TREE_H) $(RTL_H) c-tree.h \
    flags.h toplev.h $(EXPR_H)
graph.o: graph.c $(CONFIG_H) system.h toplev.h flags.h output.h $(RTL_H) \
    hard-reg-set.h $(BASIC_BLOCK_H)
sbitmap.o: sbitmap.c $(CONFIG_H) system.h $(RTL_H) flags.h $(BASIC_BLOCK_H)

hash.o: hash.c hash.h system.h toplev.h

splay-tree.o: splay-tree.c splay-tree.h

# A file used by all variants of C.

c-common.o : c-common.c $(CONFIG_H) system.h $(TREE_H) c-tree.h c-lex.h \
	flags.h toplev.h output.h $(RTL_H)

# Language-independent files.

tree-check.h: s-check
s-check : gencheck $(srcdir)/move-if-change
	./gencheck > tmp-check.h
	$(srcdir)/move-if-change tmp-check.h tree-check.h
	touch s-check

gencheck : gencheck.o tree.def $(lang_tree_files)
	$(CC) $(CFLAGS) -o $@ \
	 gencheck.o

gencheck.o : gencheck.c config.h system.h
	$(CC) -c $(CFLAGS) $(INCLUDES) $(srcdir)/gencheck.c

dumpvers: dumpvers.c

version.o: version.c
obstack.o: obstack.c $(CONFIG_H)

convert.o: convert.c $(CONFIG_H) $(TREE_H) flags.h convert.h toplev.h

tree.o : tree.c $(CONFIG_H) system.h $(TREE_H) flags.h function.h toplev.h except.h
print-tree.o : print-tree.c $(CONFIG_H) system.h $(TREE_H)
stor-layout.o : stor-layout.c $(CONFIG_H) system.h $(TREE_H) flags.h \
   function.h $(EXPR_H) $(RTL_H) toplev.h except.h
fold-const.o : fold-const.c $(CONFIG_H) system.h $(TREE_H) flags.h toplev.h \
  $(RTL_H)

toplev.o : toplev.c $(CONFIG_H) system.h $(TREE_H) $(RTL_H) \
   flags.h input.h insn-attr.h defaults.h output.h \
   insn-codes.h insn-config.h $(RECOG_H) Makefile toplev.h \
   dwarf2out.h $(EXPR_H)

rtl.o : rtl.c $(CONFIG_H) system.h $(RTL_H) bitmap.h

print-rtl.o : print-rtl.c $(CONFIG_H) system.h $(RTL_H) bitmap.h
rtlanal.o : rtlanal.c $(CONFIG_H) system.h $(RTL_H)

varasm.o : varasm.c $(CONFIG_H) system.h $(TREE_H) $(RTL_H) flags.h \
   function.h defaults.h $(EXPR_H) hard-reg-set.h $(REGS_H) \
   output.h toplev.h except.h
function.o : function.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h \
   function.h insn-flags.h insn-codes.h $(EXPR_H) $(REGS_H) hard-reg-set.h \
   insn-config.h $(RECOG_H) output.h toplev.h except.h
stmt.o : stmt.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h function.h  \
   insn-flags.h insn-config.h insn-codes.h hard-reg-set.h $(EXPR_H) except.h \
   loop.h $(RECOG_H) toplev.h output.h varray.h
except.o : except.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h \
   function.h insn-flags.h $(EXPR_H) $(REGS_H) hard-reg-set.h \
   insn-config.h $(RECOG_H) output.h except.h toplev.h
expr.o : expr.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h function.h \
   $(REGS_H) insn-flags.h insn-codes.h $(EXPR_H) insn-config.h $(RECOG_H) output.h \
   typeclass.h hard-reg-set.h toplev.h hard-reg-set.h except.h
calls.o : calls.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h $(EXPR_H) \
   insn-flags.h $(REGS_H) toplev.h output.h
expmed.o : expmed.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-config.h insn-codes.h $(EXPR_H) $(RECOG_H) real.h
explow.o : explow.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h \
   hard-reg-set.h insn-config.h $(EXPR_H) $(RECOG_H) insn-flags.h insn-codes.h
optabs.o : optabs.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-config.h insn-codes.h $(EXPR_H) $(RECOG_H) reload.h
dwarf2out.o : dwarf2out.c $(CONFIG_H) system.h $(TREE_H) $(RTL_H) dwarf2.h \
   flags.h insn-config.h reload.h output.h defaults.h \
   hard-reg-set.h $(REGS_H) $(EXPR_H) toplev.h dwarf2out.h dyn-string.h
emit-rtl.o : emit-rtl.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h \
   except.h function.h $(REGS_H) insn-config.h $(RECOG_H) real.h \
   $(EXPR_H) obstack.h hard-reg-set.h bitmap.h
real.o : real.c $(CONFIG_H) system.h $(TREE_H) toplev.h
getpwd.o : getpwd.c $(CONFIG_H) system.h

integrate.o : integrate.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h \
   integrate.h insn-flags.h insn-config.h $(EXPR_H) real.h $(REGS_H) \
   function.h output.h $(RECOG_H) except.h toplev.h

jump.o : jump.c $(CONFIG_H) system.h $(RTL_H) flags.h hard-reg-set.h $(REGS_H) \
   insn-config.h insn-flags.h $(RECOG_H) $(EXPR_H) real.h except.h \
   toplev.h
stupid.o : stupid.c $(CONFIG_H) system.h $(RTL_H) $(REGS_H) hard-reg-set.h \
   $(BASIC_BLOCK_H) insn-config.h reload.h flags.h toplev.h

cse.o : cse.c $(CONFIG_H) system.h $(RTL_H) $(REGS_H) hard-reg-set.h flags.h \
   real.h insn-config.h $(RECOG_H) $(EXPR_H) toplev.h output.h
gcse.o : gcse.c $(CONFIG_H) system.h $(RTL_H) $(REGS_H) hard-reg-set.h flags.h \
   real.h insn-config.h $(RECOG_H) $(EXPR_H) $(BASIC_BLOCK_H) output.h
resource.o : resource.c $(CONFIG_H) $(RTL_H) hard-reg-set.h system.h \
   $(BASIC_BLOCK_H) $(REGS_H) flags.h output.h resource.h
lcm.o : lcm.c $(CONFIG_H) system.h $(RTL_H) $(REGS_H) hard-reg-set.h flags.h \
   real.h insn-config.h $(RECOG_H) $(EXPR_H) $(BASIC_BLOCK_H)
loop.o : loop.c $(CONFIG_H) system.h $(RTL_H) flags.h loop.h insn-config.h \
   insn-flags.h $(REGS_H) hard-reg-set.h $(RECOG_H) $(EXPR_H) real.h \
   toplev.h varray.h
unroll.o : unroll.c $(CONFIG_H) system.h $(RTL_H) insn-config.h \
   integrate.h $(REGS_H) $(RECOG_H) flags.h $(EXPR_H) loop.h toplev.h varray.h
flow.o : flow.c $(CONFIG_H) system.h $(RTL_H) flags.h insn-config.h \
   $(BASIC_BLOCK_H) $(REGS_H) hard-reg-set.h output.h toplev.h recog.h
combine.o : combine.c $(CONFIG_H) system.h $(RTL_H) flags.h  \
   insn-config.h insn-flags.h insn-codes.h insn-attr.h $(REGS_H) $(EXPR_H) \
   $(BASIC_BLOCK_H) $(RECOG_H) real.h hard-reg-set.h toplev.h
regclass.o : regclass.c $(CONFIG_H) system.h $(RTL_H) hard-reg-set.h flags.h \
   $(BASIC_BLOCK_H) $(REGS_H) insn-config.h $(RECOG_H) reload.h real.h toplev.h \
   output.h
local-alloc.o : local-alloc.c $(CONFIG_H) system.h $(RTL_H) flags.h \
   $(BASIC_BLOCK_H) $(REGS_H) hard-reg-set.h insn-config.h $(RECOG_H) output.h \
   insn-attr.h toplev.h
bitmap.o : bitmap.c $(CONFIG_H) system.h $(RTL_H) flags.h $(BASIC_BLOCK_H) \
   $(REGS_H)
global.o : global.c $(CONFIG_H) system.h $(RTL_H) flags.h  reload.h \
   $(BASIC_BLOCK_H) $(REGS_H) hard-reg-set.h insn-config.h output.h toplev.h
varray.o : varray.c $(CONFIG_H) system.h varray.h $(RTL_H) $(TREE_H) bitmap.h

reload.o : reload.c $(CONFIG_H) system.h $(RTL_H) flags.h output.h $(EXPR_H) \
   reload.h $(RECOG_H) hard-reg-set.h insn-config.h insn-codes.h $(REGS_H) \
   real.h toplev.h
reload1.o : reload1.c $(CONFIG_H) system.h $(RTL_H) real.h flags.h $(EXPR_H) \
   reload.h $(REGS_H) hard-reg-set.h insn-config.h insn-flags.h insn-codes.h \
   $(BASIC_BLOCK_H) $(RECOG_H) output.h toplev.h
caller-save.o : caller-save.c $(CONFIG_H) system.h $(RTL_H) flags.h \
   $(REGS_H) hard-reg-set.h insn-config.h $(BASIC_BLOCK_H) \
   $(RECOG_H) reload.h $(EXPR_H) toplev.h
alias.o : alias.c $(CONFIG_H) system.h $(RTL_H) flags.h hard-reg-set.h \
   $(REGS_H) toplev.h output.h $(EXPR_H) 
regmove.o : regmove.c $(CONFIG_H) system.h $(RTL_H) insn-config.h \
   $(RECOG_H) output.h reload.h $(REGS_H) hard-reg-set.h flags.h \
   $(EXPR_H) insn-flags.h $(BASIC_BLOCK_H) toplev.h
final.o : final.c $(CONFIG_H) system.h $(RTL_H) $(TREE_H) flags.h $(REGS_H) \
   $(RECOG_H) conditions.h insn-config.h insn-attr.h except.h real.h output.h \
   hard-reg-set.h insn-flags.h insn-codes.h defaults.h \
   toplev.h reload.h dwarf2out.h
recog.o : recog.c $(CONFIG_H) system.h $(RTL_H)  \
   $(REGS_H) $(RECOG_H) hard-reg-set.h flags.h insn-config.h insn-attr.h \
   insn-flags.h insn-codes.h real.h toplev.h
dyn-string.o: dyn-string.c dyn-string.h $(CONFIG_H) system.h

thumb.o: thumb.c $(CONFIG_H) $(TREE_H) \
   $(RTL_H) $(REGS_H) hard-reg-set.h real.h insn-config.h conditions.h \
   insn-flags.h output.h insn-attr.h insn-codes.h system.h toplev.h

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

insn-emit.o : insn-emit.c $(CONFIG_H) $(RTL_H) $(EXPR_H) real.h output.h \
  insn-config.h insn-flags.h insn-codes.h system.h reload.h recog.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-emit.c

insn-emit.c: s-emit
s-emit : $(md_file) genemit $(srcdir)/move-if-change
	./genemit $(md_file) > tmp-emit.c
	$(srcdir)/move-if-change tmp-emit.c insn-emit.c
	touch s-emit

insn-recog.o : insn-recog.c $(CONFIG_H) $(RTL_H) insn-config.h $(RECOG_H) \
  real.h output.h flags.h system.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-recog.c

insn-recog.c: s-recog
s-recog : $(md_file) genrecog $(srcdir)/move-if-change
	./genrecog $(md_file) > tmp-recog.c
	$(srcdir)/move-if-change tmp-recog.c insn-recog.c
	touch s-recog

insn-opinit.o : insn-opinit.c $(CONFIG_H) $(RTL_H) insn-codes.h insn-flags.h \
  insn-config.h flags.h $(RECOG_H) $(EXPR_H) reload.h system.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-opinit.c

insn-opinit.c: s-opinit
s-opinit : $(md_file) genopinit $(srcdir)/move-if-change
	./genopinit $(md_file) > tmp-opinit.c
	$(srcdir)/move-if-change tmp-opinit.c insn-opinit.c
	touch s-opinit

insn-extract.o : insn-extract.c $(CONFIG_H) $(RTL_H) system.h toplev.h \
  insn-config.h recog.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-extract.c

insn-extract.c: s-extract
s-extract : $(md_file) genextract $(srcdir)/move-if-change
	./genextract $(md_file) > tmp-extract.c
	$(srcdir)/move-if-change tmp-extract.c insn-extract.c
	touch s-extract

insn-peep.o : insn-peep.c $(CONFIG_H) $(RTL_H) $(REGS_H) output.h real.h \
	system.h insn-config.h recog.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-peep.c

insn-peep.c: s-peep
s-peep : $(md_file) genpeep $(srcdir)/move-if-change
	./genpeep $(md_file) > tmp-peep.c
	$(srcdir)/move-if-change tmp-peep.c insn-peep.c
	touch s-peep

insn-attrtab.o : insn-attrtab.c $(CONFIG_H) $(RTL_H) $(REGS_H) real.h \
    output.h insn-attr.h insn-config.h system.h toplev.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-attrtab.c

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

insn-output.o : insn-output.c $(CONFIG_H) $(RTL_H) $(REGS_H) real.h conditions.h \
    hard-reg-set.h insn-config.h insn-flags.h insn-attr.h output.h $(RECOG_H) \
    insn-codes.h system.h
	$(CC) $(CFLAGS) $(INCLUDES) -c insn-output.c

insn-output.c: s-output
s-output : $(md_file) genoutput $(srcdir)/move-if-change
	./genoutput $(md_file) > tmp-output.c
	$(srcdir)/move-if-change tmp-output.c insn-output.c
	touch s-output

genrtl.o : genrtl.c $(CONFIG_H) $(RTL_H) system.h
genrtl.c genrtl.h : s-genrtl
	@true	# force gnu make to recheck modification times.

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

genconfig : genconfig.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	  genconfig.o $(HOST_RTL) $(HOST_PRINT)

genconfig.o : genconfig.c $(RTL_H) config.h system.h

genflags : genflags.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genflags.o $(HOST_RTL) $(HOST_PRINT)

genflags.o : genflags.c $(RTL_H) config.h system.h

gencodes : gencodes.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 gencodes.o $(HOST_RTL) $(HOST_PRINT)

gencodes.o : gencodes.c $(RTL_H) config.h system.h

genemit : genemit.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genemit.o $(HOST_RTL) $(HOST_PRINT)

genemit.o : genemit.c $(RTL_H) config.h system.h

genopinit : genopinit.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genopinit.o $(HOST_RTL) $(HOST_PRINT)

genopinit.o : genopinit.c $(RTL_H) config.h system.h

genrecog : genrecog.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genrecog.o $(HOST_RTL) $(HOST_PRINT)

genrecog.o : genrecog.c $(RTL_H) config.h system.h

genextract : genextract.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genextract.o $(HOST_RTL) $(HOST_PRINT)

genextract.o : genextract.c $(RTL_H) config.h system.h insn-config.h

genpeep : genpeep.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genpeep.o $(HOST_RTL) $(HOST_PRINT)

genpeep.o : genpeep.c $(RTL_H) config.h system.h

genattr : genattr.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genattr.o $(HOST_RTL) $(HOST_PRINT)

genattr.o : genattr.c $(RTL_H) config.h system.h

genattrtab : genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_RTLANAL)
	$(CC) $(CFLAGS) -o $@ \
	 genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_RTLANAL)

genattrtab.o : genattrtab.c $(RTL_H)  config.h system.h insn-config.h

genoutput : genoutput.o $(HOST_RTL) $(HOST_PRINT)
	$(CC) $(CFLAGS) -o $@ \
	 genoutput.o $(HOST_RTL) $(HOST_PRINT)

genoutput.o : genoutput.c $(RTL_H) config.h system.h

gengenrtl : gengenrtl.o
	$(CC) $(CFLAGS) -o $@ \
	 gengenrtl.o

gengenrtl.o : gengenrtl.c $(RTL_BASE_H) system.h
