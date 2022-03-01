/* CYGNUS LOCAL (entire file) clm/arm-elf */
/* Definitions of target machine for GNU compiler,
   for Thumb with ELF obj format.
   Copyright (C) 1995, 1996 Free Software Foundation, Inc.
   
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

#define OBJECT_FORMAT_ELF

#define CPP_PREDEFINES "-Dthumb -Dthumbelf -D__thumb -Acpu(arm) -Amachine(arm)"

#include "arm/thumb.h"

/* Run-time Target Specification.  */
#undef TARGET_VERSION
#define TARGET_VERSION fputs (" (Thumb/elf)", stderr)

#define MULTILIB_DEFAULTS { "mlittle-endian" }

/* Setting this to 32 produces more efficient code, but the value set in previous
   versions of this toolchain was 8, which produces more compact structures. The
   command line option -mstructure_size_boundary=<n> can be used to change this
   value.  */
#undef STRUCTURE_SIZE_BOUNDARY
#define STRUCTURE_SIZE_BOUNDARY arm_structure_size_boundary

extern int arm_structure_size_boundary;

/* Debug */
#define DWARF_DEBUGGING_INFO
#define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG


/* Note - it is important that these definitions match those in semi.h for the ARM port.  */
#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX "."


#undef ASM_FILE_START
#define ASM_FILE_START(STREAM) \
do {								\
  extern char *version_string;					\
  fprintf ((STREAM), "%s Generated by gcc %s for Thumb/elf\n", \
	   ASM_COMMENT_START, version_string);	                \
  fprintf ((STREAM), ASM_APP_OFF);                              \
} while (0)

/* A C statement to output something to the assembler file to switch to section
   NAME for object DECL which is either a FUNCTION_DECL, a VAR_DECL or
   NULL_TREE.  Some target formats do not support arbitrary sections.  Do not
   define this macro in such cases.  */
#define ASM_OUTPUT_SECTION_NAME(STREAM, DECL, NAME, RELOC)        \
do {								  \
  if ((DECL) && TREE_CODE (DECL) == FUNCTION_DECL)		  \
    fprintf (STREAM, "\t.section %s,\"ax\",%%progbits\n", (NAME)); \
  else if ((DECL) && DECL_READONLY_SECTION (DECL, RELOC))	  \
    fprintf (STREAM, "\t.section %s,\"a\"\n", (NAME));		  \
  else if (0 == strncmp((NAME), ".bss", sizeof(".bss") - 1))      \
      fprintf (STREAM, "\t.section %s,\"aw\",%%nobits\n", (NAME)); \
  else							 	  \
      fprintf (STREAM, "\t.section %s,\"aw\"\n", (NAME));	  \
} while (0)

/* Support the ctors/dtors and other sections.  */

#undef INIT_SECTION_ASM_OP

/* Define this macro if jump tables (for `tablejump' insns) should be
   output in the text section, along with the assembler instructions.
   Otherwise, the readonly data section is used.  */
#define JUMP_TABLES_IN_TEXT_SECTION 1

#undef READONLY_DATA_SECTION
#define READONLY_DATA_SECTION	rdata_section
#undef RDATA_SECTION_ASM_OP
#define RDATA_SECTION_ASM_OP	"\t.section .rodata"

#undef  CTORS_SECTION_ASM_OP
#define CTORS_SECTION_ASM_OP	"\t.section .ctors,\"aw\""
#undef  DTORS_SECTION_ASM_OP
#define DTORS_SECTION_ASM_OP	"\t.section .dtors,\"aw\""

#define USER_LABEL_PREFIX ""

/* If defined, a C expression whose value is a string containing the
   assembler operation to identify the following data as
   uninitialized global data.  If not defined, and neither
   `ASM_OUTPUT_BSS' nor `ASM_OUTPUT_ALIGNED_BSS' are defined,
   uninitialized global data will be output in the data section if
   `-fno-common' is passed, otherwise `ASM_OUTPUT_COMMON' will be
   used.  */
#ifndef BSS_SECTION_ASM_OP
#define BSS_SECTION_ASM_OP	".section\t.bss"
#endif

