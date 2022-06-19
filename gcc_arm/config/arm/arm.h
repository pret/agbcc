/* Definitions of target machine for GNU compiler, for Acorn RISC Machine.
   Copyright (C) 1991, 93, 94, 95, 96, 97, 98, 1999, 2002 Free Software Foundation, Inc.
   Contributed by Pieter `Tiggr' Schoenmakers (rcpieter@win.tue.nl)
   and Martin Simmons (@harleqn.co.uk).
   More major hacks by Richard Earnshaw (rwe11@cl.cam.ac.uk)
   
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

/* Configuration triples for ARM ports work as follows:
   (This is a bit of a mess and needs some thought)
   arm-*-*: little endian
   armel-*-*: little endian
   armeb-*-*: big endian
   If a non-embedded environment (ie: "real" OS) is specified, `arm'
   should default to that used by the OS.
*/

#ifndef __ARM_H__
#define __ARM_H__

#define TARGET_CPU_arm2		0x0000
#define TARGET_CPU_arm250	0x0000
#define TARGET_CPU_arm3		0x0000
#define TARGET_CPU_arm6		0x0001
#define TARGET_CPU_arm600	0x0001
#define TARGET_CPU_arm610	0x0002
#define TARGET_CPU_arm7		0x0001
#define TARGET_CPU_arm7m	0x0004
#define TARGET_CPU_arm7dm	0x0004
#define TARGET_CPU_arm7dmi	0x0004
#define TARGET_CPU_arm700	0x0001
#define TARGET_CPU_arm710	0x0002
#define TARGET_CPU_arm7100	0x0002
#define TARGET_CPU_arm7500	0x0002
#define TARGET_CPU_arm7500fe	0x1001
#define TARGET_CPU_arm7tdmi	0x0008
#define TARGET_CPU_arm8		0x0010
#define TARGET_CPU_arm810	0x0020
#define TARGET_CPU_strongarm	0x0040
#define TARGET_CPU_strongarm110 0x0040
#define TARGET_CPU_strongarm1100 0x0040
#define TARGET_CPU_arm9		0x0080
#define TARGET_CPU_arm9tdmi	0x0080
/* Configure didn't specify */
#define TARGET_CPU_generic	0x8000

enum arm_cond_code
{
  ARM_EQ = 0, ARM_NE, ARM_CS, ARM_CC, ARM_MI, ARM_PL, ARM_VS, ARM_VC,
  ARM_HI, ARM_LS, ARM_GE, ARM_LT, ARM_GT, ARM_LE, ARM_AL, ARM_NV
};
extern enum arm_cond_code arm_current_cc;
extern char *arm_condition_codes[];

#define ARM_INVERSE_CONDITION_CODE(X)  ((enum arm_cond_code) (((int)X) ^ 1))

/* This is needed by the tail-calling peepholes */
extern int frame_pointer_needed;


/* Just in case configure has failed to define anything. */
#ifndef TARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT TARGET_CPU_generic
#endif

/* If the configuration file doesn't specify the cpu, the subtarget may
   override it.  If it doesn't, then default to an ARM6. */
#if TARGET_CPU_DEFAULT == TARGET_CPU_generic
#undef TARGET_CPU_DEFAULT
#ifdef SUBTARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT SUBTARGET_CPU_DEFAULT
#else
#define TARGET_CPU_DEFAULT TARGET_CPU_arm6
#endif
#endif

#if TARGET_CPU_DEFAULT == TARGET_CPU_arm2
#define CPP_ARCH_DEFAULT_SPEC "-D__ARM_ARCH_2__"
#else
#if TARGET_CPU_DEFAULT == TARGET_CPU_arm6 || TARGET_CPU_DEFAULT == TARGET_CPU_arm610 || TARGET_CPU_DEFAULT == TARGET_CPU_arm7500fe
#define CPP_ARCH_DEFAULT_SPEC "-D__ARM_ARCH_3__"
#else
#if TARGET_CPU_DEFAULT == TARGET_CPU_arm7m
#define CPP_ARCH_DEFAULT_SPEC "-D__ARM_ARCH_3M__"
#else
#if TARGET_CPU_DEFAULT == TARGET_CPU_arm7tdmi || TARGET_CPU_DEFAULT == TARGET_CPU_arm9
#define CPP_ARCH_DEFAULT_SPEC "-D__ARM_ARCH_4T__"
#else
#if TARGET_CPU_DEFAULT == TARGET_CPU_arm8 || TARGET_CPU_DEFAULT == TARGET_CPU_arm810 || TARGET_CPU_DEFAULT == TARGET_CPU_strongarm
#define CPP_ARCH_DEFAULT_SPEC "-D__ARM_ARCH_4__"
#else
Unrecognized value in TARGET_CPU_DEFAULT.
#endif
#endif
#endif
#endif
#endif

#ifndef CPP_PREDEFINES
#define CPP_PREDEFINES  "-Darm -Acpu(arm) -Amachine(arm)"
#endif

#define CPP_SPEC "\
%(cpp_cpu_arch) %(cpp_apcs_pc) %(cpp_float) \
%(cpp_endian) %(subtarget_cpp_spec)"

/* Set the architecture define -- if -march= is set, then it overrides
   the -mcpu= setting.  */
#define CPP_CPU_ARCH_SPEC "\
%{m2:-D__arm2__ -D__ARM_ARCH_2__} \
%{m3:-D__arm2__ -D__ARM_ARCH_2__} \
%{m6:-D__arm6__ -D__ARM_ARCH_3__} \
%{march=arm2:-D__ARM_ARCH_2__} \
%{march=arm250:-D__ARM_ARCH_2__} \
%{march=arm3:-D__ARM_ARCH_2__} \
%{march=arm6:-D__ARM_ARCH_3__} \
%{march=arm600:-D__ARM_ARCH_3__} \
%{march=arm610:-D__ARM_ARCH_3__} \
%{march=arm7:-D__ARM_ARCH_3__} \
%{march=arm700:-D__ARM_ARCH_3__} \
%{march=arm710:-D__ARM_ARCH_3__} \
%{march=arm7100:-D__ARM_ARCH_3__} \
%{march=arm7500:-D__ARM_ARCH_3__} \
%{march=arm7500fe:-D__ARM_ARCH_3__} \
%{march=arm7m:-D__ARM_ARCH_3M__} \
%{march=arm7dm:-D__ARM_ARCH_3M__} \
%{march=arm7dmi:-D__ARM_ARCH_3M__} \
%{march=arm7tdmi:-D__ARM_ARCH_4T__} \
%{march=arm8:-D__ARM_ARCH_4__} \
%{march=arm810:-D__ARM_ARCH_4__} \
%{march=arm9:-D__ARM_ARCH_4T__} \
%{march=arm920:-D__ARM_ARCH_4__} \
%{march=arm920t:-D__ARM_ARCH_4T__} \
%{march=arm9tdmi:-D__ARM_ARCH_4T__} \
%{march=strongarm:-D__ARM_ARCH_4__} \
%{march=strongarm110:-D__ARM_ARCH_4__} \
%{march=strongarm1100:-D__ARM_ARCH_4__} \
%{march=armv2:-D__ARM_ARCH_2__} \
%{march=armv2a:-D__ARM_ARCH_2__} \
%{march=armv3:-D__ARM_ARCH_3__} \
%{march=armv3m:-D__ARM_ARCH_3M__} \
%{march=armv4:-D__ARM_ARCH_4__} \
%{march=armv4t:-D__ARM_ARCH_4T__} \
%{!march=*: \
 %{mcpu=arm2:-D__ARM_ARCH_2__} \
 %{mcpu=arm250:-D__ARM_ARCH_2__} \
 %{mcpu=arm3:-D__ARM_ARCH_2__} \
 %{mcpu=arm6:-D__ARM_ARCH_3__} \
 %{mcpu=arm600:-D__ARM_ARCH_3__} \
 %{mcpu=arm610:-D__ARM_ARCH_3__} \
 %{mcpu=arm7:-D__ARM_ARCH_3__} \
 %{mcpu=arm700:-D__ARM_ARCH_3__} \
 %{mcpu=arm710:-D__ARM_ARCH_3__} \
 %{mcpu=arm7100:-D__ARM_ARCH_3__} \
 %{mcpu=arm7500:-D__ARM_ARCH_3__} \
 %{mcpu=arm7500fe:-D__ARM_ARCH_3__} \
 %{mcpu=arm7m:-D__ARM_ARCH_3M__} \
 %{mcpu=arm7dm:-D__ARM_ARCH_3M__} \
 %{mcpu=arm7dmi:-D__ARM_ARCH_3M__} \
 %{mcpu=arm7tdmi:-D__ARM_ARCH_4T__} \
 %{mcpu=arm8:-D__ARM_ARCH_4__} \
 %{mcpu=arm810:-D__ARM_ARCH_4__} \
 %{mcpu=arm9:-D__ARM_ARCH_4T__} \
 %{mcpu=arm920:-D__ARM_ARCH_4__} \
 %{mcpu=arm920t:-D__ARM_ARCH_4T__} \
 %{mcpu=arm9tdmi:-D__ARM_ARCH_4T__} \
 %{mcpu=strongarm:-D__ARM_ARCH_4__} \
 %{mcpu=strongarm110:-D__ARM_ARCH_4__} \
 %{mcpu=strongarm1100:-D__ARM_ARCH_4__} \
 %{!mcpu*:%{!m6:%{!m2:%{!m3:%(cpp_cpu_arch_default)}}}}} \
"

/* Define __APCS_26__ if the PC also contains the PSR */
/* This also examines deprecated -m[236] if neither of -mapcs-{26,32} is set,
   ??? Delete this for 2.9.  */
#define CPP_APCS_PC_SPEC "\
%{mapcs-32:%{mapcs-26:%e-mapcs-26 and -mapcs-32 may not be used together} \
 -D__APCS_32__} \
%{mapcs-26:-D__APCS_26__} \
%{!mapcs-32: %{!mapcs-26:%{m6:-D__APCS_32__} %{m2:-D__APCS_26__} \
 %{m3:-D__APCS_26__} %{!m6:%{!m3:%{!m2:%(cpp_apcs_pc_default)}}}}} \
"

#ifndef CPP_APCS_PC_DEFAULT_SPEC
#define CPP_APCS_PC_DEFAULT_SPEC "-D__APCS_26__"
#endif

#define CPP_FLOAT_SPEC "\
%{msoft-float:\
  %{mhard-float:%e-msoft-float and -mhard_float may not be used together} \
  -D__SOFTFP__} \
%{!mhard-float:%{!msoft-float:%(cpp_float_default)}} \
"

/* Default is hard float, which doesn't define anything */
#define CPP_FLOAT_DEFAULT_SPEC ""

#define CPP_ENDIAN_SPEC "\
%{mbig-endian: \
  %{mlittle-endian: \
    %e-mbig-endian and -mlittle-endian may not be used together} \
  -D__ARMEB__ %{mwords-little-endian:-D__ARMWEL__}} \
%{!mlittle-endian:%{!mbig-endian:%(cpp_endian_default)}} \
"

/* Default is little endian, which doesn't define anything. */
#define CPP_ENDIAN_DEFAULT_SPEC ""

/* Translate (for now) the old -m[236] option into the appropriate -mcpu=...
   and -mapcs-xx equivalents. 
   ??? Remove support for this style in 2.9.*/
#define CC1_SPEC "\
%{m2:-mcpu=arm2 -mapcs-26} \
%{m3:-mcpu=arm3 -mapcs-26} \
%{m6:-mcpu=arm6 -mapcs-32} \
"

/* This macro defines names of additional specifications to put in the specs
   that can be used in various specifications like CC1_SPEC.  Its definition
   is an initializer with a subgrouping for each command option.

   Each subgrouping contains a string constant, that defines the
   specification name, and a string constant that used by the GNU CC driver
   program.

   Do not define this macro if it does not need to do anything.  */
