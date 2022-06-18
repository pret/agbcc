/* Output Dwarf2 format symbol table information from the GNU C compiler.
   Copyright (C) 1992, 93, 95, 96, 97, 1998 Free Software Foundation, Inc.
   Contributed by Gary Funck (gary@intrepid.com).
   Derived from DWARF 1 implementation of Ron Guilmette (rfg@monkeys.com).
   Extensively modified by Jason Merrill (jason@cygnus.com).

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

/* The first part of this file deals with the DWARF 2 frame unwind
   information, which is also used by the GCC efficient exception handling
   mechanism.  The second part, controlled only by an #ifdef
   DWARF2_DEBUGGING_INFO, deals with the other DWARF 2 debugging
   information.  */

#include <assert.h>
#include "config.h"
#include "system.h"
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "regs.h"
#include "insn-config.h"
#include "reload.h"
#include "output.h"
#include "expr.h"
#include "except.h"
#include "dwarf2.h"
#include "dwarf2out.h"
#include "toplev.h"
#include "dyn-string.h"

/* Decide whether we want to emit frame unwind information for the current
   translation unit.  */

int
dwarf2out_do_frame ()
{
  return (write_symbols == DWARF2_DEBUG);
}

#if defined (DWARF2_DEBUGGING_INFO)

#ifndef __GNUC__
#define inline
#endif

/* How to start an assembler comment.  */
#ifndef ASM_COMMENT_START
#define ASM_COMMENT_START ";#"
#endif

typedef struct dw_cfi_struct *dw_cfi_ref;
typedef struct dw_fde_struct *dw_fde_ref;
typedef union  dw_cfi_oprnd_struct *dw_cfi_oprnd_ref;

/* Call frames are described using a sequence of Call Frame
   Information instructions.  The register number, offset
   and address fields are provided as possible operands;
   their use is selected by the opcode field.  */

typedef union dw_cfi_oprnd_struct
{
  unsigned long dw_cfi_reg_num;
  long int dw_cfi_offset;
  char *dw_cfi_addr;
}
dw_cfi_oprnd;

typedef struct dw_cfi_struct
{
  dw_cfi_ref dw_cfi_next;
  enum dwarf_call_frame_info dw_cfi_opc;
  dw_cfi_oprnd dw_cfi_oprnd1;
  dw_cfi_oprnd dw_cfi_oprnd2;
}
dw_cfi_node;

/* All call frame descriptions (FDE's) in the GCC generated DWARF
   refer to a single Common Information Entry (CIE), defined at
   the beginning of the .debug_frame section.  This used of a single
   CIE obviates the need to keep track of multiple CIE's
   in the DWARF generation routines below.  */

typedef struct dw_fde_struct
{
  char *dw_fde_begin;
  char *dw_fde_current_label;
  char *dw_fde_end;
  dw_cfi_ref dw_fde_cfi;
}
dw_fde_node;

/* Maximum size (in bytes) of an artificially generated label.   */
#define MAX_ARTIFICIAL_LABEL_BYTES	30

/* Make sure we know the sizes of the various types dwarf can describe. These
   are only defaults.  If the sizes are different for your target, you should
   override these values by defining the appropriate symbols in your tm.h
   file.  */

#ifndef CHAR_TYPE_SIZE
#define CHAR_TYPE_SIZE BITS_PER_UNIT
#endif
#ifndef PTR_SIZE
#define PTR_SIZE (POINTER_SIZE / BITS_PER_UNIT)
#endif

/* The size in bytes of a DWARF field indicating an offset or length
   relative to a debug info section, specified to be 4 bytes in the DWARF-2
   specification.  */

#ifndef DWARF_OFFSET_SIZE
#define DWARF_OFFSET_SIZE 4
#endif

#define DWARF_VERSION 2

/* Round SIZE up to the nearest BOUNDARY.  */
#define DWARF_ROUND(SIZE,BOUNDARY) \
  (((SIZE) + (BOUNDARY) - 1) & ~((BOUNDARY) - 1))

/* Offsets recorded in opcodes are a multiple of this alignment factor.  */
#ifdef STACK_GROWS_DOWNWARD
#define DWARF_CIE_DATA_ALIGNMENT (-UNITS_PER_WORD)
#else
#define DWARF_CIE_DATA_ALIGNMENT UNITS_PER_WORD
#endif

/* A pointer to the base of a table that contains frame description
   information for each routine.  */
static dw_fde_ref fde_table;

/* Number of elements currently allocated for fde_table.  */
static unsigned fde_table_allocated;

/* Number of elements in fde_table currently in use.  */
static unsigned fde_table_in_use;

/* Size (in elements) of increments by which we may expand the
   fde_table.  */
#define FDE_TABLE_INCREMENT 256

/* The number of the current function definition for which debugging
   information is being generated.  These numbers range from 1 up to the
   maximum number of function definitions contained within the current
   compilation unit.  These numbers are used to create unique label id's
   unique to each function definition.  */
static unsigned current_funcdef_number = 0;

/* Some DWARF extensions (e.g., MIPS/SGI) implement a subprogram
   attribute that accelerates the lookup of the FDE associated
   with the subprogram.  This variable holds the table index of the FDE 
   associated with the current function (body) definition.  */
static unsigned current_funcdef_fde;

/* Forward declarations for functions defined in this file.  */

static char *stripattributes		(char *);
static unsigned long size_of_uleb128	(unsigned long);
static unsigned long size_of_sleb128	(long);
static void output_uleb128		(unsigned long);
static void output_sleb128		(long);
static unsigned reg_number		(rtx);

/* Definitions of defaults for assembler-dependent names of various
   pseudo-ops and section names.
   Theses may be overridden in the tm.h file (if necessary) for a particular
   assembler.  */

#ifdef OBJECT_FORMAT_ELF
#ifndef UNALIGNED_SHORT_ASM_OP
#define UNALIGNED_SHORT_ASM_OP	".2byte"
#endif
#ifndef UNALIGNED_INT_ASM_OP
#define UNALIGNED_INT_ASM_OP	".4byte"
#endif
#ifndef UNALIGNED_DOUBLE_INT_ASM_OP
#define UNALIGNED_DOUBLE_INT_ASM_OP	".8byte"
#endif
#endif /* OBJECT_FORMAT_ELF */

#ifndef ASM_BYTE_OP
#define ASM_BYTE_OP		".byte"
#endif

/* Data and reference forms for relocatable data.  */
#define DW_FORM_data (DWARF_OFFSET_SIZE == 8 ? DW_FORM_data8 : DW_FORM_data4)
#define DW_FORM_ref (DWARF_OFFSET_SIZE == 8 ? DW_FORM_ref8 : DW_FORM_ref4)

/* Pseudo-op for defining a new section.  */
#ifndef SECTION_ASM_OP
#define SECTION_ASM_OP	".section"
#endif

/* The default format used by the ASM_OUTPUT_SECTION macro (see below) to
   print the SECTION_ASM_OP and the section name.  The default here works for
   almost all svr4 assemblers, except for the sparc, where the section name
   must be enclosed in double quotes.  (See sparcv4.h).  */
#ifndef SECTION_FORMAT
#ifdef PUSHSECTION_FORMAT
#define SECTION_FORMAT PUSHSECTION_FORMAT
#else
#define SECTION_FORMAT		"\t%s\t%s\n"
#endif
#endif

#ifndef FUNC_BEGIN_LABEL
#define FUNC_BEGIN_LABEL	"LFB"
#endif
#ifndef FUNC_END_LABEL
#define FUNC_END_LABEL		"LFE"
#endif
#define CIE_AFTER_SIZE_LABEL	"LSCIE"
#define CIE_END_LABEL		"LECIE"
#define CIE_LENGTH_LABEL	"LLCIE"
#define FDE_AFTER_SIZE_LABEL	"LSFDE"
#define FDE_END_LABEL		"LEFDE"
#define FDE_LENGTH_LABEL	"LLFDE"
#define LINE_NUMBER_BEGIN_LABEL	"LSLT"
#define LINE_NUMBER_END_LABEL	"LELT"
#define LN_PROLOG_AS_LABEL	"LASLTP"
#define LN_PROLOG_END_LABEL	"LELTP"

/* Definitions of defaults for various types of primitive assembly language
   output operations.  These may be overridden from within the tm.h file,
   but typically, that is unnecessary.  */

#ifndef ASM_OUTPUT_SECTION
#define ASM_OUTPUT_SECTION(FILE, SECTION) \
  fprintf ((FILE), SECTION_FORMAT, SECTION_ASM_OP, SECTION)
#endif

#ifndef ASM_OUTPUT_DWARF_DATA1
#define ASM_OUTPUT_DWARF_DATA1(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t0x%x", ASM_BYTE_OP, (unsigned) (VALUE))
#endif

#ifndef ASM_OUTPUT_DWARF_DELTA1
#define ASM_OUTPUT_DWARF_DELTA1(FILE,LABEL1,LABEL2)			\
 do {	fprintf ((FILE), "\t%s\t", ASM_BYTE_OP);			\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, "-");						\
	assemble_name (FILE, LABEL2);					\
  } while (0)
#endif

#ifdef UNALIGNED_INT_ASM_OP

#ifndef UNALIGNED_OFFSET_ASM_OP
#define UNALIGNED_OFFSET_ASM_OP \
  (DWARF_OFFSET_SIZE == 8 ? UNALIGNED_DOUBLE_INT_ASM_OP : UNALIGNED_INT_ASM_OP)
#endif

#ifndef UNALIGNED_WORD_ASM_OP
#define UNALIGNED_WORD_ASM_OP \
  (PTR_SIZE == 8 ? UNALIGNED_DOUBLE_INT_ASM_OP : UNALIGNED_INT_ASM_OP)
#endif

#ifndef ASM_OUTPUT_DWARF_DELTA2
#define ASM_OUTPUT_DWARF_DELTA2(FILE,LABEL1,LABEL2)			\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_SHORT_ASM_OP);		\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, "-");						\
	assemble_name (FILE, LABEL2);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_DELTA4
#define ASM_OUTPUT_DWARF_DELTA4(FILE,LABEL1,LABEL2)			\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_INT_ASM_OP);		\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, "-");						\
	assemble_name (FILE, LABEL2);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_DELTA
#define ASM_OUTPUT_DWARF_DELTA(FILE,LABEL1,LABEL2)			\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_OFFSET_ASM_OP);		\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, "-");						\
	assemble_name (FILE, LABEL2);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_ADDR_DELTA
#define ASM_OUTPUT_DWARF_ADDR_DELTA(FILE,LABEL1,LABEL2)			\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_WORD_ASM_OP);		\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, "-");						\
	assemble_name (FILE, LABEL2);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_ADDR
#define ASM_OUTPUT_DWARF_ADDR(FILE,LABEL)				\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_WORD_ASM_OP);		\
	assemble_name (FILE, LABEL);					\
  } while (0)
#endif

/* ??? This macro takes an RTX in dwarfout.c and a string in dwarf2out.c.
   We resolve the conflict by creating a new macro ASM_OUTPUT_DWARF2_ADDR_CONST
   for ports that want to support both DWARF1 and DWARF2.  This needs a better
   solution.  See also the comments in sparc/sp64-elf.h.  */
#ifdef ASM_OUTPUT_DWARF2_ADDR_CONST
#undef ASM_OUTPUT_DWARF_ADDR_CONST
#define ASM_OUTPUT_DWARF_ADDR_CONST(FILE,ADDR) \
  ASM_OUTPUT_DWARF2_ADDR_CONST (FILE, ADDR)
#endif

#ifndef ASM_OUTPUT_DWARF_ADDR_CONST
#define ASM_OUTPUT_DWARF_ADDR_CONST(FILE,ADDR)				\
  fprintf ((FILE), "\t%s\t%s", UNALIGNED_WORD_ASM_OP, (ADDR))
#endif

#ifndef ASM_OUTPUT_DWARF_OFFSET4
#define ASM_OUTPUT_DWARF_OFFSET4(FILE,LABEL) \
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_INT_ASM_OP);		\
	assemble_name (FILE, LABEL);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_OFFSET
#define ASM_OUTPUT_DWARF_OFFSET(FILE,LABEL)				\
 do {	fprintf ((FILE), "\t%s\t", UNALIGNED_OFFSET_ASM_OP);		\
	assemble_name (FILE, LABEL);					\
  } while (0)
#endif

#ifndef ASM_OUTPUT_DWARF_DATA2
#define ASM_OUTPUT_DWARF_DATA2(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t0x%x", UNALIGNED_SHORT_ASM_OP, (unsigned) (VALUE))
#endif

#ifndef ASM_OUTPUT_DWARF_DATA4
#define ASM_OUTPUT_DWARF_DATA4(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t0x%x", UNALIGNED_INT_ASM_OP, (unsigned) (VALUE))
#endif

#ifndef ASM_OUTPUT_DWARF_DATA
#define ASM_OUTPUT_DWARF_DATA(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t0x%lx", UNALIGNED_OFFSET_ASM_OP, \
	   (unsigned long) (VALUE))
#endif

#ifndef ASM_OUTPUT_DWARF_ADDR_DATA
#define ASM_OUTPUT_DWARF_ADDR_DATA(FILE,VALUE) \
  fprintf ((FILE), "\t%s\t0x%lx", UNALIGNED_WORD_ASM_OP, \
	   (unsigned long) (VALUE))
#endif

#ifndef ASM_OUTPUT_DWARF_DATA8
#define ASM_OUTPUT_DWARF_DATA8(FILE,HIGH_VALUE,LOW_VALUE)		\
  do {									\
	fprintf ((FILE), "\t%s\t0x%lx\n", UNALIGNED_INT_ASM_OP, (LOW_VALUE)); \
	fprintf ((FILE), "\t%s\t0x%lx", UNALIGNED_INT_ASM_OP, (HIGH_VALUE)); \
  } while (0)
#endif

#else /* UNALIGNED_INT_ASM_OP */

/* We don't have unaligned support, let's hope the normal output works for
   .debug_frame.  */

#define ASM_OUTPUT_DWARF_ADDR(FILE,LABEL) \
  assemble_integer (gen_rtx_SYMBOL_REF (Pmode, LABEL), PTR_SIZE, 1)

#define ASM_OUTPUT_DWARF_OFFSET4(FILE,LABEL) \
  assemble_integer (gen_rtx_SYMBOL_REF (SImode, LABEL), 4, 1)

#define ASM_OUTPUT_DWARF_OFFSET(FILE,LABEL) \
  assemble_integer (gen_rtx_SYMBOL_REF (SImode, LABEL), 4, 1)

#define ASM_OUTPUT_DWARF_DELTA2(FILE,LABEL1,LABEL2)			\
  assemble_integer (gen_rtx_MINUS (HImode,			      	\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL1),   	\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL2)),	\
		    2, 1)
  
#define ASM_OUTPUT_DWARF_DELTA4(FILE,LABEL1,LABEL2)			\
  assemble_integer (gen_rtx_MINUS (SImode,			      	\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL1),   	\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL2)),	\
		    4, 1)

#define ASM_OUTPUT_DWARF_ADDR_DELTA(FILE,LABEL1,LABEL2)			\
  assemble_integer (gen_rtx_MINUS (Pmode,				\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL1),	\
			     gen_rtx_SYMBOL_REF (Pmode, LABEL2)),	\
		    PTR_SIZE, 1)

#define ASM_OUTPUT_DWARF_DELTA(FILE,LABEL1,LABEL2) \
  ASM_OUTPUT_DWARF_DELTA4 (FILE,LABEL1,LABEL2)

#define ASM_OUTPUT_DWARF_DATA4(FILE,VALUE) \
  assemble_integer (GEN_INT (VALUE), 4, 1)

#endif /* UNALIGNED_INT_ASM_OP */

/* This is similar to the default ASM_OUTPUT_ASCII, except that no trailing
   newline is produced.  When flag_debug_asm is asserted, we add commentary
   at the end of the line, so we must avoid output of a newline here.  */
#ifndef ASM_OUTPUT_DWARF_STRING
#define ASM_OUTPUT_DWARF_STRING(FILE,P) \
  do {									      \
    register int slen = strlen(P);                                            \
    register char *p = (P);  	                                              \
    register int i;					                      \
    fprintf (FILE, "\t.ascii \"");				              \
    for (i = 0; i < slen; i++)					              \
      {								              \
	  register int c = p[i];					      \
	  if (c == '\"' || c == '\\')					      \
	    putc ('\\', FILE);					              \
	  if (c >= ' ' && c < 0177)					      \
	    putc (c, FILE);					              \
	  else								      \
	    {								      \
	      fprintf (FILE, "\\%o", c);			              \
	    }							 	      \
      }								              \
    fprintf (FILE, "\\0\"");					              \
  }									      \
  while (0)
#endif

/* Return a pointer to a copy of the section string name S with all
   attributes stripped off, and an asterisk prepended (for assemble_name).  */

static inline char *
stripattributes (s)
     char *s;
{
  char *stripped = xmalloc (strlen (s) + 2);
  char *p = stripped;

  *p++ = '*';

  while (*s && *s != ',')
    *p++ = *s++;

  *p = '\0';
  return stripped;
}

/* Return the register number described by a given RTL node.  */

static unsigned
reg_number (rtl)
     register rtx rtl;
{
  register unsigned regno = REGNO (rtl);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      warning ("internal regno botch: regno = %d\n", regno);
      regno = 0;
    }

  regno = DBX_REGISTER_NUMBER (regno);
  return regno;
}

/* The running total of the size of arguments pushed onto the stack.  */
static long args_size;

/* The last args_size we actually output.  */
static long old_args_size;

/* Return the size of an unsigned LEB128 quantity.  */

static inline unsigned long
size_of_uleb128 (value)
     register unsigned long value;
{
  register unsigned long size = 0;

  do
    {
      value >>= 7;
      size += 1;
    }
  while (value != 0);

  return size;
}

/* Return the size of a signed LEB128 quantity.  */

static inline unsigned long
size_of_sleb128 (value)
     register long value;
{
  register unsigned long size = 0;
  register unsigned byte;

  do
    {
      byte = (value & 0x7f);
      value >>= 7;
      size += 1;
    }
  while (!(((value == 0) && ((byte & 0x40) == 0))
	   || ((value == -1) && ((byte & 0x40) != 0))));

  return size;
}

/* Output an unsigned LEB128 quantity.  */

static void
output_uleb128 (value)
     register unsigned long value;
{
  unsigned long save_value = value;

  fprintf (asm_out_file, "\t%s\t", ASM_BYTE_OP);
  do
    {
      register unsigned byte = (value & 0x7f);
      value >>= 7;
      if (value != 0)
	/* More bytes to follow.  */
	byte |= 0x80;

      fprintf (asm_out_file, "0x%x", byte);
      if (value != 0)
	fprintf (asm_out_file, ",");
    }
  while (value != 0);

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s ULEB128 0x%lx", ASM_COMMENT_START, save_value);
}

/* Output an signed LEB128 quantity.  */

static void
output_sleb128 (value)
     register long value;
{
  register int more;
  register unsigned byte;
  long save_value = value;

  fprintf (asm_out_file, "\t%s\t", ASM_BYTE_OP);
  do
    {
      byte = (value & 0x7f);
      /* arithmetic shift */
      value >>= 7;
      more = !((((value == 0) && ((byte & 0x40) == 0))
		|| ((value == -1) && ((byte & 0x40) != 0))));
      if (more)
	byte |= 0x80;

      fprintf (asm_out_file, "0x%x", byte);
      if (more)
	fprintf (asm_out_file, ",");
    }

  while (more);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s SLEB128 %ld", ASM_COMMENT_START, save_value);
}

/* Output a marker (i.e. a label) for the beginning of a function, before
   the prologue.  */

void
dwarf2out_begin_prologue ()
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];
  register dw_fde_ref fde;

  ++current_funcdef_number;

  function_section (current_function_decl);
  ASM_GENERATE_INTERNAL_LABEL (label, FUNC_BEGIN_LABEL,
			       current_funcdef_number);
  ASM_OUTPUT_LABEL (asm_out_file, label);

  /* Expand the fde table if necessary.  */
  if (fde_table_in_use == fde_table_allocated)
    {
      fde_table_allocated += FDE_TABLE_INCREMENT;
      fde_table
	= (dw_fde_ref) xrealloc (fde_table,
				 fde_table_allocated * sizeof (dw_fde_node));
    }

  /* Record the FDE associated with this function.  */
  current_funcdef_fde = fde_table_in_use;

  /* Add the new FDE at the end of the fde_table.  */
  fde = &fde_table[fde_table_in_use++];
  fde->dw_fde_begin = xstrdup (label);
  fde->dw_fde_current_label = NULL;
  fde->dw_fde_end = NULL;
  fde->dw_fde_cfi = NULL;

  args_size = old_args_size = 0;
}

/* Output a marker (i.e. a label) for the absolute end of the generated code
   for a function definition.  This gets called *after* the epilogue code has
   been generated.  */

void
dwarf2out_end_epilogue ()
{
  dw_fde_ref fde;
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  /* Output a label to mark the endpoint of the code generated for this
     function.        */
  ASM_GENERATE_INTERNAL_LABEL (label, FUNC_END_LABEL, current_funcdef_number);
  ASM_OUTPUT_LABEL (asm_out_file, label);
  fde = &fde_table[fde_table_in_use - 1];
  fde->dw_fde_end = xstrdup (label);
}

#endif /* .debug_frame support */

/* And now, the support for symbolic debugging information.  */
#ifdef DWARF2_DEBUGGING_INFO

extern char *getpwd (void);

/* NOTE: In the comments in this file, many references are made to
   "Debugging Information Entries".  This term is abbreviated as `DIE'
   throughout the remainder of this file.  */

/* An internal representation of the DWARF output is built, and then
   walked to generate the DWARF debugging info.  The walk of the internal
   representation is done after the entire program has been compiled.
   The types below are used to describe the internal representation.  */

/* Each DIE may have a series of attribute/value pairs.  Values
   can take on several forms.  The forms that are used in this
   implementation are listed below.  */

typedef enum
{
  dw_val_class_addr,
  dw_val_class_loc,
  dw_val_class_const,
  dw_val_class_unsigned_const,
  dw_val_class_long_long,
  dw_val_class_float,
  dw_val_class_flag,
  dw_val_class_die_ref,
  dw_val_class_fde_ref,
  dw_val_class_lbl_id,
  dw_val_class_section_offset,
  dw_val_class_str
}
dw_val_class;

/* Various DIE's use offsets relative to the beginning of the
   .debug_info section to refer to each other.  */

typedef long int dw_offset;

/* Define typedefs here to avoid circular dependencies.  */

typedef struct die_struct *dw_die_ref;
typedef struct dw_attr_struct *dw_attr_ref;
typedef struct dw_val_struct *dw_val_ref;
typedef struct dw_line_info_struct *dw_line_info_ref;
typedef struct dw_separate_line_info_struct *dw_separate_line_info_ref;
typedef struct dw_loc_descr_struct *dw_loc_descr_ref;
typedef struct pubname_struct *pubname_ref;
typedef dw_die_ref *arange_ref;

/* Describe a double word constant value.  */

typedef struct dw_long_long_struct
{
  unsigned long hi;
  unsigned long low;
}
dw_long_long_const;

/* Describe a floating point constant value.  */

typedef struct dw_fp_struct
{
  long *array;
  unsigned length;
}
dw_float_const;

/* Each entry in the line_info_table maintains the file and
   line number associated with the label generated for that
   entry.  The label gives the PC value associated with
   the line number entry.  */

typedef struct dw_line_info_struct
{
  unsigned long dw_file_num;
  unsigned long dw_line_num;
}
dw_line_info_entry;

/* Line information for functions in separate sections; each one gets its
   own sequence.  */
typedef struct dw_separate_line_info_struct
{
  unsigned long dw_file_num;
  unsigned long dw_line_num;
  unsigned long function;
}
dw_separate_line_info_entry;

/* The dw_val_node describes an attribute's value, as it is
   represented internally.  */

typedef struct dw_val_struct
{
  dw_val_class val_class;
  union
    {
      char *val_addr;
      dw_loc_descr_ref val_loc;
      long int val_int;
      long unsigned val_unsigned;
      dw_long_long_const val_long_long;
      dw_float_const val_float;
      dw_die_ref val_die_ref;
      unsigned val_fde_index;
      char *val_str;
      char *val_lbl_id;
      char *val_section;
      unsigned char val_flag;
    }
  v;
}
dw_val_node;

/* Locations in memory are described using a sequence of stack machine
   operations.  */

typedef struct dw_loc_descr_struct
{
  dw_loc_descr_ref dw_loc_next;
  enum dwarf_location_atom dw_loc_opc;
  dw_val_node dw_loc_oprnd1;
  dw_val_node dw_loc_oprnd2;
}
dw_loc_descr_node;

/* Each DIE attribute has a field specifying the attribute kind,
   a link to the next attribute in the chain, and an attribute value.
   Attributes are typically linked below the DIE they modify.  */

typedef struct dw_attr_struct
{
  enum dwarf_attribute dw_attr;
  dw_attr_ref dw_attr_next;
  dw_val_node dw_attr_val;
}
dw_attr_node;

/* The Debugging Information Entry (DIE) structure */

typedef struct die_struct
{
  enum dwarf_tag die_tag;
  dw_attr_ref die_attr;
  dw_attr_ref die_attr_last;
  dw_die_ref die_parent;
  dw_die_ref die_child;
  dw_die_ref die_child_last;
  dw_die_ref die_sib;
  dw_offset die_offset;
  unsigned long die_abbrev;
}
die_node;

/* The pubname structure */

typedef struct pubname_struct
{
  dw_die_ref die;
  char * name;
}
pubname_entry;

/* The limbo die list structure.  */
typedef struct limbo_die_struct
{
  dw_die_ref die;
  struct limbo_die_struct *next;
}
limbo_die_node;

/* Define a macro which returns non-zero for a TYPE_DECL which was
   implicitly generated for a tagged type.

   Note that unlike the gcc front end (which generates a NULL named
   TYPE_DECL node for each complete tagged type, each array type, and
   each function type node created) the g++ front end generates a
   _named_ TYPE_DECL node for each tagged type node created.
   These TYPE_DECLs have DECL_ARTIFICIAL set, so we know not to
   generate a DW_TAG_typedef DIE for them.  */

#define TYPE_DECL_IS_STUB(decl)				\
  (DECL_NAME (decl) == NULL_TREE			\
   || (DECL_ARTIFICIAL (decl)				\
       && is_tagged_type (TREE_TYPE (decl))		\
       && ((decl == TYPE_STUB_DECL (TREE_TYPE (decl)))	\
	   /* This is necessary for stub decls that	\
	      appear in nested inline functions.  */	\
	   || (DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE	\
	       && (decl_ultimate_origin (decl)		\
		   == TYPE_STUB_DECL (TREE_TYPE (decl)))))))

/* Information concerning the compilation unit's programming
   language, and compiler version.  */

extern int flag_traditional;
extern char *version_string;
extern char *language_string;

/* Fixed size portion of the DWARF compilation unit header.  */
#define DWARF_COMPILE_UNIT_HEADER_SIZE (2 * DWARF_OFFSET_SIZE + 3)

/* Fixed size portion of debugging line information prolog.  */
#define DWARF_LINE_PROLOG_HEADER_SIZE 5

/* Fixed size portion of public names info.  */
#define DWARF_PUBNAMES_HEADER_SIZE (2 * DWARF_OFFSET_SIZE + 2)

/* Fixed size portion of the address range info.  */
#define DWARF_ARANGES_HEADER_SIZE \
  (DWARF_ROUND (2 * DWARF_OFFSET_SIZE + 4, PTR_SIZE * 2) - DWARF_OFFSET_SIZE)

/* Define the architecture-dependent minimum instruction length (in bytes).
   In this implementation of DWARF, this field is used for information
   purposes only.  Since GCC generates assembly language, we have
   no a priori knowledge of how many instruction bytes are generated
   for each source line, and therefore can use only the  DW_LNE_set_address
   and DW_LNS_fixed_advance_pc line information commands.  */

#ifndef DWARF_LINE_MIN_INSTR_LENGTH
#define DWARF_LINE_MIN_INSTR_LENGTH 4
#endif

/* Minimum line offset in a special line info. opcode.
   This value was chosen to give a reasonable range of values.  */
#define DWARF_LINE_BASE  -10

/* First special line opcde - leave room for the standard opcodes.  */
#define DWARF_LINE_OPCODE_BASE  10

/* Range of line offsets in a special line info. opcode.  */
#define DWARF_LINE_RANGE  (254-DWARF_LINE_OPCODE_BASE+1)

/* Flag that indicates the initial value of the is_stmt_start flag.
   In the present implementation, we do not mark any lines as
   the beginning of a source statement, because that information
   is not made available by the GCC front-end.  */
#define	DWARF_LINE_DEFAULT_IS_STMT_START 1

/* This location is used by calc_die_sizes() to keep track
   the offset of each DIE within the .debug_info section.  */
static unsigned long next_die_offset;

/* Record the root of the DIE's built for the current compilation unit.  */
static dw_die_ref comp_unit_die;

/* A list of DIEs with a NULL parent waiting to be relocated.  */
static limbo_die_node *limbo_die_list = 0;

/* Pointer to an array of filenames referenced by this compilation unit.  */
static char **file_table;

/* Total number of entries in the table (i.e. array) pointed to by
   `file_table'.  This is the *total* and includes both used and unused
   slots.  */
static unsigned file_table_allocated;

/* Number of entries in the file_table which are actually in use.  */
static unsigned file_table_in_use;

/* Size (in elements) of increments by which we may expand the filename
   table.  */
#define FILE_TABLE_INCREMENT 64

/* Local pointer to the name of the main input file.  Initialized in
   dwarf2out_init.  */
static char *primary_filename;

/* For Dwarf output, we must assign lexical-blocks id numbers in the order in
   which their beginnings are encountered. We output Dwarf debugging info
   that refers to the beginnings and ends of the ranges of code for each
   lexical block.  The labels themselves are generated in final.c, which
   assigns numbers to the blocks in the same way.  */
static unsigned next_block_number = 2;

/* A pointer to the base of a table of references to DIE's that describe
   declarations.  The table is indexed by DECL_UID() which is a unique
   number identifying each decl.  */
static dw_die_ref *decl_die_table;

/* Number of elements currently allocated for the decl_die_table.  */
static unsigned decl_die_table_allocated;

/* Number of elements in decl_die_table currently in use.  */
static unsigned decl_die_table_in_use;

/* Size (in elements) of increments by which we may expand the
   decl_die_table.  */
#define DECL_DIE_TABLE_INCREMENT 256

/* Structure used for the decl_scope table.  scope is the current declaration
   scope, and previous is the entry that is the parent of this scope.  This
   is usually but not always the immediately preceeding entry.  */

typedef struct decl_scope_struct
{
  tree scope;
  int previous;
}
decl_scope_node;

/* A pointer to the base of a table of references to declaration
   scopes.  This table is a display which tracks the nesting
   of declaration scopes at the current scope and containing
   scopes.  This table is used to find the proper place to
   define type declaration DIE's.  */
static decl_scope_node *decl_scope_table;

/* Number of elements currently allocated for the decl_scope_table.  */
static int decl_scope_table_allocated;

/* Current level of nesting of declaration scopes.  */
static int decl_scope_depth;

/* Size (in elements) of increments by which we may expand the
   decl_scope_table.  */
#define DECL_SCOPE_TABLE_INCREMENT 64

/* A pointer to the base of a list of references to DIE's that
   are uniquely identified by their tag, presence/absence of
   children DIE's, and list of attribute/value pairs.  */
static dw_die_ref *abbrev_die_table;

/* Number of elements currently allocated for abbrev_die_table.  */
static unsigned abbrev_die_table_allocated;

/* Number of elements in type_die_table currently in use.  */
static unsigned abbrev_die_table_in_use;

/* Size (in elements) of increments by which we may expand the
   abbrev_die_table.  */
#define ABBREV_DIE_TABLE_INCREMENT 256

/* A pointer to the base of a table that contains line information
   for each source code line in .text in the compilation unit.  */
static dw_line_info_ref line_info_table;

/* Number of elements currently allocated for line_info_table.  */
static unsigned line_info_table_allocated;

/* Number of elements in separate_line_info_table currently in use.  */
static unsigned separate_line_info_table_in_use;

/* A pointer to the base of a table that contains line information
   for each source code line outside of .text in the compilation unit.  */
static dw_separate_line_info_ref separate_line_info_table;

/* Number of elements currently allocated for separate_line_info_table.  */
static unsigned separate_line_info_table_allocated;

/* Number of elements in line_info_table currently in use.  */
static unsigned line_info_table_in_use;

/* Size (in elements) of increments by which we may expand the
   line_info_table.  */
#define LINE_INFO_TABLE_INCREMENT 1024

/* A pointer to the base of a table that contains a list of publicly
   accessible names.  */
static pubname_ref pubname_table;

/* Number of elements currently allocated for pubname_table.  */
static unsigned pubname_table_allocated;

/* Number of elements in pubname_table currently in use.  */
static unsigned pubname_table_in_use;

/* Size (in elements) of increments by which we may expand the
   pubname_table.  */
#define PUBNAME_TABLE_INCREMENT 64

/* A pointer to the base of a table that contains a list of publicly
   accessible names.  */
static arange_ref arange_table;

/* Number of elements currently allocated for arange_table.  */
static unsigned arange_table_allocated;

/* Number of elements in arange_table currently in use.  */
static unsigned arange_table_in_use;

/* Size (in elements) of increments by which we may expand the
   arange_table.  */
#define ARANGE_TABLE_INCREMENT 64

/* A pointer to the base of a list of pending types which we haven't
   generated DIEs for yet, but which we will have to come back to
   later on.  */

