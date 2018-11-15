/* Generic BFD library interface and support routines.
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
#include <stdarg.h>
#include "bfd.h"
#include "bfdver.h"
#include "libiberty.h"
#include "safe-ctype.h"
#include "bfdlink.h"
#include "libbfd.h"
#undef obj_symbols
#include "elf-bfd.h"

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif


/* provide storage for subsystem, stack and heap data which may have been
   passed in on the command line.  Ld puts this data into a bfd_link_info
   struct which ultimately gets passed in to the bfd.  When it arrives, copy
   it to the following struct so that the data will be available in coffcode.h
   where it is needed.  The typedef's used are defined in bfd.h */

/*
INODE
Error reporting, Miscellaneous, typedef bfd, BFD front end

SECTION
	Error reporting

	Most BFD functions return nonzero on success (check their
	individual documentation for precise semantics).  On an error,
	they call <<bfd_set_error>> to set an error condition that callers
	can check by calling <<bfd_get_error>>.
	If that returns <<bfd_error_system_call>>, then check
	<<errno>>.

	The easiest way to report a BFD error to the user is to
	use <<bfd_perror>>.

SUBSECTION
	Type <<bfd_error_type>>

	The values returned by <<bfd_get_error>> are defined by the
	enumerated type <<bfd_error_type>>.

CODE_FRAGMENT
.
.typedef enum bfd_error
.{
.  bfd_error_no_error = 0,
.  bfd_error_system_call,
.  bfd_error_invalid_target,
.  bfd_error_wrong_format,
.  bfd_error_wrong_object_format,
.  bfd_error_invalid_operation,
.  bfd_error_no_memory,
.  bfd_error_no_symbols,
.  bfd_error_no_armap,
.  bfd_error_no_more_archived_files,
.  bfd_error_malformed_archive,
.  bfd_error_missing_dso,
.  bfd_error_file_not_recognized,
.  bfd_error_file_ambiguously_recognized,
.  bfd_error_no_contents,
.  bfd_error_nonrepresentable_section,
.  bfd_error_no_debug_section,
.  bfd_error_bad_value,
.  bfd_error_file_truncated,
.  bfd_error_file_too_big,
.  bfd_error_on_input,
.  bfd_error_invalid_error_code
.}
.bfd_error_type;
.
*/

static bfd_error_type bfd_error = bfd_error_no_error;
static bfd *input_bfd = NULL;
static bfd_error_type input_error = bfd_error_no_error;

const char *const bfd_errmsgs[] =
{
  N_("no error"),
  N_("system call error"),
  N_("invalid bfd target"),
  N_("file in wrong format"),
  N_("archive object file in wrong format"),
  N_("invalid operation"),
  N_("memory exhausted"),
  N_("no symbols"),
  N_("archive has no index; run ranlib to add one"),
  N_("no more archived files"),
  N_("malformed archive"),
  N_("DSO missing from command line"),
  N_("file format not recognized"),
  N_("file format is ambiguous"),
  N_("section has no contents"),
  N_("nonrepresentable section on output"),
  N_("symbol needs debug section which does not exist"),
  N_("bad value"),
  N_("file truncated"),
  N_("file too big"),
  N_("error reading %s: %s"),
  N_("#<invalid error code>")
};

/*
FUNCTION
	bfd_get_error

SYNOPSIS
	bfd_error_type bfd_get_error (void);

DESCRIPTION
	Return the current BFD error condition.
*/

bfd_error_type
bfd_get_error (void)
{
  return bfd_error;
}

/*
FUNCTION
	bfd_set_error

SYNOPSIS
	void bfd_set_error (bfd_error_type error_tag);

DESCRIPTION
	Set the BFD error condition to be @var{error_tag}.

	@var{error_tag} must not be bfd_error_on_input.  Use
	bfd_set_input_error for input errors instead.
*/

void
bfd_set_error (bfd_error_type error_tag)
{
  bfd_error = error_tag;
  if (bfd_error >= bfd_error_on_input)
    abort ();
}

/*
FUNCTION
	bfd_set_input_error

SYNOPSIS
	void bfd_set_input_error (bfd *input, bfd_error_type error_tag);

DESCRIPTION

	Set the BFD error condition to be bfd_error_on_input.
	@var{input} is the input bfd where the error occurred, and
	@var{error_tag} the bfd_error_type error.
*/

void
bfd_set_input_error (bfd *input, bfd_error_type error_tag)
{
  /* This is an error that occurred during bfd_close when writing an
     archive, but on one of the input files.  */
  bfd_error = bfd_error_on_input;
  input_bfd = input;
  input_error = error_tag;
  if (input_error >= bfd_error_on_input)
    abort ();
}

/*
FUNCTION
	bfd_errmsg

SYNOPSIS
	const char *bfd_errmsg (bfd_error_type error_tag);

DESCRIPTION
	Return a string describing the error @var{error_tag}, or
	the system error if @var{error_tag} is <<bfd_error_system_call>>.
*/

const char *
bfd_errmsg (bfd_error_type error_tag)
{
#ifndef errno
  extern int errno;
#endif
  if (error_tag == bfd_error_on_input)
    {
      char *buf;
      const char *msg = bfd_errmsg (input_error);

      if (asprintf (&buf, _(bfd_errmsgs [error_tag]), input_bfd->filename, msg)
	  != -1)
	return buf;

      /* Ick, what to do on out of memory?  */
      return msg;
    }

  if (error_tag == bfd_error_system_call)
    return xstrerror (errno);

  if (error_tag > bfd_error_invalid_error_code)
    error_tag = bfd_error_invalid_error_code;	/* sanity check */

  return _(bfd_errmsgs [error_tag]);
}

/*
FUNCTION
	bfd_perror

SYNOPSIS
	void bfd_perror (const char *message);

DESCRIPTION
	Print to the standard error stream a string describing the
	last BFD error that occurred, or the last system error if
	the last BFD error was a system call failure.  If @var{message}
	is non-NULL and non-empty, the error string printed is preceded
	by @var{message}, a colon, and a space.  It is followed by a newline.
*/

void
bfd_perror (const char *message)
{
  fflush (stdout);
  if (message == NULL || *message == '\0')
    fprintf (stderr, "%s\n", bfd_errmsg (bfd_get_error ()));
  else
    fprintf (stderr, "%s: %s\n", message, bfd_errmsg (bfd_get_error ()));
  fflush (stderr);
}

/*
SUBSECTION
	BFD error handler

	Some BFD functions want to print messages describing the
	problem.  They call a BFD error handler function.  This
	function may be overridden by the program.

	The BFD error handler acts like vprintf.

CODE_FRAGMENT
.
.typedef void (*bfd_error_handler_type) (const char *, va_list);
.
*/

/* The program name used when printing BFD error messages.  */

static const char *_bfd_error_program_name;

/* Support for positional parameters.  */

union _bfd_doprnt_args
{
  int i;
  long l;
  long long ll;
  double d;
  long double ld;
  void *p;
  enum
  {
    Bad,
    Int,
    Long,
    LongLong,
    Double,
    LongDouble,
    Ptr
  } type;
};

/* This macro and _bfd_doprnt taken from libiberty _doprnt.c, tidied a
   little and extended to handle '%pA', '%pB' and positional parameters.  */

