/* Select disassembly routine for specified architecture.
   Copyright (C) 1994-2018 Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "disassemble.h"
#include "safe-ctype.h"
#include <assert.h>
//
//#ifdef ARCH_all
//#define ARCH_aarch64
//#define ARCH_alpha
//#define ARCH_arc
//#define ARCH_arm
//#define ARCH_avr
//#define ARCH_bfin
//#define ARCH_cr16
//#define ARCH_cris
//#define ARCH_crx
//#define ARCH_d10v
//#define ARCH_d30v
//#define ARCH_dlx
//#define ARCH_epiphany
//#define ARCH_fr30
//#define ARCH_frv
//#define ARCH_ft32
//#define ARCH_h8300
//#define ARCH_hppa
//#define ARCH_i386
//#define ARCH_ia64
//#define ARCH_ip2k
//#define ARCH_iq2000
//#define ARCH_lm32
//#define ARCH_m32c
//#define ARCH_m32r
//#define ARCH_m68hc11
//#define ARCH_m68hc12
//#define ARCH_m68k
//#define ARCH_mcore
//#define ARCH_mep
//#define ARCH_metag
//#define ARCH_microblaze
//#define ARCH_mips
//#define ARCH_mmix
//#define ARCH_mn10200
//#define ARCH_mn10300
//#define ARCH_moxie
//#define ARCH_mt
//#define ARCH_msp430
//#define ARCH_nds32
//#define ARCH_nfp
//#define ARCH_nios2
//#define ARCH_ns32k
//#define ARCH_or1k
//#define ARCH_pdp11
//#define ARCH_pj
//#define ARCH_powerpc
//#define ARCH_pru
//#define ARCH_riscv
//#define ARCH_rs6000
//#define ARCH_rl78
//#define ARCH_rx
//#define ARCH_s390
//#define ARCH_score
//#define ARCH_sh
//#define ARCH_sparc
//#define ARCH_spu
//#define ARCH_tic30
//#define ARCH_tic4x
//#define ARCH_tic54x
//#define ARCH_tic6x
//#define ARCH_tic80
//#define ARCH_tilegx
//#define ARCH_tilepro
//#define ARCH_v850
//#define ARCH_vax
//#define ARCH_visium
//#define ARCH_wasm32
//#define ARCH_xstormy16
//#define ARCH_xc16x
//#define ARCH_xgate
//#define ARCH_xtensa
//#define ARCH_z80
//#define ARCH_z8k
//#endif
//

disassembler_ftype
disassembler (enum bfd_architecture a,
	      bfd_boolean big ATTRIBUTE_UNUSED,
	      unsigned long mach ATTRIBUTE_UNUSED,
	      bfd *abfd ATTRIBUTE_UNUSED)
{
  disassembler_ftype disassemble;

  switch (a)
    {
      /* If you add a case to this table, also add it to the
	 ARCH_all definition right above this function.  */
    case bfd_arch_arm:
      if (big)
	disassemble = print_insn_big_arm;
      else
	disassemble = print_insn_little_arm;
      break;
    default:
      return 0;
    }
  return disassemble;
}

void
disassembler_usage (FILE *stream ATTRIBUTE_UNUSED)
{
  print_arm_disassembler_options (stream);
  return;
}

void
disassemble_init_for_target (struct disassemble_info * info)
{
  if (info == NULL)
    return;

  switch (info->arch)
    {
    case bfd_arch_arm:
      info->symbol_is_valid = arm_symbol_is_valid;
      info->disassembler_needs_relocs = TRUE;
      break;
    default:
      break;
    }
}

/* Remove whitespace and consecutive commas from OPTIONS.  */

char *
remove_whitespace_and_extra_commas (char *options)
{
  char *str;
  size_t i, len;

  if (options == NULL)
    return NULL;

  /* Strip off all trailing whitespace and commas.  */
  for (len = strlen (options); len > 0; len--)
    {
      if (!ISSPACE (options[len - 1]) && options[len - 1] != ',')
	break;
      options[len - 1] = '\0';
    }

  /* Convert all remaining whitespace to commas.  */
  for (i = 0; options[i] != '\0'; i++)
    if (ISSPACE (options[i]))
      options[i] = ',';

  /* Remove consecutive commas.  */
  for (str = options; *str != '\0'; str++)
    if (*str == ',' && (*(str + 1) == ',' || str == options))
      {
	char *next = str + 1;
	while (*next == ',')
	  next++;
	len = strlen (next);
	if (str != options)
	  str++;
	memmove (str, next, len);
	next[len - (size_t)(next - str)] = '\0';
      }
  return (strlen (options) != 0) ? options : NULL;
}

/* Like STRCMP, but treat ',' the same as '\0' so that we match
   strings like "foobar" against "foobar,xxyyzz,...".  */

int
disassembler_options_cmp (const char *s1, const char *s2)
{
  unsigned char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      if (c1 == ',')
	c1 = '\0';
      c2 = (unsigned char) *s2++;
      if (c2 == ',')
	c2 = '\0';
      if (c1 == '\0')
	return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}
