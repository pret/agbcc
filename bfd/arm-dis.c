/* Instruction printing code for the ARM
   Copyright (C) 1994-2018 Free Software Foundation, Inc.
   Contributed by Richard Earnshaw (rwe@pegasus.esprit.ec.org)
   Modification by James G. Smith (jsmith@cygnus.co.uk)

   This file is part of libopcodes.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"

#include "disassemble.h"
#include "opcode/arm.h"
#include "opintl.h"
#include "safe-ctype.h"
#include "libiberty.h"

/* FIXME: This shouldn't be done here.  */
#include "elf-bfd.h"
#include "elf/internal.h"
#include "elf/arm.h"

/* FIXME: Belongs in global header.  */
#ifndef strneq
#define strneq(a,b,n)	(strncmp ((a), (b), (n)) == 0)
#endif
#define opcodes_error_handler _bfd_error_handler
/* Cached mapping symbol state.  */
enum map_type
{
  MAP_ARM,
  MAP_THUMB,
  MAP_DATA
};

struct arm_private_data
{
  /* The features to use when disassembling optional instructions.  */
  arm_feature_set features;

  /* Whether any mapping symbols are present in the provided symbol
     table.  -1 if we do not know yet, otherwise 0 or 1.  */
  int has_mapping_symbols;

  /* Track the last type (although this doesn't seem to be useful) */
  enum map_type last_type;

  /* Tracking symbol table information */
  int last_mapping_sym;
  bfd_vma last_mapping_addr;
};

struct opcode32
{
  arm_feature_set arch;		/* Architecture defining this insn.  */
  uint32_t value;		/* If arch is 0 then value is a sentinel.  */
  uint32_t mask;		/* Recognise insn if (op & mask) == value.  */
  const char *  assembler;	/* How to disassemble this insn.  */
};

struct opcode16
{
  arm_feature_set arch;		/* Architecture defining this insn.  */
  unsigned short value, mask;	/* Recognise insn if (op & mask) == value.  */
  const char *assembler;	/* How to disassemble this insn.  */
};

/* print_insn_coprocessor recognizes the following format control codes:

   %%			%

   %c			print condition code (always bits 28-31 in ARM mode)
   %q			print shifter argument
   %u			print condition code (unconditional in ARM mode,
                          UNPREDICTABLE if not AL in Thumb)
   %A			print address for ldc/stc/ldf/stf instruction
   %B			print vstm/vldm register list
   %I                   print cirrus signed shift immediate: bits 0..3|4..6
   %F			print the COUNT field of a LFM/SFM instruction.
   %P			print floating point precision in arithmetic insn
   %Q			print floating point precision in ldf/stf insn
   %R			print floating point rounding mode

   %<bitfield>c		print as a condition code (for vsel)
   %<bitfield>r		print as an ARM register
   %<bitfield>R		as %<>r but r15 is UNPREDICTABLE
   %<bitfield>ru        as %<>r but each u register must be unique.
   %<bitfield>d		print the bitfield in decimal
   %<bitfield>k		print immediate for VFPv3 conversion instruction
   %<bitfield>x		print the bitfield in hex
   %<bitfield>X		print the bitfield as 1 hex digit without leading "0x"
   %<bitfield>f		print a floating point constant if >7 else a
			floating point register
   %<bitfield>w         print as an iWMMXt width field - [bhwd]ss/us
   %<bitfield>g         print as an iWMMXt 64-bit register
   %<bitfield>G         print as an iWMMXt general purpose or control register
   %<bitfield>D		print as a NEON D register
   %<bitfield>Q		print as a NEON Q register
   %<bitfield>V		print as a NEON D or Q register
   %<bitfield>E		print a quarter-float immediate value

   %y<code>		print a single precision VFP reg.
			  Codes: 0=>Sm, 1=>Sd, 2=>Sn, 3=>multi-list, 4=>Sm pair
   %z<code>		print a double precision VFP reg
			  Codes: 0=>Dm, 1=>Dd, 2=>Dn, 3=>multi-list

   %<bitfield>'c	print specified char iff bitfield is all ones
   %<bitfield>`c	print specified char iff bitfield is all zeroes
   %<bitfield>?ab...    select from array of values in big endian order

   %L			print as an iWMMXt N/M width field.
   %Z			print the Immediate of a WSHUFH instruction.
   %l			like 'A' except use byte offsets for 'B' & 'H'
			versions.
   %i			print 5-bit immediate in bits 8,3..0
			(print "32" when 0)
   %r			print register offset address for wldt/wstr instruction.  */

enum opcode_sentinel_enum
{
  SENTINEL_IWMMXT_START = 1,
  SENTINEL_IWMMXT_END,
  SENTINEL_GENERIC_START
} opcode_sentinels;

#define UNDEFINED_INSTRUCTION      "\t\t; <UNDEFINED> instruction: %0-31x"
#define UNPREDICTABLE_INSTRUCTION  "\t; <UNPREDICTABLE>"

/* Common coprocessor opcodes shared between Arm and Thumb-2.  */

static const struct opcode32 coprocessor_opcodes[] =
{
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0e000000, 0x0f000010,
    "cdp%c\t%8-11d, %20-23d, cr%12-15d, cr%16-19d, cr%0-3d, {%5-7d}"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0e10f010, 0x0f10f010,
    "mrc%c\t%8-11d, %21-23d, APSR_nzcv, cr%16-19d, cr%0-3d, {%5-7d}"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0e100010, 0x0f100010,
    "mrc%c\t%8-11d, %21-23d, %12-15r, cr%16-19d, cr%0-3d, {%5-7d}"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0e000010, 0x0f100010,
    "mcr%c\t%8-11d, %21-23d, %12-15R, cr%16-19d, cr%0-3d, {%5-7d}"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0c000000, 0x0e100000, "stc%22'l%c\t%8-11d, cr%12-15d, %A"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x0c100000, 0x0e100000, "ldc%22'l%c\t%8-11d, cr%12-15d, %A"},
  {ARM_FEATURE_CORE_LOW (0), 0, 0, 0}
};

/* Neon opcode table:  This does not encode the top byte -- that is
   checked by the print_insn_neon routine, as it depends on whether we are
   doing thumb32 or arm32 disassembly.  */

/* print_insn_neon recognizes the following format control codes:

   %%			%

   %c			print condition code
   %u			print condition code (unconditional in ARM mode,
                          UNPREDICTABLE if not AL in Thumb)
   %A			print v{st,ld}[1234] operands
   %B			print v{st,ld}[1234] any one operands
   %C			print v{st,ld}[1234] single->all operands
   %D			print scalar
   %E			print vmov, vmvn, vorr, vbic encoded constant
   %F			print vtbl,vtbx register list

   %<bitfield>r		print as an ARM register
   %<bitfield>d		print the bitfield in decimal
   %<bitfield>e         print the 2^N - bitfield in decimal
   %<bitfield>D		print as a NEON D register
   %<bitfield>Q		print as a NEON Q register
   %<bitfield>R		print as a NEON D or Q register
   %<bitfield>Sn	print byte scaled width limited by n
   %<bitfield>Tn	print short scaled width limited by n
   %<bitfield>Un	print long scaled width limited by n

   %<bitfield>'c	print specified char iff bitfield is all ones
   %<bitfield>`c	print specified char iff bitfield is all zeroes
   %<bitfield>?ab...    select from array of values in big endian order.  */

/* Opcode tables: ARM, 16-bit Thumb, 32-bit Thumb.  All three are partially
   ordered: they must be searched linearly from the top to obtain a correct
   match.  */

/* print_insn_arm recognizes the following format control codes:

   %%			%

   %a			print address for ldr/str instruction
   %s                   print address for ldr/str halfword/signextend instruction
   %S                   like %s but allow UNPREDICTABLE addressing
   %b			print branch destination
   %c			print condition code (always bits 28-31)
   %m			print register mask for ldm/stm instruction
   %o			print operand2 (immediate or register + shift)
   %p			print 'p' iff bits 12-15 are 15
   %t			print 't' iff bit 21 set and bit 24 clear
   %B			print arm BLX(1) destination
   %C			print the PSR sub type.
   %U			print barrier type.
   %P			print address for pli instruction.

   %<bitfield>r		print as an ARM register
   %<bitfield>T		print as an ARM register + 1
   %<bitfield>R		as %r but r15 is UNPREDICTABLE
   %<bitfield>{r|R}u    as %{r|R} but if matches the other %u field then is UNPREDICTABLE
   %<bitfield>{r|R}U    as %{r|R} but if matches the other %U field then is UNPREDICTABLE
   %<bitfield>d		print the bitfield in decimal
   %<bitfield>W         print the bitfield plus one in decimal
   %<bitfield>x		print the bitfield in hex
   %<bitfield>X		print the bitfield as 1 hex digit without leading "0x"

   %<bitfield>'c	print specified char iff bitfield is all ones
   %<bitfield>`c	print specified char iff bitfield is all zeroes
   %<bitfield>?ab...    select from array of values in big endian order

   %e                   print arm SMI operand (bits 0..7,8..19).
   %E			print the LSB and WIDTH fields of a BFI or BFC instruction.
   %V                   print the 16-bit immediate field of a MOVT or MOVW instruction.
   %R			print the SPSR/CPSR or banked register of an MRS.  */

