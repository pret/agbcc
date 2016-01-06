/* CYGNUS LOCAL -- meissner/d10v abi change */
/* Subroutines used for Mitsubishi D10V.
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

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "tree.h"
#include "expr.h"
#include "obstack.h"
#include "except.h"
#include "function.h"

/* Operands to compare before the appropriate branch is done.  */
struct rtx_def *compare_op0;
struct rtx_def *compare_op1;

/* Whether or not a hard register can accept a register */
unsigned char hard_regno_mode_ok[ (int)MAX_MACHINE_MODE ][FIRST_PSEUDO_REGISTER];

/* Whether to try and avoid moves between two different modes */
unsigned char modes_tieable_p[ (NUM_MACHINE_MODES) * (NUM_MACHINE_MODES) ];

/* Map register number to smallest register class.  */
enum reg_class regno_reg_class[FIRST_PSEUDO_REGISTER];

/* Map class letter into register class */
enum reg_class reg_class_from_letter[256];

/* Values of the -mbranch-cost=n string.  */
int d10v_branch_cost = 1;
char *d10v_branch_cost_string = (char *)0;

/* Values of the -mcond-exec=n string.  */
int d10v_cond_exec = 4;
char *d10v_cond_exec_string = (char *)0;

static void d10v_split_constant32 PROTO ((rtx x, int values[]));
static void d10v_split_constant64 PROTO ((rtx x, int values[]));

/* Cached value of d10v_stack_info */
static d10v_stack_t *d10v_stack_cache = (d10v_stack_t *)0;


/* Sometimes certain combinations of command options do not make
   sense on a particular target machine.  You can define a macro
   `OVERRIDE_OPTIONS' to take account of this.  This macro, if
   defined, is executed once just after all the command options have
   been parsed.

   Don't use this macro to turn on various extra optimizations for
   `-O'.  That is what `OPTIMIZATION_OPTIONS' is for.  */

void
override_options ()
{
  int regno, i, ok_p;
  enum machine_mode mode1, mode2;

  /* Set up the branch cost information */
  if (d10v_branch_cost_string)
    d10v_branch_cost = atoi (d10v_branch_cost_string);

  /* Set up max # instructions to use with conditional execution */
  if (d10v_cond_exec_string)
    d10v_cond_exec = atoi (d10v_cond_exec_string);

  /* Setup hard_regno_mode_ok/modes_tieable_p */
  for (mode1 = VOIDmode;
       (int)mode1 < NUM_MACHINE_MODES;
       mode1 = (enum machine_mode)((int)mode1 + 1))
    {
      int size = GET_MODE_SIZE (mode1);
      int int_p = GET_MODE_CLASS (mode1) == MODE_INT;

      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
	{
	  if (mode1 == CCmode || mode1 == CC_REVmode)
	    ok_p = (regno == F0_REGNUM);

	  else if (CR_P (regno))
	    ok_p = 0;

	  else if (ACCUM_P (regno))
	    ok_p = (mode1 == SImode);

	  else if (GPR_P (regno))
	    {
	      if (mode1 == QImode || mode1 == HImode)
		ok_p = 1;
	      else
		ok_p = GPR_EVEN_P (regno);
	    }
	  else
	    ok_p = 0;

	  hard_regno_mode_ok[ (int)mode1 ][ regno ] = ok_p;
	}

      /* A C expression that is nonzero if it is desirable to choose
	 register allocation so as to avoid move instructions between a
	 value of mode MODE1 and a value of mode MODE2.

	 If `HARD_REGNO_MODE_OK (R, MODE1)' and `HARD_REGNO_MODE_OK (R,
	 MODE2)' are ever different for any R, then `MODES_TIEABLE_P (MODE1,
	 MODE2)' must be zero. */
      for (mode2 = VOIDmode;
	   (int)mode2 <= NUM_MACHINE_MODES;
	   mode2 = (enum machine_mode)((int)mode2 + 1))
	{
	  if (mode1 == mode2)
	    ok_p = 1;

	  else if (mode1 == HImode || mode2 == QImode)
	    ok_p = (mode2 == HImode || mode2 == QImode);

	  else
	    ok_p = 0;

	  modes_tieable_p[ ((int)mode1 * (NUM_MACHINE_MODES)) + (int)mode2 ] = ok_p;
	}
    }

  /* A C expression whose value is a register class containing hard
     register REGNO.  In general there is more than one such class;
     choose a class which is "minimal", meaning that no smaller class
     also contains the register. */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      enum reg_class class;

      switch (regno)
	{
	case GPR_FIRST + 4:
	case GPR_FIRST + 5:
	case GPR_FIRST + 6:
	case GPR_FIRST + 7:
	case GPR_FIRST + 8:
	case GPR_FIRST + 9:
	case GPR_FIRST + 10:
	case GPR_FIRST + 11:
	case GPR_FIRST + 12:
	case GPR_FIRST + 14:
	case GPR_FIRST + 15:
	  class = EVEN_REGS;
	  break;

	case ARG_FIRST + 0:	/* r0 */
	  class = ARG0_REGS;
	  break;

	case ARG_FIRST + 1:	/* r1 */
	  class = ARG1_REGS;
	  break;

	case ARG_FIRST + 2:	/* r2 */
	  class = ARG2_REGS;
	  break;

	case ARG_FIRST + 3:	/* r3 */
	  class = ARG3_REGS;
	  break;

	case GPR_FIRST + 13:	/* r13 */
	  class = RETURN_REGS;
	  break;

	case AP_FIRST:
	  class = GENERAL_REGS;
	  break;

	case ACCUM_FIRST + 0:
	case ACCUM_FIRST + 1:
	  class = ACCUM_REGS;
	  break;

	case CR_FIRST + 7:
	  class = REPEAT_REGS;
	  break;

	case CR_FIRST + 0:
	case CR_FIRST + 1:
	case CR_FIRST + 2:
	case CR_FIRST + 3:
	case CR_FIRST + 4:
	case CR_FIRST + 5:
	case CR_FIRST + 6:
	case CR_FIRST + 8:
	case CR_FIRST + 9:
	case CR_FIRST + 10:
	case CR_FIRST + 11:
	case CR_FIRST + 12:
	case CR_FIRST + 13:
	case CR_FIRST + 14:
	case CR_FIRST + 15:
	  class = CR_REGS;
	  break;

	case F0_REGNUM:
	  class = F0_REGS;
	  break;

	case F1_REGNUM:
	  class = F1_REGS;
	  break;

	case CARRY_REGNUM:
	  class = CARRY_REGS;
	  break;
	}

      regno_reg_class[regno] = class;
    }

  /* A C expression which defines the machine-dependent operand
     constraint letters for register classes.  If CHAR is such a
     letter, the value should be the register class corresponding to
     it.  Otherwise, the value should be `NO_REGS'.  The register
     letter `r', corresponding to class `GENERAL_REGS', will not be
     passed to this macro; you do not need to handle it.

     The following letters are unavailable, due to being used as
     constraints:
	'0'..'9'
	'<', '>'
	'E', 'F', 'G', 'H'
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'
	'Q', 'R', 'S', 'T', 'U'
	'V', 'X'
	'g', 'i', 'm', 'n', 'o', 'p', 'r', 's' */

  for (i = 0; i < 256; i++)
    reg_class_from_letter[i] = NO_REGS;

  reg_class_from_letter['C'] = CARRY_REGS;
  reg_class_from_letter['a'] = ACCUM_REGS;
  reg_class_from_letter['b'] = ARG0_REGS;
  reg_class_from_letter['c'] = CR_REGS;
  reg_class_from_letter['d'] = GENERAL_REGS;
  reg_class_from_letter['e'] = EVEN_REGS;
  reg_class_from_letter['f'] = F0_REGS;
  reg_class_from_letter['h'] = ARG1_REGS;
  reg_class_from_letter['j'] = ARG2_REGS;
  reg_class_from_letter['l'] = RETURN_REGS;
  reg_class_from_letter['q'] = ARG3_REGS;
  reg_class_from_letter['x'] = CC_REGS;
  reg_class_from_letter['y'] = F1_REGS;
  reg_class_from_letter['z'] = REPEAT_REGS;
}


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

void
encode_section_info (decl)
     tree decl;
{
  /* Mark functions so that we use the @word modifier to get a word
     address instead of a byte address.  We use the side_effects bit
     on the identifier node to indicate word addresses are needed.  */
  if (TREE_CODE (decl) == FUNCTION_DECL)
    {
      rtx symref = XEXP (DECL_RTL (decl), 0);
      tree symid = get_identifier (XSTR (symref, 0));

      SYMBOL_REF_FLAG (symref) = TRUE;
      TREE_SIDE_EFFECTS (symid) = TRUE;
    }
}


/* Return true if the register is a GPR */

int
gpr_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == REG)
    {
      int regno = REGNO (op);
      if (regno >= 0 && regno < FIRST_PSEUDO_REGISTER)
	return GPR_P (regno);

      return register_operand (op, mode);
    }

  else if (GET_CODE (op) == SUBREG)
    {
      rtx inner = SUBREG_REG (op);
      if (GET_CODE (inner) == REG)
	{
	  int regno = REGNO (SUBREG_REG (op));
	  if (regno >= 0 && regno < FIRST_PSEUDO_REGISTER)
	    return GPR_P (regno);

	  return register_operand (op, mode);
	}

      else if (GET_CODE (inner) == MEM && !reload_completed)
	return d10v_legitimate_address_p (GET_MODE (inner), inner, 0);
    }

  return FALSE;
}

/* Return true if the register is an accumulator */

int
accum_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == REG)
    {
      int regno = REGNO (op);
      if (regno >= 0 && regno < FIRST_PSEUDO_REGISTER)
	return ACCUM_P (regno);

      return register_operand (op, mode);
    }

  else if (GET_CODE (op) == SUBREG)
    {
      rtx inner = SUBREG_REG (op);
      if (GET_CODE (inner) == REG)
	{
	  int regno = REGNO (SUBREG_REG (op));
	  if (regno >= 0 && regno < FIRST_PSEUDO_REGISTER)
	    return ACCUM_P (regno);

	  return register_operand (op, mode);
	}

      else if (GET_CODE (inner) == MEM && !reload_completed)
	return d10v_legitimate_address_p (GET_MODE (inner), inner, 0);
    }

  return FALSE;
}

/* Return true if an operand is a register or is the constant 0.  */

int
reg_or_0_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return INTVAL (op) == 0;

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a signed 16-bit constant.  */

int
arith16_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return IN_RANGE_P (INTVAL (op), -32768, 32767);

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a  4-bit unsigned constants,
   or 4-bit negative unsigned constants.  */

int
arith_4bit_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return IN_RANGE_P (INTVAL (op), -16, 16) && INTVAL (op) != 0;

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a non negative constant. */

int
arith_nonnegative_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return (INTVAL (op) >= 0);

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a signed 32-bit constant.  */

int
arith32_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return TRUE;

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a signed 64-bit constant.  */

int
arith64_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE)
    return TRUE;

  else
    return gpr_operand (op, mode);
}

/* Return true if an operand is a register or a constant with the lower 16-bits of 0.  */

int
arith_lower0_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == CONST_INT)
    return ((INTVAL (op) & 0xffff) == 0);

  else
    return gpr_operand (op, mode);
}

int
ldi_shift_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (TARGET_SMALL_INSNS && optimize && GET_CODE (op) == CONST_INT)
    {
      HOST_WIDE_INT value = INTVAL (op);
      int i;

      if (IN_RANGE_P (value, -8, 7))
	return FALSE;

      for (i = 1; i < 16; i++)
	{
	  if (((value >> i) << i) == value && IN_RANGE_P (value >> i, -8, 7))
	    return i;
	}
    }

  return FALSE;
}

/* Return true if the operand is either the PC, a return, or a label_ref.  */

int
pc_or_label_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (op == pc_rtx)
    return TRUE;

  if (GET_CODE (op) == LABEL_REF)
    return TRUE;

  if (GET_CODE (op) == RETURN && direct_return () && (d10v_stack_info ())->total_size == 0)
    return TRUE;

  return FALSE;
}

/* Return true if a memory operand is a short memory operand.  */

int
short_memory_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  rtx addr;

  if (GET_CODE (op) != MEM)
    return FALSE;

  addr = XEXP (op, 0);
  switch (GET_CODE (addr))
    {
    default:
      break;

    case POST_INC:
    case POST_DEC:
    case PRE_DEC:
    case REG:
      return TRUE;
    }

  return FALSE;
}

/* Return true if an operand is simple, suitable for use in a conditional move */

int
cond_move_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (mode != QImode && mode != HImode && mode != SImode && mode != SFmode)
    return FALSE;

  else if (GET_CODE (op) == REG || GET_CODE (op) == SUBREG)
    return gpr_operand (op, mode) || (mode == SImode && accum_operand (op, mode));

  else if (GET_CODE (op) == CONST_INT && (mode == QImode || mode == HImode))
    return IN_RANGE_P (INTVAL (op), -8, 7);

  /* Don't allow post dec/inc, since we might not get the side effects correct. */
  else if (GET_CODE (op) == MEM)
    return (GET_CODE (XEXP (op, 0)) == REG);

  return FALSE;
}

/* Return true if an operand is simple, suitable for use in conditional execution.
   Unlike cond_move, we can allow auto inc/dec.  */

int
cond_exec_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (mode != QImode && mode != HImode && mode != SImode && mode != SFmode)
    return FALSE;

  else if (GET_CODE (op) == REG || GET_CODE (op) == SUBREG)
    return gpr_operand (op, mode) || (mode == SImode && accum_operand (op, mode));

  else if (GET_CODE (op) == CONST_INT && (mode == QImode || mode == HImode))
    return IN_RANGE_P (INTVAL (op), -8, 7);

  else if (GET_CODE (op) == MEM)
    return short_memory_operand (op, mode);

  return FALSE;
}

