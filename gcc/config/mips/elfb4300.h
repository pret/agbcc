/* Definitions of target machine for GNU compiler.
   NEC VR4300 version.
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

#define MIPS_CPU_STRING_DEFAULT "VR4300"

#define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

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
#define MULTILIB_DEFAULTS { "EB" }

#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-Dmips -DMIPSEB -DR4300 -D_mips -D_MIPSEB -D_R4300"