#define EXTRA_SPECS						\
  { "cpp_cpu_arch",		CPP_CPU_ARCH_SPEC },		\
  { "cpp_cpu_arch_default",	CPP_ARCH_DEFAULT_SPEC },	\
  { "cpp_apcs_pc",		CPP_APCS_PC_SPEC },		\
  { "cpp_apcs_pc_default",	CPP_APCS_PC_DEFAULT_SPEC },	\
  { "cpp_float",		CPP_FLOAT_SPEC },		\
  { "cpp_float_default",	CPP_FLOAT_DEFAULT_SPEC },	\
  { "cpp_endian",		CPP_ENDIAN_SPEC },		\
  { "cpp_endian_default",	CPP_ENDIAN_DEFAULT_SPEC },	\
  { "subtarget_cpp_spec",	SUBTARGET_CPP_SPEC },           \
  SUBTARGET_EXTRA_SPECS

#define SUBTARGET_EXTRA_SPECS
#define SUBTARGET_CPP_SPEC      ""


/* Run-time Target Specification.  */
#ifndef TARGET_VERSION
#define TARGET_VERSION  \
  fputs (" (ARM/generic)", stderr);
#endif

/* Run-time compilation parameters selecting different hardware subsets.  */
extern int target_flags;

/* The floating point instruction architecture, can be 2 or 3 */
/* CYGNUS LOCAL nickc/renamed from target_fp_name */
extern char * target_fpe_name;
/* END CYGNUS LOCAL */

/* Nonzero if the function prologue (and epilogue) should obey
   the ARM Procedure Call Standard.  */
#define ARM_FLAG_APCS_FRAME	(0x0001)

/* Nonzero if the function prologue should output the function name to enable
   the post mortem debugger to print a backtrace (very useful on RISCOS,
   unused on RISCiX).  Specifying this flag also enables
   -fno-omit-frame-pointer.
   XXX Must still be implemented in the prologue.  */
#define ARM_FLAG_POKE         (0x0002)

/* Nonzero if floating point instructions are emulated by the FPE, in which
   case instruction scheduling becomes very uninteresting.  */
#define ARM_FLAG_FPE          (0x0004)

/* Nonzero if destined for an ARM6xx.  Takes out bits that assume restoration
   of condition flags when returning from a branch & link (ie. a function) */
/* ********* DEPRECATED ******** */
#define ARM_FLAG_ARM6         (0x0008)

/* ********* DEPRECATED ******** */
#define ARM_FLAG_ARM3         (0x0010)

/* Nonzero if destined for a processor in 32-bit program mode.  Takes out bit
   that assume restoration of the condition flags when returning from a
   branch and link (ie a function).  */
#define ARM_FLAG_APCS_32      (0x0020)

/* Nonzero if stack checking should be performed on entry to each function
   which allocates temporary variables on the stack.  */
#define ARM_FLAG_APCS_STACK   (0x0040)

/* Nonzero if floating point parameters should be passed to functions in
   floating point registers.  */
#define ARM_FLAG_APCS_FLOAT   (0x0080)

/* Nonzero if re-entrant, position independent code should be generated.
   This is equivalent to -fpic.  */
#define ARM_FLAG_APCS_REENT   (0x0100)

/* Nonzero if the MMU will trap unaligned word accesses, so shorts must be
   loaded byte-at-a-time.  */
#define ARM_FLAG_SHORT_BYTE   (0x0200)

/* Nonzero if all floating point instructions are missing (and there is no
   emulator either).  Generate function calls for all ops in this case.  */
#define ARM_FLAG_SOFT_FLOAT   (0x0400)

/* Nonzero if we should compile with BYTES_BIG_ENDIAN set to 1.  */
#define ARM_FLAG_BIG_END      (0x0800)

/* Nonzero if we should compile for Thumb interworking.  */
#define ARM_FLAG_THUMB          (0x1000)

/* Nonzero if we should have little-endian words even when compiling for
   big-endian (for backwards compatibility with older versions of GCC).  */
#define ARM_FLAG_LITTLE_WORDS	(0x2000)

/* CYGNUS LOCAL */
/* Nonzero if we need to protect the prolog from scheduling */
#define ARM_FLAG_NO_SCHED_PRO	(0x4000)
/* END CYGNUS LOCAL */

/* Nonzero if a call to abort should be generated if a noreturn 
function tries to return. */
#define ARM_FLAG_ABORT_NORETURN (0x8000)

/* Nonzero if all call instructions should be indirect.  */
#define ARM_FLAG_LONG_CALLS	(0x10000)

#define TARGET_APCS			(target_flags & ARM_FLAG_APCS_FRAME)
#define TARGET_POKE_FUNCTION_NAME	(target_flags & ARM_FLAG_POKE)
#define TARGET_FPE			(target_flags & ARM_FLAG_FPE)
#define TARGET_6			(target_flags & ARM_FLAG_ARM6)
#define TARGET_3			(target_flags & ARM_FLAG_ARM3)
#define TARGET_APCS_32			(target_flags & ARM_FLAG_APCS_32)
#define TARGET_APCS_STACK		(target_flags & ARM_FLAG_APCS_STACK)
#define TARGET_APCS_FLOAT		(target_flags & ARM_FLAG_APCS_FLOAT)
#define TARGET_APCS_REENT		(target_flags & ARM_FLAG_APCS_REENT)
#define TARGET_SHORT_BY_BYTES		(target_flags & ARM_FLAG_SHORT_BYTE)
#define TARGET_SOFT_FLOAT		(target_flags & ARM_FLAG_SOFT_FLOAT)
#define TARGET_HARD_FLOAT		(! TARGET_SOFT_FLOAT)
#define TARGET_BIG_END			(target_flags & ARM_FLAG_BIG_END)
#define TARGET_THUMB_INTERWORK		(target_flags & ARM_FLAG_THUMB)
#define TARGET_LITTLE_WORDS		(target_flags & ARM_FLAG_LITTLE_WORDS)
/* CYGNUS LOCAL */
#define TARGET_NO_SCHED_PRO		(target_flags & ARM_FLAG_NO_SCHED_PRO)
/* END CYGNUS LOCAL */
#define TARGET_ABORT_NORETURN           (target_flags & ARM_FLAG_ABORT_NORETURN)
#define TARGET_LONG_CALLS		(target_flags & ARM_FLAG_LONG_CALLS)

/* SUBTARGET_SWITCHES is used to add flags on a per-config basis.
   Bit 31 is reserved.  See riscix.h.  */
#ifndef SUBTARGET_SWITCHES
#define SUBTARGET_SWITCHES
#endif

#define TARGET_SWITCHES  				\
{                         				\
  {"apcs",			ARM_FLAG_APCS_FRAME, "" }, \
  {"apcs-frame",		ARM_FLAG_APCS_FRAME, 	\
     "Generate APCS conformant stack frames" },		\
  {"no-apcs-frame",	       -ARM_FLAG_APCS_FRAME, "" }, \
  {"poke-function-name",	ARM_FLAG_POKE, 		\
     "Store function names in object code" },		\
  {"fpe",			ARM_FLAG_FPE,  "" },	\
  {"6",				ARM_FLAG_ARM6, "" },	\
  {"2",				ARM_FLAG_ARM3, "" },	\
  {"3",				ARM_FLAG_ARM3, "" },	\
  {"apcs-32",			ARM_FLAG_APCS_32, 	\
     "Use the 32bit version of the APCS" },		\
  {"apcs-26",		       -ARM_FLAG_APCS_32, 	\
     "Use the 26bit version of the APCS" },		\
  {"apcs-stack-check",		ARM_FLAG_APCS_STACK, "" }, \
  {"no-apcs-stack-check",      -ARM_FLAG_APCS_STACK, "" }, \
  {"apcs-float",		ARM_FLAG_APCS_FLOAT, 	\
     "Pass FP arguments in FP registers" },		\
  {"no-apcs-float",	       -ARM_FLAG_APCS_FLOAT, "" }, \
  {"apcs-reentrant",		ARM_FLAG_APCS_REENT, 	\
     "Generate re-entrant, PIC code" },			\
  {"no-apcs-reentrant",	       -ARM_FLAG_APCS_REENT, "" }, \
  {"short-load-bytes",		ARM_FLAG_SHORT_BYTE, 	\
     "Load shorts a byte at a time" },			\
  {"no-short-load-bytes",      -ARM_FLAG_SHORT_BYTE, "" }, \
  {"short-load-words",	       -ARM_FLAG_SHORT_BYTE, 	\
     "Load words a byte at a time" },			\
  {"no-short-load-words",	ARM_FLAG_SHORT_BYTE, "" }, \
  {"soft-float",		ARM_FLAG_SOFT_FLOAT, 	\
     "Use library calls to perform FP operations" },	\
  {"hard-float",	       -ARM_FLAG_SOFT_FLOAT, 	\
     "Use hardware floating point instructions" },	\
  {"big-endian",		ARM_FLAG_BIG_END, 	\
     "Assume target CPU is configured as big endian" },	\
  {"little-endian",	       -ARM_FLAG_BIG_END, 	\
     "Assume target CPU is configured as little endian" }, \
  {"words-little-endian",       ARM_FLAG_LITTLE_WORDS, 	\
     "Assume big endian bytes, little endian words" },	\
  {"thumb-interwork",		ARM_FLAG_THUMB, 	\
     "Support calls between THUMB and ARM instructions sets" },	\
  {"no-thumb-interwork",       -ARM_FLAG_THUMB, "" },	\
  {"abort-on-noreturn",         ARM_FLAG_ABORT_NORETURN,     \
   "Generate a call to abort if a noreturn function returns"}, \
  {"no-abort-on-noreturn",      -ARM_FLAG_ABORT_NORETURN, ""}, \
  /* CYGNUS LOCAL */					\
  {"sched-prolog",             -ARM_FLAG_NO_SCHED_PRO, 	\
     "Do not move instructions into a function's prologue" }, \
  {"no-sched-prolog",           ARM_FLAG_NO_SCHED_PRO, "" }, \
  /* END CYGNUS LOCAL */				\
  {"long-calls",		ARM_FLAG_LONG_CALLS,	\
   "Generate all call instructions as indirect calls"},	\
  {"no-long-calls",	       -ARM_FLAG_LONG_CALLS, ""}, \
  SUBTARGET_SWITCHES					\
  {"",				TARGET_DEFAULT }	\
}

#define TARGET_OPTIONS			\
{					\
  {"cpu=",  & arm_select[0].string,				\
     "Specify the name of the target CPU" },			\
  {"arch=", & arm_select[1].string,				\
     "Specify the name of the target architecture" }, 		\
  {"tune=", & arm_select[2].string, "" }, 			\
  {"fpe=",  & target_fpe_name, "" }, 				\
  {"fp=",   & target_fpe_name,					\
     "Specify the version of the floating point emulator" },	\
  { "structure-size-boundary=", & structure_size_string, 	\
      "Specify the minumum bit alignment of structures" } 	\
}

struct arm_cpu_select
{
  char *              string;
  char *              name;
  struct processors * processors;
};

/* This is a magic array.  If the user specifies a command line switch
   which matches one of the entries in TARGET_OPTIONS then the corresponding
   string pointer will be set to the value specified by the user.  */
extern struct arm_cpu_select arm_select[];

enum prog_mode_type
{
  prog_mode26,
  prog_mode32
};

/* Recast the program mode class to be the prog_mode attribute */
#define arm_prog_mode (arm_prgmode)

extern enum prog_mode_type arm_prgmode;

/* What sort of floating point unit do we have? Hardware or software.
   If software, is it issue 2 or issue 3?  */
enum floating_point_type
{
  FP_HARD,
  FP_SOFT2,
  FP_SOFT3
};

/* Recast the floating point class to be the floating point attribute.  */
#define arm_fpu_attr ((enum attr_fpu) arm_fpu)

/* What type of floating point to tune for */
extern enum floating_point_type arm_fpu;

/* What type of floating point instructions are available */
extern enum floating_point_type arm_fpu_arch;

/* Default floating point architecture.  Override in sub-target if
   necessary.  */
#define FP_DEFAULT FP_SOFT2

/* Nonzero if the processor has a fast multiply insn, and one that does
   a 64-bit multiply of two 32-bit values.  */
extern int arm_fast_multiply;