#define PRINT_TYPE(TYPE, FIELD) \
  do								\
    {								\
      TYPE value = (TYPE) args[arg_no].FIELD;			\
      result = fprintf (stream, specifier, value);		\
    } while (0)

static int
_bfd_doprnt (FILE *stream, const char *format, union _bfd_doprnt_args *args)
{
  const char *ptr = format;
  char specifier[128];
  int total_printed = 0;
  unsigned int arg_count = 0;

  while (*ptr != '\0')
    {
      int result;

      if (*ptr != '%')
	{
	  /* While we have regular characters, print them.  */
	  char *end = strchr (ptr, '%');
	  if (end != NULL)
	    result = fprintf (stream, "%.*s", (int) (end - ptr), ptr);
	  else
	    result = fprintf (stream, "%s", ptr);
	  ptr += result;
	}
      else if (ptr[1] == '%')
	{
	  fputc ('%', stream);
	  result = 1;
	  ptr += 2;
	}
      else
	{
	  /* We have a format specifier!  */
	  char *sptr = specifier;
	  int wide_width = 0, short_width = 0;
	  unsigned int arg_no;

	  /* Copy the % and move forward.  */
	  *sptr++ = *ptr++;

	  /* Check for a positional parameter.  */
	  arg_no = -1u;
	  if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
	    {
	      arg_no = *ptr - '1';
	      ptr += 2;
	    }

	  /* Move past flags.  */
	  while (strchr ("-+ #0'I", *ptr))
	    *sptr++ = *ptr++;

	  if (*ptr == '*')
	    {
	      int value;
	      unsigned int arg_index;

	      ptr++;
	      arg_index = arg_count;
	      if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
		{
		  arg_index = *ptr - '1';
		  ptr += 2;
		}
	      value = abs (args[arg_index].i);
	      arg_count++;
	      sptr += sprintf (sptr, "%d", value);
	    }
	  else
	    /* Handle explicit numeric value.  */
	    while (ISDIGIT (*ptr))
	      *sptr++ = *ptr++;

	  /* Precision.  */
	  if (*ptr == '.')
	    {
	      /* Copy and go past the period.  */
	      *sptr++ = *ptr++;
	      if (*ptr == '*')
		{
		  int value;
		  unsigned int arg_index;

		  ptr++;
		  arg_index = arg_count;
		  if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
		    {
		      arg_index = *ptr - '1';
		      ptr += 2;
		    }
		  value = abs (args[arg_index].i);
		  arg_count++;
		  sptr += sprintf (sptr, "%d", value);
		}
	      else
		/* Handle explicit numeric value.  */
		while (ISDIGIT (*ptr))
		  *sptr++ = *ptr++;
	    }
	  while (strchr ("hlL", *ptr))
	    {
	      switch (*ptr)
		{
		case 'h':
		  short_width = 1;
		  break;
		case 'l':
		  wide_width++;
		  break;
		case 'L':
		  wide_width = 2;
		  break;
		default:
		  abort();
		}
	      *sptr++ = *ptr++;
	    }

	  /* Copy the type specifier, and NULL terminate.  */
	  *sptr++ = *ptr++;
	  *sptr = '\0';
	  if ((int) arg_no < 0)
	    arg_no = arg_count;

	  switch (ptr[-1])
	    {
	    case 'd':
	    case 'i':
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
	    case 'c':
	      {
		/* Short values are promoted to int, so just copy it
		   as an int and trust the C library printf to cast it
		   to the right width.  */
		if (short_width)
		  PRINT_TYPE (int, i);
		else
		  {
		    switch (wide_width)
		      {
		      case 0:
			PRINT_TYPE (int, i);
			break;
		      case 1:
			PRINT_TYPE (long, l);
			break;
		      case 2:
		      default:
#if defined (__MSVCRT__)
			sptr[-3] = 'I';
			sptr[-2] = '6';
			sptr[-1] = '4';
			*sptr++ = ptr[-1];
			*sptr = '\0';
#endif
			PRINT_TYPE (long long, ll);
			break;
		      }
		  }
	      }
	      break;
	    case 'f':
	    case 'e':
	    case 'E':
	    case 'g':
	    case 'G':
	      {
		if (wide_width == 0)
		  PRINT_TYPE (double, d);
		else
		  {
#if defined (__GNUC__) || defined (HAVE_LONG_DOUBLE)
		    PRINT_TYPE (long double, ld);
#else
		    /* Fake it and hope for the best.  */
		    PRINT_TYPE (double, d);
#endif
		  }
	      }
	      break;
	    case 's':
	      PRINT_TYPE (char *, p);
	      break;
	    case 'p':
	      if (*ptr == 'A')
		{
		  asection *sec;
		  bfd *abfd;
		  const char *group = NULL;

		  ptr++;
		  sec = (asection *) args[arg_no].p;
		  if (sec == NULL)
		    /* Invoking %pA with a null section pointer is an
		       internal error.  */
		    abort ();
		  abfd = sec->owner;
		  if (abfd != NULL
		      && bfd_get_flavour (abfd) == bfd_target_elf_flavour
		      && elf_next_in_group (sec) != NULL
		      && (sec->flags & SEC_GROUP) == 0)
		    group = elf_group_name (sec);

		  if (group != NULL)
		    result = fprintf (stream, "%s[%s]", sec->name, group);
		  else
		    result = fprintf (stream, "%s", sec->name);
		}
	      else if (*ptr == 'B')
		{
		  bfd *abfd;

		  ptr++;
		  abfd = (bfd *) args[arg_no].p;
		  if (abfd == NULL)
		    /* Invoking %pB with a null bfd pointer is an
		       internal error.  */
		    abort ();
		  else if (abfd->my_archive
			   && !bfd_is_thin_archive (abfd->my_archive))
		    result = fprintf (stream, "%s(%s)",
				      abfd->my_archive->filename,
				      abfd->filename);
		  else
		    result = fprintf (stream, "%s", abfd->filename);
		}
	      else
		PRINT_TYPE (void *, p);
	      break;
	    default:
	      abort();
	    }
	  arg_count++;
	}
      if (result == -1)
	return -1;
      total_printed += result;
    }

  return total_printed;
}

/* First pass over FORMAT to gather ARGS.  Returns number of args.  */

