/* Definitions of target machine for GNU compiler.
   NEC VR4100 version.
   Copyright (c) 1995 Cygnus Support Inc.

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
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#define MIPS_CPU_STRING_DEFAULT "VR4100"

/* Use the MIPS EABI by default.  */
#define MIPS_ABI_DEFAULT ABI_EABI

/* The following is needed because -mips3 and -mips4 set gp64 which in
   combination with abi=eabi, causes long64 to be set. */
#define SUBTARGET_CPP_SIZE_SPEC "\
-D__SIZE_TYPE__=long\\ unsigned\\ int -D__PTRDIFF_TYPE__=long\\ int"

/* Debugging */

#define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* For the 'preferred' cases ("gN" and "ggdbN") we need to tell the 
   gnu assembler not to generate debugging information. */
   
#define SUBTARGET_ASM_DEBUGGING_SPEC "\
%{!mmips-as: \
  %{g:-g0} %{g0:-g0} %{g1:-g0} %{g2:-g0} %{g3:-g0} \
  %{ggdb:-g0} %{ggdb0:-g0} %{ggdb1:-g0} %{ggdb2:-g0} %{ggdb3:-g0} \
  %{gdwarf-2*:-g0}} \
%{gstabs:-g} %{gstabs0:-g0} %{gstabs1:-g1} %{gstabs2:-g2} %{gstabs3:-g3} \
%{gstabs+:-g} %{gstabs+0:-g0} %{gstabs+1:-g1} %{gstabs+2:-g2} %{gstabs+3:-g3} \
%{gcoff:-g} %{gcoff0:-g0} %{gcoff1:-g1} %{gcoff2:-g2} %{gcoff3:-g3}"

#include "mips/elf64.h"

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "EB", "mno-mips16" }

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "\
%{!mips1:%{!mips2:-U__mips -D__mips=3 -D__mips64}} \
%{!mabi=32: %{!mabi=n32: %{!mabi=64: -D__mips_eabi}}}"

/* __mips_soft_float must be defined by default because libgloss uses it.  */

#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-Dmips -DMIPSEB -DR4100 -D_mips -D_MIPSEB -D_R4100 -D__mips_soft_float"
