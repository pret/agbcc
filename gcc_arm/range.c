/* CYGNUS LOCAL LRS */
/* Allocate registers within a basic block, for GNU compiler.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.

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

/* Live range support that splits a variable that is spilled to the stack into
   smaller ranges so that at least the middle of small loops the variable will
   be run in registers.  This is run after global allocation if one or more
   variables were denied a register, and then global allocation is done once
   again.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "flags.h"
#include "basic-block.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "expr.h"
#include "except.h"
#include "function.h"
#include "obstack.h"
#include "range.h"
#include "toplev.h"

void init_regset_vector	PROTO ((regset *, int, struct obstack *));

extern struct obstack *rtl_obstack;

/* Information that we gather about registers */
typedef struct rinfo_def {
  rtx reg;				/* Register insn */
  int refs;				/* # of register references */
  int sets;				/* # of register sets/clobbers */
  int deaths;				/* # of register deaths */
  int live_length;			/* # of insns in range value is live */
  int n_calls;				/* # of calls this reg crosses */
  int copy_flags;			/* copy {in,out} flags */
} rinfo;

/* Basic blocks expressed as a linked list */
typedef struct bb_link_def {
  int block;				/* block number or -1 */
  rtx first_insn;			/* first insn in block */
  rtx last_insn;			/* last insn in block */
  regset live_at_start;			/* live information */
} bb_link;

/* Symbol/block node that a variable is declared in and whether a register
   only holds constants.  */
typedef struct var_info_def {
  tree symbol;				/* DECL_NODE of the symbol */
  tree block;				/* BLOCK_NODE variable is declared in */
  rtx constant_value;			/* what value a register always holds */
} var_info;

int range_max_unique;			/* Range #, monotonically increasing */

static rinfo	 *range_info;		/* Register information */
static var_info	 *range_vars;		/* Map regno -> variable */
static int	 *range_regs;		/* Registers used in the loop */
static int	  range_num_regs;	/* # of registers in range_regs */
static int	  range_loop_depth;	/* current loop depth */
static int	  range_update_used_p;	/* whether range_used should be set */
static regset	  range_used;		/* regs used in the current range */
static regset	  range_set;		/* regs set in the current range */
static regset	  range_live;		/* regs currently live */
static regset	  range_mixed_mode;	/* regs that use different modes */
static regset	  range_no_move;	/* regs that don't have simple moves */
static unsigned	  range_max_uid;	/* Size of UID->basic block mapping */
static bb_link	**range_block_insn;	/* Map INSN # to basic block # */
static bb_link   *range_block_orig;	/* original basic blocks */

/* Linked list of live ranges to try allocating registers in first before
   allocating all of the remaining registers.  */
rtx live_range_list;

#define RANGE_BLOCK_NUM(INSN)						\
(((unsigned)INSN_UID (INSN) < (unsigned)range_max_uid			\
  && range_block_insn[INSN_UID (INSN)])					\
 ? range_block_insn[INSN_UID (INSN)]->block : -1)

/* Forward references */
static void range_mark			PROTO((rtx, int, rtx));
static void range_basic_mark		PROTO((rtx, regset, rtx));
static void range_basic_insn		PROTO((rtx, regset, int));
static void range_bad_insn		PROTO((FILE *, char *, rtx));
static void range_print_flags		PROTO((FILE *, int, char *));
static void print_blocks_internal	PROTO((FILE *, tree, int));
static int  range_inner			PROTO((FILE *, rtx, rtx, rtx, rtx,
					       regset, regset, int));
static void range_update_basic_block	PROTO((FILE *, rtx, bb_link *,
					       int, int));
static void range_finish		PROTO((FILE *, rtx, int, int));
static void range_scan_blocks		PROTO((tree, tree));
static int  range_compare		PROTO((const GENERIC_PTR,
					       const GENERIC_PTR));


/* Determine which registers are used/set */

static void
range_mark (x, set_p, insn)
     rtx x;
     int set_p;
     rtx insn;
{
  register RTX_CODE code = GET_CODE (x);
  register int i, regno;
  register char *fmt;

restart:

  switch (code)
    {
    default:
      break;

      /* Make sure we mark the registers that are set */
    case SET:
      range_mark (SET_DEST (x), TRUE, insn);
      range_mark (SET_SRC (x), FALSE, insn);
      return;

      /* Treat clobber like a set.  */
      /* Pre-increment and friends always update as well as reference.  */
    case CLOBBER:
    case PRE_INC:
    case PRE_DEC:
    case POST_INC:
    case POST_DEC:
      range_mark (XEXP (x, 0), TRUE, insn);
      return;

      /* Memory addresses just reference the register, even if this is a SET */
    case MEM:
      range_mark (XEXP (x, 0), FALSE, insn);
      return;

      /* Treat subregs as using/modifying the whole register.  */
    case SUBREG:
      x = SUBREG_REG (x);
      code = GET_CODE (x);
      goto restart;

      /* Actual registers, skip hard registers */
    case REG:
      regno = REGNO (x);
      if (regno >= FIRST_PSEUDO_REGISTER)
	{
	  if (!range_info[regno].reg)
	    range_info[regno].reg = x;
	  else if (GET_MODE (x) != GET_MODE (range_info[regno].reg))
	    SET_REGNO_REG_SET (range_mixed_mode, regno);

	  range_info[regno].refs += 2*range_loop_depth;
	  SET_REGNO_REG_SET (range_live, regno);
	  if (range_update_used_p)
	    SET_REGNO_REG_SET (range_used, regno);

	  /* If there isn't a simple move pattern for the mode, skip it */
	  if (mov_optab->handlers[(int) GET_MODE (x)].insn_code
	      == CODE_FOR_nothing)
	    SET_REGNO_REG_SET (range_no_move, regno);

	  if (set_p)
	    {
	      range_info[regno].sets++;
	      SET_REGNO_REG_SET (range_set, regno);
	    }
	}

      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	range_mark (XEXP (x, i), set_p, insn);

      else if (fmt[i] == 'E')
	{
	  register unsigned j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    range_mark (XVECEXP (x, i, j), set_p, insn);
	}
    }
}


/* Like range_mark, except more stripped down, to just care about what
   registers are currently live.  */

static void
range_basic_mark (x, live, insn)
     rtx x;
     regset live;
     rtx insn;
{
  register RTX_CODE code = GET_CODE (x);
  register int i, regno, lim;
  register char *fmt;

restart:
  switch (code)
    {
    default:
      break;

      /* Treat subregs as using/modifying the whole register.  */
    case SUBREG:
      x = SUBREG_REG (x);
      code = GET_CODE (x);
      goto restart;

      /* Actual registers */
    case REG:
      regno = REGNO (x);
      lim = regno + ((regno >= FIRST_PSEUDO_REGISTER)
		     ? 1
		     : HARD_REGNO_NREGS (regno, GET_MODE (x)));

      for (i = regno; i < lim; i++)
	SET_REGNO_REG_SET (live, i);
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	range_basic_mark (XEXP (x, i), live, insn);

      else if (fmt[i] == 'E')
	{
	  register unsigned j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    range_basic_mark (XVECEXP (x, i, j), live, insn);
	}
    }
}

/* For an INSN, CALL_INSN, or JUMP_INSN, update basic live/dead information.  */

static void
range_basic_insn (insn, live, range_max_regno)
     rtx insn;
     regset live;
     int range_max_regno;
{
  rtx note, x;
  int i;

  range_basic_mark (PATTERN (insn), live, insn);

  /* Remember if the value is only set to one constant.  */
  if (GET_CODE (insn) == INSN
      && (x = single_set (insn)) != NULL_RTX
      && (GET_CODE (SET_DEST (x)) == REG
	  && REG_N_SETS (REGNO (SET_DEST (x))) == 1
	  && CONSTANT_P (SET_SRC (x)))
	  && REGNO (SET_DEST (x)) < range_max_regno)
      range_vars[ REGNO (SET_DEST (x))].constant_value = SET_SRC (x);

  /* figure out which registers are now dead.  */
  for (note = REG_NOTES (insn); note != NULL_RTX; note = XEXP (note, 1))
    {
      if (REG_NOTE_KIND (note) == REG_DEAD
	  && GET_CODE (XEXP (note, 0)) == REG)
	{
	  rtx reg = XEXP (note, 0);
	  int regno = REGNO (reg);
	  int lim = regno + ((regno >= FIRST_PSEUDO_REGISTER)
			     ? 1
			     : HARD_REGNO_NREGS (regno, GET_MODE (reg)));

	  for (i = regno; i < lim; i++)
	    CLEAR_REGNO_REG_SET (live, i);
	}
    }
}