static unsigned int
_bfd_doprnt_scan (const char *format, union _bfd_doprnt_args *args)
{
  const char *ptr = format;
  unsigned int arg_count = 0;

  while (*ptr != '\0')
    {
      if (*ptr != '%')
	{
	  ptr = strchr (ptr, '%');
	  if (ptr == NULL)
	    break;
	}
      else if (ptr[1] == '%')
	ptr += 2;
      else
	{
	  int wide_width = 0, short_width = 0;
	  unsigned int arg_no;
	  int arg_type;

	  ptr++;

	  /* Check for a positional parameter.  */
	  arg_no = -1u;
	  if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
	    {
	      arg_no = *ptr - '1';
	      ptr += 2;
	    }

	  /* Move past flags.  */
	  while (strchr ("-+ #0'I", *ptr))
	    ptr++;

	  if (*ptr == '*')
	    {
	      unsigned int arg_index;

	      ptr++;
	      arg_index = arg_count;
	      if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
		{
		  arg_index = *ptr - '1';
		  ptr += 2;
		}
	      if (arg_index >= 9)
		abort ();
	      args[arg_index].type = Int;
	      arg_count++;
	    }
	  else
	    /* Handle explicit numeric value.  */
	    while (ISDIGIT (*ptr))
	      ptr++;

	  /* Precision.  */
	  if (*ptr == '.')
	    {
	      ptr++;
	      if (*ptr == '*')
		{
		  unsigned int arg_index;

		  ptr++;
		  arg_index = arg_count;
		  if (*ptr != '0' && ISDIGIT (*ptr) && ptr[1] == '$')
		    {
		      arg_index = *ptr - '1';
		      ptr += 2;
		    }
		  if (arg_index >= 9)
		    abort ();
		  args[arg_index].type = Int;
		  arg_count++;
		}
	      else
		/* Handle explicit numeric value.  */
		while (ISDIGIT (*ptr))
		  ptr++;
	    }
	  while (strchr ("hlL", *ptr))
	    {
	      switch (*ptr)
		{
		case 'h':
		  short_width = 1;
		  break;
		case 'l':
		  wide_width++;
		  break;
		case 'L':
		  wide_width = 2;
		  break;
		default:
		  abort();
		}
	      ptr++;
	    }

	  ptr++;
	  if ((int) arg_no < 0)
	    arg_no = arg_count;

	  arg_type = Bad;
	  switch (ptr[-1])
	    {
	    case 'd':
	    case 'i':
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
	    case 'c':
	      {
		if (short_width)
		  arg_type = Int;
		else
		  {
		    switch (wide_width)
		      {
		      case 0:
			arg_type = Int;
			break;
		      case 1:
			arg_type = Long;
			break;
		      case 2:
		      default:
			arg_type = LongLong;
			break;
		      }
		  }
	      }
	      break;
	    case 'f':
	    case 'e':
	    case 'E':
	    case 'g':
	    case 'G':
	      {
		if (wide_width == 0)
		  arg_type = Double;
		else
		  {
#if defined (__GNUC__) || defined (HAVE_LONG_DOUBLE)
		    arg_type = LongDouble;
#else
		    arg_type = Double;
#endif
		  }
	      }
	      break;
	    case 's':
	      arg_type = Ptr;
	      break;
	    case 'p':
	      if (*ptr == 'A' || *ptr == 'B')
		ptr++;
	      arg_type = Ptr;
	      break;
	    default:
	      abort();
	    }

	  if (arg_no >= 9)
	    abort ();
	  args[arg_no].type = arg_type;
	  arg_count++;
	}
    }

  return arg_count;
}

static void
error_handler_internal (const char *fmt, va_list ap)
{
  unsigned int i, arg_count;
  union _bfd_doprnt_args args[9];

  for (i = 0; i < sizeof (args) / sizeof (args[0]); i++)
    args[i].type = Bad;

  arg_count = _bfd_doprnt_scan (fmt, args);
  for (i = 0; i < arg_count; i++)
    {
      switch (args[i].type)
	{
	case Int:
	  args[i].i = va_arg (ap, int);
	  break;
	case Long:
	  args[i].l = va_arg (ap, long);
	  break;
	case LongLong:
	  args[i].ll = va_arg (ap, long long);
	  break;
	case Double:
	  args[i].d = va_arg (ap, double);
	  break;
	case LongDouble:
	  args[i].ld = va_arg (ap, long double);
	  break;
	case Ptr:
	  args[i].p = va_arg (ap, void *);
	  break;
	default:
	  abort ();
	}
    }

  /* PR 4992: Don't interrupt output being sent to stdout.  */
  fflush (stdout);

  if (_bfd_error_program_name != NULL)
    fprintf (stderr, "%s: ", _bfd_error_program_name);
  else
    fprintf (stderr, "BFD: ");

  _bfd_doprnt (stderr, fmt, args);

  /* On AIX, putc is implemented as a macro that triggers a -Wunused-value
     warning, so use the fputc function to avoid it.  */
  fputc ('\n', stderr);
  fflush (stderr);
}

/* This is a function pointer to the routine which should handle BFD
   error messages.  It is called when a BFD routine encounters an
   error for which it wants to print a message.  Going through a
   function pointer permits a program linked against BFD to intercept
   the messages and deal with them itself.  */

static bfd_error_handler_type _bfd_error_internal = error_handler_internal;

/*
FUNCTION
	_bfd_error_handler

SYNOPSIS
	void _bfd_error_handler (const char *fmt, ...) ATTRIBUTE_PRINTF_1;

DESCRIPTION
	This is the default routine to handle BFD error messages.
	Like fprintf (stderr, ...), but also handles some extra format
	specifiers.

	%pA section name from section.  For group components, prints
	group name too.
	%pB file name from bfd.  For archive components, prints
	archive too.

	Beware: Only supports a maximum of 9 format arguments.
*/

void
_bfd_error_handler (const char *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  _bfd_error_internal (fmt, ap);
  va_end (ap);
}

/*
FUNCTION
	bfd_set_error_handler

SYNOPSIS
	bfd_error_handler_type bfd_set_error_handler (bfd_error_handler_type);

DESCRIPTION
	Set the BFD error handler function.  Returns the previous
	function.
*/

bfd_error_handler_type
bfd_set_error_handler (bfd_error_handler_type pnew)
{
  bfd_error_handler_type pold;

  pold = _bfd_error_internal;
  _bfd_error_internal = pnew;
  return pold;
}

/*
FUNCTION
	bfd_set_error_program_name

SYNOPSIS
	void bfd_set_error_program_name (const char *);

DESCRIPTION
	Set the program name to use when printing a BFD error.  This
	is printed before the error message followed by a colon and
	space.  The string must not be changed after it is passed to
	this function.
*/

void
bfd_set_error_program_name (const char *name)
{
  _bfd_error_program_name = name;
}

/*
SUBSECTION
	BFD assert handler

	If BFD finds an internal inconsistency, the bfd assert
	handler is called with information on the BFD version, BFD
	source file and line.  If this happens, most programs linked
	against BFD are expected to want to exit with an error, or mark
	the current BFD operation as failed, so it is recommended to
	override the default handler, which just calls
	_bfd_error_handler and continues.

CODE_FRAGMENT
.
.typedef void (*bfd_assert_handler_type) (const char *bfd_formatmsg,
.					  const char *bfd_version,
.					  const char *bfd_file,
.					  int bfd_line);
.
*/

/* Note the use of bfd_ prefix on the parameter names above: we want to
   show which one is the message and which is the version by naming the
   parameters, but avoid polluting the program-using-bfd namespace as
   the typedef is visible in the exported headers that the program
   includes.  Below, it's just for consistency.  */

static void
_bfd_default_assert_handler (const char *bfd_formatmsg,
			     const char *bfd_version,
			     const char *bfd_file,
			     int bfd_line)

{
  _bfd_error_handler (bfd_formatmsg, bfd_version, bfd_file, bfd_line);
}

/* Similar to _bfd_error_handler, a program can decide to exit on an
   internal BFD error.  We use a non-variadic type to simplify passing
   on parameters to other functions, e.g. _bfd_error_handler.  */

static bfd_assert_handler_type _bfd_assert_handler = _bfd_default_assert_handler;

/*
FUNCTION
	bfd_set_assert_handler

SYNOPSIS
	bfd_assert_handler_type bfd_set_assert_handler (bfd_assert_handler_type);

DESCRIPTION
	Set the BFD assert handler function.  Returns the previous
	function.
*/