/* Nonzero if this chip supports the ARM Architecture 4 extensions */
extern int arm_arch4;

/* CYGNUS LOCAL nickc/load scheduling */
/* Nonzero if this chip can benefit from load scheduling.  */
extern int arm_ld_sched;
/* END CYGNUS LOCAL */

/* Nonzero if this chip is a StrongARM.  */
extern int arm_is_strong;

/* Nonzero if this chip is a an ARM6 or an ARM7.  */
extern int arm_is_6_or_7;

#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT  0
#endif

/* The frame pointer register used in gcc has nothing to do with debugging;
   that is controlled by the APCS-FRAME option.  */
/* Not fully implemented yet */
/* #define CAN_DEBUG_WITHOUT_FP 1 */

#define TARGET_MEM_FUNCTIONS 1

#define OVERRIDE_OPTIONS  arm_override_options ()

/* Target machine storage Layout.  */


/* Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases,
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type.  */

/* It is far faster to zero extend chars than to sign extend them */

#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE)  \
  if (GET_MODE_CLASS (MODE) == MODE_INT		\
      && GET_MODE_SIZE (MODE) < 4)      	\
    {						\
      if (MODE == QImode)			\
	UNSIGNEDP = 1;				\
      else if (MODE == HImode)			\
	UNSIGNEDP = TARGET_SHORT_BY_BYTES != 0;	\
      (MODE) = SImode;				\
    }

/* Define this macro if the promotion described by `PROMOTE_MODE'
   should also be done for outgoing function arguments.  */
/* This is required to ensure that push insns always push a word.  */
#define PROMOTE_FUNCTION_ARGS

/* Define for XFmode extended real floating point support.
   This will automatically cause REAL_ARITHMETIC to be defined.  */
/* For the ARM:
   I think I have added all the code to make this work.  Unfortunately,
   early releases of the floating point emulation code on RISCiX used a
   different format for extended precision numbers.  On my RISCiX box there
   is a bug somewhere which causes the machine to lock up when running enquire
   with long doubles.  There is the additional aspect that Norcroft C
   treats long doubles as doubles and we ought to remain compatible.
   Perhaps someone with an FPA coprocessor and not running RISCiX would like
   to try this someday. */
/* #define LONG_DOUBLE_TYPE_SIZE 96 */

/* Disable XFmode patterns in md file */
#define ENABLE_XF_PATTERNS 0

/* Define if you don't want extended real, but do want to use the
   software floating point emulator for REAL_ARITHMETIC and
   decimal <-> binary conversion. */
/* See comment above */
#define REAL_ARITHMETIC

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.  */
#define BITS_BIG_ENDIAN  0

/* Define this if most significant byte of a word is the lowest numbered.  
   Most ARM processors are run in little endian mode, so that is the default.
   If you want to have it run-time selectable, change the definition in a
   cover file to be TARGET_BIG_ENDIAN.  */
#define BYTES_BIG_ENDIAN  (TARGET_BIG_END != 0)

/* Define this if most significant word of a multiword number is the lowest
   numbered.
   This is always false, even when in big-endian mode.  */
#define WORDS_BIG_ENDIAN  (BYTES_BIG_ENDIAN && ! TARGET_LITTLE_WORDS)

/* LIBGCC2_WORDS_BIG_ENDIAN has to be a constant, so we define this based
   on processor pre-defineds when compiling libgcc2.c.  */
#if defined(__ARMEB__) && !defined(__ARMWEL__)
#define LIBGCC2_WORDS_BIG_ENDIAN 1
#else
#define LIBGCC2_WORDS_BIG_ENDIAN 0
#endif

/* Define this if most significant word of doubles is the lowest numbered.
   This is always true, even when in little-endian mode.  */
#define FLOAT_WORDS_BIG_ENDIAN 1

/* Number of bits in an addressable storage unit */
#define BITS_PER_UNIT  8

#define BITS_PER_WORD  32

#define UNITS_PER_WORD	4

#define POINTER_SIZE  32

#define PARM_BOUNDARY  	32

#define STACK_BOUNDARY  32

#define FUNCTION_BOUNDARY  32

#define EMPTY_FIELD_BOUNDARY  32

#define BIGGEST_ALIGNMENT  32

/* Make strings word-aligned so strcpy from constants will be faster.  */
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  \
  (TREE_CODE (EXP) == STRING_CST        \
   && (ALIGN) < BITS_PER_WORD ? BITS_PER_WORD : (ALIGN))

/* Every structures size must be a multiple of 32 bits.  */
/* This is for compatibility with ARMCC.  ARM SDT Reference Manual
   (ARM DUI 0020D) page 2-20 says "Structures are aligned on word
   boundaries".  */
#ifndef STRUCTURE_SIZE_BOUNDARY
#define STRUCTURE_SIZE_BOUNDARY 32
#endif

/* Used when parsing command line option -mstructure_size_boundary.  */
extern char * structure_size_string;

/* Non-zero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1

#define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT


/* Standard register usage.  */

/* Register allocation in ARM Procedure Call Standard (as used on RISCiX):
   (S - saved over call).

	r0	   *	argument word/integer result
	r1-r3		argument word

	r4-r8	     S	register variable
	r9	     S	(rfp) register variable (real frame pointer)
				CYGNUS LOCAL nickc/comment change
	r10  	   F S	(sl) stack limit (used by -mapcs-stack-check)
				END CYGNUS LOCAL
	r11 	   F S	(fp) argument pointer
	r12		(ip) temp workspace
	r13  	   F S	(sp) lower end of current stack frame
	r14		(lr) link address/workspace
	r15	   F	(pc) program counter

	f0		floating point result
	f1-f3		floating point scratch

	f4-f7	     S	floating point variable

	cc		This is NOT a real register, but is used internally
	                to represent things that use or set the condition
			codes.
	sfp             This isn't either.  It is used during rtl generation
	                since the offset between the frame pointer and the
			auto's isn't known until after register allocation.
	afp		Nor this, we only need this because of non-local
	                goto.  Without it fp appears to be used and the
			elimination code won't get rid of sfp.  It tracks
			fp exactly at all times.

   *: See CONDITIONAL_REGISTER_USAGE  */

/* The stack backtrace structure is as follows:
  fp points to here:  |  save code pointer  |      [fp]
                      |  return link value  |      [fp, #-4]
                      |  return sp value    |      [fp, #-8]
                      |  return fp value    |      [fp, #-12]
                     [|  saved r10 value    |]
                     [|  saved r9 value     |]
                     [|  saved r8 value     |]
                     [|  saved r7 value     |]
                     [|  saved r6 value     |]
                     [|  saved r5 value     |]
                     [|  saved r4 value     |]
                     [|  saved r3 value     |]
                     [|  saved r2 value     |]
                     [|  saved r1 value     |]
                     [|  saved r0 value     |]
                     [|  saved f7 value     |]     three words
                     [|  saved f6 value     |]     three words
                     [|  saved f5 value     |]     three words
                     [|  saved f4 value     |]     three words
  r0-r3 are not normally saved in a C function.  */

/* The number of hard registers is 16 ARM + 8 FPU + 1 CC + 1 SFP.  */
#define FIRST_PSEUDO_REGISTER  27

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.  */
#define FIXED_REGISTERS  \
{                        \
  0,0,0,0,0,0,0,0,	 \
  0,0,1,1,0,1,0,1,	 \
  0,0,0,0,0,0,0,0,	 \
  1,1,1			 \
}

/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.
   The CC is not preserved over function calls on the ARM 6, so it is 
   easier to assume this for all.  SFP is preserved, since FP is. */
#define CALL_USED_REGISTERS  \
{                            \
  1,1,1,1,0,0,0,0,	     \
  0,0,1,1,1,1,1,1,	     \
  1,1,1,1,0,0,0,0,	     \
  1,1,1			     \
}

/* If doing stupid life analysis, avoid a bug causing a return value r0 to be
   trampled.  This effectively reduces the number of available registers by 1.
   XXX It is a hack, I know.
   XXX Is this still needed?  */
#define CONDITIONAL_REGISTER_USAGE  \
{							\
  if (obey_regdecls)					\
    fixed_regs[0] = 1;					\
  if (TARGET_SOFT_FLOAT)				\
    {							\
      int regno;					\
      for (regno = 16; regno < 24; ++regno)		\
	fixed_regs[regno] = call_used_regs[regno] = 1;	\
    }							\
  if (flag_pic)						\
    {							\
      fixed_regs[PIC_OFFSET_TABLE_REGNUM] = 1;		\
      call_used_regs[PIC_OFFSET_TABLE_REGNUM] = 0;	\
    }							\
  /* CYGNUS LOCAL */					\
  else if (! TARGET_APCS_STACK)				\
    {							\
      fixed_regs[10]     = 0;				\
      call_used_regs[10] = 0;				\
    }							\
  /* END CYGNUS LOCAL */				\
}

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.

   On the ARM regs are UNITS_PER_WORD bits wide; FPU regs can hold any FP
   mode.  */
#define HARD_REGNO_NREGS(REGNO, MODE)  					\
    (((REGNO) >= 16 && REGNO != FRAME_POINTER_REGNUM			\
      && (REGNO) != ARG_POINTER_REGNUM) ? 1				\
     : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
   This is TRUE for ARM regs since they can hold anything, and TRUE for FPU
   regs holding FP.  */
#define HARD_REGNO_MODE_OK(REGNO, MODE)  			\
  ((GET_MODE_CLASS (MODE) == MODE_CC) ? (REGNO == CC_REGNUM) :	\
  ((REGNO) < 16 || REGNO == FRAME_POINTER_REGNUM		\
   || REGNO == ARG_POINTER_REGNUM				\
   || GET_MODE_CLASS (MODE) == MODE_FLOAT))

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2)  \
  (GET_MODE_CLASS (MODE1) == GET_MODE_CLASS (MODE2))

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */

/* Define this if the program counter is overloaded on a register.  */
#define PC_REGNUM		15

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM	13

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM	25

/* Define this to be where the real frame pointer is if it is not possible to
   work out the offset between the frame pointer and the automatic variables
   until after register allocation has taken place.  FRAME_POINTER_REGNUM
   should point to a special register that we will make sure is eliminated. */
#define HARD_FRAME_POINTER_REGNUM 11

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms may be accessed
   via the stack pointer) in functions that seem suitable.  
   If we have to have a frame pointer we might as well make use of it.
   APCS says that the frame pointer does not need to be pushed in leaf
   functions, or simple tail call functions.  */
/* CYGNUS LOCAL */
#define FRAME_POINTER_REQUIRED					\
  (current_function_has_nonlocal_label 				\
   || (TARGET_APCS && (! leaf_function_p () && ! can_tail_call_optimise ())))

extern int can_tail_call_optimise ();
/* END CYGNUS LOCAL */

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM	26

/* The native (Norcroft) Pascal compiler for the ARM passes the static chain
   as an invisible last argument (possible since varargs don't exist in
   Pascal), so the following is not true.  */
#define STATIC_CHAIN_REGNUM	8

/* Register in which address to store a structure value
   is passed to a function.  */
#define STRUCT_VALUE_REGNUM	0

/* Internal, so that we don't need to refer to a raw number */
#define CC_REGNUM		24

/* The order in which register should be allocated.  It is good to use ip
   since no saving is required (though calls clobber it) and it never contains
   function parameters.  It is quite good to use lr since other calls may
   clobber it anyway.  Allocate r0 through r3 in reverse order since r3 is 
   least likely to contain a function parameter; in addition results are
   returned in r0.
   */
#define REG_ALLOC_ORDER  	    \
{                                   \
     3,  2,  1,  0, 12, 14,  4,  5, \
     6,  7,  8, 10,  9, 11, 13, 15, \
    16, 17, 18, 19, 20, 21, 22, 23, \
    24, 25, 26			    \
}

/* Register and constant classes.  */

