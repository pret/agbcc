/* CYGNUS LOCAL -- meissner/d10v abi change */
/* Definitions of target machine for Mitsubishi D10V.
   Copyright (C) 1996, 1997, 1998 Free Software Foundation, Inc.
   Contributed by Cygnus Support.

This file is part of GNU CC.

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

/* Include generic ELF definitions */

#include "svr4.h"

/* D10v specific macros */

/* Align an address */
#define D10V_ALIGN(addr,align) (((addr) + (align) - 1) & ~((align) - 1))

/* Truncate and sign extend a value to 16 bits.  */
#define SIGN_EXTEND_SHORT(VALUE) ((int)((((VALUE) & 0xffff) ^ 0x8000) - 0x8000))


/* Driver configuration */

/* A C string constant that tells the GNU CC driver program options to
   pass to CPP.  It can also specify how to translate options you
   give to GNU CC into options for GNU CC to pass to the CPP.

   Do not define this macro if it does not need to do anything.  */
#define CPP_SPEC "\
%{mint32:  -D__INT__=32 -D__INT_MAX__=2147483647} \
%{!mint32: -D__INT__=16 -D__INT_MAX__=32767} \
%{mdouble64: -D__DOUBLE__=64} \
%{!mdouble64: -D__DOUBLE__=32}"

/* A C string constant that tells the GNU CC driver program options to
   pass to `cc1'.  It can also specify how to translate options you
   give to GNU CC into options for GNU CC to pass to the `cc1'.

   Do not define this macro if it does not need to do anything.  */
/* #define CC1_SPEC */

/* A C string constant that tells the GNU CC driver program options to
   pass to `cc1plus'.  It can also specify how to translate options
   you give to GNU CC into options for GNU CC to pass to the
   `cc1plus'.

   Do not define this macro if it does not need to do anything.  */
/* #define CC1PLUS_SPEC */

/* A C string constant that tells the GNU CC driver program options to
   pass to the assembler.  It can also specify how to translate
   options you give to GNU CC into options for GNU CC to pass to the
   assembler.  See the file `sun3.h' for an example of this.

   Do not define this macro if it does not need to do anything.  */
#undef	ASM_SPEC
#if 1
#define ASM_SPEC "%{!mno-asm-optimize: %{O*: %{!O0: -O} %{O0: %{masm-optimize: -O}}}}"

#else
#define ASM_SPEC "%{masm-optimize: %{O*: %{!O0: -O}}} %{mno-asm-optimize: }"
#endif

/* A C string constant that tells the GNU CC driver program how to
   run any programs which cleanup after the normal assembler.
   Normally, this is not needed.  See the file `mips.h' for an
   example of this.

   Do not define this macro if it does not need to do anything.  */
/* #define ASM_FINAL_SPEC */

/* A C string constant that tells the GNU CC driver program options to
   pass to the linker.  It can also specify how to translate options
   you give to GNU CC into options for GNU CC to pass to the linker.

   Do not define this macro if it does not need to do anything.  */
/* #define LINK_SPEC */

/* Another C string constant used much like `LINK_SPEC'.  The
   difference between the two is that `LIB_SPEC' is used at the end
   of the command given to the linker.

   If this macro is not defined, a default is provided that loads the
   standard C library from the usual place.  See `gcc.c'.  */
#undef	LIB_SPEC
#define LIB_SPEC "%{msim: %e-msim is no longer supported} -lc -lnosys"

/* Another C string constant that tells the GNU CC driver program how
   and when to place a reference to `libgcc.a' into the linker
   command line.  This constant is placed both before and after the
   value of `LIB_SPEC'.

   If this macro is not defined, the GNU CC driver provides a default
   that passes the string `-lgcc' to the linker unless the `-shared'
   option is specified.  */
/* #define LIBGCC_SPEC */

/* Another C string constant used much like `LINK_SPEC'.  The
   difference between the two is that `STARTFILE_SPEC' is used at the
   very beginning of the command given to the linker.

   If this macro is not defined, a default is provided that loads the
   standard C startup file from the usual place.  See `gcc.c'.  */
#undef	STARTFILE_SPEC
#define STARTFILE_SPEC "crt0%O%s"


/* Another C string constant used much like `LINK_SPEC'.  The
   difference between the two is that `ENDFILE_SPEC' is used at the
   very end of the command given to the linker.

   Do not define this macro if it does not need to do anything.  */
#undef	ENDFILE_SPEC
#define ENDFILE_SPEC ""

/* Define this macro to provide additional specifications to put in
   the `specs' file that can be used in various specifications like
   `CC1_SPEC'.

   The definition should be an initializer for an array of structures,
   containing a string constant, that defines the specification name,
   and a string constant that provides the specification.

   Do not define this macro if it does not need to do anything.  */
/* #define EXTRA_SPECS */

/* Define this macro as a C expression for the initializer of an
   array of string to tell the driver program which options are
   defaults for this target and thus do not need to be handled
   specially when using `MULTILIB_OPTIONS'.

   Do not define this macro if `MULTILIB_OPTIONS' is not defined in
   the target makefile fragment or if none of the options listed in
   `MULTILIB_OPTIONS' are set by default.  *Note Target Fragment::.  */
#define MULTILIB_DEFAULTS { "mint16", "mdouble32" }


/* Run-time target specifications */

/* Define this to be a string constant containing `-D' options to
   define the predefined macros that identify this machine and system.
   These macros will be predefined unless the `-ansi' option is
   specified.

   In addition, a parallel set of macros are predefined, whose names
   are made by appending `__' at the beginning and at the end.  These
   `__' macros are permitted by the ANSI standard, so they are
   predefined regardless of whether `-ansi' is specified.  */
#define CPP_PREDEFINES "-D__D10V__ -Amachine(d10v)"

/* Variable to hold flag bits.  */
extern int target_flags;

/* Machine dependent switche bits */
#define MASK_INT32		0x00000001		/* ints are 32 bits */
#define MASK_DOUBLE64		0x00000002		/* double is 64 bits */
#define MASK_ADDAC3		0x00000004		/* enable ADDAC3/SUBAC3 instructions */
#define MASK_ACCUM		0x00000008		/* allow use of the accumulators */
#define MASK_NO_CALLEE_COPIES	0x00000010		/* disable callee copies */
#define MASK_NO_SMALL_INSNS	0x00000020		/* disable replacing 1 long insn with 2 short ones */
#define MASK_NO_COND_MOVE	0x00000040		/* disable conditional moves */
#define MASK_NO_LONGLONG_ALU	0x00000080		/* disable long long plus/minus */

#define MASK_DEBUG_MOVE4	0x04000000		/* debug emit_move4_words */
#define MASK_DEBUG_STACK	0x08000000		/* debug stack offsets */
#define MASK_DEBUG_ARG		0x10000000		/* debug FUNCTION_ARG */
#define MASK_DEBUG_ADDR		0x20000000		/* debug GO_IF_LEGITIMATE_ADDRESS */
#define MASK_DEBUG		0x40000000		/* general debug flag */

#ifndef TARGET_DEFAULT
/* XXX: The normal scheduler seems to have a bug regarding no callee copies, so for
   now, just set the default appropriately.  */
#ifndef HAIFA
#define TARGET_DEFAULT	MASK_NO_CALLEE_COPIES
#else
#define TARGET_DEFAULT 0
#endif
#endif

/* Machine dependent switches */
#define TARGET_INT32			(target_flags & MASK_INT32)
#define TARGET_DOUBLE64			(target_flags & MASK_DOUBLE64)
#define TARGET_ADDAC3			(target_flags & MASK_ADDAC3)
#define TARGET_ACCUM			(target_flags & MASK_ACCUM)
#define TARGET_NO_CALLEE_COPIES		(target_flags & MASK_NO_CALLEE_COPIES)
#define TARGET_NO_SMALL_INSNS		(target_flags & MASK_NO_SMALL_INSNS)
#define TARGET_NO_COND_MOVE		(target_flags & MASK_NO_COND_MOVE)
#define TARGET_NO_LONGLONG_ALU		(target_flags & MASK_NO_LONGLONG_ALU)
#define TARGET_DEBUG_MOVE4		(target_flags & MASK_DEBUG_MOVE4)
#define TARGET_DEBUG_STACK		(target_flags & MASK_DEBUG_STACK)
#define TARGET_DEBUG_ARG		(target_flags & MASK_DEBUG_ARG)
#define TARGET_DEBUG_ADDR		(target_flags & MASK_DEBUG_ADDR)
#define TARGET_DEBUG			(target_flags & MASK_DEBUG)

#define TARGET_INT16			(! TARGET_INT32)
#define TARGET_DOUBLE32			(! TARGET_DOUBLE64)
#define TARGET_CALLEE_COPIES		(! TARGET_NO_CALLEE_COPIES)
#define TARGET_SMALL_INSNS		(! TARGET_NO_SMALL_INSNS)
#define TARGET_COND_MOVE		(! TARGET_NO_COND_MOVE)
#define TARGET_LONGLONG_ALU		(! TARGET_NO_LONGLONG_ALU)

/* This macro defines names of command options to set and clear bits
   in `target_flags'.  Its definition is an initializer with a
   subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   option name, and a number, which contains the bits to set in
   `target_flags'.  A negative number says to clear bits instead; the
   negative of the number is which bits to clear.  The actual option
   name is made by appending `-m' to the specified name.

   One of the subgroupings should have a null string.  The number in
   this grouping is the default value for `target_flags'.  Any target
   options act starting with that value.  */
#define TARGET_SWITCHES							\
{									\
  { "int16",		-MASK_INT32 },					\
  { "int32",		 MASK_INT32 },					\
  { "double64",		 MASK_DOUBLE64 },				\
  { "double32",		-MASK_DOUBLE64 },				\
  { "addac3",		 MASK_ADDAC3 | MASK_ACCUM },			\
  { "no-addac3",	-MASK_ADDAC3 },					\
  { "accum",		 MASK_ACCUM },					\
  { "no-accum",		-MASK_ACCUM },					\
  { "callee-copies",	-MASK_NO_CALLEE_COPIES },			\
  { "no-callee-copies",	 MASK_NO_CALLEE_COPIES },			\
  { "small-insns",	-MASK_NO_SMALL_INSNS },				\
  { "no-small-insns",	 MASK_NO_SMALL_INSNS },				\
  { "cond-move",	-MASK_NO_COND_MOVE },				\
  { "no-cond-move",	 MASK_NO_COND_MOVE },				\
  { "longlong-alu",	-MASK_NO_LONGLONG_ALU },			\
  { "no-longlong-alu",	 MASK_NO_LONGLONG_ALU },			\
  { "debug",		 MASK_DEBUG },					\
  { "debug-arg",	 MASK_DEBUG_ARG },				\
  { "debug-addr",	 MASK_DEBUG_ADDR },				\
  { "debug-stack",	 MASK_DEBUG_STACK },				\
  { "debug-move4",	 MASK_DEBUG_MOVE4 },				\
  { "asm-optimize",	 0 },						\
  { "no-asm-optimize",	 0 },						\
  { "",			 TARGET_DEFAULT }				\
}

/* This macro is similar to `TARGET_SWITCHES' but defines names of
   command options that have values.  Its definition is an
   initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   fixed part of the option name, and the address of a variable.  The
   variable, type `char *', is set to the variable part of the given
   option if the fixed part matches.  The actual option name is made
   by appending `-m' to the specified name.  */

#define TARGET_OPTIONS							\
{									\
   {"branch-cost=",  &d10v_branch_cost_string},				\
   {"cond-exec=",    &d10v_cond_exec_string},				\
}

/* This macro is a C statement to print on `stderr' a string
   describing the particular machine description choice.  Every
   machine description should define `TARGET_VERSION'.  */
#define TARGET_VERSION fprintf (stderr, " (D10V)")

/* Sometimes certain combinations of command options do not make
   sense on a particular target machine.  You can define a macro
   `OVERRIDE_OPTIONS' to take account of this.  This macro, if
   defined, is executed once just after all the command options have
   been parsed.

   Don't use this macro to turn on various extra optimizations for
   `-O'.  That is what `OPTIMIZATION_OPTIONS' is for.  */
#define OVERRIDE_OPTIONS override_options ()

/* Some machines may desire to change what optimizations are
   performed for various optimization levels.   This macro, if
   defined, is executed once just after the optimization level is
   determined and before the remainder of the command options have
   been parsed.  Values set in this macro are used as the default
   values for the other command line options.

   LEVEL is the optimization level specified; 2 if `-O2' is
   specified, 1 if `-O' is specified, and 0 if neither is specified.

   SIZE is non-zero if `-Os' is specified, 0 otherwise.  

   On the d10v, turn off scheduling before register allocation, since
   it makes for more spills and increases the register pressure.  There
   is also a bug between passing 64 bit values by reference that the
   scheduler tweaks.  */

#define OPTIMIZATION_OPTIONS(LEVEL,SIZE)				\
do {									\
  if ((LEVEL) > 1)							\
    flag_schedule_insns = 0;						\
} while (0)

/* Define this macro if debugging can be performed even without a
   frame pointer.  If this macro is defined, GNU CC will turn on the
   `-fomit-frame-pointer' option whenever `-O' is specified.  */
#define CAN_DEBUG_WITHOUT_FP 1


/* Storage layout */

/* Define this macro to have the value 1 if the most significant bit
   in a byte has the lowest number; otherwise define it to have the
   value zero.  This means that bit-field instructions count from the
   most significant bit.  If the machine has no bit-field
   instructions, then this must still be defined, but it doesn't
   matter which value it is defined to.  This macro need not be a
   constant.

   This macro does not affect the way structure fields are packed into
   bytes or words; that is controlled by `BYTES_BIG_ENDIAN'.  */
#define BITS_BIG_ENDIAN 1

/* Define this macro to have the value 1 if the most significant byte
   in a word has the lowest number.  This macro need not be a
   constant.  */
#define BYTES_BIG_ENDIAN 1

/* Define this macro to have the value 1 if, in a multiword object,
   the most significant word has the lowest number.  This applies to
   both memory locations and registers; GNU CC fundamentally assumes
   that the order of words in memory is the same as the order in
   registers.  This macro need not be a constant.  */
#define WORDS_BIG_ENDIAN 1

/* Define this macro if WORDS_BIG_ENDIAN is not constant.  This must
   be a constant value with the same meaning as WORDS_BIG_ENDIAN,
   which will be used only when compiling libgcc2.c.  Typically the
   value will be set based on preprocessor defines.  */
/* #define LIBGCC2_WORDS_BIG_ENDIAN 1 */

/* Define this macro to have the value 1 if `DFmode', `XFmode' or
   `TFmode' floating point numbers are stored in memory with the word
   containing the sign bit at the lowest address; otherwise define it
   to have the value 0.  This macro need not be a constant.

   You need not define this macro if the ordering is the same as for
   multi-word integers. */
/* #define FLOAT_WORDS_BIG_ENDIAN 1 */

/* Define this macro to be the number of bits in an addressable
   storage unit (byte); normally 8.  */
#define BITS_PER_UNIT 8

/* Number of bits in a word; normally 32. */
#define BITS_PER_WORD 16

/* Maximum number of bits in a word.  If this is undefined, the
   default is `BITS_PER_WORD'.  Otherwise, it is the constant value
   that is the largest value that `BITS_PER_WORD' can have at
   run-time.  */
#define MAX_BITS_PER_WORD 16

/* Number of storage units in a word; normally 4.  */
#define UNITS_PER_WORD 2

/* Minimum number of units in a word.  If this is undefined, the
   default is `UNITS_PER_WORD'.  Otherwise, it is the constant value
   that is the smallest value that `UNITS_PER_WORD' can have at
   run-time.  */
#define MIN_UNITS_PER_WORD 2

/* Normal alignment required for function parameters on the stack, in
   bits.  All stack parameters receive at least this much alignment
   regardless of data type.  On most machines, this is the same as the
   size of an integer. */
#define PARM_BOUNDARY 16

/* Define this macro if you wish to preserve a certain alignment for
   the stack pointer.  The definition is a C expression for the
   desired alignment (measured in bits).

   If `PUSH_ROUNDING' is not defined, the stack will always be aligned
   to the specified boundary.  If `PUSH_ROUNDING' is defined and
   specifies a less strict alignment than `STACK_BOUNDARY', the stack
   may be momentarily unaligned while pushing arguments. */
#define STACK_BOUNDARY 16

/* Alignment required for a function entry point, in bits. */
#define FUNCTION_BOUNDARY 32

/* Biggest alignment that any data type can require on this machine,
   in bits. */
#define BIGGEST_ALIGNMENT 16

/* Biggest alignment that any structure field can require on this
   machine, in bits.  If defined, this overrides `BIGGEST_ALIGNMENT'
   for structure fields only. */
/* #define BIGGEST_FIELD_ALIGNMENT */

/* An expression for the alignment of a structure field FIELD if the
   alignment computed in the usual way is COMPUTED.  GNU CC uses this
   value instead of the value in `BIGGEST_ALIGNMENT' or
   `BIGGEST_FIELD_ALIGNMENT', if defined, for structure fields only. */
/* #define ADJUST_FIELD_ALIGN(FIELD, COMPUTED) */

/* Biggest alignment supported by the object file format of this
   machine.  Use this macro to limit the alignment which can be
   specified using the `__attribute__ ((aligned (N)))' construct.  If
   not defined, the default value is `BIGGEST_ALIGNMENT'. */
/* #define MAX_OFILE_ALIGNMENT */

/* If defined, a C expression to compute the alignment for a static
   variable.  TYPE is the data type, and BASIC-ALIGN is the alignment
   that the object would ordinarily have.  The value of this macro is
   used instead of that alignment to align the object.

   If this macro is not defined, then BASIC-ALIGN is used.

   One use of this macro is to increase alignment of medium-size data
   to make it all fit in fewer cache lines.  Another is to cause
   character arrays to be word-aligned so that `strcpy' calls that
   copy constants to character arrays can be done inline. */
/* #define DATA_ALIGNMENT(TYPE, BASIC-ALIGN) */

/* If defined, a C expression to compute the alignment given to a
   constant that is being placed in memory.  CONSTANT is the constant
   and BASIC-ALIGN is the alignment that the object would ordinarily
   have.  The value of this macro is used instead of that alignment to
   align the object.

   If this macro is not defined, then BASIC-ALIGN is used.

   The typical use of this macro is to increase alignment for string
   constants to be word aligned so that `strcpy' calls that copy
   constants can be done inline. */
/* #define CONSTANT_ALIGNMENT(CONSTANT, BASIC-ALIGN) */

/* Alignment in bits to be given to a structure bit field that
   follows an empty field such as `int : 0;'.

   Note that `PCC_BITFIELD_TYPE_MATTERS' also affects the alignment
   that results from an empty field. */
/* #define EMPTY_FIELD_BOUNDARY */

/* Number of bits which any structure or union's size must be a
   multiple of.  Each structure or union's size is rounded up to a
   multiple of this.

   If you do not define this macro, the default is the same as
   `BITS_PER_UNIT'.  */
/* #define STRUCTURE_SIZE_BOUNDARY */

/* Define this macro to be the value 1 if instructions will fail to
   work if given data not on the nominal alignment.  If instructions
   will merely go slower in that case, define this macro as 0. */
#define STRICT_ALIGNMENT 1

/* Define this if you wish to imitate the way many other C compilers
   handle alignment of bitfields and the structures that contain them.

   The behavior is that the type written for a bitfield (`int',
   `short', or other integer type) imposes an alignment for the
   entire structure, as if the structure really did contain an
   ordinary field of that type.  In addition, the bitfield is placed
   within the structure so that it would fit within such a field, not
   crossing a boundary for it.

   Thus, on most machines, a bitfield whose type is written as `int'
   would not cross a four-byte boundary, and would force four-byte
   alignment for the whole structure.  (The alignment used may not be
   four bytes; it is controlled by the other alignment parameters.)

   If the macro is defined, its definition should be a C expression;
   a nonzero value for the expression enables this behavior.

   Note that if this macro is not defined, or its value is zero, some
   bitfields may cross more than one alignment boundary.  The
   compiler can support such references if there are `insv', `extv',
   and `extzv' insns that can directly reference memory.

   The other known way of making bitfields work is to define
   `STRUCTURE_SIZE_BOUNDARY' as large as `BIGGEST_ALIGNMENT'.  Then
   every structure can be accessed with fullwords.

   Unless the machine has bitfield instructions or you define
   `STRUCTURE_SIZE_BOUNDARY' that way, you must define
   `PCC_BITFIELD_TYPE_MATTERS' to have a nonzero value.  */
/* #define PCC_BITFIELD_TYPE_MATTERS */

/* Like PCC_BITFIELD_TYPE_MATTERS except that its effect is limited to
   aligning a bitfield within the structure. */
/* #define BITFIELD_NBYTES_LIMITED */

/* Define this macro as an expression for the overall size of a
   structure (given by STRUCT as a tree node) when the size computed
   from the fields is SIZE and the alignment is ALIGN.

   The default is to round SIZE up to a multiple of ALIGN. */
/* #define ROUND_TYPE_SIZE(STRUCT, SIZE, ALIGN) */

/* Define this macro as an expression for the alignment of a structure
   (given by STRUCT as a tree node) if the alignment computed in the
   usual way is COMPUTED and the alignment explicitly specified was
   SPECIFIED.

   The default is to use SPECIFIED if it is larger; otherwise, use
   the smaller of COMPUTED and `BIGGEST_ALIGNMENT' */
/* #define ROUND_TYPE_ALIGN(STRUCT, COMPUTED, SPECIFIED) */

/* An integer expression for the size in bits of the largest integer
   machine mode that should actually be used.  All integer machine
   modes of this size or smaller can be used for structures and
   unions with the appropriate sizes.  If this macro is undefined,
   `GET_MODE_BITSIZE (DImode)' is assumed. */
/* #define MAX_FIXED_MODE_SIZE */

/* A C statement to validate the value VALUE (of type `double') for
   mode MODE.  This means that you check whether VALUE fits within
   the possible range of values for mode MODE on this target machine.
   The mode MODE is always a mode of class `MODE_FLOAT'.  OVERFLOW
   is nonzero if the value is already known to be out of range.

   If VALUE is not valid or if OVERFLOW is nonzero, you should set
   OVERFLOW to 1 and then assign some valid value to VALUE.  Allowing
   an invalid value to go through the compiler can produce incorrect
   assembler code which may even cause Unix assemblers to crash.

   This macro need not be defined if there is no work for it to do. */
/* #define CHECK_FLOAT_VALUE(MODE, VALUE, OVERFLOW) */

/* A code distinguishing the floating point format of the target
   machine.  There are three defined values:

  `IEEE_FLOAT_FORMAT'
        This code indicates IEEE floating point.  It is the default;
        there is no need to define this macro when the format is IEEE.

  `VAX_FLOAT_FORMAT'
        This code indicates the peculiar format used on the Vax.

  `UNKNOWN_FLOAT_FORMAT'
        This code indicates any other format.

   The value of this macro is compared with `HOST_FLOAT_FORMAT'
   (*note Config::.) to determine whether the target machine has the
   same format as the host machine.  If any other formats are
   actually in use on supported machines, new codes should be defined
   for them.

   The ordering of the component words of floating point values
   stored in memory is controlled by `FLOAT_WORDS_BIG_ENDIAN' for the
   target machine and `HOST_FLOAT_WORDS_BIG_ENDIAN' for the host. */
#define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT


/* Layout of Source Language Data Types */

/* Width of a pointer, in bits.  You must specify a value no wider
   than the width of `Pmode'.  If it is not equal to the width of
   `Pmode', you must define `POINTERS_EXTEND_UNSIGNED'.  */
#define POINTER_SIZE 16

/* A C expression whose value is nonzero if pointers that need to be
   extended from being `POINTER_SIZE' bits wide to `Pmode' are
   sign-extended and zero if they are zero-extended.

   You need not define this macro if the `POINTER_SIZE' is equal to
   the width of `Pmode'.  */
#define POINTERS_EXTEND_UNSIGNED 1

/* A C expression for the size in bits of the type `int' on the
   target machine.  If you don't define this, the default is one word.  */
#define INT_TYPE_SIZE (TARGET_INT32 ? 32 : 16)

/* Maximum number for the size in bits of the type `int' on the target
   machine.  If this is undefined, the default is `INT_TYPE_SIZE'.
   Otherwise, it is the constant value that is the largest value that
   `INT_TYPE_SIZE' can have at run-time.  This is used in `cpp'.  */
#define MAX_INT_TYPE_SIZE 32

/* A C expression for the size in bits of the type `short' on the
   target machine.  If you don't define this, the default is half a
   word.  (If this would be less than one storage unit, it is rounded
   up to one unit.)  */
#define SHORT_TYPE_SIZE 16

/* A C expression for the size in bits of the type `long' on the
   target machine.  If you don't define this, the default is one word.  */
#define LONG_TYPE_SIZE 32

/* Maximum number for the size in bits of the type `long' on the
   target machine.  If this is undefined, the default is
   `LONG_TYPE_SIZE'.  Otherwise, it is the constant value that is the
   largest value that `LONG_TYPE_SIZE' can have at run-time.  This is
   used in `cpp'.  */
#define MAX_LONG_TYPE_SIZE 32

/* A C expression for the size in bits of the type `long long' on the
   target machine.  If you don't define this, the default is two
   words.  If you want to support GNU Ada on your machine, the value
   of macro must be at least 64.  */
#define LONG_LONG_TYPE_SIZE 64

/* A C expression for the size in bits of the type `char' on the
   target machine.  If you don't define this, the default is one
   quarter of a word.  (If this would be less than one storage unit,
   it is rounded up to one unit.)  */
