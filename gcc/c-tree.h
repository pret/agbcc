/* Definitions for C parsing and type checking.
   Copyright (C) 1987, 1993, 1994, 1995, 1997, 1998 Free Software Foundation, Inc.

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

#ifndef _C_TREE_H
#define _C_TREE_H

/* Language-dependent contents of an identifier.  */

/* The limbo_value is used for block level extern declarations, which need
   to be type checked against subsequent extern declarations.  They can't
   be referenced after they fall out of scope, so they can't be global.  */

struct lang_identifier
{
  struct tree_identifier ignore;
  tree global_value, local_value, label_value, implicit_decl;
  tree error_locus, limbo_value;
};

/* Macros for access to language-specific slots in an identifier.  */
/* Each of these slots contains a DECL node or null.  */

/* This represents the value which the identifier has in the
   file-scope namespace.  */
#define IDENTIFIER_GLOBAL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->global_value)
/* This represents the value which the identifier has in the current
   scope.  */
#define IDENTIFIER_LOCAL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->local_value)
/* This represents the value which the identifier has as a label in
   the current label scope.  */
#define IDENTIFIER_LABEL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->label_value)
/* This records the extern decl of this identifier, if it has had one
   at any point in this compilation.  */
#define IDENTIFIER_LIMBO_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->limbo_value)
/* This records the implicit function decl of this identifier, if it
   has had one at any point in this compilation.  */
#define IDENTIFIER_IMPLICIT_DECL(NODE)	\
  (((struct lang_identifier *) (NODE))->implicit_decl)
/* This is the last function in which we printed an "undefined variable"
   message for this identifier.  Value is a FUNCTION_DECL or null.  */
#define IDENTIFIER_ERROR_LOCUS(NODE)	\
  (((struct lang_identifier *) (NODE))->error_locus)

/* In identifiers, C uses the following fields in a special way:
   TREE_PUBLIC        to record that there was a previous local extern decl.
   TREE_USED          to record that such a decl was used.
   TREE_ADDRESSABLE   to record that the address of such a decl was used.  */

/* Nonzero means reject anything that ANSI standard C forbids.  */
extern int pedantic;

/* In a RECORD_TYPE or UNION_TYPE, nonzero if any component is read-only.  */
#define C_TYPE_FIELDS_READONLY(type) TREE_LANG_FLAG_1 (type)

/* In a RECORD_TYPE or UNION_TYPE, nonzero if any component is volatile.  */
#define C_TYPE_FIELDS_VOLATILE(type) TREE_LANG_FLAG_2 (type)

/* In a RECORD_TYPE or UNION_TYPE or ENUMERAL_TYPE
   nonzero if the definition of the type has already started.  */
#define C_TYPE_BEING_DEFINED(type) TYPE_LANG_FLAG_0 (type)

/* C types are partitioned into three subsets: object, function, and
   incomplete types.  */
#define C_TYPE_OBJECT_P(type) \
  (TREE_CODE (type) != FUNCTION_TYPE && TYPE_SIZE (type))

#define C_TYPE_FUNCTION_P(type) \
  (TREE_CODE (type) == FUNCTION_TYPE)

#define C_TYPE_INCOMPLETE_P(type) \
  (TREE_CODE (type) != FUNCTION_TYPE && TYPE_SIZE (type) == 0)

/* For convenience we define a single macro to identify the class of
   object or incomplete types.  */
#define C_TYPE_OBJECT_OR_INCOMPLETE_P(type) \
  (!C_TYPE_FUNCTION_P (type))

/* In a RECORD_TYPE, a sorted array of the fields of the type.  */
struct lang_type
{
  int len;
  tree elts[1];
};

/* Mark which labels are explicitly declared.
   These may be shadowed, and may be referenced from nested functions.  */
#define C_DECLARED_LABEL_FLAG(label) TREE_LANG_FLAG_1 (label)

/* Record whether a type or decl was written with nonconstant size.
   Note that TYPE_SIZE may have simplified to a constant.  */
