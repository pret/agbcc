/* BFD library support routines for architectures.
   Copyright (C) 1990-2018 Free Software Foundation, Inc.
   Hacked by John Gilmore and Steve Chamberlain of Cygnus Support.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
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
#include "bfd.h"
#include "libbfd.h"
#include "safe-ctype.h"

/*

SECTION
	Architectures

*/

/*

SUBSECTION
	bfd_architecture
*/

/*
SUBSECTION
	bfd_arch_info
.
*/

extern const bfd_arch_info_type bfd_arm_arch;
static const bfd_arch_info_type * const bfd_archures_list[] =
  {
    SELECT_ARCHITECTURES,
  0
};

/*
FUNCTION
	bfd_printable_name

*/

const char *
bfd_printable_name (bfd *abfd)
{
  return abfd->arch_info->printable_name;
}

/*
FUNCTION
	bfd_scan_arch
*/

const bfd_arch_info_type *
bfd_scan_arch (const char *string)
{
  const bfd_arch_info_type * const *app, *ap;

  /* Look through all the installed architectures.  */
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  if (ap->scan (ap, string))
	    return ap;
	}
    }

  return NULL;
}

/*
FUNCTION
	bfd_arch_list
*/

const char **
bfd_arch_list (void)
{
  int vec_length = 0;
  const char **name_ptr;
  const char **name_list;
  const bfd_arch_info_type * const *app;
  bfd_size_type amt;

  /* Determine the number of architectures.  */
  vec_length = 0;
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      const bfd_arch_info_type *ap;
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  vec_length++;
	}
    }

  amt = (vec_length + 1) * sizeof (char **);
  name_list = (const char **) bfd_malloc (amt);
  if (name_list == NULL)
    return NULL;

  /* Point the list at each of the names.  */
  name_ptr = name_list;
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      const bfd_arch_info_type *ap;
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  *name_ptr = ap->printable_name;
	  name_ptr++;
	}
    }
  *name_ptr = NULL;

  return name_list;
}

/*
FUNCTION
	bfd_arch_get_compatible
*/

const bfd_arch_info_type *
bfd_arch_get_compatible (const bfd *abfd,
			 const bfd *bbfd,
			 bfd_boolean accept_unknowns)
{
  const bfd *ubfd, *kbfd;

  /* Look for an unknown architecture.  */
  if (abfd->arch_info->arch == bfd_arch_unknown)
    ubfd = abfd, kbfd = bbfd;
  else if (bbfd->arch_info->arch == bfd_arch_unknown)
    ubfd = bbfd, kbfd = abfd;
  else
    /* Otherwise architecture-specific code has to decide.  */
    return abfd->arch_info->compatible (abfd->arch_info, bbfd->arch_info);

  /* We can allow an unknown architecture if accept_unknowns
     is true, or if the target is the "binary" format, which
     has an unknown architecture.  Since the binary format can
     only be set by explicit request from the user, it is safe
     to assume that they know what they are doing.  */
  if (accept_unknowns
      || strcmp (bfd_get_target (ubfd), "binary") == 0)
    return kbfd->arch_info;
  return NULL;
}

/*
INTERNAL_DEFINITION
	bfd_default_arch_struct
*/

const bfd_arch_info_type bfd_default_arch_struct = {
  32, 32, 8, bfd_arch_unknown, 0, "unknown", "unknown", 2, TRUE,
  bfd_default_compatible,
  bfd_default_scan,
  bfd_arch_default_fill,
  0,
};

/*
FUNCTION
	bfd_set_arch_info
*/

void
bfd_set_arch_info (bfd *abfd, const bfd_arch_info_type *arg)
{
  abfd->arch_info = arg;
}

/*
FUNCTION
	bfd_default_set_arch_mach
*/

bfd_boolean
bfd_default_set_arch_mach (bfd *abfd,
			   enum bfd_architecture arch,
			   uint32_t mach)
{
  abfd->arch_info = bfd_lookup_arch (arch, mach);
  if (abfd->arch_info != NULL)
    return TRUE;

  abfd->arch_info = &bfd_default_arch_struct;
  bfd_set_error (bfd_error_bad_value);
  return FALSE;
}

/*
FUNCTION
	bfd_get_arch
*/

enum bfd_architecture
bfd_get_arch (bfd *abfd)
{
  return abfd->arch_info->arch;
}

/*
FUNCTION
	bfd_get_mach
*/

uint32_t
bfd_get_mach (bfd *abfd)
{
  return abfd->arch_info->mach;
}

/*
FUNCTION
	bfd_arch_bits_per_byte
*/

unsigned int
bfd_arch_bits_per_byte (bfd *abfd)
{
  return abfd->arch_info->bits_per_byte;
}

/*
FUNCTION
	bfd_arch_bits_per_address
*/

unsigned int
bfd_arch_bits_per_address (bfd *abfd)
{
  return abfd->arch_info->bits_per_address;
}

/*
INTERNAL_FUNCTION
	bfd_default_compatible
*/

const bfd_arch_info_type *
bfd_default_compatible (const bfd_arch_info_type *a,
			const bfd_arch_info_type *b)
{
  if (a->arch != b->arch)
    return NULL;

  if (a->bits_per_word != b->bits_per_word)
    return NULL;

  if (a->mach > b->mach)
    return a;

  if (b->mach > a->mach)
    return b;

  return a;
}

/*
INTERNAL_FUNCTION
	bfd_default_scan

SYNOPSIS
	bfd_boolean bfd_default_scan
	  (const struct bfd_arch_info *info, const char *string);

DESCRIPTION
	The default function for working out whether this is an
	architecture hit and a machine hit.
*/

