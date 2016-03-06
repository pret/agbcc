/* Sets (bit vectors) of hard registers, and operations on them.
   Copyright (C) 1987, 1992, 1994 Free Software Foundation, Inc.

This file is part of GNU CC

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* Define the type of a set of hard registers.  */

/* HARD_REG_ELT_TYPE is a typedef of the unsigned integral type which
   will be used for hard reg sets, either alone or in an array.

   HARD_REG_SET is a macro. Its definition is HARD_REG_ELT_TYPE,
   and it has enough bits to represent all the THUMB hard registers.

   Note that lots of code assumes that the first part of a regset is
   the same format as a HARD_REG_SET.  To help make sure this is true,
   we only try the widest integer mode (HOST_WIDE_INT) instead of all the
   smaller types.  This approach loses only if there are a very few
   registers and then only in the few cases where we have an array of
   HARD_REG_SETs, so it needn't be as complex as it used to be.  */

typedef HOST_WIDE_UINT HARD_REG_ELT_TYPE;

#define HARD_REG_SET HARD_REG_ELT_TYPE

/* HARD_CONST is used to cast a constant to the appropriate type
   for use with a HARD_REG_SET.  */

#define HARD_CONST(X) ((HARD_REG_ELT_TYPE) (X))

/* Define macros SET_HARD_REG_BIT, CLEAR_HARD_REG_BIT and TEST_HARD_REG_BIT
   to set, clear or test one bit in a hard reg set of type HARD_REG_SET.
   All three take two arguments: the set and the register number.

   In the case where sets are arrays of longs, the first argument
   is actually a pointer to a long.

   Define two macros for initializing a set:
   CLEAR_HARD_REG_SET and SET_HARD_REG_SET.
   These take just one argument.

   Also define macros for copying hard reg sets:
   COPY_HARD_REG_SET and COMPL_HARD_REG_SET.
   These take two arguments TO and FROM; they read from FROM
   and store into TO.  COMPL_HARD_REG_SET complements each bit.

   Also define macros for combining hard reg sets:
   IOR_HARD_REG_SET and AND_HARD_REG_SET.
   These take two arguments TO and FROM; they read from FROM
   and combine bitwise into TO.  Define also two variants
   IOR_COMPL_HARD_REG_SET and AND_COMPL_HARD_REG_SET
   which use the complement of the set FROM.

   Also define GO_IF_HARD_REG_SUBSET (X, Y, TO):
   if X is a subset of Y, go to TO.
*/

#define SET_HARD_REG_BIT(SET, BIT)  \
 ((SET) |= HARD_CONST (1) << (BIT))
#define CLEAR_HARD_REG_BIT(SET, BIT)  \
 ((SET) &= ~(HARD_CONST (1) << (BIT)))
#define TEST_HARD_REG_BIT(SET, BIT)  \
 ((SET) & (HARD_CONST (1) << (BIT)))

#define CLEAR_HARD_REG_SET(TO) ((TO) = HARD_CONST (0))
#define SET_HARD_REG_SET(TO) ((TO) = ~ HARD_CONST (0))

#define COPY_HARD_REG_SET(TO, FROM) ((TO) = (FROM))
#define COMPL_HARD_REG_SET(TO, FROM) ((TO) = ~(FROM))

#define IOR_HARD_REG_SET(TO, FROM) ((TO) |= (FROM))
#define IOR_COMPL_HARD_REG_SET(TO, FROM) ((TO) |= ~ (FROM))
#define AND_HARD_REG_SET(TO, FROM) ((TO) &= (FROM))
#define AND_COMPL_HARD_REG_SET(TO, FROM) ((TO) &= ~ (FROM))

#define GO_IF_HARD_REG_SUBSET(X,Y,TO) if (HARD_CONST (0) == ((X) & ~(Y))) goto TO

#define GO_IF_HARD_REG_EQUAL(X,Y,TO) if ((X) == (Y)) goto TO

/* Define some standard sets of registers.  */

/* Indexed by hard register number, contains 1 for registers
   that are fixed use (stack pointer, pc, frame pointer, etc.).
   These are the registers that cannot be used to allocate
   a pseudo reg whose life does not cross calls.  */

extern char fixed_regs[FIRST_PSEUDO_REGISTER];

/* The same info as a HARD_REG_SET.  */

extern HARD_REG_SET fixed_reg_set;

/* Indexed by hard register number, contains 1 for registers
   that are fixed use or are clobbered by function calls.
   These are the registers that cannot be used to allocate
   a pseudo reg whose life crosses calls.  */

extern char call_used_regs[FIRST_PSEUDO_REGISTER];

/* The same info as a HARD_REG_SET.  */

extern HARD_REG_SET call_used_reg_set;
  
/* Registers that we don't want to caller save.  */
extern HARD_REG_SET losing_caller_save_reg_set;

/* Indexed by hard register number, contains 1 for registers that are
   fixed use -- i.e. in fixed_regs -- or a function value return register
   or STRUCT_VALUE_REGNUM or STATIC_CHAIN_REGNUM.  These are the
   registers that cannot hold quantities across calls even if we are
   willing to save and restore them.  */

extern char call_fixed_regs[FIRST_PSEUDO_REGISTER];

/* The same info as a HARD_REG_SET.  */

extern HARD_REG_SET call_fixed_reg_set;

/* Indexed by hard register number, contains 1 for registers
   that are being used for global register decls.
   These must be exempt from ordinary flow analysis
   and are also considered fixed.  */

extern char global_regs[FIRST_PSEUDO_REGISTER];

/* For each reg class, a HARD_REG_SET saying which registers are in it.  */

extern HARD_REG_SET reg_class_contents[];

/* For each reg class, number of regs it contains.  */

extern int reg_class_size[N_REG_CLASSES];

/* For each reg class, table listing all the containing classes.  */

extern enum reg_class reg_class_superclasses[N_REG_CLASSES][N_REG_CLASSES];

/* For each reg class, table listing all the classes contained in it.  */

extern enum reg_class reg_class_subclasses[N_REG_CLASSES][N_REG_CLASSES];

/* For each pair of reg classes,
   a largest reg class contained in their union.  */

extern enum reg_class reg_class_subunion[N_REG_CLASSES][N_REG_CLASSES];

/* For each pair of reg classes,
   the smallest reg class that contains their union.  */

extern enum reg_class reg_class_superunion[N_REG_CLASSES][N_REG_CLASSES];

/* Number of non-fixed registers.  */

extern int n_non_fixed_regs;

/* Vector indexed by hardware reg giving its name.  */

extern char *reg_names[FIRST_PSEUDO_REGISTER];