bfd_assert_handler_type
bfd_set_assert_handler (bfd_assert_handler_type pnew)
{
  bfd_assert_handler_type pold;

  pold = _bfd_assert_handler;
  _bfd_assert_handler = pnew;
  return pold;
}

/*
INODE
Miscellaneous, Memory Usage, Error reporting, BFD front end

SECTION
	Miscellaneous

SUBSECTION
	Miscellaneous functions
*/

/*
FUNCTION
	bfd_get_reloc_upper_bound

SYNOPSIS
	long bfd_get_reloc_upper_bound (bfd *abfd, asection *sect);

DESCRIPTION
	Return the number of bytes required to store the
	relocation information associated with section @var{sect}
	attached to bfd @var{abfd}.  If an error occurs, return -1.

*/

long
bfd_get_reloc_upper_bound (bfd *abfd, sec_ptr asect)
{
  if (abfd->format != bfd_object)
    {
      bfd_set_error (bfd_error_invalid_operation);
      return -1;
    }

  return BFD_SEND (abfd, _get_reloc_upper_bound, (abfd, asect));
}

/*
FUNCTION
	bfd_canonicalize_reloc

SYNOPSIS
	long bfd_canonicalize_reloc
	  (bfd *abfd, asection *sec, arelent **loc, asymbol **syms);

DESCRIPTION
	Call the back end associated with the open BFD
	@var{abfd} and translate the external form of the relocation
	information attached to @var{sec} into the internal canonical
	form.  Place the table into memory at @var{loc}, which has
	been preallocated, usually by a call to
	<<bfd_get_reloc_upper_bound>>.  Returns the number of relocs, or
	-1 on error.

	The @var{syms} table is also needed for horrible internal magic
	reasons.

*/
long
bfd_canonicalize_reloc (bfd *abfd,
			sec_ptr asect,
			arelent **location,
			asymbol **symbols)
{
  if (abfd->format != bfd_object)
    {
      bfd_set_error (bfd_error_invalid_operation);
      return -1;
    }

  return BFD_SEND (abfd, _bfd_canonicalize_reloc,
		   (abfd, asect, location, symbols));
}

/*
FUNCTION
	bfd_set_reloc

SYNOPSIS
	void bfd_set_reloc
	  (bfd *abfd, asection *sec, arelent **rel, unsigned int count);

DESCRIPTION
	Set the relocation pointer and count within
	section @var{sec} to the values @var{rel} and @var{count}.
	The argument @var{abfd} is ignored.

.#define bfd_set_reloc(abfd, asect, location, count) \
.	BFD_SEND (abfd, _bfd_set_reloc, (abfd, asect, location, count))
*/

/*
FUNCTION
	bfd_set_file_flags

SYNOPSIS
	bfd_boolean bfd_set_file_flags (bfd *abfd, flagword flags);

DESCRIPTION
	Set the flag word in the BFD @var{abfd} to the value @var{flags}.

	Possible errors are:
	o <<bfd_error_wrong_format>> - The target bfd was not of object format.
	o <<bfd_error_invalid_operation>> - The target bfd was open for reading.
	o <<bfd_error_invalid_operation>> -
	The flag word contained a bit which was not applicable to the
	type of file.  E.g., an attempt was made to set the <<D_PAGED>> bit
	on a BFD format which does not support demand paging.

*/

bfd_boolean
bfd_set_file_flags (bfd *abfd, flagword flags)
{
  if (abfd->format != bfd_object)
    {
      bfd_set_error (bfd_error_wrong_format);
      return FALSE;
    }

  if (bfd_read_p (abfd))
    {
      bfd_set_error (bfd_error_invalid_operation);
      return FALSE;
    }

  bfd_get_file_flags (abfd) = flags;
  if ((flags & bfd_applicable_file_flags (abfd)) != flags)
    {
      bfd_set_error (bfd_error_invalid_operation);
      return FALSE;
    }

  return TRUE;
}

void
bfd_assert (const char *file, int line)
{
  /* xgettext:c-format */
  (*_bfd_assert_handler) (_("BFD %s assertion fail %s:%d"),
			  BFD_VERSION_STRING, file, line);
}

/* A more or less friendly abort message.  In libbfd.h abort is
   defined to call this function.  */

void
_bfd_abort (const char *file, int line, const char *fn)
{
  if (fn != NULL)
    _bfd_error_handler
      /* xgettext:c-format */
      (_("BFD %s internal error, aborting at %s:%d in %s\n"),
       BFD_VERSION_STRING, file, line, fn);
  else
    _bfd_error_handler
      /* xgettext:c-format */
      (_("BFD %s internal error, aborting at %s:%d\n"),
       BFD_VERSION_STRING, file, line);
  _bfd_error_handler (_("Please report this bug.\n"));
  _exit (EXIT_FAILURE);
}

/*
FUNCTION
	bfd_get_arch_size

SYNOPSIS
	int bfd_get_arch_size (bfd *abfd);

DESCRIPTION
	Returns the normalized architecture address size, in bits, as
	determined by the object file's format.  By normalized, we mean
	either 32 or 64.  For ELF, this information is included in the
	header.  Use bfd_arch_bits_per_address for number of bits in
	the architecture address.

RETURNS
	Returns the arch size in bits if known, <<-1>> otherwise.
*/

int
bfd_get_arch_size (bfd *abfd)
{
  if (abfd->xvec->flavour == bfd_target_elf_flavour)
    return get_elf_backend_data (abfd)->s->arch_size;

  return bfd_arch_bits_per_address (abfd) > 32 ? 64 : 32;
}

/*
FUNCTION
	bfd_get_sign_extend_vma

SYNOPSIS
	int bfd_get_sign_extend_vma (bfd *abfd);

DESCRIPTION
	Indicates if the target architecture "naturally" sign extends
	an address.  Some architectures implicitly sign extend address
	values when they are converted to types larger than the size
	of an address.  For instance, bfd_get_start_address() will
	return an address sign extended to fill a bfd_vma when this is
	the case.

RETURNS
	Returns <<1>> if the target architecture is known to sign
	extend addresses, <<0>> if the target architecture is known to
	not sign extend addresses, and <<-1>> otherwise.
*/

int
bfd_get_sign_extend_vma (bfd *abfd)
{
  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
    return get_elf_backend_data (abfd)->sign_extend_vma;

  bfd_set_error (bfd_error_wrong_format);
  return -1;
}

/*
FUNCTION
	bfd_set_start_address

SYNOPSIS
	bfd_boolean bfd_set_start_address (bfd *abfd, bfd_vma vma);

DESCRIPTION
	Make @var{vma} the entry point of output BFD @var{abfd}.

RETURNS
	Returns <<TRUE>> on success, <<FALSE>> otherwise.
*/

bfd_boolean
bfd_set_start_address (bfd *abfd, bfd_vma vma)
{
  abfd->start_address = vma;
  return TRUE;
}

/*
FUNCTION
	bfd_get_gp_size

SYNOPSIS
	unsigned int bfd_get_gp_size (bfd *abfd);

DESCRIPTION
	Return the maximum size of objects to be optimized using the GP
	register under MIPS ECOFF.  This is typically set by the <<-G>>
	argument to the compiler, assembler or linker.
*/

