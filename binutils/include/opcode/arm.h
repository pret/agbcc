/* ARM assembler/disassembler support.
   Copyright (C) 2004-2018 Free Software Foundation, Inc.

   This file is part of GDB and GAS.

   GDB and GAS are free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 3, or (at
   your option) any later version.

   GDB and GAS are distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GDB or GAS; see the file COPYING3.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

/* The following bitmasks control CPU extensions:  */
#define ARM_EXT_V1	 0x00000001	/* All processors (core set).  */
#define ARM_EXT_V2	 0x00000002	/* Multiply instructions.  */
#define ARM_EXT_V2S	 0x00000004	/* SWP instructions.       */
#define ARM_EXT_V3	 0x00000008	/* MSR MRS.                */
#define ARM_EXT_V3M	 0x00000010	/* Allow long multiplies.  */
#define ARM_EXT_V4	 0x00000020	/* Allow half word loads.  */
#define ARM_EXT_V4T	 0x00000040	/* Thumb.                  */
#define ARM_EXT_OS	 0x20000000	/* OS Extensions.  */
#define ARM_EXT_THUMB_MSR 0x02000000	/* Thumb MSR/MRS.	    */


#define FPU_ENDIAN_PURE	 0x80000000	/* Pure-endian doubles.	      */


#define FPU_ENDIAN_BIG	 0		/* Double words-big-endian.   */
#define FPU_FPA_EXT_V1	 0x40000000	/* Base FPA instruction set.  */
#define FPU_FPA_EXT_V2	 0x20000000	/* LFM/SFM.		      */

/* Architectures are the sum of the base and extensions.  The ARM ARM (rev E)
   defines the following: ARMv3, ARMv3M, ARMv4xM, ARMv4, ARMv4TxM, ARMv4T,
   ARMv5xM, ARMv5, ARMv5TxM, ARMv5T, ARMv5TExP, ARMv5TE.  To these we add
   three more to cover cores prior to ARM6.  Finally, there are cores which
   implement further extensions in the co-processor space.  */
#define ARM_AEXT_V1			  ARM_EXT_V1
#define ARM_AEXT_V2	(ARM_AEXT_V1	| ARM_EXT_V2)
#define ARM_AEXT_V2S	(ARM_AEXT_V2	| ARM_EXT_V2S)
#define ARM_AEXT_V3	(ARM_AEXT_V2S	| ARM_EXT_V3)
#define ARM_AEXT_V3M	(ARM_AEXT_V3	| ARM_EXT_V3M)
#define ARM_AEXT_V4xM	(ARM_AEXT_V3	| ARM_EXT_V4)
#define ARM_AEXT_V4	(ARM_AEXT_V3M	| ARM_EXT_V4)
#define ARM_AEXT_V4TxM	(ARM_AEXT_V4xM	| ARM_EXT_V4T | ARM_EXT_OS)
#define ARM_AEXT_V4T	(ARM_AEXT_V4	| ARM_EXT_V4T | ARM_EXT_OS)

#define FPU_VFP_V1xD	(FPU_VFP_EXT_V1xD | FPU_ENDIAN_PURE)
#define FPU_VFP_V1	(FPU_VFP_V1xD | FPU_VFP_EXT_V1)
#define FPU_VFP_V2	(FPU_VFP_V1 | FPU_VFP_EXT_V2)

#define FPU_FPA		(FPU_FPA_EXT_V1 | FPU_FPA_EXT_V2)


/* Deprecated.  */
#define FPU_ARCH_VFP	ARM_FEATURE_COPROC (FPU_ENDIAN_PURE)


#define FPU_ARCH_FPE	ARM_FEATURE_COPROC (FPU_FPA_EXT_V1)
#define FPU_ARCH_FPA	ARM_FEATURE_COPROC (FPU_FPA)

#define ARM_ARCH_V1	ARM_FEATURE_CORE_LOW (ARM_AEXT_V1)
#define ARM_ARCH_V2	ARM_FEATURE_CORE_LOW (ARM_AEXT_V2)
#define ARM_ARCH_V2S	ARM_FEATURE_CORE_LOW (ARM_AEXT_V2S)
#define ARM_ARCH_V3	ARM_FEATURE_CORE_LOW (ARM_AEXT_V3)
#define ARM_ARCH_V3M	ARM_FEATURE_CORE_LOW (ARM_AEXT_V3M)
#define ARM_ARCH_V4xM	ARM_FEATURE_CORE_LOW (ARM_AEXT_V4xM)
#define ARM_ARCH_V4	ARM_FEATURE_CORE_LOW (ARM_AEXT_V4)
#define ARM_ARCH_V4TxM	ARM_FEATURE_CORE_LOW (ARM_AEXT_V4TxM)
#define ARM_ARCH_V4T	ARM_FEATURE_CORE_LOW (ARM_AEXT_V4T)

