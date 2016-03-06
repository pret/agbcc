/* Move registers around to reduce number of move instructions needed.
   Copyright (C) 1987, 88, 89, 92-98, 1999 Free Software Foundation, Inc.

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


/* This module looks for cases where matching constraints would force
   an instruction to need a reload, and this reload would be a register
   to register move.  It then attempts to change the registers used by the
   instruction to avoid the move instruction.  */

#include "config.h"
#include "system.h"
#include "rtl.h" /* stdio.h must precede rtl.h for FFS.  */
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "reload.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "expr.h"
#include "insn-flags.h"
#include "basic-block.h"
#include "toplev.h"
/* CYGNUS LOCAL SH4-OPT */
#include "obstack.h"
/* END CYGNUS LOCAL */

static int optimize_reg_copy_1	(rtx, rtx, rtx);
static void optimize_reg_copy_2	(rtx, rtx, rtx);
static void optimize_reg_copy_3	(rtx, rtx, rtx);
static rtx gen_add3_insn	(rtx, rtx, rtx);
static void copy_src_to_dest	(rtx, rtx, rtx, int, int);
static int *regmove_bb_head;

struct match {
  int with[MAX_RECOG_OPERANDS];
  enum { READ, WRITE, READWRITE } use[MAX_RECOG_OPERANDS];
  int commutative[MAX_RECOG_OPERANDS];
  int early_clobber[MAX_RECOG_OPERANDS];
};

static int try_auto_increment (rtx, rtx, rtx, rtx, HOST_WIDE_INT, int);
static int find_matches (rtx, struct match *);
static int fixup_match_1 (rtx, rtx, rtx, rtx, rtx, int, int, int, FILE *)
;
static int reg_is_remote_constant_p (rtx, rtx, rtx);
static int stable_but_for_p (rtx, rtx, rtx);
static int regclass_compatible_p (int, int);
/* CYGNUS LOCAL SH4-OPT */
static struct rel_use *lookup_related (int, enum reg_class, HOST_WIDE_INT);
static void rel_build_chain (struct rel_use *, struct rel_use *, int);
static void rel_record_mem (rtx *, rtx, int, int, int, rtx, int, int);
static void invalidate_related (rtx, int);
static void find_related (rtx *, rtx, int, int);
static int chain_starts_earlier (const void *, const void *);
static int chain_ends_later (const void *, const void *);
static struct related *optimize_related_values_1 (struct related *, int,
							int, rtx, FILE *);
static void optimize_related_values_0 (struct related *, int, int,
					     rtx, FILE *);
static void optimize_related_values (int, FILE *);
static void count_sets (rtx, rtx);
/* END CYGNUS LOCAL */
static int loop_depth;

/* Return non-zero if registers with CLASS1 and CLASS2 can be merged without
   causing too much register allocation problems.  */
static int
regclass_compatible_p (class0, class1)
     int class0, class1;
{
  return (class0 == class1
	  || (reg_class_subset_p (class0, class1)
	      && ! CLASS_LIKELY_SPILLED_P (class0))
	  || (reg_class_subset_p (class1, class0)
	      && ! CLASS_LIKELY_SPILLED_P (class1)));
}

/* Generate and return an insn body to add r1 and c,
   storing the result in r0.  */
static rtx
gen_add3_insn (r0, r1, c)
     rtx r0, r1, c;
{
  /* CYGNUS LOCAL sh4-opt/amylaar */
  int icode = (int) add_optab->handlers[(int) GET_MODE (r0)].insn_code;
  int mcode;
  rtx s;

  if (icode == CODE_FOR_nothing
      || ! (*insn_operand_predicate[icode][0])(r0, insn_operand_mode[icode][0]))
    return NULL_RTX;

  if ((*insn_operand_predicate[icode][1])(r1, insn_operand_mode[icode][1])
      && (*insn_operand_predicate[icode][2])(c, insn_operand_mode[icode][2]))
    return (GEN_FCN (icode) (r0, r1, c));

  mcode = (int) mov_optab->handlers[(int) GET_MODE (r0)].insn_code;
  if (REGNO (r0) == REGNO (r1)
      || ! (*insn_operand_predicate[icode][1])(r0, insn_operand_mode[icode][1])
      || ! (*insn_operand_predicate[icode][2])(r1, insn_operand_mode[icode][2])
      || ! (*insn_operand_predicate[mcode][0])(r0, insn_operand_mode[mcode][0])
      || ! (*insn_operand_predicate[mcode][1])(c, insn_operand_mode[mcode][1]))
    return NULL_RTX;

  start_sequence ();
  emit_insn (GEN_FCN (mcode) (r0, c));
  emit_insn (GEN_FCN (icode) (r0, r0, r1));
  s = gen_sequence ();
  end_sequence ();
  return s;
  /* END CYGNUS LOCAL */
}


/* INC_INSN is an instruction that adds INCREMENT to REG.
   Try to fold INC_INSN as a post/pre in/decrement into INSN.
   Iff INC_INSN_SET is nonzero, inc_insn has a destination different from src.
   Return nonzero for success.  */
static int
try_auto_increment (insn, inc_insn, inc_insn_set, reg, increment, pre)
     rtx reg, insn, inc_insn ,inc_insn_set;
     HOST_WIDE_INT increment;
     int pre;
{
  enum rtx_code inc_code;

  rtx pset = single_set (insn);
  if (pset)
    {
      /* Can't use the size of SET_SRC, we might have something like
	 (sign_extend:SI (mem:QI ...  */
      rtx use = find_use_as_address (pset, reg, 0);
      if (use != 0 && use != (rtx) 1)
	{
	  int size = GET_MODE_SIZE (GET_MODE (use));
	  if (0
	      || (HAVE_POST_INCREMENT
		  && pre == 0 && (inc_code = POST_INC, increment == size))
	      || (HAVE_PRE_INCREMENT
		  && pre == 1 && (inc_code = PRE_INC, increment == size))
	      || (HAVE_POST_DECREMENT
		  && pre == 0 && (inc_code = POST_DEC, increment == -size))
	      || (HAVE_PRE_DECREMENT
		  && pre == 1 && (inc_code = PRE_DEC, increment == -size))
	  )
	    {
	      if (inc_insn_set)
		validate_change
		  (inc_insn, 
		   &SET_SRC (inc_insn_set),
		   XEXP (SET_SRC (inc_insn_set), 0), 1);
	      validate_change (insn, &XEXP (use, 0),
			       gen_rtx_fmt_e (inc_code, Pmode, reg), 1);
	      if (apply_change_group ())
		{
		  REG_NOTES (insn)
		    = gen_rtx_EXPR_LIST (REG_INC,
					 reg, REG_NOTES (insn));
		  if (! inc_insn_set)
		    {
		      PUT_CODE (inc_insn, NOTE);
		      NOTE_LINE_NUMBER (inc_insn) = NOTE_INSN_DELETED;
		      NOTE_SOURCE_FILE (inc_insn) = 0;
		    }
		  return 1;
		}
	    }
	}
    }
  return 0;
}
/* CYGNUS LOCAL SH4-OPT */
#ifdef AUTO_INC_DEC

#ifdef REGISTER_CONSTRAINTS

/* Some machines have two-address-adds and instructions that can
   use only register-indirect addressing and auto_increment, but no
   offsets.  If multiple fields of a struct are accessed more than
   once, cse will load each of the member addresses in separate registers.
   This not only costs a lot of registers, but also of instructions,
   since each add to initialize an address register must be really expanded
   into a register-register move followed by an add.
   regmove_optimize uses some heuristics to detect this case; if these
   indicate that this is likely, optimize_related_values is run once for
   the entire function.

   We build chains of uses of related values that can be satisfied with the
   same base register by taking advantage of auto-increment address modes
   instead of explicit add instructions.

   We try to link chains with disjoint lifetimes together to reduce the
   number of temporary registers and register-register copies.

   This optimization pass operates on basic blocks one at a time; it could be
   extended to work on extended basic blocks or entire functions.  */

/* For each set of values related to a common base register, we use a
   hash table which maps constant offsets to instructions.

   The instructions mapped to are those that use a register which may,
   (possibly with a change in addressing mode) differ from the initial
   value of the base register by exactly that offset after the
   execution of the instruction.
   Here we define the size of the hash table, and the hash function to use.  */
#define REL_USE_HASH_SIZE 43
#define REL_USE_HASH(I) ((I) % (HOST_WIDE_UINT) REL_USE_HASH_SIZE)

/* For each register in a set of registers that are related, we keep a
   struct related.

   u.base contains the register number of the base register (i.e. the one
   that was the source of the first three-address add for this set of
   related values).

   INSN is the instruction that initialized the register, or, for the
   base, the instruction that initialized the first non-base register.

   BASE is the register number of the base register.

   For the base register only, the member BASEINFO points to some extra data.

   'luid' here means linear uid.  We count them starting at the function
   start; they are used to avoid overlapping lifetimes.

   UPDATES is a list of instructions that set the register to a new
   value that is still related to the same base.

   When a register in a set of related values is set to something that
   is not related to the base, INVALIDATE_LUID is set to the luid of
   the instruction that does this set.  This is used to avoid re-using
   this register in an overlapping liftime for a related value.

   DEATH is first used to store the insn (if any) where the register dies.
   When the optimization is actually performed, the REG_DEAD note from
   the insn denoted by DEATH is removed.
   Thereafter, the removed death note is stored in DEATH, marking not
   only that the register dies, but also making the note available for reuse.

   We also use a struct related to keep track of registers that have been
   used for anything that we don't recognize as related values.
   The only really interesting datum for these is u.last_luid, which is
   the luid of the last reference we have seen.  These struct relateds
   are marked by a zero INSN field; most other members are not used and
   remain uninitialized.  */

struct related {
  rtx insn, reg;
  union { int base; int last_luid; } u;
  HOST_WIDE_INT offset;
  struct related *prev;
  struct update *updates;
  struct related_baseinfo *baseinfo;
  int invalidate_luid;
  rtx death;
  int reg_orig_calls_crossed, reg_set_call_tally, reg_orig_refs;
};

/* HASHTAB maps offsets to register uses with a matching MATCH_OFFSET.
   PREV_BASE points to the struct related for the previous base register
   that we currently keep track of.
   INSN_LUID is the luid of the instruction that started this set of
   related values.  */
struct related_baseinfo {
  struct rel_use *hashtab[REL_USE_HASH_SIZE];
  struct rel_use_chain *chains;
  struct related *prev_base;
  int insn_luid;
};

/* INSN is an instruction that sets a register that previously contained
   a related value to a new value that is related to the same base register.
   When the optimization is performed, we have to delete INSN.
   DEATH_INSN points to the insn (if any) where the register died that we
   set in INSN.  When we perform the optimization, the REG_DEAD note has
   to be removed from DEATH_INSN.
   PREV points to the struct update that pertains to the previous
   instruction pertaining to the same register that set it from one
   related value to another one.  */
struct update
{
  rtx insn, death_insn;
  struct update *prev;
};

struct rel_use_chain
{
  struct rel_use *chain; /* Points to first use in this chain.  */
  struct rel_use_chain *prev, *linked;
  /* Only set after the chain has been completed: */
  struct rel_use *end;  /* Last use in this chain.  */
  int start_luid, end_luid, calls_crossed;
  rtx reg; /* The register allocated for this chain.  */
  HOST_WIDE_INT match_offset; /* Offset after execution of last insn. */
};

/* ADDRP points to the place where the actual use of the related value is.
   This is commonly a memory address, and has to be set to a register
   or some auto_inc addressing of this register.
   But ADDRP is also used for all other uses of related values to
   the place where the register is inserted; we can tell that an
   unardorned register is to be inserted because no offset adjustment
   is required, hence this is handled by the same logic as register-indirect
   addressing.  The only exception to this is when SET_IN_PARALLEL is set,
   see below.
   OFFSET is the offset that is actually used in this instance, i.e.
   the value of the base register when the set of related values was
   created plus OFFSET yields the value that is used.
   This might be different from the value of the used register before
   executing INSN if we elected to use pre-{in,de}crement addressing.
   If we have the option to use post-{in,d})crement addressing, all
   choices are linked cyclically together with the SIBLING field.
   Otherwise, it's a one-link-cycle, i.e. SIBLING points at the
   struct rel_use it is a member of.
   MATCH_OFFSET is the offset that is available after the execution
   of INSN.  It is the same as OFFSET for straight register-indirect
   addressing and for pre-{in,de}crement addressing, while it differs
   for the post-{in,de}crement addressing modes.
   If SET_IN_PARALLEL is set, MATCH_OFFSET differs from OFFSET, yet
   this is no post-{in,de}crement addresing.  Rather, it is a set
   inside a PARALLEL that adds some constant to a register that holds
   one value of a set of related values that we keep track of.
   ADDRP then points only to the set destination of this set; another
   struct rel_use is used for the source of the set.  */
struct rel_use
{
  rtx insn, *addrp;
  int luid, call_tally;
  enum reg_class class;
  int set_in_parallel : 1;
  HOST_WIDE_INT offset, match_offset;
  struct rel_use *next_chain, **prev_chain_ref, *next_hash, *sibling;
};

struct related **regno_related, *rel_base_list, *unrelatedly_used;

#define rel_alloc(N) obstack_alloc(&related_obstack, (N))
#define rel_new(X) ((X) = rel_alloc (sizeof *(X)))

static struct obstack related_obstack;

/* For each integer machine mode, the minimum and maximum constant that
   can be added with a single constant.
   This is supposed to define an interval around zero; if there are
   singular points disconnected from this interval, we want to leave
   them out.  */
   
static HOST_WIDE_INT add_limits[NUM_MACHINE_MODES][2];

/* Try to find a related value with offset OFFSET from the base
   register belonging to REGNO, using a register with preferred class
   that is compatible with CLASS.  */
static struct rel_use *
lookup_related (regno, class, offset)
     int regno;
     enum reg_class class;
     HOST_WIDE_INT offset;
{
  int base = regno_related[regno]->u.base;
  int hash = REL_USE_HASH (offset);
  struct rel_use *match = regno_related[base]->baseinfo->hashtab[hash];
  for (; match; match = match->next_hash)
    {
      if (offset != match->match_offset)
	continue;
      if (match->next_chain)
	continue;
      if (regclass_compatible_p (class, match->class))
	break;
    }
  return match;
}

/* Add NEW_USE at the end of the chain that currently ends with MATCH;
   If MATCH is not set, create a new chain.
   BASE is the base register number the chain belongs to.  */
static void
rel_build_chain (new_use, match, base)
     struct rel_use *new_use, *match;
     int base;
{
  int hash;

  if (match)
    {
      struct rel_use *sibling = match;
      do
	{
	  sibling->next_chain = new_use;
	  if (sibling->prev_chain_ref)
	    *sibling->prev_chain_ref = match;
	  sibling = sibling->sibling;
	}
      while (sibling != match);
      new_use->prev_chain_ref = &match->next_chain;
      new_use->next_chain = 0;
    }
  else
    {
      struct rel_use_chain *new_chain;

      rel_new (new_chain);
      new_chain->chain = new_use;
      new_use->prev_chain_ref = &new_chain->chain;
      new_use->next_chain = 0;
      new_use->next_chain = NULL;
      new_chain->linked = 0;
      new_chain->prev = regno_related[base]->baseinfo->chains;
      regno_related[base]->baseinfo->chains = new_chain;
    }
  hash = REL_USE_HASH (new_use->offset);
  new_use->next_hash = regno_related[base]->baseinfo->hashtab[hash];
  regno_related[base]->baseinfo->hashtab[hash] = new_use;
}

/* Record the use of register ADDR in a memory reference.
   ADDRP is the memory location where the address is stored.
   SIZE is the size of the memory reference.
   PRE_OFFS is the offset that has to be added to the value in ADDR
   due to PRE_{IN,DE}CREMENT addressing in the original address; likewise,
   POST_OFFSET denotes POST_{IN,DE}CREMENT addressing.  INSN is the
   instruction that uses this address, LUID its luid, and CALL_TALLY
   the current number of calls encountered since the start of the
   function.  */