static const struct opcode32 arm_opcodes[] =
{
  /* ARM instructions.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0xe1a00000, 0xffffffff, "nop\t\t\t; (mov r0, r0)"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0xe7f000f0, 0xfff000f0, "udf\t#%e"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x012FFF10, 0x0ffffff0, "bx%c\t%0-3r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x00000090, 0x0fe000f0, "mul%20's%c\t%16-19R, %0-3R, %8-11R"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2),
    0x00200090, 0x0fe000f0, "mla%20's%c\t%16-19R, %0-3R, %8-11R, %12-15R"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V2S),
    0x01000090, 0x0fb00ff0, "swp%22'b%c\t%12-15RU, %0-3Ru, [%16-19RuU]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3M),
    0x00800090, 0x0fa000f0,
    "%22?sumull%20's%c\t%12-15Ru, %16-19Ru, %0-3R, %8-11R"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3M),
    0x00a00090, 0x0fa000f0,
    "%22?sumlal%20's%c\t%12-15Ru, %16-19Ru, %0-3R, %8-11R"},

  /* Speculation Barriers.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3), 0xe320f014, 0xffffffff, "csdb"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3), 0xf57ff040, 0xffffffff, "ssbb"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3), 0xf57ff044, 0xffffffff, "pssbb"},

  /* ARM Instructions.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x052d0004, 0x0fff0fff, "push%c\t{%12-15r}\t\t; (str%c %12-15r, %a)"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04400000, 0x0e500000, "strb%t%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04000000, 0x0e500000, "str%t%c\t%12-15r, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x06400000, 0x0e500ff0, "strb%t%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x06000000, 0x0e500ff0, "str%t%c\t%12-15r, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04400000, 0x0c500010, "strb%t%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04000000, 0x0c500010, "str%t%c\t%12-15r, %a"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04400000, 0x0e500000, "strb%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x06400000, 0x0e500010, "strb%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x004000b0, 0x0e5000f0, "strh%c\t%12-15R, %s"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x000000b0, 0x0e500ff0, "strh%c\t%12-15R, %s"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00500090, 0x0e5000f0, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00500090, 0x0e500090, "ldr%6's%5?hb%c\t%12-15R, %s"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00100090, 0x0e500ff0, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00100090, 0x0e500f90, "ldr%6's%5?hb%c\t%12-15R, %s"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02000000, 0x0fe00000, "and%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00000000, 0x0fe00010, "and%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00000010, 0x0fe00090, "and%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02200000, 0x0fe00000, "eor%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00200000, 0x0fe00010, "eor%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00200010, 0x0fe00090, "eor%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02400000, 0x0fe00000, "sub%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00400000, 0x0fe00010, "sub%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00400010, 0x0fe00090, "sub%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02600000, 0x0fe00000, "rsb%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00600000, 0x0fe00010, "rsb%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00600010, 0x0fe00090, "rsb%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02800000, 0x0fe00000, "add%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00800000, 0x0fe00010, "add%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00800010, 0x0fe00090, "add%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02a00000, 0x0fe00000, "adc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00a00000, 0x0fe00010, "adc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00a00010, 0x0fe00090, "adc%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02c00000, 0x0fe00000, "sbc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00c00000, 0x0fe00010, "sbc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00c00010, 0x0fe00090, "sbc%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x02e00000, 0x0fe00000, "rsc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00e00000, 0x0fe00010, "rsc%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00e00010, 0x0fe00090, "rsc%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3),
    0x0120f000, 0x0db0f000, "msr%c\t%C, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V3),
    0x01000000, 0x0fb00cff, "mrs%c\t%12-15R, %R"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03000000, 0x0fe00000, "tst%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01000000, 0x0fe00010, "tst%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01000010, 0x0fe00090, "tst%p%c\t%16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03300000, 0x0ff00000, "teq%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01300000, 0x0ff00010, "teq%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01300010, 0x0ff00010, "teq%p%c\t%16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03400000, 0x0fe00000, "cmp%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01400000, 0x0fe00010, "cmp%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01400010, 0x0fe00090, "cmp%p%c\t%16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03600000, 0x0fe00000, "cmn%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01600000, 0x0fe00010, "cmn%p%c\t%16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01600010, 0x0fe00090, "cmn%p%c\t%16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03800000, 0x0fe00000, "orr%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01800000, 0x0fe00010, "orr%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01800010, 0x0fe00090, "orr%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03a00000, 0x0fef0000, "mov%20's%c\t%12-15r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00000, 0x0def0ff0, "mov%20's%c\t%12-15r, %0-3r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00000, 0x0def0060, "lsl%20's%c\t%12-15R, %q"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00020, 0x0def0060, "lsr%20's%c\t%12-15R, %q"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00040, 0x0def0060, "asr%20's%c\t%12-15R, %q"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00060, 0x0def0ff0, "rrx%20's%c\t%12-15r, %0-3r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01a00060, 0x0def0060, "ror%20's%c\t%12-15R, %q"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03c00000, 0x0fe00000, "bic%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01c00000, 0x0fe00010, "bic%20's%c\t%12-15r, %16-19r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01c00010, 0x0fe00090, "bic%20's%c\t%12-15R, %16-19R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x03e00000, 0x0fe00000, "mvn%20's%c\t%12-15r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01e00000, 0x0fe00010, "mvn%20's%c\t%12-15r, %o"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x01e00010, 0x0fe00090, "mvn%20's%c\t%12-15R, %o"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x06000010, 0x0e000010, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x049d0004, 0x0fff0fff, "pop%c\t{%12-15r}\t\t; (ldr%c %12-15r, %a)"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04500000, 0x0c500000, "ldrb%t%c\t%12-15R, %a"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04300000, 0x0d700000, "ldrt%c\t%12-15R, %a"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x04100000, 0x0c500000, "ldr%c\t%12-15r, %a"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0001, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0002, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0004, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0008, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0010, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0020, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0040, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0080, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0100, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0200, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0400, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0800, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d1000, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d2000, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d4000, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d8000, 0x0fffffff, "stmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x092d0000, 0x0fff0000, "push%c\t%m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08800000, 0x0ff00000, "stm%c\t%16-19R%21'!, %m%22'^"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08000000, 0x0e100000, "stm%23?id%24?ba%c\t%16-19R%21'!, %m%22'^"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0001, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0002, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0004, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0008, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0010, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0020, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0040, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0080, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0100, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0200, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0400, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0800, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd1000, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd2000, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd4000, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd8000, 0x0fffffff, "ldmfd%c\t%16-19R!, %m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08bd0000, 0x0fff0000, "pop%c\t%m"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08900000, 0x0f900000, "ldm%c\t%16-19R%21'!, %m%22'^"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x08100000, 0x0e100000, "ldm%23?id%24?ba%c\t%16-19R%21'!, %m%22'^"},

  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x0a000000, 0x0e000000, "b%24'l%c\t%b"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x0f000000, 0x0f000000, "svc%c\t%0-23x"},

  /* The rest.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
    0x00000000, 0x00000000, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (0),
    0x00000000, 0x00000000, 0}
};

/* print_insn_thumb16 recognizes the following format control codes:

   %S                   print Thumb register (bits 3..5 as high number if bit 6 set)
   %D                   print Thumb register (bits 0..2 as high number if bit 7 set)
   %<bitfield>I         print bitfield as a signed decimal
   				(top bit of range being the sign bit)
   %N                   print Thumb register mask (with LR)
   %O                   print Thumb register mask (with PC)
   %M                   print Thumb register mask
   %b			print CZB's 6-bit unsigned branch destination
   %s			print Thumb right-shift immediate (6..10; 0 == 32).
   %c			print the condition code
   %C			print the condition code, or "s" if not conditional
   %x			print warning if conditional an not at end of IT block"
   %X			print "\t; unpredictable <IT:code>" if conditional
   %I			print IT instruction suffix and operands
   %W			print Thumb Writeback indicator for LDMIA
   %<bitfield>r		print bitfield as an ARM register
   %<bitfield>d		print bitfield as a decimal
   %<bitfield>H         print (bitfield * 2) as a decimal
   %<bitfield>W         print (bitfield * 4) as a decimal
   %<bitfield>a         print (bitfield * 4) as a pc-rel offset + decoded symbol
   %<bitfield>B         print Thumb branch destination (signed displacement)
   %<bitfield>c         print bitfield as a condition code
   %<bitnum>'c		print specified char iff bit is one
   %<bitnum>?ab		print a if bit is one else print b.  */

static const struct opcode16 thumb_opcodes[] =
{
  /* Thumb instructions.  */

  /* ARM V4T ISA (Thumb v1).  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x46C0, 0xFFFF, "nop%c\t\t\t; (mov r8, r8)"},
  /* Format 4.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4000, 0xFFC0, "and%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4040, 0xFFC0, "eor%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4080, 0xFFC0, "lsl%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x40C0, 0xFFC0, "lsr%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4100, 0xFFC0, "asr%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4140, 0xFFC0, "adc%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4180, 0xFFC0, "sbc%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x41C0, 0xFFC0, "ror%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4200, 0xFFC0, "tst%c\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4240, 0xFFC0, "neg%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4280, 0xFFC0, "cmp%c\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x42C0, 0xFFC0, "cmn%c\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4300, 0xFFC0, "orr%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4340, 0xFFC0, "mul%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4380, 0xFFC0, "bic%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x43C0, 0xFFC0, "mvn%C\t%0-2r, %3-5r"},
  /* format 13 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xB000, 0xFF80, "add%c\tsp, #%0-6W"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xB080, 0xFF80, "sub%c\tsp, #%0-6W"},
  /* format 5 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4700, 0xFF80, "bx%c\t%S%x"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4400, 0xFF00, "add%c\t%D, %S"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4500, 0xFF00, "cmp%c\t%D, %S"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x4600, 0xFF00, "mov%c\t%D, %S"},
  /* format 14 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xB400, 0xFE00, "push%c\t%N"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xBC00, 0xFE00, "pop%c\t%O"},
  /* format 2 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x1800, 0xFE00, "add%C\t%0-2r, %3-5r, %6-8r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x1A00, 0xFE00, "sub%C\t%0-2r, %3-5r, %6-8r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x1C00, 0xFE00, "add%C\t%0-2r, %3-5r, #%6-8d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x1E00, 0xFE00, "sub%C\t%0-2r, %3-5r, #%6-8d"},
  /* format 8 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x5200, 0xFE00, "strh%c\t%0-2r, [%3-5r, %6-8r]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x5A00, 0xFE00, "ldrh%c\t%0-2r, [%3-5r, %6-8r]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x5600, 0xF600, "ldrs%11?hb%c\t%0-2r, [%3-5r, %6-8r]"},
  /* format 7 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x5000, 0xFA00, "str%10'b%c\t%0-2r, [%3-5r, %6-8r]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x5800, 0xFA00, "ldr%10'b%c\t%0-2r, [%3-5r, %6-8r]"},
  /* format 1 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x0000, 0xFFC0, "mov%C\t%0-2r, %3-5r"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x0000, 0xF800, "lsl%C\t%0-2r, %3-5r, #%6-10d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x0800, 0xF800, "lsr%C\t%0-2r, %3-5r, %s"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x1000, 0xF800, "asr%C\t%0-2r, %3-5r, %s"},
  /* format 3 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x2000, 0xF800, "mov%C\t%8-10r, #%0-7d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x2800, 0xF800, "cmp%c\t%8-10r, #%0-7d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x3000, 0xF800, "add%C\t%8-10r, #%0-7d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0x3800, 0xF800, "sub%C\t%8-10r, #%0-7d"},
  /* format 6 */
  /* TODO: Disassemble PC relative "LDR rD,=<symbolic>" */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x4800, 0xF800,
    "ldr%c\t%8-10r, [pc, #%0-7W]\t; (%0-7a)"},
  /* format 9 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x6000, 0xF800, "str%c\t%0-2r, [%3-5r, #%6-10W]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x6800, 0xF800, "ldr%c\t%0-2r, [%3-5r, #%6-10W]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x7000, 0xF800, "strb%c\t%0-2r, [%3-5r, #%6-10d]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x7800, 0xF800, "ldrb%c\t%0-2r, [%3-5r, #%6-10d]"},
  /* format 10 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x8000, 0xF800, "strh%c\t%0-2r, [%3-5r, #%6-10H]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x8800, 0xF800, "ldrh%c\t%0-2r, [%3-5r, #%6-10H]"},
  /* format 11 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x9000, 0xF800, "str%c\t%8-10r, [sp, #%0-7W]"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0x9800, 0xF800, "ldr%c\t%8-10r, [sp, #%0-7W]"},
  /* format 12 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0xA000, 0xF800, "add%c\t%8-10r, pc, #%0-7W\t; (adr %8-10r, %0-7a)"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
    0xA800, 0xF800, "add%c\t%8-10r, sp, #%0-7W"},
  /* format 15 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xC000, 0xF800, "stmia%c\t%8-10r!, %M"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xC800, 0xF800, "ldmia%c\t%8-10r%W, %M"},
  /* format 17 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xDF00, 0xFF00, "svc%c\t%0-7d"},
  /* format 16 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xDE00, 0xFF00, "udf%c\t#%0-7d"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xDE00, 0xFE00, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xD000, 0xF000, "b%8-11c.n\t%0-7B%X"},
  /* format 18 */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T), 0xE000, 0xF800, "b%c.n\t%0-10B%x"},

  /* The E800 .. FFFF range is unconditionally redirected to the
     32-bit table, because even in pre-V6T2 ISAs, BL and BLX(1) pairs
     are processed via that table.  Thus, we can never encounter a
     bare "second half of BL/BLX(1)" instruction here.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),  0x0000, 0x0000, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (0), 0, 0, 0}
};

/* Thumb32 opcodes use the same table structure as the ARM opcodes.
   We adopt the convention that hw1 is the high 16 bits of .value and
   .mask, hw2 the low 16 bits.

   print_insn_thumb32 recognizes the following format control codes:

       %%		%

       %I		print a 12-bit immediate from hw1[10],hw2[14:12,7:0]
       %M		print a modified 12-bit immediate (same location)
       %J		print a 16-bit immediate from hw1[3:0,10],hw2[14:12,7:0]
       %K		print a 16-bit immediate from hw2[3:0],hw1[3:0],hw2[11:4]
       %H		print a 16-bit immediate from hw2[3:0],hw1[11:0]
       %S		print a possibly-shifted Rm

       %L		print address for a ldrd/strd instruction
       %a		print the address of a plain load/store
       %w		print the width and signedness of a core load/store
       %m		print register mask for ldm/stm

       %E		print the lsb and width fields of a bfc/bfi instruction
       %F		print the lsb and width fields of a sbfx/ubfx instruction
       %b		print a conditional branch offset
       %B		print an unconditional branch offset
       %s		print the shift field of an SSAT instruction
       %R		print the rotation field of an SXT instruction
       %U		print barrier type.
       %P		print address for pli instruction.
       %c		print the condition code
       %x		print warning if conditional an not at end of IT block"
       %X		print "\t; unpredictable <IT:code>" if conditional

       %<bitfield>d	print bitfield in decimal
       %<bitfield>D     print bitfield plus one in decimal
       %<bitfield>W	print bitfield*4 in decimal
       %<bitfield>r	print bitfield as an ARM register
       %<bitfield>R	as %<>r but r15 is UNPREDICTABLE
       %<bitfield>c	print bitfield as a condition code

       %<bitfield>'c	print specified char iff bitfield is all ones
       %<bitfield>`c	print specified char iff bitfield is all zeroes
       %<bitfield>?ab... select from array of values in big endian order

   With one exception at the bottom (done because BL and BLX(1) need
   to come dead last), this table was machine-sorted first in
   decreasing order of number of bits set in the mask, then in
   increasing numeric order of mask, then in increasing numeric order
   of opcode.  This order is not the clearest for a human reader, but
   is guaranteed never to catch a special-case bit pattern with a more
   general mask, which is important, because this instruction encoding
   makes heavy use of special-case bit patterns.  */
static const struct opcode32 thumb32_opcodes[] =
{
  /* These have been 32-bit since the invention of Thumb.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
     0xf000c000, 0xf800d001, "blx%c\t%B%x"},
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V4T),
     0xf000d000, 0xf800d000, "bl%c\t%B%x"},

  /* Fallback.  */
  {ARM_FEATURE_CORE_LOW (ARM_EXT_V1),
      0x00000000, 0x00000000, UNDEFINED_INSTRUCTION},
  {ARM_FEATURE_CORE_LOW (0), 0, 0, 0}
};

static const char *const arm_conditional[] =
{"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
 "hi", "ls", "ge", "lt", "gt", "le", "al", "<und>", ""};

static const char *const arm_fp_const[] =
{"0.0", "1.0", "2.0", "3.0", "4.0", "5.0", "0.5", "10.0"};

static const char *const arm_shift[] =
{"lsl", "lsr", "asr", "ror"};

typedef struct
{
  const char *name;
  const char *description;
  const char *reg_names[16];
}
arm_regname;