/* Some useful combinations:  */
#define ARM_ARCH_NONE	ARM_FEATURE_LOW (0, 0)
#define FPU_NONE	ARM_FEATURE_LOW (0, 0)
#define ARM_ANY		ARM_FEATURE (-1, -1, 0)	/* Any basic core.  */
#define FPU_ANY		ARM_FEATURE_COPROC (-1) /* Any FPU.  */
#define ARM_FEATURE_ALL	ARM_FEATURE (-1, -1, -1)/* All CPU and FPU features.  */
#define FPU_ANY_HARD	ARM_FEATURE_COPROC (FPU_FPA | FPU_VFP_HARD | FPU_MAVERICK)

/* There are too many feature bits to fit in a single word, so use a
   structure.  For simplicity we put all core features in array CORE
   and everything else in the other.  All the bits in element core[0]
   have been occupied, so new feature should use bit in element core[1]
   and use macro ARM_FEATURE to initialize the feature set variable.  */
typedef struct
{
  unsigned long core[2];
  unsigned long coproc;
} arm_feature_set;

/* Test whether CPU and FEAT have any features in common.  */
#define ARM_CPU_HAS_FEATURE(CPU,FEAT) \
  (((CPU).core[0] & (FEAT).core[0]) != 0 \
   || ((CPU).core[1] & (FEAT).core[1]) != 0 \
   || ((CPU).coproc & (FEAT).coproc) != 0)

/* Tests whether the features of A are a subset of B.  */
#define ARM_FSET_CPU_SUBSET(A,B) \
  (((A).core[0] & (B).core[0]) == (A).core[0] \
   && ((A).core[1] & (B).core[1]) == (A).core[1] \
   && ((A).coproc & (B).coproc) == (A).coproc)

#define ARM_CPU_IS_ANY(CPU) \
  ((CPU).core[0] == ((arm_feature_set)ARM_ANY).core[0] \
   && (CPU).core[1] == ((arm_feature_set)ARM_ANY).core[1])

#define ARM_MERGE_FEATURE_SETS(TARG,F1,F2)		\
  do							\
    {							\
      (TARG).core[0] = (F1).core[0] | (F2).core[0];	\
      (TARG).core[1] = (F1).core[1] | (F2).core[1];	\
      (TARG).coproc = (F1).coproc | (F2).coproc;	\
    }							\
  while (0)

#define ARM_CLEAR_FEATURE(TARG,F1,F2)			\
  do							\
    {							\
      (TARG).core[0] = (F1).core[0] &~ (F2).core[0];	\
      (TARG).core[1] = (F1).core[1] &~ (F2).core[1];	\
      (TARG).coproc = (F1).coproc &~ (F2).coproc;	\
    }							\
  while (0)

#define ARM_FEATURE_EQUAL(T1,T2)		\
  (   (T1).core[0] == (T2).core[0]		\
   && (T1).core[1] == (T2).core[1]		\
   && (T1).coproc  == (T2).coproc)

#define ARM_FEATURE_ZERO(T)			\
  ((T).core[0] == 0 && (T).core[1] == 0 && (T).coproc == 0)

#define ARM_FEATURE_CORE_EQUAL(T1, T2)		\
  ((T1).core[0] == (T2).core[0] && (T1).core[1] == (T2).core[1])

#define ARM_FEATURE_LOW(core, coproc) {{(core), 0}, (coproc)}
#define ARM_FEATURE_CORE(core1, core2) {{(core1), (core2)}, 0}
#define ARM_FEATURE_CORE_LOW(core) {{(core), 0}, 0}
#define ARM_FEATURE_CORE_HIGH(core) {{0, (core)}, 0}
#define ARM_FEATURE_COPROC(coproc) {{0, 0}, (coproc)}
#define ARM_FEATURE(core1, core2, coproc) {{(core1), (core2)}, (coproc)}
