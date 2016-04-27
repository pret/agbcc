/* Declarations for insn-output.c.  These functions are defined in recog.c,
   final.c, and varasm.c.
   Copyright (C) 1987, 1991, 1994, 1997, 1998 Free Software Foundation, Inc.

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

/* Initialize data in final at the beginning of a compilation.  */
extern void init_final		(char *);

/* Called at end of source file,
   to output the block-profiling table for this entire compilation.  */
extern void end_final		(char *);

/* Enable APP processing of subsequent output.
   Used before the output from an `asm' statement.  */
extern void app_enable		(void);

/* Disable APP processing of subsequent output.
   Called from varasm.c before most kinds of output.  */
extern void app_disable		(void);

/* Return the number of slots filled in the current 
   delayed branch sequence (we don't count the insn needing the
   delay slot).   Zero if not in a delayed branch sequence.  */
extern int dbr_sequence_length	(void);

/* Indicate that branch shortening hasn't yet been done.  */
extern void init_insn_lengths	(void);

#ifdef RTX_CODE
/* Obtain the current length of an insn.  If branch shortening has been done,
   get its actual length.  Otherwise, get its maximum length.  */
extern int get_attr_length	(rtx);

/* Make a pass over all insns and compute their actual lengths by shortening
   any branches of variable length if possible.  */
extern void shorten_branches	(rtx);

/* Output assembler code for the start of a function,
   and initialize some of the variables in this file
   for the new function.  The label for the function and associated
   assembler pseudo-ops have already been output in
   `assemble_start_function'.  */
extern void final_start_function  (rtx, FILE *, int);

/* Output assembler code for the end of a function.
   For clarity, args are same as those of `final_start_function'
   even though not all of them are needed.  */
extern void final_end_function  (rtx, FILE *, int);

/* Output assembler code for some insns: all or part of a function.  */
extern void final		(rtx, FILE *, int, int);

/* The final scan for one insn, INSN.  Args are same as in `final', except
   that INSN is the insn being scanned.  Value returned is the next insn to
   be scanned.  */
extern rtx final_scan_insn	(rtx, FILE *, int, int, int);

/* Replace a SUBREG with a REG or a MEM, based on the thing it is a
   subreg of.  */
extern rtx alter_subreg (rtx);

/* Report inconsistency between the assembler template and the operands.
   In an `asm', it's the user's fault; otherwise, the compiler's fault.  */
extern void output_operand_lossage  (char *);

/* Output a string of assembler code, substituting insn operands.
   Defined in final.c.  */
extern void output_asm_insn	(char *, rtx *);

/* Compute a worst-case reference address of a branch so that it
   can be safely used in the presence of aligned labels.
   Defined in final.c.  */
extern int insn_current_reference_address	(rtx);

/* Find the alignment associated with a CODE_LABEL.
   Defined in final.c.  */
extern int label_to_alignment	(rtx);

/* Output a LABEL_REF, or a bare CODE_LABEL, as an assembler symbol.  */
extern void output_asm_label	(rtx);

/* Print a memory reference operand for address X
   using machine-dependent assembler syntax.  */
extern void output_address	(rtx);

/* Print an integer constant expression in assembler syntax.
   Addition and subtraction are the only arithmetic
   that may appear in these expressions.  */
extern void output_addr_const (FILE *, rtx);

/* Output a string of assembler code, substituting numbers, strings
   and fixed syntactic prefixes.  */
extern void asm_fprintf		(FILE *file, char *p, ...);

/* Split up a CONST_DOUBLE or integer constant rtx into two rtx's for single
   words.  */
extern void split_double	(rtx, rtx *, rtx *);

/* Return nonzero if this function has no function calls.  */
extern int leaf_function_p	(void);

/* Return 1 if this function uses only the registers that can be
   safely renumbered.  */
extern int only_leaf_regs_used	(void);

/* Scan IN_RTX and its subexpressions, and renumber all regs into those
   available in leaf functions.  */