static const arm_regname regnames[] =
{
  { "reg-names-raw", N_("Select raw register names"),
    { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"}},
  { "reg-names-gcc", N_("Select register names used by GCC"),
    { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "sl",  "fp",  "ip",  "sp",  "lr",  "pc" }},
  { "reg-names-std", N_("Select register names used in ARM's ISA documentation"),
    { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp",  "lr",  "pc" }},
  { "force-thumb", N_("Assume all insns are Thumb insns"), {NULL} },
  { "no-force-thumb", N_("Examine preceding label to determine an insn's type"), {NULL} },
  { "reg-names-apcs", N_("Select register names used in the APCS"),
    { "a1", "a2", "a3", "a4", "v1", "v2", "v3", "v4", "v5", "v6", "sl",  "fp",  "ip",  "sp",  "lr",  "pc" }},
  { "reg-names-atpcs", N_("Select register names used in the ATPCS"),
    { "a1", "a2", "a3", "a4", "v1", "v2", "v3", "v4", "v5", "v6", "v7",  "v8",  "IP",  "SP",  "LR",  "PC" }},
  { "reg-names-special-atpcs", N_("Select special register names used in the ATPCS"),
    { "a1", "a2", "a3", "a4", "v1", "v2", "v3", "WR", "v5", "SB", "SL",  "FP",  "IP",  "SP",  "LR",  "PC" }}
};

static const char *const iwmmxt_wwnames[] =
{"b", "h", "w", "d"};

static const char *const iwmmxt_wwssnames[] =
{"b", "bus", "bc", "bss",
 "h", "hus", "hc", "hss",
 "w", "wus", "wc", "wss",
 "d", "dus", "dc", "dss"
};

static const char *const iwmmxt_regnames[] =
{ "wr0", "wr1", "wr2", "wr3", "wr4", "wr5", "wr6", "wr7",
  "wr8", "wr9", "wr10", "wr11", "wr12", "wr13", "wr14", "wr15"
};

static const char *const iwmmxt_cregnames[] =
{ "wcid", "wcon", "wcssf", "wcasf", "reserved", "reserved", "reserved", "reserved",
  "wcgr0", "wcgr1", "wcgr2", "wcgr3", "reserved", "reserved", "reserved", "reserved"
};

/* Default to GCC register name set.  */
static unsigned int regname_selected = 1;

#define NUM_ARM_OPTIONS   ARRAY_SIZE (regnames)
#define arm_regnames      regnames[regname_selected].reg_names

static bfd_boolean force_thumb = FALSE;

/* Current IT instruction state.  This contains the same state as the IT
   bits in the CPSR.  */
static unsigned int ifthen_state;
/* IT state for the next instruction.  */
static unsigned int ifthen_next_state;
/* The address of the insn for which the IT state is valid.  */
static bfd_vma ifthen_address;
#define IFTHEN_COND ((ifthen_state >> 4) & 0xf)
/* Indicates that the current Conditional state is unconditional or outside
   an IT block.  */
#define COND_UNCOND 16


/* Functions.  */

/* Decode a bitfield of the form matching regexp (N(-N)?,)*N(-N)?.
   Returns pointer to following character of the format string and
   fills in *VALUEP and *WIDTHP with the extracted value and number of
   bits extracted.  WIDTHP can be NULL.  */

static const char *
arm_decode_bitfield (const char *ptr,
		     uint32_t insn,
		     uint32_t *valuep,
		     int *widthp)
{
  uint32_t value = 0;
  int width = 0;

  do
    {
      int start, end;
      int bits;

      for (start = 0; *ptr >= '0' && *ptr <= '9'; ptr++)
	start = start * 10 + *ptr - '0';
      if (*ptr == '-')
	for (end = 0, ptr++; *ptr >= '0' && *ptr <= '9'; ptr++)
	  end = end * 10 + *ptr - '0';
      else
	end = start;
      bits = end - start;
      if (bits < 0)
	abort ();
      value |= ((insn >> start) & ((2ul << bits) - 1)) << width;
      width += bits + 1;
    }
  while (*ptr++ == ',');
  *valuep = value;
  if (widthp)
    *widthp = width;
  return ptr - 1;
}

static void
arm_decode_shift (long given, fprintf_ftype func, void *stream,
		  bfd_boolean print_shift)
{
  func (stream, "%s", arm_regnames[given & 0xf]);

  if ((given & 0xff0) != 0)
    {
      if ((given & 0x10) == 0)
	{
	  int amount = (given & 0xf80) >> 7;
	  int shift = (given & 0x60) >> 5;

	  if (amount == 0)
	    {
	      if (shift == 3)
		{
		  func (stream, ", rrx");
		  return;
		}

	      amount = 32;
	    }

	  if (print_shift)
	    func (stream, ", %s #%d", arm_shift[shift], amount);
	  else
	    func (stream, ", #%d", amount);
	}
      else if ((given & 0x80) == 0x80)
	func (stream, "\t; <illegal shifter operand>");
      else if (print_shift)
	func (stream, ", %s %s", arm_shift[(given & 0x60) >> 5],
	      arm_regnames[(given & 0xf00) >> 8]);
      else
	func (stream, ", %s", arm_regnames[(given & 0xf00) >> 8]);
    }
}

#define W_BIT 21
#define I_BIT 22
#define U_BIT 23
#define P_BIT 24

#define WRITEBACK_BIT_SET   (given & (1 << W_BIT))
#define IMMEDIATE_BIT_SET   (given & (1 << I_BIT))
#define NEGATIVE_BIT_SET   ((given & (1 << U_BIT)) == 0)
#define PRE_BIT_SET         (given & (1 << P_BIT))

/* Print one coprocessor instruction on INFO->STREAM.
   Return TRUE if the instuction matched, FALSE if this is not a
   recognised coprocessor instruction.  */

static bfd_boolean
print_insn_coprocessor (bfd_vma pc,
			struct disassemble_info *info,
			long given,
			bfd_boolean thumb)
{
  const struct opcode32 *insn;
  void *stream = info->stream;
  fprintf_ftype func = info->fprintf_func;
  uint32_t mask;
  uint32_t value = 0;
  int cond;
  int cp_num;
  struct arm_private_data *private_data = info->private_data;
  arm_feature_set allowed_arches = ARM_ARCH_NONE;

  allowed_arches = private_data->features;

  for (insn = coprocessor_opcodes; insn->assembler; insn++)
    {
      uint32_t u_reg = 16;
      bfd_boolean is_unpredictable = FALSE;
      signed long value_in_comment = 0;
      const char *c;

      if (ARM_FEATURE_ZERO (insn->arch))
	switch (insn->value)
	  {
	  case SENTINEL_IWMMXT_START:
	    if (info->mach != bfd_mach_arm_XScale
		&& info->mach != bfd_mach_arm_iWMMXt
		&& info->mach != bfd_mach_arm_iWMMXt2)
	      do
		insn++;
	      while ((! ARM_FEATURE_ZERO (insn->arch))
		     && insn->value != SENTINEL_IWMMXT_END);
	    continue;

	  case SENTINEL_IWMMXT_END:
	    continue;

	  case SENTINEL_GENERIC_START:
	    allowed_arches = private_data->features;
	    continue;

	  default:
	    abort ();
	  }

      mask = insn->mask;
      value = insn->value;
      cp_num = (given >> 8) & 0xf;

      if (thumb)
	{
	  /* The high 4 bits are 0xe for Arm conditional instructions, and
	     0xe for arm unconditional instructions.  The rest of the
	     encoding is the same.  */
	  mask |= 0xf0000000;
	  value |= 0xe0000000;
	  if (ifthen_state)
	    cond = IFTHEN_COND;
	  else
	    cond = COND_UNCOND;
	}
      else
	{
	  /* Only match unconditional instuctions against unconditional
	     patterns.  */
	  if ((given & 0xf0000000) == 0xf0000000)
	    {
	      mask |= 0xf0000000;
	      cond = COND_UNCOND;
	    }
	  else
	    {
	      cond = (given >> 28) & 0xf;
	      if (cond == 0xe)
		cond = COND_UNCOND;
	    }
	}

      if ((given & mask) != value)
	continue;

      if (! ARM_CPU_HAS_FEATURE (insn->arch, allowed_arches))
	continue;

      if (insn->value == 0xfe000010     /* mcr2  */
	  || insn->value == 0xfe100010  /* mrc2  */
	  || insn->value == 0xfc100000  /* ldc2  */
	  || insn->value == 0xfc000000) /* stc2  */
	{
	  if (cp_num == 9 || cp_num == 10 || cp_num == 11)
	    is_unpredictable = TRUE;
	}
      else if (insn->value == 0x0e000000     /* cdp  */
	       || insn->value == 0xfe000000  /* cdp2  */
	       || insn->value == 0x0e000010  /* mcr  */
	       || insn->value == 0x0e100010  /* mrc  */
	       || insn->value == 0x0c100000  /* ldc  */
	       || insn->value == 0x0c000000) /* stc  */
	{
	  /* Floating-point instructions.  */
	  if (cp_num == 9 || cp_num == 10 || cp_num == 11)
	    continue;
	}

      for (c = insn->assembler; *c; c++)
	{
	  if (*c == '%')
	    {
	      switch (*++c)
		{
		case '%':
		  func (stream, "%%");
		  break;

		case 'A':
		  {
		    int rn = (given >> 16) & 0xf;
		    bfd_vma offset = given & 0xff;

		    func (stream, "[%s", arm_regnames [(given >> 16) & 0xf]);

		    if (PRE_BIT_SET || WRITEBACK_BIT_SET)
		      {
			/* Not unindexed.  The offset is scaled.  */
			if (cp_num == 9)
			  /* vldr.16/vstr.16 will shift the address
			     left by 1 bit only.  */
			  offset = offset * 2;
			else
			  offset = offset * 4;

			if (NEGATIVE_BIT_SET)
			  offset = - offset;
			if (rn != 15)
			  value_in_comment = offset;
		      }

		    if (PRE_BIT_SET)
		      {
			if (offset)
			  func (stream, ", #%d]%s",
				(int) offset,
				WRITEBACK_BIT_SET ? "!" : "");
			else if (NEGATIVE_BIT_SET)
			  func (stream, ", #-0]");
			else
			  func (stream, "]");
		      }
		    else
		      {
			func (stream, "]");

			if (WRITEBACK_BIT_SET)
			  {
			    if (offset)
			      func (stream, ", #%d", (int) offset);
			    else if (NEGATIVE_BIT_SET)
			      func (stream, ", #-0");
			  }
			else
			  {
			    func (stream, ", {%s%d}",
				  (NEGATIVE_BIT_SET && !offset) ? "-" : "",
				  (int) offset);
			    value_in_comment = offset;
			  }
		      }
		    if (rn == 15 && (PRE_BIT_SET || WRITEBACK_BIT_SET))
		      {
			func (stream, "\t; ");
			/* For unaligned PCs, apply off-by-alignment
			   correction.  */
			info->print_address_func (offset + pc
						  + info->bytes_per_chunk * 2
						  - (pc & 3),
				 		  info);
		      }
		  }
		  break;

		case 'B':
		  {
		    int regno = ((given >> 12) & 0xf) | ((given >> (22 - 4)) & 0x10);
		    int offset = (given >> 1) & 0x3f;

		    if (offset == 1)
		      func (stream, "{d%d}", regno);
		    else if (regno + offset > 32)
		      func (stream, "{d%d-<overflow reg d%d>}", regno, regno + offset - 1);
		    else
		      func (stream, "{d%d-d%d}", regno, regno + offset - 1);
		  }
		  break;

		case 'u':
		  if (cond != COND_UNCOND)
		    is_unpredictable = TRUE;

		  /* Fall through.  */
		case 'c':
		  if (cond != COND_UNCOND && cp_num == 9)
		    is_unpredictable = TRUE;

		  func (stream, "%s", arm_conditional[cond]);
		  break;

		case 'I':
		  /* Print a Cirrus/DSP shift immediate.  */
		  /* Immediates are 7bit signed ints with bits 0..3 in
		     bits 0..3 of opcode and bits 4..6 in bits 5..7
		     of opcode.  */
		  {
		    int imm;

		    imm = (given & 0xf) | ((given & 0xe0) >> 1);

		    /* Is ``imm'' a negative number?  */
		    if (imm & 0x40)
		      imm -= 0x80;

		    func (stream, "%d", imm);
		  }

		  break;

		case 'F':
		  switch (given & 0x00408000)
		    {
		    case 0:
		      func (stream, "4");
		      break;
		    case 0x8000:
		      func (stream, "1");
		      break;
		    case 0x00400000:
		      func (stream, "2");
		      break;
		    default:
		      func (stream, "3");
		    }
		  break;

		case 'P':
		  switch (given & 0x00080080)
		    {
		    case 0:
		      func (stream, "s");
		      break;
		    case 0x80:
		      func (stream, "d");
		      break;
		    case 0x00080000:
		      func (stream, "e");
		      break;
		    default:
		      func (stream, _("<illegal precision>"));
		      break;
		    }
		  break;

		case 'Q':
		  switch (given & 0x00408000)
		    {
		    case 0:
		      func (stream, "s");
		      break;
		    case 0x8000:
		      func (stream, "d");
		      break;
		    case 0x00400000:
		      func (stream, "e");
		      break;
		    default:
		      func (stream, "p");
		      break;
		    }
		  break;

		case 'R':
		  switch (given & 0x60)
		    {
		    case 0:
		      break;
		    case 0x20:
		      func (stream, "p");
		      break;
		    case 0x40:
		      func (stream, "m");
		      break;
		    default:
		      func (stream, "z");
		      break;
		    }
		  break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		  {
		    int width;

		    c = arm_decode_bitfield (c, given, &value, &width);

		    switch (*c)
		      {
		      case 'R':
			if (value == 15)
			  is_unpredictable = TRUE;
			/* Fall through.  */
		      case 'r':
			if (c[1] == 'u')
			  {
			    /* Eat the 'u' character.  */
			    ++ c;

			    if (u_reg == value)
			      is_unpredictable = TRUE;
			    u_reg = value;
			  }
			func (stream, "%s", arm_regnames[value]);
			break;
		      case 'V':
			if (given & (1 << 6))
			  goto Q;
			/* FALLTHROUGH */
		      case 'D':
			func (stream, "d%d", value);
			break;
		      case 'Q':
		      Q:
			if (value & 1)
			  func (stream, "<illegal reg q%d.5>", value >> 1);
			else
			  func (stream, "q%d", value >> 1);
			break;
		      case 'd':
			func (stream, "%d", value);
			value_in_comment = value;
			break;
		      case 'E':
                        {
			  /* Converts immediate 8 bit back to float value.  */
			  unsigned floatVal = (value & 0x80) << 24
			    | (value & 0x3F) << 19
			    | ((value & 0x40) ? (0xF8 << 22) : (1 << 30));

			  /* Quarter float have a maximum value of 31.0.
			     Get floating point value multiplied by 1e7.
			     The maximum value stays in limit of a 32-bit int.  */
			  unsigned decVal =
			    (78125 << (((floatVal >> 23) & 0xFF) - 124)) *
			    (16 + (value & 0xF));

			  if (!(decVal % 1000000))
			    func (stream, "%d\t; 0x%08x %c%u.%01u", value,
				  floatVal, value & 0x80 ? '-' : ' ',
				  decVal / 10000000,
				  decVal % 10000000 / 1000000);
			  else if (!(decVal % 10000))
			    func (stream, "%d\t; 0x%08x %c%u.%03u", value,
				  floatVal, value & 0x80 ? '-' : ' ',
				  decVal / 10000000,
				  decVal % 10000000 / 10000);
			  else
			    func (stream, "%d\t; 0x%08x %c%u.%07u", value,
				  floatVal, value & 0x80 ? '-' : ' ',
				  decVal / 10000000, decVal % 10000000);
			  break;
			}
		      case 'k':
			{
			  int from = (given & (1 << 7)) ? 32 : 16;
			  func (stream, "%d", from - value);
			}
			break;

		      case 'f':
			if (value > 7)
			  func (stream, "#%s", arm_fp_const[value & 7]);
			else
			  func (stream, "f%d", value);
			break;

		      case 'w':
			if (width == 2)
			  func (stream, "%s", iwmmxt_wwnames[value]);
			else
			  func (stream, "%s", iwmmxt_wwssnames[value]);
			break;

		      case 'g':
			func (stream, "%s", iwmmxt_regnames[value]);
			break;
		      case 'G':
			func (stream, "%s", iwmmxt_cregnames[value]);
			break;

		      case 'x':
			func (stream, "0x%x", (value));
			break;

		      case 'c':
			switch (value)
			  {
			  case 0:
			    func (stream, "eq");
			    break;

			  case 1:
			    func (stream, "vs");
			    break;

			  case 2:
			    func (stream, "ge");
			    break;

			  case 3:
			    func (stream, "gt");
			    break;

			  default:
			    func (stream, "??");
			    break;
			  }
			break;

		      case '`':
			c++;
			if (value == 0)
			  func (stream, "%c", *c);
			break;
		      case '\'':
			c++;
			if (value == ((1ul << width) - 1))
			  func (stream, "%c", *c);
			break;
		      case '?':
			func (stream, "%c", c[(1 << width) - (int) value]);
			c += 1 << width;
			break;
		      default:
			abort ();
		      }
		    break;

		  case 'y':
		  case 'z':
		    {
		      int single = *c++ == 'y';
		      int regno;

		      switch (*c)
			{
			case '4': /* Sm pair */
			case '0': /* Sm, Dm */
			  regno = given & 0x0000000f;
			  if (single)
			    {
			      regno <<= 1;
			      regno += (given >> 5) & 1;
			    }
			  else
			    regno += ((given >> 5) & 1) << 4;
			  break;

			case '1': /* Sd, Dd */
			  regno = (given >> 12) & 0x0000000f;
			  if (single)
			    {
			      regno <<= 1;
			      regno += (given >> 22) & 1;
			    }
			  else
			    regno += ((given >> 22) & 1) << 4;
			  break;

			case '2': /* Sn, Dn */
			  regno = (given >> 16) & 0x0000000f;
			  if (single)
			    {
			      regno <<= 1;
			      regno += (given >> 7) & 1;
			    }
			  else
			    regno += ((given >> 7) & 1) << 4;
			  break;

			case '3': /* List */
			  func (stream, "{");
			  regno = (given >> 12) & 0x0000000f;
			  if (single)
			    {
			      regno <<= 1;
			      regno += (given >> 22) & 1;
			    }
			  else
			    regno += ((given >> 22) & 1) << 4;
			  break;

			default:
			  abort ();
			}

		      func (stream, "%c%d", single ? 's' : 'd', regno);

		      if (*c == '3')
			{
			  int count = given & 0xff;

			  if (single == 0)
			    count >>= 1;

			  if (--count)
			    {
			      func (stream, "-%c%d",
				    single ? 's' : 'd',
				    regno + count);
			    }

			  func (stream, "}");
			}
		      else if (*c == '4')
			func (stream, ", %c%d", single ? 's' : 'd',
			      regno + 1);
		    }
		    break;

		  case 'L':
		    switch (given & 0x00400100)
		      {
		      case 0x00000000: func (stream, "b"); break;
		      case 0x00400000: func (stream, "h"); break;
		      case 0x00000100: func (stream, "w"); break;
		      case 0x00400100: func (stream, "d"); break;
		      default:
			break;
		      }
		    break;

		  case 'Z':
		    {
		      /* given (20, 23) | given (0, 3) */
		      value = ((given >> 16) & 0xf0) | (given & 0xf);
		      func (stream, "%d", (int) value);
		    }
		    break;

		  case 'l':
		    /* This is like the 'A' operator, except that if
		       the width field "M" is zero, then the offset is
		       *not* multiplied by four.  */
		    {
		      int offset = given & 0xff;
		      int multiplier = (given & 0x00000100) ? 4 : 1;

		      func (stream, "[%s", arm_regnames [(given >> 16) & 0xf]);

		      if (multiplier > 1)
			{
			  value_in_comment = offset * multiplier;
			  if (NEGATIVE_BIT_SET)
			    value_in_comment = - value_in_comment;
			}

		      if (offset)
			{
			  if (PRE_BIT_SET)
			    func (stream, ", #%s%d]%s",
				  NEGATIVE_BIT_SET ? "-" : "",
				  offset * multiplier,
				  WRITEBACK_BIT_SET ? "!" : "");
			  else
			    func (stream, "], #%s%d",
				  NEGATIVE_BIT_SET ? "-" : "",
				  offset * multiplier);
			}
		      else
			func (stream, "]");
		    }
		    break;

		  case 'r':
		    {
		      int imm4 = (given >> 4) & 0xf;
		      int puw_bits = ((given >> 22) & 6) | ((given >> W_BIT) & 1);
		      int ubit = ! NEGATIVE_BIT_SET;
		      const char *rm = arm_regnames [given & 0xf];
		      const char *rn = arm_regnames [(given >> 16) & 0xf];

		      switch (puw_bits)
			{
			case 1:
			case 3:
			  func (stream, "[%s], %c%s", rn, ubit ? '+' : '-', rm);
			  if (imm4)
			    func (stream, ", lsl #%d", imm4);
			  break;

			case 4:
			case 5:
			case 6:
			case 7:
			  func (stream, "[%s, %c%s", rn, ubit ? '+' : '-', rm);
			  if (imm4 > 0)
			    func (stream, ", lsl #%d", imm4);
			  func (stream, "]");
			  if (puw_bits == 5 || puw_bits == 7)
			    func (stream, "!");
			  break;

			default:
			  func (stream, "INVALID");
			}
		    }
		    break;

		  case 'i':
		    {
		      long imm5;
		      imm5 = ((given & 0x100) >> 4) | (given & 0xf);
		      func (stream, "%d", (imm5 == 0) ? 32 : imm5);
		    }
		    break;

		  default:
		    abort ();
		  }
		}
	    }
	  else
	    func (stream, "%c", *c);
	}

      if (value_in_comment > 32 || value_in_comment < -16)
	func (stream, "\t; 0x%x", value_in_comment);

      if (is_unpredictable)
	func (stream, UNPREDICTABLE_INSTRUCTION);

      return TRUE;
    }
  return FALSE;
}

/* Decodes and prints ARM addressing modes.  Returns the offset
   used in the address, if any, if it is worthwhile printing the
   offset as a hexadecimal value in a comment at the end of the
   line of disassembly.  */

static signed long
print_arm_address (bfd_vma pc, struct disassemble_info *info, long given)
{
  void *stream = info->stream;
  fprintf_ftype func = info->fprintf_func;
  bfd_vma offset = 0;

  if (((given & 0x000f0000) == 0x000f0000)
      && ((given & 0x02000000) == 0))
    {
      offset = given & 0xfff;

      func (stream, "[pc");

      if (PRE_BIT_SET)
	{
	  /* Pre-indexed.  Elide offset of positive zero when
	     non-writeback.  */
	  if (WRITEBACK_BIT_SET || NEGATIVE_BIT_SET || offset)
	    func (stream, ", #%s%d", NEGATIVE_BIT_SET ? "-" : "", (int) offset);

	  if (NEGATIVE_BIT_SET)
	    offset = -offset;

	  offset += pc + 8;

	  /* Cope with the possibility of write-back
	     being used.  Probably a very dangerous thing
	     for the programmer to do, but who are we to
	     argue ?  */
	  func (stream, "]%s", WRITEBACK_BIT_SET ? "!" : "");
	}
      else  /* Post indexed.  */
	{
	  func (stream, "], #%s%d", NEGATIVE_BIT_SET ? "-" : "", (int) offset);

	  /* Ie ignore the offset.  */
	  offset = pc + 8;
	}

      func (stream, "\t; ");
      info->print_address_func (offset, info);
      offset = 0;
    }
  else
    {
      func (stream, "[%s",
	    arm_regnames[(given >> 16) & 0xf]);

      if (PRE_BIT_SET)
	{
	  if ((given & 0x02000000) == 0)
	    {
	      /* Elide offset of positive zero when non-writeback.  */
	      offset = given & 0xfff;
	      if (WRITEBACK_BIT_SET || NEGATIVE_BIT_SET || offset)
		func (stream, ", #%s%d", NEGATIVE_BIT_SET ? "-" : "", (int) offset);
	    }
	  else
	    {
	      func (stream, ", %s", NEGATIVE_BIT_SET ? "-" : "");
	      arm_decode_shift (given, func, stream, TRUE);
	    }

	  func (stream, "]%s",
		WRITEBACK_BIT_SET ? "!" : "");
	}
      else
	{
	  if ((given & 0x02000000) == 0)
	    {
	      /* Always show offset.  */
	      offset = given & 0xfff;
	      func (stream, "], #%s%d",
		    NEGATIVE_BIT_SET ? "-" : "", (int) offset);
	    }
	  else
	    {
	      func (stream, "], %s",
		    NEGATIVE_BIT_SET ? "-" : "");
	      arm_decode_shift (given, func, stream, TRUE);
	    }
	}
      if (NEGATIVE_BIT_SET)
	offset = -offset;
    }

  return (signed long) offset;
}

/* Return the name of a v7A special register.  */

static const char *
banked_regname (unsigned reg)
{
  switch (reg)
    {
      case 15: return "CPSR";
      case 32: return "R8_usr";
      case 33: return "R9_usr";
      case 34: return "R10_usr";
      case 35: return "R11_usr";
      case 36: return "R12_usr";
      case 37: return "SP_usr";
      case 38: return "LR_usr";
      case 40: return "R8_fiq";
      case 41: return "R9_fiq";
      case 42: return "R10_fiq";
      case 43: return "R11_fiq";
      case 44: return "R12_fiq";
      case 45: return "SP_fiq";
      case 46: return "LR_fiq";
      case 48: return "LR_irq";
      case 49: return "SP_irq";
      case 50: return "LR_svc";
      case 51: return "SP_svc";
      case 52: return "LR_abt";
      case 53: return "SP_abt";
      case 54: return "LR_und";
      case 55: return "SP_und";
      case 60: return "LR_mon";
      case 61: return "SP_mon";
      case 62: return "ELR_hyp";
      case 63: return "SP_hyp";
      case 79: return "SPSR";
      case 110: return "SPSR_fiq";
      case 112: return "SPSR_irq";
      case 114: return "SPSR_svc";
      case 116: return "SPSR_abt";
      case 118: return "SPSR_und";
      case 124: return "SPSR_mon";
      case 126: return "SPSR_hyp";
      default: return NULL;
    }
}

/* Return the name of the DMB/DSB option.  */
static const char *
data_barrier_option (unsigned option)
{
  switch (option & 0xf)
    {
    case 0xf: return "sy";
    case 0xe: return "st";
    case 0xd: return "ld";
    case 0xb: return "ish";
    case 0xa: return "ishst";
    case 0x9: return "ishld";
    case 0x7: return "un";
    case 0x6: return "unst";
    case 0x5: return "nshld";
    case 0x3: return "osh";
    case 0x2: return "oshst";
    case 0x1: return "oshld";
    default:  return NULL;
    }
}

/* Print one ARM instruction from PC on INFO->STREAM.  */

static void
print_insn_arm (bfd_vma pc, struct disassemble_info *info, long given)
{
  const struct opcode32 *insn;
  void *stream = info->stream;
  fprintf_ftype func = info->fprintf_func;
  struct arm_private_data *private_data = info->private_data;

  if (print_insn_coprocessor (pc, info, given, FALSE))
    return;

  for (insn = arm_opcodes; insn->assembler; insn++)
    {
      if ((given & insn->mask) != insn->value)
	continue;

      if (! ARM_CPU_HAS_FEATURE (insn->arch, private_data->features))
	continue;

      /* Special case: an instruction with all bits set in the condition field
	 (0xFnnn_nnnn) is only matched if all those bits are set in insn->mask,
	 or by the catchall at the end of the table.  */
      if ((given & 0xF0000000) != 0xF0000000
	  || (insn->mask & 0xF0000000) == 0xF0000000
	  || (insn->mask == 0 && insn->value == 0))
	{
	  uint32_t u_reg = 16;
	  uint32_t U_reg = 16;
	  bfd_boolean is_unpredictable = FALSE;
	  signed long value_in_comment = 0;
	  const char *c;

	  for (c = insn->assembler; *c; c++)
	    {
	      if (*c == '%')
		{
		  bfd_boolean allow_unpredictable = FALSE;

		  switch (*++c)
		    {
		    case '%':
		      func (stream, "%%");
		      break;

		    case 'a':
		      value_in_comment = print_arm_address (pc, info, given);
		      break;

		    case 'P':
		      /* Set P address bit and use normal address
			 printing routine.  */
		      value_in_comment = print_arm_address (pc, info, given | (1 << P_BIT));
		      break;

		    case 'S':
		      allow_unpredictable = TRUE;
		      /* Fall through.  */
		    case 's':
                      if ((given & 0x004f0000) == 0x004f0000)
			{
                          /* PC relative with immediate offset.  */
			  bfd_vma offset = ((given & 0xf00) >> 4) | (given & 0xf);

			  if (PRE_BIT_SET)
			    {
			      /* Elide positive zero offset.  */
			      if (offset || NEGATIVE_BIT_SET)
				func (stream, "[pc, #%s%d]\t; ",
				      NEGATIVE_BIT_SET ? "-" : "", (int) offset);
			      else
				func (stream, "[pc]\t; ");
			      if (NEGATIVE_BIT_SET)
				offset = -offset;
			      info->print_address_func (offset + pc + 8, info);
			    }
			  else
			    {
			      /* Always show the offset.  */
			      func (stream, "[pc], #%s%d",
				    NEGATIVE_BIT_SET ? "-" : "", (int) offset);
			      if (! allow_unpredictable)
				is_unpredictable = TRUE;
			    }
			}
		      else
			{
			  int offset = ((given & 0xf00) >> 4) | (given & 0xf);

			  func (stream, "[%s",
				arm_regnames[(given >> 16) & 0xf]);

			  if (PRE_BIT_SET)
			    {
			      if (IMMEDIATE_BIT_SET)
				{
				  /* Elide offset for non-writeback
				     positive zero.  */
				  if (WRITEBACK_BIT_SET || NEGATIVE_BIT_SET
				      || offset)
				    func (stream, ", #%s%d",
					  NEGATIVE_BIT_SET ? "-" : "", offset);

				  if (NEGATIVE_BIT_SET)
				    offset = -offset;

				  value_in_comment = offset;
				}
			      else
				{
				  /* Register Offset or Register Pre-Indexed.  */
				  func (stream, ", %s%s",
					NEGATIVE_BIT_SET ? "-" : "",
					arm_regnames[given & 0xf]);

				  /* Writing back to the register that is the source/
				     destination of the load/store is unpredictable.  */
				  if (! allow_unpredictable
				      && WRITEBACK_BIT_SET
				      && ((given & 0xf) == ((given >> 12) & 0xf)))
				    is_unpredictable = TRUE;
				}

			      func (stream, "]%s",
				    WRITEBACK_BIT_SET ? "!" : "");
			    }
			  else
			    {
			      if (IMMEDIATE_BIT_SET)
				{
				  /* Immediate Post-indexed.  */
				  /* PR 10924: Offset must be printed, even if it is zero.  */
				  func (stream, "], #%s%d",
					NEGATIVE_BIT_SET ? "-" : "", offset);
				  if (NEGATIVE_BIT_SET)
				    offset = -offset;
				  value_in_comment = offset;
				}
			      else
				{
				  /* Register Post-indexed.  */
				  func (stream, "], %s%s",
					NEGATIVE_BIT_SET ? "-" : "",
					arm_regnames[given & 0xf]);

				  /* Writing back to the register that is the source/
				     destination of the load/store is unpredictable.  */
				  if (! allow_unpredictable
				      && (given & 0xf) == ((given >> 12) & 0xf))
				    is_unpredictable = TRUE;
				}

			      if (! allow_unpredictable)
				{
				  /* Writeback is automatically implied by post- addressing.
				     Setting the W bit is unnecessary and ARM specify it as
				     being unpredictable.  */
				  if (WRITEBACK_BIT_SET
				      /* Specifying the PC register as the post-indexed
					 registers is also unpredictable.  */
				      || (! IMMEDIATE_BIT_SET && ((given & 0xf) == 0xf)))
				    is_unpredictable = TRUE;
				}
			    }
			}
		      break;

		    case 'b':
		      {
			bfd_vma disp = (((given & 0xffffff) ^ 0x800000) - 0x800000);
			info->print_address_func (disp * 4 + pc + 8, info);
		      }
		      break;

		    case 'c':
		      if (((given >> 28) & 0xf) != 0xe)
			func (stream, "%s",
			      arm_conditional [(given >> 28) & 0xf]);
		      break;

		    case 'm':
		      {
			int started = 0;
			int reg;

			func (stream, "{");
			for (reg = 0; reg < 16; reg++)
			  if ((given & (1 << reg)) != 0)
			    {
			      if (started)
				func (stream, ", ");
			      started = 1;
			      func (stream, "%s", arm_regnames[reg]);
			    }
			func (stream, "}");
			if (! started)
			  is_unpredictable = TRUE;
		      }
		      break;

		    case 'q':
		      arm_decode_shift (given, func, stream, FALSE);
		      break;

		    case 'o':
		      if ((given & 0x02000000) != 0)
			{
			  unsigned int rotate = (given & 0xf00) >> 7;
			  unsigned int immed = (given & 0xff);
			  unsigned int a, i;

			  a = (((immed << (32 - rotate))
				| (immed >> rotate)) & 0xffffffff);
			  /* If there is another encoding with smaller rotate,
			     the rotate should be specified directly.  */
			  for (i = 0; i < 32; i += 2)
			    if ((a << i | a >> (32 - i)) <= 0xff)
			      break;

			  if (i != rotate)
			    func (stream, "#%d, %d", immed, rotate);
			  else
			    func (stream, "#%d", a);
			  value_in_comment = a;
			}
		      else
			arm_decode_shift (given, func, stream, TRUE);
		      break;

		    case 'p':
		      if ((given & 0x0000f000) == 0x0000f000)
			{
			    func (stream, "p");
			}
		      break;

		    case 't':
		      if ((given & 0x01200000) == 0x00200000)
			func (stream, "t");
		      break;

		    case 'A':
		      {
			int offset = given & 0xff;

			value_in_comment = offset * 4;
			if (NEGATIVE_BIT_SET)
			  value_in_comment = - value_in_comment;

			func (stream, "[%s", arm_regnames [(given >> 16) & 0xf]);

			if (PRE_BIT_SET)
			  {
			    if (offset)
			      func (stream, ", #%d]%s",
				    (int) value_in_comment,
				    WRITEBACK_BIT_SET ? "!" : "");
			    else
			      func (stream, "]");
			  }
			else
			  {
			    func (stream, "]");

			    if (WRITEBACK_BIT_SET)
			      {
				if (offset)
				  func (stream, ", #%d", (int) value_in_comment);
			      }
			    else
			      {
				func (stream, ", {%d}", (int) offset);
				value_in_comment = offset;
			      }
			  }
		      }
		      break;

		    case 'B':
		      /* Print ARM V5 BLX(1) address: pc+25 bits.  */
		      {
			bfd_vma address;
			bfd_vma offset = 0;

			if (! NEGATIVE_BIT_SET)
			  /* Is signed, hi bits should be ones.  */
			  offset = (-1) ^ 0x00ffffff;

			/* Offset is (SignExtend(offset field)<<2).  */
			offset += given & 0x00ffffff;
			offset <<= 2;
			address = offset + pc + 8;

			if (given & 0x01000000)
			  /* H bit allows addressing to 2-byte boundaries.  */
			  address += 2;

		        info->print_address_func (address, info);
		      }
		      break;

		    case 'C':
		      if ((given & 0x02000200) == 0x200)
			{
			  const char * name;
			  unsigned sysm = (given & 0x004f0000) >> 16;

			  sysm |= (given & 0x300) >> 4;
			  name = banked_regname (sysm);

			  if (name != NULL)
			    func (stream, "%s", name);
			  else
			    func (stream, "(UNDEF: %u)", (uint32_t) sysm);
			}
		      else
			{
			  func (stream, "%cPSR_",
				(given & 0x00400000) ? 'S' : 'C');
			  if (given & 0x80000)
			    func (stream, "f");
			  if (given & 0x40000)
			    func (stream, "s");
			  if (given & 0x20000)
			    func (stream, "x");
			  if (given & 0x10000)
			    func (stream, "c");
			}
		      break;

		    case 'U':
		      if ((given & 0xf0) == 0x60)
			{
			  switch (given & 0xf)
			    {
			    case 0xf: func (stream, "sy"); break;
			    default:
			      func (stream, "#%d", (int) given & 0xf);
			      break;
			    }
			}
		      else
			{
			  const char * opt = data_barrier_option (given & 0xf);
			  if (opt != NULL)
			    func (stream, "%s", opt);
			  else
			      func (stream, "#%d", (int) given & 0xf);
			}
		      break;

		    case '0': case '1': case '2': case '3': case '4':
		    case '5': case '6': case '7': case '8': case '9':
		      {
			int width;
			uint32_t value;

			c = arm_decode_bitfield (c, given, &value, &width);

			switch (*c)
			  {
			  case 'R':
			    if (value == 15)
			      is_unpredictable = TRUE;
			    /* Fall through.  */
			  case 'r':
			  case 'T':
			    /* We want register + 1 when decoding T.  */
			    if (*c == 'T')
			      ++value;

			    if (c[1] == 'u')
			      {
				/* Eat the 'u' character.  */
				++ c;

				if (u_reg == value)
				  is_unpredictable = TRUE;
				u_reg = value;
			      }
			    if (c[1] == 'U')
			      {
				/* Eat the 'U' character.  */
				++ c;

				if (U_reg == value)
				  is_unpredictable = TRUE;
				U_reg = value;
			      }
			    func (stream, "%s", arm_regnames[value]);
			    break;
			  case 'd':
			    func (stream, "%d", value);
			    value_in_comment = value;
			    break;
			  case 'b':
			    func (stream, "%d", value * 8);
			    value_in_comment = value * 8;
			    break;
			  case 'W':
			    func (stream, "%d", value + 1);
			    value_in_comment = value + 1;
			    break;
			  case 'x':
			    func (stream, "0x%08x", value);

			    /* Some SWI instructions have special
			       meanings.  */
			    if ((given & 0x0fffffff) == 0x0FF00000)
			      func (stream, "\t; IMB");
			    else if ((given & 0x0fffffff) == 0x0FF00001)
			      func (stream, "\t; IMBRange");
			    break;
			  case 'X':
			    func (stream, "%01x", value & 0xf);
			    value_in_comment = value;
			    break;
			  case '`':
			    c++;
			    if (value == 0)
			      func (stream, "%c", *c);
			    break;
			  case '\'':
			    c++;
			    if (value == ((1ul << width) - 1))
			      func (stream, "%c", *c);
			    break;
			  case '?':
			    func (stream, "%c", c[(1 << width) - (int) value]);
			    c += 1 << width;
			    break;
			  default:
			    abort ();
			  }
			break;

		      case 'e':
			{
			  int imm;

			  imm = (given & 0xf) | ((given & 0xfff00) >> 4);
			  func (stream, "%d", imm);
			  value_in_comment = imm;
			}
			break;

		      case 'E':
			/* LSB and WIDTH fields of BFI or BFC.  The machine-
			   language instruction encodes LSB and MSB.  */
			{
			  int32_t msb = (given & 0x001f0000) >> 16;
			  int32_t lsb = (given & 0x00000f80) >> 7;
			  int32_t w = msb - lsb + 1;

			  if (w > 0)
			    func (stream, "#%u, #%u", lsb, w);
			  else
			    func (stream, "(invalid: %u:%u)", lsb, msb);
			}
			break;

		      case 'R':
			/* Get the PSR/banked register name.  */
			{
			  const char * name;
			  unsigned sysm = (given & 0x004f0000) >> 16;

			  sysm |= (given & 0x300) >> 4;
			  name = banked_regname (sysm);

			  if (name != NULL)
			    func (stream, "%s", name);
			  else
			    func (stream, "(UNDEF: %u)", (uint32_t) sysm);
			}
			break;

		      case 'V':
			/* 16-bit unsigned immediate from a MOVT or MOVW
			   instruction, encoded in bits 0:11 and 15:19.  */
			{
			  int32_t hi = (given & 0x000f0000) >> 4;
			  int32_t lo = (given & 0x00000fff);
			  int32_t imm16 = hi | lo;

			  func (stream, "#%u", imm16);
			  value_in_comment = imm16;
			}
			break;

		      default:
			abort ();
		      }
		    }
		}
	      else
		func (stream, "%c", *c);
	    }

	  if (value_in_comment > 32 || value_in_comment < -16)
	    func (stream, "\t; 0x%lx", value_in_comment);

	  if (is_unpredictable)
	    func (stream, UNPREDICTABLE_INSTRUCTION);

	  return;
	}
    }
  abort ();
}

/* Print one 16-bit Thumb instruction from PC on INFO->STREAM.  */

static void
print_insn_thumb16 (bfd_vma pc, struct disassemble_info *info, long given)
{
  const struct opcode16 *insn;
  void *stream = info->stream;
  fprintf_ftype func = info->fprintf_func;

  for (insn = thumb_opcodes; insn->assembler; insn++)
    if ((given & insn->mask) == insn->value)
      {
	signed long value_in_comment = 0;
	const char *c = insn->assembler;

	for (; *c; c++)
	  {
	    int domaskpc = 0;
	    int domasklr = 0;

	    if (*c != '%')
	      {
		func (stream, "%c", *c);
		continue;
	      }

	    switch (*++c)
	      {
	      case '%':
		func (stream, "%%");
		break;

	      case 'c':
		if (ifthen_state)
		  func (stream, "%s", arm_conditional[IFTHEN_COND]);
		break;

	      case 'C':
		if (ifthen_state)
		  func (stream, "%s", arm_conditional[IFTHEN_COND]);
		else
		  func (stream, "s");
		break;

	      case 'I':
		{
		  unsigned int tmp;

		  ifthen_next_state = given & 0xff;
		  for (tmp = given << 1; tmp & 0xf; tmp <<= 1)
		    func (stream, ((given ^ tmp) & 0x10) ? "e" : "t");
		  func (stream, "\t%s", arm_conditional[(given >> 4) & 0xf]);
		}
		break;

	      case 'x':
		if (ifthen_next_state)
		  func (stream, "\t; unpredictable branch in IT block\n");
		break;

	      case 'X':
		if (ifthen_state)
		  func (stream, "\t; unpredictable <IT:%s>",
			arm_conditional[IFTHEN_COND]);
		break;

	      case 'S':
		{
		  long reg;

		  reg = (given >> 3) & 0x7;
		  if (given & (1 << 6))
		    reg += 8;

		  func (stream, "%s", arm_regnames[reg]);
		}
		break;

	      case 'D':
		{
		  long reg;

		  reg = given & 0x7;
		  if (given & (1 << 7))
		    reg += 8;

		  func (stream, "%s", arm_regnames[reg]);
		}
		break;

	      case 'N':
		if (given & (1 << 8))
		  domasklr = 1;
		/* Fall through.  */
	      case 'O':
		if (*c == 'O' && (given & (1 << 8)))
		  domaskpc = 1;
		/* Fall through.  */
	      case 'M':
		{
		  int started = 0;
		  int reg;

		  func (stream, "{");

		  /* It would be nice if we could spot
		     ranges, and generate the rS-rE format: */
		  for (reg = 0; (reg < 8); reg++)
		    if ((given & (1 << reg)) != 0)
		      {
			if (started)
			  func (stream, ", ");
			started = 1;
			func (stream, "%s", arm_regnames[reg]);
		      }

		  if (domasklr)
		    {
		      if (started)
			func (stream, ", ");
		      started = 1;
		      func (stream, "%s", arm_regnames[14] /* "lr" */);
		    }

		  if (domaskpc)
		    {
		      if (started)
			func (stream, ", ");
		      func (stream, "%s", arm_regnames[15] /* "pc" */);
		    }

		  func (stream, "}");
		}
		break;

	      case 'W':
		/* Print writeback indicator for a LDMIA.  We are doing a
		   writeback if the base register is not in the register
		   mask.  */
		if ((given & (1 << ((given & 0x0700) >> 8))) == 0)
		  func (stream, "!");
	      	break;

	      case 'b':
		/* Print ARM V6T2 CZB address: pc+4+6 bits.  */
		{
		  bfd_vma address = (pc + 4
				     + ((given & 0x00f8) >> 2)
				     + ((given & 0x0200) >> 3));
		  info->print_address_func (address, info);
		}
		break;

	      case 's':
		/* Right shift immediate -- bits 6..10; 1-31 print
		   as themselves, 0 prints as 32.  */
		{
		  long imm = (given & 0x07c0) >> 6;
		  if (imm == 0)
		    imm = 32;
		  func (stream, "#%d", imm);
		}
		break;

	      case '0': case '1': case '2': case '3': case '4':
	      case '5': case '6': case '7': case '8': case '9':
		{
		  int bitstart = *c++ - '0';
		  int bitend = 0;

		  while (*c >= '0' && *c <= '9')
		    bitstart = (bitstart * 10) + *c++ - '0';

		  switch (*c)
		    {
		    case '-':
		      {
			bfd_vma reg;

			c++;
			while (*c >= '0' && *c <= '9')
			  bitend = (bitend * 10) + *c++ - '0';
			if (!bitend)
			  abort ();
			reg = given >> bitstart;
			reg &= (2 << (bitend - bitstart)) - 1;

			switch (*c)
			  {
			  case 'r':
			    func (stream, "%s", arm_regnames[reg]);
			    break;

			  case 'd':
			    func (stream, "%d", (int32_t) reg);
			    value_in_comment = reg;
			    break;

			  case 'H':
			    func (stream, "%d", (int32_t) (reg << 1));
			    value_in_comment = reg << 1;
			    break;

			  case 'W':
			    func (stream, "%d", (int32_t) (reg << 2));
			    value_in_comment = reg << 2;
			    break;

			  case 'a':
			    /* PC-relative address -- the bottom two
			       bits of the address are dropped
			       before the calculation.  */
			    info->print_address_func
			      (((pc + 4) & ~3) + (reg << 2), info);
			    value_in_comment = 0;
			    break;

			  case 'x':
			    func (stream, "0x%04x", (int32_t) reg);
			    break;

			  case 'B':
			    reg = ((reg ^ (1 << bitend)) - (1 << bitend));
			    info->print_address_func (reg * 2 + pc + 4, info);
			    value_in_comment = 0;
			    break;

			  case 'c':
			    func (stream, "%s", arm_conditional [reg]);
			    break;

			  default:
			    abort ();
			  }
		      }
		      break;

		    case '\'':
		      c++;
		      if ((given & (1 << bitstart)) != 0)
			func (stream, "%c", *c);
		      break;

		    case '?':
		      ++c;
		      if ((given & (1 << bitstart)) != 0)
			func (stream, "%c", *c++);
		      else
			func (stream, "%c", *++c);
		      break;

		    default:
		      abort ();
		    }
		}
		break;

	      default:
		abort ();
	      }
	  }

	if (value_in_comment > 32 || value_in_comment < -16)
	  func (stream, "\t; 0x%x", value_in_comment);
	return;
      }

  /* No match.  */
  abort ();
}

/* Return the name of an V7M special register.  */

static const char *
psr_name (int regno)
{
  switch (regno)
    {
    case 0x0: return "APSR";
    case 0x1: return "IAPSR";
    case 0x2: return "EAPSR";
    case 0x3: return "PSR";
    case 0x5: return "IPSR";
    case 0x6: return "EPSR";
    case 0x7: return "IEPSR";
    case 0x8: return "MSP";
    case 0x9: return "PSP";
    case 0xa: return "MSPLIM";
    case 0xb: return "PSPLIM";
    case 0x10: return "PRIMASK";
    case 0x11: return "BASEPRI";
    case 0x12: return "BASEPRI_MAX";
    case 0x13: return "FAULTMASK";
    case 0x14: return "CONTROL";
    case 0x88: return "MSP_NS";
    case 0x89: return "PSP_NS";
    case 0x8a: return "MSPLIM_NS";
    case 0x8b: return "PSPLIM_NS";
    case 0x90: return "PRIMASK_NS";
    case 0x91: return "BASEPRI_NS";
    case 0x93: return "FAULTMASK_NS";
    case 0x94: return "CONTROL_NS";
    case 0x98: return "SP_NS";
    default: return "<unknown>";
    }
}

/* Print one 32-bit Thumb instruction from PC on INFO->STREAM.  */

static void
print_insn_thumb32 (bfd_vma pc, struct disassemble_info *info, long given)
{
  const struct opcode32 *insn;
  void *stream = info->stream;
  fprintf_ftype func = info->fprintf_func;

  if (print_insn_coprocessor (pc, info, given, TRUE))
    return;

  for (insn = thumb32_opcodes; insn->assembler; insn++)
    if ((given & insn->mask) == insn->value)
      {
	bfd_boolean is_unpredictable = FALSE;
	signed long value_in_comment = 0;
	const char *c = insn->assembler;

	for (; *c; c++)
	  {
	    if (*c != '%')
	      {
		func (stream, "%c", *c);
		continue;
	      }

	    switch (*++c)
	      {
	      case '%':
		func (stream, "%%");
		break;

	      case 'c':
		if (ifthen_state)
		  func (stream, "%s", arm_conditional[IFTHEN_COND]);
		break;

	      case 'x':
		if (ifthen_next_state)
		  func (stream, "\t; unpredictable branch in IT block\n");
		break;

	      case 'X':
		if (ifthen_state)
		  func (stream, "\t; unpredictable <IT:%s>",
			arm_conditional[IFTHEN_COND]);
		break;

	      case 'I':
		{
		  unsigned int imm12 = 0;

		  imm12 |= (given & 0x000000ffu);
		  imm12 |= (given & 0x00007000u) >> 4;
		  imm12 |= (given & 0x04000000u) >> 15;
		  func (stream, "#%u", imm12);
		  value_in_comment = imm12;
		}
		break;

	      case 'M':
		{
		  unsigned int bits = 0, imm, imm8, mod;

		  bits |= (given & 0x000000ffu);
		  bits |= (given & 0x00007000u) >> 4;
		  bits |= (given & 0x04000000u) >> 15;
		  imm8 = (bits & 0x0ff);
		  mod = (bits & 0xf00) >> 8;
		  switch (mod)
		    {
		    case 0: imm = imm8; break;
		    case 1: imm = ((imm8 << 16) | imm8); break;
		    case 2: imm = ((imm8 << 24) | (imm8 << 8)); break;
		    case 3: imm = ((imm8 << 24) | (imm8 << 16) | (imm8 << 8) | imm8); break;
		    default:
		      mod  = (bits & 0xf80) >> 7;
		      imm8 = (bits & 0x07f) | 0x80;
		      imm  = (((imm8 << (32 - mod)) | (imm8 >> mod)) & 0xffffffff);
		    }
		  func (stream, "#%u", imm);
		  value_in_comment = imm;
		}
		break;

	      case 'J':
		{
		  unsigned int imm = 0;

		  imm |= (given & 0x000000ffu);
		  imm |= (given & 0x00007000u) >> 4;
		  imm |= (given & 0x04000000u) >> 15;
		  imm |= (given & 0x000f0000u) >> 4;
		  func (stream, "#%u", imm);
		  value_in_comment = imm;
		}
		break;

	      case 'K':
		{
		  unsigned int imm = 0;

		  imm |= (given & 0x000f0000u) >> 16;
		  imm |= (given & 0x00000ff0u) >> 0;
		  imm |= (given & 0x0000000fu) << 12;
		  func (stream, "#%u", imm);
		  value_in_comment = imm;
		}
		break;

	      case 'H':
		{
		  unsigned int imm = 0;

		  imm |= (given & 0x000f0000u) >> 4;
		  imm |= (given & 0x00000fffu) >> 0;
		  func (stream, "#%u", imm);
		  value_in_comment = imm;
		}
		break;

	      case 'V':
		{
		  unsigned int imm = 0;

		  imm |= (given & 0x00000fffu);
		  imm |= (given & 0x000f0000u) >> 4;
		  func (stream, "#%u", imm);
		  value_in_comment = imm;
		}
		break;

	      case 'S':
		{
		  unsigned int reg = (given & 0x0000000fu);
		  unsigned int stp = (given & 0x00000030u) >> 4;
		  unsigned int imm = 0;
		  imm |= (given & 0x000000c0u) >> 6;
		  imm |= (given & 0x00007000u) >> 10;

		  func (stream, "%s", arm_regnames[reg]);
		  switch (stp)
		    {
		    case 0:
		      if (imm > 0)
			func (stream, ", lsl #%u", imm);
		      break;

		    case 1:
		      if (imm == 0)
			imm = 32;
		      func (stream, ", lsr #%u", imm);
		      break;

		    case 2:
		      if (imm == 0)
			imm = 32;
		      func (stream, ", asr #%u", imm);
		      break;

		    case 3:
		      if (imm == 0)
			func (stream, ", rrx");
		      else
			func (stream, ", ror #%u", imm);
		    }
		}
		break;

	      case 'a':
		{
		  unsigned int Rn  = (given & 0x000f0000) >> 16;
		  unsigned int U   = ! NEGATIVE_BIT_SET;
		  unsigned int op  = (given & 0x00000f00) >> 8;
		  unsigned int i12 = (given & 0x00000fff);
		  unsigned int i8  = (given & 0x000000ff);
		  bfd_boolean writeback = FALSE, postind = FALSE;
		  bfd_vma offset = 0;

		  func (stream, "[%s", arm_regnames[Rn]);
		  if (U) /* 12-bit positive immediate offset.  */
		    {
		      offset = i12;
		      if (Rn != 15)
			value_in_comment = offset;
		    }
		  else if (Rn == 15) /* 12-bit negative immediate offset.  */
		    offset = - (int) i12;
		  else if (op == 0x0) /* Shifted register offset.  */
		    {
		      unsigned int Rm = (i8 & 0x0f);
		      unsigned int sh = (i8 & 0x30) >> 4;

		      func (stream, ", %s", arm_regnames[Rm]);
		      if (sh)
			func (stream, ", lsl #%u", sh);
		      func (stream, "]");
		      break;
		    }
		  else switch (op)
		    {
		    case 0xE:  /* 8-bit positive immediate offset.  */
		      offset = i8;
		      break;

		    case 0xC:  /* 8-bit negative immediate offset.  */
		      offset = -i8;
		      break;

		    case 0xF:  /* 8-bit + preindex with wb.  */
		      offset = i8;
		      writeback = TRUE;
		      break;

		    case 0xD:  /* 8-bit - preindex with wb.  */
		      offset = -i8;
		      writeback = TRUE;
		      break;

		    case 0xB:  /* 8-bit + postindex.  */
		      offset = i8;
		      postind = TRUE;
		      break;

		    case 0x9:  /* 8-bit - postindex.  */
		      offset = -i8;
		      postind = TRUE;
		      break;

		    default:
		      func (stream, ", <undefined>]");
		      goto skip;
		    }

		  if (postind)
		    func (stream, "], #%d", (int) offset);
		  else
		    {
		      if (offset)
			func (stream, ", #%d", (int) offset);
		      func (stream, writeback ? "]!" : "]");
		    }

		  if (Rn == 15)
		    {
		      func (stream, "\t; ");
		      info->print_address_func (((pc + 4) & ~3) + offset, info);
		    }
		}
	      skip:
		break;

	      case 'A':
		{
		  unsigned int U   = ! NEGATIVE_BIT_SET;
		  unsigned int W   = WRITEBACK_BIT_SET;
		  unsigned int Rn  = (given & 0x000f0000) >> 16;
		  unsigned int off = (given & 0x000000ff);

		  func (stream, "[%s", arm_regnames[Rn]);

		  if (PRE_BIT_SET)
		    {
		      if (off || !U)
			{
			  func (stream, ", #%c%u", U ? '+' : '-', off * 4);
			  value_in_comment = off * 4 * (U ? 1 : -1);
			}
		      func (stream, "]");
		      if (W)
			func (stream, "!");
		    }
		  else
		    {
		      func (stream, "], ");
		      if (W)
			{
			  func (stream, "#%c%u", U ? '+' : '-', off * 4);
			  value_in_comment = off * 4 * (U ? 1 : -1);
			}
		      else
			{
			  func (stream, "{%u}", off);
			  value_in_comment = off;
			}
		    }
		}
		break;

	      case 'w':
		{
		  unsigned int Sbit = (given & 0x01000000) >> 24;
		  unsigned int type = (given & 0x00600000) >> 21;

		  switch (type)
		    {
		    case 0: func (stream, Sbit ? "sb" : "b"); break;
		    case 1: func (stream, Sbit ? "sh" : "h"); break;
		    case 2:
		      if (Sbit)
			func (stream, "??");
		      break;
		    case 3:
		      func (stream, "??");
		      break;
		    }
		}
		break;

	      case 'm':
		{
		  int started = 0;
		  int reg;

		  func (stream, "{");
		  for (reg = 0; reg < 16; reg++)
		    if ((given & (1 << reg)) != 0)
		      {
			if (started)
			  func (stream, ", ");
			started = 1;
			func (stream, "%s", arm_regnames[reg]);
		      }
		  func (stream, "}");
		}
		break;

	      case 'E':
		{
		  unsigned int msb = (given & 0x0000001f);
		  unsigned int lsb = 0;

		  lsb |= (given & 0x000000c0u) >> 6;
		  lsb |= (given & 0x00007000u) >> 10;
		  func (stream, "#%u, #%u", lsb, msb - lsb + 1);
		}
		break;

	      case 'F':
		{
		  unsigned int width = (given & 0x0000001f) + 1;
		  unsigned int lsb = 0;

		  lsb |= (given & 0x000000c0u) >> 6;
		  lsb |= (given & 0x00007000u) >> 10;
		  func (stream, "#%u, #%u", lsb, width);
		}
		break;

	      case 'b':
		{
		  unsigned int S = (given & 0x04000000u) >> 26;
		  unsigned int J1 = (given & 0x00002000u) >> 13;
		  unsigned int J2 = (given & 0x00000800u) >> 11;
		  bfd_vma offset = 0;

		  offset |= !S << 20;
		  offset |= J2 << 19;
		  offset |= J1 << 18;
		  offset |= (given & 0x003f0000) >> 4;
		  offset |= (given & 0x000007ff) << 1;
		  offset -= (1 << 20);

		  info->print_address_func (pc + 4 + offset, info);
		}
		break;

	      case 'B':
		{
		  unsigned int S = (given & 0x04000000u) >> 26;
		  unsigned int I1 = (given & 0x00002000u) >> 13;
		  unsigned int I2 = (given & 0x00000800u) >> 11;
		  bfd_vma offset = 0;

		  offset |= !S << 24;
		  offset |= !(I1 ^ S) << 23;
		  offset |= !(I2 ^ S) << 22;
		  offset |= (given & 0x03ff0000u) >> 4;
		  offset |= (given & 0x000007ffu) << 1;
		  offset -= (1 << 24);
		  offset += pc + 4;

		  /* BLX target addresses are always word aligned.  */
		  if ((given & 0x00001000u) == 0)
		      offset &= ~2u;

		  info->print_address_func (offset, info);
		}
		break;

	      case 's':
		{
		  unsigned int shift = 0;

		  shift |= (given & 0x000000c0u) >> 6;
		  shift |= (given & 0x00007000u) >> 10;
		  if (WRITEBACK_BIT_SET)
		    func (stream, ", asr #%u", shift);
		  else if (shift)
		    func (stream, ", lsl #%u", shift);
		  /* else print nothing - lsl #0 */
		}
		break;

	      case 'R':
		{
		  unsigned int rot = (given & 0x00000030) >> 4;

		  if (rot)
		    func (stream, ", ror #%u", rot * 8);
		}
		break;

	      case 'U':
		if ((given & 0xf0) == 0x60)
		  {
		    switch (given & 0xf)
		      {
			case 0xf: func (stream, "sy"); break;
			default:
			  func (stream, "#%d", (int) given & 0xf);
			      break;
		      }
		  }
		else
		  {
		    const char * opt = data_barrier_option (given & 0xf);
		    if (opt != NULL)
		      func (stream, "%s", opt);
		    else
		      func (stream, "#%d", (int) given & 0xf);
		   }
		break;

	      case 'C':
		if ((given & 0xff) == 0)
		  {
		    func (stream, "%cPSR_", (given & 0x100000) ? 'S' : 'C');
		    if (given & 0x800)
		      func (stream, "f");
		    if (given & 0x400)
		      func (stream, "s");
		    if (given & 0x200)
		      func (stream, "x");
		    if (given & 0x100)
		      func (stream, "c");
		  }
		else if ((given & 0x20) == 0x20)
		  {
		    char const* name;
		    unsigned sysm = (given & 0xf00) >> 8;

		    sysm |= (given & 0x30);
		    sysm |= (given & 0x00100000) >> 14;
		    name = banked_regname (sysm);

		    if (name != NULL)
		      func (stream, "%s", name);
		    else
		      func (stream, "(UNDEF: %u)", (uint32_t) sysm);
		  }
		else
		  {
		    func (stream, "%s", psr_name (given & 0xff));
		  }
		break;

	      case 'D':
		if (((given & 0xff) == 0)
		    || ((given & 0x20) == 0x20))
		  {
		    char const* name;
		    unsigned sm = (given & 0xf0000) >> 16;

		    sm |= (given & 0x30);
		    sm |= (given & 0x00100000) >> 14;
		    name = banked_regname (sm);

		    if (name != NULL)
		      func (stream, "%s", name);
		    else
		      func (stream, "(UNDEF: %u)", (uint32_t) sm);
		  }
		else
		  func (stream, "%s", psr_name (given & 0xff));
		break;

	      case '0': case '1': case '2': case '3': case '4':
	      case '5': case '6': case '7': case '8': case '9':
		{
		  int width;
		  uint32_t val;

		  c = arm_decode_bitfield (c, given, &val, &width);

		  switch (*c)
		    {
		    case 'd':
		      func (stream, "%u", val);
		      value_in_comment = val;
		      break;

		    case 'D':
		      func (stream, "%u", val + 1);
		      value_in_comment = val + 1;
		      break;

		    case 'W':
		      func (stream, "%u", val * 4);
		      value_in_comment = val * 4;
		      break;

		    case 'R':
		      if (val == 15)
			is_unpredictable = TRUE;
		      /* Fall through.  */
		    case 'r':
		      func (stream, "%s", arm_regnames[val]);
		      break;

		    case 'c':
		      func (stream, "%s", arm_conditional[val]);
		      break;

		    case '\'':
		      c++;
		      if (val == ((1ul << width) - 1))
			func (stream, "%c", *c);
		      break;

		    case '`':
		      c++;
		      if (val == 0)
			func (stream, "%c", *c);
		      break;

		    case '?':
		      func (stream, "%c", c[(1 << width) - (int) val]);
		      c += 1 << width;
		      break;

		    case 'x':
		      func (stream, "0x%x", val);
		      break;

		    default:
		      abort ();
		    }
		}
		break;

	      case 'L':
		/* PR binutils/12534
		   If we have a PC relative offset in an LDRD or STRD
		   instructions then display the decoded address.  */
		if (((given >> 16) & 0xf) == 0xf)
		  {
		    bfd_vma offset = (given & 0xff) * 4;

		    if ((given & (1 << 23)) == 0)
		      offset = - offset;
		    func (stream, "\t; ");
		    info->print_address_func ((pc & ~3) + 4 + offset, info);
		  }
		break;

	      default:
		abort ();
	      }
	  }

	if (value_in_comment > 32 || value_in_comment < -16)
	  func (stream, "\t; 0x%x", value_in_comment);

	if (is_unpredictable)
	  func (stream, UNPREDICTABLE_INSTRUCTION);

	return;
      }

  /* No match.  */
  abort ();
}

/* Print data bytes on INFO->STREAM.  */

static void
print_insn_data (bfd_vma pc ATTRIBUTE_UNUSED,
		 struct disassemble_info *info,
		 long given)
{
  switch (info->bytes_per_chunk)
    {
    case 1:
      info->fprintf_func (info->stream, ".byte\t0x%02lx", given);
      break;
    case 2:
      info->fprintf_func (info->stream, ".short\t0x%04lx", given);
      break;
    case 4:
      info->fprintf_func (info->stream, ".word\t0x%08lx", given);
      break;
    default:
      abort ();
    }
}

/* Disallow mapping symbols ($a, $b, $d, $t etc) from
   being displayed in symbol relative addresses.

   Also disallow private symbol, with __tagsym$$ prefix,
   from ARM RVCT toolchain being displayed.  */

bfd_boolean
arm_symbol_is_valid (asymbol * sym,
		     struct disassemble_info * info ATTRIBUTE_UNUSED)
{
  const char * name;

  if (sym == NULL)
    return FALSE;

  name = bfd_asymbol_name (sym);

  return (name && *name != '$' && strncmp (name, "__tagsym$$", 10));
}

/* Parse the string of disassembler options.  */

static void
parse_arm_disassembler_options (const char *options)
{
  const char *opt;

  FOR_EACH_DISASSEMBLER_OPTION (opt, options)
    {
      if (CONST_STRNEQ (opt, "reg-names-"))
	{
	  unsigned int i;
	  for (i = 0; i < NUM_ARM_OPTIONS; i++)
	    if (disassembler_options_cmp (opt, regnames[i].name) == 0)
	      {
		regname_selected = i;
		break;
	      }

	  if (i >= NUM_ARM_OPTIONS)
	    /* xgettext: c-format */
	    opcodes_error_handler (_("unrecognised register name set: %s"),
				   opt);
	}
      else if (CONST_STRNEQ (opt, "force-thumb"))
	force_thumb = 1;
      else if (CONST_STRNEQ (opt, "no-force-thumb"))
	force_thumb = 0;
      else
	/* xgettext: c-format */
	opcodes_error_handler (_("unrecognised disassembler option: %s"), opt);
    }

  return;
}

static bfd_boolean
mapping_symbol_for_insn (bfd_vma pc, struct disassemble_info *info,
			 enum map_type *map_symbol);

/* Search back through the insn stream to determine if this instruction is
   conditionally executed.  */

static void
find_ifthen_state (bfd_vma pc,
		   struct disassemble_info *info,
		   bfd_boolean little)
{
  unsigned char b[2];
  unsigned int insn;
  int status;
  /* COUNT is twice the number of instructions seen.  It will be odd if we
     just crossed an instruction boundary.  */
  int count;
  int it_count;
  unsigned int seen_it;
  bfd_vma addr;

  ifthen_address = pc;
  ifthen_state = 0;

  addr = pc;
  count = 1;
  it_count = 0;
  seen_it = 0;
  /* Scan backwards looking for IT instructions, keeping track of where
     instruction boundaries are.  We don't know if something is actually an
     IT instruction until we find a definite instruction boundary.  */
  for (;;)
    {
      if (addr == 0 || info->symbol_at_address_func (addr, info))
	{
	  /* A symbol must be on an instruction boundary, and will not
	     be within an IT block.  */
	  if (seen_it && (count & 1))
	    break;

	  return;
	}
      addr -= 2;
      status = info->read_memory_func (addr, (bfd_byte *) b, 2, info);
      if (status)
	return;

      if (little)
	insn = (b[0]) | (b[1] << 8);
      else
	insn = (b[1]) | (b[0] << 8);
      if (seen_it)
	{
	  if ((insn & 0xf800) < 0xe800)
	    {
	      /* Addr + 2 is an instruction boundary.  See if this matches
	         the expected boundary based on the position of the last
		 IT candidate.  */
	      if (count & 1)
		break;
	      seen_it = 0;
	    }
	}
      if ((insn & 0xff00) == 0xbf00 && (insn & 0xf) != 0)
	{
	  enum map_type type = MAP_ARM;
	  bfd_boolean found = mapping_symbol_for_insn (addr, info, &type);

	  if (!found || (found && type == MAP_THUMB))
	    {
	      /* This could be an IT instruction.  */
	      seen_it = insn;
	      it_count = count >> 1;
	    }
	}
      if ((insn & 0xf800) >= 0xe800)
	count++;
      else
	count = (count + 2) | 1;
      /* IT blocks contain at most 4 instructions.  */
      if (count >= 8 && !seen_it)
	return;
    }
  /* We found an IT instruction.  */
  ifthen_state = (seen_it & 0xe0) | ((seen_it << it_count) & 0x1f);
  if ((ifthen_state & 0xf) == 0)
    ifthen_state = 0;
}

/* Returns nonzero and sets *MAP_TYPE if the N'th symbol is a
   mapping symbol.  */

static int
is_mapping_symbol (struct disassemble_info *info, int n,
		   enum map_type *map_type)
{
  const char *name;

  name = bfd_asymbol_name (info->symtab[n]);
  if (name[0] == '$' && (name[1] == 'a' || name[1] == 't' || name[1] == 'd')
      && (name[2] == 0 || name[2] == '.'))
    {
      *map_type = ((name[1] == 'a') ? MAP_ARM
		   : (name[1] == 't') ? MAP_THUMB
		   : MAP_DATA);
      return TRUE;
    }

  return FALSE;
}

/* Try to infer the code type (ARM or Thumb) from a mapping symbol.
   Returns nonzero if *MAP_TYPE was set.  */

static int
get_map_sym_type (struct disassemble_info *info,
		  int n,
		  enum map_type *map_type)
{
  /* If the symbol is in a different section, ignore it.  */
  if (info->section != NULL && info->section != info->symtab[n]->section)
    return FALSE;

  return is_mapping_symbol (info, n, map_type);
}

/* Try to infer the code type (ARM or Thumb) from a non-mapping symbol.
   Returns nonzero if *MAP_TYPE was set.  */

static int
get_sym_code_type (struct disassemble_info *info,
		   int n,
		   enum map_type *map_type)
{
  elf_symbol_type *es;
  unsigned int type;

  /* If the symbol is in a different section, ignore it.  */
  if (info->section != NULL && info->section != info->symtab[n]->section)
    return FALSE;

  es = *(elf_symbol_type **)(info->symtab + n);
  type = ELF_ST_TYPE (es->internal_elf_sym.st_info);

  /* If the symbol has function type then use that.  */
  if (type == STT_FUNC || type == STT_GNU_IFUNC)
    {
      if (ARM_GET_SYM_BRANCH_TYPE (es->internal_elf_sym.st_target_internal)
	  == ST_BRANCH_TO_THUMB)
	*map_type = MAP_THUMB;
      else
	*map_type = MAP_ARM;
      return TRUE;
    }

  return FALSE;
}

/* Search the mapping symbol state for instruction at pc.  This is only
   applicable for elf target.

   There is an assumption Here, info->private_data contains the correct AND
   up-to-date information about current scan process.  The information will be
   used to speed this search process.

   Return TRUE if the mapping state can be determined, and map_symbol
   will be updated accordingly.  Otherwise, return FALSE.  */

static bfd_boolean
mapping_symbol_for_insn (bfd_vma pc, struct disassemble_info *info,
			 enum map_type *map_symbol)
{
  bfd_vma addr;
  int n, start = 0;
  bfd_boolean found = FALSE;
  enum map_type type = MAP_ARM;
  struct arm_private_data *private_data;

  if (info->private_data == NULL || info->symtab_size == 0
      || bfd_asymbol_flavour (*info->symtab) != bfd_target_elf_flavour)
    return FALSE;

  private_data = info->private_data;
  if (pc == 0)
    start = 0;
  else
    start = private_data->last_mapping_sym;

  start = (start == -1)? 0 : start;
  addr = bfd_asymbol_value (info->symtab[start]);

  if (pc >= addr)
    {
      if (get_map_sym_type (info, start, &type))
      found = TRUE;
    }
  else
    {
      for (n = start - 1; n >= 0; n--)
	{
	  if (get_map_sym_type (info, n, &type))
	    {
	      found = TRUE;
	      break;
	    }
	}
    }

  /* No mapping symbols were found.  A leading $d may be
     omitted for sections which start with data; but for
     compatibility with legacy and stripped binaries, only
     assume the leading $d if there is at least one mapping
     symbol in the file.  */
  if (!found && private_data->has_mapping_symbols == 1)
    {
      type = MAP_DATA;
      found = TRUE;
    }

  *map_symbol = type;
  return found;
}

/* Given a bfd_mach_arm_XXX value, this function fills in the fields
   of the supplied arm_feature_set structure with bitmasks indicating
   the support base architectures and coprocessor extensions.

   FIXME: This could more efficiently implemented as a constant array,
   although it would also be less robust.  */

static void
select_arm_features (uint32_t mach,
		     arm_feature_set * features)
{
#undef ARM_SET_FEATURES
#define ARM_SET_FEATURES(FSET) \
  {							\
    const arm_feature_set fset = FSET;			\
    arm_feature_set tmp = ARM_FEATURE (0, 0, FPU_FPA) ;	\
    ARM_MERGE_FEATURE_SETS (*features, tmp, fset);	\
  }

  switch (mach)
    {
    case bfd_mach_arm_2:       ARM_SET_FEATURES (ARM_ARCH_V2); break;
    case bfd_mach_arm_2a:      ARM_SET_FEATURES (ARM_ARCH_V2S); break;
    case bfd_mach_arm_3:       ARM_SET_FEATURES (ARM_ARCH_V3); break;
    case bfd_mach_arm_3M:      ARM_SET_FEATURES (ARM_ARCH_V3M); break;
    case bfd_mach_arm_4:       ARM_SET_FEATURES (ARM_ARCH_V4); break;
    case bfd_mach_arm_4T:      ARM_SET_FEATURES (ARM_ARCH_V4T); break;
      /* If the machine type is unknown allow all
	 architecture types and all extensions.  */
    case bfd_mach_arm_unknown: ARM_SET_FEATURES (ARM_FEATURE_ALL); break;
    default:
      abort ();
    }

#undef ARM_SET_FEATURES
}


/* NOTE: There are no checks in these routines that
   the relevant number of data bytes exist.  */

static int
print_insn (bfd_vma pc, struct disassemble_info *info, bfd_boolean little)
{
  unsigned char b[4];
  long		given;
  int           status;
  int           is_thumb = FALSE;
  int           is_data = FALSE;
  int           little_code;
  unsigned int	size = 4;
  void	 	(*printer) (bfd_vma, struct disassemble_info *, long);
  bfd_boolean   found = FALSE;
  struct arm_private_data *private_data;

  if (info->disassembler_options)
    {
      parse_arm_disassembler_options (info->disassembler_options);

      /* To avoid repeated parsing of these options, we remove them here.  */
      info->disassembler_options = NULL;
    }

  /* PR 10288: Control which instructions will be disassembled.  */
  if (info->private_data == NULL)
    {
      static struct arm_private_data private;

      if ((info->flags & USER_SPECIFIED_MACHINE_TYPE) == 0)
	/* If the user did not use the -m command line switch then default to
	   disassembling all types of ARM instruction.

	   The info->mach value has to be ignored as this will be based on
	   the default archictecture for the target and/or hints in the notes
	   section, but it will never be greater than the current largest arm
	   machine value (iWMMXt2), which is only equivalent to the V5TE
	   architecture.  ARM architectures have advanced beyond the machine
	   value encoding, and these newer architectures would be ignored if
	   the machine value was used.

	   Ie the -m switch is used to restrict which instructions will be
	   disassembled.  If it is necessary to use the -m switch to tell
	   objdump that an ARM binary is being disassembled, eg because the
	   input is a raw binary file, but it is also desired to disassemble
	   all ARM instructions then use "-marm".  This will select the
	   "unknown" arm architecture which is compatible with any ARM
	   instruction.  */
	  info->mach = bfd_mach_arm_unknown;

      /* Compute the architecture bitmask from the machine number.
	 Note: This assumes that the machine number will not change
	 during disassembly....  */
      select_arm_features (info->mach, & private.features);

      private.has_mapping_symbols = -1;
      private.last_mapping_sym = -1;
      private.last_mapping_addr = 0;

      info->private_data = & private;
    }

  private_data = info->private_data;

  /* Decide if our code is going to be little-endian, despite what the
     function argument might say.  */
  little_code = ((info->endian_code == BFD_ENDIAN_LITTLE) || little);

  /* For ELF, consult the symbol table to determine what kind of code
     or data we have.  */
  if (info->symtab_size != 0
      && bfd_asymbol_flavour (*info->symtab) == bfd_target_elf_flavour)
    {
      bfd_vma addr;
      int n, start;
      int last_sym = -1;
      enum map_type type = MAP_ARM;

      /* Start scanning at the start of the function, or wherever
	 we finished last time.  */
      /* PR 14006.  When the address is 0 we are either at the start of the
	 very first function, or else the first function in a new, unlinked
	 executable section (eg because of -ffunction-sections).  Either way
	 start scanning from the beginning of the symbol table, not where we
	 left off last time.  */
      if (pc == 0)
	start = 0;
      else
	{
	  start = info->symtab_pos + 1;
	  if (start < private_data->last_mapping_sym)
	    start = private_data->last_mapping_sym;
	}
      found = FALSE;

      /* First, look for mapping symbols.  */
      if (private_data->has_mapping_symbols != 0)
	{
	  /* Scan up to the location being disassembled.  */
	  for (n = start; n < info->symtab_size; n++)
	    {
	      addr = bfd_asymbol_value (info->symtab[n]);
	      if (addr > pc)
		break;
	      if (get_map_sym_type (info, n, &type))
		{
		  last_sym = n;
		  found = TRUE;
		}
	    }

	  if (!found)
	    {
	      /* No mapping symbol found at this address.  Look backwards
		 for a preceding one.  */
	      for (n = start - 1; n >= 0; n--)
		{
		  if (get_map_sym_type (info, n, &type))
		    {
		      last_sym = n;
		      found = TRUE;
		      break;
		    }
		}
	    }

	  if (found)
	    private_data->has_mapping_symbols = 1;

	  /* No mapping symbols were found.  A leading $d may be
	     omitted for sections which start with data; but for
	     compatibility with legacy and stripped binaries, only
	     assume the leading $d if there is at least one mapping
	     symbol in the file.  */
	  if (!found && private_data->has_mapping_symbols == -1)
	    {
	      /* Look for mapping symbols, in any section.  */
	      for (n = 0; n < info->symtab_size; n++)
		if (is_mapping_symbol (info, n, &type))
		  {
		    private_data->has_mapping_symbols = 1;
		    break;
		  }
	      if (private_data->has_mapping_symbols == -1)
		private_data->has_mapping_symbols = 0;
	    }

	  if (!found && private_data->has_mapping_symbols == 1)
	    {
	      type = MAP_DATA;
	      found = TRUE;
	    }
	}

      /* Next search for function symbols to separate ARM from Thumb
	 in binaries without mapping symbols.  */
      if (!found)
	{
	  /* Scan up to the location being disassembled.  */
	  for (n = start; n < info->symtab_size; n++)
	    {
	      addr = bfd_asymbol_value (info->symtab[n]);
	      if (addr > pc)
		break;
	      if (get_sym_code_type (info, n, &type))
		{
		  last_sym = n;
		  found = TRUE;
		}
	    }

	  if (!found)
	    {
	      /* No mapping symbol found at this address.  Look backwards
		 for a preceding one.  */
	      for (n = start - 1; n >= 0; n--)
		{
		  if (get_sym_code_type (info, n, &type))
		    {
		      last_sym = n;
		      found = TRUE;
		      break;
		    }
		}
	    }
	}

      private_data->last_mapping_sym = last_sym;
      private_data->last_type = type;
      is_thumb = (private_data->last_type == MAP_THUMB);
      is_data = (private_data->last_type == MAP_DATA);

      /* Look a little bit ahead to see if we should print out
	 two or four bytes of data.  If there's a symbol,
	 mapping or otherwise, after two bytes then don't
	 print more.  */
      if (is_data)
	{
	  size = 4 - (pc & 3);
	  for (n = last_sym + 1; n < info->symtab_size; n++)
	    {
	      addr = bfd_asymbol_value (info->symtab[n]);
	      if (addr > pc
		  && (info->section == NULL
		      || info->section == info->symtab[n]->section))
		{
		  if (addr - pc < size)
		    size = addr - pc;
		  break;
		}
	    }
	  /* If the next symbol is after three bytes, we need to
	     print only part of the data, so that we can use either
	     .byte or .short.  */
	  if (size == 3)
	    size = (pc & 1) ? 1 : 2;
	}
    }

  if (info->symbols != NULL)
    {
      if (bfd_asymbol_flavour (*info->symbols) == bfd_target_elf_flavour
	       && !found)
	{
	  /* If no mapping symbol has been found then fall back to the type
	     of the function symbol.  */
	  elf_symbol_type *  es;
	  unsigned int       type;

	  es = *(elf_symbol_type **)(info->symbols);
	  type = ELF_ST_TYPE (es->internal_elf_sym.st_info);

	  is_thumb =
	    ((ARM_GET_SYM_BRANCH_TYPE (es->internal_elf_sym.st_target_internal)
	      == ST_BRANCH_TO_THUMB) || type == STT_ARM_16BIT);
	}
    }

  if (force_thumb)
    is_thumb = TRUE;

  if (is_data)
    info->display_endian = little ? BFD_ENDIAN_LITTLE : BFD_ENDIAN_BIG;
  else
    info->display_endian = little_code ? BFD_ENDIAN_LITTLE : BFD_ENDIAN_BIG;

  info->bytes_per_line = 4;

  /* PR 10263: Disassemble data if requested to do so by the user.  */
  if (is_data && ((info->flags & DISASSEMBLE_DATA) == 0))
    {
      int i;

      /* Size was already set above.  */
      info->bytes_per_chunk = size;
      printer = print_insn_data;

      status = info->read_memory_func (pc, (bfd_byte *) b, size, info);
      given = 0;
      if (little)
	for (i = size - 1; i >= 0; i--)
	  given = b[i] | (given << 8);
      else
	for (i = 0; i < (int) size; i++)
	  given = b[i] | (given << 8);
    }
  else if (!is_thumb)
    {
      /* In ARM mode endianness is a straightforward issue: the instruction
	 is four bytes long and is either ordered 0123 or 3210.  */
      printer = print_insn_arm;
      info->bytes_per_chunk = 4;
      size = 4;

      status = info->read_memory_func (pc, (bfd_byte *) b, 4, info);
      if (little_code)
	given = (b[0]) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
      else
	given = (b[3]) | (b[2] << 8) | (b[1] << 16) | (b[0] << 24);
    }
  else
    {
      /* In Thumb mode we have the additional wrinkle of two
	 instruction lengths.  Fortunately, the bits that determine
	 the length of the current instruction are always to be found
	 in the first two bytes.  */
      printer = print_insn_thumb16;
      info->bytes_per_chunk = 2;
      size = 2;

      status = info->read_memory_func (pc, (bfd_byte *) b, 2, info);
      if (little_code)
	given = (b[0]) | (b[1] << 8);
      else
	given = (b[1]) | (b[0] << 8);

      if (!status)
	{
	  /* These bit patterns signal a four-byte Thumb
	     instruction.  */
	  if ((given & 0xF800) == 0xF800
	      || (given & 0xF800) == 0xF000
	      || (given & 0xF800) == 0xE800)
	    {
	      status = info->read_memory_func (pc + 2, (bfd_byte *) b, 2, info);
	      if (little_code)
		given = (b[0]) | (b[1] << 8) | (given << 16);
	      else
		given = (b[1]) | (b[0] << 8) | (given << 16);

	      printer = print_insn_thumb32;
	      size = 4;
	    }
	}

      if (ifthen_address != pc)
	find_ifthen_state (pc, info, little_code);

      if (ifthen_state)
	{
	  if ((ifthen_state & 0xf) == 0x8)
	    ifthen_next_state = 0;
	  else
	    ifthen_next_state = (ifthen_state & 0xe0)
				| ((ifthen_state & 0xf) << 1);
	}
    }

  if (status)
    {
      info->memory_error_func (status, pc, info);
      return -1;
    }
  if (info->flags & INSN_HAS_RELOC)
    /* If the instruction has a reloc associated with it, then
       the offset field in the instruction will actually be the
       addend for the reloc.  (We are using REL type relocs).
       In such cases, we can ignore the pc when computing
       addresses, since the addend is not currently pc-relative.  */
    pc = 0;

  printer (pc, info, given);

  if (is_thumb)
    {
      ifthen_state = ifthen_next_state;
      ifthen_address += size;
    }
  return size;
}

int
print_insn_big_arm (bfd_vma pc, struct disassemble_info *info)
{
  /* Detect BE8-ness and record it in the disassembler info.  */
  if (info->flavour == bfd_target_elf_flavour
      && info->section != NULL
      && (elf_elfheader (info->section->owner)->e_flags & EF_ARM_BE8))
    info->endian_code = BFD_ENDIAN_LITTLE;

  return print_insn (pc, info, FALSE);
}

int
print_insn_little_arm (bfd_vma pc, struct disassemble_info *info)
{
  return print_insn (pc, info, TRUE);
}

const disasm_options_t *
disassembler_options_arm (void)
{
  static disasm_options_t *opts = NULL;

  if (opts == NULL)
    {
      unsigned int i;
      opts = XNEW (disasm_options_t);
      opts->name = XNEWVEC (const char *, NUM_ARM_OPTIONS + 1);
      opts->description = XNEWVEC (const char *, NUM_ARM_OPTIONS + 1);
      for (i = 0; i < NUM_ARM_OPTIONS; i++)
	{
	  opts->name[i] = regnames[i].name;
	  if (regnames[i].description != NULL)
	    opts->description[i] = _(regnames[i].description);
	  else
	    opts->description[i] = NULL;
	}
      /* The array we return must be NULL terminated.  */
      opts->name[i] = NULL;
      opts->description[i] = NULL;
    }

  return opts;
}

void
print_arm_disassembler_options (FILE *stream)
{
  unsigned int i, max_len = 0;
  fprintf (stream, _("\n\
The following ARM specific disassembler options are supported for use with\n\
the -M switch:\n"));

  for (i = 0; i < NUM_ARM_OPTIONS; i++)
    {
      unsigned int len = strlen (regnames[i].name);
      if (max_len < len)
	max_len = len;
    }

  for (i = 0, max_len++; i < NUM_ARM_OPTIONS; i++)
    fprintf (stream, "  %s%*c %s\n",
	     regnames[i].name,
	     (int)(max_len - strlen (regnames[i].name)), ' ',
	     _(regnames[i].description));
}