/* Sort the registers by number of uses */

static int
range_compare (v1p, v2p)
     const GENERIC_PTR v1p;
     const GENERIC_PTR v2p;
{
  int i1 = *(int *)v1p;
  int i2 = *(int *)v2p;
  int r1 = range_info[i1].refs;
  int r2 = range_info[i2].refs;

  if (r2 - r1)
    return r2 - r1;

  /* Make sure that range_compare always is stable, so if the values are equal,
     compare based on pseduo register number.  */
  return REGNO (range_info[i2].reg) - REGNO (range_info[i1].reg);
}


/* If writing a .range file, print a message and an rtl.  */

static void
range_bad_insn (stream, msg, insn)
     FILE *stream;
     char *msg;
     rtx insn;
{
  if (stream)
    {
      fputs (msg, stream);
      print_rtl (stream, PATTERN (insn));
      putc ('\n', stream);
    }
}


/* Print out to STREAM the copyin/copyout flags.  */

static void
range_print_flags (stream, flags, prefix)
     FILE *stream;
     int flags;
     char *prefix;
{
  if ((flags & LIVE_RANGE_COPYIN) != 0)
    {
      fprintf (stream, "%scopyin", prefix);
      prefix = ", ";
    }

  if ((flags & LIVE_RANGE_COPYIN_CONST) != 0)
    {
      fprintf (stream, "%sconst", prefix);
      prefix = ", ";
    }

  if ((flags & LIVE_RANGE_COPYOUT) != 0)
    fprintf (stream, "%scopyout", prefix);

}


/* Print out range information to STREAM, using RANGE as the range_info rtx,
   printing TAB and  COMMENT at the beginning of each line.  */

void
live_range_print (stream, range, tab, comment)
     FILE *stream;
     rtx range;
     char *tab;
     char *comment;
{
  int i;

  fprintf (stream,
	   "%s%s range #%d start, %d calls, basic block {start/end} %d/%d, loop depth %d\n",
	   tab, comment,
	   RANGE_INFO_UNIQUE (range), RANGE_INFO_NCALLS (range),
	   RANGE_INFO_BB_START (range), RANGE_INFO_BB_END (range),
	   RANGE_INFO_LOOP_DEPTH (range));

  for (i = 0; i < (int)RANGE_INFO_NUM_REGS (range); i++)
    {
      int pseudo = RANGE_REG_PSEUDO (range, i);
      int copy = RANGE_REG_COPY (range, i);

      fprintf (stream, "%s%s reg %d", tab, comment, pseudo);
      if (reg_renumber)
	{
	  if ((unsigned)pseudo >= (unsigned)max_regno)
	    fprintf (stream, " (illegal)");
	  else if (reg_renumber[pseudo] >= 0)
	    fprintf (stream, " (%s)", reg_names[ reg_renumber[pseudo]]);
	}

      fprintf (stream, ", copy %d", copy);
      if (reg_renumber)
	{
	  if ((unsigned)copy >= (unsigned)max_regno)
	    fprintf (stream, " (illegal)");
	  else if (reg_renumber[copy] >= 0)
	    fprintf (stream, " (%s)", reg_names[ reg_renumber[copy]]);
	}

      fprintf (stream,
	       ", %d ref(s), %d set(s), %d death(s), %d live length, %d calls",
	       RANGE_REG_REFS (range, i),
	       RANGE_REG_SETS (range, i),
	       RANGE_REG_DEATHS (range, i),
	       RANGE_REG_LIVE_LENGTH (range, i),
	       RANGE_REG_N_CALLS (range, i));

      range_print_flags (stream, RANGE_REG_COPY_FLAGS (range, i), ", ");
      if (REG_USERVAR_P (regno_reg_rtx[pseudo]))
	{
	  fprintf (stream, ", user");
	  if (RANGE_REG_SYMBOL_NODE (range, i))
	    {
	      tree name = DECL_NAME (RANGE_REG_SYMBOL_NODE (range, i));
	      if (name)
		fprintf (stream, " [%s]", IDENTIFIER_POINTER (name));
	    }
	}

      if (REGNO_POINTER_FLAG (pseudo))
	fprintf (stream, ", ptr");

      putc ('\n', stream);
    }
}


/* CYGNUS LOCAL -- meissner/live range */
/* Print the scoping blocks in the current function */

static void
print_blocks_internal (stream, block, level)
     FILE *stream;
     tree block;
     int level;
{
  /* Loop over all blocks */
  for (; block != NULL_TREE; block = BLOCK_CHAIN (block))
    {
      int i;
      tree vars_types[2];
      static char *vars_types_name[] = {"vars: ", "types:"};

      fprintf (stream, "%*sBlock ", level*4, "");
      fprintf (stream, HOST_WIDE_INT_PRINT_HEX, (HOST_WIDE_INT) block);

      if (BLOCK_CHAIN (block))
	{
	  fprintf (stream, ", chain ");
	  fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		   (HOST_WIDE_INT) BLOCK_CHAIN (block));
	}

      if (BLOCK_VARS (block))
	{
	  fprintf (stream, ", vars ");
	  fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		   (HOST_WIDE_INT) BLOCK_VARS (block));
	}

      if (BLOCK_TYPE_TAGS (block))
	{
	  fprintf (stream, ", types ");
	  fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		   (HOST_WIDE_INT) BLOCK_TYPE_TAGS (block));
	}

      if (BLOCK_SUBBLOCKS (block))
	{
	  fprintf (stream, ", subblocks ");
	  fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		   (HOST_WIDE_INT) BLOCK_SUBBLOCKS (block));
	}

      if (BLOCK_ABSTRACT_ORIGIN (block))
	{
	  fprintf (stream, ", abstract origin ");
	  fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		   (HOST_WIDE_INT) BLOCK_ABSTRACT_ORIGIN (block));
	}

      if (BLOCK_ABSTRACT (block))
	fprintf (stream, ", abstract");

      if (BLOCK_LIVE_RANGE_FLAG (block))
	fprintf (stream, ", live-range");

      if (BLOCK_LIVE_RANGE_VAR_FLAG (block))
	fprintf (stream, ", live-range-vars");

      if (BLOCK_HANDLER_BLOCK (block))
	fprintf (stream, ", handler");

      if (TREE_USED (block))
	fprintf (stream, ", used");

      if (TREE_ASM_WRITTEN (block))
	fprintf (stream, ", asm-written");

      fprintf (stream, "\n");
      vars_types[0] = BLOCK_VARS (block);
      vars_types[1] = BLOCK_TYPE_TAGS (block);
      for (i = 0; i < 2; i++)
	if (vars_types[i])
	  {
	    tree vars;
	    int indent = ((level < 4) ? 16 : (level*4) + 4) - 1;
	    int len = 0;

	    for (vars = BLOCK_VARS (block);
		 vars != NULL_TREE;
		 vars = TREE_CHAIN (vars))
	      {
		if (DECL_NAME (vars) && IDENTIFIER_POINTER (DECL_NAME (vars)))
		  {
		    if (len == 0)
		      {
			len = indent + 1 + strlen (vars_types_name[i]);
			fprintf (stream, "%*s%s", indent+1, "",
				 vars_types_name[i]);
		      }

		    len += IDENTIFIER_LENGTH (DECL_NAME (vars)) + 1;
		    if (len >= 80 && len > indent)
		      {
			len = indent;
			fprintf (stream, "\n%*s", indent, "");
		      }

		    fprintf (stream, " %.*s",
			     IDENTIFIER_LENGTH (DECL_NAME (vars)),
			     IDENTIFIER_POINTER (DECL_NAME (vars)));
		  }
	      }

	  fprintf (stream, "\n\n");
	}

      print_blocks_internal (stream, BLOCK_SUBBLOCKS (block), level+1);
    }
}