bfd_boolean
bfd_default_scan (const bfd_arch_info_type *info, const char *string)
{
  const char *ptr_src;
  const char *ptr_tst;
  uint32_t number;
  enum bfd_architecture arch;
  const char *printable_name_colon;

  /* Exact match of the architecture name (ARCH_NAME) and also the
     default architecture?  */
  if (strcasecmp (string, info->arch_name) == 0
      && info->the_default)
    return TRUE;

  /* Exact match of the machine name (PRINTABLE_NAME)?  */
  if (strcasecmp (string, info->printable_name) == 0)
    return TRUE;

  /* Given that printable_name contains no colon, attempt to match:
     ARCH_NAME [ ":" ] PRINTABLE_NAME?  */
  printable_name_colon = strchr (info->printable_name, ':');
  if (printable_name_colon == NULL)
    {
      size_t strlen_arch_name = strlen (info->arch_name);
      if (strncasecmp (string, info->arch_name, strlen_arch_name) == 0)
	{
	  if (string[strlen_arch_name] == ':')
	    {
	      if (strcasecmp (string + strlen_arch_name + 1,
			      info->printable_name) == 0)
		return TRUE;
	    }
	  else
	    {
	      if (strcasecmp (string + strlen_arch_name,
			      info->printable_name) == 0)
		return TRUE;
	    }
	}
    }

  /* Given that PRINTABLE_NAME has the form: <arch> ":" <mach>;
     Attempt to match: <arch> <mach>?  */
  if (printable_name_colon != NULL)
    {
      size_t colon_index = printable_name_colon - info->printable_name;
      if (strncasecmp (string, info->printable_name, colon_index) == 0
	  && strcasecmp (string + colon_index,
			 info->printable_name + colon_index + 1) == 0)
	return TRUE;
    }

  /* Given that PRINTABLE_NAME has the form: <arch> ":" <mach>; Do not
     attempt to match just <mach>, it could be ambiguous.  This test
     is left until later.  */

  /* See how much of the supplied string matches with the
     architecture, eg the string m68k:68020 would match the 68k entry
     up to the :, then we get left with the machine number.  */

  for (ptr_src = string, ptr_tst = info->arch_name;
       *ptr_src && *ptr_tst;
       ptr_src++, ptr_tst++)
    {
      if (*ptr_src != *ptr_tst)
	break;
    }

  /* Chewed up as much of the architecture as will match, skip any
     colons.  */
  if (*ptr_src == ':')
    ptr_src++;

  if (*ptr_src == 0)
    {
      /* Nothing more, then only keep this one if it is the default
	 machine for this architecture.  */
      return info->the_default;
    }

  number = 0;
  while (ISDIGIT (*ptr_src))
    {
      number = number * 10 + *ptr_src - '0';
      ptr_src++;
    }

  if (arch != info->arch)
    return FALSE;

  if (number != info->mach)
    return FALSE;

  return TRUE;
}

/*
FUNCTION
	bfd_get_arch_info

SYNOPSIS
	const bfd_arch_info_type *bfd_get_arch_info (bfd *abfd);

DESCRIPTION
	Return the architecture info struct in @var{abfd}.
*/

const bfd_arch_info_type *
bfd_get_arch_info (bfd *abfd)
{
  return abfd->arch_info;
}

/*
FUNCTION
	bfd_lookup_arch
*/

const bfd_arch_info_type *
bfd_lookup_arch (enum bfd_architecture arch, uint32_t machine)
{
  const bfd_arch_info_type * const *app, *ap;

  for (app = bfd_archures_list; *app != NULL; app++)
    {
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  if (ap->arch == arch
	      && (ap->mach == machine
		  || (machine == 0 && ap->the_default)))
	    return ap;
	}
    }

  return NULL;
}

/*
FUNCTION
	bfd_printable_arch_mach
*/
const char *
bfd_printable_arch_mach (enum bfd_architecture arch, uint32_t machine)
{
  const bfd_arch_info_type *ap = bfd_lookup_arch (arch, machine);

  if (ap)
    return ap->printable_name;
  return "UNKNOWN!";
}

/*
FUNCTION
	bfd_octets_per_byte
*/

unsigned int
bfd_octets_per_byte (bfd *abfd)
{
  return bfd_arch_mach_octets_per_byte (bfd_get_arch (abfd),
					bfd_get_mach (abfd));
}

/*
FUNCTION
	bfd_arch_mach_octets_per_byte
*/

unsigned int
bfd_arch_mach_octets_per_byte (enum bfd_architecture arch,
			       uint32_t mach)
{
  const bfd_arch_info_type *ap = bfd_lookup_arch (arch, mach);

  if (ap)
    return ap->bits_per_byte / 8;
  return 1;
}

/*
INTERNAL_FUNCTION
	bfd_arch_default_fill
*/

void *
bfd_arch_default_fill (bfd_size_type count,
		       bfd_boolean is_bigendian ATTRIBUTE_UNUSED,
		       bfd_boolean code ATTRIBUTE_UNUSED)
{
  void *fill = bfd_malloc (count);
  if (fill != NULL)
    memset (fill, 0, count);
  return fill;
}

bfd_boolean
_bfd_nowrite_set_arch_mach (bfd *abfd,
			    enum bfd_architecture arch ATTRIBUTE_UNUSED,
			    uint32_t mach ATTRIBUTE_UNUSED)
{
  return _bfd_bool_bfd_false_error (abfd);
}
