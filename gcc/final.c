/* Convert RTL to assembler code and output it, for GNU compiler.
   Copyright (C) 1987, 88, 89, 92-97, 1998 Free Software Foundation, Inc.

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


/* This is the final pass of the compiler.
   It looks at the rtl code for a function and outputs assembler code.

   Call `final_start_function' to output the assembler code for function entry,
   `final' to output assembler code for some RTL code,
   `final_end_function' to output assembler code for function exit.
   If a function is compiled in several pieces, each piece is
   output separately with `final'.

   Some optimizations are also done at this level.
   Move instructions that were made unnecessary by good register allocation
   are detected and omitted from the output.  (Though most of these
   are removed by the last jump pass.)

   Instructions to set the condition codes are omitted when it can be
   seen that the condition codes already had the desired values.

   In some cases it is sufficient if the inherited condition codes
   have related values, but this may require the following insn
   (the one that tests the condition codes) to be modified.

   The code for the function prologue and epilogue are generated
   directly as assembler code by the macros FUNCTION_PROLOGUE and
   FUNCTION_EPILOGUE.  Those instructions never exist as rtl.  */

#include "config.h"
#include "system.h"

#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "insn-config.h"
#include "insn-flags.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "recog.h"
#include "conditions.h"
#include "flags.h"
#include "real.h"
#include "hard-reg-set.h"
#include "output.h"
#include "except.h"
#include "toplev.h"
#include "reload.h"
/* CYGNUS LOCAL LRS */
#include "function.h"
#include "bitmap.h"
#include "obstack.h"

extern struct obstack *rtl_obstack;
/* END CYGNUS LOCAL */

#if defined (DWARF2_DEBUGGING_INFO)
#include "dwarf2out.h"
#endif

#ifndef INT_TYPE_SIZE
#define INT_TYPE_SIZE BITS_PER_WORD
#endif

#ifndef LONG_TYPE_SIZE
#define LONG_TYPE_SIZE BITS_PER_WORD
#endif

/* If we aren't using cc0, CC_STATUS_INIT shouldn't exist.  So define a
   null default for it to save conditionalization later.  */
#ifndef CC_STATUS_INIT
#define CC_STATUS_INIT
#endif

/* How to start an assembler comment.  */
#ifndef ASM_COMMENT_START
#define ASM_COMMENT_START ";#"
#endif

/* Is the given character a logical line separator for the assembler?  */
#ifndef IS_ASM_LOGICAL_LINE_SEPARATOR
#define IS_ASM_LOGICAL_LINE_SEPARATOR(C) ((C) == ';')
#endif

#ifndef JUMP_TABLES_IN_TEXT_SECTION
#define JUMP_TABLES_IN_TEXT_SECTION 0
#endif

/* Last insn processed by final_scan_insn.  */
static rtx debug_insn = 0;

/* Line number of last NOTE.  */
static int last_linenum;

/* Highest line number in current block.  */
static int high_block_linenum;

/* Likewise for function.  */
static int high_function_linenum;

/* Filename of last NOTE.  */
static char *last_filename;

extern int length_unit_log; /* This is defined in insn-attrtab.c.  */

/* Nonzero while outputting an `asm' with operands.
   This means that inconsistencies are the user's fault, so don't abort.
   The precise value is the insn being output, to pass to error_for_asm.  */
static rtx this_is_asm_operands;

/* Number of operands of this insn, for an `asm' with operands.  */
static unsigned int insn_noperands;

/* Compare optimization flag.  */

static rtx last_ignored_compare = 0;

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

int next_block_index;

/* CYGNUS LOCAL LRS */
/* Map block # into block nodes during final */
tree *block_nodes;
/* END CYGNUS LOCAL */

/* Assign a unique number to each insn that is output.
   This can be used to generate unique local labels.  */

static int insn_counter = 0;

#ifdef HAVE_cc0
/* This variable contains machine-dependent flags (defined in tm.h)
   set and examined by output routines
   that describe how to interpret the condition codes properly.  */

CC_STATUS cc_status;

/* During output of an insn, this contains a copy of cc_status
   from before the insn.  */

CC_STATUS cc_prev_status;
#endif

/* Indexed by hardware reg number, is 1 if that register is ever
   used in the current function.

   In life_analysis, or in stupid_life_analysis, this is set
   up to record the hard regs used explicitly.  Reload adds
   in the hard regs used for holding pseudo regs.  Final uses
   it to generate the code in the function prologue and epilogue
   to save and restore registers as needed.  */

char regs_ever_live[FIRST_PSEUDO_REGISTER];

/* Nonzero means current function must be given a frame pointer.
   Set in stmt.c if anything is allocated on the stack there.
   Set in reload1.c if anything is allocated on the stack there.  */

int frame_pointer_needed;

/* Length so far allocated in PENDING_BLOCKS.  */

static int max_block_depth;

/* CYGNUS LOCAL LRS */
/* Stack of sequence numbers of symbol-blocks of which we have seen the
   beginning but not yet the end.  Sequence numbers are assigned at
   the beginning; this stack allows us to find the sequence number
   of a block that is ending.  */

struct block_seq {
  int number;
  tree block;
};

static struct block_seq *pending_blocks;
/* END CYGNUS LOCAL */

/* Number of elements currently in use in PENDING_BLOCKS.  */

static int block_depth;

/* Nonzero if have enabled APP processing of our assembler output.  */

static int app_on;

/* If we are outputting an insn sequence, this contains the sequence rtx.
   Zero otherwise.  */

rtx final_sequence;


/* Indexed by line number, nonzero if there is a note for that line.  */

static char *line_note_exists;

/* CYGNUS LOCAL LRS */
/* Current marker number for live ranges.  */
extern int range_max_number;
/* END CYGNUS LOCAL */

extern rtx peephole		(rtx);

#ifdef HAVE_ATTR_length
static int asm_insn_count	(rtx);
#endif
static void output_source_line	(FILE *, rtx);
static rtx walk_alter_subreg	(rtx);
static void output_asm_name	(void);
static void output_operand	(rtx, int);
#ifdef LEAF_REGISTERS
static void leaf_renumber_regs	(rtx);
#endif
#ifdef HAVE_cc0
static int alter_cond		(rtx);
#endif

extern char *getpwd ();

/* Initialize data in final at the beginning of a compilation.  */

void
init_final (filename)
     char *filename;
{
  next_block_index = 2;
  app_on = 0;
  max_block_depth = 20;
  /* CYGNUS LOCAL LRS */
  pending_blocks
    = (struct block_seq *) xmalloc (20 * sizeof (struct block_seq));
  /* END CYGNUS LOCAL */
  final_sequence = 0;

}

/* Called at end of source file,
   to output the block-profiling table for this entire compilation.  */

void
end_final (filename)
     char *filename;
{
}

/* Enable APP processing of subsequent output.
   Used before the output from an `asm' statement.  */

void
app_enable ()
{
  if (! app_on)
    {
      fputs (ASM_APP_ON, asm_out_file);
      app_on = 1;
    }
}

/* Disable APP processing of subsequent output.
   Called from varasm.c before most kinds of output.  */

void
app_disable ()
{
  if (app_on)
    {
      fputs (ASM_APP_OFF, asm_out_file);
      app_on = 0;
    }
}

/* The next two pages contain routines used to compute the length of an insn
   and to shorten branches.  */

/* Arrays for insn lengths, and addresses.  The latter is referenced by
   `insn_current_length'.  */

static short *insn_lengths;
int *insn_addresses;

/* Max uid for which the above arrays are valid.  */
static int insn_lengths_max_uid;

/* Address of insn being processed.  Used by `insn_current_length'.  */
int insn_current_address;

/* Address of insn being processed in previous iteration.  */
int insn_last_address;

/* konwn invariant alignment of insn being processed.  */
int insn_current_align;

/* After shorten_branches, for any insn, uid_align[INSN_UID (insn)]
   gives the next following alignment insn that increases the known
   alignment, or NULL_RTX if there is no such insn.
   For any alignment obtained this way, we can again index uid_align with
   its uid to obtain the next following align that in turn increases the
   alignment, till we reach NULL_RTX; the sequence obtained this way
   for each insn we'll call the alignment chain of this insn in the following
   comments.  */

struct label_alignment {
  short alignment;
  short max_skip;
};

static rtx *uid_align;
static int *uid_shuid;
static struct label_alignment *label_align;

static void
print_wint(FILE *file, HOST_WIDE_INT value)
{
  const char *fmt = HOST_WIDE_INT_PRINT_DEC;
  if (flag_hex_asm)
    {
      fmt = HOST_WIDE_INT_PRINT_HEX;
      if (value < 0)
        {
          fputc('-', file);
          value = -value;
        }
    }
  fprintf(file, fmt, value);
}

/* Indicate that branch shortening hasn't yet been done.  */

void
init_insn_lengths ()
{
  if (label_align)
    {
      free (label_align);
      label_align = 0;
    }
  if (uid_shuid)
    {
      free (uid_shuid);
      uid_shuid = 0;
    }
  if (insn_lengths)
    {
      free (insn_lengths);
      insn_lengths = 0;
      insn_lengths_max_uid = 0;
    }
  if (insn_addresses)
    {
      free (insn_addresses);
      insn_addresses = 0;
    }
  if (uid_align)
    {
      free (uid_align);
      uid_align = 0;
    }
}

/* Obtain the current length of an insn.  If branch shortening has been done,
   get its actual length.  Otherwise, get its maximum length.  */

int
get_attr_length (insn)
     rtx insn;
{
#ifdef HAVE_ATTR_length
  rtx body;
  int i;
  int length = 0;

  if (insn_lengths_max_uid > INSN_UID (insn))
    return insn_lengths[INSN_UID (insn)];
  else
    switch (GET_CODE (insn))
      {
      case NOTE:
      case BARRIER:
      case CODE_LABEL:
	return 0;

      case CALL_INSN:
	length = insn_default_length (insn);
	break;

      case JUMP_INSN:
	body = PATTERN (insn);
        if (GET_CODE (body) == ADDR_VEC || GET_CODE (body) == ADDR_DIFF_VEC)
	  {
	    /* Alignment is machine-dependent and should be handled by
	       ADDR_VEC_ALIGN.  */
	  }
	else
	  length = insn_default_length (insn);
	break;

      case INSN:
	body = PATTERN (insn);
	if (GET_CODE (body) == USE || GET_CODE (body) == CLOBBER)
	  return 0;

	else if (GET_CODE (body) == ASM_INPUT || asm_noperands (body) >= 0)
	  length = asm_insn_count (body) * insn_default_length (insn);
	else if (GET_CODE (body) == SEQUENCE)
	  for (i = 0; i < XVECLEN (body, 0); i++)
	    length += get_attr_length (XVECEXP (body, 0, i));
	else
	  length = insn_default_length (insn);
	break;

      default:
	break;
      }

  return length;
#else /* not HAVE_ATTR_length */
  return 0;
#endif /* not HAVE_ATTR_length */
}

/* Code to handle alignment inside shorten_branches.  */