void
print_all_blocks ()
{
  fprintf (stderr, "\n");
  print_blocks_internal (stderr, DECL_INITIAL (current_function_decl), 0);
}


/* Function with debugging output to STREAM that handles a sequence of insns
   that goes from RANGE_START to RANGE_END, splitting the range of variables
   used between INNER_START and INNER_END.  Registers in LIVE_AT_START are live
   at the first insn.  BB_{START,END} holds the basic block numbers of the
   first and last insns.  Return the number of variables that had their ranges
   split into a new pseudo variable used only within the loop.  */

static int
range_inner (stream, range_start, range_end, inner_start, inner_end,
	     live_at_start, live_at_end, loop_depth)
     FILE *stream;
     rtx range_start;
     rtx range_end;
     rtx inner_start;
     rtx inner_end;
     regset live_at_start;
     regset live_at_end;
     int loop_depth;
{
  rtx insn;
  rtx pattern;
  rtx label_ref;
  rtx label_chain;
  rtx start;
  rtx end;
  rtx *regs;
  rtvec regs_rtvec;
  rtx note;
  int first_label = get_first_label_num ();
  int last_label = max_label_num ();
  unsigned num_labels = last_label - first_label + 1;
  unsigned indx;
  int i;
  int regno;
  int block;
  int *loop_label_ref = (int *) alloca (sizeof (int) * num_labels);
  int *all_label_ref = (int *) alloca (sizeof (int) * num_labels);
  char *found_label_ref = (char *) alloca (sizeof (char) * num_labels);
  int ok_p = TRUE;
  int n_insns;
  int n_calls;
  regset live_at_start_copy;
  regset live_at_end_copy;
  int bb_start;
  int bb_end;
  char *nl;
  rtx ri;

  bzero ((char *)loop_label_ref, sizeof (int) * num_labels);
  bzero ((char *)all_label_ref, sizeof (int) * num_labels);
  bzero ((char *) range_info, sizeof (rinfo) * max_reg_num ());
  CLEAR_REG_SET (range_used);
  CLEAR_REG_SET (range_set);
  CLEAR_REG_SET (range_mixed_mode);
  CLEAR_REG_SET (range_no_move);
  COPY_REG_SET (range_live, live_at_start);
  range_num_regs = 0;
  range_loop_depth = loop_depth;

  if (stream)
    fprintf (stream, "\nPossible range from %d to %d (inner range %d to %d)\nLive at start: ",
	     INSN_UID (range_start), INSN_UID (range_end),
	     INSN_UID (inner_start), INSN_UID (inner_end));

  /* Mark the live registers as needing copies to the tmp register from the
     original value.  */
  EXECUTE_IF_SET_IN_REG_SET (live_at_start, FIRST_PSEUDO_REGISTER, i,
			     {
			       range_info[i].copy_flags |= LIVE_RANGE_COPYIN;
			       if (stream)
				 fprintf (stream, " %d", i);
			       if (range_vars[i].constant_value)
				 {
				   range_info[i].copy_flags
				     |= LIVE_RANGE_COPYIN_CONST;

				   if (stream)
				     fprintf (stream, " [constant]");
				 }
			     });

  if (stream)
    {
      fprintf (stream, "\nLive at end:   ");
      EXECUTE_IF_SET_IN_REG_SET (live_at_end, FIRST_PSEUDO_REGISTER, i,
				 {
				   fprintf (stream, " %d", i);
				 });

      putc ('\n', stream);
    }

  /* Calculate basic block start and end */
  bb_start = -1;
  for (insn = range_start;
       insn && insn != range_end && (bb_start = RANGE_BLOCK_NUM (insn)) < 0;
       insn = NEXT_INSN (insn))
    ;

  bb_end = -1;
  for (insn = range_end;
       insn && insn != range_start && (bb_end = RANGE_BLOCK_NUM (insn)) < 0;
       insn = PREV_INSN (insn))
    ;

  if (bb_start < 0)
    {
      ok_p = FALSE;
      if (stream)
	fprintf (stream, "Cannot find basic block start\n");
    }

  if (bb_end < 0)
    {
      ok_p = FALSE;
      if (stream)
	fprintf (stream, "Cannot find basic block end\n");
    }

  /* Scan the loop, looking for jumps outside/inside of the loop.  If the
     loop has such jumps, we ignore it.  */
  n_insns = n_calls = block = 0;
  range_update_used_p = FALSE;
  for (insn = range_start; insn && insn != range_end; insn = NEXT_INSN (insn))
    {
      enum rtx_code code = GET_CODE (insn);
      int block_tmp = RANGE_BLOCK_NUM (insn);
      if (block_tmp >= 0 && block != block_tmp)
	{
	  block = block_tmp;
	  COPY_REG_SET (range_live, basic_block_live_at_start[block]);
	}

      /* Only mark registers that appear between INNER_START and INNER_END */
      if (insn == inner_start)
	range_update_used_p = TRUE;
      else if (insn == inner_end)
	range_update_used_p = FALSE;

      if (GET_RTX_CLASS (code) == 'i')
	{
	  n_insns++;
	  /* Mark used registers */
	  range_mark (PATTERN (insn), FALSE, insn);

	  /* Update live length, & number of calls that the insn crosses */
	  EXECUTE_IF_SET_IN_REG_SET (range_live, FIRST_PSEUDO_REGISTER, i,
				     {
				       range_info[i].live_length++;
				       if (GET_CODE (insn) == CALL_INSN)
					 range_info[i].n_calls++;
				     });

	  /* figure out which ones will be dead by the end of the region */
	  for (note = REG_NOTES (insn); note != NULL_RTX; note = XEXP (note, 1))
	    {
	      if (REG_NOTE_KIND (note) == REG_DEAD
		  && GET_CODE (XEXP (note, 0)) == REG
		  && REGNO (XEXP (note, 0)) > FIRST_PSEUDO_REGISTER)
		{
		  CLEAR_REGNO_REG_SET (range_live, REGNO (XEXP (note, 0)));
		  range_info[ REGNO (XEXP (note, 0)) ].deaths++;
		}
	    }
	}

      switch (code)
	{
	default:
	  break;

	  /* Update loop_depth */
	case NOTE:
	  if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	    range_loop_depth++;
	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
	    range_loop_depth++;
	  break;

	/* Record whether there was a call.  */
	case CALL_INSN:
	  n_calls++;
	  break;

	/* Found a label, record the number of uses */
	case CODE_LABEL:
	  indx = CODE_LABEL_NUMBER (insn) - first_label;
	  if (indx >= num_labels)
	    {
	      if (stream)
		fprintf (stream, "Code label %d doesn't fit in label table",
			 CODE_LABEL_NUMBER (insn));

	      ok_p = FALSE;
	      continue;
	    }

	  all_label_ref[indx] = LABEL_NUSES (insn);
	  break;

	/* Found a jump of some sort, see where the jump goes.  */
	case JUMP_INSN:
	  pattern = PATTERN (insn);

	  /* Switch statement, process all labels */
	  if (GET_CODE (pattern) == ADDR_VEC
	      || GET_CODE (pattern) == ADDR_DIFF_VEC)
	    {
	      int vec_num = (GET_CODE (PATTERN (insn)) == ADDR_VEC) ? 0 : 1;
	      int len = XVECLEN (pattern, vec_num);
	      bzero ((char *)found_label_ref, sizeof (char) * num_labels);

	      for (i = 0; i < len; i++)
		{
		  label_ref = XEXP (XVECEXP (pattern, vec_num, i), 0);
		  indx = CODE_LABEL_NUMBER (label_ref) - first_label;

		  if (indx >= num_labels)
		    {
		      range_bad_insn (stream,
				      "Label ref doesn't fit in label table\n",
				      label_ref);
		      ok_p = FALSE;
		    }

		  /* Only process duplicated labels once, since the LABEL_REF
		     chain only includes it once.  */
		  else if (!found_label_ref[indx])
		    {
		      found_label_ref[indx] = TRUE;
		      loop_label_ref[indx]++;
		    }
		}
	    }

	  else
	    {
	      label_ref = JUMP_LABEL (insn);
	      if (!label_ref)
		{
		  rtx sset;
		  range_bad_insn
		    (stream,
		     (((sset = single_set (insn)) != NULL_RTX
		       && (GET_CODE (SET_SRC (sset)) == REG
			   || GET_CODE (SET_SRC (sset)) == SUBREG
			   || GET_CODE (SET_SRC (sset)) == MEM))
		      ? "Jump to indeterminate label in inner loop\n"
		      : "JUMP_LABEL (insn) is null.\n"),
		     insn);

		  ok_p = FALSE;
		  continue;
		}

	      if (GET_CODE (label_ref) != CODE_LABEL)
		{
		  range_bad_insn (stream,
				  "JUMP_LABEL (insn) is not a CODE_LABEL.\n",
				  insn);

		  ok_p = FALSE;
		  continue;
		}

	      indx = CODE_LABEL_NUMBER (label_ref) - first_label;
	      if (indx >= num_labels)
		{
		  range_bad_insn (stream,
				  "Label ref doesn't fit in label table\n",
				  insn);

		  ok_p = FALSE;
		  continue;
		}

	      loop_label_ref[indx]++;
	    }

	  break;
	}
    }

  /* Now that we've scanned the loop, check for any jumps into or out of
     the loop, and if we've found them, don't do live range splitting.
     If there are no registers used in the loop, there is nothing to do.  */
  nl = (char *)0;
  if (ok_p)
    {
      for (i = 0; i < (int)num_labels; i++)
	if (loop_label_ref[i] != all_label_ref[i])
	  {
	    ok_p = FALSE;
	    if (stream)
	      {
		nl = "\n";
		if (!all_label_ref[i])
		  fprintf (stream, "label %d was outside of the loop.\n", i+first_label);
		else
		  fprintf (stream, "label %d had %d references, only %d were in loop.\n",
			   i+first_label, all_label_ref[i], loop_label_ref[i]);
	      }
	  }

      /* ignore any registers that use different modes, or that don't have simple
	 move instructions. */
      if (stream)
	{
	  EXECUTE_IF_SET_IN_REG_SET (range_mixed_mode, FIRST_PSEUDO_REGISTER, regno,
				     {
				       nl = "\n";
				       fprintf (stream,
						"Skipping register %d used with different types\n",
						regno);
				     });

	  EXECUTE_IF_SET_IN_REG_SET (range_no_move, FIRST_PSEUDO_REGISTER, regno,
				     {
				       nl = "\n";
				       fprintf (stream,
						"Skipping register %d that needs complex moves\n",
						regno);
				     });
	}

      IOR_REG_SET (range_mixed_mode, range_no_move);
      AND_COMPL_REG_SET (range_used, range_mixed_mode);
      AND_COMPL_REG_SET (range_set, range_mixed_mode);
      EXECUTE_IF_SET_IN_REG_SET(range_used, FIRST_PSEUDO_REGISTER, regno,
				{
				  /* If the register isn't live at the end,
				     indicate that it must have died.  */
				  if (REGNO_REG_SET_P (live_at_end, regno))
				    range_info[regno].copy_flags
				      |= LIVE_RANGE_COPYOUT;

				  /* If the register is only used in a single
				     basic block, let local-alloc allocate
				     a register for it.  */
				  if (REG_BASIC_BLOCK (regno) >= 0)
				    {
				      nl = "\n";
				      if (stream)
					fprintf (stream,
						 "Skipping %d due to being used in a single basic block\n",
						 regno);
				      continue;
				    }

				  /* If the register is live only within the
				     range, don't bother with it.  */
				  if (REG_LIVE_LENGTH (regno)
				      <= range_info[regno].live_length)
				    {
				      nl = "\n";
				      if (stream)
					fprintf (stream,
						 "Skipping %d due to being used only in range\n",
						 regno);
				      continue;
				    }

				  range_regs[range_num_regs++] = regno;
				});

      if (range_num_regs == 0)
	{
	  ok_p = FALSE;
	  if (stream)
	    fprintf (stream, "No registers found in loop\n");
	}
    }

  if (stream && nl)
    fputs (nl, stream);

  if (!ok_p)
    return 0;

  qsort (range_regs, range_num_regs, sizeof (int), range_compare);

  CLEAR_REG_SET (range_used);
  for (i = 0; i < range_num_regs; i++)
    SET_REGNO_REG_SET (range_used, range_regs[i]);

#if 0
  /* Narrow down range_start so that we include only those insns that reference
     one of the live range variables.  */
  regs_ref = ALLOCA_REG_SET ();
  regs_set = ALLOCA_REG_SET ();
  regs_tmp = ALLOCA_REG_SET ();

  uses_regs = inner_start;
  for (insn = inner_start; insn; insn = PREV_INSN (insn))
    {
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i'
	  && regset_mentioned_p (range_used, PATTERN (insn), FALSE, regs_ref,
				 regs_set))
	{
	  uses_regs = insn;
	}

      if (insn == range_start)
	break;
    }
  range_start = uses_regs;

  /* Narrow down range_end so that we include only those insns that reference
     one of the live range variables.  */
  CLEAR_REG_SET (regs_ref);
  uses_regs = inner_end;
  for (insn = inner_end; insn; insn = NEXT_INSN (insn))
    {
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i'
	  && regset_mentioned_p (range_used, PATTERN (insn), FALSE, regs_ref,
				 regs_set))
	{
	  uses_regs = insn;
	}

      if (insn == range_end)
	break;
    }
  range_end = uses_regs;

  FREE_REG_SET (regs_ref);
  FREE_REG_SET (regs_set);
  FREE_REG_SET (regs_tmp);