unsigned int
bfd_get_gp_size (bfd *abfd)
{
  if (abfd->format == bfd_object)
    {
      if (abfd->xvec->flavour == bfd_target_elf_flavour)
	return elf_gp_size (abfd);
    }
  return 0;
}

/*
FUNCTION
	bfd_set_gp_size

SYNOPSIS
	void bfd_set_gp_size (bfd *abfd, unsigned int i);

DESCRIPTION
	Set the maximum size of objects to be optimized using the GP
	register under MIPS ELF.  This is typically set by
	the <<-G>> argument to the compiler, assembler or linker.
*/

void
bfd_set_gp_size (bfd *abfd, unsigned int i)
{
  /* Don't try to set GP size on an archive or core file!  */
  if (abfd->format != bfd_object)
    return;

  if (abfd->xvec->flavour == bfd_target_elf_flavour)
    elf_gp_size (abfd) = i;
}

/* Get the GP value.  This is an internal function used by some of the
   relocation special_function routines on targets which support a GP
   register.  */

bfd_vma
_bfd_get_gp_value (bfd *abfd)
{
  if (! abfd)
    return 0;
  if (abfd->format != bfd_object)
    return 0;

  if (abfd->xvec->flavour == bfd_target_elf_flavour)
    return elf_gp (abfd);

  return 0;
}

/* Set the GP value.  */

void
_bfd_set_gp_value (bfd *abfd, bfd_vma v)
{
  if (! abfd)
    abort ();
  if (abfd->format != bfd_object)
    return;

  if (abfd->xvec->flavour == bfd_target_elf_flavour)
    elf_gp (abfd) = v;
}

/*
FUNCTION
	bfd_scan_vma

SYNOPSIS
	bfd_vma bfd_scan_vma (const char *string, const char **end, int base);

DESCRIPTION
	Convert, like <<strtoul>>, a numerical expression
	@var{string} into a <<bfd_vma>> integer, and return that integer.
	(Though without as many bells and whistles as <<strtoul>>.)
	The expression is assumed to be unsigned (i.e., positive).
	If given a @var{base}, it is used as the base for conversion.
	A base of 0 causes the function to interpret the string
	in hex if a leading "0x" or "0X" is found, otherwise
	in octal if a leading zero is found, otherwise in decimal.

	If the value would overflow, the maximum <<bfd_vma>> value is
	returned.
*/

bfd_vma
bfd_scan_vma (const char *string, const char **end, int base)
{
  bfd_vma value;
  bfd_vma cutoff;
  unsigned int cutlim;
  int overflow;

  /* Let the host do it if possible.  */
  if (sizeof (bfd_vma) <= sizeof (unsigned long))
    return strtoul (string, (char **) end, base);

  if (sizeof (bfd_vma) <= sizeof (unsigned long long))
    return strtoull (string, (char **) end, base);

  if (base == 0)
    {
      if (string[0] == '0')
	{
	  if ((string[1] == 'x') || (string[1] == 'X'))
	    base = 16;
	  else
	    base = 8;
	}
    }

  if ((base < 2) || (base > 36))
    base = 10;

  if (base == 16
      && string[0] == '0'
      && (string[1] == 'x' || string[1] == 'X')
      && ISXDIGIT (string[2]))
    {
      string += 2;
    }

  cutoff = (~ (bfd_vma) 0) / (bfd_vma) base;
  cutlim = (~ (bfd_vma) 0) % (bfd_vma) base;
  value = 0;
  overflow = 0;
  while (1)
    {
      unsigned int digit;

      digit = *string;
      if (ISDIGIT (digit))
	digit = digit - '0';
      else if (ISALPHA (digit))
	digit = TOUPPER (digit) - 'A' + 10;
      else
	break;
      if (digit >= (unsigned int) base)
	break;
      if (value > cutoff || (value == cutoff && digit > cutlim))
	overflow = 1;
      value = value * base + digit;
      ++string;
    }

  if (overflow)
    value = ~ (bfd_vma) 0;

  if (end != NULL)
    *end = string;

  return value;
}

/*
FUNCTION
	bfd_copy_private_header_data

SYNOPSIS
	bfd_boolean bfd_copy_private_header_data (bfd *ibfd, bfd *obfd);

DESCRIPTION
	Copy private BFD header information from the BFD @var{ibfd} to the
	the BFD @var{obfd}.  This copies information that may require
	sections to exist, but does not require symbol tables.  Return
	<<true>> on success, <<false>> on error.
	Possible error returns are:

	o <<bfd_error_no_memory>> -
	Not enough memory exists to create private data for @var{obfd}.

.#define bfd_copy_private_header_data(ibfd, obfd) \
.	BFD_SEND (obfd, _bfd_copy_private_header_data, \
.		  (ibfd, obfd))

*/

/*
FUNCTION
	bfd_copy_private_bfd_data

SYNOPSIS
	bfd_boolean bfd_copy_private_bfd_data (bfd *ibfd, bfd *obfd);

DESCRIPTION
	Copy private BFD information from the BFD @var{ibfd} to the
	the BFD @var{obfd}.  Return <<TRUE>> on success, <<FALSE>> on error.
	Possible error returns are:

	o <<bfd_error_no_memory>> -
	Not enough memory exists to create private data for @var{obfd}.

.#define bfd_copy_private_bfd_data(ibfd, obfd) \
.	BFD_SEND (obfd, _bfd_copy_private_bfd_data, \
.		  (ibfd, obfd))

*/

/*
FUNCTION
	bfd_set_private_flags

SYNOPSIS
	bfd_boolean bfd_set_private_flags (bfd *abfd, flagword flags);

DESCRIPTION
	Set private BFD flag information in the BFD @var{abfd}.
	Return <<TRUE>> on success, <<FALSE>> on error.  Possible error
	returns are:

	o <<bfd_error_no_memory>> -
	Not enough memory exists to create private data for @var{obfd}.

.#define bfd_set_private_flags(abfd, flags) \
.	BFD_SEND (abfd, _bfd_set_private_flags, (abfd, flags))

*/

