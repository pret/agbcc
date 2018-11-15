/* simple-object.c -- simple routines to read and write object files.
   Copyright (C) 2010-2018 Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Google.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "libiberty.h"
#include "simple-object.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#include "simple-object-common.h"

/* The known object file formats.  */

static const struct simple_object_functions * const format_functions[] =
{
  &simple_object_elf_functions,
  &simple_object_mach_o_functions,
  &simple_object_coff_functions,
  &simple_object_xcoff_functions
};

/* Read data from a file using the simple_object error reporting
   conventions.  */

int
simple_object_internal_read (int descriptor, off_t offset,
			     unsigned char *buffer, size_t size,
			     const char **errmsg, int *err)
{
  if (lseek (descriptor, offset, SEEK_SET) < 0)
    {
      *errmsg = "lseek";
      *err = errno;
      return 0;
    }

  do
    {
      ssize_t got = read (descriptor, buffer, size);
      if (got == 0)
	break;
      else if (got > 0)
	{
	  buffer += got;
	  size -= got;
	}
      else if (errno != EINTR)
	{
	  *errmsg = "read";
	  *err = errno;
	  return 0;
	}
    }
  while (size > 0);

  if (size > 0)
    {
      *errmsg = "file too short";
      *err = 0;
      return 0;
    }

  return 1;
}

/* Write data to a file using the simple_object error reporting
   conventions.  */

int
simple_object_internal_write (int descriptor, off_t offset,
			      const unsigned char *buffer, size_t size,
			      const char **errmsg, int *err)
{
  if (lseek (descriptor, offset, SEEK_SET) < 0)
    {
      *errmsg = "lseek";
      *err = errno;
      return 0;
    }

  do
    {
      ssize_t wrote = write (descriptor, buffer, size);
      if (wrote == 0)
	break;
      else if (wrote > 0)
	{
	  buffer += wrote;
	  size -= wrote;
	}
      else if (errno != EINTR)
	{
	  *errmsg = "write";
	  *err = errno;
	  return 0;
	}
    }
  while (size > 0);

  if (size > 0)
    {
      *errmsg = "short write";
      *err = 0;
      return 0;
    }

  return 1;
}

/* Internal data passed to find_one_section.  */

struct find_one_section_data
{
  /* The section we are looking for.  */
  const char *name;
  /* Where to store the section offset.  */
  off_t *offset;
  /* Where to store the section length.  */
  off_t *length;
  /* Set if the name is found.  */
  int found;
};


/* Callback to identify and rename LTO debug sections by name.
   Returns non-NULL if NAME is a LTO debug section, NULL if not.
   If RENAME is true it will rename LTO debug sections to non-LTO
   ones.  */

static char *
handle_lto_debug_sections (const char *name, int rename)
{
  char *newname = rename ? XCNEWVEC (char, strlen (name) + 1)
	  	         : strdup (name);

  /* ???  So we can't use .gnu.lto_ prefixed sections as the assembler
     complains about bogus section flags.  Which means we need to arrange
     for that to be fixed or .gnu.debuglto_ marked as SHF_EXCLUDE (to make
     fat lto object tooling work for the fat part).  */
  /* Also include corresponding reloc sections.  */
  if (strncmp (name, ".rela", sizeof (".rela") - 1) == 0)
    {
      if (rename)
        strncpy (newname, name, sizeof (".rela") - 1);
      name += sizeof (".rela") - 1;
    }
  else if (strncmp (name, ".rel", sizeof (".rel") - 1) == 0)
    {
      if (rename)
        strncpy (newname, name, sizeof (".rel") - 1);
      name += sizeof (".rel") - 1;
    }
  /* ???  For now this handles both .gnu.lto_ and .gnu.debuglto_ prefixed
     sections.  */
  /* Copy LTO debug sections and rename them to their non-LTO name.  */
  if (strncmp (name, ".gnu.debuglto_", sizeof (".gnu.debuglto_") - 1) == 0)
    return rename ? strcat (newname, name + sizeof (".gnu.debuglto_") - 1) : newname;
  else if (strncmp (name, ".gnu.lto_.debug_",
		    sizeof (".gnu.lto_.debug_") -1) == 0)
    return rename ? strcat (newname, name + sizeof (".gnu.lto_") - 1) : newname;
  /* Copy over .note.GNU-stack section under the same name if present.  */
  else if (strcmp (name, ".note.GNU-stack") == 0)
    return strcpy (newname, name);
  /* Copy over .comment section under the same name if present.  Solaris
     ld uses them to relax its checking of ELF gABI access rules for
     COMDAT sections in objects produced by GCC.  */
  else if (strcmp (name, ".comment") == 0)
    return strcpy (newname, name);
  free (newname);
  return NULL;
}

/* Start creating a section.  */

simple_object_write_section *
simple_object_write_create_section (simple_object_write *sobj, const char *name,
				    unsigned int align,
				    const char **errmsg ATTRIBUTE_UNUSED,
				    int *err ATTRIBUTE_UNUSED)
{
  simple_object_write_section *ret;

  ret = XNEW (simple_object_write_section);
  ret->next = NULL;
  ret->name = strdup (name);
  ret->align = align;
  ret->buffers = NULL;
  ret->last_buffer = NULL;

  if (sobj->last_section == NULL)
    {
      sobj->sections = ret;
      sobj->last_section = ret;
    }
  else
    {
      sobj->last_section->next = ret;
      sobj->last_section = ret;
    }

  return ret;
}

/* Add data to a section.  */

const char *
simple_object_write_add_data (simple_object_write *sobj ATTRIBUTE_UNUSED,
			      simple_object_write_section *section,
			      const void *buffer,
			      size_t size, int copy,
			      int *err ATTRIBUTE_UNUSED)
{
  struct simple_object_write_section_buffer *wsb;

  wsb = XNEW (struct simple_object_write_section_buffer);
  wsb->next = NULL;
  wsb->size = size;

  if (!copy)
    {
      wsb->buffer = buffer;
      wsb->free_buffer = NULL;
    }
  else
    {
      wsb->free_buffer = (void *) XNEWVEC (char, size);
      memcpy (wsb->free_buffer, buffer, size);
      wsb->buffer = wsb->free_buffer;
    }

  if (section->last_buffer == NULL)
    {
      section->buffers = wsb;
      section->last_buffer = wsb;
    }
  else
    {
      section->last_buffer->next = wsb;
      section->last_buffer = wsb;
    }

  return NULL;
}