static tree *pending_types_list;

/* Number of elements currently allocated for the pending_types_list.  */
static unsigned pending_types_allocated;

/* Number of elements of pending_types_list currently in use.  */
static unsigned pending_types;

/* Size (in elements) of increments by which we may expand the pending
   types list.  Actually, a single hunk of space of this size should
   be enough for most typical programs.	 */
#define PENDING_TYPES_INCREMENT 64

/* Record whether the function being analyzed contains inlined functions.  */
static int current_function_has_inlines;

/* A pointer to the ..._DECL node which we have most recently been working
   on.  We keep this around just in case something about it looks screwy and
   we want to tell the user what the source coordinates for the actual
   declaration are.  */
static tree dwarf_last_decl;

/* Forward declarations for functions defined in this file.  */

static void addr_const_to_string	(dyn_string_t, rtx);
static char *addr_to_string		(rtx);
static int is_pseudo_reg		(rtx);
static tree type_main_variant		(tree);
static int is_tagged_type		(tree);
static char *dwarf_tag_name		(unsigned);
static char *dwarf_attr_name		(unsigned);
static char *dwarf_form_name		(unsigned);
static char *dwarf_stack_op_name	(unsigned);
#if 0
static char *dwarf_type_encoding_name	(unsigned);
#endif
static tree decl_ultimate_origin	(tree);
static tree block_ultimate_origin	(tree);
static tree decl_class_context		(tree);
static void add_dwarf_attr		(dw_die_ref, dw_attr_ref);
static void add_AT_flag			(dw_die_ref,
					       enum dwarf_attribute,
					       unsigned);
static void add_AT_int			(dw_die_ref,
					       enum dwarf_attribute, long);
static void add_AT_unsigned		(dw_die_ref,
					       enum dwarf_attribute,
					       unsigned long);
static void add_AT_long_long		(dw_die_ref,
					       enum dwarf_attribute,
					       unsigned long, unsigned long);
static void add_AT_float		(dw_die_ref,
					       enum dwarf_attribute,
					       unsigned, long *);
static void add_AT_string		(dw_die_ref,
					       enum dwarf_attribute, char *);
static void add_AT_die_ref		(dw_die_ref,
					       enum dwarf_attribute,
					       dw_die_ref);
static void add_AT_fde_ref		(dw_die_ref,
					       enum dwarf_attribute,
					       unsigned);
static void add_AT_loc			(dw_die_ref,
					       enum dwarf_attribute,
					       dw_loc_descr_ref);
static void add_AT_addr			(dw_die_ref,
					       enum dwarf_attribute, char *);
static void add_AT_lbl_id		(dw_die_ref,
					       enum dwarf_attribute, char *);
static void add_AT_section_offset	(dw_die_ref,
					       enum dwarf_attribute, char *);
static int is_extern_subr_die		(dw_die_ref);
static dw_attr_ref get_AT		(dw_die_ref,
					       enum dwarf_attribute);
static char *get_AT_low_pc		(dw_die_ref);
static char *get_AT_hi_pc		(dw_die_ref);
static char *get_AT_string		(dw_die_ref,
					       enum dwarf_attribute);
static int get_AT_flag			(dw_die_ref,
					       enum dwarf_attribute);
static unsigned get_AT_unsigned		(dw_die_ref,
					       enum dwarf_attribute);
static int is_c_family			(void);
static int is_fortran			(void);
static void remove_AT			(dw_die_ref,
					       enum dwarf_attribute);
static void remove_children		(dw_die_ref);
static void add_child_die		(dw_die_ref, dw_die_ref);
static dw_die_ref new_die		(enum dwarf_tag, dw_die_ref);
static dw_die_ref lookup_type_die	(tree);
static void equate_type_number_to_die	(tree, dw_die_ref);
static dw_die_ref lookup_decl_die	(tree);
static void equate_decl_number_to_die	(tree, dw_die_ref);
static dw_loc_descr_ref new_loc_descr	(enum dwarf_location_atom,
					       unsigned long, unsigned long);
static void add_loc_descr		(dw_loc_descr_ref *,
					       dw_loc_descr_ref);
static void print_spaces		(FILE *);
static void print_die			(dw_die_ref, FILE *);
static void print_dwarf_line_table	(FILE *);
static void add_sibling_attributes	(dw_die_ref);
static void build_abbrev_table		(dw_die_ref);
static unsigned long size_of_string	(char *);
static unsigned long size_of_loc_descr	(dw_loc_descr_ref);
static unsigned long size_of_locs	(dw_loc_descr_ref);
static int constant_size		(long unsigned);
static unsigned long size_of_die	(dw_die_ref);
static void calc_die_sizes		(dw_die_ref);
static unsigned long size_of_line_prolog	(void);
static unsigned long size_of_line_info	(void);
static unsigned long size_of_pubnames	(void);
static unsigned long size_of_aranges	(void);
static enum dwarf_form value_format	(dw_val_ref);
static void output_value_format		(dw_val_ref);
static void output_abbrev_section	(void);
static void output_loc_operands		(dw_loc_descr_ref);
static unsigned long sibling_offset	(dw_die_ref);
static void output_die			(dw_die_ref);
static void output_compilation_unit_header (void);
static char *dwarf2_name		(tree, int);
static void add_pubname			(tree, dw_die_ref);
static void output_pubnames		(void);
static void add_arange			(tree, dw_die_ref);
static void output_aranges		(void);
static void output_line_info		(void);
static int is_body_block		(tree);
static dw_die_ref base_type_die		(tree);
static int is_base_type			(tree);
static dw_die_ref modified_type_die	(tree, int, int, dw_die_ref);
static int type_is_enum			(tree);
static dw_loc_descr_ref reg_loc_descriptor (rtx);
static dw_loc_descr_ref based_loc_descr	(unsigned, long);
static int is_based_loc			(rtx);
static dw_loc_descr_ref mem_loc_descriptor (rtx);
static dw_loc_descr_ref concat_loc_descriptor (rtx, rtx);
static dw_loc_descr_ref loc_descriptor	(rtx);
static unsigned ceiling			(unsigned, unsigned);
static tree field_type			(tree);
static unsigned simple_type_align_in_bits (tree);
static unsigned simple_type_size_in_bits (tree);
static unsigned field_byte_offset		(tree);
static void add_AT_location_description	(dw_die_ref,
					       enum dwarf_attribute, rtx);
static void add_data_member_location_attribute (dw_die_ref, tree);
static void add_const_value_attribute	(dw_die_ref, rtx);
static void add_location_or_const_value_attribute (dw_die_ref, tree);
static void add_name_attribute		(dw_die_ref, char *);
static void add_bound_info		(dw_die_ref,
					       enum dwarf_attribute, tree);
static void add_subscript_info		(dw_die_ref, tree);
static void add_byte_size_attribute	(dw_die_ref, tree);
static void add_bit_offset_attribute	(dw_die_ref, tree);
static void add_bit_size_attribute	(dw_die_ref, tree);
static void add_prototyped_attribute	(dw_die_ref, tree);
static void add_abstract_origin_attribute (dw_die_ref, tree);
static void add_pure_or_virtual_attribute (dw_die_ref, tree);
static void add_src_coords_attributes	(dw_die_ref, tree);
static void add_name_and_src_coords_attributes (dw_die_ref, tree);
static void push_decl_scope		(tree);
static dw_die_ref scope_die_for		(tree, dw_die_ref);
static void pop_decl_scope		(void);
static void add_type_attribute		(dw_die_ref, tree, int, int,
					       dw_die_ref);
static char *type_tag			(tree);
static tree member_declared_type	(tree);
#if 0
static char *decl_start_label		(tree);
#endif
static void gen_array_type_die		(tree, dw_die_ref);
static void gen_set_type_die		(tree, dw_die_ref);
#if 0
static void gen_entry_point_die		(tree, dw_die_ref);
#endif
static void pend_type			(tree);
static void output_pending_types_for_scope (dw_die_ref);
static void gen_inlined_enumeration_type_die (tree, dw_die_ref);
static void gen_inlined_structure_type_die (tree, dw_die_ref);
static void gen_inlined_union_type_die	(tree, dw_die_ref);
static void gen_enumeration_type_die	(tree, dw_die_ref);
static dw_die_ref gen_formal_parameter_die (tree, dw_die_ref);
static void gen_unspecified_parameters_die (tree, dw_die_ref);
static void gen_formal_types_die	(tree, dw_die_ref);
static void gen_subprogram_die		(tree, dw_die_ref);
static void gen_variable_die		(tree, dw_die_ref);
static void gen_label_die		(tree, dw_die_ref);
static void gen_lexical_block_die	(tree, dw_die_ref, int);
static void gen_inlined_subroutine_die	(tree, dw_die_ref, int);
static void gen_field_die		(tree, dw_die_ref);
static void gen_ptr_to_mbr_type_die	(tree, dw_die_ref);
static void gen_compile_unit_die	(char *);
static void gen_string_type_die		(tree, dw_die_ref);
static void gen_inheritance_die		(tree, dw_die_ref);
static void gen_member_die		(tree, dw_die_ref);
static void gen_struct_or_union_type_die (tree, dw_die_ref);
static void gen_subroutine_type_die	(tree, dw_die_ref);
static void gen_typedef_die		(tree, dw_die_ref);
static void gen_type_die		(tree, dw_die_ref);
static void gen_tagged_type_instantiation_die (tree, dw_die_ref);
static void gen_block_die		(tree, dw_die_ref, int);
static void decls_for_scope		(tree, dw_die_ref, int);
static int is_redundant_typedef		(tree);
static void gen_decl_die		(tree, dw_die_ref);
static unsigned lookup_filename		(char *);

/* Section names used to hold DWARF debugging information.  */
#ifndef DEBUG_INFO_SECTION
#define DEBUG_INFO_SECTION	".debug_info"
#endif
#ifndef ABBREV_SECTION
#define ABBREV_SECTION		".debug_abbrev"
#endif
#ifndef ARANGES_SECTION
#define ARANGES_SECTION		".debug_aranges"
#endif
#ifndef DW_MACINFO_SECTION
#define DW_MACINFO_SECTION	".debug_macinfo"
#endif
#ifndef DEBUG_LINE_SECTION
#define DEBUG_LINE_SECTION	".debug_line"
#endif
#ifndef LOC_SECTION
#define LOC_SECTION		".debug_loc"
#endif
#ifndef PUBNAMES_SECTION
#define PUBNAMES_SECTION	".debug_pubnames"
#endif
#ifndef STR_SECTION
#define STR_SECTION		".debug_str"
#endif

/* Standard ELF section names for compiled code and data.  */
#ifndef TEXT_SECTION
#define TEXT_SECTION		".text"
#endif
#ifndef DATA_SECTION
#define DATA_SECTION		".data"
#endif
#ifndef BSS_SECTION
#define BSS_SECTION		".bss"
#endif


/* Definitions of defaults for formats and names of various special
   (artificial) labels which may be generated within this file (when the -g
   options is used and DWARF_DEBUGGING_INFO is in effect.
   If necessary, these may be overridden from within the tm.h file, but
   typically, overriding these defaults is unnecessary.  */

static char text_end_label[MAX_ARTIFICIAL_LABEL_BYTES];

#ifndef TEXT_END_LABEL
#define TEXT_END_LABEL		"Letext"
#endif
#ifndef DATA_END_LABEL
#define DATA_END_LABEL		"Ledata"
#endif
#ifndef BSS_END_LABEL
#define BSS_END_LABEL           "Lebss"
#endif
#ifndef INSN_LABEL_FMT
#define INSN_LABEL_FMT		"LI%u_"
#endif
#ifndef BLOCK_BEGIN_LABEL
#define BLOCK_BEGIN_LABEL	"LBB"
#endif
#ifndef BLOCK_END_LABEL
#define BLOCK_END_LABEL		"LBE"
#endif
#ifndef BODY_BEGIN_LABEL
#define BODY_BEGIN_LABEL	"Lbb"
#endif
#ifndef BODY_END_LABEL
#define BODY_END_LABEL		"Lbe"
#endif
#ifndef LINE_CODE_LABEL
#define LINE_CODE_LABEL		"LM"
#endif
#ifndef SEPARATE_LINE_CODE_LABEL
#define SEPARATE_LINE_CODE_LABEL	"LSM"
#endif

/* Convert a reference to the assembler name of a C-level name.  This
   macro has the same effect as ASM_OUTPUT_LABELREF, but copies to
   a string rather than writing to a file.  */
#ifndef ASM_NAME_TO_STRING
#define ASM_NAME_TO_STRING(STR, NAME)			\
  do {							\
      if ((NAME)[0] == '*')				\
	dyn_string_append (STR, NAME + 1);		\
      else						\
	{						\
	  dyn_string_append (STR, NAME);		\
	}						\
  }							\
  while (0)
#endif

/* Convert an integer constant expression into assembler syntax.  Addition
   and subtraction are the only arithmetic that may appear in these
   expressions.   This is an adaptation of output_addr_const in final.c.
   Here, the target of the conversion is a string buffer.  We can't use
   output_addr_const directly, because it writes to a file.  */

static void
addr_const_to_string (str, x)
     dyn_string_t str;
     rtx x;
{
  char buf1[256];

restart:
  switch (GET_CODE (x))
    {
    case PC:
      abort ();
      break;

    case SYMBOL_REF:
      ASM_NAME_TO_STRING (str, XSTR (x, 0));
      break;

    case LABEL_REF:
      ASM_GENERATE_INTERNAL_LABEL (buf1, "L", CODE_LABEL_NUMBER (XEXP (x, 0)));
      ASM_NAME_TO_STRING (str, buf1);
      break;

    case CODE_LABEL:
      ASM_GENERATE_INTERNAL_LABEL (buf1, "L", CODE_LABEL_NUMBER (x));
      ASM_NAME_TO_STRING (str, buf1);
      break;

    case CONST_INT:
      sprintf (buf1, HOST_WIDE_INT_PRINT_DEC, INTVAL (x));
      dyn_string_append (str, buf1);
      break;

    case CONST:
      /* This used to output parentheses around the expression, but that does 
         not work on the 386 (either ATT or BSD assembler).  */
      addr_const_to_string (str, XEXP (x, 0));
      break;

    case CONST_DOUBLE:
      if (GET_MODE (x) == VOIDmode)
	{
	  /* We can use %d if the number is one word and positive.  */
	  if (CONST_DOUBLE_HIGH (x))
	    sprintf (buf1, HOST_WIDE_INT_PRINT_DOUBLE_HEX,
		     CONST_DOUBLE_HIGH (x), CONST_DOUBLE_LOW (x));
	  else if (CONST_DOUBLE_LOW (x) < 0)
	    sprintf (buf1, HOST_WIDE_INT_PRINT_HEX, CONST_DOUBLE_LOW (x));
	  else
	    sprintf (buf1, HOST_WIDE_INT_PRINT_DEC,
		     CONST_DOUBLE_LOW (x));
	  dyn_string_append (str, buf1);
	}
      else
	/* We can't handle floating point constants; PRINT_OPERAND must
	   handle them.  */
	output_operand_lossage ("floating constant misused");
      break;

    case PLUS:
      /* Some assemblers need integer constants to appear last (eg masm).  */
      if (GET_CODE (XEXP (x, 0)) == CONST_INT)
	{
	  addr_const_to_string (str, XEXP (x, 1));
	  if (INTVAL (XEXP (x, 0)) >= 0)
	    dyn_string_append (str, "+");

	  addr_const_to_string (str, XEXP (x, 0));
	}
      else
	{
	  addr_const_to_string (str, XEXP (x, 0));
	  if (INTVAL (XEXP (x, 1)) >= 0)
	    dyn_string_append (str, "+");

	  addr_const_to_string (str, XEXP (x, 1));
	}
      break;

    case MINUS:
      /* Avoid outputting things like x-x or x+5-x, since some assemblers
         can't handle that.  */
      x = simplify_subtraction (x);
      if (GET_CODE (x) != MINUS)
	goto restart;

      addr_const_to_string (str, XEXP (x, 0));
      dyn_string_append (str, "-");
      if (GET_CODE (XEXP (x, 1)) == CONST_INT
	  && INTVAL (XEXP (x, 1)) < 0)
	{
	  dyn_string_append (str, ASM_OPEN_PAREN);
	  addr_const_to_string (str, XEXP (x, 1));
	  dyn_string_append (str, ASM_CLOSE_PAREN);
	}
      else
	addr_const_to_string (str, XEXP (x, 1));
      break;

    case ZERO_EXTEND:
    case SIGN_EXTEND:
      addr_const_to_string (str, XEXP (x, 0));
      break;

    default:
      output_operand_lossage ("invalid expression as operand");
    }
}

/* Convert an address constant to a string, and return a pointer to
   a copy of the result, located on the heap.  */

static char *
addr_to_string (x)
     rtx x;
{
  dyn_string_t ds = dyn_string_new (256);
  char *s;

  addr_const_to_string (ds, x);
  
  /* Return the dynamically allocated string, but free the
     dyn_string_t itself.  */
  s = ds->s;
  free (ds);
  return s;
}

/* Test if rtl node points to a pseudo register.  */

static inline int
is_pseudo_reg (rtl)
     register rtx rtl;
{
  return (((GET_CODE (rtl) == REG) && (REGNO (rtl) >= FIRST_PSEUDO_REGISTER))
	  || ((GET_CODE (rtl) == SUBREG)
	      && (REGNO (XEXP (rtl, 0)) >= FIRST_PSEUDO_REGISTER)));
}

/* Return a reference to a type, with its const and volatile qualifiers
   removed.  */

static inline tree
type_main_variant (type)
     register tree type;
{
  type = TYPE_MAIN_VARIANT (type);

  /* There really should be only one main variant among any group of variants 
     of a given type (and all of the MAIN_VARIANT values for all members of
     the group should point to that one type) but sometimes the C front-end
     messes this up for array types, so we work around that bug here.  */

  if (TREE_CODE (type) == ARRAY_TYPE)
    while (type != TYPE_MAIN_VARIANT (type))
      type = TYPE_MAIN_VARIANT (type);

  return type;
}

/* Return non-zero if the given type node represents a tagged type.  */

static inline int
is_tagged_type (type)
     register tree type;
{
  register enum tree_code code = TREE_CODE (type);

  return (code == RECORD_TYPE || code == UNION_TYPE
	  || code == QUAL_UNION_TYPE || code == ENUMERAL_TYPE);
}

/* Convert a DIE tag into its string name.  */

static char *
dwarf_tag_name (tag)
     register unsigned tag;
{
  switch (tag)
    {
    case DW_TAG_padding:
      return "DW_TAG_padding";
    case DW_TAG_array_type:
      return "DW_TAG_array_type";
    case DW_TAG_class_type:
      return "DW_TAG_class_type";
    case DW_TAG_entry_point:
      return "DW_TAG_entry_point";
    case DW_TAG_enumeration_type:
      return "DW_TAG_enumeration_type";
    case DW_TAG_formal_parameter:
      return "DW_TAG_formal_parameter";
    case DW_TAG_imported_declaration:
      return "DW_TAG_imported_declaration";
    case DW_TAG_label:
      return "DW_TAG_label";
    case DW_TAG_lexical_block:
      return "DW_TAG_lexical_block";
    case DW_TAG_member:
      return "DW_TAG_member";
    case DW_TAG_pointer_type:
      return "DW_TAG_pointer_type";
    case DW_TAG_reference_type:
      return "DW_TAG_reference_type";
    case DW_TAG_compile_unit:
      return "DW_TAG_compile_unit";
    case DW_TAG_string_type:
      return "DW_TAG_string_type";
    case DW_TAG_structure_type:
      return "DW_TAG_structure_type";
    case DW_TAG_subroutine_type:
      return "DW_TAG_subroutine_type";
    case DW_TAG_typedef:
      return "DW_TAG_typedef";
    case DW_TAG_union_type:
      return "DW_TAG_union_type";
    case DW_TAG_unspecified_parameters:
      return "DW_TAG_unspecified_parameters";
    case DW_TAG_variant:
      return "DW_TAG_variant";
    case DW_TAG_common_block:
      return "DW_TAG_common_block";
    case DW_TAG_common_inclusion:
      return "DW_TAG_common_inclusion";
    case DW_TAG_inheritance:
      return "DW_TAG_inheritance";
    case DW_TAG_inlined_subroutine:
      return "DW_TAG_inlined_subroutine";
    case DW_TAG_module:
      return "DW_TAG_module";
    case DW_TAG_ptr_to_member_type:
      return "DW_TAG_ptr_to_member_type";
    case DW_TAG_set_type:
      return "DW_TAG_set_type";
    case DW_TAG_subrange_type:
      return "DW_TAG_subrange_type";
    case DW_TAG_with_stmt:
      return "DW_TAG_with_stmt";
    case DW_TAG_access_declaration:
      return "DW_TAG_access_declaration";
    case DW_TAG_base_type:
      return "DW_TAG_base_type";
    case DW_TAG_catch_block:
      return "DW_TAG_catch_block";
    case DW_TAG_const_type:
      return "DW_TAG_const_type";
    case DW_TAG_constant:
      return "DW_TAG_constant";
    case DW_TAG_enumerator:
      return "DW_TAG_enumerator";
    case DW_TAG_file_type:
      return "DW_TAG_file_type";
    case DW_TAG_friend:
      return "DW_TAG_friend";
    case DW_TAG_namelist:
      return "DW_TAG_namelist";
    case DW_TAG_namelist_item:
      return "DW_TAG_namelist_item";
    case DW_TAG_packed_type:
      return "DW_TAG_packed_type";
    case DW_TAG_subprogram:
      return "DW_TAG_subprogram";
    case DW_TAG_template_type_param:
      return "DW_TAG_template_type_param";
    case DW_TAG_template_value_param:
      return "DW_TAG_template_value_param";
    case DW_TAG_thrown_type:
      return "DW_TAG_thrown_type";
    case DW_TAG_try_block:
      return "DW_TAG_try_block";
    case DW_TAG_variant_part:
      return "DW_TAG_variant_part";
    case DW_TAG_variable:
      return "DW_TAG_variable";
    case DW_TAG_volatile_type:
      return "DW_TAG_volatile_type";
    case DW_TAG_MIPS_loop:
      return "DW_TAG_MIPS_loop";
    case DW_TAG_format_label:
      return "DW_TAG_format_label";
    case DW_TAG_function_template:
      return "DW_TAG_function_template";
    case DW_TAG_class_template:
      return "DW_TAG_class_template";
    default:
      return "DW_TAG_<unknown>";
    }
}

/* Convert a DWARF attribute code into its string name.  */

static char *
dwarf_attr_name (attr)
     register unsigned attr;
{
  switch (attr)
    {
    case DW_AT_sibling:
      return "DW_AT_sibling";
    case DW_AT_location:
      return "DW_AT_location";
    case DW_AT_name:
      return "DW_AT_name";
    case DW_AT_ordering:
      return "DW_AT_ordering";
    case DW_AT_subscr_data:
      return "DW_AT_subscr_data";
    case DW_AT_byte_size:
      return "DW_AT_byte_size";
    case DW_AT_bit_offset:
      return "DW_AT_bit_offset";
    case DW_AT_bit_size:
      return "DW_AT_bit_size";
    case DW_AT_element_list:
      return "DW_AT_element_list";
    case DW_AT_stmt_list:
      return "DW_AT_stmt_list";
    case DW_AT_low_pc:
      return "DW_AT_low_pc";
    case DW_AT_high_pc:
      return "DW_AT_high_pc";
    case DW_AT_language:
      return "DW_AT_language";
    case DW_AT_member:
      return "DW_AT_member";
    case DW_AT_discr:
      return "DW_AT_discr";
    case DW_AT_discr_value:
      return "DW_AT_discr_value";
    case DW_AT_visibility:
      return "DW_AT_visibility";
    case DW_AT_import:
      return "DW_AT_import";
    case DW_AT_string_length:
      return "DW_AT_string_length";
    case DW_AT_common_reference:
      return "DW_AT_common_reference";
    case DW_AT_comp_dir:
      return "DW_AT_comp_dir";
    case DW_AT_const_value:
      return "DW_AT_const_value";
    case DW_AT_containing_type:
      return "DW_AT_containing_type";
    case DW_AT_default_value:
      return "DW_AT_default_value";
    case DW_AT_inline:
      return "DW_AT_inline";
    case DW_AT_is_optional:
      return "DW_AT_is_optional";
    case DW_AT_lower_bound:
      return "DW_AT_lower_bound";
    case DW_AT_producer:
      return "DW_AT_producer";
    case DW_AT_prototyped:
      return "DW_AT_prototyped";
    case DW_AT_return_addr:
      return "DW_AT_return_addr";
    case DW_AT_start_scope:
      return "DW_AT_start_scope";
    case DW_AT_stride_size:
      return "DW_AT_stride_size";
    case DW_AT_upper_bound:
      return "DW_AT_upper_bound";
    case DW_AT_abstract_origin:
      return "DW_AT_abstract_origin";
    case DW_AT_accessibility:
      return "DW_AT_accessibility";
    case DW_AT_address_class:
      return "DW_AT_address_class";
    case DW_AT_artificial:
      return "DW_AT_artificial";
    case DW_AT_base_types:
      return "DW_AT_base_types";
    case DW_AT_calling_convention:
      return "DW_AT_calling_convention";
    case DW_AT_count:
      return "DW_AT_count";
    case DW_AT_data_member_location:
      return "DW_AT_data_member_location";
    case DW_AT_decl_column:
      return "DW_AT_decl_column";
    case DW_AT_decl_file:
      return "DW_AT_decl_file";
    case DW_AT_decl_line:
      return "DW_AT_decl_line";
    case DW_AT_declaration:
      return "DW_AT_declaration";
    case DW_AT_discr_list:
      return "DW_AT_discr_list";
    case DW_AT_encoding:
      return "DW_AT_encoding";
    case DW_AT_external:
      return "DW_AT_external";
    case DW_AT_frame_base:
      return "DW_AT_frame_base";
    case DW_AT_friend:
      return "DW_AT_friend";
    case DW_AT_identifier_case:
      return "DW_AT_identifier_case";
    case DW_AT_macro_info:
      return "DW_AT_macro_info";
    case DW_AT_namelist_items:
      return "DW_AT_namelist_items";
    case DW_AT_priority:
      return "DW_AT_priority";
    case DW_AT_segment:
      return "DW_AT_segment";
    case DW_AT_specification:
      return "DW_AT_specification";
    case DW_AT_static_link:
      return "DW_AT_static_link";
    case DW_AT_type:
      return "DW_AT_type";
    case DW_AT_use_location:
      return "DW_AT_use_location";
    case DW_AT_variable_parameter:
      return "DW_AT_variable_parameter";
    case DW_AT_virtuality:
      return "DW_AT_virtuality";
    case DW_AT_vtable_elem_location:
      return "DW_AT_vtable_elem_location";

    case DW_AT_MIPS_fde:
      return "DW_AT_MIPS_fde";
    case DW_AT_MIPS_loop_begin:
      return "DW_AT_MIPS_loop_begin";
    case DW_AT_MIPS_tail_loop_begin:
      return "DW_AT_MIPS_tail_loop_begin";
    case DW_AT_MIPS_epilog_begin:
      return "DW_AT_MIPS_epilog_begin";
    case DW_AT_MIPS_loop_unroll_factor:
      return "DW_AT_MIPS_loop_unroll_factor";
    case DW_AT_MIPS_software_pipeline_depth:
      return "DW_AT_MIPS_software_pipeline_depth";
    case DW_AT_MIPS_linkage_name:
      return "DW_AT_MIPS_linkage_name";
    case DW_AT_MIPS_stride:
      return "DW_AT_MIPS_stride";
    case DW_AT_MIPS_abstract_name:
      return "DW_AT_MIPS_abstract_name";
    case DW_AT_MIPS_clone_origin:
      return "DW_AT_MIPS_clone_origin";
    case DW_AT_MIPS_has_inlines:
      return "DW_AT_MIPS_has_inlines";

    case DW_AT_sf_names:
      return "DW_AT_sf_names";
    case DW_AT_src_info:
      return "DW_AT_src_info";
    case DW_AT_mac_info:
      return "DW_AT_mac_info";
    case DW_AT_src_coords:
      return "DW_AT_src_coords";
    case DW_AT_body_begin:
      return "DW_AT_body_begin";
    case DW_AT_body_end:
      return "DW_AT_body_end";
    default:
      return "DW_AT_<unknown>";
    }
}

/* Convert a DWARF value form code into its string name.  */

static char *
dwarf_form_name (form)
     register unsigned form;
{
  switch (form)
    {
    case DW_FORM_addr:
      return "DW_FORM_addr";
    case DW_FORM_block2:
      return "DW_FORM_block2";
    case DW_FORM_block4:
      return "DW_FORM_block4";
    case DW_FORM_data2:
      return "DW_FORM_data2";
    case DW_FORM_data4:
      return "DW_FORM_data4";
    case DW_FORM_data8:
      return "DW_FORM_data8";
    case DW_FORM_string:
      return "DW_FORM_string";
    case DW_FORM_block:
      return "DW_FORM_block";
    case DW_FORM_block1:
      return "DW_FORM_block1";
    case DW_FORM_data1:
      return "DW_FORM_data1";
    case DW_FORM_flag:
      return "DW_FORM_flag";
    case DW_FORM_sdata:
      return "DW_FORM_sdata";
    case DW_FORM_strp:
      return "DW_FORM_strp";
    case DW_FORM_udata:
      return "DW_FORM_udata";
    case DW_FORM_ref_addr:
      return "DW_FORM_ref_addr";
    case DW_FORM_ref1:
      return "DW_FORM_ref1";
    case DW_FORM_ref2:
      return "DW_FORM_ref2";
    case DW_FORM_ref4:
      return "DW_FORM_ref4";
    case DW_FORM_ref8:
      return "DW_FORM_ref8";
    case DW_FORM_ref_udata:
      return "DW_FORM_ref_udata";
    case DW_FORM_indirect:
      return "DW_FORM_indirect";
    default:
      return "DW_FORM_<unknown>";
    }
}

/* Convert a DWARF stack opcode into its string name.  */