/* Register classes: all ARM regs or all FPU regs---simple! */
enum reg_class
{
  NO_REGS,
  FPU_REGS,
  GENERAL_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define N_REG_CLASSES  (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES  \
{			\
  "NO_REGS",		\
  "FPU_REGS",		\
  "GENERAL_REGS",	\
  "ALL_REGS",		\
}

/* Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  */
#define REG_CLASS_CONTENTS  \
{				\
  0x0000000, /* NO_REGS  */	\
  0x0FF0000, /* FPU_REGS */	\
  0x200FFFF, /* GENERAL_REGS */	\
  0x2FFFFFF  /* ALL_REGS */	\
}

/* The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  */
#define REGNO_REG_CLASS(REGNO)  			\
  (((REGNO) < 16 || REGNO == FRAME_POINTER_REGNUM	\
    || REGNO == ARG_POINTER_REGNUM)			\
   ? GENERAL_REGS : (REGNO) == CC_REGNUM		\
   ? NO_REGS : FPU_REGS)

/* The class value for index registers, and the one for base regs.  */
#define INDEX_REG_CLASS  GENERAL_REGS
#define BASE_REG_CLASS	GENERAL_REGS

/* Get reg_class from a letter such as appears in the machine description.
   We only need constraint `f' for FPU_REGS (`r' == GENERAL_REGS).  */
#define REG_CLASS_FROM_LETTER(C)  \
  ((C)=='f' ? FPU_REGS : NO_REGS)

/* The letters I, J, K, L and M in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.
	I: immediate arithmetic operand (i.e. 8 bits shifted as required).
	J: valid indexing constants.  
	K: ~value ok in rhs argument of data operand.
	L: -value ok in rhs argument of data operand. 
        M: 0..32, or a power of 2  (for shifts, or mult done by shift).  */
#define CONST_OK_FOR_LETTER_P(VALUE, C)  		\
  ((C) == 'I' ? const_ok_for_arm (VALUE) :		\
   (C) == 'J' ? ((VALUE) < 4096 && (VALUE) > -4096) :	\
   (C) == 'K' ? (const_ok_for_arm (~(VALUE))) :		\
   (C) == 'L' ? (const_ok_for_arm (-(VALUE))) :		\
   (C) == 'M' ? (((VALUE >= 0 && VALUE <= 32))		\
		 || (((VALUE) & ((VALUE) - 1)) == 0))	\
   : 0)

/* For the ARM, `Q' means that this is a memory operand that is just
   an offset from a register.  
   `S' means any symbol that has the SYMBOL_REF_FLAG set or a CONSTANT_POOL
   address.  This means that the symbol is in the text segment and can be
   accessed without using a load. */

#define EXTRA_CONSTRAINT(OP, C)						    \
  ((C) == 'Q' ? GET_CODE (OP) == MEM && GET_CODE (XEXP (OP, 0)) == REG	    \
   : (C) == 'R' ? (GET_CODE (OP) == MEM					    \
		   && GET_CODE (XEXP (OP, 0)) == SYMBOL_REF		    \
		   && CONSTANT_POOL_ADDRESS_P (XEXP (OP, 0)))		    \
   : (C) == 'S' ? (optimize > 0 && CONSTANT_ADDRESS_P (OP))		    \
   : 0)

/* Constant letter 'G' for the FPU immediate constants. 
   'H' means the same constant negated.  */
#define CONST_DOUBLE_OK_FOR_LETTER_P(X,C)			\
    ((C) == 'G' ? const_double_rtx_ok_for_fpu (X) 		\
     : (C) == 'H' ? neg_const_double_rtx_ok_for_fpu (X) : 0)

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */
#define PREFERRED_RELOAD_CLASS(X, CLASS)  (CLASS)

/* Return the register class of a scratch register needed to copy IN into
   or out of a register in CLASS in MODE.  If it can be done directly,
   NO_REGS is returned.  */
#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS,MODE,X)		\
  (((MODE) == HImode && ! arm_arch4 && true_regnum (X) == -1)	\
   ? GENERAL_REGS : NO_REGS)

/* If we need to load shorts byte-at-a-time, then we need a scratch. */
#define SECONDARY_INPUT_RELOAD_CLASS(CLASS,MODE,X)		\
  (((MODE) == HImode && ! arm_arch4 && TARGET_SHORT_BY_BYTES	\
    && (GET_CODE (X) == MEM					\
	|| ((GET_CODE (X) == REG || GET_CODE (X) == SUBREG)	\
	    && true_regnum (X) == -1)))				\
   ? GENERAL_REGS : NO_REGS)

/* Try a machine-dependent way of reloading an illegitimate address
   operand.  If we find one, push the reload and jump to WIN.  This
   macro is used in only one place: `find_reloads_address' in reload.c.

   For the ARM, we wish to handle large displacements off a base
   register by splitting the addend across a MOV and the mem insn.
   This can cut the number of reloads needed. */
#define LEGITIMIZE_RELOAD_ADDRESS(X,MODE,OPNUM,TYPE,IND_LEVELS,WIN)	\
do {									\
  if (GET_CODE (X) == PLUS						\
      && GET_CODE (XEXP (X, 0)) == REG					\
      && REGNO (XEXP (X, 0)) < FIRST_PSEUDO_REGISTER			\
      && REG_MODE_OK_FOR_BASE_P (XEXP (X, 0), MODE)			\
      && GET_CODE (XEXP (X, 1)) == CONST_INT)				\
    {									\
      HOST_WIDE_INT val = INTVAL (XEXP (X, 1));				\
      HOST_WIDE_INT low, high;						\
									\
      if (MODE == DImode || (TARGET_SOFT_FLOAT && MODE == DFmode))	\
	low = ((val & 0xf) ^ 0x8) - 0x8;				\
      else if (MODE == SImode || MODE == QImode				\
	       || (MODE == SFmode && TARGET_SOFT_FLOAT)			\
	       || (MODE == HImode && ! arm_arch4))			\
	/* Need to be careful, -4096 is not a valid offset */		\
	low = val >= 0 ? (val & 0xfff) : -((-val) & 0xfff);		\
      else if (MODE == HImode && arm_arch4)				\
	/* Need to be careful, -256 is not a valid offset */		\
	low = val >= 0 ? (val & 0xff) : -((-val) & 0xff);		\
      else if (GET_MODE_CLASS (MODE) == MODE_FLOAT			\
	       && TARGET_HARD_FLOAT)					\
	/* Need to be careful, -1024 is not a valid offset */		\
	low = val >= 0 ? (val & 0x3ff) : -((-val) & 0x3ff);		\
      else								\
	break;								\
									\
      high = ((((val - low) & 0xffffffff) ^ 0x80000000) - 0x80000000);	\
      /* Check for overflow or zero */					\
      if (low == 0 || high == 0 || (high + low != val))			\
	break;								\
									\
      /* Reload the high part into a base reg; leave the low part	\
	 in the mem.  */						\
      X = gen_rtx_PLUS (GET_MODE (X),					\
			gen_rtx_PLUS (GET_MODE (X), XEXP (X, 0),	\
				      GEN_INT (high)),			\
			GEN_INT (low));					\
      push_reload (XEXP (X, 0), NULL_RTX, &XEXP (X, 0), NULL_PTR,	\
		   BASE_REG_CLASS, GET_MODE (X), VOIDmode, 0, 0,	\
		   OPNUM, TYPE);					\
      goto WIN;								\
    }									\
} while (0)

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.
   ARM regs are UNITS_PER_WORD bits while FPU regs can hold any FP mode */
#define CLASS_MAX_NREGS(CLASS, MODE)  \
    ((CLASS) == FPU_REGS ? 1					       \
     : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* Moves between FPU_REGS and GENERAL_REGS are two memory insns.  */
#define REGISTER_MOVE_COST(CLASS1, CLASS2)  \
  ((((CLASS1) == FPU_REGS && (CLASS2) != FPU_REGS)	\
    || ((CLASS2) == FPU_REGS && (CLASS1) != FPU_REGS))	\
   ? 20 : 2)

/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD  1

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD 1

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET  0

/* If we generate an insn to push BYTES bytes,
   this says how many the stack pointer really advances by.  */
/* The push insns do not do this rounding implicitly.  So don't define this. */
/* #define PUSH_ROUNDING(NPUSHED)  (((NPUSHED) + 3) & ~3) */

/* Define this if the maximum size of all the outgoing args is to be
   accumulated and pushed during the prologue.  The amount can be
   found in the variable current_function_outgoing_args_size.  */
#define ACCUMULATE_OUTGOING_ARGS

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL)  4

/* Value is the number of byte of arguments automatically
   popped when returning from a subroutine call.
   FUNDECL is the declaration node of the function (as a tree),
   FUNTYPE is the data type of the function (as a tree),
   or for a library call it is an identifier node for the subroutine name.
   SIZE is the number of bytes of arguments passed on the stack.

   On the ARM, the caller does not pop any of its arguments that were passed
   on the stack.  */
#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE)  0

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */
#define FUNCTION_VALUE(VALTYPE, FUNC)  \
  (GET_MODE_CLASS (TYPE_MODE (VALTYPE)) == MODE_FLOAT && TARGET_HARD_FLOAT \
   ? gen_rtx (REG, TYPE_MODE (VALTYPE), 16) \
   : gen_rtx (REG, TYPE_MODE (VALTYPE), 0))

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */
#define LIBCALL_VALUE(MODE)  \
  (GET_MODE_CLASS (MODE) == MODE_FLOAT && TARGET_HARD_FLOAT \
   ? gen_rtx (REG, MODE, 16) \
   : gen_rtx (REG, MODE, 0))

/* 1 if N is a possible register number for a function value.
   On the ARM, only r0 and f0 can return results.  */
#define FUNCTION_VALUE_REGNO_P(REGNO)  \
  ((REGNO) == 0 || ((REGNO) == 16) && TARGET_HARD_FLOAT)

/* How large values are returned */
/* A C expression which can inhibit the returning of certain function values
   in registers, based on the type of value. */
/* CYGNUS LOCAL */     
#define RETURN_IN_MEMORY(TYPE) arm_return_in_memory (TYPE)
/* END CYGNUS LOCAL */

/* Define DEFAULT_PCC_STRUCT_RETURN to 1 if all structure and union return
   values must be in memory.  On the ARM, they need only do so if larger
   than a word, or if they contain elements offset from zero in the struct. */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* Define where to put the arguments to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
    This is null for libcalls where that information may
    not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
    (otherwise it is an extra parameter matching an ellipsis).

   On the ARM, normally the first 16 bytes are passed in registers r0-r3; all
   other arguments are passed on the stack.  If (NAMED == 0) (which happens
   only in assign_parms, since SETUP_INCOMING_VARARGS is defined), say it is
   passed in the stack (function_prologue will indeed make it pass in the
   stack if necessary).  */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)  \
  ((NAMED)						\
   ? ((CUM) >= 16 ? 0 : gen_rtx (REG, MODE, (CUM) / 4))	\
   : 0)

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.  */
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED)  \
  ((CUM) < 16 && 16 < (CUM) + ((MODE) != BLKmode            \
			       ? GET_MODE_SIZE (MODE)       \
			       : int_size_in_bytes (TYPE))  \
   ? 4 - (CUM) / 4 : 0)

/* A C type for declaring a variable that is used as the first argument of
   `FUNCTION_ARG' and other related values.  For some target machines, the
   type `int' suffices and can hold the number of bytes of argument so far.

   On the ARM, this is the number of bytes of arguments scanned so far.  */
#define CUMULATIVE_ARGS  int

/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.
   On the ARM, the offset starts at 0.  */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT)  \
  ((CUM) = (((FNTYPE) && aggregate_value_p (TREE_TYPE ((FNTYPE)))) ? 4 : 0))

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)  \
  (CUM) += ((MODE) != BLKmode                       \
	    ? (GET_MODE_SIZE (MODE) + 3) & ~3       \
	    : (int_size_in_bytes (TYPE) + 3) & ~3)  \

/* 1 if N is a possible register number for function argument passing.
   On the ARM, r0-r3 are used to pass args.  */
#define FUNCTION_ARG_REGNO_P(REGNO)  \
  ((REGNO) >= 0 && (REGNO) <= 3)