#define CHAR_TYPE_SIZE 8

/* Maximum number for the size in bits of the type `char' on the
   target machine.  If this is undefined, the default is
   `CHAR_TYPE_SIZE'.  Otherwise, it is the constant value that is the
   largest value that `CHAR_TYPE_SIZE' can have at run-time.  This is
   used in `cpp'.  */
#define MAX_CHAR_TYPE_SIZE 8

/* A C expression for the size in bits of the type `float' on the
   target machine.  If you don't define this, the default is one word. */
#define FLOAT_TYPE_SIZE 32

/* A C expression for the size in bits of the type `double' on the
   target machine.  If you don't define this, the default is two
   words.  */
#define DOUBLE_TYPE_SIZE ((TARGET_DOUBLE64) ? 64 : 32)

/* A C expression for the size in bits of the type `long double' on
   the target machine.  If you don't define this, the default is two
   words.  */
#define LONG_DOUBLE_TYPE_SIZE 64

/* An expression whose value is 1 or 0, according to whether the type
   `char' should be signed or unsigned by default.  The user can
   always override this default with the options `-fsigned-char' and
   `-funsigned-char'.  */
#define DEFAULT_SIGNED_CHAR 1

/* A C expression to determine whether to give an `enum' type only as
   many bytes as it takes to represent the range of possible values
   of that type.  A nonzero value means to do that; a zero value
   means all `enum' types should be allocated like `int'.

   If you don't define the macro, the default is 0.  */
#define DEFAULT_SHORT_ENUMS 0

/* A C expression for a string describing the name of the data type
   to use for size values.  The typedef name `size_t' is defined
   using the contents of the string.

   The string can contain more than one keyword.  If so, separate
   them with spaces, and write first any length keyword, then
   `unsigned' if appropriate, and finally `int'.  The string must
   exactly match one of the data type names defined in the function
   `init_decl_processing' in the file `c-decl.c'.  You may not omit
   `int' or change the order--that would cause the compiler to crash
   on startup.

   If you don't define this macro, the default is `"long unsigned
   int"'.

   On the D10V, ints can be either 16 or 32 bits, so we need to adjust
   size_t appropriately.  */
#undef	SIZE_TYPE
#define SIZE_TYPE "short unsigned int"

/* A C expression for a string describing the name of the data type
   to use for the result of subtracting two pointers.  The typedef
   name `ptrdiff_t' is defined using the contents of the string.  See
   `SIZE_TYPE' above for more information.

   If you don't define this macro, the default is `"long int"'.  */
#undef	PTRDIFF_TYPE
#define PTRDIFF_TYPE "short int"

/* A C expression for a string describing the name of the data type
   to use for wide characters.  The typedef name `wchar_t' is defined
   using the contents of the string.  See `SIZE_TYPE' above for more
   information.

   If you don't define this macro, the default is `"int"'.  */
#undef	WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"

/* A C expression for the size in bits of the data type for wide
   characters.  This is used in `cpp', which cannot make use of
   `WCHAR_TYPE'.  */
#undef	WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16

/* Maximum number for the size in bits of the data type for wide
   characters.  If this is undefined, the default is
   `WCHAR_TYPE_SIZE'.  Otherwise, it is the constant value that is the
   largest value that `WCHAR_TYPE_SIZE' can have at run-time.  This is
   used in `cpp'.  */
#define MAX_WCHAR_TYPE_SIZE 16

/* Define results of standard character escape sequences.  */
#define TARGET_BELL 007
#define TARGET_BS 010
#define TARGET_TAB 011
#define TARGET_NEWLINE 012
#define TARGET_VT 013
#define TARGET_FF 014
#define TARGET_CR 015


/* Basic Characteristics of Registers */

/* Return true if a value is inside a range */
#define IN_RANGE_P(VALUE, LOW, HIGH) \
  (((unsigned)((VALUE) - (LOW))) <= ((unsigned)((HIGH) - (LOW))))

/* First/last general purpose registers */
#define GPR_FIRST	0
#define GPR_LAST	15
#define GPR_ZERO_REGNUM	(GPR_FIRST + 14)	/* register that holds 0 */
#define GPR_P(REGNO)	IN_RANGE_P (REGNO, GPR_FIRST, GPR_LAST)
#define GPR_OR_PSEUDO_P(REGNO) (GPR_P (REGNO) || REGNO >= FIRST_PSEUDO_REGISTER)

/* Registers arguments are passed in */
#define ARG_FIRST	(GPR_FIRST + 0)
#define ARG_LAST	(GPR_FIRST + 3)
#define RETURN_REGNUM	ARG_FIRST

/* Register pair to save accumulators in */
#define SAVE_ACC_REGNUM	(GPR_FIRST + 12)

/* Register to save accumulator guard digits in */
#define SAVE_GUARD_REGNUM (GPR_FIRST + 5)

/* Even/odd registers for multiword items */
#define GPR_EVEN_P(REGNO) (GPR_P(REGNO) && ((((REGNO) - GPR_FIRST) & 1) == 0))
#define GPR_ODD_P(REGNO)  (GPR_P(REGNO) && ((((REGNO) - GPR_FIRST) & 1) != 0))

/* First/last control registers */
#define CR_FIRST	16
#define CR_LAST		31
#define CR_P(REGNO)	IN_RANGE_P (REGNO, CR_FIRST, CR_LAST)

#define REPEAT_REGNUM	(CR_FIRST + 7)

/* First/last accumulator registers */
#define ACCUM_FIRST	32
#define ACCUM_LAST	33
#define ACCUM_P(REGNO)	IN_RANGE_P (REGNO, ACCUM_FIRST, ACCUM_LAST)

/* Argument pointer pseudo register */
#define AP_FIRST	34

/* Condition/carry code 'registers'.  We ignore the fact that these
   are actually stored in CR0.  */
#define CC_FIRST	35
#define CC_LAST		37
#define CC_P(REGNO)	IN_RANGE_P (REGNO, CC_FIRST, CC_LAST)

#define F0_REGNUM	(CC_FIRST + 0)
#define F1_REGNUM	(CC_FIRST + 1)
#define CARRY_REGNUM	(CC_FIRST + 2)

/* Number of hardware registers known to the compiler.  They receive
   numbers 0 through `FIRST_PSEUDO_REGISTER-1'; thus, the first
   pseudo register's number really is assigned the number
   `FIRST_PSEUDO_REGISTER'. */
#define FIRST_PSEUDO_REGISTER 38

/* An initializer that says which registers are used for fixed
   purposes all throughout the compiled code and are therefore not
   available for general allocation.  These would include the stack
   pointer, the frame pointer (except on machines where that can be
   used as a general register when no frame pointer is needed), the
   program counter on machines where that is considered one of the
   addressable registers, and any other numbered register with a
   standard use.

   This information is expressed as a sequence of numbers, separated
   by commas and surrounded by braces.  The Nth number is 1 if
   register N is fixed, 0 otherwise.

   The table initialized from this macro, and the table initialized by
   the following one, may be overridden at run time either
   automatically, by the actions of the macro
   `CONDITIONAL_REGISTER_USAGE', or by the user with the command
   options `-ffixed-REG', `-fcall-used-REG' and `-fcall-saved-REG'. */
#define FIXED_REGISTERS							\
{									\
  0, /* r0   */	0, /* r1   */	0, /* r2   */	0, /* r3   */		\
  0, /* r4   */	0, /* r5   */	0, /* r6   */	0, /* r7   */		\
  0, /* r8   */	0, /* r9   */	0, /* r10  */	0, /* r11  */		\
  0, /* r12  */	0, /* r13  */	1, /* r14  */	1, /* r15  */		\
  1, /* cr0  */	1, /* cr1  */	1, /* cr2  */	1, /* cr3  */		\
  1, /* cr4  */	1, /* cr5  */	1, /* cr6  */	1, /* cr7  */		\
  1, /* cr8  */	1, /* cr9  */	1, /* cr10 */	1, /* cr11 */		\
  1, /* cr12 */	1, /* cr13 */	1, /* cr14 */	1, /* cr15 */		\
  1, /* a0   */	1, /* a1   */						\
  1, /* ap   */								\
  1, /* f0   */	1, /* f1   */	1, /* c    */				\
}

/* Like `FIXED_REGISTERS' but has 1 for each register that is
   clobbered (in general) by function calls as well as for fixed
   registers.  This macro therefore identifies the registers that are
   not available for general allocation of values that must live
   across function calls.

   If a register has 0 in `CALL_USED_REGISTERS', the compiler
   automatically saves it on function entry and restores it on
   function exit, if the register is used within the function. */
#define CALL_USED_REGISTERS						\
{									\
  1, /* r0   */	1, /* r1   */	1, /* r2   */	1, /* r3   */		\
  1, /* r4   */ 1, /* r5   */	0, /* r6   */	0, /* r7   */		\
  0, /* r8   */	0, /* r9   */	0, /* r10  */	0, /* r11  */		\
  1, /* r12  */	1, /* r13  */	1, /* r14  */	1, /* r15  */		\
  1, /* cr0  */	1, /* cr1  */	1, /* cr2  */	1, /* cr3  */		\
  1, /* cr4  */	1, /* cr5  */	1, /* cr6  */	1, /* cr7  */		\
  1, /* cr8  */	1, /* cr9  */	1, /* cr10 */	1, /* cr11 */		\
  1, /* cr12 */	1, /* cr13 */	1, /* cr14 */	1, /* cr15 */		\
  1, /* a0   */	1, /* a1   */						\
  1, /* ap   */								\
  1, /* f0   */	1, /* f1   */	1, /* c    */				\
}

/* Zero or more C statements that may conditionally modify two
   variables `fixed_regs' and `call_used_regs' (both of type `char
   []') after they have been initialized from the two preceding
   macros.

   This is necessary in case the fixed or call-clobbered registers
   depend on target flags. */
#define CONDITIONAL_REGISTER_USAGE					\
do									\
  {									\
    if (TARGET_ACCUM)							\
      {									\
	int i;								\
	for (i = ACCUM_FIRST; i <= ACCUM_LAST; i++)			\
	  fixed_regs[i] = call_used_regs[i] = 0;			\
      }									\
  }									\
while (0)

/* If this macro is defined and has a nonzero value, it means that
   `setjmp' and related functions fail to save the registers, or that
   `longjmp' fails to restore them.  To compensate, the compiler
   avoids putting variables in registers in functions that use
   `setjmp'. */
/* #define NON_SAVING_SETJMP */


/* Order of allocating registers */

/* If defined, an initializer for a vector of integers, containing the
   numbers of hard registers in the order in which GNU CC should
   prefer to use them (from most preferred to least).

   If this macro is not defined, registers are used lowest numbered
   first (all else being equal).

   One use of this macro is on machines where the highest numbered
   registers must always be saved and the save-multiple-registers
   instruction supports only sequences of consecutive registers.  On
   such machines, define `REG_ALLOC_ORDER' to be an initializer that
   lists the highest numbered allocatable register first.

   On the d10v, we save some time by putting the fixed registers at
   the end of the list.  Also, we put the odd registers whose even
   register is fixed before the even/odd register pairs.  */
#define REG_ALLOC_ORDER							\
{									\
  /* Volatile GPR registers */						\
  GPR_FIRST+0,								\
  GPR_FIRST+1,								\
  GPR_FIRST+2,								\
  GPR_FIRST+3,								\
  GPR_FIRST+4,								\
  GPR_FIRST+5,								\
  GPR_FIRST+12,								\
  GPR_FIRST+13,								\
									\
  /* Callee preserved GPR registers */					\
  GPR_FIRST+6,								\
  GPR_FIRST+7,								\
  GPR_FIRST+8,								\
  GPR_FIRST+9,								\
  GPR_FIRST+10,								\
  GPR_FIRST+11,								\
									\
  /* Accumulators */							\
  ACCUM_FIRST+0,							\
  ACCUM_FIRST+1,							\
									\
  /* Repeat count register */						\
  REPEAT_REGNUM,							\
									\
  /* Condition code, carry registers */					\
  F0_REGNUM,								\
  F1_REGNUM,								\
  CARRY_REGNUM,								\
									\
  /* Fixed registers */							\
  GPR_FIRST+14,			/* zero register */			\
  GPR_FIRST+15,			/* stack pointer */			\
  CR_FIRST+0,								\
  CR_FIRST+1,								\
  CR_FIRST+2,								\
  CR_FIRST+3,								\
  CR_FIRST+4,								\
  CR_FIRST+5,								\
  CR_FIRST+6,								\
  CR_FIRST+8,								\
  CR_FIRST+9,								\
  CR_FIRST+10,								\
  CR_FIRST+11,								\
  CR_FIRST+12,								\
  CR_FIRST+13,								\
  CR_FIRST+14,								\
  CR_FIRST+15,								\
  ARG_POINTER_REGNUM,							\
}

/* A C statement (sans semicolon) to choose the order in which to
   allocate hard registers for pseudo-registers local to a basic
   block.

   Store the desired register order in the array `reg_alloc_order'.
   Element 0 should be the register to allocate first; element 1, the
   next register; and so on.

   The macro body should not assume anything about the contents of
   `reg_alloc_order' before execution of the macro.

   On most machines, it is not necessary to define this macro. */
/* #define ORDER_REGS_FOR_LOCAL_ALLOC */


/* How values fit in registers */

/* A C expression for the number of consecutive hard registers,
   starting at register number REGNO, required to hold a value of mode
   MODE. */

#define HARD_REGNO_NREGS(REGNO, MODE)					\
  ((ACCUM_P (REGNO))							\
   ? ((GET_MODE_SIZE (MODE) + 4 - 1) / 4)				\
   : ((GET_MODE_SIZE (MODE) + 2 - 1) / 2))

/* A C expression that is nonzero if it is permissible to store a
   value of mode MODE in hard register number REGNO (or in several
   registers starting with that one).  */

extern unsigned char hard_regno_mode_ok[][FIRST_PSEUDO_REGISTER];
#define HARD_REGNO_MODE_OK(REGNO, MODE) hard_regno_mode_ok[ (int)MODE ][ REGNO ]

/* A C expression that is nonzero if it is desirable to choose
   register allocation so as to avoid move instructions between a
   value of mode MODE1 and a value of mode MODE2.

   If `HARD_REGNO_MODE_OK (R, MODE1)' and `HARD_REGNO_MODE_OK (R,
   MODE2)' are ever different for any R, then `MODES_TIEABLE_P (MODE1,
   MODE2)' must be zero. */

extern unsigned char modes_tieable_p[];
#define MODES_TIEABLE_P(MODE1, MODE2) \
  modes_tieable_p[ (((int)(MODE1)) * (NUM_MACHINE_MODES)) + (int)(MODE2) ]


/* Register classes */

/* An enumeral type that must be defined with all the register class
   names as enumeral values.  `NO_REGS' must be first.  `ALL_REGS'
   must be the last register class, followed by one more enumeral
   value, `LIM_REG_CLASSES', which is not a register class but rather
   tells how many classes there are.

   Each register class has a number, which is the value of casting
   the class name to type `int'.  The number serves as an index in
   many of the tables described below.

   All things being equal, the register allocator prefers the higher
   register class, so put all of the general registers higher than
   the accumulators and CRs.  */

enum reg_class
{
  NO_REGS,
  REPEAT_REGS,
  CR_REGS,
  ACCUM_REGS,
  F0_REGS,
  F1_REGS,
  F_REGS,
  CARRY_REGS,
  CC_REGS,
  ARG0_REGS,
  ARG1_REGS,
  ARG2_REGS,
  ARG3_REGS,
  RETURN_REGS,
  EVEN_REGS,
  GENERAL_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};


/* The number of distinct register classes */
#define N_REG_CLASSES ((int) LIM_REG_CLASSES)

/* An initializer containing the names of the register classes as C
   string constants.  These names are used in writing some of the
   debugging dumps. */
#define REG_CLASS_NAMES							\
{									\
  "NO_REGS",								\
  "REPEAT_REGS",							\
  "CR_REGS",								\
  "ACCUM_REGS",								\
  "F0_REGS",								\
  "F1_REGS",								\
  "F_REGS",								\
  "CARRY_REGS",								\
  "CC_REGS",								\
  "ARG0_REGS",								\
  "ARG1_REGS",								\
  "ARG2_REGS",								\
  "ARG3_REGS",								\
  "RETURN_REGS",							\
  "EVEN_REGS",								\
  "GENERAL_REGS",							\
  "ALL_REGS"								\
}

/* An initializer containing the contents of the register classes, as
   integers which are bit masks.  The Nth integer specifies the
   contents of class N.  The way the integer MASK is interpreted is
   that register R is in the class if `MASK & (1 << R)' is 1.

   When the machine has more than 32 registers, an integer does not
   suffice.  Then the integers are replaced by sub-initializers,
   braced groupings containing several integers.  Each
   sub-initializer must be suitable as an initializer for the type
   `HARD_REG_SET' which is defined in `hard-reg-set.h'. */
#define REG_CLASS_CONTENTS						\
{									\
  { 0x00000000, 0x00000000 },	/* NO_REGS */				\
  { 0x00800000, 0x00000000 },	/* REPEAT_REGS */			\
  { 0xffff0000, 0x00000000 },	/* CR_REGS */				\
  { 0x00000000, 0x00000003 },	/* ACCUM_REGS */			\
  { 0x00000000, 0x00000008 },	/* F0_REGS */				\
  { 0x00000000, 0x00000010 },	/* F1_REGS */				\
  { 0x00000000, 0x00000018 },	/* F_REGS */				\
  { 0x00000000, 0x00000020 },	/* CARRY_REGS */			\
  { 0x00000000, 0x00000038 },	/* CC_REGS */				\
  { 0x00000001, 0x00000000 },	/* ARG0_REGS */				\
  { 0x00000002, 0x00000000 },	/* ARG1_REGS */				\
  { 0x00000004, 0x00000000 },	/* ARG2_REGS */				\
  { 0x00000008, 0x00000000 },	/* ARG3_REGS */				\
  { 0x00002000, 0x00000000 },	/* R13_REGS */				\
  { 0x0000ffff, 0x00000000 },	/* EVEN_REGS */				\
  { 0x0000ffff, 0x00000004 },	/* GENERAL_REGS */			\
  { 0xffffffff, 0x0000001f },	/* ALL_REGS */				\
}

/* A C expression whose value is a register class containing hard
   register REGNO.  In general there is more than one such class;
   choose a class which is "minimal", meaning that no smaller class
   also contains the register. */

extern enum reg_class regno_reg_class[];
#define REGNO_REG_CLASS(REGNO) regno_reg_class[ (REGNO) ]

/* A macro whose definition is the name of the class to which a valid
   base register must belong.  A base register is one used in an
   address which is the register value plus a displacement. */
#define BASE_REG_CLASS GENERAL_REGS

/* A macro whose definition is the name of the class to which a valid
   index register must belong.  An index register is one used in an
   address where its value is either multiplied by a scale factor or
   added to another register (as well as added to a displacement). */
#define INDEX_REG_CLASS GENERAL_REGS

/* A C expression which defines the machine-dependent operand
   constraint letters for register classes.  If CHAR is such a
   letter, the value should be the register class corresponding to
   it.  Otherwise, the value should be `NO_REGS'.  The register
   letter `r', corresponding to class `GENERAL_REGS', will not be
   passed to this macro; you do not need to handle it. */

extern enum reg_class reg_class_from_letter[];
#define REG_CLASS_FROM_LETTER(CHAR) reg_class_from_letter[ CHAR ]

/* A C expression which is nonzero if register number NUM is suitable
   for use as a base register in operand addresses.  It may be either
   a suitable hard register or a pseudo register that has been
   allocated such a hard register. */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */

#define REGNO_OK_FOR_BASE_P(REGNO)					\
((REGNO) < FIRST_PSEUDO_REGISTER					\
 ? GPR_P (REGNO) || (REGNO) == ARG_POINTER_REGNUM			\
 : (reg_renumber[REGNO] >= 0 && GPR_P (reg_renumber[REGNO])))

/* A C expression which is nonzero if register number NUM is suitable
   for use as an index register in operand addresses.  It may be
   either a suitable hard register or a pseudo register that has been
   allocated such a hard register.

   The difference between an index register and a base register is
   that the index register may be scaled.  If an address involves the
   sum of two registers, neither one of them scaled, then either one
   may be labeled the "base" and the other the "index"; but whichever
   labeling is used must fit the machine's constraints of which
   registers may serve in each capacity.  The compiler will try both
   labelings, looking for one that is valid, and will reload one or
   both registers only if neither labeling works.  */
#define REGNO_OK_FOR_INDEX_P(REGNO) REGNO_OK_FOR_BASE_P (REGNO)

/* A C expression that places additional restrictions on the register
   class to use when it is necessary to copy value X into a register
   in class CLASS.  The value is a register class; perhaps CLASS, or
   perhaps another, smaller class.  On many machines, the following
   definition is safe:

        #define PREFERRED_RELOAD_CLASS(X,CLASS) CLASS

   Sometimes returning a more restrictive class makes better code.
   For example, on the 68000, when X is an integer constant that is
   in range for a `moveq' instruction, the value of this macro is
   always `DATA_REGS' as long as CLASS includes the data registers.
   Requiring a data register guarantees that a `moveq' will be used.

   If X is a `const_double', by returning `NO_REGS' you can force X
   into a memory constant.  This is useful on certain machines where
   immediate floating values cannot be loaded into certain kinds of
   registers. */
#define PREFERRED_RELOAD_CLASS(X, CLASS) \
  ((CLASS == EVEN_REGS || CLASS == ACCUM_REGS) ? EVEN_REGS : GENERAL_REGS)

/* Like `PREFERRED_RELOAD_CLASS', but for output reloads instead of
   input reloads.  If you don't define this macro, the default is to
   use CLASS, unchanged. */
/* #define PREFERRED_OUTPUT_RELOAD_CLASS(X, CLASS) */

/* A C expression that places additional restrictions on the register
   class to use when it is necessary to be able to hold a value of
   mode MODE in a reload register for which class CLASS would
   ordinarily be used.

   Unlike `PREFERRED_RELOAD_CLASS', this macro should be used when
   there are certain modes that simply can't go in certain reload
   classes.

   The value is a register class; perhaps CLASS, or perhaps another,
   smaller class.

   Don't define this macro unless the target machine has limitations
   which require the macro to do something nontrivial. */
/* #define LIMIT_RELOAD_CLASS (MODE, CLASS) */

/* Many machines have some registers that cannot be copied directly
   to or from memory or even from other types of registers.  An
   example is the `MQ' register, which on most machines, can only be
   copied to or from general registers, but not memory.  Some
   machines allow copying all registers to and from memory, but
   require a scratch register for stores to some memory locations
   (e.g., those with symbolic address on the RT, and those with
   certain symbolic address on the Sparc when compiling PIC).  In
   some cases, both an intermediate and a scratch register are
   required.

   You should define these macros to indicate to the reload phase
   that it may need to allocate at least one register for a reload in
   addition to the register to contain the data.  Specifically, if
   copying X to a register CLASS in MODE requires an intermediate
   register, you should define `SECONDARY_INPUT_RELOAD_CLASS' to
   return the largest register class all of whose registers can be
   used as intermediate registers or scratch registers.

   If copying a register CLASS in MODE to X requires an intermediate
   or scratch register, `SECONDARY_OUTPUT_RELOAD_CLASS' should be
   defined to return the largest register class required.  If the
   requirements for input and output reloads are the same, the macro
   `SECONDARY_RELOAD_CLASS' should be used instead of defining both
   macros identically.

   The values returned by these macros are often `GENERAL_REGS'.
   Return `NO_REGS' if no spare register is needed; i.e., if X can be
   directly copied to or from a register of CLASS in MODE without
   requiring a scratch register.  Do not define this macro if it
   would always return `NO_REGS'.

   If a scratch register is required (either with or without an
   intermediate register), you should define patterns for
   `reload_inM' or `reload_outM', as required (*note Standard
   Names::..  These patterns, which will normally be implemented with
   a `define_expand', should be similar to the `movM' patterns,
   except that operand 2 is the scratch register.

   Define constraints for the reload register and scratch register
   that contain a single register class.  If the original reload
   register (whose class is CLASS) can meet the constraint given in
   the pattern, the value returned by these macros is used for the
   class of the scratch register.  Otherwise, two additional reload
   registers are required.  Their classes are obtained from the
   constraints in the insn pattern.

   X might be a pseudo-register or a `subreg' of a pseudo-register,
   which could either be in a hard register or in memory.  Use
   `true_regnum' to find out; it will return -1 if the pseudo is in
   memory and the hard register number if it is in a register.

   These macros should not be used in the case where a particular
   class of registers can only be copied to memory and not to another
   class of registers.  In that case, secondary reload registers are
   not needed and would not be helpful.  Instead, a stack location
   must be used to perform the copy and the `movM' pattern should use
   memory as a intermediate storage.  This case often occurs between
   floating-point and general registers. */
#define SECONDARY_RELOAD_CLASS(CLASS, MODE, X)				\
 ((CLASS) == ACCUM_REGS ? EVEN_REGS					\
  : ((CLASS) == GENERAL_REGS || (CLASS) == EVEN_REGS) ? NO_REGS		\
  : GENERAL_REGS)

/* #define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X) */
/* #define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X) */

/* Certain machines have the property that some registers cannot be
   copied to some other registers without using memory.  Define this
   macro on those machines to be a C expression that is non-zero if
   objects of mode M in registers of CLASS1 can only be copied to
   registers of class CLASS2 by storing a register of CLASS1 into
   memory and loading that memory location into a register of CLASS2.

   Do not define this macro if its value would always be zero. */