extern void leaf_renumber_regs_insn (rtx);

/* Functions in flow.c */
extern void allocate_for_life_analysis	(void);
extern int regno_uninitialized		(int);
extern int regno_clobbered_at_setjmp	(int);
extern void dump_flow_info		(FILE *);
extern void find_basic_blocks         (rtx, int, FILE *);
extern void free_basic_block_vars     (int);
extern void set_block_num             (rtx, int);
extern void life_analysis             (rtx, int, FILE *);
#endif

/* Functions in varasm.c.  */

/* Tell assembler to switch to text section.  */
extern void text_section		(void);

/* Tell assembler to switch to data section.  */
extern void data_section		(void);

/* Tell assembler to make sure its in the data section.  */
extern void force_data_section		(void);

/* Tell assembler to switch to read-only data section.  This is normally
   the text section.  */
extern void readonly_data_section	(void);

/* Determine if we're in the text section. */
extern int in_text_section		(void);

#ifdef TREE_CODE
/* Tell assembler to change to section NAME for DECL.
   If DECL is NULL, just switch to section NAME.
   If NAME is NULL, get the name from DECL.
   If RELOC is 1, the initializer for DECL contains relocs.  */
extern void named_section		(tree, char *, int);

/* Tell assembler to switch to the section for function DECL.  */
extern void function_section		(tree);

/* Tell assembler to switch to the section for the exception table.  */
extern void exception_section		(void);

/* Create the rtl to represent a function, for a function definition.
   DECL is a FUNCTION_DECL node which describes which function.
   The rtl is stored into DECL.  */
extern void make_function_rtl		(tree);

/* Declare DECL to be a weak symbol.  */
extern void declare_weak		(tree);
#endif /* TREE_CODE */

/* Emit any pending weak declarations.  */
extern void weak_finish			(void);

/* Decode an `asm' spec for a declaration as a register name.
   Return the register number, or -1 if nothing specified,
   or -2 if the ASMSPEC is not `cc' or `memory' and is not recognized,
   or -3 if ASMSPEC is `cc' and is not recognized,
   or -4 if ASMSPEC is `memory' and is not recognized.
   Accept an exact spelling or a decimal number.
   Prefixes such as % are optional.  */
extern int decode_reg_name		(char *);

#ifdef TREE_CODE
/* Create the DECL_RTL for a declaration for a static or external variable
   or static or external function.
   ASMSPEC, if not 0, is the string which the user specified
   as the assembler symbol name.
   TOP_LEVEL is nonzero if this is a file-scope variable.

   This is never called for PARM_DECL nodes.  */
extern void make_decl_rtl		(tree, char *, int);

/* Make the rtl for variable VAR be volatile.
   Use this only for static variables.  */
extern void make_var_volatile		(tree);

/* Output alignment directive to align for constant expression EXP.  */
extern void assemble_constant_align	(tree);

extern void assemble_alias		(tree, tree);

/* Output a string of literal assembler code
   for an `asm' keyword used between functions.  */
extern void assemble_asm		(tree);

/* Output assembler code for the constant pool of a function and associated
   with defining the name of the function.  DECL describes the function.
   NAME is the function's name.  For the constant pool, we use the current
   constant pool data.  */
extern void assemble_start_function	(tree, char *);

/* Output assembler code associated with defining the size of the
   function.  DECL describes the function.  NAME is the function's name.  */
extern void assemble_end_function	(tree, char *);

/* Assemble code to leave SIZE bytes of zeros.  */
extern void assemble_zeros		(int);

/* Assemble an alignment pseudo op for an ALIGN-bit boundary.  */
extern void assemble_align		(int);