static void
rel_record_mem (addrp, addr, size, pre_offs, post_offs, insn, luid, call_tally)
     rtx *addrp, addr, insn;
     int size, pre_offs, post_offs;
     int luid, call_tally;
{
  static rtx auto_inc;
  rtx orig_addr = *addrp;
  int regno, base;
  HOST_WIDE_INT offset;
  struct rel_use *new_use, *match;
  enum reg_class class;
  int hash;

  if (GET_CODE (addr) != REG)
    abort ();
  
  regno = REGNO (addr);
  if (! regno_related[regno] || ! regno_related[regno]->insn
      || regno_related[regno]->invalidate_luid)
    return;

  regno_related[regno]->reg_orig_refs += loop_depth;

  offset = regno_related[regno]->offset += pre_offs;
  base = regno_related[regno]->u.base;

  if (! auto_inc)
    {
      push_obstacks_nochange ();
      end_temporary_allocation ();
      auto_inc = gen_rtx_PRE_INC (Pmode, addr);
      pop_obstacks ();
    }

  XEXP (auto_inc, 0) = addr;
  *addrp = auto_inc;

  rel_new (new_use);
  new_use->insn = insn;
  new_use->addrp = addrp;
  new_use->luid = luid;
  new_use->call_tally = call_tally;
  new_use->class = class = reg_preferred_class (regno);
  new_use->set_in_parallel = 0;
  new_use->offset = offset;
  new_use->match_offset = offset;
  new_use->sibling = new_use;

  do
    {
      match = lookup_related (regno, class, offset);
      if (! match)
	{
	  /* We can choose PRE_{IN,DE}CREMENT on the spot with the information
	     we have gathered about the preceding instructions, while we have
	     to record POST_{IN,DE}CREMENT possibilities so that we can check
	     later if we have a use for their output value.  */
	  /* We use recog here directly because we are only testing here if
	     the changes could be made, but don't really want to make a
	     change right now.  The caching from recog_memoized would only
	     get in the way.  */
	  match = lookup_related (regno, class, offset - size);
	  if (HAVE_PRE_INCREMENT && match)
	    {
	      PUT_CODE (auto_inc, PRE_INC);
	      if (recog (PATTERN (insn), insn, NULL) >= 0)
		break;
	    }
	  match = lookup_related (regno, class, offset + size);
	  if (HAVE_PRE_DECREMENT && match)
	    {
	      PUT_CODE (auto_inc, PRE_DEC);
	      if (recog (PATTERN (insn), insn, NULL) >= 0)
		break;
	    }
	  match = 0;
	}
      PUT_CODE (auto_inc, POST_INC);
      if (HAVE_POST_INCREMENT && recog (PATTERN (insn), insn, NULL) >= 0)
	{
	  struct rel_use *inc_use;

	  rel_new (inc_use);
	  *inc_use = *new_use;
	  inc_use->sibling = new_use;
	  new_use->sibling = inc_use;
	  inc_use->prev_chain_ref = NULL;
	  inc_use->next_chain = NULL;
	  hash = REL_USE_HASH (inc_use->match_offset = offset + size);
	  inc_use->next_hash = regno_related[base]->baseinfo->hashtab[hash];
	  regno_related[base]->baseinfo->hashtab[hash] = inc_use;
	}
      PUT_CODE (auto_inc, POST_DEC);
      if (HAVE_POST_DECREMENT && recog (PATTERN (insn), insn, NULL) >= 0)
	{
	  struct rel_use *dec_use;

	  rel_new (dec_use);
	  *dec_use = *new_use;
	  dec_use->sibling = new_use->sibling;
	  new_use->sibling = dec_use;
	  dec_use->prev_chain_ref = NULL;
	  dec_use->next_chain = NULL;
	  hash = REL_USE_HASH (dec_use->match_offset = offset + size);
	  dec_use->next_hash = regno_related[base]->baseinfo->hashtab[hash];
	  regno_related[base]->baseinfo->hashtab[hash] = dec_use;
	}
    }
  while (0);
  rel_build_chain (new_use, match, base);
  *addrp = orig_addr;

  regno_related[regno]->offset += post_offs;
}

/* Note that REG is set to something that we do not regognize as a
   related value, at an insn with linear uid LUID.  */
static void
invalidate_related (reg, luid)
     rtx reg;
     int luid;
{
  int regno = REGNO (reg);
  struct related *rel = regno_related[regno];
  if (! rel)
    {
      rel_new (rel);
      regno_related[regno] = rel;
      rel->prev = unrelatedly_used;
      unrelatedly_used = rel;
      rel->reg = reg;
      rel->insn = NULL_RTX;
      rel->invalidate_luid = 0;
      rel->u.last_luid = luid;
    }
  else if (rel->invalidate_luid)
    ; /* do nothing */
  else if (! rel->insn)
    rel->u.last_luid = luid;
  else
    rel->invalidate_luid = luid;
}

/* Check the RTL fragment pointed to by XP for related values - that is,
   if any new are created, or if they are assigned new values.  Also
   note any other sets so that we can track lifetime conflicts.
   INSN is the instruction XP points into, LUID its luid, and CALL_TALLY
   the number of preceding calls in the function.  */
static void
find_related (xp, insn, luid, call_tally)
     rtx *xp, insn;
     int luid, call_tally;
{
  rtx x = *xp;
  enum rtx_code code = GET_CODE (x);
  char *fmt;
  int i;

  switch (code)
    {
    case SET:
      {
	rtx dst = SET_DEST (x);
	rtx src = SET_SRC (x);

	/* First, check out if this sets a new related value.
	   We don't care about register class differences here, since
	   we might still find multiple related values share the same
	   class even if it is disjunct from the class of the original
	   register.
	   We use a do .. while (0);  here because there are many possible
	   conditions that make us want to handle this like an ordinary set.  */
	do
	  {
	    rtx src_reg, src_const;
	    int src_regno, dst_regno;
	    struct related *new_related;

	    /* First check that we have actually something like
	       (set (reg pseudo_dst) (plus (reg pseudo_src) (const_int))) .  */
	    if (GET_CODE (src) != PLUS)
	      break;
	    src_reg = XEXP (src, 0);
	    src_const = XEXP (src, 1);
	    if (GET_CODE (src_reg) != REG
		|| GET_CODE (src_const) != CONST_INT
		|| GET_CODE (dst) != REG)
	      break;
	    dst_regno = REGNO (dst);
	    src_regno = REGNO (src_reg);
	    if (src_regno < FIRST_PSEUDO_REGISTER
		|| dst_regno < FIRST_PSEUDO_REGISTER)
	      break;

	    /* We only know how to remove the set if that is
	       all what the insn does.  */
	    if (x != single_set (insn))
	      break;

	    /* We cannot handle multiple lifetimes.  */
	    if ((regno_related[src_regno]
		 && regno_related[src_regno]->invalidate_luid)
		|| (regno_related[dst_regno]
		    && regno_related[dst_regno]->invalidate_luid))
	      break;

	    /* Check if this is merely an update of a register with a
	       value belonging to a group of related values we already
	       track.  */
	    if (regno_related[dst_regno] && regno_related[dst_regno]->insn)
	      {
		struct update *new_update;

		/* If the base register changes, don't handle this as a
		   related value.  We can currently only attribute the
		   register to one base, and keep record of one lifetime
		   during which we might re-use the register.  */
		if (! regno_related[src_regno]
		    || ! regno_related[src_regno]->insn
		    ||(regno_related[dst_regno]->u.base
		       != regno_related[src_regno]->u.base))
		  break;
		regno_related[src_regno]->reg_orig_refs += loop_depth;
		regno_related[dst_regno]->reg_orig_refs += loop_depth;
		regno_related[dst_regno]->offset
		  = regno_related[src_regno]->offset + INTVAL (XEXP (src, 1));
		rel_new (new_update);
		new_update->insn = insn;
		new_update->death_insn = regno_related[dst_regno]->death;
		regno_related[dst_regno]->death = NULL_RTX;
		new_update->prev = regno_related[dst_regno]->updates;
		regno_related[dst_regno]->updates = new_update;
		return;
	      }
	    if (! regno_related[src_regno]
		|| ! regno_related[src_regno]->insn)
	      {
		if (src_regno == dst_regno)
		  break;
		rel_new (new_related);
		new_related->reg = src_reg;
		new_related->insn = insn;
		new_related->updates = 0;
		new_related->reg_set_call_tally = call_tally;
		new_related->reg_orig_refs = loop_depth;
		new_related->u.base = src_regno;
		new_related->offset = 0;
		new_related->prev = 0;
		new_related->invalidate_luid = 0;
		new_related->death = NULL_RTX;
		rel_new (new_related->baseinfo);
		zero_memory ((char *) new_related->baseinfo,
		       sizeof *new_related->baseinfo);
		new_related->baseinfo->prev_base = rel_base_list;
		rel_base_list = new_related;
		new_related->baseinfo->insn_luid = luid;
		regno_related[src_regno] = new_related;
	      }
	    /* If the destination register has been used since we started
	       tracking this group of related values, there would be tricky
	       lifetime problems that we don't want to tackle right now.  */
	    else if (regno_related[dst_regno]
		     && (regno_related[dst_regno]->u.last_luid
			 >= regno_related[regno_related[src_regno]->u.base]->baseinfo->insn_luid))
	      break;
	    rel_new (new_related);
	    new_related->reg = dst;
	    new_related->insn = insn;
	    new_related->updates = 0;
	    new_related->reg_set_call_tally = call_tally;
	    new_related->reg_orig_refs = loop_depth;
	    new_related->u.base = regno_related[src_regno]->u.base;
	    new_related->offset =
	      regno_related[src_regno]->offset + INTVAL (XEXP (src, 1));
	    new_related->invalidate_luid = 0;
	    new_related->death = NULL_RTX;
	    new_related->prev = regno_related[src_regno]->prev;
	    regno_related[src_regno]->prev = new_related;
	    regno_related[dst_regno] = new_related;
	    return;
	  }
	while (0);

	/* The SET has not been recognized as setting up a related value.
	   If the destination is ultimately a register, we have to
	   invalidate what we have memorized about any related value
	   previously stored into it.  */
	while (GET_CODE (dst) == SUBREG
	       || GET_CODE (dst) == ZERO_EXTRACT
	       || GET_CODE (dst) == SIGN_EXTRACT
	       || GET_CODE (dst) == STRICT_LOW_PART)
	  dst = XEXP (dst, 0);
	if (GET_CODE (dst) == REG)
	  {
	    find_related (&SET_SRC (x), insn, luid, call_tally);
	    invalidate_related (dst, luid);
	    return;
	  }
	break;
      }
    case CLOBBER:
      {
	rtx dst = XEXP (x, 0);
	while (GET_CODE (dst) == SUBREG
	       || GET_CODE (dst) == ZERO_EXTRACT
	       || GET_CODE (dst) == SIGN_EXTRACT
	       || GET_CODE (dst) == STRICT_LOW_PART)
	  dst = XEXP (dst, 0);
	if (GET_CODE (dst) == REG)
	  {
	    int regno = REGNO (dst);
	    struct related *rel = regno_related[regno];

	    /* If this clobbers a register that belongs to a set of related
	       values, we have to check if the same register appears somewhere
	       else in the insn : this is then likely to be a match_dup.  */
		
	    if (rel
		&& rel->insn
		&& ! rel->invalidate_luid
		&& xp != &PATTERN (insn)
		&& count_occurrences (PATTERN (insn), dst) > 1)
	      {
		enum reg_class class = reg_preferred_class (regno);
		struct rel_use *new_use, *match;
		HOST_WIDE_INT offset = rel->offset;

		rel_new (new_use);
		new_use->insn = insn;
		new_use->addrp = &XEXP (x, 0);
		new_use->luid = luid;
		new_use->call_tally = call_tally;
		new_use->class = class;
		new_use->set_in_parallel = 1;
		new_use->sibling = new_use;
		do
		  {
		    new_use->match_offset = new_use->offset = offset;
		    match = lookup_related (regno, class, offset);
		    offset++;
		  }
		while (! match || match->luid != luid);
		rel_build_chain (new_use, match, rel->u.base);
		/* Prevent other registers from using the same chain.  */
		new_use->next_chain = new_use;
	      }
	    invalidate_related (dst, luid);
	    return;
	  }
	break;
      }
    case REG:
      {
	int regno = REGNO (x);
	if (! regno_related[regno])
	  {
	    rel_new (regno_related[regno]);
	    regno_related[regno]->prev = unrelatedly_used;
	    unrelatedly_used = regno_related[regno];
	    regno_related[regno]->reg = x;
	    regno_related[regno]->insn = NULL_RTX;
	    regno_related[regno]->u.last_luid = luid;
	  }
	else if (! regno_related[regno]->insn)
	  regno_related[regno]->u.last_luid = luid;
	else if (! regno_related[regno]->invalidate_luid)
	  {
	    struct rel_use *new_use, *match;
	    HOST_WIDE_INT offset;
	    int base;
	    enum reg_class class;

	    regno_related[regno]->reg_orig_refs += loop_depth;

	    offset = regno_related[regno]->offset;
	    base = regno_related[regno]->u.base;

	    rel_new (new_use);
	    new_use->insn = insn;
	    new_use->addrp = xp;
	    new_use->luid = luid;
	    new_use->call_tally = call_tally;
	    new_use->class = class = reg_preferred_class (regno);
	    new_use->set_in_parallel = 0;
	    new_use->offset = offset;
	    new_use->match_offset = offset;
	    new_use->sibling = new_use;

	    match = lookup_related (regno, class, offset);
	    rel_build_chain (new_use, match, base);
	  }
	return;
      }
    case MEM:
      {
	int size = GET_MODE_SIZE (GET_MODE (x));
	rtx *addrp= &XEXP (x, 0), addr = *addrp;

	switch (GET_CODE (addr))
	  {
	  case REG:
	    rel_record_mem (addrp, addr, size, 0, 0,
			    insn, luid, call_tally);
	    return;
	  case PRE_INC:
	    rel_record_mem (addrp, XEXP (addr, 0), size, size, 0,
			    insn, luid, call_tally);
	    return;
	  case POST_INC:
	    rel_record_mem (addrp, XEXP (addr, 0), size, 0, size,
			    insn, luid, call_tally);
	    return;
	  case PRE_DEC:
	    rel_record_mem (addrp, XEXP (addr, 0), size, -size, 0,
			    insn, luid, call_tally);
	    return;
	  case POST_DEC:
	    rel_record_mem (addrp, XEXP (addr, 0), size, 0, -size,
			    insn, luid, call_tally);
	    return;
	  default:
	    break;
	  }
	break;
      }
    case PARALLEL:
      {
	for (i = XVECLEN (x, 0) - 1; i >= 0; i--)
	  {
	    rtx *yp = &XVECEXP (x, 0, i);
	    rtx y = *yp;
	    if (GET_CODE (y) == SET)
	      {
		rtx dst;

		find_related (&SET_SRC (y), insn, luid, call_tally);
		dst = SET_DEST (y);
		while (GET_CODE (dst) == SUBREG
		       || GET_CODE (dst) == ZERO_EXTRACT
		       || GET_CODE (dst) == SIGN_EXTRACT
		       || GET_CODE (dst) == STRICT_LOW_PART)
		  dst = XEXP (dst, 0);
		if (GET_CODE (dst) != REG)
		  find_related (&SET_DEST (y), insn, luid, call_tally);
	      }
	    else if (GET_CODE (y) != CLOBBER)
	      find_related (yp, insn, luid, call_tally);
	  }
	for (i = XVECLEN (x, 0) - 1; i >= 0; i--)
	  {
	    rtx *yp = &XVECEXP (x, 0, i);
	    rtx y = *yp;
	    if (GET_CODE (y) == SET)
	      {
		rtx *dstp;

		dstp = &SET_DEST (y);
		while (GET_CODE (*dstp) == SUBREG
		       || GET_CODE (*dstp) == ZERO_EXTRACT
		       || GET_CODE (*dstp) == SIGN_EXTRACT
		       || GET_CODE (*dstp) == STRICT_LOW_PART)
		  dstp = &XEXP (*dstp, 0);
		if (GET_CODE (*dstp) == REG)
		  {
		    int regno = REGNO (*dstp);
		    rtx src = SET_SRC (y);
		    if (regno_related[regno] && regno_related[regno]->insn
			&& GET_CODE (src) == PLUS
			&& XEXP (src, 0) == *dstp
			&& GET_CODE (XEXP (src, 1)) == CONST_INT)
		      {
			struct rel_use *new_use, *match;
			enum reg_class class;

			regno_related[regno]->reg_orig_refs += loop_depth;
			rel_new (new_use);
			new_use->insn = insn;
			new_use->addrp = dstp;
			new_use->luid = luid;
			new_use->call_tally = call_tally;
			new_use->class = class = reg_preferred_class (regno);
			new_use->set_in_parallel = 1;
			new_use->offset = regno_related[regno]->offset;
			new_use->match_offset
			  = regno_related[regno]->offset
			  += INTVAL (XEXP (src, 1));
			new_use->sibling = new_use;
			match = lookup_related (regno, class, new_use->offset);
			rel_build_chain (new_use, match,
					 regno_related[regno]->u.base);
		      }
		    else
		      invalidate_related (*dstp, luid);
		  }
	      }
	    else if (GET_CODE (y) == CLOBBER)
	      find_related (yp, insn, luid, call_tally);
	  }
	return;
      }
    default:
      break;
    }
  fmt = GET_RTX_FORMAT (code);

  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	find_related (&XEXP (x, i), insn, luid, call_tally);
      if (fmt[i] == 'E')
	{
	  register int j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    find_related (&XVECEXP (x, i, j), insn, luid, call_tally);
	}
    }
}