#endif

  /* Mark the live range region.  */
  regs = (rtx *) alloca (sizeof (rtx) * range_num_regs);
  for (i = 0; i < range_num_regs; i++)
    {
      int r = range_regs[i];
      rinfo *ri = &range_info[r];

      if (range_vars[r].block)
	BLOCK_LIVE_RANGE_VAR_FLAG (range_vars[r].block) = TRUE;

      REG_N_RANGE_CANDIDATE_P (r) = 1;
      regs[i] = gen_rtx (RANGE_REG, VOIDmode,
			 r, -1, ri->refs, ri->sets, ri->deaths,
			 ri->copy_flags, ri->live_length, ri->n_calls,
			 range_vars[r].symbol, range_vars[r].block);
    }

  live_at_start_copy = OBSTACK_ALLOC_REG_SET (rtl_obstack);
  live_at_end_copy = OBSTACK_ALLOC_REG_SET (rtl_obstack);
  COPY_REG_SET (live_at_start_copy, live_at_start);
  COPY_REG_SET (live_at_end_copy, live_at_end);

  regs_rtvec = gen_rtvec_v (range_num_regs, regs);
  start = emit_note_before (NOTE_INSN_RANGE_START, range_start);
  end = emit_note_after (NOTE_INSN_RANGE_END, range_end);

  ri = gen_rtx (RANGE_INFO, VOIDmode, start, end, regs_rtvec, n_calls, n_insns,
		range_max_unique++, bb_start, bb_end, loop_depth,
		live_at_start_copy, live_at_end_copy, 0, 0);

  NOTE_RANGE_INFO (start) = ri;
  NOTE_RANGE_INFO (end) = ri;

  live_range_list = gen_rtx (INSN_LIST, VOIDmode, start, live_range_list);
  return range_num_regs;
}


/* Scan all blocks looking for user variables so that we can map from
   register number back to the appropriate DECL_NODE and BLOCK_NODE.  */

static void
range_scan_blocks (block, args)
     tree block;
     tree args;
{
  tree var;

  /* Scan arguments */
  for (var = args; var != NULL_TREE; var = TREE_CHAIN (var))
    {
      if (DECL_RTL (var) && GET_CODE (DECL_RTL (var)) == REG)
	{
	  int regno = REGNO (DECL_RTL (var));
	  if ((unsigned)regno >= (unsigned)max_regno)
	    abort ();

	  range_vars[regno].symbol = var;
	  range_vars[regno].block = block;
	}
    }

  /* Loop over all blocks */
  for (; block != NULL_TREE; block = BLOCK_CHAIN (block))
    if (TREE_USED (block))
      {
	/* Record all symbols in the block */
	for (var = BLOCK_VARS (block); var != NULL_TREE; var = TREE_CHAIN (var))
	  {
	    if (DECL_RTL (var) && GET_CODE (DECL_RTL (var)) == REG)
	      {
		int regno = REGNO (DECL_RTL (var));
		if ((unsigned)regno >= (unsigned)max_regno)
		  abort ();

		range_vars[regno].symbol = var;
		range_vars[regno].block = block;
	      }
	  }
      
	/* Record the subblocks, and their subblocks...  */
	range_scan_blocks (BLOCK_SUBBLOCKS (block), NULL_TREE);
      }
}