/*
FUNCTION
	Other functions

DESCRIPTION
	The following functions exist but have not yet been documented.

.#define bfd_sizeof_headers(abfd, info) \
.	BFD_SEND (abfd, _bfd_sizeof_headers, (abfd, info))
.
.#define bfd_find_nearest_line(abfd, sec, syms, off, file, func, line) \
.	BFD_SEND (abfd, _bfd_find_nearest_line, \
.		  (abfd, syms, sec, off, file, func, line, NULL))
.
.#define bfd_find_nearest_line_discriminator(abfd, sec, syms, off, file, func, \
.					    line, disc) \
.	BFD_SEND (abfd, _bfd_find_nearest_line, \
.		  (abfd, syms, sec, off, file, func, line, disc))
.
.#define bfd_find_line(abfd, syms, sym, file, line) \
.	BFD_SEND (abfd, _bfd_find_line, \
.		  (abfd, syms, sym, file, line))
.
.#define bfd_find_inliner_info(abfd, file, func, line) \
.	BFD_SEND (abfd, _bfd_find_inliner_info, \
.		  (abfd, file, func, line))
.
.#define bfd_debug_info_start(abfd) \
.	BFD_SEND (abfd, _bfd_debug_info_start, (abfd))
.
.#define bfd_debug_info_end(abfd) \
.	BFD_SEND (abfd, _bfd_debug_info_end, (abfd))
.
.#define bfd_debug_info_accumulate(abfd, section) \
.	BFD_SEND (abfd, _bfd_debug_info_accumulate, (abfd, section))
.
.#define bfd_stat_arch_elt(abfd, stat) \
.	BFD_SEND (abfd, _bfd_stat_arch_elt,(abfd, stat))
.
.#define bfd_update_armap_timestamp(abfd) \
.	BFD_SEND (abfd, _bfd_update_armap_timestamp, (abfd))
.
.#define bfd_set_arch_mach(abfd, arch, mach)\
.	BFD_SEND ( abfd, _bfd_set_arch_mach, (abfd, arch, mach))
.
.#define bfd_relax_section(abfd, section, link_info, again) \
.	BFD_SEND (abfd, _bfd_relax_section, (abfd, section, link_info, again))
.
.#define bfd_gc_sections(abfd, link_info) \
.	BFD_SEND (abfd, _bfd_gc_sections, (abfd, link_info))
.
.#define bfd_lookup_section_flags(link_info, flag_info, section) \
.	BFD_SEND (abfd, _bfd_lookup_section_flags, (link_info, flag_info, section))
.
.#define bfd_merge_sections(abfd, link_info) \
.	BFD_SEND (abfd, _bfd_merge_sections, (abfd, link_info))
.
.#define bfd_is_group_section(abfd, sec) \
.	BFD_SEND (abfd, _bfd_is_group_section, (abfd, sec))
.
.#define bfd_discard_group(abfd, sec) \
.	BFD_SEND (abfd, _bfd_discard_group, (abfd, sec))
.
.#define bfd_link_hash_table_create(abfd) \
.	BFD_SEND (abfd, _bfd_link_hash_table_create, (abfd))
.
.#define bfd_link_add_symbols(abfd, info) \
.	BFD_SEND (abfd, _bfd_link_add_symbols, (abfd, info))
.
.#define bfd_link_just_syms(abfd, sec, info) \
.	BFD_SEND (abfd, _bfd_link_just_syms, (sec, info))
.
.#define bfd_final_link(abfd, info) \
.	BFD_SEND (abfd, _bfd_final_link, (abfd, info))
.
.#define bfd_free_cached_info(abfd) \
.	BFD_SEND (abfd, _bfd_free_cached_info, (abfd))
.
.#define bfd_get_dynamic_symtab_upper_bound(abfd) \
.	BFD_SEND (abfd, _bfd_get_dynamic_symtab_upper_bound, (abfd))
.
.#define bfd_print_private_bfd_data(abfd, file)\
.	BFD_SEND (abfd, _bfd_print_private_bfd_data, (abfd, file))
.
.#define bfd_canonicalize_dynamic_symtab(abfd, asymbols) \
.	BFD_SEND (abfd, _bfd_canonicalize_dynamic_symtab, (abfd, asymbols))
.
.#define bfd_get_synthetic_symtab(abfd, count, syms, dyncount, dynsyms, ret) \
.	BFD_SEND (abfd, _bfd_get_synthetic_symtab, (abfd, count, syms, \
.						    dyncount, dynsyms, ret))
.
.#define bfd_get_dynamic_reloc_upper_bound(abfd) \
.	BFD_SEND (abfd, _bfd_get_dynamic_reloc_upper_bound, (abfd))
.
.#define bfd_canonicalize_dynamic_reloc(abfd, arels, asyms) \
.	BFD_SEND (abfd, _bfd_canonicalize_dynamic_reloc, (abfd, arels, asyms))
.
.extern bfd_byte *bfd_get_relocated_section_contents
.  (bfd *, struct bfd_link_info *, struct bfd_link_order *, bfd_byte *,
.   bfd_boolean, asymbol **);
.

*/

bfd_byte *
bfd_get_relocated_section_contents (bfd *abfd,
				    struct bfd_link_info *link_info,
				    struct bfd_link_order *link_order,
				    bfd_byte *data,
				    bfd_boolean relocatable,
				    asymbol **symbols)
{
  bfd *abfd2;
  bfd_byte *(*fn) (bfd *, struct bfd_link_info *, struct bfd_link_order *,
		   bfd_byte *, bfd_boolean, asymbol **);

  if (link_order->type == bfd_indirect_link_order)
    {
      abfd2 = link_order->u.indirect.section->owner;
      if (abfd2 == NULL)
	abfd2 = abfd;
    }
  else
    abfd2 = abfd;

  fn = abfd2->xvec->_bfd_get_relocated_section_contents;

  return (*fn) (abfd, link_info, link_order, data, relocatable, symbols);
}

/* Record information about an ELF program header.  */

bfd_boolean
bfd_record_phdr (bfd *abfd,
		 uint32_t type,
		 bfd_boolean flags_valid,
		 flagword flags,
		 bfd_boolean at_valid,
		 bfd_vma at,
		 bfd_boolean includes_filehdr,
		 bfd_boolean includes_phdrs,
		 unsigned int count,
		 asection **secs)
{
  struct elf_segment_map *m, **pm;
  bfd_size_type amt;

  if (bfd_get_flavour (abfd) != bfd_target_elf_flavour)
    return TRUE;

  amt = sizeof (struct elf_segment_map);
  amt += ((bfd_size_type) count - 1) * sizeof (asection *);
  m = (struct elf_segment_map *) bfd_zalloc (abfd, amt);
  if (m == NULL)
    return FALSE;

  m->p_type = type;
  m->p_flags = flags;
  m->p_paddr = at;
  m->p_flags_valid = flags_valid;
  m->p_paddr_valid = at_valid;
  m->includes_filehdr = includes_filehdr;
  m->includes_phdrs = includes_phdrs;
  m->count = count;
  if (count > 0)
    memcpy (m->sections, secs, count * sizeof (asection *));

  for (pm = &elf_seg_map (abfd); *pm != NULL; pm = &(*pm)->next)
    ;
  *pm = m;

  return TRUE;
}

/* bfd_sprintf_vma and bfd_fprintf_vma display an address in the
   target's address size.  */

void
bfd_sprintf_vma (bfd *abfd ATTRIBUTE_UNUSED, char *buf, bfd_vma value)
{
  sprintf_vma (buf, value);
}

void
bfd_fprintf_vma (bfd *abfd ATTRIBUTE_UNUSED, void *stream, bfd_vma value)
{
  fprintf_vma ((FILE *) stream, value);
}

/*
FUNCTION
	bfd_alt_mach_code

SYNOPSIS
	bfd_boolean bfd_alt_mach_code (bfd *abfd, int alternative);

DESCRIPTION

	When more than one machine code number is available for the
	same machine type, this function can be used to switch between
	the preferred one (alternative == 0) and any others.  Currently,
	only ELF supports this feature, with up to two alternate
	machine codes.
*/

bfd_boolean
bfd_alt_mach_code (bfd *abfd, int alternative)
{
  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
    {
      int code;

      switch (alternative)
	{
	case 0:
	  code = get_elf_backend_data (abfd)->elf_machine_code;
	  break;

	case 1:
	  code = get_elf_backend_data (abfd)->elf_machine_alt1;
	  if (code == 0)
	    return FALSE;
	  break;

	case 2:
	  code = get_elf_backend_data (abfd)->elf_machine_alt2;
	  if (code == 0)
	    return FALSE;
	  break;

	default:
	  return FALSE;
	}

      elf_elfheader (abfd)->e_machine = code;

      return TRUE;
    }

  return FALSE;
}