#define C_TYPE_VARIABLE_SIZE(type) TYPE_LANG_FLAG_1 (type)
#define C_DECL_VARIABLE_SIZE(type) DECL_LANG_FLAG_0 (type)

/* Record in each node resulting from a binary operator
   what operator was specified for it.  */
#define C_EXP_ORIGINAL_CODE(exp) ((enum tree_code) TREE_COMPLEXITY (exp))

#if 0 /* Not used.  */
/* Record whether a decl for a function or function pointer has
   already been mentioned (in a warning) because it was called
   but didn't have a prototype.  */
#define C_MISSING_PROTOTYPE_WARNED(decl) DECL_LANG_FLAG_2(decl)
#endif

/* Store a value in that field.  */
#define C_SET_EXP_ORIGINAL_CODE(exp, code) \
  (TREE_COMPLEXITY (exp) = (int) (code))

/* Record whether a typedef for type `int' was actually `signed int'.  */
#define C_TYPEDEF_EXPLICITLY_SIGNED(exp) DECL_LANG_FLAG_1 ((exp))

/* Nonzero for a declaration of a built in function if there has been no
   occasion that would declare the function in ordinary C.
   Using the function draws a pedantic warning in this case.  */
#define C_DECL_ANTICIPATED(exp) DECL_LANG_FLAG_3 ((exp))

/* For FUNCTION_TYPE, a hidden list of types of arguments.  The same as
   TYPE_ARG_TYPES for functions with prototypes, but created for functions
   without prototypes.  */
#define TYPE_ACTUAL_ARG_TYPES(NODE) TYPE_NONCOPIED_PARTS (NODE)

/* In a FIELD_DECL, nonzero if the decl was originally a bitfield.  */
#define DECL_C_BIT_FIELD(NODE) DECL_LANG_FLAG_4 (NODE)

/* Nonzero if the type T promotes to itself.
   ANSI C states explicitly the list of types that promote;
   in particular, short promotes to int even if they have the same width.  */
#define C_PROMOTING_INTEGER_TYPE_P(t)				\
  (TREE_CODE ((t)) == INTEGER_TYPE				\
   && (TYPE_MAIN_VARIANT (t) == char_type_node			\
       || TYPE_MAIN_VARIANT (t) == signed_char_type_node	\
       || TYPE_MAIN_VARIANT (t) == unsigned_char_type_node	\
       || TYPE_MAIN_VARIANT (t) == short_integer_type_node	\
       || TYPE_MAIN_VARIANT (t) == short_unsigned_type_node))

/* In a VAR_DECL, means the variable is really an iterator.  */
#define ITERATOR_P(D) (DECL_LANG_FLAG_4(D))

/* In a VAR_DECL for an iterator, means we are within
   an explicit loop over that iterator.  */
#define ITERATOR_BOUND_P(NODE) ((NODE)->common.readonly_flag)

/* in c-aux-info.c */
extern void gen_aux_info_record                 (tree, int, int, int);

/* in c-common.c */
extern void declare_function_name               (void);
extern void decl_attributes                     (tree, tree, tree);
extern int c_get_alias_set                      (tree);
extern void c_apply_type_quals_to_decl          (int, tree);
/* Print an error message for invalid operands to arith operation CODE.
   NOP_EXPR is used as a special case (see truthvalue_conversion).  */
extern void binary_op_error                     (enum tree_code);
extern void c_expand_expr_stmt                  (tree);
extern void c_expand_start_cond                 (tree, int, int);
extern void c_expand_start_else                 (void);
extern void c_expand_end_cond                   (void);
/* Validate the expression after `case' and apply default promotions.  */
extern tree check_case_value                    (tree);
/* Concatenate a list of STRING_CST nodes into one STRING_CST.  */
extern tree combine_strings                     (tree);
extern void constant_expression_warning         (tree);
extern tree convert_and_check			(tree, tree);
extern void overflow_warning			(tree);
extern void unsigned_conversion_warning		(tree, tree);
/* Read the rest of the current #-directive line.  */
extern char *get_directive_line                 (FILE *);
#define GET_DIRECTIVE_LINE() get_directive_line(finput)