static char *
dwarf_stack_op_name (op)
     register unsigned op;
{
  switch (op)
    {
    case DW_OP_addr:
      return "DW_OP_addr";
    case DW_OP_deref:
      return "DW_OP_deref";
    case DW_OP_const1u:
      return "DW_OP_const1u";
    case DW_OP_const1s:
      return "DW_OP_const1s";
    case DW_OP_const2u:
      return "DW_OP_const2u";
    case DW_OP_const2s:
      return "DW_OP_const2s";
    case DW_OP_const4u:
      return "DW_OP_const4u";
    case DW_OP_const4s:
      return "DW_OP_const4s";
    case DW_OP_const8u:
      return "DW_OP_const8u";
    case DW_OP_const8s:
      return "DW_OP_const8s";
    case DW_OP_constu:
      return "DW_OP_constu";
    case DW_OP_consts:
      return "DW_OP_consts";
    case DW_OP_dup:
      return "DW_OP_dup";
    case DW_OP_drop:
      return "DW_OP_drop";
    case DW_OP_over:
      return "DW_OP_over";
    case DW_OP_pick:
      return "DW_OP_pick";
    case DW_OP_swap:
      return "DW_OP_swap";
    case DW_OP_rot:
      return "DW_OP_rot";
    case DW_OP_xderef:
      return "DW_OP_xderef";
    case DW_OP_abs:
      return "DW_OP_abs";
    case DW_OP_and:
      return "DW_OP_and";
    case DW_OP_div:
      return "DW_OP_div";
    case DW_OP_minus:
      return "DW_OP_minus";
    case DW_OP_mod:
      return "DW_OP_mod";
    case DW_OP_mul:
      return "DW_OP_mul";
    case DW_OP_neg:
      return "DW_OP_neg";
    case DW_OP_not:
      return "DW_OP_not";
    case DW_OP_or:
      return "DW_OP_or";
    case DW_OP_plus:
      return "DW_OP_plus";
    case DW_OP_plus_uconst:
      return "DW_OP_plus_uconst";
    case DW_OP_shl:
      return "DW_OP_shl";
    case DW_OP_shr:
      return "DW_OP_shr";
    case DW_OP_shra:
      return "DW_OP_shra";
    case DW_OP_xor:
      return "DW_OP_xor";
    case DW_OP_bra:
      return "DW_OP_bra";
    case DW_OP_eq:
      return "DW_OP_eq";
    case DW_OP_ge:
      return "DW_OP_ge";
    case DW_OP_gt:
      return "DW_OP_gt";
    case DW_OP_le:
      return "DW_OP_le";
    case DW_OP_lt:
      return "DW_OP_lt";
    case DW_OP_ne:
      return "DW_OP_ne";
    case DW_OP_skip:
      return "DW_OP_skip";
    case DW_OP_lit0:
      return "DW_OP_lit0";
    case DW_OP_lit1:
      return "DW_OP_lit1";
    case DW_OP_lit2:
      return "DW_OP_lit2";
    case DW_OP_lit3:
      return "DW_OP_lit3";
    case DW_OP_lit4:
      return "DW_OP_lit4";
    case DW_OP_lit5:
      return "DW_OP_lit5";
    case DW_OP_lit6:
      return "DW_OP_lit6";
    case DW_OP_lit7:
      return "DW_OP_lit7";
    case DW_OP_lit8:
      return "DW_OP_lit8";
    case DW_OP_lit9:
      return "DW_OP_lit9";
    case DW_OP_lit10:
      return "DW_OP_lit10";
    case DW_OP_lit11:
      return "DW_OP_lit11";
    case DW_OP_lit12:
      return "DW_OP_lit12";
    case DW_OP_lit13:
      return "DW_OP_lit13";
    case DW_OP_lit14:
      return "DW_OP_lit14";
    case DW_OP_lit15:
      return "DW_OP_lit15";
    case DW_OP_lit16:
      return "DW_OP_lit16";
    case DW_OP_lit17:
      return "DW_OP_lit17";
    case DW_OP_lit18:
      return "DW_OP_lit18";
    case DW_OP_lit19:
      return "DW_OP_lit19";
    case DW_OP_lit20:
      return "DW_OP_lit20";
    case DW_OP_lit21:
      return "DW_OP_lit21";
    case DW_OP_lit22:
      return "DW_OP_lit22";
    case DW_OP_lit23:
      return "DW_OP_lit23";
    case DW_OP_lit24:
      return "DW_OP_lit24";
    case DW_OP_lit25:
      return "DW_OP_lit25";
    case DW_OP_lit26:
      return "DW_OP_lit26";
    case DW_OP_lit27:
      return "DW_OP_lit27";
    case DW_OP_lit28:
      return "DW_OP_lit28";
    case DW_OP_lit29:
      return "DW_OP_lit29";
    case DW_OP_lit30:
      return "DW_OP_lit30";
    case DW_OP_lit31:
      return "DW_OP_lit31";
    case DW_OP_reg0:
      return "DW_OP_reg0";
    case DW_OP_reg1:
      return "DW_OP_reg1";
    case DW_OP_reg2:
      return "DW_OP_reg2";
    case DW_OP_reg3:
      return "DW_OP_reg3";
    case DW_OP_reg4:
      return "DW_OP_reg4";
    case DW_OP_reg5:
      return "DW_OP_reg5";
    case DW_OP_reg6:
      return "DW_OP_reg6";
    case DW_OP_reg7:
      return "DW_OP_reg7";
    case DW_OP_reg8:
      return "DW_OP_reg8";
    case DW_OP_reg9:
      return "DW_OP_reg9";
    case DW_OP_reg10:
      return "DW_OP_reg10";
    case DW_OP_reg11:
      return "DW_OP_reg11";
    case DW_OP_reg12:
      return "DW_OP_reg12";
    case DW_OP_reg13:
      return "DW_OP_reg13";
    case DW_OP_reg14:
      return "DW_OP_reg14";
    case DW_OP_reg15:
      return "DW_OP_reg15";
    case DW_OP_reg16:
      return "DW_OP_reg16";
    case DW_OP_reg17:
      return "DW_OP_reg17";
    case DW_OP_reg18:
      return "DW_OP_reg18";
    case DW_OP_reg19:
      return "DW_OP_reg19";
    case DW_OP_reg20:
      return "DW_OP_reg20";
    case DW_OP_reg21:
      return "DW_OP_reg21";
    case DW_OP_reg22:
      return "DW_OP_reg22";
    case DW_OP_reg23:
      return "DW_OP_reg23";
    case DW_OP_reg24:
      return "DW_OP_reg24";
    case DW_OP_reg25:
      return "DW_OP_reg25";
    case DW_OP_reg26:
      return "DW_OP_reg26";
    case DW_OP_reg27:
      return "DW_OP_reg27";
    case DW_OP_reg28:
      return "DW_OP_reg28";
    case DW_OP_reg29:
      return "DW_OP_reg29";
    case DW_OP_reg30:
      return "DW_OP_reg30";
    case DW_OP_reg31:
      return "DW_OP_reg31";
    case DW_OP_breg0:
      return "DW_OP_breg0";
    case DW_OP_breg1:
      return "DW_OP_breg1";
    case DW_OP_breg2:
      return "DW_OP_breg2";
    case DW_OP_breg3:
      return "DW_OP_breg3";
    case DW_OP_breg4:
      return "DW_OP_breg4";
    case DW_OP_breg5:
      return "DW_OP_breg5";
    case DW_OP_breg6:
      return "DW_OP_breg6";
    case DW_OP_breg7:
      return "DW_OP_breg7";
    case DW_OP_breg8:
      return "DW_OP_breg8";
    case DW_OP_breg9:
      return "DW_OP_breg9";
    case DW_OP_breg10:
      return "DW_OP_breg10";
    case DW_OP_breg11:
      return "DW_OP_breg11";
    case DW_OP_breg12:
      return "DW_OP_breg12";
    case DW_OP_breg13:
      return "DW_OP_breg13";
    case DW_OP_breg14:
      return "DW_OP_breg14";
    case DW_OP_breg15:
      return "DW_OP_breg15";
    case DW_OP_breg16:
      return "DW_OP_breg16";
    case DW_OP_breg17:
      return "DW_OP_breg17";
    case DW_OP_breg18:
      return "DW_OP_breg18";
    case DW_OP_breg19:
      return "DW_OP_breg19";
    case DW_OP_breg20:
      return "DW_OP_breg20";
    case DW_OP_breg21:
      return "DW_OP_breg21";
    case DW_OP_breg22:
      return "DW_OP_breg22";
    case DW_OP_breg23:
      return "DW_OP_breg23";
    case DW_OP_breg24:
      return "DW_OP_breg24";
    case DW_OP_breg25:
      return "DW_OP_breg25";
    case DW_OP_breg26:
      return "DW_OP_breg26";
    case DW_OP_breg27:
      return "DW_OP_breg27";
    case DW_OP_breg28:
      return "DW_OP_breg28";
    case DW_OP_breg29:
      return "DW_OP_breg29";
    case DW_OP_breg30:
      return "DW_OP_breg30";
    case DW_OP_breg31:
      return "DW_OP_breg31";
    case DW_OP_regx:
      return "DW_OP_regx";
    case DW_OP_fbreg:
      return "DW_OP_fbreg";
    case DW_OP_bregx:
      return "DW_OP_bregx";
    case DW_OP_piece:
      return "DW_OP_piece";
    case DW_OP_deref_size:
      return "DW_OP_deref_size";
    case DW_OP_xderef_size:
      return "DW_OP_xderef_size";
    case DW_OP_nop:
      return "DW_OP_nop";
    default:
      return "OP_<unknown>";
    }
}

/* Convert a DWARF type code into its string name.  */

#if 0
static char *
dwarf_type_encoding_name (enc)
     register unsigned enc;
{
  switch (enc)
    {
    case DW_ATE_address:
      return "DW_ATE_address";
    case DW_ATE_boolean:
      return "DW_ATE_boolean";
    case DW_ATE_complex_float:
      return "DW_ATE_complex_float";
    case DW_ATE_float:
      return "DW_ATE_float";
    case DW_ATE_signed:
      return "DW_ATE_signed";
    case DW_ATE_signed_char:
      return "DW_ATE_signed_char";
    case DW_ATE_unsigned:
      return "DW_ATE_unsigned";
    case DW_ATE_unsigned_char:
      return "DW_ATE_unsigned_char";
    default:
      return "DW_ATE_<unknown>";
    }
}
#endif

/* Determine the "ultimate origin" of a decl.  The decl may be an inlined
   instance of an inlined instance of a decl which is local to an inline
   function, so we have to trace all of the way back through the origin chain
   to find out what sort of node actually served as the original seed for the
   given block.  */

static tree
decl_ultimate_origin (decl)
     register tree decl;
{
#ifdef ENABLE_CHECKING 
  if (DECL_FROM_INLINE (DECL_ORIGIN (decl)))
    /* Since the DECL_ABSTRACT_ORIGIN for a DECL is supposed to be the
       most distant ancestor, this should never happen.  */
    abort ();
#endif

  return DECL_ABSTRACT_ORIGIN (decl);
}

/* Determine the "ultimate origin" of a block.  The block may be an inlined
   instance of an inlined instance of a block which is local to an inline
   function, so we have to trace all of the way back through the origin chain
   to find out what sort of node actually served as the original seed for the
   given block.  */

static tree
block_ultimate_origin (block)
     register tree block;
{
  register tree immediate_origin = BLOCK_ABSTRACT_ORIGIN (block);

  if (immediate_origin == NULL_TREE)
    return NULL_TREE;
  else
    {
      register tree ret_val;
      register tree lookahead = immediate_origin;

      do
	{
	  ret_val = lookahead;
	  lookahead = (TREE_CODE (ret_val) == BLOCK)
	    ? BLOCK_ABSTRACT_ORIGIN (ret_val)
	    : NULL;
	}
      while (lookahead != NULL && lookahead != ret_val);

      return ret_val;
    }
}

/* Get the class to which DECL belongs, if any.  In g++, the DECL_CONTEXT
   of a virtual function may refer to a base class, so we check the 'this'
   parameter.  */

static tree
decl_class_context (decl)
     tree decl;
{
  tree context = NULL_TREE;

  if (TREE_CODE (decl) != FUNCTION_DECL || ! DECL_VINDEX (decl))
    context = DECL_CONTEXT (decl);
  else
    context = TYPE_MAIN_VARIANT
      (TREE_TYPE (TREE_VALUE (TYPE_ARG_TYPES (TREE_TYPE (decl)))));

  if (context && TREE_CODE_CLASS (TREE_CODE (context)) != 't')
    context = NULL_TREE;

  return context;
}

/* Add an attribute/value pair to a DIE */

static inline void
add_dwarf_attr (die, attr)
     register dw_die_ref die;
     register dw_attr_ref attr;
{
  if (die != NULL && attr != NULL)
    {
      if (die->die_attr == NULL)
	{
	  die->die_attr = attr;
	  die->die_attr_last = attr;
	}
      else
	{
	  die->die_attr_last->dw_attr_next = attr;
	  die->die_attr_last = attr;
	}
    }
}

/* Add a flag value attribute to a DIE.  */

static inline void
add_AT_flag (die, attr_kind, flag)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register unsigned flag;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_flag;
  attr->dw_attr_val.v.val_flag = flag;
  add_dwarf_attr (die, attr);
}

/* Add a signed integer attribute value to a DIE.  */

static inline void
add_AT_int (die, attr_kind, int_val)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register long int int_val;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_const;
  attr->dw_attr_val.v.val_int = int_val;
  add_dwarf_attr (die, attr);
}

/* Add an unsigned integer attribute value to a DIE.  */

static inline void
add_AT_unsigned (die, attr_kind, unsigned_val)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register unsigned long unsigned_val;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_unsigned_const;
  attr->dw_attr_val.v.val_unsigned = unsigned_val;
  add_dwarf_attr (die, attr);
}

/* Add an unsigned double integer attribute value to a DIE.  */

static inline void
add_AT_long_long (die, attr_kind, val_hi, val_low)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register unsigned long val_hi;
     register unsigned long val_low;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_long_long;
  attr->dw_attr_val.v.val_long_long.hi = val_hi;
  attr->dw_attr_val.v.val_long_long.low = val_low;
  add_dwarf_attr (die, attr);
}

/* Add a floating point attribute value to a DIE and return it.  */

static inline void
add_AT_float (die, attr_kind, length, array)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register unsigned length;
     register long *array;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_float;
  attr->dw_attr_val.v.val_float.length = length;
  attr->dw_attr_val.v.val_float.array = array;
  add_dwarf_attr (die, attr);
}

/* Add a string attribute value to a DIE.  */

static inline void
add_AT_string (die, attr_kind, str)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register char *str;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_str;
  attr->dw_attr_val.v.val_str = xstrdup (str);
  add_dwarf_attr (die, attr);
}

/* Add a DIE reference attribute value to a DIE.  */

static inline void
add_AT_die_ref (die, attr_kind, targ_die)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register dw_die_ref targ_die;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_die_ref;
  attr->dw_attr_val.v.val_die_ref = targ_die;
  add_dwarf_attr (die, attr);
}

/* Add an FDE reference attribute value to a DIE.  */

static inline void
add_AT_fde_ref (die, attr_kind, targ_fde)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register unsigned targ_fde;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_fde_ref;
  attr->dw_attr_val.v.val_fde_index = targ_fde;
  add_dwarf_attr (die, attr);
}

/* Add a location description attribute value to a DIE.  */

static inline void
add_AT_loc (die, attr_kind, loc)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register dw_loc_descr_ref loc;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_loc;
  attr->dw_attr_val.v.val_loc = loc;
  add_dwarf_attr (die, attr);
}

/* Add an address constant attribute value to a DIE.  */

static inline void
add_AT_addr (die, attr_kind, addr)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     char *addr;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_addr;
  attr->dw_attr_val.v.val_addr = addr;
  add_dwarf_attr (die, attr);
}

/* Add a label identifier attribute value to a DIE.  */

static inline void
add_AT_lbl_id (die, attr_kind, lbl_id)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register char *lbl_id;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_lbl_id;
  attr->dw_attr_val.v.val_lbl_id = xstrdup (lbl_id);
  add_dwarf_attr (die, attr);
}

/* Add a section offset attribute value to a DIE.  */

static inline void
add_AT_section_offset (die, attr_kind, section)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
     register char *section;
{
  register dw_attr_ref attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));

  attr->dw_attr_next = NULL;
  attr->dw_attr = attr_kind;
  attr->dw_attr_val.val_class = dw_val_class_section_offset;
  attr->dw_attr_val.v.val_section = section;
  add_dwarf_attr (die, attr);
  
}

/* Test if die refers to an external subroutine.  */

static inline int
is_extern_subr_die (die)
     register dw_die_ref die;
{
  register dw_attr_ref a;
  register int is_subr = FALSE;
  register int is_extern = FALSE;

  if (die != NULL && die->die_tag == DW_TAG_subprogram)
    {
      is_subr = TRUE;
      for (a = die->die_attr; a != NULL; a = a->dw_attr_next)
	{
	  if (a->dw_attr == DW_AT_external
	      && a->dw_attr_val.val_class == dw_val_class_flag
	      && a->dw_attr_val.v.val_flag != 0)
	    {
	      is_extern = TRUE;
	      break;
	    }
	}
    }

  return is_subr && is_extern;
}

/* Get the attribute of type attr_kind.  */

static inline dw_attr_ref
get_AT (die, attr_kind)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
{
  register dw_attr_ref a;
  register dw_die_ref spec = NULL;
  
  if (die != NULL)
    {
      for (a = die->die_attr; a != NULL; a = a->dw_attr_next)
	{
	  if (a->dw_attr == attr_kind)
	    return a;

	  if (a->dw_attr == DW_AT_specification
	      || a->dw_attr == DW_AT_abstract_origin)
	    spec = a->dw_attr_val.v.val_die_ref;
	}

      if (spec)
	return get_AT (spec, attr_kind);
    }

  return NULL;
}

/* Return the "low pc" attribute value, typically associated with
   a subprogram DIE.  Return null if the "low pc" attribute is
   either not prsent, or if it cannot be represented as an
   assembler label identifier.  */

static inline char *
get_AT_low_pc (die)
     register dw_die_ref die;
{
  register dw_attr_ref a = get_AT (die, DW_AT_low_pc);

  if (a && a->dw_attr_val.val_class == dw_val_class_lbl_id)
    return a->dw_attr_val.v.val_lbl_id;

  return NULL;
}

/* Return the "high pc" attribute value, typically associated with
   a subprogram DIE.  Return null if the "high pc" attribute is
   either not prsent, or if it cannot be represented as an
   assembler label identifier.  */

static inline char *
get_AT_hi_pc (die)
     register dw_die_ref die;
{
  register dw_attr_ref a = get_AT (die, DW_AT_high_pc);

  if (a && a->dw_attr_val.val_class == dw_val_class_lbl_id)
    return a->dw_attr_val.v.val_lbl_id;

  return NULL;
}

/* Return the value of the string attribute designated by ATTR_KIND, or
   NULL if it is not present.  */

static inline char *
get_AT_string (die, attr_kind)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
{
  register dw_attr_ref a = get_AT (die, attr_kind);

  if (a && a->dw_attr_val.val_class == dw_val_class_str)
    return a->dw_attr_val.v.val_str;

  return NULL;
}

/* Return the value of the flag attribute designated by ATTR_KIND, or -1
   if it is not present.  */

static inline int
get_AT_flag (die, attr_kind)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
{
  register dw_attr_ref a = get_AT (die, attr_kind);

  if (a && a->dw_attr_val.val_class == dw_val_class_flag)
    return a->dw_attr_val.v.val_flag;

  return -1;
}

/* Return the value of the unsigned attribute designated by ATTR_KIND, or 0
   if it is not present.  */

static inline unsigned
get_AT_unsigned (die, attr_kind)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
{
  register dw_attr_ref a = get_AT (die, attr_kind);

  if (a && a->dw_attr_val.val_class == dw_val_class_unsigned_const)
    return a->dw_attr_val.v.val_unsigned;

  return 0;
}

static inline int
is_c_family ()
{
  register unsigned lang = get_AT_unsigned (comp_unit_die, DW_AT_language);

  return (lang == DW_LANG_C || lang == DW_LANG_C89
	  || lang == DW_LANG_C_plus_plus);
} 

static inline int
is_fortran ()
{
  register unsigned lang = get_AT_unsigned (comp_unit_die, DW_AT_language);

  return (lang == DW_LANG_Fortran77 || lang == DW_LANG_Fortran90);
} 

/* Remove the specified attribute if present.  */

static inline void
remove_AT (die, attr_kind)
     register dw_die_ref die;
     register enum dwarf_attribute attr_kind;
{
  register dw_attr_ref a;
  register dw_attr_ref removed = NULL;;

  if (die != NULL)
    {
      if (die->die_attr->dw_attr == attr_kind)
	{
	  removed = die->die_attr;
	  if (die->die_attr_last == die->die_attr)
	    die->die_attr_last = NULL;

	  die->die_attr = die->die_attr->dw_attr_next;
	}

      else
	for (a = die->die_attr; a->dw_attr_next != NULL;
	     a = a->dw_attr_next)
	  if (a->dw_attr_next->dw_attr == attr_kind)
	    {
	      removed = a->dw_attr_next;
	      if (die->die_attr_last == a->dw_attr_next)
		die->die_attr_last = a;

	      a->dw_attr_next = a->dw_attr_next->dw_attr_next;
	      break;
	    }

      if (removed != 0)
	free (removed);
    }
}

/* Discard the children of this DIE.  */

static inline void
remove_children (die)
     register dw_die_ref die;
{
  register dw_die_ref child_die = die->die_child;

  die->die_child = NULL;
  die->die_child_last = NULL;

  while (child_die != NULL)
    {
      register dw_die_ref tmp_die = child_die;
      register dw_attr_ref a;

      child_die = child_die->die_sib;
      
      for (a = tmp_die->die_attr; a != NULL; )
	{
	  register dw_attr_ref tmp_a = a;

	  a = a->dw_attr_next;
	  free (tmp_a);
	}

      free (tmp_die);
    }
}

/* Add a child DIE below its parent.  */

static inline void
add_child_die (die, child_die)
     register dw_die_ref die;
     register dw_die_ref child_die;
{
  if (die != NULL && child_die != NULL)
    {
      if (die == child_die)
	abort ();
      child_die->die_parent = die;
      child_die->die_sib = NULL;

      if (die->die_child == NULL)
	{
	  die->die_child = child_die;
	  die->die_child_last = child_die;
	}
      else
	{
	  die->die_child_last->die_sib = child_die;
	  die->die_child_last = child_die;
	}
    }
}

/* Return a pointer to a newly created DIE node.  */

static inline dw_die_ref
new_die (tag_value, parent_die)
     register enum dwarf_tag tag_value;
     register dw_die_ref parent_die;
{
  register dw_die_ref die = (dw_die_ref) xmalloc (sizeof (die_node));

  die->die_tag = tag_value;
  die->die_abbrev = 0;
  die->die_offset = 0;
  die->die_child = NULL;
  die->die_parent = NULL;
  die->die_sib = NULL;
  die->die_child_last = NULL;
  die->die_attr = NULL;
  die->die_attr_last = NULL;

  if (parent_die != NULL)
    add_child_die (parent_die, die);
  else
    {
      limbo_die_node *limbo_node;

      limbo_node = (limbo_die_node *) xmalloc (sizeof (limbo_die_node));
      limbo_node->die = die;
      limbo_node->next = limbo_die_list;
      limbo_die_list = limbo_node;
    }

  return die;
}

/* Return the DIE associated with the given type specifier.  */

static inline dw_die_ref
lookup_type_die (type)
     register tree type;
{
  return (dw_die_ref) TYPE_SYMTAB_POINTER (type);
}

/* Equate a DIE to a given type specifier.  */

static void
equate_type_number_to_die (type, type_die)
     register tree type;
     register dw_die_ref type_die;
{
  TYPE_SYMTAB_POINTER (type) = (char *) type_die;
}

/* Return the DIE associated with a given declaration.  */

static inline dw_die_ref
lookup_decl_die (decl)
     register tree decl;
{
  register unsigned decl_id = DECL_UID (decl);

  return (decl_id < decl_die_table_in_use
	  ? decl_die_table[decl_id] : NULL);
}

/* Equate a DIE to a particular declaration.  */

static void
equate_decl_number_to_die (decl, decl_die)
     register tree decl;
     register dw_die_ref decl_die;
{
  register unsigned decl_id = DECL_UID (decl);
  register unsigned num_allocated;

  if (decl_id >= decl_die_table_allocated)
    {
      num_allocated
	= ((decl_id + 1 + DECL_DIE_TABLE_INCREMENT - 1)
	   / DECL_DIE_TABLE_INCREMENT)
	  * DECL_DIE_TABLE_INCREMENT;

      decl_die_table
	= (dw_die_ref *) xrealloc (decl_die_table,
				   sizeof (dw_die_ref) * num_allocated);

      zero_memory ((char *) &decl_die_table[decl_die_table_allocated],
	     (num_allocated - decl_die_table_allocated) * sizeof (dw_die_ref));
      decl_die_table_allocated = num_allocated;
    }

  if (decl_id >= decl_die_table_in_use)
    decl_die_table_in_use = (decl_id + 1);

  decl_die_table[decl_id] = decl_die;
}

/* Return a pointer to a newly allocated location description.  Location
   descriptions are simple expression terms that can be strung
   together to form more complicated location (address) descriptions.  */

static inline dw_loc_descr_ref
new_loc_descr (op, oprnd1, oprnd2)
     register enum dwarf_location_atom op;
     register unsigned long oprnd1;
     register unsigned long oprnd2;
{
  register dw_loc_descr_ref descr
    = (dw_loc_descr_ref) xmalloc (sizeof (dw_loc_descr_node));

  descr->dw_loc_next = NULL;
  descr->dw_loc_opc = op;
  descr->dw_loc_oprnd1.val_class = dw_val_class_unsigned_const;
  descr->dw_loc_oprnd1.v.val_unsigned = oprnd1;
  descr->dw_loc_oprnd2.val_class = dw_val_class_unsigned_const;
  descr->dw_loc_oprnd2.v.val_unsigned = oprnd2;

  return descr;
}

/* Add a location description term to a location description expression.  */

static inline void
add_loc_descr (list_head, descr)
     register dw_loc_descr_ref *list_head;
     register dw_loc_descr_ref descr;
{
  register dw_loc_descr_ref *d;

  /* Find the end of the chain.  */
  for (d = list_head; (*d) != NULL; d = &(*d)->dw_loc_next)
    ;

  *d = descr;
}

/* Keep track of the number of spaces used to indent the
   output of the debugging routines that print the structure of
   the DIE internal representation.  */
static int print_indent;

/* Indent the line the number of spaces given by print_indent.  */

static inline void
print_spaces (outfile)
     FILE *outfile;
{
  fprintf (outfile, "%*s", print_indent, "");
}

/* Print the information associated with a given DIE, and its children.
   This routine is a debugging aid only.  */

static void
print_die (die, outfile)
     dw_die_ref die;
     FILE *outfile;
{
  register dw_attr_ref a;
  register dw_die_ref c;

  print_spaces (outfile);
  fprintf (outfile, "DIE %4lu: %s\n",
	   die->die_offset, dwarf_tag_name (die->die_tag));
  print_spaces (outfile);
  fprintf (outfile, "  abbrev id: %lu", die->die_abbrev);
  fprintf (outfile, " offset: %lu\n", die->die_offset);

  for (a = die->die_attr; a != NULL; a = a->dw_attr_next)
    {
      print_spaces (outfile);
      fprintf (outfile, "  %s: ", dwarf_attr_name (a->dw_attr));

      switch (a->dw_attr_val.val_class)
	{
	case dw_val_class_addr:
	  fprintf (outfile, "address");
	  break;
	case dw_val_class_loc:
	  fprintf (outfile, "location descriptor");
	  break;
	case dw_val_class_const:
	  fprintf (outfile, "%ld", a->dw_attr_val.v.val_int);
	  break;
	case dw_val_class_unsigned_const:
	  fprintf (outfile, "%lu", a->dw_attr_val.v.val_unsigned);
	  break;
	case dw_val_class_long_long:
	  fprintf (outfile, "constant (%lu,%lu)",
		  a->dw_attr_val.v.val_long_long.hi,
		  a->dw_attr_val.v.val_long_long.low);
	  break;
	case dw_val_class_float:
	  fprintf (outfile, "floating-point constant");
	  break;
	case dw_val_class_flag:
	  fprintf (outfile, "%u", a->dw_attr_val.v.val_flag);
	  break;
	case dw_val_class_die_ref:
	  if (a->dw_attr_val.v.val_die_ref != NULL)
	    fprintf (outfile, "die -> %lu",
		     a->dw_attr_val.v.val_die_ref->die_offset);
	  else
	    fprintf (outfile, "die -> <null>");
	  break;
	case dw_val_class_lbl_id:
	  fprintf (outfile, "label: %s", a->dw_attr_val.v.val_lbl_id);
	  break;
	case dw_val_class_section_offset:
	  fprintf (outfile, "section: %s", a->dw_attr_val.v.val_section);
	  break;
	case dw_val_class_str:
	  if (a->dw_attr_val.v.val_str != NULL)
	    fprintf (outfile, "\"%s\"", a->dw_attr_val.v.val_str);
	  else
	    fprintf (outfile, "<null>");
	  break;
	default:
	  break;
	}

      fprintf (outfile, "\n");
    }

  if (die->die_child != NULL)
    {
      print_indent += 4;
      for (c = die->die_child; c != NULL; c = c->die_sib)
	print_die (c, outfile);

      print_indent -= 4;
    }
}

/* Print the contents of the source code line number correspondence table.
   This routine is a debugging aid only.  */

static void
print_dwarf_line_table (outfile)
     FILE *outfile;
{
  register unsigned i;
  register dw_line_info_ref line_info;

  fprintf (outfile, "\n\nDWARF source line information\n");
  for (i = 1; i < line_info_table_in_use; ++i)
    {
      line_info = &line_info_table[i];
      fprintf (outfile, "%5d: ", i);
      fprintf (outfile, "%-20s", file_table[line_info->dw_file_num]);
      fprintf (outfile, "%6ld", line_info->dw_line_num);
      fprintf (outfile, "\n");
    }

  fprintf (outfile, "\n\n");
}

/* Print the information collected for a given DIE.  */

void
debug_dwarf_die (die)
     dw_die_ref die;
{
  print_die (die, stderr);
}

/* Print all DWARF information collected for the compilation unit.
   This routine is a debugging aid only.  */

void
debug_dwarf ()
{
  print_indent = 0;
  print_die (comp_unit_die, stderr);
  print_dwarf_line_table (stderr);
}

/* Traverse the DIE, and add a sibling attribute if it may have the
   effect of speeding up access to siblings.  To save some space,
   avoid generating sibling attributes for DIE's without children.  */

static void
add_sibling_attributes(die)
     register dw_die_ref die;
{
  register dw_die_ref c;
  register dw_attr_ref attr;
  if (die != comp_unit_die && die->die_child != NULL)
    {
      attr = (dw_attr_ref) xmalloc (sizeof (dw_attr_node));
      attr->dw_attr_next = NULL;
      attr->dw_attr = DW_AT_sibling;
      attr->dw_attr_val.val_class = dw_val_class_die_ref;
      attr->dw_attr_val.v.val_die_ref = die->die_sib;

      /* Add the sibling link to the front of the attribute list.  */
      attr->dw_attr_next = die->die_attr;
      if (die->die_attr == NULL)
	die->die_attr_last = attr;

      die->die_attr = attr;
    }

  for (c = die->die_child; c != NULL; c = c->die_sib)
    add_sibling_attributes (c);
}

/* The format of each DIE (and its attribute value pairs)
   is encoded in an abbreviation table.  This routine builds the
   abbreviation table and assigns a unique abbreviation id for
   each abbreviation entry.  The children of each die are visited
   recursively.  */

static void
build_abbrev_table (die)
     register dw_die_ref die;
{
  register unsigned long abbrev_id;
  register unsigned long n_alloc;
  register dw_die_ref c;
  register dw_attr_ref d_attr, a_attr;
  for (abbrev_id = 1; abbrev_id < abbrev_die_table_in_use; ++abbrev_id)
    {
      register dw_die_ref abbrev = abbrev_die_table[abbrev_id];

      if (abbrev->die_tag == die->die_tag)
	{
	  if ((abbrev->die_child != NULL) == (die->die_child != NULL))
	    {
	      a_attr = abbrev->die_attr;
	      d_attr = die->die_attr;

	      while (a_attr != NULL && d_attr != NULL)
		{
		  if ((a_attr->dw_attr != d_attr->dw_attr)
		      || (value_format (&a_attr->dw_attr_val)
			  != value_format (&d_attr->dw_attr_val)))
		    break;

		  a_attr = a_attr->dw_attr_next;
		  d_attr = d_attr->dw_attr_next;
		}

	      if (a_attr == NULL && d_attr == NULL)
		break;
	    }
	}
    }

  if (abbrev_id >= abbrev_die_table_in_use)
    {
      if (abbrev_die_table_in_use >= abbrev_die_table_allocated)
	{
	  n_alloc = abbrev_die_table_allocated + ABBREV_DIE_TABLE_INCREMENT;
	  abbrev_die_table 
	    = (dw_die_ref *) xrealloc (abbrev_die_table,
				       sizeof (dw_die_ref) * n_alloc);

	  zero_memory ((char *) &abbrev_die_table[abbrev_die_table_allocated],
		 (n_alloc - abbrev_die_table_allocated) * sizeof (dw_die_ref));
	  abbrev_die_table_allocated = n_alloc;
	}

      ++abbrev_die_table_in_use;
      abbrev_die_table[abbrev_id] = die;
    }

  die->die_abbrev = abbrev_id;
  for (c = die->die_child; c != NULL; c = c->die_sib)
    build_abbrev_table (c);
}

/* Return the size of a string, including the null byte.

   This used to treat backslashes as escapes, and hence they were not included
   in the count.  However, that conflicts with what ASM_OUTPUT_ASCII does,
   which treats a backslash as a backslash, escaping it if necessary, and hence
   we must include them in the count.  */

static unsigned long
size_of_string (str)
     register char *str;
{
  return strlen (str) + 1;
}

/* Return the size of a location descriptor.  */

static unsigned long
size_of_loc_descr (loc)
     register dw_loc_descr_ref loc;
{
  register unsigned long size = 1;

  switch (loc->dw_loc_opc)
    {
    case DW_OP_addr:
      size += PTR_SIZE;
      break;
    case DW_OP_const1u:
    case DW_OP_const1s:
      size += 1;
      break;
    case DW_OP_const2u:
    case DW_OP_const2s:
      size += 2;
      break;
    case DW_OP_const4u:
    case DW_OP_const4s:
      size += 4;
      break;
    case DW_OP_const8u:
    case DW_OP_const8s:
      size += 8;
      break;
    case DW_OP_constu:
      size += size_of_uleb128 (loc->dw_loc_oprnd1.v.val_unsigned);
      break;
    case DW_OP_consts:
      size += size_of_sleb128 (loc->dw_loc_oprnd1.v.val_int);
      break;
    case DW_OP_pick:
      size += 1;
      break;
    case DW_OP_plus_uconst:
      size += size_of_uleb128 (loc->dw_loc_oprnd1.v.val_unsigned);
      break;
    case DW_OP_skip:
    case DW_OP_bra:
      size += 2;
      break;
    case DW_OP_breg0:
    case DW_OP_breg1:
    case DW_OP_breg2:
    case DW_OP_breg3:
    case DW_OP_breg4:
    case DW_OP_breg5:
    case DW_OP_breg6:
    case DW_OP_breg7:
    case DW_OP_breg8:
    case DW_OP_breg9:
    case DW_OP_breg10:
    case DW_OP_breg11:
    case DW_OP_breg12:
    case DW_OP_breg13:
    case DW_OP_breg14:
    case DW_OP_breg15:
    case DW_OP_breg16:
    case DW_OP_breg17:
    case DW_OP_breg18:
    case DW_OP_breg19:
    case DW_OP_breg20:
    case DW_OP_breg21:
    case DW_OP_breg22:
    case DW_OP_breg23:
    case DW_OP_breg24:
    case DW_OP_breg25:
    case DW_OP_breg26:
    case DW_OP_breg27:
    case DW_OP_breg28:
    case DW_OP_breg29:
    case DW_OP_breg30:
    case DW_OP_breg31:
      size += size_of_sleb128 (loc->dw_loc_oprnd1.v.val_int);
      break;
    case DW_OP_regx:
      size += size_of_uleb128 (loc->dw_loc_oprnd1.v.val_unsigned);
      break;
    case DW_OP_fbreg:
      size += size_of_sleb128 (loc->dw_loc_oprnd1.v.val_int);
      break;
    case DW_OP_bregx:
      size += size_of_uleb128 (loc->dw_loc_oprnd1.v.val_unsigned);
      size += size_of_sleb128 (loc->dw_loc_oprnd2.v.val_int);
      break;
    case DW_OP_piece:
      size += size_of_uleb128 (loc->dw_loc_oprnd1.v.val_unsigned);
      break;
    case DW_OP_deref_size:
    case DW_OP_xderef_size:
      size += 1;
      break;
    default:
      break;
    }

  return size;
}

