/* simple-object.h -- simple routines to read and write object files
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

#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <stddef.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* This header file provides four types with associated functions.
   They are used to read and write object files.  This is a minimal
   interface, intended to support the needs of gcc without bringing in
   all the power and complexity of BFD.  */

/* The type simple_object_read * is used to read an existing object
   file.  */

typedef struct simple_object_read_struct simple_object_read;

/* The type simple_object_attributes holds the attributes of an object
   file that matter for creating a file or ensuring that two files are
   compatible.  This is a set of magic numbers.  */

typedef struct simple_object_attributes_struct simple_object_attributes;


/* The type simple_object_write is used to create a new object file.  */

typedef struct simple_object_write_struct simple_object_write;

/* The type simple_object_write_section is a handle for a section
   which is being written.  */

typedef struct simple_object_write_section_struct simple_object_write_section;

/* Add a section to SIMPLE_OBJECT.  NAME is the name of the new
   section.  ALIGN is the required alignment expressed as the number
   of required low-order 0 bits (e.g., 2 for alignment to a 32-bit
   boundary).  The section is created as containing data, readable,
   not writable, not executable, not loaded at runtime.  On error this
   returns NULL, sets *ERRMSG to an error message, and sets *ERR to an
   errno value or 0 if there isn't one.  */

extern simple_object_write_section *
simple_object_write_create_section (simple_object_write *simple_object,
				    const char *name, unsigned int align,
				    const char **errmsg, int *err);

/* Add data BUFFER/SIZE to SECTION in SIMPLE_OBJECT.  If COPY is
   non-zero, the data will be copied into memory if necessary.  If
   COPY is zero, BUFFER must persist until SIMPLE_OBJECT is released.
   On success this returns NULL.  On error this returns an error
   message, and sets *ERR to an errno value or 0 if there isn't
   one.  */

extern const char *
simple_object_write_add_data (simple_object_write *simple_object,
			      simple_object_write_section *section,
			      const void *buffer, size_t size,
			      int copy, int *err);


#ifdef __cplusplus
}
#endif

#endif