/* Subroutine of build_binary_op, used for comparison operations.
   See if the operands have both been converted from subword integer types
   and, if so, perhaps change them both back to their original type.  */
extern tree shorten_compare                     (tree *, tree *, tree *, enum tree_code *);
/* Prepare expr to be an argument of a TRUTH_NOT_EXPR,
   or validate its data type for an `if' or `while' statement or ?..: exp. */
extern tree truthvalue_conversion               (tree);
extern tree type_for_mode                       (enum machine_mode, int);
extern tree type_for_size                       (unsigned, int);

/* in c-convert.c */
extern tree convert                             (tree, tree);

/* in c-decl.c */
/* Standard named or nameless data types of the C compiler.  */
extern tree char_array_type_node;
extern tree char_type_node;
extern tree const_ptr_type_node;
extern tree const_string_type_node;
extern tree default_function_type;
extern tree double_ftype_double;
extern tree double_ftype_double_double;
extern tree double_type_node;
extern tree float_type_node;
#if HOST_BITS_PER_WIDE_INT >= 64
extern tree intTI_type_node;
#endif
extern tree intDI_type_node;
extern tree intHI_type_node;
extern tree intQI_type_node;
extern tree intSI_type_node;
extern tree int_array_type_node;
extern tree int_ftype_cptr_cptr_sizet;
extern tree int_ftype_int;
extern tree int_ftype_ptr_ptr_int;
extern tree int_ftype_string_string;
extern tree integer_type_node;
extern tree long_double_type_node;
extern tree long_ftype_long;
extern tree long_integer_type_node;
extern tree long_long_integer_type_node;
extern tree long_long_unsigned_type_node;
extern tree long_unsigned_type_node;
extern tree complex_integer_type_node;
extern tree complex_float_type_node;
extern tree complex_double_type_node;
extern tree complex_long_double_type_node;
extern tree ptr_type_node;
extern tree ptrdiff_type_node;
extern tree short_integer_type_node;
extern tree short_unsigned_type_node;
extern tree signed_char_type_node;
extern tree signed_wchar_type_node;
extern tree string_ftype_ptr_ptr;
extern tree string_type_node;
extern tree unsigned_char_type_node;
#if HOST_BITS_PER_WIDE_INT >= 64
extern tree unsigned_intTI_type_node;
#endif
extern tree unsigned_intDI_type_node;
extern tree unsigned_intHI_type_node;
extern tree unsigned_intQI_type_node;
extern tree unsigned_intSI_type_node;
extern tree unsigned_type_node;
extern tree unsigned_wchar_type_node;
extern tree void_ftype_ptr_int_int;
extern tree void_ftype_ptr_ptr_int;
extern tree void_type_node;
extern tree wchar_array_type_node;
extern tree wchar_type_node;
extern tree boolean_type_node;
extern tree boolean_true_node;
extern tree boolean_false_node;

extern tree build_enumerator                    (tree, tree);
/* Declare a predefined function.  Return the declaration.  */
extern tree builtin_function                    (char *, tree, enum built_in_function function_, char *);
/* Add qualifiers to a type, in the fashion for C.  */
extern tree c_build_qualified_type              (tree, int);
#define c_build_type_variant(TYPE, CONST_P, VOLATILE_P)		  \
  c_build_qualified_type (TYPE, 				  \
			  ((CONST_P) ? TYPE_QUAL_CONST : 0) |	  \
			  ((VOLATILE_P) ? TYPE_QUAL_VOLATILE : 0))