/*
FUNCTION
	bfd_emul_get_maxpagesize

SYNOPSIS
	bfd_vma bfd_emul_get_maxpagesize (const char *);

DESCRIPTION
	Returns the maximum page size, in bytes, as determined by
	emulation.

RETURNS
	Returns the maximum page size in bytes for ELF, 0 otherwise.
*/

bfd_vma
bfd_emul_get_maxpagesize (const char *emul)
{
  const bfd_target *target;

  target = bfd_find_target (emul, NULL);
  if (target != NULL
      && target->flavour == bfd_target_elf_flavour)
    return xvec_get_elf_backend_data (target)->maxpagesize;

  return 0;
}

static void
bfd_elf_set_pagesize (const bfd_target *target, bfd_vma size,
		      int offset, const bfd_target *orig_target)
{
  if (target->flavour == bfd_target_elf_flavour)
    {
      const struct elf_backend_data *bed;

      bed = xvec_get_elf_backend_data (target);
      *((bfd_vma *) ((char *) bed + offset)) = size;
    }

  if (target->alternative_target
      && target->alternative_target != orig_target)
    bfd_elf_set_pagesize (target->alternative_target, size, offset,
			  orig_target);
}

/*
FUNCTION
	bfd_emul_set_maxpagesize

SYNOPSIS
	void bfd_emul_set_maxpagesize (const char *, bfd_vma);

DESCRIPTION
	For ELF, set the maximum page size for the emulation.  It is
	a no-op for other formats.

*/

void
bfd_emul_set_maxpagesize (const char *emul, bfd_vma size)
{
  const bfd_target *target;

  target = bfd_find_target (emul, NULL);
  if (target)
    bfd_elf_set_pagesize (target, size,
			  offsetof (struct elf_backend_data,
				    maxpagesize), target);
}

/*
FUNCTION
	bfd_emul_get_commonpagesize

SYNOPSIS
	bfd_vma bfd_emul_get_commonpagesize (const char *, bfd_boolean);

DESCRIPTION
	Returns the common page size, in bytes, as determined by
	emulation.

RETURNS
	Returns the common page size in bytes for ELF, 0 otherwise.
*/

bfd_vma
bfd_emul_get_commonpagesize (const char *emul, bfd_boolean relro)
{
  const bfd_target *target;

  target = bfd_find_target (emul, NULL);
  if (target != NULL
      && target->flavour == bfd_target_elf_flavour)
    {
      const struct elf_backend_data *bed;

      bed = xvec_get_elf_backend_data (target);
      if (relro)
	return bed->relropagesize;
      else
	return bed->commonpagesize;
    }
  return 0;
}

/*
FUNCTION
	bfd_emul_set_commonpagesize

SYNOPSIS
	void bfd_emul_set_commonpagesize (const char *, bfd_vma);

DESCRIPTION
	For ELF, set the common page size for the emulation.  It is
	a no-op for other formats.

*/

void
bfd_emul_set_commonpagesize (const char *emul, bfd_vma size)
{
  const bfd_target *target;

  target = bfd_find_target (emul, NULL);
  if (target)
    bfd_elf_set_pagesize (target, size,
			  offsetof (struct elf_backend_data,
				    commonpagesize), target);
}

/*
FUNCTION
	bfd_update_compression_header

SYNOPSIS
	void bfd_update_compression_header
	  (bfd *abfd, bfd_byte *contents, asection *sec);

DESCRIPTION
	Set the compression header at CONTENTS of SEC in ABFD and update
	elf_section_flags for compression.
*/

void
bfd_update_compression_header (bfd *abfd, bfd_byte *contents,
			       asection *sec)
{
  if ((abfd->flags & BFD_COMPRESS) != 0)
    {
      if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
	{
	  if ((abfd->flags & BFD_COMPRESS_GABI) != 0)
	    {
	      const struct elf_backend_data *bed
		= get_elf_backend_data (abfd);

	      /* Set the SHF_COMPRESSED bit.  */
	      elf_section_flags (sec) |= SHF_COMPRESSED;

	      if (bed->s->elfclass == ELFCLASS32)
		{
		  Elf32_External_Chdr *echdr
		    = (Elf32_External_Chdr *) contents;
		  bfd_put_32 (abfd, ELFCOMPRESS_ZLIB, &echdr->ch_type);
		  bfd_put_32 (abfd, sec->size, &echdr->ch_size);
		  bfd_put_32 (abfd, 1 << sec->alignment_power,
			      &echdr->ch_addralign);
		}
	      else
		{
		  Elf64_External_Chdr *echdr
		    = (Elf64_External_Chdr *) contents;
		  bfd_put_32 (abfd, ELFCOMPRESS_ZLIB, &echdr->ch_type);
		  bfd_put_32 (abfd, 0, &echdr->ch_reserved);
		  bfd_put_64 (abfd, sec->size, &echdr->ch_size);
		  bfd_put_64 (abfd, 1 << sec->alignment_power,
			      &echdr->ch_addralign);
		}
	    }
	  else
	    {
	      /* Clear the SHF_COMPRESSED bit.  */
	      elf_section_flags (sec) &= ~SHF_COMPRESSED;

	      /* Write the zlib header.  It should be "ZLIB" followed by
		 the uncompressed section size, 8 bytes in big-endian
		 order.  */
	      memcpy (contents, "ZLIB", 4);
	      bfd_putb64 (sec->size, contents + 4);
	    }
	}
    }
  else
    abort ();
}

/*
   FUNCTION
   bfd_check_compression_header

   SYNOPSIS
	bfd_boolean bfd_check_compression_header
	  (bfd *abfd, bfd_byte *contents, asection *sec,
	  bfd_size_type *uncompressed_size);

DESCRIPTION
	Check the compression header at CONTENTS of SEC in ABFD and
	store the uncompressed size in UNCOMPRESSED_SIZE if the
	compression header is valid.

RETURNS
	Return TRUE if the compression header is valid.
*/

bfd_boolean
bfd_check_compression_header (bfd *abfd, bfd_byte *contents,
			      asection *sec,
			      bfd_size_type *uncompressed_size)
{
  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour
      && (elf_section_flags (sec) & SHF_COMPRESSED) != 0)
    {
      Elf_Internal_Chdr chdr;
      const struct elf_backend_data *bed = get_elf_backend_data (abfd);
      if (bed->s->elfclass == ELFCLASS32)
	{
	  Elf32_External_Chdr *echdr = (Elf32_External_Chdr *) contents;
	  chdr.ch_type = bfd_get_32 (abfd, &echdr->ch_type);
	  chdr.ch_size = bfd_get_32 (abfd, &echdr->ch_size);
	  chdr.ch_addralign = bfd_get_32 (abfd, &echdr->ch_addralign);
	}
      else
	{
	  Elf64_External_Chdr *echdr = (Elf64_External_Chdr *) contents;
	  chdr.ch_type = bfd_get_32 (abfd, &echdr->ch_type);
	  chdr.ch_size = bfd_get_64 (abfd, &echdr->ch_size);
	  chdr.ch_addralign = bfd_get_64 (abfd, &echdr->ch_addralign);
	}
      if (chdr.ch_type == ELFCOMPRESS_ZLIB
	  && chdr.ch_addralign == 1U << sec->alignment_power)
	{
	  *uncompressed_size = chdr.ch_size;
	  return TRUE;
	}
    }

  return FALSE;
}