/* Recalculate the basic blocks due to adding copyins and copyouts.  */

static void
range_update_basic_block (stream, first_insn, new_bb,
			  new_bb_count, range_max_regno)
     FILE *stream;
     rtx first_insn;
     bb_link *new_bb;
     int new_bb_count;
     int range_max_regno;
{
  int i;
  rtx insn;
  rtx range;
  rtx block_end;
  rtx block_start;
  int block;
  int in_block_p;
  int old_n_basic_blocks = n_basic_blocks;
  int *map_bb = (int *) alloca (sizeof (int) * n_basic_blocks);
  regset live = ALLOCA_REG_SET ();

  COPY_REG_SET (live, basic_block_live_at_start[0]);

  /* Go through and add NOTE_INSN_LIVE notes for each current basic block.  */
  for (i = 0; i < old_n_basic_blocks; i++)
    {
      rtx p = emit_note_before (NOTE_INSN_LIVE, BLOCK_HEAD (i));
      NOTE_LIVE_INFO (p) = gen_rtx (RANGE_LIVE, VOIDmode,
				    basic_block_live_at_start[i], i);
      map_bb[i] = -1;

      if (stream)
	{
	  fprintf (stream,
		   "Old basic block #%d, first insn %d, last insn %d, live",
		   i,
		   INSN_UID (BLOCK_HEAD (i)),
		   INSN_UID (BLOCK_END (i)));
	  bitmap_print (stream, basic_block_live_at_start[i], " {", "}\n");
	}
    }

  if (stream)
    putc ('\n', stream);

  /* Recalculate the basic blocks.  */
  free ((char *)x_basic_block_head);
  free ((char *)x_basic_block_end);
  x_basic_block_head = x_basic_block_end = (rtx *)0;
  find_basic_blocks (first_insn, max_regno, stream);
  free_basic_block_vars (TRUE);

  /* Restore the live information.  We assume that flow will find either a
     previous start of a basic block, or the newly created insn blocks as
     the start of the new basic blocks.  */

  if (old_n_basic_blocks != n_basic_blocks)
    basic_block_live_at_start
      = (regset *) oballoc (sizeof (regset) * n_basic_blocks);

  init_regset_vector (basic_block_live_at_start, n_basic_blocks,
		      rtl_obstack);

  block = 0;
  in_block_p = FALSE;
  block_start = BLOCK_HEAD (0);
  block_end = BLOCK_END (0);
  for (insn = first_insn; insn; insn = NEXT_INSN (insn))
    {
      /* If this is the start of a basic block update live information.  */
      if (insn == block_start)
	{
	  int i;
	  in_block_p = TRUE;
	  
	  /* See if this was the start of one of the "new" basic
	     blocks.  If so, get register livetime information from
	     the data we saved when we created the range.  */
	  for (i = 0; i < new_bb_count; i++)
	    if (new_bb[i].first_insn == insn)
	      break;

	  if (i < new_bb_count)
	    COPY_REG_SET (live, new_bb[i].live_at_start);

	  COPY_REG_SET (basic_block_live_at_start[block], live);
	}

      if (GET_CODE (insn) == NOTE)
	{
	  /* Is this where an old basic block began?  */
	  if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LIVE)
	    {
	      rtx info = NOTE_LIVE_INFO (insn);
	      COPY_REG_SET (live, RANGE_LIVE_BITMAP (info));
	      map_bb[RANGE_LIVE_ORIG_BLOCK (info)] = block;
	      FREE_REG_SET (RANGE_LIVE_BITMAP (info));
	      NOTE_LIVE_INFO (insn) = NULL_RTX;
	      if (stream)
		fprintf (stream, "Old basic block #%d is now %d.\n",
			 RANGE_LIVE_ORIG_BLOCK (info), block);
	    }

	  /* If a range start/end, use stored live information.  */
	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_RANGE_START)
	    {
	      rtx ri = NOTE_RANGE_INFO (insn);
	      int old_block = RANGE_INFO_BB_START (ri);

	      RANGE_INFO_BB_START (ri) = block;
	      COPY_REG_SET (live, RANGE_INFO_LIVE_START (ri));
	      if (stream)
		fprintf (stream,
			 "Changing range start basic block from %d to %d\n",
			 old_block, block);
	    }

	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_RANGE_END)
	    {
	      rtx ri = NOTE_RANGE_INFO (insn);
	      int old_block = RANGE_INFO_BB_END (ri);
	      int new_block = (in_block_p) ? block : block-1;

	      RANGE_INFO_BB_END (ri) = new_block;
	      COPY_REG_SET (live, RANGE_INFO_LIVE_END (ri));
	      if (stream)
		fprintf (stream,
			 "Changing range end basic block from %d to %d\n",
			 old_block, new_block);
	    }
	}

      /* Update live/dead information.  */
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
	range_basic_insn (insn, live, range_max_regno);

      /* Advance basic block if at end.  */
      if (insn == block_end)
	{
	  block++;
	  in_block_p = FALSE;
	  if (block < n_basic_blocks)
	    {
	      block_start = BLOCK_HEAD (block);
	      block_end = BLOCK_END (block);
	    }
	  else
	    block_start = block_end = NULL_RTX;
	}
    }

  if (stream)
    {
      putc ('\n', stream);
      fflush (stream);
    }

  /* Update REG_BASIC_BLOCK field now.  */
  for (i = 0; i < max_regno; i++)
    {
      if (REG_BASIC_BLOCK (i) >= 0)
	REG_BASIC_BLOCK (i) = map_bb[REG_BASIC_BLOCK (i)];
    }
}


/* Allocate the new registers for live range splitting.  Do this after we've
   scanned all of the insns, so that we grow the tables only once.  */

