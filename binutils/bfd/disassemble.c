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