/* Here is an explanation how the algorithm in align_fuzz can give
   proper results:

   Call a sequence of instructions beginning with alignment point X
   and continuing until the next alignment point `block X'.  When `X'
   is used in an expression, it means the alignment value of the
   alignment point.

   Call the distance between the start of the first insn of block X, and
   the end of the last insn of block X `IX', for the `inner size of X'.
   This is clearly the sum of the instruction lengths.

   Likewise with the next alignment-delimited block following X, which we
   shall call block Y.

   Call the distance between the start of the first insn of block X, and
   the start of the first insn of block Y `OX', for the `outer size of X'.

   The estimated padding is then OX - IX.

   OX can be safely estimated as

           if (X >= Y)
                   OX = round_up(IX, Y)
           else
                   OX = round_up(IX, X) + Y - X

   Clearly est(IX) >= real(IX), because that only depends on the
   instruction lengths, and those being overestimated is a given.

   Clearly round_up(foo, Z) >= round_up(bar, Z) if foo >= bar, so
   we needn't worry about that when thinking about OX.

   When X >= Y, the alignment provided by Y adds no uncertainty factor
   for branch ranges starting before X, so we can just round what we have.
   But when X < Y, we don't know anything about the, so to speak,
   `middle bits', so we have to assume the worst when aligning up from an
   address mod X to one mod Y, which is Y - X.  */

#ifndef LABEL_ALIGN
#define LABEL_ALIGN(LABEL) 0
#endif

#ifndef LABEL_ALIGN_MAX_SKIP
#define LABEL_ALIGN_MAX_SKIP 0
#endif

#ifndef LOOP_ALIGN
#define LOOP_ALIGN(LABEL) 0
#endif

#ifndef LOOP_ALIGN_MAX_SKIP
#define LOOP_ALIGN_MAX_SKIP 0
#endif

#ifndef LABEL_ALIGN_AFTER_BARRIER
#define LABEL_ALIGN_AFTER_BARRIER(LABEL) 0
#endif

#ifndef LABEL_ALIGN_AFTER_BARRIER_MAX_SKIP
#define LABEL_ALIGN_AFTER_BARRIER_MAX_SKIP 0
#endif

#ifndef ADDR_VEC_ALIGN
int
final_addr_vec_align (addr_vec)
     rtx addr_vec;
{
  int align = exact_log2 (GET_MODE_SIZE (GET_MODE (PATTERN (addr_vec))));

  if (align > BIGGEST_ALIGNMENT / BITS_PER_UNIT)
    align = BIGGEST_ALIGNMENT / BITS_PER_UNIT;
  return align;

}
#define ADDR_VEC_ALIGN(ADDR_VEC) final_addr_vec_align (ADDR_VEC)
#endif

#ifndef INSN_LENGTH_ALIGNMENT
#define INSN_LENGTH_ALIGNMENT(INSN) length_unit_log
#endif

#define INSN_SHUID(INSN) (uid_shuid[INSN_UID (INSN)])

static int min_labelno, max_labelno;

#define LABEL_TO_ALIGNMENT(LABEL) \
  (label_align[CODE_LABEL_NUMBER (LABEL) - min_labelno].alignment)

#define LABEL_TO_MAX_SKIP(LABEL) \
  (label_align[CODE_LABEL_NUMBER (LABEL) - min_labelno].max_skip)

/* For the benefit of port specific code do this also as a function.  */
int
label_to_alignment (label)
     rtx label;
{
  return LABEL_TO_ALIGNMENT (label);
}

#ifdef HAVE_ATTR_length
/* The differences in addresses
   between a branch and its target might grow or shrink depending on
   the alignment the start insn of the range (the branch for a forward
   branch or the label for a backward branch) starts out on; if these
   differences are used naively, they can even oscillate infinitely.
   We therefore want to compute a 'worst case' address difference that
   is independent of the alignment the start insn of the range end
   up on, and that is at least as large as the actual difference.
   The function align_fuzz calculates the amount we have to add to the
   naively computed difference, by traversing the part of the alignment
   chain of the start insn of the range that is in front of the end insn
   of the range, and considering for each alignment the maximum amount
   that it might contribute to a size increase.

   For casesi tables, we also want to know worst case minimum amounts of
   address difference, in case a machine description wants to introduce
   some common offset that is added to all offsets in a table.
   For this purpose, align_fuzz with a growth argument of 0 comuptes the
   appropriate adjustment.  */


/* Compute the maximum delta by which the difference of the addresses of
   START and END might grow / shrink due to a different address for start
   which changes the size of alignment insns between START and END.
   KNOWN_ALIGN_LOG is the alignment known for START.
   GROWTH should be ~0 if the objective is to compute potential code size
   increase, and 0 if the objective is to compute potential shrink.
   The return value is undefined for any other value of GROWTH.  */
int
align_fuzz (start, end, known_align_log, growth)
     rtx start, end;
     int known_align_log;
     unsigned growth;
{
  int uid = INSN_UID (start);
  rtx align_label;
  int known_align = 1 << known_align_log;
  int end_shuid = INSN_SHUID (end);
  int fuzz = 0;

  for (align_label = uid_align[uid]; align_label; align_label = uid_align[uid])
    {
      int align_addr, new_align;

      uid = INSN_UID (align_label);
      align_addr = insn_addresses[uid] - insn_lengths[uid];
      if (uid_shuid[uid] > end_shuid)
	break;
      known_align_log = LABEL_TO_ALIGNMENT (align_label);
      new_align = 1 << known_align_log;
      if (new_align < known_align)
	continue;
      fuzz += (-align_addr ^ growth) & (new_align - known_align);
      known_align = new_align;
    }
  return fuzz;
}

/* Compute a worst-case reference address of a branch so that it
   can be safely used in the presence of aligned labels.  Since the
   size of the branch itself is unknown, the size of the branch is
   not included in the range.  I.e. for a forward branch, the reference
   address is the end address of the branch as known from the previous
   branch shortening pass, minus a value to account for possible size
   increase due to alignment.  For a backward branch, it is the start
   address of the branch as known from the current pass, plus a value
   to account for possible size increase due to alignment.
   NB.: Therefore, the maximum offset allowed for backward branches needs
   to exclude the branch size.  */
int
insn_current_reference_address (branch)
     rtx branch;
{
  rtx dest;
  rtx seq = NEXT_INSN (PREV_INSN (branch));
  int seq_uid = INSN_UID (seq);
  if (GET_CODE (branch) != JUMP_INSN)
    /* This can happen for example on the PA; the objective is to know the
       offset to address something in front of the start of the function.
       Thus, we can treat it like a backward branch.
       We assume here that FUNCTION_BOUNDARY / BITS_PER_UNIT is larger than
       any alignment we'd encounter, so we skip the call to align_fuzz.  */
    return insn_current_address;
  dest = JUMP_LABEL (branch);
  /* BRANCH has no proper alignment chain set, so use SEQ.  */
  if (INSN_SHUID (branch) < INSN_SHUID (dest))
    {
      /* Forward branch. */
      return (insn_last_address + insn_lengths[seq_uid]
	      - align_fuzz (seq, dest, length_unit_log, ~0));
    }
  else
    {
      /* Backward branch. */
      return (insn_current_address
	      + align_fuzz (dest, seq, length_unit_log, ~0));
    }
}
#endif /* HAVE_ATTR_length */

/* Make a pass over all insns and compute their actual lengths by shortening
   any branches of variable length if possible.  */

/* Give a default value for the lowest address in a function.  */

#ifndef FIRST_INSN_ADDRESS
#define FIRST_INSN_ADDRESS 0
#endif

/* shorten_branches might be called multiple times:  for example, the SH
   port splits out-of-range conditional branches in MACHINE_DEPENDENT_REORG.
   In order to do this, it needs proper length information, which it obtains
   by calling shorten_branches.  This cannot be collapsed with
   shorten_branches itself into a single pass unless we also want to intergate
   reorg.c, since the branch splitting exposes new instructions with delay
   slots.  */