/* Like `ASM_OUTPUT_BSS' except takes the required alignment as a
   separate, explicit argument.  If you define this macro, it is used
   in place of `ASM_OUTPUT_BSS', and gives you more flexibility in
   handling the required alignment of the variable.  The alignment is
   specified as the number of bits.

   Try to use function `asm_output_aligned_bss' defined in file
   `varasm.c' when defining this macro. */
#ifndef ASM_OUTPUT_ALIGNED_BSS
#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)
#endif

/* Don't know how to order these.  UNALIGNED_WORD_ASM_OP is in
   dwarf2.out. */ 
#define UNALIGNED_WORD_ASM_OP ".4byte"

#define ASM_OUTPUT_DWARF2_ADDR_CONST(FILE,ADDR)				\
     if (((ADDR)[0] == '.') && ((ADDR)[1] == 'L')) 			\
       fprintf ((FILE), "\t%s\t%s", UNALIGNED_WORD_ASM_OP, (ADDR));	\
     else                                                     		\
       fprintf ((FILE), "\t%s\t%s",					\
             UNALIGNED_WORD_ASM_OP, (ADDR))

#define ASM_OUTPUT_DWARF_ADDR_CONST(FILE,RTX)                   \
do {								\
  fprintf ((FILE), "\t%s\t", UNALIGNED_WORD_ASM_OP);		\
  output_addr_const ((FILE), (RTX));				\
  fputc ('\n', (FILE));						\
} while (0)

/* This is how to equate one symbol to another symbol.  The syntax used is
   `SYM1=SYM2'.  Note that this is different from the way equates are done
   with most svr4 assemblers, where the syntax is `.set SYM1,SYM2'.  */

#define ASM_OUTPUT_DEF(FILE,LABEL1,LABEL2)				\
 do {	fprintf ((FILE), "\t");						\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, " = ");						\
	assemble_name (FILE, LABEL2);					\
	fprintf (FILE, "\n");						\
  } while (0)

/* For aliases of functions we use .thumb_set instead.  */
#define ASM_OUTPUT_DEF_FROM_DECLS(FILE,DECL1,DECL2)		\
  do						   		\
    {								\
      char * LABEL1 = XSTR (XEXP (DECL_RTL (decl), 0), 0);	\
      char * LABEL2 = IDENTIFIER_POINTER (DECL2);		\
								\
      if (TREE_CODE (DECL1) == FUNCTION_DECL)			\
	{							\
	  fprintf (FILE, "\t.thumb_set ");			\
	  assemble_name (FILE, LABEL1);			   	\
	  fprintf (FILE, ",");			   		\
	  assemble_name (FILE, LABEL2);		   		\
	  fprintf (FILE, "\n");					\
	}							\
      else							\
	ASM_OUTPUT_DEF (FILE, LABEL1, LABEL2);			\
    }								\
  while (0)

/* A list of other sections which the compiler might be "in" at any
   given time.  */
#undef EXTRA_SECTIONS
#define EXTRA_SECTIONS SUBTARGET_EXTRA_SECTIONS in_rdata, in_ctors, in_dtors

#define SUBTARGET_EXTRA_SECTIONS

/* A list of extra section function definitions.  */

#undef EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS \
  RDATA_SECTION_FUNCTION	\
  CTORS_SECTION_FUNCTION	\
  DTORS_SECTION_FUNCTION	\
  SUBTARGET_EXTRA_SECTION_FUNCTIONS

#define SUBTARGET_EXTRA_SECTION_FUNCTIONS

#define RDATA_SECTION_FUNCTION \
void									\
rdata_section ()							\
{									\
  if (in_section != in_rdata)						\
    {									\
      fprintf (asm_out_file, "%s\n", RDATA_SECTION_ASM_OP);		\
      in_section = in_rdata;						\
    }									\
}

#define CTOR_LIST_BEGIN                                 \
asm (CTORS_SECTION_ASM_OP);                             \
func_ptr __CTOR_LIST__[1] = { (func_ptr) (-1) }

#define CTOR_LIST_END                                   \
asm (CTORS_SECTION_ASM_OP);                             \
func_ptr __CTOR_END__[1] = { (func_ptr) 0 };

#define DTOR_LIST_BEGIN                                 \
asm (DTORS_SECTION_ASM_OP);                             \
func_ptr __DTOR_LIST__[1] = { (func_ptr) (-1) }