/* Perform any actions needed for a function that is receiving a variable
   number of arguments.  CUM is as above.  MODE and TYPE are the mode and type
   of the current parameter.  PRETEND_SIZE is a variable that should be set to
   the amount of stack that must be pushed by the prolog to pretend that our
   caller pushed it.

   Normally, this macro will push all remaining incoming registers on the
   stack and set PRETEND_SIZE to the length of the registers pushed.

   On the ARM, PRETEND_SIZE is set in order to have the prologue push the last
   named arg and all anonymous args onto the stack.
   XXX I know the prologue shouldn't be pushing registers, but it is faster
   that way.  */
#define SETUP_INCOMING_VARARGS(CUM, MODE, TYPE, PRETEND_SIZE, NO_RTL)  \
{									\
  extern int current_function_anonymous_args;				\
  current_function_anonymous_args = 1;					\
  if ((CUM) < 16)							\
    (PRETEND_SIZE) = 16 - (CUM);					\
}

/* Generate assembly output for the start of a function.  */
#define FUNCTION_PROLOGUE(STREAM, SIZE)  \
  output_func_prologue ((STREAM), (SIZE))

/* Call the function profiler with a given profile label.  The Acorn compiler
   puts this BEFORE the prolog but gcc puts it afterwards.  The ``mov ip,lr''
   seems like a good idea to stick with cc convention.  ``prof'' doesn't seem
   to mind about this!  */
#define FUNCTION_PROFILER(STREAM,LABELNO)  				    \
{									    \
    fprintf(STREAM, "\tmov\t%sip, %slr\n", REGISTER_PREFIX, REGISTER_PREFIX); \
    fprintf(STREAM, "\tbl\tmcount\n");					    \
    fprintf(STREAM, "\t.word\tLP%d\n", (LABELNO));			    \
}

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.

   On the ARM, the function epilogue recovers the stack pointer from the
   frame.  */
#define EXIT_IGNORE_STACK 1

/* Generate the assembly code for function exit. */
#define FUNCTION_EPILOGUE(STREAM, SIZE)  \
  output_func_epilogue ((STREAM), (SIZE))

/* Determine if the epilogue should be output as RTL.
   You should override this if you define FUNCTION_EXTRA_EPILOGUE.  */
#define USE_RETURN_INSN(ISCOND) use_return_insn (ISCOND)

/* Definitions for register eliminations.

   This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.

   We have two registers that can be eliminated on the ARM.  First, the
   arg pointer register can often be eliminated in favor of the stack
   pointer register.  Secondly, the pseudo frame pointer register can always
   be eliminated; it is replaced with either the stack or the real frame
   pointer. */

#define ELIMINABLE_REGS					\
{{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
 {ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},	\
 {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
 {FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM}}

/* Given FROM and TO register numbers, say whether this elimination is allowed.
   Frame pointer elimination is automatically handled.

   All eliminations are permissible.  Note that ARG_POINTER_REGNUM and
   HARD_FRAME_POINTER_REGNUM are in fact the same thing.  If we need a frame
   pointer, we must eliminate FRAME_POINTER_REGNUM into
   HARD_FRAME_POINTER_REGNUM and not into STACK_POINTER_REGNUM.  */
#define CAN_ELIMINATE(FROM, TO)		\
  (((TO) == STACK_POINTER_REGNUM && frame_pointer_needed) ? 0 : 1)

/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
{									\
  int volatile_func = arm_volatile_func ();				\
  if ((FROM) == ARG_POINTER_REGNUM && (TO) == HARD_FRAME_POINTER_REGNUM)\
    (OFFSET) = 0;							\
  else if ((FROM) == FRAME_POINTER_REGNUM				\
	   && (TO) == STACK_POINTER_REGNUM)				\
    (OFFSET) = (current_function_outgoing_args_size			\
		+ (get_frame_size () + 3 & ~3));			\
  else									\
    {									\
      int regno;							\
      int offset = 12;							\
      int saved_hard_reg = 0;						\
									\
      if (! volatile_func)						\
        {								\
          for (regno = 0; regno <= 10; regno++)				\
	    if (regs_ever_live[regno] && ! call_used_regs[regno])	\
	      saved_hard_reg = 1, offset += 4;				\
          for (regno = 16; regno <=23; regno++)				\
	    if (regs_ever_live[regno] && ! call_used_regs[regno])	\
	      offset += 12;						\
	}								\
      if ((FROM) == FRAME_POINTER_REGNUM)				\
	(OFFSET) = -offset;						\
      else								\
	{								\
	   if (! frame_pointer_needed)					\
	     offset -= 16;						\
	   if (! volatile_func						\
	       && (regs_ever_live[14] || saved_hard_reg)) 		\
	     offset += 4;						\
	   offset += current_function_outgoing_args_size;		\
	   (OFFSET) = (get_frame_size () + 3 & ~3) + offset;		\
         }								\
    }									\
}

/* CYGNUS LOCAL */
/* Special case handling of the location of arguments passed on the stack.  */
#define DEBUGGER_ARG_OFFSET(value, addr) value ? value : arm_debugger_arg_offset (value, addr)
/* END CYGNUS LOCAL */

/* Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.

   On the ARM, (if r8 is the static chain regnum, and remembering that
   referencing pc adds an offset of 8) the trampoline looks like:
	   ldr 		r8, [pc, #0]
	   ldr		pc, [pc]
	   .word	static chain value
	   .word	function's address
   ??? FIXME: When the trampoline returns, r8 will be clobbered.  */
#define TRAMPOLINE_TEMPLATE(FILE)				\
{								\
  fprintf ((FILE), "\tldr\t%sr8, [%spc, #0]\n",			\
	   REGISTER_PREFIX, REGISTER_PREFIX);			\
  fprintf ((FILE), "\tldr\t%spc, [%spc, #0]\n",			\
	   REGISTER_PREFIX, REGISTER_PREFIX);			\
  fprintf ((FILE), "\t.word\t0\n");				\
  fprintf ((FILE), "\t.word\t0\n");				\
}

/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE  16

/* Alignment required for a trampoline in units.  */
#define TRAMPOLINE_ALIGN  4

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */
#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)  \
{									\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 8)),	\
		  (CXT));						\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 12)),	\
		  (FNADDR));						\
}


/* Addressing modes, and classification of registers for them.  */

#define HAVE_POST_INCREMENT  1
#define HAVE_PRE_INCREMENT  1
#define HAVE_POST_DECREMENT  1
#define HAVE_PRE_DECREMENT  1

/* Macros to check register numbers against specific register classes.  */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.

   On the ARM, don't allow the pc to be used.  */
#define REGNO_OK_FOR_BASE_P(REGNO)				\
  ((REGNO) < 15 || (REGNO) == FRAME_POINTER_REGNUM		\
   || (REGNO) == ARG_POINTER_REGNUM				\
   || (unsigned) reg_renumber[(REGNO)] < 15			\
   || (unsigned) reg_renumber[(REGNO)] == FRAME_POINTER_REGNUM	\
   || (unsigned) reg_renumber[(REGNO)] == ARG_POINTER_REGNUM)
#define REGNO_OK_FOR_INDEX_P(REGNO) \
  REGNO_OK_FOR_BASE_P(REGNO)

/* Maximum number of registers that can appear in a valid memory address.
   Shifts in addresses can't be by a register. */

#define MAX_REGS_PER_ADDRESS 2

/* Recognize any constant value that is a valid address.  */
/* XXX We can address any constant, eventually...  */

#ifdef AOF_ASSEMBLER

#define CONSTANT_ADDRESS_P(X)		\
  (GET_CODE (X) == SYMBOL_REF		\
   && CONSTANT_POOL_ADDRESS_P (X))

#else

#define CONSTANT_ADDRESS_P(X)  			\
  (GET_CODE (X) == SYMBOL_REF 			\
   && (CONSTANT_POOL_ADDRESS_P (X)		\
       || (optimize > 0 && SYMBOL_REF_FLAG (X))))

#endif /* AOF_ASSEMBLER */

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.

   On the ARM, allow any integer (invalid ones are removed later by insn
   patterns), nice doubles and symbol_refs which refer to the function's
   constant pool XXX.  */
#define LEGITIMATE_CONSTANT_P(X)	(! label_mentioned_p (X))

/* Symbols in the text segment can be accessed without indirecting via the
   constant pool; it may take an extra binary operation, but this is still
   faster than indirecting via memory.  Don't do this when not optimizing,
   since we won't be calculating al of the offsets necessary to do this
   simplification.  */
/* This doesn't work with AOF syntax, since the string table may be in
   a different AREA.  */
#ifndef AOF_ASSEMBLER
#define ENCODE_SECTION_INFO(decl)					\
{									\
  if (optimize > 0 && TREE_CONSTANT (decl)				\
      && (!flag_writable_strings || TREE_CODE (decl) != STRING_CST))	\
    {									\
      rtx rtl = (TREE_CODE_CLASS (TREE_CODE (decl)) != 'd'		\
                 ? TREE_CST_RTL (decl) : DECL_RTL (decl));		\
      SYMBOL_REF_FLAG (XEXP (rtl, 0)) = 1;				\
    }									\
}
#endif

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.  */
#ifndef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
#define REG_OK_FOR_BASE_P(X)  				\
  (REGNO (X) < 16 || REGNO (X) >= FIRST_PSEUDO_REGISTER \
   || REGNO (X) == FRAME_POINTER_REGNUM || REGNO (X) == ARG_POINTER_REGNUM)

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
#define REG_OK_FOR_INDEX_P(X)  \
  REG_OK_FOR_BASE_P(X)

#define REG_OK_FOR_PRE_POST_P(X)  			\
  (REGNO (X) < 16 || REGNO (X) >= FIRST_PSEUDO_REGISTER	\
   || REGNO (X) == FRAME_POINTER_REGNUM || REGNO (X) == ARG_POINTER_REGNUM)

#else

/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X)  REGNO_OK_FOR_BASE_P (REGNO (X))

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X)  REGNO_OK_FOR_INDEX_P (REGNO (X))

#define REG_OK_FOR_PRE_POST_P(X)  					   \
  (REGNO (X) < 16 || (unsigned) reg_renumber[REGNO (X)] < 16		   \
   || REGNO (X) == FRAME_POINTER_REGNUM || REGNO (X) == ARG_POINTER_REGNUM \
   || (unsigned) reg_renumber[REGNO (X)] == FRAME_POINTER_REGNUM	   \
   || (unsigned) reg_renumber[REGNO (X)] == ARG_POINTER_REGNUM)

#endif

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address.

   The other macros defined here are used only in GO_IF_LEGITIMATE_ADDRESS.  */
#define BASE_REGISTER_RTX_P(X)  \
  (GET_CODE (X) == REG && REG_OK_FOR_BASE_P (X))

#define INDEX_REGISTER_RTX_P(X)  \
  (GET_CODE (X) == REG && REG_OK_FOR_INDEX_P (X))

/* A C statement (sans semicolon) to jump to LABEL for legitimate index RTXs
   used by the macro GO_IF_LEGITIMATE_ADDRESS.  Floating point indices can
   only be small constants. */