/* Return true if an operand is F0.  */

int
carry_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != REG)
    return FALSE;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (REGNO (op) != CARRY_REGNUM)
    return FALSE;

  return 1;
}

int
f0_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != REG)
    return FALSE;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (REGNO (op) != F0_REGNUM)
    return FALSE;

  return 1;
}

/* Return true if an operand is F1.  */

int
f1_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != REG)
    return FALSE;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (REGNO (op) != F1_REGNUM)
    return FALSE;

  return 1;
}

/* Return true if an operand is F0 or F1.  */

int
f_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != REG)
    return FALSE;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (REGNO (op) != F0_REGNUM && REGNO (op) != F1_REGNUM)
    return FALSE;

  return 1;
}

/* Return true if the code is a test of f0 */

int
f0_compare_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int i;
  rtx x;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (GET_CODE (op) != NE && GET_CODE (op) != EQ)
    return FALSE;

  x = XEXP (op, 0);
  if (GET_CODE (x) != REG || REGNO (x) != F0_REGNUM)
    return FALSE;

  x = XEXP (op, 1);
  if (GET_CODE (x) != CONST_INT || INTVAL (x) != 0)
    return FALSE;

  return TRUE;
}

/* Return true if the code is a relational operations (EQ, LE, etc.)  */

int
compare_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int i;
  rtx x;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  if (GET_RTX_CLASS (GET_CODE (op)) != '<')
    return FALSE;

  /* Don't allow paradoxical SUBREGs */
  for (i = 0; i < 2; i++)
    {
      x = XEXP (op, i);
      while (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);

      if (!(GET_CODE (x) == REG || (i == 1 && GET_CODE (x) == CONST_INT)))
	return FALSE;
    }

  return TRUE;
}

/* Return true if the code is a equality operation (EQ, NE) */

int
equality_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int i;
  rtx x;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  switch (GET_CODE (op))
    {
    default:
      return FALSE;

    case EQ:
    case NE:
      break;
    }

  /* Don't allow paradoxical SUBREGs */
  for (i = 0; i < 2; i++)
    {
      x = XEXP (op, i);
      while (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);

      if (!(GET_CODE (x) == REG || (i == 1 && GET_CODE (x) == CONST_INT)))
	return FALSE;
    }

  return TRUE;
}

/* Return true if the code is a signed relational operations (LT, LE, etc.) */

int
signed_compare_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int i;
  rtx x;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  switch (GET_CODE (op))
    {
    default:
      return FALSE;

    case LT:
    case LE:
    case GT:
    case GE:
      break;
    }

  /* Don't allow paradoxical SUBREGs */
  for (i = 0; i < 2; i++)
    {
      x = XEXP (op, i);
      while (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);

      if (!(GET_CODE (x) == REG || (i == 1 && GET_CODE (x) == CONST_INT)))
	return FALSE;
    }

  return TRUE;
}

/* Return true if the code is a unsigned relational operations (LEU, etc.) */

int
unsigned_compare_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int i;
  rtx x;

  if (GET_MODE (op) != CCmode && GET_MODE (op) != CC_REVmode)
    return FALSE;

  switch (GET_CODE (op))
    {
    default:
      return FALSE;

    case LTU:
    case LEU:
    case GTU:
    case GEU:
      break;
    }

  /* Don't allow paradoxical SUBREGs */
  for (i = 0; i < 2; i++)
    {
      x = XEXP (op, i);
      while (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);

      if (!(GET_CODE (x) == REG || (i == 1 && GET_CODE (x) == CONST_INT)))
	return FALSE;
    }

  return TRUE;
}

/* Return true if the unary operator can go inside of a exef0{f,t} || operation.  */

int
unary_parallel_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  rtx op0;

  /* Only do this after register allocation, so that we can look at the register # */
  if (!reload_completed)
    return FALSE;

  if (GET_RTX_CLASS (GET_CODE (op)) != '1')
    return FALSE;

  op0 = XEXP (op, 0);
  while (GET_CODE (op0) == SUBREG)
    op0 = SUBREG_REG (op0);

  switch (GET_CODE (op))
    {
    case NEG:
    case NOT:
    case SIGN_EXTEND:
    case ZERO_EXTEND:
      if (GET_MODE (op) == HImode && GET_CODE (op0) == REG && GPR_P (REGNO (op0)))
	return TRUE;

      break;
    }

  return FALSE;
}

/* Return true if the binary operator can go inside of a exef0{f,t} || operation.  */

int
binary_parallel_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  rtx op0, op1;

  /* Only do this after register allocation, so that we can look at the register # */
  if (!reload_completed)
    return FALSE;

  if (GET_RTX_CLASS (GET_CODE (op)) != '2' && GET_RTX_CLASS (GET_CODE (op)) != 'c')
    return FALSE;

  op0 = XEXP (op, 0);
  op1 = XEXP (op, 1);

  while (GET_CODE (op0) == SUBREG)
    op0 = SUBREG_REG (op0);

  while (GET_CODE (op1) == SUBREG)
    op1 = SUBREG_REG (op1);

  if (GET_CODE (op0) != REG)
    return FALSE;

  /* reg1 op= reg2 */
  if (GET_CODE (op1) == REG)
    {
      switch (GET_CODE (op))
	{
	case PLUS:
	case MINUS:
	  if (GET_MODE (op) == SImode && GPR_P (REGNO (op0)) && GPR_P (REGNO (op1)))
	    return TRUE;

	  /* fall through */

	case MULT:
	case AND:
	case IOR:
	case XOR:
	case ASHIFT:
	case ASHIFTRT:
	case LSHIFTRT:
	  return (GET_MODE (op) == HImode && GPR_P (REGNO (op0)) && GPR_P (REGNO (op1)));
	}
    }

  /* reg op= constant */
  else if (GET_CODE (op1) == CONST_INT)
    {
      HOST_WIDE_INT value = INTVAL (op1);
      int log;

      if (GET_MODE (op) == HImode)
	switch (GET_CODE (op))
	  {
	  case PLUS:
	  case MINUS:
	    return IN_RANGE_P (value, -16, 16);

	  case IOR:
	  case XOR:
	    if (value == 0)
	      return TRUE;

	    log = exact_log2 (value);
	    return IN_RANGE_P (log, 0, 15);

	  case AND:
	    if ((value & 0xffff) == 0xffff)
	      return TRUE;

	    log = exact_log2 (~value);
	    return IN_RANGE_P (log, 0, 15);
	  }
    }

  return FALSE;
}

/* Return true if the {sign,zero} extend operator from memory can go inside
   of a exef0{f,t} || operation.  */

int
extend_parallel_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  /* Only do this after register allocation, so that we can look at the register # */
  if (!reload_completed)
    return FALSE;

  if (GET_RTX_CLASS (GET_CODE (op)) != '1')
    return FALSE;

  switch (GET_CODE (op))
    {
    case SIGN_EXTEND:
    case ZERO_EXTEND:
      if (GET_MODE (op) == HImode && short_memory_operand (XEXP (op, 0), QImode))
	return TRUE;

      break;
    }

  return FALSE;
}

/* Return true if the {smin,smax} operator can go inside of a exef0{f,t} || operation.  */

int
minmax_parallel_operator (op, mode)
     rtx op;
     enum machine_mode mode;
{
  /* Only do this after register allocation, so that we can look at the register # */
  if (!reload_completed)
    return FALSE;

  if (GET_RTX_CLASS (GET_CODE (op)) != 'c')
    return FALSE;

  switch (GET_CODE (op))
    {
    case SMIN:
    case SMAX:
      if (GET_MODE (op) == HImode
	  && gpr_operand (XEXP (op,0), mode)
	  && gpr_operand (XEXP (op, 1)))
	return TRUE;

      if (GET_MODE (op) == SImode
	  && accum_operand (XEXP (op,0), mode)
	  && accum_operand (XEXP (op, 1)))
	return TRUE;

      break;
    }

  return FALSE;
}


/* Return true if a 32-bit integer constant can be loaded by two parallel
   ldi constants.  */

int
parallel_ldi (value)
     HOST_WIDE_INT value;
{
  int low = SIGN_EXTEND_SHORT (value);
  int high = SIGN_EXTEND_SHORT (value >> 16);

  return IN_RANGE_P (low, -8, 7) && IN_RANGE_P (high, -8, 7);
}


/* Return true if the two operands reference adjacement memory locations so that
   a ld2w/st2w instruction can be used.  The insn argument is the last insn of
   the peephole, so that REG_DEAD notes are useful.  */

int
adjacent_memory_operands (op1, op2, insn)
     rtx op1;
     rtx op2;
     rtx insn;
{
  rtx addr1, addr2;
  rtx const1, const2;
  enum rtx_code code1, code2;

  if (GET_CODE (op1) != MEM || GET_MODE (op1) != HImode)
    return FALSE;

  if (GET_CODE (op2) != MEM || GET_MODE (op2) != HImode)
    return FALSE;

  addr1 = XEXP (op1, 0);
  addr2 = XEXP (op2, 0);
  code1 = GET_CODE (addr1);
  code2 = GET_CODE (addr2);
  if (((code1 == POST_INC && code2 == POST_INC))
      || ((code1 == POST_DEC && code2 == POST_DEC))
      || ((code1 == PRE_DEC && code2 == PRE_DEC)))
    {
      addr1 = XEXP (addr1, 0);
      addr2 = XEXP (addr2, 0);
      return (GET_CODE (addr1) == REG && GET_CODE (addr2) == REG
	      && REGNO (addr1) == REGNO (addr2));
    }

  else if ((code1 == POST_INC || code1 == POST_DEC || code1 == PRE_DEC)
	   && code2 != POST_INC && code2 != POST_DEC && code2 != PRE_DEC)
    {
      addr1 = XEXP (addr1, 0);
      return (GET_CODE (addr1) == REG && code2 == REG
	      && REGNO (addr1) == REGNO (addr2)
	      && find_reg_note (insn, REG_DEAD, addr1)); 
    }

  else if (code1 == POST_INC || code1 == POST_DEC || code1 == PRE_DEC
	   || code2 == POST_INC || code2 == POST_DEC || code2 == PRE_DEC)
    return FALSE;

  if (code1 == CONST)
    addr1 = XEXP (addr1, 0);

  if (code2 == CONST)
    code2 = GET_CODE (addr2);

  const1 = const0_rtx;
  const2 = const0_rtx;
  addr1 = eliminate_constant_term (addr1, &const1);
  addr2 = eliminate_constant_term (addr2, &const2);

  if (!rtx_equal_p (addr1, addr2))
    return FALSE;

  if (GET_CODE (const1) == CONST_INT
      && GET_CODE (const2) == CONST_INT
      && INTVAL (const1) == INTVAL (const2) - 2)
    return TRUE;

  return FALSE;
}


/* Expand a divmod operation */

void
d10v_expand_divmod (operands, unsigned_p)
     rtx operands[];
     int unsigned_p;
{
  rtx ret = emit_library_call_value (gen_rtx (SYMBOL_REF, Pmode, (unsigned_p) ? "__udivmodhi4" : "__divmodhi4"),
				     NULL_RTX, TRUE, SImode, 2,
				     operands[1], HImode,
				     operands[2], HImode);

  emit_move_insn (operands[0], gen_rtx (SUBREG, HImode, ret, 1));
  emit_move_insn (operands[3], gen_rtx (SUBREG, HImode, ret, 0));
}


/* A C statement to initialize the variable parts of a trampoline.
   ADDR is an RTX for the address of the trampoline; FNADDR is an RTX
   for the address of the nested function; STATIC_CHAIN is an RTX for
   the static chain value that should be passed to the function when
   it is called. */

void
initialize_trampoline (addr, fnaddr, static_chain)
     rtx addr;
     rtx fnaddr;
     rtx static_chain;
{
  rtx byte_addr = gen_reg_rtx (Pmode);
  rtx reg1 = gen_reg_rtx (Pmode);
  rtx reg2 = gen_reg_rtx (Pmode);

  emit_move_insn (byte_addr, addr);

  /* ldi.l r7,<static chain> */
  emit_move_insn (gen_rtx (MEM, HImode, gen_rtx (POST_INC, HImode, byte_addr)),
		  GEN_INT (0xE000 | ((STATIC_CHAIN_REGNUM - GPR_FIRST) << 4)));

  emit_move_insn (gen_rtx (MEM, HImode, gen_rtx (POST_INC, HImode, byte_addr)),
		  static_chain);

  /* bra.l <function> */
  emit_move_insn (gen_rtx (MEM, HImode, gen_rtx (POST_INC, HImode, byte_addr)),
		  GEN_INT (0xE400));

  emit_move_insn (reg1, fnaddr);
  emit_move_insn (reg2, addr);
  emit_insn (gen_subhi3 (reg1, reg1, reg2));
  emit_move_insn (gen_rtx (MEM, HImode, byte_addr), reg1);

  error ("Trampolines currently don't work");
}


/* Emit the appropriate comparison code for two operands.  Return TRUE if
   F0 contains a true value for the condition to be successful, FALSE if
   not.

   Arguments:

	code		Which comparison to emit
	op{0,1}		Operands
	temp		Temporary register that we can clobber (not used right now)
	insn		Whole insn
	mode		Mode of the operands
	output_p	Whether to actually emit code or just return true/false.  */