/* Return the size of a series of location descriptors.  */

static unsigned long
size_of_locs (loc)
     register dw_loc_descr_ref loc;
{
  register unsigned long size = 0;

  for (; loc != NULL; loc = loc->dw_loc_next)
    size += size_of_loc_descr (loc);

  return size;
}

/* Return the power-of-two number of bytes necessary to represent VALUE.  */

static int
constant_size (value)
     long unsigned value;
{
  int log;

  if (value == 0)
    log = 0;
  else
    log = floor_log2 (value);

  log = log / 8;
  log = 1 << (floor_log2 (log) + 1);

  return log;
}

/* Return the size of a DIE, as it is represented in the
   .debug_info section.  */

static unsigned long
size_of_die (die)
     register dw_die_ref die;
{
  register unsigned long size = 0;
  register dw_attr_ref a;

  size += size_of_uleb128 (die->die_abbrev);
  for (a = die->die_attr; a != NULL; a = a->dw_attr_next)
    {
      switch (a->dw_attr_val.val_class)
	{
	case dw_val_class_addr:
	  size += PTR_SIZE;
	  break;
	case dw_val_class_loc:
	  {
	    register unsigned long lsize
	      = size_of_locs (a->dw_attr_val.v.val_loc);

	    /* Block length.  */
	    size += constant_size (lsize);
	    size += lsize;
	  }
	  break;
	case dw_val_class_const:
	  size += 4;
	  break;
	case dw_val_class_unsigned_const:
	  size += constant_size (a->dw_attr_val.v.val_unsigned);
	  break;
	case dw_val_class_long_long:
	  size += 1 + 8; /* block */
	  break;
	case dw_val_class_float:
	  size += 1 + a->dw_attr_val.v.val_float.length * 4; /* block */
	  break;
	case dw_val_class_flag:
	  size += 1;
	  break;
	case dw_val_class_die_ref:
	  size += DWARF_OFFSET_SIZE;
	  break;
	case dw_val_class_fde_ref:
	  size += DWARF_OFFSET_SIZE;
	  break;
	case dw_val_class_lbl_id:
	  size += PTR_SIZE;
	  break;
	case dw_val_class_section_offset:
	  size += DWARF_OFFSET_SIZE;
	  break;
	case dw_val_class_str:
	  size += size_of_string (a->dw_attr_val.v.val_str);
	  break;
	default:
	  abort ();
	}
    }

  return size;
}

/* Size the debugging information associated with a given DIE.
   Visits the DIE's children recursively.  Updates the global
   variable next_die_offset, on each time through.  Uses the
   current value of next_die_offset to update the die_offset
   field in each DIE.  */

static void
calc_die_sizes (die)
     dw_die_ref die;
{
  register dw_die_ref c;
  die->die_offset = next_die_offset;
  next_die_offset += size_of_die (die);

  for (c = die->die_child; c != NULL; c = c->die_sib)
    calc_die_sizes (c);

  if (die->die_child != NULL)
    /* Count the null byte used to terminate sibling lists.  */
    next_die_offset += 1;
}

/* Return the size of the line information prolog generated for the
   compilation unit.  */

static unsigned long
size_of_line_prolog ()
{
  register unsigned long size;
  register unsigned long ft_index;

  size = DWARF_LINE_PROLOG_HEADER_SIZE;

  /* Count the size of the table giving number of args for each
     standard opcode.  */
  size += DWARF_LINE_OPCODE_BASE - 1;

  /* Include directory table is empty (at present).  Count only the
     null byte used to terminate the table.  */
  size += 1;

  for (ft_index = 1; ft_index < file_table_in_use; ++ft_index)
    {
      /* File name entry.  */
      size += size_of_string (file_table[ft_index]);

      /* Include directory index.  */
      size += size_of_uleb128 (0);

      /* Modification time.  */
      size += size_of_uleb128 (0);

      /* File length in bytes.  */
      size += size_of_uleb128 (0);
    }

  /* Count the file table terminator.  */
  size += 1;
  return size;
}

/* Return the size of the line information generated for this
   compilation unit.  */

static unsigned long
size_of_line_info ()
{
  register unsigned long size;
  register unsigned long lt_index;
  register unsigned long current_line;
  register long line_offset;
  register long line_delta;
  register unsigned long current_file;
  register unsigned long function;
  unsigned long size_of_set_address;

  /* Size of a DW_LNE_set_address instruction.  */
  size_of_set_address = 1 + size_of_uleb128 (1 + PTR_SIZE) + 1 + PTR_SIZE;

  /* Version number.  */
  size = 2;

  /* Prolog length specifier.  */
  size += DWARF_OFFSET_SIZE;

  /* Prolog.  */
  size += size_of_line_prolog ();

  /* Set address register instruction.  */
  size += size_of_set_address;

  current_file = 1;
  current_line = 1;
  for (lt_index = 1; lt_index < line_info_table_in_use; ++lt_index)
    {
      register dw_line_info_ref line_info;

      /* Advance pc instruction.  */
      /* ??? See the DW_LNS_advance_pc comment in output_line_info.  */
      if (0)
	size += 1 + 2;
      else
	size += size_of_set_address;

      line_info = &line_info_table[lt_index];
      if (line_info->dw_file_num != current_file)
	{
	  /* Set file number instruction.  */
	  size += 1;
	  current_file = line_info->dw_file_num;
	  size += size_of_uleb128 (current_file);
	}

      if (line_info->dw_line_num != current_line)
	{
	  line_offset = line_info->dw_line_num - current_line;
	  line_delta = line_offset - DWARF_LINE_BASE;
	  current_line = line_info->dw_line_num;
	  if (line_delta >= 0 && line_delta < (DWARF_LINE_RANGE - 1))
	    /* 1-byte special line number instruction.  */
	    size += 1;
	  else
	    {
	      /* Advance line instruction.  */
	      size += 1;
	      size += size_of_sleb128 (line_offset);
	      /* Generate line entry instruction.  */
	      size += 1;
	    }
	}
    }

  /* Advance pc instruction.  */
  if (0)
    size += 1 + 2;
  else
    size += size_of_set_address;

  /* End of line number info. marker.  */
  size += 1 + size_of_uleb128 (1) + 1;

  function = 0;
  current_file = 1;
  current_line = 1;
  for (lt_index = 0; lt_index < separate_line_info_table_in_use; )
    {
      register dw_separate_line_info_ref line_info
	= &separate_line_info_table[lt_index];
      if (function != line_info->function)
	{
	  function = line_info->function;
	  /* Set address register instruction.  */
	  size += size_of_set_address;
	}
      else
	{
	  /* Advance pc instruction.  */
	  if (0)
	    size += 1 + 2;
	  else
	    size += size_of_set_address;
	}

      if (line_info->dw_file_num != current_file)
	{
	  /* Set file number instruction.  */
	  size += 1;
	  current_file = line_info->dw_file_num;
	  size += size_of_uleb128 (current_file);
	}

      if (line_info->dw_line_num != current_line)
	{
	  line_offset = line_info->dw_line_num - current_line;
	  line_delta = line_offset - DWARF_LINE_BASE;
	  current_line = line_info->dw_line_num;
	  if (line_delta >= 0 && line_delta < (DWARF_LINE_RANGE - 1))
	    /* 1-byte special line number instruction.  */
	    size += 1;
	  else
	    {
	      /* Advance line instruction.  */
	      size += 1;
	      size += size_of_sleb128 (line_offset);

	      /* Generate line entry instruction.  */
	      size += 1;
	    }
	}

      ++lt_index;

      /* If we're done with a function, end its sequence.  */
      if (lt_index == separate_line_info_table_in_use
	  || separate_line_info_table[lt_index].function != function)
	{
	  current_file = 1;
	  current_line = 1;

	  /* Advance pc instruction.  */
	  if (0)
	    size += 1 + 2;
	  else
	    size += size_of_set_address;

	  /* End of line number info. marker.  */
	  size += 1 + size_of_uleb128 (1) + 1;
	}
    }

  return size;
}

/* Return the size of the .debug_pubnames table  generated for the
   compilation unit.  */

static unsigned long
size_of_pubnames ()
{
  register unsigned long size;
  register unsigned i;

  size = DWARF_PUBNAMES_HEADER_SIZE;
  for (i = 0; i < pubname_table_in_use; ++i)
    {
      register pubname_ref p = &pubname_table[i];
      size += DWARF_OFFSET_SIZE + size_of_string (p->name);
    }

  size += DWARF_OFFSET_SIZE;
  return size;
}

/* Return the size of the information in the .debug_aranges section.  */

static unsigned long
size_of_aranges ()
{
  register unsigned long size;

  size = DWARF_ARANGES_HEADER_SIZE;

  /* Count the address/length pair for this compilation unit.  */
  size += 2 * PTR_SIZE;
  size += 2 * PTR_SIZE * arange_table_in_use;

  /* Count the two zero words used to terminated the address range table.  */
  size += 2 * PTR_SIZE;
  return size;
}

/* Select the encoding of an attribute value.  */

static enum dwarf_form
value_format (v)
     dw_val_ref v;
{
  switch (v->val_class)
    {
    case dw_val_class_addr:
      return DW_FORM_addr;
    case dw_val_class_loc:
      switch (constant_size (size_of_locs (v->v.val_loc)))
	{
	case 1:
	  return DW_FORM_block1;
	case 2:
	  return DW_FORM_block2;
	default:
	  abort ();
	}
    case dw_val_class_const:
      return DW_FORM_data4;
    case dw_val_class_unsigned_const:
      switch (constant_size (v->v.val_unsigned))
	{
	case 1:
	  return DW_FORM_data1;
	case 2:
	  return DW_FORM_data2;
	case 4:
	  return DW_FORM_data4;
	case 8:
	  return DW_FORM_data8;
	default:
	  abort ();
	}
    case dw_val_class_long_long:
      return DW_FORM_block1;
    case dw_val_class_float:
      return DW_FORM_block1;
    case dw_val_class_flag:
      return DW_FORM_flag;
    case dw_val_class_die_ref:
      return DW_FORM_ref;
    case dw_val_class_fde_ref:
      return DW_FORM_data;
    case dw_val_class_lbl_id:
      return DW_FORM_addr;
    case dw_val_class_section_offset:
      return DW_FORM_data;
    case dw_val_class_str:
      return DW_FORM_string;
    default:
      abort ();
    }
}

/* Output the encoding of an attribute value.  */

static void
output_value_format (v)
     dw_val_ref v;
{
  enum dwarf_form form = value_format (v);

  output_uleb128 (form);
  if (flag_debug_asm)
    fprintf (asm_out_file, " (%s)", dwarf_form_name (form));

  fputc ('\n', asm_out_file);
}

/* Output the .debug_abbrev section which defines the DIE abbreviation
   table.  */

static void
output_abbrev_section ()
{
  unsigned long abbrev_id;

  dw_attr_ref a_attr;
  for (abbrev_id = 1; abbrev_id < abbrev_die_table_in_use; ++abbrev_id)
    {
      register dw_die_ref abbrev = abbrev_die_table[abbrev_id];

      output_uleb128 (abbrev_id);
      if (flag_debug_asm)
	fprintf (asm_out_file, " (abbrev code)");

      fputc ('\n', asm_out_file);
      output_uleb128 (abbrev->die_tag);
      if (flag_debug_asm)
	fprintf (asm_out_file, " (TAG: %s)",
		 dwarf_tag_name (abbrev->die_tag));

      fputc ('\n', asm_out_file);
      fprintf (asm_out_file, "\t%s\t0x%x", ASM_BYTE_OP,
	       abbrev->die_child != NULL ? DW_children_yes : DW_children_no);

      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s %s",
		 ASM_COMMENT_START,
		 (abbrev->die_child != NULL
		  ? "DW_children_yes" : "DW_children_no"));

      fputc ('\n', asm_out_file);

      for (a_attr = abbrev->die_attr; a_attr != NULL;
	   a_attr = a_attr->dw_attr_next)
	{
	  output_uleb128 (a_attr->dw_attr);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, " (%s)",
		     dwarf_attr_name (a_attr->dw_attr));

	  fputc ('\n', asm_out_file);
	  output_value_format (&a_attr->dw_attr_val);
	}

      fprintf (asm_out_file, "\t%s\t0,0\n", ASM_BYTE_OP);
    }
}

/* Output location description stack opcode's operands (if any).  */

static void
output_loc_operands (loc)
     register dw_loc_descr_ref loc;
{
  register dw_val_ref val1 = &loc->dw_loc_oprnd1;
  register dw_val_ref val2 = &loc->dw_loc_oprnd2;

  switch (loc->dw_loc_opc)
    {
    case DW_OP_addr:
      ASM_OUTPUT_DWARF_ADDR_CONST (asm_out_file, val1->v.val_addr);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_const1u:
    case DW_OP_const1s:
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, val1->v.val_flag);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_const2u:
    case DW_OP_const2s:
      ASM_OUTPUT_DWARF_DATA2 (asm_out_file, val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_const4u:
    case DW_OP_const4s:
      ASM_OUTPUT_DWARF_DATA4 (asm_out_file, val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_const8u:
    case DW_OP_const8s:
      abort ();
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_constu:
      output_uleb128 (val1->v.val_unsigned);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_consts:
      output_sleb128 (val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_pick:
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_plus_uconst:
      output_uleb128 (val1->v.val_unsigned);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_skip:
    case DW_OP_bra:
      ASM_OUTPUT_DWARF_DATA2 (asm_out_file, val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_breg0:
    case DW_OP_breg1:
    case DW_OP_breg2:
    case DW_OP_breg3:
    case DW_OP_breg4:
    case DW_OP_breg5:
    case DW_OP_breg6:
    case DW_OP_breg7:
    case DW_OP_breg8:
    case DW_OP_breg9:
    case DW_OP_breg10:
    case DW_OP_breg11:
    case DW_OP_breg12:
    case DW_OP_breg13:
    case DW_OP_breg14:
    case DW_OP_breg15:
    case DW_OP_breg16:
    case DW_OP_breg17:
    case DW_OP_breg18:
    case DW_OP_breg19:
    case DW_OP_breg20:
    case DW_OP_breg21:
    case DW_OP_breg22:
    case DW_OP_breg23:
    case DW_OP_breg24:
    case DW_OP_breg25:
    case DW_OP_breg26:
    case DW_OP_breg27:
    case DW_OP_breg28:
    case DW_OP_breg29:
    case DW_OP_breg30:
    case DW_OP_breg31:
      output_sleb128 (val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_regx:
      output_uleb128 (val1->v.val_unsigned);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_fbreg:
      output_sleb128 (val1->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_bregx:
      output_uleb128 (val1->v.val_unsigned);
      fputc ('\n', asm_out_file);
      output_sleb128 (val2->v.val_int);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_piece:
      output_uleb128 (val1->v.val_unsigned);
      fputc ('\n', asm_out_file);
      break;
    case DW_OP_deref_size:
    case DW_OP_xderef_size:
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, val1->v.val_flag);
      fputc ('\n', asm_out_file);
      break;
    default:
      break;
    }
}

/* Compute the offset of a sibling.  */

static unsigned long
sibling_offset (die)
     dw_die_ref die;
{
  unsigned long offset;

  if (die->die_child_last == NULL)
    offset = die->die_offset + size_of_die (die);
  else
    offset = sibling_offset (die->die_child_last) + 1;

  return offset;
}

/* Output the DIE and its attributes.  Called recursively to generate
   the definitions of each child DIE.  */

static void
output_die (die)
     register dw_die_ref die;
{
  register dw_attr_ref a;
  register dw_die_ref c;
  register unsigned long ref_offset;
  register unsigned long size;
  register dw_loc_descr_ref loc;

  output_uleb128 (die->die_abbrev);
  if (flag_debug_asm)
    fprintf (asm_out_file, " (DIE (0x%lx) %s)",
	     die->die_offset, dwarf_tag_name (die->die_tag));

  fputc ('\n', asm_out_file);

  for (a = die->die_attr; a != NULL; a = a->dw_attr_next)
    {
      switch (a->dw_attr_val.val_class)
	{
	case dw_val_class_addr:
	  ASM_OUTPUT_DWARF_ADDR_CONST (asm_out_file,
				       a->dw_attr_val.v.val_addr);
	  break;

	case dw_val_class_loc:
	  size = size_of_locs (a->dw_attr_val.v.val_loc);

	  /* Output the block length for this list of location operations.  */
	  switch (constant_size (size))
	    {
	    case 1:
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, size);
	      break;
	    case 2:
	      ASM_OUTPUT_DWARF_DATA2 (asm_out_file, size);
	      break;
	    default:
	      abort ();
	    }

	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s %s",
		     ASM_COMMENT_START, dwarf_attr_name (a->dw_attr));

	  fputc ('\n', asm_out_file);
	  for (loc = a->dw_attr_val.v.val_loc; loc != NULL;
	       loc = loc->dw_loc_next)
	    {
	      /* Output the opcode.  */
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, loc->dw_loc_opc);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s %s", ASM_COMMENT_START,
			 dwarf_stack_op_name (loc->dw_loc_opc));

	      fputc ('\n', asm_out_file);

	      /* Output the operand(s) (if any).  */
	      output_loc_operands (loc);
	    }
	  break;

	case dw_val_class_const:
	  ASM_OUTPUT_DWARF_DATA4 (asm_out_file, a->dw_attr_val.v.val_int);
	  break;

	case dw_val_class_unsigned_const:
	  switch (constant_size (a->dw_attr_val.v.val_unsigned))
	    {
	    case 1:
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file,
				      a->dw_attr_val.v.val_unsigned);
	      break;
	    case 2:
	      ASM_OUTPUT_DWARF_DATA2 (asm_out_file,
				      a->dw_attr_val.v.val_unsigned);
	      break;
	    case 4:
	      ASM_OUTPUT_DWARF_DATA4 (asm_out_file,
				      a->dw_attr_val.v.val_unsigned);
	      break;
	    case 8:
	      ASM_OUTPUT_DWARF_DATA8 (asm_out_file,
				      a->dw_attr_val.v.val_long_long.hi,
				      a->dw_attr_val.v.val_long_long.low);
	      break;
	    default:
	      abort ();
	    }
	  break;

	case dw_val_class_long_long:
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 8);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s %s",
		   ASM_COMMENT_START, dwarf_attr_name (a->dw_attr));

	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DATA8 (asm_out_file,
				  a->dw_attr_val.v.val_long_long.hi,
				  a->dw_attr_val.v.val_long_long.low);

	  if (flag_debug_asm)
	    fprintf (asm_out_file,
		     "\t%s long long constant", ASM_COMMENT_START);
	  
	  fputc ('\n', asm_out_file);
	  break;

	case dw_val_class_float:
	  {
	    register unsigned int i;
	    ASM_OUTPUT_DWARF_DATA1 (asm_out_file,
				    a->dw_attr_val.v.val_float.length * 4);
	    if (flag_debug_asm)
	      fprintf (asm_out_file, "\t%s %s",
		       ASM_COMMENT_START, dwarf_attr_name (a->dw_attr));

	    fputc ('\n', asm_out_file);
	    for (i = 0; i < a->dw_attr_val.v.val_float.length; ++i)
	      {
		ASM_OUTPUT_DWARF_DATA4 (asm_out_file,
					a->dw_attr_val.v.val_float.array[i]);
		if (flag_debug_asm)
		  fprintf (asm_out_file, "\t%s fp constant word %u",
			   ASM_COMMENT_START, i);

		fputc ('\n', asm_out_file);
	      }
	  break;
	  }

	case dw_val_class_flag:
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, a->dw_attr_val.v.val_flag);
	  break;

	case dw_val_class_die_ref:
	  if (a->dw_attr_val.v.val_die_ref != NULL)
	    ref_offset = a->dw_attr_val.v.val_die_ref->die_offset;
	  else if (a->dw_attr == DW_AT_sibling)
	    ref_offset = sibling_offset(die);
	  else
	    abort ();

	  ASM_OUTPUT_DWARF_DATA (asm_out_file, ref_offset);
	  break;

	case dw_val_class_fde_ref:
	  {
	    char l1[20];
	    ASM_GENERATE_INTERNAL_LABEL
	      (l1, FDE_AFTER_SIZE_LABEL, a->dw_attr_val.v.val_fde_index * 2);
	    ASM_OUTPUT_DWARF_OFFSET (asm_out_file, l1);
	    fprintf (asm_out_file, " - %d", DWARF_OFFSET_SIZE);
	  }
	  break;

	case dw_val_class_lbl_id:
	  ASM_OUTPUT_DWARF_ADDR (asm_out_file, a->dw_attr_val.v.val_lbl_id);
	  break;

	case dw_val_class_section_offset:
	  ASM_OUTPUT_DWARF_OFFSET (asm_out_file,
				   stripattributes
				   (a->dw_attr_val.v.val_section));
	  break;

	case dw_val_class_str:
	  if (flag_debug_asm)
	    ASM_OUTPUT_DWARF_STRING (asm_out_file, a->dw_attr_val.v.val_str);
	  else
	    ASM_OUTPUT_ASCII (asm_out_file,
			      a->dw_attr_val.v.val_str,
			      (int) strlen (a->dw_attr_val.v.val_str) + 1);
	  break;

	default:
	  abort ();
	}

      if (a->dw_attr_val.val_class != dw_val_class_loc
	  && a->dw_attr_val.val_class != dw_val_class_long_long
	  && a->dw_attr_val.val_class != dw_val_class_float)
	{
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s %s",
		     ASM_COMMENT_START, dwarf_attr_name (a->dw_attr));

	  fputc ('\n', asm_out_file);
	}
    }

  for (c = die->die_child; c != NULL; c = c->die_sib)
    output_die (c);

  if (die->die_child != NULL)
    {
      /* Add null byte to terminate sibling list. */
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s end of children of DIE 0x%lx",
		 ASM_COMMENT_START, die->die_offset);

      fputc ('\n', asm_out_file);
    }
}

/* Output the compilation unit that appears at the beginning of the
   .debug_info section, and precedes the DIE descriptions.  */

static void
output_compilation_unit_header ()
{
  ASM_OUTPUT_DWARF_DATA (asm_out_file, next_die_offset - DWARF_OFFSET_SIZE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Length of Compilation Unit Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA2 (asm_out_file, DWARF_VERSION);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DWARF version number", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_OFFSET (asm_out_file, stripattributes (ABBREV_SECTION));
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Offset Into Abbrev. Section",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, PTR_SIZE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Pointer Size (in bytes)", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
}

/* The DWARF2 pubname for a nested thingy looks like "A::f".  The output
   of decl_printable_name for C++ looks like "A::f(int)".  Let's drop the
   argument list, and maybe the scope.  */

static char *
dwarf2_name (decl, scope)
     tree decl;
     int scope;
{
  return (*decl_printable_name) (decl, scope ? 1 : 0);
}

/* Add a new entry to .debug_pubnames if appropriate.  */

static void
add_pubname (decl, die)
     tree decl;
     dw_die_ref die;
{
  pubname_ref p;

  if (! TREE_PUBLIC (decl))
    return;

  if (pubname_table_in_use == pubname_table_allocated)
    {
      pubname_table_allocated += PUBNAME_TABLE_INCREMENT;
      pubname_table = (pubname_ref) xrealloc
	(pubname_table, pubname_table_allocated * sizeof (pubname_entry));
    }

  p = &pubname_table[pubname_table_in_use++];
  p->die = die;

  p->name = xstrdup (dwarf2_name (decl, 1));
}

/* Output the public names table used to speed up access to externally
   visible names.  For now, only generate entries for externally
   visible procedures.  */

static void
output_pubnames ()
{
  register unsigned i;
  register unsigned long pubnames_length = size_of_pubnames ();

  ASM_OUTPUT_DWARF_DATA (asm_out_file, pubnames_length);

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Length of Public Names Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA2 (asm_out_file, DWARF_VERSION);

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DWARF Version", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_OFFSET (asm_out_file, stripattributes (DEBUG_INFO_SECTION));
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Offset of Compilation Unit Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA (asm_out_file, next_die_offset);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Compilation Unit Length", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  for (i = 0; i < pubname_table_in_use; ++i)
    {
      register pubname_ref pub = &pubname_table[i];

      ASM_OUTPUT_DWARF_DATA (asm_out_file, pub->die->die_offset);
      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s DIE offset", ASM_COMMENT_START);

      fputc ('\n', asm_out_file);

      if (flag_debug_asm)
	{
	  ASM_OUTPUT_DWARF_STRING (asm_out_file, pub->name);
	  fprintf (asm_out_file, "%s external name", ASM_COMMENT_START);
	}
      else
	{
	  ASM_OUTPUT_ASCII (asm_out_file, pub->name,
			    (int) strlen (pub->name) + 1);
	}

      fputc ('\n', asm_out_file);
    }

  ASM_OUTPUT_DWARF_DATA (asm_out_file, 0);
  fputc ('\n', asm_out_file);
}

/* Add a new entry to .debug_aranges if appropriate.  */

static void
add_arange (decl, die)
     tree decl;
     dw_die_ref die;
{
  if (! DECL_SECTION_NAME (decl))
    return;

  if (arange_table_in_use == arange_table_allocated)
    {
      arange_table_allocated += ARANGE_TABLE_INCREMENT;
      arange_table
	= (arange_ref) xrealloc (arange_table,
				 arange_table_allocated * sizeof (dw_die_ref));
    }

  arange_table[arange_table_in_use++] = die;
}

/* Output the information that goes into the .debug_aranges table.
   Namely, define the beginning and ending address range of the
   text section generated for this compilation unit.  */

static void
output_aranges ()
{
  register unsigned i;
  register unsigned long aranges_length = size_of_aranges ();

  ASM_OUTPUT_DWARF_DATA (asm_out_file, aranges_length);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Length of Address Ranges Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA2 (asm_out_file, DWARF_VERSION);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DWARF Version", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_OFFSET (asm_out_file, stripattributes (DEBUG_INFO_SECTION));
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Offset of Compilation Unit Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, PTR_SIZE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Size of Address", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Size of Segment Descriptor",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA4 (asm_out_file, 4);
  if (PTR_SIZE == 8)
    fprintf (asm_out_file, ",0,0");

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Pad to %d byte boundary",
	     ASM_COMMENT_START, 2 * PTR_SIZE);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_ADDR (asm_out_file, stripattributes (TEXT_SECTION));
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Address", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_ADDR_DELTA (asm_out_file, text_end_label,
			       stripattributes (TEXT_SECTION));
  if (flag_debug_asm)
    fprintf (asm_out_file, "%s Length", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  for (i = 0; i < arange_table_in_use; ++i)
    {
      dw_die_ref a = arange_table[i];

      if (a->die_tag == DW_TAG_subprogram)
	ASM_OUTPUT_DWARF_ADDR (asm_out_file, get_AT_low_pc (a));
      else
	{
	  char *name = get_AT_string (a, DW_AT_MIPS_linkage_name);
	  if (! name)
	    name = get_AT_string (a, DW_AT_name);

	  ASM_OUTPUT_DWARF_ADDR (asm_out_file, name);
	}

      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s Address", ASM_COMMENT_START);

      fputc ('\n', asm_out_file);
      if (a->die_tag == DW_TAG_subprogram)
	ASM_OUTPUT_DWARF_ADDR_DELTA (asm_out_file, get_AT_hi_pc (a),
				     get_AT_low_pc (a));
      else
	ASM_OUTPUT_DWARF_ADDR_DATA (asm_out_file,
				    get_AT_unsigned (a, DW_AT_byte_size));

      if (flag_debug_asm)
	fprintf (asm_out_file, "%s Length", ASM_COMMENT_START);

      fputc ('\n', asm_out_file);
    }

  /* Output the terminator words.  */
  ASM_OUTPUT_DWARF_ADDR_DATA (asm_out_file, 0);
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_ADDR_DATA (asm_out_file, 0);
  fputc ('\n', asm_out_file);
}

/* Output the source line number correspondence information.  This
   information goes into the .debug_line section.

   If the format of this data changes, then the function size_of_line_info
   must also be adjusted the same way.  */