/* Assemble a string constant with the specified C string as contents.  */
extern void assemble_string		(char *, int);
/* Assemble everything that is needed for a variable or function declaration.
   Not used for automatic variables, and not used for function definitions.
   Should not be called for variables of incomplete structure type.

   TOP_LEVEL is nonzero if this variable has file scope.
   AT_END is nonzero if this is the special handling, at end of compilation,
   to define things that have had only tentative definitions.
   DONT_OUTPUT_DATA if nonzero means don't actually output the
   initial value (that will be done by the caller).  */
extern void assemble_variable		(tree, int, int, int);

/* Output something to declare an external symbol to the assembler.
   (Most assemblers don't need this, so we normally output nothing.)
   Do nothing if DECL is not external.  */
extern void assemble_external		(tree);
#endif /* TREE_CODE */

#ifdef RTX_CODE
/* Similar, for calling a library function FUN.  */
extern void assemble_external_libcall	(rtx);
#endif

/* Declare the label NAME global.  */
extern void assemble_global		(char *);

/* Assemble a label named NAME.  */
extern void assemble_label		(char *);

/* Output to FILE a reference to the assembler name of a C-level name NAME.
   If NAME starts with a *, the rest of NAME is output verbatim.
   Otherwise NAME is transformed in an implementation-defined way
   (usually by the addition of an underscore).
   Many macros in the tm file are defined to call this function.  */
extern void assemble_name		(FILE *, char *);

#ifdef RTX_CODE
/* Assemble the integer constant X into an object of SIZE bytes.
   X must be either a CONST_INT or CONST_DOUBLE.

   Return 1 if we were able to output the constant, otherwise 0.  If FORCE is
   non-zero, abort if we can't output the constant.  */
extern int assemble_integer		(rtx, int, int);

#ifdef EMUSHORT
/* Assemble the floating-point constant D into an object of size MODE.  */
extern void assemble_real		(REAL_VALUE_TYPE,
					       enum machine_mode);
#endif
#endif

/* At the end of a function, forget the memory-constants
   previously made for CONST_DOUBLEs.  Mark them as not on real_constant_chain.
   Also clear out real_constant_chain and clear out all the chain-pointers.  */
extern void clear_const_double_mem	(void);

/* Start deferring output of subconstants.  */
extern void defer_addressed_constants	(void);

/* Stop deferring output of subconstants,
   and output now all those that have been deferred.  */
extern void output_deferred_addressed_constants (void);

/* Initialize constant pool hashing for next function.  */
extern void init_const_rtx_hash_table	(void);

/* Return the size of the constant pool.  */
extern int get_pool_size		(void);

#ifdef TREE_CODE
/* Write all the constants in the constant pool.  */
extern void output_constant_pool	(char *, tree);

/* Output assembler code for constant EXP to FILE, with no label.
   This includes the pseudo-op such as ".int" or ".byte", and a newline.
   Assumes output_addressed_constants has been done on EXP already.

   Generate exactly SIZE bytes of assembler data, padding at the end
   with zeros if necessary.  SIZE must always be specified.  */
extern void output_constant		(tree, int);
#endif

/* When outputting assembler code, indicates which alternative
   of the constraints was actually satisfied.  */
extern int which_alternative;

#ifdef RTX_CODE
/* When outputting delayed branch sequences, this rtx holds the
   sequence being output.  It is null when no delayed branch
   sequence is being output, so it can be used as a test in the
   insn output code.

   This variable is defined  in final.c.  */
extern rtx final_sequence;
#endif

/* CYGNUS LOCAL -- meissner/live range */
/* All the symbol-blocks (levels of scoping) in the compilation
   are assigned sequence numbers in order of appearance of the
   beginnings of the symbol-blocks.  Both final and dbxout do this,
   and assume that they will both give the same number to each block.
   Final uses these sequence numbers to generate assembler label names
   LBBnnn and LBEnnn for the beginning and end of the symbol-block.
   Dbxout uses the sequence numbers to generate references to the same labels
   from the dbx debugging information.

   Sdb records this level at the beginning of each function,
   in order to find the current level when recursing down declarations.
   It outputs the block beginning and endings
   at the point in the asm file where the blocks would begin and end.  */

