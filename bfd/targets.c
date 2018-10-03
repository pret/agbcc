/* Generic target-file-type support for the BFD library.
   Copyright (C) 1990-2018 Free Software Foundation, Inc.
   Written by Cygnus Support.

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
#include "fnmatch.h"

/*
   It's okay to see some:
#if 0
   directives in this source file, as targets.c uses them to exclude
   certain BFD vectors.  This comment is specially formatted to catch
   users who grep for ^#if 0, so please keep it this way!
*/

/*
SECTION
	Targets
snip
*/

/*

INODE
	bfd_target,  , Targets, Targets
DOCDD
SUBSECTION
	bfd_target

*/

/* All known xvecs (even those that don't compile on all systems).
   Alphabetized for easy reference.
   They are listed a second time below, since
   we can't intermix extern's and initializers.  */

extern const bfd_target arm_elf32_le_vec;
extern const bfd_target arm_elf32_fdpic_le_vec;
extern const bfd_target elf32_le_vec;

/* These are always included.  */
extern const bfd_target binary_vec;


static const bfd_target * const _bfd_target_vector[] =
{

	SELECT_VECS,

	&binary_vec,
	NULL /* end of list marker */
};
const bfd_target * const *bfd_target_vector = _bfd_target_vector;

/* bfd_default_vector[0] contains either the address of the default vector,
   if there is one, or zero if there isn't.  */

const bfd_target *bfd_default_vector[] = {
#ifdef DEFAULT_VECTOR
	&DEFAULT_VECTOR,
#endif
	NULL
};

/* bfd_associated_vector[] contains the associated target vectors used
   to reduce the ambiguity in bfd_check_format_matches.  */

static const bfd_target *_bfd_associated_vector[] = {
#ifdef ASSOCIATED_VECS
	ASSOCIATED_VECS,
#endif
	NULL
};
const bfd_target * const *bfd_associated_vector = _bfd_associated_vector;

/* When there is an ambiguous match, bfd_check_format_matches puts the
   names of the matching targets in an array.  This variable is the maximum
   number of entries that the array could possibly need.  */
const size_t _bfd_target_vector_entries = sizeof (_bfd_target_vector)/sizeof (*_bfd_target_vector);

/* This array maps configuration triplets onto BFD vectors.  */

struct targmatch
{
  /* The configuration triplet.  */
  const char *triplet;
  /* The BFD vector.  If this is NULL, then the vector is found by
     searching forward for the next structure with a non NULL vector
     field.  */
  const bfd_target *vector;
};

/* targmatch.h is built by Makefile out of config.bfd.  */
static const struct targmatch bfd_target_match[] = {
#include "targmatch.h"
  { NULL, NULL }
};

/* Find a target vector, given a name or configuration triplet.  */

static const bfd_target *
find_target (const char *name)
{
  const bfd_target * const *target;
  const struct targmatch *match;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    if (strcmp (name, (*target)->name) == 0)
      return *target;

  /* If we couldn't match on the exact name, try matching on the
     configuration triplet.  FIXME: We should run the triplet through
     config.sub first, but that is hard.  */
  for (match = &bfd_target_match[0]; match->triplet != NULL; match++)
    {
      if (fnmatch (match->triplet, name, 0) == 0)
	{
	  while (match->vector == NULL)
	    ++match;
	  return match->vector;
	}
    }

  bfd_set_error (bfd_error_invalid_target);
  return NULL;
}

/*
FUNCTION
	bfd_set_default_target

SYNOPSIS
	bfd_boolean bfd_set_default_target (const char *name);

DESCRIPTION
	Set the default target vector to use when recognizing a BFD.
	This takes the name of the target, which may be a BFD target
	name or a configuration triplet.
*/

bfd_boolean
bfd_set_default_target (const char *name)
{
  const bfd_target *target;

  if (bfd_default_vector[0] != NULL
      && strcmp (name, bfd_default_vector[0]->name) == 0)
    return TRUE;

  target = find_target (name);
  if (target == NULL)
    return FALSE;

  bfd_default_vector[0] = target;
  return TRUE;
}

/*
FUNCTION
	bfd_find_target

SYNOPSIS
	const bfd_target *bfd_find_target (const char *target_name, bfd *abfd);

DESCRIPTION
	Return a pointer to the transfer vector for the object target
	named @var{target_name}. */