#define DTOR_LIST_END                                   \
asm (DTORS_SECTION_ASM_OP);                             \
func_ptr __DTOR_END__[1] = { (func_ptr) 0 };

#define CTORS_SECTION_FUNCTION \
void									\
ctors_section ()							\
{									\
  if (in_section != in_ctors)						\
    {									\
      fprintf (asm_out_file, "%s\n", CTORS_SECTION_ASM_OP);		\
      in_section = in_ctors;						\
    }									\
}

#define DTORS_SECTION_FUNCTION \
void									\
dtors_section ()							\
{									\
  if (in_section != in_dtors)						\
    {									\
      fprintf (asm_out_file, "%s\n", DTORS_SECTION_ASM_OP);		\
      in_section = in_dtors;						\
    }									\
}

/* Support the ctors/dtors sections for g++.  */

#define INT_ASM_OP ".word"

#define INVOKE__main

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crtbegin%O%s crt0%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend%O%s"

/* A C statement (sans semicolon) to output an element in the table of
   global constructors.  */
#undef ASM_OUTPUT_CONSTRUCTOR
#define ASM_OUTPUT_CONSTRUCTOR(STREAM,NAME) \
do {						\
  ctors_section ();				\
  fprintf (STREAM, "\t%s\t ", INT_ASM_OP);	\
  assemble_name (STREAM, NAME);			\
  fprintf (STREAM, "\n");			\
} while (0)

/* A C statement (sans semicolon) to output an element in the table of
   global destructors.  */
#undef ASM_OUTPUT_DESTRUCTOR
#define ASM_OUTPUT_DESTRUCTOR(STREAM,NAME) \
do {						\
  dtors_section ();                   		\
  fprintf (STREAM, "\t%s\t ", INT_ASM_OP);	\
  assemble_name (STREAM, NAME);              	\
  fprintf (STREAM, "\n");			\
} while (0)

/* The ARM development system has atexit and doesn't have _exit,
   so define this for now.  */
#define HAVE_ATEXIT

/* The ARM development system defines __main.  */
#define NAME__MAIN "__gccmain"
#define SYMBOL__MAIN __gccmain

#define MAKE_DECL_ONE_ONLY(DECL) (DECL_WEAK (DECL) = 1)
#define UNIQUE_SECTION_P(DECL) (DECL_ONE_ONLY (DECL))
#define UNIQUE_SECTION(DECL,RELOC)				\
do {								\
  int len;							\
  char * name, * string, * prefix;				\
								\
  name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (DECL));	\
								\
  if (! DECL_ONE_ONLY (DECL))					\
    {								\
      prefix = ".";                                             \
      if (TREE_CODE (DECL) == FUNCTION_DECL)			\
	prefix = ".text.";					\
      else if (DECL_READONLY_SECTION (DECL, RELOC))		\
	prefix = ".rodata.";					\
      else							\
	prefix = ".data.";					\
    }								\
  else if (TREE_CODE (DECL) == FUNCTION_DECL)			\
    prefix = ".gnu.linkonce.t.";				\
  else if (DECL_READONLY_SECTION (DECL, RELOC))			\
    prefix = ".gnu.linkonce.r.";				\
  else								\
    prefix = ".gnu.linkonce.d.";				\
								\
  len = strlen (name) + strlen (prefix);			\
  string = alloca (len + 1);					\
  sprintf (string, "%s%s", prefix, name);			\
								\
  DECL_SECTION_NAME (DECL) = build_string (len, string);	\
} while (0)

/* This is how we tell the assembler that a symbol is weak.  */
#ifndef ASM_WEAKEN_LABEL
#define ASM_WEAKEN_LABEL(FILE, NAME) 		\
  do						\
    {						\
      fputs ("\t.weak\t", FILE);		\
      assemble_name (FILE, NAME); 		\
      fputc ('\n', FILE);			\
    }						\
  while (0)
#endif

#ifndef TYPE_ASM_OP

/* These macros generate the special .type and .size directives which
   are used to set the corresponding fields of the linker symbol table
   entries in an ELF object file under SVR4.  These macros also output
   the starting labels for the relevant functions/objects.  */