extern void c_decode_option                     (char *);
extern void c_mark_varargs                      (void);
extern tree check_identifier                    (tree, tree);
extern void clear_parm_order                    (void);
extern tree combine_parm_decls                  (tree, tree, int);
extern int  complete_array_type                 (tree, tree, int);
extern void declare_parm_level                  (int);
extern tree define_label                        (char *, int, tree);
extern void delete_block                        (tree);
extern void finish_decl                         (tree, tree, tree);
extern void finish_decl_top_level               (tree, tree, tree);
extern tree finish_enum                         (tree, tree, tree);
extern void finish_function                     (int);
extern tree finish_struct                       (tree, tree, tree);
extern tree get_parm_info                       (int);
extern tree getdecls                            (void);
extern tree gettags                             (void);
extern int  global_bindings_p                   (void);
extern tree grokfield                           (char *, int, tree, tree, tree);
extern tree groktypename                        (tree);
extern tree groktypename_in_parm_context        (tree);
extern tree implicitly_declare                  (tree);
extern int  in_parm_level_p                     (void);
extern void init_decl_processing                (void);
extern void insert_block                        (tree);
extern void keep_next_level                     (void);
extern int  kept_level_p                        (void);
extern tree lookup_label                        (tree);
extern tree lookup_name                         (tree);
extern tree lookup_name_current_level		(tree);
extern tree lookup_name_current_level_global	(tree);
extern tree maybe_build_cleanup                 (tree);
extern void parmlist_tags_warning               (void);
extern void pending_xref_error                  (void);
extern void pop_c_function_context              (void);
extern void pop_label_level                     (void);
extern tree poplevel                            (int, int, int);
extern void print_lang_decl                     (FILE *, tree, int);
extern void print_lang_identifier               (FILE *, tree, int);
extern void print_lang_type                     (FILE *, tree, int);
extern void push_c_function_context             (void);
extern void push_label_level                    (void);
extern void push_parm_decl                      (tree);
extern tree pushdecl                            (tree);
extern tree pushdecl_top_level                  (tree);
extern void pushlevel                           (int);
extern void pushtag                             (tree, tree);
extern void set_block                           (tree);
extern tree shadow_label                        (tree);
extern void shadow_record_fields                (tree);
extern void shadow_tag                          (tree);
extern void shadow_tag_warned                   (tree, int);
extern tree start_enum                          (tree);
extern int  start_function                      (tree, tree, tree,
						       tree, int);
extern tree start_decl                          (tree, tree, int,
						       tree, tree);
extern tree start_struct                        (enum tree_code, tree);
extern void store_parm_decls                    (void);
extern tree xref_tag                            (enum tree_code, tree);

/* in c-typeck.c */
extern tree require_complete_type		(tree);
extern void incomplete_type_error		(tree, tree);
/* Given two integer or real types, return the type for their sum.
   Given two compatible ANSI C types, returns the merged type.  */
extern tree common_type                         (tree, tree);
extern int comptypes				(tree, tree);
extern int self_promoting_args_p		(tree);
extern tree c_sizeof                            (tree);
extern tree c_sizeof_nowarn                     (tree);
extern tree c_size_in_bytes                     (tree);
extern tree c_alignof				(tree);
extern tree c_alignof_expr			(tree);
extern tree default_conversion                  (tree);
extern tree build_component_ref                 (tree, tree);
extern tree build_indirect_ref                  (tree, char *);
extern tree build_array_ref                     (tree, tree);
extern tree build_function_call                 (tree, tree);
extern tree parser_build_binary_op              (enum tree_code,
						       tree, tree);
extern tree build_binary_op                     (enum tree_code,
						       tree, tree, int);
extern tree build_unary_op                      (enum tree_code,
						       tree, int);
extern int lvalue_p				(tree);
extern int lvalue_or_else			(tree, char *);
extern void readonly_warning			(tree, char *);
extern int mark_addressable			(tree);
extern tree build_conditional_expr              (tree, tree, tree);
extern tree build_compound_expr                 (tree);
extern tree build_c_cast                        (tree, tree);
extern tree build_modify_expr                   (tree, enum tree_code,
						       tree);