/* #define SECONDARY_MEMORY_NEEDED(CLASS1, CLASS2, M) */

/* Normally when `SECONDARY_MEMORY_NEEDED' is defined, the compiler
   allocates a stack slot for a memory location needed for register
   copies.  If this macro is defined, the compiler instead uses the
   memory location defined by this macro.

   Do not define this macro if you do not define
   `SECONDARY_MEMORY_NEEDED'. */
/* #define SECONDARY_MEMORY_NEEDED_RTX(MODE) */

/* When the compiler needs a secondary memory location to copy
   between two registers of mode MODE, it normally allocates
   sufficient memory to hold a quantity of `BITS_PER_WORD' bits and
   performs the store and load operations in a mode that many bits
   wide and whose class is the same as that of MODE.

   This is right thing to do on most machines because it ensures that
   all bits of the register are copied and prevents accesses to the
   registers in a narrower mode, which some machines prohibit for
   floating-point registers.

   However, this default behavior is not correct on some machines,
   such as the DEC Alpha, that store short integers in floating-point
   registers differently than in integer registers.  On those
   machines, the default widening will not work correctly and you
   must define this macro to suppress that widening in some cases.
   See the file `alpha.h' for details.

   Do not define this macro if you do not define
   `SECONDARY_MEMORY_NEEDED' or if widening MODE to a mode that is
   `BITS_PER_WORD' bits wide is correct for your machine. */
/* #define SECONDARY_MEMORY_NEEDED_MODE(MODE) */

/* Normally the compiler avoids choosing registers that have been
   explicitly mentioned in the rtl as spill registers (these
   registers are normally those used to pass parameters and return
   values).  However, some machines have so few registers of certain
   classes that there would not be enough registers to use as spill
   registers if this were done.

   Define `SMALL_REGISTER_CLASSES' on these machines.  When it is
   defined, the compiler allows registers explicitly used in the rtl
   to be used as spill registers but avoids extending the lifetime of
   these registers.

   It is always safe to define this macro, but if you unnecessarily
   define it, you will reduce the amount of optimizations that can be
   performed in some cases.  If you do not define this macro when it
   is required, the compiler will run out of spill registers and
   print a fatal error message.  For most machines, you should not
   define this macro. */
#define SMALL_REGISTER_CLASSES 1

/* A C expression whose value is nonzero if pseudos that have been
   assigned to registers of class CLASS would likely be spilled
   because registers of CLASS are needed for spill registers.

   The default value of this macro returns 1 if CLASS has exactly one
   register and zero otherwise.  On most machines, this default
   should be used.  Only define this macro to some other expression
   if pseudo allocated by `local-alloc.c' end up in memory because
   their hard registers were needed for spill registers.  If this
   macro returns nonzero for those classes, those pseudos will only
   be allocated by `global.c', which knows how to reallocate the
   pseudo to another register.  If there would not be another
   register available for reallocation, you should not change the
   definition of this macro since the only effect of such a
   definition would be to slow down register allocation. */
/* #define CLASS_LIKELY_SPILLED_P(CLASS) */

/* A C expression for the maximum number of consecutive registers of
   class CLASS needed to hold a value of mode MODE.

   This is closely related to the macro `HARD_REGNO_NREGS'.  In fact,
   the value of the macro `CLASS_MAX_NREGS (CLASS, MODE)' should be
   the maximum value of `HARD_REGNO_NREGS (REGNO, MODE)' for all
   REGNO values in the class CLASS.

   This macro helps control the handling of multiple-word values in
   the reload pass. */
#define CLASS_MAX_NREGS(CLASS, MODE)					\
(((CLASS) == ACCUM_REGS)						\
 ? ((GET_MODE_SIZE (MODE) + 4 - 1) / 4)					\
 : ((GET_MODE_SIZE (MODE) + 2 - 1) / 2))

/* If defined, a C expression for a class that contains registers
   which the compiler must always access in a mode that is the same
   size as the mode in which it loaded the register.

   For the example, loading 32-bit integer or floating-point objects
   into floating-point registers on the Alpha extends them to 64-bits.
   Therefore loading a 64-bit object and then storing it as a 32-bit
   object does not store the low-order 32-bits, as would be the case
   for a normal register.  Therefore, `alpha.h' defines this macro as
   `FLOAT_REGS'. */
/* #define CLASS_CANNOT_CHANGE_SIZE */

/* A C expression that defines the machine-dependent operand
   constraint letters that specify particular ranges of integer
   values.  If C is one of those letters, the expression should check
   that VALUE, an integer, is in the appropriate range and return 1
   if so, 0 otherwise.  If C is not one of those letters, the value
   should be 0 regardless of VALUE.

   'I' is for 4-bit unsigned constants (imm4), note bit pattern 0 == 16.
   'J' is for ~ (1 << n), for n <= 15
   'K' is for 32-bit constants with the lower 8 bits in the range -8..7.
   'L' is for 4-bit signed constants (imm4).
   'M' is for 16-bit non-negative constants.
   'N' is for 4-bit negative unsigned constants (imm4), note bit pattern 0 == 16.
   'O' is zero.
   'P' is for (1 << n), where n <= 15.  */

#define CONST_OK_FOR_LETTER_P(VALUE, C)					\
((C) == 'I'   ? IN_RANGE_P (VALUE, 1, 16)				\
 : (C) == 'J' ? IN_RANGE_P (exact_log2 (~ (VALUE)), 0, 15)		\
 : (C) == 'K' ? IN_RANGE_P (SIGN_EXTEND_SHORT (VALUE), -8, 7)		\
 : (C) == 'L' ? IN_RANGE_P (VALUE, -8, 7)				\
 : (C) == 'M' ? IN_RANGE_P (VALUE, 0, 32767)				\
 : (C) == 'N' ? IN_RANGE_P (VALUE, -16, -1)				\
 : (C) == 'O' ? ((VALUE) == 0)						\
 : (C) == 'P' ? IN_RANGE_P (exact_log2 (VALUE), 0, 15)			\
 : 0)

/* A C expression that defines the machine-dependent operand
   constraint letters that specify particular ranges of
   `const_double' values.

   If C is one of those letters, the expression should check that
   VALUE, an RTX of code `const_double', is in the appropriate range
   and return 1 if so, 0 otherwise.  If C is not one of those
   letters, the value should be 0 regardless of VALUE.

   `const_double' is used for all floating-point constants and for
   `DImode' fixed-point constants.  A given letter can accept either
   or both kinds of values.  It can use `GET_MODE' to distinguish
   between these kinds. */

#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) \
((C) == 'G' ? (CONST_DOUBLE_LOW (VALUE) == 0 && CONST_DOUBLE_HIGH (VALUE) == 0) \
 : 0)

/* A C expression that defines the optional machine-dependent
     constraint letters that can be used to segregate specific types of
     operands, usually memory references, for the target machine.
     Normally this macro will not be defined.  If it is required for a
     particular target machine, it should return 1 if VALUE corresponds
     to the operand type represented by the constraint letter C.  If C
     is not defined as an extra constraint, the value returned should
     be 0 regardless of VALUE.

     'Q' is for memory references that are short.  */

#define EXTRA_CONSTRAINT(VALUE, C)					\
  ((C) == 'Q'	? short_memory_operand (VALUE, GET_MODE (VALUE))	\
   :		  0)


/* Stack layout */

/* Structure used to define the d10v stack */
typedef struct d10v_stack {
  int varargs_p;		/* whether this is a varargs function */
  int varargs_size;		/* size to hold varargs args passed in regs */
  int vars_size;		/* variable save area size */
  int parm_size;		/* outgoing parameter size */
  int gpr_size;			/* size of saved GPR registers */
  int accum_size;		/* size of saved ACCUM registers */
  int total_size;		/* total bytes allocated for stack */
				/* which registers are to be saved */
  unsigned char save_p[FIRST_PSEUDO_REGISTER];
} d10v_stack_t;

/* Define this macro if pushing a word onto the stack moves the stack
   pointer to a smaller address.

   When we say, "define this macro if ...," it means that the
   compiler checks this macro only with `#ifdef' so the precise
   definition used does not matter. */

#define STACK_GROWS_DOWNWARD 1

/* Define this macro if the addresses of local variable slots are at
   negative offsets from the frame pointer.

   On the D10v, we grow upwards, from the area before the outgoing
   arguments.  */
/* #define FRAME_GROWS_DOWNWARD */

/* Define this macro if successive arguments to a function occupy
   decreasing addresses on the stack. */
/* #define ARGS_GROW_DOWNWARD */

/* Offset from the frame pointer to the first local variable slot to
   be allocated.

   If `FRAME_GROWS_DOWNWARD', find the next slot's offset by
   subtracting the first slot's length from `STARTING_FRAME_OFFSET'.
   Otherwise, it is found by adding the length of the first slot to
   the value `STARTING_FRAME_OFFSET'.

   On the D10V, the frame pointer is the same as the stack pointer,
   except for dynamic allocations.  So we start after the outgoing
   parameter area.  */

#define STARTING_FRAME_OFFSET						\
  (D10V_ALIGN (current_function_outgoing_args_size, 2))

/* Offset from the stack pointer register to the first location at
   which outgoing arguments are placed.  If not specified, the
   default value of zero is used.  This is the proper value for most
   machines.

   If `ARGS_GROW_DOWNWARD', this is the offset to the location above
   the first location at which outgoing arguments are placed. */
/* #define STACK_POINTER_OFFSET */

/* Offset from the argument pointer register to the first argument's
   address.  On some machines it may depend on the data type of the
   function.

   If `ARGS_GROW_DOWNWARD', this is the offset to the location above
   the first argument's address. */
#define FIRST_PARM_OFFSET(FUNDECL) 0

/* Offset from the stack pointer register to an item dynamically
   allocated on the stack, e.g., by `alloca'.

   The default value for this macro is `STACK_POINTER_OFFSET' plus the
   length of the outgoing arguments.  The default is correct for most
   machines.  See `function.c' for details. */
/* #define STACK_DYNAMIC_OFFSET (FUNDECL) */

/* A C expression whose value is RTL representing the address in a
   stack frame where the pointer to the caller's frame is stored.
   Assume that FRAMEADDR is an RTL expression for the address of the
   stack frame itself.

   If you don't define this macro, the default is to return the value
   of FRAMEADDR--that is, the stack frame address is also the address
   of the stack word that points to the previous frame. */
/* #define DYNAMIC_CHAIN_ADDRESS (FRAMEADDR) */

/* If defined, a C expression that produces the machine-specific code
   to setup the stack so that arbitrary frames can be accessed.  For
   example, on the Sparc, we must flush all of the register windows
   to the stack before we can access arbitrary stack frames.  This
   macro will seldom need to be defined. */
/* #define SETUP_FRAME_ADDRESSES() */

/* A C expression whose value is RTL representing the value of the
   return address for the frame COUNT steps up from the current frame.
   FRAMEADDR is the frame pointer of the COUNT frame, or the frame
   pointer of the COUNT - 1 frame if `RETURN_ADDR_IN_PREVIOUS_FRAME'
   is defined. */
/* #define RETURN_ADDR_RTX (COUNT, FRAMEADDR) */

/* Define this if the return address of a particular stack frame is
   accessed from the frame pointer of the previous stack frame. */
/* #define RETURN_ADDR_IN_PREVIOUS_FRAME */


/* Stack based registers */

/* The register number of the stack pointer register, which must also
   be a fixed register according to `FIXED_REGISTERS'.  On most
   machines, the hardware determines which register this is. */
#define STACK_POINTER_REGNUM (GPR_FIRST + 15)

/* The register number of the frame pointer register, which is used to
   access automatic variables in the stack frame.  On some machines,
   the hardware determines which register this is.  On other
   machines, you can choose any register you wish for this purpose. */
#define FRAME_POINTER_REGNUM (GPR_FIRST + 11)

/* On some machines the offset between the frame pointer and starting
   offset of the automatic variables is not known until after register
   allocation has been done (for example, because the saved registers
   are between these two locations).  On those machines, define
   `FRAME_POINTER_REGNUM' the number of a special, fixed register to
   be used internally until the offset is known, and define
   `HARD_FRAME_POINTER_REGNUM' to be actual the hard register number
   used for the frame pointer.

   You should define this macro only in the very rare circumstances
   when it is not possible to calculate the offset between the frame
   pointer and the automatic variables until after register
   allocation has been completed.  When this macro is defined, you
   must also indicate in your definition of `ELIMINABLE_REGS' how to
   eliminate `FRAME_POINTER_REGNUM' into either
   `HARD_FRAME_POINTER_REGNUM' or `STACK_POINTER_REGNUM'.

   Do not define this macro if it would be the same as
   `FRAME_POINTER_REGNUM'. */
/* #define HARD_FRAME_POINTER_REGNUM */

/* The register number of the arg pointer register, which is used to
   access the function's argument list.  On some machines, this is
   the same as the frame pointer register.  On some machines, the
   hardware determines which register this is.  On other machines,
   you can choose any register you wish for this purpose.  If this is
   not the same register as the frame pointer register, then you must
   mark it as a fixed register according to `FIXED_REGISTERS', or
   arrange to be able to eliminate it (*note Elimination::.). */
#define ARG_POINTER_REGNUM AP_FIRST

/* The register number of the return address pointer register, which
   is used to access the current function's return address from the
   stack.  On some machines, the return address is not at a fixed
   offset from the frame pointer or stack pointer or argument
   pointer.  This register can be defined to point to the return
   address on the stack, and then be converted by `ELIMINABLE_REGS'
   into either the frame pointer or stack pointer.

   Do not define this macro unless there is no other way to get the
   return address from the stack. */
/* #define RETURN_ADDRESS_POINTER_REGNUM */

/* Register numbers used for passing a function's static chain
   pointer.

   The static chain register need not be a fixed register.

   If the static chain is passed in memory, these macros should not be
   defined; instead, the next two macros should be defined. */
#define STATIC_CHAIN_REGNUM (GPR_FIRST + 4)

/* If the static chain is passed in memory, these macros provide rtx
   giving `mem' expressions that denote where they are stored.
   `STATIC_CHAIN' and `STATIC_CHAIN_INCOMING' give the locations as
   seen by the calling and called functions, respectively.  Often the
   former will be at an offset from the stack pointer and the latter
   at an offset from the frame pointer.

   The variables `stack_pointer_rtx', `frame_pointer_rtx', and
   `arg_pointer_rtx' will have been initialized prior to the use of
   these macros and should be used to refer to those items.

   If the static chain is passed in a register, the two previous
   macros should be defined instead. */
/* #define STATIC_CHAIN_INCOMING */
/* #define STATIC_CHAIN */

/* Local d10v return link register number */
#define RETURN_ADDRESS_REGNUM (GPR_FIRST + 13)


/* Eliminating the frame and arg pointers */

/* A C expression which is nonzero if a function must have and use a
   frame pointer.  This expression is evaluated  in the reload pass.
   If its value is nonzero the function will have a frame pointer.

   The expression can in principle examine the current function and
   decide according to the facts, but on most machines the constant 0
   or the constant 1 suffices.  Use 0 when the machine allows code to
   be generated with no frame pointer, and doing so saves some time
   or space.  Use 1 when there is no possible advantage to avoiding a
   frame pointer.

   In certain cases, the compiler does not know how to produce valid
   code without a frame pointer.  The compiler recognizes those cases
   and automatically gives the function a frame pointer regardless of
   what `FRAME_POINTER_REQUIRED' says.  You don't need to worry about
   them.

   In a function that does not require a frame pointer, the frame
   pointer register can be allocated for ordinary usage, unless you
   mark it as a fixed register.  See `FIXED_REGISTERS' for more
   information. */
#define FRAME_POINTER_REQUIRED 0

/* A C statement to store in the variable DEPTH-VAR the difference
   between the frame pointer and the stack pointer values immediately
   after the function prologue.  The value would be computed from
   information such as the result of `get_frame_size ()' and the
   tables of registers `regs_ever_live' and `call_used_regs'.

   If `ELIMINABLE_REGS' is defined, this macro will be not be used and
   need not be defined.  Otherwise, it must be defined even if
   `FRAME_POINTER_REQUIRED' is defined to always be true; in that
   case, you may set DEPTH-VAR to anything. */
/* #define INITIAL_FRAME_POINTER_OFFSET(DEPTH) */

/* If defined, this macro specifies a table of register pairs used to
   eliminate unneeded registers that point into the stack frame.  If
   it is not defined, the only elimination attempted by the compiler
   is to replace references to the frame pointer with references to
   the stack pointer.

   The definition of this macro is a list of structure
   initializations, each of which specifies an original and
   replacement register.

   On some machines, the position of the argument pointer is not
   known until the compilation is completed.  In such a case, a
   separate hard register must be used for the argument pointer.
   This register can be eliminated by replacing it with either the
   frame pointer or the argument pointer, depending on whether or not
   the frame pointer has been eliminated.

   In this case, you might specify:
        #define ELIMINABLE_REGS  \
        {{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM}, \
         {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM}, \
         {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

   Note that the elimination of the argument pointer with the stack
   pointer is specified first since that is the preferred elimination. */

#define ELIMINABLE_REGS							\
{{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM },			\
 { ARG_POINTER_REGNUM,	 STACK_POINTER_REGNUM },			\
 { ARG_POINTER_REGNUM,   FRAME_POINTER_REGNUM }}			\

/* A C expression that returns non-zero if the compiler is allowed to
   try to replace register number FROM-REG with register number
   TO-REG.  This macro need only be defined if `ELIMINABLE_REGS' is
   defined, and will usually be the constant 1, since most of the
   cases preventing register elimination are things that the compiler
   already knows about. */

#define CAN_ELIMINATE(FROM, TO)						\
 ((FROM) == ARG_POINTER_REGNUM && (TO) == STACK_POINTER_REGNUM		\
  ? ! frame_pointer_needed						\
  : 1)

/* This macro is similar to `INITIAL_FRAME_POINTER_OFFSET'.  It
   specifies the initial difference between the specified pair of
   registers.  This macro must be defined if `ELIMINABLE_REGS' is
   defined. */

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
{									\
  d10v_stack_t *info = d10v_stack_info ();				\
									\
 if ((FROM) == FRAME_POINTER_REGNUM && (TO) == STACK_POINTER_REGNUM)	\
   (OFFSET) = 0;							\
 else if ((FROM) == ARG_POINTER_REGNUM && (TO) == FRAME_POINTER_REGNUM)	\
   (OFFSET) = info->total_size;						\
 else if ((FROM) == ARG_POINTER_REGNUM && (TO) == STACK_POINTER_REGNUM)	\
   (OFFSET) = info->total_size;						\
  else									\
    abort ();								\
}


/* Define this macro if the `longjmp' function restores registers from
   the stack frames, rather than from those saved specifically by
   `setjmp'.  Certain quantities must not be kept in registers across
   a call to `setjmp' on such machines. */
/* #define LONGJMP_RESTORE_FROM_STACK */


/* Passing arguments */

/* Define this macro if an argument declared in a prototype as an
   integral type smaller than `int' should actually be passed as an
   `int'.  In addition to avoiding errors in certain cases of
   mismatch, it also makes for better code on certain machines. */
/* #define PROMOTE_PROTOTYPES */

/* A C expression that is the number of bytes actually pushed onto the
   stack when an instruction attempts to push NPUSHED bytes.

   If the target machine does not have a push instruction, do not
   define this macro.  That directs GNU CC to use an alternate
   strategy: to allocate the entire argument block and then store the
   arguments into it.

   On some machines, the definition

        #define PUSH_ROUNDING(BYTES) (BYTES)

   will suffice.  But on other machines, instructions that appear to
   push one byte actually push two bytes in an attempt to maintain
   alignment.  Then the definition should be

        #define PUSH_ROUNDING(BYTES) (((BYTES) + 1) & ~1) */
/* #define PUSH_ROUNDING(NPUSHED) */

/* If defined, the maximum amount of space required for outgoing
   arguments will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.

   Defining both `PUSH_ROUNDING' and `ACCUMULATE_OUTGOING_ARGS' is
   not proper. */
#define ACCUMULATE_OUTGOING_ARGS 1

/* Define this macro if functions should assume that stack space has
   been allocated for arguments even when their values are passed in
   registers.

   The value of this macro is the size, in bytes, of the area
   reserved for arguments passed in registers for the function
   represented by FNDECL.

   This space can be allocated by the caller, or be a part of the
   machine-dependent stack frame: `OUTGOING_REG_PARM_STACK_SPACE' says
   which. */
/* Currently, emit_library_call(,_value} always think that a definition of
   REG_PARM_STACK_SPACE means that all register values need stack space,
   so defining it to always 0 doesn't work; e.g. ashldi3 is affected.  */
/* #define REG_PARM_STACK_SPACE(FNDECL) 0 */

/* Define these macros in addition to the one above if functions might
   allocate stack space for arguments even when their values are
   passed in registers.  These should be used when the stack space
   allocated for arguments in registers is not a simple constant
   independent of the function declaration.

   The value of the first macro is the size, in bytes, of the area
   that we should initially assume would be reserved for arguments
   passed in registers.

   The value of the second macro is the actual size, in bytes, of the
   area that will be reserved for arguments passed in registers.
   This takes two arguments: an integer representing the number of
   bytes of fixed sized arguments on the stack, and a tree
   representing the number of bytes of variable sized arguments on
   the stack.

   When these macros are defined, `REG_PARM_STACK_SPACE' will only be
   called for libcall functions, the current function, or for a
   function being called when it is known that such stack space must
   be allocated.  In each case this value can be easily computed.

   When deciding whether a called function needs such stack space,
   and how much space to reserve, GNU CC uses these two macros
   instead of `REG_PARM_STACK_SPACE'. */
/* #define MAYBE_REG_PARM_STACK_SPACE */
/* #define FINAL_REG_PARM_STACK_SPACE(CONST_SIZE, VAR_SIZE) */

/* Define this if it is the responsibility of the caller to allocate
   the area reserved for arguments passed in registers.

   If `ACCUMULATE_OUTGOING_ARGS' is defined, this macro controls
   whether the space for these arguments counts in the value of
   `current_function_outgoing_args_size'. */
#define OUTGOING_REG_PARM_STACK_SPACE 1

/* Define this macro if `REG_PARM_STACK_SPACE' is defined, but the
   stack parameters don't skip the area specified by it.

   Normally, when a parameter is not passed in registers, it is
   placed on the stack beyond the `REG_PARM_STACK_SPACE' area.
   Defining this macro suppresses this behavior and causes the
   parameter to be passed on the stack in its natural location. */
/* #define STACK_PARMS_IN_REG_PARM_AREA */

/* A C expression that should indicate the number of bytes of its own
   arguments that a function pops on returning, or 0 if the function
   pops no arguments and the caller must therefore pop them all after
   the function returns.

   FUNDECL is a C variable whose value is a tree node that describes
   the function in question.  Normally it is a node of type
   `FUNCTION_DECL' that describes the declaration of the function.
   From this it is possible to obtain the DECL_MACHINE_ATTRIBUTES of
   the function.

   FUNTYPE is a C variable whose value is a tree node that describes
   the function in question.  Normally it is a node of type
   `FUNCTION_TYPE' that describes the data type of the function.
   From this it is possible to obtain the data types of the value and
   arguments (if known).

   When a call to a library function is being considered, FUNTYPE
   will contain an identifier node for the library function.  Thus, if
   you need to distinguish among various library functions, you can
   do so by their names.  Note that "library function" in this
   context means a function used to perform arithmetic, whose name is
   known specially in the compiler and was not mentioned in the C
   code being compiled.

   STACK-SIZE is the number of bytes of arguments passed on the
   stack.  If a variable number of bytes is passed, it is zero, and
   argument popping will always be the responsibility of the calling
   function.

   On the Vax, all functions always pop their arguments, so the
   definition of this macro is STACK-SIZE.  On the 68000, using the
   standard calling convention, no functions pop their arguments, so
   the value of the macro is always 0 in this case.  But an
   alternative calling convention is available in which functions
   that take a fixed number of arguments pop them but other functions
   (such as `printf') pop nothing (the caller pops all).  When this
   convention is in use, FUNTYPE is examined to determine whether a
   function takes a fixed number of arguments. */
#define RETURN_POPS_ARGS(FUNDECL, FUNTYPE, STACKSIZE) 0


/* Function arguments */

/* Nonzero if we do not know how to pass TYPE solely in registers.
   Values that come in registers with inconvenient padding are stored
   to memory at the function start.  */

#define MUST_PASS_IN_STACK(MODE,TYPE)			\
  ((TYPE) != 0						\
   && (TREE_CODE (TYPE_SIZE (TYPE)) != INTEGER_CST	\
       || TREE_ADDRESSABLE (TYPE)))

/* A C expression that controls whether a function argument is passed
   in a register, and which register.

   The arguments are CUM, which summarizes all the previous
   arguments; MODE, the machine mode of the argument; TYPE, the data
   type of the argument as a tree node or 0 if that is not known
   (which happens for C support library functions); and NAMED, which
   is 1 for an ordinary argument and 0 for nameless arguments that
   correspond to `...' in the called function's prototype.

   The value of the expression should either be a `reg' RTX for the
   hard register in which to pass the argument, or zero to pass the
   argument on the stack.

   For machines like the Vax and 68000, where normally all arguments
   are pushed, zero suffices as a definition.

   The usual way to make the ANSI library `stdarg.h' work on a machine
   where some arguments are usually passed in registers, is to cause
   nameless arguments to be passed on the stack instead.  This is done
   by making `FUNCTION_ARG' return 0 whenever NAMED is 0.

   You may use the macro `MUST_PASS_IN_STACK (MODE, TYPE)' in the
   definition of this macro to determine if this argument is of a
   type that must be passed in the stack.  If `REG_PARM_STACK_SPACE'
   is not defined and `FUNCTION_ARG' returns non-zero for such an
   argument, the compiler will abort.  If `REG_PARM_STACK_SPACE' is
   defined, the argument will be computed in the stack and then
   loaded into a register. */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
  function_arg(&CUM, MODE, TYPE, NAMED)

