/* Subroutines shared by all languages that are variants of C.
   Copyright (C) 1992, 93-98, 1999 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "tree.h"
#include "c-lex.h"
#include "c-tree.h"
#include "flags.h"
#include "obstack.h"
#include "toplev.h"
#include "output.h"
#include "rtl.h"

#ifndef WCHAR_TYPE_SIZE
#ifdef INT_TYPE_SIZE
#define WCHAR_TYPE_SIZE INT_TYPE_SIZE
#else
#define WCHAR_TYPE_SIZE	BITS_PER_WORD
#endif
#endif

extern struct obstack permanent_obstack;

/* Nonzero means the expression being parsed will never be evaluated.
   This is a count, since unevaluated expressions can nest.  */
int skip_evaluation;

enum attrs {A_PACKED, A_NOCOMMON, A_COMMON, A_NORETURN, A_CONST, A_T_UNION,
	    A_NO_CHECK_MEMORY_USAGE, A_NO_INSTRUMENT_FUNCTION,
	    A_MODE, A_SECTION, A_ALIGNED,
	    A_UNUSED, A_WEAK, A_ALIAS};

static void declare_hidden_char_array	(char *, char *);
static void add_attribute		(enum attrs, char *,
					       int, int, int);
static void init_attributes		(void);
static tree c_find_base_decl            (tree);

/* Keep a stack of if statements.  We record the number of compound
   statements seen up to the if keyword, as well as the line number
   and file of the if.  If a potentially ambiguous else is seen, that
   fact is recorded; the warning is issued when we can be sure that
   the enclosing if statement does not have an else branch.  */
typedef struct
{
  int compstmt_count;
  int line;
  char *file;
  int needs_warning;
} if_elt;

static if_elt *if_stack;

/* Amount of space in the if statement stack.  */
static int if_stack_space = 0;

/* Stack pointer.  */
static int if_stack_pointer = 0;

/* Generate RTL for the start of an if-then, and record the start of it
   for ambiguous else detection.  */

void
c_expand_start_cond (cond, exitflag, compstmt_count)
     tree cond;
     int exitflag;
     int compstmt_count;
{
  /* Make sure there is enough space on the stack.  */
  if (if_stack_space == 0)
    {
      if_stack_space = 10;
      if_stack = (if_elt *)xmalloc (10 * sizeof (if_elt));
    }
  else if (if_stack_space == if_stack_pointer)
    {
      if_stack_space += 10;
      if_stack = (if_elt *)xrealloc (if_stack, if_stack_space * sizeof (if_elt));
    }

  /* Record this if statement.  */
  if_stack[if_stack_pointer].compstmt_count = compstmt_count;
  if_stack[if_stack_pointer].file = input_filename;
  if_stack[if_stack_pointer].line = lineno;
  if_stack[if_stack_pointer].needs_warning = 0;
  if_stack_pointer++;

  expand_start_cond (cond, exitflag);
}

/* Generate RTL for the end of an if-then.  Optionally warn if a nested
   if statement had an ambiguous else clause.  */

void
c_expand_end_cond ()
{
  if_stack_pointer--;
  if (if_stack[if_stack_pointer].needs_warning)
    warning_with_file_and_line (if_stack[if_stack_pointer].file,
				if_stack[if_stack_pointer].line,
				"suggest explicit braces to avoid ambiguous `else'");
  expand_end_cond ();
}

/* Generate RTL between the then-clause and the else-clause
   of an if-then-else.  */

void
c_expand_start_else ()
{
  /* An ambiguous else warning must be generated for the enclosing if
     statement, unless we see an else branch for that one, too.  */
  if (warn_parentheses
      && if_stack_pointer > 1
      && (if_stack[if_stack_pointer - 1].compstmt_count
	  == if_stack[if_stack_pointer - 2].compstmt_count))
    if_stack[if_stack_pointer - 2].needs_warning = 1;

  /* Even if a nested if statement had an else branch, it can't be
     ambiguous if this one also has an else.  So don't warn in that
     case.  Also don't warn for any if statements nested in this else.  */
  if_stack[if_stack_pointer - 1].needs_warning = 0;
  if_stack[if_stack_pointer - 1].compstmt_count--;

  expand_start_else ();
}

/* Make bindings for __FUNCTION__, __PRETTY_FUNCTION__, and __func__.  */

void
declare_function_name ()
{
  char *name, *printable_name;

  if (current_function_decl == NULL)
    {
      name = "";
      printable_name = "top level";
    }
  else
    {
      /* Allow functions to be nameless (such as artificial ones).  */
      if (DECL_NAME (current_function_decl))
        name = IDENTIFIER_POINTER (DECL_NAME (current_function_decl));
      else
	name = "";
      printable_name = (*decl_printable_name) (current_function_decl, 2);
    }

  declare_hidden_char_array ("__FUNCTION__", name);
  declare_hidden_char_array ("__PRETTY_FUNCTION__", printable_name);
  /* The ISO C people "of course" couldn't use __FUNCTION__ in the
     ISO C 9x standard; instead a new variable is invented.  */
  declare_hidden_char_array ("__func__", name);
}