static void
range_finish (stream, first_insn, count, range_max_regno)
     FILE *stream;
     rtx first_insn;
     int count;
     int range_max_regno;
{
  rtx range;
  int new_max_regno = max_reg_num () + count;
  rtx *replacements = (rtx *) alloca (new_max_regno * sizeof (rtx));
  regset old_dead = ALLOCA_REG_SET ();
  regset new_live = ALLOCA_REG_SET ();
  regset copyouts = ALLOCA_REG_SET ();
  rtx insn;
  int i;
  tree *block_list		/* set up NOTE_BLOCK_NUM field */
    = ((write_symbols == NO_DEBUG)
       ? (tree *)0
       : identify_blocks (DECL_INITIAL (current_function_decl), first_insn));

  bb_link *new_bb;
  int new_bb_count = 0;
  int max_bb_count = 0;

  no_new_pseudos = 0;
  for (range = live_range_list; range; range = XEXP (range, 1))
    max_bb_count += 2;

  new_bb = (bb_link *) alloca (sizeof (bb_link) * max_bb_count);
  bzero ((char *)new_bb, sizeof (bb_link) * max_bb_count);

  if (stream)
    putc ('\n', stream);

  /* Grow the register tables */
  allocate_reg_info (new_max_regno, FALSE, FALSE);

  for (range = live_range_list; range; range = XEXP (range, 1))
    {
      rtx range_start = XEXP (range, 0);
      rtx ri = NOTE_RANGE_INFO (range_start);
      rtx range_end = RANGE_INFO_NOTE_END (ri);
      int bb_start = RANGE_INFO_BB_START (ri);
      int bb_end = RANGE_INFO_BB_END (ri);
      int bb = (bb_start >= 0 && bb_start == bb_end) ? bb_start : -2;
      rtx after;
      rtx before;
      int new_scope_p = (write_symbols != NO_DEBUG	/* create new scope */
			 && flag_live_range_scope	/* block for vars? */
			 && (write_symbols != DBX_DEBUG
			     || !LIVE_RANGE_GDBSTAB_P ()));

      after = range_start;
      before = range_end;

      bzero ((char *)replacements, new_max_regno * sizeof (rtx));
      CLEAR_REG_SET (old_dead);
      CLEAR_REG_SET (copyouts);

      /* Allocate new registers, set up the fields needed.  */
      for (i = 0; i < (int)RANGE_INFO_NUM_REGS (ri); i++)
	{
	  int old_regno = RANGE_REG_PSEUDO (ri, i);
	  rtx old_reg = regno_reg_rtx[old_regno];
	  enum machine_mode mode = GET_MODE (old_reg);
	  rtx new_reg = gen_reg_rtx (mode);
	  int new_regno = REGNO (new_reg);

	  replacements[old_regno] = new_reg;

	  RANGE_REG_COPY (ri, i) = new_regno;
	  REG_N_RANGE_COPY_P (new_regno) = TRUE;
	  REG_N_REFS (new_regno) = RANGE_REG_REFS (ri, i);
	  REG_N_SETS (new_regno) = RANGE_REG_SETS (ri, i);
	  REG_N_DEATHS (new_regno) = RANGE_REG_DEATHS (ri, i);
	  REG_LIVE_LENGTH (new_regno) = RANGE_REG_LIVE_LENGTH (ri, i);
	  REG_N_CALLS_CROSSED (new_regno) = RANGE_REG_N_CALLS (ri, i);
	  REG_CHANGES_SIZE (new_regno) = 0;
	  REG_BASIC_BLOCK (new_regno) = bb;
	  REGNO_POINTER_FLAG (new_regno) = REGNO_POINTER_FLAG (old_regno);
	  REGNO_POINTER_ALIGN (new_regno) = REGNO_POINTER_ALIGN (old_regno);
	  REG_FUNCTION_VALUE_P (new_reg) = REG_FUNCTION_VALUE_P (old_reg);
	  REG_USERVAR_P (new_reg) = REG_USERVAR_P (old_reg);
	  REG_LOOP_TEST_P (new_reg) = REG_LOOP_TEST_P (old_reg);
	  RTX_UNCHANGING_P (new_reg) = RTX_UNCHANGING_P (old_reg);

#if 0
	  /* Until we can make sure we get this right, don't update the
	     reference counts on the old register.  */
	  REG_N_REFS (old_regno) -= REG_N_REFS (new_regno);
	  REG_N_SETS (old_regno) -= REG_N_SETS (new_regno);
	  REG_N_DEATHS (old_regno) -= REG_N_DEATHS (new_regno);
	  REG_N_CALLS_CROSSED (old_regno) -= REG_N_CALLS_CROSSED (new_regno);
	  REG_LIVE_LENGTH (old_regno) -= REG_LIVE_LENGTH (new_regno);

	  if (REG_N_REFS (old_regno) <= 0)
	    error ("Setting # references of register %d to %d\n",
		   old_regno, REG_N_REFS (old_regno));

	  if (REG_N_SETS (old_regno) < 0)
	    error ("Setting # sets of register %d to %d\n",
		   old_regno, REG_N_SETS (old_regno));

	  if (REG_N_CALLS_CROSSED (old_regno) < 0)
	    error ("Setting # calls crossed of register %d to %d\n",
		   old_regno, REG_N_CALLS_CROSSED (old_regno));

	  if (REG_N_DEATHS (old_regno) < 0)
	    error ("Setting # deaths of register %d to %d\n",
		   old_regno, REG_N_SETS (old_regno));

	  if (REG_LIVE_LENGTH (old_regno) <= 0)
	    error ("Setting live length of register %d to %d\n",
		   old_regno, REG_LIVE_LENGTH (old_regno));
#endif

	  SET_REGNO_REG_SET (old_dead, old_regno);

	  /* If this is a user variable, add the range into the list of
	     different ranges the variable spans.  */
	  if (RANGE_REG_SYMBOL_NODE (ri, i))
	    {
	      tree sym = RANGE_REG_SYMBOL_NODE (ri, i);
	      rtx var = DECL_LIVE_RANGE_RTL (sym);

	      if (!var)
		DECL_LIVE_RANGE_RTL (sym) = var
		  = gen_rtx (RANGE_VAR, VOIDmode, NULL_RTX,
			     RANGE_REG_BLOCK_NODE (ri, i), 0);

	      RANGE_VAR_NUM (var)++;
	      RANGE_VAR_LIST (var) = gen_rtx (EXPR_LIST, VOIDmode, ri,
					      RANGE_VAR_LIST (var));
	    }

#if 0
	  /* global.c implements regs_may_share as requiring the registers
	     to share the same hard register.  */
	  regs_may_share = gen_rtx (EXPR_LIST, VOIDmode, old_reg,
				    gen_rtx (EXPR_LIST, VOIDmode, new_reg,
					     regs_may_share));
#endif

	  /* Create a new scoping block for debug information if needed.  */
	  if (new_scope_p
	      && RANGE_REG_SYMBOL_NODE (ri, i) != NULL_TREE
	      && RANGE_REG_BLOCK_NODE (ri, i) != NULL_TREE)
	    {
	      new_scope_p = FALSE;
	      range_start = emit_note_after (NOTE_INSN_BLOCK_BEG, range_start);
	      NOTE_BLOCK_NUMBER (range_start) = NOTE_BLOCK_LIVE_RANGE_BLOCK;

	      range_end = emit_note_before (NOTE_INSN_BLOCK_END, range_end);
	      NOTE_BLOCK_NUMBER (range_end) = NOTE_BLOCK_LIVE_RANGE_BLOCK;

	      if (stream)
		fprintf (stream, "Creating new scoping block\n");
	    }

	  /* If needed, generate the appropriate copies into and out of
	     the new register.  Tell global.c that we want to share registers
	     if possible.  Since we might be creating a new basic block
	     for the copyin or copyout, tell local alloc to keep its grubby
	     paws off of the registers that need copies.  */
	  if ((RANGE_REG_COPY_FLAGS (ri, i) & LIVE_RANGE_COPYIN) != 0)
	    {
	      after
		= emit_insn_after (GEN_FCN (mov_optab->handlers[(int) mode].insn_code)
				   (new_reg, old_reg), after);

	      RANGE_REG_LIVE_LENGTH (ri, i)++;
	      REG_LIVE_LENGTH (old_regno)++;
	      REG_LIVE_LENGTH (new_regno)++;
	      REG_N_REFS (old_regno)++;
	      REG_N_REFS (new_regno)++;
	      REG_N_SETS (new_regno)++;
	      REG_N_DEATHS (old_regno)++;
	      REG_BASIC_BLOCK (new_regno) = REG_BLOCK_GLOBAL;
	      REG_NOTES (after) = gen_rtx (EXPR_LIST, REG_EQUAL /* REG_EQUIV */, old_reg,
					   gen_rtx (EXPR_LIST, REG_DEAD,
						    old_reg,
						    REG_NOTES (after)));
	    }

	  if ((RANGE_REG_COPY_FLAGS (ri, i) & LIVE_RANGE_COPYOUT) != 0)
	    {
	      before
		= emit_insn_before (GEN_FCN (mov_optab->handlers[(int) mode].insn_code)
				    (old_reg, new_reg), before);

	      RANGE_REG_LIVE_LENGTH (ri, i)++;
	      REG_LIVE_LENGTH (old_regno)++;
	      REG_LIVE_LENGTH (new_regno)++;
	      REG_N_REFS (old_regno)++;
	      REG_N_REFS (new_regno)++;
	      REG_N_SETS (old_regno)++;
	      REG_N_DEATHS (new_regno)++;
	      REG_BASIC_BLOCK (new_regno) = REG_BLOCK_GLOBAL;
	      REG_NOTES (before) = gen_rtx (EXPR_LIST, REG_DEAD, new_reg,
					    REG_NOTES (before));
	      SET_REGNO_REG_SET (copyouts, new_regno);
	    }
	}

      /* Add insns created for copyins to new basic block list, if new copyins
	 were created, and the insns aren't already part of a basic block.  */
      if (range_start != after)
	{
	  int in_bb_p;
	  rtx end;

	  /* If the insns created are the first, add them to the beginning
	     of the basic block.  */
	  if (bb_start == 0)
	    {
	      rtx temp = get_insns ();
	      /* Search forward until we hit a CODE_LABEL or real insn.  */
	      while (! (GET_CODE (temp) == CODE_LABEL
		        || GET_RTX_CLASS (GET_CODE (temp)) == 'i'))
		temp = NEXT_INSN (temp);
	      BLOCK_HEAD (0) = temp;
	    }


	  /* Check if the insns are already in the basic block */
	  in_bb_p = FALSE;
	  end = BLOCK_END (bb_start);
	  for (insn = BLOCK_HEAD (bb_start);
	       insn && insn != end;
	       insn = NEXT_INSN (insn))
	    {
	      if (insn == after)
		{
		  in_bb_p = TRUE;
		  break;
		}
	    }

	  /* If needed, create a new basic block.  */
	  if (!in_bb_p)
	    {
	      bb_link *p = &new_bb[new_bb_count++];
	      p->first_insn = NEXT_INSN (range_start);
	      p->last_insn = after;
	      p->live_at_start = RANGE_INFO_LIVE_START (ri);
	    }
	}

      /* Add insns created for copyouts to new basic block list, if new
	 copyouts were created, and the insns aren't already part of a basic
	 block, or can be added to a basic block trivally.  */
      if (range_end != before)
	{
	  int in_bb_p = FALSE;
	  rtx end = BLOCK_END (bb_end);

	  /* Check if the insns are already in the basic block */
	  for (insn = BLOCK_HEAD (bb_end);
	       insn && insn != end;
	       insn = NEXT_INSN (insn))
	    {
	      if (insn == before)
		{
		  in_bb_p = TRUE;
		  break;
		}
	    }

	  /* If needed, create a new basic block.  */
	  if (!in_bb_p)
	    {
	      bb_link *p = &new_bb[new_bb_count++];
	      p->first_insn = before;
	      p->last_insn = PREV_INSN (range_end);
	      p->live_at_start = OBSTACK_ALLOC_REG_SET (rtl_obstack);
	      IOR_REG_SET (p->live_at_start, RANGE_INFO_LIVE_END (ri));
	      IOR_REG_SET (p->live_at_start, copyouts);
	      AND_COMPL_REG_SET (p->live_at_start, old_dead);
	    }
	}

      /* Replace the registers */
      for (insn = NEXT_INSN (after);
	   insn != NULL_RTX && insn != before;
	   insn = NEXT_INSN (insn))
	{
	  enum rtx_code code = GET_CODE (insn);

	  if (GET_RTX_CLASS (code) == 'i')
	    {
	      rtx note;
	      PATTERN (insn) = replace_regs (PATTERN (insn), replacements,
					     new_max_regno, TRUE);

	      for (note = REG_NOTES (insn);
		   note != NULL_RTX;
		   note = XEXP (note, 1))
		{
		  if ((REG_NOTE_KIND (note) == REG_DEAD
		       || REG_NOTE_KIND (note) == REG_UNUSED)
		      && GET_CODE (XEXP (note, 0)) == REG
		      && replacements[ REGNO (XEXP (note, 0))] != NULL_RTX)
		    XEXP (note, 0) = replacements[ REGNO (XEXP (note, 0))];
		}
	    }
	}
    }

  if (stream)
    fflush (stream);

  /* Update # registers */
  max_regno = new_max_regno;

  /* Recalculate basic blocks if we need to.  */
  if (new_bb_count)
    range_update_basic_block (stream, first_insn, new_bb,
			      new_bb_count, range_max_regno);

  /* After recreating the basic block, update the live information,
     replacing the old registers with the new copies.  */
  for (range = live_range_list; range; range = XEXP (range, 1))
    {
      rtx range_start = XEXP (range, 0);
      rtx ri = NOTE_RANGE_INFO (range_start);
      int bb_start = RANGE_INFO_BB_START (ri);
      int bb_end = RANGE_INFO_BB_END (ri);
      int block;

      bzero ((char *)replacements, new_max_regno * sizeof (rtx));
      CLEAR_REG_SET (old_dead);
      for (i = 0; i < (int)RANGE_INFO_NUM_REGS (ri); i++)
	{
	  int old_regno = RANGE_REG_PSEUDO (ri, i);
	  int new_regno = RANGE_REG_COPY (ri, i);
	  if (new_regno >= 0)
	    {
	      replacements[old_regno] = regno_reg_rtx[new_regno];
	      SET_REGNO_REG_SET (old_dead, old_regno);
	    }
	}

      /* Update live information */
      for (block = bb_start+1; block <= bb_end; block++)
	{
	  regset bits = basic_block_live_at_start[block];

	  CLEAR_REG_SET (new_live);
	  EXECUTE_IF_AND_IN_REG_SET (bits, old_dead,
				     FIRST_PSEUDO_REGISTER, i,
				     {
				       int n = REGNO (replacements[i]);
				       SET_REGNO_REG_SET (new_live, n);
				     });

	  AND_COMPL_REG_SET (bits, old_dead);
	  IOR_REG_SET (bits, new_live);
	  basic_block_live_at_start[block] = bits;
	}

      if (stream)
	{
	  putc ('\n', stream);
	  live_range_print (stream, ri, "::", "");
	}
    }

  /* Add new scoping blocks and reset NOTE_BLOCK_NUMBER field.  */
  if (write_symbols != NO_DEBUG)
    {
      reorder_blocks (block_list, DECL_INITIAL (current_function_decl),
		      first_insn);
      free ((char *)block_list);
    }

  /* Release any storage allocated */
  FREE_REG_SET (old_dead);
  FREE_REG_SET (new_live);
  FREE_REG_SET (copyouts);

  if (stream)
    {
      putc ('\n', stream);
      print_blocks_internal (stream, DECL_INITIAL (current_function_decl), 0);
      putc ('\n', stream);
      fflush (stream);
    }
  no_new_pseudos = 1;
}