/* Define this macro if the target machine has "register windows", so
   that the register in which a function sees an arguments is not
   necessarily the same as the one in which the caller passed the
   argument.

   For such machines, `FUNCTION_ARG' computes the register in which
   the caller passes the value, and `FUNCTION_INCOMING_ARG' should be
   defined in a similar fashion to tell the function being called
   where the arguments will arrive.

   If `FUNCTION_INCOMING_ARG' is not defined, `FUNCTION_ARG' serves
   both purposes. */
/* #define FUNCTION_INCOMING_ARG(CUM, MODE, TYPE, NAMED) */

/* A C expression for the number of words, at the beginning of an
   argument, must be put in registers.  The value must be zero for
   arguments that are passed entirely in registers or that are
   entirely pushed on the stack.

   On some machines, certain arguments must be passed partially in
   registers and partially in memory.  On these machines, typically
   the first N words of arguments are passed in registers, and the
   rest on the stack.  If a multi-word argument (a `double' or a
   structure) crosses that boundary, its first few words must be
   passed in registers and the rest must be pushed.  This macro tells
   the compiler when this occurs, and how many of the words should go
   in registers.

   `FUNCTION_ARG' for these arguments should return the first
   register to be used by the caller for this argument; likewise
   `FUNCTION_INCOMING_ARG', for the called function. */
/* #define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) */

/* If defined, a C expression that indicates when it is the called
   function's responsibility to make a copy of arguments passed by
   invisible reference.  Normally, the caller makes a copy and passes
   the address of the copy to the routine being called.  When
   FUNCTION_ARG_CALLEE_COPIES is defined and is nonzero, the caller
   does not make a copy.  Instead, it passes a pointer to the "live"
   value.  The called function must not modify this value.  If it can
   be determined that the value won't be modified, it need not make a
   copy; otherwise a copy must be made. */
#define FUNCTION_ARG_CALLEE_COPIES(CUM, MODE, TYPE, NAMED) \
  function_arg_callee_copies(&CUM, MODE, TYPE, NAMED)

/* If defined, a C expression that indicates whether items passed by
   reference should be kept as a reference, or should be allocated to
   a pseudo register.  */
#define FUNCTION_ARG_KEEP_AS_REFERENCE(CUM, MODE, TYPE, NAMED) 1

/* A C type for declaring a variable that is used as the first
   argument of `FUNCTION_ARG' and other related values.  For some
   target machines, the type `int' suffices and can hold the number
   of bytes of argument so far.

   There is no need to record in `CUMULATIVE_ARGS' anything about the
   arguments that have been passed on the stack.  The compiler has
   other variables to keep track of that.  For target machines on
   which all arguments are passed on the stack, there is no need to
   store anything in `CUMULATIVE_ARGS'; however, the data structure
   must exist and should not be empty, so use `int'. */
typedef struct { int reg, stack; } d10v_cumulative_args;
#define CUMULATIVE_ARGS d10v_cumulative_args

/* A C statement (sans semicolon) for initializing the variable CUM
   for the state at the beginning of the argument list.  The variable
   has type `CUMULATIVE_ARGS'.  The value of FNTYPE is the tree node
   for the data type of the function which will receive the args, or 0
   if the args are to a compiler support library function.  The value
   of INDIRECT is nonzero when processing an indirect call, for
   example a call through a function pointer.  The value of INDIRECT
   is zero for a call to an explicitly named function, a library
   function call, or when `INIT_CUMULATIVE_ARGS' is used to find
   arguments for the function being compiled.

   When processing a call to a compiler support library function,
   LIBNAME identifies which one.  It is a `symbol_ref' rtx which
   contains the name of the function, as a string.  LIBNAME is 0 when
   an ordinary C function call is being processed.  Thus, each time
   this macro is called, either LIBNAME or FNTYPE is nonzero, but
   never both of them at once. */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT) \
  init_cumulative_args (&CUM, FNTYPE, LIBNAME, INDIRECT, FALSE)

/* Like `INIT_CUMULATIVE_ARGS' but overrides it for the purposes of
   finding the arguments for the function being compiled.  If this
   macro is undefined, `INIT_CUMULATIVE_ARGS' is used instead.

   The value passed for LIBNAME is always 0, since library routines
   with special calling conventions are never compiled with GNU CC.
   The argument LIBNAME exists for symmetry with
   `INIT_CUMULATIVE_ARGS'. */
#define INIT_CUMULATIVE_INCOMING_ARGS(CUM, FNTYPE, LIBNAME) \
  init_cumulative_args (&CUM, FNTYPE, LIBNAME, FALSE, TRUE)

/* A C statement (sans semicolon) to update the summarizer variable
   CUM to advance past an argument in the argument list.  The values
   MODE, TYPE and NAMED describe that argument.  Once this is done,
   the variable CUM is suitable for analyzing the *following*
   argument with `FUNCTION_ARG', etc.

   This macro need not do anything if the argument in question was
   passed on the stack.  The compiler knows how to track the amount
   of stack space used for arguments without any special help. */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
  function_arg_advance (&CUM, MODE, TYPE, NAMED)

/* If defined, a C expression which determines whether, and in which
   direction, to pad out an argument with extra space.  The value
   should be of type `enum direction': either `upward' to pad above
   the argument, `downward' to pad below, or `none' to inhibit
   padding.

   The *amount* of padding is always just enough to reach the next
   multiple of `FUNCTION_ARG_BOUNDARY'; this macro does not control
   it.

   This macro has a default definition which is right for most
   systems.  For little-endian machines, the default is to pad
   upward.  For big-endian machines, the default is to pad downward
   for an argument of constant size shorter than an `int', and upward
   otherwise. */
/* #define FUNCTION_ARG_PADDING(MODE, TYPE) */

/* If defined, a C expression that gives the alignment boundary, in
   bits, of an argument with the specified mode and type.  If it is
   not defined, `PARM_BOUNDARY' is used for all arguments. */
#define FUNCTION_ARG_BOUNDARY(MODE, TYPE) \
  function_arg_boundary (MODE, TYPE)

/* A C expression that is nonzero if REGNO is the number of a hard
   register in which function arguments are sometimes passed.  This
   does *not* include implicit arguments such as the static chain and
   the structure-value address.  On many machines, no registers can be
   used for this purpose since all function arguments are pushed on
   the stack. */
#define FUNCTION_ARG_REGNO_P(REGNO) IN_RANGE_P (REGNO, ARG_FIRST, ARG_LAST)


/* How values are returned */

/* Define this macro if `-traditional' should not cause functions
   declared to return `float' to convert the value to `double'. */
/* #define TRADITIONAL_RETURN_FLOAT */

/* A C expression to create an RTX representing the place where a
   function returns a value of data type VALTYPE.  VALTYPE is a tree
   node representing a data type.  Write `TYPE_MODE (VALTYPE)' to get
   the machine mode used to represent that type.  On many machines,
   only the mode is relevant.  (Actually, on most machines, scalar
   values are returned in the same place regardless of mode).

   If `PROMOTE_FUNCTION_RETURN' is defined, you must apply the same
   promotion rules specified in `PROMOTE_MODE' if VALTYPE is a scalar
   type.

   If the precise function being called is known, FUNC is a tree node
   (`FUNCTION_DECL') for it; otherwise, FUNC is a null pointer.  This
   makes it possible to use a different value-returning convention
   for specific functions when all their calls are known.

   `FUNCTION_VALUE' is not used for return vales with aggregate data
   types, because these are returned in another way.  See
   `STRUCT_VALUE_REGNUM' and related macros, below. */
#define FUNCTION_VALUE(VALTYPE, FUNC) \
  gen_rtx (REG, TYPE_MODE (VALTYPE), RETURN_REGNUM)

/* Define this macro if the target machine has "register windows" so
   that the register in which a function returns its value is not the
   same as the one in which the caller sees the value.

   For such machines, `FUNCTION_VALUE' computes the register in which
   the caller will see the value.  `FUNCTION_OUTGOING_VALUE' should be
   defined in a similar fashion to tell the function where to put the
   value.

   If `FUNCTION_OUTGOING_VALUE' is not defined, `FUNCTION_VALUE'
   serves both purposes.

   `FUNCTION_OUTGOING_VALUE' is not used for return vales with
   aggregate data types, because these are returned in another way.
   See `STRUCT_VALUE_REGNUM' and related macros, below. */
/* #define FUNCTION_OUTGOING_VALUE(VALTYPE, FUNC) */

/* A C expression to create an RTX representing the place where a
   library function returns a value of mode MODE.  If the precise
   function being called is known, FUNC is a tree node
   (`FUNCTION_DECL') for it; otherwise, FUNC is a null pointer.  This
   makes it possible to use a different value-returning convention
   for specific functions when all their calls are known.

   Note that "library function" in this context means a compiler
   support routine, used to perform arithmetic, whose name is known
   specially by the compiler and was not mentioned in the C code being
   compiled.

   The definition of `LIBRARY_VALUE' need not be concerned aggregate
   data types, because none of the library functions returns such
   types. */

#define LIBCALL_VALUE(MODE) gen_rtx (REG, MODE, RETURN_REGNUM)

/* A C expression that is nonzero if REGNO is the number of a hard
   register in which the values of called function may come back.

   A register whose use for returning values is limited to serving as
   the second of a pair (for a value of type `double', say) need not
   be recognized by this macro.  So for most machines, this definition
   suffices:

        #define FUNCTION_VALUE_REGNO_P(N) ((N) == 0)

   If the machine has register windows, so that the caller and the
   called function use different registers for the return value, this
   macro should recognize only the caller's register numbers. */
#define FUNCTION_VALUE_REGNO_P(REGNO) ((REGNO) == RETURN_REGNUM)

/* Define this macro if `untyped_call' and `untyped_return' need more
   space than is implied by `FUNCTION_VALUE_REGNO_P' for saving and
   restoring an arbitrary return value. */
/* #define APPLY_RESULT_SIZE */

/* A C expression which can inhibit the returning of certain function
   values in registers, based on the type of value.  A nonzero value
   says to return the function value in memory, just as large
   structures are always returned.  Here TYPE will be a C expression
   of type `tree', representing the data type of the value.

   Note that values of mode `BLKmode' must be explicitly handled by
   this macro.  Also, the option `-fpcc-struct-return' takes effect
   regardless of this macro.  On most systems, it is possible to
   leave the macro undefined; this causes a default definition to be
   used, whose value is the constant 1 for `BLKmode' values, and 0
   otherwise.

   Do not use this macro to indicate that structures and unions
   should always be returned in memory.  You should instead use
   `DEFAULT_PCC_STRUCT_RETURN' to indicate this. */
/* #define RETURN_IN_MEMORY(TYPE) */

/* Define this macro to be 1 if all structure and union return values
   must be in memory.  Since this results in slower code, this should
   be defined only if needed for compatibility with other compilers
   or with an ABI.  If you define this macro to be 0, then the
   conventions used for structure and union return values are decided
   by the `RETURN_IN_MEMORY' macro.

   If not defined, this defaults to the value 1. */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* If the structure value address is passed in a register, then
   `STRUCT_VALUE_REGNUM' should be the number of that register. */
#define STRUCT_VALUE_REGNUM (GPR_FIRST + 2)

/* If the structure value address is not passed in a register, define
   `STRUCT_VALUE' as an expression returning an RTX for the place
   where the address is passed.  If it returns 0, the address is
   passed as an "invisible" first argument. */
#define STRUCT_VALUE 0

/* On some architectures the place where the structure value address
   is found by the called function is not the same place that the
   caller put it.  This can be due to register windows, or it could
   be because the function prologue moves it to a different place.

   If the incoming location of the structure value address is in a
   register, define this macro as the register number. */
/* #define STRUCT_VALUE_INCOMING_REGNUM */

/* If the incoming location is not a register, then you should define
   `STRUCT_VALUE_INCOMING' as an expression for an RTX for where the
   called function should find the value.  If it should find the
   value on the stack, define this to create a `mem' which refers to
   the frame pointer.  A definition of 0 means that the address is
   passed as an "invisible" first argument. */
/* #define STRUCT_VALUE_INCOMING */

/* Define this macro if the usual system convention on the target
   machine for returning structures and unions is for the called
   function to return the address of a static variable containing the
   value.

   Do not define this if the usual system convention is for the
   caller to pass an address to the subroutine.

   This macro has effect in `-fpcc-struct-return' mode, but it does
   nothing when you use `-freg-struct-return' mode. */
/* #define PCC_STATIC_STRUCT_RETURN */


/* Caller saves */

/* Define this macro if function calls on the target machine do not
   preserve any registers; in other words, if `CALL_USED_REGISTERS'
   has 1 for all registers.  This macro enables `-fcaller-saves' by
   default.  Eventually that option will be enabled by default on all
   machines and both the option and this macro will be eliminated. */
/* #define DEFAULT_CALLER_SAVES */

/* A C expression to determine whether it is worthwhile to consider
     placing a pseudo-register in a call-clobbered hard register and
     saving and restoring it around each function call.  The expression
     should be 1 when this is worth doing, and 0 otherwise.

     If you don't define this macro, a default is used which is good on
     most machines: `4 * CALLS < REFS'.  */
/* #define CALLER_SAVE_PROFITABLE (REFS, CALLS) */


/* Prologues and epilogues */

/* A C compound statement that outputs the assembler code for entry
   to a function.  The prologue is responsible for setting up the
   stack frame, initializing the frame pointer register, saving
   registers that must be saved, and allocating SIZE additional bytes
   of storage for the local variables.  SIZE is an integer.  FILE is
   a stdio stream to which the assembler code should be output. */

#define FUNCTION_PROLOGUE(FILE, SIZE) function_prologue (FILE, SIZE)

/* Define this macro as a C expression that is nonzero if the return
   instruction or the function epilogue ignores the value of the stack
   pointer; in other words, if it is safe to delete an instruction to
   adjust the stack pointer before a return from the function.

   Note that this macro's value is relevant only for functions for
   which frame pointers are maintained.  It is never safe to delete a
   final stack adjustment in a function that has no frame pointer,
   and the compiler knows this regardless of `EXIT_IGNORE_STACK'. */
/* #define EXIT_IGNORE_STACK */

/* A C compound statement that outputs the assembler code for exit
   from a function.  The epilogue is responsible for restoring the
   saved registers and stack pointer to their values when the
   function was called, and returning control to the caller.  This
   macro takes the same arguments as the macro `FUNCTION_PROLOGUE',
   and the registers to restore are determined from `regs_ever_live'
   and `CALL_USED_REGISTERS' in the same way. */

#define FUNCTION_EPILOGUE(FILE, SIZE) function_epilogue (FILE, SIZE)

/* Define this macro if the function epilogue contains delay slots to
   which instructions from the rest of the function can be "moved".
   The definition should be a C expression whose value is an integer
   representing the number of delay slots there. */
/* #define DELAY_SLOTS_FOR_EPILOGUE */

/* A C expression that returns 1 if INSN can be placed in delay slot
   number N of the epilogue.

   The argument N is an integer which identifies the delay slot now
   being considered (since different slots may have different rules of
   eligibility).  It is never negative and is always less than the
   number of epilogue delay slots (what `DELAY_SLOTS_FOR_EPILOGUE'
   returns).  If you reject a particular insn for a given delay slot,
   in principle, it may be reconsidered for a subsequent delay slot.
   Also, other insns may (at least in principle) be considered for
   the so far unfilled delay slot.

   The insns accepted to fill the epilogue delay slots are put in an
   RTL list made with `insn_list' objects, stored in the variable
   `current_function_epilogue_delay_list'.  The insn for the first
   delay slot comes first in the list.  Your definition of the macro
   `FUNCTION_EPILOGUE' should fill the delay slots by outputting the
   insns in this list, usually by calling `final_scan_insn'.

   You need not define this macro if you did not define
   `DELAY_SLOTS_FOR_EPILOGUE'. */
/* #define ELIGIBLE_FOR_EPILOGUE_DELAY(INSN, N) */

/* A C compound statement that outputs the assembler code for a thunk
   function, used to implement C++ virtual function calls with
   multiple inheritance.  The thunk acts as a wrapper around a
   virtual function, adjusting the implicit object parameter before
   handing control off to the real function.

   First, emit code to add the integer DELTA to the location that
   contains the incoming first argument.  Assume that this argument
   contains a pointer, and is the one used to pass the `this' pointer
   in C++.  This is the incoming argument *before* the function
   prologue, e.g. `%o0' on a sparc.  The addition must preserve the
   values of all other incoming arguments.

   After the addition, emit code to jump to FUNCTION, which is a
   `FUNCTION_DECL'.  This is a direct pure jump, not a call, and does
   not touch the return address.  Hence returning from FUNCTION will
   return to whoever called the current `thunk'.

   The effect must be as if FUNCTION had been called directly with
   the adjusted first argument.  This macro is responsible for
   emitting all of the code for a thunk function; `FUNCTION_PROLOGUE'
   and `FUNCTION_EPILOGUE' are not invoked.

   The THUNK_FNDECL is redundant.  (DELTA and FUNCTION have already
   been extracted from it.)  It might possibly be useful on some
   targets, but probably not.

   For many targets, the target-independent code in the C++ frontend
   will be sufficient and you can leave this macro undefined.  You
   need to define this macro if the code generated by default would
   clobber any of the incoming arguments; this is only likely if
   parameters can be passed in registers.  You should also define
   this macro if the default code is sub-optimal. */
/* #define ASM_OUTPUT_MI_THUNK(FILE, THUNK_FNDECL, DELTA, FUNCTION) */


/* Profiling */

/* A C statement or compound statement to output to FILE some
   assembler code to call the profiling subroutine `mcount'.  Before
   calling, the assembler code must load the address of a counter
   variable into a register where `mcount' expects to find the
   address.  The name of this variable is `LP' followed by the number
   LABELNO, so you would generate the name using `LP%d' in a
   `fprintf'.

   The details of how the address should be passed to `mcount' are
   determined by your operating system environment, not by GNU CC.  To
   figure them out, compile a small program for profiling using the
   system's installed C compiler and look at the assembler code that
   results. */
#define FUNCTION_PROFILER(FILE, LABELNO) abort ()

/* Define this macro if the code for function profiling should come
   before the function prologue.  Normally, the profiling code comes
   after. */
/* #define PROFILE_BEFORE_PROLOGUE */

/* A C statement or compound statement to output to FILE some
   assembler code to initialize basic-block profiling for the current
   object module.  The global compile flag `profile_block_flag'
   distingishes two profile modes.

  `profile_block_flag != 2'
        Output code to call the subroutine `__bb_init_func' once per
        object module, passing it as its sole argument the address of
        a block allocated in the object module.

        The name of the block is a local symbol made with this
        statement:

             ASM_GENERATE_INTERNAL_LABEL (BUFFER, "LPBX", 0);

        Of course, since you are writing the definition of
        `ASM_GENERATE_INTERNAL_LABEL' as well as that of this macro,
        you can take a short cut in the definition of this macro and
        use the name that you know will result.

        The first word of this block is a flag which will be nonzero
        if the object module has already been initialized.  So test
        this word first, and do not call `__bb_init_func' if the flag
        is nonzero.  BLOCK_OR_LABEL contains a unique number which
        may be used to generate a label as a branch destination when
        `__bb_init_func' will not be called.

        Described in assembler language, the code to be output looks
        like:

               cmp (LPBX0),0
               bne local_label
               parameter1 <- LPBX0
               call __bb_init_func
             local_label:

  `profile_block_flag == 2'
        Output code to call the subroutine `__bb_init_trace_func' and
        pass two parameters to it.  The first parameter is the same as
        for `__bb_init_func'.  The second parameter is the number of
        the first basic block of the function as given by
        BLOCK_OR_LABEL.  Note that `__bb_init_trace_func' has to be
        called, even if the object module has been initialized
        already.

        Described in assembler language, the code to be output looks
        like:
             parameter1 <- LPBX0
             parameter2 <- BLOCK_OR_LABEL
             call __bb_init_trace_func */
/* #define FUNCTION_BLOCK_PROFILER (FILE, LABELNO) */

/* A C statement or compound statement to output to FILE some
   assembler code to increment the count associated with the basic
   block number BLOCKNO. */
/* #define BLOCK_PROFILER(FILE, BLOCKNO) */

/* A C statement or compound statement to output to FILE assembler
   code to call function `__bb_trace_ret'.  The assembler code should
   only be output if the global compile flag `profile_block_flag' ==
   2.  This macro has to be used at every place where code for
   returning from a function is generated (e.g. `FUNCTION_EPILOGUE').
   Although you have to write the definition of `FUNCTION_EPILOGUE'
   as well, you have to define this macro to tell the compiler, that
   the proper call to `__bb_trace_ret' is produced. */
/* #define FUNCTION_BLOCK_PROFILER_EXIT(FILE) */

/* A C statement or compound statement to save all registers, which
   may be clobbered by a function call, including condition codes.
   The `asm' statement will be mostly likely needed to handle this
   task.  Local labels in the assembler code can be concatenated with
   the string ID, to obtain a unique lable name.

   Registers or condition codes clobbered by `FUNCTION_PROLOGUE' or
   `FUNCTION_EPILOGUE' must be saved in the macros
   `FUNCTION_BLOCK_PROFILER', `FUNCTION_BLOCK_PROFILER_EXIT' and
   `BLOCK_PROFILER' prior calling `__bb_init_trace_func',
   `__bb_trace_ret' and `__bb_trace_func' respectively. */
/* #define MACHINE_STATE_SAVE(ID) */

/* A C statement or compound statement to restore all registers,
   including condition codes, saved by `MACHINE_STATE_SAVE'.

   Registers or condition codes clobbered by `FUNCTION_PROLOGUE' or
   `FUNCTION_EPILOGUE' must be restored in the macros
   `FUNCTION_BLOCK_PROFILER', `FUNCTION_BLOCK_PROFILER_EXIT' and
   `BLOCK_PROFILER' after calling `__bb_init_trace_func',
   `__bb_trace_ret' and `__bb_trace_func' respectively. */
/* #define MACHINE_STATE_RESTORE(ID) */

/* A C function or functions which are needed in the library to
   support block profiling. */
/* #define BLOCK_PROFILER_CODE */


/* Variable arguments */

/* If defined, is a C expression that produces the machine-specific
   code for a call to `__builtin_saveregs'.  This code will be moved
   to the very beginning of the function, before any parameter access
   are made.  The return value of this function should be an RTX that
   contains the value to use as the return of `__builtin_saveregs'.

   The argument ARGS is a `tree_list' containing the arguments that
   were passed to `__builtin_saveregs'.

   If this macro is not defined, the compiler will output an ordinary
   call to the library function `__builtin_saveregs'. */
#define EXPAND_BUILTIN_SAVEREGS(ARGS) expand_builtin_saveregs (ARGS)


/* This macro offers an alternative to using `__builtin_saveregs' and
   defining the macro `EXPAND_BUILTIN_SAVEREGS'.  Use it to store the
   anonymous register arguments into the stack so that all the arguments
   appear to have been passed consecutively on the stack.  Once this is done,
   you can use the standard implementation of varargs that works for machines
   that pass all their arguments on the stack. */
#define SETUP_INCOMING_VARARGS(ARGS_SO_FAR, MODE, TYPE, PRETEND_ARGS_SIZE, SECOND_TIME) \
  setup_incoming_varargs (&ARGS_SO_FAR, MODE, TYPE, &PRETEND_ARGS_SIZE, SECOND_TIME)

/* Define this macro if the location where a function argument is
   passed depends on whether or not it is a named argument.

   This macro controls how the NAMED argument to `FUNCTION_ARG' is
   set for varargs and stdarg functions.  With this macro defined,
   the NAMED argument is always true for named arguments, and false
   for unnamed arguments.  If this is not defined, but
   `SETUP_INCOMING_VARARGS' is defined, then all arguments are
   treated as named.  Otherwise, all named arguments except the last
   are treated as named. */
/* #define STRICT_ARGUMENT_NAMING */


/* Trampolines */

/* A C statement to output, on the stream FILE, assembler code for a
   block of data that contains the constant parts of a trampoline.
   This code should not include a label--the label is taken care of
   automatically.

   If you do not define this macro, it means no template is needed
   for the target.  Do not define this macro on systems where the
   block move code to copy the trampoline into place would be larger
   than the code to generate it on the spot. */
/* #define TRAMPOLINE_TEMPLATE(FILE) */

/* The name of a subroutine to switch to the section in which the
   trampoline template is to be placed (*note Sections::.).  The
   default is a value of `readonly_data_section', which places the
   trampoline in the section containing read-only data. */
/* #define TRAMPOLINE_SECTION */

/* A C expression for the size in bytes of the trampoline, as an
   integer. */
#define TRAMPOLINE_SIZE 8

/* Alignment required for trampolines, in bits.

   If you don't define this macro, the value of `BIGGEST_ALIGNMENT'
   is used for aligning trampolines. */
#define TRAMPOLINE_ALIGNMENT 32

/* A C statement to initialize the variable parts of a trampoline.
   ADDR is an RTX for the address of the trampoline; FNADDR is an RTX
   for the address of the nested function; STATIC_CHAIN is an RTX for
   the static chain value that should be passed to the function when
   it is called. */