static void
declare_hidden_char_array (name, value)
     char *name, *value;
{
  tree decl, type, init;
  int vlen;

  /* If the default size of char arrays isn't big enough for the name,
     or if we want to give warnings for large objects, make a bigger one.  */
  vlen = strlen (value) + 1;
  type = char_array_type_node;
  if (TREE_INT_CST_LOW (TYPE_MAX_VALUE (TYPE_DOMAIN (type))) < vlen
      || warn_larger_than)
    type = build_array_type (char_type_node,
			     build_index_type (build_int_2 (vlen, 0)));
  push_obstacks_nochange ();
  decl = build_decl (VAR_DECL, get_identifier (name), type);
  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_ASM_WRITTEN (decl) = 1;
  DECL_SOURCE_LINE (decl) = 0;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IN_SYSTEM_HEADER (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  init = build_string (vlen, value);
  TREE_TYPE (init) = type;
  DECL_INITIAL (decl) = init;
  finish_decl (pushdecl (decl), init, NULL_TREE);
}

/* Given a chain of STRING_CST nodes,
   concatenate them into one STRING_CST
   and give it a suitable array-of-chars data type.  */

tree
combine_strings (strings)
     tree strings;
{
  register tree value, t;
  register int length = 1;
  int wide_length = 0;
  int wide_flag = 0;
  int wchar_bytes = TYPE_PRECISION (wchar_type_node) / BITS_PER_UNIT;
  int nchars;

  if (TREE_CHAIN (strings))
    {
      /* More than one in the chain, so concatenate.  */
      register char *p, *q;

      /* Don't include the \0 at the end of each substring,
	 except for the last one.
	 Count wide strings and ordinary strings separately.  */
      for (t = strings; t; t = TREE_CHAIN (t))
	{
	  if (TREE_TYPE (t) == wchar_array_type_node)
	    {
	      wide_length += (TREE_STRING_LENGTH (t) - wchar_bytes);
	      wide_flag = 1;
	    }
	  else
	    length += (TREE_STRING_LENGTH (t) - 1);
	}

      /* If anything is wide, the non-wides will be converted,
	 which makes them take more space.  */
      if (wide_flag)
	length = length * wchar_bytes + wide_length;

      p = savealloc (length);

      /* Copy the individual strings into the new combined string.
	 If the combined string is wide, convert the chars to ints
	 for any individual strings that are not wide.  */

      q = p;
      for (t = strings; t; t = TREE_CHAIN (t))
	{
	  int len = (TREE_STRING_LENGTH (t)
		     - ((TREE_TYPE (t) == wchar_array_type_node)
			? wchar_bytes : 1));
	  if ((TREE_TYPE (t) == wchar_array_type_node) == wide_flag)
	    {
	      memcpy (q, TREE_STRING_POINTER (t), len);
	      q += len;
	    }
	  else
	    {
	      int i;
	      for (i = 0; i < len; i++)
		{
		  ((int *) q)[i] = TREE_STRING_POINTER (t)[i];
		}
	      q += len * wchar_bytes;
	    }
	}
      if (wide_flag)
	{
	  int i;
	  for (i = 0; i < wchar_bytes; i++)
	    *q++ = 0;
	}
      else
	*q = 0;

      value = make_node (STRING_CST);
      TREE_STRING_POINTER (value) = p;
      TREE_STRING_LENGTH (value) = length;
    }
  else
    {
      value = strings;
      length = TREE_STRING_LENGTH (value);
      if (TREE_TYPE (value) == wchar_array_type_node)
	wide_flag = 1;
    }

  /* Compute the number of elements, for the array type.  */
  nchars = wide_flag ? length / wchar_bytes : length;

  /* Create the array type for the string constant.
     -Wwrite-strings says make the string constant an array of const char
     so that copying it to a non-const pointer will get a warning.
     For C++, this is the standard behavior.  */
  if (flag_const_strings
      && (! flag_traditional  && ! flag_writable_strings))
    {
      tree elements
	= build_type_variant (wide_flag ? wchar_type_node : char_type_node,
			      1, 0);
      TREE_TYPE (value)
	= build_array_type (elements,
			    build_index_type (build_int_2 (nchars - 1, 0)));
    }
  else
    TREE_TYPE (value)
      = build_array_type (wide_flag ? wchar_type_node : char_type_node,
			  build_index_type (build_int_2 (nchars - 1, 0)));

  TREE_READONLY (value) = TREE_CONSTANT (value) = ! flag_writable_strings;
  TREE_STATIC (value) = 1;
  return value;
}

/* To speed up processing of attributes, we maintain an array of
   IDENTIFIER_NODES and the corresponding attribute types.  */

/* Array to hold attribute information.  */

static struct {enum attrs id; tree name; int min, max, decl_req;} attrtab[50];

static int attrtab_idx = 0;

/* Add an entry to the attribute table above.  */

static void
add_attribute (id, string, min_len, max_len, decl_req)
     enum attrs id;
     char *string;
     int min_len, max_len;
     int decl_req;
{
  char buf[100];

  attrtab[attrtab_idx].id = id;
  attrtab[attrtab_idx].name = get_identifier (string);
  attrtab[attrtab_idx].min = min_len;
  attrtab[attrtab_idx].max = max_len;
  attrtab[attrtab_idx++].decl_req = decl_req;

  sprintf (buf, "__%s__", string);

  attrtab[attrtab_idx].id = id;
  attrtab[attrtab_idx].name = get_identifier (buf);
  attrtab[attrtab_idx].min = min_len;
  attrtab[attrtab_idx].max = max_len;
  attrtab[attrtab_idx++].decl_req = decl_req;
}

/* Initialize attribute table.  */

static void
init_attributes ()
{
  add_attribute (A_PACKED, "packed", 0, 0, 0);
  add_attribute (A_NOCOMMON, "nocommon", 0, 0, 1);
  add_attribute (A_COMMON, "common", 0, 0, 1);
  add_attribute (A_NORETURN, "noreturn", 0, 0, 1);
  add_attribute (A_NORETURN, "volatile", 0, 0, 1);
  add_attribute (A_UNUSED, "unused", 0, 0, 0);
  add_attribute (A_CONST, "const", 0, 0, 1);
  add_attribute (A_T_UNION, "transparent_union", 0, 0, 0);
  add_attribute (A_MODE, "mode", 1, 1, 1);
  add_attribute (A_SECTION, "section", 1, 1, 1);
  add_attribute (A_ALIGNED, "aligned", 0, 1, 0);
  add_attribute (A_WEAK, "weak", 0, 0, 1);
  add_attribute (A_ALIAS, "alias", 1, 1, 1);
  add_attribute (A_NO_INSTRUMENT_FUNCTION, "no_instrument_function", 0, 0, 1);
  add_attribute (A_NO_CHECK_MEMORY_USAGE, "no_check_memory_usage", 0, 0, 1);
}

/* Process the attributes listed in ATTRIBUTES and PREFIX_ATTRIBUTES
   and install them in NODE, which is either a DECL (including a TYPE_DECL)
   or a TYPE.  PREFIX_ATTRIBUTES can appear after the declaration specifiers
   and declaration modifiers but before the declaration proper.  */

void
decl_attributes (node, attributes, prefix_attributes)
     tree node, attributes, prefix_attributes;
{
  tree decl = 0, type = 0;
  int is_type = 0;
  tree a;

  if (attrtab_idx == 0)
    init_attributes ();

  if (TREE_CODE_CLASS (TREE_CODE (node)) == 'd')
    {
      decl = node;
      type = TREE_TYPE (decl);
      is_type = TREE_CODE (node) == TYPE_DECL;
    }
  else if (TREE_CODE_CLASS (TREE_CODE (node)) == 't')
    type = node, is_type = 1;
  
#ifdef INSERT_ATTRIBUTES
  INSERT_ATTRIBUTES (node, & attributes, & prefix_attributes);
#endif
  
  attributes = chainon (prefix_attributes, attributes);

  for (a = attributes; a; a = TREE_CHAIN (a))
    {
      tree name = TREE_PURPOSE (a);
      tree args = TREE_VALUE (a);
      int i;
      enum attrs id;

      for (i = 0; i < attrtab_idx; i++)
	if (attrtab[i].name == name)
	  break;

      if (i == attrtab_idx)
	{
	  if (! valid_machine_attribute (name, args, decl, type))
	    warning ("`%s' attribute directive ignored",
		     IDENTIFIER_POINTER (name));
	  else if (decl != 0)
	    type = TREE_TYPE (decl);
	  continue;
	}
      else if (attrtab[i].decl_req && decl == 0)
	{
	  warning ("`%s' attribute does not apply to types",
		   IDENTIFIER_POINTER (name));
	  continue;
	}
      else if (list_length (args) < attrtab[i].min
	       || list_length (args) > attrtab[i].max)
	{
	  error ("wrong number of arguments specified for `%s' attribute",
		 IDENTIFIER_POINTER (name));
	  continue;
	}

      id = attrtab[i].id;
      switch (id)
	{
	case A_PACKED:
	  if (is_type)
	    TYPE_PACKED (type) = 1;
	  else if (TREE_CODE (decl) == FIELD_DECL)
	    DECL_PACKED (decl) = 1;
	  /* We can't set DECL_PACKED for a VAR_DECL, because the bit is
	     used for DECL_REGISTER.  It wouldn't mean anything anyway.  */
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_NOCOMMON:
	  if (TREE_CODE (decl) == VAR_DECL)
	    DECL_COMMON (decl) = 0;
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_COMMON:
	  if (TREE_CODE (decl) == VAR_DECL)
	    DECL_COMMON (decl) = 1;
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_NORETURN:
	  if (TREE_CODE (decl) == FUNCTION_DECL)
	    TREE_THIS_VOLATILE (decl) = 1;
	  else if (TREE_CODE (type) == POINTER_TYPE
		   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
	    TREE_TYPE (decl) = type
	      = build_pointer_type
		(build_type_variant (TREE_TYPE (type),
				     TREE_READONLY (TREE_TYPE (type)), 1));
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_UNUSED:
	  if (is_type)
	    TREE_USED (type) = 1;
	  else if (TREE_CODE (decl) == PARM_DECL
		   || TREE_CODE (decl) == VAR_DECL
		   || TREE_CODE (decl) == FUNCTION_DECL
		   || TREE_CODE (decl) == LABEL_DECL)
	    TREE_USED (decl) = 1;
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_CONST:
	  if (TREE_CODE (decl) == FUNCTION_DECL)
	    TREE_READONLY (decl) = 1;
	  else if (TREE_CODE (type) == POINTER_TYPE
		   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
	    TREE_TYPE (decl) = type
	      = build_pointer_type
		(build_type_variant (TREE_TYPE (type), 1,
				     TREE_THIS_VOLATILE (TREE_TYPE (type))));
	  else
	    warning ( "`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_T_UNION:
	  if (is_type
	      && TREE_CODE (type) == UNION_TYPE
	      && (decl == 0
		  || (TYPE_FIELDS (type) != 0
		      && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))))
	    TYPE_TRANSPARENT_UNION (type) = 1;
	  else if (decl != 0 && TREE_CODE (decl) == PARM_DECL
		   && TREE_CODE (type) == UNION_TYPE
		   && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))
	    DECL_TRANSPARENT_UNION (decl) = 1;
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_MODE:
	  if (TREE_CODE (TREE_VALUE (args)) != IDENTIFIER_NODE)
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  else
	    {
	      int j;
	      char *p = IDENTIFIER_POINTER (TREE_VALUE (args));
	      int len = strlen (p);
	      enum machine_mode mode = VOIDmode;
	      tree typefm;

	      if (len > 4 && p[0] == '_' && p[1] == '_'
		  && p[len - 1] == '_' && p[len - 2] == '_')
		{
		  char *newp = (char *) alloca (len - 1);

		  strcpy (newp, &p[2]);
		  newp[len - 4] = '\0';
		  p = newp;
		}

	      /* Give this decl a type with the specified mode.
		 First check for the special modes.  */
	      if (! strcmp (p, "byte"))
		mode = byte_mode;
	      else if (!strcmp (p, "word"))
		mode = word_mode;
	      else if (! strcmp (p, "pointer"))
		mode = ptr_mode;
	      else
		for (j = 0; j < NUM_MACHINE_MODES; j++)
		  if (!strcmp (p, GET_MODE_NAME (j)))
		    mode = (enum machine_mode) j;

	      if (mode == VOIDmode)
		error ("unknown machine mode `%s'", p);
	      else if (0 == (typefm = type_for_mode (mode,
						     TREE_UNSIGNED (type))))
		error ("no data type for mode `%s'", p);
	      else
		{
		  TREE_TYPE (decl) = type = typefm;
		  DECL_SIZE (decl) = 0;
		  layout_decl (decl, 0);
		}
	    }
	  break;

	case A_SECTION:
#ifdef ASM_OUTPUT_SECTION_NAME
	  if ((TREE_CODE (decl) == FUNCTION_DECL
	       || TREE_CODE (decl) == VAR_DECL)
	      && TREE_CODE (TREE_VALUE (args)) == STRING_CST)
	    {
	      if (TREE_CODE (decl) == VAR_DECL
		  && current_function_decl != NULL_TREE
		  && ! TREE_STATIC (decl))
		error_with_decl (decl,
		  "section attribute cannot be specified for local variables");
	      /* The decl may have already been given a section attribute from
		 a previous declaration.  Ensure they match.  */
	      else if (DECL_SECTION_NAME (decl) != NULL_TREE
		       && strcmp (TREE_STRING_POINTER (DECL_SECTION_NAME (decl)),
				  TREE_STRING_POINTER (TREE_VALUE (args))) != 0)
		error_with_decl (node,
				 "section of `%s' conflicts with previous declaration");
	      else
		DECL_SECTION_NAME (decl) = TREE_VALUE (args);
	    }
	  else
	    error_with_decl (node,
			   "section attribute not allowed for `%s'");
#else
	  error_with_decl (node,
		  "section attributes are not supported for this target");
#endif
	  break;

	case A_ALIGNED:
	  {
	    tree align_expr
	      = (args ? TREE_VALUE (args)
		 : size_int (BIGGEST_ALIGNMENT / BITS_PER_UNIT));
	    int align;
	    
	    /* Strip any NOPs of any kind.  */
	    while (TREE_CODE (align_expr) == NOP_EXPR
		   || TREE_CODE (align_expr) == CONVERT_EXPR
		   || TREE_CODE (align_expr) == NON_LVALUE_EXPR)
	      align_expr = TREE_OPERAND (align_expr, 0);

	    if (TREE_CODE (align_expr) != INTEGER_CST)
	      {
		error ("requested alignment is not a constant");
		continue;
	      }

	    align = TREE_INT_CST_LOW (align_expr) * BITS_PER_UNIT;

	    if (exact_log2 (align) == -1)
	      error ("requested alignment is not a power of 2");
	    else if (is_type)
	      TYPE_ALIGN (type) = align;
	    else if (TREE_CODE (decl) != VAR_DECL
		     && TREE_CODE (decl) != FIELD_DECL)
	      error_with_decl (decl,
			       "alignment may not be specified for `%s'");
	    else
	      DECL_ALIGN (decl) = align;
	  }
	  break;

	case A_WEAK:
	  declare_weak (decl);
	  break;

	case A_ALIAS:
	  if ((TREE_CODE (decl) == FUNCTION_DECL && DECL_INITIAL (decl))
	      || (TREE_CODE (decl) != FUNCTION_DECL && ! DECL_EXTERNAL (decl)))
	    error_with_decl (decl,
			     "`%s' defined both normally and as an alias");
	  else if (decl_function_context (decl) == 0)
	    {
	      tree id;

	      id = TREE_VALUE (args);
	      if (TREE_CODE (id) != STRING_CST)
		{
		  error ("alias arg not a string");
		  break;
		}
	      id = get_identifier (TREE_STRING_POINTER (id));

	      if (TREE_CODE (decl) == FUNCTION_DECL)
		DECL_INITIAL (decl) = error_mark_node;
	      else
		DECL_EXTERNAL (decl) = 0;
	      assemble_alias (decl, id);
	    }
	  else
	    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
	  break;

	case A_NO_CHECK_MEMORY_USAGE:
	  if (TREE_CODE (decl) != FUNCTION_DECL)
	    {
	      error_with_decl (decl,
			       "`%s' attribute applies only to functions",
			       IDENTIFIER_POINTER (name));
	    }
	  else if (DECL_INITIAL (decl))
	    {
	      error_with_decl (decl,
			       "can't set `%s' attribute after definition",
			       IDENTIFIER_POINTER (name));
	    }
	  else
	    DECL_NO_CHECK_MEMORY_USAGE (decl) = 1;
	  break;

	case A_NO_INSTRUMENT_FUNCTION:
	  if (TREE_CODE (decl) != FUNCTION_DECL)
	    {
	      error_with_decl (decl,
			       "`%s' attribute applies only to functions",
			       IDENTIFIER_POINTER (name));
	    }
	  else if (DECL_INITIAL (decl))
	    {
	      error_with_decl (decl,
			       "can't set `%s' attribute after definition",
			       IDENTIFIER_POINTER (name));
	    }
	  else
	    DECL_NO_INSTRUMENT_FUNCTION_ENTRY_EXIT (decl) = 1;
	  break;
	}
    }
}

/* Split SPECS_ATTRS, a list of declspecs and prefix attributes, into two
   lists.  SPECS_ATTRS may also be just a typespec (eg: RECORD_TYPE).

   The head of the declspec list is stored in DECLSPECS.
   The head of the attribute list is stored in PREFIX_ATTRIBUTES.

   Note that attributes in SPECS_ATTRS are stored in the TREE_PURPOSE of
   the list elements.  We drop the containing TREE_LIST nodes and link the
   resulting attributes together the way decl_attributes expects them.  */

void
split_specs_attrs (specs_attrs, declspecs, prefix_attributes)
     tree specs_attrs;
     tree *declspecs, *prefix_attributes;
{
  tree t, s, a, next, specs, attrs;

  /* This can happen in c++ (eg: decl: typespec initdecls ';').  */
  if (specs_attrs != NULL_TREE
      && TREE_CODE (specs_attrs) != TREE_LIST)
    {
      *declspecs = specs_attrs;
      *prefix_attributes = NULL_TREE;
      return;
    }

  /* Remember to keep the lists in the same order, element-wise.  */

  specs = s = NULL_TREE;
  attrs = a = NULL_TREE;
  for (t = specs_attrs; t; t = next)
    {
      next = TREE_CHAIN (t);
      /* Declspecs have a non-NULL TREE_VALUE.  */
      if (TREE_VALUE (t) != NULL_TREE)
	{
	  if (specs == NULL_TREE)
	    specs = s = t;
	  else
	    {
	      TREE_CHAIN (s) = t;
	      s = t;
	    }
	}
      else
	{
	  if (attrs == NULL_TREE)
	    attrs = a = TREE_PURPOSE (t);
	  else
	    {
	      TREE_CHAIN (a) = TREE_PURPOSE (t);
	      a = TREE_PURPOSE (t);
	    }
	  /* More attrs can be linked here, move A to the end.  */
	  while (TREE_CHAIN (a) != NULL_TREE)
	    a = TREE_CHAIN (a);
	}
    }

  /* Terminate the lists.  */
  if (s != NULL_TREE)
    TREE_CHAIN (s) = NULL_TREE;
  if (a != NULL_TREE)
    TREE_CHAIN (a) = NULL_TREE;

  /* All done.  */
  *declspecs = specs;
  *prefix_attributes = attrs;
}

/* Strip attributes from SPECS_ATTRS, a list of declspecs and attributes.
   This function is used by the parser when a rule will accept attributes
   in a particular position, but we don't want to support that just yet.

   A warning is issued for every ignored attribute.  */

tree
strip_attrs (specs_attrs)
     tree specs_attrs;
{
  tree specs, attrs;

  split_specs_attrs (specs_attrs, &specs, &attrs);

  while (attrs)
    {
      warning ("`%s' attribute ignored",
	       IDENTIFIER_POINTER (TREE_PURPOSE (attrs)));
      attrs = TREE_CHAIN (attrs);
    }

  return specs;
}

/* Print a warning if a constant expression had overflow in folding.
   Invoke this function on every expression that the language
   requires to be a constant expression.
   Note the ANSI C standard says it is erroneous for a
   constant expression to overflow.  */

void
constant_expression_warning (value)
     tree value;
{
  if ((TREE_CODE (value) == INTEGER_CST || TREE_CODE (value) == REAL_CST
       || TREE_CODE (value) == COMPLEX_CST)
      && TREE_CONSTANT_OVERFLOW (value) && pedantic)
    pedwarn ("overflow in constant expression");
}

/* Print a warning if an expression had overflow in folding.
   Invoke this function on every expression that
   (1) appears in the source code, and
   (2) might be a constant expression that overflowed, and
   (3) is not already checked by convert_and_check;
   however, do not invoke this function on operands of explicit casts.  */

void
overflow_warning (value)
     tree value;
{
  if ((TREE_CODE (value) == INTEGER_CST
       || (TREE_CODE (value) == COMPLEX_CST
	   && TREE_CODE (TREE_REALPART (value)) == INTEGER_CST))
      && TREE_OVERFLOW (value))
    {
      TREE_OVERFLOW (value) = 0;
      if (skip_evaluation == 0)
	warning ("integer overflow in expression");
    }
  else if ((TREE_CODE (value) == REAL_CST
	    || (TREE_CODE (value) == COMPLEX_CST
		&& TREE_CODE (TREE_REALPART (value)) == REAL_CST))
	   && TREE_OVERFLOW (value))
    {
      TREE_OVERFLOW (value) = 0;
      if (skip_evaluation == 0)
	warning ("floating point overflow in expression");
    }
}

/* Print a warning if a large constant is truncated to unsigned,
   or if -Wconversion is used and a constant < 0 is converted to unsigned.
   Invoke this function on every expression that might be implicitly
   converted to an unsigned type.  */

void
unsigned_conversion_warning (result, operand)
     tree result, operand;
{
  if (TREE_CODE (operand) == INTEGER_CST
      && TREE_CODE (TREE_TYPE (result)) == INTEGER_TYPE
      && TREE_UNSIGNED (TREE_TYPE (result))
      && skip_evaluation == 0
      && !int_fits_type_p (operand, TREE_TYPE (result)))
    {
      if (!int_fits_type_p (operand, signed_type (TREE_TYPE (result))))
	/* This detects cases like converting -129 or 256 to unsigned char.  */
	warning ("large integer implicitly truncated to unsigned type");
      else if (warn_conversion)
	warning ("negative integer implicitly converted to unsigned type");
    }
}

/* Convert EXPR to TYPE, warning about conversion problems with constants.
   Invoke this function on every expression that is converted implicitly,
   i.e. because of language rules and not because of an explicit cast.  */

tree
convert_and_check (type, expr)
     tree type, expr;
{
  tree t = convert (type, expr);
  if (TREE_CODE (t) == INTEGER_CST)
    {
      if (TREE_OVERFLOW (t))
	{
	  TREE_OVERFLOW (t) = 0;

	  /* Do not diagnose overflow in a constant expression merely
	     because a conversion overflowed.  */
	  TREE_CONSTANT_OVERFLOW (t) = TREE_CONSTANT_OVERFLOW (expr);

	  /* No warning for converting 0x80000000 to int.  */
	  if (!(TREE_UNSIGNED (type) < TREE_UNSIGNED (TREE_TYPE (expr))
		&& TREE_CODE (TREE_TYPE (expr)) == INTEGER_TYPE
		&& TYPE_PRECISION (type) == TYPE_PRECISION (TREE_TYPE (expr))))
	    /* If EXPR fits in the unsigned version of TYPE,
	       don't warn unless pedantic.  */
	    if ((pedantic
		 || TREE_UNSIGNED (type)
		 || ! int_fits_type_p (expr, unsigned_type (type)))
	        && skip_evaluation == 0)
	      warning ("overflow in implicit constant conversion");
	}
      else
	unsigned_conversion_warning (t, expr);
    }
  return t;
}

void
c_expand_expr_stmt (expr)
     tree expr;
{
  /* Do default conversion if safe and possibly important,
     in case within ({...}).  */
  if ((TREE_CODE (TREE_TYPE (expr)) == ARRAY_TYPE && lvalue_p (expr))
      || TREE_CODE (TREE_TYPE (expr)) == FUNCTION_TYPE)
    expr = default_conversion (expr);

  if (TREE_TYPE (expr) != error_mark_node
      && TYPE_SIZE (TREE_TYPE (expr)) == 0
      && TREE_CODE (TREE_TYPE (expr)) != ARRAY_TYPE)
    error ("expression statement has incomplete type");

  expand_expr_stmt (expr);
}

/* Validate the expression after `case' and apply default promotions.  */

tree
check_case_value (value)
     tree value;
{
  if (value == NULL_TREE)
    return value;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  STRIP_TYPE_NOPS (value);

  if (TREE_CODE (value) != INTEGER_CST
      && value != error_mark_node)
    {
      error ("case label does not reduce to an integer constant");
      value = error_mark_node;
    }
  else
    /* Promote char or short to int.  */
    value = default_conversion (value);

  constant_expression_warning (value);

  return value;
}

/* Return an integer type with BITS bits of precision,
   that is unsigned if UNSIGNEDP is nonzero, otherwise signed.  */

tree
type_for_size (bits, unsignedp)
     unsigned bits;
     int unsignedp;
{
  if (bits == TYPE_PRECISION (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;

  if (bits == TYPE_PRECISION (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;

  if (bits == TYPE_PRECISION (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;

  if (bits == TYPE_PRECISION (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;

  if (bits == TYPE_PRECISION (long_long_integer_type_node))
    return (unsignedp ? long_long_unsigned_type_node
	    : long_long_integer_type_node);

  if (bits <= TYPE_PRECISION (intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (bits <= TYPE_PRECISION (intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (bits <= TYPE_PRECISION (intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (bits <= TYPE_PRECISION (intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;

  return 0;
}

/* Return a data type that has machine mode MODE.
   If the mode is an integer,
   then UNSIGNEDP selects between signed and unsigned types.  */

tree
type_for_mode (mode, unsignedp)
     enum machine_mode mode;
     int unsignedp;
{
  if (mode == TYPE_MODE (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;

  if (mode == TYPE_MODE (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;

  if (mode == TYPE_MODE (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;

  if (mode == TYPE_MODE (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;

  if (mode == TYPE_MODE (long_long_integer_type_node))
    return unsignedp ? long_long_unsigned_type_node : long_long_integer_type_node;

  if (mode == TYPE_MODE (intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (mode == TYPE_MODE (intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (mode == TYPE_MODE (intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (mode == TYPE_MODE (intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;

#if HOST_BITS_PER_WIDE_INT >= 64
  if (mode == TYPE_MODE (intTI_type_node))
    return unsignedp ? unsigned_intTI_type_node : intTI_type_node;
#endif

  if (mode == TYPE_MODE (float_type_node))
    return float_type_node;

  if (mode == TYPE_MODE (double_type_node))
    return double_type_node;

  if (mode == TYPE_MODE (long_double_type_node))
    return long_double_type_node;

  if (mode == TYPE_MODE (build_pointer_type (char_type_node)))
    return build_pointer_type (char_type_node);

  if (mode == TYPE_MODE (build_pointer_type (integer_type_node)))
    return build_pointer_type (integer_type_node);

  return 0;
}

/* Return the minimum number of bits needed to represent VALUE in a
   signed or unsigned type, UNSIGNEDP says which.  */

int
min_precision (value, unsignedp)
     tree value;
     int unsignedp;
{
  int log;

  /* If the value is negative, compute its negative minus 1.  The latter
     adjustment is because the absolute value of the largest negative value
     is one larger than the largest positive value.  This is equivalent to
     a bit-wise negation, so use that operation instead.  */

  if (tree_int_cst_sgn (value) < 0)
    value = fold (build1 (BIT_NOT_EXPR, TREE_TYPE (value), value));

  /* Return the number of bits needed, taking into account the fact
     that we need one more bit for a signed than unsigned type.  */

  if (integer_zerop (value))
    log = 0;
  else if (TREE_INT_CST_HIGH (value) != 0)
    log = HOST_BITS_PER_WIDE_INT + floor_log2 (TREE_INT_CST_HIGH (value));
  else
    log = floor_log2 (TREE_INT_CST_LOW (value));

  return log + 1 + ! unsignedp;
}

/* Print an error message for invalid operands to arith operation CODE.
   NOP_EXPR is used as a special case (see truthvalue_conversion).  */

void
binary_op_error (code)
     enum tree_code code;
{
  register char *opname;

  switch (code)
    {
    case NOP_EXPR:
      error ("invalid truth-value expression");
      return;

    case PLUS_EXPR:
      opname = "+"; break;
    case MINUS_EXPR:
      opname = "-"; break;
    case MULT_EXPR:
      opname = "*"; break;
    case MAX_EXPR:
      opname = "max"; break;
    case MIN_EXPR:
      opname = "min"; break;
    case EQ_EXPR:
      opname = "=="; break;
    case NE_EXPR:
      opname = "!="; break;
    case LE_EXPR:
      opname = "<="; break;
    case GE_EXPR:
      opname = ">="; break;
    case LT_EXPR:
      opname = "<"; break;
    case GT_EXPR:
      opname = ">"; break;
    case LSHIFT_EXPR:
      opname = "<<"; break;
    case RSHIFT_EXPR:
      opname = ">>"; break;
    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
      opname = "%"; break;
    case TRUNC_DIV_EXPR:
    case FLOOR_DIV_EXPR:
      opname = "/"; break;
    case BIT_AND_EXPR:
      opname = "&"; break;
    case BIT_IOR_EXPR:
      opname = "|"; break;
    case TRUTH_ANDIF_EXPR:
      opname = "&&"; break;
    case TRUTH_ORIF_EXPR:
      opname = "||"; break;
    case BIT_XOR_EXPR:
      opname = "^"; break;
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      opname = "rotate"; break;
    default:
      opname = "unknown"; break;
    }
  error ("invalid operands to binary %s", opname);
}

/* Subroutine of build_binary_op, used for comparison operations.
   See if the operands have both been converted from subword integer types
   and, if so, perhaps change them both back to their original type.
   This function is also responsible for converting the two operands
   to the proper common type for comparison.

   The arguments of this function are all pointers to local variables
   of build_binary_op: OP0_PTR is &OP0, OP1_PTR is &OP1,
   RESTYPE_PTR is &RESULT_TYPE and RESCODE_PTR is &RESULTCODE.

   If this function returns nonzero, it means that the comparison has
   a constant value.  What this function returns is an expression for
   that value.  */

tree
shorten_compare (op0_ptr, op1_ptr, restype_ptr, rescode_ptr)
     tree *op0_ptr, *op1_ptr;
     tree *restype_ptr;
     enum tree_code *rescode_ptr;
{
  register tree type;
  tree op0 = *op0_ptr;
  tree op1 = *op1_ptr;
  int unsignedp0, unsignedp1;
  int real1, real2;
  tree primop0, primop1;
  enum tree_code code = *rescode_ptr;

  /* Throw away any conversions to wider types
     already present in the operands.  */

  primop0 = get_narrower (op0, &unsignedp0);
  primop1 = get_narrower (op1, &unsignedp1);

  /* Handle the case that OP0 does not *contain* a conversion
     but it *requires* conversion to FINAL_TYPE.  */

  if (op0 == primop0 && TREE_TYPE (op0) != *restype_ptr)
    unsignedp0 = TREE_UNSIGNED (TREE_TYPE (op0));
  if (op1 == primop1 && TREE_TYPE (op1) != *restype_ptr)
    unsignedp1 = TREE_UNSIGNED (TREE_TYPE (op1));

  /* If one of the operands must be floated, we cannot optimize.  */
  real1 = TREE_CODE (TREE_TYPE (primop0)) == REAL_TYPE;
  real2 = TREE_CODE (TREE_TYPE (primop1)) == REAL_TYPE;

  /* If first arg is constant, swap the args (changing operation
     so value is preserved), for canonicalization.  Don't do this if
     the second arg is 0.  */

  if (TREE_CONSTANT (primop0)
      && ! integer_zerop (primop1) && ! real_zerop (primop1))
    {
      register tree tem = primop0;
      register int temi = unsignedp0;
      primop0 = primop1;
      primop1 = tem;
      tem = op0;
      op0 = op1;
      op1 = tem;
      *op0_ptr = op0;
      *op1_ptr = op1;
      unsignedp0 = unsignedp1;
      unsignedp1 = temi;
      temi = real1;
      real1 = real2;
      real2 = temi;

      switch (code)
	{
	case LT_EXPR:
	  code = GT_EXPR;
	  break;
	case GT_EXPR:
	  code = LT_EXPR;
	  break;
	case LE_EXPR:
	  code = GE_EXPR;
	  break;
	case GE_EXPR:
	  code = LE_EXPR;
	  break;
	default:
	  break;
	}
      *rescode_ptr = code;
    }

  /* If comparing an integer against a constant more bits wide,
     maybe we can deduce a value of 1 or 0 independent of the data.
     Or else truncate the constant now
     rather than extend the variable at run time.

     This is only interesting if the constant is the wider arg.
     Also, it is not safe if the constant is unsigned and the
     variable arg is signed, since in this case the variable
     would be sign-extended and then regarded as unsigned.
     Our technique fails in this case because the lowest/highest
     possible unsigned results don't follow naturally from the
     lowest/highest possible values of the variable operand.
     For just EQ_EXPR and NE_EXPR there is another technique that
     could be used: see if the constant can be faithfully represented
     in the other operand's type, by truncating it and reextending it
     and see if that preserves the constant's value.  */

  if (!real1 && !real2
      && TREE_CODE (primop1) == INTEGER_CST
      && TYPE_PRECISION (TREE_TYPE (primop0)) < TYPE_PRECISION (*restype_ptr))
    {
      int min_gt, max_gt, min_lt, max_lt;
      tree maxval, minval;
      /* 1 if comparison is nominally unsigned.  */
      int unsignedp = TREE_UNSIGNED (*restype_ptr);
      tree val;

      type = signed_or_unsigned_type (unsignedp0, TREE_TYPE (primop0));

      maxval = TYPE_MAX_VALUE (type);
      minval = TYPE_MIN_VALUE (type);

      if (unsignedp && !unsignedp0)
	*restype_ptr = signed_type (*restype_ptr);

      if (TREE_TYPE (primop1) != *restype_ptr)
	primop1 = convert (*restype_ptr, primop1);
      if (type != *restype_ptr)
	{
	  minval = convert (*restype_ptr, minval);
	  maxval = convert (*restype_ptr, maxval);
	}

      if (unsignedp && unsignedp0)
	{
	  min_gt = INT_CST_LT_UNSIGNED (primop1, minval);
	  max_gt = INT_CST_LT_UNSIGNED (primop1, maxval);
	  min_lt = INT_CST_LT_UNSIGNED (minval, primop1);
	  max_lt = INT_CST_LT_UNSIGNED (maxval, primop1);
	}
      else
	{
	  min_gt = INT_CST_LT (primop1, minval);
	  max_gt = INT_CST_LT (primop1, maxval);
	  min_lt = INT_CST_LT (minval, primop1);
	  max_lt = INT_CST_LT (maxval, primop1);
	}

      val = 0;
      /* This used to be a switch, but Genix compiler can't handle that.  */
      if (code == NE_EXPR)
	{
	  if (max_lt || min_gt)
	    val = boolean_true_node;
	}
      else if (code == EQ_EXPR)
	{
	  if (max_lt || min_gt)
	    val = boolean_false_node;
	}
      else if (code == LT_EXPR)
	{
	  if (max_lt)
	    val = boolean_true_node;
	  if (!min_lt)
	    val = boolean_false_node;
	}
      else if (code == GT_EXPR)
	{
	  if (min_gt)
	    val = boolean_true_node;
	  if (!max_gt)
	    val = boolean_false_node;
	}
      else if (code == LE_EXPR)
	{
	  if (!max_gt)
	    val = boolean_true_node;
	  if (min_gt)
	    val = boolean_false_node;
	}
      else if (code == GE_EXPR)
	{
	  if (!min_lt)
	    val = boolean_true_node;
	  if (max_lt)
	    val = boolean_false_node;
	}

      /* If primop0 was sign-extended and unsigned comparison specd,
	 we did a signed comparison above using the signed type bounds.
	 But the comparison we output must be unsigned.

	 Also, for inequalities, VAL is no good; but if the signed
	 comparison had *any* fixed result, it follows that the
	 unsigned comparison just tests the sign in reverse
	 (positive values are LE, negative ones GE).
	 So we can generate an unsigned comparison
	 against an extreme value of the signed type.  */

      if (unsignedp && !unsignedp0)
	{
	  if (val != 0)
	    switch (code)
	      {
	      case LT_EXPR:
	      case GE_EXPR:
		primop1 = TYPE_MIN_VALUE (type);
		val = 0;
		break;

	      case LE_EXPR:
	      case GT_EXPR:
		primop1 = TYPE_MAX_VALUE (type);
		val = 0;
		break;

	      default:
		break;
	      }
	  type = unsigned_type (type);
	}

      if (!max_gt && !unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
	{
	  /* This is the case of (char)x >?< 0x80, which people used to use
	     expecting old C compilers to change the 0x80 into -0x80.  */
	  if (val == boolean_false_node)
	    warning ("comparison is always false due to limited range of data type");
	  if (val == boolean_true_node)
	    warning ("comparison is always true due to limited range of data type");
	}

      if (!min_lt && unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
	{
	  /* This is the case of (unsigned char)x >?< -1 or < 0.  */
	  if (val == boolean_false_node)
	    warning ("comparison is always false due to limited range of data type");
	  if (val == boolean_true_node)
	    warning ("comparison is always true due to limited range of data type");
	}

      if (val != 0)
	{
	  /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
	  if (TREE_SIDE_EFFECTS (primop0))
	    return build (COMPOUND_EXPR, TREE_TYPE (val), primop0, val);
	  return val;
	}

      /* Value is not predetermined, but do the comparison
	 in the type of the operand that is not constant.
	 TYPE is already properly set.  */
    }
  else if (real1 && real2
	   && (TYPE_PRECISION (TREE_TYPE (primop0))
	       == TYPE_PRECISION (TREE_TYPE (primop1))))
    type = TREE_TYPE (primop0);

  /* If args' natural types are both narrower than nominal type
     and both extend in the same manner, compare them
     in the type of the wider arg.
     Otherwise must actually extend both to the nominal
     common type lest different ways of extending
     alter the result.
     (eg, (short)-1 == (unsigned short)-1  should be 0.)  */

  else if (unsignedp0 == unsignedp1 && real1 == real2
	   && TYPE_PRECISION (TREE_TYPE (primop0)) < TYPE_PRECISION (*restype_ptr)
	   && TYPE_PRECISION (TREE_TYPE (primop1)) < TYPE_PRECISION (*restype_ptr))
    {
      type = common_type (TREE_TYPE (primop0), TREE_TYPE (primop1));
      type = signed_or_unsigned_type (unsignedp0
				      || TREE_UNSIGNED (*restype_ptr),
				      type);
      /* Make sure shorter operand is extended the right way
	 to match the longer operand.  */
      primop0 = convert (signed_or_unsigned_type (unsignedp0, TREE_TYPE (primop0)),
			 primop0);
      primop1 = convert (signed_or_unsigned_type (unsignedp1, TREE_TYPE (primop1)),
			 primop1);
    }
  else
    {
      /* Here we must do the comparison on the nominal type
	 using the args exactly as we received them.  */
      type = *restype_ptr;
      primop0 = op0;
      primop1 = op1;

      if (!real1 && !real2 && integer_zerop (primop1)
	  && TREE_UNSIGNED (*restype_ptr))
	{
	  tree value = 0;
	  switch (code)
	    {
	    case GE_EXPR:
	      /* All unsigned values are >= 0, so we warn if extra warnings
		 are requested.  However, if OP0 is a constant that is
		 >= 0, the signedness of the comparison isn't an issue,
		 so suppress the warning.  */
	      if (extra_warnings
		  && ! (TREE_CODE (primop0) == INTEGER_CST
			&& ! TREE_OVERFLOW (convert (signed_type (type),
						     primop0))))
		warning ("comparison of unsigned expression >= 0 is always true");
	      value = boolean_true_node;
	      break;

	    case LT_EXPR:
	      if (extra_warnings
		  && ! (TREE_CODE (primop0) == INTEGER_CST
			&& ! TREE_OVERFLOW (convert (signed_type (type),
						     primop0))))
		warning ("comparison of unsigned expression < 0 is always false");
	      value = boolean_false_node;
	      break;

	    default:
	      break;
	    }

	  if (value != 0)
	    {
	      /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
	      if (TREE_SIDE_EFFECTS (primop0))
		return build (COMPOUND_EXPR, TREE_TYPE (value),
			      primop0, value);
	      return value;
	    }
	}
    }

  *op0_ptr = convert (type, primop0);
  *op1_ptr = convert (type, primop1);

  *restype_ptr = boolean_type_node;

  return 0;
}

/* Prepare expr to be an argument of a TRUTH_NOT_EXPR,
   or validate its data type for an `if' or `while' statement or ?..: exp.

   This preparation consists of taking the ordinary
   representation of an expression expr and producing a valid tree
   boolean expression describing whether expr is nonzero.  We could
   simply always do build_binary_op (NE_EXPR, expr, boolean_false_node, 1),
   but we optimize comparisons, &&, ||, and !.

   The resulting type should always be `boolean_type_node'.  */

tree
truthvalue_conversion (expr)
     tree expr;
{
  if (TREE_CODE (expr) == ERROR_MARK)
    return expr;

#if 0 /* This appears to be wrong for C++.  */
  /* These really should return error_mark_node after 2.4 is stable.
     But not all callers handle ERROR_MARK properly.  */
  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case RECORD_TYPE:
      error ("struct type value used where scalar is required");
      return boolean_false_node;

    case UNION_TYPE:
      error ("union type value used where scalar is required");
      return boolean_false_node;

    case ARRAY_TYPE:
      error ("array type value used where scalar is required");
      return boolean_false_node;

    default:
      break;
    }
#endif /* 0 */

  switch (TREE_CODE (expr))
    {
      /* It is simpler and generates better code to have only TRUTH_*_EXPR
	 or comparison expressions as truth values at this level.  */
#if 0
    case COMPONENT_REF:
      /* A one-bit unsigned bit-field is already acceptable.  */
      if (1 == TREE_INT_CST_LOW (DECL_SIZE (TREE_OPERAND (expr, 1)))
	  && TREE_UNSIGNED (TREE_OPERAND (expr, 1)))
	return expr;
      break;
#endif

    case EQ_EXPR:
      /* It is simpler and generates better code to have only TRUTH_*_EXPR
	 or comparison expressions as truth values at this level.  */
#if 0
      if (integer_zerop (TREE_OPERAND (expr, 1)))
	return build_unary_op (TRUTH_NOT_EXPR, TREE_OPERAND (expr, 0), 0);
#endif
    case NE_EXPR: case LE_EXPR: case GE_EXPR: case LT_EXPR: case GT_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_NOT_EXPR:
      TREE_TYPE (expr) = boolean_type_node;
      return expr;

    case ERROR_MARK:
      return expr;

    case INTEGER_CST:
      return integer_zerop (expr) ? boolean_false_node : boolean_true_node;

    case REAL_CST:
      return real_zerop (expr) ? boolean_false_node : boolean_true_node;

    case ADDR_EXPR:
      /* If we are taking the address of a external decl, it might be zero
	 if it is weak, so we cannot optimize.  */
      if (TREE_CODE_CLASS (TREE_CODE (TREE_OPERAND (expr, 0))) == 'd'
	  && DECL_EXTERNAL (TREE_OPERAND (expr, 0)))
	break;

      if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 0)))
	return build (COMPOUND_EXPR, boolean_type_node,
		      TREE_OPERAND (expr, 0), boolean_true_node);
      else
	return boolean_true_node;

    case COMPLEX_EXPR:
      return build_binary_op ((TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 1))
			       ? TRUTH_OR_EXPR : TRUTH_ORIF_EXPR),
			      truthvalue_conversion (TREE_OPERAND (expr, 0)),
			      truthvalue_conversion (TREE_OPERAND (expr, 1)),
			      0);

    case NEGATE_EXPR:
    case ABS_EXPR:
    case FLOAT_EXPR:
    case FFS_EXPR:
      /* These don't change whether an object is non-zero or zero.  */
      return truthvalue_conversion (TREE_OPERAND (expr, 0));

    case LROTATE_EXPR:
    case RROTATE_EXPR:
      /* These don't change whether an object is zero or non-zero, but
	 we can't ignore them if their second arg has side-effects.  */
      if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 1)))
	return build (COMPOUND_EXPR, boolean_type_node, TREE_OPERAND (expr, 1),
		      truthvalue_conversion (TREE_OPERAND (expr, 0)));
      else
	return truthvalue_conversion (TREE_OPERAND (expr, 0));

    case COND_EXPR:
      /* Distribute the conversion into the arms of a COND_EXPR.  */
      return fold (build (COND_EXPR, boolean_type_node, TREE_OPERAND (expr, 0),
			  truthvalue_conversion (TREE_OPERAND (expr, 1)),
			  truthvalue_conversion (TREE_OPERAND (expr, 2))));

    case CONVERT_EXPR:
      /* Don't cancel the effect of a CONVERT_EXPR from a REFERENCE_TYPE,
	 since that affects how `default_conversion' will behave.  */
      if (TREE_CODE (TREE_TYPE (expr)) == REFERENCE_TYPE
	  || TREE_CODE (TREE_TYPE (TREE_OPERAND (expr, 0))) == REFERENCE_TYPE)
	break;
      /* fall through...  */
    case NOP_EXPR:
      /* If this is widening the argument, we can ignore it.  */
      if (TYPE_PRECISION (TREE_TYPE (expr))
	  >= TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (expr, 0))))
	return truthvalue_conversion (TREE_OPERAND (expr, 0));
      break;

    case MINUS_EXPR:
      /* With IEEE arithmetic, x - x may not equal 0, so we can't optimize
	 this case.  */
      if (TARGET_FLOAT_FORMAT == IEEE_FLOAT_FORMAT
	  && TREE_CODE (TREE_TYPE (expr)) == REAL_TYPE)
	break;
      /* fall through...  */
    case BIT_XOR_EXPR:
      /* This and MINUS_EXPR can be changed into a comparison of the
	 two objects.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 0))
	  == TREE_TYPE (TREE_OPERAND (expr, 1)))
	return build_binary_op (NE_EXPR, TREE_OPERAND (expr, 0),
				TREE_OPERAND (expr, 1), 1);
      return build_binary_op (NE_EXPR, TREE_OPERAND (expr, 0),
			      fold (build1 (NOP_EXPR,
					    TREE_TYPE (TREE_OPERAND (expr, 0)),
					    TREE_OPERAND (expr, 1))), 1);

    case BIT_AND_EXPR:
      if (integer_onep (TREE_OPERAND (expr, 1))
	  && TREE_TYPE (expr) != boolean_type_node)
	/* Using convert here would cause infinite recursion.  */
	return build1 (NOP_EXPR, boolean_type_node, expr);
      break;

    case MODIFY_EXPR:
      if (warn_parentheses && C_EXP_ORIGINAL_CODE (expr) == MODIFY_EXPR)
	warning ("suggest parentheses around assignment used as truth value");
      break;

    default:
      break;
    }

  if (TREE_CODE (TREE_TYPE (expr)) == COMPLEX_TYPE)
    {
      tree tem = save_expr (expr);
      return (build_binary_op
	      ((TREE_SIDE_EFFECTS (expr)
		? TRUTH_OR_EXPR : TRUTH_ORIF_EXPR),
	       truthvalue_conversion (build_unary_op (REALPART_EXPR, tem, 0)),
	       truthvalue_conversion (build_unary_op (IMAGPART_EXPR, tem, 0)),
	       0));
    }

  return build_binary_op (NE_EXPR, expr, integer_zero_node, 1);
}

/* Read the rest of a #-directive from input stream FINPUT.
   In normal use, the directive name and the white space after it
   have already been read, so they won't be included in the result.
   We allow for the fact that the directive line may contain
   a newline embedded within a character or string literal which forms
   a part of the directive.

   The value is a string in a reusable buffer.  It remains valid
   only until the next time this function is called.

   The terminating character ('\n' or EOF) is left in FINPUT for the
   caller to re-read.  */

char *
get_directive_line (finput)
FILE *finput;
{
    static char *directive_buffer = NULL;
    static unsigned buffer_length = 0;
    char *buffer_limit;
    int looking_for = 0;
    int char_escaped = 0;

    if (buffer_length == 0)
    {
        directive_buffer = (char *)xmalloc(128);
        buffer_length = 128;
    }

    buffer_limit = &directive_buffer[buffer_length];

    char *p = directive_buffer;

    for (;;)
    {
        int c;

        /* Make buffer bigger if it is full.  */
        if (p >= buffer_limit)
        {
            unsigned bytes_used = (p - directive_buffer);
            buffer_length *= 2;
            directive_buffer = (char *)xrealloc(directive_buffer, buffer_length);
            p = &directive_buffer[bytes_used];
            buffer_limit = &directive_buffer[buffer_length];
        }

        c = getc(finput);

        /* Discard initial whitespace.  */
        if ((c == ' ' || c == '\t') && p == directive_buffer)
        {
            do
            {
                c = getc(finput);
            } while (c == ' ' || c == '\t');
        }

        /* Detect the end of the directive.  */
        if (looking_for == 0 && (c == '\n' || c == EOF))
        {
            ungetc(c, finput);
            c = 0;
        }

        *p++ = c;

        if (c == 0)
            return directive_buffer;

        /* Handle string and character constant syntax.  */
        if (looking_for)
        {
            if (looking_for == c && !char_escaped)
                looking_for = 0; /* Found terminator... stop looking.  */
        }
        else
        {
            if (c == '\'' || c == '"')
                looking_for = c; /* Don't stop buffering until we see another one of these (or an EOF).  */
        }

        /* Handle backslash.  */
        char_escaped = (c == '\\' && !char_escaped);
    }
}

/* Make a variant type in the proper way for C/C++, propagating qualifiers
   down to the element type of an array.  */

tree
c_build_qualified_type (type, type_quals)
     tree type;
     int type_quals;
{
  /* A restrict-qualified pointer type must be a pointer to object or
     incomplete type.  Note that the use of POINTER_TYPE_P also allows
     REFERENCE_TYPEs, which is appropriate for C++.  Unfortunately,
     the C++ front-end also use POINTER_TYPE for pointer-to-member
     values, so even though it should be illegal to use `restrict'
     with such an entity we don't flag that here.  Thus, special case
     code for that case is required in the C++ front-end.  */
  if ((type_quals & TYPE_QUAL_RESTRICT)
      && (!POINTER_TYPE_P (type)
	  || !C_TYPE_OBJECT_OR_INCOMPLETE_P (TREE_TYPE (type))))
    {
      error ("invalid use of `restrict'");
      type_quals &= ~TYPE_QUAL_RESTRICT;
    }

  if (TREE_CODE (type) == ARRAY_TYPE)
    return build_array_type (c_build_qualified_type (TREE_TYPE (type),
						     type_quals),
			     TYPE_DOMAIN (type));
  return build_qualified_type (type, type_quals);
}

/* Apply the TYPE_QUALS to the new DECL.  */

void
c_apply_type_quals_to_decl (type_quals, decl)
     int type_quals;
     tree decl;
{
  if (type_quals & TYPE_QUAL_CONST)
    TREE_READONLY (decl) = 1;
  if (type_quals & TYPE_QUAL_VOLATILE)
    {
      TREE_SIDE_EFFECTS (decl) = 1;
      TREE_THIS_VOLATILE (decl) = 1;
    }
  if (type_quals & TYPE_QUAL_RESTRICT)
    {
      if (!TREE_TYPE (decl)
	  || !POINTER_TYPE_P (TREE_TYPE (decl))
	  || !C_TYPE_OBJECT_OR_INCOMPLETE_P (TREE_TYPE (TREE_TYPE (decl))))
	error ("invalid use of `restrict'");
      else if (flag_strict_aliasing)
	{
	  /* No two restricted pointers can point at the same thing.
	     However, a restricted pointer can point at the same thing
	     as an unrestricted pointer, if that unrestricted pointer
	     is based on the restricted pointer.  So, we make the
	     alias set for the restricted pointer a subset of the
	     alias set for the type pointed to by the type of the
	     decl.  */

	  int pointed_to_alias_set 
	    = get_alias_set (TREE_TYPE (TREE_TYPE (decl)));
	  
	  if (!pointed_to_alias_set)
	    /* It's not legal to make a subset of alias set zero.  */
	    ;
	  else
	    {
	      DECL_POINTER_ALIAS_SET (decl) = new_alias_set ();
	      record_alias_subset  (pointed_to_alias_set,
				    DECL_POINTER_ALIAS_SET (decl));
	    }
	}
    }
}

/* T is an expression with pointer type.  Find the DECL on which this
   expression is based.  (For example, in `a[i]' this would be `a'.)
   If there is no such DECL, or a unique decl cannot be determined,
   NULL_TREE is retured.  */

static tree
c_find_base_decl (t)
     tree t;
{
  int i;
  tree decl;

  if (t == NULL_TREE || t == error_mark_node)
    return NULL_TREE;

  if (!POINTER_TYPE_P (TREE_TYPE (t)))
    return NULL_TREE;

  decl = NULL_TREE;

  if (TREE_CODE (t) == FIELD_DECL 
      || TREE_CODE (t) == PARM_DECL
      || TREE_CODE (t) == VAR_DECL)
    /* Aha, we found a pointer-typed declaration.  */
    return t;

  /* It would be nice to deal with COMPONENT_REFs here.  If we could
     tell that `a' and `b' were the same, then `a->f' and `b->f' are
     also the same.  */

  /* Handle general expressions.  */
  switch (TREE_CODE_CLASS (TREE_CODE (t)))
    {
    case '1':
    case '2':
    case '3':
      for (i = tree_code_length [(int) TREE_CODE (t)]; --i >= 0;)
	{
	  tree d = c_find_base_decl (TREE_OPERAND (t, i));
	  if (d)
	    {
	      if (!decl)
		decl = d;
	      else if (d && d != decl)
		/* Two different declarations.  That's confusing; let's
		   just assume we don't know what's going on.  */
		decl = NULL_TREE;
	    }
	}
      break;

    default:
      break;
    }

  return decl;
}

/* Return the typed-based alias set for T, which may be an expression
   or a type.  */

int
c_get_alias_set (t)
     tree t;
{
  tree type;
  tree u;

  if (t == error_mark_node)
    return 0;

  type = (TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    ? t : TREE_TYPE (t);

  if (type == error_mark_node)
    return 0;

  /* Deal with special cases first; for certain kinds of references
     we're interested in more than just the type.  */

  if (TREE_CODE (t) == BIT_FIELD_REF)
    /* Perhaps reads and writes to this piece of data alias fields
       neighboring the bitfield.  Perhaps that's impossible.  For now,
       let's just assume that bitfields can alias everything, which is
       the conservative assumption.  */
    return 0;

  /* Permit type-punning when accessing a union, provided the access
     is directly through the union.  For example, this code does not
     permit taking the address of a union member and then storing
     through it.  Even the type-punning allowed here is a GCC
     extension, albeit a common and useful one; the C standard says
     that such accesses have implementation-defined behavior.  */
  for (u = t;
       TREE_CODE (u) == COMPONENT_REF || TREE_CODE (u) == ARRAY_REF;
       u = TREE_OPERAND (u, 0))
    if (TREE_CODE (u) == COMPONENT_REF
	&& TREE_CODE (TREE_TYPE (TREE_OPERAND (u, 0))) == UNION_TYPE)
      return 0;

  if (TREE_CODE (t) == INDIRECT_REF)
    {
      /* Check for accesses through restrict-qualified pointers.  */
      tree decl = c_find_base_decl (TREE_OPERAND (t, 0));

      if (decl && DECL_POINTER_ALIAS_SET_KNOWN_P (decl))
	/* We use the alias set indicated in the declaration.  */
	return DECL_POINTER_ALIAS_SET (decl);
    }

  /* From here on, only the type matters.  */

  if (TREE_CODE (t) == COMPONENT_REF
      && DECL_BIT_FIELD_TYPE (TREE_OPERAND (t, 1)))
    /* Since build_modify_expr calls get_unwidened for stores to
       component references, the type of a bit field can be changed
       from (say) `unsigned int : 16' to `unsigned short' or from 
       `enum E : 16' to `short'.  We want the real type of the
       bit-field in this case, not some the integral equivalent.  */
    type = DECL_BIT_FIELD_TYPE (TREE_OPERAND (t, 1));

  if (TYPE_ALIAS_SET_KNOWN_P (type))
    /* If we've already calculated the value, just return it.  */
    return TYPE_ALIAS_SET (type);
  else if (TYPE_MAIN_VARIANT (type) != type)
    /* The C standard specifically allows aliasing between
       cv-qualified variants of types.  */
    TYPE_ALIAS_SET (type) = c_get_alias_set (TYPE_MAIN_VARIANT (type));
  else if (TREE_CODE (type) == INTEGER_TYPE)
    {
      tree signed_variant;

      /* The C standard specifically allows aliasing between signed and
	 unsigned variants of the same type.  We treat the signed
	 variant as canonical.  */
      signed_variant = signed_type (type);

      if (signed_variant != type)
	TYPE_ALIAS_SET (type) = c_get_alias_set (signed_variant);
      else if (signed_variant == signed_char_type_node)
	/* The C standard guarantess that any object may be accessed
	   via an lvalue that has character type.  We don't have to
	   check for unsigned_char_type_node or char_type_node because
	   we are specifically looking at the signed variant.  */
	TYPE_ALIAS_SET (type) = 0;
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    /* Anything that can alias one of the array elements can alias
       the entire array as well.  */
    TYPE_ALIAS_SET (type) = c_get_alias_set (TREE_TYPE (type));
  else if (TREE_CODE (type) == FUNCTION_TYPE)
    /* There are no objects of FUNCTION_TYPE, so there's no point in
       using up an alias set for them.  (There are, of course,
       pointers and references to functions, but that's 
       different.)  */
    TYPE_ALIAS_SET (type) = 0;
  else if (TREE_CODE (type) == RECORD_TYPE
	   || TREE_CODE (type) == UNION_TYPE)
    /* If TYPE is a struct or union type then we're reading or
       writing an entire struct.  Thus, we don't know anything about
       aliasing.  (In theory, such an access can only alias objects
       whose type is the same as one of the fields, recursively, but
       we don't yet make any use of that information.)  */
    TYPE_ALIAS_SET (type) = 0;

  if (!TYPE_ALIAS_SET_KNOWN_P (type)) 
    /* TYPE is something we haven't seen before.  Put it in a new
       alias set.  */
    TYPE_ALIAS_SET (type) = new_alias_set ();

  return TYPE_ALIAS_SET (type);
}