static void
output_line_info ()
{
  char l1[20], l2[20], p1[20], p2[20];
  char line_label[MAX_ARTIFICIAL_LABEL_BYTES];
  char prev_line_label[MAX_ARTIFICIAL_LABEL_BYTES];
  register unsigned opc;
  register unsigned n_op_args;
  register unsigned long ft_index;
  register unsigned long lt_index;
  register unsigned long current_line;
  register long line_offset;
  register long line_delta;
  register unsigned long current_file;
  register unsigned long function;

  ASM_GENERATE_INTERNAL_LABEL (l1, LINE_NUMBER_BEGIN_LABEL, 0);
  ASM_GENERATE_INTERNAL_LABEL (l2, LINE_NUMBER_END_LABEL, 0);
  ASM_GENERATE_INTERNAL_LABEL (p1, LN_PROLOG_AS_LABEL, 0);
  ASM_GENERATE_INTERNAL_LABEL (p2, LN_PROLOG_END_LABEL, 0);

  if (flag_legacy_debug_info)
    ASM_OUTPUT_DWARF_DATA (asm_out_file, size_of_line_info ());
  else
    ASM_OUTPUT_DWARF_DELTA (asm_out_file, l2, l1);

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Length of Source Line Info.",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);

  if (!flag_legacy_debug_info)
    ASM_OUTPUT_LABEL(asm_out_file, l1);

  ASM_OUTPUT_DWARF_DATA2 (asm_out_file, DWARF_VERSION);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DWARF Version", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);

  if (flag_legacy_debug_info)
    ASM_OUTPUT_DWARF_DATA (asm_out_file, size_of_line_prolog ());
  else
    ASM_OUTPUT_DWARF_DELTA (asm_out_file, p2, p1);

  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Prolog Length", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);

  if (!flag_legacy_debug_info)
    ASM_OUTPUT_LABEL(asm_out_file, p1);

  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DWARF_LINE_MIN_INSTR_LENGTH);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Minimum Instruction Length",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DWARF_LINE_DEFAULT_IS_STMT_START);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Default is_stmt_start flag",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  fprintf (asm_out_file, "\t%s\t%d", ASM_BYTE_OP, DWARF_LINE_BASE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Line Base Value (Special Opcodes)",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  fprintf (asm_out_file, "\t%s\t%u", ASM_BYTE_OP, DWARF_LINE_RANGE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Line Range Value (Special Opcodes)",
	     ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  fprintf (asm_out_file, "\t%s\t%u", ASM_BYTE_OP, DWARF_LINE_OPCODE_BASE);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s Special Opcode Base", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  for (opc = 1; opc < DWARF_LINE_OPCODE_BASE; ++opc)
    {
      switch (opc)
	{
	case DW_LNS_advance_pc:
	case DW_LNS_advance_line:
	case DW_LNS_set_file:
	case DW_LNS_set_column:
	case DW_LNS_fixed_advance_pc:
	  n_op_args = 1;
	  break;
	default:
	  n_op_args = 0;
	  break;
	}
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, n_op_args);
      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s opcode: 0x%x has %d args",
		 ASM_COMMENT_START, opc, n_op_args);
      fputc ('\n', asm_out_file);
    }

  if (flag_debug_asm)
    fprintf (asm_out_file, "%s Include Directory Table\n", ASM_COMMENT_START);

  /* Include directory table is empty, at present */
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
  fputc ('\n', asm_out_file);
  if (flag_debug_asm)
    fprintf (asm_out_file, "%s File Name Table\n", ASM_COMMENT_START);

  for (ft_index = 1; ft_index < file_table_in_use; ++ft_index)
    {
      if (flag_debug_asm)
	{
	  ASM_OUTPUT_DWARF_STRING (asm_out_file, file_table[ft_index]);
	  fprintf (asm_out_file, "%s File Entry: 0x%lx",
		   ASM_COMMENT_START, ft_index);
	}
      else
	{
	  ASM_OUTPUT_ASCII (asm_out_file,
			    file_table[ft_index],
			    (int) strlen (file_table[ft_index]) + 1);
	}

      fputc ('\n', asm_out_file);

      /* Include directory index */
      output_uleb128 (0);
      fputc ('\n', asm_out_file);

      /* Modification time */
      output_uleb128 (0);
      fputc ('\n', asm_out_file);

      /* File length in bytes */
      output_uleb128 (0);
      fputc ('\n', asm_out_file);
    }

  /* Terminate the file name table */
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
  fputc ('\n', asm_out_file);

  if (!flag_legacy_debug_info)
    ASM_OUTPUT_LABEL (asm_out_file, p2);

  /* Set the address register to the first location in the text section */
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DW_LNE_set_address", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  output_uleb128 (1 + PTR_SIZE);
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_ADDR (asm_out_file, stripattributes (TEXT_SECTION));
  fputc ('\n', asm_out_file);

  /* Generate the line number to PC correspondence table, encoded as
     a series of state machine operations.  */
  current_file = 1;
  current_line = 1;
  strcpy (prev_line_label, stripattributes (TEXT_SECTION));
  for (lt_index = 1; lt_index < line_info_table_in_use; ++lt_index)
    {
      register dw_line_info_ref line_info;

      /* Emit debug info for the address of the current line, choosing
	 the encoding that uses the least amount of space.  */
      /* ??? Unfortunately, we have little choice here currently, and must
	 always use the most general form.  Gcc does not know the address
	 delta itself, so we can't use DW_LNS_advance_pc.  There are no known
	 dwarf2 aware assemblers at this time, so we can't use any special
	 pseudo ops that would allow the assembler to optimally encode this for
	 us.  Many ports do have length attributes which will give an upper
	 bound on the address range.  We could perhaps use length attributes
	 to determine when it is safe to use DW_LNS_fixed_advance_pc.  */
      ASM_GENERATE_INTERNAL_LABEL (line_label, LINE_CODE_LABEL, lt_index);
      if (0)
	{
	  /* This can handle deltas up to 0xffff.  This takes 3 bytes.  */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_fixed_advance_pc);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNS_fixed_advance_pc",
		     ASM_COMMENT_START);

	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DELTA2 (asm_out_file, line_label, prev_line_label);
	  fputc ('\n', asm_out_file);
	}
      else
	{
	  /* This can handle any delta.  This takes 4+PTR_SIZE bytes.  */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNE_set_address",
		     ASM_COMMENT_START);
	  fputc ('\n', asm_out_file);
	  output_uleb128 (1 + PTR_SIZE);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_ADDR (asm_out_file, line_label);
	  fputc ('\n', asm_out_file);
	}
      strcpy (prev_line_label, line_label);

      /* Emit debug info for the source file of the current line, if
	 different from the previous line.  */
      line_info = &line_info_table[lt_index];
      if (line_info->dw_file_num != current_file)
	{
	  current_file = line_info->dw_file_num;
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_set_file);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNS_set_file", ASM_COMMENT_START);

	  fputc ('\n', asm_out_file);
	  output_uleb128 (current_file);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, " (\"%s\")", file_table[current_file]);

	  fputc ('\n', asm_out_file);
	}

      /* Emit debug info for the current line number, choosing the encoding
	 that uses the least amount of space.  */
      line_offset = line_info->dw_line_num - current_line;
      line_delta = line_offset - DWARF_LINE_BASE;
      current_line = line_info->dw_line_num;
      if (line_delta >= 0 && line_delta < (DWARF_LINE_RANGE - 1))
	{
	  /* This can handle deltas from -10 to 234, using the current
	     definitions of DWARF_LINE_BASE and DWARF_LINE_RANGE.  This
	     takes 1 byte.  */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file,
				  DWARF_LINE_OPCODE_BASE + line_delta);
	  if (flag_debug_asm)
	      fprintf (asm_out_file,
		       "\t%s line %ld", ASM_COMMENT_START, current_line);

	  fputc ('\n', asm_out_file);
	}
      else
	{
	  /* This can handle any delta.  This takes at least 4 bytes, depending
	     on the value being encoded.  */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_advance_line);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s advance to line %ld",
		     ASM_COMMENT_START, current_line);

	  fputc ('\n', asm_out_file);
	  output_sleb128 (line_offset);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_copy);
	  fputc ('\n', asm_out_file);
	}
    }

  /* Emit debug info for the address of the end of the function.  */
  if (0)
    {
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_fixed_advance_pc);
      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s DW_LNS_fixed_advance_pc",
		 ASM_COMMENT_START);

      fputc ('\n', asm_out_file);
      ASM_OUTPUT_DWARF_DELTA2 (asm_out_file, text_end_label, prev_line_label);
      fputc ('\n', asm_out_file);
    }
  else
    {
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
      if (flag_debug_asm)
	fprintf (asm_out_file, "\t%s DW_LNE_set_address", ASM_COMMENT_START);
      fputc ('\n', asm_out_file);
      output_uleb128 (1 + PTR_SIZE);
      fputc ('\n', asm_out_file);
      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
      fputc ('\n', asm_out_file);
      ASM_OUTPUT_DWARF_ADDR (asm_out_file, text_end_label);
      fputc ('\n', asm_out_file);
    }

  /* Output the marker for the end of the line number info.  */
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
  if (flag_debug_asm)
    fprintf (asm_out_file, "\t%s DW_LNE_end_sequence", ASM_COMMENT_START);

  fputc ('\n', asm_out_file);
  output_uleb128 (1);
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_end_sequence);
  fputc ('\n', asm_out_file);

  function = 0;
  current_file = 1;
  current_line = 1;
  for (lt_index = 0; lt_index < separate_line_info_table_in_use; )
    {
      register dw_separate_line_info_ref line_info
	= &separate_line_info_table[lt_index];

      /* Emit debug info for the address of the current line.  If this is
	 a new function, or the first line of a function, then we need
	 to handle it differently.  */
      ASM_GENERATE_INTERNAL_LABEL (line_label, SEPARATE_LINE_CODE_LABEL,
				   lt_index);
      if (function != line_info->function)
	{
	  function = line_info->function;

	  /* Set the address register to the first line in the function */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNE_set_address",
		     ASM_COMMENT_START);

	  fputc ('\n', asm_out_file);
	  output_uleb128 (1 + PTR_SIZE);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_ADDR (asm_out_file, line_label);
	  fputc ('\n', asm_out_file);
	}
      else
	{
	  /* ??? See the DW_LNS_advance_pc comment above.  */
	  if (0)
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_fixed_advance_pc);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s DW_LNS_fixed_advance_pc",
			 ASM_COMMENT_START);

	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_DELTA2 (asm_out_file, line_label,
				       prev_line_label);
	      fputc ('\n', asm_out_file);
	    }
	  else
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s DW_LNE_set_address",
			 ASM_COMMENT_START);
	      fputc ('\n', asm_out_file);
	      output_uleb128 (1 + PTR_SIZE);
	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_ADDR (asm_out_file, line_label);
	      fputc ('\n', asm_out_file);
	    }
	}
      strcpy (prev_line_label, line_label);

      /* Emit debug info for the source file of the current line, if
	 different from the previous line.  */
      if (line_info->dw_file_num != current_file)
	{
	  current_file = line_info->dw_file_num;
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_set_file);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNS_set_file", ASM_COMMENT_START);

	  fputc ('\n', asm_out_file);
	  output_uleb128 (current_file);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, " (\"%s\")", file_table[current_file]);

	  fputc ('\n', asm_out_file);
	}

      /* Emit debug info for the current line number, choosing the encoding
	 that uses the least amount of space.  */
      if (line_info->dw_line_num != current_line)
	{
	  line_offset = line_info->dw_line_num - current_line;
	  line_delta = line_offset - DWARF_LINE_BASE;
	  current_line = line_info->dw_line_num;
	  if (line_delta >= 0 && line_delta < (DWARF_LINE_RANGE - 1))
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file,
				      DWARF_LINE_OPCODE_BASE + line_delta);
	      if (flag_debug_asm)
		fprintf (asm_out_file,
			 "\t%s line %ld", ASM_COMMENT_START, current_line);

	      fputc ('\n', asm_out_file);
	    }
	  else
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_advance_line);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s advance to line %ld",
			 ASM_COMMENT_START, current_line);

	      fputc ('\n', asm_out_file);
	      output_sleb128 (line_offset);
	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_copy);
	      fputc ('\n', asm_out_file);
	    }
	}

      ++lt_index;

      /* If we're done with a function, end its sequence.  */
      if (lt_index == separate_line_info_table_in_use
	  || separate_line_info_table[lt_index].function != function)
	{
	  current_file = 1;
	  current_line = 1;

	  /* Emit debug info for the address of the end of the function.  */
	  ASM_GENERATE_INTERNAL_LABEL (line_label, FUNC_END_LABEL, function);
	  if (0)
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNS_fixed_advance_pc);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s DW_LNS_fixed_advance_pc",
			 ASM_COMMENT_START);

	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_DELTA2 (asm_out_file, line_label,
				       prev_line_label);
	      fputc ('\n', asm_out_file);
	    }
	  else
	    {
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
	      if (flag_debug_asm)
		fprintf (asm_out_file, "\t%s DW_LNE_set_address",
			 ASM_COMMENT_START);
	      fputc ('\n', asm_out_file);
	      output_uleb128 (1 + PTR_SIZE);
	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_set_address);
	      fputc ('\n', asm_out_file);
	      ASM_OUTPUT_DWARF_ADDR (asm_out_file, line_label);
	      fputc ('\n', asm_out_file);
	    }

	  /* Output the marker for the end of this sequence.  */
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, 0);
	  if (flag_debug_asm)
	    fprintf (asm_out_file, "\t%s DW_LNE_end_sequence",
		     ASM_COMMENT_START);

	  fputc ('\n', asm_out_file);
	  output_uleb128 (1);
	  fputc ('\n', asm_out_file);
	  ASM_OUTPUT_DWARF_DATA1 (asm_out_file, DW_LNE_end_sequence);
	  fputc ('\n', asm_out_file);
	}
    }

  if (!flag_legacy_debug_info)
    /* Output the marker for the end of the line number info.  */
    ASM_OUTPUT_LABEL (asm_out_file, l2);
}

/* Given a pointer to a BLOCK node return non-zero if (and only if) the node
   in question represents the outermost pair of curly braces (i.e. the "body
   block") of a function or method.

   For any BLOCK node representing a "body block" of a function or method, the
   BLOCK_SUPERCONTEXT of the node will point to another BLOCK node which
   represents the outermost (function) scope for the function or method (i.e.
   the one which includes the formal parameters).  The BLOCK_SUPERCONTEXT of
   *that* node in turn will point to the relevant FUNCTION_DECL node. */

static inline int
is_body_block (stmt)
     register tree stmt;
{
  if (TREE_CODE (stmt) == BLOCK)
    {
      register tree parent = BLOCK_SUPERCONTEXT (stmt);

      if (TREE_CODE (parent) == BLOCK)
	{
	  register tree grandparent = BLOCK_SUPERCONTEXT (parent);

	  if (TREE_CODE (grandparent) == FUNCTION_DECL)
	    return 1;
	}
    }

  return 0;
}

/* Given a pointer to a tree node for some base type, return a pointer to
   a DIE that describes the given type.

   This routine must only be called for GCC type nodes that correspond to
   Dwarf base (fundamental) types.  */

static dw_die_ref
base_type_die (type)
     register tree type;
{
  register dw_die_ref base_type_result;
  register char *type_name;
  register enum dwarf_type encoding;
  register tree name = TYPE_NAME (type);

  if (TREE_CODE (type) == ERROR_MARK
      || TREE_CODE (type) == VOID_TYPE)
    return 0;

  if (TREE_CODE (name) == TYPE_DECL)
    name = DECL_NAME (name);
  type_name = IDENTIFIER_POINTER (name);

  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE:
      /* Carefully distinguish the C character types, without messing
         up if the language is not C. Note that we check only for the names
         that contain spaces; other names might occur by coincidence in other 
         languages.  */
      if (! (TYPE_PRECISION (type) == CHAR_TYPE_SIZE
	     && (type == char_type_node
		 || ! strcmp (type_name, "signed char")
		 || ! strcmp (type_name, "unsigned char"))))
	{
	  if (TREE_UNSIGNED (type))
	    encoding = DW_ATE_unsigned;
	  else
	    encoding = DW_ATE_signed;
	  break;
	}
      /* else fall through */

    case CHAR_TYPE:
      /* GNU Pascal/Ada CHAR type.  Not used in C.  */
      if (TREE_UNSIGNED (type))
	encoding = DW_ATE_unsigned_char;
      else
	encoding = DW_ATE_signed_char;
      break;

    case REAL_TYPE:
      encoding = DW_ATE_float;
      break;

    case COMPLEX_TYPE:
      encoding = DW_ATE_complex_float;
      break;

    case BOOLEAN_TYPE:
      /* GNU FORTRAN/Ada/C++ BOOLEAN type.  */
      encoding = DW_ATE_boolean;
      break;

    default:
      abort (); /* No other TREE_CODEs are Dwarf fundamental types.  */
    }

  base_type_result = new_die (DW_TAG_base_type, comp_unit_die);
  add_AT_string (base_type_result, DW_AT_name, type_name);
  add_AT_unsigned (base_type_result, DW_AT_byte_size,
		   int_size_in_bytes (type));
  add_AT_unsigned (base_type_result, DW_AT_encoding, encoding);

  return base_type_result;
}

/* Given a pointer to an arbitrary ..._TYPE tree node, return non-zero if the
   given input type is a Dwarf "fundamental" type.  Otherwise return null.  */

static inline int
is_base_type (type)
     register tree type;
{
  switch (TREE_CODE (type))
    {
    case ERROR_MARK:
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case COMPLEX_TYPE:
    case BOOLEAN_TYPE:
    case CHAR_TYPE:
      return 1;

    case SET_TYPE:
    case ARRAY_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case ENUMERAL_TYPE:
    case FUNCTION_TYPE:
    case METHOD_TYPE:
    case POINTER_TYPE:
    case REFERENCE_TYPE:
    case FILE_TYPE:
    case OFFSET_TYPE:
    case LANG_TYPE:
      return 0;

    default:
      abort ();
    }

  return 0;
}

/* Given a pointer to an arbitrary ..._TYPE tree node, return a debugging
   entry that chains various modifiers in front of the given type.  */

static dw_die_ref
modified_type_die (type, is_const_type, is_volatile_type, context_die)
     register tree type;
     register int is_const_type;
     register int is_volatile_type;
     register dw_die_ref context_die;
{
  register enum tree_code code = TREE_CODE (type);
  register dw_die_ref mod_type_die = NULL;
  register dw_die_ref sub_die = NULL;
  register tree item_type = NULL;

  if (code != ERROR_MARK)
    {
      type = build_type_variant (type, is_const_type, is_volatile_type);

      mod_type_die = lookup_type_die (type);
      if (mod_type_die)
	return mod_type_die;

      /* Handle C typedef types. */
      if (TYPE_NAME (type) && TREE_CODE (TYPE_NAME (type)) == TYPE_DECL
	  && DECL_ORIGINAL_TYPE (TYPE_NAME (type)))
	{
	  tree dtype = TREE_TYPE (TYPE_NAME (type));
	  if (type == dtype)
	    {
	      /* For a named type, use the typedef.  */
	      gen_type_die (type, context_die);
	      mod_type_die = lookup_type_die (type);
	    }

	  else if (is_const_type < TYPE_READONLY (dtype)
		   || is_volatile_type < TYPE_VOLATILE (dtype))
	    /* cv-unqualified version of named type.  Just use the unnamed
	       type to which it refers.  */
	    mod_type_die
	      = modified_type_die (DECL_ORIGINAL_TYPE (TYPE_NAME (type)),
				   is_const_type, is_volatile_type,
				   context_die);
	  /* Else cv-qualified version of named type; fall through.  */
	}

      if (mod_type_die)
	/* OK */;
      else if (is_const_type)
	{
	  mod_type_die = new_die (DW_TAG_const_type, comp_unit_die);
	  sub_die = modified_type_die (type, 0, is_volatile_type, context_die);
	}
      else if (is_volatile_type)
	{
	  mod_type_die = new_die (DW_TAG_volatile_type, comp_unit_die);
	  sub_die = modified_type_die (type, 0, 0, context_die);
	}
      else if (code == POINTER_TYPE)
	{
	  mod_type_die = new_die (DW_TAG_pointer_type, comp_unit_die);
	  add_AT_unsigned (mod_type_die, DW_AT_byte_size, PTR_SIZE);
#if 0
	  add_AT_unsigned (mod_type_die, DW_AT_address_class, 0);
#endif
	  item_type = TREE_TYPE (type);
	}
      else if (code == REFERENCE_TYPE)
	{
	  mod_type_die = new_die (DW_TAG_reference_type, comp_unit_die);
	  add_AT_unsigned (mod_type_die, DW_AT_byte_size, PTR_SIZE);
#if 0
	  add_AT_unsigned (mod_type_die, DW_AT_address_class, 0);
#endif 
	  item_type = TREE_TYPE (type);
	}
      else if (is_base_type (type))
	mod_type_die = base_type_die (type);
      else
	{
	  gen_type_die (type, context_die);

	  /* We have to get the type_main_variant here (and pass that to the
	     `lookup_type_die' routine) because the ..._TYPE node we have
	     might simply be a *copy* of some original type node (where the
	     copy was created to help us keep track of typedef names) and
	     that copy might have a different TYPE_UID from the original
	     ..._TYPE node.  */
	  mod_type_die = lookup_type_die (type_main_variant (type));
	  if (mod_type_die == NULL)
	    abort ();
	}
    }

  equate_type_number_to_die (type, mod_type_die);
  if (item_type)
    /* We must do this after the equate_type_number_to_die call, in case
       this is a recursive type.  This ensures that the modified_type_die
       recursion will terminate even if the type is recursive.  Recursive
       types are possible in Ada.  */
    sub_die = modified_type_die (item_type,
				 TYPE_READONLY (item_type),
				 TYPE_VOLATILE (item_type),
				 context_die);

  if (sub_die != NULL)
    add_AT_die_ref (mod_type_die, DW_AT_type, sub_die);

  return mod_type_die;
}

/* Given a pointer to an arbitrary ..._TYPE tree node, return true if it is
   an enumerated type.   */

static inline int
type_is_enum (type)
     register tree type;
{
  return TREE_CODE (type) == ENUMERAL_TYPE;
}

/* Return a location descriptor that designates a machine register.  */

static dw_loc_descr_ref
reg_loc_descriptor (rtl)
     register rtx rtl;
{
  register dw_loc_descr_ref loc_result = NULL;
  register unsigned reg = reg_number (rtl);

  if (reg <= 31)
    loc_result = new_loc_descr (DW_OP_reg0 + reg, 0, 0);
  else
    loc_result = new_loc_descr (DW_OP_regx, reg, 0);

  return loc_result;
}

/* Return a location descriptor that designates a base+offset location.  */

static dw_loc_descr_ref
based_loc_descr (reg, offset)
     unsigned reg;
     long int offset;
{
  register dw_loc_descr_ref loc_result;
  /* For the "frame base", we use the frame pointer or stack pointer
     registers, since the RTL for local variables is relative to one of
     them.  */
  register unsigned fp_reg = DBX_REGISTER_NUMBER (frame_pointer_needed
						  ? FRAME_POINTER_REGNUM
						  : STACK_POINTER_REGNUM);

  if (reg == fp_reg)
    loc_result = new_loc_descr (DW_OP_fbreg, offset, 0);
  else if (reg <= 31)
    loc_result = new_loc_descr (DW_OP_breg0 + reg, offset, 0);
  else
    loc_result = new_loc_descr (DW_OP_bregx, reg, offset);

  return loc_result;
}

/* Return true if this RTL expression describes a base+offset calculation.  */

static inline int
is_based_loc (rtl)
     register rtx rtl;
{
    return (GET_CODE (rtl) == PLUS
	    && ((GET_CODE (XEXP (rtl, 0)) == REG
		 && GET_CODE (XEXP (rtl, 1)) == CONST_INT)));
}

/* The following routine converts the RTL for a variable or parameter
   (resident in memory) into an equivalent Dwarf representation of a
   mechanism for getting the address of that same variable onto the top of a
   hypothetical "address evaluation" stack.

   When creating memory location descriptors, we are effectively transforming
   the RTL for a memory-resident object into its Dwarf postfix expression
   equivalent.  This routine recursively descends an RTL tree, turning
   it into Dwarf postfix code as it goes.  */

static dw_loc_descr_ref
mem_loc_descriptor (rtl)
     register rtx rtl;
{
  dw_loc_descr_ref mem_loc_result = NULL;
  /* Note that for a dynamically sized array, the location we will generate a 
     description of here will be the lowest numbered location which is
     actually within the array.  That's *not* necessarily the same as the
     zeroth element of the array.  */

  switch (GET_CODE (rtl))
    {
    case SUBREG:
      /* The case of a subreg may arise when we have a local (register)
         variable or a formal (register) parameter which doesn't quite fill
         up an entire register.  For now, just assume that it is
         legitimate to make the Dwarf info refer to the whole register which
         contains the given subreg.  */
      rtl = XEXP (rtl, 0);

      /* ... fall through ... */

    case REG:
      /* Whenever a register number forms a part of the description of the
         method for calculating the (dynamic) address of a memory resident
         object, DWARF rules require the register number be referred to as 
         a "base register".  This distinction is not based in any way upon
         what category of register the hardware believes the given register
         belongs to.  This is strictly DWARF terminology we're dealing with
         here. Note that in cases where the location of a memory-resident
         data object could be expressed as: OP_ADD (OP_BASEREG (basereg),
         OP_CONST (0)) the actual DWARF location descriptor that we generate
         may just be OP_BASEREG (basereg).  This may look deceptively like
         the object in question was allocated to a register (rather than in
         memory) so DWARF consumers need to be aware of the subtle
         distinction between OP_REG and OP_BASEREG.  */
      mem_loc_result = based_loc_descr (reg_number (rtl), 0);
      break;

    case MEM:
      mem_loc_result = mem_loc_descriptor (XEXP (rtl, 0));
      add_loc_descr (&mem_loc_result, new_loc_descr (DW_OP_deref, 0, 0));
      break;

    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
      mem_loc_result = new_loc_descr (DW_OP_addr, 0, 0);
      mem_loc_result->dw_loc_oprnd1.val_class = dw_val_class_addr;
      mem_loc_result->dw_loc_oprnd1.v.val_addr = addr_to_string (rtl);
      break;

    case PLUS:
      if (is_based_loc (rtl))
	mem_loc_result = based_loc_descr (reg_number (XEXP (rtl, 0)),
					  INTVAL (XEXP (rtl, 1)));
      else
	{
	  add_loc_descr (&mem_loc_result, mem_loc_descriptor (XEXP (rtl, 0)));
	  add_loc_descr (&mem_loc_result, mem_loc_descriptor (XEXP (rtl, 1)));
	  add_loc_descr (&mem_loc_result, new_loc_descr (DW_OP_plus, 0, 0));
	}
      break;

    case MULT:
      /* If a pseudo-reg is optimized away, it is possible for it to
	 be replaced with a MEM containing a multiply.  */
      add_loc_descr (&mem_loc_result, mem_loc_descriptor (XEXP (rtl, 0)));
      add_loc_descr (&mem_loc_result, mem_loc_descriptor (XEXP (rtl, 1)));
      add_loc_descr (&mem_loc_result, new_loc_descr (DW_OP_mul, 0, 0));
      break;

    case CONST_INT:
      mem_loc_result = new_loc_descr (DW_OP_constu, INTVAL (rtl), 0);
      break;

    default:
      abort ();
    }

  return mem_loc_result;
}

/* Return a descriptor that describes the concatenation of two locations.
   This is typically a complex variable.  */

static dw_loc_descr_ref
concat_loc_descriptor (x0, x1)
     register rtx x0, x1;
{
  dw_loc_descr_ref cc_loc_result = NULL;

  if (!is_pseudo_reg (x0)
      && (GET_CODE (x0) != MEM || !is_pseudo_reg (XEXP (x0, 0))))
    add_loc_descr (&cc_loc_result, loc_descriptor (x0));
  add_loc_descr (&cc_loc_result,
	         new_loc_descr (DW_OP_piece, GET_MODE_SIZE (GET_MODE (x0)), 0));

  if (!is_pseudo_reg (x1)
      && (GET_CODE (x1) != MEM || !is_pseudo_reg (XEXP (x1, 0))))
    add_loc_descr (&cc_loc_result, loc_descriptor (x1));
  add_loc_descr (&cc_loc_result,
		 new_loc_descr (DW_OP_piece, GET_MODE_SIZE (GET_MODE (x1)), 0));

  return cc_loc_result;
}

/* Output a proper Dwarf location descriptor for a variable or parameter
   which is either allocated in a register or in a memory location.  For a
   register, we just generate an OP_REG and the register number.  For a
   memory location we provide a Dwarf postfix expression describing how to
   generate the (dynamic) address of the object onto the address stack.  */

static dw_loc_descr_ref
loc_descriptor (rtl)
     register rtx rtl;
{
  dw_loc_descr_ref loc_result = NULL;
  switch (GET_CODE (rtl))
    {
    case SUBREG:
      /* The case of a subreg may arise when we have a local (register)
         variable or a formal (register) parameter which doesn't quite fill
         up an entire register.  For now, just assume that it is
         legitimate to make the Dwarf info refer to the whole register which
         contains the given subreg.  */
      rtl = XEXP (rtl, 0);

      /* ... fall through ... */

    case REG:
      loc_result = reg_loc_descriptor (rtl);
      break;

    case MEM:
      loc_result = mem_loc_descriptor (XEXP (rtl, 0));
      break;

    case CONCAT:
      loc_result = concat_loc_descriptor (XEXP (rtl, 0), XEXP (rtl, 1));
      break;

    default:
      abort ();
    }

  return loc_result;
}

/* Given an unsigned value, round it up to the lowest multiple of `boundary'
   which is not less than the value itself.  */

static inline unsigned
ceiling (value, boundary)
     register unsigned value;
     register unsigned boundary;
{
  return (((value + boundary - 1) / boundary) * boundary);
}

/* Given a pointer to what is assumed to be a FIELD_DECL node, return a
   pointer to the declared type for the relevant field variable, or return
   `integer_type_node' if the given node turns out to be an
   ERROR_MARK node.  */

static inline tree
field_type (decl)
     register tree decl;
{
  register tree type;

  if (TREE_CODE (decl) == ERROR_MARK)
    return integer_type_node;

  type = DECL_BIT_FIELD_TYPE (decl);
  if (type == NULL_TREE)
    type = TREE_TYPE (decl);

  return type;
}

/* Given a pointer to a tree node, assumed to be some kind of a ..._TYPE
   node, return the alignment in bits for the type, or else return
   BITS_PER_WORD if the node actually turns out to be an
   ERROR_MARK node.  */

static inline unsigned
simple_type_align_in_bits (type)
     register tree type;
{
  return (TREE_CODE (type) != ERROR_MARK) ? TYPE_ALIGN (type) : BITS_PER_WORD;
}

/* Given a pointer to a tree node, assumed to be some kind of a ..._TYPE
   node, return the size in bits for the type if it is a constant, or else
   return the alignment for the type if the type's size is not constant, or
   else return BITS_PER_WORD if the type actually turns out to be an
   ERROR_MARK node.  */

static inline unsigned
simple_type_size_in_bits (type)
     register tree type;
{
  if (TREE_CODE (type) == ERROR_MARK)
    return BITS_PER_WORD;
  else
    {
      register tree type_size_tree = TYPE_SIZE (type);

      if (TREE_CODE (type_size_tree) != INTEGER_CST)
	return TYPE_ALIGN (type);

      return (unsigned) TREE_INT_CST_LOW (type_size_tree);
    }
}

/* Given a pointer to what is assumed to be a FIELD_DECL node, compute and
   return the byte offset of the lowest addressed byte of the "containing
   object" for the given FIELD_DECL, or return 0 if we are unable to
   determine what that offset is, either because the argument turns out to
   be a pointer to an ERROR_MARK node, or because the offset is actually
   variable.  (We can't handle the latter case just yet).  */

static unsigned
field_byte_offset (decl)
     register tree decl;
{
  register unsigned type_align_in_bytes;
  register unsigned type_align_in_bits;
  register unsigned type_size_in_bits;
  register unsigned object_offset_in_align_units;
  register unsigned object_offset_in_bits;
  register unsigned object_offset_in_bytes;
  register tree type;
  register tree bitpos_tree;
  register tree field_size_tree;
  register unsigned bitpos_int;
  register unsigned deepest_bitpos;
  register unsigned field_size_in_bits;

  if (TREE_CODE (decl) == ERROR_MARK)
    return 0;

  if (TREE_CODE (decl) != FIELD_DECL)
    abort ();

  type = field_type (decl);

  bitpos_tree = DECL_FIELD_BITPOS (decl);
  field_size_tree = DECL_SIZE (decl);

  /* We cannot yet cope with fields whose positions or sizes are variable, so 
     for now, when we see such things, we simply return 0.  Someday, we may
     be able to handle such cases, but it will be damn difficult.  */
  if (TREE_CODE (bitpos_tree) != INTEGER_CST)
    return 0;
  bitpos_int = (unsigned) TREE_INT_CST_LOW (bitpos_tree);

  if (TREE_CODE (field_size_tree) != INTEGER_CST)
    return 0;

  field_size_in_bits = (unsigned) TREE_INT_CST_LOW (field_size_tree);
  type_size_in_bits = simple_type_size_in_bits (type);
  type_align_in_bits = simple_type_align_in_bits (type);
  type_align_in_bytes = type_align_in_bits / BITS_PER_UNIT;

  /* Note that the GCC front-end doesn't make any attempt to keep track of
     the starting bit offset (relative to the start of the containing
     structure type) of the hypothetical "containing object" for a bit-
     field.  Thus, when computing the byte offset value for the start of the
     "containing object" of a bit-field, we must deduce this information on 
     our own. This can be rather tricky to do in some cases.  For example,
     handling the following structure type definition when compiling for an
     i386/i486 target (which only aligns long long's to 32-bit boundaries)
     can be very tricky:

	 struct S { int field1; long long field2:31; };

     Fortunately, there is a simple rule-of-thumb which can be
     used in such cases.  When compiling for an i386/i486, GCC will allocate
     8 bytes for the structure shown above.  It decides to do this based upon 
     one simple rule for bit-field allocation.  Quite simply, GCC allocates
     each "containing object" for each bit-field at the first (i.e. lowest
     addressed) legitimate alignment boundary (based upon the required
     minimum alignment for the declared type of the field) which it can
     possibly use, subject to the condition that there is still enough
     available space remaining in the containing object (when allocated at
     the selected point) to fully accommodate all of the bits of the
     bit-field itself.  This simple rule makes it obvious why GCC allocates
     8 bytes for each object of the structure type shown above.  When looking
     for a place to allocate the "containing object" for `field2', the
     compiler simply tries to allocate a 64-bit "containing object" at each
     successive 32-bit boundary (starting at zero) until it finds a place to
     allocate that 64- bit field such that at least 31 contiguous (and
     previously unallocated) bits remain within that selected 64 bit field.
     (As it turns out, for the example above, the compiler finds that it is
     OK to allocate the "containing object" 64-bit field at bit-offset zero
     within the structure type.) Here we attempt to work backwards from the
     limited set of facts we're given, and we try to deduce from those facts, 
     where GCC must have believed that the containing object started (within
     the structure type). The value we deduce is then used (by the callers of 
     this routine) to generate DW_AT_location and DW_AT_bit_offset attributes 
     for fields (both bit-fields and, in the case of DW_AT_location, regular
     fields as well).  */

  /* Figure out the bit-distance from the start of the structure to the
     "deepest" bit of the bit-field.  */
  deepest_bitpos = bitpos_int + field_size_in_bits;

  /* This is the tricky part.  Use some fancy footwork to deduce where the
     lowest addressed bit of the containing object must be.  */
  object_offset_in_bits
    = ceiling (deepest_bitpos, type_align_in_bits) - type_size_in_bits;

  /* Compute the offset of the containing object in "alignment units".  */
  object_offset_in_align_units = object_offset_in_bits / type_align_in_bits;

  /* Compute the offset of the containing object in bytes.  */
  object_offset_in_bytes = object_offset_in_align_units * type_align_in_bytes;

  return object_offset_in_bytes;
}

/* The following routines define various Dwarf attributes and any data
   associated with them.  */

/* Add a location description attribute value to a DIE.

   This emits location attributes suitable for whole variables and
   whole parameters.  Note that the location attributes for struct fields are
   generated by the routine `data_member_location_attribute' below.  */

static void
add_AT_location_description (die, attr_kind, rtl)
     dw_die_ref die;
     enum dwarf_attribute attr_kind;
     register rtx rtl;
{
  /* Handle a special case.  If we are about to output a location descriptor
     for a variable or parameter which has been optimized out of existence,
     don't do that.  A variable which has been optimized out
     of existence will have a DECL_RTL value which denotes a pseudo-reg.
     Currently, in some rare cases, variables can have DECL_RTL values which
     look like (MEM (REG pseudo-reg#)).  These cases are due to bugs
     elsewhere in the compiler.  We treat such cases as if the variable(s) in 
     question had been optimized out of existence.  */

  if (is_pseudo_reg (rtl)
      || (GET_CODE (rtl) == MEM
	  && is_pseudo_reg (XEXP (rtl, 0)))
      || (GET_CODE (rtl) == CONCAT
	  && is_pseudo_reg (XEXP (rtl, 0))
	  && is_pseudo_reg (XEXP (rtl, 1))))
    return;

  add_AT_loc (die, attr_kind, loc_descriptor (rtl));
}

/* Attach the specialized form of location attribute used for data
   members of struct and union types.  In the special case of a
   FIELD_DECL node which represents a bit-field, the "offset" part
   of this special location descriptor must indicate the distance
   in bytes from the lowest-addressed byte of the containing struct
   or union type to the lowest-addressed byte of the "containing
   object" for the bit-field.  (See the `field_byte_offset' function
   above).. For any given bit-field, the "containing object" is a
   hypothetical object (of some integral or enum type) within which
   the given bit-field lives.  The type of this hypothetical
   "containing object" is always the same as the declared type of
   the individual bit-field itself (for GCC anyway... the DWARF
   spec doesn't actually mandate this).  Note that it is the size
   (in bytes) of the hypothetical "containing object" which will
   be given in the DW_AT_byte_size attribute for this bit-field.
   (See the `byte_size_attribute' function below.)  It is also used
   when calculating the value of the DW_AT_bit_offset attribute.
   (See the `bit_offset_attribute' function below).  */

static void
add_data_member_location_attribute (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  register unsigned long offset;
  register dw_loc_descr_ref loc_descr;
  register enum dwarf_location_atom op;

  if (TREE_CODE (decl) == TREE_VEC)
    offset = TREE_INT_CST_LOW (BINFO_OFFSET (decl));
  else
    offset = field_byte_offset (decl);

  /* The DWARF2 standard says that we should assume that the structure address
     is already on the stack, so we can specify a structure field address
     by using DW_OP_plus_uconst.  */

  op = DW_OP_plus_uconst;

  loc_descr = new_loc_descr (op, offset, 0);
  add_AT_loc (die, DW_AT_data_member_location, loc_descr);
}