#define INITIALIZE_TRAMPOLINE(ADDR, FNADDR, STATIC_CHAIN) \
  initialize_trampoline (ADDR, FNADDR, STATIC_CHAIN)

/* A C expression to allocate run-time space for a trampoline.  The
   expression value should be an RTX representing a memory reference
   to the space for the trampoline.

   If this macro is not defined, by default the trampoline is
   allocated as a stack slot.  This default is right for most
   machines.  The exceptions are machines where it is impossible to
   execute instructions in the stack area.  On such machines, you may
   have to implement a separate stack, using this macro in
   conjunction with `FUNCTION_PROLOGUE' and `FUNCTION_EPILOGUE'.

   FP points to a data structure, a `struct function', which
   describes the compilation status of the immediate containing
   function of the function which the trampoline is for.  Normally
   (when `ALLOCATE_TRAMPOLINE' is not defined), the stack slot for the
   trampoline is in the stack frame of this containing function.
   Other allocation strategies probably must do something analogous
   with this information.

   Implementing trampolines is difficult on many machines because they
   have separate instruction and data caches.  Writing into a stack
   location fails to clear the memory in the instruction cache, so when
   the program jumps to that location, it executes the old contents.

   Here are two possible solutions.  One is to clear the relevant parts
   of the instruction cache whenever a trampoline is set up.  The other is
   to make all trampolines identical, by having them jump to a standard
   subroutine.  The former technique makes trampoline execution faster; the
   latter makes initialization faster.

   To clear the instruction cache when a trampoline is initialized,
   define the following macros which describe the shape of the cache. */
/* #define ALLOCATE_TRAMPOLINE(FP) allocate_trampoline(FP) */

/* The total size in bytes of the cache. */
/* #define INSN_CACHE_SIZE */

/* The length in bytes of each cache line.  The cache is divided into
   cache lines which are disjoint slots, each holding a contiguous
   chunk of data fetched from memory.  Each time data is brought into
   the cache, an entire line is read at once.  The data loaded into a
   cache line is always aligned on a boundary equal to the line size. */
/* #define INSN_CACHE_LINE_WIDTH */

/* The number of alternative cache lines that can hold any particular
   memory location.

   Alternatively, if the machine has system calls or instructions to
   clear the instruction cache directly, you can define the following
   macro.  */
/* #define INSN_CACHE_DEPTH */

/* If defined, expands to a C expression clearing the *instruction
   cache* in the specified interval.  If it is not defined, and the
   macro INSN_CACHE_SIZE is defined, some generic code is generated
   to clear the cache.  The definition of this macro would typically
   be a series of `asm' statements.  Both BEG and END are both pointer
   expressions.

   To use a standard subroutine, define the following macro.  In
   addition, you must make sure that the instructions in a trampoline fill
   an entire cache line with identical instructions, or else ensure that
   the beginning of the trampoline code is always aligned at the same
   point in its cache line.  Look in `m68k.h' as a guide.
/* #define CLEAR_INSN_CACHE (BEG, END) */

/* Define this macro if trampolines need a special subroutine to do
   their work.  The macro should expand to a series of `asm'
   statements which will be compiled with GNU CC.  They go in a
   library function named `__transfer_from_trampoline'.

   If you need to avoid executing the ordinary prologue code of a
   compiled C function when you jump to the subroutine, you can do so
   by placing a special label of your own in the assembler code.  Use
   one `asm' statement to generate an assembler label, and another to
   make the label global.  Then trampolines can use that label to
   jump directly to your special assembler code. */
/* #define TRANSFER_FROM_TRAMPOLINE */


/* Address modes */

/* Define this macro if the machine supports post-increment,
   pre-increment, post-decrement, and/or pre-decrement
   addressing.  */
#define HAVE_POST_INCREMENT 1
#define HAVE_POST_DECREMENT 1
#define HAVE_PRE_DECREMENT 1
/* #define HAVE_PRE_INCREMENT 0 */

/** A C expression that is 1 if the RTX X is a constant which is a
   valid address.  On most machines, this can be defined as
   `CONSTANT_P (X)', but a few machines are more restrictive in which
   constant addresses are supported.

   `CONSTANT_P' accepts integer-values expressions whose values are
   not explicitly known, such as `symbol_ref', `label_ref', and
   `high' expressions and `const' arithmetic expressions, in addition
   to `const_int' and `const_double' expressions. */
#define CONSTANT_ADDRESS_P(X) CONSTANT_P(X)

/* A number, the maximum number of registers that can appear in a
   valid memory address.  Note that it is up to you to specify a
   value equal to the maximum number that `GO_IF_LEGITIMATE_ADDRESS'
   would ever accept. */
#define MAX_REGS_PER_ADDRESS 1

/* A C compound statement with a conditional `goto LABEL;' executed
   if X (an RTX) is a legitimate memory address on the target machine
   for a memory operand of mode MODE.

   It usually pays to define several simpler macros to serve as
   subroutines for this one.  Otherwise it may be too complicated to
   understand.

   This macro must exist in two variants: a strict variant and a
   non-strict one.  The strict variant is used in the reload pass.  It
   must be defined so that any pseudo-register that has not been
   allocated a hard register is considered a memory reference.  In
   contexts where some kind of register is required, a pseudo-register
   with no hard register must be rejected.

   The non-strict variant is used in other passes.  It must be
   defined to accept all pseudo-registers in every context where some
   kind of register is required.

   Compiler source files that want to use the strict variant of this
   macro define the macro `REG_OK_STRICT'.  You should use an `#ifdef
   REG_OK_STRICT' conditional to define the strict variant in that
   case and the non-strict variant otherwise.

   Subroutines to check for acceptable registers for various purposes
   (one for base registers, one for index registers, and so on) are
   typically among the subroutines used to define
   `GO_IF_LEGITIMATE_ADDRESS'.  Then only these subroutine macros
   need have two variants; the higher levels of macros may be the
   same whether strict or not.  */

#ifdef	REG_OK_STRICT
#define REG_OK_STRICT_P 1
#else
#define REG_OK_STRICT_P 0
#endif

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)				\
do {									\
    if (d10v_legitimate_address_p (MODE, X, REG_OK_STRICT_P))		\
      goto ADDR;							\
} while (0)

/* A C expression that is nonzero if X (assumed to be a `reg' RTX) is
   valid for use as a base register.  For hard registers, it should
   always accept those which the hardware permits and reject the
   others.  Whether the macro accepts or rejects pseudo registers
   must be controlled by `REG_OK_STRICT' as described above.  This
   usually requires two variant definitions, of which `REG_OK_STRICT'
   controls the one actually used.  */

#ifdef REG_OK_STRICT
#define REG_OK_FOR_BASE_P(X) GPR_P (REGNO (X))
#else
#define REG_OK_FOR_BASE_P(X)						\
 (GPR_P (REGNO (X))							\
  || REGNO (X) == ARG_POINTER_REGNUM					\
  || REGNO (X) >= FIRST_PSEUDO_REGISTER)
#endif

/* A C expression that is nonzero if X (assumed to be a `reg' RTX) is
   valid for use as an index register.

   The difference between an index register and a base register is
   that the index register may be scaled.  If an address involves the
   sum of two registers, neither one of them scaled, then either one
   may be labeled the "base" and the other the "index"; but whichever
   labeling is used must fit the machine's constraints of which
   registers may serve in each capacity.  The compiler will try both
   labelings, looking for one that is valid, and will reload one or
   both registers only if neither labeling works.  */
#define REG_OK_FOR_INDEX_P(X) REG_OK_FOR_BASE_P(X)

/* A C compound statement that attempts to replace X with a valid
   memory address for an operand of mode MODE.  WIN will be a C
   statement label elsewhere in the code; the macro definition may use

        GO_IF_LEGITIMATE_ADDRESS (MODE, X, WIN);

   to avoid further processing if the address has become legitimate.

   X will always be the result of a call to `break_out_memory_refs',
   and OLDX will be the operand that was given to that function to
   produce X.

   The code generated by this macro should not alter the substructure
   of X.  If it transforms X into a more legitimate form, it should
   assign X (which will always be a C variable) a new value.

   It is not necessary for this macro to come up with a legitimate
   address.  The compiler has standard ways of doing so in all cases.
   In fact, it is safe for this macro to do nothing.  But often a
   machine-dependent strategy can generate better code. */
#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)				\
do {									\
  rtx y = d10v_legitimize_address (X, OLDX, MODE, REG_OK_STRICT_P);	\
  if (y)								\
    {									\
      X = y;								\
      GO_IF_LEGITIMATE_ADDRESS (MODE, X, WIN);				\
    }									\
} while (0)

/* A C statement or compound statement with a conditional `goto
   LABEL;' executed if memory address X (an RTX) can have different
   meanings depending on the machine mode of the memory reference it
   is used for or if the address is valid for some modes but not
   others.

   Autoincrement and autodecrement addresses typically have
   mode-dependent effects because the amount of the increment or
   decrement is the size of the operand being addressed.  Some
   machines have other mode-dependent addresses.  Many RISC machines
   have no mode-dependent addresses.

   You may assume that ADDR is a valid address for the machine. */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR, LABEL)			\
do {									\
  if (d10v_mode_dependent_address_p (ADDR))				\
    goto LABEL;								\
} while (0)								\

/* A C expression that is nonzero if X is a legitimate constant for
   an immediate operand on the target machine.  You can assume that X
   satisfies `CONSTANT_P', so you need not check this.  In fact, `1'
   is a suitable definition for this macro on machines where anything
   `CONSTANT_P' is valid. */
#define LEGITIMATE_CONSTANT_P(X) 1


/* Condition codes */

/* C code for a data type which is used for declaring the `mdep'
   component of `cc_status'.  It defaults to `int'.

   This macro is not used on machines that do not use `cc0'. */
/* #define CC_STATUS_MDEP */

/* A C expression to initialize the `mdep' field to "empty".  The
   default definition does nothing, since most machines don't use the
   field anyway.  If you want to use the field, you should probably
   define this macro to initialize it.

   This macro is not used on machines that do not use `cc0'. */
/* #define CC_STATUS_MDEP_INIT */

/* A C compound statement to set the components of `cc_status'
   appropriately for an insn INSN whose body is EXP.  It is this
   macro's responsibility to recognize insns that set the condition
   code as a byproduct of other activity as well as those that
   explicitly set `(cc0)'.

   This macro is not used on machines that do not use `cc0'.

   If there are insns that do not set the condition code but do alter
   other machine registers, this macro must check to see whether they
   invalidate the expressions that the condition code is recorded as
   reflecting.  For example, on the 68000, insns that store in address
   registers do not set the condition code, which means that usually
   `NOTICE_UPDATE_CC' can leave `cc_status' unaltered for such insns.
   But suppose that the previous insn set the condition code based
   on location `a4@(102)' and the current insn stores a new value in
   `a4'.  Although the condition code is not changed by this, it will
   no longer be true that it reflects the contents of `a4@(102)'.
   Therefore, `NOTICE_UPDATE_CC' must alter `cc_status' in this case
   to say that nothing is known about the condition code value.

   The definition of `NOTICE_UPDATE_CC' must be prepared to deal with
   the results of peephole optimization: insns whose patterns are
   `parallel' RTXs containing various `reg', `mem' or constants which
   are just the operands.  The RTL structure of these insns is not
   sufficient to indicate what the insns actually do.  What
   `NOTICE_UPDATE_CC' should do when it sees one is just to run
   `CC_STATUS_INIT'.

   A possible definition of `NOTICE_UPDATE_CC' is to call a function
   that looks at an attribute (*note Insn Attributes::.) named, for
   example, `cc'.  This avoids having detailed information about
   patterns in two places, the `md' file and in `NOTICE_UPDATE_CC'. */
/* #define NOTICE_UPDATE_CC (EXP, INSN) */

/* A list of names to be used for additional modes for condition code
   values in registers (*note Jump Patterns::.).  These names are
   added to `enum machine_mode' and all have class `MODE_CC'.  By
   convention, they should start with `CC' and end with `mode'.

   You should only define this macro if your machine does not use
   `cc0' and only if additional modes are required. */
#define EXTRA_CC_MODES CC_REVmode

/* A list of C strings giving the names for the modes listed in
   `EXTRA_CC_MODES'.  For example, the Sparc defines this macro and
   `EXTRA_CC_MODES' as

        #define EXTRA_CC_MODES CC_NOOVmode, CCFPmode, CCFPEmode
        #define EXTRA_CC_NAMES "CC_NOOV", "CCFP", "CCFPE"

   This macro is not required if `EXTRA_CC_MODES' is not defined. */
#define EXTRA_CC_NAMES "CC_REV"

/* Returns a mode from class `MODE_CC' to be used when comparison
   operation code OP is applied to rtx X and Y.  For example, on the
   Sparc, `SELECT_CC_MODE' is defined as (see *note Jump Patterns::.
   for a description of the reason for this definition)

        #define SELECT_CC_MODE(OP,X,Y) \
          (GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT          \
           ? ((OP == EQ || OP == NE) ? CCFPmode : CCFPEmode)    \
           : ((GET_CODE (X) == PLUS || GET_CODE (X) == MINUS    \
               || GET_CODE (X) == NEG) \
              ? CC_NOOVmode : CCmode))

   You need not define this macro if `EXTRA_CC_MODES' is not defined. */
#define SELECT_CC_MODE(OP, X, Y) ((enum machine_mode) select_cc_mode (OP, X, Y))

/* One some machines not all possible comparisons are defined, but
   you can convert an invalid comparison into a valid one.  For
   example, the Alpha does not have a `GT' comparison, but you can
   use an `LT' comparison instead and swap the order of the operands.

   On such machines, define this macro to be a C statement to do any
   required conversions.  CODE is the initial comparison code and OP0
   and OP1 are the left and right operands of the comparison,
   respectively.  You should modify CODE, OP0, and OP1 as required.

   GNU CC will not assume that the comparison resulting from this
   macro is valid but will see if the resulting insn matches a
   pattern in the `md' file.

   You need not define this macro if it would never change the
   comparison code or operands. */

#if 0
#define CANONICALIZE_COMPARISON(CODE,OP0,OP1)				\
  do {									\
    if (((CODE) == GE || (CODE) == GT || (CODE) == GEU || (CODE) == GTU) \
	&& (GET_CODE (OP1) == REG || (OP1) == const0_rtx))		\
      {									\
	rtx tem = (OP0);						\
	(OP0) = (OP1);							\
	(OP1) = tem;							\
	(CODE) = swap_condition (CODE);					\
      }									\
    if (((CODE) == LT || (CODE) == LTU)					\
	&& GET_CODE (OP1) == CONST_INT && INTVAL (OP1) == 256)		\
      {									\
	(CODE) = (CODE) == LT ? LE : LEU;				\
	(OP1) = GEN_INT (255);						\
      }									\
  } while (0)
#endif


/*   A C expression whose value is one if it is always safe to reverse a
     comparison whose mode is MODE.  If `SELECT_CC_MODE' can ever
     return MODE for a floating-point inequality comparison, then
     `REVERSIBLE_CC_MODE (MODE)' must be zero.

     You need not define this macro if it would always returns zero or
     if the floating-point format is anything other than
     `IEEE_FLOAT_FORMAT'.  For example, here is the definition used on
     the Sparc, where floating-point inequality comparisons are always
     given `CCFPEmode':

          #define REVERSIBLE_CC_MODE(MODE)  ((MODE) != CCFPEmode) */
/* #define REVERSIBLE_CC_MODE (MODE) */


/* Describing Relative Costs of Operations  */

/* A part of a C `switch' statement that describes the relative costs
   of constant RTL expressions.  It must contain `case' labels for
   expression codes `const_int', `const', `symbol_ref', `label_ref'
   and `const_double'.  Each case must ultimately reach a `return'
   statement to return the relative cost of the use of that kind of
   constant value in an expression.  The cost may depend on the
   precise value of the constant, which is available for examination
   in X, and the rtx code of the expression in which it is contained,
   found in OUTER_CODE. */
#define CONST_COSTS(X, CODE, OUTER_CODE)				\
  return COSTS_N_INSNS (1);

/* Like `CONST_COSTS' but applies to nonconstant RTL expressions.
   This can be used, for example, to indicate how costly a multiply
   instruction is.  In writing this macro, you can use the construct
   `COSTS_N_INSNS (N)' to specify a cost equal to N fast
   instructions.  OUTER_CODE is the code of the expression in which X
   is contained.

   This macro is optional; do not define it if the default cost
   assumptions are adequate for the target machine.  */
#define RTX_COSTS(X, CODE, OUTER_CODE)					\
  case MULT:								\
    return COSTS_N_INSNS (2);						\
  case DIV:								\
  case MOD:								\
  case UDIV:								\
  case UMOD:								\
    return COSTS_N_INSNS (TARGET_DEBUG ? 2 : 16);

/* An expression giving the cost of an addressing mode that contains
   ADDRESS.  If not defined, the cost is computed from the ADDRESS
   expression and the `CONST_COSTS' values.

   For most CISC machines, the default cost is a good approximation
   of the true cost of the addressing mode.  However, on RISC
   machines, all instructions normally have the same length and
   execution time.  Hence all addresses will have equal costs.

   In cases where more than one form of an address is known, the form
   with the lowest cost will be used.  If multiple forms have the
   same, lowest, cost, the one that is the most complex will be used.

   This macro will normally either not be defined or be defined as a
   constant.  */
/* #define ADDRESS_COST (ADDRESS) */

/* A C expression for the cost of moving data from a register in class
   FROM to one in class TO.  The classes are expressed using the
   enumeration values such as `GENERAL_REGS'.  A value of 2 is the
   default; other values are interpreted relative to that.

   It is not required that the cost always equal 2 when FROM is the
   same as TO; on some machines it is expensive to move between
   registers if they are not general registers.

   If reload sees an insn consisting of a single `set' between two
   hard registers, and if `REGISTER_MOVE_COST' applied to their
   classes returns a value of 2, reload does not check to ensure that
   the constraints of the insn are met.  Setting a cost of other than
   2 will allow reload to verify that the constraints are met.  You
   should do this if the `movM' pattern's constraints do not allow
   such copying.  */
/* #define REGISTER_MOVE_COST (FROM, TO) */

/* A C expression for the cost of moving data of mode M between a
   register and memory.  A value of 4 is the default; this cost is
   relative to those in `REGISTER_MOVE_COST'.

   If moving between registers and memory is more expensive than
   between two registers, you should define this macro to express the
   relative cost.  */
/* #define MEMORY_MOVE_COST (M,C,I) */

/* A C expression for the cost of a branch instruction.  A value of 1
   is the default; other values are interpreted relative to that.  */
#define BRANCH_COST d10v_branch_cost

/* Values of the -mbranch-cost=n string.  */
extern int d10v_branch_cost;
extern char *d10v_branch_cost_string;

/* A C expression for the maximum number of instructions to execute via
   conditional execution instructions instead of a branch.  A value of
   BRANCH_COST+1 is the default if the machine does not use cc0, and 1 if it
   does use cc0. */
#define MAX_CONDITIONAL_EXECUTE d10v_cond_exec

/* Values of the -mcond-exec=n string.  */
extern int d10v_cond_exec;
extern char *d10v_cond_exec_string;


/* Define this macro as a C expression which is nonzero if accessing
   less than a word of memory (i.e. a `char' or a `short') is no
   faster than accessing a word of memory, i.e., if such access
   require more than one instruction or if there is no difference in
   cost between byte and (aligned) word loads.

   When this macro is not defined, the compiler will access a field by
   finding the smallest containing object; when it is defined, a
   fullword load will be used if alignment permits.  Unless bytes
   accesses are faster than word accesses, using word accesses is
   preferable since it may eliminate subsequent memory access if
   subsequent accesses occur to other fields in the same word of the
   structure, but to different bytes.  */
#define SLOW_BYTE_ACCESS 0

/* Define this macro if zero-extension (of a `char' or `short' to an
   `int') can be done faster if the destination is a register that is
   known to be zero.

   If you define this macro, you must have instruction patterns that
   recognize RTL structures like this:

          (set (strict_low_part (subreg:QI (reg:SI ...) 0)) ...)

   and likewise for `HImode'.  */
/* #define SLOW_ZERO_EXTEND */

/* Define this macro to be the value 1 if unaligned accesses have a
   cost many times greater than aligned accesses, for example if they
   are emulated in a trap handler.
   
   When this macro is non-zero, the compiler will act as if
   `STRICT_ALIGNMENT' were non-zero when generating code for block
   moves.  This can cause significantly more instructions to be
   produced.  Therefore, do not set this macro non-zero if unaligned
   accesses only add a cycle or two to the time for a memory access.
   
   If the value of this macro is always zero, it need not be defined. */
/* #define SLOW_UNALIGNED_ACCESS */

/* Define this macro to inhibit strength reduction of memory
   addresses.  (On some machines, such strength reduction seems to do
   harm rather than good.)  */
/* #define DONT_REDUCE_ADDR */

/* The number of scalar move insns which should be generated instead
   of a string move insn or a library call.  Increasing the value
   will always make code faster, but eventually incurs high cost in
   increased code size.

   If you don't define this, a reasonable default is used. */
#define MOVE_RATIO 4

/* Define this macro if it is as good or better to call a constant
   function address than to call an address kept in a register. */
#define NO_FUNCTION_CSE

/* Define this macro if it is as good or better for a function to call
   itself with an explicit address than to call an address kept in a
   register. */
#define NO_RECURSIVE_FUNCTION_CSE

/* A C statement (sans semicolon) to update the integer variable COST
   based on the relationship between INSN that is dependent on
   DEP_INSN through the dependence LINK.  The default is to make no
   adjustment to COST.  This can be used for example to specify to
   the scheduler that an output- or anti-dependence does not incur
   the same cost as a data-dependence. */
/* #define ADJUST_COST (INSN, LINK, DEP_INSN, COST) */

/* A C statement (sans semicolon) to update the integer scheduling
   priority `INSN_PRIORITY(INSN)'.  Reduce the priority to execute
   the INSN earlier, increase the priority to execute INSN later.
   Do not define this macro if you do not need to adjust the
   scheduling priorities of insns. */
/* #define ADJUST_PRIORITY (INSN) */


/* Dividing the Output into Sections (Texts, Data, ...) */

/* A C expression whose value is a string containing the assembler
   operation that should precede instructions and read-only data.
   Normally `".text"' is right.  */
#define TEXT_SECTION_ASM_OP "\t.text"


/* A C expression whose value is a string containing the assembler
   operation to identify the following data as writable initialized
   data.  Normally `".data"' is right.  */
#define DATA_SECTION_ASM_OP "\t.data"


/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as shared data.
   If not defined, `DATA_SECTION_ASM_OP' will be used. */
/* #define SHARED_SECTION_ASM_OP */

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as
   uninitialized global data.  If not defined, and neither
   `ASM_OUTPUT_BSS' nor `ASM_OUTPUT_ALIGNED_BSS' are defined,
   uninitialized global data will be output in the data section if
   `-fno-common' is passed, otherwise `ASM_OUTPUT_COMMON' will be
   used.  */
#ifndef BSS_SECTION_ASM_OP
#define BSS_SECTION_ASM_OP ".section\t.bss"
#endif

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as
   uninitialized global shared data.  If not defined, and
   `BSS_SECTION_ASM_OP' is, the latter will be used. */
/* #define SHARED_BSS_SECTION_ASM_OP */

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as
   initialization code.  If not defined, GNU CC will assume such a
   section does not exist. */
/* #define INIT_SECTION_ASM_OP */

/* A list of names for sections other than the standard two, which are
   `in_text' and `in_data'.  You need not define this macro on a
   system with no other sections (that GCC needs to use). */
/* #define EXTRA_SECTIONS */

/* One or more functions to be defined in `varasm.c'.  These
   functions should do jobs analogous to those of `text_section' and
   `data_section', for your additional sections.  Do not define this
   macro if you do not define `EXTRA_SECTIONS'. */
/* #define EXTRA_SECTION_FUNCTIONS */

/* On most machines, read-only variables, constants, and jump tables
   are placed in the text section.  If this is not the case on your
   machine, this macro should be defined to be the name of a function
   (either `data_section' or a function defined in `EXTRA_SECTIONS')
   that switches to the section to be used for read-only items.

   If these items should be placed in the text section, this macro
   should not be defined. */
/* #define READONLY_DATA_SECTION */

/* A C statement or statements to switch to the appropriate section
   for output of EXP.  You can assume that EXP is either a `VAR_DECL'
   node or a constant of some sort.  RELOC indicates whether the
   initial value of EXP requires link-time relocations.  Select the
   section by calling `text_section' or one of the alternatives for
   other sections.

   Do not define this macro if you put all read-only variables and
   constants in the read-only data section (usually the text section). */
/* #define SELECT_SECTION (EXP, RELOC) */

/* A C statement or statements to switch to the appropriate section
   for output of RTX in mode MODE.  You can assume that RTX is some
   kind of constant in RTL.  The argument MODE is redundant except in
   the case of a `const_int' rtx.  Select the section by calling
   `text_section' or one of the alternatives for other sections.

   Do not define this macro if you put all constants in the read-only
   data section. */
/* #define SELECT_RTX_SECTION (MODE, RTX) */

/* Define this macro if jump tables (for `tablejump' insns) should be
   output in the text section, along with the assembler instructions.
   Otherwise, the readonly data section is used.

   This macro is irrelevant if there is no separate readonly data
   section. */
/* #define JUMP_TABLES_IN_TEXT_SECTION */