#define TYPE_ASM_OP     ".type"
#define SIZE_ASM_OP     ".size"

/* The following macro defines the format used to output the second
   operand of the .type assembler directive.  Different svr4 assemblers
   expect various different forms for this operand.  The one given here
   is just a default.  You may need to override it in your machine-
   specific tm.h file (depending upon the particulars of your assembler).  */
#define TYPE_OPERAND_FMT	"%s"

/* Write the extra assembler code needed to declare a function's result.
   Most svr4 assemblers don't require any special declaration of the
   result value, but there are exceptions.  */
#ifndef ASM_DECLARE_RESULT
#define ASM_DECLARE_RESULT(FILE, RESULT)
#endif

/* Write the extra assembler code needed to declare a function properly.
   Some svr4 assemblers need to also have something extra said about the
   function's return value.  We allow for that here.  */
#undef  ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)	\
  do							\
    {							\
      fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);		\
      assemble_name (FILE, NAME);			\
      putc (',', FILE);					\
      fprintf (FILE, TYPE_OPERAND_FMT, "function");	\
      putc ('\n', FILE);				\
      ASM_DECLARE_RESULT (FILE, DECL_RESULT (DECL));	\
      if (! is_called_in_ARM_mode (decl))		\
        fprintf (FILE, "\t.thumb_func\n") ;		\
      else						\
        fprintf (FILE, "\t.code\t32\n") ;		\
      ASM_OUTPUT_LABEL(FILE, NAME);			\
    }							\
  while (0)

/* Write the extra assembler code needed to declare an object properly.  */
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)		\
  do								\
    {								\
      fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);			\
      assemble_name (FILE, NAME);				\
      putc (',', FILE);						\
      fprintf (FILE, TYPE_OPERAND_FMT, "object");		\
      putc ('\n', FILE);					\
      size_directive_output = 0;				\
      if (!flag_inhibit_size_directive && DECL_SIZE (DECL))	\
        {							\
	  size_directive_output = 1;				\
	  fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);		\
	  assemble_name (FILE, NAME);				\
	  putc (',', FILE);					\
	  fprintf (FILE, HOST_WIDE_INT_PRINT_DEC,		\
		   int_size_in_bytes (TREE_TYPE (DECL)));	\
	  fputc ('\n', FILE);					\
        }							\
      ASM_OUTPUT_LABEL(FILE, NAME);				\
    }								\
  while (0)

/* Output the size directive for a decl in rest_of_decl_compilation
   in the case where we did not do so before the initializer.
   Once we find the error_mark_node, we know that the value of
   size_directive_output was set
   by ASM_DECLARE_OBJECT_NAME when it was run for the same decl.  */
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP_LEVEL, AT_END)	\
  do									\
    {									\
      char * name = XSTR (XEXP (DECL_RTL (DECL), 0), 0);		\
      if (!flag_inhibit_size_directive && DECL_SIZE (DECL)		\
          && ! AT_END && TOP_LEVEL					\
	  && DECL_INITIAL (DECL) == error_mark_node			\
	  && !size_directive_output)					\
        {								\
	  size_directive_output = 1;					\
	  fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);			\
	  assemble_name (FILE, name);					\
	  putc (',', FILE);						\
	  fprintf (FILE, HOST_WIDE_INT_PRINT_DEC,			\
		  int_size_in_bytes (TREE_TYPE (DECL)));		\
	 fputc ('\n', FILE);						\
        }								\
    }									\
  while (0)

/* This is how to declare the size of a function.  */
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)		\
  do								\
    {								\
      if (!flag_inhibit_size_directive)				\
        {							\
          char label[256];					\
	  static int labelno;					\
	  labelno ++;						\
	  ASM_GENERATE_INTERNAL_LABEL (label, "Lfe", labelno);	\
	  ASM_OUTPUT_INTERNAL_LABEL (FILE, "Lfe", labelno);	\
	  fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);		\
	  assemble_name (FILE, (FNAME));			\
          fprintf (FILE, ",");					\
	  assemble_name (FILE, label);				\
          fprintf (FILE, "-");					\
	  assemble_name (FILE, (FNAME));			\
	  putc ('\n', FILE);					\
        }							\
    }								\
  while (0)

#endif /* TYPE_ASM_OP */