#define GO_IF_LEGITIMATE_INDEX(MODE, BASE_REGNO, INDEX, LABEL)  	\
do									\
{									\
  HOST_WIDE_INT range;							\
  enum rtx_code code = GET_CODE (INDEX);				\
									\
  if (TARGET_HARD_FLOAT && GET_MODE_CLASS (MODE) == MODE_FLOAT)		\
    {									\
      if (code == CONST_INT && INTVAL (INDEX) < 1024			\
	  && INTVAL (INDEX) > -1024					\
	  && (INTVAL (INDEX) & 3) == 0)					\
	goto LABEL;							\
    }									\
  else									\
    {									\
      if (INDEX_REGISTER_RTX_P (INDEX) && GET_MODE_SIZE (MODE) <= 4)	\
	goto LABEL;							\
      if (GET_MODE_SIZE (MODE) <= 4  && code == MULT			\
	  && (! arm_arch4 || (MODE) != HImode))				\
	{								\
	  rtx xiop0 = XEXP (INDEX, 0);					\
	  rtx xiop1 = XEXP (INDEX, 1);					\
	  if (INDEX_REGISTER_RTX_P (xiop0)				\
	      && power_of_two_operand (xiop1, SImode))			\
	    goto LABEL;							\
	  if (INDEX_REGISTER_RTX_P (xiop1)				\
	      && power_of_two_operand (xiop0, SImode))			\
	    goto LABEL;							\
	}								\
      if (GET_MODE_SIZE (MODE) <= 4					\
	  && (code == LSHIFTRT || code == ASHIFTRT			\
	      || code == ASHIFT || code == ROTATERT)			\
	  && (! arm_arch4 || (MODE) != HImode))				\
	{								\
	  rtx op = XEXP (INDEX, 1);					\
	  if (INDEX_REGISTER_RTX_P (XEXP (INDEX, 0))			\
	      && GET_CODE (op) == CONST_INT && INTVAL (op) > 0		\
	      && INTVAL (op) <= 31)					\
	    goto LABEL;							\
        }								\
      /* NASTY: Since this limits the addressing of unsigned byte loads */      \
      range = ((MODE) == HImode || (MODE) == QImode)                    \
              ? (arm_arch4 ? 256 : 4095) : 4096;                        \
      if (code == CONST_INT && INTVAL (INDEX) < range			\
	  && INTVAL (INDEX) > -range)  	      				\
        goto LABEL;							\
    }									\
} while (0)

/* Jump to LABEL if X is a valid address RTX.  This must also take
   REG_OK_STRICT into account when deciding about valid registers, but it uses
   the above macros so we are in luck.  Allow REG, REG+REG, REG+INDEX,
   INDEX+REG, REG-INDEX, and non floating SYMBOL_REF to the constant pool.
   Allow REG-only and AUTINC-REG if handling TImode or HImode.  Other symbol
   refs must be forced though a static cell to ensure addressability.  */
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL)  			\
{									\
  if (BASE_REGISTER_RTX_P (X))						\
    goto LABEL;								\
  else if ((GET_CODE (X) == POST_INC || GET_CODE (X) == PRE_DEC)	\
	   && GET_CODE (XEXP (X, 0)) == REG				\
	   && REG_OK_FOR_PRE_POST_P (XEXP (X, 0)))			\
    goto LABEL;								\
  else if (GET_MODE_SIZE (MODE) >= 4 && reload_completed		\
	   && (GET_CODE (X) == LABEL_REF				\
	       || (GET_CODE (X) == CONST				\
		   && GET_CODE (XEXP ((X), 0)) == PLUS			\
		   && GET_CODE (XEXP (XEXP ((X), 0), 0)) == LABEL_REF	\
		   && GET_CODE (XEXP (XEXP ((X), 0), 1)) == CONST_INT)))\
    goto LABEL;								\
  else if ((MODE) == TImode)						\
    ;									\
  else if ((MODE) == DImode || (TARGET_SOFT_FLOAT && (MODE) == DFmode))	\
    {                                                              	\
      if (GET_CODE (X) == PLUS && BASE_REGISTER_RTX_P (XEXP (X, 0)) 	\
	  && GET_CODE (XEXP (X, 1)) == CONST_INT)   			\
	{                                          			\
	  HOST_WIDE_INT val = INTVAL (XEXP (X, 1)); 			\
          if (val == 4 || val == -4 || val == -8)			\
	    goto LABEL;							\
	}								\
    }									\
  else if (GET_CODE (X) == PLUS)					\
    {									\
      rtx xop0 = XEXP(X,0);						\
      rtx xop1 = XEXP(X,1);						\
									\
      if (BASE_REGISTER_RTX_P (xop0))					\
	GO_IF_LEGITIMATE_INDEX (MODE, REGNO (xop0), xop1, LABEL);	\
      else if (BASE_REGISTER_RTX_P (xop1))				\
	GO_IF_LEGITIMATE_INDEX (MODE, REGNO (xop1), xop0, LABEL);	\
    }									\
  /* Reload currently can't handle MINUS, so disable this for now */	\
  /* else if (GET_CODE (X) == MINUS)					\
    {									\
      rtx xop0 = XEXP (X,0);						\
      rtx xop1 = XEXP (X,1);						\
									\
      if (BASE_REGISTER_RTX_P (xop0))					\
	GO_IF_LEGITIMATE_INDEX (MODE, -1, xop1, LABEL);			\
    } */								\
  else if (GET_MODE_CLASS (MODE) != MODE_FLOAT				\
	   && GET_CODE (X) == SYMBOL_REF				\
	   && CONSTANT_POOL_ADDRESS_P (X))				\
    goto LABEL;								\
  else if ((GET_CODE (X) == PRE_INC || GET_CODE (X) == POST_DEC)	\
	   && (GET_MODE_SIZE (MODE) <= 4)				\
	   && GET_CODE (XEXP (X, 0)) == REG				\
	   && REG_OK_FOR_PRE_POST_P (XEXP (X, 0)))			\
    goto LABEL;								\
}

/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.

   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.

   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.

   It is always safe for this macro to do nothing.  It exists to recognize
   opportunities to optimize the output.

   On the ARM, try to convert [REG, #BIGCONST]
   into ADD BASE, REG, #UPPERCONST and [BASE, #VALIDCONST],
   where VALIDCONST == 0 in case of TImode.  */
extern struct rtx_def *legitimize_pic_address ();
#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)				 \
{									 \
  if (GET_CODE (X) == PLUS)						 \
    {									 \
      rtx xop0 = XEXP (X, 0);						 \
      rtx xop1 = XEXP (X, 1);						 \
									 \
      if (CONSTANT_P (xop0) && ! symbol_mentioned_p (xop0))		 \
	xop0 = force_reg (SImode, xop0);				 \
      if (CONSTANT_P (xop1) && ! symbol_mentioned_p (xop1))		 \
	xop1 = force_reg (SImode, xop1);				 \
      if (BASE_REGISTER_RTX_P (xop0) && GET_CODE (xop1) == CONST_INT)	 \
	{								 \
	  HOST_WIDE_INT n, low_n;					 \
	  rtx base_reg, val;						 \
	  n = INTVAL (xop1);						 \
									 \
	  if (MODE == DImode || (TARGET_SOFT_FLOAT && MODE == DFmode))	 \
	    {								 \
	      low_n = n & 0x0f;						 \
	      n &= ~0x0f;						 \
	      if (low_n > 4)						 \
		{							 \
		  n += 16;						 \
		  low_n -= 16;						 \
		}							 \
	    }								 \
	  else								 \
	    {								 \
	      low_n = ((MODE) == TImode ? 0				 \
		       : n >= 0 ? (n & 0xfff) : -((-n) & 0xfff));	 \
	      n -= low_n;						 \
	    }								 \
	  base_reg = gen_reg_rtx (SImode);				 \
	  val = force_operand (gen_rtx (PLUS, SImode, xop0,		 \
					GEN_INT (n)), NULL_RTX);	 \
	  emit_move_insn (base_reg, val);				 \
	  (X) = (low_n == 0 ? base_reg					 \
		 : gen_rtx (PLUS, SImode, base_reg, GEN_INT (low_n)));	 \
	}								 \
      else if (xop0 != XEXP (X, 0) || xop1 != XEXP (x, 1))		 \
	(X) = gen_rtx (PLUS, SImode, xop0, xop1);			 \
    }									 \
  else if (GET_CODE (X) == MINUS)					 \
    {									 \
      rtx xop0 = XEXP (X, 0);						 \
      rtx xop1 = XEXP (X, 1);						 \
									 \
      if (CONSTANT_P (xop0))						 \
	xop0 = force_reg (SImode, xop0);				 \
      if (CONSTANT_P (xop1) && ! symbol_mentioned_p (xop1))		 \
	xop1 = force_reg (SImode, xop1);				 \
      if (xop0 != XEXP (X, 0) || xop1 != XEXP (X, 1))			 \
	(X) = gen_rtx (MINUS, SImode, xop0, xop1);			 \
    }									 \
  if (flag_pic)								 \
    (X) = legitimize_pic_address (OLDX, MODE, NULL_RTX);		 \
  if (memory_address_p (MODE, X))					 \
    goto WIN;								 \
}

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.  */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)  \
{									\
  if (GET_CODE(ADDR) == PRE_DEC || GET_CODE(ADDR) == POST_DEC		\
      || GET_CODE(ADDR) == PRE_INC || GET_CODE(ADDR) == POST_INC)	\
    goto LABEL;								\
}

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* Define as C expression which evaluates to nonzero if the tablejump
   instruction expects the table to contain offsets from the address of the
   table.
   Do not define this if the table should contain absolute addresses. */
/* #define CASE_VECTOR_PC_RELATIVE 1 */

/* Specify the tree operation to be used to convert reals to integers.  */
#define IMPLICIT_FIX_EXPR  FIX_ROUND_EXPR

/* This is the kind of divide that is easiest to do in the general case.  */
#define EASY_DIV_EXPR  TRUNC_DIV_EXPR

/* signed 'char' is most compatible, but RISC OS wants it unsigned.
   unsigned is probably best, but may break some code.  */
#ifndef DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR  0
#endif

/* Don't cse the address of the function being compiled.  */
#define NO_RECURSIVE_FUNCTION_CSE 1

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none.  */
#define LOAD_EXTEND_OP(MODE)						\
  ((arm_arch4 || (MODE) == QImode) ? ZERO_EXTEND			\
   : ((BYTES_BIG_ENDIAN && (MODE) == HImode) ? SIGN_EXTEND : NIL))

/* Define this if zero-extension is slow (more than one real instruction).
   On the ARM, it is more than one instruction only if not fetching from
   memory.  */
/* #define SLOW_ZERO_EXTEND */

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 0

/* Immediate shift counts are truncated by the output routines (or was it
   the assembler?).  Shift counts in a register are truncated by ARM.  Note
   that the native compiler puts too large (> 32) immediate shift counts
   into a register and shifts by the register, letting the ARM decide what
   to do instead of doing that itself.  */
/* This is all wrong.  Defining SHIFT_COUNT_TRUNCATED tells combine that
   code like (X << (Y % 32)) for register X, Y is equivalent to (X << Y).
   On the arm, Y in a register is used modulo 256 for the shift. Only for
   rotates is modulo 32 used. */
/* #define SHIFT_COUNT_TRUNCATED 1 */

/* All integers have the same format so truncation is easy.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC,INPREC)  1

/* Calling from registers is a massive pain.  */
#define NO_FUNCTION_CSE 1

/* Chars and shorts should be passed as ints.  */
#define PROMOTE_PROTOTYPES 1

/* The machine modes of pointers and functions */
#define Pmode  SImode
#define FUNCTION_MODE  Pmode

/* The structure type of the machine dependent info field of insns
   No uses for this yet.  */
/* #define INSN_MACHINE_INFO  struct machine_info  */

/* The relative costs of various types of constants.  Note that cse.c defines
   REG = 1, SUBREG = 2, any node = (2 + sum of subnodes).  */
#define CONST_COSTS(RTX, CODE, OUTER_CODE)			\
  case CONST_INT:						\
    if (const_ok_for_arm (INTVAL (RTX)))			\
      return (OUTER_CODE) == SET ? 2 : -1;	    		\
    else if (OUTER_CODE == AND                  		\
             && const_ok_for_arm (~INTVAL (RTX)))		\
      return -1;	                              		\
    else if ((OUTER_CODE == COMPARE             		\
              || OUTER_CODE == PLUS || OUTER_CODE == MINUS)     \
             && const_ok_for_arm (-INTVAL (RTX)))		\
      return -1;	                              		\
    else                                        		\
      return 5;		                               		\
  case CONST: 							\
  case LABEL_REF:						\
  case SYMBOL_REF:						\
    return 6;							\
  case CONST_DOUBLE:						\
    if (const_double_rtx_ok_for_fpu (RTX))			\
      return (OUTER_CODE) == SET ? 2 : -1;			\
    else if (((OUTER_CODE) == COMPARE || (OUTER_CODE) == PLUS)	\
	     && neg_const_double_rtx_ok_for_fpu (RTX))		\
       return -1;						\
    return(7);