/* Define this macro if references to a symbol must be treated
   differently depending on something about the variable or function
   named by the symbol (such as what section it is in).

   The macro definition, if any, is executed immediately after the
   rtl for DECL has been created and stored in `DECL_RTL (DECL)'.
   The value of the rtl will be a `mem' whose address is a
   `symbol_ref'.

   The usual thing for this macro to do is to record a flag in the
   `symbol_ref' (such as `SYMBOL_REF_FLAG') or to store a modified
   name string in the `symbol_ref' (if one bit is not enough
   information). */
#define ENCODE_SECTION_INFO(DECL) encode_section_info (DECL)

/* Decode SYM_NAME and store the real name part in VAR, sans the
   characters that encode section info.  Define this macro if
   `ENCODE_SECTION_INFO' alters the symbol's name string. */
/* #define STRIP_NAME_ENCODING (VAR, SYM_NAME) */

/* For objects going into their own sections, a C expression of the
   name of the section, expressed as a STRING_CST node, to put DECL
   into.  The STRING_CST node must be allocated in the saveable
   obstack.  Function `build_string' can be used to do this.  If you
   do not define this macro, GNU CC will use the symbol name as the
   section name. */
/* #define UNIQUE_SECTION (DECL) */


/* Position Independent Code */

/* The register number of the register used to address a table of
   static data addresses in memory.  In some cases this register is
   defined by a processor's "application binary interface" (ABI).
   When this macro is defined, RTL is generated for this register
   once, as with the stack pointer and frame pointer registers.  If
   this macro is not defined, it is up to the machine-dependent files
   to allocate such a register (if necessary). */
/* #define PIC_OFFSET_TABLE_REGNUM */

/* Define this macro if the register defined by
   `PIC_OFFSET_TABLE_REGNUM' is clobbered by calls.  Do not define
   this macro if `PPIC_OFFSET_TABLE_REGNUM' is not defined. */
/* #define PIC_OFFSET_TABLE_REG_CALL_CLOBBERED */

/* By generating position-independent code, when two different
   programs (A and B) share a common library (libC.a), the text of
   the library can be shared whether or not the library is linked at
   the same address for both programs.  In some of these
   environments, position-independent code requires not only the use
   of different addressing modes, but also special code to enable the
   use of these addressing modes.

   The `FINALIZE_PIC' macro serves as a hook to emit these special
   codes once the function is being compiled into assembly code, but
   not before.  (It is not done before, because in the case of
   compiling an inline function, it would lead to multiple PIC
   prologues being included in functions which used inline functions
   and were compiled to assembly language.) */
/* #define FINALIZE_PIC */

/* A C expression that is nonzero if X is a legitimate immediate
   operand on the target machine when generating position independent
   code.  You can assume that X satisfies `CONSTANT_P', so you need
   not check this.  You can also assume FLAG_PIC is true, so you need
   not check it either.  You need not define this macro if all
   constants (including `SYMBOL_REF') can be immediate operands when
   generating position independent code. */
/* #define LEGITIMATE_PIC_OPERAND_P (X) */


/* The Overall Framework of an Assembler File */

/* A C expression which outputs to the stdio stream STREAM some
   appropriate text to go at the start of an assembler file.

   Normally this macro is defined to output a line containing
   `#NO_APP', which is a comment that has no effect on most
   assemblers but tells the GNU assembler that it can save time by not
   checking for certain assembler constructs.

   On systems that use SDB, it is necessary to output certain
   commands; see `attasm.h'. */
/* #define ASM_FILE_START (STREAM) */

/* A C expression which outputs to the stdio stream STREAM some
   appropriate text to go at the end of an assembler file.

   If this macro is not defined, the default is to output nothing
   special at the end of the file.  Most systems don't require any
   definition.

   On systems that use SDB, it is necessary to output certain
   commands; see `attasm.h'. */
/* #define ASM_FILE_END (STREAM) */

/* A C statement to output assembler commands which will identify the
   object file as having been compiled with GNU CC (or another GNU
   compiler).

   If you don't define this macro, the string `gcc_compiled.:' is
   output.  This string is calculated to define a symbol which, on
   BSD systems, will never be defined for any other reason.  GDB
   checks for the presence of this symbol when reading the symbol
   table of an executable.

   On non-BSD systems, you must arrange communication with GDB in
   some other fashion.  If GDB is not used on your system, you can
     define this macro with an empty body */
/* #define ASM_IDENTIFY_GCC (FILE) */

/* A C string constant describing how to begin a comment in the target
   assembler language.  The compiler assumes that the comment will
   end at the end of the line.  */
#define ASM_COMMENT_START ";"

/* A C string constant for text to be output before each `asm'
   statement or group of consecutive ones.  Normally this is
   `"#APP"', which is a comment that has no effect on most assemblers
   but tells the GNU assembler that it must check the lines that
   follow for all valid assembler constructs.  */
#define ASM_APP_ON ";APP_ON\n"

/* A C string constant for text to be output after each `asm'
   statement or group of consecutive ones.  Normally this is
   `"#NO_APP"', which tells the GNU assembler to resume making the
   time-saving assumptions that are valid for ordinary compiler
   output.  */
#define ASM_APP_OFF ";APP_OFF\n"

/* A C statement to output COFF information or DWARF debugging
   information which indicates that filename NAME is the current
   source file to the stdio stream STREAM.

   This macro need not be defined if the standard form of output for
   the file format in use is appropriate. */
/* #define ASM_OUTPUT_SOURCE_FILENAME (STREAM, NAME) */

/* A C statement to output DBX or SDB debugging information before
   code for line number LINE of the current source file to the stdio
   stream STREAM.

   This macro need not be defined if the standard form of debugging
     information for the debugger in use is appropriate */
/* #define ASM_OUTPUT_SOURCE_LINE (STREAM, LINE) */

/* A C statement to output something to the assembler file to handle a
   `#ident' directive containing the text STRING.  If this macro is
   not defined, nothing is output for a `#ident' directive. */
/* #define ASM_OUTPUT_IDENT (STREAM, STRING) */

/* A C statement to output something to the assembler file to switch
   to section NAME for object DECL which is either a `FUNCTION_DECL',
   a `VAR_DECL' or `NULL_TREE'.  Some target formats do not support
   arbitrary sections.  Do not define this macro in such cases.

   At present this macro is only used to support section attributes.
   When this macro is undefined, section attributes are disabled. */
/* #define ASM_OUTPUT_SECTION_NAME (STREAM, DECL, NAME, RELOC) */

/* A C statement to output any assembler statements which are
    required to precede any Objective C object definitions or message
    sending.  The statement is executed only when compiling an
    Objective C program. */
/* #define `BJC_PROLOGUE */


/* Output of Data */

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble a floating-point constant of `TFmode',
   `DFmode', `SFmode', `TQFmode', `HFmode', or `QFmode',
   respectively, whose value is VALUE.  VALUE will be a C expression
   of type `REAL_VALUE_TYPE'.  Macros such as
   `REAL_VALUE_TO_TARGET_DOUBLE' are useful for writing these
   definitions.

/* #define ASM_OUTPUT_LONG_DOUBLE (STREAM, VALUE) */
/* #define ASM_OUTPUT_DOUBLE (STREAM, VALUE) */
/* #define ASM_OUTPUT_FLOAT (STREAM, VALUE) */
/* #define ASM_OUTPUT_THREE_QUARTER_FLOAT (STREAM, VALUE) */
/* #define ASM_OUTPUT_SHORT_FLOAT (STREAM, VALUE) */
/* #define ASM_OUTPUT_BYTE_FLOAT (STREAM, VALUE) */

#if 1
#define ASM_OUTPUT_DOUBLE(FILE, VALUE)					\
  {									\
    if (REAL_VALUE_ISINF (VALUE)					\
        || REAL_VALUE_ISNAN (VALUE)					\
	|| REAL_VALUE_MINUS_ZERO (VALUE))				\
      {									\
	long t[2];							\
	REAL_VALUE_TO_TARGET_DOUBLE ((VALUE), t);			\
	fprintf (FILE, "\t.long 0x%lx\n\t.long 0x%lx\n",		\
		t[0] & 0xffffffff, t[1] & 0xffffffff);			\
      }									\
    else								\
      {									\
	char str[30];							\
	REAL_VALUE_TO_DECIMAL (VALUE, "%.20e", str);			\
	fprintf (FILE, "\t.double 0d%s\n", str);			\
      }									\
  }