void
shorten_branches (first)
     rtx first;
{
  rtx insn;
  int max_uid;
  int i;
  int max_log;
  int max_skip;
#ifdef HAVE_ATTR_length
#define MAX_CODE_ALIGN 16
  rtx seq;
  int something_changed = 1;
  char *varying_length;
  rtx body;
  int uid;
  rtx align_tab[MAX_CODE_ALIGN];

  /* In order to make sure that all instructions have valid length info,
     we must split them before we compute the address/length info.  */

  for (insn = NEXT_INSN (first); insn; insn = NEXT_INSN (insn))
    if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
      {
	rtx old = insn;
	insn = try_split (PATTERN (old), old, 1);
	/* When not optimizing, the old insn will be still left around
	   with only the 'deleted' bit set.  Transform it into a note
	   to avoid confusion of subsequent processing.  */
	if (INSN_DELETED_P (old))
          {
            PUT_CODE (old , NOTE);
            NOTE_LINE_NUMBER (old) = NOTE_INSN_DELETED;
            NOTE_SOURCE_FILE (old) = 0;
          }
      }
#endif

  /* We must do some computations even when not actually shortening, in
     order to get the alignment information for the labels.  */

  init_insn_lengths ();

  /* Compute maximum UID and allocate label_align / uid_shuid.  */
  max_uid = get_max_uid ();

  max_labelno = max_label_num ();
  min_labelno = get_first_label_num ();
  label_align = (struct label_alignment *) xmalloc (
    (max_labelno - min_labelno + 1) * sizeof (struct label_alignment));
  zero_memory ((char *) label_align,
    (max_labelno - min_labelno + 1) * sizeof (struct label_alignment));

  uid_shuid = (int *) xmalloc (max_uid * sizeof *uid_shuid);

  /* Initialize label_align and set up uid_shuid to be strictly
     monotonically rising with insn order.  */
  /* We use max_log here to keep track of the maximum alignment we want to
     impose on the next CODE_LABEL (or the current one if we are processing
     the CODE_LABEL itself).  */

  max_log = 0;
  max_skip = 0;

  for (insn = get_insns (), i = 1; insn; insn = NEXT_INSN (insn))
    {
      int log;

      INSN_SHUID (insn) = i++;
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
	{
	  /* reorg might make the first insn of a loop being run once only,
             and delete the label in front of it.  Then we want to apply
             the loop alignment to the new label created by reorg, which
             is separated by the former loop start insn from the
	     NOTE_INSN_LOOP_BEG.  */
	}
      else if (GET_CODE (insn) == CODE_LABEL)
	{
	  rtx next;

	  log = LABEL_ALIGN (insn);
	  if (max_log < log)
	    {
	      max_log = log;
	      max_skip = LABEL_ALIGN_MAX_SKIP;
	    }
	  next = NEXT_INSN (insn);
	  /* ADDR_VECs only take room if read-only data goes into the text
	     section.  */
	  if (JUMP_TABLES_IN_TEXT_SECTION
#if !defined(READONLY_DATA_SECTION)
	      || 1
#endif
	      )
	    if (next && GET_CODE (next) == JUMP_INSN)
	      {
		rtx nextbody = PATTERN (next);
		if (GET_CODE (nextbody) == ADDR_VEC
		    || GET_CODE (nextbody) == ADDR_DIFF_VEC)
		  {
		    log = ADDR_VEC_ALIGN (next);
		    if (max_log < log)
		      {
			max_log = log;
			max_skip = LABEL_ALIGN_MAX_SKIP;
		      }
		  }
	      }
	  LABEL_TO_ALIGNMENT (insn) = max_log;
	  LABEL_TO_MAX_SKIP (insn) = max_skip;
	  max_log = 0;
	  max_skip = 0;
	}
      else if (GET_CODE (insn) == BARRIER)
	{
	  rtx label;

	  for (label = insn; label && GET_RTX_CLASS (GET_CODE (label)) != 'i';
	       label = NEXT_INSN (label))
	    if (GET_CODE (label) == CODE_LABEL)
	      {
		log = LABEL_ALIGN_AFTER_BARRIER (insn);
		if (max_log < log)
		  {
		    max_log = log;
		    max_skip = LABEL_ALIGN_AFTER_BARRIER_MAX_SKIP;
		  }
		break;
	      }
	}
      /* Again, we allow NOTE_INSN_LOOP_BEG - INSN - CODE_LABEL
	 sequences in order to handle reorg output efficiently.  */
      else if (GET_CODE (insn) == NOTE
	       && NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	{
	  rtx label;
	  int nest = 0;

	  /* Search for the label that starts the loop.
	     Don't skip past the end of the loop, since that could
	     lead to putting an alignment where it does not belong.
	     However, a label after a nested (non-)loop would be OK.  */
	  for (label = insn; label; label = NEXT_INSN (label))
	    {
	      if (GET_CODE (label) == NOTE
		  && NOTE_LINE_NUMBER (label) == NOTE_INSN_LOOP_BEG)
		nest++;
	      else if (GET_CODE (label) == NOTE
		       && NOTE_LINE_NUMBER (label) == NOTE_INSN_LOOP_END
		       && --nest == 0)
		break;
	      else if (GET_CODE (label) == CODE_LABEL)
		{
		  log = LOOP_ALIGN (label);
		  if (max_log < log)
		    {
		      max_log = log;
		      max_skip = LOOP_ALIGN_MAX_SKIP;
		    }
		  break;
		}
	    }
	}
      else
	continue;
    }
#ifdef HAVE_ATTR_length

  /* Allocate the rest of the arrays.  */
  insn_lengths = (short *) xmalloc (max_uid * sizeof (short));
  insn_addresses = (int *) xmalloc (max_uid * sizeof (int));
  insn_lengths_max_uid = max_uid;
  /* Syntax errors can lead to labels being outside of the main insn stream.
     Initialize insn_addresses, so that we get reproducible results.  */
  zero_memory ((char *)insn_addresses, max_uid * sizeof *insn_addresses);
  uid_align = (rtx *) xmalloc (max_uid * sizeof *uid_align);

  varying_length = (char *) xmalloc (max_uid * sizeof (char));

  zero_memory (varying_length, max_uid);

  /* Initialize uid_align.  We scan instructions
     from end to start, and keep in align_tab[n] the last seen insn
     that does an alignment of at least n+1, i.e. the successor
     in the alignment chain for an insn that does / has a known
     alignment of n.  */

  zero_memory ((char *) uid_align, max_uid * sizeof *uid_align);

  for (i = MAX_CODE_ALIGN; --i >= 0; )
    align_tab[i] = NULL_RTX;
  seq = get_last_insn ();
  for (; seq; seq = PREV_INSN (seq))
    {
      int uid = INSN_UID (seq);
      int log;
      log = (GET_CODE (seq) == CODE_LABEL ? LABEL_TO_ALIGNMENT (seq) : 0);
      uid_align[uid] = align_tab[0];
      if (log)
	{
	  /* Found an alignment label.  */
	  uid_align[uid] = align_tab[log];
	  for (i = log - 1; i >= 0; i--)
	    align_tab[i] = seq;
	}
    }


  /* Compute initial lengths, addresses, and varying flags for each insn.  */
  for (insn_current_address = FIRST_INSN_ADDRESS, insn = first;
       insn != 0;
       insn_current_address += insn_lengths[uid], insn = NEXT_INSN (insn))
    {
      uid = INSN_UID (insn);

      insn_lengths[uid] = 0;

      if (GET_CODE (insn) == CODE_LABEL)
	{
	  int log = LABEL_TO_ALIGNMENT (insn);
	  if (log)
	    {
	      int align = 1 << log;
	      int new_address = (insn_current_address + align - 1) & -align;
	      insn_lengths[uid] = new_address - insn_current_address;
	      insn_current_address = new_address;
	    }
	}

      insn_addresses[uid] = insn_current_address;

      if (GET_CODE (insn) == NOTE || GET_CODE (insn) == BARRIER
	  || GET_CODE (insn) == CODE_LABEL)
	continue;
      if (INSN_DELETED_P (insn))
	continue;

      body = PATTERN (insn);
      if (GET_CODE (body) == ADDR_VEC || GET_CODE (body) == ADDR_DIFF_VEC)
	{
	  /* This only takes room if read-only data goes into the text
	     section.  */
	  if (JUMP_TABLES_IN_TEXT_SECTION
#if !defined(READONLY_DATA_SECTION)
	      || 1
#endif
	      )
	    insn_lengths[uid] = (XVECLEN (body,
					  GET_CODE (body) == ADDR_DIFF_VEC)
				 * GET_MODE_SIZE (GET_MODE (body)));
	  /* Alignment is handled by ADDR_VEC_ALIGN.  */
	}
      else if (asm_noperands (body) >= 0)
	insn_lengths[uid] = asm_insn_count (body) * insn_default_length (insn);
      else if (GET_CODE (body) == SEQUENCE)
	{
	  int i;
	  /* Inside a delay slot sequence, we do not do any branch shortening
	     if the shortening could change the number of delay slots
	     of the branch.  */
	  for (i = 0; i < XVECLEN (body, 0); i++)
	    {
	      rtx inner_insn = XVECEXP (body, 0, i);
	      int inner_uid = INSN_UID (inner_insn);
	      int inner_length;

	      if (asm_noperands (PATTERN (XVECEXP (body, 0, i))) >= 0)
		inner_length = (asm_insn_count (PATTERN (inner_insn))
				* insn_default_length (inner_insn));
	      else
		inner_length = insn_default_length (inner_insn);

	      insn_lengths[inner_uid] = inner_length;
	      varying_length[inner_uid] = 0;
	      insn_lengths[uid] += inner_length;
	    }
	}
      else if (GET_CODE (body) != USE && GET_CODE (body) != CLOBBER)
	{
	  insn_lengths[uid] = insn_default_length (insn);
	  varying_length[uid] = insn_variable_length_p (insn);
	}

      /* If needed, do any adjustment.  */
    }

  /* Now loop over all the insns finding varying length insns.  For each,
     get the current insn length.  If it has changed, reflect the change.
     When nothing changes for a full pass, we are done.  */

  while (something_changed)
    {
      something_changed = 0;
      insn_current_align = MAX_CODE_ALIGN - 1;
      for (insn_current_address = FIRST_INSN_ADDRESS, insn = first;
	   insn != 0;
	   insn = NEXT_INSN (insn))
	{
	  int new_length;
	  int length_align;

	  uid = INSN_UID (insn);

	  if (GET_CODE (insn) == CODE_LABEL)
	    {
	      int log = LABEL_TO_ALIGNMENT (insn);
	      if (log > insn_current_align)
		{
		  int align = 1 << log;
		  int new_address= (insn_current_address + align - 1) & -align;
		  insn_lengths[uid] = new_address - insn_current_address;
		  insn_current_align = log;
		  insn_current_address = new_address;
		}
	      else
		insn_lengths[uid] = 0;
	      insn_addresses[uid] = insn_current_address;
	      continue;
	    }

	  length_align = INSN_LENGTH_ALIGNMENT (insn);
	  if (length_align < insn_current_align)
	    insn_current_align = length_align;

	  insn_last_address = insn_addresses[uid];
	  insn_addresses[uid] = insn_current_address;


	  if (! (varying_length[uid]))
	    {
	      insn_current_address += insn_lengths[uid];
	      continue;
	    }
	  if (GET_CODE (insn) == INSN && GET_CODE (PATTERN (insn)) == SEQUENCE)
	    {
	      int i;

	      body = PATTERN (insn);
	      new_length = 0;
	      for (i = 0; i < XVECLEN (body, 0); i++)
		{
		  rtx inner_insn = XVECEXP (body, 0, i);
		  int inner_uid = INSN_UID (inner_insn);
		  int inner_length;

		  insn_addresses[inner_uid] = insn_current_address;

		  /* insn_current_length returns 0 for insns with a
		     non-varying length.  */
		  if (! varying_length[inner_uid])
		    inner_length = insn_lengths[inner_uid];
		  else
		    inner_length = insn_current_length (inner_insn);

		  if (inner_length != insn_lengths[inner_uid])
		    {
		      insn_lengths[inner_uid] = inner_length;
		      something_changed = 1;
		    }
		  insn_current_address += insn_lengths[inner_uid];
		  new_length += inner_length;
		}
	    }
	  else
	    {
	      new_length = insn_current_length (insn);
	      insn_current_address += new_length;
	    }


	  if (new_length != insn_lengths[uid])
	    {
	      insn_lengths[uid] = new_length;
	      something_changed = 1;
	    }
	}
      /* For a non-optimizing compile, do only a single pass.  */
      if (!optimize)
	break;
    }

  free (varying_length);

#endif /* HAVE_ATTR_length */
}

#ifdef HAVE_ATTR_length
/* Given the body of an INSN known to be generated by an ASM statement, return
   the number of machine instructions likely to be generated for this insn.
   This is used to compute its length.  */

static int
asm_insn_count (body)
     rtx body;
{
  char *template;
  int count = 1;

  if (GET_CODE (body) == ASM_INPUT)
    template = XSTR (body, 0);
  else
    template = decode_asm_operands (body, NULL, NULL,
				    NULL, NULL);

  for ( ; *template; template++)
    if (IS_ASM_LOGICAL_LINE_SEPARATOR(*template) || *template == '\n')
      count++;

  return count;
}
#endif

/* Output assembler code for the start of a function,
   and initialize some of the variables in this file
   for the new function.  The label for the function and associated
   assembler pseudo-ops have already been output in `assemble_start_function'.

   FIRST is the first insn of the rtl for the function being compiled.
   FILE is the file to write assembler code to.
   OPTIMIZE is nonzero if we should eliminate redundant
     test and compare insns.  */

void
final_start_function (first, file, optimize)
     rtx first;
     FILE *file;
     int optimize;
{
  block_depth = 0;
  this_is_asm_operands = 0;

  /* Initial line number is supposed to be output
     before the function's prologue and label
     so that the function's address will not appear to be
     in the last statement of the preceding function.  */
  if (NOTE_LINE_NUMBER (first) != NOTE_INSN_DELETED)
    last_linenum = high_block_linenum = high_function_linenum
      = NOTE_LINE_NUMBER (first);

#if defined (DWARF2_DEBUGGING_INFO)
  /* Output DWARF definition of the function.  */
  if (dwarf2out_do_frame ())
    dwarf2out_begin_prologue ();
#endif

      if (NOTE_LINE_NUMBER (first) != NOTE_INSN_DELETED)
	output_source_line (file, first);

#ifdef FUNCTION_PROLOGUE
  /* First output the function prologue: code to set up the stack frame.  */
  FUNCTION_PROLOGUE (file, get_frame_size ());
#endif
}

/* Output assembler code for the end of a function.
   For clarity, args are same as those of `final_start_function'
   even though not all of them are needed.  */

void
final_end_function (first, file, optimize)
     rtx first;
     FILE *file;
     int optimize;
{
  if (app_on)
    {
      fputs (ASM_APP_OFF, file);
      app_on = 0;
    }


#ifdef FUNCTION_EPILOGUE
  /* Finally, output the function epilogue:
     code to restore the stack frame and return to the caller.  */
  FUNCTION_EPILOGUE (file, get_frame_size ());
#endif


#if defined (DWARF2_DEBUGGING_INFO)
  if (dwarf2out_do_frame ())
    dwarf2out_end_epilogue ();
#endif

  /* If FUNCTION_EPILOGUE is not defined, then the function body
     itself contains return instructions wherever needed.  */
}

/* Output assembler code for some insns: all or part of a function.
   For description of args, see `final_start_function', above.

   PRESCAN is 1 if we are not really outputting,
     just scanning as if we were outputting.
   Prescanning deletes and rearranges insns just like ordinary output.
   PRESCAN is -2 if we are outputting after having prescanned.
   In this case, don't try to delete or rearrange insns
   because that has already been done.
   Prescanning is done only on certain machines.  */

void
final (first, file, optimize, prescan)
     rtx first;
     FILE *file;
     int optimize;
     int prescan;
{
  register rtx insn;
  int max_line = 0;
  int max_uid = 0;

  last_ignored_compare = 0;

  check_exception_handler_labels ();

  /* CYGNUS LOCAL LRS */
  if (write_symbols != NO_DEBUG)
    block_nodes = identify_blocks (DECL_INITIAL (current_function_decl), first);
  /* END CYGNUS LOCAL */

  /* Make a map indicating which line numbers appear in this function. */
    for (insn = first; insn; insn = NEXT_INSN (insn))
        if (GET_CODE (insn) == NOTE && NOTE_LINE_NUMBER (insn) > max_line)
            max_line = NOTE_LINE_NUMBER (insn);

  line_note_exists = (char *) oballoc (max_line + 1);
  zero_memory (line_note_exists, max_line + 1);

  for (insn = first; insn; insn = NEXT_INSN (insn))
    {
      if (INSN_UID (insn) > max_uid)       /* find largest UID */
        max_uid = INSN_UID (insn);
      if (GET_CODE (insn) == NOTE && NOTE_LINE_NUMBER (insn) > 0)
        line_note_exists[NOTE_LINE_NUMBER (insn)] = 1;
#ifdef HAVE_cc0
      /* If CC tracking across branches is enabled, record the insn which
	 jumps to each branch only reached from one place.  */
      if (optimize && GET_CODE (insn) == JUMP_INSN)
	{
	  rtx lab = JUMP_LABEL (insn);
	  if (lab && LABEL_NUSES (lab) == 1)
	    {
	      LABEL_REFS (lab) = insn;
	    }
	}
#endif
    }

  /* Initialize insn_eh_region table if eh is being used. */

  init_insn_eh_region (first, max_uid);

  init_recog ();

  CC_STATUS_INIT;

  /* Output the insns.  */
  for (insn = NEXT_INSN (first); insn;)
    {
#ifdef HAVE_ATTR_length
      insn_current_address = insn_addresses[INSN_UID (insn)];
#endif
      insn = final_scan_insn (insn, file, optimize, prescan, 0);
    }

  /* CYGNUS LOCAL LRS */
  if (write_symbols != NO_DEBUG)
    free ((char *)block_nodes);
  block_nodes = (tree *)0;
  /* END CYGNUS LOCAL */

  free_insn_eh_region ();
}

/* The final scan for one insn, INSN.
   Args are same as in `final', except that INSN
   is the insn being scanned.
   Value returned is the next insn to be scanned.

   NOPEEPHOLES is the flag to disallow peephole processing (currently
   used for within delayed branch sequence output).  */

rtx
final_scan_insn (insn, file, optimize, prescan, nopeepholes)
     rtx insn;
     FILE *file;
     int optimize;
     int prescan;
     int nopeepholes;
{
#ifdef HAVE_cc0
  rtx set;
#endif

  insn_counter++;

  /* Ignore deleted insns.  These can occur when we split insns (due to a
     template of "#") while not optimizing.  */
  if (INSN_DELETED_P (insn))
    return NEXT_INSN (insn);

  switch (GET_CODE (insn))
    {
    case NOTE:
      if (prescan > 0)
	break;

      /* Align the beginning of a loop, for higher speed
	 on certain machines.  */

      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	break; /* This used to depend on optimize, but that was bogus.  */
      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
	break;

      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_EH_REGION_BEG
	  && ! exceptions_via_longjmp)
	{
	  ASM_OUTPUT_INTERNAL_LABEL (file, "LEHB", NOTE_BLOCK_NUMBER (insn));
          if (! flag_new_exceptions)
            add_eh_table_entry (NOTE_BLOCK_NUMBER (insn));
#ifdef ASM_OUTPUT_EH_REGION_BEG
	  ASM_OUTPUT_EH_REGION_BEG (file, NOTE_BLOCK_NUMBER (insn));
#endif
	  break;
	}

      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_EH_REGION_END
	  && ! exceptions_via_longjmp)
	{
	  ASM_OUTPUT_INTERNAL_LABEL (file, "LEHE", NOTE_BLOCK_NUMBER (insn));
          if (flag_new_exceptions)
            add_eh_table_entry (NOTE_BLOCK_NUMBER (insn));
#ifdef ASM_OUTPUT_EH_REGION_END
	  ASM_OUTPUT_EH_REGION_END (file, NOTE_BLOCK_NUMBER (insn));
#endif
	  break;
	}

      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_PROLOGUE_END)
	{
#ifdef FUNCTION_END_PROLOGUE
	  FUNCTION_END_PROLOGUE (file);
#endif
	  break;
	}

