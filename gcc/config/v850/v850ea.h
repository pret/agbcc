/* CYGNUS LOCAL entire file
   
   Definitions of target machine for GNU compiler. 
   NEC V850EA series
   Copyright (C) 1997, 1998, 1999 Free Software Foundation, Inc.
   Contributed by Nick Clifton (nickc@cygnus.com).

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

#define CPP_SPEC		"%{mv850ea:-D__v850ea__} %{mv850e:-D__v850e__} %{mv850:-D__v850__} %{!mv*:-D__v850ea__}"

#define TARGET_VERSION 		fprintf (stderr, " (NEC V850EA)");

#define MASK_V850EA             0x00000030
#define MASK_US_BIT_SET         0x00001000
#define MASK_US_MASK_SET        0x00002000

#define MASK_DEFAULT            MASK_V850EA

/* Special version of TARGET_V850E which will be true if MASK_V850EA flag is set.  */
#define TARGET_V850E   		((target_flags & MASK_V850E) == MASK_V850E)
#define TARGET_V850EA   	((target_flags & MASK_CPU) == MASK_V850EA)

#define TARGET_US_BIT_SET	(target_flags & MASK_US_BIT_SET)

#define EXTRA_SWITCHES 		{ "v850e",   MASK_V850E, "Compile for v850e processor" },            \
                                { "v850e", -(MASK_V850E ^ MASK_CPU), "" }, /* Make sure that the other bits are cleared.  */ \
                       		{ "v850ea", MASK_V850EA, "" }, \
                       		{ "US-bit-set", (MASK_US_BIT_SET | MASK_US_MASK_SET), "" },	\
                       		{ "no-US-bit-set", - MASK_US_BIT_SET, "" },	\
                       		{ "no-US-bit-set", MASK_US_MASK_SET, "" },

/* We must pass a -mv850ea option to the assembler if no explicit -mv* option
   is given, because the assembler's processor default may not be correct.  */

#define SUBTARGET_ASM_SPEC "%{!mv*:-mv850ea}"

/* Now include the rest of the definitions.  */
#include "v850e.h"