#define ASM_OUTPUT_FLOAT(FILE, VALUE)					\
  {									\
    if (REAL_VALUE_ISINF (VALUE)					\
        || REAL_VALUE_ISNAN (VALUE)					\
	|| REAL_VALUE_MINUS_ZERO (VALUE))				\
      {									\
	long t;								\
	REAL_VALUE_TO_TARGET_SINGLE ((VALUE), t);			\
	fprintf (FILE, "\t.long 0x%lx\n", t & 0xffffffff);		\
      }									\
    else								\
      {									\
	char str[30];							\
	REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", str);			\
	fprintf (FILE, "\t.float 0d%s\n", str);				\
      }									\
  }

#else
#define ASM_OUTPUT_DOUBLE(FILE, VALUE)					\
do									\
  {									\
    long t[2];								\
    char str[30];							\
									\
    REAL_VALUE_TO_TARGET_DOUBLE ((VALUE), t);				\
    if (REAL_VALUE_ISINF (VALUE))					\
      strcpy (str, "Infinity");						\
    else if (REAL_VALUE_ISNAN (VALUE))					\
      strcpy (str, "NaN");						\
    else if (REAL_VALUE_MINUS_ZERO (VALUE))				\
      strcpy (str, "-0");						\
    else								\
      REAL_VALUE_TO_DECIMAL ((VALUE), "%.20g", str);			\
    									\
    fprintf (FILE, "\t.long 0x%lx,0x%lx\t; %s\n",			\
	     t[0] & 0xffffffff, t[1] & 0xffffffff, str);		\
  }									\
while (0)

#define ASM_OUTPUT_FLOAT(FILE, VALUE)					\
do									\
  {									\
    long t;								\
    char str[30];							\
									\
    if (REAL_VALUE_ISINF (VALUE))					\
      strcpy (str, "Infinity");						\
    else if (REAL_VALUE_ISNAN (VALUE))					\
      strcpy (str, "NaN");						\
    else if (REAL_VALUE_MINUS_ZERO (VALUE))				\
      strcpy (str, "-0");						\
    else								\
      REAL_VALUE_TO_DECIMAL ((VALUE), "%.20g", str);			\
    									\
    REAL_VALUE_TO_TARGET_SINGLE ((VALUE), t);				\
    fprintf (FILE, "\t.long 0x%lx\t; %s\n", t & 0xffffffff, str);	\
  }									\
while (0)
#endif

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble an integer of 16, 8, 4, 2 or 1 bytes,
   respectively, whose value is VALUE.  The argument EXP will be an
   RTL expression which represents a constant value.  Use
   `output_addr_const (STREAM, EXP)' to output this value as an
   assembler expression.

   For sizes larger than `UNITS_PER_WORD', if the action of a macro
   would be identical to repeatedly calling the macro corresponding to
   a size of `UNITS_PER_WORD', once for each word, you need not define
   the macro.

   On the D10V, we want to emit word addresses for functions & labels,
   not byte addresses.  ENCODE_SECTION_INFO has set TREE_SIDE_EFFECT on
   function nodes.  */

/* #define ASM_OUTPUT_QUADRUPLE_INT (STREAM, EXP) */
/* #define ASM_OUTPUT_DOUBLE_INT (STREAM, EXP) */

#define ASM_OUTPUT_INT(STREAM, EXP)					\
do {									\
  fputs ("\t.long ", STREAM);						\
  output_addr_const (STREAM, EXP);					\
  putc ('\n', STREAM);							\
} while (0)

/* Use d10v_output_addr_const, not output_addr_const for HI values
   to insure proper truncation and sign extension of negative offsets,
   which sometimes get converted to unsigned HI values.  */
#define ASM_OUTPUT_SHORT(STREAM, EXP)					\
do {									\
  fputs ("\t.word ", STREAM);						\
  if (GET_CODE (EXP) == LABEL_REF)					\
    {									\
      d10v_output_addr_const (STREAM, EXP);				\
      fputs ("@word", STREAM);						\
    }									\
									\
  else if (GET_CODE (EXP) != SYMBOL_REF)				\
    d10v_output_addr_const (STREAM, EXP);				\
									\
  else									\
    {									\
      tree id = get_identifier (XSTR (EXP, 0));				\
      if (TREE_SIDE_EFFECTS (id))					\
	fprintf (STREAM, "%s@word", XSTR (EXP, 0));			\
      else								\
	d10v_output_addr_const (STREAM, EXP);				\
    }									\
									\
  putc ('\n', STREAM);							\
} while (0)

#define ASM_OUTPUT_CHAR(STREAM, EXP)					\
do {									\
  fputs ("\t.byte ", STREAM);						\
  output_addr_const (STREAM, EXP);					\
  putc ('\n', STREAM);							\
} while (0)

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble a single byte containing the number VALUE.  */
#define ASM_OUTPUT_BYTE(STREAM, VALUE) \
  fprintf (STREAM, "\t%s %d\n", ASM_BYTE_OP, VALUE)

/* A C string constant giving the pseudo-op to use for a sequence of
   single-byte constants.  If this macro is not defined, the default
   is `"byte"'.  */
/* #define ASM_BYTE_OP */

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble a string constant containing the LEN bytes
   at PTR.  PTR will be a C expression of type `char *' and LEN a C
   expression of type `int'.

   If the assembler has a `.ascii' pseudo-op as found in the Berkeley
   Unix assembler, do not define the macro `ASM_OUTPUT_ASCII'. */
/* #define ASM_OUTPUT_ASCII (STREAM, PTR, LEN) */

/* A C statement to output assembler commands to define the start of
   the constant pool for a function.  FUNNAME is a string giving the
   name of the function.  Should the return type of the function be
   required, it can be obtained via FUNDECL.  SIZE is the size, in
   bytes, of the constant pool that will be written immediately after
   this call.

   If no constant-pool prefix is required, the usual case, this macro
   need not be defined. */
/* #define ASM_OUTPUT_POOL_PROLOGUE (FILE FUNNAME FUNDECL SIZE) */

/* A C statement (with or without semicolon) to output a constant in
   the constant pool, if it needs special treatment.  (This macro
   need not do anything for RTL expressions that can be output
   normally.)

   The argument FILE is the standard I/O stream to output the
   assembler code on.  X is the RTL expression for the constant to
   output, and MODE is the machine mode (in case X is a `const_int').
   ALIGN is the required alignment for the value X; you should
   output an assembler directive to force this much alignment.

   The argument LABELNO is a number to use in an internal label for
   the address of this pool entry.  The definition of this macro is
   responsible for outputting the label definition at the proper
   place.  Here is how to do this:

        ASM_OUTPUT_INTERNAL_LABEL (FILE, "LC", LABELNO);

   When you output a pool entry specially, you should end with a
   `goto' to the label JUMPTO.  This will prevent the same pool entry
   from being output a second time in the usual manner.

   You need not define this macro if it would do nothing. */
/* #define ASM_OUTPUT_SPECIAL_POOL_ENTRY (FILE, X, MODE, ALIGN, LABELNO, JUMPTO) */

/* Define this macro as a C expression which is nonzero if C is used
   as a logical line separator by the assembler.

   If you do not define this macro, the default is that only the
   character `;' is treated as a logical line separator. */
/* #define IS_ASM_LOGICAL_LINE_SEPARATOR (C) */

/* These macros are defined as C string constant, describing the
   syntax in the assembler for grouping arithmetic expressions. */

#define ASM_OPEN_PAREN "("
#define ASM_CLOSE_PAREN ")"


/* Output of Uninitialized Variables */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of a common-label named NAME whose
   size is SIZE bytes.  The variable ROUNDED is the size rounded up
   to whatever alignment the caller wants.

   Use the expression `assemble_name (STREAM, NAME)' to output the
   name itself; before and after that, output the additional
   assembler syntax for defining the name, and a newline.

   This macro controls how the assembler definitions of uninitialized
   common global variables are output. */
/* #define ASM_OUTPUT_COMMON (STREAM, NAME, SIZE, ROUNDED) */

/* Like `ASM_OUTPUT_COMMON' except takes the required alignment as a
   separate, explicit argument.  If you define this macro, it is used
   in place of `ASM_OUTPUT_COMMON', and gives you more flexibility in
   handling the required alignment of the variable.  The alignment is
   specified as the number of bits. */
/* #define ASM_OUTPUT_ALIGNED_COMMON (STREAM, NAME, SIZE, ALIGNMENT) */

/* If defined, it is similar to `ASM_OUTPUT_COMMON', except that it
   is used when NAME is shared.  If not defined, `ASM_OUTPUT_COMMON'
   will be used. */
/* #define ASM_OUTPUT_SHARED_COMMON (STREAM, NAME, SIZE, ROUNDED) */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes.  The variable ROUNDED is the size
   rounded up to whatever alignment the caller wants.

   Try to use function `asm_output_bss' defined in `varasm.c' when
   defining this macro.  If unable, use the expression `assemble_name
   (STREAM, NAME)' to output the name itself; before and after that,
   output the additional assembler syntax for defining the name, and
   a newline.

   This macro controls how the assembler definitions of uninitialized
   global variables are output.  This macro exists to properly
   support languages like `c++' which do not have `common' data.
   However, this macro currently is not defined for all targets.  If
   this macro and `ASM_OUTPUT_ALIGNED_BSS' are not defined then
   `ASM_OUTPUT_COMMON' or `ASM_OUTPUT_ALIGNED_COMMON' is used. */
/* #define ASM_OUTPUT_BSS (STREAM, DECL, NAME, SIZE, ROUNDED) */

/* Like `ASM_OUTPUT_BSS' except takes the required alignment as a
   separate, explicit argument.  If you define this macro, it is used
   in place of `ASM_OUTPUT_BSS', and gives you more flexibility in
   handling the required alignment of the variable.  The alignment is
   specified as the number of bits.

   Try to use function `asm_output_aligned_bss' defined in file
   `varasm.c' when defining this macro. */
#ifndef ASM_OUTPUT_ALIGNED_BSS
#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGNMENT) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGNMENT)
#endif

/* If defined, it is similar to `ASM_OUTPUT_BSS', except that it is
   used when NAME is shared.  If not defined, `ASM_OUTPUT_BSS' will
   be used. */
/* #define ASM_OUTPUT_SHARED_BSS (STREAM, DECL, NAME, SIZE, ROUNDED) */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of a local-common-label named NAME
   whose size is SIZE bytes.  The variable ROUNDED is the size
   rounded up to whatever alignment the caller wants.

   Use the expression `assemble_name (STREAM, NAME)' to output the
   name itself; before and after that, output the additional
   assembler syntax for defining the name, and a newline.

   This macro controls how the assembler definitions of uninitialized
   static variables are output. */
/* #define ASM_OUTPUT_LOCAL (STREAM, NAME, SIZE, ROUNDED) */

/* Like `ASM_OUTPUT_LOCAL' except takes the required alignment as a
   separate, explicit argument.  If you define this macro, it is used
   in place of `ASM_OUTPUT_LOCAL', and gives you more flexibility in
   handling the required alignment of the variable.  The alignment is
   specified as the number of bits. */
/* #define ASM_OUTPUT_ALIGNED_LOCAL (STREAM, NAME, SIZE, ALIGNMENT) */

/* If defined, it is similar to `ASM_OUTPUT_LOCAL', except that it is
   used when NAME is shared.  If not defined, `ASM_OUTPUT_LOCAL' will
   be used. */
/* #define ASM_OUTPUT_SHARED_LOCAL (STREAM, NAME, SIZE, ROUNDED) */


/* Output and Generation of Labels */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of a label named NAME.  Use the
   expression `assemble_name (STREAM, NAME)' to output the name
   itself; before and after that, output the additional assembler
   syntax for defining the name, and a newline.  */
#define ASM_OUTPUT_LABEL(STREAM, NAME)					\
do {									\
  assemble_name (STREAM, NAME);						\
  fputs (":\n", STREAM);						\
} while (0)


/* A C statement (sans semicolon) to output to the stdio stream
   STREAM any text necessary for declaring the name NAME of a
   function which is being defined.  This macro is responsible for
   outputting the label definition (perhaps using
   `ASM_OUTPUT_LABEL').  The argument DECL is the `FUNCTION_DECL'
   tree node representing the function.

   If this macro is not defined, then the function name is defined in
   the usual manner as a label (by means of `ASM_OUTPUT_LABEL').  */
/* #define ASM_DECLARE_FUNCTION_NAME (STREAM, NAME, DECL) */


/*
`ASM_DECLARE_FUNCTION_SIZE (STREAM, NAME, DECL)'
     A C statement (sans semicolon) to output to the stdio stream
     STREAM any text necessary for declaring the size of a function
     which is being defined.  The argument NAME is the name of the
     function.  The argument DECL is the `FUNCTION_DECL' tree node
     representing the function.

     If this macro is not defined, then the function size is not
     defined.

`ASM_DECLARE_OBJECT_NAME (STREAM, NAME, DECL)'
     A C statement (sans semicolon) to output to the stdio stream
     STREAM any text necessary for declaring the name NAME of an
     initialized variable which is being defined.  This macro must
     output the label definition (perhaps using `ASM_OUTPUT_LABEL').
     The argument DECL is the `VAR_DECL' tree node representing the
     variable.

     If this macro is not defined, then the variable name is defined in
     the usual manner as a label (by means of `ASM_OUTPUT_LABEL').

`ASM_FINISH_DECLARE_OBJECT (STREAM, DECL, TOPLEVEL, ATEND)'
     A C statement (sans semicolon) to finish up declaring a variable
     name once the compiler has processed its initializer fully and
     thus has had a chance to determine the size of an array when
     controlled by an initializer.  This is used on systems where it's
     necessary to declare something about the size of the object.

     If you don't define this macro, that is equivalent to defining it
     to do nothing.
*/

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM some commands that will make the label NAME global; that
   is, available for reference from other files.  Use the expression
   `assemble_name (STREAM, NAME)' to output the name itself; before
   and after that, output the additional assembler syntax for making
   that name global, and a newline.  */

#define ASM_GLOBALIZE_LABEL(STREAM,NAME)				\
do {									\
  fputs ("\t.globl ", STREAM);						\
  assemble_name (STREAM, NAME);						\
  fputs ("\n", STREAM);							\
} while (0)


/* A C statement (sans semicolon) to output to the stdio stream
   STREAM some commands that will make the label NAME weak; that is,
   available for reference from other files but only used if no other
   definition is available.  Use the expression `assemble_name
   (STREAM, NAME)' to output the name itself; before and after that,
   output the additional assembler syntax for making that name weak,
   and a newline.

   If you don't define this macro, GNU CC will not support weak
   symbols and you should not define the `SUPPORTS_WEAK' macro. */
/* #define ASM_WEAKEN_LABEL */

/* A C expression which evaluates to true if the target supports weak
   symbols.

   If you don't define this macro, `defaults.h' provides a default
   definition.  If `ASM_WEAKEN_LABEL' is defined, the default
   definition is `1'; otherwise, it is `0'.  Define this macro if you
   want to control weak symbol support with a compiler flag such as
   `-melf'. */
/* #define SUPPORTS_WEAK */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM any text necessary for declaring the name of an external
   symbol named NAME which is referenced in this compilation but not
   defined.  The value of DECL is the tree node for the declaration.

   This macro need not be defined if it does not need to output
   anything.  The GNU assembler and most Unix assemblers don't
   require anything. */

/* #define ASM_OUTPUT_EXTERNAL(STREAM, DECL, NAME) */

/* A C statement (sans semicolon) to output on STREAM an assembler
   pseudo-op to declare a library function name external.  The name
   of the library function is given by SYMREF, which has type `rtx'
   and is a `symbol_ref'.

   This macro need not be defined if it does not need to output
   anything.  The GNU assembler and most Unix assemblers don't
   require anything. */

#undef ASM_OUTPUT_EXTERNAL_LIBCALL
/* #define ASM_OUTPUT_EXTERNAL_LIBCALL(STREAM, SYMREF) */

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM a reference in assembler syntax to a label named NAME.
   This should add `_' to the front of the name, if that is customary
   on your operating system, as it is in most Berkeley Unix systems.
   This macro is used in `assemble_name'.  */
/* #define ASM_OUTPUT_LABELREF(STREAM, NAME) */

/* A C statement to output to the stdio stream STREAM a label whose
   name is made from the string PREFIX and the number NUM.

   It is absolutely essential that these labels be distinct from the
   labels used for user-level functions and variables.  Otherwise,
   certain programs will have name conflicts with internal labels.

   It is desirable to exclude internal labels from the symbol table
   of the object file.  Most assemblers have a naming convention for
   labels that should be excluded; on many systems, the letter `L' at
   the beginning of a label has this effect.  You should find out what
   convention your system uses, and follow it.

   The usual definition of this macro is as follows:

          fprintf (STREAM, "L%s%d:\n", PREFIX, NUM) */
/* #define ASM_OUTPUT_INTERNAL_LABEL(STREAM, PREFIX, NUM) */

/* A C statement to store into the string STRING a label whose name
   is made from the string PREFIX and the number NUM.

   This string, when output subsequently by `assemble_name', should
   produce the output that `ASM_OUTPUT_INTERNAL_LABEL' would produce
   with the same PREFIX and NUM.

   If the string begins with `*', then `assemble_name' will output
   the rest of the string unchanged.  It is often convenient for
   `ASM_GENERATE_INTERNAL_LABEL' to use `*' in this way.  If the
   string doesn't start with `*', then `ASM_OUTPUT_LABELREF' gets to
   output the string, and may change it.  (Of course,
   `ASM_OUTPUT_LABELREF' is also part of your machine description, so
   you should know what it does on your machine.) */
/* #define ASM_GENERATE_INTERNAL_LABEL(STRING, PREFIX, NUM) */

/* A C expression to assign to OUTVAR (which is a variable of type
   `char *') a newly allocated string made from the string NAME and
   the number NUMBER, with some suitable punctuation added.  Use
   `alloca' to get space for the string.

   The string will be used as an argument to `ASM_OUTPUT_LABELREF' to
   produce an assembler label for an internal static variable whose
   name is NAME.  Therefore, the string must be such as to result in
   valid assembler code.  The argument NUMBER is different each time
   this macro is executed; it prevents conflicts between
   similarly-named internal static variables in different scopes.

   Ideally this string should not be a valid C identifier, to prevent
   any conflict with the user's own symbols.  Most assemblers allow
   periods or percent signs in assembler symbols; putting at least
   one of these between the name and the number will suffice.  */
#define ASM_FORMAT_PRIVATE_NAME(OUTVAR, NAME, NUMBER)			\
do {									\
  (OUTVAR) = (char *) alloca (strlen ((NAME)) + 12);			\
  sprintf ((OUTVAR), "%s.%ld", (NAME), (long)(NUMBER));			\
} while (0)

/*
`ASM_OUTPUT_DEF (STREAM, NAME, VALUE)'
     A C statement to output to the stdio stream STREAM assembler code
     which defines (equates) the symbol NAME to have the value VALUE.

     If SET_ASM_OP is defined, a default definition is provided which is
     correct for most systems.

`OBJC_GEN_METHOD_LABEL (BUF, IS_INST, CLASS_NAME, CAT_NAME, SEL_NAME)'
     Define this macro to override the default assembler names used for
     Objective C methods.

     The default name is a unique method number followed by the name of
     the class (e.g. `_1_Foo').  For methods in categories, the name of
     the category is also included in the assembler name (e.g.
     `_1_Foo_Bar').

     These names are safe on most systems, but make debugging difficult
     since the method's selector is not present in the name.
     Therefore, particular systems define other ways of computing names.

     BUF is an expression of type `char *' which gives you a buffer in
     which to store the name; its length is as long as CLASS_NAME,
     CAT_NAME and SEL_NAME put together, plus 50 characters extra.

     The argument IS_INST specifies whether the method is an instance
     method or a class method; CLASS_NAME is the name of the class;
     CAT_NAME is the name of the category (or NULL if the method is not
     in a category); and SEL_NAME is the name of the selector.

     On systems where the assembler can handle quoted names, you can
     use this macro to provide more human-readable names.
*/


/* Macros Controlling Initialization Routines  */

/*
`INIT_SECTION_ASM_OP'
     If defined, a C string constant for the assembler operation to
     identify the following data as initialization code.  If not
     defined, GNU CC will assume such a section does not exist.  When
     you are using special sections for initialization and termination
     functions, this macro also controls how `crtstuff.c' and
     `libgcc2.c' arrange to run the initialization functions.

`HAS_INIT_SECTION'
     If defined, `main' will not call `__main' as described above.
     This macro should be defined for systems that control the contents
     of the init section on a symbol-by-symbol basis, such as OSF/1,
     and should not be defined explicitly for systems that support
     `INIT_SECTION_ASM_OP'.

`LD_INIT_SWITCH'
     If defined, a C string constant for a switch that tells the linker
     that the following symbol is an initialization routine.

`LD_FINI_SWITCH'
     If defined, a C string constant for a switch that tells the linker
     that the following symbol is a finalization routine.

`INVOKE__main'
     If defined, `main' will call `__main' despite the presence of
     `INIT_SECTION_ASM_OP'.  This macro should be defined for systems
     where the init section is not actually run automatically, but is
     still useful for collecting the lists of constructors and
     destructors.

`ASM_OUTPUT_CONSTRUCTOR (STREAM, NAME)'
     Define this macro as a C statement to output on the stream STREAM
     the assembler code to arrange to call the function named NAME at
     initialization time.

     Assume that NAME is the name of a C function generated
     automatically by the compiler.  This function takes no arguments.
     Use the function `assemble_name' to output the name NAME; this
     performs any system-specific syntactic transformations such as
     adding an underscore.

     If you don't define this macro, nothing special is output to
     arrange to call the function.  This is correct when the function
     will be called in some other manner--for example, by means of the
     `collect2' program, which looks through the symbol table to find
     these functions by their names.

`ASM_OUTPUT_DESTRUCTOR (STREAM, NAME)'
     This is like `ASM_OUTPUT_CONSTRUCTOR' but used for termination
     functions rather than initialization functions.

   If your system uses `collect2' as the means of processing
constructors, then that program normally uses `nm' to scan an object
file for constructor functions to be called.  On certain kinds of
systems, you can define these macros to make `collect2' work faster
(and, in some cases, make it work at all):

`OBJECT_FORMAT_COFF'
     Define this macro if the system uses COFF (Common Object File
     Format) object files, so that `collect2' can assume this format
     and scan object files directly for dynamic constructor/destructor
     functions.

`OBJECT_FORMAT_ROSE'
     Define this macro if the system uses ROSE format object files, so
     that `collect2' can assume this format and scan object files
     directly for dynamic constructor/destructor functions.

     These macros are effective only in a native compiler; `collect2' as
     part of a cross compiler always uses `nm' for the target machine.

`REAL_NM_FILE_NAME'
     Define this macro as a C string constant containing the file name
     to use to execute `nm'.  The default is to search the path
     normally for `nm'.

     If your system supports shared libraries and has a program to list
     the dynamic dependencies of a given library or executable, you can
     define these macros to enable support for running initialization
     and termination functions in shared libraries:

`LDD_SUFFIX'
     Define this macro to a C string constant containing the name of the
     program which lists dynamic dependencies, like `"ldd"' under SunOS
     4.

`PARSE_LDD_OUTPUT (PTR)'
     Define this macro to be C code that extracts filenames from the
     output of the program denoted by `LDD_SUFFIX'.  PTR is a variable
     of type `char *' that points to the beginning of a line of output
     from `LDD_SUFFIX'.  If the line lists a dynamic dependency, the
     code must advance PTR to the beginning of the filename on that
     line.  Otherwise, it must set PTR to `NULL'.
*/


/* Output of Assembler Instructions */

/* A C initializer containing the assembler's names for the machine
   registers, each one as a C string constant.  This is what
   translates register numbers in the compiler into assembler
   language.  */

#define REGISTER_NAMES							\
{									\
  "r0",   "r1",   "r2",   "r3",   "r4",   "r5",   "r6",   "r7",		\
  "r8",   "r9",   "r10",  "r11",  "r12",  "r13",  "r14",  "sp",		\
  "cr0",  "cr1",  "cr2",  "cr3",  "cr4",  "cr5",  "cr6",  "cr7",	\
  "cr8",  "cr9",  "cr10", "cr11", "cr12", "cr13", "cr14", "cr15",	\
  "a0",   "a1",								\
  "ap",									\
  "f0",   "f1",   "c",							\
}

/* If defined, a C initializer for an array of structures containing
   a name and a register number.  This macro defines additional names
   for hard registers, thus allowing the `asm' option in declarations
   to refer to registers using alternate names.  */
#define ADDITIONAL_REGISTER_NAMES					\
{									\
  "r15",	STACK_POINTER_REGNUM,					\
  "repeat",	REPEAT_REGNUM,						\
  "carry",	CARRY_REGNUM,						\
}


/*
`ASM_OUTPUT_OPCODE (STREAM, PTR)'
     Define this macro if you are using an unusual assembler that
     requires different names for the machine instructions.

     The definition is a C statement or statements which output an
     assembler instruction opcode to the stdio stream STREAM.  The
     macro-operand PTR is a variable of type `char *' which points to
     the opcode name in its "internal" form--the form that is written
     in the machine description.  The definition should output the
     opcode name to STREAM, performing any translation you desire, and
     increment the variable PTR to point at the end of the opcode so
     that it will not be output twice.

     In fact, your macro definition may process less than the entire
     opcode name, or more than the opcode name; but if you want to
     process text that includes `%'-sequences to substitute operands,
     you must take care of the substitution yourself.  Just be sure to
     increment PTR over whatever text should not be output normally.

     If you need to look at the operand values, they can be found as the
     elements of `recog_operand'.

     If the macro definition does nothing, the instruction is output in
     the usual way.

`FINAL_PRESCAN_INSN (INSN, OPVEC, NOPERANDS)'
     If defined, a C statement to be executed just prior to the output
     of assembler code for INSN, to modify the extracted operands so
     they will be output differently.

     Here the argument OPVEC is the vector containing the operands
     extracted from INSN, and NOPERANDS is the number of elements of
     the vector which contain meaningful data for this insn.  The
     contents of this vector are what will be used to convert the insn
     template into assembler code, so you can change the assembler
     output by changing the contents of the vector.

     This macro is useful when various assembler syntaxes share a single
     file of instruction patterns; by defining this macro differently,
     you can cause a large class of instructions to be output
     differently (such as with rearranged operands).  Naturally,
     variations in assembler syntax affecting individual insn patterns
     ought to be handled by writing conditional output routines in
     those patterns.

     If this macro is not defined, it is equivalent to a null statement.

`FINAL_PRESCAN_LABEL'
     If defined, `FINAL_PRESCAN_INSN' will be called on each
     `CODE_LABEL'.  In that case, OPVEC will be a null pointer and
     NOPERANDS will be zero.
*/

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand X.  X is an RTL
   expression.

   CODE is a value that can be used to specify one of several ways of
   printing the operand.  It is used when identical operands must be
   printed differently depending on the context.  CODE comes from the
   `%' specification that was used to request printing of the
   operand.  If the specification was just `%DIGIT' then CODE is 0;
   if the specification was `%LTR DIGIT' then CODE is the ASCII code
   for LTR.

   If X is a register, this macro should print the register's name.
   The names can be found in an array `reg_names' whose type is `char
   *[]'.  `reg_names' is initialized from `REGISTER_NAMES'.

   When the machine description has a specification `%PUNCT' (a `%'
   followed by a punctuation character), this macro is called with a
   null pointer for X and the punctuation character for CODE.  */

#define PRINT_OPERAND(STREAM, X, CODE) print_operand (STREAM, X, CODE)

/* A C expression which evaluates to true if CODE is a valid
     punctuation character for use in the `PRINT_OPERAND' macro.  If
     `PRINT_OPERAND_PUNCT_VALID_P' is not defined, it means that no
     punctuation characters (except for the standard one, `%') are used
     in this way. */

#define PRINT_OPERAND_PUNCT_VALID_P(CODE) ((CODE) == '|' || (CODE) == '.')

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is X.  X is an RTL expression.

   On some machines, the syntax for a symbolic address depends on the
   section that the address refers to.  On these machines, define the
   macro `ENCODE_SECTION_INFO' to store the information into the
   `symbol_ref', and then check for it here.  *Note Assembler
   Format::.  */

#define PRINT_OPERAND_ADDRESS(STREAM, X) print_operand_address (STREAM, X)

/*
`DBR_OUTPUT_SEQEND(FILE)'
     A C statement, to be executed after all slot-filler instructions
     have been output.  If necessary, call `dbr_sequence_length' to
     determine the number of slots filled in a sequence (zero if not
     currently outputting a sequence), to decide how many no-ops to
     output, or whatever.

     Don't define this macro if it has nothing to do, but it is helpful
     in reading assembly output if the extent of the delay sequence is
     made explicit (e.g. with white space).

     Note that output routines for instructions with delay slots must be
     prepared to deal with not being output as part of a sequence (i.e.
     when the scheduling pass is not run, or when no slot fillers could
     be found.)  The variable `final_sequence' is null when not
     processing a sequence, otherwise it contains the `sequence' rtx
     being output.

`REGISTER_PREFIX'
`LOCAL_LABEL_PREFIX'
`USER_LABEL_PREFIX'
`IMMEDIATE_PREFIX'
     If defined, C string expressions to be used for the `%R', `%L',
     `%U', and `%I' options of `asm_fprintf' (see `final.c').  These
     are useful when a single `md' file must support multiple assembler
     formats.  In that case, the various `tm.h' files can define these
     macros differently.

`ASSEMBLER_DIALECT'
     If your target supports multiple dialects of assembler language
     (such as different opcodes), define this macro as a C expression
     that gives the numeric index of the assembler language dialect to
     use, with zero as the first variant.

     If this macro is defined, you may use
     `{option0|option1|option2...}' constructs in the output templates
     of patterns (*note Output Template::.) or in the first argument of
     `asm_fprintf'.  This construct outputs `option0', `option1' or
     `option2', etc., if the value of `ASSEMBLER_DIALECT' is zero, one
     or two, etc.  Any special characters within these strings retain
     their usual meaning.

     If you do not define this macro, the characters `{', `|' and `}'
     do not have any special meaning when used in templates or operands
     to `asm_fprintf'.

     Define the macros `REGISTER_PREFIX', `LOCAL_LABEL_PREFIX',
     `USER_LABEL_PREFIX' and `IMMEDIATE_PREFIX' if you can express the
     variations in assemble language syntax with that mechanism.  Define
     `ASSEMBLER_DIALECT' and use the `{option0|option1}' syntax if the
     syntax variant are larger and involve such things as different
     opcodes or operand order.
*/

/* A C expression to output to STREAM some assembler code which will
   push hard register number REGNO onto the stack.  The code need not
   be optimal, since this macro is used only when profiling.  */

#define ASM_OUTPUT_REG_PUSH(STREAM, REGNO)				\
  fprintf (STREAM, "\tsubi %s,-2\n\tst %s,@-%s\n",			\
	   reg_names[STACK_POINTER_REGNUM],				\
	   reg_names[REGNO],						\
	   reg_names[STACK_POINTER_REGNUM])

/* A C expression to output to STREAM some assembler code which will
   pop hard register number REGNO off of the stack.  The code need
   not be optimal, since this macro is used only when profiling.  */

#define ASM_OUTPUT_REG_POP(STREAM, REGNO)				\
  fprintf (STREAM, "\tld %s,@%s+\n\taddi %s,2\n",			\
	   reg_names[REGNO],						\
	   reg_names[STACK_POINTER_REGNUM],				\
	   reg_names[STACK_POINTER_REGNUM])


/* Output of Dispatch Tables */

/* A C statement to output to the stdio stream STREAM an assembler
   pseudo-instruction to generate a difference between two labels.
   vALUE and REL are the numbers of two internal labels.  The
   definitions of these labels are output using
   `ASM_OUTPUT_INTERNAL_LABEL', and they must be printed in the same
   way here.  For example,

          fprintf (STREAM, "\t.word L%d-L%d\n",
                   VALUE, REL)

     You must provide this macro on machines where the addresses in a
     dispatch table are relative to the table's own address.  If
     defined, GNU CC will also use this macro on all machines when
     producing PIC.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL) \
  fprintf (STREAM, "\t.word .L%d@word-.L%d@word\n", VALUE, REL)


/* This macro should be provided on machines where the addresses in a
   dispatch table are absolute.

   The definition should be a C statement to output to the stdio
   stream STREAM an assembler pseudo-instruction to generate a
   reference to a label.  VALUE is the number of an internal label
   whose definition is output using `ASM_OUTPUT_INTERNAL_LABEL'.  */
#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE) \
  fprintf (STREAM, "\t.word .L%d@word\n", VALUE)

/*
`ASM_OUTPUT_CASE_LABEL (STREAM, PREFIX, NUM, TABLE)'
     Define this if the label before a jump-table needs to be output
     specially.  The first three arguments are the same as for
     `ASM_OUTPUT_INTERNAL_LABEL'; the fourth argument is the jump-table
     which follows (a `jump_insn' containing an `addr_vec' or
     `addr_diff_vec').

     This feature is used on system V to output a `swbeg' statement for
     the table.

     If this macro is not defined, these labels are output with
     `ASM_OUTPUT_INTERNAL_LABEL'.

`ASM_OUTPUT_CASE_END (STREAM, NUM, TABLE)'
     Define this if something special must be output at the end of a
     jump-table.  The definition should be a C statement to be executed
     after the assembler code for the table is written.  It should write
     the appropriate code to stdio stream STREAM.  The argument TABLE
     is the jump-table insn, and NUM is the label-number of the
     preceding label.

     If this macro is not defined, nothing special is output at the end
     of the jump-table.
*/


/* Assembler Commands for Alignment  */

/*
`LABEL_ALIGN_AFTER_BARRIER(LABEL)'
     The alignment (log base 2) to put in front of LABEL, which follows
     a BARRIER.

     A C expression to output text to align the location counter in the
     way that is desirable at a point in the code that is reached only
     by jumping.

     This macro need not be defined if you don't want any special
     alignment to be done at such a time.  Most machine descriptions do
     not currently define the macro.

`LOOP_ALIGN (LABEL)'
     The desired alignment for the location counter at the beginning
     of a loop.

     This macro need not be defined if you don't want any special
     alignment to be done at such a time.  Most machine descriptions do
     not currently define the macro.

`ASM_OUTPUT_SKIP (STREAM, NBYTES)'
     A C statement to output to the stdio stream STREAM an assembler
     instruction to advance the location counter by NBYTES bytes.
     Those bytes should be zero when loaded.  NBYTES will be a C
     expression of type `int'.

`ASM_NO_SKIP_IN_TEXT'
     Define this macro if `ASM_OUTPUT_SKIP' should not be used in the
     text section because it fails put zeros in the bytes that are
     skipped.  This is true on many Unix systems, where the pseudo-op
     to skip bytes produces no-op instructions rather than zeros when
     used in the text section.
*/

/* A C statement to output to the stdio stream STREAM an assembler
   command to advance the location counter to a multiple of 2 to the
   POWER bytes.  POWER will be a C expression of type `int'.  */

#define ASM_OUTPUT_ALIGN(STREAM, POWER)					\
do {									\
  if ((POWER) != 0)							\
    fprintf (STREAM, "\t.align %d\n", (POWER));				\
} while (0)


/* Macros Affecting All Debugging Formats */

/* A C expression that returns the DBX register number for the
   compiler register number REGNO.  In simple cases, the value of this
   expression may be REGNO itself.  But sometimes there are some
   registers that the compiler knows about and DBX does not, or vice
   versa.  In such cases, some register may need to have one number in
   the compiler and another for DBX.

   If two registers have consecutive numbers inside GNU CC, and they
   can be used as a pair to hold a multiword value, then they *must*
   have consecutive numbers after renumbering with
   `DBX_REGISTER_NUMBER'.  Otherwise, debuggers will be unable to
   access such a pair, because they expect register pairs to be
   consecutive in their own numbering scheme.

   If you find yourself defining `DBX_REGISTER_NUMBER' in way that
   does not preserve register pairs, then what you must do instead is
   redefine the actual register numbering scheme.  */
#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

/*
`DEBUGGER_AUTO_OFFSET (X)'
     A C expression that returns the integer offset value for an
     automatic variable having address X (an RTL expression).  The
     default computation assumes that X is based on the frame-pointer
     and gives the offset from the frame-pointer.  This is required for
     targets that produce debugging output for DBX or COFF-style
     debugging output for SDB and allow the frame-pointer to be
     eliminated when the `-g' options is used.

`DEBUGGER_ARG_OFFSET (OFFSET, X)'
     A C expression that returns the integer offset value for an
     argument having address X (an RTL expression).  The nominal offset
     is OFFSET.
*/

/* A C expression that returns the type of debugging output GNU CC
   produces when the user specifies `-g' or `-ggdb'.  Define this if
   you have arranged for GNU CC to support more than one format of
   debugging output.  Currently, the allowable values are `DBX_DEBUG',
   `SDB_DEBUG', `DWARF_DEBUG', and `XCOFF_DEBUG'.

   The value of this macro only affects the default debugging output;
   the user can always get a specific type of output by using
   `-gstabs', `-gcoff', `-gdwarf', or `-gxcoff'.  */
#undef	PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG

/* Disable Dwarf2 right now, since it wants a fixed size for DOUBLE and INT */
#undef DWARF2_DEBUGGING_INFO


/* Specific Options for DBX Output */

/*
`DBX_DEBUGGING_INFO'
     Define this macro if GNU CC should produce debugging output for DBX
     in response to the `-g' option.

`XCOFF_DEBUGGING_INFO'
     Define this macro if GNU CC should produce XCOFF format debugging
     output in response to the `-g' option.  This is a variant of DBX
     format.

`DEFAULT_GDB_EXTENSIONS'
     Define this macro to control whether GNU CC should by default
     generate GDB's extended version of DBX debugging information
     (assuming DBX-format debugging information is enabled at all).  If
     you don't define the macro, the default is 1: always generate the
     extended information if there is any occasion to.

`DEBUG_SYMS_TEXT'
     Define this macro if all `.stabs' commands should be output while
     in the text section.

`ASM_STABS_OP'
     A C string constant naming the assembler pseudo op to use instead
     of `.stabs' to define an ordinary debugging symbol.  If you don't
     define this macro, `.stabs' is used.  This macro applies only to
     DBX debugging information format.

`ASM_STABD_OP'
     A C string constant naming the assembler pseudo op to use instead
     of `.stabd' to define a debugging symbol whose value is the current
     location.  If you don't define this macro, `.stabd' is used.  This
     macro applies only to DBX debugging information format.

`ASM_STABN_OP'
     A C string constant naming the assembler pseudo op to use instead
     of `.stabn' to define a debugging symbol with no name.  If you
     don't define this macro, `.stabn' is used.  This macro applies
     only to DBX debugging information format.

`DBX_NO_XREFS'
     Define this macro if DBX on your system does not support the
     construct `xsTAGNAME'.  On some systems, this construct is used to
     describe a forward reference to a structure named TAGNAME.  On
     other systems, this construct is not supported at all.

`DBX_CONTIN_LENGTH'
     A symbol name in DBX-format debugging information is normally
     continued (split into two separate `.stabs' directives) when it
     exceeds a certain length (by default, 80 characters).  On some
     operating systems, DBX requires this splitting; on others,
     splitting must not be done.  You can inhibit splitting by defining
     this macro with the value zero.  You can override the default
     splitting-length by defining this macro as an expression for the
     length you desire.

`DBX_CONTIN_CHAR'
     Normally continuation is indicated by adding a `\' character to
     the end of a `.stabs' string when a continuation follows.  To use
     a different character instead, define this macro as a character
     constant for the character you want to use.  Do not define this
     macro if backslash is correct for your system.

`DBX_STATIC_STAB_DATA_SECTION'
     Define this macro if it is necessary to go to the data section
     before outputting the `.stabs' pseudo-op for a non-global static
     variable.

`DBX_TYPE_DECL_STABS_CODE'
     The value to use in the "code" field of the `.stabs' directive for
     a typedef.  The default is `N_LSYM'.

`DBX_STATIC_CONST_VAR_CODE'
     The value to use in the "code" field of the `.stabs' directive for
     a static variable located in the text section.  DBX format does not
     provide any "right" way to do this.  The default is `N_FUN'.

`DBX_REGPARM_STABS_CODE'
     The value to use in the "code" field of the `.stabs' directive for
     a parameter passed in registers.  DBX format does not provide any
     "right" way to do this.  The default is `N_RSYM'.

`DBX_REGPARM_STABS_LETTER'
     The letter to use in DBX symbol data to identify a symbol as a
     parameter passed in registers.  DBX format does not customarily
     provide any way to do this.  The default is `'P''.

`DBX_MEMPARM_STABS_LETTER'
     The letter to use in DBX symbol data to identify a symbol as a
     stack parameter.  The default is `'p''.

`DBX_FUNCTION_FIRST'
     Define this macro if the DBX information for a function and its
     arguments should precede the assembler code for the function.
     Normally, in DBX format, the debugging information entirely
     follows the assembler code.

`DBX_LBRAC_FIRST'
     Define this macro if the `N_LBRAC' symbol for a block should
     precede the debugging information for variables and functions
     defined in that block.  Normally, in DBX format, the `N_LBRAC'
     symbol comes first.

`DBX_BLOCKS_FUNCTION_RELATIVE'
     Define this macro if the value of a symbol describing the scope of
     a block (`N_LBRAC' or `N_RBRAC') should be relative to the start
     of the enclosing function.  Normally, GNU C uses an absolute
     address.

`DBX_USE_BINCL'
     Define this macro if GNU C should generate `N_BINCL' and `N_EINCL'
     stabs for included header files, as on Sun systems.  This macro
     also directs GNU C to output a type number as a pair of a file
     number and a type number within the file.  Normally, GNU C does not
     generate `N_BINCL' or `N_EINCL' stabs, and it outputs a single
     number for a type number.
*/


 /* Open-Ended Hooks for DBX Format */

/*
`DBX_OUTPUT_LBRAC (STREAM, NAME)'
     Define this macro to say how to output to STREAM the debugging
     information for the start of a scope level for variable names.  The
     argument NAME is the name of an assembler symbol (for use with
     `assemble_name') whose value is the address where the scope begins.

`DBX_OUTPUT_RBRAC (STREAM, NAME)'
     Like `DBX_OUTPUT_LBRAC', but for the end of a scope level.

`DBX_OUTPUT_ENUM (STREAM, TYPE)'
     Define this macro if the target machine requires special handling
     to output an enumeration type.  The definition should be a C
     statement (sans semicolon) to output the appropriate information
     to STREAM for the type TYPE.

`DBX_OUTPUT_FUNCTION_END (STREAM, FUNCTION)'
     Define this macro if the target machine requires special output at
     the end of the debugging information for a function.  The
     definition should be a C statement (sans semicolon) to output the
     appropriate information to STREAM.  FUNCTION is the
     `FUNCTION_DECL' node for the function.

`DBX_OUTPUT_STANDARD_TYPES (SYMS)'
     Define this macro if you need to control the order of output of the
     standard data types at the beginning of compilation.  The argument
     SYMS is a `tree' which is a chain of all the predefined global
     symbols, including names of data types.

     Normally, DBX output starts with definitions of the types for
     integers and characters, followed by all the other predefined
     types of the particular language in no particular order.

     On some machines, it is necessary to output different particular
     types first.  To do this, define `DBX_OUTPUT_STANDARD_TYPES' to
     output those symbols in the necessary order.  Any predefined types
     that you don't explicitly output will be output afterward in no
     particular order.

     Be careful not to define this macro so that it works only for C.
     There are no global variables to access most of the built-in
     types, because another language may have another set of types.
     The way to output a particular type is to look through SYMS to see
     if you can find it.  Here is an example:

          {
            tree decl;
            for (decl = syms; decl; decl = TREE_CHAIN (decl))
              if (!strcmp (IDENTIFIER_POINTER (DECL_NAME (decl)),
                           "long int"))
                dbxout_symbol (decl);
            ...
          }

     This does nothing if the expected type does not exist.

     See the function `init_decl_processing' in `c-decl.c' to find the
     names to use for all the built-in C types.
*/


/* File Names in DBX Format */

/*
`DBX_WORKING_DIRECTORY'
     Define this if DBX wants to have the current directory recorded in
     each object file.

     Note that the working directory is always recorded if GDB
     extensions are enabled.

`DBX_OUTPUT_MAIN_SOURCE_FILENAME (STREAM, NAME)'
     A C statement to output DBX debugging information to the stdio
     stream STREAM which indicates that file NAME is the main source
     file--the file specified as the input file for compilation.  This
     macro is called only once, at the beginning of compilation.

     This macro need not be defined if the standard form of output for
     DBX debugging information is appropriate.

`DBX_OUTPUT_MAIN_SOURCE_DIRECTORY (STREAM, NAME)'
     A C statement to output DBX debugging information to the stdio
     stream STREAM which indicates that the current directory during
     compilation is named NAME.

     This macro need not be defined if the standard form of output for
     DBX debugging information is appropriate.

`DBX_OUTPUT_MAIN_SOURCE_FILE_END (STREAM, NAME)'
     A C statement to output DBX debugging information at the end of
     compilation of the main source file NAME.

     If you don't define this macro, nothing special is output at the
     end of compilation, which is correct for most machines.

`DBX_OUTPUT_SOURCE_FILENAME (STREAM, NAME)'
     A C statement to output DBX debugging information to the stdio
     stream STREAM which indicates that file NAME is the current source
     file.  This output is generated each time input shifts to a
     different source file as a result of `#include', the end of an
     included file, or a `#line' command.

     This macro need not be defined if the standard form of output for
     DBX debugging information is appropriate.
*/


/* Macros for SDB and DWARF Output */

/*
`SDB_DEBUGGING_INFO'
     Define this macro if GNU CC should produce COFF-style debugging
     output for SDB in response to the `-g' option.

`DWARF_DEBUGGING_INFO'
     Define this macro if GNU CC should produce dwarf format debugging
     output in response to the `-g' option.

`PUT_SDB_...'
     Define these macros to override the assembler syntax for the
     special SDB assembler directives.  See `sdbout.c' for a list of
     these macros and their arguments.  If the standard syntax is used,
     you need not define them yourself.

`SDB_DELIM'
     Some assemblers do not support a semicolon as a delimiter, even
     between SDB assembler directives.  In that case, define this macro
     to be the delimiter to use (usually `\n').  It is not necessary to
     define a new set of `PUT_SDB_OP' macros if this is the only change
     required.

`SDB_GENERATE_FAKE'
     Define this macro to override the usual method of constructing a
     dummy name for anonymous structure and union types.  See
     `sdbout.c' for more information.

`SDB_ALLOW_UNKNOWN_REFERENCES'
     Define this macro to allow references to unknown structure, union,
     or enumeration tags to be emitted.  Standard COFF does not allow
     handling of unknown references, MIPS ECOFF has support for it.

`SDB_ALLOW_FORWARD_REFERENCES'
     Define this macro to allow references to structure, union, or
     enumeration tags that have not yet been seen to be handled.  Some
     assemblers choke if forward tags are used, while some require it.
*/


/* Miscellaneous Parameters  */

/* Use the software emulation of floating point.  */
#define REAL_ARITHMETIC

/* Define this if you have defined special-purpose predicates in the
   file `MACHINE.c'.  This macro is called within an initializer of an
   array of structures.  The first field in the structure is the name
   of a predicate and the second field is an array of rtl codes.  For
   each predicate, list all rtl codes that can be in expressions
   matched by the predicate.  The list should have a trailing comma.  */

#define PREDICATE_CODES							\
{ "short_memory_operand",	{ MEM }},				\
{ "reg_or_0_operand",		{ REG, SUBREG, CONST_INT }},		\
{ "arith16_operand",		{ REG, SUBREG, CONST_INT }},		\
{ "arith32_operand",		{ REG, SUBREG, CONST_INT }},		\
{ "arith64_operand",		{ REG, SUBREG, CONST_INT, CONST_DOUBLE }}, \
{ "arith_4bit_operand",		{ REG, SUBREG, CONST_INT }},		\
{ "arith_lower0_operand",	{ REG, SUBREG, CONST_INT }},		\
{ "arith_nonnegative_operand",	{ REG, SUBREG, CONST_INT }},		\
{ "gpr_operand",		{ REG, SUBREG }},			\
{ "accum_operand",		{ REG, SUBREG }},			\
{ "ldi_shift_operand",		{ CONST_INT }},				\
{ "cond_move_operand",		{ REG, SUBREG, CONST_INT, MEM }},	\
{ "cond_exec_operand",		{ REG, SUBREG, CONST_INT, MEM }},	\
{ "pc_or_label_operand",	{ PC, LABEL_REF }},			\
{ "carry_operand",		{ REG }},				\
{ "f0_operand",			{ REG }},				\
{ "f1_operand",			{ REG }},				\
{ "f_operand",			{ REG }},				\
{ "carry_operand",		{ REG }},				\
{ "f0_compare_operator",	{ EQ, NE }},				\
{ "compare_operator",		{ EQ, NE, GT, GE, GTU, GEU, LT, LE,	\
				  LTU, LEU }},				\
{ "equality_operator",		{ EQ, NE, GT, GE, GTU, GEU, LT, LE,	\
				  LTU, LEU }},				\
{ "unsigned_compare_operator",	{ EQ, NE, GT, GE, GTU, GEU, LT, LE,	\
				  LTU, LEU }},				\
{ "signed_compare_operator",	{ EQ, NE, GT, GE, GTU, GEU, LT, LE,	\
				  LTU, LEU }},				\
{ "unary_parallel_operator",	{ ABS, NEG, NOT, REG, SUBREG,		\
				  CONST_INT, MEM,			\
				  SIGN_EXTEND, ZERO_EXTEND  }},		\
{ "binary_parallel_operator",	{ PLUS, MINUS, MULT,			\
				  AND, IOR, XOR,			\
				  ASHIFT, ASHIFTRT, LSHIFTRT }},	\
{ "extend_parallel_operator",	{ SIGN_EXTEND, ZERO_EXTEND }},		\
{ "minmax_parallel_operator",	{ SMIN, SMAX }},

/* An alias for a machine mode name.  This is the machine mode that
   elements of a jump-table should have.  */
#define CASE_VECTOR_MODE Pmode

/*
`CASE_VECTOR_PC_RELATIVE'
     Define this macro if jump-tables should contain relative addresses.

`CASE_DROPS_THROUGH'
     Define this if control falls through a `case' insn when the index
     value is out of range.  This means the specified default-label is
     actually ignored by the `case' insn proper.

`CASE_VALUES_THRESHOLD'
     Define this to be the smallest number of different values for
     which it is best to use a jump-table instead of a tree of
     conditional branches.  The default is four for machines with a
     `casesi' instruction and five otherwise.  This is best for most
     machines.
*/

/* Define this macro if operations between registers with integral
   mode smaller than a word are always performed on the entire
   register.  Most RISC machines have this property and most CISC
   machines do not. */
#define WORD_REGISTER_OPERATIONS 1

/* Define this macro to be a C expression indicating when insns that
   read memory in MODE, an integral mode narrower than a word, set the
   bits outside of MODE to be either the sign-extension or the
   zero-extension of the data read.  Return `SIGN_EXTEND' for values
   of MODE for which the insn sign-extends, `ZERO_EXTEND' for which
   it zero-extends, and `NIL' for other modes.

   This macro is not called with MODE non-integral or with a width
   greater than or equal to `BITS_PER_WORD', so you may return any
   value in this case.  Do not define this macro if it would always
   return `NIL'.  On machines where this macro is defined, you will
   normally define it as the constant `SIGN_EXTEND' or `ZERO_EXTEND'.  */
#define LOAD_EXTEND_OP(MODE) (((MODE) == QImode) ? ZERO_EXTEND : NIL)

/* An alias for a tree code that should be used by default for
   conversion of floating point values to fixed point.  Normally,
   `FIX_ROUND_EXPR' is used.  */
#define IMPLICIT_FIX_EXPR FIX_ROUND_EXPR


/* Define this macro if the same instructions that convert a floating
   point number to a signed fixed point number also convert validly
   to an unsigned one.  */
/* #define FIXUNS_TRUNC_LIKE_FIX_TRUNC */


/* An alias for a tree code that is the easiest kind of division to
     compile code for in the general case.  It may be `TRUNC_DIV_EXPR',
     `FLOOR_DIV_EXPR', `CEIL_DIV_EXPR' or `ROUND_DIV_EXPR'.  These four
     division operators differ in how they round the result to an
     integer.  `EASY_DIV_EXPR' is used when it is permissible to use
     any of those kinds of division and the choice should be made on
     the basis of efficiency. */
#define EASY_DIV_EXPR TRUNC_DIV_EXPR

/* The maximum number of bytes that a single instruction can move
   quickly from memory to memory.  */
#define MOVE_MAX 4


/* The maximum number of bytes that a single instruction can move
   quickly from memory to memory.  If this is undefined, the default
   is `MOVE_MAX'.  Otherwise, it is the constant value that is the
   largest value that `MOVE_MAX' can have at run-time.  */
/* #define MAX_MOVE_MAX */

/*
`SHIFT_COUNT_TRUNCATED'
     A C expression that is nonzero if on this machine the number of
     bits actually used for the count of a shift operation is equal to
     the number of bits needed to represent the size of the object
     being shifted.  When this macro is non-zero, the compiler will
     assume that it is safe to omit a sign-extend, zero-extend, and
     certain bitwise `and' instructions that truncates the count of a
     shift operation.  On machines that have instructions that act on
     bitfields at variable positions, which may include `bit test'
     instructions, a nonzero `SHIFT_COUNT_TRUNCATED' also enables
     deletion of truncations of the values that serve as arguments to
     bitfield instructions.

     If both types of instructions truncate the count (for shifts) and
     position (for bitfield operations), or if no variable-position
     bitfield instructions exist, you should define this macro.

     However, on some machines, such as the 80386 and the 680x0,
     truncation only applies to shift operations and not the (real or
     pretended) bitfield operations.  Define `SHIFT_COUNT_TRUNCATED' to
     be zero on such machines.  Instead, add patterns to the `md' file
     that include the implied truncation of the shift instructions.

     You need not define this macro if it would always have the value
     of zero.
*/

/* A C expression which is nonzero if on this machine it is safe to
   "convert" an integer of INPREC bits to one of OUTPREC bits (where
   OUTPREC is smaller than INPREC) by merely operating on it as if it
   had only OUTPREC bits.

   On many machines, this expression can be 1.

   When `TRULY_NOOP_TRUNCATION' returns 1 for a pair of sizes for
   modes for which `MODES_TIEABLE_P' is 0, suboptimal code can result.
   If this is the case, making `TRULY_NOOP_TRUNCATION' return 0 in
   such cases may improve things.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1


/* A C expression describing the value returned by a comparison
   operator with an integral mode and stored by a store-flag
   instruction (`sCOND') when the condition is true.  This
   description must apply to *all* the `sCOND' patterns and all the
   comparison operators whose results have a `MODE_INT' mode.

   A value of 1 or -1 means that the instruction implementing the
   comparison operator returns exactly 1 or -1 when the comparison is
   true and 0 when the comparison is false.  Otherwise, the value
   indicates which bits of the result are guaranteed to be 1 when the
   comparison is true.  This value is interpreted in the mode of the
   comparison operation, which is given by the mode of the first
   operand in the `sCOND' pattern.  Either the low bit or the sign
   bit of `STORE_FLAG_VALUE' be on.  Presently, only those bits are
   used by the compiler.

   If `STORE_FLAG_VALUE' is neither 1 or -1, the compiler will
   generate code that depends only on the specified bits.  It can also
   replace comparison operators with equivalent operations if they
   cause the required bits to be set, even if the remaining bits are
   undefined.  For example, on a machine whose comparison operators
   return an `SImode' value and where `STORE_FLAG_VALUE' is defined as
   `0x80000000', saying that just the sign bit is relevant, the
   expression

          (ne:SI (and:SI X (const_int POWER-OF-2)) (const_int 0))

   can be converted to

          (ashift:SI X (const_int N))

   where N is the appropriate shift count to move the bit being
   tested into the sign bit.

   There is no way to describe a machine that always sets the
   low-order bit for a true value, but does not guarantee the value
   of any other bits, but we do not know of any machine that has such
   an instruction.  If you are trying to port GNU CC to such a
   machine, include an instruction to perform a logical-and of the
   result with 1 in the pattern for the comparison operators and let
   us know (*note How to Report Bugs: Bug Reporting.).
     
   Often, a machine will have multiple instructions that obtain a
   value from a comparison (or the condition codes).  Here are rules
   to guide the choice of value for `STORE_FLAG_VALUE', and hence the
   instructions to be used:

        * Use the shortest sequence that yields a valid definition for
          `STORE_FLAG_VALUE'.  It is more efficient for the compiler to
          "normalize" the value (convert it to, e.g., 1 or 0) than for
          the comparison operators to do so because there may be
          opportunities to combine the normalization with other
          operations.

        * For equal-length sequences, use a value of 1 or -1, with -1
          being slightly preferred on machines with expensive jumps and
          1 preferred on other machines.

        * As a second choice, choose a value of `0x80000001' if
          instructions exist that set both the sign and low-order bits
          but do not define the others.

        * Otherwise, use a value of `0x80000000'.

   Many machines can produce both the value chosen for
   `STORE_FLAG_VALUE' and its negation in the same number of
   instructions.  On those machines, you should also define a pattern
   for those cases, e.g., one matching

          (set A (neg:M (ne:M B C)))

   Some machines can also perform `and' or `plus' operations on
   condition code values with less instructions than the corresponding
   `sCOND' insn followed by `and' or `plus'.  On those machines,
   define the appropriate patterns.  Use the names `incscc' and
   `decscc', respectively, for the patterns which perform `plus' or
   `minus' operations on condition code values.  See `rs6000.md' for
   some examples.  The GNU Superoptizer can be used to find such
   instruction sequences on other machines.

   You need not define `STORE_FLAG_VALUE' if the machine has no
   store-flag instructions.  */
#define STORE_FLAG_VALUE 1

/*
`FLOAT_STORE_FLAG_VALUE'
     A C expression that gives a non-zero floating point value that is
     returned when comparison operators with floating-point results are
     true.  Define this macro on machine that have comparison
     operations that return floating-point values.  If there are no
     such operations, do not define this macro.
*/

/* An alias for the machine mode for pointers.  On most machines,
   define this to be the integer mode corresponding to the width of a
   hardware pointer; `SImode' on 32-bit machine or `DImode' on 64-bit
   machines.  On some machines you must define this to be one of the
   partial integer modes, such as `PSImode'.

   The width of `Pmode' must be at least as large as the value of
   `POINTER_SIZE'.  If it is not equal, you must define the macro
   `POINTERS_EXTEND_UNSIGNED' to specify how pointers are extended to
   `Pmode'.  */
#define Pmode HImode

/* An alias for the machine mode used for memory references to
   functions being called, in `call' RTL expressions.  On most
   machines this should be `QImode'.  */
#define FUNCTION_MODE Pmode

/*
`INTEGRATE_THRESHOLD (DECL)'
     A C expression for the maximum number of instructions above which
     the function DECL should not be inlined.  DECL is a
     `FUNCTION_DECL' node.

     The default definition of this macro is 64 plus 8 times the number
     of arguments that the function accepts.  Some people think a larger
     threshold should be used on RISC machines.

`SCCS_DIRECTIVE'
     Define this if the preprocessor should ignore `#sccs' directives
     and print no error message.

`NO_IMPLICIT_EXTERN_C'
     Define this macro if the system header files support C++ as well
     as C.  This macro inhibits the usual method of using system header
     files in C++, which is to pretend that the file's contents are
     enclosed in `extern "C" {...}'.

`HANDLE_PRAGMA (STREAM, NODE)'
     Define this macro if you want to implement any pragmas.  If
     defined, it is a C expression whose value is 1 if the pragma was
     handled by the function.  The argument STREAM is the stdio input
     stream from which the source text can be read.  NODE is the tree
     node for the identifier after the `#pragma'.

     It is generally a bad idea to implement new uses of `#pragma'.  The
     only reason to define this macro is for compatibility with other
     compilers that do support `#pragma' for the sake of any user
     programs which already use it.

`VALID_MACHINE_DECL_ATTRIBUTE (DECL, ATTRIBUTES, IDENTIFIER, ARGS)'
     If defined, a C expression whose value is nonzero if IDENTIFIER
     with arguments ARGS is a valid machine specific attribute for DECL.
     The attributes in ATTRIBUTES have previously been assigned to DECL.

`VALID_MACHINE_TYPE_ATTRIBUTE (TYPE, ATTRIBUTES, IDENTIFIER, ARGS)'
     If defined, a C expression whose value is nonzero if IDENTIFIER
     with arguments ARGS is a valid machine specific attribute for TYPE.
     The attributes in ATTRIBUTES have previously been assigned to TYPE.

`COMP_TYPE_ATTRIBUTES (TYPE1, TYPE2)'
     If defined, a C expression whose value is zero if the attributes on
     TYPE1 and TYPE2 are incompatible, one if they are compatible, and
     two if they are nearly compatible (which causes a warning to be
     generated).

`SET_DEFAULT_TYPE_ATTRIBUTES (TYPE)'
     If defined, a C statement that assigns default attributes to newly
     defined TYPE.

`MERGE_MACHINE_TYPE_ATTRIBUTES (TYPE1, TYPE2)'
     Define this macro if the merging of type attributes needs special
     handling.  If defined, the result is a list of the combined
     TYPE_ATTRIBUTES of TYPE1 and TYPE2.  It is assumed that comptypes
     has already been called and returned 1.

`MERGE_MACHINE_DECL_ATTRIBUTES (OLDDECL, NEWDECL)'
     Define this macro if the merging of decl attributes needs special
     handling.  If defined, the result is a list of the combined
     DECL_MACHINE_ATTRIBUTES of OLDDECL and NEWDECL.  NEWDECL is a
     duplicate declaration of OLDDECL.  Examples of when this is needed
     are when one attribute overrides another, or when an attribute is
     nullified by a subsequent definition.

`DOLLARS_IN_IDENTIFIERS'
     Define this macro to control use of the character `$' in identifier
     names.  The value should be 0, 1, or 2.  0 means `$' is not allowed
     by default; 1 means it is allowed by default if `-traditional' is
     used; 2 means it is allowed by default provided `-ansi' is not
     used.  1 is the default; there is no need to define this macro in
     that case.

`NO_DOLLAR_IN_LABEL'
     Define this macro if the assembler does not accept the character
     `$' in label names.  By default constructors and destructors in
     G++ have `$' in the identifiers.  If this macro is defined, `.' is
     used instead.

`NO_DOT_IN_LABEL'
     Define this macro if the assembler does not accept the character
     `.' in label names.  By default constructors and destructors in G++
     have names that use `.'.  If this macro is defined, these names
     are rewritten to avoid `.'.

`DEFAULT_MAIN_RETURN'
     Define this macro if the target system expects every program's
     `main' function to return a standard "success" value by default
     (if no other value is explicitly returned).

     The definition should be a C statement (sans semicolon) to
     generate the appropriate rtl instructions.  It is used only when
     compiling the end of `main'.

`HAVE_ATEXIT'
     Define this if the target system supports the function `atexit'
     from the ANSI C standard.  If this is not defined, and
     `INIT_SECTION_ASM_OP' is not defined, a default `exit' function
     will be provided to support C++.

`EXIT_BODY'
     Define this if your `exit' function needs to do something besides
     calling an external function `_cleanup' before terminating with
     `_exit'.  The `EXIT_BODY' macro is only needed if netiher
     `HAVE_ATEXIT' nor `INIT_SECTION_ASM_OP' are defined.

`INSN_SETS_ARE_DELAYED (INSN)'
     Define this macro as a C expression that is nonzero if it is safe
     for the delay slot scheduler to place instructions in the delay
     slot of INSN, even if they appear to use a resource set or
     clobbered in INSN.  INSN is always a `jump_insn' or an `insn'; GNU
     CC knows that every `call_insn' has this behavior.  On machines
     where some `insn' or `jump_insn' is really a function call and
     hence has this behavior, you should define this macro.

     You need not define this macro if it would always return zero.

`INSN_REFERENCES_ARE_DELAYED (INSN)'
     Define this macro as a C expression that is nonzero if it is safe
     for the delay slot scheduler to place instructions in the delay
     slot of INSN, even if they appear to set or clobber a resource
     referenced in INSN.  INSN is always a `jump_insn' or an `insn'.
     On machines where some `insn' or `jump_insn' is really a function
     call and its operands are registers whose use is actually in the
     subroutine it calls, you should define this macro.  Doing so
     allows the delay slot scheduler to move instructions which copy
     arguments into the argument registers into the delay slot of INSN.

     You need not define this macro if it would always return zero.

`MACHINE_DEPENDENT_REORG (INSN)'
     In rare cases, correct code generation requires extra machine
     dependent processing between the second jump optimization pass and
     delayed branch scheduling.  On those machines, define this macro
     as a C statement to act on the code starting at INSN.
*/


/* Hack to omit compiling L_shtab in libgcc2.  Shtab is an array mapping
   n -> 1<<n that is used in the Tahoe, but it assumes that int can handle
   32-bits.  */

#undef L_shtab


/* Haifa scheduler options */

/* uncomment for disabling the corresponding default options */
/* #define  MACHINE_no_sched_interblock */
/* #define  MACHINE_no_sched_speculative */
/* #define  MACHINE_no_sched_speculative_load */

/* indicate that issue rate is defined for this machine
   (no need to use the default) */
#define ISSUE_RATE 2


/* External functions which don't have a proper prototype.  */
extern int offsettable_memref_p ();

/* External data */
extern int optimize;

/* Variables to hold compare operators until the appropriate branch or scc
   operation is done.  */
extern struct rtx_def *compare_op0;
extern struct rtx_def *compare_op1;

/* Functions defined in d10v.c  */
#define Rtx struct rtx_def *

extern void override_options ();
extern void encode_section_info ();
extern int  short_memory_operand ();
extern int  reg_or_0_operand ();
extern int  arith16_operand ();
extern int  arith32_operand ();
extern int  arith64_operand ();
extern int  arith_4bit_operand ();
extern int  arith_lower0_operand ();
extern int  arith_nonnegative_operand ();
extern int  gpr_operand ();
extern int  accum_operand ();
extern int  ldi_shift_operand ();
extern int  cond_move_operand ();
extern int  cond_exec_operand ();
extern int  pc_or_label_operand ();
extern int  f0_operand ();
extern int  f1_operand ();
extern int  f_operand ();
extern int  carry_operand ();
extern int  f0_compare_operator ();
extern int  compare_operator ();
extern int  equality_operator ();
extern int  signed_compare_operator ();
extern int  unsigned_compare_operator ();
extern int  unary_parallel_operator ();
extern int  binary_parallel_operator ();
extern int  extend_parallel_operator ();
extern int  parallel_ldi ();
extern int  adjacent_memory_operands ();
extern void d10v_expand_divmod ();
extern void initialize_trampoline ();
extern int  emit_comparison ();
extern int  select_cc_mode ();
extern Rtx  expand_comparison ();
extern char *emit_move_word ();
extern char *emit_move_2words ();
extern char *emit_move_4words ();
extern char *emit_add ();
extern char *emit_subtract ();
extern char *emit_cond_move ();
extern Rtx  d10v_split_logical_op ();
extern void d10v_output_addr_const ();
extern void print_operand ();
extern void print_operand_address ();
extern void init_cumulative_args ();
extern Rtx  function_arg ();
extern int  function_arg_partial_nregs ();
extern int  function_arg_pass_by_reference ();
extern int  function_arg_callee_copies ();
extern void function_arg_advance ();
extern int  function_arg_boundary ();
extern void setup_incoming_varargs ();
extern struct rtx_def *expand_builtin_saveregs ();
extern int  d10v_legitimate_address_p ();
extern Rtx  d10v_legitimize_address ();
extern int  d10v_mode_dependent_address_p ();
extern Rtx  d10v_subword_mode ();
extern Rtx  d10v_subword ();
extern int  direct_return ();
extern void function_prologue ();
extern void function_epilogue ();
extern d10v_stack_t *d10v_stack_info ();
/* END CYGNUS LOCAL -- meissner/d10v abi change */