/* Comparison functions for qsort.  */
static int
chain_starts_earlier (chain1, chain2)
     const void * chain1;
     const void * chain2;
{
  int d = ((*(struct rel_use_chain **)chain2)->start_luid
	   - (*(struct rel_use_chain **)chain1)->start_luid);
  if (! d)
    d = ((*(struct rel_use_chain **)chain2)->chain->offset
         - (*(struct rel_use_chain **)chain1)->chain->offset);
  if (! d)
    d = ((*(struct rel_use_chain **)chain2)->chain->set_in_parallel
         - (*(struct rel_use_chain **)chain1)->chain->set_in_parallel);
  /* If set_in_parallel is not set on both chain's first use, they must
     differ in start_luid or offset, since otherwise they would use the
     same chain.
     Thus the remaining problem is with set_in_parallel uses; for these, we
     know that *addrp is a register.  Since the same register may not be set
     multiple times in the same insn, the registers must be different.  */
     
  if (! d)
    d = (REGNO (*(*(struct rel_use_chain **)chain2)->chain->addrp)
         - REGNO (*(*(struct rel_use_chain **)chain1)->chain->addrp));
  return d;
}

static int
chain_ends_later (chain1, chain2)
     const void * chain1;
     const void * chain2;
{
  int d = ((*(struct rel_use_chain **)chain1)->end_luid
	   - (*(struct rel_use_chain **)chain2)->end_luid);
  if (! d)
    d = ((*(struct rel_use_chain **)chain2)->chain->offset
         - (*(struct rel_use_chain **)chain1)->chain->offset);
  if (! d)
    d = ((*(struct rel_use_chain **)chain2)->chain->set_in_parallel
         - (*(struct rel_use_chain **)chain1)->chain->set_in_parallel);
  /* If set_in_parallel is not set on both chain's first use, they must
     differ in start_luid or offset, since otherwise they would use the
     same chain.
     Thus the remaining problem is with set_in_parallel uses; for these, we
     know that *addrp is a register.  Since the same register may not be set
     multiple times in the same insn, the registers must be different.  */
     
  if (! d)
    d = (REGNO (*(*(struct rel_use_chain **)chain2)->chain->addrp)
         - REGNO (*(*(struct rel_use_chain **)chain1)->chain->addrp));
  return d;
}

static void
count_sets (x, pat)
    rtx x, pat;
{
  if (GET_CODE (x) == REG)
    REG_N_SETS (REGNO (x))++;
}

/* Perform the optimization for a single set of related values.
   INSERT_AFTER is an instruction after which we may emit instructions
   to initialize registers that remain live beyond the end of the group
   of instructions which have been examined.  */
static struct related *
optimize_related_values_1 (rel_base, luid, call_tally, insert_after,
			   regmove_dump_file)
     struct related *rel_base;
     int luid, call_tally;
     rtx insert_after;
     FILE *regmove_dump_file;
{
  struct related_baseinfo *baseinfo = rel_base->baseinfo;
  struct related *rel;
  struct rel_use_chain *chain, *chain0, **chain_starttab, **chain_endtab;
  struct rel_use_chain **pred_chainp, *pred_chain, *last_init_chain;
  int num_regs, num_av_regs, num_chains, num_linked, max_end_luid, i;
  struct rel_use_chain *rel_base_reg_user;
  int mode;
  HOST_WIDE_INT rel_base_reg_user_offset = 0;

  /* For any registers that are still live, we have to arrange
     to have them set to their proper values.
     Also count with how many registers (not counting base) we are
     dealing with here.  */
  for (num_regs = -1, rel = rel_base; rel; rel = rel->prev, num_regs++)
    {
      int regno = REGNO (rel->reg);

      if (! rel->death
	  && ! rel->invalidate_luid)
	{
	  enum reg_class class = reg_preferred_class (regno);
	  struct rel_use *new_use, *match;

	  rel_new (new_use);
	  new_use->insn = NULL_RTX;
	  new_use->addrp = &rel->reg;
	  new_use->luid = luid;
	  new_use->call_tally = call_tally;
	  new_use->class = class;
	  new_use->set_in_parallel = 1;
	  new_use->match_offset = new_use->offset = rel->offset;
	  new_use->sibling = new_use;
	  match = lookup_related (regno, class, rel->offset);
	  rel_build_chain (new_use, match, REGNO (rel_base->reg));
	  /* Prevent other registers from using the same chain.  */
	  new_use->next_chain = new_use;
	}

      if (! rel->death)
	rel->reg_orig_calls_crossed = call_tally - rel->reg_set_call_tally;
    }

  /* Now for every chain of values related to the base, set start
     and end luid, match_offset, and reg.  Also count the number of these
     chains, and determine the largest end luid.  */
  num_chains = 0;
  for (max_end_luid = 0, chain = baseinfo->chains; chain; chain = chain->prev)
    {
      struct rel_use *use, *next;

      num_chains++;
      next = chain->chain;
      chain->start_luid = next->luid;
      do
	{
	  use = next;
	  next = use->next_chain;
	}
      while (next && next != use);
      use->next_chain = 0;
      chain->end = use;
      chain->end_luid = use->luid;
      chain->match_offset = use->match_offset;
      chain->calls_crossed = use->call_tally - chain->chain->call_tally;
      
      chain->reg = use->insn ? NULL_RTX : *use->addrp;

      if (use->luid > max_end_luid)
	max_end_luid = use->luid;

      if (regmove_dump_file)
	fprintf (regmove_dump_file, "Chain start: %d end: %d\n",
		 chain->start_luid, chain->end_luid);
    }

  if (regmove_dump_file)
    fprintf (regmove_dump_file,
	     "Insn %d reg %d: found %d chains.\n",
	     INSN_UID (rel_base->insn), REGNO (rel_base->reg), num_chains);

  /* For every chain, we try to find another chain the lifetime of which
     ends before the lifetime of said chain starts.
     So we first sort according to luid of first and last instruction that
     is in the chain, respectively;  this is O(n * log n) on average.  */
  chain_starttab = rel_alloc (num_chains * sizeof *chain_starttab);
  chain_endtab = rel_alloc (num_chains * sizeof *chain_starttab);
  for (chain = baseinfo->chains, i = 0; chain; chain = chain->prev, i++)
    {
      chain_starttab[i] = chain;
      chain_endtab[i] = chain;
    }
  qsort (chain_starttab, num_chains, sizeof *chain_starttab,
	 chain_starts_earlier);
  qsort (chain_endtab, num_chains, sizeof *chain_endtab, chain_ends_later);

  /* Now we go through every chain, starting with the one that starts
     second (we can skip the first because we know there would be no match),
     and check it against the chain that ends first.  */
  /* ??? We assume here that reg_class_compatible_p will seldom return false.
     If that is not true, we should do a more thorough search for suitable
     chain combinations.  */
  pred_chainp = chain_endtab;
  pred_chain = *pred_chainp;
  for (num_linked = 0, i = num_chains - 2; i >= 0; i--)
    {
      struct rel_use_chain *succ_chain = chain_starttab[i];
      if (succ_chain->start_luid > pred_chain->end_luid
	  && (pred_chain->calls_crossed
	      ? succ_chain->calls_crossed
	      : succ_chain->end->call_tally == pred_chain->chain->call_tally)
	  && regclass_compatible_p (succ_chain->chain->class,
				     pred_chain->chain->class)
	  /* add_limits is not valid for MODE_PARTIAL_INT .  */
	  && GET_MODE_CLASS (GET_MODE (rel_base->reg)) == MODE_INT
	  && (succ_chain->chain->offset - pred_chain->match_offset
	      >= add_limits[(int) GET_MODE (rel_base->reg)][0])
	  && (succ_chain->chain->offset - pred_chain->match_offset
	      <= add_limits[(int) GET_MODE (rel_base->reg)][1]))
	{
	  /* We can link these chains together.  */
	  pred_chain->linked = succ_chain;
	  succ_chain->start_luid = 0;
	  num_linked++;

	  pred_chain = *++pred_chainp;
	}
    }

  if (regmove_dump_file && num_linked)
    fprintf (regmove_dump_file, "Linked to %d sets of chains.\n",
	     num_chains - num_linked);

  /* Now count the number of registers that are available for reuse.  */
  /* ??? In rare cases, we might reuse more if we took different
     end luids of the chains into account.  Or we could just allocate
     some new regs.  But that would probably not be worth the effort.  */
  /* ??? We should pay attention to preferred register classes here to,
     if the to-be-allocated register have a life outside the range that
     we handle.  */
  for (num_av_regs = 0, rel = rel_base; rel; rel = rel->prev)
    {
      if (! rel->invalidate_luid
	  || rel->invalidate_luid > max_end_luid)
	num_av_regs++;
    }

    /* Propagate mandatory register assignments to the first chain in
       all sets of liked chains, and set rel_base_reg_user.  */
    for (rel_base_reg_user = 0, i = 0; i < num_chains; i++)
      {
        struct rel_use_chain *chain = chain_starttab[i];
        if (chain->linked)
          chain->reg = chain->linked->reg;
        if (chain->reg == rel_base->reg)
          rel_base_reg_user = chain;
      }
    /* If rel_base->reg is not a mandatory allocated register, allocate
       it to that chain that starts first and has no allocated register,
       and that allows the addition of the start value in a single
       instruction.  */
    mode = (int) GET_MODE (rel_base->reg);
    if (! rel_base_reg_user)
      {
        for ( i = num_chains - 1; i >= 0; --i)
          {
            struct rel_use_chain *chain = chain_starttab[i];
            if (! chain->reg
		&& chain->start_luid
                && chain->chain->offset >= add_limits[mode][0]
                && chain->chain->offset <= add_limits[mode][1])
              {
                chain->reg = rel_base->reg;
                rel_base_reg_user = chain;
                break;
              }
          }
      }
  else
    rel_base_reg_user_offset = rel_base_reg_user->chain->offset;

  /* Now check if it is worth doing this optimization after all.
     Using separate registers per value, like in the code generated by cse,
     costs two instructions per register (one move and one add).
     Using the chains we have set up, we need two instructions for every
     linked set of chains, plus one instruction for every link.
     We do the optimization if we save instructions, or if we
     stay with the same number of instructions, but save registers.
     We also require that we have enough registers available for reuse.
     Moreover, we have to check that we can add the offset for
     rel_base_reg_user, in case it is a mandatory allocated register.  */
  if (2 * num_regs > 2 * num_chains - num_linked - (num_linked != 0)
      && num_av_regs - (! rel_base_reg_user) >= num_chains - num_linked
      && rel_base_reg_user_offset  >= add_limits[mode][0]
      && rel_base_reg_user_offset  <= add_limits[mode][1])
    {
      /* Hold the current offset between the initial value of rel_base->reg
	 and the current value of rel_base->rel before the instruction
	 that starts the current set of chains.  */
      int base_offset = 0;
      /* The next use of rel_base->reg that we have to look out for.  */
      struct rel_use *base_use;
      /* Pointer to next insn where we look for it.  */
      rtx base_use_scan = 0;
      int base_last_use_call_tally = rel_base->reg_set_call_tally;
      int base_regno;
      int base_seen;

      if (regmove_dump_file)
	fprintf (regmove_dump_file, "Optimization is worth while.\n");

      /* First, remove all the setting insns, death notes
	 and refcount increments that are now obsolete.  */
      for (rel = rel_base; rel; rel = rel->prev)
	{
	  struct update *update;
	  int regno = REGNO (rel->reg);

	  if (rel != rel_base)
	    {
	      /* The first setting insn might be the start of a basic block.  */
	      if (rel->insn == rel_base->insn
		  /* We have to preserve insert_after.  */
		  || rel->insn == insert_after)
		{
		  PUT_CODE (rel->insn, NOTE);
		  NOTE_LINE_NUMBER (rel->insn) = NOTE_INSN_DELETED;
		  NOTE_SOURCE_FILE (rel->insn) = 0;
		}
	      else
		delete_insn (rel->insn);
	      REG_N_SETS (regno)--;
	    }
	  REG_N_CALLS_CROSSED (regno) -= rel->reg_orig_calls_crossed;
	  for (update = rel->updates; update; update = update->prev)
	    {
	      rtx death_insn = update->death_insn;
	      if (death_insn)
		{
		  rtx death_note
		    = find_reg_note (death_insn, REG_DEAD, rel->reg);
		  if (! death_note)
		    death_note
		      = find_reg_note (death_insn, REG_UNUSED, rel->reg);
		  remove_note (death_insn, death_note);
		  REG_N_DEATHS (regno)--;
		}
	      /* We have to preserve insert_after.  */
	      if (rel->insn == insert_after)
		{
		  PUT_CODE (update->insn, NOTE);
		  NOTE_LINE_NUMBER (update->insn) = NOTE_INSN_DELETED;
		  NOTE_SOURCE_FILE (update->insn) = 0;
		}
	      else
		delete_insn (update->insn);
	      REG_N_SETS (regno)--;
	    }
	  if (rel->death)
	    {
	      rtx death_note = find_reg_note (rel->death, REG_DEAD, rel->reg);
	      if (! death_note)
		death_note = find_reg_note (rel->death, REG_UNUSED, rel->reg);
	      remove_note (rel->death, death_note);
	      rel->death = death_note;
	      REG_N_DEATHS (regno)--;
	    }
	}
      /* Go through all the chains and install the planned changes.  */
      rel = rel_base;
      if (rel_base_reg_user)
	{
	  base_use = rel_base_reg_user->chain;
	  base_use_scan = chain_starttab[num_chains - 1]->chain->insn;
	}
      for (i = 0; ! chain_starttab[i]->start_luid; i++);
      last_init_chain = chain_starttab[i];
      base_regno = REGNO (rel_base->reg);
      base_seen = 0;
      for (i = num_chains - 1; i >= 0; i--)
	{
	  int first_call_tally;
	  rtx reg;
	  int regno;
	  struct rel_use *use, *last_use;

	  chain0 = chain_starttab[i];
	  if (! chain0->start_luid)
	    continue;
	  first_call_tally = chain0->chain->call_tally;
	  reg = chain0->reg;
	  /* If this chain has not got a register yet, assign one.  */
	  if (! reg)
	    {
	      do
		rel = rel->prev;
	      while (! rel->death
		     || (rel->invalidate_luid
			 && rel->invalidate_luid <= max_end_luid));
	      reg = rel->reg;
	    }
	  regno = REGNO (reg);

	  use = chain0->chain;

	  /* Update base_offset.  */
	  if (rel_base_reg_user)
	    {
	      rtx use_insn = use->insn;
	      rtx base_use_insn = base_use->insn;

	      if (! use_insn)
		use_insn = insert_after;

	      while (base_use_scan != use_insn)
		{
		  if (base_use_scan == base_use_insn)
		    {
		      base_offset = base_use->match_offset;
		      base_use = base_use->next_chain;
		      if (! base_use)
			{
			  rel_base_reg_user = rel_base_reg_user->linked;
			  if (! rel_base_reg_user)
			    break;
			  base_use = rel_base_reg_user->chain;
			}
		      base_use_insn = base_use->insn;
		    }
		  base_use_scan = NEXT_INSN (base_use_scan);
		}
	      /* If we are processing the start of a chain that starts with
		 an instruction that also uses the base register, (that happens
		 only if USE_INSN contains multiple distinct, but related
		 values) and the chains using the base register have already
		 been processed, the initializing instruction of the new
		 register will end up after the adjustment of the base
		 register.  */
	      if (use_insn == base_use_insn && base_seen)
		base_offset = base_use->offset;
	    }
	  if (regno == base_regno)
	    base_seen = 1;
	  if (regno != base_regno || use->offset - base_offset)
	    {
	      rtx add;
	      add = gen_add3_insn (reg, rel_base->reg,
				       GEN_INT (use->offset - base_offset));
	      if (! add)
		abort ();
	      if (GET_CODE (add) == SEQUENCE)
		{
		  int i;

		  for (i = XVECLEN (add, 0) - 1; i >= 0; i--)
		    note_stores (PATTERN (XVECEXP (add, 0, i)), count_sets);
		}
	      else
		note_stores (add, count_sets);
	      if (use->insn)
		add = emit_insn_before (add, use->insn);
	      else
		add = emit_insn_after (add, insert_after);
	      if (use->call_tally > base_last_use_call_tally)
		base_last_use_call_tally = use->call_tally;
	      /* If this is the last reg initializing insn, and we
		 still have to place a death note for the base reg,
		 attach it to this insn -
		 unless we are still using the base register.  */
	      if (chain0 == last_init_chain
		  && rel_base->death
		  && regno != base_regno)
		{
	          XEXP (rel_base->death, 0) = rel_base->reg;
	          XEXP (rel_base->death, 1) = REG_NOTES (add);
	          REG_NOTES (add) = rel_base->death;
	          REG_N_DEATHS (base_regno)++;
		}
	    }
	  for (last_use = 0, chain = chain0; chain; chain = chain->linked)
	    {
	      int last_offset;

	      use = chain->chain;
	      if (last_use)
		{
		  rtx add
		    = gen_add3_insn (reg, reg,
				     GEN_INT (use->offset - last_use->offset));
		  if (! add)
		    abort ();
		  if (use->insn)
		    emit_insn_before (add, use->insn);
		  else
		    {
		      /* Set use->insn, so that base_offset will be adjusted
			 in time if REG is REL_BASE->REG .  */
		      use->insn = emit_insn_after (add, last_use->insn);
		    }
		  REG_N_SETS (regno)++;
		}
	      for (last_offset = use->offset; use; use = use->next_chain)
		{
		  rtx addr;
		  int use_offset;

		  addr = *use->addrp;
		  if (GET_CODE (addr) != REG)
		    remove_note (use->insn,
				 find_reg_note (use->insn, REG_INC,
						XEXP (addr, 0)));
		  use_offset = use->offset;
		  if (use_offset == last_offset)
		    {
		      if (use->set_in_parallel)
			{
			  REG_N_SETS (REGNO (addr))--;
			  addr = reg;
			}
		      else if (use->match_offset > use_offset)
			addr = gen_rtx_POST_INC (Pmode, reg);
		      else if (use->match_offset < use_offset)
			addr = gen_rtx_POST_DEC (Pmode, reg);
		      else
			addr = reg;
		    }
		  else if (use_offset > last_offset)
		    addr = gen_rtx_PRE_INC (Pmode, reg);
		  else
		    addr = gen_rtx_PRE_DEC (Pmode, reg);
		  /* Group changes from the same chain for the same insn
		     together, to avoid failures for match_dups.  */
		  validate_change (use->insn, use->addrp, addr, 1);
		  if ((! use->next_chain || use->next_chain->insn != use->insn)
		      && ! apply_change_group ())
		    abort ();
		  if (addr != reg)
		    REG_NOTES (use->insn)
		      = gen_rtx_EXPR_LIST (REG_INC, reg, REG_NOTES (use->insn));
		  last_use = use;
		  last_offset = use->match_offset;
		}
	    }
	  /* If REG dies, attach its death note to the last using insn in
	     the set of linked chains we just handled.
	     However, if REG is the base register, don't do this if there
	     will be a later initializing instruction for another register.
	     The initializing instruction for last_init_chain will be inserted
	     before last_init_chain->chain->insn, so if the luids (and hence
	     the insns these stand for) are equal, put the death note here.  */
	  if (reg == rel->reg
	      && rel->death
	      && (rel != rel_base
		  || last_use->luid >= last_init_chain->start_luid))
	    {
	      XEXP (rel->death, 0) = reg;
	      PUT_MODE (rel->death, (reg_set_p (reg, PATTERN (last_use->insn))
				     ? REG_UNUSED : REG_DEAD));
	      XEXP (rel->death, 1) = REG_NOTES (last_use->insn);
	      REG_NOTES (last_use->insn) = rel->death;
	      /* Mark this death as 'used up'.  That is important for the
	        base register.  */
	      rel->death = NULL_RTX;
	      REG_N_DEATHS (regno)++;
	    }
	  if (regno == base_regno)
	    base_last_use_call_tally = last_use->call_tally;
	  else
	    REG_N_CALLS_CROSSED (regno)
	      += last_use->call_tally - first_call_tally;
	}

      REG_N_CALLS_CROSSED (base_regno) +=
	base_last_use_call_tally - rel_base->reg_set_call_tally;
    }

  /* Finally, clear the entries that we used in regno_related.  We do it
     item by item here, because doing it with zero_memory for each basic block
     would give O(n*n) time complexity.  */
  for (rel = rel_base; rel; rel = rel->prev)
    regno_related[REGNO (rel->reg)] = 0;
  return baseinfo->prev_base;
}