#ifdef FUNCTION_BEGIN_EPILOGUE
      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_EPILOGUE_BEG)
	{
	  FUNCTION_BEGIN_EPILOGUE (file);
	  break;
	}
#endif

      if (write_symbols == NO_DEBUG)
	break;
      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_FUNCTION_BEG)
	{
	  break;
	}
      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_DELETED)
	break;			/* An insn that was "deleted" */
      if (app_on)
	{
	  fputs (ASM_APP_OFF, file);
	  app_on = 0;
	}
      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_BLOCK_BEG
	  && (debug_info_level == DINFO_LEVEL_NORMAL
	      || debug_info_level == DINFO_LEVEL_VERBOSE
	      || write_symbols == DWARF_DEBUG
	      || write_symbols == DWARF2_DEBUG))
	{
	  /* CYGNUS LOCAL LRS */
          tree block = block_nodes[ NOTE_BLOCK_NUMBER (insn)];

	  /* Beginning of a symbol-block.  Assign it a sequence number
	     and push the number onto the stack PENDING_BLOCKS.  */

	  if (block_depth == max_block_depth)
	    {
	      /* PENDING_BLOCKS is full; make it longer.  */
	      max_block_depth *= 2;
	      pending_blocks
		= (struct block_seq *) xrealloc (pending_blocks,
						 (max_block_depth
						  * sizeof (struct block_seq)));
	    }
	  pending_blocks[block_depth].block = block;
	  pending_blocks[block_depth++].number = next_block_index;
	  /* END CYGNUS LOCAL */

	  high_block_linenum = last_linenum;

	  /* Output debugging info about the symbol-block beginning.  */

#ifdef DWARF2_DEBUGGING_INFO
	  if (write_symbols == DWARF2_DEBUG)
	    dwarf2out_begin_block (next_block_index);
#endif

	  next_block_index++;
	}
      else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_BLOCK_END
	       && (debug_info_level == DINFO_LEVEL_NORMAL
		   || debug_info_level == DINFO_LEVEL_VERBOSE
	           || write_symbols == DWARF_DEBUG
	           || write_symbols == DWARF2_DEBUG))
	{
	  /* End of a symbol-block.  Pop its sequence number off
	     PENDING_BLOCKS and output debugging info based on that.  */

	  --block_depth;
	  if (block_depth < 0)
	    abort ();

	  /* CYGNUS LOCAL LRS */
#ifdef DWARF2_DEBUGGING_INFO
	  if (write_symbols == DWARF2_DEBUG)
	    dwarf2out_end_block (pending_blocks[block_depth].number);
#endif
	}
      /* END CYGNUS LOCAL */
      else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_DELETED_LABEL
	       && (debug_info_level == DINFO_LEVEL_NORMAL
		   || debug_info_level == DINFO_LEVEL_VERBOSE))
	{
#ifdef DWARF2_DEBUGGING_INFO
          if (write_symbols == DWARF2_DEBUG)
            dwarf2out_label (insn);
#endif
	}
      else if (NOTE_LINE_NUMBER (insn) > 0)
	/* This note is a line-number.  */
	{
	  register rtx note;

#if 0 /* This is what we used to do.  */
	  output_source_line (file, insn);
#endif
	  int note_after = 0;

	  /* If there is anything real after this note,
	     output it.  If another line note follows, omit this one.  */
	  for (note = NEXT_INSN (insn); note; note = NEXT_INSN (note))
	    {
	      if (GET_CODE (note) != NOTE && GET_CODE (note) != CODE_LABEL)
		break;
	      /* These types of notes can be significant
		 so make sure the preceding line number stays.  */
	      else if (GET_CODE (note) == NOTE
		       && (NOTE_LINE_NUMBER (note) == NOTE_INSN_BLOCK_BEG
			   || NOTE_LINE_NUMBER (note) == NOTE_INSN_BLOCK_END
			   || NOTE_LINE_NUMBER (note) == NOTE_INSN_FUNCTION_BEG))
  		break;
	      else if (GET_CODE (note) == NOTE && NOTE_LINE_NUMBER (note) > 0)
		{
		  /* Another line note follows; we can delete this note
		     if no intervening line numbers have notes elsewhere.  */
		  int num;
		  for (num = NOTE_LINE_NUMBER (insn) + 1;
		       num < NOTE_LINE_NUMBER (note);
		       num++)
		    if (line_note_exists[num])
		      break;

		  if (num >= NOTE_LINE_NUMBER (note))
		    note_after = 1;
		  break;
		}
	    }

	  /* Output this line note
	     if it is the first or the last line note in a row.  */
	  if (!note_after)
	    output_source_line (file, insn);
	}
      break;

    case BARRIER:
      break;

    case CODE_LABEL:
      /* The target port might emit labels in the output function for
	 some insn, e.g. sh.c output_branchy_insn.  */
      if (CODE_LABEL_NUMBER (insn) <= max_labelno)
	{
	  int align = LABEL_TO_ALIGNMENT (insn);

	  if (align && NEXT_INSN (insn))
	    ASM_OUTPUT_ALIGN (file, align);
	}