#define ARM_FRAME_RTX(X)				\
  ((X) == frame_pointer_rtx || (X) == stack_pointer_rtx	\
   || (X) == arg_pointer_rtx)

#define DEFAULT_RTX_COSTS(X,CODE,OUTER_CODE)		\
   return arm_rtx_costs (X, CODE, OUTER_CODE);

/* Moves to and from memory are quite expensive */
#define MEMORY_MOVE_COST(MODE,CLASS,IN)  10

/* All address computations that can be done are free, but rtx cost returns
   the same for practically all of them.  So we weight the different types
   of address here in the order (most pref first):
   PRE/POST_INC/DEC, SHIFT or NON-INT sum, INT sum, REG, MEM or LABEL. */
#define ADDRESS_COST(X)							     \
  (10 - ((GET_CODE (X) == MEM || GET_CODE (X) == LABEL_REF		     \
	  || GET_CODE (X) == SYMBOL_REF)				     \
	 ? 0								     \
	 : ((GET_CODE (X) == PRE_INC || GET_CODE (X) == PRE_DEC		     \
	     || GET_CODE (X) == POST_INC || GET_CODE (X) == POST_DEC)	     \
	    ? 10							     \
	    : (((GET_CODE (X) == PLUS || GET_CODE (X) == MINUS)		     \
		? 6 + (GET_CODE (XEXP (X, 1)) == CONST_INT ? 2 		     \
		       : ((GET_RTX_CLASS (GET_CODE (XEXP (X, 0))) == '2'     \
			   || GET_RTX_CLASS (GET_CODE (XEXP (X, 0))) == 'c'  \
			   || GET_RTX_CLASS (GET_CODE (XEXP (X, 1))) == '2'  \
			   || GET_RTX_CLASS (GET_CODE (XEXP (X, 1))) == 'c') \
			  ? 1 : 0))					     \
		: 4)))))
	 
   

/* Try to generate sequences that don't involve branches, we can then use
   conditional instructions */
#define BRANCH_COST 4

/* A C statement to update the variable COST based on the relationship
   between INSN that is dependent on DEP through dependence LINK.  */
#define ADJUST_COST(INSN,LINK,DEP,COST) \
  (COST) = arm_adjust_cost ((INSN), (LINK), (DEP), (COST))

/* Position Independent Code.  */
/* We decide which register to use based on the compilation options and
   the assembler in use; this is more general than the APCS restriction of
   using sb (r9) all the time.  */
extern int arm_pic_register;

/* The register number of the register used to address a table of static
   data addresses in memory.  */
#define PIC_OFFSET_TABLE_REGNUM arm_pic_register

#define FINALIZE_PIC arm_finalize_pic ()

#define LEGITIMATE_PIC_OPERAND_P(X) (! symbol_mentioned_p (X))
 


/* Condition code information. */
/* Given a comparison code (EQ, NE, etc.) and the first operand of a COMPARE,
   return the mode to be used for the comparison. 
   CCFPEmode should be used with floating inequalities,
   CCFPmode should be used with floating equalities.
   CC_NOOVmode should be used with SImode integer equalities.
   CC_Zmode should be used if only the Z flag is set correctly
   CCmode should be used otherwise. */

#define EXTRA_CC_MODES CC_NOOVmode, CC_Zmode, CC_SWPmode, \
  CCFPmode, CCFPEmode, CC_DNEmode, CC_DEQmode, CC_DLEmode, \
  CC_DLTmode, CC_DGEmode, CC_DGTmode, CC_DLEUmode, CC_DLTUmode, \
  CC_DGEUmode, CC_DGTUmode, CC_Cmode

#define EXTRA_CC_NAMES "CC_NOOV", "CC_Z", "CC_SWP", "CCFP", "CCFPE", \
  "CC_DNE", "CC_DEQ", "CC_DLE", "CC_DLT", "CC_DGE", "CC_DGT", "CC_DLEU", \
  "CC_DLTU", "CC_DGEU", "CC_DGTU", "CC_C"

enum machine_mode arm_select_cc_mode ();
#define SELECT_CC_MODE(OP,X,Y)  arm_select_cc_mode ((OP), (X), (Y))

#define REVERSIBLE_CC_MODE(MODE) ((MODE) != CCFPEmode)

enum rtx_code arm_canonicalize_comparison ();
#define CANONICALIZE_COMPARISON(CODE,OP0,OP1)			\
do								\
{								\
  if (GET_CODE (OP1) == CONST_INT				\
      && ! (const_ok_for_arm (INTVAL (OP1))			\
	    || (const_ok_for_arm (- INTVAL (OP1)))))		\
    {								\
      rtx const_op = OP1;					\
      CODE = arm_canonicalize_comparison ((CODE), &const_op);	\
      OP1 = const_op;						\
    }								\
} while (0)

#define STORE_FLAG_VALUE 1

/* Define the information needed to generate branch insns.  This is
   stored from the compare operation.  Note that we can't use "rtx" here
   since it hasn't been defined!  */

extern struct rtx_def *arm_compare_op0, *arm_compare_op1;
extern int arm_compare_fp;

/* Define the codes that are matched by predicates in arm.c */
#define PREDICATE_CODES							\
  {"s_register_operand", {SUBREG, REG}},				\
  {"f_register_operand", {SUBREG, REG}},				\
  {"arm_add_operand", {SUBREG, REG, CONST_INT}},			\
  {"fpu_add_operand", {SUBREG, REG, CONST_DOUBLE}},			\
  {"arm_rhs_operand", {SUBREG, REG, CONST_INT}},			\
  {"fpu_rhs_operand", {SUBREG, REG, CONST_DOUBLE}},			\
  {"arm_not_operand", {SUBREG, REG, CONST_INT}},			\
  {"offsettable_memory_operand", {MEM}},				\
  {"bad_signed_byte_operand", {MEM}},					\
  {"alignable_memory_operand", {MEM}},					\
  {"shiftable_operator", {PLUS, MINUS, AND, IOR, XOR}},			\
  {"minmax_operator", {SMIN, SMAX, UMIN, UMAX}},			\
  {"shift_operator", {ASHIFT, ASHIFTRT, LSHIFTRT, ROTATERT, MULT}},	\
  {"di_operand", {SUBREG, REG, CONST_INT, CONST_DOUBLE, MEM}},		\
  {"soft_df_operand", {SUBREG, REG, CONST_DOUBLE, MEM}},		\
  {"load_multiple_operation", {PARALLEL}},				\
  {"store_multiple_operation", {PARALLEL}},				\
  {"equality_operator", {EQ, NE}},					\
  {"arm_rhsm_operand", {SUBREG, REG, CONST_INT, MEM}},			\
  {"const_shift_operand", {CONST_INT}},					\
  {"index_operand", {SUBREG, REG, CONST_INT}},				\
  {"reg_or_int_operand", {SUBREG, REG, CONST_INT}},			\
  {"multi_register_push", {PARALLEL}},					\
  {"cc_register", {REG}},						\
  {"dominant_cc_register", {REG}},



/* Gcc puts the pool in the wrong place for ARM, since we can only
   load addresses a limited distance around the pc.  We do some
   special munging to move the constant pool values to the correct
   point in the code.  */
#define MACHINE_DEPENDENT_REORG(INSN)	arm_reorg ((INSN))

/* The pool is empty, since we have moved everything into the code.  */
#define ASM_OUTPUT_SPECIAL_POOL_ENTRY(FILE,X,MODE,ALIGN,LABELNO,JUMPTO)	\
  goto JUMPTO

/* Output an internal label definition.  */
#ifndef ASM_OUTPUT_INTERNAL_LABEL
#define ASM_OUTPUT_INTERNAL_LABEL(STREAM, PREFIX, NUM)  	\
  do                                    	      	   	\
    {						      	   	\
      char * s = (char *) alloca (40 + strlen (PREFIX));	\
      extern int arm_target_label, arm_ccfsm_state;	   	\
      extern rtx arm_target_insn;				\
						           	\
      if (arm_ccfsm_state == 3 && arm_target_label == (NUM)   	\
	&& !strcmp (PREFIX, "L"))				\
	{							\
	  arm_ccfsm_state = 0;				        \
	  arm_target_insn = NULL;				\
	}							\
	ASM_GENERATE_INTERNAL_LABEL (s, (PREFIX), (NUM));   	\
        /* CYGNUS LOCAL variation */				\
	arm_asm_output_label (STREAM, s);	                \
        /* END CYGNUS LOCAL variation */			\
    } while (0)
#endif

/* CYGNUS LOCAL */
/* Output a label definition.  */
#undef  ASM_OUTPUT_LABEL
#define ASM_OUTPUT_LABEL(STREAM,NAME) arm_asm_output_label ((STREAM), (NAME))
/* END CYGNUS LOCAL */
     
/* Output a push or a pop instruction (only used when profiling).  */
#define ASM_OUTPUT_REG_PUSH(STREAM,REGNO) \
  fprintf (STREAM,"\tstmfd\t%ssp!,{%s%s}\n", \
	  REGISTER_PREFIX, REGISTER_PREFIX, reg_names [REGNO])

#define ASM_OUTPUT_REG_POP(STREAM,REGNO) \
  fprintf (STREAM,"\tldmfd\t%ssp!,{%s%s}\n", \
	  REGISTER_PREFIX, REGISTER_PREFIX, reg_names [REGNO])

/* Target characters.  */
#define TARGET_BELL	007
#define TARGET_BS	010
#define TARGET_TAB	011
#define TARGET_NEWLINE	012
#define TARGET_VT	013
#define TARGET_FF	014
#define TARGET_CR	015

/* Only perform branch elimination (by making instructions conditional) if
   we're optimising.  Otherwise it's of no use anyway.  */
#define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS)  \
  if (optimize)					    \
    final_prescan_insn (INSN, OPVEC, NOPERANDS)

#define PRINT_OPERAND_PUNCT_VALID_P(CODE)	\
  ((CODE) == '?' || (CODE) == '|' || (CODE) == '@')
/* Output an operand of an instruction.  */
#define PRINT_OPERAND(STREAM, X, CODE)  \
  arm_print_operand (STREAM, X, CODE)

#define ARM_SIGN_EXTEND(x)  ((HOST_WIDE_INT)		\
  (HOST_BITS_PER_WIDE_INT <= 32 ? (x)			\
   : (((x) & (unsigned HOST_WIDE_INT) 0xffffffff) |	\
      (((x) & (unsigned HOST_WIDE_INT) 0x80000000)	\
       ? ((~ (HOST_WIDE_INT) 0)				\
	  & ~ (unsigned HOST_WIDE_INT) 0xffffffff)	\
       : 0))))