int
emit_comparison (code, op0, op1, temp, insn, mode, output_p)
     enum rtx_code code;
     rtx op0, op1, temp, insn;
     enum machine_mode mode;
     int output_p;
{
  int ret = FALSE;
  char *template = (char *)0;
  int op1_int_p = CONSTANT_P (op1);
  rtx xop[10];

  /* Skip subregs that we find.  */
  while (GET_CODE (op0) == SUBREG)
    op0 = SUBREG_REG (op0);

  while (GET_CODE (op1) == SUBREG)
    op1 = SUBREG_REG (op1);

  xop[0] = op0;
  xop[1] = op1;
  xop[2] = temp;

  /* 16-bit comparisons */
  if (mode == HImode)
    {
      switch (code)
	{
	default:
	  break;

	case EQ:
	  template = (!op1_int_p) ? "cmpeq %0,%1" : "cmpeqi %0,%1";
	  ret = TRUE;
	  break;

	case NE:
	  template = (!op1_int_p) ? "cmpeq %0,%1" : "cmpeqi %0,%1";
	  ret = FALSE;
	  break;

	case LT:
	  template = (!op1_int_p) ? "cmp %0,%1" : "cmpi %0,%1";
	  ret = TRUE;
	  break;

	case LE:
	  if (!op1_int_p)
	    {			/* convert LE x,y to !GT x,y (ie, LT y,x) */
	      template = "cmp %1,%0";
	      ret = FALSE;
	      break;
	    }
	  else if (GET_CODE (op1) != CONST_INT || INTVAL (op1) != 32767)
	    {
	      /* If some sum of a symbol/label and a constant ends up
		 as 32768, it would be the linker's task to fix that up.  */
	      if (output_p)
		xop[3] = plus_constant_for_output (op1, 1);

	      template = "cmpi %0,%3";
	      ret = TRUE;
	      break;
	    }
	  else
	    {
	      template = "cmpeqi %0,%1";
	      ret = TRUE;
	      break;
	    }

	case GT:
	  if (!op1_int_p)
	    {			/* convert GT x,y to LT y,x */
	      template = "cmp %1,%0";
	      ret = TRUE;
	      break;
	    }
	  else if (GET_CODE (op1) != CONST_INT || INTVAL (op1) != 32767)
	    {
	      if (output_p)
		xop[3] = plus_constant_for_output (op1, 1);

	      template = "cmpi %0,%3";
	      ret = FALSE;
	      break;
	    }
	  else
	    {
	      template = "cmpeq %0,%0";
	      ret = FALSE;
	      break;
	    }

	case GE:
	  template = (!op1_int_p) ? "cmp %0,%1" : "cmpi %0,%1";
	  ret = FALSE;
	  break;

	case LTU:
	  template = (!op1_int_p) ? "cmpu %0,%1" : "cmpui %0,%1";
	  ret = TRUE;
	  break;

	case LEU:
	  if (!op1_int_p)
	    {			/* convert LEU x,y to !GTU x,y (ie, LTU y,x) */
	      template = "cmpu %1,%0";
	      ret = FALSE;
	      break;
	    }
	  else if (GET_CODE (op1) != CONST_INT || ~INTVAL (op1) & 65535)
	    {
	      if (output_p)
		xop[3] = plus_constant_for_output (op1, 1);

	      template = "cmpui %0,%3";
	      ret = TRUE;
	      break;
	    }
	  else
	    {
	      template = "cmpeq %0,%0";
	      ret = TRUE;
	      break;
	    }

	case GTU:
	  if (!op1_int_p)
	    {			/* convert GTU x,y to LTU y,x */
	      template = "cmpu %1,%0";
	      ret = TRUE;
	      break;
	    }
	  else if (GET_CODE (op1) != CONST_INT || ~INTVAL (op1) & 65535)
	    {			/* convert GTU x,y to !LTU x,y+1 */
	      if (output_p)
		xop[3] = plus_constant_for_output (op1, 1);

	      template = "cmpui %0,%3";
	      ret = FALSE;
	      break;
	    }
	  else
	    {
	      /* Convert GTU x,65535 to !EQ x,x */
	      template = "cmpeq %0,%0";
	      ret = FALSE;
	      break;
	    }

	case GEU:
	  template = (!op1_int_p) ? "cmpu %0,%1" : "cmpui %0,%1";
	  ret = FALSE;
	  break;
	}
    }

  /* 32-bit comparisons in the accumulators. */
  else if (mode == SImode
	   && GET_CODE (op0) == REG && ACCUM_P (REGNO (op0))
	   && GET_CODE (op1) == REG && ACCUM_P (REGNO (op1)))
    {
      switch (code)
	{
	default:
	  break;

	case EQ:
	  template = "cmpeq %0,%1";
	  ret = TRUE;
	  break;

	case NE:
	  template = "cmpeq %0,%1";
	  ret = FALSE;
	  break;

	case LT:
	  template = "cmp %0,%1";
	  ret = TRUE;
	  break;

	case LE:
	  /* convert LE x,y to !GT x,y (ie, LT y,x) */
	  template = "cmp %1,%0";
	  ret = FALSE;
	  break;

	case GT:
	  /* convert GT x,y to LT y,x */
	  template = "cmp %1,%0";
	  ret = TRUE;
	  break;

	case GE:
	  template = "cmp %0,%1";
	  ret = FALSE;
	  break;
	}
    }

  /* 32-bit comparisons in the general purpose registers. */
  else if (mode == SImode
	   && GET_CODE (op0) == REG && GPR_OR_PSEUDO_P (REGNO (op0))
	   && GET_CODE (op1) == REG && GPR_OR_PSEUDO_P (REGNO (op1)))
    {
      switch (code)
	{
	default:
	  break;

	case EQ:
	  template = "cmpeq %U0,%U1\n\texef0t || cmpeq %L0,%L1";
	  ret = TRUE;
	  break;

	case NE:
	  template = "cmpeq %U0,%U1\n\texef0t || cmpeq %L0,%L1";
	  ret = FALSE;
	  break;

	case LT:
	  template = "cmpeq %U0,%U1\n\tcmp %U0,%U1\n\texef1t || cmpu %L0,%L1";
	  ret = TRUE;
	  break;

	case LE:
	  /* convert LE x,y to !GT x,y (ie, LT y,x) */
	  template = "cmpeq %U1,%U0\n\tcmp %U1,%U0\n\texef1t || cmpu %L1,%L0";
	  ret = FALSE;
	  break;

	case GT:
	  /* convert GT x,y to LT y,x */
	  template = "cmpeq %U1,%U0\n\tcmp %U1,%U0\n\texef1t || cmpu %L1,%L0";
	  ret = TRUE;
	  break;

	case GE:
	  /* convert GE x,y to !LT x,y */
	  template = "cmpeq %U0,%U1\n\tcmp %U0,%U1\n\texef1t || cmpu %L0,%L1";
	  ret = FALSE;
	  break;

	case LTU:
	  template = "cmpeq %U0,%U1\n\tcmpu %U0,%U1\n\texef1t || cmpu %L0,%L1";
	  ret = TRUE;
	  break;

	case LEU:
	  /* convert LEU x,y to !GTU x,y (ie, LTU y,x) */
	  template = "cmpeq %U1,%U0\n\tcmpu %U1,%U0\n\texef1t || cmpu %L1,%L0";
	  ret = FALSE;
	  break;

	case GTU:
	  /* convert GTU x,y to LTU y,x */
	  template = "cmpeq %U1,%U0\n\tcmpu %U1,%U0\n\texef1t || cmpu %L1,%L0";
	  ret = TRUE;
	  break;

	case GEU:
	  /* convert GEU x,y to !LTU x,y */
	  template = "cmpeq %U0,%U1\n\tcmpu %U0,%U1\n\texef1t || cmpu %L0,%L1";
	  ret = FALSE;
	  break;
	}
    }

  /* 32-bit comparison against 0 in the general purpose registers. */
  else if (mode == SImode
	   && GET_CODE (op0) == REG && GPR_OR_PSEUDO_P (REGNO (op0))
	   && GET_CODE (op1) == CONST_INT && INTVAL (op1) == 0)
    {
      switch (code)
	{
	default:
	  break;

	case EQ:
	case LEU:	/* convert LEU x,0 to EQ x,0 */
	  template = "cmpeqi %U0,0\n\texef0t || cmpeqi %L0,0";
	  ret = TRUE;
	  break;

	case NE:
	case GTU:	/* convert GTU x,0 to NE x,0 */
	  template = "cmpeqi %U0,0\n\texef0t || cmpeqi %L0,0";
	  ret = FALSE;
	  break;

	case LT:
	  template = "cmpi %U0,0";
	  ret = TRUE;
	  break;

	case LE:
	  template = "cmpeqi %U0,0\n\tcmpi %U0,0\n\texef1t || cmpeqi %L0,0";
	  ret = TRUE;
	  break;

	case GT:
	  /* convert GT x,0 to !LE x,0 */
	  template = "cmpeqi %U0,0\n\tcmpi %U0,0\n\texef1t || cmpeqi %L0,0";
	  ret = FALSE;
	  break;

	case GE:
	  /* convert GE x,0 to !LT x,0 */
	  template = "cmpi %U0,0\n";
	  ret = FALSE;
	  break;

	case LTU:
	  /* LTU x,0 is always false */
	  template = "cmpeq %U0,%U0";
	  ret = FALSE;
	  break;

	case GEU:
	  /* GEU x,0 is always true */
	  template = "cmpeq %U0,%U0";
	  ret = TRUE;
	  break;
	}
    }

  /* 32-bit comparison against a constant with -8..7 in the lower 16 bits */
  else if (mode == SImode
	   && GET_CODE (op0) == REG && GPR_OR_PSEUDO_P (REGNO (op0))
	   && GET_CODE (op1) == CONST_INT
	   && IN_RANGE_P (SIGN_EXTEND_SHORT (INTVAL (op1)), -8, 7))
    {
      switch (code)
	{
	default:
	  break;

	case EQ:
	  template = "cmpeqi %U0,%U1\n\texef0t || cmpeqi %L0,%L1";
	  ret = TRUE;
	  break;

	case NE:
	  template = "cmpeqi %U0,%U1\n\texef0t || cmpeqi %L0,%L1";
	  ret = FALSE;
	  break;
	}
    }

  /* Abort on unhandled cases.  */
  if (!template)
    {
      if (!insn)
	insn = gen_rtx (SET, VOIDmode,
			gen_rtx (REG, CCmode, F0_REGNUM),
			gen_rtx (code, CCmode, op0, op1));

      fatal_insn ("emit_comparison:", insn);
      return TRUE;
    }

  /* If we are outputing, do it now.  */
  if (output_p)
    output_asm_insn (template, xop);

  return ret;
}


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
int
select_cc_mode (op, x, y)
     enum rtx_code op;
     rtx x;
     rtx y;
{
  int ret = emit_comparison (op, x, y, NULL_RTX, NULL_RTX, GET_MODE (x), FALSE);

  fprintf (stderr, "select_cc_mode: emit_comparison returned %d\n", ret);
  debug_rtx (x);
  debug_rtx (y);

  return (ret ? (int)CCmode : (int)CC_REVmode);
}


/* Emit the code to set F0 based on comparing op0 and op1.  Use CCmode for
   comparisons that set F0 to 1 if the comparison is true, and CC_REVmode
   for comparisons that set F0 to 0 if the comparison is true.  */

Rtx
expand_comparison (code, op0, op1, reg_p)
     enum rtx_code code;
     rtx op0;
     rtx op1;
     rtx *reg_p;
{
  rtx f0, seq;
  enum machine_mode mode;

  start_sequence ();
  if (GET_MODE (op0) == DImode)
    {
      int unsigned_p = (code == GTU || code == GEU || code == LTU || code == LEU);

      if (op1 == CONST0_RTX (DImode))
	op0 = emit_library_call_value (gen_rtx (SYMBOL_REF, Pmode, (unsigned_p) ? "__ucmpdi0" : "__cmpdi0"),
				       NULL_RTX, TRUE, HImode, 1, op0, DImode);
      else
	op0 = emit_library_call_value (gen_rtx (SYMBOL_REF, Pmode, (unsigned_p) ? "__ucmpdi" : "__cmpdi"),
				       NULL_RTX, TRUE, HImode, 2,
				       op0, DImode,
				       op1, DImode);

      op1 = const1_rtx;
    }

  mode = (emit_comparison (code, op0, op1, NULL_RTX, NULL_RTX, GET_MODE (op0), FALSE) ? CCmode : CC_REVmode);
  f0 = gen_rtx (REG, mode, F0_REGNUM);

  emit_insn (gen_rtx (SET, VOIDmode, f0, gen_rtx (code, mode, op0, op1)));
  seq = gen_sequence ();
  end_sequence ();

  *reg_p = f0;
  return seq;
}


/* Emit the instructions to move 1 word.  */

char *
emit_move_word (op0, op1, insn)
     rtx op0;
     rtx op1;
     rtx insn;
{
  char *ret = "";
  int shift;

  if (REG_P (op0) && REG_P (op1))
    {
      int reg0 = REGNO (op0);
      int reg1 = REGNO (op1);

      if (GPR_P (reg0) && GPR_P (reg1))
	ret = "mv %0,%1";

      else if (GPR_P (reg0) && CR_P (reg1))
	ret = "mvfc %0,%1";

      else if (CR_P (reg0) && GPR_P (reg1))
	ret = "mvtc %1,%0";

      else if (GPR_P (reg0) && reg1 == F0_REGNUM)
	ret = "setf0t %0";

      else
	fatal_insn ("emit_move_word:", insn);
    }

  else if (REG_P (op0) && GPR_P (REGNO (op0)) && GET_CODE (op1) == MEM)
    ret = "ld %0,%M1";

  else if (GET_CODE (op0) == MEM && REG_P (op1) && GPR_P (REGNO (op1)))
    ret = "st %1,%M0";

  else if (GET_CODE (op0) == MEM && GET_CODE (op1) == CONST_INT && INTVAL (op1) == 0)
    ret = "st %.,%M0";

  /* XXX: We should do LABEL_REF's that are not part switch statements with the
     @word modifier, but if we do, that breaks switch statements which need to
     index into the array of labels.  Using casesi is clumsy because it converts
     the switch value to SI before the call.  */
  else if (REG_P (op0) && GPR_P (REGNO (op0))
	   && GET_CODE (op1) == SYMBOL_REF && SYMBOL_REF_FLAG (op1))
    ret = "ldi %0,%1@word";

  else if (REG_P (op0) && GPR_P (REGNO (op0))
	   && GET_CODE (op1) == CONST_INT
	   && ldi_shift_operand (op1, HImode))
    ret = "ldi %0,%s1\n\tslli %0,%S1";

  else if (REG_P (op0) && GPR_P (REGNO (op0))
	   && (GET_CODE (op1) == CONST_INT
	       || GET_CODE (op1) == SYMBOL_REF
	       || GET_CODE (op1) == LABEL_REF
	       || GET_CODE (op1) == CONST))
    ret = "ldi %0,%1";

  else
    fatal_insn ("emit_move_word:", insn);

  return ret;
}