#ifdef HAVE_cc0
      CC_STATUS_INIT;
      /* If this label is reached from only one place, set the condition
	 codes from the instruction just before the branch.  */

      /* Disabled because some insns set cc_status in the C output code
	 and NOTICE_UPDATE_CC alone can set incorrect status.  */
      if (0 /* optimize && LABEL_NUSES (insn) == 1*/)
	{
	  rtx jump = LABEL_REFS (insn);
	  rtx barrier = prev_nonnote_insn (insn);
	  rtx prev;
	  /* If the LABEL_REFS field of this label has been set to point
	     at a branch, the predecessor of the branch is a regular
	     insn, and that branch is the only way to reach this label,
	     set the condition codes based on the branch and its
	     predecessor.  */
	  if (barrier && GET_CODE (barrier) == BARRIER
	      && jump && GET_CODE (jump) == JUMP_INSN
	      && (prev = prev_nonnote_insn (jump))
	      && GET_CODE (prev) == INSN)
	    {
	      NOTICE_UPDATE_CC (PATTERN (prev), prev);
	      NOTICE_UPDATE_CC (PATTERN (jump), jump);
	    }
	}
#endif
      if (prescan > 0)
	break;

#ifdef FINAL_PRESCAN_LABEL
      FINAL_PRESCAN_INSN (insn, NULL, 0);
#endif

#ifdef DWARF2_DEBUGGING_INFO
      if (write_symbols == DWARF2_DEBUG && LABEL_NAME (insn))
	dwarf2out_label (insn);
#endif
      if (app_on)
	{
	  fputs (ASM_APP_OFF, file);
	  app_on = 0;
	}
      if (NEXT_INSN (insn) != 0
	  && GET_CODE (NEXT_INSN (insn)) == JUMP_INSN)
	{
	  rtx nextbody = PATTERN (NEXT_INSN (insn));

	  /* If this label is followed by a jump-table,
	     make sure we put the label in the read-only section.  Also
	     possibly write the label and jump table together.  */

	  if (GET_CODE (nextbody) == ADDR_VEC
	      || GET_CODE (nextbody) == ADDR_DIFF_VEC)
	    {
#if defined(ASM_OUTPUT_ADDR_VEC) || defined(ASM_OUTPUT_ADDR_DIFF_VEC)
	      /* In this case, the case vector is being moved by the
		 target, so don't output the label at all.  Leave that
		 to the back end macros.  */
#else
	      if (! JUMP_TABLES_IN_TEXT_SECTION)
		{
		  readonly_data_section ();
#ifdef READONLY_DATA_SECTION
		  ASM_OUTPUT_ALIGN (file,
				    exact_log2 (BIGGEST_ALIGNMENT
						/ BITS_PER_UNIT));
#endif /* READONLY_DATA_SECTION */
		}
	      else
		function_section (current_function_decl);

#ifdef ASM_OUTPUT_CASE_LABEL
	      ASM_OUTPUT_CASE_LABEL (file, "L", CODE_LABEL_NUMBER (insn),
				     NEXT_INSN (insn));
#else
	      ASM_OUTPUT_INTERNAL_LABEL (file, "L", CODE_LABEL_NUMBER (insn));
#endif
#endif
	      break;
	    }
	}

      ASM_OUTPUT_INTERNAL_LABEL (file, "L", CODE_LABEL_NUMBER (insn));
      break;

    default:
      {
	register rtx body = PATTERN (insn);
	int insn_code_number;
	char *template;
#ifdef HAVE_cc0
	rtx note;
#endif

	/* An INSN, JUMP_INSN or CALL_INSN.
	   First check for special kinds that recog doesn't recognize.  */

	if (GET_CODE (body) == USE /* These are just declarations */
	    || GET_CODE (body) == CLOBBER)
	  break;

#ifdef HAVE_cc0
	/* If there is a REG_CC_SETTER note on this insn, it means that
	   the setting of the condition code was done in the delay slot
	   of the insn that branched here.  So recover the cc status
	   from the insn that set it.  */

	note = find_reg_note (insn, REG_CC_SETTER, NULL_RTX);
	if (note)
	  {
	    NOTICE_UPDATE_CC (PATTERN (XEXP (note, 0)), XEXP (note, 0));
	    cc_prev_status = cc_status;
	  }
#endif

	/* Detect insns that are really jump-tables
	   and output them as such.  */

	if (GET_CODE (body) == ADDR_VEC || GET_CODE (body) == ADDR_DIFF_VEC)
	  {
#if !(defined(ASM_OUTPUT_ADDR_VEC) || defined(ASM_OUTPUT_ADDR_DIFF_VEC))
	    register int vlen, idx;
#endif

	    if (prescan > 0)
	      break;

	    if (app_on)
	      {
		fputs (ASM_APP_OFF, file);
		app_on = 0;
	      }

#if defined(ASM_OUTPUT_ADDR_VEC) || defined(ASM_OUTPUT_ADDR_DIFF_VEC)
	    if (GET_CODE (body) == ADDR_VEC)
	      {
#ifdef ASM_OUTPUT_ADDR_VEC
		ASM_OUTPUT_ADDR_VEC (PREV_INSN (insn), body);
#else
		abort();
#endif
	      }
	    else
	      {
#ifdef ASM_OUTPUT_ADDR_DIFF_VEC
		ASM_OUTPUT_ADDR_DIFF_VEC (PREV_INSN (insn), body);
#else
		abort();
#endif
	      }
#else
	    vlen = XVECLEN (body, GET_CODE (body) == ADDR_DIFF_VEC);
	    for (idx = 0; idx < vlen; idx++)
	      {
		if (GET_CODE (body) == ADDR_VEC)
		  {
#ifdef ASM_OUTPUT_ADDR_VEC_ELT
		    ASM_OUTPUT_ADDR_VEC_ELT
		      (file, CODE_LABEL_NUMBER (XEXP (XVECEXP (body, 0, idx), 0)));
#else
		    abort ();
#endif
		  }
		else
		  {
#ifdef ASM_OUTPUT_ADDR_DIFF_ELT
		    ASM_OUTPUT_ADDR_DIFF_ELT
		      (file,
		       body,
		       CODE_LABEL_NUMBER (XEXP (XVECEXP (body, 1, idx), 0)),
		       CODE_LABEL_NUMBER (XEXP (XEXP (body, 0), 0)));
#else
		    abort ();
#endif
		  }
	      }
#ifdef ASM_OUTPUT_CASE_END
	    ASM_OUTPUT_CASE_END (file,
				 CODE_LABEL_NUMBER (PREV_INSN (insn)),
				 insn);
#endif
#endif

	    function_section (current_function_decl);

	    break;
	  }

	if (GET_CODE (body) == ASM_INPUT)
	  {
	    /* There's no telling what that did to the condition codes.  */
	    CC_STATUS_INIT;
	    if (prescan > 0)
	      break;
	    if (! app_on)
	      {
		fputs (ASM_APP_ON, file);
		app_on = 1;
	      }
	    fprintf (asm_out_file, "\t%s\n", XSTR (body, 0));
	    break;
	  }

	/* Detect `asm' construct with operands.  */
	if (asm_noperands (body) >= 0)
	  {
	    unsigned int noperands = asm_noperands (body);
	    rtx *ops = (rtx *) alloca (noperands * sizeof (rtx));
	    char *string;

	    /* There's no telling what that did to the condition codes.  */
	    CC_STATUS_INIT;
	    if (prescan > 0)
	      break;

	    if (! app_on)
	      {
		fputs (ASM_APP_ON, file);
		app_on = 1;
	      }

	    /* Get out the operand values.  */
	    string = decode_asm_operands (body, ops, NULL,
					  NULL, NULL);
	    /* Inhibit aborts on what would otherwise be compiler bugs.  */
	    insn_noperands = noperands;
	    this_is_asm_operands = insn;

	    /* Output the insn using them.  */
	    output_asm_insn (string, ops);
	    this_is_asm_operands = 0;
	    break;
	  }

	if (prescan <= 0 && app_on)
	  {
	    fputs (ASM_APP_OFF, file);
	    app_on = 0;
	  }

	if (GET_CODE (body) == SEQUENCE)
	  {
	    /* A delayed-branch sequence */
	    register int i;
	    rtx next;

	    if (prescan > 0)
	      break;
	    final_sequence = body;

	    /* The first insn in this SEQUENCE might be a JUMP_INSN that will
	       force the restoration of a comparison that was previously
	       thought unnecessary.  If that happens, cancel this sequence
	       and cause that insn to be restored.  */

	    next = final_scan_insn (XVECEXP (body, 0, 0), file, 0, prescan, 1);
	    if (next != XVECEXP (body, 0, 1))
	      {
		final_sequence = 0;
		return next;
	      }

	    for (i = 1; i < XVECLEN (body, 0); i++)
	      {
		rtx insn = XVECEXP (body, 0, i);
		rtx next = NEXT_INSN (insn);
		/* We loop in case any instruction in a delay slot gets
		   split.  */
		do
		  insn = final_scan_insn (insn, file, 0, prescan, 1);
		while (insn != next);
	      }
#ifdef DBR_OUTPUT_SEQEND
	    DBR_OUTPUT_SEQEND (file);
#endif
	    final_sequence = 0;

	    /* If the insn requiring the delay slot was a CALL_INSN, the
	       insns in the delay slot are actually executed before the
	       called function.  Hence we don't preserve any CC-setting
	       actions in these insns and the CC must be marked as being
	       clobbered by the function.  */
	    if (GET_CODE (XVECEXP (body, 0, 0)) == CALL_INSN)
	      {
		CC_STATUS_INIT;
	      }
	    break;
	  }

	/* We have a real machine instruction as rtl.  */

	body = PATTERN (insn);

#ifdef HAVE_cc0
	set = single_set(insn);

	/* Check for redundant test and compare instructions
	   (when the condition codes are already set up as desired).
	   This is done only when optimizing; if not optimizing,
	   it should be possible for the user to alter a variable
	   with the debugger in between statements
	   and the next statement should reexamine the variable
	   to compute the condition codes.  */

	if (optimize)
	  {
#if 0
	    rtx set = single_set(insn);
#endif

	    if (set
		&& GET_CODE (SET_DEST (set)) == CC0
		&& insn != last_ignored_compare)
	      {
		if (GET_CODE (SET_SRC (set)) == SUBREG)
		  SET_SRC (set) = alter_subreg (SET_SRC (set));
		else if (GET_CODE (SET_SRC (set)) == COMPARE)
		  {
		    if (GET_CODE (XEXP (SET_SRC (set), 0)) == SUBREG)
		      XEXP (SET_SRC (set), 0)
			= alter_subreg (XEXP (SET_SRC (set), 0));
		    if (GET_CODE (XEXP (SET_SRC (set), 1)) == SUBREG)
		      XEXP (SET_SRC (set), 1)
			= alter_subreg (XEXP (SET_SRC (set), 1));
		  }
		if ((cc_status.value1 != 0
		     && rtx_equal_p (SET_SRC (set), cc_status.value1))
		    || (cc_status.value2 != 0
			&& rtx_equal_p (SET_SRC (set), cc_status.value2)))
		  {
		    /* Don't delete insn if it has an addressing side-effect.  */
		    if (! FIND_REG_INC_NOTE (insn, 0)
			/* or if anything in it is volatile.  */
			&& ! volatile_refs_p (PATTERN (insn)))
		      {
			/* We don't really delete the insn; just ignore it.  */
			last_ignored_compare = insn;
			break;
		      }
		  }
	      }
	  }
#endif

	/* Don't bother outputting obvious no-ops, even without -O.
	   This optimization is fast and doesn't interfere with debugging.
	   Don't do this if the insn is in a delay slot, since this
	   will cause an improper number of delay insns to be written.  */
	if (final_sequence == 0
	    && prescan >= 0
	    && GET_CODE (insn) == INSN && GET_CODE (body) == SET
	    && GET_CODE (SET_SRC (body)) == REG
	    && GET_CODE (SET_DEST (body)) == REG
	    && REGNO (SET_SRC (body)) == REGNO (SET_DEST (body)))
	  break;

#ifdef HAVE_cc0
	/* If this is a conditional branch, maybe modify it
	   if the cc's are in a nonstandard state
	   so that it accomplishes the same thing that it would
	   do straightforwardly if the cc's were set up normally.  */

	if (cc_status.flags != 0
	    && GET_CODE (insn) == JUMP_INSN
	    && GET_CODE (body) == SET
	    && SET_DEST (body) == pc_rtx
	    && GET_CODE (SET_SRC (body)) == IF_THEN_ELSE
	    && GET_RTX_CLASS (GET_CODE (XEXP (SET_SRC (body), 0))) == '<'
	    && XEXP (XEXP (SET_SRC (body), 0), 0) == cc0_rtx
	    /* This is done during prescan; it is not done again
	       in final scan when prescan has been done.  */
	    && prescan >= 0)
	  {
	    /* This function may alter the contents of its argument
	       and clear some of the cc_status.flags bits.
	       It may also return 1 meaning condition now always true
	       or -1 meaning condition now always false
	       or 2 meaning condition nontrivial but altered.  */
	    register int result = alter_cond (XEXP (SET_SRC (body), 0));
	    /* If condition now has fixed value, replace the IF_THEN_ELSE
	       with its then-operand or its else-operand.  */
	    if (result == 1)
	      SET_SRC (body) = XEXP (SET_SRC (body), 1);
	    if (result == -1)
	      SET_SRC (body) = XEXP (SET_SRC (body), 2);

	    /* The jump is now either unconditional or a no-op.
	       If it has become a no-op, don't try to output it.
	       (It would not be recognized.)  */
	    if (SET_SRC (body) == pc_rtx)
	      {
		PUT_CODE (insn, NOTE);
		NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
		NOTE_SOURCE_FILE (insn) = 0;
		break;
	      }
	    else if (GET_CODE (SET_SRC (body)) == RETURN)
	      /* Replace (set (pc) (return)) with (return).  */
	      PATTERN (insn) = body = SET_SRC (body);

	    /* Rerecognize the instruction if it has changed.  */
	    if (result != 0)
	      INSN_CODE (insn) = -1;
	  }

	/* Make same adjustments to instructions that examine the
	   condition codes without jumping and instructions that
	   handle conditional moves (if this machine has either one).  */

	if (cc_status.flags != 0
	    && set != 0)
	  {
	    rtx cond_rtx, then_rtx, else_rtx;

	    if (GET_CODE (insn) != JUMP_INSN
		&& GET_CODE (SET_SRC (set)) == IF_THEN_ELSE)
	      {
		cond_rtx = XEXP (SET_SRC (set), 0);
		then_rtx = XEXP (SET_SRC (set), 1);
		else_rtx = XEXP (SET_SRC (set), 2);
	      }
	    else
	      {
		cond_rtx = SET_SRC (set);
		then_rtx = const_true_rtx;
		else_rtx = const0_rtx;
	      }

	    switch (GET_CODE (cond_rtx))
	      {
	      case GTU:
	      case GT:
	      case LTU:
	      case LT:
	      case GEU:
	      case GE:
	      case LEU:
	      case LE:
	      case EQ:
	      case NE:
		{
		  register int result;
		  if (XEXP (cond_rtx, 0) != cc0_rtx)
		    break;
		  result = alter_cond (cond_rtx);
		  if (result == 1)
		    validate_change (insn, &SET_SRC (set), then_rtx, 0);
		  else if (result == -1)
		    validate_change (insn, &SET_SRC (set), else_rtx, 0);
		  else if (result == 2)
		    INSN_CODE (insn) = -1;
		  if (SET_DEST (set) == SET_SRC (set))
		    {
		      PUT_CODE (insn, NOTE);
		      NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
		      NOTE_SOURCE_FILE (insn) = 0;
		    }
		}
		break;

	      default:
		break;
	      }
	  }

#endif

	/* Do machine-specific peephole optimizations if desired.  */

	if (optimize && !flag_no_peephole && !nopeepholes)
	  {
	    rtx next = peephole (insn);
	    /* When peepholing, if there were notes within the peephole,
	       emit them before the peephole.  */
	    if (next != 0 && next != NEXT_INSN (insn))
	      {
		rtx prev = PREV_INSN (insn);
		rtx note;

		for (note = NEXT_INSN (insn); note != next;
		     note = NEXT_INSN (note))
		  final_scan_insn (note, file, optimize, prescan, nopeepholes);

		/* In case this is prescan, put the notes
		   in proper position for later rescan.  */
		note = NEXT_INSN (insn);
		PREV_INSN (note) = prev;
		NEXT_INSN (prev) = note;
		NEXT_INSN (PREV_INSN (next)) = insn;
		PREV_INSN (insn) = PREV_INSN (next);
		NEXT_INSN (insn) = next;
		PREV_INSN (next) = insn;
	      }

	    /* PEEPHOLE might have changed this.  */
	    body = PATTERN (insn);
	  }

	/* Try to recognize the instruction.
	   If successful, verify that the operands satisfy the
	   constraints for the instruction.  Crash if they don't,
	   since `reload' should have changed them so that they do.  */

	insn_code_number = recog_memoized (insn);
	extract_insn (insn);
	cleanup_subreg_operands (insn);

#ifdef REGISTER_CONSTRAINTS
	if (! constrain_operands (1))
	  fatal_insn_not_found (insn);
#endif

	/* Some target machines need to prescan each insn before
	   it is output.  */

#ifdef FINAL_PRESCAN_INSN
	FINAL_PRESCAN_INSN (insn, recog_operand, recog_n_operands);
#endif

#ifdef HAVE_cc0
	cc_prev_status = cc_status;

	/* Update `cc_status' for this instruction.
	   The instruction's output routine may change it further.
	   If the output routine for a jump insn needs to depend
	   on the cc status, it should look at cc_prev_status.  */

	NOTICE_UPDATE_CC (body, insn);
#endif

	debug_insn = insn;


	/* If the proper template needs to be chosen by some C code,
	   run that code and get the real template.  */

	template = insn_template[insn_code_number];
	if (template == 0)
	  {
	    template = (*insn_outfun[insn_code_number]) (recog_operand, insn);

	    /* If the C code returns 0, it means that it is a jump insn
	       which follows a deleted test insn, and that test insn
	       needs to be reinserted.  */
	    if (template == 0)
	      {
		if (prev_nonnote_insn (insn) != last_ignored_compare)
		  abort ();
		return prev_nonnote_insn (insn);
	      }
	  }

	/* If the template is the string "#", it means that this insn must
	   be split.  */
	if (template[0] == '#' && template[1] == '\0')
	  {
	    rtx new = try_split (body, insn, 0);

	    /* If we didn't split the insn, go away.  */
	    if (new == insn && PATTERN (new) == body)
	      fatal_insn ("Could not split insn", insn);

#ifdef HAVE_ATTR_length
	    /* This instruction should have been split in shorten_branches,
	       to ensure that we would have valid length info for the
	       splitees.  */
	    abort ();
#endif

	    return new;
	  }

	if (prescan > 0)
	  break;

	/* Output assembler code from the template.  */

	output_asm_insn (template, recog_operand);


#if 0
	/* It's not at all clear why we did this and doing so interferes
	   with tests we'd like to do to use REG_WAS_0 notes, so let's try
	   with this out.  */

	/* Mark this insn as having been output.  */
	INSN_DELETED_P (insn) = 1;
#endif

	debug_insn = 0;
      }
    }
  return NEXT_INSN (insn);
}