/* Attach an DW_AT_const_value attribute for a variable or a parameter which
   does not have a "location" either in memory or in a register.  These
   things can arise in GNU C when a constant is passed as an actual parameter
   to an inlined function.  They can also arise in C++ where declared
   constants do not necessarily get memory "homes".  */

static void
add_const_value_attribute (die, rtl)
     register dw_die_ref die;
     register rtx rtl;
{
  switch (GET_CODE (rtl))
    {
    case CONST_INT:
      /* Note that a CONST_INT rtx could represent either an integer or a
         floating-point constant.  A CONST_INT is used whenever the constant
         will fit into a single word.  In all such cases, the original mode
         of the constant value is wiped out, and the CONST_INT rtx is
         assigned VOIDmode.  */
      add_AT_unsigned (die, DW_AT_const_value, (unsigned) INTVAL (rtl));
      break;

    case CONST_DOUBLE:
      /* Note that a CONST_DOUBLE rtx could represent either an integer or a
         floating-point constant.  A CONST_DOUBLE is used whenever the
         constant requires more than one word in order to be adequately
         represented.  We output CONST_DOUBLEs as blocks.  */
      {
	register enum machine_mode mode = GET_MODE (rtl);

	if (GET_MODE_CLASS (mode) == MODE_FLOAT)
	  {
	    register unsigned length = GET_MODE_SIZE (mode) / sizeof (long);
	    long array[4];
	    REAL_VALUE_TYPE rv;

	    REAL_VALUE_FROM_CONST_DOUBLE (rv, rtl);
	    switch (mode)
	      {
	      case SFmode:
		REAL_VALUE_TO_TARGET_SINGLE (rv, array[0]);
		break;

	      case DFmode:
		REAL_VALUE_TO_TARGET_DOUBLE (rv, array);
		break;

	      default:
		abort ();
	      }

	    add_AT_float (die, DW_AT_const_value, length, array);
	  }
	else
	  add_AT_long_long (die, DW_AT_const_value,
			    CONST_DOUBLE_HIGH (rtl), CONST_DOUBLE_LOW (rtl));
      }
      break;

    case CONST_STRING:
      add_AT_string (die, DW_AT_const_value, XSTR (rtl, 0));
      break;

    case SYMBOL_REF:
    case LABEL_REF:
    case CONST:
      add_AT_addr (die, DW_AT_const_value, addr_to_string (rtl));
      break;

    case PLUS:
      /* In cases where an inlined instance of an inline function is passed
         the address of an `auto' variable (which is local to the caller) we
         can get a situation where the DECL_RTL of the artificial local
         variable (for the inlining) which acts as a stand-in for the
         corresponding formal parameter (of the inline function) will look
         like (plus:SI (reg:SI FRAME_PTR) (const_int ...)).  This is not
         exactly a compile-time constant expression, but it isn't the address 
         of the (artificial) local variable either.  Rather, it represents the 
         *value* which the artificial local variable always has during its
         lifetime.  We currently have no way to represent such quasi-constant 
         values in Dwarf, so for now we just punt and generate nothing.  */
      break;

    default:
      /* No other kinds of rtx should be possible here.  */
      abort ();
    }

}

/* Generate *either* an DW_AT_location attribute or else an DW_AT_const_value
   data attribute for a variable or a parameter.  We generate the
   DW_AT_const_value attribute only in those cases where the given variable
   or parameter does not have a true "location" either in memory or in a
   register.  This can happen (for example) when a constant is passed as an
   actual argument in a call to an inline function.  (It's possible that
   these things can crop up in other ways also.)  Note that one type of
   constant value which can be passed into an inlined function is a constant
   pointer.  This can happen for example if an actual argument in an inlined
   function call evaluates to a compile-time constant address.  */

static void
add_location_or_const_value_attribute (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  register rtx rtl;
  register tree declared_type;
  register tree passed_type;

  if (TREE_CODE (decl) == ERROR_MARK)
    return;

  if (TREE_CODE (decl) != VAR_DECL && TREE_CODE (decl) != PARM_DECL)
    abort ();

  /* Here we have to decide where we are going to say the parameter "lives"
     (as far as the debugger is concerned).  We only have a couple of
     choices.  GCC provides us with DECL_RTL and with DECL_INCOMING_RTL.

     DECL_RTL normally indicates where the parameter lives during most of the 
     activation of the function.  If optimization is enabled however, this
     could be either NULL or else a pseudo-reg.  Both of those cases indicate 
     that the parameter doesn't really live anywhere (as far as the code
     generation parts of GCC are concerned) during most of the function's
     activation.  That will happen (for example) if the parameter is never
     referenced within the function.

     We could just generate a location descriptor here for all non-NULL
     non-pseudo values of DECL_RTL and ignore all of the rest, but we can be
     a little nicer than that if we also consider DECL_INCOMING_RTL in cases
     where DECL_RTL is NULL or is a pseudo-reg.

     Note however that we can only get away with using DECL_INCOMING_RTL as
     a backup substitute for DECL_RTL in certain limited cases.  In cases
     where DECL_ARG_TYPE (decl) indicates the same type as TREE_TYPE (decl),
     we can be sure that the parameter was passed using the same type as it is
     declared to have within the function, and that its DECL_INCOMING_RTL
     points us to a place where a value of that type is passed.

     In cases where DECL_ARG_TYPE (decl) and TREE_TYPE (decl) are different,
     we cannot (in general) use DECL_INCOMING_RTL as a substitute for DECL_RTL
     because in these cases DECL_INCOMING_RTL points us to a value of some
     type which is *different* from the type of the parameter itself.  Thus,
     if we tried to use DECL_INCOMING_RTL to generate a location attribute in
     such cases, the debugger would end up (for example) trying to fetch a
     `float' from a place which actually contains the first part of a
     `double'.  That would lead to really incorrect and confusing
     output at debug-time.

     So, in general, we *do not* use DECL_INCOMING_RTL as a backup for DECL_RTL
     in cases where DECL_ARG_TYPE (decl) != TREE_TYPE (decl).  There
     are a couple of exceptions however.  On little-endian machines we can
     get away with using DECL_INCOMING_RTL even when DECL_ARG_TYPE (decl) is
     not the same as TREE_TYPE (decl), but only when DECL_ARG_TYPE (decl) is
     an integral type that is smaller than TREE_TYPE (decl). These cases arise
     when (on a little-endian machine) a non-prototyped function has a
     parameter declared to be of type `short' or `char'.  In such cases,
     TREE_TYPE (decl) will be `short' or `char', DECL_ARG_TYPE (decl) will
     be `int', and DECL_INCOMING_RTL will point to the lowest-order byte of the
     passed `int' value.  If the debugger then uses that address to fetch
     a `short' or a `char' (on a little-endian machine) the result will be
     the correct data, so we allow for such exceptional cases below.

     Note that our goal here is to describe the place where the given formal
     parameter lives during most of the function's activation (i.e. between
     the end of the prologue and the start of the epilogue).  We'll do that
     as best as we can. Note however that if the given formal parameter is
     modified sometime during the execution of the function, then a stack
     backtrace (at debug-time) will show the function as having been
     called with the *new* value rather than the value which was
     originally passed in.  This happens rarely enough that it is not
     a major problem, but it *is* a problem, and I'd like to fix it.

     A future version of dwarf2out.c may generate two additional
     attributes for any given DW_TAG_formal_parameter DIE which will
     describe the "passed type" and the "passed location" for the
     given formal parameter in addition to the attributes we now
     generate to indicate the "declared type" and the "active
     location" for each parameter.  This additional set of attributes
     could be used by debuggers for stack backtraces. Separately, note
     that sometimes DECL_RTL can be NULL and DECL_INCOMING_RTL can be
     NULL also.  This happens (for example) for inlined-instances of
     inline function formal parameters which are never referenced.
     This really shouldn't be happening.  All PARM_DECL nodes should
     get valid non-NULL DECL_INCOMING_RTL values, but integrate.c
     doesn't currently generate these values for inlined instances of
     inline function parameters, so when we see such cases, we are
     just out-of-luck for the time being (until integrate.c
     gets fixed).  */

  /* Use DECL_RTL as the "location" unless we find something better.  */
  rtl = DECL_RTL (decl);

  if (TREE_CODE (decl) == PARM_DECL)
    {
      if (rtl == NULL_RTX || is_pseudo_reg (rtl))
	{
	  declared_type = type_main_variant (TREE_TYPE (decl));
	  passed_type = type_main_variant (DECL_ARG_TYPE (decl));

	  /* This decl represents a formal parameter which was optimized out.
	     Note that DECL_INCOMING_RTL may be NULL in here, but we handle
	     all* cases where (rtl == NULL_RTX) just below.  */
	  if (declared_type == passed_type)
	    rtl = DECL_INCOMING_RTL (decl);
	  else if (TREE_CODE (declared_type) == INTEGER_TYPE
		   && TYPE_SIZE (declared_type) <= TYPE_SIZE (passed_type))
		rtl = DECL_INCOMING_RTL (decl);
	}
    }

  if (rtl == NULL_RTX)
    return;

  rtl = eliminate_regs (rtl, 0, NULL_RTX);

  switch (GET_CODE (rtl))
    {
    case ADDRESSOF:
      /* The address of a variable that was optimized away; don't emit
	 anything.  */
      break;

    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_STRING:
    case SYMBOL_REF:
    case LABEL_REF:
    case CONST:
    case PLUS:
      /* DECL_RTL could be (plus (reg ...) (const_int ...)) */
      add_const_value_attribute (die, rtl);
      break;

    case MEM:
    case REG:
    case SUBREG:
    case CONCAT:
      add_AT_location_description (die, DW_AT_location, rtl);
      break;

    default:
      abort ();
    }
}

/* Generate an DW_AT_name attribute given some string value to be included as
   the value of the attribute.  */

static inline void
add_name_attribute (die, name_string)
     register dw_die_ref die;
     register char *name_string;
{
  if (name_string != NULL && *name_string != 0)
    add_AT_string (die, DW_AT_name, name_string);
}

/* Given a tree node describing an array bound (either lower or upper) output
   a representation for that bound.  */

static void
add_bound_info (subrange_die, bound_attr, bound)
     register dw_die_ref subrange_die;
     register enum dwarf_attribute bound_attr;
     register tree bound;
{
  register unsigned bound_value = 0;

  /* If this is an Ada unconstrained array type, then don't emit any debug
     info because the array bounds are unknown.  They are parameterized when
     the type is instantiated.  */
  if (contains_placeholder_p (bound))
    return;

  switch (TREE_CODE (bound))
    {
    case ERROR_MARK:
      return;

    /* All fixed-bounds are represented by INTEGER_CST nodes.        */
    case INTEGER_CST:
      bound_value = TREE_INT_CST_LOW (bound);
      if (bound_attr == DW_AT_lower_bound
	  && ((is_c_family () && bound_value == 0)
	      || (is_fortran () && bound_value == 1)))
	/* use the default */;
      else
	add_AT_unsigned (subrange_die, bound_attr, bound_value);
      break;

    case CONVERT_EXPR:
    case NOP_EXPR:
    case NON_LVALUE_EXPR:
      add_bound_info (subrange_die, bound_attr, TREE_OPERAND (bound, 0));
      break;
      
    case SAVE_EXPR:
      /* If optimization is turned on, the SAVE_EXPRs that describe how to
         access the upper bound values may be bogus.  If they refer to a
         register, they may only describe how to get at these values at the
         points in the generated code right after they have just been
         computed.  Worse yet, in the typical case, the upper bound values
         will not even *be* computed in the optimized code (though the
         number of elements will), so these SAVE_EXPRs are entirely
         bogus. In order to compensate for this fact, we check here to see
         if optimization is enabled, and if so, we don't add an attribute
         for the (unknown and unknowable) upper bound.  This should not
         cause too much trouble for existing (stupid?)  debuggers because
         they have to deal with empty upper bounds location descriptions
         anyway in order to be able to deal with incomplete array types.
         Of course an intelligent debugger (GDB?)  should be able to
         comprehend that a missing upper bound specification in a array
         type used for a storage class `auto' local array variable
         indicates that the upper bound is both unknown (at compile- time)
         and unknowable (at run-time) due to optimization.

	 We assume that a MEM rtx is safe because gcc wouldn't put the
	 value there unless it was going to be used repeatedly in the
	 function, i.e. for cleanups.  */
      if (! optimize || GET_CODE (SAVE_EXPR_RTL (bound)) == MEM)
	{
	  register dw_die_ref ctx = lookup_decl_die (current_function_decl);
	  register dw_die_ref decl_die = new_die (DW_TAG_variable, ctx);
	  register rtx loc = SAVE_EXPR_RTL (bound);

	  /* If the RTL for the SAVE_EXPR is memory, handle the case where
	     it references an outer function's frame.  */

	  if (GET_CODE (loc) == MEM)
	    {
	      rtx new_addr = fix_lexical_addr (XEXP (loc, 0), bound);

	      if (XEXP (loc, 0) != new_addr)
		loc = gen_rtx (MEM, GET_MODE (loc), new_addr);
	    }

	  add_AT_flag (decl_die, DW_AT_artificial, 1);
	  add_type_attribute (decl_die, TREE_TYPE (bound), 1, 0, ctx);
	  add_AT_location_description (decl_die, DW_AT_location, loc);
	  add_AT_die_ref (subrange_die, bound_attr, decl_die);
	}

      /* Else leave out the attribute.  */
      break;

    case MAX_EXPR:
    case VAR_DECL:
    case COMPONENT_REF:
      /* ??? These types of bounds can be created by the Ada front end,
	 and it isn't clear how to emit debug info for them.  */
      break;

    default:
      abort ();
    }
}

/* Note that the block of subscript information for an array type also
   includes information about the element type of type given array type.  */

static void
add_subscript_info (type_die, type)
     register dw_die_ref type_die;
     register tree type;
{
  register unsigned dimension_number;
  register tree lower, upper;
  register dw_die_ref subrange_die;

  /* The GNU compilers represent multidimensional array types as sequences of 
     one dimensional array types whose element types are themselves array
     types.  Here we squish that down, so that each multidimensional array
     type gets only one array_type DIE in the Dwarf debugging info. The draft 
     Dwarf specification say that we are allowed to do this kind of
     compression in C (because there is no difference between an array or
     arrays and a multidimensional array in C) but for other source languages 
     (e.g. Ada) we probably shouldn't do this.  */

  for (dimension_number = 0;
       TREE_CODE (type) == ARRAY_TYPE;
       type = TREE_TYPE (type), dimension_number++)
    {
      register tree domain = TYPE_DOMAIN (type);

      /* Arrays come in three flavors: Unspecified bounds, fixed bounds,
	 and (in GNU C only) variable bounds.  Handle all three forms 
         here.  */
      subrange_die = new_die (DW_TAG_subrange_type, type_die);
      if (domain)
	{
	  /* We have an array type with specified bounds.  */
	  lower = TYPE_MIN_VALUE (domain);
	  upper = TYPE_MAX_VALUE (domain);

	  /* define the index type.  */
	  if (TREE_TYPE (domain))
	    {
	      /* ??? This is probably an Ada unnamed subrange type.  Ignore the
		 TREE_TYPE field.  We can't emit debug info for this
		 because it is an unnamed integral type.  */
	      if (TREE_CODE (domain) == INTEGER_TYPE
		  && TYPE_NAME (domain) == NULL_TREE
		  && TREE_CODE (TREE_TYPE (domain)) == INTEGER_TYPE
		  && TYPE_NAME (TREE_TYPE (domain)) == NULL_TREE)
		;	
	      else
		add_type_attribute (subrange_die, TREE_TYPE (domain), 0, 0,
				    type_die);
	    }

	  /* ??? If upper is NULL, the array has unspecified length,
	     but it does have a lower bound.  This happens with Fortran
	       dimension arr(N:*)
       	     Since the debugger is definitely going to need to know N
	     to produce useful results, go ahead and output the lower
	     bound solo, and hope the debugger can cope.  */

	  add_bound_info (subrange_die, DW_AT_lower_bound, lower);
	  if (upper)
	    add_bound_info (subrange_die, DW_AT_upper_bound, upper);
	}
      else
	/* We have an array type with an unspecified length.  The DWARF-2
	     spec does not say how to handle this; let's just leave out the
	     bounds.  */
	{;}
      

    }
}

static void
add_byte_size_attribute (die, tree_node)
     dw_die_ref die;
     register tree tree_node;
{
  register unsigned size;

  switch (TREE_CODE (tree_node))
    {
    case ERROR_MARK:
      size = 0;
      break;
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      size = int_size_in_bytes (tree_node);
      break;
    case FIELD_DECL:
      /* For a data member of a struct or union, the DW_AT_byte_size is
         generally given as the number of bytes normally allocated for an
         object of the *declared* type of the member itself.  This is true
         even for bit-fields.  */
      size = simple_type_size_in_bits (field_type (tree_node)) / BITS_PER_UNIT;
      break;
    default:
      abort ();
    }

  /* Note that `size' might be -1 when we get to this point.  If it is, that
     indicates that the byte size of the entity in question is variable.  We
     have no good way of expressing this fact in Dwarf at the present time,
     so just let the -1 pass on through.  */

  add_AT_unsigned (die, DW_AT_byte_size, size);
}

/* For a FIELD_DECL node which represents a bit-field, output an attribute
   which specifies the distance in bits from the highest order bit of the
   "containing object" for the bit-field to the highest order bit of the
   bit-field itself.

   For any given bit-field, the "containing object" is a hypothetical
   object (of some integral or enum type) within which the given bit-field
   lives.  The type of this hypothetical "containing object" is always the
   same as the declared type of the individual bit-field itself.  The
   determination of the exact location of the "containing object" for a
   bit-field is rather complicated.  It's handled by the
   `field_byte_offset' function (above).

   Note that it is the size (in bytes) of the hypothetical "containing object"
   which will be given in the DW_AT_byte_size attribute for this bit-field.
   (See `byte_size_attribute' above).  */

static inline void
add_bit_offset_attribute (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  register unsigned object_offset_in_bytes = field_byte_offset (decl);
  register tree type = DECL_BIT_FIELD_TYPE (decl);
  register tree bitpos_tree = DECL_FIELD_BITPOS (decl);
  register unsigned bitpos_int;
  register unsigned highest_order_object_bit_offset;
  register unsigned highest_order_field_bit_offset;
  register unsigned bit_offset;

  /* Must be a field and a bit field.  */
  if (!type
      || TREE_CODE (decl) != FIELD_DECL)
    abort ();

  /* We can't yet handle bit-fields whose offsets are variable, so if we
     encounter such things, just return without generating any attribute
     whatsoever.  */
  if (TREE_CODE (bitpos_tree) != INTEGER_CST)
    return;

  bitpos_int = (unsigned) TREE_INT_CST_LOW (bitpos_tree);

  /* Note that the bit offset is always the distance (in bits) from the
     highest-order bit of the "containing object" to the highest-order bit of 
     the bit-field itself.  Since the "high-order end" of any object or field 
     is different on big-endian and little-endian machines, the computation
     below must take account of these differences.  */
  highest_order_object_bit_offset = object_offset_in_bytes * BITS_PER_UNIT;
  highest_order_field_bit_offset = bitpos_int;


      highest_order_field_bit_offset
	+= (unsigned) TREE_INT_CST_LOW (DECL_SIZE (decl));

      highest_order_object_bit_offset += simple_type_size_in_bits (type);


  bit_offset
    = highest_order_object_bit_offset - highest_order_field_bit_offset;

  add_AT_unsigned (die, DW_AT_bit_offset, bit_offset);
}

/* For a FIELD_DECL node which represents a bit field, output an attribute
   which specifies the length in bits of the given field.  */

static inline void
add_bit_size_attribute (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  /* Must be a field and a bit field.  */
  if (TREE_CODE (decl) != FIELD_DECL
      || ! DECL_BIT_FIELD_TYPE (decl))
    abort ();
  add_AT_unsigned (die, DW_AT_bit_size,
		   (unsigned) TREE_INT_CST_LOW (DECL_SIZE (decl)));
}

/* If the compiled language is ANSI C, then add a 'prototyped'
   attribute, if arg types are given for the parameters of a function.  */

static inline void
add_prototyped_attribute (die, func_type)
     register dw_die_ref die;
     register tree func_type;
{
  if (get_AT_unsigned (comp_unit_die, DW_AT_language) == DW_LANG_C89
      && TYPE_ARG_TYPES (func_type) != NULL)
    add_AT_flag (die, DW_AT_prototyped, 1);
}


/* Add an 'abstract_origin' attribute below a given DIE.  The DIE is found
   by looking in either the type declaration or object declaration
   equate table.  */

static inline void
add_abstract_origin_attribute (die, origin)
     register dw_die_ref die;
     register tree origin;
{
  dw_die_ref origin_die = NULL;
  if (TREE_CODE_CLASS (TREE_CODE (origin)) == 'd')
    origin_die = lookup_decl_die (origin);
  else if (TREE_CODE_CLASS (TREE_CODE (origin)) == 't')
    origin_die = lookup_type_die (origin);

  add_AT_die_ref (die, DW_AT_abstract_origin, origin_die);
}

/* We do not currently support the pure_virtual attribute.  */

static inline void
add_pure_or_virtual_attribute (die, func_decl)
     register dw_die_ref die;
     register tree func_decl;
{
  if (DECL_VINDEX (func_decl))
    {
      add_AT_unsigned (die, DW_AT_virtuality, DW_VIRTUALITY_virtual);
      add_AT_loc (die, DW_AT_vtable_elem_location,
		  new_loc_descr (DW_OP_constu,
				 TREE_INT_CST_LOW (DECL_VINDEX (func_decl)),
				 0));

      /* GNU extension: Record what type this method came from originally.  */
      if (debug_info_level > DINFO_LEVEL_TERSE)
	add_AT_die_ref (die, DW_AT_containing_type,
			lookup_type_die (DECL_CONTEXT (func_decl)));
    }
}

/* Add source coordinate attributes for the given decl.  */

static void
add_src_coords_attributes (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  register unsigned file_index = lookup_filename (DECL_SOURCE_FILE (decl));

  add_AT_unsigned (die, DW_AT_decl_file, file_index);
  add_AT_unsigned (die, DW_AT_decl_line, DECL_SOURCE_LINE (decl));
}

/* Add an DW_AT_name attribute and source coordinate attribute for the
   given decl, but only if it actually has a name.  */

static void
add_name_and_src_coords_attributes (die, decl)
     register dw_die_ref die;
     register tree decl;
{
  register tree decl_name;

  decl_name = DECL_NAME (decl); 
  if (decl_name != NULL && IDENTIFIER_POINTER (decl_name) != NULL)
    {
      add_name_attribute (die, dwarf2_name (decl, 0));
      add_src_coords_attributes (die, decl);
      if ((TREE_CODE (decl) == FUNCTION_DECL || TREE_CODE (decl) == VAR_DECL)
	  && DECL_ASSEMBLER_NAME (decl) != DECL_NAME (decl))
	add_AT_string (die, DW_AT_MIPS_linkage_name,
		       IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl)));
    }
}

/* Push a new declaration scope. */

static void
push_decl_scope (scope)
     tree scope;
{
  tree containing_scope;
  int i;

  /* Make room in the decl_scope_table, if necessary.  */
  if (decl_scope_table_allocated == decl_scope_depth)
    {
      decl_scope_table_allocated += DECL_SCOPE_TABLE_INCREMENT;
      decl_scope_table
	= (decl_scope_node *) xrealloc (decl_scope_table,
					(decl_scope_table_allocated
					 * sizeof (decl_scope_node)));
    }

  decl_scope_table[decl_scope_depth].scope = scope;

  /* Sometimes, while recursively emitting subtypes within a class type,
     we end up recuring on a subtype at a higher level then the current
     subtype.  In such a case, we need to search the decl_scope_table to
     find the parent of this subtype.  */

  if (AGGREGATE_TYPE_P (scope))
    containing_scope = TYPE_CONTEXT (scope);
  else
    containing_scope = NULL_TREE;

  /* The normal case.  */
  if (decl_scope_depth == 0
      || containing_scope == NULL_TREE
      /* Ignore namespaces for the moment.  */
      || TREE_CODE (containing_scope) == NAMESPACE_DECL
      || containing_scope == decl_scope_table[decl_scope_depth - 1].scope)
    decl_scope_table[decl_scope_depth].previous = decl_scope_depth - 1;
  else
    {
      /* We need to search for the containing_scope.  */
      for (i = 0; i < decl_scope_depth; i++)
	if (decl_scope_table[i].scope == containing_scope)
	  break;

      if (i == decl_scope_depth)
	abort ();
      else
	decl_scope_table[decl_scope_depth].previous = i;
    }

  decl_scope_depth++;
}

/* Return the DIE for the scope that immediately contains this declaration.  */

static dw_die_ref
scope_die_for (t, context_die)
    register tree t; 
    register dw_die_ref context_die;
{
  register dw_die_ref scope_die = NULL;
  register tree containing_scope;
  register int i;

  /* Walk back up the declaration tree looking for a place to define
     this type.  */
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    containing_scope = TYPE_CONTEXT (t);
  else if (TREE_CODE (t) == FUNCTION_DECL && DECL_VINDEX (t))
    containing_scope = decl_class_context (t);
  else
    containing_scope = DECL_CONTEXT (t);

  /* Ignore namespaces for the moment.  */
  if (containing_scope && TREE_CODE (containing_scope) == NAMESPACE_DECL)
    containing_scope = NULL_TREE;

  /* Ignore function type "scopes" from the C frontend.  They mean that
     a tagged type is local to a parmlist of a function declarator, but
     that isn't useful to DWARF.  */
  if (containing_scope && TREE_CODE (containing_scope) == FUNCTION_TYPE)
    containing_scope = NULL_TREE;

  /* Function-local tags and functions get stuck in limbo until they are
     fixed up by decls_for_scope.  */
  if (context_die == NULL && containing_scope != NULL_TREE
      && (TREE_CODE (t) == FUNCTION_DECL || is_tagged_type (t)))
    return NULL;

  if (containing_scope == NULL_TREE)
    scope_die = comp_unit_die;
  else
    {
      for (i = decl_scope_depth - 1, scope_die = context_die;
	   i >= 0 && decl_scope_table[i].scope != containing_scope;
	   (scope_die = scope_die->die_parent,
	    i = decl_scope_table[i].previous))
	;

      /* ??? Integrate_decl_tree does not handle BLOCK_TYPE_TAGS, nor
	 does it try to handle types defined by TYPE_DECLs.  Such types
	 thus have an incorrect TYPE_CONTEXT, which points to the block
	 they were originally defined in, instead of the current block
	 created by function inlining.  We try to detect that here and
	 work around it.  */

      if (i < 0 && scope_die == comp_unit_die
	  && TREE_CODE (containing_scope) == BLOCK
	  && is_tagged_type (t)
	  && (block_ultimate_origin (decl_scope_table[decl_scope_depth - 1].scope)
	      == containing_scope))
	{
	  scope_die = context_die;
	  /* Since the checks below are no longer applicable.  */
	  i = 0;
	}

      if (i < 0)
	{
	  if (scope_die != comp_unit_die
	      || TREE_CODE_CLASS (TREE_CODE (containing_scope)) != 't')
	    abort ();
	  if (debug_info_level > DINFO_LEVEL_TERSE
	      && !TREE_ASM_WRITTEN (containing_scope))
	    abort ();
	}
    }

  return scope_die;
}

/* Pop a declaration scope.  */
static inline void
pop_decl_scope ()
{
  if (decl_scope_depth <= 0)
    abort ();
  --decl_scope_depth;
}

/* Many forms of DIEs require a "type description" attribute.  This
   routine locates the proper "type descriptor" die for the type given
   by 'type', and adds an DW_AT_type attribute below the given die.  */

static void
add_type_attribute (object_die, type, decl_const, decl_volatile, context_die)
     register dw_die_ref object_die;
     register tree type;
     register int decl_const;
     register int decl_volatile;
     register dw_die_ref context_die;
{
  register enum tree_code code  = TREE_CODE (type);
  register dw_die_ref type_die  = NULL;

  /* ??? If this type is an unnamed subrange type of an integral or
     floating-point type, use the inner type.  This is because we have no
     support for unnamed types in base_type_die.  This can happen if this is
     an Ada subrange type.  Correct solution is emit a subrange type die.  */
  if ((code == INTEGER_TYPE || code == REAL_TYPE)
      && TREE_TYPE (type) != 0 && TYPE_NAME (type) == 0)
    type = TREE_TYPE (type), code = TREE_CODE (type);

  if (code == ERROR_MARK)
    return;

  /* Handle a special case.  For functions whose return type is void, we
     generate *no* type attribute.  (Note that no object may have type
     `void', so this only applies to function return types).  */
  if (code == VOID_TYPE)
    return;

  type_die = modified_type_die (type,
				decl_const || TYPE_READONLY (type),
				decl_volatile || TYPE_VOLATILE (type),
				context_die);
  if (type_die != NULL)
    add_AT_die_ref (object_die, DW_AT_type, type_die);
}

/* Given a tree pointer to a struct, class, union, or enum type node, return
   a pointer to the (string) tag name for the given type, or zero if the type
   was declared without a tag.  */

static char *
type_tag (type)
     register tree type;
{
  register char *name = 0;

  if (TYPE_NAME (type) != 0)
    {
      register tree t = 0;

      /* Find the IDENTIFIER_NODE for the type name.  */
      if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
	t = TYPE_NAME (type);

      /* The g++ front end makes the TYPE_NAME of *each* tagged type point to 
         a TYPE_DECL node, regardless of whether or not a `typedef' was
         involved.  */
      else if (TREE_CODE (TYPE_NAME (type)) == TYPE_DECL
	       && ! DECL_IGNORED_P (TYPE_NAME (type)))
	t = DECL_NAME (TYPE_NAME (type));

      /* Now get the name as a string, or invent one.  */
      if (t != 0)
	name = IDENTIFIER_POINTER (t);
    }

  return (name == 0 || *name == '\0') ? 0 : name;
}

/* Return the type associated with a data member, make a special check
   for bit field types.  */

static inline tree
member_declared_type (member)
     register tree member;
{
  return (DECL_BIT_FIELD_TYPE (member)
	  ? DECL_BIT_FIELD_TYPE (member)
	  : TREE_TYPE (member));
}

/* Get the decl's label, as described by its RTL. This may be different
   from the DECL_NAME name used in the source file.  */

#if 0
static char *
decl_start_label (decl)
     register tree decl;
{
  rtx x;
  char *fnname;
  x = DECL_RTL (decl);
  if (GET_CODE (x) != MEM)
    abort ();

  x = XEXP (x, 0);
  if (GET_CODE (x) != SYMBOL_REF)
    abort ();

  fnname = XSTR (x, 0);
  return fnname;
}
#endif

/* These routines generate the internal representation of the DIE's for
   the compilation unit.  Debugging information is collected by walking
   the declaration trees passed in from dwarf2out_decl().  */

static void
gen_array_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref scope_die = scope_die_for (type, context_die);
  register dw_die_ref array_die;
  register tree element_type;

  array_die = new_die (DW_TAG_array_type, scope_die);

#if 0
  /* We default the array ordering.  SDB will probably do
     the right things even if DW_AT_ordering is not present.  It's not even
     an issue until we start to get into multidimensional arrays anyway.  If
     SDB is ever caught doing the Wrong Thing for multi-dimensional arrays,
     then we'll have to put the DW_AT_ordering attribute back in.  (But if
     and when we find out that we need to put these in, we will only do so
     for multidimensional arrays.  */
  add_AT_unsigned (array_die, DW_AT_ordering, DW_ORD_row_major);
#endif

    add_subscript_info (array_die, type);

  equate_type_number_to_die (type, array_die);

  /* Add representation of the type of the elements of this array type.  */
  element_type = TREE_TYPE (type);

  while (TREE_CODE (element_type) == ARRAY_TYPE)
    element_type = TREE_TYPE (element_type);

  gen_type_die (element_type, context_die);

  add_type_attribute (array_die, element_type, 0, 0, context_die);
}

static void
gen_set_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die
    = new_die (DW_TAG_set_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, type_die);
  add_type_attribute (type_die, TREE_TYPE (type), 0, 0, context_die);
}

#if 0
static void
gen_entry_point_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register tree origin = decl_ultimate_origin (decl);
  register dw_die_ref decl_die = new_die (DW_TAG_entry_point, context_die);
  if (origin != NULL)
    add_abstract_origin_attribute (decl_die, origin);
  else
    {
      add_name_and_src_coords_attributes (decl_die, decl);
      add_type_attribute (decl_die, TREE_TYPE (TREE_TYPE (decl)),
			  0, 0, context_die);
    }

  if (DECL_ABSTRACT (decl))
    equate_decl_number_to_die (decl, decl_die);
  else
    add_AT_lbl_id (decl_die, DW_AT_low_pc, decl_start_label (decl));
}
#endif

/* Remember a type in the pending_types_list.  */

static void
pend_type (type)
     register tree type;
{
  if (pending_types == pending_types_allocated)
    {
      pending_types_allocated += PENDING_TYPES_INCREMENT;
      pending_types_list
	= (tree *) xrealloc (pending_types_list,
			     sizeof (tree) * pending_types_allocated);
    }

  pending_types_list[pending_types++] = type;
}

/* Output any pending types (from the pending_types list) which we can output
   now (taking into account the scope that we are working on now).

   For each type output, remove the given type from the pending_types_list
   *before* we try to output it.  */

static void
output_pending_types_for_scope (context_die)
     register dw_die_ref context_die;
{
  register tree type;

  while (pending_types)
    {
      --pending_types;
      type = pending_types_list[pending_types];
      gen_type_die (type, context_die);
      if (!TREE_ASM_WRITTEN (type))
	abort ();
    }
}

/* Generate a DIE to represent an inlined instance of an enumeration type.  */