extern tree initializer_constant_valid_p	(tree, tree);
extern void store_init_value                    (tree, tree);
extern void error_init				(char *, char *,
						       char *);
extern void pedwarn_init			(char *, char *,
						       char *);
extern void start_init				(tree, tree, int);
extern void finish_init				(void);
extern void really_start_incremental_init	(tree);
extern void push_init_level			(int);
extern tree pop_init_level			(int);
extern void set_init_index			(tree, tree);
extern void set_init_label			(tree);
extern void process_init_element		(tree);
extern void c_expand_asm_operands		(tree, tree, tree, tree,
						       int, char *, int);
extern void c_expand_return			(tree);
extern tree c_expand_start_case                 (tree);

/* in c-iterate.c */
extern void init_iterators			(void);
extern void iterator_expand			(tree);
extern void iterator_for_loop_start		(tree);
extern void iterator_for_loop_end		(tree);
extern void iterator_for_loop_record		(tree);
extern void push_iterator_stack			(void);
extern void pop_iterator_stack			(void);

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement that specifies a return value is seen.  */

extern int current_function_returns_value;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement with no argument is seen.  */

extern int current_function_returns_null;

/* Nonzero means the expression being parsed will never be evaluated.
   This is a count, since unevaluated expressions can nest.  */

extern int skip_evaluation;

/* Nonzero means allow type mismatches in conditional expressions;
   just make their values `void'.   */

extern int flag_cond_mismatch;

/* Nonzero means don't recognize the keyword `asm'.  */

extern int flag_no_asm;

/* Nonzero means environment is hosted (i.e., not freestanding) */

extern int flag_hosted;

/* Nonzero means ignore `#ident' directives.  */

extern int flag_no_ident;

/* Nonzero means warn about implicit declarations.  */

extern int warn_implicit;

/* Nonzero means give string constants the type `const char *'
   to get extra warnings from them.  These warnings will be too numerous
   to be useful, except in thoroughly ANSIfied programs.  */

extern int flag_const_strings;

/* Nonzero means warn about sizeof (function) or addition/subtraction
   of function pointers.  */

extern int warn_pointer_arith;

/* Nonzero means warn for all old-style non-prototype function decls.  */

extern int warn_strict_prototypes;

/* Nonzero means warn about multiple (redundant) decls for the same single
   variable or function.  */

extern int warn_redundant_decls;

/* Nonzero means warn about extern declarations of objects not at
   file-scope level and about *all* declarations of functions (whether
   extern or static) not at file-scope level.  Note that we exclude
   implicit function declarations.  To get warnings about those, use
   -Wimplicit.  */

extern int warn_nested_externs;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   from the pointer target type.  */

extern int warn_cast_qual;

/* Nonzero means warn when casting a function call to a type that does
   not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   when there is no previous declaration of sqrt or malloc.  */

extern int warn_bad_function_cast;

/* Warn about functions which might be candidates for attribute noreturn. */

extern int warn_missing_noreturn;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */

extern int warn_traditional;

/* Warn about a subscript that has type char.  */

extern int warn_char_subscripts;

/* Warn if a type conversion is done that might have confusing results.  */

extern int warn_conversion;

/* Warn if main is suspicious. */

extern int warn_main;

/* Nonzero means do some things the same way PCC does.  */

extern int flag_traditional;

/* Nonzero means use the ISO C9x dialect of C.  */

extern int flag_isoc9x;

/* Nonzero means to allow single precision math even if we're generally
   being traditional. */
extern int flag_allow_single_precision;

/* Nonzero means warn about suggesting putting in ()'s.  */

extern int warn_parentheses;

/* Warn if initializer is not completely bracketed.  */

extern int warn_missing_braces;

/* Warn about comparison of signed and unsigned values.  */

extern int warn_sign_compare;

/* Warn about multicharacter constants.  */

extern int warn_multichar;

/* Warn about long long.  */

extern int warn_long_long;

/* In c-decl.c */
extern void finish_incomplete_decl (tree);

#endif /* not _C_TREE_H */