/* Emit the instructions to move 2 words.  */

char *
emit_move_2words (op0, op1, insn)
     rtx op0;
     rtx op1;
     rtx insn;
{
  char *ret = "";

  if (REG_P (op0) && REG_P (op1))
    {
      int reg0 = REGNO (op0);
      int reg1 = REGNO (op1);

      if (GPR_P (reg0) && GPR_P (reg1))
	ret = "mv2w %0,%1";

      else if (GPR_P (reg0) && ACCUM_P (reg1))
	ret = "mv2wfac %0,%1";

      else if (ACCUM_P (reg0) && GPR_P (reg1))
	ret = "mv2wtac %1,%0";

      else
	fatal_insn ("emit_move_2words:", insn);
    }

  else if (REG_P (op0) && GPR_P (REGNO (op0)) && GET_CODE (op1) == MEM)
    ret = "ld2w %0,%M1";

  else if (GET_CODE (op0) == MEM && REG_P (op1) && GPR_P (REGNO (op1)))
    ret = "st2w %1,%M0";

  else if (REG_P (op0) && GPR_P (REGNO (op0))
	   && (GET_CODE (op1) == CONST_INT || GET_CODE (op1) == CONST_DOUBLE))
    ret = "#";

  else if (REG_P (op0) && ACCUM_P (REGNO (op0)) && GET_CODE (op1) == CONST_INT && INTVAL (op1) == 0)
    ret = "clrac %0";

  else
    fatal_insn ("emit_move_2words:", insn);

  return ret;
}



/* Emit the instructions to move 4 words.  */