static void
gen_inlined_enumeration_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die = new_die (DW_TAG_enumeration_type,
					  scope_die_for (type, context_die));

  if (!TREE_ASM_WRITTEN (type))
    abort ();
  add_abstract_origin_attribute (type_die, type);
}

/* Generate a DIE to represent an inlined instance of a structure type.  */

static void
gen_inlined_structure_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die = new_die (DW_TAG_structure_type,
					  scope_die_for (type, context_die));

  if (!TREE_ASM_WRITTEN (type))
    abort ();
  add_abstract_origin_attribute (type_die, type);
}

/* Generate a DIE to represent an inlined instance of a union type.  */

static void
gen_inlined_union_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die = new_die (DW_TAG_union_type,
					  scope_die_for (type, context_die));

  if (!TREE_ASM_WRITTEN (type))
    abort ();
  add_abstract_origin_attribute (type_die, type);
}

/* Generate a DIE to represent an enumeration type.  Note that these DIEs
   include all of the information about the enumeration values also. Each
   enumerated type name/value is listed as a child of the enumerated type
   DIE.  */

static void
gen_enumeration_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die = lookup_type_die (type);

  if (type_die == NULL)
    {
      type_die = new_die (DW_TAG_enumeration_type,
			  scope_die_for (type, context_die));
      equate_type_number_to_die (type, type_die);
      add_name_attribute (type_die, type_tag (type));
    }
  else if (! TYPE_SIZE (type))
    return;
  else
    remove_AT (type_die, DW_AT_declaration);

  /* Handle a GNU C/C++ extension, i.e. incomplete enum types.  If the
     given enum type is incomplete, do not generate the DW_AT_byte_size
     attribute or the DW_AT_element_list attribute.  */
  if (TYPE_SIZE (type))
    {
      register tree link;

      TREE_ASM_WRITTEN (type) = 1;
      add_byte_size_attribute (type_die, type);
      if (TYPE_STUB_DECL (type) != NULL_TREE)
	add_src_coords_attributes (type_die, TYPE_STUB_DECL (type));

      /* If the first reference to this type was as the return type of an
	 inline function, then it may not have a parent.  Fix this now.  */
      if (type_die->die_parent == NULL)
	add_child_die (scope_die_for (type, context_die), type_die);

      for (link = TYPE_FIELDS (type);
	   link != NULL; link = TREE_CHAIN (link))
	{
	  register dw_die_ref enum_die = new_die (DW_TAG_enumerator, type_die);

	  add_name_attribute (enum_die,
			      IDENTIFIER_POINTER (TREE_PURPOSE (link)));
	  add_AT_unsigned (enum_die, DW_AT_const_value,
			   (unsigned) TREE_INT_CST_LOW (TREE_VALUE (link)));
	}
    }
  else
    add_AT_flag (type_die, DW_AT_declaration, 1);
}


/* Generate a DIE to represent either a real live formal parameter decl or to
   represent just the type of some formal parameter position in some function
   type.

   Note that this routine is a bit unusual because its argument may be a
   ..._DECL node (i.e. either a PARM_DECL or perhaps a VAR_DECL which
   represents an inlining of some PARM_DECL) or else some sort of a ..._TYPE
   node.  If it's the former then this function is being called to output a
   DIE to represent a formal parameter object (or some inlining thereof).  If
   it's the latter, then this function is only being called to output a
   DW_TAG_formal_parameter DIE to stand as a placeholder for some formal
   argument type of some subprogram type.  */

static dw_die_ref
gen_formal_parameter_die (node, context_die)
     register tree node;
     register dw_die_ref context_die;
{
  register dw_die_ref parm_die
    = new_die (DW_TAG_formal_parameter, context_die);
  register tree origin;

  switch (TREE_CODE_CLASS (TREE_CODE (node)))
    {
    case 'd':
      origin = decl_ultimate_origin (node);
      if (origin != NULL)
	add_abstract_origin_attribute (parm_die, origin);
      else
	{
	  add_name_and_src_coords_attributes (parm_die, node);
	  add_type_attribute (parm_die, TREE_TYPE (node),
			      TREE_READONLY (node),
			      TREE_THIS_VOLATILE (node),
			      context_die);
	  if (DECL_ARTIFICIAL (node))
	    add_AT_flag (parm_die, DW_AT_artificial, 1);
	}

      equate_decl_number_to_die (node, parm_die);
      if (! DECL_ABSTRACT (node))
	add_location_or_const_value_attribute (parm_die, node);

      break;

    case 't':
      /* We were called with some kind of a ..._TYPE node.  */
      add_type_attribute (parm_die, node, 0, 0, context_die);
      break;

    default:
      abort ();
    }

  return parm_die;
}

/* Generate a special type of DIE used as a stand-in for a trailing ellipsis
   at the end of an (ANSI prototyped) formal parameters list.  */

static void
gen_unspecified_parameters_die (decl_or_type, context_die)
     register tree decl_or_type;
     register dw_die_ref context_die;
{
  new_die (DW_TAG_unspecified_parameters, context_die);
}

/* Generate a list of nameless DW_TAG_formal_parameter DIEs (and perhaps a
   DW_TAG_unspecified_parameters DIE) to represent the types of the formal
   parameters as specified in some function type specification (except for
   those which appear as part of a function *definition*).

   Note we must be careful here to output all of the parameter DIEs before*
   we output any DIEs needed to represent the types of the formal parameters.
   This keeps svr4 SDB happy because it (incorrectly) thinks that the first
   non-parameter DIE it sees ends the formal parameter list.  */

static void
gen_formal_types_die (function_or_method_type, context_die)
     register tree function_or_method_type;
     register dw_die_ref context_die;
{
  register tree link;
  register tree formal_type = NULL;
  register tree first_parm_type = TYPE_ARG_TYPES (function_or_method_type);

#if 0
  /* In the case where we are generating a formal types list for a C++
     non-static member function type, skip over the first thing on the
     TYPE_ARG_TYPES list because it only represents the type of the hidden
     `this pointer'.  The debugger should be able to figure out (without
     being explicitly told) that this non-static member function type takes a 
     `this pointer' and should be able to figure what the type of that hidden 
     parameter is from the DW_AT_member attribute of the parent
     DW_TAG_subroutine_type DIE.  */
  if (TREE_CODE (function_or_method_type) == METHOD_TYPE)
    first_parm_type = TREE_CHAIN (first_parm_type);
#endif

  /* Make our first pass over the list of formal parameter types and output a 
     DW_TAG_formal_parameter DIE for each one.  */
  for (link = first_parm_type; link; link = TREE_CHAIN (link))
    {
      register dw_die_ref parm_die;
      
      formal_type = TREE_VALUE (link);
      if (formal_type == void_type_node)
	break;

      /* Output a (nameless) DIE to represent the formal parameter itself.  */
      parm_die = gen_formal_parameter_die (formal_type, context_die);
      if (TREE_CODE (function_or_method_type) == METHOD_TYPE
	  && link == first_parm_type)
	add_AT_flag (parm_die, DW_AT_artificial, 1);
    }

  /* If this function type has an ellipsis, add a
     DW_TAG_unspecified_parameters DIE to the end of the parameter list.  */
  if (formal_type != void_type_node)
    gen_unspecified_parameters_die (function_or_method_type, context_die);

  /* Make our second (and final) pass over the list of formal parameter types 
     and output DIEs to represent those types (as necessary).  */
  for (link = TYPE_ARG_TYPES (function_or_method_type);
       link;
       link = TREE_CHAIN (link))
    {
      formal_type = TREE_VALUE (link);
      if (formal_type == void_type_node)
	break;

      gen_type_die (formal_type, context_die);
    }
}

/* Generate a DIE to represent a declared function (either file-scope or
   block-local).  */

static void
gen_subprogram_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  char label_id[MAX_ARTIFICIAL_LABEL_BYTES];
  register tree origin = decl_ultimate_origin (decl);
  register dw_die_ref subr_die;
  register rtx fp_reg;
  register tree fn_arg_types;
  register tree outer_scope;
  register dw_die_ref old_die = lookup_decl_die (decl);
  register int declaration
    = (current_function_decl != decl
       || (context_die
	   && (context_die->die_tag == DW_TAG_structure_type
	       || context_die->die_tag == DW_TAG_union_type)));

  if (origin != NULL)
    {
      subr_die = new_die (DW_TAG_subprogram, context_die);
      add_abstract_origin_attribute (subr_die, origin);
    }
  else if (old_die && DECL_ABSTRACT (decl)
	   && get_AT_unsigned (old_die, DW_AT_inline))
    {
      /* This must be a redefinition of an extern inline function.
	 We can just reuse the old die here.  */
      subr_die = old_die;

      /* Clear out the inlined attribute and parm types.  */
      remove_AT (subr_die, DW_AT_inline);
      remove_children (subr_die);
    }
  else if (old_die)
    {
      register unsigned file_index
	= lookup_filename (DECL_SOURCE_FILE (decl));

      if (get_AT_flag (old_die, DW_AT_declaration) != 1)
	{
	  /* ??? This can happen if there is a bug in the program, for
	     instance, if it has duplicate function definitions.  Ideally,
	     we should detect this case and ignore it.  For now, if we have
	     already reported an error, any error at all, then assume that
	     we got here because of a input error, not a dwarf2 bug.  */
	  extern int errorcount;
	  if (errorcount)
	    return;
	  abort ();
	}

      /* If the definition comes from the same place as the declaration,
	 maybe use the old DIE.  We always want the DIE for this function
	 that has the *_pc attributes to be under comp_unit_die so the
	 debugger can find it.  For inlines, that is the concrete instance,
	 so we can use the old DIE here.  For non-inline methods, we want a
	 specification DIE at toplevel, so we need a new DIE.  For local
	 class methods, this does not apply.  */
      if ((DECL_ABSTRACT (decl) || old_die->die_parent == comp_unit_die
	   || context_die == NULL)
	  && get_AT_unsigned (old_die, DW_AT_decl_file) == file_index
	  && (get_AT_unsigned (old_die, DW_AT_decl_line)
	      == DECL_SOURCE_LINE (decl)))
	{
	  subr_die = old_die;

	  /* Clear out the declaration attribute and the parm types.  */
	  remove_AT (subr_die, DW_AT_declaration);
	  remove_children (subr_die);
	}
      else
	{
	  subr_die = new_die (DW_TAG_subprogram, context_die);
	  add_AT_die_ref (subr_die, DW_AT_specification, old_die);
	  if (get_AT_unsigned (old_die, DW_AT_decl_file) != file_index)
	    add_AT_unsigned (subr_die, DW_AT_decl_file, file_index);
	  if (get_AT_unsigned (old_die, DW_AT_decl_line)
	      != DECL_SOURCE_LINE (decl))
	    add_AT_unsigned
	      (subr_die, DW_AT_decl_line, DECL_SOURCE_LINE (decl));
	}
    }
  else
    {
      register dw_die_ref scope_die;

      if (DECL_CONTEXT (decl))
	scope_die = scope_die_for (decl, context_die);
      else
	/* Don't put block extern declarations under comp_unit_die.  */
	scope_die = context_die;

      subr_die = new_die (DW_TAG_subprogram, scope_die);
			 
      if (TREE_PUBLIC (decl))
	add_AT_flag (subr_die, DW_AT_external, 1);

      add_name_and_src_coords_attributes (subr_die, decl);
      if (debug_info_level > DINFO_LEVEL_TERSE)
	{
	  register tree type = TREE_TYPE (decl);

	  add_prototyped_attribute (subr_die, type);
	  add_type_attribute (subr_die, TREE_TYPE (type), 0, 0, context_die);
	}

      add_pure_or_virtual_attribute (subr_die, decl);
      if (DECL_ARTIFICIAL (decl))
	add_AT_flag (subr_die, DW_AT_artificial, 1);
      if (TREE_PROTECTED (decl))
	add_AT_unsigned (subr_die, DW_AT_accessibility, DW_ACCESS_protected);
      else if (TREE_PRIVATE (decl))
	add_AT_unsigned (subr_die, DW_AT_accessibility, DW_ACCESS_private);
    }

  if (declaration)
    {
      add_AT_flag (subr_die, DW_AT_declaration, 1);

      /* The first time we see a member function, it is in the context of
         the class to which it belongs.  We make sure of this by emitting
         the class first.  The next time is the definition, which is
         handled above.  The two may come from the same source text.  */
      if (DECL_CONTEXT (decl))
	equate_decl_number_to_die (decl, subr_die);
    }
  else if (DECL_ABSTRACT (decl))
    {
      /* ??? Checking DECL_DEFER_OUTPUT is correct for static inline functions,
	 but not for extern inline functions.  We can't get this completely
	 correct because information about whether the function was declared
	 inline is not saved anywhere.  */
      if (DECL_DEFER_OUTPUT (decl))
	{
	  if (DECL_INLINE (decl))
	    add_AT_unsigned (subr_die, DW_AT_inline, DW_INL_declared_inlined);
	  else
	    add_AT_unsigned (subr_die, DW_AT_inline,
			     DW_INL_declared_not_inlined);
	}
      else if (DECL_INLINE (decl))
	add_AT_unsigned (subr_die, DW_AT_inline, DW_INL_inlined);
      else
	abort ();

      equate_decl_number_to_die (decl, subr_die);
    }
  else if (!DECL_EXTERNAL (decl))
    {
      if (origin == NULL_TREE)
	equate_decl_number_to_die (decl, subr_die);

      ASM_GENERATE_INTERNAL_LABEL (label_id, FUNC_BEGIN_LABEL,
				   current_funcdef_number);
      add_AT_lbl_id (subr_die, DW_AT_low_pc, label_id);
      ASM_GENERATE_INTERNAL_LABEL (label_id, FUNC_END_LABEL,
				   current_funcdef_number);
      add_AT_lbl_id (subr_die, DW_AT_high_pc, label_id);

      add_pubname (decl, subr_die);
      add_arange (decl, subr_die);


      /* Define the "frame base" location for this routine.  We use the
         frame pointer or stack pointer registers, since the RTL for local
         variables is relative to one of them.  */
      fp_reg
	= frame_pointer_needed ? frame_pointer_rtx : stack_pointer_rtx;
      add_AT_loc (subr_die, DW_AT_frame_base, reg_loc_descriptor (fp_reg));

#if 0
      /* ??? This fails for nested inline functions, because context_display
	 is not part of the state saved/restored for inline functions.  */
      if (current_function_needs_context)
	add_AT_location_description (subr_die, DW_AT_static_link,
				     lookup_static_chain (decl));
#endif
    }

  /* Now output descriptions of the arguments for this function. This gets
     (unnecessarily?) complex because of the fact that the DECL_ARGUMENT list 
     for a FUNCTION_DECL doesn't indicate cases where there was a trailing
     `...' at the end of the formal parameter list.  In order to find out if
     there was a trailing ellipsis or not, we must instead look at the type
     associated with the FUNCTION_DECL.  This will be a node of type
     FUNCTION_TYPE. If the chain of type nodes hanging off of this
     FUNCTION_TYPE node ends with a void_type_node then there should *not* be 
     an ellipsis at the end.  */
  push_decl_scope (decl);

  /* In the case where we are describing a mere function declaration, all we
     need to do here (and all we *can* do here) is to describe the *types* of 
     its formal parameters.  */
  if (debug_info_level <= DINFO_LEVEL_TERSE)
    ;
  else if (declaration)
    gen_formal_types_die (TREE_TYPE (decl), subr_die);
  else
    {
      /* Generate DIEs to represent all known formal parameters */
      register tree arg_decls = DECL_ARGUMENTS (decl);
      register tree parm;

      /* When generating DIEs, generate the unspecified_parameters DIE
         instead if we come across the arg "__builtin_va_alist" */
      for (parm = arg_decls; parm; parm = TREE_CHAIN (parm))
	if (TREE_CODE (parm) == PARM_DECL)
	  {
	    if (DECL_NAME (parm)
		&& !strcmp (IDENTIFIER_POINTER (DECL_NAME (parm)),
			    "__builtin_va_alist"))
	      gen_unspecified_parameters_die (parm, subr_die);
	    else
	      gen_decl_die (parm, subr_die);
	  }

      /* Decide whether we need a unspecified_parameters DIE at the end.
         There are 2 more cases to do this for: 1) the ansi ... declaration - 
         this is detectable when the end of the arg list is not a
         void_type_node 2) an unprototyped function declaration (not a
         definition).  This just means that we have no info about the
         parameters at all.  */
      fn_arg_types = TYPE_ARG_TYPES (TREE_TYPE (decl));
      if (fn_arg_types != NULL)
	{
	  /* this is the prototyped case, check for ...  */
	  if (TREE_VALUE (tree_last (fn_arg_types)) != void_type_node)
	    gen_unspecified_parameters_die (decl, subr_die);
	}
      else if (DECL_INITIAL (decl) == NULL_TREE)
	gen_unspecified_parameters_die (decl, subr_die);
    }

  /* Output Dwarf info for all of the stuff within the body of the function
     (if it has one - it may be just a declaration).  */
  outer_scope = DECL_INITIAL (decl);

  /* Note that here, `outer_scope' is a pointer to the outermost BLOCK
     node created to represent a function. This outermost BLOCK actually
     represents the outermost binding contour for the function, i.e. the
     contour in which the function's formal parameters and labels get
     declared. Curiously, it appears that the front end doesn't actually
     put the PARM_DECL nodes for the current function onto the BLOCK_VARS
     list for this outer scope.  (They are strung off of the DECL_ARGUMENTS
     list for the function instead.) The BLOCK_VARS list for the
     `outer_scope' does provide us with a list of the LABEL_DECL nodes for
     the function however, and we output DWARF info for those in
     decls_for_scope.  Just within the `outer_scope' there will be a BLOCK
     node representing the function's outermost pair of curly braces, and
     any blocks used for the base and member initializers of a C++
     constructor function.  */
  if (! declaration && TREE_CODE (outer_scope) != ERROR_MARK)
    {
      current_function_has_inlines = 0;
      decls_for_scope (outer_scope, subr_die, 0);

    }

  pop_decl_scope ();
}

/* Generate a DIE to represent a declared data object.  */

static void
gen_variable_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register tree origin = decl_ultimate_origin (decl);
  register dw_die_ref var_die = new_die (DW_TAG_variable, context_die);

  dw_die_ref old_die = lookup_decl_die (decl);
  int declaration
    = (DECL_EXTERNAL (decl)
       || current_function_decl != decl_function_context (decl)
       || context_die->die_tag == DW_TAG_structure_type
       || context_die->die_tag == DW_TAG_union_type);

  if (origin != NULL)
    add_abstract_origin_attribute (var_die, origin);
  /* Loop unrolling can create multiple blocks that refer to the same
     static variable, so we must test for the DW_AT_declaration flag.  */
  /* ??? Loop unrolling/reorder_blocks should perhaps be rewritten to
     copy decls and set the DECL_ABSTRACT flag on them instead of
     sharing them.  */
  else if (old_die && TREE_STATIC (decl)
 	   && get_AT_flag (old_die, DW_AT_declaration) == 1)
    {
      /* ??? This is an instantiation of a C++ class level static.  */
      add_AT_die_ref (var_die, DW_AT_specification, old_die);
      if (DECL_NAME (decl))
	{
	  register unsigned file_index
	    = lookup_filename (DECL_SOURCE_FILE (decl));

	  if (get_AT_unsigned (old_die, DW_AT_decl_file) != file_index)
	    add_AT_unsigned (var_die, DW_AT_decl_file, file_index);

	  if (get_AT_unsigned (old_die, DW_AT_decl_line)
	      != DECL_SOURCE_LINE (decl))

	    add_AT_unsigned (var_die, DW_AT_decl_line,
			     DECL_SOURCE_LINE (decl));
	}
    }
  else
    {
      add_name_and_src_coords_attributes (var_die, decl);
      add_type_attribute (var_die, TREE_TYPE (decl),
			  TREE_READONLY (decl),
			  TREE_THIS_VOLATILE (decl), context_die);

      if (TREE_PUBLIC (decl))
	add_AT_flag (var_die, DW_AT_external, 1);

      if (DECL_ARTIFICIAL (decl))
	add_AT_flag (var_die, DW_AT_artificial, 1);

      if (TREE_PROTECTED (decl))
	add_AT_unsigned (var_die, DW_AT_accessibility, DW_ACCESS_protected);

      else if (TREE_PRIVATE (decl))
	add_AT_unsigned (var_die, DW_AT_accessibility, DW_ACCESS_private);
    }

  if (declaration)
    add_AT_flag (var_die, DW_AT_declaration, 1);
  
  if ((declaration && decl_class_context (decl)) || DECL_ABSTRACT (decl))
    equate_decl_number_to_die (decl, var_die);

  if (! declaration && ! DECL_ABSTRACT (decl))
    {
      equate_decl_number_to_die (decl, var_die);
      add_location_or_const_value_attribute (var_die, decl);
      add_pubname (decl, var_die);
    }
}

/* Generate a DIE to represent a label identifier.  */

static void
gen_label_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register tree origin = decl_ultimate_origin (decl);
  register dw_die_ref lbl_die = new_die (DW_TAG_label, context_die);
  register rtx insn;
  char label[MAX_ARTIFICIAL_LABEL_BYTES];
  char label2[MAX_ARTIFICIAL_LABEL_BYTES];

  if (origin != NULL)
    add_abstract_origin_attribute (lbl_die, origin);
  else
    add_name_and_src_coords_attributes (lbl_die, decl);

  if (DECL_ABSTRACT (decl))
    equate_decl_number_to_die (decl, lbl_die);
  else
    {
      insn = DECL_RTL (decl);
      if (GET_CODE (insn) == CODE_LABEL)
	{
	  /* When optimization is enabled (via -O) some parts of the compiler 
	     (e.g. jump.c and cse.c) may try to delete CODE_LABEL insns which 
	     represent source-level labels which were explicitly declared by
	     the user.  This really shouldn't be happening though, so catch
	     it if it ever does happen.  */
	  if (INSN_DELETED_P (insn))
	    abort ();

	  sprintf (label2, INSN_LABEL_FMT, current_funcdef_number);
	  ASM_GENERATE_INTERNAL_LABEL (label, label2,
				       (unsigned) INSN_UID (insn));
	  add_AT_lbl_id (lbl_die, DW_AT_low_pc, label);
	}
    }
}

/* Generate a DIE for a lexical block.  */

static void
gen_lexical_block_die (stmt, context_die, depth)
     register tree stmt;
     register dw_die_ref context_die;
     int depth;
{
  register dw_die_ref stmt_die = new_die (DW_TAG_lexical_block, context_die);
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  if (! BLOCK_ABSTRACT (stmt))
    {
      ASM_GENERATE_INTERNAL_LABEL (label, BLOCK_BEGIN_LABEL,
				   next_block_number);
      add_AT_lbl_id (stmt_die, DW_AT_low_pc, label);
      ASM_GENERATE_INTERNAL_LABEL (label, BLOCK_END_LABEL, next_block_number);
      add_AT_lbl_id (stmt_die, DW_AT_high_pc, label);
    }

  push_decl_scope (stmt);
  decls_for_scope (stmt, stmt_die, depth);
  pop_decl_scope ();
}

/* Generate a DIE for an inlined subprogram.  */

static void
gen_inlined_subroutine_die (stmt, context_die, depth)
     register tree stmt;
     register dw_die_ref context_die;
     int depth;
{
  if (! BLOCK_ABSTRACT (stmt))
    {
      register dw_die_ref subr_die
	= new_die (DW_TAG_inlined_subroutine, context_die);
      register tree decl = block_ultimate_origin (stmt);
      char label[MAX_ARTIFICIAL_LABEL_BYTES];

      add_abstract_origin_attribute (subr_die, decl);
      ASM_GENERATE_INTERNAL_LABEL (label, BLOCK_BEGIN_LABEL,
				   next_block_number);
      add_AT_lbl_id (subr_die, DW_AT_low_pc, label);
      ASM_GENERATE_INTERNAL_LABEL (label, BLOCK_END_LABEL, next_block_number);
      add_AT_lbl_id (subr_die, DW_AT_high_pc, label);
      push_decl_scope (decl);
      decls_for_scope (stmt, subr_die, depth);
      pop_decl_scope ();
      current_function_has_inlines = 1;
    }
}

/* Generate a DIE for a field in a record, or structure.  */

static void
gen_field_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register dw_die_ref decl_die = new_die (DW_TAG_member, context_die);

  add_name_and_src_coords_attributes (decl_die, decl);
  add_type_attribute (decl_die, member_declared_type (decl),
		      TREE_READONLY (decl), TREE_THIS_VOLATILE (decl),
		      context_die);

  /* If this is a bit field...  */
  if (DECL_BIT_FIELD_TYPE (decl))
    {
      add_byte_size_attribute (decl_die, decl);
      add_bit_size_attribute (decl_die, decl);
      add_bit_offset_attribute (decl_die, decl);
    }

  if (TREE_CODE (DECL_FIELD_CONTEXT (decl)) != UNION_TYPE)
    add_data_member_location_attribute (decl_die, decl);

  if (DECL_ARTIFICIAL (decl))
    add_AT_flag (decl_die, DW_AT_artificial, 1);

  if (TREE_PROTECTED (decl))
    add_AT_unsigned (decl_die, DW_AT_accessibility, DW_ACCESS_protected);

  else if (TREE_PRIVATE (decl))
    add_AT_unsigned (decl_die, DW_AT_accessibility, DW_ACCESS_private);
}

#if 0
/* Don't generate either pointer_type DIEs or reference_type DIEs here.
   Use modified_type_die instead.
   We keep this code here just in case these types of DIEs may be needed to
   represent certain things in other languages (e.g. Pascal) someday.  */
static void
gen_pointer_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref ptr_die
    = new_die (DW_TAG_pointer_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, ptr_die);
  add_type_attribute (ptr_die, TREE_TYPE (type), 0, 0, context_die);
  add_AT_unsigned (mod_type_die, DW_AT_byte_size, PTR_SIZE);
}

/* Don't generate either pointer_type DIEs or reference_type DIEs here.
   Use modified_type_die instead.
   We keep this code here just in case these types of DIEs may be needed to
   represent certain things in other languages (e.g. Pascal) someday.  */
static void
gen_reference_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref ref_die
    = new_die (DW_TAG_reference_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, ref_die);
  add_type_attribute (ref_die, TREE_TYPE (type), 0, 0, context_die);
  add_AT_unsigned (mod_type_die, DW_AT_byte_size, PTR_SIZE);
}
#endif

/* Generate a DIE for a pointer to a member type.  */
static void
gen_ptr_to_mbr_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref ptr_die
    = new_die (DW_TAG_ptr_to_member_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, ptr_die);
  add_AT_die_ref (ptr_die, DW_AT_containing_type,
		  lookup_type_die (TYPE_OFFSET_BASETYPE (type)));
  add_type_attribute (ptr_die, TREE_TYPE (type), 0, 0, context_die);
}

/* Generate the DIE for the compilation unit.  */

static void
gen_compile_unit_die (main_input_filename)
     register char *main_input_filename;
{
  char producer[250];
  char *wd = getpwd ();

  comp_unit_die = new_die (DW_TAG_compile_unit, NULL);
  add_name_attribute (comp_unit_die, main_input_filename);

  if (wd != NULL)
    add_AT_string (comp_unit_die, DW_AT_comp_dir, wd);

  sprintf (producer, "%s %s", language_string, version_string);

  add_AT_string (comp_unit_die, DW_AT_producer, producer);

  if (strcmp (language_string, "GNU C++") == 0)
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_C_plus_plus);

  else if (strcmp (language_string, "GNU Ada") == 0)
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_Ada83);

  else if (strcmp (language_string, "GNU F77") == 0)
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_Fortran77);

  else if (strcmp (language_string, "GNU Pascal") == 0)
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_Pascal83);

  else if (flag_traditional)
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_C);

  else
    add_AT_unsigned (comp_unit_die, DW_AT_language, DW_LANG_C89);

#if 0 /* unimplemented */
  if (debug_info_level >= DINFO_LEVEL_VERBOSE)
    add_AT_unsigned (comp_unit_die, DW_AT_macro_info, 0);
#endif
}

/* Generate a DIE for a string type.  */

static void
gen_string_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die
    = new_die (DW_TAG_string_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, type_die);

  /* Fudge the string length attribute for now.  */
  
  /* TODO: add string length info.
   string_length_attribute (TYPE_MAX_VALUE (TYPE_DOMAIN (type)));
			      bound_representation (upper_bound, 0, 'u'); */
}

/* Generate the DIE for a base class.  */

static void
gen_inheritance_die (binfo, context_die)
     register tree binfo;
     register dw_die_ref context_die;
{
  dw_die_ref die = new_die (DW_TAG_inheritance, context_die);

  add_type_attribute (die, BINFO_TYPE (binfo), 0, 0, context_die);
  add_data_member_location_attribute (die, binfo);

  if (TREE_VIA_VIRTUAL (binfo))
    add_AT_unsigned (die, DW_AT_virtuality, DW_VIRTUALITY_virtual);
  if (TREE_VIA_PUBLIC (binfo))
    add_AT_unsigned (die, DW_AT_accessibility, DW_ACCESS_public);
  else if (TREE_VIA_PROTECTED (binfo))
    add_AT_unsigned (die, DW_AT_accessibility, DW_ACCESS_protected);
}

/* Generate a DIE for a class member.  */

static void
gen_member_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register tree member;

  /* If this is not an incomplete type, output descriptions of each of its
     members. Note that as we output the DIEs necessary to represent the
     members of this record or union type, we will also be trying to output
     DIEs to represent the *types* of those members. However the `type'
     function (above) will specifically avoid generating type DIEs for member 
     types *within* the list of member DIEs for this (containing) type execpt 
     for those types (of members) which are explicitly marked as also being
     members of this (containing) type themselves.  The g++ front- end can
     force any given type to be treated as a member of some other
     (containing) type by setting the TYPE_CONTEXT of the given (member) type 
     to point to the TREE node representing the appropriate (containing)
     type.  */

  /* First output info about the base classes.  */
  if (TYPE_BINFO (type) && TYPE_BINFO_BASETYPES (type))
    {
      register tree bases = TYPE_BINFO_BASETYPES (type);
      register int n_bases = TREE_VEC_LENGTH (bases);
      register int i;

      for (i = 0; i < n_bases; i++)
	gen_inheritance_die (TREE_VEC_ELT (bases, i), context_die);
    }

  /* Now output info about the data members and type members.  */
  for (member = TYPE_FIELDS (type); member; member = TREE_CHAIN (member))
    gen_decl_die (member, context_die);

  /* Now output info about the function members (if any).  */
  for (member = TYPE_METHODS (type); member; member = TREE_CHAIN (member))
    gen_decl_die (member, context_die);
}

/* Generate a DIE for a structure or union type.  */

static void
gen_struct_or_union_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die = lookup_type_die (type);
  register dw_die_ref scope_die = 0;
  register int nested = 0;

  if (type_die && ! TYPE_SIZE (type))
    return;

  if (TYPE_CONTEXT (type) != NULL_TREE
      && AGGREGATE_TYPE_P (TYPE_CONTEXT (type)))
    nested = 1;

  scope_die = scope_die_for (type, context_die);

  if (! type_die || (nested && scope_die == comp_unit_die))
    /* First occurrence of type or toplevel definition of nested class.  */
    {
      register dw_die_ref old_die = type_die;

      type_die = new_die (TREE_CODE (type) == RECORD_TYPE
			  ? DW_TAG_structure_type : DW_TAG_union_type,
			  scope_die);
      equate_type_number_to_die (type, type_die);
      add_name_attribute (type_die, type_tag (type));
      if (old_die)
	add_AT_die_ref (type_die, DW_AT_specification, old_die);
    }
  else
    remove_AT (type_die, DW_AT_declaration);

  /* If we're not in the right context to be defining this type, defer to
     avoid tricky recursion.  */
  if (TYPE_SIZE (type) && decl_scope_depth > 0 && scope_die == comp_unit_die)
    {
      add_AT_flag (type_die, DW_AT_declaration, 1);
      pend_type (type);
    }
  /* If this type has been completed, then give it a byte_size attribute and
     then give a list of members.  */
  else if (TYPE_SIZE (type))
    {
      /* Prevent infinite recursion in cases where the type of some member of 
         this type is expressed in terms of this type itself.  */
      TREE_ASM_WRITTEN (type) = 1;
      add_byte_size_attribute (type_die, type);
      if (TYPE_STUB_DECL (type) != NULL_TREE)
	add_src_coords_attributes (type_die, TYPE_STUB_DECL (type));

      /* If the first reference to this type was as the return type of an
	 inline function, then it may not have a parent.  Fix this now.  */
      if (type_die->die_parent == NULL)
	add_child_die (scope_die, type_die);

      push_decl_scope (type);
      gen_member_die (type, type_die);
      pop_decl_scope ();

      /* GNU extension: Record what type our vtable lives in.  */
      if (TYPE_VFIELD (type))
	{
	  tree vtype = DECL_FCONTEXT (TYPE_VFIELD (type));

	  gen_type_die (vtype, context_die);
	  add_AT_die_ref (type_die, DW_AT_containing_type,
			  lookup_type_die (vtype));
	}
    }
  else
    add_AT_flag (type_die, DW_AT_declaration, 1);
}

/* Generate a DIE for a subroutine _type_.  */

static void
gen_subroutine_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  register tree return_type = TREE_TYPE (type);
  register dw_die_ref subr_die
    = new_die (DW_TAG_subroutine_type, scope_die_for (type, context_die));

  equate_type_number_to_die (type, subr_die);
  add_prototyped_attribute (subr_die, type);
  add_type_attribute (subr_die, return_type, 0, 0, context_die);
  gen_formal_types_die (type, subr_die);
}