/*
FUNCTION
	bfd_get_compression_header_size

SYNOPSIS
	int bfd_get_compression_header_size (bfd *abfd, asection *sec);

DESCRIPTION
	Return the size of the compression header of SEC in ABFD.

RETURNS
	Return the size of the compression header in bytes.
*/

int
bfd_get_compression_header_size (bfd *abfd, asection *sec)
{
  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
    {
      if (sec == NULL)
	{
	  if (!(abfd->flags & BFD_COMPRESS_GABI))
	    return 0;
	}
      else if (!(elf_section_flags (sec) & SHF_COMPRESSED))
	return 0;

      if (get_elf_backend_data (abfd)->s->elfclass == ELFCLASS32)
	return sizeof (Elf32_External_Chdr);
      else
	return sizeof (Elf64_External_Chdr);
    }

  return 0;
}

/*
FUNCTION
	bfd_convert_section_size

SYNOPSIS
	bfd_size_type bfd_convert_section_size
	  (bfd *ibfd, asection *isec, bfd *obfd, bfd_size_type size);

DESCRIPTION
	Convert the size @var{size} of the section @var{isec} in input
	BFD @var{ibfd} to the section size in output BFD @var{obfd}.
*/

bfd_size_type
bfd_convert_section_size (bfd *ibfd, sec_ptr isec, bfd *obfd,
			  bfd_size_type size)
{
  bfd_size_type hdr_size;

  /* Do nothing if input file will be decompressed.  */
  if ((ibfd->flags & BFD_DECOMPRESS))
    return size;

  /* Do nothing if either input or output aren't ELF.  */
  if (bfd_get_flavour (ibfd) != bfd_target_elf_flavour
      || bfd_get_flavour (obfd) != bfd_target_elf_flavour)
    return size;

  /* Do nothing if ELF classes of input and output are the same. */
  if (get_elf_backend_data (ibfd)->s->elfclass
      == get_elf_backend_data (obfd)->s->elfclass)
    return size;

  /* Do nothing if the input section isn't a SHF_COMPRESSED section. */
  hdr_size = bfd_get_compression_header_size (ibfd, isec);
  if (hdr_size == 0)
    return size;

  /* Adjust the size of the output SHF_COMPRESSED section.  */
  if (hdr_size == sizeof (Elf32_External_Chdr))
    return (size - sizeof (Elf32_External_Chdr)
	    + sizeof (Elf64_External_Chdr));
  else
    return (size - sizeof (Elf64_External_Chdr)
	    + sizeof (Elf32_External_Chdr));
}

/*
FUNCTION
	bfd_convert_section_contents

SYNOPSIS
	bfd_boolean bfd_convert_section_contents
	  (bfd *ibfd, asection *isec, bfd *obfd,
	   bfd_byte **ptr, bfd_size_type *ptr_size);

DESCRIPTION
	Convert the contents, stored in @var{*ptr}, of the section
	@var{isec} in input BFD @var{ibfd} to output BFD @var{obfd}
	if needed.  The original buffer pointed to by @var{*ptr} may
	be freed and @var{*ptr} is returned with memory malloc'd by this
	function, and the new size written to @var{ptr_size}.
*/

bfd_boolean
bfd_convert_section_contents (bfd *ibfd, sec_ptr isec, bfd *obfd,
			      bfd_byte **ptr, bfd_size_type *ptr_size)
{
  bfd_byte *contents;
  bfd_size_type ihdr_size, ohdr_size, size;
  Elf_Internal_Chdr chdr;
  bfd_boolean use_memmove;

  /* Do nothing if input file will be decompressed.  */
  if ((ibfd->flags & BFD_DECOMPRESS))
    return TRUE;

  /* Do nothing if either input or output aren't ELF.  */
  if (bfd_get_flavour (ibfd) != bfd_target_elf_flavour
      || bfd_get_flavour (obfd) != bfd_target_elf_flavour)
    return TRUE;

  /* Do nothing if ELF classes of input and output are the same. */
  if (get_elf_backend_data (ibfd)->s->elfclass
      == get_elf_backend_data (obfd)->s->elfclass)
    return TRUE;

  /* Do nothing if the input section isn't a SHF_COMPRESSED section. */
  ihdr_size = bfd_get_compression_header_size (ibfd, isec);
  if (ihdr_size == 0)
    return TRUE;

  contents = *ptr;

  /* Convert the contents of the input SHF_COMPRESSED section to
     output.  Get the input compression header and the size of the
     output compression header.  */
  if (ihdr_size == sizeof (Elf32_External_Chdr))
    {
      Elf32_External_Chdr *echdr = (Elf32_External_Chdr *) contents;
      chdr.ch_type = bfd_get_32 (ibfd, &echdr->ch_type);
      chdr.ch_size = bfd_get_32 (ibfd, &echdr->ch_size);
      chdr.ch_addralign = bfd_get_32 (ibfd, &echdr->ch_addralign);

      ohdr_size = sizeof (Elf64_External_Chdr);

      use_memmove = FALSE;
    }
  else
    {
      Elf64_External_Chdr *echdr = (Elf64_External_Chdr *) contents;
      chdr.ch_type = bfd_get_32 (ibfd, &echdr->ch_type);
      chdr.ch_size = bfd_get_64 (ibfd, &echdr->ch_size);
      chdr.ch_addralign = bfd_get_64 (ibfd, &echdr->ch_addralign);

      ohdr_size = sizeof (Elf32_External_Chdr);
      use_memmove = TRUE;
    }

  size = bfd_get_section_size (isec) - ihdr_size + ohdr_size;
  if (!use_memmove)
    {
      contents = (bfd_byte *) bfd_malloc (size);
      if (contents == NULL)
	return FALSE;
    }

  /* Write out the output compression header.  */
  if (ohdr_size == sizeof (Elf32_External_Chdr))
    {
      Elf32_External_Chdr *echdr = (Elf32_External_Chdr *) contents;
      bfd_put_32 (obfd, ELFCOMPRESS_ZLIB, &echdr->ch_type);
      bfd_put_32 (obfd, chdr.ch_size, &echdr->ch_size);
      bfd_put_32 (obfd, chdr.ch_addralign, &echdr->ch_addralign);
    }
  else
    {
      Elf64_External_Chdr *echdr = (Elf64_External_Chdr *) contents;
      bfd_put_32 (obfd, ELFCOMPRESS_ZLIB, &echdr->ch_type);
      bfd_put_32 (obfd, 0, &echdr->ch_reserved);
      bfd_put_64 (obfd, chdr.ch_size, &echdr->ch_size);
      bfd_put_64 (obfd, chdr.ch_addralign, &echdr->ch_addralign);
    }

  /* Copy the compressed contents.  */
  if (use_memmove)
    memmove (contents + ohdr_size, *ptr + ihdr_size, size - ohdr_size);
  else
    {
      memcpy (contents + ohdr_size, *ptr + ihdr_size, size - ohdr_size);
      free (*ptr);
      *ptr = contents;
    }

  *ptr_size = size;
  return TRUE;
}