extern int next_block_index;

#ifdef TREE_CODE
/* Map block # into block nodes during final */
extern tree *block_nodes;
#endif
/* END CYGNUS LOCAL -- meissner/live range */


/* Number of bytes of args popped by function being compiled on its return.
   Zero if no bytes are to be popped.
   May affect compilation of return insn or of function epilogue.  */

extern int current_function_pops_args;

/* Nonzero if function being compiled needs to be given an address
   where the value should be stored.  */

extern int current_function_returns_struct;

/* Nonzero if function being compiled needs to
   return the address of where it has put a structure value.  */

extern int current_function_returns_pcc_struct;

/* Nonzero if function being compiled needs to be passed a static chain.  */

extern int current_function_needs_context;

/* Nonzero if function being compiled can call setjmp.  */

extern int current_function_calls_setjmp;

/* Nonzero if function being compiled can call longjmp.  */

extern int current_function_calls_longjmp;

/* Nonzero if function being compiled can call alloca,
   either as a subroutine or builtin.  */

extern int current_function_calls_alloca;

/* Nonzero if function being compiled receives nonlocal gotos
   from nested functions.  */

extern int current_function_has_nonlocal_label;

/* Nonzero if function being compiled contains nested functions.  */

extern int current_function_contains_functions;

/* Nonzero if function being compiled doesn't modify the stack pointer
   (ignoring the prologue and epilogue).  This is only valid after
   life_analysis has run. */

extern int current_function_sp_is_unchanging;

/* Nonzero if the current function returns a pointer type */

extern int current_function_returns_pointer;

/* If function's args have a fixed size, this is that size, in bytes.
   Otherwise, it is -1.
   May affect compilation of return insn or of function epilogue.  */

extern int current_function_args_size;

/* # bytes the prologue should push and pretend that the caller pushed them.
   The prologue must do this, but only if parms can be passed in registers.  */

extern int current_function_pretend_args_size;

/* # of bytes of outgoing arguments required to be pushed by the prologue.
   If this is non-zero, it means that ACCUMULATE_OUTGOING_ARGS was defined
   and no stack adjusts will be done on function calls.  */

extern int current_function_outgoing_args_size;

/* Nonzero if current function uses varargs.h or equivalent.
   Zero for functions that use stdarg.h.  */

extern int current_function_varargs;

/* Nonzero if current function uses stdarg.h or equivalent.
   Zero for functions that use varargs.h.  */

extern int current_function_stdarg;

/* Quantities of various kinds of registers
   used for the current function's args.  */

extern CUMULATIVE_ARGS current_function_args_info;

/* Name of function now being compiled.  */

extern char *current_function_name;

#ifdef RTX_CODE
/* If non-zero, an RTL expression for that location at which the current
   function returns its result.  Usually equal to
   DECL_RTL (DECL_RESULT (current_function_decl)).  */

extern rtx current_function_return_rtx;
#endif

/* This is nonzero if the current function uses the constant pool.  */
extern int current_function_uses_const_pool;

/* Language-specific reason why the current function cannot be made inline.  */
extern char *current_function_cannot_inline;

/* File in which assembler code is being written.  */

#ifdef BUFSIZ
extern FILE *asm_out_file;
#endif

/* Default file in which to dump debug output.  */

#ifdef BUFSIZ
extern FILE *rtl_dump_file;
#endif

/* Decide whether DECL needs to be in a writable section.  RELOC is the same
   as for SELECT_SECTION.  */

#define DECL_READONLY_SECTION(DECL,RELOC)		\
  (TREE_READONLY (DECL)					\
   && ! TREE_THIS_VOLATILE (DECL)			\
   && DECL_INITIAL (DECL)				\
   && (DECL_INITIAL (DECL) == error_mark_node		\
       || TREE_CONSTANT (DECL_INITIAL (DECL)))		\
   && ! (RELOC && (DECL_ONE_ONLY (DECL))))