/* Finalize the optimization for any related values know so far, and reset
   the entries in regno_related that we have disturbed.  */
static void
optimize_related_values_0 (rel_base_list, luid, call_tally, insert_after,
			   regmove_dump_file)
     struct related *rel_base_list;
     int luid, call_tally;
     rtx insert_after;
     FILE *regmove_dump_file;
{
  while (rel_base_list)
    rel_base_list
      = optimize_related_values_1 (rel_base_list, luid, call_tally,
				   insert_after, regmove_dump_file);
  for ( ; unrelatedly_used; unrelatedly_used = unrelatedly_used->prev)
    regno_related[REGNO (unrelatedly_used->reg)] = 0;
}

/* Scan the entire function for instances where multiple registers are
   set to values that differ only by a constant.
   Then try to reduce the number of instructions and/or registers needed
   by exploiting auto_increment and true two-address additions.  */
   
static void
optimize_related_values (nregs, regmove_dump_file)
     int nregs;
     FILE *regmove_dump_file;
{
  int b;
  rtx insn;
  int luid = 0;
  int call_tally = 0;
  int save_loop_depth = loop_depth;
  enum machine_mode mode;

  if (regmove_dump_file)
    fprintf (regmove_dump_file, "Starting optimize_related_values.\n");

  /* For each integer mode, find minimum and maximum value for a single-
     instruction reg-constant add.  */
  for (mode = GET_CLASS_NARROWEST_MODE (MODE_INT); mode != VOIDmode;
       mode = GET_MODE_WIDER_MODE (mode))
    {
      rtx reg = gen_rtx_REG (mode, FIRST_PSEUDO_REGISTER);
      int icode = (int) add_optab->handlers[(int) mode].insn_code;
      HOST_WIDE_INT tmp;
      rtx add, set;
      int p, p_max;

      add_limits[(int) mode][0] = 0;
      add_limits[(int) mode][1] = 0;
      if (icode == CODE_FOR_nothing
	  || ! (*insn_operand_predicate[icode][0]) (reg, mode)
	  || ! (*insn_operand_predicate[icode][1]) (reg, mode)
	  || ! (*insn_operand_predicate[icode][2]) (const1_rtx, mode))
	continue;
      add = GEN_FCN (icode) (reg, reg, const1_rtx);
      if (GET_CODE (add) == SEQUENCE)
	continue;
      add = make_insn_raw (add);
      set = single_set (add);
      if (! set
	  || GET_CODE (SET_SRC (set)) != PLUS
	  || XEXP (SET_SRC (set), 1) != const1_rtx)
	continue;
      p_max = GET_MODE_BITSIZE (mode) - 1;
      if (p_max > HOST_BITS_PER_WIDE_INT - 2)
	p_max = HOST_BITS_PER_WIDE_INT - 2;
      for (p = 2; p < p_max; p++)
	{
	  if (! validate_change (add, &XEXP (SET_SRC (set), 1),
				 GEN_INT (((HOST_WIDE_INT) 1 << p) - 1), 0))
	    break;
	}
      add_limits[(int) mode][1] = tmp = INTVAL (XEXP (SET_SRC (set), 1));
      /* We need a range of known good values for the constant of the add.
	 Thus, before checking for the power of two, check for one less first,
	 in case the power of two is an exceptional value.  */
      if (validate_change (add, &XEXP (SET_SRC (set), 1), GEN_INT (-tmp), 0))
	{
	  if (validate_change (add, &XEXP (SET_SRC (set), 1),
			       GEN_INT (-tmp - 1), 0))
	    add_limits[(int) mode][0] = -tmp - 1;
	  else
	    add_limits[(int) mode][0] = -tmp;
	}
    }

  /* Insert notes before basic block ends, so that we can safely
     insert other insns.
     Don't do this when it would separate a BARRIER from the insn that
     it belongs to; we really need the notes only when the basic block
     end is due to a following label or to the end of the function.
     We must never dispose a JUMP_INSN as last insn of a basic block,
     since this confuses save_call_clobbered_regs.  */
  for (b = 0; b < n_basic_blocks; b++)
    {
      rtx end = BLOCK_END (b);
      if (GET_CODE (end) != JUMP_INSN)
	{
	  rtx next = next_nonnote_insn (BLOCK_END (b));
	  if (! next || GET_CODE (next) != BARRIER)
	    BLOCK_END (b) = emit_note_after (NOTE_INSN_DELETED, BLOCK_END (b));
	}
    }

  gcc_obstack_init (&related_obstack);
  regno_related = rel_alloc (nregs * sizeof *regno_related);
  zero_memory ((char *) regno_related, nregs * sizeof *regno_related);
  rel_base_list = 0;
  loop_depth = 1;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      rtx cc0_user = NULL_RTX;

      luid++;

      if (GET_CODE (insn) == NOTE)
	{
	  if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	    loop_depth++;
	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
	    loop_depth--;
	}
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
	{
	  rtx note;
	  if (GET_CODE (insn) == CALL_INSN)
	    call_tally++;
	  find_related (&PATTERN (insn), insn, luid, call_tally);
	  for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
	    {
	      if (REG_NOTE_KIND (note) == REG_DEAD
		  || (REG_NOTE_KIND (note) == REG_UNUSED
		      && GET_CODE (XEXP (note, 0)) == REG))
		{
		  rtx reg = XEXP (note, 0);
		  int regno = REGNO (reg);
		  if (REG_NOTE_KIND (note) == REG_DEAD
		      && reg_set_p (reg, PATTERN (insn)))
		    {
		      remove_note (insn, note);
		      REG_N_DEATHS (regno)--;
		    }
		  else if (regno_related[regno]
			   && ! regno_related[regno]->invalidate_luid)
		    {
		      regno_related[regno]->death = insn;
		      regno_related[regno]->reg_orig_calls_crossed
			= call_tally - regno_related[regno]->reg_set_call_tally;
		    }
		}
	    }

#ifdef HAVE_cc0
	  if (sets_cc0_p (PATTERN (insn)))
	    cc0_user = next_cc0_user (insn);
#endif
	}

      /* We always end the current processing when we have a cc0-setter-user
	 pair, not only when the user ends a basic block.  Otherwise, we
	 might end up with one or more extra instructions inserted in front
	 of the user, to set up or adjust a register.
	 There are cases where this could be handled smarter, but most of the
	 time the user will be a branch anyways, so the extra effort to
	 handle the occaisonal conditional instruction is probably not
	 justified by the little possible extra gain.  */
      if (GET_CODE (insn) == CODE_LABEL
	  || GET_CODE (insn) == JUMP_INSN
	  || (flag_exceptions && GET_CODE (insn) == CALL_INSN)
	  || cc0_user)
	{
	  optimize_related_values_0 (rel_base_list, luid, call_tally,
				     prev_nonnote_insn (insn), regmove_dump_file);
	  rel_base_list = 0;
	  if (cc0_user)
	    insn = cc0_user;
	}
    }
  optimize_related_values_0 (rel_base_list, luid, call_tally,
			     get_last_insn (), regmove_dump_file);
  obstack_free (&related_obstack, 0);
  loop_depth = save_loop_depth;
  if (regmove_dump_file)
    fprintf (regmove_dump_file, "Finished optimize_related_values.\n");
}
#endif  /* REGISTER_CONSTRAINTS */
/* END CYGNUS LOCAL */
#endif  /* AUTO_INC_DEC */

static int *regno_src_regno;

/* Indicate how good a choice REG (which appears as a source) is to replace
   a destination register with.  The higher the returned value, the better
   the choice.  The main objective is to avoid using a register that is
   a candidate for tying to a hard register, since the output might in
   turn be a candidate to be tied to a different hard register.  */
int
replacement_quality(reg)
     rtx reg;
{
  int src_regno;

  /* Bad if this isn't a register at all.  */
  if (GET_CODE (reg) != REG)
    return 0;

  /* If this register is not meant to get a hard register,
     it is a poor choice.  */
  if (REG_LIVE_LENGTH (REGNO (reg)) < 0)
    return 0;

  src_regno = regno_src_regno[REGNO (reg)];

  /* If it was not copied from another register, it is fine.  */
  if (src_regno < 0)
    return 3;

  /* Copied from a hard register?  */
  if (src_regno < FIRST_PSEUDO_REGISTER)
    return 1;

  /* Copied from a pseudo register - not as bad as from a hard register,
     yet still cumbersome, since the register live length will be lengthened
     when the registers get tied.  */
  return 2;
}

/* INSN is a copy from SRC to DEST, both registers, and SRC does not die
   in INSN.

   Search forward to see if SRC dies before either it or DEST is modified,
   but don't scan past the end of a basic block.  If so, we can replace SRC
   with DEST and let SRC die in INSN. 

   This will reduce the number of registers live in that range and may enable
   DEST to be tied to SRC, thus often saving one register in addition to a
   register-register copy.  */