/* Generate a DIE for a type definition */

static void
gen_typedef_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register dw_die_ref type_die;
  register tree origin;

  if (TREE_ASM_WRITTEN (decl))
    return;
  TREE_ASM_WRITTEN (decl) = 1;

  type_die = new_die (DW_TAG_typedef, scope_die_for (decl, context_die));
  origin = decl_ultimate_origin (decl);
  if (origin != NULL)
    add_abstract_origin_attribute (type_die, origin);
  else
    {
      register tree type;
      add_name_and_src_coords_attributes (type_die, decl);
      if (DECL_ORIGINAL_TYPE (decl))
	{
	  type = DECL_ORIGINAL_TYPE (decl);
	  equate_type_number_to_die (TREE_TYPE (decl), type_die);
	}
      else
	type = TREE_TYPE (decl);
      add_type_attribute (type_die, type, TREE_READONLY (decl),
			  TREE_THIS_VOLATILE (decl), context_die);
    }

  if (DECL_ABSTRACT (decl))
    equate_decl_number_to_die (decl, type_die);
}

/* Generate a type description DIE.  */

static void
gen_type_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  if (type == NULL_TREE || type == error_mark_node)
    return;

  /* We are going to output a DIE to represent the unqualified version of
     this type (i.e. without any const or volatile qualifiers) so get the
     main variant (i.e. the unqualified version) of this type now.  */
  type = type_main_variant (type);

  if (TREE_ASM_WRITTEN (type))
    return;

  if (TYPE_NAME (type) && TREE_CODE (TYPE_NAME (type)) == TYPE_DECL
      && DECL_ORIGINAL_TYPE (TYPE_NAME (type)))
    { 
      TREE_ASM_WRITTEN (type) = 1;
      gen_decl_die (TYPE_NAME (type), context_die);
      return;
    }

  switch (TREE_CODE (type))
    {
    case ERROR_MARK:
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      /* We must set TREE_ASM_WRITTEN in case this is a recursive type.  This
	 ensures that the gen_type_die recursion will terminate even if the
	 type is recursive.  Recursive types are possible in Ada.  */
      /* ??? We could perhaps do this for all types before the switch
	 statement.  */
      TREE_ASM_WRITTEN (type) = 1;

      /* For these types, all that is required is that we output a DIE (or a
         set of DIEs) to represent the "basis" type.  */
      gen_type_die (TREE_TYPE (type), context_die);
      break;

    case OFFSET_TYPE:
      /* This code is used for C++ pointer-to-data-member types. 
	 Output a description of the relevant class type.  */
      gen_type_die (TYPE_OFFSET_BASETYPE (type), context_die);

      /* Output a description of the type of the object pointed to.  */
      gen_type_die (TREE_TYPE (type), context_die);

      /* Now output a DIE to represent this pointer-to-data-member type
         itself.  */
      gen_ptr_to_mbr_type_die (type, context_die);
      break;

    case SET_TYPE:
      gen_type_die (TYPE_DOMAIN (type), context_die);
      gen_set_type_die (type, context_die);
      break;

    case FILE_TYPE:
      gen_type_die (TREE_TYPE (type), context_die);
      abort ();			/* No way to represent these in Dwarf yet!  */
      break;

    case FUNCTION_TYPE:
      /* Force out return type (in case it wasn't forced out already).  */
      gen_type_die (TREE_TYPE (type), context_die);
      gen_subroutine_type_die (type, context_die);
      break;

    case METHOD_TYPE:
      /* Force out return type (in case it wasn't forced out already).  */
      gen_type_die (TREE_TYPE (type), context_die);
      gen_subroutine_type_die (type, context_die);
      break;

    case ARRAY_TYPE:
      if (TYPE_STRING_FLAG (type) && TREE_CODE (TREE_TYPE (type)) == CHAR_TYPE)
	{
	  gen_type_die (TREE_TYPE (type), context_die);
	  gen_string_type_die (type, context_die);
	}
      else
	gen_array_type_die (type, context_die);
      break;

    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      /* If this is a nested type whose containing class hasn't been
	 written out yet, writing it out will cover this one, too.  */
      if (TYPE_CONTEXT (type)
	  && AGGREGATE_TYPE_P (TYPE_CONTEXT (type))
	  && ! TREE_ASM_WRITTEN (TYPE_CONTEXT (type)))
	{
	  gen_type_die (TYPE_CONTEXT (type), context_die);

	  if (TREE_ASM_WRITTEN (TYPE_CONTEXT (type)))
	    return;

	  /* If that failed, attach ourselves to the stub.  */
	  push_decl_scope (TYPE_CONTEXT (type));
	  context_die = lookup_type_die (TYPE_CONTEXT (type));
	}

      if (TREE_CODE (type) == ENUMERAL_TYPE)
	gen_enumeration_type_die (type, context_die);
      else
	gen_struct_or_union_type_die (type, context_die);

      if (TYPE_CONTEXT (type)
	  && AGGREGATE_TYPE_P (TYPE_CONTEXT (type))
	  && ! TREE_ASM_WRITTEN (TYPE_CONTEXT (type)))
	pop_decl_scope ();

      /* Don't set TREE_ASM_WRITTEN on an incomplete struct; we want to fix
	 it up if it is ever completed.  gen_*_type_die will set it for us
	 when appropriate.  */
      return;

    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case COMPLEX_TYPE:
    case BOOLEAN_TYPE:
    case CHAR_TYPE:
      /* No DIEs needed for fundamental types.  */
      break;

    case LANG_TYPE:
      /* No Dwarf representation currently defined.  */
      break;

    default:
      abort ();
    }

  TREE_ASM_WRITTEN (type) = 1;
}

/* Generate a DIE for a tagged type instantiation.  */

static void
gen_tagged_type_instantiation_die (type, context_die)
     register tree type;
     register dw_die_ref context_die;
{
  if (type == NULL_TREE || type == error_mark_node)
    return;

  /* We are going to output a DIE to represent the unqualified version of
     this type (i.e. without any const or volatile qualifiers) so make sure
     that we have the main variant (i.e. the unqualified version) of this
     type now.  */
  if (type != type_main_variant (type)
      || !TREE_ASM_WRITTEN (type))
    abort ();

  switch (TREE_CODE (type))
    {
    case ERROR_MARK:
      break;

    case ENUMERAL_TYPE:
      gen_inlined_enumeration_type_die (type, context_die);
      break;

    case RECORD_TYPE:
      gen_inlined_structure_type_die (type, context_die);
      break;

    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      gen_inlined_union_type_die (type, context_die);
      break;

    default:
      abort ();
    }
}

/* Generate a DW_TAG_lexical_block DIE followed by DIEs to represent all of the
   things which are local to the given block.  */

static void
gen_block_die (stmt, context_die, depth)
     register tree stmt;
     register dw_die_ref context_die;
     int depth;
{
  register int must_output_die = 0;
  register tree origin;
  register tree decl;
  register enum tree_code origin_code;

  /* Ignore blocks never really used to make RTL.  */

  if (stmt == NULL_TREE || !TREE_USED (stmt))
    return;

  /* Determine the "ultimate origin" of this block.  This block may be an
     inlined instance of an inlined instance of inline function, so we have
     to trace all of the way back through the origin chain to find out what
     sort of node actually served as the original seed for the creation of
     the current block.  */
  origin = block_ultimate_origin (stmt);
  origin_code = (origin != NULL) ? TREE_CODE (origin) : ERROR_MARK;

  /* Determine if we need to output any Dwarf DIEs at all to represent this
     block.  */
  if (origin_code == FUNCTION_DECL)
    /* The outer scopes for inlinings *must* always be represented.  We
       generate DW_TAG_inlined_subroutine DIEs for them.  (See below.) */
    must_output_die = 1;
  else
    {
      /* In the case where the current block represents an inlining of the
         "body block" of an inline function, we must *NOT* output any DIE for 
         this block because we have already output a DIE to represent the
         whole inlined function scope and the "body block" of any function
         doesn't really represent a different scope according to ANSI C
         rules.  So we check here to make sure that this block does not
         represent a "body block inlining" before trying to set the
         `must_output_die' flag.  */
      if (! is_body_block (origin ? origin : stmt))
	{
	  /* Determine if this block directly contains any "significant"
	     local declarations which we will need to output DIEs for.  */
	  if (debug_info_level > DINFO_LEVEL_TERSE)
	    /* We are not in terse mode so *any* local declaration counts
	       as being a "significant" one.  */
	    must_output_die = (BLOCK_VARS (stmt) != NULL);
	  else
	    /* We are in terse mode, so only local (nested) function
	       definitions count as "significant" local declarations.  */
	    for (decl = BLOCK_VARS (stmt);
		 decl != NULL; decl = TREE_CHAIN (decl))
	      if (TREE_CODE (decl) == FUNCTION_DECL
		  && DECL_INITIAL (decl))
		{
		  must_output_die = 1;
		  break;
		}
	}
    }

  /* It would be a waste of space to generate a Dwarf DW_TAG_lexical_block
     DIE for any block which contains no significant local declarations at
     all.  Rather, in such cases we just call `decls_for_scope' so that any
     needed Dwarf info for any sub-blocks will get properly generated. Note
     that in terse mode, our definition of what constitutes a "significant"
     local declaration gets restricted to include only inlined function
     instances and local (nested) function definitions.  */
  if (must_output_die)
    {
      if (origin_code == FUNCTION_DECL)
	gen_inlined_subroutine_die (stmt, context_die, depth);
      else
	gen_lexical_block_die (stmt, context_die, depth);
    }
  else
    decls_for_scope (stmt, context_die, depth);
}

/* Generate all of the decls declared within a given scope and (recursively)
   all of its sub-blocks.  */

static void
decls_for_scope (stmt, context_die, depth)
     register tree stmt;
     register dw_die_ref context_die;
     int depth;
{
  register tree decl;
  register tree subblocks;

  /* Ignore blocks never really used to make RTL.  */
  if (stmt == NULL_TREE || ! TREE_USED (stmt))
    return;

  if (!BLOCK_ABSTRACT (stmt) && depth > 0)
    next_block_number++;

  /* Output the DIEs to represent all of the data objects and typedefs
     declared directly within this block but not within any nested
     sub-blocks.  Also, nested function and tag DIEs have been
     generated with a parent of NULL; fix that up now.  */
  for (decl = BLOCK_VARS (stmt);
       decl != NULL; decl = TREE_CHAIN (decl))
    {
      register dw_die_ref die;

      if (TREE_CODE (decl) == FUNCTION_DECL)
	die = lookup_decl_die (decl);
      else if (TREE_CODE (decl) == TYPE_DECL && TYPE_DECL_IS_STUB (decl))
	die = lookup_type_die (TREE_TYPE (decl));
      else
	die = NULL;

      if (die != NULL && die->die_parent == NULL)
	add_child_die (context_die, die);
      else
	gen_decl_die (decl, context_die);
    }

  /* Output the DIEs to represent all sub-blocks (and the items declared
     therein) of this block.  */
  for (subblocks = BLOCK_SUBBLOCKS (stmt);
       subblocks != NULL;
       subblocks = BLOCK_CHAIN (subblocks))
    gen_block_die (subblocks, context_die, depth + 1);
}

/* Is this a typedef we can avoid emitting?  */

static inline int
is_redundant_typedef (decl)
     register tree decl;
{
  if (TYPE_DECL_IS_STUB (decl))
    return 1;

  if (DECL_ARTIFICIAL (decl)
      && DECL_CONTEXT (decl)
      && is_tagged_type (DECL_CONTEXT (decl))
      && TREE_CODE (TYPE_NAME (DECL_CONTEXT (decl))) == TYPE_DECL
      && DECL_NAME (decl) == DECL_NAME (TYPE_NAME (DECL_CONTEXT (decl))))
    /* Also ignore the artificial member typedef for the class name.  */
    return 1;

  return 0;
}

/* Generate Dwarf debug information for a decl described by DECL.  */

static void
gen_decl_die (decl, context_die)
     register tree decl;
     register dw_die_ref context_die;
{
  register tree origin;

  /* Make a note of the decl node we are going to be working on.  We may need 
     to give the user the source coordinates of where it appeared in case we
     notice (later on) that something about it looks screwy.  */
  dwarf_last_decl = decl;

  if (TREE_CODE (decl) == ERROR_MARK)
    return;

  /* If this ..._DECL node is marked to be ignored, then ignore it. But don't 
     ignore a function definition, since that would screw up our count of
     blocks, and that in turn will completely screw up the labels we will 
     reference in subsequent DW_AT_low_pc and DW_AT_high_pc attributes (for
     subsequent blocks).  */
  if (DECL_IGNORED_P (decl) && TREE_CODE (decl) != FUNCTION_DECL)
    return;

  switch (TREE_CODE (decl))
    {
    case CONST_DECL:
      /* The individual enumerators of an enum type get output when we output 
         the Dwarf representation of the relevant enum type itself.  */
      break;

    case FUNCTION_DECL:
      /* Don't output any DIEs to represent mere function declarations,
	 unless they are class members or explicit block externs.  */
      if (DECL_INITIAL (decl) == NULL_TREE && DECL_CONTEXT (decl) == NULL_TREE
	  && (current_function_decl == NULL_TREE || ! DECL_ARTIFICIAL (decl)))
	break;

      if (debug_info_level > DINFO_LEVEL_TERSE)
	{
	  /* Before we describe the FUNCTION_DECL itself, make sure that we
	     have described its return type.  */
	  gen_type_die (TREE_TYPE (TREE_TYPE (decl)), context_die);

	  /* And its containing type.  */
	  origin = decl_class_context (decl);
	  if (origin != NULL_TREE)
	    gen_type_die (origin, context_die);

	  /* And its virtual context.  */
	  if (DECL_VINDEX (decl) != NULL_TREE)
	    gen_type_die (DECL_CONTEXT (decl), context_die);
	}

      /* Now output a DIE to represent the function itself.  */
      gen_subprogram_die (decl, context_die);
      break;

    case TYPE_DECL:
      /* If we are in terse mode, don't generate any DIEs to represent any
         actual typedefs.  */
      if (debug_info_level <= DINFO_LEVEL_TERSE)
	break;

      /* In the special case of a TYPE_DECL node representing the 
         declaration of some type tag, if the given TYPE_DECL is marked as
         having been instantiated from some other (original) TYPE_DECL node
         (e.g. one which was generated within the original definition of an
         inline function) we have to generate a special (abbreviated)
         DW_TAG_structure_type, DW_TAG_union_type, or DW_TAG_enumeration_type 
         DIE here.  */
      if (TYPE_DECL_IS_STUB (decl) && DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE)
	{
	  gen_tagged_type_instantiation_die (TREE_TYPE (decl), context_die);
	  break;
	}

      if (is_redundant_typedef (decl))
	gen_type_die (TREE_TYPE (decl), context_die);
      else
	/* Output a DIE to represent the typedef itself.  */
	gen_typedef_die (decl, context_die);
      break;

    case LABEL_DECL:
      if (debug_info_level >= DINFO_LEVEL_NORMAL)
	gen_label_die (decl, context_die);
      break;

    case VAR_DECL:
      /* If we are in terse mode, don't generate any DIEs to represent any
         variable declarations or definitions.  */
      if (debug_info_level <= DINFO_LEVEL_TERSE)
	break;

      /* Output any DIEs that are needed to specify the type of this data
         object.  */
      gen_type_die (TREE_TYPE (decl), context_die);

      /* And its containing type.  */
      origin = decl_class_context (decl);
      if (origin != NULL_TREE)
	gen_type_die (origin, context_die);

      /* Now output the DIE to represent the data object itself.  This gets
         complicated because of the possibility that the VAR_DECL really
         represents an inlined instance of a formal parameter for an inline
         function.  */
      origin = decl_ultimate_origin (decl);
      if (origin != NULL_TREE && TREE_CODE (origin) == PARM_DECL)
	gen_formal_parameter_die (decl, context_die);
      else
	gen_variable_die (decl, context_die);
      break;

    case FIELD_DECL:
      /* Ignore the nameless fields that are used to skip bits, but
	 handle C++ anonymous unions.  */
      if (DECL_NAME (decl) != NULL_TREE
	  || TREE_CODE (TREE_TYPE (decl)) == UNION_TYPE)
	{
	  gen_type_die (member_declared_type (decl), context_die);
	  gen_field_die (decl, context_die);
	}
      break;

    case PARM_DECL:
      gen_type_die (TREE_TYPE (decl), context_die);
      gen_formal_parameter_die (decl, context_die);
      break;

    default:
      abort ();
    }
}

/* Write the debugging output for DECL.  */

void
dwarf2out_decl (decl)
     register tree decl;
{
  register dw_die_ref context_die = comp_unit_die;

  if (TREE_CODE (decl) == ERROR_MARK)
    return;

  /* If this ..._DECL node is marked to be ignored, then ignore it.  We gotta 
     hope that the node in question doesn't represent a function definition.
     If it does, then totally ignoring it is bound to screw up our count of
     blocks, and that in turn will completely screw up the labels we will 
     reference in subsequent DW_AT_low_pc and DW_AT_high_pc attributes (for
     subsequent blocks).  (It's too bad that BLOCK nodes don't carry their
     own sequence numbers with them!) */
  if (DECL_IGNORED_P (decl))
    {
      if (TREE_CODE (decl) == FUNCTION_DECL
          && DECL_INITIAL (decl) != NULL)
	abort ();

      return;
    }

  switch (TREE_CODE (decl))
    {
    case FUNCTION_DECL:
      /* Ignore this FUNCTION_DECL if it refers to a builtin declaration of a 
         builtin function.  Explicit programmer-supplied declarations of
         these same functions should NOT be ignored however.  */
      if (DECL_EXTERNAL (decl) && DECL_FUNCTION_CODE (decl))
	return;

      /* What we would really like to do here is to filter out all mere
         file-scope declarations of file-scope functions which are never
         referenced later within this translation unit (and keep all of ones
         that *are* referenced later on) but we aren't clairvoyant, so we have 
         no idea which functions will be referenced in the future (i.e. later 
         on within the current translation unit). So here we just ignore all
         file-scope function declarations which are not also definitions.  If 
         and when the debugger needs to know something about these functions,
         it wil have to hunt around and find the DWARF information associated 
         with the definition of the function. Note that we can't just check
         `DECL_EXTERNAL' to find out which FUNCTION_DECL nodes represent
         definitions and which ones represent mere declarations.  We have to
         check `DECL_INITIAL' instead. That's because the C front-end
         supports some weird semantics for "extern inline" function
         definitions.  These can get inlined within the current translation
         unit (an thus, we need to generate DWARF info for their abstract
         instances so that the DWARF info for the concrete inlined instances
         can have something to refer to) but the compiler never generates any 
         out-of-lines instances of such things (despite the fact that they
         *are* definitions).  The important point is that the C front-end
         marks these "extern inline" functions as DECL_EXTERNAL, but we need
         to generate DWARF for them anyway. Note that the C++ front-end also
         plays some similar games for inline function definitions appearing
         within include files which also contain 
	 `#pragma interface' pragmas.  */
      if (DECL_INITIAL (decl) == NULL_TREE)
	return;

      /* If we're a nested function, initially use a parent of NULL; if we're
	 a plain function, this will be fixed up in decls_for_scope.  If
	 we're a method, it will be ignored, since we already have a DIE.  */
      if (decl_function_context (decl))
	context_die = NULL;

      break;

    case VAR_DECL:
      /* Ignore this VAR_DECL if it refers to a file-scope extern data object 
         declaration and if the declaration was never even referenced from
         within this entire compilation unit.  We suppress these DIEs in
         order to save space in the .debug section (by eliminating entries
         which are probably useless).  Note that we must not suppress
         block-local extern declarations (whether used or not) because that
         would screw-up the debugger's name lookup mechanism and cause it to
         miss things which really ought to be in scope at a given point.  */
      if (DECL_EXTERNAL (decl) && !TREE_USED (decl))
	return;

      /* If we are in terse mode, don't generate any DIEs to represent any
         variable declarations or definitions.  */
      if (debug_info_level <= DINFO_LEVEL_TERSE)
	return;
      break;

    case TYPE_DECL:
      /* Don't bother trying to generate any DIEs to represent any of the
         normal built-in types for the language we are compiling.  */
      if (DECL_SOURCE_LINE (decl) == 0)
	{
	  /* OK, we need to generate one for `bool' so GDB knows what type
             comparisons have.  */
	  if ((get_AT_unsigned (comp_unit_die, DW_AT_language)
	       == DW_LANG_C_plus_plus)
	      && TREE_CODE (TREE_TYPE (decl)) == BOOLEAN_TYPE)
	    modified_type_die (TREE_TYPE (decl), 0, 0, NULL);

	  return;
	}

      /* If we are in terse mode, don't generate any DIEs for types.  */
      if (debug_info_level <= DINFO_LEVEL_TERSE)
	return;

      /* If we're a function-scope tag, initially use a parent of NULL;
	 this will be fixed up in decls_for_scope.  */
      if (decl_function_context (decl))
	context_die = NULL;

      break;

    default:
      return;
    }

  gen_decl_die (decl, context_die);
  output_pending_types_for_scope (comp_unit_die);
}

/* Output a marker (i.e. a label) for the beginning of the generated code for
   a lexical block.  */

void
dwarf2out_begin_block (blocknum)
     register unsigned blocknum;
{
  function_section (current_function_decl);
  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, BLOCK_BEGIN_LABEL, blocknum);
}

/* Output a marker (i.e. a label) for the end of the generated code for a
   lexical block.  */

void
dwarf2out_end_block (blocknum)
     register unsigned blocknum;
{
  function_section (current_function_decl);
  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, BLOCK_END_LABEL, blocknum);
}

/* Output a marker (i.e. a label) at a point in the assembly code which
   corresponds to a given source level label.  */

void
dwarf2out_label (insn)
     register rtx insn;
{
  char label[MAX_ARTIFICIAL_LABEL_BYTES];

  if (debug_info_level >= DINFO_LEVEL_NORMAL)
    {
      function_section (current_function_decl);
      sprintf (label, INSN_LABEL_FMT, current_funcdef_number);
      ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, label,
				 (unsigned) INSN_UID (insn));
    }
}

/* Lookup a filename (in the list of filenames that we know about here in
   dwarf2out.c) and return its "index".  The index of each (known) filename is
   just a unique number which is associated with only that one filename.
   We need such numbers for the sake of generating labels
   (in the .debug_sfnames section) and references to those
   files  numbers (in the .debug_srcinfo and.debug_macinfo sections).
   If the filename given as an argument is not found in our current list,
   add it to the list and assign it the next available unique index number.
   In order to speed up searches, we remember the index of the filename
   was looked up last.  This handles the majority of all searches.  */

static unsigned
lookup_filename (file_name)
     char *file_name;
{
  static unsigned last_file_lookup_index = 0;
  register unsigned i;

  /* Check to see if the file name that was searched on the previous call
     matches this file name. If so, return the index.  */
  if (last_file_lookup_index != 0)
    if (strcmp (file_name, file_table[last_file_lookup_index]) == 0)
      return last_file_lookup_index;

  /* Didn't match the previous lookup, search the table */
  for (i = 1; i < file_table_in_use; ++i)
    if (strcmp (file_name, file_table[i]) == 0)
      {
	last_file_lookup_index = i;
	return i;
      }

  /* Prepare to add a new table entry by making sure there is enough space in 
     the table to do so.  If not, expand the current table.  */
  if (file_table_in_use == file_table_allocated)
    {
      file_table_allocated += FILE_TABLE_INCREMENT;
      file_table
	= (char **) xrealloc (file_table,
			      file_table_allocated * sizeof (char *));
    }

  /* Add the new entry to the end of the filename table.  */
  file_table[file_table_in_use] = xstrdup (file_name);
  last_file_lookup_index = file_table_in_use++;

  return last_file_lookup_index;
}

/* Output a label to mark the beginning of a source code line entry
   and record information relating to this source line, in
   'line_info_table' for later output of the .debug_line section.  */

void
dwarf2out_line (filename, line)
     register char *filename;
     register unsigned line;
{
  if (debug_info_level >= DINFO_LEVEL_NORMAL)
    {
      function_section (current_function_decl);

      if (DECL_SECTION_NAME (current_function_decl))
	{
	  register dw_separate_line_info_ref line_info;
	  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, SEPARATE_LINE_CODE_LABEL,
				     separate_line_info_table_in_use);
	  fputc ('\n', asm_out_file);

	  /* expand the line info table if necessary */
	  if (separate_line_info_table_in_use
	      == separate_line_info_table_allocated)
	    {
	      separate_line_info_table_allocated += LINE_INFO_TABLE_INCREMENT;
	      separate_line_info_table
		= (dw_separate_line_info_ref)
		  xrealloc (separate_line_info_table,
			    separate_line_info_table_allocated
			    * sizeof (dw_separate_line_info_entry));
	    }

	  /* Add the new entry at the end of the line_info_table.  */
	  line_info
	    = &separate_line_info_table[separate_line_info_table_in_use++];
	  line_info->dw_file_num = lookup_filename (filename);
	  line_info->dw_line_num = line;
	  line_info->function = current_funcdef_number;
	}
      else
	{
	  register dw_line_info_ref line_info;

	  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, LINE_CODE_LABEL,
				     line_info_table_in_use);
	  fputc ('\n', asm_out_file);

	  /* Expand the line info table if necessary.  */
	  if (line_info_table_in_use == line_info_table_allocated)
	    {
	      line_info_table_allocated += LINE_INFO_TABLE_INCREMENT;
	      line_info_table
		= (dw_line_info_ref)
		  xrealloc (line_info_table,
			    (line_info_table_allocated
			     * sizeof (dw_line_info_entry)));
	    }

	  /* Add the new entry at the end of the line_info_table.  */
	  line_info = &line_info_table[line_info_table_in_use++];
	  line_info->dw_file_num = lookup_filename (filename);
	  line_info->dw_line_num = line;
	}
    }
}

/* Record the beginning of a new source file, for later output
   of the .debug_macinfo section.  At present, unimplemented.  */

void
dwarf2out_start_source_file (filename)
     register char *filename ATTRIBUTE_UNUSED;
{
}

/* Record the end of a source file, for later output
   of the .debug_macinfo section.  At present, unimplemented.  */

void
dwarf2out_end_source_file ()
{
}

/* Called from check_newline in c-parse.y.  The `buffer' parameter contains
   the tail part of the directive line, i.e. the part which is past the
   initial whitespace, #, whitespace, directive-name, whitespace part.  */

void
dwarf2out_define (lineno, buffer)
     register unsigned lineno;
     register char *buffer;
{
  static int initialized = 0;
  if (!initialized)
    {
      dwarf2out_start_source_file (primary_filename);
      initialized = 1;
    }
}

/* Called from check_newline in c-parse.y.  The `buffer' parameter contains
   the tail part of the directive line, i.e. the part which is past the
   initial whitespace, #, whitespace, directive-name, whitespace part.  */

void
dwarf2out_undef (lineno, buffer)
     register unsigned lineno ATTRIBUTE_UNUSED;
     register char *buffer ATTRIBUTE_UNUSED;
{
}

/* Set up for Dwarf output at the start of compilation.  */

void
dwarf2out_init (asm_out_file, main_input_filename)
     register FILE *asm_out_file;
     register char *main_input_filename;
{
  /* Remember the name of the primary input file.  */
  primary_filename = main_input_filename;

  /* Allocate the initial hunk of the file_table.  */
  file_table = (char **) xmalloc (FILE_TABLE_INCREMENT * sizeof (char *));
  zero_memory ((char *) file_table, FILE_TABLE_INCREMENT * sizeof (char *));
  file_table_allocated = FILE_TABLE_INCREMENT;

  /* Skip the first entry - file numbers begin at 1.  */
  file_table_in_use = 1;

  /* Allocate the initial hunk of the decl_die_table.  */
  decl_die_table
    = (dw_die_ref *) xmalloc (DECL_DIE_TABLE_INCREMENT * sizeof (dw_die_ref));
  zero_memory ((char *) decl_die_table,
	 DECL_DIE_TABLE_INCREMENT * sizeof (dw_die_ref));
  decl_die_table_allocated = DECL_DIE_TABLE_INCREMENT;
  decl_die_table_in_use = 0;

  /* Allocate the initial hunk of the decl_scope_table.  */
  decl_scope_table
    = (decl_scope_node *) xmalloc (DECL_SCOPE_TABLE_INCREMENT
				   * sizeof (decl_scope_node));
  zero_memory ((char *) decl_scope_table,
	 DECL_SCOPE_TABLE_INCREMENT * sizeof (decl_scope_node));
  decl_scope_table_allocated = DECL_SCOPE_TABLE_INCREMENT;
  decl_scope_depth = 0;

  /* Allocate the initial hunk of the abbrev_die_table.  */
  abbrev_die_table
    = (dw_die_ref *) xmalloc (ABBREV_DIE_TABLE_INCREMENT
			      * sizeof (dw_die_ref));
  zero_memory ((char *) abbrev_die_table,
	 ABBREV_DIE_TABLE_INCREMENT * sizeof (dw_die_ref));
  abbrev_die_table_allocated = ABBREV_DIE_TABLE_INCREMENT;
  /* Zero-th entry is allocated, but unused */
  abbrev_die_table_in_use = 1;

  /* Allocate the initial hunk of the line_info_table.  */
  line_info_table
    = (dw_line_info_ref) xmalloc (LINE_INFO_TABLE_INCREMENT
				  * sizeof (dw_line_info_entry));
  zero_memory ((char *) line_info_table,
	 LINE_INFO_TABLE_INCREMENT * sizeof (dw_line_info_entry));
  line_info_table_allocated = LINE_INFO_TABLE_INCREMENT;
  /* Zero-th entry is allocated, but unused */
  line_info_table_in_use = 1;

  /* Generate the initial DIE for the .debug section.  Note that the (string) 
     value given in the DW_AT_name attribute of the DW_TAG_compile_unit DIE
     will (typically) be a relative pathname and that this pathname should be 
     taken as being relative to the directory from which the compiler was
     invoked when the given (base) source file was compiled.  */
  gen_compile_unit_die (main_input_filename);

  ASM_GENERATE_INTERNAL_LABEL (text_end_label, TEXT_END_LABEL, 0);
}

/* Output stuff that dwarf requires at the end of every file,
   and generate the DWARF-2 debugging info.  */

void
dwarf2out_finish ()
{
  limbo_die_node *node, *next_node;
  dw_die_ref die;
  dw_attr_ref a;

  /* Traverse the limbo die list, and add parent/child links.  The only
     dies without parents that should be here are concrete instances of
     inline functions, and the comp_unit_die.  We can ignore the comp_unit_die.
     For concrete instances, we can get the parent die from the abstract
     instance.  */
  for (node = limbo_die_list; node; node = next_node)
    {
      next_node = node->next;
      die = node->die;

      if (die->die_parent == NULL)
	{
	  a = get_AT (die, DW_AT_abstract_origin);
	  if (a)
	    add_child_die (a->dw_attr_val.v.val_die_ref->die_parent, die);
	  else if (die == comp_unit_die)
	      ;
	  else
	    abort ();
	}
      free (node);
    }

  /* Traverse the DIE tree and add sibling attributes to those DIE's
     that have children.  */
  add_sibling_attributes (comp_unit_die);

  /* Output a terminator label for the .text section.  */
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_SECTION (asm_out_file, TEXT_SECTION);
  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, TEXT_END_LABEL, 0);

#if 0
  /* Output a terminator label for the .data section.  */
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_SECTION (asm_out_file, DATA_SECTION);
  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, DATA_END_LABEL, 0);

  /* Output a terminator label for the .bss section.  */
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_SECTION (asm_out_file, BSS_SECTION);
  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, BSS_END_LABEL, 0);
#endif

  /* Output the source line correspondence table.  */
  if (line_info_table_in_use > 1 || separate_line_info_table_in_use)
    {
      fputc ('\n', asm_out_file);
      ASM_OUTPUT_SECTION (asm_out_file, DEBUG_LINE_SECTION);
      output_line_info ();

      /* We can only use the low/high_pc attributes if all of the code
	 was in .text.  */
      if (separate_line_info_table_in_use == 0)
	{
	  add_AT_lbl_id (comp_unit_die, DW_AT_low_pc,
			 stripattributes (TEXT_SECTION));
	  add_AT_lbl_id (comp_unit_die, DW_AT_high_pc, text_end_label);
	}

      add_AT_section_offset (comp_unit_die, DW_AT_stmt_list, DEBUG_LINE_SECTION);
    }

  /* Output the abbreviation table.  */
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_SECTION (asm_out_file, ABBREV_SECTION);
  build_abbrev_table (comp_unit_die);
  output_abbrev_section ();

  /* Initialize the beginning DIE offset - and calculate sizes/offsets.   */
  next_die_offset = DWARF_COMPILE_UNIT_HEADER_SIZE;
  calc_die_sizes (comp_unit_die);

  /* Output debugging information.  */
  fputc ('\n', asm_out_file);
  ASM_OUTPUT_SECTION (asm_out_file, DEBUG_INFO_SECTION);
  output_compilation_unit_header ();
  output_die (comp_unit_die);

  if (pubname_table_in_use)
    {
      /* Output public names table.  */
      fputc ('\n', asm_out_file);
      ASM_OUTPUT_SECTION (asm_out_file, PUBNAMES_SECTION);
      output_pubnames ();
    }

  if (fde_table_in_use)
    {
      /* Output the address range information.  */
      fputc ('\n', asm_out_file);
      ASM_OUTPUT_SECTION (asm_out_file, ARANGES_SECTION);
      output_aranges ();
    }
}
#endif /* DWARF2_DEBUGGING_INFO */