/* Main function for live_range support. Return the number of variables that
   were spilled to the stack were used in small loops and were copied into new
   pseudo registers for the run of that loop.  Since we are run after
   flow_analysis and local_alloc, we have to set up the appropriate tables for
   any new pseudo variables we create.  */

int
live_range (first_insn, stream)
     rtx first_insn;
     FILE *stream;
{
  rtx insn;
  rtx prev;
  rtx next;
  rtx loop_start = NULL_RTX;
  rtx loop_prefix = NULL_RTX;
  int count = 0;
  int i;
  int basic_block;
  int user_block;
  int loop_user_block;
  int loop_depth;
  int range_max_regno;
  regset live;
  regset simple_insns_live;
  regset loop_live;
  rtx simple_insns = NULL_RTX;
  int *insn_ruid;
  int ruid;

  struct skip_flags {
    int *flag;
    char *reason;
  };

  /* Awkward cases we don't want to handle.  */
  static struct skip_flags skip[] = {
    { &current_function_has_nonlocal_label,	"nonlocal label" },
    { &current_function_has_nonlocal_goto,	"nonlocal goto" },
    { &current_function_calls_setjmp,		"calls setjmp" },
    { &current_function_calls_longjmp,		"calls longjmp" }
  };

  for (i = 0; i < (int)(sizeof (skip) / sizeof (skip[0])); i++)
    if (*skip[i].flag)
      {
	if (stream)
	  fprintf (stream, "Function has %s, skipping live range splitting\n",
		   skip[i].reason);

	return 0;
      }

  if (n_basic_blocks <= 0)
    {
      if (stream)
	fprintf (stream, "Function has no more than 1 basic block, skipping live range splitting\n");

      return 0;
    }

  live_range_list = NULL_RTX;
  range_set = ALLOCA_REG_SET ();
  range_used = ALLOCA_REG_SET ();
  range_mixed_mode = ALLOCA_REG_SET ();
  range_no_move = ALLOCA_REG_SET ();
  range_live = ALLOCA_REG_SET ();
  live = ALLOCA_REG_SET ();
  simple_insns_live = ALLOCA_REG_SET ();
  loop_live = ALLOCA_REG_SET ();
  range_max_regno = max_regno;
  range_info = (rinfo *) alloca (sizeof (rinfo) * max_regno);
  range_regs = (int *) alloca (sizeof (int) * max_regno);
  range_max_uid = (unsigned)get_max_uid ();
  range_vars = (var_info *) alloca (sizeof (var_info) * max_regno);
  range_block_insn = (bb_link **) alloca (sizeof (bb_link *) * range_max_uid);
  range_block_orig = (bb_link *) alloca (sizeof (bb_link) * n_basic_blocks);
  bzero ((char *)range_vars, sizeof (var_info) * max_regno);
  bzero ((char *)range_block_insn, sizeof (bb_link *) * range_max_uid);
  bzero ((char *)range_block_orig, sizeof (bb_link) * n_basic_blocks);

  /* Figure out which basic block things are in.  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      rtx end = BLOCK_END (i);
      rtx head = BLOCK_HEAD (i);
      range_block_orig[i].block = i;
      range_block_orig[i].first_insn = head;
      range_block_orig[i].last_insn = end;
      range_block_orig[i].live_at_start = basic_block_live_at_start[i];

      range_block_insn[INSN_UID (end)] = &range_block_orig[i];
      for (insn = head; insn && insn != end; insn = NEXT_INSN (insn))
	range_block_insn[INSN_UID (insn)] = &range_block_orig[i];
    }
      
  /* Map user variables to their pseudo register */
  range_scan_blocks (DECL_INITIAL (current_function_decl),
		     DECL_ARGUMENTS (current_function_decl));

  /* Search for inner loops that do not span logical block boundaries.
     Include an non-jump INSNs before the loop to include any setup for the
     loop that is not included within the LOOP_BEG note.  */
  basic_block = loop_user_block = loop_depth = 0;
  user_block = 0;
  COPY_REG_SET (live, basic_block_live_at_start[0]);

  insn_ruid = (int *) alloca ((range_max_uid + 1) * sizeof (int));
  bzero ((char *) insn_ruid, (range_max_uid + 1) * sizeof (int));
  ruid = 0;

  for (insn = first_insn; insn; insn = NEXT_INSN (insn))
    {
      int block_tmp;
      enum rtx_code code = GET_CODE (insn);

      /* This might be a note insn emitted by range_inner, in which case we
	 can't put it in insn_ruid because that will give an out-of-range
	 array access.  Since we only use it for JUMP_INSNs this should be
	 OK.  */
	 
      if (INSN_UID (insn) <= range_max_uid)
	insn_ruid[INSN_UID (insn)] = ++ruid;

      /* If this is a different basic block, update live variables.  */
      block_tmp = RANGE_BLOCK_NUM (insn);
      if (block_tmp >= 0 && basic_block != block_tmp)
	{
	  basic_block = block_tmp;
	  COPY_REG_SET (live, basic_block_live_at_start[basic_block]);
	}

      /* Keep track of liveness for simple insns that might preceed LOOP_BEG */
      if (GET_CODE (insn) == INSN && simple_insns == NULL_RTX)
	{
#ifdef HAVE_cc0
	  if (reg_referenced_p (cc0_rtx, PATTERN (insn)))
	    simple_insns = NULL_RTX;
	  else
#endif
	    {
	      simple_insns = insn;
	      COPY_REG_SET (simple_insns_live, live);
	    }
	}
      else if (GET_CODE (insn) != INSN)
	{
	  /* Allow simple notes to not zap the simple_insns block */
	  if (GET_CODE (insn) != NOTE
	      || (NOTE_LINE_NUMBER (insn) < 0
		  && NOTE_LINE_NUMBER (insn) != NOTE_INSN_LOOP_BEG
		  && NOTE_LINE_NUMBER (insn) != NOTE_INSN_DELETED))
	    simple_insns = NULL_RTX;
	}

      /* Update live/dead information.  */
      if (GET_RTX_CLASS (code) == 'i')
	range_basic_insn (insn, live, range_max_regno);

      /* Look for inner loops */
      else if (code == NOTE)
	{
	  if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	    {
	      /* Add simple insns that occur before the loop begins */
	      if (simple_insns)
		{
		  loop_prefix = simple_insns;
		  COPY_REG_SET (loop_live, simple_insns_live);
		}
	      else
		{
		  loop_prefix = insn;
		  COPY_REG_SET (loop_live, live);
		}

	      loop_start = insn;
	      loop_user_block = user_block;
	      loop_depth++;
	    }

	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
	    {
	      loop_depth--;
	      if (loop_start && loop_user_block == user_block)
		{
		  rtx scan_start;

		  /* See whether a new basic block begins with the
		     next insn -- if so, use its live information.  */
		  rtx follow_insn, p;
		  for (follow_insn = NEXT_INSN (insn);
		       (follow_insn
			&& ((block_tmp = RANGE_BLOCK_NUM (follow_insn)) < 0
			    || block_tmp == basic_block)
			&& GET_CODE (follow_insn) == NOTE);
		       follow_insn = NEXT_INSN (follow_insn))
		    ;

		  if (!follow_insn)
		    CLEAR_REG_SET (live);
		  else if (block_tmp >= 0 && block_tmp != basic_block)
		    COPY_REG_SET (live, basic_block_live_at_start[block_tmp]);

		  /* Do not create live ranges for phony loops.  The code to
		     detect phony loops was mostly lifted from scan_loop. 

		     Try to find the label for the start of the loop.  */
		  for (p = NEXT_INSN (loop_start);
		       (p != insn
			&& GET_CODE (p) != CODE_LABEL
			&& GET_RTX_CLASS (GET_CODE (p)) != 'i'
			&& (GET_CODE (p) != NOTE
			    || (NOTE_LINE_NUMBER (p) != NOTE_INSN_LOOP_BEG
				&& NOTE_LINE_NUMBER (p) != NOTE_INSN_LOOP_END)));
		      p = NEXT_INSN (p))
		    ;

		  scan_start = p;

		  /* Detect a jump to the bottom of the loop.  */
		  if (GET_CODE (p) == JUMP_INSN)
		    {
		      if (simplejump_p (p)
			  && JUMP_LABEL (p) != 0
		          /* Check to see whether the jump actually
			     jumps out of the loop (meaning it's no loop).
			     This case can happen for things like do
			     do {..} while (0).  */
			  && insn_ruid[INSN_UID (JUMP_LABEL (p))] > 0
			  && (insn_ruid[INSN_UID (loop_start)]
			      <= insn_ruid[INSN_UID (JUMP_LABEL (p))])
			  && (insn_ruid[INSN_UID (insn)]
			      >= insn_ruid[INSN_UID (JUMP_LABEL (p))]))
			scan_start = JUMP_LABEL (p);
		    }

		  /* If we did not find the CODE_LABEL for the start of this
		     loop, then we either have a phony loop or something very
		     strange has happened.  Do not perform LRS opts on such
		     a loop.  */
		  if (GET_CODE (scan_start) == CODE_LABEL)
		    count += range_inner (stream, loop_prefix, insn, loop_start,
					  insn, loop_live, live, loop_depth);
   		}

	      loop_start = NULL_RTX;
	      loop_prefix = NULL_RTX;
	    }

	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_BLOCK_BEG)
	    user_block++;

	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_BLOCK_END)
	    user_block--;
	}
    }

  /* Reverse list of live ranges so that it goes forwards, not backwards.  */
  prev = next = NULL_RTX;
  for (insn = live_range_list; insn != NULL_RTX; insn  = next)
    {
      next = XEXP (insn, 1);
      XEXP (insn, 1) = prev;
      prev = insn;
    }
  live_range_list = prev;

  /* If we discovered any live ranges, create them now */
  if (count)
    range_finish (stream, first_insn, count, range_max_regno);

  FREE_REG_SET (range_set);
  FREE_REG_SET (range_used);
  FREE_REG_SET (range_live);
  FREE_REG_SET (range_mixed_mode);
  FREE_REG_SET (range_no_move);
  FREE_REG_SET (simple_insns_live);
  FREE_REG_SET (loop_live);
  FREE_REG_SET (live);
  range_block_insn = (bb_link **)0;
  range_block_orig = (bb_link *)0;
  range_info = (rinfo *)0;
  range_regs = (int *)0;

  return count;
}


/* Initialize live_range information */

void
init_live_range ()
{
  live_range_list = NULL_RTX;
}
/* END CYGNUS LOCAL */