/* Output the address of an operand.  */
#define PRINT_OPERAND_ADDRESS(STREAM,X)  \
{									\
    int is_minus = GET_CODE (X) == MINUS;				\
									\
    if (GET_CODE (X) == REG)						\
	fprintf (STREAM, "[%s%s, #0]", REGISTER_PREFIX,			\
		 reg_names[REGNO (X)]);					\
    else if (GET_CODE (X) == PLUS || is_minus)				\
      {									\
	rtx base = XEXP (X, 0);						\
	rtx index = XEXP (X, 1);					\
	char * base_reg_name;						\
	HOST_WIDE_INT offset = 0;					\
	if (GET_CODE (base) != REG)					\
	  {								\
	    /* Ensure that BASE is a register (one of them must be). */	\
	    rtx temp = base;						\
	    base = index;						\
	    index = temp;						\
	  }								\
	base_reg_name = reg_names[REGNO (base)];			\
	switch (GET_CODE (index))					\
	  {								\
	  case CONST_INT:						\
	    offset = INTVAL (index);					\
	    if (is_minus)						\
	      offset = -offset;						\
	    fprintf (STREAM, "[%s%s, #%d]", REGISTER_PREFIX,		\
		     base_reg_name, offset);				\
	    break;							\
									\
	  case REG:							\
	    fprintf (STREAM, "[%s%s, %s%s%s]", REGISTER_PREFIX,		\
		     base_reg_name, is_minus ? "-" : "",		\
		     REGISTER_PREFIX, reg_names[REGNO (index)] );	\
	    break;							\
									\
	  case MULT:							\
	  case ASHIFTRT:						\
	  case LSHIFTRT:						\
	  case ASHIFT:							\
	  case ROTATERT:						\
	  {								\
	    fprintf (STREAM, "[%s%s, %s%s%s", REGISTER_PREFIX,		\
		     base_reg_name, is_minus ? "-" : "", REGISTER_PREFIX,\
		     reg_names[REGNO (XEXP (index, 0))]);		\
	    arm_print_operand (STREAM, index, 'S');			\
	    fputs ("]", STREAM);					\
	    break;							\
	  }								\
	    								\
	  default:							\
	    abort();							\
	}								\
    }							        	\
  else if (GET_CODE (X) == PRE_INC || GET_CODE (X) == POST_INC		\
	   || GET_CODE (X) == PRE_DEC || GET_CODE (X) == POST_DEC)	\
    {									\
      extern int output_memory_reference_mode;				\
      									\
      if (GET_CODE (XEXP (X, 0)) != REG)				\
	abort ();							\
									\
      if (GET_CODE (X) == PRE_DEC || GET_CODE (X) == PRE_INC)		\
	fprintf (STREAM, "[%s%s, #%s%d]!", REGISTER_PREFIX,		\
		 reg_names[REGNO (XEXP (X, 0))],			\
		 GET_CODE (X) == PRE_DEC ? "-" : "",			\
		 GET_MODE_SIZE (output_memory_reference_mode));		\
      else								\
	fprintf (STREAM, "[%s%s], #%s%d", REGISTER_PREFIX,		\
		 reg_names[REGNO (XEXP (X, 0))],			\
		 GET_CODE (X) == POST_DEC ? "-" : "",			\
		 GET_MODE_SIZE (output_memory_reference_mode));		\
    }									\
  else output_addr_const(STREAM, X);					\
}

/* Handles PIC addr specially */
#define OUTPUT_INT_ADDR_CONST(STREAM,X) \
  {									\
    if (flag_pic && GET_CODE(X) == CONST && is_pic(X))			\
      {									\
	output_addr_const(STREAM, XEXP (XEXP (XEXP (X, 0), 0), 0));	\
	fputs(" - (", STREAM);						\
	output_addr_const(STREAM, XEXP (XEXP (XEXP (X, 0), 1), 0));	\
	fputs(")", STREAM);						\
      }									\
    else output_addr_const(STREAM, X);					\
  }

/* Output code to add DELTA to the first argument, and then jump to FUNCTION.
   Used for C++ multiple inheritance.  */
#define ASM_OUTPUT_MI_THUNK(FILE, THUNK_FNDECL, DELTA, FUNCTION)	\
do {									\
  int mi_delta = (DELTA);						\
  char *mi_op = mi_delta < 0 ? "sub" : "add";				\
  int shift = 0;							\
  int this_regno = (aggregate_value_p (TREE_TYPE (TREE_TYPE (FUNCTION))) \
		    ? 1 : 0);						\
  if (mi_delta < 0) mi_delta = -mi_delta;				\
  while (mi_delta != 0)							\
    {									\
      if (mi_delta & (3 << shift) == 0)					\
	shift += 2;							\
      else								\
	{								\
	  fprintf (FILE, "\t%s\t%s%s, %s%s, #%d\n",			\
		   mi_op, REGISTER_PREFIX, reg_names[this_regno],	\
		   REGISTER_PREFIX, reg_names[this_regno],		\
		   mi_delta & (0xff << shift));				\
          /* CYGNUS LOCAL */						\
	  arm_increase_location (4);					\
          /* END CYGNUS LOCAL */					\
	  mi_delta &= ~(0xff << shift);					\
	  shift += 8;							\
	}								\
    }									\
  fputs ("\tb\t", FILE);						\
  assemble_name (FILE, XSTR (XEXP (DECL_RTL (FUNCTION), 0), 0));	\
  fputc ('\n', FILE);							\
  /* CYGNUS LOCAL */							\
  arm_increase_location (4);						\
  /* END CYGNUS LOCAL */						\
} while (0)

/* A C expression whose value is RTL representing the value of the return
   address for the frame COUNT steps up from the current frame.  */

#define RETURN_ADDR_RTX(COUNT, FRAME)	\
  ((COUNT == 0)				\
   ? gen_rtx (MEM, Pmode, plus_constant (FRAME, -4)) \
   : NULL_RTX)

/* Used to mask out junk bits from the return address, such as
   processor state, interrupt status, condition codes and the like.  */
#define MASK_RETURN_ADDR \
  /* If we are generating code for an ARM2/ARM3 machine or for an ARM6	\
     in 26 bit mode, the condition codes must be masked out of the	\
     return address.  This does not apply to ARM6 and later processors	\
     when running in 32 bit mode.  */					\
  ((!TARGET_APCS_32) ? (GEN_INT (0x03fffffc)) : (GEN_INT (0xffffffff)))

/* Prototypes for arm.c -- actually, they aren't since the types aren't 
   fully defined yet.  */

void arm_override_options (/* void */);
int use_return_insn (/* void */);
int const_ok_for_arm (/* HOST_WIDE_INT */);
int const_ok_for_op (/* HOST_WIDE_INT, enum rtx_code, 
			enum machine_mode */);
int arm_split_constant (/* enum rtx_code, enum machine_mode, 
			   HOST_WIDE_INT, struct rtx_def *,
			   struct rtx_def *, int */);
enum rtx_code arm_canonicalize_comparison (/* enum rtx_code, 
					      struct rtx_def ** */);
int arm_return_in_memory (/* union tree_node * */);
int legitimate_pic_operand_p (/* struct rtx_def * */);
struct rtx_def *legitimize_pic_address (/* struct rtx_def *, 
					   enum machine_mode,
					   struct rtx_def * */);
int is_pic (/* struct rtx_def * */);
void arm_finalize_pic (/* void */);
int arm_rtx_costs (/* struct rtx_def *, enum rtx_code, enum rtx_code */);
int arm_adjust_cost (/* struct rtx_def *, struct rtx_def *, 
			struct rtx_def *, int */);
int const_double_rtx_ok_for_fpu (/* struct rtx_def * */);
int neg_const_double_rtx_ok_for_fpu (/* struct rtx_def * */);
int s_register_operand (/* struct rtx_def *, enum machine_mode */);
int f_register_operand (/* struct rtx_def *, enum machine_mode */);
int reg_or_int_operand (/* struct rtx_def *, enum machine_mode */);
int reload_memory_operand (/* struct rtx_def *, enum machine_mode */);
int arm_rhs_operand (/* struct rtx_def *, enum machine_mode */);
int arm_rhsm_operand (/* struct rtx_def *, enum machine_mode */);
int arm_add_operand (/* struct rtx_def *, enum machine_mode */);
int arm_not_operand (/* struct rtx_def *, enum machine_mode */);
int offsettable_memory_operand (/* struct rtx_def *, enum machine_mode */);
int alignable_memory_operand (/* struct rtx_def *, enum machine_mode */);
int bad_signed_byte_operand (/* struct rtx_def *, enum machine_mode */);
int fpu_rhs_operand (/* struct rtx_def *, enum machine_mode */);
int fpu_add_operand (/* struct rtx_def *, enum machine_mode */);
int power_of_two_operand (/* struct rtx_def *, enum machine_mode */);
int di_operand (/* struct rtx_def *, enum machine_mode */);
int soft_df_operand (/* struct rtx_def *, enum machine_mode */);
int index_operand (/* struct rtx_def *, enum machine_mode */);
int const_shift_operand (/* struct rtx_def *, enum machine_mode */);
int shiftable_operator (/* struct rtx_def *, enum machine_mode */);
int shift_operator (/* struct rtx_def *, enum machine_mode */);
int equality_operator (/* struct rtx_def *, enum machine_mode */);
int minmax_operator (/* struct rtx_def *, enum machine_mode */);
int cc_register (/* struct rtx_def *, enum machine_mode */);
int dominant_cc_register (/* struct rtx_def *, enum machine_mode */);
int symbol_mentioned_p (/* struct rtx_def * */);
int label_mentioned_p (/* struct rtx_def * */);
enum rtx_code minmax_code (/* struct rtx_def * */);
int adjacent_mem_locations (/* struct rtx_def *, struct rtx_def * */);
int load_multiple_operation (/* struct rtx_def *, enum machine_mode */);
int store_multiple_operation (/* struct rtx_def *, enum machine_mode */);
int load_multiple_sequence (/* struct rtx_def **, int, int *, int *,
			       HOST_WIDE_INT * */);
char *emit_ldm_seq (/* struct rtx_def **, int */);
int store_multiple_sequence (/* struct rtx_def **, int, int *, int *,
				HOST_WIDE_INT * */);
char *emit_stm_seq (/* struct rtx_def **, int */);
int multi_register_push (/* struct rtx_def *, enum machine_mode */);
int arm_valid_machine_decl_attribute (/* union tree_node *, union tree_node *,
					 union tree_node *,
					 union tree_node * */);
struct rtx_def *arm_gen_load_multiple (/* int, int, struct rtx_def *, 
					  int, int, int, int, int */);
struct rtx_def *arm_gen_store_multiple (/* int, int, struct rtx_def *,
					   int, int, int, int, int */);
int arm_gen_movstrqi (/* struct rtx_def ** */);
struct rtx_def *gen_rotated_half_load (/* struct rtx_def * */);
enum machine_mode arm_select_cc_mode (/* enum rtx_code, struct rtx_def *,
					 struct rtx_def * */);
struct rtx_def *gen_compare_reg (/* enum rtx_code, struct rtx_def *,
				    struct rtx_def * */);
void arm_reload_in_hi (/* struct rtx_def ** */);
void arm_reload_out_hi (/* struct rtx_def ** */);
void arm_reorg (/* struct rtx_def * */);
char *fp_immediate_constant (/* struct rtx_def * */);
void print_multi_reg (/* FILE *, char *, int, int */);
char *output_call (/* struct rtx_def ** */);
char *output_call_mem (/* struct rtx_def ** */);
char *output_mov_long_double_fpu_from_arm (/* struct rtx_def ** */);
char *output_mov_long_double_arm_from_fpu (/* struct rtx_def ** */);
char *output_mov_long_double_arm_from_arm (/* struct rtx_def ** */);
char *output_mov_double_fpu_from_arm (/* struct rtx_def ** */);
char *output_mov_double_arm_from_fpu (/* struct rtx_def ** */);
char *output_move_double (/* struct rtx_def ** */);
char *output_mov_immediate (/* struct rtx_def ** */);
char *output_add_immediate (/* struct rtx_def ** */);
char *arithmetic_instr (/* struct rtx_def *, int */);
void output_ascii_pseudo_op (/* FILE *, unsigned char *, int */);
char *output_return_instruction (/* struct rtx_def *, int, int */);
int arm_volatile_func (/* void */);
void output_func_prologue (/* FILE *, int */);
void output_func_epilogue (/* FILE *, int */);
void arm_expand_prologue (/* void */);
void arm_print_operand (/* FILE *, struct rtx_def *, int */);
void final_prescan_insn (/* struct rtx_def *, struct rtx_def **, int */);
#ifdef AOF_ASSEMBLER
struct rtx_def *aof_pic_entry (/* struct rtx_def * */);
void aof_dump_pic_table (/* FILE * */);
char *aof_text_section (/* void */);
char *aof_data_section (/* void */);
void aof_add_import (/* char * */);
void aof_delete_import (/* char * */);
void aof_dump_imports (/* FILE * */);
#endif
/* CYGNUS LOCAL nickc */
int ok_integer_or_other ();
/* END CYGNUS LOCAL */
int s_register_operand (/* register rtx op, enum machine_mode mode */);

void arm_asm_output_label (/*FILE *, char **/);
void arm_increase_location PARAMS ((int));
int short_branch PARAMS ((int, int));
int arm_insn_not_targeted (/* rtx */);
int arm_backwards_branch PARAMS ((int, int));

#endif /* __ARM_H__ */
