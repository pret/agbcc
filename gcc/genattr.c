/* Generate attribute information (insn-attr.h) from machine description.
   Copyright (C) 1991, 1994, 1996, 1998, 1999 Free Software Foundation, Inc.
   Contributed by Richard Kenner (kenner@vlsi1.ultra.nyu.edu)

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
#include "obstack.h"

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack obstack;
struct obstack *rtl_obstack = &obstack;

/* Define this so we can link with print-rtl.o to get debug_rtx function.  */
char **insn_name_ptr = 0;

static void write_upcase (char *);
static void gen_attr (rtx);

static void
write_upcase (char *str)
{
  for (; *str; str++)
    if (*str >= 'a' && *str <= 'z')
      printf ("%c", *str - 'a' + 'A');
    else
      printf ("%c", *str);
}

static void
gen_attr (rtx attr)
{
  char *p;

  printf ("#define HAVE_ATTR_%s\n", XSTR (attr, 0));

  /* If numeric attribute, don't need to write an enum.  */
  if (*XSTR (attr, 1) == '\0')
    printf ("extern int get_attr_%s ();\n", XSTR (attr, 0));
  else
    {
      printf ("enum attr_%s {", XSTR (attr, 0));
      write_upcase (XSTR (attr, 0));
      printf ("_");

      for (p = XSTR (attr, 1); *p != '\0'; p++)
	{
	  if (*p == ',')
	    {
	      printf (", ");
	      write_upcase (XSTR (attr, 0));
	      printf ("_");
	    }
	  else if (*p >= 'a' && *p <= 'z')
	    printf ("%c", *p - 'a' + 'A');
	  else
	    printf ("%c", *p);
	}

      printf ("};\n");
      printf ("extern enum attr_%s get_attr_%s ();\n\n",
	      XSTR (attr, 0), XSTR (attr, 0));
    }

  /* If `length' attribute, write additional function definitions and define
     variables used by `insn_current_length'.  */
  if (! strcmp (XSTR (attr, 0), "length"))
    {
      printf ("extern void init_lengths ();\n");
      printf ("extern void shorten_branches (rtx);\n");
      printf ("extern int insn_default_length (rtx);\n");
      printf ("extern int insn_variable_length_p (rtx);\n");
      printf ("extern int insn_current_length (rtx);\n\n");
      printf ("extern int *insn_addresses;\n");
      printf ("extern int insn_current_address;\n\n");
    }
}

static void
fatal(const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  fprintf (stderr, "genattr: ");
  vfprintf (stderr, format, ap);
  va_end (ap);
  fprintf (stderr, "\n");
  exit (EXIT_FAILURE);
}

void *
xmalloc(size_t size)
{
  void *val = malloc(size);

  if (val == NULL)
    fatal ("virtual memory exhausted");

  return val;
}

int
main(int argc, char **argv)
{
  rtx desc;
  FILE *infile;
  int c;

  obstack_init (rtl_obstack);

  if (argc <= 1)
    fatal ("No input file name.");

  infile = fopen (argv[1], "r");
  if (infile == 0)
    {
      perror (argv[1]);
      exit (EXIT_FAILURE);
    }

  init_rtl ();

  printf ("/* Generated automatically by the program `genattr'\n\
from the machine description file `md'.  */\n\n");

  /* For compatibility, define the attribute `alternative', which is just
     a reference to the variable `which_alternative'.  */

  printf ("#define HAVE_ATTR_alternative\n");
  printf ("#define get_attr_alternative(insn) which_alternative\n");
     
  /* Read the machine description.  */

  while (1)
    {
      c = read_skip_spaces (infile);
      if (c == EOF)
	break;
      ungetc (c, infile);

      desc = read_rtx (infile);
      if (GET_CODE (desc) == DEFINE_ATTR)
	gen_attr (desc);
    }

  /* Output flag masks for use by reorg.  

     Flags are used to hold branch direction and prediction information
     for use by eligible_for_...  */
  printf("\n#define ATTR_FLAG_forward\t0x1\n");
  printf("#define ATTR_FLAG_backward\t0x2\n");
  printf("#define ATTR_FLAG_likely\t0x4\n");
  printf("#define ATTR_FLAG_very_likely\t0x8\n");
  printf("#define ATTR_FLAG_unlikely\t0x10\n");
  printf("#define ATTR_FLAG_very_unlikely\t0x20\n");

  fflush (stdout);
  exit (ferror (stdout) != 0 ? EXIT_FAILURE : EXIT_SUCCESS);
  /* NOTREACHED */
  return 0;
}