const bfd_target *
bfd_find_target (const char *target_name, bfd *abfd)
{
  const char *targname;
  const bfd_target *target;

  if (target_name != NULL)
    targname = target_name;
  else
    targname = getenv ("GNUTARGET");

  /* This is safe; the vector cannot be null.  */
  if (targname == NULL || strcmp (targname, "default") == 0)
    {
      if (bfd_default_vector[0] != NULL)
	target = bfd_default_vector[0];
      else
	target = bfd_target_vector[0];
      if (abfd)
	{
	  abfd->xvec = target;
	  abfd->target_defaulted = TRUE;
	}
      return target;
    }

  if (abfd)
    abfd->target_defaulted = FALSE;

  target = find_target (targname);
  if (target == NULL)
    return NULL;

  if (abfd)
    abfd->xvec = target;
  return target;
}

/* Helper function for bfd_get_target_info to determine the target's
   architecture.  This method handles bfd internal target names as
   tuples and triplets.  */
static bfd_boolean
_bfd_find_arch_match (const char *tname, const char **arch,
		      const char **def_target_arch)
{
  if (!arch)
    return FALSE;

  while (*arch != NULL)
    {
      const char *in_a = strstr (*arch, tname);
      char end_ch = (in_a ? in_a[strlen (tname)] : 0);

      if (in_a && (in_a == *arch || in_a[-1] == ':')
	  && end_ch == 0)
	{
	  *def_target_arch = *arch;
	  return TRUE;
	}
      arch++;
    }
  return FALSE;
}

/*
FUNCTION
	bfd_get_target_info

DESCRIPTION
	Return a pointer to the transfer vector for the object target
	named @var{target_name}. */
const bfd_target *
bfd_get_target_info (const char *target_name, bfd *abfd,
		     bfd_boolean *is_bigendian,
		     int *underscoring, const char **def_target_arch)
{
  const bfd_target *target_vec;

  if (is_bigendian)
    *is_bigendian = FALSE;
  if (underscoring)
    *underscoring = -1;
  if (def_target_arch)
    *def_target_arch = NULL;
  target_vec = bfd_find_target (target_name, abfd);
  if (! target_vec)
    return NULL;
  if (is_bigendian)
    *is_bigendian = ((target_vec->byteorder == BFD_ENDIAN_BIG) ? TRUE
							       : FALSE);
  if (underscoring)
    *underscoring = ((int) target_vec->symbol_leading_char) & 0xff;

  if (def_target_arch)
    {
      const char *tname = target_vec->name;
      const char **arches = bfd_arch_list ();

      if (arches && tname)
	{
	  char *hyp = strchr (tname, '-');

	  if (hyp != NULL)
	    {
	      tname = ++hyp;

	      /* Make sure we detect architecture names
		 for triplets like "pe-arm-wince-little".  */
	      if (!_bfd_find_arch_match (tname, arches, def_target_arch))
		{
		  char new_tname[50];

		  strcpy (new_tname, hyp);
		  while ((hyp = strrchr (new_tname, '-')) != NULL)
		    {
		      *hyp = 0;
		      if (_bfd_find_arch_match (new_tname, arches,
						def_target_arch))
			break;
		    }
		}
	    }
	  else
	    _bfd_find_arch_match (tname, arches, def_target_arch);
	}

      if (arches)
	free (arches);
    }
  return target_vec;
}

/*
FUNCTION
	bfd_target_list

DESCRIPTION
	Return a freshly malloced NULL-terminated
	vector of the names of all the valid BFD targets. Do not
	modify the names.

*/

const char **
bfd_target_list (void)
{
  int vec_length = 0;
  bfd_size_type amt;
  const bfd_target * const *target;
  const  char **name_list, **name_ptr;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    vec_length++;

  amt = (vec_length + 1) * sizeof (char **);
  name_ptr = name_list = (const  char **) bfd_malloc (amt);

  if (name_list == NULL)
    return NULL;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    if (target == &bfd_target_vector[0]
	|| *target != bfd_target_vector[0])
      *name_ptr++ = (*target)->name;

  *name_ptr = NULL;
  return name_list;
}

/*
FUNCTION
	bfd_iterate_over_targets
*/

const bfd_target *
bfd_iterate_over_targets (int (*func) (const bfd_target *, void *),
			  void *data)
{
  const bfd_target *const *target;

  for (target = bfd_target_vector; *target != NULL; ++target)
    if (func (*target, data))
      return *target;

  return NULL;
}

/*
FUNCTION
	bfd_flavour_name
*/

const char *
bfd_flavour_name (enum bfd_flavour flavour)
{
  switch (flavour)
    {
    case bfd_target_elf_flavour: return "ELF";
    default: return "unknown file format";
    }

  abort ();
}