char *
emit_move_4words (op0, op1, tmp_si, tmp_hi, insn)
     rtx op0;
     rtx op1;
     rtx tmp_si;
     rtx tmp_hi;
     rtx insn;
{
  char *ret = "";
  int gpr0_p = (REG_P (op0) && GPR_P (REGNO (op0)));
  int gpr1_p = (REG_P (op1) && GPR_P (REGNO (op1)));
  int mem0_p = (GET_CODE (op0) == MEM);
  int mem1_p = (GET_CODE (op1) == MEM);
  int const1_p = (GET_CODE (op1) == CONST_INT || GET_CODE (op1) == CONST_DOUBLE);

  /* Check for overlap */
  if (gpr0_p && refers_to_regno_p (REGNO (op0), REGNO (op0)+2, op1, (rtx *)0))
    {
      if (TARGET_DEBUG_MOVE4)
	{
	  fprintf (stderr, "\n%s: overlap found in emit_move_4words\n",
		   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
	  debug_rtx (insn);
	  fprintf (stderr, "----------\n");
	}

      if (gpr1_p)
	{
	  if (REGNO (op0) == REGNO (op1) + 2)
	    ret = "mv2w %C0,%C1\n\tmv2w %A0,%A1";
	  else
	    ret = "mv2w %A0,%A1\n\tmv2w %C0,%C1";
	}

      else if (mem1_p)
	ret = "ld2w %C0,%C1\n\tld2w %A0,%A1";

      else
	fatal_insn ("move 4 words overlap", insn);
    }

  else if (gpr0_p && gpr1_p)
    {
      if (REGNO (op0) == REGNO (op1) + 2)
	ret = "mv2w %C0,%C1\n\tmv2w %A0,%A1";
      else
	ret = "mv2w %A0,%A1\n\tmv2w %C0,%C1";
    }

  else if (gpr0_p && mem1_p)
    {
      /* If we use post increment/post decrement, it allows the second instruction
	 to also be a 16-bit instruction */
      rtx addr = XEXP (op1, 0);

      if (GET_CODE (addr) == REG && REGNO (addr) != STACK_POINTER_REGNUM
	  && !IN_RANGE_P (REGNO (addr), REGNO (op0), REGNO (op0)+3))
	ret = "ld2w %A0,@%1+\n\tld2w %C0,@%1-";

      else if (GET_CODE (addr) == REG && REGNO (addr) != STACK_POINTER_REGNUM
	       && IN_RANGE_P (REGNO (addr), REGNO (op0)+2, REGNO (op0)+3))
	ret = "ld2w %A0,@%1+\n\tld2w %C0,@%1";

      else
	ret = "ld2w %A0,%A1\n\tld2w %C0,%C1";
    }

  else if (mem0_p && gpr1_p)
    {
      /* If we use post increment/post decrement, it allows the second instruction
	 to also be a 16-bit instruction */
      rtx addr = XEXP (op0, 0);

      if (GET_CODE (addr) == REG && REGNO (addr) != STACK_POINTER_REGNUM
	  && !IN_RANGE_P (REGNO (addr), REGNO (op1), REGNO (op1)+3))
	ret = "st2w %A1,@%0+\n\tst2w %C1,@%0-";
      else
	ret = "st2w %A1,%A0\n\tst2w %C1,%C0";
    }

  else if (mem0_p && mem1_p)
    {
      if (TARGET_DEBUG_MOVE4)
	{
	  fprintf (stderr, "\n%s: memory to memory move in emit_move_4wordsn",
		   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

	  debug_rtx (insn);
	  fprintf (stderr, "----------\n");
	}

      /* Check for the scratch space overlapping registers used for pointers.  */
      if (GET_CODE (tmp_si) == REG && GET_MODE (tmp_si) == SImode)
	{
	  int reg = REGNO (tmp_si);

	  if (refers_to_regno_p (reg, reg+2, op0, (rtx *)0)
	      || refers_to_regno_p (reg, reg+2, op1, (rtx *)0))
	    {
	      if (!refers_to_regno_p (reg, reg+1, op0, (rtx *)0)
		  && !refers_to_regno_p (reg, reg+1, op1, (rtx *)0))
		{
		  tmp_si = NULL_RTX;
		  tmp_hi = gen_rtx (REG, HImode, reg);
		}
	      else if (!refers_to_regno_p (reg+1, reg+2, op0, (rtx *)0)
		       && !refers_to_regno_p (reg+1, reg+2, op1, (rtx *)0))
		{
		  tmp_si = NULL_RTX;
		  tmp_hi = gen_rtx (REG, HImode, reg+1);
		}
	      else
		fatal_insn ("emit_move_4words:", insn);
	    }
	}
      else if (GET_CODE (tmp_hi) == REG && GET_MODE (tmp_hi) == HImode)
	{
	  if (refers_to_regno_p (REGNO (tmp_hi), REGNO (tmp_hi)+1, op0, (rtx *)0)
	      || refers_to_regno_p (REGNO (tmp_hi), REGNO (tmp_hi)+1, op1, (rtx *)0))
	    fatal_insn ("emit_move_4words:", insn);
	}

      if (tmp_si && GET_CODE (tmp_si) == REG && GET_MODE (tmp_si) == SImode)
	ret = "ld2w %2,%A1\n\tst2w %2,%A0\n\tld2w %2,%C1\n\tst2w %2,%C0";

      else if (tmp_hi && GET_CODE (tmp_hi) == REG && GET_MODE (tmp_hi) == HImode)
	ret = "ld %3,%A1\nst %3,%A0\nld %3,%B1\nst %3,%B0\nld %3,%C1\nst %3,%C0\nld %3,%D1\nst %3,%D0\n";

      else
	fatal_insn ("emit_move_4words:", insn);
    }

  else if (gpr0_p
	   && (GET_CODE (op1) == CONST_INT || GET_CODE (op1) == CONST_DOUBLE))
    ret = "ldi %A0,%A1\n\tldi %B0,%B1\n\tldi %C0,%C1\n\tldi %D0,%D1";

  else if (mem0_p
	   && ((GET_CODE (op1) == CONST_INT && INTVAL (op1) == 0)
	       || (GET_CODE (op1) == CONST_DOUBLE
		   && CONST_DOUBLE_LOW (op1) == 0
		   && CONST_DOUBLE_HIGH (op1) == 0)))
    ret = "st %.,%A0\n\tst %.,%B0\n\tst %.,%C0\n\tst %.,%D0";

  else if (mem0_p && const1_p && GET_CODE (tmp_si) == REG && GET_MODE (tmp_si) == SImode)
    {
      int part0, part1, part2, part3;

      if (GET_CODE (op1) == CONST_INT && INTVAL (op1) < 0)
	{
	  HOST_WIDE_INT value = INTVAL (op1);
	  part0 = 0xffff;
	  part1 = 0xffff;
	  part2 = (value >> 16) & 0xffff;
	  part3 = value & 0xffff;
	}

      else if (GET_CODE (op1) == CONST_INT)
	{
	  HOST_WIDE_INT value = INTVAL (op1);
	  part0 = 0;
	  part1 = 0;
	  part2 = (value >> 16) & 0xffff;
	  part3 = value & 0xffff;
	}

      else
	{
	  part0 = (CONST_DOUBLE_HIGH (op1) >> 16) & 0xffff;
	  part1 = CONST_DOUBLE_HIGH (op1) & 0xffff;
	  part2 = (CONST_DOUBLE_LOW (op1) >> 16) & 0xffff;
	  part3 = CONST_DOUBLE_LOW (op1) & 0xffff;
	}

      if (part0 == part2 && part1 == part3)
	ret = "ldi %U2,%A1\n\tldi %L2,%B1\n\tst2w %2,%A0\n\tst2w %2,%C0";

      else if (part0 == part2)
	ret = "ldi %U2,%A1\n\tldi %L2,%B1\n\tst2w %2,%A0\n\tldi %L2,%D1\n\tst2w %2,%C0";

      else if (part1 == part3)
	ret = "ldi %U2,%A1\n\tldi %L2,%B1\n\tst2w %2,%A0\n\tldi %U2,%C1\n\tst2w %2,%C0";

      else
	ret = "ldi %U2,%A1\n\tldi %L2,%B1\n\tst2w %2,%A0\n\tldi %U2,%C1\n\tldi %L2,%D1\n\tst2w %2,%C0";
    }

  else
    fatal_insn ("emit_move_4words:", insn);

  return ret;
}



/* Emit code to do add of multiword values */

char *
emit_add (operands, insn)
     rtx operands[], insn;
{
  rtx op0 = operands[0];
  rtx op1 = operands[1];
  rtx op2 = operands[2];
  enum machine_mode mode = GET_MODE (op0);
  int regno0 = (GET_CODE (op0) == REG) ? REGNO (op0) : -1;
  int regno1 = (GET_CODE (op1) == REG) ? REGNO (op1) : -1;
  int regno2 = (GET_CODE (op2) == REG) ? REGNO (op2) : -1;
  HOST_WIDE_INT value;

  if (mode == SImode)
    {
      if (GET_CODE (op2) != CONST_INT)
	{
	  if (GPR_P (regno0) && regno0 == regno1 && GPR_P (regno2))
	    return "add2w %0,%2";

	  else if (ACCUM_P (regno0) && regno0 == regno1 && (GPR_P (regno2) || ACCUM_P (regno2)))
	    return "add %0,%2";

	  else if (GPR_P (regno0) && (GPR_P (regno1) || ACCUM_P (regno1)) && ACCUM_P (regno2) && TARGET_ADDAC3)
	    return "addac3 %0,%1,%2";
	}

      else if (regno0 != regno1)
	return "ldi %U0,%U2\n\tldi %L0,%L2\n\tadd2w %0,%1";

      else if (INTVAL (op2) < 0)
	fatal_insn ("emit_add, negative constant", insn);

      else if (GPR_P (regno0) && GPR_P (regno1))
	{
	  value = INTVAL (op2);
	  if (IN_RANGE_P (value, 1, 16) && regno0 == regno1)
	    return "addi %L0,%2\n\tcpfg f0,c\n\texef0t || addi %U0,1";

	  else if (regno0 == regno1)
	    return "add3 %L0,%L0,%2\n\tcpfg f0,c\n\texef0t || addi %U0,1";

	  else
	    return "add3 %L0,%L1,%2\n\tcpfg f0,c || mv %U0,%U1\n\texef0t || addi %U0,1";
	}
    }

  else if (mode == DImode && regno0 == regno1 && GPR_P (regno0))
    {
      int num_carries = 0;
      if (GPR_P (regno2))
	{
	  output_asm_insn ("add2w %A0,%A2", operands);
	  output_asm_insn ("add2w %C0,%C2", operands);
	  num_carries = 2;
	}

      else if (GET_CODE (op2) == CONST_INT && INTVAL (op2) >= 0)
	{
	  num_carries = 3;
	  value = INTVAL (op2);
	  if (IN_RANGE_P (value, 1, 16) && regno0 == regno1)
	    output_asm_insn ("addi %D0,%2", operands);

	  else if (IN_RANGE_P (value, 1, 16) && TARGET_SMALL_INSNS && optimize)
	    output_asm_insn ("mv %D0,%D1\n\taddi %D0,%2", operands);

	  else
	    output_asm_insn ("add3 %D0,%D1,%2", operands);
	}

      else if (GET_CODE (op2) == CONST_DOUBLE
	       && CONST_DOUBLE_HIGH (op2) == 0
	       && CONST_DOUBLE_LOW (op2) >= 0)
	{
	  num_carries = 3;
	  value = CONST_DOUBLE_LOW (op2);
	  if (IN_RANGE_P (value, 1, 16) && regno0 == regno1)
	    output_asm_insn ("addi %D0,%2", operands);

	  else if (IN_RANGE_P (value, 1, 16) && TARGET_SMALL_INSNS && optimize)
	    output_asm_insn ("mv %D0,%D1\n\taddi %D0,%2", operands);

	  else
	    output_asm_insn ("add3 %D0,%D1,%2", operands);
	}

      if (num_carries)
	{
	  output_asm_insn ("cpfg f0,c", operands);

	  if (num_carries == 3)
	    {
	      output_asm_insn ("exef0t || addi %C0,1", operands);
	      output_asm_insn ("cpfg f0,c || exef0t", operands);
	    }

	  output_asm_insn ("exef0t || addi %B0,1", operands);
	  output_asm_insn ("cpfg f0,c || exef0t", operands);
	  output_asm_insn ("exef0t || addi %A0,1", operands);
	  return "";
	}
    }

  fatal_insn ("emit_add:", insn);
}


/* Emit code to do subtract of multiword values */

char *
emit_subtract (operands, insn)
     rtx operands[], insn;
{
  rtx op0 = operands[0];
  rtx op1 = operands[1];
  rtx op2 = operands[2];
  enum machine_mode mode = GET_MODE (op0);
  int regno0 = (GET_CODE (op0) == REG) ? REGNO (op0) : -1;
  int regno1 = (GET_CODE (op1) == REG) ? REGNO (op1) : -1;
  int regno2 = (GET_CODE (op2) == REG) ? REGNO (op2) : -1;
  HOST_WIDE_INT value;

  if (mode == SImode)
    {
      if (GPR_P (regno0) && regno0 == regno1 && GPR_P (regno2))
	return "sub2w %0,%2";

      else if (ACCUM_P (regno0) && regno0 == regno1 && (GPR_P (regno2) || ACCUM_P (regno2)))
	return "sub %0,%2";

      else if (GPR_P (regno0) && (GPR_P (regno1) || ACCUM_P (regno1)) && ACCUM_P (regno2) && TARGET_ADDAC3)
	return "subac3 %0,%1,%2";
    }

  else if (mode == DImode && GPR_P (regno0) && regno0 == regno1 && GPR_P (regno2))
    return "sub2w %A0,%A2\n\tsub2w %C0,%C2\n\tcpfg f0,c\n\texef0f || subi %B0,1\n\tcpfg f0,c || exef0f\n\texef0f || subi %A0,1";

  fatal_insn ("emit_subtract:", insn);
}


/* Output a conditional move instruction
   operands[0] is the destination
   operands[1] is the NE test
   operands[2] is f0
   operands[3] is the value to move if the test was true
   operands[4] is the value to move if the test was false  */

char *
emit_cond_move (operands, insn)
     rtx operands[];
     rtx insn;
{
  char *load = (char *)0;
  char *store = (char *)0;
  char *move_false = (char *)0;
  char *move_true = (char *)0;
  rtx dest = operands[0];
  rtx xop[10];
  rtx op_true;
  rtx op_false;
  enum machine_mode mode = GET_MODE (dest);
  char buffer[80];

  if (GET_MODE (operands[1]) == CC_REVmode)
    {
      op_true = operands[4];
      op_false = operands[3];
    }
  else
    {
      op_true = operands[3];
      op_false = operands[4];
    }

  switch (mode)
    {
    case QImode:
      load = "ldub";
      store = "stb";
      move_false = "mvf0f %0,%1";
      move_true = "mvf0t %0,%1";
      break;

    case HImode:
      load = "ld";
      store = "st";
      move_false = "mvf0f %0,%1";
      move_true = "mvf0t %0,%1";
      break;

    case SImode:
    case SFmode:
      load = "ld2w";
      store = "st2w";
      move_false = "exef0f || mv2w %0,%1";
      move_true = "exef0t || mv2w %0,%1";
      break;

    default:
      fatal_insn ("emit_cond_move", insn);
    }

  xop[0] = dest;
  if (GET_CODE (dest) == REG
      && GET_CODE (op_false) == CONST_INT
      && !refers_to_regno_p (REGNO (dest), REGNO (dest)+1, op_true, (rtx *)0))
    {
      xop[1] = op_false;
      output_asm_insn ("ldi %0,%1", xop);

      xop[1] = op_true;
      switch (GET_CODE (op_true))
	{
	case REG:
	  output_asm_insn (move_true, xop);
	  break;

	case MEM:
	  sprintf (buffer, "%s %%0,%%M1 || exef0t", load);
	  output_asm_insn (buffer, xop);
	  break;

	case CONST_INT:
	  output_asm_insn ("exef0t || ldi %0,%1", xop);
	  break;

	default:
	  fatal_insn ("emit_cond_move", insn);
	}
    }

  else if (GET_CODE (dest) == REG
	   && GET_CODE (op_true) == CONST_INT
	   && !refers_to_regno_p (REGNO (dest), REGNO (dest)+1, op_false, (rtx *)0))
    {
      xop[1] = op_true;
      output_asm_insn ("ldi %0,%1", xop);

      xop[1] = op_false;
      switch (GET_CODE (op_false))
	{
	case REG:
	  output_asm_insn (move_false, xop);
	  break;

	case MEM:
	  sprintf (buffer, "%s %%0,%%M1 || exef0f", load);
	  output_asm_insn (buffer, xop);
	  break;

	default:
	  fatal_insn ("emit_cond_move", insn);
	}
    }

  else if (GET_CODE (dest) == REG)
    {
      int dest_reg = REGNO (dest);

      /* Move value into register for false condition */
      xop[1] = op_false;
      switch (GET_CODE (op_false))
	{
	case REG:
	  if (REGNO (op_false) != dest_reg)
	    output_asm_insn (move_false, xop);
	  break;

	case MEM:
	  sprintf (buffer, "%s %%0,%%M1 || exef0f", load);
	  output_asm_insn (buffer, xop);
	  break;

	case CONST_INT:
	  output_asm_insn ("exef0f || ldi %0,%1", xop);
	  break;

	default:
	  fatal_insn ("emit_cond_move", insn);
	}

      /* Move value into register for true condition */
      xop[1] = op_true;
      switch (GET_CODE (op_true))
	{
	case REG:
	  if (REGNO (op_true) != dest_reg)
	    output_asm_insn (move_true, xop);
	  break;

	case MEM:
	  sprintf (buffer, "%s %%0,%%M1 || exef0t", load);
	  output_asm_insn (buffer, xop);
	  break;

	case CONST_INT:
	  output_asm_insn ("exef0t || ldi %0,%1", xop);
	  break;

	default:
	  fatal_insn ("emit_cond_move", insn);
	}
    }

  else if (GET_CODE (dest) == MEM
	   && (GET_CODE (op_false) == REG
	       || (GET_CODE (op_false) == CONST_INT
		   && INTVAL (op_false) == 0
		   && GET_MODE_SIZE (mode) <= 2))
	   && (GET_CODE (op_true) == REG
	       || (GET_CODE (op_true) == CONST_INT
		   && INTVAL (op_true) == 0
		   && GET_MODE_SIZE (mode) <= 2)))
    {
      xop[1] = op_false;
      sprintf (buffer, "%s %s,%%M0 || exef0f", store, (GET_CODE (op_false) == CONST_INT ? "%." : "%1"));
      output_asm_insn (buffer, xop);

      xop[1] = op_true;
      sprintf (buffer, "%s %s,%%M0 || exef0t", store, (GET_CODE (op_true) == CONST_INT ? "%." : "%1"));
      output_asm_insn (buffer, xop);
    }

  else
    fatal_insn ("emit_cond_move", insn);

  return "";
}


/* Split a multiword logical operation into its component parts */

Rtx
d10v_split_logical_op (operands, code)
     rtx operands[];
     enum rtx_code code;
{
  rtx ret;
  rtx op0 = operands[0];
  rtx op1 = operands[1];
  rtx op2 = operands[2];
  rtx split_op0;
  rtx split_op1;
  rtx split_op2;
  int split_values[4];
  int i, max_words;
  int off0 = 0, off1 = 0, off2 = 0;
  enum machine_mode mode = GET_MODE (op0);
  rtx insn;

  if (GET_CODE (op0) == SUBREG)
    {
      off0 = SUBREG_WORD (op0);
      op0 = SUBREG_REG (op0);
    }

  if (GET_CODE (op0) != REG)
    goto fail;

  if (GET_CODE (op1) == SUBREG)
    {
      off1 = SUBREG_WORD (op1);
      op1 = SUBREG_REG (op1);
    }

  if (GET_CODE (op1) != REG)
    goto fail;

  if (code != NOT)
    {
      if (GET_CODE (op2) == SUBREG)
	{
	  off2 = SUBREG_WORD (op2);
	  op2 = SUBREG_REG (op2);
	}

      if (GET_CODE (op2) == CONST_INT || GET_CODE (op2) == CONST_DOUBLE)
	{
	  if (mode == DImode)
	    d10v_split_constant64 (op2, split_values);

	  else if (mode == SImode)
	    d10v_split_constant32 (op2, split_values);

	  else
	    goto fail;
	}
      else if (GET_CODE (op2) != REG)
	goto fail;
    }
  else
    op2 = NULL_RTX;

  max_words = GET_MODE_SIZE (mode) / UNITS_PER_WORD;
  start_sequence ();

  for (i = 0; i < max_words; i++)
    {
      if (!reload_completed)
	{
	  split_op0 = gen_rtx (SUBREG, HImode, op0, i+off0);
	  split_op1 = gen_rtx (SUBREG, HImode, op1, i+off1);
	}
      else
	{
	  split_op0 = gen_rtx (REG, HImode, REGNO (op0) + i+off0);
	  split_op1 = gen_rtx (REG, HImode, REGNO (op1) + i+off1);
	}

      if (code == NOT)
	{
	  emit_insn (gen_rtx (SET, VOIDmode,
			      split_op0,
			      gen_rtx (NOT, HImode, split_op1)));
	  continue;
	}

      if (GET_CODE (op2) != REG)
	{
	  /* Optimize <reg> <op>= <value> operations to skip NOPs */
	  if ((REGNO (op0) + i + off0) == (REGNO (op1) + i + off1))
	    {
	      if (split_values[i] == 0 && code == IOR)
		continue;

	      if ((split_values[i] & 0xffff) == 0xffff && code == AND)
		continue;
	    }
	  
	  /* Optimize <reg1> = <reg2> <op> <value> that turn into moves */
	  else if ((split_values[i] == 0 && code == IOR)
		   || ((split_values[i] & 0xffff) == 0xffff && code == AND))
	    {
	      emit_insn (gen_rtx (SET, VOIDmode, split_op0, split_op1));
	      continue;
	    }

	  split_op2 = GEN_INT (split_values[i]);
	}
      else
	split_op2 = gen_rtx (SUBREG, HImode, op2, i+off2);

      emit_insn (gen_rtx (SET, VOIDmode,
			  split_op0,
			  gen_rtx (code, HImode, split_op1, split_op2)));
    }

  ret = gen_sequence ();
  end_sequence ();
  return ret;

fail:
  if (code != NOT)
    insn = gen_rtx (SET, VOIDmode,
		    operands[0],
		    gen_rtx (code, mode, operands[1], operands[2]));
  else
    insn = gen_rtx (SET, VOIDmode,
		    operands[0],
		    gen_rtx (NOT, mode, operands[1]));

  fatal_insn ("bad insn to d10v_split_logical_op", insn);
  return NULL_RTX;
}


/* Split a 32-bit constant into 2 16-bit values.  */

static void
d10v_split_constant32 (x, values)
     rtx x;
     int values[];
{
  enum rtx_code code = GET_CODE (x);
  HOST_WIDE_INT v;

  if (code == CONST_INT)
    {
      v = INTVAL (x);
      values[0] = SIGN_EXTEND_SHORT (v >> 16);
      values[1] = SIGN_EXTEND_SHORT (v);
    }

  else if (code == CONST_DOUBLE && GET_MODE (x) == SFmode)
    {
      REAL_VALUE_TYPE rv;

      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_SINGLE (rv, v);
      values[0] = SIGN_EXTEND_SHORT (v >> 16);
      values[1] = SIGN_EXTEND_SHORT (v);
    }

  else
    fatal_insn ("Bad rtx passed to d10v_split_constant32:", x);
}


/* Split a 64-bit constant into 4 16-bit values.  */

static void
d10v_split_constant64 (x, values)
     rtx x;
     int values[];
{
  enum rtx_code code = GET_CODE (x);
  HOST_WIDE_INT v;

  if (code == CONST_INT)
    {
      v = INTVAL (x);
      if (v < 0)
	values[0] = values[1] = -1;
      else
	values[0] = values[1] = 0;

      values[2] = SIGN_EXTEND_SHORT (v >> 16);
      values[3] = SIGN_EXTEND_SHORT (v);
    }

  else if (code == CONST_DOUBLE && GET_MODE (x) == DFmode)
    {
      long t[2];
      REAL_VALUE_TYPE rv;

      /* d10v is big endian, so the high word is first */
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_DOUBLE (rv, t);
      values[0] = SIGN_EXTEND_SHORT (t[0] >> 16);
      values[1] = SIGN_EXTEND_SHORT (t[0]);
      values[2] = SIGN_EXTEND_SHORT (t[1] >> 16);
      values[3] = SIGN_EXTEND_SHORT (t[1]);
    }

  else if (code == CONST_DOUBLE
	   && (GET_MODE (x) == DImode || GET_MODE (x) == VOIDmode))
    {
      v = CONST_DOUBLE_HIGH (x);
      values[0] = SIGN_EXTEND_SHORT (v >> 16);
      values[1] = SIGN_EXTEND_SHORT (v);

      v = CONST_DOUBLE_LOW (x);
      values[2] = SIGN_EXTEND_SHORT (v >> 16);
      values[3] = SIGN_EXTEND_SHORT (v);
    }

  else
    fatal_insn ("Bad rtx passed to d10v_split_constant64:", x);
}


/* Print an integer constant expression in assembler syntax.
   Note, for the d10v, we need to explicitly truncate integer
   constants to 16 bits.  */

void
d10v_output_addr_const (file, x)
     FILE *file;
     rtx x;
{
  char buf[256];

 restart:
  switch (GET_CODE (x))
    {
    default:
      output_addr_const (file, x);
      break;

    case SYMBOL_REF:
      /* We could use parentheses only for symbols whose names conflict
         with the register names.  But it would make the compiler
         slower. */
      fputc ('(', file);
      assemble_name (file, XSTR (x, 0));
      fputc (')', file);
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
      fprintf (file, "%d", SIGN_EXTEND_SHORT (INTVAL (x)));
      break;

    case CONST:
      d10v_output_addr_const (file, XEXP (x, 0));
      break;

    case PLUS:
      /* Some assemblers need integer constants to appear last (eg masm).  */
      if (GET_CODE (XEXP (x, 0)) == CONST_INT)
	{
	  d10v_output_addr_const (file, XEXP (x, 1));
	  if (INTVAL (XEXP (x, 0)) >= 0)
	    fprintf (file, "+");
	  d10v_output_addr_const (file, XEXP (x, 0));
	}
      else
	{
	  d10v_output_addr_const (file, XEXP (x, 0));
	  if (INTVAL (XEXP (x, 1)) >= 0)
	    fprintf (file, "+");
	  d10v_output_addr_const (file, XEXP (x, 1));
	}
      break;

    case MINUS:
      /* Avoid outputting things like x-x or x+5-x,
	 since some assemblers can't handle that.  */
      x = simplify_subtraction (x);
      if (GET_CODE (x) != MINUS)
	goto restart;

      d10v_output_addr_const (file, XEXP (x, 0));
      fprintf (file, "-");
      if (GET_CODE (XEXP (x, 1)) == CONST_INT
	  && INTVAL (XEXP (x, 1)) < 0)
	{
	  fprintf (file, ASM_OPEN_PAREN);
	  d10v_output_addr_const (file, XEXP (x, 1));
	  fprintf (file, ASM_CLOSE_PAREN);
	}
      else
	d10v_output_addr_const (file, XEXP (x, 1));
      break;

    case ZERO_EXTEND:
    case SIGN_EXTEND:
      d10v_output_addr_const (file, XEXP (x, 0));
      break;
    }
}


/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is X.  X is an RTL expression.

   On some machines, the syntax for a symbolic address depends on the
   section that the address refers to.  On these machines, define the
   macro `ENCODE_SECTION_INFO' to store the information into the
   `symbol_ref', and then check for it here.  *Note Assembler
   Format::.  */

void
print_operand_address (stream, x)
     FILE *stream;
     rtx x;
{
  rtx x0, x1;

  if (GET_CODE (x) == MEM)
    x = XEXP (x, 0);

  switch (GET_CODE (x))
    {
    default:
      break;

    case REG:
      fputs (reg_names[ REGNO (x) ], stream);
      return;

    case CONST_INT:
      fprintf (stream, "%d", SIGN_EXTEND_SHORT (INTVAL (x)));
      return;

    case SYMBOL_REF:
    case CONST:
    case LABEL_REF:
      d10v_output_addr_const (stream, x);
      return;
    }

  fatal_insn ("Bad insn to print_operand_address:", x);
}


/* Print a memory reference suitable for the ld/st instructions.  */

static void
print_operand_memory_reference (stream, x)
     FILE *stream;
     rtx x;
{
  rtx x0, x1;
  int offset;

  switch (GET_CODE (x))
    {
    default:
      fatal_insn ("Bad insn to print_operand_memory_reference:", x);
      break;

    case SUBREG:
      offset = 0;
      do {
	offset += SUBREG_WORD (x);
	x = SUBREG_REG (x);
      } while (GET_CODE (x) == SUBREG);
      fprintf (stream, "@%s", reg_names[ REGNO (x)+offset ]);
      break;

    case REG:
      fprintf (stream, "@%s", reg_names[ REGNO (x) ]);
      break;

    case CONST_INT:
      fprintf (stream, "@(%d,%s)", SIGN_EXTEND_SHORT (INTVAL (x)), reg_names[ GPR_ZERO_REGNUM ]);
      break;

    case SYMBOL_REF:
    case CONST:
    case LABEL_REF:
      fputs ("@(", stream);
      d10v_output_addr_const (stream, x);
      fprintf (stream, ",%s)", reg_names[ GPR_ZERO_REGNUM ]);
      break;

    case PLUS:
      x0 = XEXP (x, 0);
      x1 = XEXP (x, 1);
      offset = 0;
      while (GET_CODE (x0) == SUBREG)
	{
	  offset += SUBREG_WORD (x0);
	  x0 = SUBREG_REG (x0);
	}

      if (CONSTANT_ADDRESS_P (x1) && GET_CODE (x0) == REG && GPR_P (REGNO (x0)))
	{
	  if (GET_CODE (x1) == CONST_INT && INTVAL (x1) == 0)
	    fprintf (stream, "@%s", reg_names[ REGNO (x0)+offset ]);
	  else if (GET_CODE (x1) == CONST_INT)
	    fprintf (stream, "@(%d,%s)", SIGN_EXTEND_SHORT (INTVAL (x1)),
		     reg_names[ REGNO (x0)+offset ]);
	  else
	    {
	      fputs ("@(", stream);
	      d10v_output_addr_const (stream, x1);
	      fprintf (stream, ",%s)", reg_names[ REGNO (x0)+offset ]);
	      return;
	    }
	}
      else
	fatal_insn ("Bad insn to print_operand_memory_reference:", x);

      break;

    case POST_INC:
      x0 = XEXP (x, 0);
      if (GET_CODE (x0) == REG && GPR_P (REGNO (x0)))
	fprintf (stream, "@%s+", reg_names[ REGNO (x0) ]);
      else
	fatal_insn ("Bad insn to print_operand_memory_reference:", x);

      break;

    case POST_DEC:
      x0 = XEXP (x, 0);
      if (GET_CODE (x0) == REG && GPR_P (REGNO (x0))
	  && REGNO (x0) != STACK_POINTER_REGNUM)
	fprintf (stream, "@%s-", reg_names[ REGNO (x0) ]);
      else
	fatal_insn ("Bad insn to print_operand_memory_reference:", x);

      break;

    case PRE_DEC:
      x0 = XEXP (x, 0);
      if (GET_CODE (x0) == REG && REGNO (x0) == STACK_POINTER_REGNUM)
	fprintf (stream, "@-%s", reg_names[ REGNO (x0) ]);
      else
	fatal_insn ("Bad insn to print_operand_memory_reference:", x);

      break;

    }
}


/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand X.  X is an RTL
   expression.

   LETTER is a value that can be used to specify one of several ways of
   printing the operand.  It is used when identical operands must be
   printed differently depending on the context.  LETTER comes from the
   `%' specification that was used to request printing of the
   operand.  If the specification was just `%DIGIT' then LETTER is 0;
   if the specification was `%LTR DIGIT' then LETTER is the ASCII code
   for LTR.

   If X is a register, this macro should print the register's name.
   The names can be found in an array `reg_names' whose type is `char
   *[]'.  `reg_names' is initialized from `REGISTER_NAMES'.

   When the machine description has a specification `%PUNCT' (a `%'
   followed by a punctuation character), this macro is called with a
   null pointer for X and the punctuation character for LETTER.  */

void
print_operand (stream, x, letter)
    FILE *stream;
    rtx x;
    int letter;
{
  enum rtx_code code = (x) ? GET_CODE (x) : NIL;
  int regadd;
  int split_values[4];
  int shift, log;
  rtx x0;

  switch (letter)
    {
    case '|':	/* Issue parallel instructions */
      fputs (" || ", stream);
      break;

    case '.':	/* Output r14 */
      fputs (reg_names[GPR_ZERO_REGNUM], stream);
      break;

    case 'U':	/* Extract the upper part (word 0) of a 32-bit value */
      regadd = 0;
      goto common_32bit;

    case 'L':	/* Extract the lower part (word 1) of a 32-bit value */
      regadd = 1;
      /* fall through */

    common_32bit:
      if (code == CONST_INT || code == CONST_DOUBLE)
	{
	  d10v_split_constant32 (x, split_values);
	  fprintf (stream, "%d", SIGN_EXTEND_SHORT (split_values[regadd]));
	}

      else if (code == REG)
	fprintf (stream, "%s", reg_names[ REGNO (x)+regadd ]);

      else if (code == MEM && offsettable_memref_p (x))
	print_operand_address (stream, plus_constant (XEXP (x, 0), regadd * UNITS_PER_WORD));

      else
	fatal_insn ("Bad insn for %U/%L:", x);

      break;

    case 'A':	/* Extract word 0 of a 64-bit value */
    case 'B':	/* Extract word 1 of a 64-bit value */
    case 'C':	/* Extract word 2 of a 64-bit value */
    case 'D':	/* Extract word 3 of a 64-bit value */
      regadd = letter - 'A';
      if (code == CONST_INT || code == CONST_DOUBLE)
	{
	  d10v_split_constant64 (x, split_values);
	  fprintf (stream, "%d", split_values[regadd]);
	}

      else if (code == REG)
	fprintf (stream, "%s", reg_names[ REGNO (x)+regadd ]);

      else if (code == MEM && offsettable_memref_p (x))
	print_operand_memory_reference (stream, plus_constant (XEXP (x, 0), regadd * UNITS_PER_WORD));

      else
	fatal_insn ("Bad insn for %A/%B/%C/%D:", x);

      break;

    case 'M':	/* Print a memory reference for ld/st instructions */
      if (GET_CODE (x) != MEM)
	fatal_insn ("Bad insn to print_operand 'M' modifier:", x);

      print_operand_memory_reference (stream, XEXP (x, 0));
      break;

    case 'F':	/* Print an appropriate 'f' or 't' for a false comparision instruction.  */
    case 'T':	/* Print an appropriate 'f' or 't' for a true  comparision instruction.  */
      if ((GET_CODE (x) == NE || GET_CODE (x) == EQ)
	  && (GET_MODE (x) == CCmode || GET_MODE (x) == CC_REVmode)
	  && GET_CODE (XEXP (x, 0)) == REG && REGNO (XEXP (x, 0)) == F0_REGNUM
	  && GET_CODE (XEXP (x, 1)) == CONST_INT && INTVAL (XEXP (x, 1)) == 0)
	{
	  int true_false = (GET_MODE (x) == CC_REVmode);

	  if (letter == 'F')
	    true_false = !true_false;

	  if (GET_CODE (x) == EQ)
	    true_false = !true_false;

	  putc ((true_false) ? 't' : 'f', stream);
	}

      else
	fatal_insn ("Bad insn to print_operand 'F' or 'T' modifier:", x);
      break;

    case 'b':	/* emit offset single bit to change */
      if (GET_CODE (x) == CONST_INT && (log = exact_log2 (INTVAL (x))) >= 0)
	fprintf (stream, "%d", 15 - log);

      else if (GET_CODE (x) == CONST_INT && (log = exact_log2 (~ INTVAL (x))) >= 0)
	fprintf (stream, "%d", 15 - log);

      else
	fatal_insn ("Bad insn to print_operand 'b' modifier:", x);
      break;

    case 's':	/* print ldi.s value for composing ldi.l out of ldi.s and slli */
      if (GET_CODE (x) == CONST_INT
	  && (shift = ldi_shift_operand (x, HImode)) > 0)
	{
	  fprintf (stream, "%d", ((int) INTVAL (x)) >> shift);
	}

      else
	fatal_insn ("Bad insn to print_operand 'S' modifier:", x);
      break;

    case 'S':	/* print shift value for composing ldi.l out of ldi.s and slli */
      if (GET_CODE (x) == CONST_INT
	  && (shift = ldi_shift_operand (x, HImode)) > 0)
	{
	  fprintf (stream, "%d", shift);
	}

      else
	fatal_insn ("Bad insn to print_operand 'S' modifier:", x);
      break;

    case 'u':	/* print a 'u' if the operator is ZERO_EXTEND */
      if (GET_CODE (x) == ZERO_EXTEND)
	putc ('u', stream);
      break;

    case 0:
      if (code == REG)
	fputs (reg_names[ REGNO (x) ], stream);

      else if (code == CONST_INT)
	fprintf (stream, "%d", (int)INTVAL (x));

      else if (code == MEM)
	print_operand_address (stream, XEXP (x, 0));

      else if (CONSTANT_ADDRESS_P (x))
	print_operand_address (stream, x);

      else
	fatal_insn ("Bad insn in print_operand, 0 case", x);

      return;

    default:
      {
	char buf[80];

	sprintf (buf, "Invalid asm template character '%%%c'", letter);
	fatal_insn (buf, x);
      }
    }
}


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

void
init_cumulative_args (cum, fntype, libname, indirect, incoming)
     CUMULATIVE_ARGS *cum;
     tree fntype;
     rtx libname;
     int indirect;
     int incoming;
{
  cum->reg = 0;
  cum->stack = 4;

  if (TARGET_DEBUG_ARG)
    {
      fprintf (stderr, "\ninit_cumulative_args:");
      if (indirect)
	fputs (" indirect", stderr);

      if (incoming)
	fputs (" incoming", stderr);

      if (fntype)
	{
	  tree ret_type = TREE_TYPE (fntype);
	  fprintf (stderr, " return=%s,",
		   tree_code_name[ (int)TREE_CODE (ret_type) ]);
	}

      if (libname && GET_CODE (libname) == SYMBOL_REF)
	fprintf (stderr, " libname=%s", XSTR (libname, 0));

      putc ('\n', stderr);
    }
}


/* If defined, a C expression that gives the alignment boundary, in
   bits, of an argument with the specified mode and type.  If it is
   not defined, `PARM_BOUNDARY' is used for all arguments. */

int
function_arg_boundary (mode, type)
     enum machine_mode mode;
     tree type;
{
  int size = ((mode == BLKmode && type)
	      ? int_size_in_bytes (type)
	      : GET_MODE_SIZE (mode));

  return ((size >= 2 * UNITS_PER_WORD) ? 2*BITS_PER_WORD : BITS_PER_WORD);
}


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

struct rtx_def *
function_arg (cum, mode, type, named)
     CUMULATIVE_ARGS *cum;
     enum machine_mode mode;
     tree type;
     int named;
{
  int align = ((cum->reg & 1) != 0
	       && function_arg_boundary (mode, type) >= (2*BITS_PER_WORD));
  int size = ((mode == BLKmode && type)
	      ? int_size_in_bytes (type)
	      : GET_MODE_SIZE (mode));

  rtx ret;

  if ((cum->reg + align) * 2 + size > (ARG_LAST - ARG_FIRST + 1) * 2)
    ret = NULL_RTX;

  else
    ret = gen_rtx (REG, mode, ARG_FIRST + cum->reg + align);

  if (TARGET_DEBUG_ARG)
    fprintf (stderr,
	     "function_arg: words = %2d %2d, mode = %4s, named = %d, align = %d, size = %3d, arg = %s\n",
	     cum->reg, cum->stack, GET_MODE_NAME (mode), named, align, size,
	     (ret) ? reg_names[ REGNO (ret) ] : "memory");

  return ret;
}



/* If defined, a C expression that indicates when it is the called
   function's responsibility to make a copy of arguments passed by
   invisible reference.  Normally, the caller makes a copy and passes
   the address of the copy to the routine being called.  When
   FUNCTION_ARG_CALLEE_COPIES is defined and is nonzero, the caller
   does not make a copy.  Instead, it passes a pointer to the "live"
   value.  The called function must not modify this value.  If it can
   be determined that the value won't be modified, it need not make a
   copy; otherwise a copy must be made. */

int
function_arg_callee_copies (cum, mode, type, named)
     CUMULATIVE_ARGS *cum;
     enum machine_mode mode;
     tree type;
     int named;
{
  return TARGET_CALLEE_COPIES != 0;
}


/* A C statement (sans semicolon) to update the summarizer variable
   CUM to advance past an argument in the argument list.  The values
   MODE, TYPE and NAMED describe that argument.  Once this is done,
   the variable CUM is suitable for analyzing the *following*
   argument with `FUNCTION_ARG', etc.

   This macro need not do anything if the argument in question was
   passed on the stack.  The compiler knows how to track the amount
   of stack space used for arguments without any special help. */

void
function_arg_advance (cum, mode, type, named)
     CUMULATIVE_ARGS *cum;
     enum machine_mode mode;
     tree type;
     int named;
{
  int align = ((cum->reg & 1) != 0
	       && function_arg_boundary (mode, type) >= (2*BITS_PER_WORD));

  int size = ((mode == BLKmode && type)
	      ? int_size_in_bytes (type)
	      : GET_MODE_SIZE (mode));

  int words = (size + (UNITS_PER_WORD - 1)) / UNITS_PER_WORD;

  if (cum->reg + align + words <= (ARG_LAST - ARG_FIRST + 1))
    cum->reg += align + words;
  else
#if 0 /* ??? there should be no such alignment here, but there appears to be.
         This is probably a backend bug.  */
    cum->stack += words;
#else
    cum->stack += align + words;
#endif

  if (TARGET_DEBUG_ARG)
    fprintf (stderr,
	     "function_adv: words = %2d %2d, mode = %4s, named = %d, align = %d, size = %3d\n",
	     cum->reg, cum->stack, GET_MODE_NAME (mode), named, align, size);
}


/* Perform any needed actions needed for a function that is receiving a
   variable number of arguments. 

   CUM is as above.

   MODE and TYPE are the mode and type of the current parameter.

   PRETEND_SIZE is a variable that should be set to the amount of stack
   that must be pushed by the prolog to pretend that our caller pushed
   it.

   Normally, this macro will push all remaining incoming registers on the
   stack and set PRETEND_SIZE to the length of the registers pushed.  */

void
setup_incoming_varargs (cum, mode, type, pretend_size, no_rtl)
     CUMULATIVE_ARGS *cum;
     enum machine_mode mode;
     tree type;
     int *pretend_size;
     int no_rtl;

{
  if (TARGET_DEBUG_ARG)
    fprintf (stderr,
	     "setup_vararg: words = %2d %2d, mode = %4s, no_rtl= %d\n",
	     cum->reg, cum->stack, GET_MODE_NAME (mode), no_rtl);
}


/* If defined, is a C expression that produces the machine-specific
   code for a call to `__builtin_saveregs'.  This code will be moved
   to the very beginning of the function, before any parameter access
   are made.  The return value of this function should be an RTX that
   contains the value to use as the return of `__builtin_saveregs'.

   The argument ARGS is a `tree_list' containing the arguments that
   were passed to `__builtin_saveregs'.

   If this macro is not defined, the compiler will output an ordinary
   call to the library function `__builtin_saveregs'.
   
   On the D10V return the start address of the area on the stack
   used to hold arguments (including the 4 words pushed at the start
   of the function for the arguments passed in registers.  */

struct rtx_def *
expand_builtin_saveregs (args)
     tree args;
{
  return gen_rtx (PLUS, Pmode,
		  virtual_incoming_args_rtx,
		  GEN_INT (- UNITS_PER_WORD * (ARG_LAST + 1 - ARG_FIRST)));
}


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

#define XREGNO_OK_FOR_BASE_P(REGNO, STRICT_P)				\
  (GPR_P (REGNO) ||							\
   (!STRICT_P && ((REGNO) == ARG_POINTER_REGNUM				\
		  || (REGNO) >= FIRST_PSEUDO_REGISTER)))

int
d10v_legitimate_address_p (mode, x, strict_p)
     enum machine_mode mode;
     rtx x;
     int strict_p;
{
  rtx x0, x1;
  int ret = 0;

  switch (GET_CODE (x))
    {
    default:
      break;

    case PLUS:
      x0 = XEXP (x, 0);
      x1 = XEXP (x, 1);

      while (GET_CODE (x0) == SUBREG)
	x0 = SUBREG_REG (x0);

      while (GET_CODE (x1) == SUBREG)
	x1 = SUBREG_REG (x1);

      if (GET_CODE (x0) == REG
	  && XREGNO_OK_FOR_BASE_P (REGNO (x0), strict_p)
	  && CONSTANT_P (x1)
	  && LEGITIMATE_CONSTANT_P (x1))
	ret = 1;

      break;

    case SUBREG:
      do
	{
	  x = SUBREG_REG (x);
	}
      while (GET_CODE (x) == SUBREG);	/* fall through */

    case REG:
      ret = XREGNO_OK_FOR_BASE_P (REGNO (x), strict_p);
      break;

    case CONST_INT:
      ret = IN_RANGE_P (INTVAL (x), -32768, 32767);
      break;

    case CONST:
    case SYMBOL_REF:
    case LABEL_REF:
      ret = 1;
      break;

    case POST_INC:
      x0 = XEXP (x, 0);
      if ((mode == HImode || mode == SImode || mode == SFmode)
	  && GET_CODE (x0) == REG && XREGNO_OK_FOR_BASE_P (REGNO (x0), strict_p))
	ret = 1;
      break;

    case POST_DEC:
      x0 = XEXP (x, 0);
      if ((mode == HImode || mode == SImode || mode == SFmode)
	  && GET_CODE (x0) == REG && XREGNO_OK_FOR_BASE_P (REGNO (x0), strict_p)
	  && REGNO (x0) != STACK_POINTER_REGNUM)
	ret = 1;
      break;

    case PRE_DEC:
      x0 = XEXP (x, 0);
      if ((mode == HImode || mode == SImode || mode == SFmode)
	  && GET_CODE (x0) == REG && REGNO (x0) == STACK_POINTER_REGNUM)
	ret = 1;
      break;
    }

  if (TARGET_DEBUG_ADDR)
    {
      fprintf (stderr, "\n========== GO_IF_LEGITIMATE_ADDRESS, mode = %s, result = %d, addresses are %sstrict\n",
	       GET_MODE_NAME (mode), ret, (strict_p) ? "" : "not ");
      debug_rtx (x);
    }

  return ret;
}



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

struct rtx_def *
d10v_legitimize_address (x, oldx, mode, strict_p)
     rtx x;
     rtx oldx;
     enum machine_mode mode;
     int strict_p;
{
  rtx ret = NULL_RTX;

  if (TARGET_DEBUG_ADDR)
    {
      if (ret)
	{
	  fprintf (stderr, "\n========== LEGITIMIZE_ADDRESS, transformed:\n");
	  debug_rtx (x);
	  fprintf (stderr, "\ninto:\n");
	  debug_rtx (ret);
	}
      else
	{
	  fprintf (stderr, "\n========== LEGITIMIZE_ADDRESS, did nothing with:\n");
	  debug_rtx (x);
	}
    }

  return ret;
}


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

int
d10v_mode_dependent_address_p (addr)
     rtx addr;
{
  switch (GET_CODE (addr))
    {
    default:
      break;

    case POST_INC:
    case POST_DEC:
    case PRE_DEC:
      return 1;
    }
  return 0;
}


/* Return an RTX that refers to the nth subword of a multiword
   expression.  This is similar to operand_subword, but that
   has a bias that SImode is a word.  */

struct rtx_def *
d10v_subword (op, word_num, target_mode, source_mode)
     rtx op;
     int word_num;
     enum machine_mode target_mode;
     enum machine_mode source_mode;
{
  enum rtx_code code = GET_CODE (op);
  enum machine_mode mode = GET_MODE (op);
  int split_values[4];

  if (!IN_RANGE_P (word_num, 0, 3))
    error ("div10_subword_mode: Internal error -- word_num (%d) should be >= 0 && <= 3", word_num);

  if (target_mode == SImode && word_num != 0 && word_num != 2)
    error ("div10_subword_mode: Internal error -- word_num (%d) should be 0 or 2 for SImode", word_num);

  if (target_mode != HImode && target_mode != SImode)
    error ("d10v_subword_mode: Internal error -- target_mode (%s) must be HImode or SImode", GET_MODE_NAME (target_mode));

  if (code == REG)
    {
      if ((UNITS_PER_WORD * word_num) >= GET_MODE_SIZE (mode))
	error ("div10_subword: Internal error -- word_num (%d) is too large for mode %s",
	       word_num, GET_MODE_NAME (mode));

      if (!reload_completed)
	return gen_rtx (SUBREG, target_mode, op, word_num);

      return gen_rtx (REG, target_mode, REGNO (op) + word_num);
    }

  else if (code == SUBREG)
    {
      if ((UNITS_PER_WORD * word_num) >= GET_MODE_SIZE (mode))
	error ("div10_subword: Internal error -- word_num (%d) is too large for mode %s",
	       word_num, GET_MODE_NAME (mode));

      if (!reload_completed)
	return gen_rtx (SUBREG, target_mode, op, word_num);

      do
	{
	  word_num += SUBREG_WORD (op);
	  op = SUBREG_REG (op);
	}
      while (GET_CODE (op) == SUBREG);

      return gen_rtx (REG, target_mode, REGNO (op) + word_num);
    }

  else if ((code == CONST_INT || code == CONST_DOUBLE) && GET_MODE_SIZE (source_mode) == 2)
    {
      if (word_num > 1)
	error ("div10_subword: Internal error -- word_num (%d) is too large for 32-bit constants", word_num);

      d10v_split_constant32 (op, split_values);
      return GEN_INT (SIGN_EXTEND_SHORT (split_values[word_num]));
    }

  else if ((code == CONST_INT || code == CONST_DOUBLE) && GET_MODE_SIZE (source_mode) == 4)
    {
      d10v_split_constant32 (op, split_values);
      return GEN_INT (SIGN_EXTEND_SHORT (split_values[word_num]));
    }

  else if ((code == CONST_INT || code == CONST_DOUBLE) && GET_MODE_SIZE (source_mode) == 8)
    {
      HOST_WIDE_INT value;

      d10v_split_constant64 (op, split_values);
      value = SIGN_EXTEND_SHORT (split_values[word_num]);
      if (GET_MODE_SIZE (target_mode) == 4)
	value = (value << 16) & (split_values[word_num+1] & 0xffff);

      return GEN_INT (value);
    }

  else if (code == MEM && word_num == 0)
    return op;

  else if (code == MEM)
    return change_address (op, target_mode, plus_constant (XEXP (op, 0), word_num * 16));

  else
    fatal_insn ("Cannot get subword of:", op);

  return NULL_RTX;
}


/* Calculate the stack information for the current function.

   D10V stack frames look like:

	high		|  ....				|
			+-------------------------------+
			| Argument word #8		|
			+-------------------------------+
			| Argument word #7		|
			+-------------------------------+
			| Argument word #6		|
			+-------------------------------+
			| Argument word #5		|
		Prev sp	+-------------------------------+
			|				|
			| Save for arguments 1..4 if	|
			| the func. uses stdarg/varargs	|
			|				|
			+-------------------------------+
			|				|
			| Save area for preserved regs	|
			|				|
			+-------------------------------+
			|				|
			| Local variables		|
			|				|
			+-------------------------------+
			|				|
			| alloca space if used		|
			|				|
			+-------------------------------+
			|				|
			| Space for outgoing arguments	|
			|				|
			+-------------------------------+
			| Return address if -mnew-stack	|
			+-------------------------------+
			| Previous sp if -mnew-stack	|
	low	SP---->	+-------------------------------+
*/

d10v_stack_t *
d10v_stack_info ()
{
  static d10v_stack_t info, zero_info;
  d10v_stack_t *info_ptr = &info;
  tree fndecl		 = current_function_decl;
  tree fntype		 = TREE_TYPE (fndecl);
  int varargs_p		 = 0;
  tree cur_arg;
  tree next_arg;
  int saved_gprs;
  int saved_accs;
  int i;

  /* If we've already calculated the values and reload is complete, just return now */
  if (d10v_stack_cache)
    return d10v_stack_cache;

  /* Zero all fields portably */
  info = zero_info;

  if (profile_flag)
    regs_ever_live[RETURN_ADDRESS_REGNUM] = 1;

  /* Determine if this is a stdarg function */
  if (TYPE_ARG_TYPES (fntype) != 0
      && (TREE_VALUE (tree_last (TYPE_ARG_TYPES (fntype))) != void_type_node))
    varargs_p = 1;
  else
    {
      /* Find the last argument, and see if it is __builtin_va_alist.  */
      for (cur_arg = DECL_ARGUMENTS (fndecl); cur_arg != (tree)0; cur_arg = next_arg)
	{
	  next_arg = TREE_CHAIN (cur_arg);
	  if (next_arg == (tree)0)
	    {
	      if (DECL_NAME (cur_arg)
		  && !strcmp (IDENTIFIER_POINTER (DECL_NAME (cur_arg)), "__builtin_va_alist"))
		varargs_p = 1;

	      break;
	    }
	}
    }

  /* Calculate which registers need to be saved & save area size */
  saved_accs = 0;
  for (i = ACCUM_FIRST; i <= ACCUM_LAST; i++)
    {
      if (regs_ever_live[i] && !call_used_regs[i])
	{
	  info_ptr->save_p[i] = 1;
	  saved_accs++;
	}
    }

  /* If any accumulators are to be saved, we need a one two word register
     pair to save the main portion of the accs and another register to
     save the guard digits.  */
  if (saved_accs)
    {
      regs_ever_live[SAVE_GUARD_REGNUM] = 1;
      regs_ever_live[SAVE_ACC_REGNUM] = 1;
      regs_ever_live[SAVE_ACC_REGNUM+1] = 1;
    }

  saved_gprs = 0;
  for (i = GPR_FIRST; i <= GPR_LAST; i++)
    {
      if (regs_ever_live[i] && (!call_used_regs[i] || i == RETURN_ADDRESS_REGNUM))
	{
	  info_ptr->save_p[i] = 1;
	  saved_gprs++;
	}
    }

  /* Determine various sizes */
  info_ptr->varargs_p	 = varargs_p;
  info_ptr->varargs_size = D10V_ALIGN (((varargs_p)
					? (ARG_LAST + 1 - ARG_FIRST) * UNITS_PER_WORD
					: 0), UNITS_PER_WORD);

  info_ptr->gpr_size	 = D10V_ALIGN (UNITS_PER_WORD * saved_gprs, UNITS_PER_WORD);
  info_ptr->accum_size	 = D10V_ALIGN (3 * UNITS_PER_WORD * saved_accs, UNITS_PER_WORD);
  info_ptr->vars_size    = D10V_ALIGN (get_frame_size (), UNITS_PER_WORD);
  info_ptr->parm_size    = D10V_ALIGN (current_function_outgoing_args_size, UNITS_PER_WORD);
  info_ptr->total_size	 = D10V_ALIGN (info_ptr->gpr_size
				       + info_ptr->accum_size
				       + info_ptr->vars_size
				       + info_ptr->parm_size
				       + info_ptr->varargs_size, (STACK_BOUNDARY / BITS_PER_UNIT));

  if (reload_completed)
    d10v_stack_cache = info_ptr;

  return info_ptr;
}


/* Internal function to print all of the information about the stack */

void
debug_stack_info (info)
     d10v_stack_t *info;
{
  int i;

  if (!info)
    info = d10v_stack_info ();

  fprintf (stderr, "\nStack information for function %s:\n",
	   ((current_function_decl && DECL_NAME (current_function_decl))
	    ? IDENTIFIER_POINTER (DECL_NAME (current_function_decl))
	    : "<unknown>"));

  fprintf (stderr, "\tvarargs_p       = %d\n", info->varargs_p);
  fprintf (stderr, "\tvarargs_size    = %d\n", info->varargs_size);
  fprintf (stderr, "\tvars_size       = %d\n", info->vars_size);
  fprintf (stderr, "\tparm_size       = %d\n", info->parm_size);
  fprintf (stderr, "\tgpr_size        = %d\n", info->gpr_size);
  fprintf (stderr, "\taccum_size      = %d\n", info->accum_size);
  fprintf (stderr, "\ttotal_size      = %d\n", info->total_size);
  fprintf (stderr, "\tsaved registers =");

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (info->save_p[i])
      fprintf (stderr, " %s", reg_names[i]);

  putc ('\n', stderr);
  fflush (stderr);
}


/* Return non-zero if this function is known to have a null or 1 instruction epilogue.  */

int
direct_return ()
{
  if (reload_completed)
    {
      d10v_stack_t *info = d10v_stack_info ();

      /* If no epilogue code is needed, can use just a simple jump */
      if (info->total_size == 0)
	return 1;

      /* Also allow the common case where r13 is reloaded before the return,
	 since we take the same space as a forward branch for the two.  */
      if (info->total_size == UNITS_PER_WORD && info->save_p[RETURN_ADDRESS_REGNUM])
	return 1;

      /* Using similar logic, if just a small amount of local stack was allocated
	 and no registers saved, skip forward branch */
      if (info->total_size == info->vars_size
	  && IN_RANGE_P (info->total_size, 1, 16))
	return 1;
    }

  return 0;
}


/* Internal function to print a memory reference.  */

static void
print_memory_ref (stream, instruction, reg, offset, index_reg)
     FILE *stream;
     char *instruction;
     int reg;
     int offset;
     int index_reg;
{
  if (offset == 0)
    fprintf (stream, "\t%s %s,@%s\n", instruction, reg_names[reg],
	     reg_names[index_reg]);
  else
    fprintf (stream, "\t%s %s,@(%d,%s)\n", instruction, reg_names[reg], offset,
	     reg_names[index_reg]);
}


/* A C compound statement that outputs the assembler code for entry
   to a function.  The prologue is responsible for setting up the
   stack frame, initializing the frame pointer register, saving
   registers that must be saved, and allocating SIZE additional bytes
   of storage for the local variables.  SIZE is an integer.  FILE is
   a stdio stream to which the assembler code should be output. */

void
function_prologue (stream, size)
     FILE *stream;
     int size;
{
  int i, offset;
  d10v_stack_t *info	= d10v_stack_info ();
  char *sp_name		= reg_names[STACK_POINTER_REGNUM];
  int total_size	= info->total_size;
  int gpr_size		= info->gpr_size;
  int accum_size	= info->accum_size;

  if (TARGET_DEBUG_STACK)
    debug_stack_info (info);

  /* Save any variable arguments */
  if (info->varargs_p)
    {
      offset = 0;
      for (i = ARG_LAST-1; i >= ARG_FIRST; i -= 2)
	{
	  fprintf (stream, "\tst2w %s,@-%s\n", reg_names[i], sp_name);
	  offset -= 2 * UNITS_PER_WORD;
	  total_size -= 2 * UNITS_PER_WORD;
	}
    }

  /* Save the GPRs by pushing */
  for (i = GPR_FIRST; i <= GPR_LAST; i++)
    {
      if (info->save_p[i])
	{
	  if (GPR_EVEN_P (i) && info->save_p[i+1])
	    {
	      total_size -= 2 * UNITS_PER_WORD;
	      fprintf (stream, "\tst2w %s,@-%s\n", reg_names[i++], sp_name);
	    }
	  else
	    {
	      total_size -= UNITS_PER_WORD;
	      fprintf (stream, "\tst %s,@-%s\n", reg_names[i], sp_name);
	    }
	}
    }

  /* Now save the ACs (both guard digits and 32-bit value) by moving into r12/r13 and pushing */
  for (i = ACCUM_FIRST; i <= ACCUM_LAST; i++)
    {
      if (info->save_p[i])
	{
	  total_size -= 3 * UNITS_PER_WORD;
	  fprintf (stream, "\tmvfacg %s,%s\n", reg_names[SAVE_GUARD_REGNUM],
		   reg_names[i]);

	  fprintf (stream, "\tmv2wfac %s,%s\n", reg_names[SAVE_ACC_REGNUM],
		   reg_names[i]);

	  fprintf (stream, "\tst %s,@-%s\n", reg_names[SAVE_GUARD_REGNUM],
		   sp_name);

	  fprintf (stream, "\tst2w %s,@-%s\n", reg_names[SAVE_ACC_REGNUM],
		   sp_name);
	}
    }

  if (total_size > 0)
    {
      if (total_size <= 16)
	fprintf (stream, "\tsubi %s,%d\n", sp_name, total_size);

      else
	fprintf (stream, "\tadd3 %s,%s,%d\n", sp_name, sp_name, -total_size);
    }

  /* Update frame pointer if needed */
  if (frame_pointer_needed)
    fprintf (stream, "\tmv %s,%s\n", reg_names[FRAME_POINTER_REGNUM], sp_name);
}


/* A C compound statement that outputs the assembler code for exit
   from a function.  The epilogue is responsible for restoring the
   saved registers and stack pointer to their values when the
   function was called, and returning control to the caller.  This
   macro takes the same arguments as the macro `FUNCTION_PROLOGUE',
   and the registers to restore are determined from `regs_ever_live'
   and `CALL_USED_REGISTERS' in the same way. */

void
function_epilogue (stream, size)
     FILE *stream;
     int size;
{
  int i;
  rtx insn	= get_last_insn ();
  char *sp_name = reg_names[STACK_POINTER_REGNUM];

  /* If the last insn was a BARRIER, we don't have to write anything except
     the trace table.  */
  if (GET_CODE (insn) == NOTE)
    insn = prev_nonnote_insn (insn);

  if (insn == 0 ||  GET_CODE (insn) != BARRIER)
    {
      d10v_stack_t *info  = d10v_stack_info ();
      int varargs_size    = info->varargs_size;
      int stack_size	  = info->total_size - varargs_size;
      int gpr_size	  = info->gpr_size;
      int accum_size	  = info->accum_size;

      /* If we have a varargs area, but no saved registers (ie, this is a stub)
	 fold the varargs area back into stack size */
      if (varargs_size && !gpr_size)
	{
	  stack_size += varargs_size;
	  varargs_size = 0;
	}

      /* Restore stack pointer if needed */
      if (frame_pointer_needed)
	{
	  char *fp_name = reg_names[FRAME_POINTER_REGNUM];

	  /* We can combine restoring the stack pointer with the adjustment directly.  */
	  if (stack_size > 0)
	    {
	      fprintf (stream, "\tadd3 %s,%s,%d\n", sp_name, fp_name, stack_size - gpr_size - accum_size);
	      stack_size = gpr_size + accum_size;
	    }
	  else
	    fprintf (stream, "\tmv %s,%s\n", sp_name, fp_name);
	}

      /* Restore stack pointer, other than the space needed to load registers w/postdecrement */
      stack_size -= gpr_size + accum_size;
      if (stack_size > 0)
	{
	  if (stack_size <= 16)
	    fprintf (stream, "\taddi %s,%d\n", sp_name, stack_size);
	  else
	    fprintf (stream, "\tadd3 %s,%s,%d\n", sp_name, sp_name, stack_size);
	}

      /* Restore any saved accumulators.  We must restore both the accumulator and the guard digits */
      for (i = ACCUM_FIRST; i <= ACCUM_LAST; i++)
	{
	  if (info->save_p[i])
	    {
	      fprintf (stream, "\tld2w %s,@%s+\n", reg_names[SAVE_ACC_REGNUM],
		       sp_name);

	      fprintf (stream, "\tld %s,@%s+\n", reg_names[SAVE_GUARD_REGNUM],
		       sp_name);

	      fprintf (stream, "\tmv2wtac %s,%s\n", reg_names[SAVE_ACC_REGNUM],
		       reg_names[i]);

	      fprintf (stream, "\tmvtacg %s,%s\n", reg_names[SAVE_GUARD_REGNUM],
		       reg_names[i]);
	    }
	}

      /* Restore the gprs by poping the registers.  */
      for (i = GPR_LAST; i >= GPR_FIRST; i--)
	{
	  if (info->save_p[i])
	    {
	      if (GPR_ODD_P (i) && info->save_p[i-1])
		fprintf (stream, "\tld2w %s,@%s+\n", reg_names[--i], sp_name);
	      else
		fprintf (stream, "\tld %s,@%s+\n", reg_names[i], sp_name);
	    }
	}

      /* Finally remove varargs area */
      if (varargs_size)
	fprintf (stream, "\taddi %s,%d\n", sp_name, varargs_size);

      /* Return to the user */
      fprintf (stream, "\tjmp %s\n", reg_names[RETURN_ADDRESS_REGNUM]);
    }

  /* Clear out stack cache */
  d10v_stack_cache = (d10v_stack_t *)0;
}
/* END CYGNUS LOCAL -- meissner/d10v abi change */
