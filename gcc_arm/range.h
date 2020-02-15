/* CYGNUS LOCAL LRS */
/* Allocate registers within a basic block, for GNU compiler.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.

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

/* Global declarations for live range support.  */
/* Linked list of live ranges to try allocating registers in first before
   allocating all of the remaining registers.  */
extern rtx live_range_list;

/* # of distinct ranges seen so far */
extern int range_max_unique;

/* Bits for the different live range copy options.  */
/* Allow copys from original register into new register */
#define LIVE_RANGE_COPYIN 0x1

/* Allow copys from new register back into original register */
#define LIVE_RANGE_COPYOUT 0x2
   
/* Allow copies from constant integers */
#define LIVE_RANGE_COPYIN_CONST 0x4

/* Default value for using GDB specific stabs to denote live ranges */
#define LIVE_RANGE_GDB_DEFAULT 1

/* Default value for creating scoping blocks for live ranges */
#define LIVE_RANGE_SCOPE_DEFAULT 0

/* Determine whether or not to use new style stabs for live range debugging.
   Assumes that write_symbols == DBX_DEBUG has already been checked for.  */
#define LIVE_RANGE_GDBSTAB_P() (use_gnu_debug_info_extensions		\
				&& flag_live_range_gdb)

/* Live range functions */
#ifdef BUFSIZ
extern void live_range_print	PROTO((FILE *, rtx, char *, char *));
extern int  live_range		PROTO((rtx, FILE *));
#endif
extern void init_live_range	PROTO((void));
/* END CYGNUS LOCAL LRS */