static int
optimize_reg_copy_1 (insn, dest, src)
     rtx insn;
     rtx dest;
     rtx src;
{
  rtx p, q;
  rtx note;
  rtx dest_death = 0;
  int sregno = REGNO (src);
  int dregno = REGNO (dest);

  /* We don't want to mess with hard regs if register classes are small. */
  if (sregno == dregno
      || (SMALL_REGISTER_CLASSES
	  && (sregno < FIRST_PSEUDO_REGISTER
	      || dregno < FIRST_PSEUDO_REGISTER))
      /* We don't see all updates to SP if they are in an auto-inc memory
	 reference, so we must disallow this optimization on them.  */
      || sregno == STACK_POINTER_REGNUM || dregno == STACK_POINTER_REGNUM)
    return 0;

  for (p = NEXT_INSN (insn); p; p = NEXT_INSN (p))
    {
      if (GET_CODE (p) == CODE_LABEL || GET_CODE (p) == JUMP_INSN
	  || (GET_CODE (p) == NOTE
	      && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
		  || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
	break;

      /* ??? We can't scan past the end of a basic block without updating
	 the register lifetime info (REG_DEAD/basic_block_live_at_start).
	 A CALL_INSN might be the last insn of a basic block, if it is inside
	 an EH region.  There is no easy way to tell, so we just always break
	 when we see a CALL_INSN if flag_exceptions is nonzero.  */
      if (flag_exceptions && GET_CODE (p) == CALL_INSN)
	break;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;

      if (reg_set_p (src, p) || reg_set_p (dest, p)
	  /* Don't change a USE of a register.  */
	  || (GET_CODE (PATTERN (p)) == USE
	      && reg_overlap_mentioned_p (src, XEXP (PATTERN (p), 0))))
	break;

      /* See if all of SRC dies in P.  This test is slightly more
	 conservative than it needs to be.  */
      if ((note = find_regno_note (p, REG_DEAD, sregno)) != 0
	  && GET_MODE (XEXP (note, 0)) == GET_MODE (src))
	{
	  int failed = 0;
	  int d_length = 0;
	  int s_length = 0;
	  int d_n_calls = 0;
	  int s_n_calls = 0;

	  /* We can do the optimization.  Scan forward from INSN again,
	     replacing regs as we go.  Set FAILED if a replacement can't
	     be done.  In that case, we can't move the death note for SRC.
	     This should be rare.  */

	  /* Set to stop at next insn.  */
	  for (q = next_real_insn (insn);
	       q != next_real_insn (p);
	       q = next_real_insn (q))
	    {
	      if (reg_overlap_mentioned_p (src, PATTERN (q)))
		{
		  /* If SRC is a hard register, we might miss some
		     overlapping registers with validate_replace_rtx,
		     so we would have to undo it.  We can't if DEST is
		     present in the insn, so fail in that combination
		     of cases.  */
		  if (sregno < FIRST_PSEUDO_REGISTER
		      && reg_mentioned_p (dest, PATTERN (q)))
		    failed = 1;

		  /* Replace all uses and make sure that the register
		     isn't still present.  */
		  else if (validate_replace_rtx (src, dest, q)
			   && (sregno >= FIRST_PSEUDO_REGISTER
			       || ! reg_overlap_mentioned_p (src,
							     PATTERN (q))))
		    {
		      /* We assume that a register is used exactly once per
			 insn in the REG_N_REFS updates below.  If this is not
			 correct, no great harm is done.

			 Since we do not know if we will change the lifetime of
			 SREGNO or DREGNO, we must not update REG_LIVE_LENGTH
			 or REG_N_CALLS_CROSSED at this time.   */
		      if (sregno >= FIRST_PSEUDO_REGISTER)
			REG_N_REFS (sregno) -= loop_depth;

		      if (dregno >= FIRST_PSEUDO_REGISTER)
			REG_N_REFS (dregno) += loop_depth;
		    }
		  else
		    {
		      validate_replace_rtx (dest, src, q);
		      failed = 1;
		    }
		}

	      /* For SREGNO, count the total number of insns scanned.
		 For DREGNO, count the total number of insns scanned after
		 passing the death note for DREGNO.  */
	      s_length++;
	      if (dest_death)
		d_length++;

	      /* If the insn in which SRC dies is a CALL_INSN, don't count it
		 as a call that has been crossed.  Otherwise, count it.  */
	      if (q != p && GET_CODE (q) == CALL_INSN)
		{
		  /* Similarly, total calls for SREGNO, total calls beyond
		     the death note for DREGNO.  */
		  s_n_calls++;
		  if (dest_death)
		    d_n_calls++;
		}

	      /* If DEST dies here, remove the death note and save it for
		 later.  Make sure ALL of DEST dies here; again, this is
		 overly conservative.  */
	      if (dest_death == 0
		  && (dest_death = find_regno_note (q, REG_DEAD, dregno)) != 0)
		{
		  if (GET_MODE (XEXP (dest_death, 0)) != GET_MODE (dest))
		    failed = 1, dest_death = 0;
		  else
		    remove_note (q, dest_death);
		}
	    }

	  if (! failed)
	    {
	      /* These counters need to be updated if and only if we are
		 going to move the REG_DEAD note.  */
	      if (sregno >= FIRST_PSEUDO_REGISTER)
		{
		  if (REG_LIVE_LENGTH (sregno) >= 0)
		    {
		      REG_LIVE_LENGTH (sregno) -= s_length;
		      /* REG_LIVE_LENGTH is only an approximation after
			 combine if sched is not run, so make sure that we
			 still have a reasonable value.  */
		      if (REG_LIVE_LENGTH (sregno) < 2)
			REG_LIVE_LENGTH (sregno) = 2;
		    }

		  REG_N_CALLS_CROSSED (sregno) -= s_n_calls;
		}

	      /* Move death note of SRC from P to INSN.  */
	      remove_note (p, note);
	      XEXP (note, 1) = REG_NOTES (insn);
	      REG_NOTES (insn) = note;
	    }

	  /* Put death note of DEST on P if we saw it die.  */
	  if (dest_death)
	    {
	      XEXP (dest_death, 1) = REG_NOTES (p);
	      REG_NOTES (p) = dest_death;

	      if (dregno >= FIRST_PSEUDO_REGISTER)
		{
		  /* If and only if we are moving the death note for DREGNO,
		     then we need to update its counters.  */
		  if (REG_LIVE_LENGTH (dregno) >= 0)
		    REG_LIVE_LENGTH (dregno) += d_length;
		  REG_N_CALLS_CROSSED (dregno) += d_n_calls;
		}
	    }

	  return ! failed;
	}

      /* If SRC is a hard register which is set or killed in some other
	 way, we can't do this optimization.  */
      else if (sregno < FIRST_PSEUDO_REGISTER
	       && dead_or_set_p (p, src))
	break;
    }
  return 0;
}

/* INSN is a copy of SRC to DEST, in which SRC dies.  See if we now have
   a sequence of insns that modify DEST followed by an insn that sets
   SRC to DEST in which DEST dies, with no prior modification of DEST.
   (There is no need to check if the insns in between actually modify
   DEST.  We should not have cases where DEST is not modified, but
   the optimization is safe if no such modification is detected.)
   In that case, we can replace all uses of DEST, starting with INSN and
   ending with the set of SRC to DEST, with SRC.  We do not do this
   optimization if a CALL_INSN is crossed unless SRC already crosses a
   call or if DEST dies before the copy back to SRC.

   It is assumed that DEST and SRC are pseudos; it is too complicated to do
   this for hard registers since the substitutions we may make might fail.  */

static void
optimize_reg_copy_2 (insn, dest, src)
     rtx insn;
     rtx dest;
     rtx src;
{
  rtx p, q;
  rtx set;
  int sregno = REGNO (src);
  int dregno = REGNO (dest);

  for (p = NEXT_INSN (insn); p; p = NEXT_INSN (p))
    {
      if (GET_CODE (p) == CODE_LABEL || GET_CODE (p) == JUMP_INSN
	  || (GET_CODE (p) == NOTE
	      && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
		  || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
	break;

      /* ??? We can't scan past the end of a basic block without updating
	 the register lifetime info (REG_DEAD/basic_block_live_at_start).
	 A CALL_INSN might be the last insn of a basic block, if it is inside
	 an EH region.  There is no easy way to tell, so we just always break
	 when we see a CALL_INSN if flag_exceptions is nonzero.  */
      if (flag_exceptions && GET_CODE (p) == CALL_INSN)
	break;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;

      set = single_set (p);
      if (set && SET_SRC (set) == dest && SET_DEST (set) == src
	  && find_reg_note (p, REG_DEAD, dest))
	{
	  /* We can do the optimization.  Scan forward from INSN again,
	     replacing regs as we go.  */

	  /* Set to stop at next insn.  */
	  for (q = insn; q != NEXT_INSN (p); q = NEXT_INSN (q))
	    if (GET_RTX_CLASS (GET_CODE (q)) == 'i')
	      {
		if (reg_mentioned_p (dest, PATTERN (q)))
		  {
		    PATTERN (q) = replace_rtx (PATTERN (q), dest, src);

		    /* We assume that a register is used exactly once per
		       insn in the updates below.  If this is not correct,
		       no great harm is done.  */
		    REG_N_REFS (dregno) -= loop_depth;
		    REG_N_REFS (sregno) += loop_depth;
		  }


	      if (GET_CODE (q) == CALL_INSN)
		{
		  REG_N_CALLS_CROSSED (dregno)--;
		  REG_N_CALLS_CROSSED (sregno)++;
		}
	      }

	  remove_note (p, find_reg_note (p, REG_DEAD, dest));
	  REG_N_DEATHS (dregno)--;
	  remove_note (insn, find_reg_note (insn, REG_DEAD, src));
	  REG_N_DEATHS (sregno)--;
	  return;
	}

      if (reg_set_p (src, p)
	  || find_reg_note (p, REG_DEAD, dest)
	  || (GET_CODE (p) == CALL_INSN && REG_N_CALLS_CROSSED (sregno) == 0))
	break;
    }
}
/* INSN is a ZERO_EXTEND or SIGN_EXTEND of SRC to DEST.
   Look if SRC dies there, and if it is only set once, by loading
   it from memory.  If so, try to encorporate the zero/sign extension
   into the memory read, change SRC to the mode of DEST, and alter
   the remaining accesses to use the appropriate SUBREG.  This allows
   SRC and DEST to be tied later.  */
static void
optimize_reg_copy_3 (insn, dest, src)
     rtx insn;
     rtx dest;
     rtx src;
{
  rtx src_reg = XEXP (src, 0);
  int src_no = REGNO (src_reg);
  int dst_no = REGNO (dest);
  rtx p, set, subreg;
  enum machine_mode old_mode;

  if (src_no < FIRST_PSEUDO_REGISTER
      || dst_no < FIRST_PSEUDO_REGISTER
      || ! find_reg_note (insn, REG_DEAD, src_reg)
      || REG_N_SETS (src_no) != 1)
    return;
  for (p = PREV_INSN (insn); ! reg_set_p (src_reg, p); p = PREV_INSN (p))
    {
      if (GET_CODE (p) == CODE_LABEL || GET_CODE (p) == JUMP_INSN
	  || (GET_CODE (p) == NOTE
	      && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
		  || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
	return;

      /* ??? We can't scan past the end of a basic block without updating
	 the register lifetime info (REG_DEAD/basic_block_live_at_start).
	 A CALL_INSN might be the last insn of a basic block, if it is inside
	 an EH region.  There is no easy way to tell, so we just always break
	 when we see a CALL_INSN if flag_exceptions is nonzero.  */
      if (flag_exceptions && GET_CODE (p) == CALL_INSN)
	return;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;
    }
  if (! (set = single_set (p))
      || GET_CODE (SET_SRC (set)) != MEM
      || SET_DEST (set) != src_reg)
    return;

  /* Be conserative: although this optimization is also valid for
     volatile memory references, that could cause trouble in later passes.  */
  if (MEM_VOLATILE_P (SET_SRC (set)))
    return;

  /* Do not use a SUBREG to truncate from one mode to another if truncation
     is not a nop.  */
  if (GET_MODE_BITSIZE (GET_MODE (src_reg)) <= GET_MODE_BITSIZE (GET_MODE (src))
      && !TRULY_NOOP_TRUNCATION (GET_MODE_BITSIZE (GET_MODE (src)),
				 GET_MODE_BITSIZE (GET_MODE (src_reg))))
    return;

  old_mode = GET_MODE (src_reg);
  PUT_MODE (src_reg, GET_MODE (src));
  XEXP (src, 0) = SET_SRC (set);

  /* Include this change in the group so that it's easily undone if
     one of the changes in the group is invalid.  */
  validate_change (p, &SET_SRC (set), src, 1);

  /* Now walk forward making additional replacements.  We want to be able
     to undo all the changes if a later substitution fails.  */
  subreg = gen_rtx_SUBREG (old_mode, src_reg, 0);
  while (p = NEXT_INSN (p), p != insn)
    {
      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;

      /* Make a tenative change.  */
      validate_replace_rtx_group (src_reg, subreg, p);
    }

  validate_replace_rtx_group (src, src_reg, insn);

  /* Now see if all the changes are valid.  */
  if (! apply_change_group ())
    {
      /* One or more changes were no good.  Back out everything.  */
      PUT_MODE (src_reg, old_mode);
      XEXP (src, 0) = src_reg;
    }
}


/* If we were not able to update the users of src to use dest directly, try
   instead moving the value to dest directly before the operation.  */

static void
copy_src_to_dest (insn, src, dest, loop_depth, old_max_uid)
     rtx insn;
     rtx src;
     rtx dest;
     int loop_depth;
     int old_max_uid;
{
  rtx seq;
  rtx link;
  rtx next;
  rtx set;
  rtx move_insn;
  rtx *p_insn_notes;
  rtx *p_move_notes;
  int src_regno;
  int dest_regno;
  int bb;
  int insn_uid;
  int move_uid;

  /* A REG_LIVE_LENGTH of -1 indicates the register is equivalent to a constant
     or memory location and is used infrequently; a REG_LIVE_LENGTH of -2 is
     parameter when there is no frame pointer that is not allocated a register.
     For now, we just reject them, rather than incrementing the live length.  */

  if (GET_CODE (src) == REG
      && REG_LIVE_LENGTH (REGNO (src)) > 0
      && GET_CODE (dest) == REG
      && REG_LIVE_LENGTH (REGNO (dest)) > 0
      && (set = single_set (insn)) != NULL_RTX
      && !reg_mentioned_p (dest, SET_SRC (set))
      && GET_MODE (src) == GET_MODE (dest))
    {
      int old_num_regs = reg_rtx_no;

      /* Generate the src->dest move.  */
      start_sequence ();
      emit_move_insn (dest, src);
      seq = gen_sequence ();
      end_sequence ();
      /* If this sequence uses new registers, we may not use it.  */
      if (old_num_regs != reg_rtx_no
	  || ! validate_replace_rtx (src, dest, insn))
	{
	  /* We have to restore reg_rtx_no to its old value, lest
	     recompute_reg_usage will try to compute the usage of the
	     new regs, yet reg_n_info is not valid for them.  */
	  reg_rtx_no = old_num_regs;
	  return;
	}
      emit_insn_before (seq, insn);
      move_insn = PREV_INSN (insn);
      p_move_notes = &REG_NOTES (move_insn);
      p_insn_notes = &REG_NOTES (insn);

      /* Move any notes mentioning src to the move instruction */
      for (link = REG_NOTES (insn); link != NULL_RTX; link = next)
	{
	  next = XEXP (link, 1);
	  if (XEXP (link, 0) == src)
	    {
	      *p_move_notes = link;
	      p_move_notes = &XEXP (link, 1);
	    }
	  else
	    {
	      *p_insn_notes = link;
	      p_insn_notes = &XEXP (link, 1);
	    }
	}

      *p_move_notes = NULL_RTX;
      *p_insn_notes = NULL_RTX;

      /* Is the insn the head of a basic block?  If so extend it */
      insn_uid = INSN_UID (insn);
      move_uid = INSN_UID (move_insn);
      if (insn_uid < old_max_uid)
	{
	  bb = regmove_bb_head[insn_uid];
	  if (bb >= 0)
	    {
	      BLOCK_HEAD (bb) = move_insn;
	      regmove_bb_head[insn_uid] = -1;
	    }
	}

      /* Update the various register tables.  */
      dest_regno = REGNO (dest);
      REG_N_SETS (dest_regno) += loop_depth;
      REG_N_REFS (dest_regno) += loop_depth;
      REG_LIVE_LENGTH (dest_regno)++;
      if (REGNO_FIRST_UID (dest_regno) == insn_uid)
	REGNO_FIRST_UID (dest_regno) = move_uid;

      src_regno = REGNO (src);
      if (! find_reg_note (move_insn, REG_DEAD, src))
	REG_LIVE_LENGTH (src_regno)++;

      if (REGNO_FIRST_UID (src_regno) == insn_uid)
	REGNO_FIRST_UID (src_regno) = move_uid;

      if (REGNO_LAST_UID (src_regno) == insn_uid)
	REGNO_LAST_UID (src_regno) = move_uid;

      if (REGNO_LAST_NOTE_UID (src_regno) == insn_uid)
	REGNO_LAST_NOTE_UID (src_regno) = move_uid;
    }
}


/* Return whether REG is set in only one location, and is set to a
   constant, but is set in a different basic block from INSN (an
   instructions which uses REG).  In this case REG is equivalent to a
   constant, and we don't want to break that equivalence, because that
   may increase register pressure and make reload harder.  If REG is
   set in the same basic block as INSN, we don't worry about it,
   because we'll probably need a register anyhow (??? but what if REG
   is used in a different basic block as well as this one?).  FIRST is
   the first insn in the function.  */

static int
reg_is_remote_constant_p (reg, insn, first)
     rtx reg;
     rtx insn;
     rtx first;
{
  register rtx p;

  if (REG_N_SETS (REGNO (reg)) != 1)
    return 0;

  /* Look for the set.  */
  for (p = LOG_LINKS (insn); p; p = XEXP (p, 1))
    {
      rtx s;

      if (REG_NOTE_KIND (p) != 0)
	continue;
      s = single_set (XEXP (p, 0));
      if (s != 0
	  && GET_CODE (SET_DEST (s)) == REG
	  && REGNO (SET_DEST (s)) == REGNO (reg))
	{
	  /* The register is set in the same basic block.  */
	  return 0;
	}
    }

  for (p = first; p && p != insn; p = NEXT_INSN (p))
    {
      rtx s;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;
      s = single_set (p);
      if (s != 0
	  && GET_CODE (SET_DEST (s)) == REG
	  && REGNO (SET_DEST (s)) == REGNO (reg))
	{
	  /* This is the instruction which sets REG.  If there is a
             REG_EQUAL note, then REG is equivalent to a constant.  */
	  if (find_reg_note (p, REG_EQUAL, NULL_RTX))
	    return 1;
	  return 0;
	}
    }

  return 0;
}

/* INSN is adding a CONST_INT to a REG.  We search backwards looking for
   another add immediate instruction with the same source and dest registers,
   and if we find one, we change INSN to an increment, and return 1.  If
   no changes are made, we return 0.

   This changes
     (set (reg100) (plus reg1 offset1))
     ...
     (set (reg100) (plus reg1 offset2))
   to
     (set (reg100) (plus reg1 offset1))
     ...
     (set (reg100) (plus reg100 offset2-offset1))  */

/* ??? What does this comment mean?  */
/* cse disrupts preincrement / postdecrement squences when it finds a
   hard register as ultimate source, like the frame pointer.  */

int
fixup_match_2 (insn, dst, src, offset, regmove_dump_file)
     rtx insn, dst, src, offset;
     FILE *regmove_dump_file;
{
  rtx p, dst_death = 0;
  int length, num_calls = 0;

  /* If SRC dies in INSN, we'd have to move the death note.  This is
     considered to be very unlikely, so we just skip the optimization
     in this case.  */
  if (find_regno_note (insn, REG_DEAD, REGNO (src)))
    return 0;

  /* Scan backward to find the first instruction that sets DST.  */

  for (length = 0, p = PREV_INSN (insn); p; p = PREV_INSN (p))
    {
      rtx pset;

      if (GET_CODE (p) == CODE_LABEL
          || GET_CODE (p) == JUMP_INSN
          || (GET_CODE (p) == NOTE
              && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
                  || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
        break;

      /* ??? We can't scan past the end of a basic block without updating
	 the register lifetime info (REG_DEAD/basic_block_live_at_start).
	 A CALL_INSN might be the last insn of a basic block, if it is inside
	 an EH region.  There is no easy way to tell, so we just always break
	 when we see a CALL_INSN if flag_exceptions is nonzero.  */
      if (flag_exceptions && GET_CODE (p) == CALL_INSN)
	break;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
        continue;

      if (find_regno_note (p, REG_DEAD, REGNO (dst)))
	dst_death = p;
      if (! dst_death)
	length++;

      pset = single_set (p);
      if (pset && SET_DEST (pset) == dst
	  && GET_CODE (SET_SRC (pset)) == PLUS
	  && XEXP (SET_SRC (pset), 0) == src
	  && GET_CODE (XEXP (SET_SRC (pset), 1)) == CONST_INT)
        {
	  HOST_WIDE_INT newconst
	    = INTVAL (offset) - INTVAL (XEXP (SET_SRC (pset), 1));
	  rtx add = gen_add3_insn (dst, dst, GEN_INT (newconst));

	  if (add && validate_change (insn, &PATTERN (insn), add, 0))
	    {
	      /* Remove the death note for DST from DST_DEATH.  */
	      if (dst_death)
		{
		  remove_death (REGNO (dst), dst_death);
		  REG_LIVE_LENGTH (REGNO (dst)) += length;
		  REG_N_CALLS_CROSSED (REGNO (dst)) += num_calls;
		}

	      REG_N_REFS (REGNO (dst)) += loop_depth;
	      REG_N_REFS (REGNO (src)) -= loop_depth;

	      if (regmove_dump_file)
		fprintf (regmove_dump_file,
			 "Fixed operand of insn %d.\n",
			  INSN_UID (insn));

#ifdef AUTO_INC_DEC
	      for (p = PREV_INSN (insn); p; p = PREV_INSN (p))
		{
		  if (GET_CODE (p) == CODE_LABEL
		      || GET_CODE (p) == JUMP_INSN
		      || (GET_CODE (p) == NOTE
			  && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
			      || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
		    break;
		  if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
		    continue;
		  if (reg_overlap_mentioned_p (dst, PATTERN (p)))
		    {
		      if (try_auto_increment (p, insn, 0, dst, newconst, 0))
			return 1;
		      break;
		    }
		}
	      for (p = NEXT_INSN (insn); p; p = NEXT_INSN (p))
		{
		  if (GET_CODE (p) == CODE_LABEL
		      || GET_CODE (p) == JUMP_INSN
		      || (GET_CODE (p) == NOTE
			  && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
			      || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
		    break;
		  if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
		    continue;
		  if (reg_overlap_mentioned_p (dst, PATTERN (p)))
		    {
		      try_auto_increment (p, insn, 0, dst, newconst, 1);
		      break;
		    }
		}
#endif
	      return 1;
	    }
        }

      if (reg_set_p (dst, PATTERN (p)))
        break;

      /* If we have passed a call instruction, and the
         pseudo-reg SRC is not already live across a call,
         then don't perform the optimization.  */
      /* reg_set_p is overly conservative for CALL_INSNS, thinks that all
	 hard regs are clobbered.  Thus, we only use it for src for
	 non-call insns.  */
      if (GET_CODE (p) == CALL_INSN)
        {
	  if (! dst_death)
	    num_calls++;

          if (REG_N_CALLS_CROSSED (REGNO (src)) == 0)
            break;

	  if (call_used_regs [REGNO (dst)]
	      || find_reg_fusage (p, CLOBBER, dst))
	    break;
        }
      else if (reg_set_p (src, PATTERN (p)))
        break;
    }

  return 0;
}

void
regmove_optimize (f, nregs, regmove_dump_file)
     rtx f;
     int nregs;
     FILE *regmove_dump_file;
{
  int old_max_uid = get_max_uid ();
  rtx insn;
  struct match match;
  int pass;
/* CYGNUS LOCAL SH4-OPT */
  int related_values_optimized = 0;
/* END CYGNUS LOCAL */
  int i;
  rtx copy_src, copy_dst;

  regno_src_regno = (int *)alloca (sizeof *regno_src_regno * nregs);
  for (i = nregs; --i >= 0; ) regno_src_regno[i] = -1;

  regmove_bb_head = (int *)alloca (sizeof (int) * (old_max_uid + 1));
  for (i = old_max_uid; i >= 0; i--) regmove_bb_head[i] = -1;
  for (i = 0; i < n_basic_blocks; i++)
    regmove_bb_head[INSN_UID (BLOCK_HEAD (i))] = i;

  /* A forward/backward pass.  Replace output operands with input operands.  */

  loop_depth = 1;

  for (pass = 0; pass <= 2; pass++)
    {
      if (! flag_regmove && pass >= flag_expensive_optimizations)
	return;

      if (regmove_dump_file)
	fprintf (regmove_dump_file, "Starting %s pass...\n",
		 pass ? "backward" : "forward");

      for (insn = pass ? get_last_insn () : f; insn;
	   insn = pass ? PREV_INSN (insn) : NEXT_INSN (insn))
	{
	  rtx set;
	  int op_no, match_no;

	  if (GET_CODE (insn) == NOTE)
	    {
	      if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
		loop_depth++;
	      else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
		loop_depth--;
	    }

	  set = single_set (insn);
	  if (! set)
	    continue;

	  if (flag_expensive_optimizations && ! pass
	      && (GET_CODE (SET_SRC (set)) == SIGN_EXTEND
		  || GET_CODE (SET_SRC (set)) == ZERO_EXTEND)
	      && GET_CODE (XEXP (SET_SRC (set), 0)) == REG
	      && GET_CODE (SET_DEST(set)) == REG)
	    optimize_reg_copy_3 (insn, SET_DEST (set), SET_SRC (set));

	  if (flag_expensive_optimizations && ! pass
	      && GET_CODE (SET_SRC (set)) == REG
	      && GET_CODE (SET_DEST(set)) == REG)
	    {
	      /* If this is a register-register copy where SRC is not dead,
		 see if we can optimize it.  If this optimization succeeds,
		 it will become a copy where SRC is dead.  */
	      if ((find_reg_note (insn, REG_DEAD, SET_SRC (set))
		   || optimize_reg_copy_1 (insn, SET_DEST (set), SET_SRC (set)))
		  && REGNO (SET_DEST (set)) >= FIRST_PSEUDO_REGISTER)
		{
		  /* Similarly for a pseudo-pseudo copy when SRC is dead.  */
		  if (REGNO (SET_SRC (set)) >= FIRST_PSEUDO_REGISTER)
		    optimize_reg_copy_2 (insn, SET_DEST (set), SET_SRC (set));
		  if (regno_src_regno[REGNO (SET_DEST (set))] < 0
		      && SET_SRC (set) != SET_DEST (set))
		    {
		      int srcregno = REGNO (SET_SRC(set));
		      if (regno_src_regno[srcregno] >= 0)
			srcregno = regno_src_regno[srcregno];
		      regno_src_regno[REGNO (SET_DEST (set))] = srcregno;
		    }
		}
	    }
          if (! flag_regmove)
            continue;

#ifdef REGISTER_CONSTRAINTS
	  if (! find_matches (insn, &match))
	    continue;

	  /* Now scan through the operands looking for a source operand
	     which is supposed to match the destination operand.
	     Then scan forward for an instruction which uses the dest
	     operand.
	     If it dies there, then replace the dest in both operands with
	     the source operand.  */

	  for (op_no = 0; op_no < recog_n_operands; op_no++)
	    {
	      rtx src, dst, src_subreg;
	      enum reg_class src_class, dst_class;

	      match_no = match.with[op_no];

	      /* Nothing to do if the two operands aren't supposed to match.  */
	      if (match_no < 0)
		continue;

	      src = recog_operand[op_no];
	      dst = recog_operand[match_no];

	      if (GET_CODE (src) != REG)
		continue;

	      src_subreg = src;
	      if (GET_CODE (dst) == SUBREG
		  && GET_MODE_SIZE (GET_MODE (dst))
		     >= GET_MODE_SIZE (GET_MODE (SUBREG_REG (dst))))
		{
		  src_subreg
		    = gen_rtx_SUBREG (GET_MODE (SUBREG_REG (dst)),
				      src, SUBREG_WORD (dst));
		  dst = SUBREG_REG (dst);
		}
	      if (GET_CODE (dst) != REG
		  || REGNO (dst) < FIRST_PSEUDO_REGISTER)
		continue;

	      if (REGNO (src) < FIRST_PSEUDO_REGISTER)
		{
		  if (match.commutative[op_no] < op_no)
		    regno_src_regno[REGNO (dst)] = REGNO (src);
		  continue;
		}

	      if (REG_LIVE_LENGTH (REGNO (src)) < 0)
		continue;

	      /* op_no/src must be a read-only operand, and
		 match_operand/dst must be a write-only operand.  */
	      if (match.use[op_no] != READ
		  || match.use[match_no] != WRITE)
		continue;

	      if (match.early_clobber[match_no]
		  && count_occurrences (PATTERN (insn), src) > 1)
		continue;

	      /* Make sure match_operand is the destination.  */
	      if (recog_operand[match_no] != SET_DEST (set))
		continue;

	      /* If the operands already match, then there is nothing to do.  */
	      /* But in the commutative case, we might find a better match.  */
	      if (operands_match_p (src, dst)
		  || (match.commutative[op_no] >= 0
		      && operands_match_p (recog_operand[match.commutative
							 [op_no]], dst)
		      && (replacement_quality (recog_operand[match.commutative
							     [op_no]])
			  >= replacement_quality (src))))
		continue;

	      src_class = reg_preferred_class (REGNO (src));
	      dst_class = reg_preferred_class (REGNO (dst));
	      if (! regclass_compatible_p (src_class, dst_class))
		continue;
	  
/* CYGNUS LOCAL SH4-OPT */
#ifdef AUTO_INC_DEC
	      /* See the comment in front of REL_USE_HASH_SIZE what
		 this is about.  */
	      if (flag_expensive_optimizations
		  && GET_MODE (dst) == Pmode
		  && GET_CODE (SET_SRC (set)) == PLUS
		  && XEXP (SET_SRC (set), 0) == src_subreg
		  && GET_CODE (XEXP (SET_SRC (set), 1)) == CONST_INT
		  && ! related_values_optimized)
		{
		  optimize_related_values (nregs, regmove_dump_file);
		  related_values_optimized = 1;
		}
#endif
/* END CYGNUS LOCAL */
	      if (fixup_match_1 (insn, set, src, src_subreg, dst, pass,
				 op_no, match_no,
				 regmove_dump_file))
		break;
	    }
	}
    }

  /* A backward pass.  Replace input operands with output operands.  */

  if (regmove_dump_file)
    fprintf (regmove_dump_file, "Starting backward pass...\n");

  loop_depth = 1;

  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      if (GET_CODE (insn) == NOTE)
	{
	  if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_END)
	    loop_depth++;
	  else if (NOTE_LINE_NUMBER (insn) == NOTE_INSN_LOOP_BEG)
	    loop_depth--;
	}
      if (GET_RTX_CLASS (GET_CODE (insn)) == 'i')
	{
	  int op_no, match_no;
	  int success = 0;

	  if (! find_matches (insn, &match))
	    continue;

	  /* Now scan through the operands looking for a destination operand
	     which is supposed to match a source operand.
	     Then scan backward for an instruction which sets the source
	     operand.  If safe, then replace the source operand with the
	     dest operand in both instructions.  */

	  copy_src = NULL_RTX;
	  copy_dst = NULL_RTX;
	  for (op_no = 0; op_no < recog_n_operands; op_no++)
	    {
	      rtx set, p, src, dst;
	      rtx src_note, dst_note;
	      int num_calls = 0;
	      enum reg_class src_class, dst_class;
	      int length;

	      match_no = match.with[op_no];

	      /* Nothing to do if the two operands aren't supposed to match.  */
	      if (match_no < 0)
		continue;

	      dst = recog_operand[match_no];
	      src = recog_operand[op_no];

	      if (GET_CODE (src) != REG)
		continue;

	      if (GET_CODE (dst) != REG
		  || REGNO (dst) < FIRST_PSEUDO_REGISTER
		  || REG_LIVE_LENGTH (REGNO (dst)) < 0)
		continue;

	      /* If the operands already match, then there is nothing to do.  */
	      if (operands_match_p (src, dst)
		  || (match.commutative[op_no] >= 0
		      && operands_match_p (recog_operand[match.commutative[op_no]], dst)))
		continue;

	      set = single_set (insn);
	      if (! set)
		continue;

	      /* match_no/dst must be a write-only operand, and
		 operand_operand/src must be a read-only operand.  */
	      if (match.use[op_no] != READ
		  || match.use[match_no] != WRITE)
		continue;

	      if (match.early_clobber[match_no]
		  && count_occurrences (PATTERN (insn), src) > 1)
		continue;

	      /* Make sure match_no is the destination.  */
	      if (recog_operand[match_no] != SET_DEST (set))
		continue;

	      if (REGNO (src) < FIRST_PSEUDO_REGISTER)
		{
		  if (GET_CODE (SET_SRC (set)) == PLUS
		      && GET_CODE (XEXP (SET_SRC (set), 1)) == CONST_INT
		      && XEXP (SET_SRC (set), 0) == src
		      && fixup_match_2 (insn, dst, src,
					XEXP (SET_SRC (set), 1),
					regmove_dump_file))
		    break;
		  continue;
		}
	      src_class = reg_preferred_class (REGNO (src));
	      dst_class = reg_preferred_class (REGNO (dst));
	      if (! regclass_compatible_p (src_class, dst_class))
		{
		  if (!copy_src)
		    {
		      copy_src = src;
		      copy_dst = dst;
		    }
		  continue;
		}

	      /* Can not modify an earlier insn to set dst if this insn
		 uses an old value in the source.  */
	      if (reg_overlap_mentioned_p (dst, SET_SRC (set)))
		{
		  if (!copy_src)
		    {
		      copy_src = src;
		      copy_dst = dst;
		    }
		  continue;
		}

	      if (! (src_note = find_reg_note (insn, REG_DEAD, src)))
		{
		  if (!copy_src)
		    {
		      copy_src = src;
		      copy_dst = dst;
		    }
		  continue;
		}


	      /* If src is set once in a different basic block,
		 and is set equal to a constant, then do not use
		 it for this optimization, as this would make it
		 no longer equivalent to a constant.  */

              if (reg_is_remote_constant_p (src, insn, f))
		{
		  if (!copy_src)
		    {
		      copy_src = src;
		      copy_dst = dst;
		    }
		  continue;
		}


	      if (regmove_dump_file)
		fprintf (regmove_dump_file,
			 "Could fix operand %d of insn %d matching operand %d.\n",
			 op_no, INSN_UID (insn), match_no);

	      /* Scan backward to find the first instruction that uses
		 the input operand.  If the operand is set here, then
		 replace it in both instructions with match_no.  */

	      for (length = 0, p = PREV_INSN (insn); p; p = PREV_INSN (p))
		{
		  rtx pset;

		  if (GET_CODE (p) == CODE_LABEL
		      || GET_CODE (p) == JUMP_INSN
		      || (GET_CODE (p) == NOTE
			  && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
			      || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
		    break;

		  /* ??? We can't scan past the end of a basic block without
		     updating the register lifetime info
		     (REG_DEAD/basic_block_live_at_start).
		     A CALL_INSN might be the last insn of a basic block, if
		     it is inside an EH region.  There is no easy way to tell,
		     so we just always break when we see a CALL_INSN if
		     flag_exceptions is nonzero.  */
		  if (flag_exceptions && GET_CODE (p) == CALL_INSN)
		    break;

		  if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
		    continue;

		  length++;

		  /* ??? See if all of SRC is set in P.  This test is much
		     more conservative than it needs to be.  */
		  pset = single_set (p);
		  if (pset && SET_DEST (pset) == src)
		    {
		      /* We use validate_replace_rtx, in case there
			 are multiple identical source operands.  All of
			 them have to be changed at the same time.  */
		      if (validate_replace_rtx (src, dst, insn))
			{
			  if (validate_change (p, &SET_DEST (pset),
					       dst, 0))
			    success = 1;
			  else
			    {
			      /* Change all source operands back.
				 This modifies the dst as a side-effect.  */
			      validate_replace_rtx (dst, src, insn);
			      /* Now make sure the dst is right.  */
			      validate_change (insn,
					       recog_operand_loc[match_no],
					       dst, 0);
			    }
			}
		      break;
		    }

		  if (reg_overlap_mentioned_p (src, PATTERN (p))
		      || reg_overlap_mentioned_p (dst, PATTERN (p)))
		    break;

		  /* If we have passed a call instruction, and the
		     pseudo-reg DST is not already live across a call,
		     then don't perform the optimization.  */
		  if (GET_CODE (p) == CALL_INSN)
		    {
		      num_calls++;

		      if (REG_N_CALLS_CROSSED (REGNO (dst)) == 0)
			break;
		    }
		}

	      if (success)
		{
		  int dstno, srcno;

		  /* Remove the death note for SRC from INSN.  */
		  remove_note (insn, src_note);
		  /* Move the death note for SRC to P if it is used
		     there.  */
		  if (reg_overlap_mentioned_p (src, PATTERN (p)))
		    {
		      XEXP (src_note, 1) = REG_NOTES (p);
		      REG_NOTES (p) = src_note;
		    }
		  /* If there is a REG_DEAD note for DST on P, then remove
		     it, because DST is now set there.  */
		  if ((dst_note = find_reg_note (p, REG_DEAD, dst)))
		    remove_note (p, dst_note);

		  dstno = REGNO (dst);
		  srcno = REGNO (src);

		  REG_N_SETS (dstno)++;
		  REG_N_SETS (srcno)--;

		  REG_N_CALLS_CROSSED (dstno) += num_calls;
		  REG_N_CALLS_CROSSED (srcno) -= num_calls;

		  REG_LIVE_LENGTH (dstno) += length;
		  if (REG_LIVE_LENGTH (srcno) >= 0)
		    {
		      REG_LIVE_LENGTH (srcno) -= length;
		      /* REG_LIVE_LENGTH is only an approximation after
			 combine if sched is not run, so make sure that we
			 still have a reasonable value.  */
		      if (REG_LIVE_LENGTH (srcno) < 2)
			REG_LIVE_LENGTH (srcno) = 2;
		    }

		  /* We assume that a register is used exactly once per
		     insn in the updates above.  If this is not correct,
		     no great harm is done.  */

		  REG_N_REFS (dstno) += 2 * loop_depth;
		  REG_N_REFS (srcno) -= 2 * loop_depth;

                  /* If that was the only time src was set,
                     and src was not live at the start of the
                     function, we know that we have no more
                     references to src; clear REG_N_REFS so it
                     won't make reload do any work.  */
                  if (REG_N_SETS (REGNO (src)) == 0
                      && ! regno_uninitialized (REGNO (src)))
                    REG_N_REFS (REGNO (src)) = 0;

		  if (regmove_dump_file)
		    fprintf (regmove_dump_file,
			     "Fixed operand %d of insn %d matching operand %d.\n",
			     op_no, INSN_UID (insn), match_no);

		  break;
		}
	    }

	  /* If we weren't able to replace any of the alternatives, try an
	     alternative appoach of copying the source to the destination.  */
	  if (!success && copy_src != NULL_RTX)
	    copy_src_to_dest (insn, copy_src, copy_dst, loop_depth,
			      old_max_uid);

	}
    }
#endif /* REGISTER_CONSTRAINTS */

  /* In fixup_match_1, some insns may have been inserted after basic block
     ends.  Fix that here.  */
  for (i = 0; i < n_basic_blocks; i++)
    {
      rtx end = BLOCK_END (i);
      rtx new = end;
      rtx next = NEXT_INSN (new);
      while (next != 0 && INSN_UID (next) >= old_max_uid
	     && (i == n_basic_blocks - 1 || BLOCK_HEAD (i + 1) != next))
	new = next, next = NEXT_INSN (new);
      BLOCK_END (i) = new;
    }
}

/* Returns nonzero if INSN's pattern has matching constraints for any operand.
   Returns 0 if INSN can't be recognized, or if the alternative can't be
   determined.

   Initialize the info in MATCHP based on the constraints.  */

static int
find_matches (insn, matchp)
     rtx insn;
     struct match *matchp;
{
  int likely_spilled[MAX_RECOG_OPERANDS];
  int op_no;
  int any_matches = 0;

  extract_insn (insn);
  if (! constrain_operands (0))
    return 0;

  /* Must initialize this before main loop, because the code for
     the commutative case may set matches for operands other than
     the current one.  */
  for (op_no = recog_n_operands; --op_no >= 0; )
    matchp->with[op_no] = matchp->commutative[op_no] = -1;

  for (op_no = 0; op_no < recog_n_operands; op_no++)
    {
      char *p, c;
      int i = 0;

      p = recog_constraints[op_no];

      likely_spilled[op_no] = 0;
      matchp->use[op_no] = READ;
      matchp->early_clobber[op_no] = 0;
      if (*p == '=')
	matchp->use[op_no] = WRITE;
      else if (*p == '+')
	matchp->use[op_no] = READWRITE;

      for (;*p && i < which_alternative; p++)
	if (*p == ',')
	  i++;

      while ((c = *p++) != '\0' && c != ',')
	switch (c)
	  {
	  case '=':
	    break;
	  case '+':
	    break;
	  case '&':
	    matchp->early_clobber[op_no] = 1;
	    break;
	  case '%':
	    matchp->commutative[op_no] = op_no + 1;
	    matchp->commutative[op_no + 1] = op_no;
	    break;
	  case '0': case '1': case '2': case '3': case '4':
	  case '5': case '6': case '7': case '8': case '9':
	    c -= '0';
	    if (c < op_no && likely_spilled[(unsigned char) c])
	      break;
	    matchp->with[op_no] = c;
	    any_matches = 1;
	    if (matchp->commutative[op_no] >= 0)
	      matchp->with[matchp->commutative[op_no]] = c;
	    break;
	  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'h':
	  case 'j': case 'k': case 'l': case 'p': case 'q': case 't': case 'u':
	  case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B':
	  case 'C': case 'D': case 'W': case 'Y': case 'Z':
	    if (CLASS_LIKELY_SPILLED_P (REG_CLASS_FROM_LETTER ((unsigned char)c)))
	      likely_spilled[op_no] = 1;
	    break;
	  }
    }
  return any_matches;
}

/* Try to replace output operand DST in SET, with input operand SRC.  SET is
   the only set in INSN.  INSN has just been recgnized and constrained.
   SRC is operand number OPERAND_NUMBER in INSN.
   DST is operand number MATCH_NUMBER in INSN.
   If BACKWARD is nonzero, we have been called in a backward pass.
   Return nonzero for success.  */
static int
fixup_match_1 (insn, set, src, src_subreg, dst, backward, operand_number,
	       match_number, regmove_dump_file)
     rtx insn, set, src, src_subreg, dst;
     int backward, operand_number, match_number;
     FILE *regmove_dump_file;
{
  rtx p;
  rtx post_inc = 0, post_inc_set = 0, search_end = 0;
  int success = 0;
  int num_calls = 0, s_num_calls = 0;
  enum rtx_code code = NOTE;
  HOST_WIDE_INT insn_const, newconst;
  rtx overlap = 0; /* need to move insn ? */
  rtx src_note = find_reg_note (insn, REG_DEAD, src), dst_note;
  int length, s_length, true_loop_depth;

  if (! src_note)
    {
      /* Look for (set (regX) (op regA constX))
		  (set (regY) (op regA constY))
	 and change that to
		  (set (regA) (op regA constX)).
		  (set (regY) (op regA constY-constX)).
	 This works for add and shift operations, if
	 regA is dead after or set by the second insn.  */

      code = GET_CODE (SET_SRC (set));
      if ((code == PLUS || code == LSHIFTRT
	   || code == ASHIFT || code == ASHIFTRT)
	  && XEXP (SET_SRC (set), 0) == src
	  && GET_CODE (XEXP (SET_SRC (set), 1)) == CONST_INT)
	insn_const = INTVAL (XEXP (SET_SRC (set), 1));
      else if (! stable_but_for_p (SET_SRC (set), src, dst))
	return 0;
      else
	/* We might find a src_note while scanning.  */
	code = NOTE;
    }

  if (regmove_dump_file)
    fprintf (regmove_dump_file,
	     "Could fix operand %d of insn %d matching operand %d.\n",
	     operand_number, INSN_UID (insn), match_number);

  /* If SRC is equivalent to a constant set in a different basic block,
     then do not use it for this optimization.  We want the equivalence
     so that if we have to reload this register, we can reload the
     constant, rather than extending the lifespan of the register.  */
  if (reg_is_remote_constant_p (src, insn, get_insns ()))
    return 0;

  /* Scan forward to find the next instruction that
     uses the output operand.  If the operand dies here,
     then replace it in both instructions with
     operand_number.  */

  for (length = s_length = 0, p = NEXT_INSN (insn); p; p = NEXT_INSN (p))
    {
      if (GET_CODE (p) == CODE_LABEL || GET_CODE (p) == JUMP_INSN
	  || (GET_CODE (p) == NOTE
	      && (NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_BEG
		  || NOTE_LINE_NUMBER (p) == NOTE_INSN_LOOP_END)))
	break;

      /* ??? We can't scan past the end of a basic block without updating
	 the register lifetime info (REG_DEAD/basic_block_live_at_start).
	 A CALL_INSN might be the last insn of a basic block, if it is
	 inside an EH region.  There is no easy way to tell, so we just
	 always break when we see a CALL_INSN if flag_exceptions is nonzero.  */
      if (flag_exceptions && GET_CODE (p) == CALL_INSN)
	break;

      if (GET_RTX_CLASS (GET_CODE (p)) != 'i')
	continue;

      length++;
      if (src_note)
	s_length++;

      if (reg_set_p (src, p) || reg_set_p (dst, p)
	  || (GET_CODE (PATTERN (p)) == USE
	      && reg_overlap_mentioned_p (src, XEXP (PATTERN (p), 0))))
	break;

      /* See if all of DST dies in P.  This test is
	 slightly more conservative than it needs to be.  */
      if ((dst_note = find_regno_note (p, REG_DEAD, REGNO (dst)))
	  && (GET_MODE (XEXP (dst_note, 0)) == GET_MODE (dst)))
	{
	  if (! src_note)
	    {
	      rtx q;
	      rtx set2;

	      /* If an optimization is done, the value of SRC while P
		 is executed will be changed.  Check that this is OK.  */
	      if (reg_overlap_mentioned_p (src, PATTERN (p)))
		break;
	      for (q = p; q; q = NEXT_INSN (q))
		{
		  if (GET_CODE (q) == CODE_LABEL || GET_CODE (q) == JUMP_INSN
		      || (GET_CODE (q) == NOTE
			  && (NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_BEG
			      || NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_END)))
		    {
		      q = 0;
		      break;
		    }

		  /* ??? We can't scan past the end of a basic block without
		     updating the register lifetime info
		     (REG_DEAD/basic_block_live_at_start).
		     A CALL_INSN might be the last insn of a basic block, if
		     it is inside an EH region.  There is no easy way to tell,
		     so we just always break when we see a CALL_INSN if
		     flag_exceptions is nonzero.  */
		  if (flag_exceptions && GET_CODE (q) == CALL_INSN)
		    {
		      q = 0;
		      break;
		    }

		  if (GET_RTX_CLASS (GET_CODE (q)) != 'i')
		    continue;
		  if (reg_overlap_mentioned_p (src, PATTERN (q))
		      || reg_set_p (src, q))
		    break;
		}
	      if (q)
		set2 = single_set (q);
	      if (! q || ! set2 || GET_CODE (SET_SRC (set2)) != code
		  || XEXP (SET_SRC (set2), 0) != src
		  || GET_CODE (XEXP (SET_SRC (set2), 1)) != CONST_INT
		  || (SET_DEST (set2) != src
		      && ! find_reg_note (q, REG_DEAD, src)))
		{
		  /* If this is a PLUS, we can still save a register by doing
		     src += insn_const;
		     P;
		     src -= insn_const; .
		     This also gives opportunities for subsequent
		     optimizations in the backward pass, so do it there.  */
		  if (code == PLUS && backward
		      /* Don't do this if we can likely tie DST to SET_DEST
			 of P later; we can't do this tying here if we got a
			 hard register.  */
		      && ! (dst_note && ! REG_N_CALLS_CROSSED (REGNO (dst))
			    && single_set (p)
			    && GET_CODE (SET_DEST (single_set (p))) == REG
			    && (REGNO (SET_DEST (single_set (p)))
				< FIRST_PSEUDO_REGISTER))
#ifdef HAVE_cc0
		      /* We may not emit an insn directly
			 after P if the latter sets CC0.  */
		      && ! sets_cc0_p (PATTERN (p))
#endif
		      )

		    {
		      search_end = q;
		      q = insn;
		      set2 = set;
		      newconst = -insn_const;
		      code = MINUS;
		    }
		  else
		    break;
		}
	      else
		{
		  newconst = INTVAL (XEXP (SET_SRC (set2), 1)) - insn_const;
		  /* Reject out of range shifts.  */
		  if (code != PLUS
		      && (newconst < 0
			  || (newconst
			      >= GET_MODE_BITSIZE (GET_MODE (SET_SRC (set2))))))
		    break;
		  if (code == PLUS)
		    {
		      post_inc = q;
		      if (SET_DEST (set2) != src)
			post_inc_set = set2;
		    }
		}
	      /* We use 1 as last argument to validate_change so that all
		 changes are accepted or rejected together by apply_change_group
		 when it is called by validate_replace_rtx .  */
	      validate_change (q, &XEXP (SET_SRC (set2), 1),
			       GEN_INT (newconst), 1);
	    }
	  validate_change (insn, recog_operand_loc[match_number], src, 1);
	  if (validate_replace_rtx (dst, src_subreg, p))
	    success = 1;
	  break;
	}

      if (reg_overlap_mentioned_p (dst, PATTERN (p)))
	break;
      if (! src_note && reg_overlap_mentioned_p (src, PATTERN (p)))
	{
	  /* INSN was already checked to be movable when
	     we found no REG_DEAD note for src on it.  */
	  overlap = p;
	  src_note = find_reg_note (p, REG_DEAD, src);
	}

      /* If we have passed a call instruction, and the pseudo-reg SRC is not
	 already live across a call, then don't perform the optimization.  */
      if (GET_CODE (p) == CALL_INSN)
	{
	  if (REG_N_CALLS_CROSSED (REGNO (src)) == 0)
	    break;

	  num_calls++;

	  if (src_note)
	    s_num_calls++;

	}
    }

  if (! success)
    return 0;

  true_loop_depth = backward ? 2 - loop_depth : loop_depth;

  /* Remove the death note for DST from P.  */
  remove_note (p, dst_note);
  if (code == MINUS)
    {
      post_inc = emit_insn_after (copy_rtx (PATTERN (insn)), p);
      if ((HAVE_PRE_INCREMENT || HAVE_PRE_DECREMENT)
	  && search_end
	  && try_auto_increment (search_end, post_inc, 0, src, newconst, 1))
	post_inc = 0;
      validate_change (insn, &XEXP (SET_SRC (set), 1), GEN_INT (insn_const), 0);
      REG_N_SETS (REGNO (src))++;
      REG_N_REFS (REGNO (src)) += true_loop_depth;
      REG_LIVE_LENGTH (REGNO (src))++;
    }
  if (overlap)
    {
      /* The lifetime of src and dest overlap,
	 but we can change this by moving insn.  */
      rtx pat = PATTERN (insn);
      if (src_note)
	remove_note (overlap, src_note);
#if defined (HAVE_POST_INCREMENT) || defined (HAVE_POST_DECREMENT)
      if (code == PLUS
	  && try_auto_increment (overlap, insn, 0, src, insn_const, 0))
	insn = overlap;
      else
#endif
	{
	  rtx notes = REG_NOTES (insn);

	  emit_insn_after_with_line_notes (pat, PREV_INSN (p), insn);
	  PUT_CODE (insn, NOTE);
	  NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
	  NOTE_SOURCE_FILE (insn) = 0;
	  /* emit_insn_after_with_line_notes has no
	     return value, so search for the new insn.  */
	  for (insn = p; PATTERN (insn) != pat; )
	    insn = PREV_INSN (insn);

	  REG_NOTES (insn) = notes;
	}
    }
  /* Sometimes we'd generate src = const; src += n;
     if so, replace the instruction that set src
     in the first place.  */

  if (! overlap && (code == PLUS || code == MINUS))
    {
      rtx note = find_reg_note (insn, REG_EQUAL, NULL_RTX);
      rtx q, set2;
      int num_calls2 = 0, s_length2 = 0;

      if (note && CONSTANT_P (XEXP (note, 0)))
	{
	  for (q = PREV_INSN (insn); q; q = PREV_INSN(q))
	    {
	      if (GET_CODE (q) == CODE_LABEL || GET_CODE (q) == JUMP_INSN
		  || (GET_CODE (q) == NOTE
		      && (NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_BEG
			  || NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_END)))
		{
		  q = 0;
		  break;
		}

	      /* ??? We can't scan past the end of a basic block without
		 updating the register lifetime info
		 (REG_DEAD/basic_block_live_at_start).
		 A CALL_INSN might be the last insn of a basic block, if
		 it is inside an EH region.  There is no easy way to tell,
		 so we just always break when we see a CALL_INSN if
		 flag_exceptions is nonzero.  */
	      if (flag_exceptions && GET_CODE (q) == CALL_INSN)
		{
		  q = 0;
		  break;
		}

	      if (GET_RTX_CLASS (GET_CODE (q)) != 'i')
		continue;
	      s_length2++;
	      if (reg_set_p (src, q))
		{
		  set2 = single_set (q);
		  break;
		}
	      if (reg_overlap_mentioned_p (src, PATTERN (q)))
		{
		  q = 0;
		  break;
		}
	      if (GET_CODE (p) == CALL_INSN)
		num_calls2++;
	    }
	  if (q && set2 && SET_DEST (set2) == src && CONSTANT_P (SET_SRC (set2))
	      && validate_change (insn, &SET_SRC (set), XEXP (note, 0), 0))
	    {
	      PUT_CODE (q, NOTE);
	      NOTE_LINE_NUMBER (q) = NOTE_INSN_DELETED;
	      NOTE_SOURCE_FILE (q) = 0;
	      REG_N_SETS (REGNO (src))--;
	      REG_N_CALLS_CROSSED (REGNO (src)) -= num_calls2;
	      REG_N_REFS (REGNO (src)) -= true_loop_depth;
	      REG_LIVE_LENGTH (REGNO (src)) -= s_length2;
	      insn_const = 0;
	    }
	}
    }

  /* Don't remove this seemingly useless if, it is needed to pair with the
     else in the next two conditionally included code blocks.  */
  if (0)
    {;}
  else if ((HAVE_PRE_INCREMENT || HAVE_PRE_DECREMENT)
	   && (code == PLUS || code == MINUS) && insn_const
	   && try_auto_increment (p, insn, 0, src, insn_const, 1))
    insn = p;
  else if ((HAVE_POST_INCREMENT || HAVE_POST_DECREMENT)
	   && post_inc
	   && try_auto_increment (p, post_inc, post_inc_set, src, newconst, 0))
    post_inc = 0;
  /* If post_inc still prevails, try to find an
     insn where it can be used as a pre-in/decrement.
     If code is MINUS, this was already tried.  */
  if (post_inc && code == PLUS
  /* Check that newconst is likely to be usable
     in a pre-in/decrement before starting the search.  */
      && ((HAVE_PRE_INCREMENT && newconst > 0 && newconst <= MOVE_MAX)
	  || (HAVE_PRE_DECREMENT && newconst < 0 && newconst >= -MOVE_MAX))
      && exact_log2 (newconst))
    {
      rtx q, inc_dest;

      inc_dest = post_inc_set ? SET_DEST (post_inc_set) : src;
      for (q = post_inc; (q = NEXT_INSN (q)); )
	{
	  if (GET_CODE (q) == CODE_LABEL || GET_CODE (q) == JUMP_INSN
	      || (GET_CODE (q) == NOTE
		  && (NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_BEG
		      || NOTE_LINE_NUMBER (q) == NOTE_INSN_LOOP_END)))
	    break;

	  /* ??? We can't scan past the end of a basic block without updating
	     the register lifetime info (REG_DEAD/basic_block_live_at_start).
	     A CALL_INSN might be the last insn of a basic block, if it
	     is inside an EH region.  There is no easy way to tell so we
	     just always break when we see a CALL_INSN if flag_exceptions
	     is nonzero.  */
	  if (flag_exceptions && GET_CODE (q) == CALL_INSN)
	    break;

	  if (GET_RTX_CLASS (GET_CODE (q)) != 'i')
	    continue;
	  if (src != inc_dest && (reg_overlap_mentioned_p (src, PATTERN (q))
				  || reg_set_p (src, q)))
	    break;
	  if (reg_set_p (inc_dest, q))
	    break;
	  if (reg_overlap_mentioned_p (inc_dest, PATTERN (q)))
	    {
	      try_auto_increment (q, post_inc,
				  post_inc_set, inc_dest, newconst, 1);
	      break;
	    }
	}
    }
  /* Move the death note for DST to INSN if it is used
     there.  */
  if (reg_overlap_mentioned_p (dst, PATTERN (insn)))
    {
      XEXP (dst_note, 1) = REG_NOTES (insn);
      REG_NOTES (insn) = dst_note;
    }

  if (src_note)
    {
      /* Move the death note for SRC from INSN to P.  */
      if (! overlap)
	remove_note (insn, src_note);
      XEXP (src_note, 1) = REG_NOTES (p);
      REG_NOTES (p) = src_note;

      REG_N_CALLS_CROSSED (REGNO (src)) += s_num_calls;
    }

  REG_N_SETS (REGNO (src))++;
  REG_N_SETS (REGNO (dst))--;

  REG_N_CALLS_CROSSED (REGNO (dst)) -= num_calls;

  REG_LIVE_LENGTH (REGNO (src)) += s_length;
  if (REG_LIVE_LENGTH (REGNO (dst)) >= 0)
    {
      REG_LIVE_LENGTH (REGNO (dst)) -= length;
      /* REG_LIVE_LENGTH is only an approximation after
	 combine if sched is not run, so make sure that we
	 still have a reasonable value.  */
      if (REG_LIVE_LENGTH (REGNO (dst)) < 2)
	REG_LIVE_LENGTH (REGNO (dst)) = 2;
    }

  /* We assume that a register is used exactly once per
      insn in the updates above.  If this is not correct,
      no great harm is done.  */

  REG_N_REFS (REGNO (src)) += 2 * true_loop_depth;
  REG_N_REFS (REGNO (dst)) -= 2 * true_loop_depth;

  /* If that was the only time dst was set,
     and dst was not live at the start of the
     function, we know that we have no more
     references to dst; clear REG_N_REFS so it
     won't make reload do any work.  */
  if (REG_N_SETS (REGNO (dst)) == 0
      && ! regno_uninitialized (REGNO (dst)))
    REG_N_REFS (REGNO (dst)) = 0;

  if (regmove_dump_file)
    fprintf (regmove_dump_file,
	     "Fixed operand %d of insn %d matching operand %d.\n",
	     operand_number, INSN_UID (insn), match_number);
  return 1;
}


/* return nonzero if X is stable but for mentioning SRC or mentioning /
   changing DST .  If in doubt, presume it is unstable.  */
static int
stable_but_for_p (x, src, dst)
     rtx x, src, dst;
{
  RTX_CODE code = GET_CODE (x);
  switch (GET_RTX_CLASS (code))
    {
    case '<': case '1': case 'c': case '2': case 'b': case '3':
      {
	int i;
	char *fmt = GET_RTX_FORMAT (code);
	for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
	  if (fmt[i] == 'e' && ! stable_but_for_p (XEXP (x, i), src, dst))
	      return 0;
	return 1;
      }
    case 'o':
      if (x == src || x == dst)
	return 1;
      /* fall through */
    default:
      return ! rtx_unstable_p (x);
    }
}

/* Test if regmove seems profitable for this target.  Regmove is useful only
   if some common patterns are two address, i.e. require matching constraints,
   so we check that condition here.  */

int
regmove_profitable_p ()
{
#ifdef REGISTER_CONSTRAINTS
  struct match match;
  enum machine_mode mode;
  optab tstoptab = add_optab;
  do /* check add_optab and ashl_optab */
    for (mode = GET_CLASS_NARROWEST_MODE (MODE_INT); mode != VOIDmode;
	   mode = GET_MODE_WIDER_MODE (mode))
	{
	  int icode = (int) tstoptab->handlers[(int) mode].insn_code;
	  rtx reg0, reg1, reg2, pat;
	  int i;
    
	  if (GET_MODE_BITSIZE (mode) < 32 || icode == CODE_FOR_nothing)
	    continue;
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    if (TEST_HARD_REG_BIT (reg_class_contents[GENERAL_REGS], i))
	      break;
	  if (i + 2 >= FIRST_PSEUDO_REGISTER)
	    break;
	  reg0 = gen_rtx_REG (insn_operand_mode[icode][0], i);
	  reg1 = gen_rtx_REG (insn_operand_mode[icode][1], i + 1);
	  reg2 = gen_rtx_REG (insn_operand_mode[icode][2], i + 2);
	  if (! (*insn_operand_predicate[icode][0]) (reg0, VOIDmode)
	      || ! (*insn_operand_predicate[icode][1]) (reg1, VOIDmode)
	      || ! (*insn_operand_predicate[icode][2]) (reg2, VOIDmode))
	    break;
	  pat = GEN_FCN (icode) (reg0, reg1, reg2);
	  if (! pat)
	    continue;
	  if (GET_CODE (pat) == SEQUENCE)
	    pat = XVECEXP (pat, 0,  XVECLEN (pat, 0) - 1);
	  else
	    pat = make_insn_raw (pat);
	  if (! single_set (pat)
	      || GET_CODE (SET_SRC (single_set (pat))) != tstoptab->code)
	    /* Unexpected complexity;  don't need to handle this unless
	       we find a machine where this occurs and regmove should
	       be enabled.  */
	    break;
	  if (find_matches (pat, &match))
	    return 1;
	  break;
	}
  while (tstoptab != ashl_optab && (tstoptab = ashl_optab, 1));
#endif /* REGISTER_CONSTRAINTS */
  return 0;
}