/* Output debugging info to the assembler file FILE
   based on the NOTE-insn INSN, assumed to be a line number.  */

static void
output_source_line (file, insn)
     FILE *file;
     rtx insn;
{
  register char *filename = NOTE_SOURCE_FILE (insn);

  last_filename = filename;
  last_linenum = NOTE_LINE_NUMBER (insn);
  high_block_linenum = MAX (last_linenum, high_block_linenum);
  high_function_linenum = MAX (last_linenum, high_function_linenum);

  if (write_symbols != NO_DEBUG)
    {




#ifdef DWARF2_DEBUGGING_INFO
      if (write_symbols == DWARF2_DEBUG)
	dwarf2out_line (filename, NOTE_LINE_NUMBER (insn));
#endif
    }
}


/* For each operand in INSN, simplify (subreg (reg)) so that it refers
   directly to the desired hard register.  */
void
cleanup_subreg_operands (insn)
     rtx insn;
{
  int i;

  extract_insn (insn);
  for (i = 0; i < recog_n_operands; i++)
    {
      if (GET_CODE (recog_operand[i]) == SUBREG)
        recog_operand[i] = alter_subreg (recog_operand[i]);
      else if (GET_CODE (recog_operand[i]) == PLUS
               || GET_CODE (recog_operand[i]) == MULT)
       recog_operand[i] = walk_alter_subreg (recog_operand[i]);
    }

  for (i = 0; i < recog_n_dups; i++)
    {
      if (GET_CODE (*recog_dup_loc[i]) == SUBREG)
        *recog_dup_loc[i] = alter_subreg (*recog_dup_loc[i]);
      else if (GET_CODE (*recog_dup_loc[i]) == PLUS
               || GET_CODE (*recog_dup_loc[i]) == MULT)
        *recog_dup_loc[i] = walk_alter_subreg (*recog_dup_loc[i]);
    }
}

/* If X is a SUBREG, replace it with a REG or a MEM,
   based on the thing it is a subreg of.  */

rtx
alter_subreg (x)
     register rtx x;
{
  register rtx y = SUBREG_REG (x);

  if (GET_CODE (y) == SUBREG)
    y = alter_subreg (y);

  /* If reload is operating, we may be replacing inside this SUBREG.
     Check for that and make a new one if so.  */
  if (reload_in_progress && find_replacement (&SUBREG_REG (x)) != 0)
    x = copy_rtx (x);

  if (GET_CODE (y) == REG)
    {
      /* If the word size is larger than the size of this register,
	 adjust the register number to compensate.  */
      /* ??? Note that this just catches stragglers created by/for
	 integrate.  It would be better if we either caught these
	 earlier, or kept _all_ subregs until now and eliminate
	 gen_lowpart and friends.  */

      PUT_CODE (x, REG);
#ifdef ALTER_HARD_SUBREG
      REGNO (x) = ALTER_HARD_SUBREG(GET_MODE (x), SUBREG_WORD (x),
				    GET_MODE (y), REGNO (y));
#else
      REGNO (x) = REGNO (y) + SUBREG_WORD (x);
#endif
      /* This field has a different meaning for REGs and SUBREGs.  Make sure
	 to clear it!  */
      x->used = 0;
    }
  else if (GET_CODE (y) == MEM)
    {
      register int offset = SUBREG_WORD (x) * UNITS_PER_WORD;
      PUT_CODE (x, MEM);
      MEM_COPY_ATTRIBUTES (x, y);
      MEM_ALIAS_SET (x) = MEM_ALIAS_SET (y);
      XEXP (x, 0) = plus_constant (XEXP (y, 0), offset);
    }

  return x;
}

/* Do alter_subreg on all the SUBREGs contained in X.  */

static rtx
walk_alter_subreg (x)
     rtx x;
{
  switch (GET_CODE (x))
    {
    case PLUS:
    case MULT:
      XEXP (x, 0) = walk_alter_subreg (XEXP (x, 0));
      XEXP (x, 1) = walk_alter_subreg (XEXP (x, 1));
      break;

    case MEM:
      XEXP (x, 0) = walk_alter_subreg (XEXP (x, 0));
      break;

    case SUBREG:
      return alter_subreg (x);

    default:
      break;
    }

  return x;
}

#ifdef HAVE_cc0

/* Given BODY, the body of a jump instruction, alter the jump condition
   as required by the bits that are set in cc_status.flags.
   Not all of the bits there can be handled at this level in all cases.

   The value is normally 0.
   1 means that the condition has become always true.
   -1 means that the condition has become always false.
   2 means that COND has been altered.  */

static int
alter_cond (cond)
     register rtx cond;
{
  int value = 0;

  if (cc_status.flags & CC_REVERSED)
    {
      value = 2;
      PUT_CODE (cond, swap_condition (GET_CODE (cond)));
    }

  if (cc_status.flags & CC_INVERTED)
    {
      value = 2;
      PUT_CODE (cond, reverse_condition (GET_CODE (cond)));
    }

  if (cc_status.flags & CC_NOT_POSITIVE)
    switch (GET_CODE (cond))
      {
      case LE:
      case LEU:
      case GEU:
	/* Jump becomes unconditional.  */
	return 1;

      case GT:
      case GTU:
      case LTU:
	/* Jump becomes no-op.  */
	return -1;

      case GE:
	PUT_CODE (cond, EQ);
	value = 2;
	break;

      case LT:
	PUT_CODE (cond, NE);
	value = 2;
	break;

      default:
	break;
      }

  if (cc_status.flags & CC_NOT_NEGATIVE)
    switch (GET_CODE (cond))
      {
      case GE:
      case GEU:
	/* Jump becomes unconditional.  */
	return 1;

      case LT:
      case LTU:
	/* Jump becomes no-op.  */
	return -1;

      case LE:
      case LEU:
	PUT_CODE (cond, EQ);
	value = 2;
	break;

      case GT:
      case GTU:
	PUT_CODE (cond, NE);
	value = 2;
	break;

      default:
	break;
      }

  if (cc_status.flags & CC_NO_OVERFLOW)
    switch (GET_CODE (cond))
      {
      case GEU:
	/* Jump becomes unconditional.  */
	return 1;

      case LEU:
	PUT_CODE (cond, EQ);
	value = 2;
	break;

      case GTU:
	PUT_CODE (cond, NE);
	value = 2;
	break;

      case LTU:
	/* Jump becomes no-op.  */
	return -1;

      default:
	break;
      }

  if (cc_status.flags & (CC_Z_IN_NOT_N | CC_Z_IN_N))
    switch (GET_CODE (cond))
      {
      default:
	abort ();

      case NE:
	PUT_CODE (cond, cc_status.flags & CC_Z_IN_N ? GE : LT);
	value = 2;
	break;

      case EQ:
	PUT_CODE (cond, cc_status.flags & CC_Z_IN_N ? LT : GE);
	value = 2;
	break;
      }

  if (cc_status.flags & CC_NOT_SIGNED)
    /* The flags are valid if signed condition operators are converted
       to unsigned.  */
    switch (GET_CODE (cond))
      {
      case LE:
	PUT_CODE (cond, LEU);
	value = 2;
	break;

      case LT:
	PUT_CODE (cond, LTU);
	value = 2;
	break;

      case GT:
	PUT_CODE (cond, GTU);
	value = 2;
	break;

      case GE:
	PUT_CODE (cond, GEU);
	value = 2;
	break;

      default:
	break;
      }

  return value;
}
#endif

/* Report inconsistency between the assembler template and the operands.
   In an `asm', it's the user's fault; otherwise, the compiler's fault.  */

void
output_operand_lossage (str)
     char *str;
{
  if (this_is_asm_operands)
    error_for_asm (this_is_asm_operands, "invalid `asm': %s", str);
  else
    fatal ("Internal compiler error, output_operand_lossage `%s'", str);
}

/* Output of assembler code from a template, and its subroutines.  */

/* Output text from TEMPLATE to the assembler output file,
   obeying %-directions to substitute operands taken from
   the vector OPERANDS.

   %N (for N a digit) means print operand N in usual manner.
   %lN means require operand N to be a CODE_LABEL or LABEL_REF
      and print the label name with no punctuation.
   %cN means require operand N to be a constant
      and print the constant expression with no punctuation.
   %aN means expect operand N to be a memory address
      (not a memory reference!) and print a reference
      to that address.
   %nN means expect operand N to be a constant
      and print a constant expression for minus the value
      of the operand, with no other punctuation.  */

static void
output_asm_name ()
{
  if (flag_print_asm_name)
    {
      /* Annotate the assembly with a comment describing the pattern and
	 alternative used.  */
      if (debug_insn)
	{
	  register int num = INSN_CODE (debug_insn);
	  fprintf (asm_out_file, "\t%s %d\t%s",
		   ASM_COMMENT_START, INSN_UID (debug_insn), insn_name[num]);
	  if (insn_n_alternatives[num] > 1)
	    fprintf (asm_out_file, "/%d", which_alternative + 1);
#ifdef HAVE_ATTR_length
	  fprintf (asm_out_file, "\t[length = %d]", get_attr_length (debug_insn));
#endif
	  /* Clear this so only the first assembler insn
	     of any rtl insn will get the special comment for -dp.  */
	  debug_insn = 0;
	}
    }
}

void
output_asm_insn (template, operands)
     char *template;
     rtx *operands;
{
  register char *p;
  register int c;

  /* An insn may return a null string template
     in a case where no assembler code is needed.  */
  if (*template == 0)
    return;

  p = template;
  putc ('\t', asm_out_file);

#ifdef ASM_OUTPUT_OPCODE
  ASM_OUTPUT_OPCODE (asm_out_file, p);
#endif

  while ((c = *p++))
    switch (c)
      {
      case '\n':
	output_asm_name ();
	putc (c, asm_out_file);
#ifdef ASM_OUTPUT_OPCODE
	while ((c = *p) == '\t')
	  {
	    putc (c, asm_out_file);
	    p++;
	  }
	ASM_OUTPUT_OPCODE (asm_out_file, p);
#endif
	break;


      case '%':
	/* %% outputs a single %.  */
	if (*p == '%')
	  {
	    p++;
	    putc (c, asm_out_file);
	  }
	/* %= outputs a number which is unique to each insn in the entire
	   compilation.  This is useful for making local labels that are
	   referred to more than once in a given insn.  */
	else if (*p == '=')
	  {
	    p++;
	    fprintf (asm_out_file, "%d", insn_counter);
	  }
	/* % followed by a letter and some digits
	   outputs an operand in a special way depending on the letter.
	   Letters `acln' are implemented directly.
	   Other letters are passed to `output_operand' so that
	   the PRINT_OPERAND macro can define them.  */
	else if ((*p >= 'a' && *p <= 'z')
		 || (*p >= 'A' && *p <= 'Z'))
	  {
	    int letter = *p++;
	    c = atoi (p);

	    if (! (*p >= '0' && *p <= '9'))
	      output_operand_lossage ("operand number missing after %-letter");
	    else if (this_is_asm_operands && (c < 0 || (unsigned int) c >= insn_noperands))
	      output_operand_lossage ("operand number out of range");
	    else if (letter == 'l')
	      output_asm_label (operands[c]);
	    else if (letter == 'a')
	      output_address (operands[c]);
	    else if (letter == 'c')
	      {
		if (CONSTANT_ADDRESS_P (operands[c]))
		  output_addr_const (asm_out_file, operands[c]);
		else
		  output_operand (operands[c], 'c');
	      }
	    else if (letter == 'n')
	      {
		if (GET_CODE (operands[c]) == CONST_INT)
		  {
		    print_wint(asm_out_file, -INTVAL(operands[c]));
		  }
		else
		  {
		    putc ('-', asm_out_file);
		    output_addr_const (asm_out_file, operands[c]);
		  }
	      }
	    else
	      output_operand (operands[c], letter);

	    while ((c = *p) >= '0' && c <= '9') p++;
	  }
	/* % followed by a digit outputs an operand the default way.  */
	else if (*p >= '0' && *p <= '9')
	  {
	    c = atoi (p);
	    if (this_is_asm_operands && (c < 0 || (unsigned int) c >= insn_noperands))
	      output_operand_lossage ("operand number out of range");
	    else
	      output_operand (operands[c], 0);
	    while ((c = *p) >= '0' && c <= '9') p++;
	  }
	/* % followed by punctuation: output something for that
	   punctuation character alone, with no operand.
	   The PRINT_OPERAND macro decides what is actually done.  */
#ifdef PRINT_OPERAND_PUNCT_VALID_P
	else if (PRINT_OPERAND_PUNCT_VALID_P ((unsigned char)*p))
	  output_operand (NULL_RTX, *p++);
#endif
	else
	  output_operand_lossage ("invalid %%-code");
	break;

      default:
	putc (c, asm_out_file);
      }

  output_asm_name ();

  putc ('\n', asm_out_file);
}

/* Output a LABEL_REF, or a bare CODE_LABEL, as an assembler symbol.  */

void
output_asm_label (x)
     rtx x;
{
  char buf[256];

  if (GET_CODE (x) == LABEL_REF)
    ASM_GENERATE_INTERNAL_LABEL (buf, "L", CODE_LABEL_NUMBER (XEXP (x, 0)));
  else if (GET_CODE (x) == CODE_LABEL)
    ASM_GENERATE_INTERNAL_LABEL (buf, "L", CODE_LABEL_NUMBER (x));
  else
    output_operand_lossage ("`%l' operand isn't a label");

  assemble_name (asm_out_file, buf);
}

/* Print operand X using machine-dependent assembler syntax.
   The macro PRINT_OPERAND is defined just to control this function.
   CODE is a non-digit that preceded the operand-number in the % spec,
   such as 'z' if the spec was `%z3'.  CODE is 0 if there was no char
   between the % and the digits.
   When CODE is a non-letter, X is 0.

   The meanings of the letters are machine-dependent and controlled
   by PRINT_OPERAND.  */

static void
output_operand (x, code)
     rtx x;
     int code;
{
  if (x && GET_CODE (x) == SUBREG)
    x = alter_subreg (x);

  /* If X is a pseudo-register, abort now rather than writing trash to the
     assembler file.  */

  if (x && GET_CODE (x) == REG && REGNO (x) >= FIRST_PSEUDO_REGISTER)
    abort ();

  PRINT_OPERAND (asm_out_file, x, code);
}

/* Print a memory reference operand for address X
   using machine-dependent assembler syntax.  */

void
output_address (x)
     rtx x;
{
    walk_alter_subreg (x);

    if (GET_CODE(x) == REG)
    {
        fprintf(asm_out_file, "[%s]", reg_names[REGNO(x)]);
    }
    else if (GET_CODE(x) == POST_INC)
    {
        fprintf(asm_out_file, "%s!", reg_names[REGNO(XEXP(x, 0))]);
    }
    else if (GET_CODE(x) == PLUS)
    {
        if (GET_CODE(XEXP(x, 1)) == CONST_INT)
        {
            fprintf(asm_out_file, "[%s, #", reg_names[REGNO(XEXP(x, 0))]);
            print_wint(asm_out_file, INTVAL(XEXP(x, 1)));
            fprintf(asm_out_file, "]");
        }
        else
        {
            fprintf(asm_out_file, "[%s, %s]",
                reg_names[REGNO(XEXP(x, 0))],
                reg_names[REGNO(XEXP(x, 1))]);
        }
    }
    else
    {
        output_addr_const(asm_out_file, x);
    }
}

/* Print an integer constant expression in assembler syntax.
   Addition and subtraction are the only arithmetic
   that may appear in these expressions.  */

void
output_addr_const (file, x)
     FILE *file;
     rtx x;
{
  char buf[256];

 restart:
  switch (GET_CODE (x))
    {
    case PC:
      abort ();
      break;

    case SYMBOL_REF:
      assemble_name (file, XSTR (x, 0));
      break;

    case LABEL_REF:
      ASM_GENERATE_INTERNAL_LABEL (buf, "L", CODE_LABEL_NUMBER (XEXP (x, 0)));
      assemble_name (file, buf);
      break;

    case CODE_LABEL:
      ASM_GENERATE_INTERNAL_LABEL (buf, "L", CODE_LABEL_NUMBER (x));
      assemble_name (file, buf);
      break;

    case CONST_INT:
      print_wint(file, INTVAL(x));
      break;

    case CONST:
      /* This used to output parentheses around the expression,
	 but that does not work on the 386 (either ATT or BSD assembler).  */
      output_addr_const (file, XEXP (x, 0));
      break;

    case CONST_DOUBLE:
      if (GET_MODE (x) == VOIDmode)
	{
	  /* We can use %d if the number is one word and positive.  */
	  if (CONST_DOUBLE_HIGH (x))
	    fprintf (file, HOST_WIDE_INT_PRINT_DOUBLE_HEX,
		     CONST_DOUBLE_HIGH (x), CONST_DOUBLE_LOW (x));
	  else if  (CONST_DOUBLE_LOW (x) < 0)
	    fprintf (file, HOST_WIDE_INT_PRINT_HEX, CONST_DOUBLE_LOW (x));
	  else
	    print_wint(file, CONST_DOUBLE_LOW(x));
	}
      else
	/* We can't handle floating point constants;
	   PRINT_OPERAND must handle them.  */
	output_operand_lossage ("floating constant misused");
      break;

    case PLUS:
      /* Some assemblers need integer constants to appear last (eg masm).  */
      if (GET_CODE (XEXP (x, 0)) == CONST_INT)
	{
	  output_addr_const (file, XEXP (x, 1));
	  if (INTVAL (XEXP (x, 0)) >= 0 || flag_hex_asm)
	    fprintf (file, "+");
	  output_addr_const (file, XEXP (x, 0));
	}
      else
	{
	  output_addr_const (file, XEXP (x, 0));
	  if (INTVAL (XEXP (x, 1)) >= 0 || flag_hex_asm)
	    fprintf (file, "+");
	  output_addr_const (file, XEXP (x, 1));
	}
      break;

    case MINUS:
      /* Avoid outputting things like x-x or x+5-x,
	 since some assemblers can't handle that.  */
      x = simplify_subtraction (x);
      if (GET_CODE (x) != MINUS)
	goto restart;

      output_addr_const (file, XEXP (x, 0));
      fprintf (file, "-");
      if (GET_CODE (XEXP (x, 1)) == CONST_INT
	  && INTVAL (XEXP (x, 1)) < 0)
	{
	  fprintf (file, ASM_OPEN_PAREN);
	  output_addr_const (file, XEXP (x, 1));
	  fprintf (file, ASM_CLOSE_PAREN);
	}
      else
	output_addr_const (file, XEXP (x, 1));
      break;

    case ZERO_EXTEND:
    case SIGN_EXTEND:
      output_addr_const (file, XEXP (x, 0));
      break;

    default:
      output_operand_lossage ("invalid expression as operand");
    }
}

/* A poor man's fprintf, with the added features of %I, %R, and %L.
   %R prints the value of REGISTER_PREFIX.
   %L prints the value of LOCAL_LABEL_PREFIX.
   %I prints the value of IMMEDIATE_PREFIX.
   %O runs ASM_OUTPUT_OPCODE to transform what follows in the string.
   Also supported are %d, %x, %s, %e, %f, %g and %%.

   We handle alternate assembler dialects here, just like output_asm_insn.  */

void
asm_fprintf (FILE *file, char *p, ...)
{
  va_list argptr;
  char buf[10];
  char *q, c;

  va_start (argptr, p);


  buf[0] = '%';

  while ((c = *p++))
    switch (c)
      {

      case '%':
	c = *p++;
	q = &buf[1];
	while ((c >= '0' && c <= '9') || c == '.')
	  {
	    *q++ = c;
	    c = *p++;
	  }
	switch (c)
	  {
	  case '%':
	    fprintf (file, "%%");
	    break;

	  case 'd':  case 'i':  case 'u':
	  case 'x':  case 'p':  case 'X':
	  case 'o':
	    *q++ = c;
	    *q = 0;
	    fprintf (file, buf, va_arg (argptr, int));
	    break;

	  case 'l':
	    *q++ = c;
	    *q++ = *p++;
	    *q = 0;
	    fprintf (file, buf, va_arg (argptr, long));
	    break;

	  case 'e':
	  case 'f':
	  case 'g':
	    *q++ = c;
	    *q = 0;
	    fprintf (file, buf, va_arg (argptr, double));
	    break;

	  case 's':
	    *q++ = c;
	    *q = 0;
	    fprintf (file, buf, va_arg (argptr, char *));
	    break;

	  case 'O':
#ifdef ASM_OUTPUT_OPCODE
	    ASM_OUTPUT_OPCODE (asm_out_file, p);
#endif
	    break;

	  case 'R':
#ifdef REGISTER_PREFIX
	    fprintf (file, "%s", REGISTER_PREFIX);
#endif
	    break;

	  case 'I':
#ifdef IMMEDIATE_PREFIX
	    fprintf (file, "%s", IMMEDIATE_PREFIX);
#endif
	    break;

	  case 'L':
#ifdef LOCAL_LABEL_PREFIX
	    fprintf (file, "%s", LOCAL_LABEL_PREFIX);
#endif
	    break;

	  default:
	    abort ();
	  }
	break;

      default:
	fputc (c, file);
      }
}

/* Split up a CONST_DOUBLE or integer constant rtx
   into two rtx's for single words,
   storing in *FIRST the word that comes first in memory in the target
   and in *SECOND the other.  */

void
split_double (value, first, second)
     rtx value;
     rtx *first, *second;
{
  if (GET_CODE (value) == CONST_INT)
    {
      if (HOST_BITS_PER_WIDE_INT >= (2 * BITS_PER_WORD))
	{
	  /* In this case the CONST_INT holds both target words.
	     Extract the bits from it into two word-sized pieces.
	     Sign extend each half to HOST_WIDE_INT.  */
	  rtx low, high;
	  /* On machines where HOST_BITS_PER_WIDE_INT == BITS_PER_WORD
	     the shift below will cause a compiler warning, even though
	     this code won't be executed.  So put the shift amounts in
	     variables to avoid the warning.  */
	  int rshift = HOST_BITS_PER_WIDE_INT - BITS_PER_WORD;
	  int lshift = HOST_BITS_PER_WIDE_INT - 2 * BITS_PER_WORD;

	  low = GEN_INT ((INTVAL (value) << rshift) >> rshift);
	  high = GEN_INT ((INTVAL (value) << lshift) >> rshift);

	      *first = low;
	      *second = high;
	}
      else
	{
	  /* The rule for using CONST_INT for a wider mode
	     is that we regard the value as signed.
	     So sign-extend it.  */
	  rtx high = (INTVAL (value) < 0 ? constm1_rtx : const0_rtx);

	      *first = value;
	      *second = high;
	}
    }
  else if (GET_CODE (value) != CONST_DOUBLE)
    {
	  *first = value;
	  *second = const0_rtx;
    }
  else if (GET_MODE (value) == VOIDmode
	   /* This is the old way we did CONST_DOUBLE integers.  */
	   || GET_MODE_CLASS (GET_MODE (value)) == MODE_INT)
    {
      /* In an integer, the words are defined as most and least significant.
	 So order them by the target's convention.  */
	  *first = GEN_INT (CONST_DOUBLE_LOW (value));
	  *second = GEN_INT (CONST_DOUBLE_HIGH (value));
    }
  else
    {
      REAL_VALUE_TYPE r; long l[2];
      REAL_VALUE_FROM_CONST_DOUBLE (r, value);

      /* Note, this converts the REAL_VALUE_TYPE to the target's
	 format, splits up the floating point double and outputs
	 exactly 32 bits of it into each of l[0] and l[1] --
	 not necessarily BITS_PER_WORD bits.  */
      REAL_VALUE_TO_TARGET_DOUBLE (r, l);

      *first = GEN_INT ((HOST_WIDE_INT) l[0]);
      *second = GEN_INT ((HOST_WIDE_INT) l[1]);
    }
}

/* Return nonzero if this function has no function calls.  */

int
leaf_function_p ()
{
  rtx insn;

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (GET_CODE (insn) == CALL_INSN)
	return 0;
      if (GET_CODE (insn) == INSN
	  && GET_CODE (PATTERN (insn)) == SEQUENCE
	  && GET_CODE (XVECEXP (PATTERN (insn), 0, 0)) == CALL_INSN)
	return 0;
    }

  return 1;
}
