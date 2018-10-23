/* tc-arm.c -- Assemble for the ARM
   Copyright (C) 1994-2018 Free Software Foundation, Inc.
   Contributed by Richard Earnshaw (rwe@pegasus.esprit.ec.org)
	Modified by David Taylor (dtaylor@armltd.co.uk)
	Cirrus coprocessor mods by Aldy Hernandez (aldyh@redhat.com)
	Cirrus coprocessor fixes by Petko Manolov (petkan@nucleusys.com)
	Cirrus coprocessor fixes by Vladimir Ivanov (vladitx@nucleusys.com)

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#include "as.h"
#include <limits.h>
#include <stdarg.h>
#define	 NO_RELOC 0
#include "safe-ctype.h"
#include "subsegs.h"
#include "obstack.h"
#include "libiberty.h"
#include "opcode/arm.h"

#include "elf/arm.h"
#include "dw2gencfi.h"

#include "dwarf2dbg.h"

/* Must be at least the size of the largest unwind opcode (currently two).  */
#define ARM_OPCODE_CHUNK_SIZE 8

/* This structure holds the unwinding state.  */

static struct
{
  symbolS *	  proc_start;
  symbolS *	  table_entry;
  symbolS *	  personality_routine;
  int		  personality_index;
  /* The segment containing the function.  */
  segT		  saved_seg;
  subsegT	  saved_subseg;
  /* Opcodes generated from this function.  */
  unsigned char * opcodes;
  int		  opcode_count;
  int		  opcode_alloc;
  /* The number of bytes pushed to the stack.  */
  offsetT	  frame_size;
  /* We don't add stack adjustment opcodes immediately so that we can merge
     multiple adjustments.  We can also omit the final adjustment
     when using a frame pointer.  */
  offsetT	  pending_offset;
  /* These two fields are set by both unwind_movsp and unwind_setfp.  They
     hold the reg+offset to use when restoring sp from a frame pointer.	 */
  offsetT	  fp_offset;
  int		  fp_reg;
  /* Nonzero if an unwind_setfp directive has been seen.  */
  unsigned	  fp_used:1;
  /* Nonzero if the last opcode restores sp from fp_reg.  */
  unsigned	  sp_restored:1;
} unwind;

/* Whether --fdpic was given.  */
static int arm_fdpic;

/* Results from operand parsing worker functions.  */

typedef enum
{
  PARSE_OPERAND_SUCCESS,
  PARSE_OPERAND_FAIL,
  PARSE_OPERAND_FAIL_NO_BACKTRACK
} parse_operand_result;

enum arm_float_abi
{
  ARM_FLOAT_ABI_HARD,
  ARM_FLOAT_ABI_SOFTFP,
  ARM_FLOAT_ABI_SOFT
};

/* Types of processor to assemble for.	*/
#ifndef CPU_DEFAULT
/* The code that was here used to select a default CPU depending on compiler
   pre-defines which were only present when doing native builds, thus
   changing gas' default behaviour depending upon the build host.

   If you have a target that requires a default CPU option then the you
   should define CPU_DEFAULT here.  */
#endif

#define streq(a, b)	      (strcmp (a, b) == 0)

/* Current set of feature bits available (CPU+FPU).  Different from
   selected_cpu + selected_fpu in case of autodetection since the CPU
   feature bits are then all set.  */
static arm_feature_set cpu_variant;
/* Feature bits used in each execution state.  Used to set build attribute
   (in particular Tag_*_ISA_use) in CPU autodetection mode.  */
static arm_feature_set arm_arch_used;
static arm_feature_set thumb_arch_used;

/* Flags stored in private area of BFD structure.  */
static int uses_apcs_26	     = FALSE;
static int atpcs	     = FALSE;
static int support_interwork = FALSE;
static int uses_apcs_float   = FALSE;
static int pic_code	     = FALSE;
static int fix_v4bx	     = FALSE;
/* Warn on using deprecated features.  */
static int warn_on_deprecated = TRUE;

/* Understand CodeComposer Studio assembly syntax.  */
bfd_boolean codecomposer_syntax = FALSE;

/* Variables that we set while parsing command-line options.  Once all
   options have been read we re-process these values to set the real
   assembly flags.  */

/* CPU and FPU feature bits set for legacy CPU and FPU options (eg. -marm1
   instead of -mcpu=arm1).  */
static const arm_feature_set *legacy_cpu = NULL;
static const arm_feature_set *legacy_fpu = NULL;

/* CPU, extension and FPU feature bits selected by -mcpu.  */
static const arm_feature_set *mcpu_cpu_opt = NULL;
static arm_feature_set *mcpu_ext_opt = NULL;
static const arm_feature_set *mcpu_fpu_opt = NULL;

/* CPU, extension and FPU feature bits selected by -march.  */
static const arm_feature_set *march_cpu_opt = NULL;
static arm_feature_set *march_ext_opt = NULL;
static const arm_feature_set *march_fpu_opt = NULL;

/* Feature bits selected by -mfpu.  */
static const arm_feature_set *mfpu_opt = NULL;
static const arm_feature_set fpu_default = FPU_DEFAULT;

/* Constants for known architecture features.  */
static const arm_feature_set fpu_arch_fpa = FPU_ARCH_FPA;
#ifdef CPU_DEFAULT
static const arm_feature_set cpu_default = CPU_DEFAULT;
#endif

static const arm_feature_set arm_ext_v1 = ARM_FEATURE_CORE_LOW (ARM_EXT_V1);
static const arm_feature_set arm_ext_v2 = ARM_FEATURE_CORE_LOW (ARM_EXT_V2);
static const arm_feature_set arm_ext_v2s = ARM_FEATURE_CORE_LOW (ARM_EXT_V2S);
static const arm_feature_set arm_ext_v3 = ARM_FEATURE_CORE_LOW (ARM_EXT_V3);
static const arm_feature_set arm_ext_v3m = ARM_FEATURE_CORE_LOW (ARM_EXT_V3M);
static const arm_feature_set arm_ext_v4 = ARM_FEATURE_CORE_LOW (ARM_EXT_V4);
static const arm_feature_set arm_ext_v4t = ARM_FEATURE_CORE_LOW (ARM_EXT_V4T);

static const arm_feature_set arm_ext_msr =
  ARM_FEATURE_CORE_LOW (ARM_EXT_THUMB_MSR);
static const arm_feature_set arm_ext_os = ARM_FEATURE_CORE_LOW (ARM_EXT_OS);

static const arm_feature_set arm_arch_any = ARM_ANY;
static const arm_feature_set fpu_any = FPU_ANY;
static const arm_feature_set arm_arch_full ATTRIBUTE_UNUSED = ARM_FEATURE (-1, -1, -1);
static const arm_feature_set arm_arch_none = ARM_ARCH_NONE;
static int mfloat_abi_opt = -1;
/* Architecture feature bits selected by the last -mcpu/-march or .cpu/.arch
   directive.  */
static arm_feature_set selected_arch = ARM_ARCH_NONE;
/* Extension feature bits selected by the last -mcpu/-march or .arch_extension
   directive.  */
static arm_feature_set selected_ext = ARM_ARCH_NONE;
/* Feature bits selected by the last -mcpu/-march or by the combination of the
   last .cpu/.arch directive .arch_extension directives since that
   directive.  */
static arm_feature_set selected_cpu = ARM_ARCH_NONE;
/* FPU feature bits selected by the last -mfpu or .fpu directive.  */
static arm_feature_set selected_fpu = FPU_NONE;
/* Feature bits selected by the last .object_arch directive.  */
static arm_feature_set selected_object_arch = ARM_ARCH_NONE;
/* Must be long enough to hold any of the names in arm_cpus.  */
static char selected_cpu_name[20];

extern FLONUM_TYPE generic_floating_point_number;

/* Return if no cpu was selected on command-line.  */
static bfd_boolean
no_cpu_selected (void)
{
  return ARM_FEATURE_EQUAL (selected_cpu, arm_arch_none);
}

# ifdef EABI_DEFAULT
static int meabi_flags = EABI_DEFAULT;
# else
static int meabi_flags = EF_ARM_EABI_UNKNOWN;
# endif

static int attributes_set_explicitly[NUM_KNOWN_OBJ_ATTRIBUTES];

bfd_boolean
arm_is_eabi (void)
{
  return (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4);
}

/* Pre-defined "_GLOBAL_OFFSET_TABLE_"	*/
symbolS * GOT_symbol;

/* 0: assemble for ARM,
   1: assemble for Thumb,
   2: assemble for Thumb even though target CPU does not support thumb
      instructions.  */
static int thumb_mode = 0;
/* A value distinct from the possible values for thumb_mode that we
   can use to record whether thumb_mode has been copied into the
   tc_frag_data field of a frag.  */
#define MODE_RECORDED (1 << 4)

/* Specifies the intrinsic IT insn behavior mode.  */
enum implicit_it_mode
{
  IMPLICIT_IT_MODE_NEVER  = 0x00,
  IMPLICIT_IT_MODE_ARM    = 0x01,
  IMPLICIT_IT_MODE_THUMB  = 0x02,
  IMPLICIT_IT_MODE_ALWAYS = (IMPLICIT_IT_MODE_ARM | IMPLICIT_IT_MODE_THUMB)
};
static int implicit_it_mode = IMPLICIT_IT_MODE_ARM;

/* If unified_syntax is true, we are processing the new unified
   ARM/Thumb syntax.  Important differences from the old ARM mode:

     - Immediate operands do not require a # prefix.
     - Conditional affixes always appear at the end of the
       instruction.  (For backward compatibility, those instructions
       that formerly had them in the middle, continue to accept them
       there.)
     - The IT instruction may appear, and if it does is validated
       against subsequent conditional affixes.  It does not generate
       machine code.

   Important differences from the old Thumb mode:

     - Immediate operands do not require a # prefix.
     - Most of the V6T2 instructions are only available in unified mode.
     - The .N and .W suffixes are recognized and honored (it is an error
       if they cannot be honored).
     - All instructions set the flags if and only if they have an 's' affix.
     - Conditional affixes may be used.  They are validated against
       preceding IT instructions.  Unlike ARM mode, you cannot use a
       conditional affix except in the scope of an IT instruction.  */

static bfd_boolean unified_syntax = FALSE;

/* An immediate operand can start with #, and ld*, st*, pld operands
   can contain [ and ].  We need to tell APP not to elide whitespace
   before a [, which can appear as the first operand for pld.
   Likewise, a { can appear as the first operand for push, pop, vld*, etc.  */
const char arm_symbol_chars[] = "#[]{}";

enum neon_el_type
{
  NT_invtype,
  NT_untyped,
  NT_integer,
  NT_float,
  NT_poly,
  NT_signed,
  NT_unsigned
};

struct neon_type_el
{
  enum neon_el_type type;
  unsigned size;
};

#define NEON_MAX_TYPE_ELS 4

struct neon_type
{
  struct neon_type_el el[NEON_MAX_TYPE_ELS];
  unsigned elems;
};

enum it_instruction_type
{
   OUTSIDE_IT_INSN,
   INSIDE_IT_INSN,
   INSIDE_IT_LAST_INSN,
   IF_INSIDE_IT_LAST_INSN, /* Either outside or inside;
			      if inside, should be the last one.  */
   NEUTRAL_IT_INSN,        /* This could be either inside or outside,
			      i.e. BKPT and NOP.  */
   IT_INSN                 /* The IT insn has been parsed.  */
};

/* The maximum number of operands we need.  */
#define ARM_IT_MAX_OPERANDS 6

struct arm_it
{
  const char *	error;
  unsigned long instruction;
  int		size;
  int		size_req;
  int		cond;
  /* "uncond_value" is set to the value in place of the conditional field in
     unconditional versions of the instruction, or -1 if nothing is
     appropriate.  */
  int		uncond_value;
  struct neon_type vectype;
  /* This does not indicate an actual NEON instruction, only that
     the mnemonic accepts neon-style type suffixes.  */
  int		is_neon;
  /* Set to the opcode if the instruction needs relaxation.
     Zero if the instruction is not relaxed.  */
  unsigned long	relax;
  struct
  {
    bfd_reloc_code_real_type type;
    expressionS		     exp;
    int			     pc_rel;
  } reloc;

  enum it_instruction_type it_insn_type;

  struct
  {
    unsigned reg;
    signed int imm;
    struct neon_type_el vectype;
    unsigned present	: 1;  /* Operand present.  */
    unsigned isreg	: 1;  /* Operand was a register.  */
    unsigned immisreg	: 1;  /* .imm field is a second register.  */
    unsigned isscalar   : 1;  /* Operand is a (Neon) scalar.  */
    unsigned immisalign : 1;  /* Immediate is an alignment specifier.  */
    unsigned immisfloat : 1;  /* Immediate was parsed as a float.  */
    /* Note: we abuse "regisimm" to mean "is Neon register" in VMOV
       instructions. This allows us to disambiguate ARM <-> vector insns.  */
    unsigned regisimm   : 1;  /* 64-bit immediate, reg forms high 32 bits.  */
    unsigned isvec      : 1;  /* Is a single, double or quad VFP/Neon reg.  */
    unsigned isquad     : 1;  /* Operand is Neon quad-precision register.  */
    unsigned issingle   : 1;  /* Operand is VFP single-precision register.  */
    unsigned hasreloc	: 1;  /* Operand has relocation suffix.  */
    unsigned writeback	: 1;  /* Operand has trailing !  */
    unsigned preind	: 1;  /* Preindexed address.  */
    unsigned postind	: 1;  /* Postindexed address.  */
    unsigned negative	: 1;  /* Index register was negated.  */
    unsigned shifted	: 1;  /* Shift applied to operation.  */
    unsigned shift_kind : 3;  /* Shift operation (enum shift_kind).  */
  } operands[ARM_IT_MAX_OPERANDS];
};

static struct arm_it inst;

#define NUM_FLOAT_VALS 8

const char * fp_const[] =
{
  "0.0", "1.0", "2.0", "3.0", "4.0", "5.0", "0.5", "10.0", 0
};

/* Number of littlenums required to hold an extended precision number.	*/
#define MAX_LITTLENUMS 6

LITTLENUM_TYPE fp_values[NUM_FLOAT_VALS][MAX_LITTLENUMS];

#define FAIL	(-1)
#define SUCCESS (0)

#define SUFF_S 1
#define SUFF_E 3

#define CP_T_X	 0x00008000
#define CP_T_Y	 0x00400000

#define CONDS_BIT	 0x00100000
#define LOAD_BIT	 0x00100000

struct asm_cond
{
  const char *	 template_name;
  unsigned long  value;
};

#define COND_ALWAYS 0xE

struct asm_psr
{
  const char *   template_name;
  unsigned long  field;
};

struct asm_barrier_opt
{
  const char *    template_name;
  unsigned long   value;
  const arm_feature_set arch;
};

/* The bit that distinguishes CPSR and SPSR.  */
#define SPSR_BIT   (1 << 22)

/* The individual PSR flag bits.  */
#define PSR_c	(1 << 16)
#define PSR_x	(1 << 17)
#define PSR_s	(1 << 18)
#define PSR_f	(1 << 19)

struct reloc_entry
{
  const char *              name;
  bfd_reloc_code_real_type  reloc;
};

enum vfp_reg_pos
{
  VFP_REG_Sd, VFP_REG_Sm, VFP_REG_Sn,
  VFP_REG_Dd, VFP_REG_Dm, VFP_REG_Dn
};

enum vfp_ldstm_type
{
  VFP_LDSTMIA, VFP_LDSTMDB, VFP_LDSTMIAX, VFP_LDSTMDBX
};

/* Bits for DEFINED field in neon_typed_alias.  */
#define NTA_HASTYPE  1
#define NTA_HASINDEX 2

struct neon_typed_alias
{
  unsigned char        defined;
  unsigned char        index;
  struct neon_type_el  eltype;
};

/* ARM register categories.  This includes coprocessor numbers and various
   architecture extensions' registers.  Each entry should have an error message
   in reg_expected_msgs below.  */
enum arm_reg_type
{
  REG_TYPE_RN,
  REG_TYPE_CP,
  REG_TYPE_CN,
  REG_TYPE_FN,
  REG_TYPE_VFS,
  REG_TYPE_VFD,
  REG_TYPE_NQ,
  REG_TYPE_VFSD,
  REG_TYPE_NDQ,
  REG_TYPE_NSD,
  REG_TYPE_NSDQ,
  REG_TYPE_VFC,
  REG_TYPE_MVF,
  REG_TYPE_MVD,
  REG_TYPE_MVFX,
  REG_TYPE_MVDX,
  REG_TYPE_MVAX,
  REG_TYPE_DSPSC,
  REG_TYPE_MMXWR,
  REG_TYPE_MMXWC,
  REG_TYPE_MMXWCG,
  REG_TYPE_XSCALE,
  REG_TYPE_RNB
};

/* Structure for a hash table entry for a register.
   If TYPE is REG_TYPE_VFD or REG_TYPE_NQ, the NEON field can point to extra
   information which states whether a vector type or index is specified (for a
   register alias created with .dn or .qn). Otherwise NEON should be NULL.  */
struct reg_entry
{
  const char *               name;
  unsigned int               number;
  unsigned char              type;
  unsigned char              builtin;
  struct neon_typed_alias *  neon;
};

/* Diagnostics used when we don't get a register of the expected type.	*/
const char * const reg_expected_msgs[] =
{
  [REG_TYPE_RN]	    = N_("ARM register expected"),
  [REG_TYPE_CP]	    = N_("bad or missing co-processor number"),
  [REG_TYPE_CN]	    = N_("co-processor register expected"),
  [REG_TYPE_FN]	    = N_("FPA register expected"),
  [REG_TYPE_VFS]    = N_("VFP single precision register expected"),
  [REG_TYPE_VFD]    = N_("VFP/Neon double precision register expected"),
  [REG_TYPE_NQ]	    = N_("Neon quad precision register expected"),
  [REG_TYPE_VFSD]   = N_("VFP single or double precision register expected"),
  [REG_TYPE_NDQ]    = N_("Neon double or quad precision register expected"),
  [REG_TYPE_NSD]    = N_("Neon single or double precision register expected"),
  [REG_TYPE_NSDQ]   = N_("VFP single, double or Neon quad precision register"
			 " expected"),
  [REG_TYPE_VFC]    = N_("VFP system register expected"),
  [REG_TYPE_MMXWR]  = N_("iWMMXt data register expected"),
  [REG_TYPE_MMXWC]  = N_("iWMMXt control register expected"),
  [REG_TYPE_MMXWCG] = N_("iWMMXt scalar register expected"),
  [REG_TYPE_RNB]    = N_("")
};

/* Some well known registers that we refer to directly elsewhere.  */
#define REG_SP	13
#define REG_LR	14
#define REG_PC	15

/* ARM instructions take 4bytes in the object file, Thumb instructions
   take 2:  */
#define INSN_SIZE	4

struct asm_opcode
{
  /* Basic string to match.  */
  const char * template_name;

  /* Parameters to instruction.	 */
  unsigned int operands[8];

  /* Conditional tag - see opcode_lookup.  */
  unsigned int tag : 4;

  /* Basic instruction code.  */
  unsigned int avalue : 28;

  /* Thumb-format instruction code.  */
  unsigned int tvalue;

  /* Which architecture variant provides this instruction.  */
  const arm_feature_set * avariant;
  const arm_feature_set * tvariant;

  /* Function to call to encode instruction in ARM format.  */
  void (* aencode) (void);

  /* Function to call to encode instruction in Thumb format.  */
  void (* tencode) (void);
};

/* Defines for various bits that we will want to toggle.  */
#define INST_IMMEDIATE	0x02000000
#define OFFSET_REG	0x02000000
#define HWOFFSET_IMM	0x00400000
#define SHIFT_BY_REG	0x00000010
#define PRE_INDEX	0x01000000
#define INDEX_UP	0x00800000
#define WRITE_BACK	0x00200000
#define LDM_TYPE_2_OR_3	0x00400000
#define CPSI_MMOD	0x00020000

#define LITERAL_MASK	0xf000f000
#define OPCODE_MASK	0xfe1fffff
#define V4_STR_BIT	0x00000020

#define T2_SUBS_PC_LR	0xf3de8f00

#define DATA_OP_SHIFT	21
#define SBIT_SHIFT	20

#define T2_OPCODE_MASK	0xfe1fffff
#define T2_DATA_OP_SHIFT 21
#define T2_SBIT_SHIFT	 20

#define A_COND_MASK         0xf0000000
#define A_PUSH_POP_OP_MASK  0x0fff0000

/* Opcodes for pushing/poping registers to/from the stack.  */
#define A1_OPCODE_PUSH    0x092d0000
#define A2_OPCODE_PUSH    0x052d0004
#define A2_OPCODE_POP     0x049d0004

/* Codes to distinguish the arithmetic instructions.  */
#define OPCODE_AND	0
#define OPCODE_EOR	1
#define OPCODE_SUB	2
#define OPCODE_RSB	3
#define OPCODE_ADD	4
#define OPCODE_ADC	5
#define OPCODE_SBC	6
#define OPCODE_RSC	7
#define OPCODE_TST	8
#define OPCODE_TEQ	9
#define OPCODE_CMP	10
#define OPCODE_CMN	11
#define OPCODE_ORR	12
#define OPCODE_MOV	13
#define OPCODE_BIC	14
#define OPCODE_MVN	15

#define T2_OPCODE_AND	0
#define T2_OPCODE_BIC	1
#define T2_OPCODE_ORR	2
#define T2_OPCODE_ORN	3
#define T2_OPCODE_EOR	4
#define T2_OPCODE_ADD	8
#define T2_OPCODE_ADC	10
#define T2_OPCODE_SBC	11
#define T2_OPCODE_SUB	13
#define T2_OPCODE_RSB	14

#define T_OPCODE_MUL 0x4340
#define T_OPCODE_TST 0x4200
#define T_OPCODE_CMN 0x42c0
#define T_OPCODE_NEG 0x4240
#define T_OPCODE_MVN 0x43c0

#define T_OPCODE_ADD_R3	0x1800
#define T_OPCODE_SUB_R3 0x1a00
#define T_OPCODE_ADD_HI 0x4400
#define T_OPCODE_ADD_ST 0xb000
#define T_OPCODE_SUB_ST 0xb080
#define T_OPCODE_ADD_SP 0xa800
#define T_OPCODE_ADD_PC 0xa000
#define T_OPCODE_ADD_I8 0x3000
#define T_OPCODE_SUB_I8 0x3800
#define T_OPCODE_ADD_I3 0x1c00
#define T_OPCODE_SUB_I3 0x1e00

#define T_OPCODE_ASR_R	0x4100
#define T_OPCODE_LSL_R	0x4080
#define T_OPCODE_LSR_R	0x40c0
#define T_OPCODE_ROR_R	0x41c0
#define T_OPCODE_ASR_I	0x1000
#define T_OPCODE_LSL_I	0x0000
#define T_OPCODE_LSR_I	0x0800

#define T_OPCODE_MOV_I8	0x2000
#define T_OPCODE_CMP_I8 0x2800
#define T_OPCODE_CMP_LR 0x4280
#define T_OPCODE_MOV_HR 0x4600
#define T_OPCODE_CMP_HR 0x4500

#define T_OPCODE_LDR_PC 0x4800
#define T_OPCODE_LDR_SP 0x9800
#define T_OPCODE_STR_SP 0x9000
#define T_OPCODE_LDR_IW 0x6800
#define T_OPCODE_STR_IW 0x6000
#define T_OPCODE_LDR_IH 0x8800
#define T_OPCODE_STR_IH 0x8000
#define T_OPCODE_LDR_IB 0x7800
#define T_OPCODE_STR_IB 0x7000
#define T_OPCODE_LDR_RW 0x5800
#define T_OPCODE_STR_RW 0x5000
#define T_OPCODE_LDR_RH 0x5a00
#define T_OPCODE_STR_RH 0x5200
#define T_OPCODE_LDR_RB 0x5c00
#define T_OPCODE_STR_RB 0x5400

#define T_OPCODE_PUSH	0xb400
#define T_OPCODE_POP	0xbc00

#define T_OPCODE_BRANCH 0xe000

#define THUMB_SIZE	2	/* Size of thumb instruction.  */
#define THUMB_PP_PC_LR 0x0100
#define THUMB_LOAD_BIT 0x0800
#define THUMB2_LOAD_BIT 0x00100000

#define BAD_ARGS	_("bad arguments to instruction")
#define BAD_SP          _("r13 not allowed here")
#define BAD_PC		_("r15 not allowed here")
#define BAD_COND	_("instruction cannot be conditional")
#define BAD_OVERLAP	_("registers may not be the same")
#define BAD_HIREG	_("lo register required")
#define BAD_THUMB32	_("instruction not supported in Thumb16 mode")
#define BAD_BRANCH	_("branch must be last instruction in IT block")
#define BAD_NOT_IT	_("instruction not allowed in IT block")
#define BAD_FPU		_("selected FPU does not support instruction")
#define BAD_OUT_IT 	_("thumb conditional instruction should be in IT block")
#define BAD_IT_COND	_("incorrect condition in IT block")
#define BAD_IT_IT 	_("IT falling in the range of a previous IT block")
#define MISSING_FNSTART	_("missing .fnstart before unwinding directive")
#define BAD_PC_ADDRESSING \
	_("cannot use register index with PC-relative addressing")
#define BAD_PC_WRITEBACK \
	_("cannot use writeback with PC-relative addressing")
#define BAD_RANGE	_("branch out of range")
#define UNPRED_REG(R)	_("using " R " results in unpredictable behaviour")
#define THUMB1_RELOC_ONLY  _("relocation valid in thumb1 code only")

static struct hash_control * arm_ops_hsh;
static struct hash_control * arm_cond_hsh;
static struct hash_control * arm_shift_hsh;
static struct hash_control * arm_psr_hsh;
static struct hash_control * arm_reg_hsh;
static struct hash_control * arm_reloc_hsh;
static struct hash_control * arm_barrier_opt_hsh;

/* Stuff needed to resolve the label ambiguity
   As:
     ...
     label:   <insn>
   may differ from:
     ...
     label:
	      <insn>  */

symbolS *  last_label_seen;
static int label_is_thumb_function_name = FALSE;

/* Literal pool structure.  Held on a per-section
   and per-sub-section basis.  */

#define MAX_LITERAL_POOL_SIZE 1024
typedef struct literal_pool
{
  expressionS	         literals [MAX_LITERAL_POOL_SIZE];
  unsigned int	         next_free_entry;
  unsigned int	         id;
  symbolS *	         symbol;
  segT		         section;
  subsegT	         sub_section;
  struct dwarf2_line_info locs [MAX_LITERAL_POOL_SIZE];
  struct literal_pool *  next;
  unsigned int		 alignment;
} literal_pool;

/* Pointer to a linked list of literal pools.  */
literal_pool * list_of_pools = NULL;

typedef enum asmfunc_states
{
  OUTSIDE_ASMFUNC,
  WAITING_ASMFUNC_NAME,
  WAITING_ENDASMFUNC
} asmfunc_states;

static asmfunc_states asmfunc_state = OUTSIDE_ASMFUNC;

#define now_it seg_info (now_seg)->tc_segment_info_data.current_it

static inline int
now_it_compatible (int cond)
{
  return (cond & ~1) == (now_it.cc & ~1);
}

static int in_it_block (void);

static int handle_it_state (void);

static void force_automatic_it_block_close (void);

static void it_fsm_post_encode (void);

#define set_it_insn_type(type)			\
  do						\
    {						\
      inst.it_insn_type = type;			\
      if (handle_it_state () == FAIL)		\
	return;					\
    }						\
  while (0)

#define set_it_insn_type_nonvoid(type, failret) \
  do						\
    {                                           \
      inst.it_insn_type = type;			\
      if (handle_it_state () == FAIL)		\
	return failret;				\
    }						\
  while(0)

#define set_it_insn_type_last()				\
  do							\
    {							\
      if (inst.cond == COND_ALWAYS)			\
	set_it_insn_type (IF_INSIDE_IT_LAST_INSN);	\
      else						\
	set_it_insn_type (INSIDE_IT_LAST_INSN);		\
    }							\
  while (0)

/* Pure syntax.	 */

/* This array holds the chars that always start a comment.  If the
   pre-processor is disabled, these aren't very useful.	 */
char arm_comment_chars[] = "@";

/* This array holds the chars that only start a comment at the beginning of
   a line.  If the line seems to have the form '# 123 filename'
   .line and .file directives will appear in the pre-processed output.	*/
/* Note that input_file.c hand checks for '#' at the beginning of the
   first line of the input file.  This is because the compiler outputs
   #NO_APP at the beginning of its output.  */
/* Also note that comments like this one will always work.  */
const char line_comment_chars[] = "#";

char arm_line_separator_chars[] = ";";

/* Chars that can be used to separate mant
   from exp in floating point numbers.	*/
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.  */
/* As in 0f12.456  */
/* or	 0d1.2345e12  */

const char FLT_CHARS[] = "rRsSfFdDxXeEpP";

/* Prefix characters that indicate the start of an immediate
   value.  */
#define is_immediate_prefix(C) ((C) == '#' || (C) == '$')

/* Separator character handling.  */

#define skip_whitespace(str)  do { if (*(str) == ' ') ++(str); } while (0)

static inline int
skip_past_char (char ** str, char c)
{
  /* PR gas/14987: Allow for whitespace before the expected character.  */
  skip_whitespace (*str);

  if (**str == c)
    {
      (*str)++;
      return SUCCESS;
    }
  else
    return FAIL;
}

#define skip_past_comma(str) skip_past_char (str, ',')

/* Arithmetic expressions (possibly involving symbols).	 */

/* Return TRUE if anything in the expression is a bignum.  */

static bfd_boolean
walk_no_bignums (symbolS * sp)
{
  if (symbol_get_value_expression (sp)->X_op == O_big)
    return TRUE;

  if (symbol_get_value_expression (sp)->X_add_symbol)
    {
      return (walk_no_bignums (symbol_get_value_expression (sp)->X_add_symbol)
	      || (symbol_get_value_expression (sp)->X_op_symbol
		  && walk_no_bignums (symbol_get_value_expression (sp)->X_op_symbol)));
    }

  return FALSE;
}

static bfd_boolean in_my_get_expression = FALSE;

/* Third argument to my_get_expression.	 */
#define GE_NO_PREFIX 0
#define GE_IMM_PREFIX 1
#define GE_OPT_PREFIX 2
/* This is a bit of a hack. Use an optional prefix, and also allow big (64-bit)
   immediates, as can be used in Neon VMVN and VMOV immediate instructions.  */
#define GE_OPT_PREFIX_BIG 3

static int
my_get_expression (expressionS * ep, char ** str, int prefix_mode)
{
  char * save_in;

  /* In unified syntax, all prefixes are optional.  */
  if (unified_syntax)
    prefix_mode = (prefix_mode == GE_OPT_PREFIX_BIG) ? prefix_mode
		  : GE_OPT_PREFIX;

  switch (prefix_mode)
    {
    case GE_NO_PREFIX: break;
    case GE_IMM_PREFIX:
      if (!is_immediate_prefix (**str))
	{
	  inst.error = _("immediate expression requires a # prefix");
	  return FAIL;
	}
      (*str)++;
      break;
    case GE_OPT_PREFIX:
    case GE_OPT_PREFIX_BIG:
      if (is_immediate_prefix (**str))
	(*str)++;
      break;
    default:
      abort ();
    }

  memset (ep, 0, sizeof (expressionS));

  save_in = input_line_pointer;
  input_line_pointer = *str;
  in_my_get_expression = TRUE;
  expression (ep);
  in_my_get_expression = FALSE;

  if (ep->X_op == O_illegal || ep->X_op == O_absent)
    {
      /* We found a bad or missing expression in md_operand().  */
      *str = input_line_pointer;
      input_line_pointer = save_in;
      if (inst.error == NULL)
	inst.error = (ep->X_op == O_absent
		      ? _("missing expression") :_("bad expression"));
      return 1;
    }

  /* Get rid of any bignums now, so that we don't generate an error for which
     we can't establish a line number later on.	 Big numbers are never valid
     in instructions, which is where this routine is always called.  */
  if (prefix_mode != GE_OPT_PREFIX_BIG
      && (ep->X_op == O_big
	  || (ep->X_add_symbol
	      && (walk_no_bignums (ep->X_add_symbol)
		  || (ep->X_op_symbol
		      && walk_no_bignums (ep->X_op_symbol))))))
    {
      inst.error = _("invalid constant");
      *str = input_line_pointer;
      input_line_pointer = save_in;
      return 1;
    }

  *str = input_line_pointer;
  input_line_pointer = save_in;
  return SUCCESS;
}

/* Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.

   Note that fp constants aren't represent in the normal way on the ARM.
   In big endian mode, things are as expected.	However, in little endian
   mode fp constants are big-endian word-wise, and little-endian byte-wise
   within the words.  For example, (double) 1.1 in big endian mode is
   the byte sequence 3f f1 99 99 99 99 99 9a, and in little endian mode is
   the byte sequence 99 99 f1 3f 9a 99 99 99.

   ??? The format of 12 byte floats is uncertain according to gcc's arm.h.  */

const char *
md_atof (int type, char * litP, int * sizeP)
{
  int prec;
  LITTLENUM_TYPE words[MAX_LITTLENUMS];
  char *t;
  int i;

  switch (type)
    {
    case 'f':
    case 'F':
    case 's':
    case 'S':
      prec = 2;
      break;

    case 'd':
    case 'D':
    case 'r':
    case 'R':
      prec = 4;
      break;

    case 'x':
    case 'X':
      prec = 5;
      break;

    case 'p':
    case 'P':
      prec = 5;
      break;

    default:
      *sizeP = 0;
      return _("Unrecognized or unsupported floating point constant");
    }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
    input_line_pointer = t;
  *sizeP = prec * sizeof (LITTLENUM_TYPE);

  if (target_big_endian)
    {
      for (i = 0; i < prec; i++)
	{
	  md_number_to_chars (litP, (valueT) words[i], sizeof (LITTLENUM_TYPE));
	  litP += sizeof (LITTLENUM_TYPE);
	}
    }
  else
    {
	/* For a 4 byte float the order of elements in `words' is 1 0.
	   For an 8 byte float the order is 1 0 3 2.  */
	for (i = 0; i < prec; i += 2)
	  {
	    md_number_to_chars (litP, (valueT) words[i + 1],
				sizeof (LITTLENUM_TYPE));
	    md_number_to_chars (litP + sizeof (LITTLENUM_TYPE),
				(valueT) words[i], sizeof (LITTLENUM_TYPE));
	    litP += 2 * sizeof (LITTLENUM_TYPE);
	  }
    }

  return NULL;
}

/* We handle all bad expressions here, so that we can report the faulty
   instruction in the error message.  */

void
md_operand (expressionS * exp)
{
  if (in_my_get_expression)
    exp->X_op = O_illegal;
}

/* Immediate values.  */

/* Generic immediate-value read function for use in directives.
   Accepts anything that 'expression' can fold to a constant.
   *val receives the number.  */

static int
immediate_for_directive (int *val)
{
  expressionS exp;
  exp.X_op = O_illegal;

  if (is_immediate_prefix (*input_line_pointer))
    {
      input_line_pointer++;
      expression (&exp);
    }

  if (exp.X_op != O_constant)
    {
      as_bad (_("expected #constant"));
      ignore_rest_of_line ();
      return FAIL;
    }
  *val = exp.X_add_number;
  return SUCCESS;
}

/* Register parsing.  */

/* Generic register parser.  CCP points to what should be the
   beginning of a register name.  If it is indeed a valid register
   name, advance CCP over it and return the reg_entry structure;
   otherwise return NULL.  Does not issue diagnostics.	*/

static struct reg_entry *
arm_reg_parse_multi (char **ccp)
{
  char *start = *ccp;
  char *p;
  struct reg_entry *reg;

  skip_whitespace (start);

#ifdef REGISTER_PREFIX
  if (*start != REGISTER_PREFIX)
    return NULL;
  start++;
#endif
#ifdef OPTIONAL_REGISTER_PREFIX
  if (*start == OPTIONAL_REGISTER_PREFIX)
    start++;
#endif

  p = start;
  if (!ISALPHA (*p) || !is_name_beginner (*p))
    return NULL;

  do
    p++;
  while (ISALPHA (*p) || ISDIGIT (*p) || *p == '_');

  reg = (struct reg_entry *) hash_find_n (arm_reg_hsh, start, p - start);

  if (!reg)
    return NULL;

  *ccp = p;
  return reg;
}

static int
arm_reg_alt_syntax (char **ccp, char *start, struct reg_entry *reg,
		    enum arm_reg_type type)
{
  /* Alternative syntaxes are accepted for a few register classes.  */
  switch (type)
    {
    case REG_TYPE_MVF:
    case REG_TYPE_MVD:
    case REG_TYPE_MVFX:
    case REG_TYPE_MVDX:
      /* Generic coprocessor register names are allowed for these.  */
      if (reg && reg->type == REG_TYPE_CN)
	return reg->number;
      break;

    case REG_TYPE_CP:
      /* For backward compatibility, a bare number is valid here.  */
      {
	unsigned long processor = strtoul (start, ccp, 10);
	if (*ccp != start && processor <= 15)
	  return processor;
      }
      /* Fall through.  */

    case REG_TYPE_MMXWC:
      /* WC includes WCG.  ??? I'm not sure this is true for all
	 instructions that take WC registers.  */
      if (reg && reg->type == REG_TYPE_MMXWCG)
	return reg->number;
      break;

    default:
      break;
    }

  return FAIL;
}

/* As arm_reg_parse_multi, but the register must be of type TYPE, and the
   return value is the register number or FAIL.  */

static int
arm_reg_parse (char **ccp, enum arm_reg_type type)
{
  char *start = *ccp;
  struct reg_entry *reg = arm_reg_parse_multi (ccp);
  int ret;

  /* Do not allow a scalar (reg+index) to parse as a register.  */
  if (reg && reg->neon && (reg->neon->defined & NTA_HASINDEX))
    return FAIL;

  if (reg && reg->type == type)
    return reg->number;

  if ((ret = arm_reg_alt_syntax (ccp, start, reg, type)) != FAIL)
    return ret;

  *ccp = start;
  return FAIL;
}

/* Parse a Neon type specifier. *STR should point at the leading '.'
   character. Does no verification at this stage that the type fits the opcode
   properly. E.g.,

     .i32.i32.s16
     .s32.f32
     .u16

   Can all be legally parsed by this function.

   Fills in neon_type struct pointer with parsed information, and updates STR
   to point after the parsed type specifier. Returns SUCCESS if this was a legal
   type, FAIL if not.  */

static int
parse_neon_type (struct neon_type *type, char **str)
{
  char *ptr = *str;

  if (type)
    type->elems = 0;

  while (type->elems < NEON_MAX_TYPE_ELS)
    {
      enum neon_el_type thistype = NT_untyped;
      unsigned thissize = -1u;

      if (*ptr != '.')
	break;

      ptr++;

      /* Just a size without an explicit type.  */
      if (ISDIGIT (*ptr))
	goto parsesize;

      switch (TOLOWER (*ptr))
	{
	case 'i': thistype = NT_integer; break;
	case 'f': thistype = NT_float; break;
	case 'p': thistype = NT_poly; break;
	case 's': thistype = NT_signed; break;
	case 'u': thistype = NT_unsigned; break;
	case 'd':
	  thistype = NT_float;
	  thissize = 64;
	  ptr++;
	  goto done;
	default:
	  as_bad (_("unexpected character `%c' in type specifier"), *ptr);
	  return FAIL;
	}

      ptr++;

      /* .f is an abbreviation for .f32.  */
      if (thistype == NT_float && !ISDIGIT (*ptr))
	thissize = 32;
      else
	{
	parsesize:
	  thissize = strtoul (ptr, &ptr, 10);

	  if (thissize != 8 && thissize != 16 && thissize != 32
	      && thissize != 64)
	    {
	      as_bad (_("bad size %d in type specifier"), thissize);
	      return FAIL;
	    }
	}

      done:
      if (type)
	{
	  type->el[type->elems].type = thistype;
	  type->el[type->elems].size = thissize;
	  type->elems++;
	}
    }

  /* Empty/missing type is not a successful parse.  */
  if (type->elems == 0)
    return FAIL;

  *str = ptr;

  return SUCCESS;
}

/* Errors may be set multiple times during parsing or bit encoding
   (particularly in the Neon bits), but usually the earliest error which is set
   will be the most meaningful. Avoid overwriting it with later (cascading)
   errors by calling this function.  */

static void
first_error (const char *err)
{
  if (!inst.error)
    inst.error = err;
}

/* Parse a single type, e.g. ".s32", leading period included.  */
static int
parse_neon_operand_type (struct neon_type_el *vectype, char **ccp)
{
  char *str = *ccp;
  struct neon_type optype;

  if (*str == '.')
    {
      if (parse_neon_type (&optype, &str) == SUCCESS)
	{
	  if (optype.elems == 1)
	    *vectype = optype.el[0];
	  else
	    {
	      first_error (_("only one type should be specified for operand"));
	      return FAIL;
	    }
	}
      else
	{
	  first_error (_("vector type expected"));
	  return FAIL;
	}
    }
  else
    return FAIL;

  *ccp = str;

  return SUCCESS;
}

/* Special meanings for indices (which have a range of 0-7), which will fit into
   a 4-bit integer.  */

#define NEON_ALL_LANES		15
#define NEON_INTERLEAVE_LANES	14

/* Parse either a register or a scalar, with an optional type. Return the
   register number, and optionally fill in the actual type of the register
   when multiple alternatives were given (NEON_TYPE_NDQ) in *RTYPE, and
   type/index information in *TYPEINFO.  */

static int
parse_typed_reg_or_scalar (char **ccp, enum arm_reg_type type,
			   enum arm_reg_type *rtype,
			   struct neon_typed_alias *typeinfo)
{
  char *str = *ccp;
  struct reg_entry *reg = arm_reg_parse_multi (&str);
  struct neon_typed_alias atype;
  struct neon_type_el parsetype;

  atype.defined = 0;
  atype.index = -1;
  atype.eltype.type = NT_invtype;
  atype.eltype.size = -1;

  /* Try alternate syntax for some types of register. Note these are mutually
     exclusive with the Neon syntax extensions.  */
  if (reg == NULL)
    {
      int altreg = arm_reg_alt_syntax (&str, *ccp, reg, type);
      if (altreg != FAIL)
	*ccp = str;
      if (typeinfo)
	*typeinfo = atype;
      return altreg;
    }

  /* Undo polymorphism when a set of register types may be accepted.  */
  if ((type == REG_TYPE_NDQ
       && (reg->type == REG_TYPE_NQ || reg->type == REG_TYPE_VFD))
      || (type == REG_TYPE_VFSD
	  && (reg->type == REG_TYPE_VFS || reg->type == REG_TYPE_VFD))
      || (type == REG_TYPE_NSDQ
	  && (reg->type == REG_TYPE_VFS || reg->type == REG_TYPE_VFD
	      || reg->type == REG_TYPE_NQ))
      || (type == REG_TYPE_NSD
	  && (reg->type == REG_TYPE_VFS || reg->type == REG_TYPE_VFD))
      || (type == REG_TYPE_MMXWC
	  && (reg->type == REG_TYPE_MMXWCG)))
    type = (enum arm_reg_type) reg->type;

  if (type != reg->type)
    return FAIL;

  if (reg->neon)
    atype = *reg->neon;

  if (parse_neon_operand_type (&parsetype, &str) == SUCCESS)
    {
      if ((atype.defined & NTA_HASTYPE) != 0)
	{
	  first_error (_("can't redefine type for operand"));
	  return FAIL;
	}
      atype.defined |= NTA_HASTYPE;
      atype.eltype = parsetype;
    }

  if (skip_past_char (&str, '[') == SUCCESS)
    {
      if (type != REG_TYPE_VFD
	  && !(type == REG_TYPE_VFS))
	{
	  first_error (_("only D registers may be indexed"));
	  return FAIL;
	}

      if ((atype.defined & NTA_HASINDEX) != 0)
	{
	  first_error (_("can't change index for operand"));
	  return FAIL;
	}

      atype.defined |= NTA_HASINDEX;

      if (skip_past_char (&str, ']') == SUCCESS)
	atype.index = NEON_ALL_LANES;
      else
	{
	  expressionS exp;

	  my_get_expression (&exp, &str, GE_NO_PREFIX);

	  if (exp.X_op != O_constant)
	    {
	      first_error (_("constant expression required"));
	      return FAIL;
	    }

	  if (skip_past_char (&str, ']') == FAIL)
	    return FAIL;

	  atype.index = exp.X_add_number;
	}
    }

  if (typeinfo)
    *typeinfo = atype;

  if (rtype)
    *rtype = type;

  *ccp = str;

  return reg->number;
}

/* Like arm_reg_parse, but allow allow the following extra features:
    - If RTYPE is non-zero, return the (possibly restricted) type of the
      register (e.g. Neon double or quad reg when either has been requested).
    - If this is a Neon vector type with additional type information, fill
      in the struct pointed to by VECTYPE (if non-NULL).
   This function will fault on encountering a scalar.  */

static int
arm_typed_reg_parse (char **ccp, enum arm_reg_type type,
		     enum arm_reg_type *rtype, struct neon_type_el *vectype)
{
  struct neon_typed_alias atype;
  char *str = *ccp;
  int reg = parse_typed_reg_or_scalar (&str, type, rtype, &atype);

  if (reg == FAIL)
    return FAIL;

  /* Do not allow regname(... to parse as a register.  */
  if (*str == '(')
    return FAIL;

  /* Do not allow a scalar (reg+index) to parse as a register.  */
  if ((atype.defined & NTA_HASINDEX) != 0)
    {
      first_error (_("register operand expected, but got scalar"));
      return FAIL;
    }

  if (vectype)
    *vectype = atype.eltype;

  *ccp = str;

  return reg;
}

/* Parse a Neon scalar. Most of the time when we're parsing a scalar, we don't
   have enough information to be able to do a good job bounds-checking. So, we
   just do easy checks here, and do further checks later.  */

static int
parse_scalar (char **ccp, int elsize, struct neon_type_el *type)
{
  int reg;
  char *str = *ccp;
  struct neon_typed_alias atype;
  enum arm_reg_type reg_type = REG_TYPE_VFD;

  if (elsize == 4)
    reg_type = REG_TYPE_VFS;

  reg = parse_typed_reg_or_scalar (&str, reg_type, NULL, &atype);

  if (reg == FAIL || (atype.defined & NTA_HASINDEX) == 0)
    return FAIL;

  if (atype.index == NEON_ALL_LANES)
    {
      first_error (_("scalar must have an index"));
      return FAIL;
    }
  else if (atype.index >= 64 / elsize)
    {
      first_error (_("scalar index out of range"));
      return FAIL;
    }

  if (type)
    *type = atype.eltype;

  *ccp = str;

  return reg * 16 + atype.index;
}

/* Parse an ARM register list.  Returns the bitmask, or FAIL.  */

static long
parse_reg_list (char ** strp)
{
  char * str = * strp;
  long	 range = 0;
  int	 another_range;

  /* We come back here if we get ranges concatenated by '+' or '|'.  */
  do
    {
      skip_whitespace (str);

      another_range = 0;

      if (*str == '{')
	{
	  int in_range = 0;
	  int cur_reg = -1;

	  str++;
	  do
	    {
	      int reg;

	      if ((reg = arm_reg_parse (&str, REG_TYPE_RN)) == FAIL)
		{
		  first_error (_(reg_expected_msgs[REG_TYPE_RN]));
		  return FAIL;
		}

	      if (in_range)
		{
		  int i;

		  if (reg <= cur_reg)
		    {
		      first_error (_("bad range in register list"));
		      return FAIL;
		    }

		  for (i = cur_reg + 1; i < reg; i++)
		    {
		      if (range & (1 << i))
			as_tsktsk
			  (_("Warning: duplicated register (r%d) in register list"),
			   i);
		      else
			range |= 1 << i;
		    }
		  in_range = 0;
		}

	      if (range & (1 << reg))
		as_tsktsk (_("Warning: duplicated register (r%d) in register list"),
			   reg);
	      else if (reg <= cur_reg)
		as_tsktsk (_("Warning: register range not in ascending order"));

	      range |= 1 << reg;
	      cur_reg = reg;
	    }
	  while (skip_past_comma (&str) != FAIL
		 || (in_range = 1, *str++ == '-'));
	  str--;

	  if (skip_past_char (&str, '}') == FAIL)
	    {
	      first_error (_("missing `}'"));
	      return FAIL;
	    }
	}
      else
	{
	  expressionS exp;

	  if (my_get_expression (&exp, &str, GE_NO_PREFIX))
	    return FAIL;

	  if (exp.X_op == O_constant)
	    {
	      if (exp.X_add_number
		  != (exp.X_add_number & 0x0000ffff))
		{
		  inst.error = _("invalid register mask");
		  return FAIL;
		}

	      if ((range & exp.X_add_number) != 0)
		{
		  int regno = range & exp.X_add_number;

		  regno &= -regno;
		  regno = (1 << regno) - 1;
		  as_tsktsk
		    (_("Warning: duplicated register (r%d) in register list"),
		     regno);
		}

	      range |= exp.X_add_number;
	    }
	  else
	    {
	      if (inst.reloc.type != 0)
		{
		  inst.error = _("expression too complex");
		  return FAIL;
		}

	      memcpy (&inst.reloc.exp, &exp, sizeof (expressionS));
	      inst.reloc.type = BFD_RELOC_ARM_MULTI;
	      inst.reloc.pc_rel = 0;
	    }
	}

      if (*str == '|' || *str == '+')
	{
	  str++;
	  another_range = 1;
	}
    }
  while (another_range);

  *strp = str;
  return range;
}

/* Types of registers in a list.  */

enum reg_list_els
{
  REGLIST_VFP_S,
  REGLIST_VFP_D,
  REGLIST_NEON_D
};

/* Parse a VFP register list.  If the string is invalid return FAIL.
   Otherwise return the number of registers, and set PBASE to the first
   register.  Parses registers of type ETYPE.
   If REGLIST_NEON_D is used, several syntax enhancements are enabled:
     - Q registers can be used to specify pairs of D registers
     - { } can be omitted from around a singleton register list
	 FIXME: This is not implemented, as it would require backtracking in
	 some cases, e.g.:
	   vtbl.8 d3,d4,d5
	 This could be done (the meaning isn't really ambiguous), but doesn't
	 fit in well with the current parsing framework.
     - 32 D registers may be used (also true for VFPv3).
   FIXME: Types are ignored in these register lists, which is probably a
   bug.  */

static int
parse_vfp_reg_list (char **ccp, unsigned int *pbase, enum reg_list_els etype)
{
  char *str = *ccp;
  int base_reg;
  int new_base;
  enum arm_reg_type regtype = (enum arm_reg_type) 0;
  int max_regs = 0;
  int count = 0;
  int warned = 0;
  unsigned long mask = 0;
  int i;

  if (skip_past_char (&str, '{') == FAIL)
    {
      inst.error = _("expecting {");
      return FAIL;
    }

  switch (etype)
    {
    case REGLIST_VFP_S:
      regtype = REG_TYPE_VFS;
      max_regs = 32;
      break;

    case REGLIST_VFP_D:
      regtype = REG_TYPE_VFD;
      break;

    case REGLIST_NEON_D:
      regtype = REG_TYPE_NDQ;
      break;
    }

  if (etype != REGLIST_VFP_S)
    {
      /* VFPv3 allows 32 D registers, except for the VFPv3-D16 variant.  */

	max_regs = 16;
    }

  base_reg = max_regs;

  do
    {
      int setmask = 1, addregs = 1;

      new_base = arm_typed_reg_parse (&str, regtype, &regtype, NULL);

      if (new_base == FAIL)
	{
	  first_error (_(reg_expected_msgs[regtype]));
	  return FAIL;
	}

      if (new_base >= max_regs)
	{
	  first_error (_("register out of range in list"));
	  return FAIL;
	}

      /* Note: a value of 2 * n is returned for the register Q<n>.  */
      if (regtype == REG_TYPE_NQ)
	{
	  setmask = 3;
	  addregs = 2;
	}

      if (new_base < base_reg)
	base_reg = new_base;

      if (mask & (setmask << new_base))
	{
	  first_error (_("invalid register list"));
	  return FAIL;
	}

      if ((mask >> new_base) != 0 && ! warned)
	{
	  as_tsktsk (_("register list not in ascending order"));
	  warned = 1;
	}

      mask |= setmask << new_base;
      count += addregs;

      if (*str == '-') /* We have the start of a range expression */
	{
	  int high_range;

	  str++;

	  if ((high_range = arm_typed_reg_parse (&str, regtype, NULL, NULL))
	      == FAIL)
	    {
	      inst.error = gettext (reg_expected_msgs[regtype]);
	      return FAIL;
	    }

	  if (high_range >= max_regs)
	    {
	      first_error (_("register out of range in list"));
	      return FAIL;
	    }

	  if (regtype == REG_TYPE_NQ)
	    high_range = high_range + 1;

	  if (high_range <= new_base)
	    {
	      inst.error = _("register range not in ascending order");
	      return FAIL;
	    }

	  for (new_base += addregs; new_base <= high_range; new_base += addregs)
	    {
	      if (mask & (setmask << new_base))
		{
		  inst.error = _("invalid register list");
		  return FAIL;
		}

	      mask |= setmask << new_base;
	      count += addregs;
	    }
	}
    }
  while (skip_past_comma (&str) != FAIL);

  str++;

  /* Sanity check -- should have raised a parse error above.  */
  if (count == 0 || count > max_regs)
    abort ();

  *pbase = base_reg;

  /* Final test -- the registers must be consecutive.  */
  mask >>= base_reg;
  for (i = 0; i < count; i++)
    {
      if ((mask & (1u << i)) == 0)
	{
	  inst.error = _("non-contiguous register range");
	  return FAIL;
	}
    }

  *ccp = str;

  return count;
}

/* True if two alias types are the same.  */

static bfd_boolean
neon_alias_types_same (struct neon_typed_alias *a, struct neon_typed_alias *b)
{
  if (!a && !b)
    return TRUE;

  if (!a || !b)
    return FALSE;

  if (a->defined != b->defined)
    return FALSE;

  if ((a->defined & NTA_HASTYPE) != 0
      && (a->eltype.type != b->eltype.type
	  || a->eltype.size != b->eltype.size))
    return FALSE;

  if ((a->defined & NTA_HASINDEX) != 0
      && (a->index != b->index))
    return FALSE;

  return TRUE;
}

/* Parse element/structure lists for Neon VLD<n> and VST<n> instructions.
   The base register is put in *PBASE.
   The lane (or one of the NEON_*_LANES constants) is placed in bits [3:0] of
   the return value.
   The register stride (minus one) is put in bit 4 of the return value.
   Bits [6:5] encode the list length (minus one).
   The type of the list elements is put in *ELTYPE, if non-NULL.  */

static int
parse_neon_el_struct_list (char **str, unsigned *pbase,
			   struct neon_type_el *eltype)
{
  char *ptr = *str;
  int base_reg = -1;
  int reg_incr = -1;
  int count = 0;
  int lane = -1;
  int leading_brace = 0;
  enum arm_reg_type rtype = REG_TYPE_NDQ;
  const char *const incr_error = _("register stride must be 1 or 2");
  const char *const type_error = _("mismatched element/structure types in list");
  struct neon_typed_alias firsttype;
  firsttype.defined = 0;
  firsttype.eltype.type = NT_invtype;
  firsttype.eltype.size = -1;
  firsttype.index = -1;

  if (skip_past_char (&ptr, '{') == SUCCESS)
    leading_brace = 1;

  do
    {
      struct neon_typed_alias atype;
      int getreg = parse_typed_reg_or_scalar (&ptr, rtype, &rtype, &atype);

      if (getreg == FAIL)
	{
	  first_error (_(reg_expected_msgs[rtype]));
	  return FAIL;
	}

      if (base_reg == -1)
	{
	  base_reg = getreg;
	  if (rtype == REG_TYPE_NQ)
	    {
	      reg_incr = 1;
	    }
	  firsttype = atype;
	}
      else if (reg_incr == -1)
	{
	  reg_incr = getreg - base_reg;
	  if (reg_incr < 1 || reg_incr > 2)
	    {
	      first_error (_(incr_error));
	      return FAIL;
	    }
	}
      else if (getreg != base_reg + reg_incr * count)
	{
	  first_error (_(incr_error));
	  return FAIL;
	}

      if (! neon_alias_types_same (&atype, &firsttype))
	{
	  first_error (_(type_error));
	  return FAIL;
	}

      /* Handle Dn-Dm or Qn-Qm syntax. Can only be used with non-indexed list
	 modes.  */
      if (ptr[0] == '-')
	{
	  struct neon_typed_alias htype;
	  int hireg, dregs = (rtype == REG_TYPE_NQ) ? 2 : 1;
	  if (lane == -1)
	    lane = NEON_INTERLEAVE_LANES;
	  else if (lane != NEON_INTERLEAVE_LANES)
	    {
	      first_error (_(type_error));
	      return FAIL;
	    }
	  if (reg_incr == -1)
	    reg_incr = 1;
	  else if (reg_incr != 1)
	    {
	      first_error (_("don't use Rn-Rm syntax with non-unit stride"));
	      return FAIL;
	    }
	  ptr++;
	  hireg = parse_typed_reg_or_scalar (&ptr, rtype, NULL, &htype);
	  if (hireg == FAIL)
	    {
	      first_error (_(reg_expected_msgs[rtype]));
	      return FAIL;
	    }
	  if (! neon_alias_types_same (&htype, &firsttype))
	    {
	      first_error (_(type_error));
	      return FAIL;
	    }
	  count += hireg + dregs - getreg;
	  continue;
	}

      /* If we're using Q registers, we can't use [] or [n] syntax.  */
      if (rtype == REG_TYPE_NQ)
	{
	  count += 2;
	  continue;
	}

      if ((atype.defined & NTA_HASINDEX) != 0)
	{
	  if (lane == -1)
	    lane = atype.index;
	  else if (lane != atype.index)
	    {
	      first_error (_(type_error));
	      return FAIL;
	    }
	}
      else if (lane == -1)
	lane = NEON_INTERLEAVE_LANES;
      else if (lane != NEON_INTERLEAVE_LANES)
	{
	  first_error (_(type_error));
	  return FAIL;
	}
      count++;
    }
  while ((count != 1 || leading_brace) && skip_past_comma (&ptr) != FAIL);

  /* No lane set by [x]. We must be interleaving structures.  */
  if (lane == -1)
    lane = NEON_INTERLEAVE_LANES;

  /* Sanity check.  */
  if (lane == -1 || base_reg == -1 || count < 1 || count > 4
      || (count > 1 && reg_incr == -1))
    {
      first_error (_("error parsing element/structure list"));
      return FAIL;
    }

  if ((count > 1 || leading_brace) && skip_past_char (&ptr, '}') == FAIL)
    {
      first_error (_("expected }"));
      return FAIL;
    }

  if (reg_incr == -1)
    reg_incr = 1;

  if (eltype)
    *eltype = firsttype.eltype;

  *pbase = base_reg;
  *str = ptr;

  return lane | ((reg_incr - 1) << 4) | ((count - 1) << 5);
}

/* Parse an explicit relocation suffix on an expression.  This is
   either nothing, or a word in parentheses.  Note that if !OBJ_ELF,
   arm_reloc_hsh contains no entries, so this function can only
   succeed if there is no () after the word.  Returns -1 on error,
   BFD_RELOC_UNUSED if there wasn't any suffix.	 */

static int
parse_reloc (char **str)
{
  struct reloc_entry *r;
  char *p, *q;

  if (**str != '(')
    return BFD_RELOC_UNUSED;

  p = *str + 1;
  q = p;

  while (*q && *q != ')' && *q != ',')
    q++;
  if (*q != ')')
    return -1;

  if ((r = (struct reloc_entry *)
       hash_find_n (arm_reloc_hsh, p, q - p)) == NULL)
    return -1;

  *str = q + 1;
  return r->reloc;
}

/* Directives: register aliases.  */

static struct reg_entry *
insert_reg_alias (char *str, unsigned number, int type)
{
  struct reg_entry *new_reg;
  const char *name;

  if ((new_reg = (struct reg_entry *) hash_find (arm_reg_hsh, str)) != 0)
    {
      if (new_reg->builtin)
	as_warn (_("ignoring attempt to redefine built-in register '%s'"), str);

      /* Only warn about a redefinition if it's not defined as the
	 same register.	 */
      else if (new_reg->number != number || new_reg->type != type)
	as_warn (_("ignoring redefinition of register alias '%s'"), str);

      return NULL;
    }

  name = strdup (str);
  new_reg = XNEW (struct reg_entry);

  new_reg->name = name;
  new_reg->number = number;
  new_reg->type = type;
  new_reg->builtin = FALSE;
  new_reg->neon = NULL;

  if (hash_insert (arm_reg_hsh, name, (void *) new_reg))
    abort ();

  return new_reg;
}

static void
insert_neon_reg_alias (char *str, int number, int type,
		       struct neon_typed_alias *atype)
{
  struct reg_entry *reg = insert_reg_alias (str, number, type);

  if (!reg)
    {
      first_error (_("attempt to redefine typed alias"));
      return;
    }

  if (atype)
    {
      reg->neon = XNEW (struct neon_typed_alias);
      *reg->neon = *atype;
    }
}

/* Look for the .req directive.	 This is of the form:

	new_register_name .req existing_register_name

   If we find one, or if it looks sufficiently like one that we want to
   handle any error here, return TRUE.  Otherwise return FALSE.  */

static bfd_boolean
create_register_alias (char * newname, char *p)
{
  struct reg_entry *old;
  char *oldname, *nbuf;
  size_t nlen;

  /* The input scrubber ensures that whitespace after the mnemonic is
     collapsed to single spaces.  */
  oldname = p;
  if (strncmp (oldname, " .req ", 6) != 0)
    return FALSE;

  oldname += 6;
  if (*oldname == '\0')
    return FALSE;

  old = (struct reg_entry *) hash_find (arm_reg_hsh, oldname);
  if (!old)
    {
      as_warn (_("unknown register '%s' -- .req ignored"), oldname);
      return TRUE;
    }

  /* If TC_CASE_SENSITIVE is defined, then newname already points to
     the desired alias name, and p points to its end.  If not, then
     the desired alias name is in the global original_case_string.  */
#ifdef TC_CASE_SENSITIVE
  nlen = p - newname;
#else
  newname = original_case_string;
  nlen = strlen (newname);
#endif

  nbuf = xmemdup0 (newname, nlen);

  /* Create aliases under the new name as stated; an all-lowercase
     version of the new name; and an all-uppercase version of the new
     name.  */
  if (insert_reg_alias (nbuf, old->number, old->type) != NULL)
    {
      for (p = nbuf; *p; p++)
	*p = TOUPPER (*p);

      if (strncmp (nbuf, newname, nlen))
	{
	  /* If this attempt to create an additional alias fails, do not bother
	     trying to create the all-lower case alias.  We will fail and issue
	     a second, duplicate error message.  This situation arises when the
	     programmer does something like:
	       foo .req r0
	       Foo .req r1
	     The second .req creates the "Foo" alias but then fails to create
	     the artificial FOO alias because it has already been created by the
	     first .req.  */
	  if (insert_reg_alias (nbuf, old->number, old->type) == NULL)
	    {
	      free (nbuf);
	      return TRUE;
	    }
	}

      for (p = nbuf; *p; p++)
	*p = TOLOWER (*p);

      if (strncmp (nbuf, newname, nlen))
	insert_reg_alias (nbuf, old->number, old->type);
    }

  free (nbuf);
  return TRUE;
}

/* Create a Neon typed/indexed register alias using directives, e.g.:
     X .dn d5.s32[1]
     Y .qn 6.s16
     Z .dn d7
     T .dn Z[0]
   These typed registers can be used instead of the types specified after the
   Neon mnemonic, so long as all operands given have types. Types can also be
   specified directly, e.g.:
     vadd d0.s32, d1.s32, d2.s32  */

static bfd_boolean
create_neon_reg_alias (char *newname, char *p)
{
  enum arm_reg_type basetype;
  struct reg_entry *basereg;
  struct reg_entry mybasereg;
  struct neon_type ntype;
  struct neon_typed_alias typeinfo;
  char *namebuf, *nameend ATTRIBUTE_UNUSED;
  int namelen;

  typeinfo.defined = 0;
  typeinfo.eltype.type = NT_invtype;
  typeinfo.eltype.size = -1;
  typeinfo.index = -1;

  nameend = p;

  if (strncmp (p, " .dn ", 5) == 0)
    basetype = REG_TYPE_VFD;
  else if (strncmp (p, " .qn ", 5) == 0)
    basetype = REG_TYPE_NQ;
  else
    return FALSE;

  p += 5;

  if (*p == '\0')
    return FALSE;

  basereg = arm_reg_parse_multi (&p);

  if (basereg && basereg->type != basetype)
    {
      as_bad (_("bad type for register"));
      return FALSE;
    }

  if (basereg == NULL)
    {
      expressionS exp;
      /* Try parsing as an integer.  */
      my_get_expression (&exp, &p, GE_NO_PREFIX);
      if (exp.X_op != O_constant)
	{
	  as_bad (_("expression must be constant"));
	  return FALSE;
	}
      basereg = &mybasereg;
      basereg->number = (basetype == REG_TYPE_NQ) ? exp.X_add_number * 2
						  : exp.X_add_number;
      basereg->neon = 0;
    }

  if (basereg->neon)
    typeinfo = *basereg->neon;

  if (parse_neon_type (&ntype, &p) == SUCCESS)
    {
      /* We got a type.  */
      if (typeinfo.defined & NTA_HASTYPE)
	{
	  as_bad (_("can't redefine the type of a register alias"));
	  return FALSE;
	}

      typeinfo.defined |= NTA_HASTYPE;
      if (ntype.elems != 1)
	{
	  as_bad (_("you must specify a single type only"));
	  return FALSE;
	}
      typeinfo.eltype = ntype.el[0];
    }

  if (skip_past_char (&p, '[') == SUCCESS)
    {
      expressionS exp;
      /* We got a scalar index.  */

      if (typeinfo.defined & NTA_HASINDEX)
	{
	  as_bad (_("can't redefine the index of a scalar alias"));
	  return FALSE;
	}

      my_get_expression (&exp, &p, GE_NO_PREFIX);

      if (exp.X_op != O_constant)
	{
	  as_bad (_("scalar index must be constant"));
	  return FALSE;
	}

      typeinfo.defined |= NTA_HASINDEX;
      typeinfo.index = exp.X_add_number;

      if (skip_past_char (&p, ']') == FAIL)
	{
	  as_bad (_("expecting ]"));
	  return FALSE;
	}
    }

  /* If TC_CASE_SENSITIVE is defined, then newname already points to
     the desired alias name, and p points to its end.  If not, then
     the desired alias name is in the global original_case_string.  */
#ifdef TC_CASE_SENSITIVE
  namelen = nameend - newname;
#else
  newname = original_case_string;
  namelen = strlen (newname);
#endif

  namebuf = xmemdup0 (newname, namelen);

  insert_neon_reg_alias (namebuf, basereg->number, basetype,
			 typeinfo.defined != 0 ? &typeinfo : NULL);

  /* Insert name in all uppercase.  */
  for (p = namebuf; *p; p++)
    *p = TOUPPER (*p);

  if (strncmp (namebuf, newname, namelen))
    insert_neon_reg_alias (namebuf, basereg->number, basetype,
			   typeinfo.defined != 0 ? &typeinfo : NULL);

  /* Insert name in all lowercase.  */
  for (p = namebuf; *p; p++)
    *p = TOLOWER (*p);

  if (strncmp (namebuf, newname, namelen))
    insert_neon_reg_alias (namebuf, basereg->number, basetype,
			   typeinfo.defined != 0 ? &typeinfo : NULL);

  free (namebuf);
  return TRUE;
}

/* Should never be called, as .req goes between the alias and the
   register name, not at the beginning of the line.  */

static void
s_req (int a ATTRIBUTE_UNUSED)
{
  as_bad (_("invalid syntax for .req directive"));
}

static void
s_dn (int a ATTRIBUTE_UNUSED)
{
  as_bad (_("invalid syntax for .dn directive"));
}

static void
s_qn (int a ATTRIBUTE_UNUSED)
{
  as_bad (_("invalid syntax for .qn directive"));
}

/* The .unreq directive deletes an alias which was previously defined
   by .req.  For example:

       my_alias .req r11
       .unreq my_alias	  */

static void
s_unreq (int a ATTRIBUTE_UNUSED)
{
  char * name;
  char saved_char;

  name = input_line_pointer;

  while (*input_line_pointer != 0
	 && *input_line_pointer != ' '
	 && *input_line_pointer != '\n')
    ++input_line_pointer;

  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  if (!*name)
    as_bad (_("invalid syntax for .unreq directive"));
  else
    {
      struct reg_entry *reg = (struct reg_entry *) hash_find (arm_reg_hsh,
							      name);

      if (!reg)
	as_bad (_("unknown register alias '%s'"), name);
      else if (reg->builtin)
	as_warn (_("ignoring attempt to use .unreq on fixed register name: '%s'"),
		 name);
      else
	{
	  char * p;
	  char * nbuf;

	  hash_delete (arm_reg_hsh, name, FALSE);
	  free ((char *) reg->name);
	  if (reg->neon)
	    free (reg->neon);
	  free (reg);

	  /* Also locate the all upper case and all lower case versions.
	     Do not complain if we cannot find one or the other as it
	     was probably deleted above.  */

	  nbuf = strdup (name);
	  for (p = nbuf; *p; p++)
	    *p = TOUPPER (*p);
	  reg = (struct reg_entry *) hash_find (arm_reg_hsh, nbuf);
	  if (reg)
	    {
	      hash_delete (arm_reg_hsh, nbuf, FALSE);
	      free ((char *) reg->name);
	      if (reg->neon)
		free (reg->neon);
	      free (reg);
	    }

	  for (p = nbuf; *p; p++)
	    *p = TOLOWER (*p);
	  reg = (struct reg_entry *) hash_find (arm_reg_hsh, nbuf);
	  if (reg)
	    {
	      hash_delete (arm_reg_hsh, nbuf, FALSE);
	      free ((char *) reg->name);
	      if (reg->neon)
		free (reg->neon);
	      free (reg);
	    }

	  free (nbuf);
	}
    }

  *input_line_pointer = saved_char;
  demand_empty_rest_of_line ();
}

/* Directives: Instruction set selection.  */

/* This code is to handle mapping symbols as defined in the ARM ELF spec.
   (See "Mapping symbols", section 4.5.5, ARM AAELF version 1.0).
   Note that previously, $a and $t has type STT_FUNC (BSF_OBJECT flag),
   and $d has type STT_OBJECT (BSF_OBJECT flag). Now all three are untyped.  */

/* Create a new mapping symbol for the transition to STATE.  */

static void
make_mapping_symbol (enum mstate state, valueT value, fragS *frag)
{
  symbolS * symbolP;
  const char * symname;
  int type;

  switch (state)
    {
    case MAP_DATA:
      symname = "$d";
      type = BSF_NO_FLAGS;
      break;
    case MAP_ARM:
      symname = "$a";
      type = BSF_NO_FLAGS;
      break;
    case MAP_THUMB:
      symname = "$t";
      type = BSF_NO_FLAGS;
      break;
    default:
      abort ();
    }

  symbolP = symbol_new (symname, now_seg, value, frag);
  symbol_get_bfdsym (symbolP)->flags |= type | BSF_LOCAL;

  switch (state)
    {
    case MAP_ARM:
      THUMB_SET_FUNC (symbolP, 0);
      ARM_SET_THUMB (symbolP, 0);
      ARM_SET_INTERWORK (symbolP, support_interwork);
      break;

    case MAP_THUMB:
      THUMB_SET_FUNC (symbolP, 1);
      ARM_SET_THUMB (symbolP, 1);
      ARM_SET_INTERWORK (symbolP, support_interwork);
      break;

    case MAP_DATA:
    default:
      break;
    }

  /* Save the mapping symbols for future reference.  Also check that
     we do not place two mapping symbols at the same offset within a
     frag.  We'll handle overlap between frags in
     check_mapping_symbols.

     If .fill or other data filling directive generates zero sized data,
     the mapping symbol for the following code will have the same value
     as the one generated for the data filling directive.  In this case,
     we replace the old symbol with the new one at the same address.  */
  if (value == 0)
    {
      if (frag->tc_frag_data.first_map != NULL)
	{
	  know (S_GET_VALUE (frag->tc_frag_data.first_map) == 0);
	  symbol_remove (frag->tc_frag_data.first_map, &symbol_rootP, &symbol_lastP);
	}
      frag->tc_frag_data.first_map = symbolP;
    }
  if (frag->tc_frag_data.last_map != NULL)
    {
      know (S_GET_VALUE (frag->tc_frag_data.last_map) <= S_GET_VALUE (symbolP));
      if (S_GET_VALUE (frag->tc_frag_data.last_map) == S_GET_VALUE (symbolP))
	symbol_remove (frag->tc_frag_data.last_map, &symbol_rootP, &symbol_lastP);
    }
  frag->tc_frag_data.last_map = symbolP;
}

/* We must sometimes convert a region marked as code to data during
   code alignment, if an odd number of bytes have to be padded.  The
   code mapping symbol is pushed to an aligned address.  */

static void
insert_data_mapping_symbol (enum mstate state,
			    valueT value, fragS *frag, offsetT bytes)
{
  /* If there was already a mapping symbol, remove it.  */
  if (frag->tc_frag_data.last_map != NULL
      && S_GET_VALUE (frag->tc_frag_data.last_map) == frag->fr_address + value)
    {
      symbolS *symp = frag->tc_frag_data.last_map;

      if (value == 0)
	{
	  know (frag->tc_frag_data.first_map == symp);
	  frag->tc_frag_data.first_map = NULL;
	}
      frag->tc_frag_data.last_map = NULL;
      symbol_remove (symp, &symbol_rootP, &symbol_lastP);
    }

  make_mapping_symbol (MAP_DATA, value, frag);
  make_mapping_symbol (state, value + bytes, frag);
}

static void mapping_state_2 (enum mstate state, int max_chars);

/* Set the mapping state to STATE.  Only call this when about to
   emit some STATE bytes to the file.  */

#define TRANSITION(from, to) (mapstate == (from) && state == (to))
void
mapping_state (enum mstate state)
{
  enum mstate mapstate = seg_info (now_seg)->tc_segment_info_data.mapstate;

  if (mapstate == state)
    /* The mapping symbol has already been emitted.
       There is nothing else to do.  */
    return;

  if (state == MAP_ARM || state == MAP_THUMB)
    /*  PR gas/12931
	All ARM instructions require 4-byte alignment.
	(Almost) all Thumb instructions require 2-byte alignment.

	When emitting instructions into any section, mark the section
	appropriately.

	Some Thumb instructions are alignment-sensitive modulo 4 bytes,
	but themselves require 2-byte alignment; this applies to some
	PC- relative forms.  However, these cases will involve implicit
	literal pool generation or an explicit .align >=2, both of
	which will cause the section to me marked with sufficient
	alignment.  Thus, we don't handle those cases here.  */
    record_alignment (now_seg, state == MAP_ARM ? 2 : 1);

  if (TRANSITION (MAP_UNDEFINED, MAP_DATA))
    /* This case will be evaluated later.  */
    return;

  mapping_state_2 (state, 0);
}

/* Same as mapping_state, but MAX_CHARS bytes have already been
   allocated.  Put the mapping symbol that far back.  */

static void
mapping_state_2 (enum mstate state, int max_chars)
{
  enum mstate mapstate = seg_info (now_seg)->tc_segment_info_data.mapstate;

  if (!SEG_NORMAL (now_seg))
    return;

  if (mapstate == state)
    /* The mapping symbol has already been emitted.
       There is nothing else to do.  */
    return;

  if (TRANSITION (MAP_UNDEFINED, MAP_ARM)
	  || TRANSITION (MAP_UNDEFINED, MAP_THUMB))
    {
      struct frag * const frag_first = seg_info (now_seg)->frchainP->frch_root;
      const int add_symbol = (frag_now != frag_first) || (frag_now_fix () > 0);

      if (add_symbol)
	make_mapping_symbol (MAP_DATA, (valueT) 0, frag_first);
    }

  seg_info (now_seg)->tc_segment_info_data.mapstate = state;
  make_mapping_symbol (state, (valueT) frag_now_fix () - max_chars, frag_now);
}
#undef TRANSITION

static void
opcode_select (int width)
{
  switch (width)
    {
    case 16:
      if (! thumb_mode)
	{
	  if (!ARM_CPU_HAS_FEATURE (cpu_variant, arm_ext_v4t))
	    as_bad (_("selected processor does not support THUMB opcodes"));

	  thumb_mode = 1;
	  /* No need to force the alignment, since we will have been
	     coming from ARM mode, which is word-aligned.  */
	  record_alignment (now_seg, 1);
	}
      break;

    case 32:
      if (thumb_mode)
	{
	  if (!ARM_CPU_HAS_FEATURE (cpu_variant, arm_ext_v1))
	    as_bad (_("selected processor does not support ARM opcodes"));

	  thumb_mode = 0;

	  if (!need_pass_2)
	    frag_align (2, 0, 0);

	  record_alignment (now_seg, 1);
	}
      break;

    default:
      as_bad (_("invalid instruction size selected (%d)"), width);
    }
}

static void
s_arm (int ignore ATTRIBUTE_UNUSED)
{
  opcode_select (32);
  demand_empty_rest_of_line ();
}

static void
s_thumb (int ignore ATTRIBUTE_UNUSED)
{
  opcode_select (16);
  demand_empty_rest_of_line ();
}

static void
s_code (int unused ATTRIBUTE_UNUSED)
{
  int temp;

  temp = get_absolute_expression ();
  switch (temp)
    {
    case 16:
    case 32:
      opcode_select (temp);
      break;

    default:
      as_bad (_("invalid operand to .code directive (%d) (expecting 16 or 32)"), temp);
    }
}

static void
s_force_thumb (int ignore ATTRIBUTE_UNUSED)
{
  /* If we are not already in thumb mode go into it, EVEN if
     the target processor does not support thumb instructions.
     This is used by gcc/config/arm/lib1funcs.asm for example
     to compile interworking support functions even if the
     target processor should not support interworking.	*/
  if (! thumb_mode)
    {
      thumb_mode = 2;
      record_alignment (now_seg, 1);
    }

  demand_empty_rest_of_line ();
}

static void
s_thumb_func (int ignore ATTRIBUTE_UNUSED)
{
  s_thumb (0);

  /* The following label is the name/address of the start of a Thumb function.
     We need to know this for the interworking support.	 */
  label_is_thumb_function_name = TRUE;
}

/* Perform a .set directive, but also mark the alias as
   being a thumb function.  */

static void
s_thumb_set (int equiv)
{
  /* XXX the following is a duplicate of the code for s_set() in read.c
     We cannot just call that code as we need to get at the symbol that
     is created.  */
  char *    name;
  char	    delim;
  char *    end_name;
  symbolS * symbolP;

  /* Especial apologies for the random logic:
     This just grew, and could be parsed much more simply!
     Dean - in haste.  */
  delim	    = get_symbol_name (& name);
  end_name  = input_line_pointer;
  (void) restore_line_pointer (delim);

  if (*input_line_pointer != ',')
    {
      *end_name = 0;
      as_bad (_("expected comma after name \"%s\""), name);
      *end_name = delim;
      ignore_rest_of_line ();
      return;
    }

  input_line_pointer++;
  *end_name = 0;

  if (name[0] == '.' && name[1] == '\0')
    {
      /* XXX - this should not happen to .thumb_set.  */
      abort ();
    }

  if ((symbolP = symbol_find (name)) == NULL
      && (symbolP = md_undefined_symbol (name)) == NULL)
    {
#ifndef NO_LISTING
      /* When doing symbol listings, play games with dummy fragments living
	 outside the normal fragment chain to record the file and line info
	 for this symbol.  */
      if (listing & LISTING_SYMBOLS)
	{
	  extern struct list_info_struct * listing_tail;
	  fragS * dummy_frag = (fragS * ) malloc (sizeof (fragS));

	  memset (dummy_frag, 0, sizeof (fragS));
	  dummy_frag->fr_type = rs_fill;
	  dummy_frag->line = listing_tail;
	  symbolP = symbol_new (name, undefined_section, 0, dummy_frag);
	  dummy_frag->fr_symbol = symbolP;
	}
      else
#endif
	symbolP = symbol_new (name, undefined_section, 0, &zero_address_frag);
    }				/* Make a new symbol.  */

  symbol_table_insert (symbolP);

  * end_name = delim;

  if (equiv
      && S_IS_DEFINED (symbolP)
      && S_GET_SEGMENT (symbolP) != reg_section)
    as_bad (_("symbol `%s' already defined"), S_GET_NAME (symbolP));

  pseudo_set (symbolP);

  demand_empty_rest_of_line ();

  /* XXX Now we come to the Thumb specific bit of code.	 */

  THUMB_SET_FUNC (symbolP, 1);
  ARM_SET_THUMB (symbolP, 1);
  ARM_SET_INTERWORK (symbolP, support_interwork);
}

/* Directives: Mode selection.  */

/* .syntax [unified|divided] - choose the new unified syntax
   (same for Arm and Thumb encoding, modulo slight differences in what
   can be represented) or the old divergent syntax for each mode.  */
static void
s_syntax (int unused ATTRIBUTE_UNUSED)
{
  char *name, delim;

  delim = get_symbol_name (& name);

  if (!strcasecmp (name, "unified"))
    unified_syntax = TRUE;
  else if (!strcasecmp (name, "divided"))
    unified_syntax = FALSE;
  else
    {
      as_bad (_("unrecognized syntax mode \"%s\""), name);
      return;
    }
  (void) restore_line_pointer (delim);
  demand_empty_rest_of_line ();
}

/* Directives: sectioning and alignment.  */

static void
s_bss (int ignore ATTRIBUTE_UNUSED)
{
  /* We don't support putting frags in the BSS segment, we fake it by
     marking in_bss, then looking at s_skip for clues.	*/
  subseg_set (bss_section, 0);
  demand_empty_rest_of_line ();

#ifdef md_elf_section_change_hook
  md_elf_section_change_hook ();
#endif
}

static void
s_even (int ignore ATTRIBUTE_UNUSED)
{
  /* Never make frag if expect extra pass.  */
  if (!need_pass_2)
    frag_align (1, 0, 0);

  record_alignment (now_seg, 1);

  demand_empty_rest_of_line ();
}

/* Directives: CodeComposer Studio.  */

/*  .ref  (for CodeComposer Studio syntax only).  */
static void
s_ccs_ref (int unused ATTRIBUTE_UNUSED)
{
  if (codecomposer_syntax)
    ignore_rest_of_line ();
  else
    as_bad (_(".ref pseudo-op only available with -mccs flag."));
}

/*  If name is not NULL, then it is used for marking the beginning of a
    function, whereas if it is NULL then it means the function end.  */
static void
asmfunc_debug (const char * name)
{
  static const char * last_name = NULL;

  if (name != NULL)
    {
      gas_assert (last_name == NULL);
      last_name = name;
    }
  else
    {
      gas_assert (last_name != NULL);

      last_name = NULL;
    }
}

static void
s_ccs_asmfunc (int unused ATTRIBUTE_UNUSED)
{
  if (codecomposer_syntax)
    {
      switch (asmfunc_state)
	{
	case OUTSIDE_ASMFUNC:
	  asmfunc_state = WAITING_ASMFUNC_NAME;
	  break;

	case WAITING_ASMFUNC_NAME:
	  as_bad (_(".asmfunc repeated."));
	  break;

	case WAITING_ENDASMFUNC:
	  as_bad (_(".asmfunc without function."));
	  break;
	}
      demand_empty_rest_of_line ();
    }
  else
    as_bad (_(".asmfunc pseudo-op only available with -mccs flag."));
}

static void
s_ccs_endasmfunc (int unused ATTRIBUTE_UNUSED)
{
  if (codecomposer_syntax)
    {
      switch (asmfunc_state)
	{
	case OUTSIDE_ASMFUNC:
	  as_bad (_(".endasmfunc without a .asmfunc."));
	  break;

	case WAITING_ASMFUNC_NAME:
	  as_bad (_(".endasmfunc without function."));
	  break;

	case WAITING_ENDASMFUNC:
	  asmfunc_state = OUTSIDE_ASMFUNC;
	  asmfunc_debug (NULL);
	  break;
	}
      demand_empty_rest_of_line ();
    }
  else
    as_bad (_(".endasmfunc pseudo-op only available with -mccs flag."));
}

static void
s_ccs_def (int name)
{
  if (codecomposer_syntax)
    s_globl (name);
  else
    as_bad (_(".def pseudo-op only available with -mccs flag."));
}

/* Directives: Literal pools.  */

static literal_pool *
find_literal_pool (void)
{
  literal_pool * pool;

  for (pool = list_of_pools; pool != NULL; pool = pool->next)
    {
      if (pool->section == now_seg
	  && pool->sub_section == now_subseg)
	break;
    }

  return pool;
}

static literal_pool *
find_or_make_literal_pool (void)
{
  /* Next literal pool ID number.  */
  static unsigned int latest_pool_num = 1;
  literal_pool *      pool;

  pool = find_literal_pool ();

  if (pool == NULL)
    {
      /* Create a new pool.  */
      pool = XNEW (literal_pool);
      if (! pool)
	return NULL;

      pool->next_free_entry = 0;
      pool->section	    = now_seg;
      pool->sub_section	    = now_subseg;
      pool->next	    = list_of_pools;
      pool->symbol	    = NULL;
      pool->alignment	    = 2;

      /* Add it to the list.  */
      list_of_pools = pool;
    }

  /* New pools, and emptied pools, will have a NULL symbol.  */
  if (pool->symbol == NULL)
    {
      pool->symbol = symbol_create (FAKE_LABEL_NAME, undefined_section,
				    (valueT) 0, &zero_address_frag);
      pool->id = latest_pool_num ++;
    }

  /* Done.  */
  return pool;
}

/* Add the literal in the global 'inst'
   structure to the relevant literal pool.  */

static int
add_to_lit_pool (unsigned int nbytes)
{
#define PADDING_SLOT 0x1
#define LIT_ENTRY_SIZE_MASK 0xFF
  literal_pool * pool;
  unsigned int entry, pool_size = 0;
  bfd_boolean padding_slot_p = FALSE;
  unsigned imm1 = 0;
  unsigned imm2 = 0;

  if (nbytes == 8)
    {
      imm1 = inst.operands[1].imm;
      imm2 = (inst.operands[1].regisimm ? inst.operands[1].reg
	       : inst.reloc.exp.X_unsigned ? 0
	       : ((bfd_int64_t) inst.operands[1].imm) >> 32);
      if (target_big_endian)
	{
	  imm1 = imm2;
	  imm2 = inst.operands[1].imm;
	}
    }

  pool = find_or_make_literal_pool ();

  /* Check if this literal value is already in the pool.  */
  for (entry = 0; entry < pool->next_free_entry; entry ++)
    {
      if (nbytes == 4)
	{
	  if ((pool->literals[entry].X_op == inst.reloc.exp.X_op)
	      && (inst.reloc.exp.X_op == O_constant)
	      && (pool->literals[entry].X_add_number
		  == inst.reloc.exp.X_add_number)
	      && (pool->literals[entry].X_md == nbytes)
	      && (pool->literals[entry].X_unsigned
		  == inst.reloc.exp.X_unsigned))
	    break;

	  if ((pool->literals[entry].X_op == inst.reloc.exp.X_op)
	      && (inst.reloc.exp.X_op == O_symbol)
	      && (pool->literals[entry].X_add_number
		  == inst.reloc.exp.X_add_number)
	      && (pool->literals[entry].X_add_symbol
		  == inst.reloc.exp.X_add_symbol)
	      && (pool->literals[entry].X_op_symbol
		  == inst.reloc.exp.X_op_symbol)
	      && (pool->literals[entry].X_md == nbytes))
	    break;
	}
      else if ((nbytes == 8)
	       && !(pool_size & 0x7)
	       && ((entry + 1) != pool->next_free_entry)
	       && (pool->literals[entry].X_op == O_constant)
	       && (pool->literals[entry].X_add_number == (offsetT) imm1)
	       && (pool->literals[entry].X_unsigned
		   == inst.reloc.exp.X_unsigned)
	       && (pool->literals[entry + 1].X_op == O_constant)
	       && (pool->literals[entry + 1].X_add_number == (offsetT) imm2)
	       && (pool->literals[entry + 1].X_unsigned
		   == inst.reloc.exp.X_unsigned))
	break;

      padding_slot_p = ((pool->literals[entry].X_md >> 8) == PADDING_SLOT);
      if (padding_slot_p && (nbytes == 4))
	break;

      pool_size += 4;
    }

  /* Do we need to create a new entry?	*/
  if (entry == pool->next_free_entry)
    {
      if (entry >= MAX_LITERAL_POOL_SIZE)
	{
	  inst.error = _("literal pool overflow");
	  return FAIL;
	}

      if (nbytes == 8)
	{
	  /* For 8-byte entries, we align to an 8-byte boundary,
	     and split it into two 4-byte entries, because on 32-bit
	     host, 8-byte constants are treated as big num, thus
	     saved in "generic_bignum" which will be overwritten
	     by later assignments.

	     We also need to make sure there is enough space for
	     the split.

	     We also check to make sure the literal operand is a
	     constant number.  */
	  if (!(inst.reloc.exp.X_op == O_constant
	        || inst.reloc.exp.X_op == O_big))
	    {
	      inst.error = _("invalid type for literal pool");
	      return FAIL;
	    }
	  else if (pool_size & 0x7)
	    {
	      if ((entry + 2) >= MAX_LITERAL_POOL_SIZE)
		{
		  inst.error = _("literal pool overflow");
		  return FAIL;
		}

	      pool->literals[entry] = inst.reloc.exp;
	      pool->literals[entry].X_op = O_constant;
	      pool->literals[entry].X_add_number = 0;
	      pool->literals[entry++].X_md = (PADDING_SLOT << 8) | 4;
	      pool->next_free_entry += 1;
	      pool_size += 4;
	    }
	  else if ((entry + 1) >= MAX_LITERAL_POOL_SIZE)
	    {
	      inst.error = _("literal pool overflow");
	      return FAIL;
	    }

	  pool->literals[entry] = inst.reloc.exp;
	  pool->literals[entry].X_op = O_constant;
	  pool->literals[entry].X_add_number = imm1;
	  pool->literals[entry].X_unsigned = inst.reloc.exp.X_unsigned;
	  pool->literals[entry++].X_md = 4;
	  pool->literals[entry] = inst.reloc.exp;
	  pool->literals[entry].X_op = O_constant;
	  pool->literals[entry].X_add_number = imm2;
	  pool->literals[entry].X_unsigned = inst.reloc.exp.X_unsigned;
	  pool->literals[entry].X_md = 4;
	  pool->alignment = 3;
	  pool->next_free_entry += 1;
	}
      else
	{
	  pool->literals[entry] = inst.reloc.exp;
	  pool->literals[entry].X_md = 4;
	}

      /* PR ld/12974: Record the location of the first source line to reference
	 this entry in the literal pool.  If it turns out during linking that the
	 symbol does not exist we will be able to give an accurate line number for
	 the (first use of the) missing reference.  */
      if (debug_type == DEBUG_DWARF2)
	dwarf2_where (pool->locs + entry);
      pool->next_free_entry += 1;
    }
  else if (padding_slot_p)
    {
      pool->literals[entry] = inst.reloc.exp;
      pool->literals[entry].X_md = nbytes;
    }

  inst.reloc.exp.X_op	      = O_symbol;
  inst.reloc.exp.X_add_number = pool_size;
  inst.reloc.exp.X_add_symbol = pool->symbol;

  return SUCCESS;
}

bfd_boolean
tc_start_label_without_colon (void)
{
  bfd_boolean ret = TRUE;

  if (codecomposer_syntax && asmfunc_state == WAITING_ASMFUNC_NAME)
    {
      const char *label = input_line_pointer;

      while (!is_end_of_line[(int) label[-1]])
	--label;

      if (*label == '.')
	{
	  as_bad (_("Invalid label '%s'"), label);
	  ret = FALSE;
	}

      asmfunc_debug (label);

      asmfunc_state = WAITING_ENDASMFUNC;
    }

  return ret;
}

/* Can't use symbol_new here, so have to create a symbol and then at
   a later date assign it a value. That's what these functions do.  */

static void
symbol_locate (symbolS *    symbolP,
	       const char * name,	/* It is copied, the caller can modify.	 */
	       segT	    segment,	/* Segment identifier (SEG_<something>).  */
	       valueT	    valu,	/* Symbol value.  */
	       fragS *	    frag)	/* Associated fragment.	 */
{
  size_t name_length;
  char * preserved_copy_of_name;

  name_length = strlen (name) + 1;   /* +1 for \0.  */
  obstack_grow (&notes, name, name_length);
  preserved_copy_of_name = (char *) obstack_finish (&notes);

#ifdef tc_canonicalize_symbol_name
  preserved_copy_of_name =
    tc_canonicalize_symbol_name (preserved_copy_of_name);
#endif

  S_SET_NAME (symbolP, preserved_copy_of_name);

  S_SET_SEGMENT (symbolP, segment);
  S_SET_VALUE (symbolP, valu);
  symbol_clear_list_pointers (symbolP);

  symbol_set_frag (symbolP, frag);

  /* Link to end of symbol chain.  */
  {
    extern int symbol_table_frozen;

    if (symbol_table_frozen)
      abort ();
  }

  symbol_append (symbolP, symbol_lastP, & symbol_rootP, & symbol_lastP);

  obj_symbol_new_hook (symbolP);

#ifdef tc_symbol_new_hook
  tc_symbol_new_hook (symbolP);
#endif
}

static void
s_ltorg (int ignored ATTRIBUTE_UNUSED)
{
  unsigned int entry;
  literal_pool * pool;
  char sym_name[20];

  pool = find_literal_pool ();
  if (pool == NULL
      || pool->symbol == NULL
      || pool->next_free_entry == 0)
    return;

  /* Align pool as you have word accesses.
     Only make a frag if we have to.  */
  if (!need_pass_2)
    frag_align (pool->alignment, 0, 0);

  record_alignment (now_seg, 2);

  seg_info (now_seg)->tc_segment_info_data.mapstate = MAP_DATA;
  make_mapping_symbol (MAP_DATA, (valueT) frag_now_fix (), frag_now);
  sprintf (sym_name, "$$lit_\002%x", pool->id);

  symbol_locate (pool->symbol, sym_name, now_seg,
		 (valueT) frag_now_fix (), frag_now);
  symbol_table_insert (pool->symbol);

  ARM_SET_THUMB (pool->symbol, thumb_mode);

  ARM_SET_INTERWORK (pool->symbol, support_interwork);

  for (entry = 0; entry < pool->next_free_entry; entry ++)
    {
      if (debug_type == DEBUG_DWARF2)
	dwarf2_gen_line_info (frag_now_fix (), pool->locs + entry);
      /* First output the expression in the instruction to the pool.  */
      emit_expr (&(pool->literals[entry]),
		 pool->literals[entry].X_md & LIT_ENTRY_SIZE_MASK);
    }

  /* Mark the pool as empty.  */
  pool->next_free_entry = 0;
  pool->symbol = NULL;
}

/* Forward declarations for functions below, in the MD interface
   section.  */
static void fix_new_arm (fragS *, int, short, expressionS *, int, int);
static valueT create_unwind_entry (int);
static void start_unwind_section (const segT, int);
static void add_unwind_opcode (valueT, int);
static void flush_pending_unwind (void);

/* Directives: Data.  */

static void
s_arm_elf_cons (int nbytes)
{
  expressionS exp;

#ifdef md_flush_pending_output
  md_flush_pending_output ();
#endif

  if (is_it_end_of_statement ())
    {
      demand_empty_rest_of_line ();
      return;
    }

#ifdef md_cons_align
  md_cons_align (nbytes);
#endif

  mapping_state (MAP_DATA);
  do
    {
      int reloc;
      char *base = input_line_pointer;

      expression (& exp);

      if (exp.X_op != O_symbol)
	emit_expr (&exp, (unsigned int) nbytes);
      else
	{
	  char *before_reloc = input_line_pointer;
	  reloc = parse_reloc (&input_line_pointer);
	  if (reloc == -1)
	    {
	      as_bad (_("unrecognized relocation suffix"));
	      ignore_rest_of_line ();
	      return;
	    }
	  else if (reloc == BFD_RELOC_UNUSED)
	    emit_expr (&exp, (unsigned int) nbytes);
	  else
	    {
	      reloc_howto_type *howto = (reloc_howto_type *)
		  bfd_reloc_type_lookup (stdoutput,
					 (bfd_reloc_code_real_type) reloc);
	      int size = bfd_get_reloc_size (howto);

	      if (reloc == BFD_RELOC_ARM_PLT32)
		{
		  as_bad (_("(plt) is only valid on branch targets"));
		  reloc = BFD_RELOC_UNUSED;
		  size = 0;
		}

	      if (size > nbytes)
		as_bad (ngettext ("%s relocations do not fit in %d byte",
				  "%s relocations do not fit in %d bytes",
				  nbytes),
			howto->name, nbytes);
	      else
		{
		  /* We've parsed an expression stopping at O_symbol.
		     But there may be more expression left now that we
		     have parsed the relocation marker.  Parse it again.
		     XXX Surely there is a cleaner way to do this.  */
		  char *p = input_line_pointer;
		  int offset;
		  char *save_buf = XNEWVEC (char, input_line_pointer - base);

		  memcpy (save_buf, base, input_line_pointer - base);
		  memmove (base + (input_line_pointer - before_reloc),
			   base, before_reloc - base);

		  input_line_pointer = base + (input_line_pointer-before_reloc);
		  expression (&exp);
		  memcpy (base, save_buf, p - base);

		  offset = nbytes - size;
		  p = frag_more (nbytes);
		  memset (p, 0, nbytes);
		  fix_new_exp (frag_now, p - frag_now->fr_literal + offset,
			       size, &exp, 0, (enum bfd_reloc_code_real) reloc);
		  free (save_buf);
		}
	    }
	}
    }
  while (*input_line_pointer++ == ',');

  /* Put terminator back into stream.  */
  input_line_pointer --;
  demand_empty_rest_of_line ();
}

/* Emit an expression containing a 32-bit thumb instruction.
   Implementation based on put_thumb32_insn.  */

static void
emit_thumb32_expr (expressionS * exp)
{
  expressionS exp_high = *exp;

  exp_high.X_add_number = (unsigned long)exp_high.X_add_number >> 16;
  emit_expr (& exp_high, (unsigned int) THUMB_SIZE);
  exp->X_add_number &= 0xffff;
  emit_expr (exp, (unsigned int) THUMB_SIZE);
}

/*  Guess the instruction size based on the opcode.  */

static int
thumb_insn_size (int opcode)
{
  if ((unsigned int) opcode < 0xe800u)
    return 2;
  else if ((unsigned int) opcode >= 0xe8000000u)
    return 4;
  else
    return 0;
}

static bfd_boolean
emit_insn (expressionS *exp, int nbytes)
{
  int size = 0;

  if (exp->X_op == O_constant)
    {
      size = nbytes;

      if (size == 0)
	size = thumb_insn_size (exp->X_add_number);

      if (size != 0)
	{
	  if (size == 2 && (unsigned int)exp->X_add_number > 0xffffu)
	    {
	      as_bad (_(".inst.n operand too big. "\
			"Use .inst.w instead"));
	      size = 0;
	    }
	  else
	    {
	      if (now_it.state == AUTOMATIC_IT_BLOCK)
		set_it_insn_type_nonvoid (OUTSIDE_IT_INSN, 0);
	      else
		set_it_insn_type_nonvoid (NEUTRAL_IT_INSN, 0);

	      if (thumb_mode && (size > THUMB_SIZE) && !target_big_endian)
		emit_thumb32_expr (exp);
	      else
		emit_expr (exp, (unsigned int) size);

	      it_fsm_post_encode ();
	    }
	}
      else
	as_bad (_("cannot determine Thumb instruction size. "	\
		  "Use .inst.n/.inst.w instead"));
    }
  else
    as_bad (_("constant expression required"));

  return (size != 0);
}

/* Like s_arm_elf_cons but do not use md_cons_align and
   set the mapping state to MAP_ARM/MAP_THUMB.  */

static void
s_arm_elf_inst (int nbytes)
{
  if (is_it_end_of_statement ())
    {
      demand_empty_rest_of_line ();
      return;
    }

  /* Calling mapping_state () here will not change ARM/THUMB,
     but will ensure not to be in DATA state.  */

  if (thumb_mode)
    mapping_state (MAP_THUMB);
  else
    {
      if (nbytes != 0)
	{
	  as_bad (_("width suffixes are invalid in ARM mode"));
	  ignore_rest_of_line ();
	  return;
	}

      nbytes = 4;

      mapping_state (MAP_ARM);
    }

  do
    {
      expressionS exp;

      expression (& exp);

      if (! emit_insn (& exp, nbytes))
	{
	  ignore_rest_of_line ();
	  return;
	}
    }
  while (*input_line_pointer++ == ',');

  /* Put terminator back into stream.  */
  input_line_pointer --;
  demand_empty_rest_of_line ();
}

/* Parse a .rel31 directive.  */

static void
s_arm_rel31 (int ignored ATTRIBUTE_UNUSED)
{
  expressionS exp;
  char *p;
  valueT highbit;

  highbit = 0;
  if (*input_line_pointer == '1')
    highbit = 0x80000000;
  else if (*input_line_pointer != '0')
    as_bad (_("expected 0 or 1"));

  input_line_pointer++;
  if (*input_line_pointer != ',')
    as_bad (_("missing comma"));
  input_line_pointer++;

#ifdef md_flush_pending_output
  md_flush_pending_output ();
#endif

#ifdef md_cons_align
  md_cons_align (4);
#endif

  mapping_state (MAP_DATA);

  expression (&exp);

  p = frag_more (4);
  md_number_to_chars (p, highbit, 4);
  fix_new_arm (frag_now, p - frag_now->fr_literal, 4, &exp, 1,
	       BFD_RELOC_ARM_PREL31);

  demand_empty_rest_of_line ();
}

/* Directives: AEABI stack-unwind tables.  */

/* Parse an unwind_fnstart directive.  Simply records the current location.  */

static void
s_arm_unwind_fnstart (int ignored ATTRIBUTE_UNUSED)
{
  demand_empty_rest_of_line ();
  if (unwind.proc_start)
    {
      as_bad (_("duplicate .fnstart directive"));
      return;
    }

  /* Mark the start of the function.  */
  unwind.proc_start = expr_build_dot ();

  /* Reset the rest of the unwind info.	 */
  unwind.opcode_count = 0;
  unwind.table_entry = NULL;
  unwind.personality_routine = NULL;
  unwind.personality_index = -1;
  unwind.frame_size = 0;
  unwind.fp_offset = 0;
  unwind.fp_reg = REG_SP;
  unwind.fp_used = 0;
  unwind.sp_restored = 0;
}


/* Parse a handlerdata directive.  Creates the exception handling table entry
   for the function.  */

static void
s_arm_unwind_handlerdata (int ignored ATTRIBUTE_UNUSED)
{
  demand_empty_rest_of_line ();
  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  if (unwind.table_entry)
    as_bad (_("duplicate .handlerdata directive"));

  create_unwind_entry (1);
}

/* Parse an unwind_fnend directive.  Generates the index table entry.  */

static void
s_arm_unwind_fnend (int ignored ATTRIBUTE_UNUSED)
{
  long where;
  char *ptr;
  valueT val;
  unsigned int marked_pr_dependency;

  demand_empty_rest_of_line ();

  if (!unwind.proc_start)
    {
      as_bad (_(".fnend directive without .fnstart"));
      return;
    }

  /* Add eh table entry.  */
  if (unwind.table_entry == NULL)
    val = create_unwind_entry (0);
  else
    val = 0;

  /* Add index table entry.  This is two words.	 */
  start_unwind_section (unwind.saved_seg, 1);
  frag_align (2, 0, 0);
  record_alignment (now_seg, 2);

  ptr = frag_more (8);
  memset (ptr, 0, 8);
  where = frag_now_fix () - 8;

  /* Self relative offset of the function start.  */
  fix_new (frag_now, where, 4, unwind.proc_start, 0, 1,
	   BFD_RELOC_ARM_PREL31);

  /* Indicate dependency on EHABI-defined personality routines to the
     linker, if it hasn't been done already.  */
  marked_pr_dependency
    = seg_info (now_seg)->tc_segment_info_data.marked_pr_dependency;
  if (unwind.personality_index >= 0 && unwind.personality_index < 3
      && !(marked_pr_dependency & (1 << unwind.personality_index)))
    {
      static const char *const name[] =
	{
	  "__aeabi_unwind_cpp_pr0",
	  "__aeabi_unwind_cpp_pr1",
	  "__aeabi_unwind_cpp_pr2"
	};
      symbolS *pr = symbol_find_or_make (name[unwind.personality_index]);
      fix_new (frag_now, where, 0, pr, 0, 1, BFD_RELOC_NONE);
      seg_info (now_seg)->tc_segment_info_data.marked_pr_dependency
	|= 1 << unwind.personality_index;
    }

  if (val)
    /* Inline exception table entry.  */
    md_number_to_chars (ptr + 4, val, 4);
  else
    /* Self relative offset of the table entry.	 */
    fix_new (frag_now, where + 4, 4, unwind.table_entry, 0, 1,
	     BFD_RELOC_ARM_PREL31);

  /* Restore the original section.  */
  subseg_set (unwind.saved_seg, unwind.saved_subseg);

  unwind.proc_start = NULL;
}


/* Parse an unwind_cantunwind directive.  */

static void
s_arm_unwind_cantunwind (int ignored ATTRIBUTE_UNUSED)
{
  demand_empty_rest_of_line ();
  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  if (unwind.personality_routine || unwind.personality_index != -1)
    as_bad (_("personality routine specified for cantunwind frame"));

  unwind.personality_index = -2;
}


/* Parse a personalityindex directive.	*/

static void
s_arm_unwind_personalityindex (int ignored ATTRIBUTE_UNUSED)
{
  expressionS exp;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  if (unwind.personality_routine || unwind.personality_index != -1)
    as_bad (_("duplicate .personalityindex directive"));

  expression (&exp);

  if (exp.X_op != O_constant
      || exp.X_add_number < 0 || exp.X_add_number > 15)
    {
      as_bad (_("bad personality routine number"));
      ignore_rest_of_line ();
      return;
    }

  unwind.personality_index = exp.X_add_number;

  demand_empty_rest_of_line ();
}


/* Parse a personality directive.  */

static void
s_arm_unwind_personality (int ignored ATTRIBUTE_UNUSED)
{
  char *name, *p, c;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  if (unwind.personality_routine || unwind.personality_index != -1)
    as_bad (_("duplicate .personality directive"));

  c = get_symbol_name (& name);
  p = input_line_pointer;
  if (c == '"')
    ++ input_line_pointer;
  unwind.personality_routine = symbol_find_or_make (name);
  *p = c;
  demand_empty_rest_of_line ();
}


/* Parse a directive saving core registers.  */

static void
s_arm_unwind_save_core (void)
{
  valueT op;
  long range;
  int n;

  range = parse_reg_list (&input_line_pointer);
  if (range == FAIL)
    {
      as_bad (_("expected register list"));
      ignore_rest_of_line ();
      return;
    }

  demand_empty_rest_of_line ();

  /* Turn .unwind_movsp ip followed by .unwind_save {..., ip, ...}
     into .unwind_save {..., sp...}.  We aren't bothered about the value of
     ip because it is clobbered by calls.  */
  if (unwind.sp_restored && unwind.fp_reg == 12
      && (range & 0x3000) == 0x1000)
    {
      unwind.opcode_count--;
      unwind.sp_restored = 0;
      range = (range | 0x2000) & ~0x1000;
      unwind.pending_offset = 0;
    }

  /* Pop r4-r15.  */
  if (range & 0xfff0)
    {
      /* See if we can use the short opcodes.  These pop a block of up to 8
	 registers starting with r4, plus maybe r14.  */
      for (n = 0; n < 8; n++)
	{
	  /* Break at the first non-saved register.	 */
	  if ((range & (1 << (n + 4))) == 0)
	    break;
	}
      /* See if there are any other bits set.  */
      if (n == 0 || (range & (0xfff0 << n) & 0xbff0) != 0)
	{
	  /* Use the long form.  */
	  op = 0x8000 | ((range >> 4) & 0xfff);
	  add_unwind_opcode (op, 2);
	}
      else
	{
	  /* Use the short form.  */
	  if (range & 0x4000)
	    op = 0xa8; /* Pop r14.	*/
	  else
	    op = 0xa0; /* Do not pop r14.  */
	  op |= (n - 1);
	  add_unwind_opcode (op, 1);
	}
    }

  /* Pop r0-r3.	 */
  if (range & 0xf)
    {
      op = 0xb100 | (range & 0xf);
      add_unwind_opcode (op, 2);
    }

  /* Record the number of bytes pushed.	 */
  for (n = 0; n < 16; n++)
    {
      if (range & (1 << n))
	unwind.frame_size += 4;
    }
}


/* Parse a directive saving FPA registers.  */

static void
s_arm_unwind_save_fpa (int reg)
{
  expressionS exp;
  int num_regs;
  valueT op;

  /* Get Number of registers to transfer.  */
  if (skip_past_comma (&input_line_pointer) != FAIL)
    expression (&exp);
  else
    exp.X_op = O_illegal;

  if (exp.X_op != O_constant)
    {
      as_bad (_("expected , <constant>"));
      ignore_rest_of_line ();
      return;
    }

  num_regs = exp.X_add_number;

  if (num_regs < 1 || num_regs > 4)
    {
      as_bad (_("number of registers must be in the range [1:4]"));
      ignore_rest_of_line ();
      return;
    }

  demand_empty_rest_of_line ();

  if (reg == 4)
    {
      /* Short form.  */
      op = 0xb4 | (num_regs - 1);
      add_unwind_opcode (op, 1);
    }
  else
    {
      /* Long form.  */
      op = 0xc800 | (reg << 4) | (num_regs - 1);
      add_unwind_opcode (op, 2);
    }
  unwind.frame_size += num_regs * 12;
}


/* Parse a directive saving VFP registers for ARMv6 and above.  */

static void
s_arm_unwind_save_vfp_armv6 (void)
{
  int count;
  unsigned int start;
  valueT op;
  int num_vfpv3_regs = 0;
  int num_regs_below_16;

  count = parse_vfp_reg_list (&input_line_pointer, &start, REGLIST_VFP_D);
  if (count == FAIL)
    {
      as_bad (_("expected register list"));
      ignore_rest_of_line ();
      return;
    }

  demand_empty_rest_of_line ();

  /* We always generate FSTMD/FLDMD-style unwinding opcodes (rather
     than FSTMX/FLDMX-style ones).  */

  /* Generate opcode for (VFPv3) registers numbered in the range 16 .. 31.  */
  if (start >= 16)
    num_vfpv3_regs = count;
  else if (start + count > 16)
    num_vfpv3_regs = start + count - 16;

  if (num_vfpv3_regs > 0)
    {
      int start_offset = start > 16 ? start - 16 : 0;
      op = 0xc800 | (start_offset << 4) | (num_vfpv3_regs - 1);
      add_unwind_opcode (op, 2);
    }

  /* Generate opcode for registers numbered in the range 0 .. 15.  */
  num_regs_below_16 = num_vfpv3_regs > 0 ? 16 - (int) start : count;
  gas_assert (num_regs_below_16 + num_vfpv3_regs == count);
  if (num_regs_below_16 > 0)
    {
      op = 0xc900 | (start << 4) | (num_regs_below_16 - 1);
      add_unwind_opcode (op, 2);
    }

  unwind.frame_size += count * 8;
}


/* Parse a directive saving VFP registers for pre-ARMv6.  */

static void
s_arm_unwind_save_vfp (void)
{
  int count;
  unsigned int reg;
  valueT op;

  count = parse_vfp_reg_list (&input_line_pointer, &reg, REGLIST_VFP_D);
  if (count == FAIL)
    {
      as_bad (_("expected register list"));
      ignore_rest_of_line ();
      return;
    }

  demand_empty_rest_of_line ();

  if (reg == 8)
    {
      /* Short form.  */
      op = 0xb8 | (count - 1);
      add_unwind_opcode (op, 1);
    }
  else
    {
      /* Long form.  */
      op = 0xb300 | (reg << 4) | (count - 1);
      add_unwind_opcode (op, 2);
    }
  unwind.frame_size += count * 8 + 4;
}


/* Parse a directive saving iWMMXt data registers.  */

static void
s_arm_unwind_save_mmxwr (void)
{
  int reg;
  int hi_reg;
  int i;
  unsigned mask = 0;
  valueT op;

  if (*input_line_pointer == '{')
    input_line_pointer++;

  do
    {
      reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWR);

      if (reg == FAIL)
	{
	  as_bad ("%s", _(reg_expected_msgs[REG_TYPE_MMXWR]));
	  goto error;
	}

      if (mask >> reg)
	as_tsktsk (_("register list not in ascending order"));
      mask |= 1 << reg;

      if (*input_line_pointer == '-')
	{
	  input_line_pointer++;
	  hi_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWR);
	  if (hi_reg == FAIL)
	    {
	      as_bad ("%s", _(reg_expected_msgs[REG_TYPE_MMXWR]));
	      goto error;
	    }
	  else if (reg >= hi_reg)
	    {
	      as_bad (_("bad register range"));
	      goto error;
	    }
	  for (; reg < hi_reg; reg++)
	    mask |= 1 << reg;
	}
    }
  while (skip_past_comma (&input_line_pointer) != FAIL);

  skip_past_char (&input_line_pointer, '}');

  demand_empty_rest_of_line ();

  /* Generate any deferred opcodes because we're going to be looking at
     the list.	*/
  flush_pending_unwind ();

  for (i = 0; i < 16; i++)
    {
      if (mask & (1 << i))
	unwind.frame_size += 8;
    }

  /* Attempt to combine with a previous opcode.	 We do this because gcc
     likes to output separate unwind directives for a single block of
     registers.	 */
  if (unwind.opcode_count > 0)
    {
      i = unwind.opcodes[unwind.opcode_count - 1];
      if ((i & 0xf8) == 0xc0)
	{
	  i &= 7;
	  /* Only merge if the blocks are contiguous.  */
	  if (i < 6)
	    {
	      if ((mask & 0xfe00) == (1 << 9))
		{
		  mask |= ((1 << (i + 11)) - 1) & 0xfc00;
		  unwind.opcode_count--;
		}
	    }
	  else if (i == 6 && unwind.opcode_count >= 2)
	    {
	      i = unwind.opcodes[unwind.opcode_count - 2];
	      reg = i >> 4;
	      i &= 0xf;

	      op = 0xffff << (reg - 1);
	      if (reg > 0
		  && ((mask & op) == (1u << (reg - 1))))
		{
		  op = (1 << (reg + i + 1)) - 1;
		  op &= ~((1 << reg) - 1);
		  mask |= op;
		  unwind.opcode_count -= 2;
		}
	    }
	}
    }

  hi_reg = 15;
  /* We want to generate opcodes in the order the registers have been
     saved, ie. descending order.  */
  for (reg = 15; reg >= -1; reg--)
    {
      /* Save registers in blocks.  */
      if (reg < 0
	  || !(mask & (1 << reg)))
	{
	  /* We found an unsaved reg.  Generate opcodes to save the
	     preceding block.	*/
	  if (reg != hi_reg)
	    {
	      if (reg == 9)
		{
		  /* Short form.  */
		  op = 0xc0 | (hi_reg - 10);
		  add_unwind_opcode (op, 1);
		}
	      else
		{
		  /* Long form.	 */
		  op = 0xc600 | ((reg + 1) << 4) | ((hi_reg - reg) - 1);
		  add_unwind_opcode (op, 2);
		}
	    }
	  hi_reg = reg - 1;
	}
    }

  return;
error:
  ignore_rest_of_line ();
}

static void
s_arm_unwind_save_mmxwcg (void)
{
  int reg;
  int hi_reg;
  unsigned mask = 0;
  valueT op;

  if (*input_line_pointer == '{')
    input_line_pointer++;

  skip_whitespace (input_line_pointer);

  do
    {
      reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWCG);

      if (reg == FAIL)
	{
	  as_bad ("%s", _(reg_expected_msgs[REG_TYPE_MMXWCG]));
	  goto error;
	}

      reg -= 8;
      if (mask >> reg)
	as_tsktsk (_("register list not in ascending order"));
      mask |= 1 << reg;

      if (*input_line_pointer == '-')
	{
	  input_line_pointer++;
	  hi_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWCG);
	  if (hi_reg == FAIL)
	    {
	      as_bad ("%s", _(reg_expected_msgs[REG_TYPE_MMXWCG]));
	      goto error;
	    }
	  else if (reg >= hi_reg)
	    {
	      as_bad (_("bad register range"));
	      goto error;
	    }
	  for (; reg < hi_reg; reg++)
	    mask |= 1 << reg;
	}
    }
  while (skip_past_comma (&input_line_pointer) != FAIL);

  skip_past_char (&input_line_pointer, '}');

  demand_empty_rest_of_line ();

  /* Generate any deferred opcodes because we're going to be looking at
     the list.	*/
  flush_pending_unwind ();

  for (reg = 0; reg < 16; reg++)
    {
      if (mask & (1 << reg))
	unwind.frame_size += 4;
    }
  op = 0xc700 | mask;
  add_unwind_opcode (op, 2);
  return;
error:
  ignore_rest_of_line ();
}


/* Parse an unwind_save directive.
   If the argument is non-zero, this is a .vsave directive.  */

static void
s_arm_unwind_save (int arch_v6)
{
  char *peek;
  struct reg_entry *reg;
  bfd_boolean had_brace = FALSE;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  /* Figure out what sort of save we have.  */
  peek = input_line_pointer;

  if (*peek == '{')
    {
      had_brace = TRUE;
      peek++;
    }

  reg = arm_reg_parse_multi (&peek);

  if (!reg)
    {
      as_bad (_("register expected"));
      ignore_rest_of_line ();
      return;
    }

  switch (reg->type)
    {
    case REG_TYPE_FN:
      if (had_brace)
	{
	  as_bad (_("FPA .unwind_save does not take a register list"));
	  ignore_rest_of_line ();
	  return;
	}
      input_line_pointer = peek;
      s_arm_unwind_save_fpa (reg->number);
      return;

    case REG_TYPE_RN:
      s_arm_unwind_save_core ();
      return;

    case REG_TYPE_VFD:
      if (arch_v6)
	s_arm_unwind_save_vfp_armv6 ();
      else
	s_arm_unwind_save_vfp ();
      return;

    case REG_TYPE_MMXWR:
      s_arm_unwind_save_mmxwr ();
      return;

    case REG_TYPE_MMXWCG:
      s_arm_unwind_save_mmxwcg ();
      return;

    default:
      as_bad (_(".unwind_save does not support this kind of register"));
      ignore_rest_of_line ();
    }
}


/* Parse an unwind_movsp directive.  */

static void
s_arm_unwind_movsp (int ignored ATTRIBUTE_UNUSED)
{
  int reg;
  valueT op;
  int offset;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  reg = arm_reg_parse (&input_line_pointer, REG_TYPE_RN);
  if (reg == FAIL)
    {
      as_bad ("%s", _(reg_expected_msgs[REG_TYPE_RN]));
      ignore_rest_of_line ();
      return;
    }

  /* Optional constant.	 */
  if (skip_past_comma (&input_line_pointer) != FAIL)
    {
      if (immediate_for_directive (&offset) == FAIL)
	return;
    }
  else
    offset = 0;

  demand_empty_rest_of_line ();

  if (reg == REG_SP || reg == REG_PC)
    {
      as_bad (_("SP and PC not permitted in .unwind_movsp directive"));
      return;
    }

  if (unwind.fp_reg != REG_SP)
    as_bad (_("unexpected .unwind_movsp directive"));

  /* Generate opcode to restore the value.  */
  op = 0x90 | reg;
  add_unwind_opcode (op, 1);

  /* Record the information for later.	*/
  unwind.fp_reg = reg;
  unwind.fp_offset = unwind.frame_size - offset;
  unwind.sp_restored = 1;
}

/* Parse an unwind_pad directive.  */

static void
s_arm_unwind_pad (int ignored ATTRIBUTE_UNUSED)
{
  int offset;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  if (immediate_for_directive (&offset) == FAIL)
    return;

  if (offset & 3)
    {
      as_bad (_("stack increment must be multiple of 4"));
      ignore_rest_of_line ();
      return;
    }

  /* Don't generate any opcodes, just record the details for later.  */
  unwind.frame_size += offset;
  unwind.pending_offset += offset;

  demand_empty_rest_of_line ();
}

/* Parse an unwind_setfp directive.  */

static void
s_arm_unwind_setfp (int ignored ATTRIBUTE_UNUSED)
{
  int sp_reg;
  int fp_reg;
  int offset;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  fp_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_RN);
  if (skip_past_comma (&input_line_pointer) == FAIL)
    sp_reg = FAIL;
  else
    sp_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_RN);

  if (fp_reg == FAIL || sp_reg == FAIL)
    {
      as_bad (_("expected <reg>, <reg>"));
      ignore_rest_of_line ();
      return;
    }

  /* Optional constant.	 */
  if (skip_past_comma (&input_line_pointer) != FAIL)
    {
      if (immediate_for_directive (&offset) == FAIL)
	return;
    }
  else
    offset = 0;

  demand_empty_rest_of_line ();

  if (sp_reg != REG_SP && sp_reg != unwind.fp_reg)
    {
      as_bad (_("register must be either sp or set by a previous"
		"unwind_movsp directive"));
      return;
    }

  /* Don't generate any opcodes, just record the information for later.	 */
  unwind.fp_reg = fp_reg;
  unwind.fp_used = 1;
  if (sp_reg == REG_SP)
    unwind.fp_offset = unwind.frame_size - offset;
  else
    unwind.fp_offset -= offset;
}

/* Parse an unwind_raw directive.  */

static void
s_arm_unwind_raw (int ignored ATTRIBUTE_UNUSED)
{
  expressionS exp;
  /* This is an arbitrary limit.	 */
  unsigned char op[16];
  int count;

  if (!unwind.proc_start)
    as_bad (MISSING_FNSTART);

  expression (&exp);
  if (exp.X_op == O_constant
      && skip_past_comma (&input_line_pointer) != FAIL)
    {
      unwind.frame_size += exp.X_add_number;
      expression (&exp);
    }
  else
    exp.X_op = O_illegal;

  if (exp.X_op != O_constant)
    {
      as_bad (_("expected <offset>, <opcode>"));
      ignore_rest_of_line ();
      return;
    }

  count = 0;

  /* Parse the opcode.	*/
  for (;;)
    {
      if (count >= 16)
	{
	  as_bad (_("unwind opcode too long"));
	  ignore_rest_of_line ();
	}
      if (exp.X_op != O_constant || exp.X_add_number & ~0xff)
	{
	  as_bad (_("invalid unwind opcode"));
	  ignore_rest_of_line ();
	  return;
	}
      op[count++] = exp.X_add_number;

      /* Parse the next byte.  */
      if (skip_past_comma (&input_line_pointer) == FAIL)
	break;

      expression (&exp);
    }

  /* Add the opcode bytes in reverse order.  */
  while (count--)
    add_unwind_opcode (op[count], 1);

  demand_empty_rest_of_line ();
}


/* Parse a .eabi_attribute directive.  */

static void
s_arm_eabi_attribute (int ignored ATTRIBUTE_UNUSED)
{
  int tag = obj_elf_vendor_attribute (OBJ_ATTR_PROC);

  if (tag < NUM_KNOWN_OBJ_ATTRIBUTES)
    attributes_set_explicitly[tag] = 1;
}

/* Emit a tls fix for the symbol.  */

static void
s_arm_tls_descseq (int ignored ATTRIBUTE_UNUSED)
{
  char *p;
  expressionS exp;
#ifdef md_flush_pending_output
  md_flush_pending_output ();
#endif

#ifdef md_cons_align
  md_cons_align (4);
#endif

  /* Since we're just labelling the code, there's no need to define a
     mapping symbol.  */
  expression (&exp);
  p = obstack_next_free (&frchain_now->frch_obstack);
  fix_new_arm (frag_now, p - frag_now->fr_literal, 4, &exp, 0,
	       thumb_mode ? BFD_RELOC_ARM_THM_TLS_DESCSEQ
	       : BFD_RELOC_ARM_TLS_DESCSEQ);
}

static void s_arm_arch (int);
static void s_arm_object_arch (int);
static void s_arm_cpu (int);
static void s_arm_fpu (int);
static void s_arm_arch_extension (int);

/* This table describes all the machine specific pseudo-ops the assembler
   has to support.  The fields are:
     pseudo-op name without dot
     function to call to execute this pseudo-op
     Integer arg to pass to the function.  */

const pseudo_typeS md_pseudo_table[] =
{
  /* Never called because '.req' does not start a line.	 */
  { "req",	   s_req,	  0 },
  /* Following two are likewise never called.  */
  { "dn",	   s_dn,          0 },
  { "qn",          s_qn,          0 },
  { "unreq",	   s_unreq,	  0 },
  { "bss",	   s_bss,	  0 },
  { "align",	   s_align_ptwo,  2 },
  { "arm",	   s_arm,	  0 },
  { "thumb",	   s_thumb,	  0 },
  { "code",	   s_code,	  0 },
  { "force_thumb", s_force_thumb, 0 },
  { "thumb_func",  s_thumb_func,  0 },
  { "thumb_set",   s_thumb_set,	  0 },
  { "even",	   s_even,	  0 },
  { "ltorg",	   s_ltorg,	  0 },
  { "pool",	   s_ltorg,	  0 },
  { "syntax",	   s_syntax,	  0 },
  { "cpu",	   s_arm_cpu,	  0 },
  { "arch",	   s_arm_arch,	  0 },
  { "object_arch", s_arm_object_arch,	0 },
  { "fpu",	   s_arm_fpu,	  0 },
  { "arch_extension", s_arm_arch_extension, 0 },
  { "word",	        s_arm_elf_cons, 4 },
  { "long",	        s_arm_elf_cons, 4 },
  { "inst.n",           s_arm_elf_inst, 2 },
  { "inst.w",           s_arm_elf_inst, 4 },
  { "inst",             s_arm_elf_inst, 0 },
  { "rel31",	        s_arm_rel31,	  0 },
  { "fnstart",		s_arm_unwind_fnstart,	0 },
  { "fnend",		s_arm_unwind_fnend,	0 },
  { "cantunwind",	s_arm_unwind_cantunwind, 0 },
  { "personality",	s_arm_unwind_personality, 0 },
  { "personalityindex",	s_arm_unwind_personalityindex, 0 },
  { "handlerdata",	s_arm_unwind_handlerdata, 0 },
  { "save",		s_arm_unwind_save,	0 },
  { "vsave",		s_arm_unwind_save,	1 },
  { "movsp",		s_arm_unwind_movsp,	0 },
  { "pad",		s_arm_unwind_pad,	0 },
  { "setfp",		s_arm_unwind_setfp,	0 },
  { "unwind_raw",	s_arm_unwind_raw,	0 },
  { "eabi_attribute",	s_arm_eabi_attribute,	0 },
  { "tlsdescseq",	s_arm_tls_descseq,      0 },
  { "extend",	   float_cons, 'x' },
  { "ldouble",	   float_cons, 'x' },
  { "packed",	   float_cons, 'p' },

  /* These are for compatibility with CodeComposer Studio.  */
  {"ref",          s_ccs_ref,        0},
  {"def",          s_ccs_def,        0},
  {"asmfunc",      s_ccs_asmfunc,    0},
  {"endasmfunc",   s_ccs_endasmfunc, 0},

  { 0, 0, 0 }
};

/* Parser functions used exclusively in instruction operands.  */

/* Generic immediate-value read function for use in insn parsing.
   STR points to the beginning of the immediate (the leading #);
   VAL receives the value; if the value is outside [MIN, MAX]
   issue an error.  PREFIX_OPT is true if the immediate prefix is
   optional.  */

static int
parse_immediate (char **str, int *val, int min, int max,
		 bfd_boolean prefix_opt)
{
  expressionS exp;

  my_get_expression (&exp, str, prefix_opt ? GE_OPT_PREFIX : GE_IMM_PREFIX);
  if (exp.X_op != O_constant)
    {
      inst.error = _("constant expression required");
      return FAIL;
    }

  if (exp.X_add_number < min || exp.X_add_number > max)
    {
      inst.error = _("immediate value out of range");
      return FAIL;
    }

  *val = exp.X_add_number;
  return SUCCESS;
}

/* Less-generic immediate-value read function with the possibility of loading a
   big (64-bit) immediate, as required by Neon VMOV, VMVN and logic immediate
   instructions. Puts the result directly in inst.operands[i].  */

static int
parse_big_immediate (char **str, int i, expressionS *in_exp,
		     bfd_boolean allow_symbol_p)
{
  expressionS exp;
  expressionS *exp_p = in_exp ? in_exp : &exp;
  char *ptr = *str;

  my_get_expression (exp_p, &ptr, GE_OPT_PREFIX_BIG);

  if (exp_p->X_op == O_constant)
    {
      inst.operands[i].imm = exp_p->X_add_number & 0xffffffff;
      /* If we're on a 64-bit host, then a 64-bit number can be returned using
	 O_constant.  We have to be careful not to break compilation for
	 32-bit X_add_number, though.  */
      if ((exp_p->X_add_number & ~(offsetT)(0xffffffffU)) != 0)
	{
	  /* X >> 32 is illegal if sizeof (exp_p->X_add_number) == 4.  */
	  inst.operands[i].reg = (((exp_p->X_add_number >> 16) >> 16)
				  & 0xffffffff);
	  inst.operands[i].regisimm = 1;
	}
    }
  else if (exp_p->X_op == O_big
	   && LITTLENUM_NUMBER_OF_BITS * exp_p->X_add_number > 32)
    {
      unsigned parts = 32 / LITTLENUM_NUMBER_OF_BITS, j, idx = 0;

      /* Bignums have their least significant bits in
	 generic_bignum[0]. Make sure we put 32 bits in imm and
	 32 bits in reg,  in a (hopefully) portable way.  */
      gas_assert (parts != 0);

      /* Make sure that the number is not too big.
	 PR 11972: Bignums can now be sign-extended to the
	 size of a .octa so check that the out of range bits
	 are all zero or all one.  */
      if (LITTLENUM_NUMBER_OF_BITS * exp_p->X_add_number > 64)
	{
	  LITTLENUM_TYPE m = -1;

	  if (generic_bignum[parts * 2] != 0
	      && generic_bignum[parts * 2] != m)
	    return FAIL;

	  for (j = parts * 2 + 1; j < (unsigned) exp_p->X_add_number; j++)
	    if (generic_bignum[j] != generic_bignum[j-1])
	      return FAIL;
	}

      inst.operands[i].imm = 0;
      for (j = 0; j < parts; j++, idx++)
	inst.operands[i].imm |= generic_bignum[idx]
				<< (LITTLENUM_NUMBER_OF_BITS * j);
      inst.operands[i].reg = 0;
      for (j = 0; j < parts; j++, idx++)
	inst.operands[i].reg |= generic_bignum[idx]
				<< (LITTLENUM_NUMBER_OF_BITS * j);
      inst.operands[i].regisimm = 1;
    }
  else if (!(exp_p->X_op == O_symbol && allow_symbol_p))
    return FAIL;

  *str = ptr;

  return SUCCESS;
}

/* Returns the pseudo-register number of an FPA immediate constant,
   or FAIL if there isn't a valid constant here.  */

static int
parse_fpa_immediate (char ** str)
{
  LITTLENUM_TYPE words[MAX_LITTLENUMS];
  char *	 save_in;
  expressionS	 exp;
  int		 i;
  int		 j;

  /* First try and match exact strings, this is to guarantee
     that some formats will work even for cross assembly.  */

  for (i = 0; fp_const[i]; i++)
    {
      if (strncmp (*str, fp_const[i], strlen (fp_const[i])) == 0)
	{
	  char *start = *str;

	  *str += strlen (fp_const[i]);
	  if (is_end_of_line[(unsigned char) **str])
	    return i + 8;
	  *str = start;
	}
    }

  /* Just because we didn't get a match doesn't mean that the constant
     isn't valid, just that it is in a format that we don't
     automatically recognize.  Try parsing it with the standard
     expression routines.  */

  memset (words, 0, MAX_LITTLENUMS * sizeof (LITTLENUM_TYPE));

  /* Look for a raw floating point number.  */
  if ((save_in = atof_ieee (*str, 'x', words)) != NULL
      && is_end_of_line[(unsigned char) *save_in])
    {
      for (i = 0; i < NUM_FLOAT_VALS; i++)
	{
	  for (j = 0; j < MAX_LITTLENUMS; j++)
	    {
	      if (words[j] != fp_values[i][j])
		break;
	    }

	  if (j == MAX_LITTLENUMS)
	    {
	      *str = save_in;
	      return i + 8;
	    }
	}
    }

  /* Try and parse a more complex expression, this will probably fail
     unless the code uses a floating point prefix (eg "0f").  */
  save_in = input_line_pointer;
  input_line_pointer = *str;
  if (expression (&exp) == absolute_section
      && exp.X_op == O_big
      && exp.X_add_number < 0)
    {
      /* FIXME: 5 = X_PRECISION, should be #define'd where we can use it.
	 Ditto for 15.	*/
#define X_PRECISION 5
#define E_PRECISION 15L
      if (gen_to_words (words, X_PRECISION, E_PRECISION) == 0)
	{
	  for (i = 0; i < NUM_FLOAT_VALS; i++)
	    {
	      for (j = 0; j < MAX_LITTLENUMS; j++)
		{
		  if (words[j] != fp_values[i][j])
		    break;
		}

	      if (j == MAX_LITTLENUMS)
		{
		  *str = input_line_pointer;
		  input_line_pointer = save_in;
		  return i + 8;
		}
	    }
	}
    }

  *str = input_line_pointer;
  input_line_pointer = save_in;
  inst.error = _("invalid FPA immediate expression");
  return FAIL;
}

/* Returns 1 if a number has "quarter-precision" float format
   0baBbbbbbc defgh000 00000000 00000000.  */

static int
is_quarter_float (unsigned imm)
{
  int bs = (imm & 0x20000000) ? 0x3e000000 : 0x40000000;
  return (imm & 0x7ffff) == 0 && ((imm & 0x7e000000) ^ bs) == 0;
}


/* Detect the presence of a floating point or integer zero constant,
   i.e. #0.0 or #0.  */

static bfd_boolean
parse_ifimm_zero (char **in)
{
  int error_code;

  if (!is_immediate_prefix (**in))
    {
      /* In unified syntax, all prefixes are optional.  */
      if (!unified_syntax)
	return FALSE;
    }
  else
    ++*in;

  /* Accept #0x0 as a synonym for #0.  */
  if (strncmp (*in, "0x", 2) == 0)
    {
      int val;
      if (parse_immediate (in, &val, 0, 0, TRUE) == FAIL)
        return FALSE;
      return TRUE;
    }

  error_code = atof_generic (in, ".", EXP_CHARS,
                             &generic_floating_point_number);

  if (!error_code
      && generic_floating_point_number.sign == '+'
      && (generic_floating_point_number.low
          > generic_floating_point_number.leader))
    return TRUE;

  return FALSE;
}

/* Parse an 8-bit "quarter-precision" floating point number of the form:
   0baBbbbbbc defgh000 00000000 00000000.
   The zero and minus-zero cases need special handling, since they can't be
   encoded in the "quarter-precision" float format, but can nonetheless be
   loaded as integer constants.  */

static unsigned
parse_qfloat_immediate (char **ccp, int *immed)
{
  char *str = *ccp;
  char *fpnum;
  LITTLENUM_TYPE words[MAX_LITTLENUMS];
  int found_fpchar = 0;

  skip_past_char (&str, '#');

  /* We must not accidentally parse an integer as a floating-point number. Make
     sure that the value we parse is not an integer by checking for special
     characters '.' or 'e'.
     FIXME: This is a horrible hack, but doing better is tricky because type
     information isn't in a very usable state at parse time.  */
  fpnum = str;
  skip_whitespace (fpnum);

  if (strncmp (fpnum, "0x", 2) == 0)
    return FAIL;
  else
    {
      for (; *fpnum != '\0' && *fpnum != ' ' && *fpnum != '\n'; fpnum++)
	if (*fpnum == '.' || *fpnum == 'e' || *fpnum == 'E')
	  {
	    found_fpchar = 1;
	    break;
	  }

      if (!found_fpchar)
	return FAIL;
    }

  if ((str = atof_ieee (str, 's', words)) != NULL)
    {
      unsigned fpword = 0;
      int i;

      /* Our FP word must be 32 bits (single-precision FP).  */
      for (i = 0; i < 32 / LITTLENUM_NUMBER_OF_BITS; i++)
	{
	  fpword <<= LITTLENUM_NUMBER_OF_BITS;
	  fpword |= words[i];
	}

      if (is_quarter_float (fpword) || (fpword & 0x7fffffff) == 0)
	*immed = fpword;
      else
	return FAIL;

      *ccp = str;

      return SUCCESS;
    }

  return FAIL;
}

/* Shift operands.  */
enum shift_kind
{
  SHIFT_LSL, SHIFT_LSR, SHIFT_ASR, SHIFT_ROR, SHIFT_RRX
};

struct asm_shift_name
{
  const char	  *name;
  enum shift_kind  kind;
};

/* Third argument to parse_shift.  */
enum parse_shift_mode
{
  NO_SHIFT_RESTRICT,		/* Any kind of shift is accepted.  */
  SHIFT_IMMEDIATE,		/* Shift operand must be an immediate.	*/
  SHIFT_LSL_OR_ASR_IMMEDIATE,	/* Shift must be LSL or ASR immediate.	*/
  SHIFT_ASR_IMMEDIATE,		/* Shift must be ASR immediate.	 */
  SHIFT_LSL_IMMEDIATE,		/* Shift must be LSL immediate.	 */
};

/* Parse a <shift> specifier on an ARM data processing instruction.
   This has three forms:

     (LSL|LSR|ASL|ASR|ROR) Rs
     (LSL|LSR|ASL|ASR|ROR) #imm
     RRX

   Note that ASL is assimilated to LSL in the instruction encoding, and
   RRX to ROR #0 (which cannot be written as such).  */

static int
parse_shift (char **str, int i, enum parse_shift_mode mode)
{
  const struct asm_shift_name *shift_name;
  enum shift_kind shift;
  char *s = *str;
  char *p = s;
  int reg;

  for (p = *str; ISALPHA (*p); p++)
    ;

  if (p == *str)
    {
      inst.error = _("shift expression expected");
      return FAIL;
    }

  shift_name = (const struct asm_shift_name *) hash_find_n (arm_shift_hsh, *str,
							    p - *str);

  if (shift_name == NULL)
    {
      inst.error = _("shift expression expected");
      return FAIL;
    }

  shift = shift_name->kind;

  switch (mode)
    {
    case NO_SHIFT_RESTRICT:
    case SHIFT_IMMEDIATE:   break;

    case SHIFT_LSL_OR_ASR_IMMEDIATE:
      if (shift != SHIFT_LSL && shift != SHIFT_ASR)
	{
	  inst.error = _("'LSL' or 'ASR' required");
	  return FAIL;
	}
      break;

    case SHIFT_LSL_IMMEDIATE:
      if (shift != SHIFT_LSL)
	{
	  inst.error = _("'LSL' required");
	  return FAIL;
	}
      break;

    case SHIFT_ASR_IMMEDIATE:
      if (shift != SHIFT_ASR)
	{
	  inst.error = _("'ASR' required");
	  return FAIL;
	}
      break;

    default: abort ();
    }

  if (shift != SHIFT_RRX)
    {
      /* Whitespace can appear here if the next thing is a bare digit.	*/
      skip_whitespace (p);

      if (mode == NO_SHIFT_RESTRICT
	  && (reg = arm_reg_parse (&p, REG_TYPE_RN)) != FAIL)
	{
	  inst.operands[i].imm = reg;
	  inst.operands[i].immisreg = 1;
	}
      else if (my_get_expression (&inst.reloc.exp, &p, GE_IMM_PREFIX))
	return FAIL;
    }
  inst.operands[i].shift_kind = shift;
  inst.operands[i].shifted = 1;
  *str = p;
  return SUCCESS;
}

/* Parse a <shifter_operand> for an ARM data processing instruction:

      #<immediate>
      #<immediate>, <rotate>
      <Rm>
      <Rm>, <shift>

   where <shift> is defined by parse_shift above, and <rotate> is a
   multiple of 2 between 0 and 30.  Validation of immediate operands
   is deferred to md_apply_fix.  */

static int
parse_shifter_operand (char **str, int i)
{
  int value;
  expressionS exp;

  if ((value = arm_reg_parse (str, REG_TYPE_RN)) != FAIL)
    {
      inst.operands[i].reg = value;
      inst.operands[i].isreg = 1;

      /* parse_shift will override this if appropriate */
      inst.reloc.exp.X_op = O_constant;
      inst.reloc.exp.X_add_number = 0;

      if (skip_past_comma (str) == FAIL)
	return SUCCESS;

      /* Shift operation on register.  */
      return parse_shift (str, i, NO_SHIFT_RESTRICT);
    }

  if (my_get_expression (&inst.reloc.exp, str, GE_IMM_PREFIX))
    return FAIL;

  if (skip_past_comma (str) == SUCCESS)
    {
      /* #x, y -- ie explicit rotation by Y.  */
      if (my_get_expression (&exp, str, GE_NO_PREFIX))
	return FAIL;

      if (exp.X_op != O_constant || inst.reloc.exp.X_op != O_constant)
	{
	  inst.error = _("constant expression expected");
	  return FAIL;
	}

      value = exp.X_add_number;
      if (value < 0 || value > 30 || value % 2 != 0)
	{
	  inst.error = _("invalid rotation");
	  return FAIL;
	}
      if (inst.reloc.exp.X_add_number < 0 || inst.reloc.exp.X_add_number > 255)
	{
	  inst.error = _("invalid constant");
	  return FAIL;
	}

      /* Encode as specified.  */
      inst.operands[i].imm = inst.reloc.exp.X_add_number | value << 7;
      return SUCCESS;
    }

  inst.reloc.type = BFD_RELOC_ARM_IMMEDIATE;
  inst.reloc.pc_rel = 0;
  return SUCCESS;
}

/* Group relocation information.  Each entry in the table contains the
   textual name of the relocation as may appear in assembler source
   and must end with a colon.
   Along with this textual name are the relocation codes to be used if
   the corresponding instruction is an ALU instruction (ADD or SUB only),
   an LDR, an LDRS, or an LDC.  */

struct group_reloc_table_entry
{
  const char *name;
  int alu_code;
  int ldr_code;
  int ldrs_code;
  int ldc_code;
};

typedef enum
{
  /* Varieties of non-ALU group relocation.  */

  GROUP_LDR,
  GROUP_LDRS,
  GROUP_LDC
} group_reloc_type;

static struct group_reloc_table_entry group_reloc_table[] =
  { /* Program counter relative: */
    { "pc_g0_nc",
      BFD_RELOC_ARM_ALU_PC_G0_NC,	/* ALU */
      0,				/* LDR */
      0,				/* LDRS */
      0 },				/* LDC */
    { "pc_g0",
      BFD_RELOC_ARM_ALU_PC_G0,		/* ALU */
      BFD_RELOC_ARM_LDR_PC_G0,		/* LDR */
      BFD_RELOC_ARM_LDRS_PC_G0,		/* LDRS */
      BFD_RELOC_ARM_LDC_PC_G0 },	/* LDC */
    { "pc_g1_nc",
      BFD_RELOC_ARM_ALU_PC_G1_NC,	/* ALU */
      0,				/* LDR */
      0,				/* LDRS */
      0 },				/* LDC */
    { "pc_g1",
      BFD_RELOC_ARM_ALU_PC_G1,		/* ALU */
      BFD_RELOC_ARM_LDR_PC_G1, 		/* LDR */
      BFD_RELOC_ARM_LDRS_PC_G1,		/* LDRS */
      BFD_RELOC_ARM_LDC_PC_G1 },	/* LDC */
    { "pc_g2",
      BFD_RELOC_ARM_ALU_PC_G2,		/* ALU */
      BFD_RELOC_ARM_LDR_PC_G2,		/* LDR */
      BFD_RELOC_ARM_LDRS_PC_G2,		/* LDRS */
      BFD_RELOC_ARM_LDC_PC_G2 },	/* LDC */
    /* Section base relative */
    { "sb_g0_nc",
      BFD_RELOC_ARM_ALU_SB_G0_NC,	/* ALU */
      0,				/* LDR */
      0,				/* LDRS */
      0 },				/* LDC */
    { "sb_g0",
      BFD_RELOC_ARM_ALU_SB_G0,		/* ALU */
      BFD_RELOC_ARM_LDR_SB_G0,		/* LDR */
      BFD_RELOC_ARM_LDRS_SB_G0,		/* LDRS */
      BFD_RELOC_ARM_LDC_SB_G0 },	/* LDC */
    { "sb_g1_nc",
      BFD_RELOC_ARM_ALU_SB_G1_NC,	/* ALU */
      0,				/* LDR */
      0,				/* LDRS */
      0 },				/* LDC */
    { "sb_g1",
      BFD_RELOC_ARM_ALU_SB_G1,		/* ALU */
      BFD_RELOC_ARM_LDR_SB_G1, 		/* LDR */
      BFD_RELOC_ARM_LDRS_SB_G1,		/* LDRS */
      BFD_RELOC_ARM_LDC_SB_G1 },	/* LDC */
    { "sb_g2",
      BFD_RELOC_ARM_ALU_SB_G2,		/* ALU */
      BFD_RELOC_ARM_LDR_SB_G2,		/* LDR */
      BFD_RELOC_ARM_LDRS_SB_G2,		/* LDRS */
      BFD_RELOC_ARM_LDC_SB_G2 },	/* LDC */
    /* Absolute thumb alu relocations.  */
    { "lower0_7",
      BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC,/* ALU.  */
      0,				/* LDR.  */
      0,				/* LDRS.  */
      0 },				/* LDC.  */
    { "lower8_15",
      BFD_RELOC_ARM_THUMB_ALU_ABS_G1_NC,/* ALU.  */
      0,				/* LDR.  */
      0,				/* LDRS.  */
      0 },				/* LDC.  */
    { "upper0_7",
      BFD_RELOC_ARM_THUMB_ALU_ABS_G2_NC,/* ALU.  */
      0,				/* LDR.  */
      0,				/* LDRS.  */
      0 },				/* LDC.  */
    { "upper8_15",
      BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC,/* ALU.  */
      0,				/* LDR.  */
      0,				/* LDRS.  */
      0 } };				/* LDC.  */

/* Given the address of a pointer pointing to the textual name of a group
   relocation as may appear in assembler source, attempt to find its details
   in group_reloc_table.  The pointer will be updated to the character after
   the trailing colon.  On failure, FAIL will be returned; SUCCESS
   otherwise.  On success, *entry will be updated to point at the relevant
   group_reloc_table entry. */

static int
find_group_reloc_table_entry (char **str, struct group_reloc_table_entry **out)
{
  unsigned int i;
  for (i = 0; i < ARRAY_SIZE (group_reloc_table); i++)
    {
      int length = strlen (group_reloc_table[i].name);

      if (strncasecmp (group_reloc_table[i].name, *str, length) == 0
	  && (*str)[length] == ':')
	{
	  *out = &group_reloc_table[i];
	  *str += (length + 1);
	  return SUCCESS;
	}
    }

  return FAIL;
}

/* Parse a <shifter_operand> for an ARM data processing instruction
   (as for parse_shifter_operand) where group relocations are allowed:

      #<immediate>
      #<immediate>, <rotate>
      #:<group_reloc>:<expression>
      <Rm>
      <Rm>, <shift>

   where <group_reloc> is one of the strings defined in group_reloc_table.
   The hashes are optional.

   Everything else is as for parse_shifter_operand.  */

static parse_operand_result
parse_shifter_operand_group_reloc (char **str, int i)
{
  /* Determine if we have the sequence of characters #: or just :
     coming next.  If we do, then we check for a group relocation.
     If we don't, punt the whole lot to parse_shifter_operand.  */

  if (((*str)[0] == '#' && (*str)[1] == ':')
      || (*str)[0] == ':')
    {
      struct group_reloc_table_entry *entry;

      if ((*str)[0] == '#')
	(*str) += 2;
      else
	(*str)++;

      /* Try to parse a group relocation.  Anything else is an error.  */
      if (find_group_reloc_table_entry (str, &entry) == FAIL)
	{
	  inst.error = _("unknown group relocation");
	  return PARSE_OPERAND_FAIL_NO_BACKTRACK;
	}

      /* We now have the group relocation table entry corresponding to
	 the name in the assembler source.  Next, we parse the expression.  */
      if (my_get_expression (&inst.reloc.exp, str, GE_NO_PREFIX))
	return PARSE_OPERAND_FAIL_NO_BACKTRACK;

      /* Record the relocation type (always the ALU variant here).  */
      inst.reloc.type = (bfd_reloc_code_real_type) entry->alu_code;
      gas_assert (inst.reloc.type != 0);

      return PARSE_OPERAND_SUCCESS;
    }
  else
    return parse_shifter_operand (str, i) == SUCCESS
	   ? PARSE_OPERAND_SUCCESS : PARSE_OPERAND_FAIL;

  /* Never reached.  */
}

/* Parse a Neon alignment expression.  Information is written to
   inst.operands[i].  We assume the initial ':' has been skipped.

   align	.imm = align << 8, .immisalign=1, .preind=0  */
static parse_operand_result
parse_neon_alignment (char **str, int i)
{
  char *p = *str;
  expressionS exp;

  my_get_expression (&exp, &p, GE_NO_PREFIX);

  if (exp.X_op != O_constant)
    {
      inst.error = _("alignment must be constant");
      return PARSE_OPERAND_FAIL;
    }

  inst.operands[i].imm = exp.X_add_number << 8;
  inst.operands[i].immisalign = 1;
  /* Alignments are not pre-indexes.  */
  inst.operands[i].preind = 0;

  *str = p;
  return PARSE_OPERAND_SUCCESS;
}

/* Parse all forms of an ARM address expression.  Information is written
   to inst.operands[i] and/or inst.reloc.

   Preindexed addressing (.preind=1):

   [Rn, #offset]       .reg=Rn .reloc.exp=offset
   [Rn, +/-Rm]	       .reg=Rn .imm=Rm .immisreg=1 .negative=0/1
   [Rn, +/-Rm, shift]  .reg=Rn .imm=Rm .immisreg=1 .negative=0/1
		       .shift_kind=shift .reloc.exp=shift_imm

   These three may have a trailing ! which causes .writeback to be set also.

   Postindexed addressing (.postind=1, .writeback=1):

   [Rn], #offset       .reg=Rn .reloc.exp=offset
   [Rn], +/-Rm	       .reg=Rn .imm=Rm .immisreg=1 .negative=0/1
   [Rn], +/-Rm, shift  .reg=Rn .imm=Rm .immisreg=1 .negative=0/1
		       .shift_kind=shift .reloc.exp=shift_imm

   Unindexed addressing (.preind=0, .postind=0):

   [Rn], {option}      .reg=Rn .imm=option .immisreg=0

   Other:

   [Rn]{!}	       shorthand for [Rn,#0]{!}
   =immediate	       .isreg=0 .reloc.exp=immediate
   label	       .reg=PC .reloc.pc_rel=1 .reloc.exp=label

  It is the caller's responsibility to check for addressing modes not
  supported by the instruction, and to set inst.reloc.type.  */

static parse_operand_result
parse_address_main (char **str, int i, int group_relocations,
		    group_reloc_type group_type)
{
  char *p = *str;
  int reg;

  if (skip_past_char (&p, '[') == FAIL)
    {
      if (skip_past_char (&p, '=') == FAIL)
	{
	  /* Bare address - translate to PC-relative offset.  */
	  inst.reloc.pc_rel = 1;
	  inst.operands[i].reg = REG_PC;
	  inst.operands[i].isreg = 1;
	  inst.operands[i].preind = 1;

	  if (my_get_expression (&inst.reloc.exp, &p, GE_OPT_PREFIX_BIG))
	    return PARSE_OPERAND_FAIL;
	}
      else if (parse_big_immediate (&p, i, &inst.reloc.exp,
				    /*allow_symbol_p=*/TRUE))
	return PARSE_OPERAND_FAIL;

      *str = p;
      return PARSE_OPERAND_SUCCESS;
    }

  /* PR gas/14887: Allow for whitespace after the opening bracket.  */
  skip_whitespace (p);

  if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) == FAIL)
    {
      inst.error = _(reg_expected_msgs[REG_TYPE_RN]);
      return PARSE_OPERAND_FAIL;
    }
  inst.operands[i].reg = reg;
  inst.operands[i].isreg = 1;

  if (skip_past_comma (&p) == SUCCESS)
    {
      inst.operands[i].preind = 1;

      if (*p == '+') p++;
      else if (*p == '-') p++, inst.operands[i].negative = 1;

      if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) != FAIL)
	{
	  inst.operands[i].imm = reg;
	  inst.operands[i].immisreg = 1;

	  if (skip_past_comma (&p) == SUCCESS)
	    if (parse_shift (&p, i, SHIFT_IMMEDIATE) == FAIL)
	      return PARSE_OPERAND_FAIL;
	}
      else if (skip_past_char (&p, ':') == SUCCESS)
	{
	  /* FIXME: '@' should be used here, but it's filtered out by generic
	     code before we get to see it here. This may be subject to
	     change.  */
	  parse_operand_result result = parse_neon_alignment (&p, i);

	  if (result != PARSE_OPERAND_SUCCESS)
	    return result;
	}
      else
	{
	  if (inst.operands[i].negative)
	    {
	      inst.operands[i].negative = 0;
	      p--;
	    }

	  if (group_relocations
	      && ((*p == '#' && *(p + 1) == ':') || *p == ':'))
	    {
	      struct group_reloc_table_entry *entry;

	      /* Skip over the #: or : sequence.  */
	      if (*p == '#')
		p += 2;
	      else
		p++;

	      /* Try to parse a group relocation.  Anything else is an
		 error.  */
	      if (find_group_reloc_table_entry (&p, &entry) == FAIL)
		{
		  inst.error = _("unknown group relocation");
		  return PARSE_OPERAND_FAIL_NO_BACKTRACK;
		}

	      /* We now have the group relocation table entry corresponding to
		 the name in the assembler source.  Next, we parse the
		 expression.  */
	      if (my_get_expression (&inst.reloc.exp, &p, GE_NO_PREFIX))
		return PARSE_OPERAND_FAIL_NO_BACKTRACK;

	      /* Record the relocation type.  */
	      switch (group_type)
		{
		  case GROUP_LDR:
		    inst.reloc.type = (bfd_reloc_code_real_type) entry->ldr_code;
		    break;

		  case GROUP_LDRS:
		    inst.reloc.type = (bfd_reloc_code_real_type) entry->ldrs_code;
		    break;

		  case GROUP_LDC:
		    inst.reloc.type = (bfd_reloc_code_real_type) entry->ldc_code;
		    break;

		  default:
		    gas_assert (0);
		}

	      if (inst.reloc.type == 0)
		{
		  inst.error = _("this group relocation is not allowed on this instruction");
		  return PARSE_OPERAND_FAIL_NO_BACKTRACK;
		}
	    }
	  else
	    {
	      char *q = p;

	      if (my_get_expression (&inst.reloc.exp, &p, GE_IMM_PREFIX))
		return PARSE_OPERAND_FAIL;
	      /* If the offset is 0, find out if it's a +0 or -0.  */
	      if (inst.reloc.exp.X_op == O_constant
		  && inst.reloc.exp.X_add_number == 0)
		{
		  skip_whitespace (q);
		  if (*q == '#')
		    {
		      q++;
		      skip_whitespace (q);
		    }
		  if (*q == '-')
		    inst.operands[i].negative = 1;
		}
	    }
	}
    }
  else if (skip_past_char (&p, ':') == SUCCESS)
    {
      /* FIXME: '@' should be used here, but it's filtered out by generic code
	 before we get to see it here. This may be subject to change.  */
      parse_operand_result result = parse_neon_alignment (&p, i);

      if (result != PARSE_OPERAND_SUCCESS)
	return result;
    }

  if (skip_past_char (&p, ']') == FAIL)
    {
      inst.error = _("']' expected");
      return PARSE_OPERAND_FAIL;
    }

  if (skip_past_char (&p, '!') == SUCCESS)
    inst.operands[i].writeback = 1;

  else if (skip_past_comma (&p) == SUCCESS)
    {
      if (skip_past_char (&p, '{') == SUCCESS)
	{
	  /* [Rn], {expr} - unindexed, with option */
	  if (parse_immediate (&p, &inst.operands[i].imm,
			       0, 255, TRUE) == FAIL)
	    return PARSE_OPERAND_FAIL;

	  if (skip_past_char (&p, '}') == FAIL)
	    {
	      inst.error = _("'}' expected at end of 'option' field");
	      return PARSE_OPERAND_FAIL;
	    }
	  if (inst.operands[i].preind)
	    {
	      inst.error = _("cannot combine index with option");
	      return PARSE_OPERAND_FAIL;
	    }
	  *str = p;
	  return PARSE_OPERAND_SUCCESS;
	}
      else
	{
	  inst.operands[i].postind = 1;
	  inst.operands[i].writeback = 1;

	  if (inst.operands[i].preind)
	    {
	      inst.error = _("cannot combine pre- and post-indexing");
	      return PARSE_OPERAND_FAIL;
	    }

	  if (*p == '+') p++;
	  else if (*p == '-') p++, inst.operands[i].negative = 1;

	  if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) != FAIL)
	    {
	      /* We might be using the immediate for alignment already. If we
		 are, OR the register number into the low-order bits.  */
	      if (inst.operands[i].immisalign)
		inst.operands[i].imm |= reg;
	      else
		inst.operands[i].imm = reg;
	      inst.operands[i].immisreg = 1;

	      if (skip_past_comma (&p) == SUCCESS)
		if (parse_shift (&p, i, SHIFT_IMMEDIATE) == FAIL)
		  return PARSE_OPERAND_FAIL;
	    }
	  else
	    {
	      char *q = p;

	      if (inst.operands[i].negative)
		{
		  inst.operands[i].negative = 0;
		  p--;
		}
	      if (my_get_expression (&inst.reloc.exp, &p, GE_IMM_PREFIX))
		return PARSE_OPERAND_FAIL;
	      /* If the offset is 0, find out if it's a +0 or -0.  */
	      if (inst.reloc.exp.X_op == O_constant
		  && inst.reloc.exp.X_add_number == 0)
		{
		  skip_whitespace (q);
		  if (*q == '#')
		    {
		      q++;
		      skip_whitespace (q);
		    }
		  if (*q == '-')
		    inst.operands[i].negative = 1;
		}
	    }
	}
    }

  /* If at this point neither .preind nor .postind is set, we have a
     bare [Rn]{!}, which is shorthand for [Rn,#0]{!}.  */
  if (inst.operands[i].preind == 0 && inst.operands[i].postind == 0)
    {
      inst.operands[i].preind = 1;
      inst.reloc.exp.X_op = O_constant;
      inst.reloc.exp.X_add_number = 0;
    }
  *str = p;
  return PARSE_OPERAND_SUCCESS;
}

static int
parse_address (char **str, int i)
{
  return parse_address_main (str, i, 0, GROUP_LDR) == PARSE_OPERAND_SUCCESS
	 ? SUCCESS : FAIL;
}

static parse_operand_result
parse_address_group_reloc (char **str, int i, group_reloc_type type)
{
  return parse_address_main (str, i, 1, type);
}

/* Parse an operand for a MOVW or MOVT instruction.  */
static int
parse_half (char **str)
{
  char * p;

  p = *str;
  skip_past_char (&p, '#');
  if (strncasecmp (p, ":lower16:", 9) == 0)
    inst.reloc.type = BFD_RELOC_ARM_MOVW;
  else if (strncasecmp (p, ":upper16:", 9) == 0)
    inst.reloc.type = BFD_RELOC_ARM_MOVT;

  if (inst.reloc.type != BFD_RELOC_UNUSED)
    {
      p += 9;
      skip_whitespace (p);
    }

  if (my_get_expression (&inst.reloc.exp, &p, GE_NO_PREFIX))
    return FAIL;

  if (inst.reloc.type == BFD_RELOC_UNUSED)
    {
      if (inst.reloc.exp.X_op != O_constant)
	{
	  inst.error = _("constant expression expected");
	  return FAIL;
	}
      if (inst.reloc.exp.X_add_number < 0
	  || inst.reloc.exp.X_add_number > 0xffff)
	{
	  inst.error = _("immediate value out of range");
	  return FAIL;
	}
    }
  *str = p;
  return SUCCESS;
}

/* Miscellaneous. */

/* Parse a PSR flag operand.  The value returned is FAIL on syntax error,
   or a bitmask suitable to be or-ed into the ARM msr instruction.  */
static int
parse_psr (char **str, bfd_boolean lhs ATTRIBUTE_UNUSED)
{
  char *p;
  unsigned long psr_field;
  const struct asm_psr *psr;
  char *start;
  bfd_boolean m_profile = FALSE;

  /* PR gas/12698:  If the user has specified -march=all then m_profile will
     be TRUE, but we want to ignore it in this case as we are building for any
     CPU type, including non-m variants.  */
  if (ARM_FEATURE_CORE_EQUAL (selected_cpu, arm_arch_any))
    m_profile = FALSE;

  /* CPSR's and SPSR's can now be lowercase.  This is just a convenience
     feature for ease of use and backwards compatibility.  */
  p = *str;
  if (strncasecmp (p, "SPSR", 4) == 0)
    {
      if (m_profile)
	goto unsupported_psr;

      psr_field = SPSR_BIT;
    }
  else if (strncasecmp (p, "CPSR", 4) == 0)
    {
      if (m_profile)
	goto unsupported_psr;

      psr_field = 0;
    }
  else
    goto unsupported_psr;

  p += 4;

  if (*p == '_')
    {
      /* A suffix follows.  */
      p++;
      start = p;

      do
	p++;
      while (ISALNUM (*p) || *p == '_');

	{
	  psr = (const struct asm_psr *) hash_find_n (arm_psr_hsh, start,
						      p - start);
	  if (!psr)
	    goto error;

	  psr_field |= psr->field;
	}
    }
  else
    {
      if (ISALNUM (*p))
	goto error;    /* Garbage after "[CS]PSR".  */

      if (!m_profile)
	/* These bits are never right for M-profile devices: don't set them
	   (only code paths which read/write APSR reach here).  */
	psr_field |= (PSR_c | PSR_f);
    }
  *str = p;
  return psr_field;

 unsupported_psr:
  inst.error = _("selected processor does not support requested special "
		 "purpose register");
  return FAIL;

 error:
  inst.error = _("flag for {c}psr instruction expected");
  return FAIL;
}

/* Parse the flags argument to CPSI[ED].  Returns FAIL on error, or a
   value suitable for splatting into the AIF field of the instruction.	*/

static int
parse_cps_flags (char **str)
{
  int val = 0;
  int saw_a_flag = 0;
  char *s = *str;

  for (;;)
    switch (*s++)
      {
      case '\0': case ',':
	goto done;

      case 'a': case 'A': saw_a_flag = 1; val |= 0x4; break;
      case 'i': case 'I': saw_a_flag = 1; val |= 0x2; break;
      case 'f': case 'F': saw_a_flag = 1; val |= 0x1; break;

      default:
	inst.error = _("unrecognized CPS flag");
	return FAIL;
      }

 done:
  if (saw_a_flag == 0)
    {
      inst.error = _("missing CPS flags");
      return FAIL;
    }

  *str = s - 1;
  return val;
}

/* Parse an endian specifier ("BE" or "LE", case insensitive);
   returns 0 for big-endian, 1 for little-endian, FAIL for an error.  */

static int
parse_endian_specifier (char **str)
{
  int little_endian;
  char *s = *str;

  if (strncasecmp (s, "BE", 2))
    little_endian = 0;
  else if (strncasecmp (s, "LE", 2))
    little_endian = 1;
  else
    {
      inst.error = _("valid endian specifiers are be or le");
      return FAIL;
    }

  if (ISALNUM (s[2]) || s[2] == '_')
    {
      inst.error = _("valid endian specifiers are be or le");
      return FAIL;
    }

  *str = s + 2;
  return little_endian;
}

/* Parse a rotation specifier: ROR #0, #8, #16, #24.  *val receives a
   value suitable for poking into the rotate field of an sxt or sxta
   instruction, or FAIL on error.  */

static int
parse_ror (char **str)
{
  int rot;
  char *s = *str;

  if (strncasecmp (s, "ROR", 3) == 0)
    s += 3;
  else
    {
      inst.error = _("missing rotation field after comma");
      return FAIL;
    }

  if (parse_immediate (&s, &rot, 0, 24, FALSE) == FAIL)
    return FAIL;

  switch (rot)
    {
    case  0: *str = s; return 0x0;
    case  8: *str = s; return 0x1;
    case 16: *str = s; return 0x2;
    case 24: *str = s; return 0x3;

    default:
      inst.error = _("rotation can only be 0, 8, 16, or 24");
      return FAIL;
    }
}

/* Parse a conditional code (from conds[] below).  The value returned is in the
   range 0 .. 14, or FAIL.  */
static int
parse_cond (char **str)
{
  char *q;
  const struct asm_cond *c;
  int n;
  /* Condition codes are always 2 characters, so matching up to
     3 characters is sufficient.  */
  char cond[3];

  q = *str;
  n = 0;
  while (ISALPHA (*q) && n < 3)
    {
      cond[n] = TOLOWER (*q);
      q++;
      n++;
    }

  c = (const struct asm_cond *) hash_find_n (arm_cond_hsh, cond, n);
  if (!c)
    {
      inst.error = _("condition required");
      return FAIL;
    }

  *str = q;
  return c->value;
}

/* Record a use of the given feature.  */
static void
record_feature_use (const arm_feature_set *feature)
{
  if (thumb_mode)
    ARM_MERGE_FEATURE_SETS (thumb_arch_used, thumb_arch_used, *feature);
  else
    ARM_MERGE_FEATURE_SETS (arm_arch_used, arm_arch_used, *feature);
}

/* If the given feature is currently allowed, mark it as used and return TRUE.
   Return FALSE otherwise.  */
static bfd_boolean
mark_feature_used (const arm_feature_set *feature)
{
  /* Ensure the option is currently allowed.  */
  if (!ARM_CPU_HAS_FEATURE (cpu_variant, *feature))
    return FALSE;

  /* Add the appropriate architecture feature for the barrier option used.  */
  record_feature_use (feature);

  return TRUE;
}

/* Parse an option for a barrier instruction.  Returns the encoding for the
   option, or FAIL.  */
static int
parse_barrier (char **str)
{
  char *p, *q;
  const struct asm_barrier_opt *o;

  p = q = *str;
  while (ISALPHA (*q))
    q++;

  o = (const struct asm_barrier_opt *) hash_find_n (arm_barrier_opt_hsh, p,
						    q - p);
  if (!o)
    return FAIL;

  if (!mark_feature_used (&o->arch))
    return FAIL;

  *str = q;
  return o->value;
}

/* Parse the operands of a table branch instruction.  Similar to a memory
   operand.  */
static int
parse_tb (char **str)
{
  char * p = *str;
  int reg;

  if (skip_past_char (&p, '[') == FAIL)
    {
      inst.error = _("'[' expected");
      return FAIL;
    }

  if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) == FAIL)
    {
      inst.error = _(reg_expected_msgs[REG_TYPE_RN]);
      return FAIL;
    }
  inst.operands[0].reg = reg;

  if (skip_past_comma (&p) == FAIL)
    {
      inst.error = _("',' expected");
      return FAIL;
    }

  if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) == FAIL)
    {
      inst.error = _(reg_expected_msgs[REG_TYPE_RN]);
      return FAIL;
    }
  inst.operands[0].imm = reg;

  if (skip_past_comma (&p) == SUCCESS)
    {
      if (parse_shift (&p, 0, SHIFT_LSL_IMMEDIATE) == FAIL)
	return FAIL;
      if (inst.reloc.exp.X_add_number != 1)
	{
	  inst.error = _("invalid shift");
	  return FAIL;
	}
      inst.operands[0].shifted = 1;
    }

  if (skip_past_char (&p, ']') == FAIL)
    {
      inst.error = _("']' expected");
      return FAIL;
    }
  *str = p;
  return SUCCESS;
}

/* Parse the operands of a Neon VMOV instruction. See do_neon_mov for more
   information on the types the operands can take and how they are encoded.
   Up to four operands may be read; this function handles setting the
   ".present" field for each read operand itself.
   Updates STR and WHICH_OPERAND if parsing is successful and returns SUCCESS,
   else returns FAIL.  */

static int
parse_neon_mov (char **str, int *which_operand)
{
  int i = *which_operand, val;
  enum arm_reg_type rtype;
  char *ptr = *str;
  struct neon_type_el optype;

  if ((val = parse_scalar (&ptr, 8, &optype)) != FAIL)
    {
      /* Case 4: VMOV<c><q>.<size> <Dn[x]>, <Rd>.  */
      inst.operands[i].reg = val;
      inst.operands[i].isscalar = 1;
      inst.operands[i].vectype = optype;
      inst.operands[i++].present = 1;

      if (skip_past_comma (&ptr) == FAIL)
	goto wanted_comma;

      if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) == FAIL)
	goto wanted_arm;

      inst.operands[i].reg = val;
      inst.operands[i].isreg = 1;
      inst.operands[i].present = 1;
    }
  else if ((val = arm_typed_reg_parse (&ptr, REG_TYPE_NSDQ, &rtype, &optype))
	   != FAIL)
    {
      /* Cases 0, 1, 2, 3, 5 (D only).  */
      if (skip_past_comma (&ptr) == FAIL)
	goto wanted_comma;

      inst.operands[i].reg = val;
      inst.operands[i].isreg = 1;
      inst.operands[i].isquad = (rtype == REG_TYPE_NQ);
      inst.operands[i].issingle = (rtype == REG_TYPE_VFS);
      inst.operands[i].isvec = 1;
      inst.operands[i].vectype = optype;
      inst.operands[i++].present = 1;

      if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) != FAIL)
	{
	  /* Case 5: VMOV<c><q> <Dm>, <Rd>, <Rn>.
	     Case 13: VMOV <Sd>, <Rm>  */
	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  inst.operands[i].present = 1;

	  if (rtype == REG_TYPE_NQ)
	    {
	      first_error (_("can't use Neon quad register here"));
	      return FAIL;
	    }
	  else if (rtype != REG_TYPE_VFS)
	    {
	      i++;
	      if (skip_past_comma (&ptr) == FAIL)
		goto wanted_comma;
	      if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) == FAIL)
		goto wanted_arm;
	      inst.operands[i].reg = val;
	      inst.operands[i].isreg = 1;
	      inst.operands[i].present = 1;
	    }
	}
      else if ((val = arm_typed_reg_parse (&ptr, REG_TYPE_NSDQ, &rtype,
					   &optype)) != FAIL)
	{
	  /* Case 0: VMOV<c><q> <Qd>, <Qm>
	     Case 1: VMOV<c><q> <Dd>, <Dm>
	     Case 8: VMOV.F32 <Sd>, <Sm>
	     Case 15: VMOV <Sd>, <Se>, <Rn>, <Rm>  */

	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  inst.operands[i].isquad = (rtype == REG_TYPE_NQ);
	  inst.operands[i].issingle = (rtype == REG_TYPE_VFS);
	  inst.operands[i].isvec = 1;
	  inst.operands[i].vectype = optype;
	  inst.operands[i].present = 1;

	  if (skip_past_comma (&ptr) == SUCCESS)
	    {
	      /* Case 15.  */
	      i++;

	      if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) == FAIL)
		goto wanted_arm;

	      inst.operands[i].reg = val;
	      inst.operands[i].isreg = 1;
	      inst.operands[i++].present = 1;

	      if (skip_past_comma (&ptr) == FAIL)
		goto wanted_comma;

	      if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) == FAIL)
		goto wanted_arm;

	      inst.operands[i].reg = val;
	      inst.operands[i].isreg = 1;
	      inst.operands[i].present = 1;
	    }
	}
      else if (parse_qfloat_immediate (&ptr, &inst.operands[i].imm) == SUCCESS)
	  /* Case 2: VMOV<c><q>.<dt> <Qd>, #<float-imm>
	     Case 3: VMOV<c><q>.<dt> <Dd>, #<float-imm>
	     Case 10: VMOV.F32 <Sd>, #<imm>
	     Case 11: VMOV.F64 <Dd>, #<imm>  */
	inst.operands[i].immisfloat = 1;
      else if (parse_big_immediate (&ptr, i, NULL, /*allow_symbol_p=*/FALSE)
	       == SUCCESS)
	  /* Case 2: VMOV<c><q>.<dt> <Qd>, #<imm>
	     Case 3: VMOV<c><q>.<dt> <Dd>, #<imm>  */
	;
      else
	{
	  first_error (_("expected <Rm> or <Dm> or <Qm> operand"));
	  return FAIL;
	}
    }
  else if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) != FAIL)
    {
      /* Cases 6, 7.  */
      inst.operands[i].reg = val;
      inst.operands[i].isreg = 1;
      inst.operands[i++].present = 1;

      if (skip_past_comma (&ptr) == FAIL)
	goto wanted_comma;

      if ((val = parse_scalar (&ptr, 8, &optype)) != FAIL)
	{
	  /* Case 6: VMOV<c><q>.<dt> <Rd>, <Dn[x]>  */
	  inst.operands[i].reg = val;
	  inst.operands[i].isscalar = 1;
	  inst.operands[i].present = 1;
	  inst.operands[i].vectype = optype;
	}
      else if ((val = arm_reg_parse (&ptr, REG_TYPE_RN)) != FAIL)
	{
	  /* Case 7: VMOV<c><q> <Rd>, <Rn>, <Dm>  */
	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  inst.operands[i++].present = 1;

	  if (skip_past_comma (&ptr) == FAIL)
	    goto wanted_comma;

	  if ((val = arm_typed_reg_parse (&ptr, REG_TYPE_VFSD, &rtype, &optype))
	      == FAIL)
	    {
	      first_error (_(reg_expected_msgs[REG_TYPE_VFSD]));
	      return FAIL;
	    }

	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  inst.operands[i].isvec = 1;
	  inst.operands[i].issingle = (rtype == REG_TYPE_VFS);
	  inst.operands[i].vectype = optype;
	  inst.operands[i].present = 1;

	  if (rtype == REG_TYPE_VFS)
	    {
	      /* Case 14.  */
	      i++;
	      if (skip_past_comma (&ptr) == FAIL)
		goto wanted_comma;
	      if ((val = arm_typed_reg_parse (&ptr, REG_TYPE_VFS, NULL,
					      &optype)) == FAIL)
		{
		  first_error (_(reg_expected_msgs[REG_TYPE_VFS]));
		  return FAIL;
		}
	      inst.operands[i].reg = val;
	      inst.operands[i].isreg = 1;
	      inst.operands[i].isvec = 1;
	      inst.operands[i].issingle = 1;
	      inst.operands[i].vectype = optype;
	      inst.operands[i].present = 1;
	    }
	}
      else if ((val = arm_typed_reg_parse (&ptr, REG_TYPE_VFS, NULL, &optype))
	       != FAIL)
	{
	  /* Case 13.  */
	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  inst.operands[i].isvec = 1;
	  inst.operands[i].issingle = 1;
	  inst.operands[i].vectype = optype;
	  inst.operands[i].present = 1;
	}
    }
  else
    {
      first_error (_("parse error"));
      return FAIL;
    }

  /* Successfully parsed the operands. Update args.  */
  *which_operand = i;
  *str = ptr;
  return SUCCESS;

 wanted_comma:
  first_error (_("expected comma"));
  return FAIL;

 wanted_arm:
  first_error (_(reg_expected_msgs[REG_TYPE_RN]));
  return FAIL;
}

/* Use this macro when the operand constraints are different
   for ARM and THUMB (e.g. ldrd).  */
#define MIX_ARM_THUMB_OPERANDS(arm_operand, thumb_operand) \
	((arm_operand) | ((thumb_operand) << 16))

/* Matcher codes for parse_operands.  */
enum operand_parse_code
{
  OP_stop,	/* end of line */

  OP_RR,	/* ARM register */
  OP_RRnpc,	/* ARM register, not r15 */
  OP_RRnpcsp,	/* ARM register, neither r15 nor r13 (a.k.a. 'BadReg') */
  OP_RRnpcb,	/* ARM register, not r15, in square brackets */
  OP_RRnpctw,	/* ARM register, not r15 in Thumb-state or with writeback,
		   optional trailing ! */
  OP_RRw,	/* ARM register, not r15, optional trailing ! */
  OP_RCP,	/* Coprocessor number */
  OP_RCN,	/* Coprocessor register */
  OP_RF,	/* FPA register */
  OP_RVS,	/* VFP single precision register */
  OP_RVD,	/* VFP double precision register (0..15) */
  OP_RND,       /* Neon double precision register (0..31) */
  OP_RNQ,	/* Neon quad precision register */
  OP_RVSD,	/* VFP single or double precision register */
  OP_RNSD,      /* Neon single or double precision register */
  OP_RNDQ,      /* Neon double or quad precision register */
  OP_RNSDQ,	/* Neon single, double or quad precision register */
  OP_RNSC,      /* Neon scalar D[X] */
  OP_RVC,	/* VFP control register */
  OP_RMF,	/* Maverick F register */
  OP_RMD,	/* Maverick D register */
  OP_RMFX,	/* Maverick FX register */
  OP_RMDX,	/* Maverick DX register */
  OP_RMAX,	/* Maverick AX register */
  OP_RMDS,	/* Maverick DSPSC register */
  OP_RIWR,	/* iWMMXt wR register */
  OP_RIWC,	/* iWMMXt wC register */
  OP_RIWG,	/* iWMMXt wCG register */
  OP_RXA,	/* XScale accumulator register */

  OP_REGLST,	/* ARM register list */
  OP_VRSLST,	/* VFP single-precision register list */
  OP_VRDLST,	/* VFP double-precision register list */
  OP_VRSDLST,   /* VFP single or double-precision register list (& quad) */
  OP_NRDLST,    /* Neon double-precision register list (d0-d31, qN aliases) */
  OP_NSTRLST,   /* Neon element/structure list */

  OP_RNDQ_I0,   /* Neon D or Q reg, or immediate zero.  */
  OP_RVSD_I0,	/* VFP S or D reg, or immediate zero.  */
  OP_RSVD_FI0, /* VFP S or D reg, or floating point immediate zero.  */
  OP_RR_RNSC,   /* ARM reg or Neon scalar.  */
  OP_RNSD_RNSC, /* Neon S or D reg, or Neon scalar.  */
  OP_RNSDQ_RNSC, /* Vector S, D or Q reg, or Neon scalar.  */
  OP_RNDQ_RNSC, /* Neon D or Q reg, or Neon scalar.  */
  OP_RND_RNSC,  /* Neon D reg, or Neon scalar.  */
  OP_VMOV,      /* Neon VMOV operands.  */
  OP_RNDQ_Ibig,	/* Neon D or Q reg, or big immediate for logic and VMVN.  */
  OP_RNDQ_I63b, /* Neon D or Q reg, or immediate for shift.  */
  OP_RIWR_I32z, /* iWMMXt wR register, or immediate 0 .. 32 for iWMMXt2.  */

  OP_I0,        /* immediate zero */
  OP_I7,	/* immediate value 0 .. 7 */
  OP_I15,	/*		   0 .. 15 */
  OP_I16,	/*		   1 .. 16 */
  OP_I16z,      /*                 0 .. 16 */
  OP_I31,	/*		   0 .. 31 */
  OP_I31w,	/*		   0 .. 31, optional trailing ! */
  OP_I32,	/*		   1 .. 32 */
  OP_I32z,	/*		   0 .. 32 */
  OP_I63,	/*		   0 .. 63 */
  OP_I63s,	/*		 -64 .. 63 */
  OP_I64,	/*		   1 .. 64 */
  OP_I64z,	/*		   0 .. 64 */
  OP_I255,	/*		   0 .. 255 */

  OP_I4b,	/* immediate, prefix optional, 1 .. 4 */
  OP_I7b,	/*			       0 .. 7 */
  OP_I15b,	/*			       0 .. 15 */
  OP_I31b,	/*			       0 .. 31 */

  OP_SH,	/* shifter operand */
  OP_SHG,	/* shifter operand with possible group relocation */
  OP_ADDR,	/* Memory address expression (any mode) */
  OP_ADDRGLDR,	/* Mem addr expr (any mode) with possible LDR group reloc */
  OP_ADDRGLDRS, /* Mem addr expr (any mode) with possible LDRS group reloc */
  OP_ADDRGLDC,  /* Mem addr expr (any mode) with possible LDC group reloc */
  OP_EXP,	/* arbitrary expression */
  OP_EXPi,	/* same, with optional immediate prefix */
  OP_EXPr,	/* same, with optional relocation suffix */
  OP_HALF,	/* 0 .. 65535 or low/high reloc.  */
  OP_IROT1,	/* VCADD rotate immediate: 90, 270.  */
  OP_IROT2,	/* VCMLA rotate immediate: 0, 90, 180, 270.  */

  OP_CPSF,	/* CPS flags */
  OP_ENDI,	/* Endianness specifier */
  OP_wPSR,	/* CPSR/SPSR/APSR mask for msr (writing).  */
  OP_rPSR,	/* CPSR/SPSR/APSR mask for msr (reading).  */
  OP_COND,	/* conditional code */
  OP_TB,	/* Table branch.  */

  OP_APSR_RR,   /* ARM register or "APSR_nzcv".  */

  OP_RRnpc_I0,	/* ARM register or literal 0 */
  OP_RR_EXr,	/* ARM register or expression with opt. reloc stuff. */
  OP_RR_EXi,	/* ARM register or expression with imm prefix */
  OP_RF_IF,	/* FPA register or immediate */
  OP_RIWR_RIWC, /* iWMMXt R or C reg */
  OP_RIWC_RIWG, /* iWMMXt wC or wCG reg */

  /* Optional operands.	 */
  OP_oI7b,	 /* immediate, prefix optional, 0 .. 7 */
  OP_oI31b,	 /*				0 .. 31 */
  OP_oI32b,      /*                             1 .. 32 */
  OP_oI32z,      /*                             0 .. 32 */
  OP_oIffffb,	 /*				0 .. 65535 */
  OP_oI255c,	 /*	  curly-brace enclosed, 0 .. 255 */

  OP_oRR,	 /* ARM register */
  OP_oRRnpc,	 /* ARM register, not the PC */
  OP_oRRnpcsp,	 /* ARM register, neither the PC nor the SP (a.k.a. BadReg) */
  OP_oRRw,	 /* ARM register, not r15, optional trailing ! */
  OP_oRND,       /* Optional Neon double precision register */
  OP_oRNQ,       /* Optional Neon quad precision register */
  OP_oRNDQ,      /* Optional Neon double or quad precision register */
  OP_oRNSDQ,	 /* Optional single, double or quad precision vector register */
  OP_oSHll,	 /* LSL immediate */
  OP_oSHar,	 /* ASR immediate */
  OP_oSHllar,	 /* LSL or ASR immediate */
  OP_oROR,	 /* ROR 0/8/16/24 */
  OP_oBARRIER_I15, /* Option argument for a barrier instruction.  */

  /* Some pre-defined mixed (ARM/THUMB) operands.  */
  OP_RR_npcsp		= MIX_ARM_THUMB_OPERANDS (OP_RR, OP_RRnpcsp),
  OP_RRnpc_npcsp	= MIX_ARM_THUMB_OPERANDS (OP_RRnpc, OP_RRnpcsp),
  OP_oRRnpc_npcsp	= MIX_ARM_THUMB_OPERANDS (OP_oRRnpc, OP_oRRnpcsp),

  OP_FIRST_OPTIONAL = OP_oI7b
};

/* Generic instruction operand parser.	This does no encoding and no
   semantic validation; it merely squirrels values away in the inst
   structure.  Returns SUCCESS or FAIL depending on whether the
   specified grammar matched.  */
static int
parse_operands (char *str, const unsigned int *pattern, bfd_boolean thumb)
{
  unsigned const int *upat = pattern;
  char *backtrack_pos = 0;
  const char *backtrack_error = 0;
  int i, val = 0, backtrack_index = 0;
  enum arm_reg_type rtype;
  parse_operand_result result;
  unsigned int op_parse_code;

#define po_char_or_fail(chr)			\
  do						\
    {						\
      if (skip_past_char (&str, chr) == FAIL)	\
	goto bad_args;				\
    }						\
  while (0)

#define po_reg_or_fail(regtype)					\
  do								\
    {								\
      val = arm_typed_reg_parse (& str, regtype, & rtype,	\
				 & inst.operands[i].vectype);	\
      if (val == FAIL)						\
	{							\
	  first_error (_(reg_expected_msgs[regtype]));		\
	  goto failure;						\
	}							\
      inst.operands[i].reg = val;				\
      inst.operands[i].isreg = 1;				\
      inst.operands[i].isquad = (rtype == REG_TYPE_NQ);		\
      inst.operands[i].issingle = (rtype == REG_TYPE_VFS);	\
      inst.operands[i].isvec = (rtype == REG_TYPE_VFS		\
			     || rtype == REG_TYPE_VFD		\
			     || rtype == REG_TYPE_NQ);		\
    }								\
  while (0)

#define po_reg_or_goto(regtype, label)				\
  do								\
    {								\
      val = arm_typed_reg_parse (& str, regtype, & rtype,	\
				 & inst.operands[i].vectype);	\
      if (val == FAIL)						\
	goto label;						\
								\
      inst.operands[i].reg = val;				\
      inst.operands[i].isreg = 1;				\
      inst.operands[i].isquad = (rtype == REG_TYPE_NQ);		\
      inst.operands[i].issingle = (rtype == REG_TYPE_VFS);	\
      inst.operands[i].isvec = (rtype == REG_TYPE_VFS		\
			     || rtype == REG_TYPE_VFD		\
			     || rtype == REG_TYPE_NQ);		\
    }								\
  while (0)

#define po_imm_or_fail(min, max, popt)				\
  do								\
    {								\
      if (parse_immediate (&str, &val, min, max, popt) == FAIL)	\
	goto failure;						\
      inst.operands[i].imm = val;				\
    }								\
  while (0)

#define po_scalar_or_goto(elsz, label)					\
  do									\
    {									\
      val = parse_scalar (& str, elsz, & inst.operands[i].vectype);	\
      if (val == FAIL)							\
	goto label;							\
      inst.operands[i].reg = val;					\
      inst.operands[i].isscalar = 1;					\
    }									\
  while (0)

#define po_misc_or_fail(expr)			\
  do						\
    {						\
      if (expr)					\
	goto failure;				\
    }						\
  while (0)

#define po_misc_or_fail_no_backtrack(expr)		\
  do							\
    {							\
      result = expr;					\
      if (result == PARSE_OPERAND_FAIL_NO_BACKTRACK)	\
	backtrack_pos = 0;				\
      if (result != PARSE_OPERAND_SUCCESS)		\
	goto failure;					\
    }							\
  while (0)

#define po_barrier_or_imm(str)				   \
  do							   \
    {						 	   \
      val = parse_barrier (&str);			   \
      if (val == FAIL && ! ISALPHA (*str))		   \
	goto immediate;					   \
      if (val == FAIL					   \
	  /* ISB can only take SY as an option.  */	   \
	  || ((inst.instruction & 0xf0) == 0x60		   \
	       && val != 0xf))				   \
	{						   \
	   inst.error = _("invalid barrier type");	   \
	   backtrack_pos = 0;				   \
	   goto failure;				   \
	}						   \
    }							   \
  while (0)

  skip_whitespace (str);

  for (i = 0; upat[i] != OP_stop; i++)
    {
      op_parse_code = upat[i];
      if (op_parse_code >= 1<<16)
	op_parse_code = thumb ? (op_parse_code >> 16)
				: (op_parse_code & ((1<<16)-1));

      if (op_parse_code >= OP_FIRST_OPTIONAL)
	{
	  /* Remember where we are in case we need to backtrack.  */
	  gas_assert (!backtrack_pos);
	  backtrack_pos = str;
	  backtrack_error = inst.error;
	  backtrack_index = i;
	}

      if (i > 0 && (i > 1 || inst.operands[0].present))
	po_char_or_fail (',');

      switch (op_parse_code)
	{
	  /* Registers */
	case OP_oRRnpc:
	case OP_oRRnpcsp:
	case OP_RRnpc:
	case OP_RRnpcsp:
	case OP_oRR:
	case OP_RR:    po_reg_or_fail (REG_TYPE_RN);	  break;
	case OP_RCP:   po_reg_or_fail (REG_TYPE_CP);	  break;
	case OP_RCN:   po_reg_or_fail (REG_TYPE_CN);	  break;
	case OP_RF:    po_reg_or_fail (REG_TYPE_FN);	  break;
	case OP_RVS:   po_reg_or_fail (REG_TYPE_VFS);	  break;
	case OP_RVD:   po_reg_or_fail (REG_TYPE_VFD);	  break;
	case OP_oRND:
	case OP_RND:   po_reg_or_fail (REG_TYPE_VFD);	  break;
	case OP_RVC:
	  po_reg_or_goto (REG_TYPE_VFC, coproc_reg);
	  break;
	  /* Also accept generic coprocessor regs for unknown registers.  */
	  coproc_reg:
	  po_reg_or_fail (REG_TYPE_CN);
	  break;
	case OP_RMF:   po_reg_or_fail (REG_TYPE_MVF);	  break;
	case OP_RMD:   po_reg_or_fail (REG_TYPE_MVD);	  break;
	case OP_RMFX:  po_reg_or_fail (REG_TYPE_MVFX);	  break;
	case OP_RMDX:  po_reg_or_fail (REG_TYPE_MVDX);	  break;
	case OP_RMAX:  po_reg_or_fail (REG_TYPE_MVAX);	  break;
	case OP_RMDS:  po_reg_or_fail (REG_TYPE_DSPSC);	  break;
	case OP_RIWR:  po_reg_or_fail (REG_TYPE_MMXWR);	  break;
	case OP_RIWC:  po_reg_or_fail (REG_TYPE_MMXWC);	  break;
	case OP_RIWG:  po_reg_or_fail (REG_TYPE_MMXWCG);  break;
	case OP_RXA:   po_reg_or_fail (REG_TYPE_XSCALE);  break;
	case OP_oRNQ:
	case OP_RNQ:   po_reg_or_fail (REG_TYPE_NQ);      break;
	case OP_RNSD:  po_reg_or_fail (REG_TYPE_NSD);     break;
	case OP_oRNDQ:
	case OP_RNDQ:  po_reg_or_fail (REG_TYPE_NDQ);     break;
	case OP_RVSD:  po_reg_or_fail (REG_TYPE_VFSD);    break;
	case OP_oRNSDQ:
	case OP_RNSDQ: po_reg_or_fail (REG_TYPE_NSDQ);    break;

	/* Neon scalar. Using an element size of 8 means that some invalid
	   scalars are accepted here, so deal with those in later code.  */
	case OP_RNSC:  po_scalar_or_goto (8, failure);    break;

	case OP_RNDQ_I0:
	  {
	    po_reg_or_goto (REG_TYPE_NDQ, try_imm0);
	    break;
	    try_imm0:
	    po_imm_or_fail (0, 0, TRUE);
	  }
	  break;

	case OP_RVSD_I0:
	  po_reg_or_goto (REG_TYPE_VFSD, try_imm0);
	  break;

	case OP_RSVD_FI0:
	  {
	    po_reg_or_goto (REG_TYPE_VFSD, try_ifimm0);
	    break;
	    try_ifimm0:
	    if (parse_ifimm_zero (&str))
	      inst.operands[i].imm = 0;
	    else
	    {
	      inst.error
	        = _("only floating point zero is allowed as immediate value");
	      goto failure;
	    }
	  }
	  break;

	case OP_RR_RNSC:
	  {
	    po_scalar_or_goto (8, try_rr);
	    break;
	    try_rr:
	    po_reg_or_fail (REG_TYPE_RN);
	  }
	  break;

	case OP_RNSDQ_RNSC:
	  {
	    po_scalar_or_goto (8, try_nsdq);
	    break;
	    try_nsdq:
	    po_reg_or_fail (REG_TYPE_NSDQ);
	  }
	  break;

	case OP_RNSD_RNSC:
	  {
	    po_scalar_or_goto (8, try_s_scalar);
	    break;
	    try_s_scalar:
	    po_scalar_or_goto (4, try_nsd);
	    break;
	    try_nsd:
	    po_reg_or_fail (REG_TYPE_NSD);
	  }
	  break;

	case OP_RNDQ_RNSC:
	  {
	    po_scalar_or_goto (8, try_ndq);
	    break;
	    try_ndq:
	    po_reg_or_fail (REG_TYPE_NDQ);
	  }
	  break;

	case OP_RND_RNSC:
	  {
	    po_scalar_or_goto (8, try_vfd);
	    break;
	    try_vfd:
	    po_reg_or_fail (REG_TYPE_VFD);
	  }
	  break;

	case OP_VMOV:
	  /* WARNING: parse_neon_mov can move the operand counter, i. If we're
	     not careful then bad things might happen.  */
	  po_misc_or_fail (parse_neon_mov (&str, &i) == FAIL);
	  break;

	case OP_RNDQ_Ibig:
	  {
	    po_reg_or_goto (REG_TYPE_NDQ, try_immbig);
	    break;
	    try_immbig:
	    /* There's a possibility of getting a 64-bit immediate here, so
	       we need special handling.  */
	    if (parse_big_immediate (&str, i, NULL, /*allow_symbol_p=*/FALSE)
		== FAIL)
	      {
		inst.error = _("immediate value is out of range");
		goto failure;
	      }
	  }
	  break;

	case OP_RNDQ_I63b:
	  {
	    po_reg_or_goto (REG_TYPE_NDQ, try_shimm);
	    break;
	    try_shimm:
	    po_imm_or_fail (0, 63, TRUE);
	  }
	  break;

	case OP_RRnpcb:
	  po_char_or_fail ('[');
	  po_reg_or_fail  (REG_TYPE_RN);
	  po_char_or_fail (']');
	  break;

	case OP_RRnpctw:
	case OP_RRw:
	case OP_oRRw:
	  po_reg_or_fail (REG_TYPE_RN);
	  if (skip_past_char (&str, '!') == SUCCESS)
	    inst.operands[i].writeback = 1;
	  break;

	  /* Immediates */
	case OP_I7:	 po_imm_or_fail (  0,	   7, FALSE);	break;
	case OP_I15:	 po_imm_or_fail (  0,	  15, FALSE);	break;
	case OP_I16:	 po_imm_or_fail (  1,	  16, FALSE);	break;
	case OP_I16z:	 po_imm_or_fail (  0,     16, FALSE);   break;
	case OP_I31:	 po_imm_or_fail (  0,	  31, FALSE);	break;
	case OP_I32:	 po_imm_or_fail (  1,	  32, FALSE);	break;
	case OP_I32z:	 po_imm_or_fail (  0,     32, FALSE);   break;
	case OP_I63s:	 po_imm_or_fail (-64,	  63, FALSE);	break;
	case OP_I63:	 po_imm_or_fail (  0,     63, FALSE);   break;
	case OP_I64:	 po_imm_or_fail (  1,     64, FALSE);   break;
	case OP_I64z:	 po_imm_or_fail (  0,     64, FALSE);   break;
	case OP_I255:	 po_imm_or_fail (  0,	 255, FALSE);	break;

	case OP_I4b:	 po_imm_or_fail (  1,	   4, TRUE);	break;
	case OP_oI7b:
	case OP_I7b:	 po_imm_or_fail (  0,	   7, TRUE);	break;
	case OP_I15b:	 po_imm_or_fail (  0,	  15, TRUE);	break;
	case OP_oI31b:
	case OP_I31b:	 po_imm_or_fail (  0,	  31, TRUE);	break;
	case OP_oI32b:   po_imm_or_fail (  1,     32, TRUE);    break;
	case OP_oI32z:   po_imm_or_fail (  0,     32, TRUE);    break;
	case OP_oIffffb: po_imm_or_fail (  0, 0xffff, TRUE);	break;

	  /* Immediate variants */
	case OP_oI255c:
	  po_char_or_fail ('{');
	  po_imm_or_fail (0, 255, TRUE);
	  po_char_or_fail ('}');
	  break;

	case OP_I31w:
	  /* The expression parser chokes on a trailing !, so we have
	     to find it first and zap it.  */
	  {
	    char *s = str;
	    while (*s && *s != ',')
	      s++;
	    if (s[-1] == '!')
	      {
		s[-1] = '\0';
		inst.operands[i].writeback = 1;
	      }
	    po_imm_or_fail (0, 31, TRUE);
	    if (str == s - 1)
	      str = s;
	  }
	  break;

	  /* Expressions */
	case OP_EXPi:	EXPi:
	  po_misc_or_fail (my_get_expression (&inst.reloc.exp, &str,
					      GE_OPT_PREFIX));
	  break;

	case OP_EXP:
	  po_misc_or_fail (my_get_expression (&inst.reloc.exp, &str,
					      GE_NO_PREFIX));
	  break;

	case OP_EXPr:	EXPr:
	  po_misc_or_fail (my_get_expression (&inst.reloc.exp, &str,
					      GE_NO_PREFIX));
	  if (inst.reloc.exp.X_op == O_symbol)
	    {
	      val = parse_reloc (&str);
	      if (val == -1)
		{
		  inst.error = _("unrecognized relocation suffix");
		  goto failure;
		}
	      else if (val != BFD_RELOC_UNUSED)
		{
		  inst.operands[i].imm = val;
		  inst.operands[i].hasreloc = 1;
		}
	    }
	  break;

	  /* Operand for MOVW or MOVT.  */
	case OP_HALF:
	  po_misc_or_fail (parse_half (&str));
	  break;

	  /* Register or expression.  */
	case OP_RR_EXr:	  po_reg_or_goto (REG_TYPE_RN, EXPr); break;
	case OP_RR_EXi:	  po_reg_or_goto (REG_TYPE_RN, EXPi); break;

	  /* Register or immediate.  */
	case OP_RRnpc_I0: po_reg_or_goto (REG_TYPE_RN, I0);   break;
	I0:		  po_imm_or_fail (0, 0, FALSE);	      break;

	case OP_RF_IF:    po_reg_or_goto (REG_TYPE_FN, IF);   break;
	IF:
	  if (!is_immediate_prefix (*str))
	    goto bad_args;
	  str++;
	  val = parse_fpa_immediate (&str);
	  if (val == FAIL)
	    goto failure;
	  /* FPA immediates are encoded as registers 8-15.
	     parse_fpa_immediate has already applied the offset.  */
	  inst.operands[i].reg = val;
	  inst.operands[i].isreg = 1;
	  break;

	case OP_RIWR_I32z: po_reg_or_goto (REG_TYPE_MMXWR, I32z); break;
	I32z:		  po_imm_or_fail (0, 32, FALSE);	  break;

	  /* Two kinds of register.  */
	case OP_RIWR_RIWC:
	  {
	    struct reg_entry *rege = arm_reg_parse_multi (&str);
	    if (!rege
		|| (rege->type != REG_TYPE_MMXWR
		    && rege->type != REG_TYPE_MMXWC
		    && rege->type != REG_TYPE_MMXWCG))
	      {
		inst.error = _("iWMMXt data or control register expected");
		goto failure;
	      }
	    inst.operands[i].reg = rege->number;
	    inst.operands[i].isreg = (rege->type == REG_TYPE_MMXWR);
	  }
	  break;

	case OP_RIWC_RIWG:
	  {
	    struct reg_entry *rege = arm_reg_parse_multi (&str);
	    if (!rege
		|| (rege->type != REG_TYPE_MMXWC
		    && rege->type != REG_TYPE_MMXWCG))
	      {
		inst.error = _("iWMMXt control register expected");
		goto failure;
	      }
	    inst.operands[i].reg = rege->number;
	    inst.operands[i].isreg = 1;
	  }
	  break;

	  /* Misc */
	case OP_CPSF:	 val = parse_cps_flags (&str);		break;
	case OP_ENDI:	 val = parse_endian_specifier (&str);	break;
	case OP_oROR:	 val = parse_ror (&str);		break;
	case OP_COND:	 val = parse_cond (&str);		break;
	case OP_oBARRIER_I15:
	  po_barrier_or_imm (str); break;
	  immediate:
	  if (parse_immediate (&str, &val, 0, 15, TRUE) == FAIL)
	    goto failure;
	  break;

	case OP_wPSR:
	case OP_rPSR:
	  po_reg_or_goto (REG_TYPE_RNB, try_psr);

	      inst.error = _("Banked registers are not available with this "
			     "architecture.");
	      goto failure;
         try_psr:
	  val = parse_psr (&str, op_parse_code == OP_wPSR);
         break;
	case OP_APSR_RR:
	  po_reg_or_goto (REG_TYPE_RN, try_apsr);
	  break;
	  try_apsr:
	  /* Parse "APSR_nvzc" operand (for FMSTAT-equivalent MRS
	     instruction).  */
	  if (strncasecmp (str, "APSR_", 5) == 0)
	    {
	      unsigned found = 0;
	      str += 5;
	      while (found < 15)
		switch (*str++)
		  {
		  case 'c': found = (found & 1) ? 16 : found | 1; break;
		  case 'n': found = (found & 2) ? 16 : found | 2; break;
		  case 'z': found = (found & 4) ? 16 : found | 4; break;
		  case 'v': found = (found & 8) ? 16 : found | 8; break;
		  default: found = 16;
		  }
	      if (found != 15)
		goto failure;
	      inst.operands[i].isvec = 1;
	      /* APSR_nzcv is encoded in instructions as if it were the REG_PC.  */
	      inst.operands[i].reg = REG_PC;
	    }
	  else
	    goto failure;
	  break;

	case OP_TB:
	  po_misc_or_fail (parse_tb (&str));
	  break;

	  /* Register lists.  */
	case OP_REGLST:
	  val = parse_reg_list (&str);
	  if (*str == '^')
	    {
	      inst.operands[i].writeback = 1;
	      str++;
	    }
	  break;

	case OP_VRSLST:
	  val = parse_vfp_reg_list (&str, &inst.operands[i].reg, REGLIST_VFP_S);
	  break;

	case OP_VRDLST:
	  val = parse_vfp_reg_list (&str, &inst.operands[i].reg, REGLIST_VFP_D);
	  break;

	case OP_VRSDLST:
	  /* Allow Q registers too.  */
	  val = parse_vfp_reg_list (&str, &inst.operands[i].reg,
				    REGLIST_NEON_D);
	  if (val == FAIL)
	    {
	      inst.error = NULL;
	      val = parse_vfp_reg_list (&str, &inst.operands[i].reg,
					REGLIST_VFP_S);
	      inst.operands[i].issingle = 1;
	    }
	  break;

	case OP_NRDLST:
	  val = parse_vfp_reg_list (&str, &inst.operands[i].reg,
				    REGLIST_NEON_D);
	  break;

	case OP_NSTRLST:
	  val = parse_neon_el_struct_list (&str, &inst.operands[i].reg,
					   &inst.operands[i].vectype);
	  break;

	  /* Addressing modes */
	case OP_ADDR:
	  po_misc_or_fail (parse_address (&str, i));
	  break;

	case OP_ADDRGLDR:
	  po_misc_or_fail_no_backtrack (
	    parse_address_group_reloc (&str, i, GROUP_LDR));
	  break;

	case OP_ADDRGLDRS:
	  po_misc_or_fail_no_backtrack (
	    parse_address_group_reloc (&str, i, GROUP_LDRS));
	  break;

	case OP_ADDRGLDC:
	  po_misc_or_fail_no_backtrack (
	    parse_address_group_reloc (&str, i, GROUP_LDC));
	  break;

	case OP_SH:
	  po_misc_or_fail (parse_shifter_operand (&str, i));
	  break;

	case OP_SHG:
	  po_misc_or_fail_no_backtrack (
	    parse_shifter_operand_group_reloc (&str, i));
	  break;

	case OP_oSHll:
	  po_misc_or_fail (parse_shift (&str, i, SHIFT_LSL_IMMEDIATE));
	  break;

	case OP_oSHar:
	  po_misc_or_fail (parse_shift (&str, i, SHIFT_ASR_IMMEDIATE));
	  break;

	case OP_oSHllar:
	  po_misc_or_fail (parse_shift (&str, i, SHIFT_LSL_OR_ASR_IMMEDIATE));
	  break;

	default:
	  as_fatal (_("unhandled operand code %d"), op_parse_code);
	}

      /* Various value-based sanity checks and shared operations.  We
	 do not signal immediate failures for the register constraints;
	 this allows a syntax error to take precedence.	 */
      switch (op_parse_code)
	{
	case OP_oRRnpc:
	case OP_RRnpc:
	case OP_RRnpcb:
	case OP_RRw:
	case OP_oRRw:
	case OP_RRnpc_I0:
	  if (inst.operands[i].isreg && inst.operands[i].reg == REG_PC)
	    inst.error = BAD_PC;
	  break;

	case OP_oRRnpcsp:
	case OP_RRnpcsp:
	  if (inst.operands[i].isreg)
	    {
	      if (inst.operands[i].reg == REG_PC)
		inst.error = BAD_PC;
	      else if (inst.operands[i].reg == REG_SP)
		{
		  gas_assert (thumb);
		  inst.error = BAD_SP;
		}
	    }
	  break;

	case OP_RRnpctw:
	  if (inst.operands[i].isreg
	      && inst.operands[i].reg == REG_PC
	      && (inst.operands[i].writeback || thumb))
	    inst.error = BAD_PC;
	  break;

	case OP_CPSF:
	case OP_ENDI:
	case OP_oROR:
	case OP_wPSR:
	case OP_rPSR:
	case OP_COND:
	case OP_oBARRIER_I15:
	case OP_REGLST:
	case OP_VRSLST:
	case OP_VRDLST:
	case OP_VRSDLST:
	case OP_NRDLST:
	case OP_NSTRLST:
	  if (val == FAIL)
	    goto failure;
	  inst.operands[i].imm = val;
	  break;

	default:
	  break;
	}

      /* If we get here, this operand was successfully parsed.	*/
      inst.operands[i].present = 1;
      continue;

    bad_args:
      inst.error = BAD_ARGS;

    failure:
      if (!backtrack_pos)
	{
	  /* The parse routine should already have set inst.error, but set a
	     default here just in case.  */
	  if (!inst.error)
	    inst.error = _("syntax error");
	  return FAIL;
	}

      /* Do not backtrack over a trailing optional argument that
	 absorbed some text.  We will only fail again, with the
	 'garbage following instruction' error message, which is
	 probably less helpful than the current one.  */
      if (backtrack_index == i && backtrack_pos != str
	  && upat[i+1] == OP_stop)
	{
	  if (!inst.error)
	    inst.error = _("syntax error");
	  return FAIL;
	}

      /* Try again, skipping the optional argument at backtrack_pos.  */
      str = backtrack_pos;
      inst.error = backtrack_error;
      inst.operands[backtrack_index].present = 0;
      i = backtrack_index;
      backtrack_pos = 0;
    }

  /* Check that we have parsed all the arguments.  */
  if (*str != '\0' && !inst.error)
    inst.error = _("garbage following instruction");

  return inst.error ? FAIL : SUCCESS;
}

#undef po_char_or_fail
#undef po_reg_or_fail
#undef po_reg_or_goto
#undef po_imm_or_fail
#undef po_scalar_or_fail
#undef po_barrier_or_imm

/* Shorthand macro for instruction encoding functions issuing errors.  */
#define constraint(expr, err)			\
  do						\
    {						\
      if (expr)					\
	{					\
	  inst.error = err;			\
	  return;				\
	}					\
    }						\
  while (0)

/* Reject "bad registers" for Thumb-2 instructions.  Many Thumb-2
   instructions are unpredictable if these registers are used.  This
   is the BadReg predicate in ARM's Thumb-2 documentation.

   Before ARMv8-A, REG_PC and REG_SP were not allowed in quite a few
   places, while the restriction on REG_SP was relaxed since ARMv8-A.  */
#define reject_bad_reg(reg)					\
  do								\
   if (reg == REG_PC)						\
     {								\
       inst.error = BAD_PC;					\
       return;							\
     }								\
   else if (reg == REG_SP)					\
     {								\
       inst.error = BAD_SP;					\
       return;							\
     }								\
  while (0)

/* If REG is R13 (the stack pointer), warn that its use is
   deprecated.  */
#define warn_deprecated_sp(reg)			\
  do						\
    if (warn_on_deprecated && reg == REG_SP)	\
       as_tsktsk (_("use of r13 is deprecated"));	\
  while (0)

/* Functions for operand encoding.  ARM, then Thumb.  */

#define rotate_left(v, n) (v << (n & 31) | v >> ((32 - n) & 31))


/* If VAL can be encoded in the immediate field of an ARM instruction,
   return the encoded form.  Otherwise, return FAIL.  */

static unsigned int
encode_arm_immediate (unsigned int val)
{
  unsigned int a, i;

  if (val <= 0xff)
    return val;

  for (i = 2; i < 32; i += 2)
    if ((a = rotate_left (val, i)) <= 0xff)
      return a | (i << 7); /* 12-bit pack: [shift-cnt,const].  */

  return FAIL;
}

/* If VAL can be encoded in the immediate field of a Thumb32 instruction,
   return the encoded form.  Otherwise, return FAIL.  */
static unsigned int
encode_thumb32_immediate (unsigned int val)
{
  unsigned int a, i;

  if (val <= 0xff)
    return val;

  for (i = 1; i <= 24; i++)
    {
      a = val >> i;
      if ((val & ~(0xff << i)) == 0)
	return ((val >> i) & 0x7f) | ((32 - i) << 7);
    }

  a = val & 0xff;
  if (val == ((a << 16) | a))
    return 0x100 | a;
  if (val == ((a << 24) | (a << 16) | (a << 8) | a))
    return 0x300 | a;

  a = val & 0xff00;
  if (val == ((a << 16) | a))
    return 0x200 | (a >> 8);

  return FAIL;
}

/* Encode a <shift> in an ARM-format instruction.  The immediate,
   if any, is handled by md_apply_fix.	 */
static void
encode_arm_shift (int i)
{
  /* register-shifted register.  */
  if (inst.operands[i].immisreg)
    {
      int op_index;
      for (op_index = 0; op_index <= i; ++op_index)
	{
	  /* Check the operand only when it's presented.  In pre-UAL syntax,
	     if the destination register is the same as the first operand, two
	     register form of the instruction can be used.  */
	  if (inst.operands[op_index].present && inst.operands[op_index].isreg
	      && inst.operands[op_index].reg == REG_PC)
	    as_warn (UNPRED_REG ("r15"));
	}

      if (inst.operands[i].imm == REG_PC)
	as_warn (UNPRED_REG ("r15"));
    }

  if (inst.operands[i].shift_kind == SHIFT_RRX)
    inst.instruction |= SHIFT_ROR << 5;
  else
    {
      inst.instruction |= inst.operands[i].shift_kind << 5;
      if (inst.operands[i].immisreg)
	{
	  inst.instruction |= SHIFT_BY_REG;
	  inst.instruction |= inst.operands[i].imm << 8;
	}
      else
	inst.reloc.type = BFD_RELOC_ARM_SHIFT_IMM;
    }
}

static void
encode_arm_shifter_operand (int i)
{
  if (inst.operands[i].isreg)
    {
      inst.instruction |= inst.operands[i].reg;
      encode_arm_shift (i);
    }
  else
    {
      inst.instruction |= INST_IMMEDIATE;
      if (inst.reloc.type != BFD_RELOC_ARM_IMMEDIATE)
	inst.instruction |= inst.operands[i].imm;
    }
}

/* Subroutine of encode_arm_addr_mode_2 and encode_arm_addr_mode_3.  */
static void
encode_arm_addr_mode_common (int i, bfd_boolean is_t)
{
  /* PR 14260:
     Generate an error if the operand is not a register.  */
  constraint (!inst.operands[i].isreg,
	      _("Instruction does not support =N addresses"));

  inst.instruction |= inst.operands[i].reg << 16;

  if (inst.operands[i].preind)
    {
      if (is_t)
	{
	  inst.error = _("instruction does not accept preindexed addressing");
	  return;
	}
      inst.instruction |= PRE_INDEX;
      if (inst.operands[i].writeback)
	inst.instruction |= WRITE_BACK;

    }
  else if (inst.operands[i].postind)
    {
      gas_assert (inst.operands[i].writeback);
      if (is_t)
	inst.instruction |= WRITE_BACK;
    }
  else /* unindexed - only for coprocessor */
    {
      inst.error = _("instruction does not accept unindexed addressing");
      return;
    }

  if (((inst.instruction & WRITE_BACK) || !(inst.instruction & PRE_INDEX))
      && (((inst.instruction & 0x000f0000) >> 16)
	  == ((inst.instruction & 0x0000f000) >> 12)))
    as_warn ((inst.instruction & LOAD_BIT)
	     ? _("destination register same as write-back base")
	     : _("source register same as write-back base"));
}

/* inst.operands[i] was set up by parse_address.  Encode it into an
   ARM-format mode 2 load or store instruction.	 If is_t is true,
   reject forms that cannot be used with a T instruction (i.e. not
   post-indexed).  */
static void
encode_arm_addr_mode_2 (int i, bfd_boolean is_t)
{
  const bfd_boolean is_pc = (inst.operands[i].reg == REG_PC);

  encode_arm_addr_mode_common (i, is_t);

  if (inst.operands[i].immisreg)
    {
      constraint ((inst.operands[i].imm == REG_PC
		   || (is_pc && inst.operands[i].writeback)),
		  BAD_PC_ADDRESSING);
      inst.instruction |= INST_IMMEDIATE;  /* yes, this is backwards */
      inst.instruction |= inst.operands[i].imm;
      if (!inst.operands[i].negative)
	inst.instruction |= INDEX_UP;
      if (inst.operands[i].shifted)
	{
	  if (inst.operands[i].shift_kind == SHIFT_RRX)
	    inst.instruction |= SHIFT_ROR << 5;
	  else
	    {
	      inst.instruction |= inst.operands[i].shift_kind << 5;
	      inst.reloc.type = BFD_RELOC_ARM_SHIFT_IMM;
	    }
	}
    }
  else /* immediate offset in inst.reloc */
    {
      if (is_pc && !inst.reloc.pc_rel)
	{
	  /* If is_t is TRUE, it's called from do_ldstt.  ldrt/strt
	     cannot use PC in addressing.
	     PC cannot be used in writeback addressing, either.  */
	  constraint ((is_t || inst.operands[i].writeback),
		      BAD_PC_ADDRESSING);
	}

      if (inst.reloc.type == BFD_RELOC_UNUSED)
	{
	  /* Prefer + for zero encoded value.  */
	  if (!inst.operands[i].negative)
	    inst.instruction |= INDEX_UP;
	  inst.reloc.type = BFD_RELOC_ARM_OFFSET_IMM;
	}
    }
}

/* inst.operands[i] was set up by parse_address.  Encode it into an
   ARM-format mode 3 load or store instruction.	 Reject forms that
   cannot be used with such instructions.  If is_t is true, reject
   forms that cannot be used with a T instruction (i.e. not
   post-indexed).  */
static void
encode_arm_addr_mode_3 (int i, bfd_boolean is_t)
{
  if (inst.operands[i].immisreg && inst.operands[i].shifted)
    {
      inst.error = _("instruction does not accept scaled register index");
      return;
    }

  encode_arm_addr_mode_common (i, is_t);

  if (inst.operands[i].immisreg)
    {
      constraint ((inst.operands[i].imm == REG_PC
		   || (is_t && inst.operands[i].reg == REG_PC)),
		  BAD_PC_ADDRESSING);
      constraint (inst.operands[i].reg == REG_PC && inst.operands[i].writeback,
		  BAD_PC_WRITEBACK);
      inst.instruction |= inst.operands[i].imm;
      if (!inst.operands[i].negative)
	inst.instruction |= INDEX_UP;
    }
  else /* immediate offset in inst.reloc */
    {
      constraint ((inst.operands[i].reg == REG_PC && !inst.reloc.pc_rel
		   && inst.operands[i].writeback),
		  BAD_PC_WRITEBACK);
      inst.instruction |= HWOFFSET_IMM;
      if (inst.reloc.type == BFD_RELOC_UNUSED)
	{
	  /* Prefer + for zero encoded value.  */
	  if (!inst.operands[i].negative)
	    inst.instruction |= INDEX_UP;

	  inst.reloc.type = BFD_RELOC_ARM_OFFSET_IMM8;
	}
    }
}

enum lit_type
{
  CONST_THUMB,
  CONST_ARM,
  CONST_VEC
};

static void do_vfp_nsyn_opcode (const char *);

/* inst.reloc.exp describes an "=expr" load pseudo-operation.
   Determine whether it can be performed with a move instruction; if
   it can, convert inst.instruction to that move instruction and
   return TRUE; if it can't, convert inst.instruction to a literal-pool
   load and return FALSE.  If this is not a valid thing to do in the
   current context, set inst.error and return TRUE.

   inst.operands[i] describes the destination register.	 */

static bfd_boolean
move_or_literal_pool (int i, enum lit_type t, bfd_boolean mode_3)
{
  unsigned long tbit;
  bfd_boolean thumb_p = (t == CONST_THUMB);
  bfd_boolean arm_p   = (t == CONST_ARM);

  if (thumb_p)
    tbit = (inst.instruction > 0xffff) ? THUMB2_LOAD_BIT : THUMB_LOAD_BIT;
  else
    tbit = LOAD_BIT;

  if ((inst.instruction & tbit) == 0)
    {
      inst.error = _("invalid pseudo operation");
      return TRUE;
    }

  if (inst.reloc.exp.X_op != O_constant
      && inst.reloc.exp.X_op != O_symbol
      && inst.reloc.exp.X_op != O_big)
    {
      inst.error = _("constant expression expected");
      return TRUE;
    }

  if (inst.reloc.exp.X_op == O_constant
      || inst.reloc.exp.X_op == O_big)
    {
#if defined BFD_HOST_64_BIT
      bfd_int64_t v;
#else
      offsetT v;
#endif
      if (inst.reloc.exp.X_op == O_big)
	{
	  LITTLENUM_TYPE w[X_PRECISION];
	  LITTLENUM_TYPE * l;

	  if (inst.reloc.exp.X_add_number == -1)
	    {
	      gen_to_words (w, X_PRECISION, E_PRECISION);
	      l = w;
	      /* FIXME: Should we check words w[2..5] ?  */
	    }
	  else
	    l = generic_bignum;

#if defined BFD_HOST_64_BIT
	  v =
	    ((((((((bfd_int64_t) l[3] & LITTLENUM_MASK)
		  << LITTLENUM_NUMBER_OF_BITS)
		 | ((bfd_int64_t) l[2] & LITTLENUM_MASK))
		<< LITTLENUM_NUMBER_OF_BITS)
	       | ((bfd_int64_t) l[1] & LITTLENUM_MASK))
	      << LITTLENUM_NUMBER_OF_BITS)
	     | ((bfd_int64_t) l[0] & LITTLENUM_MASK));
#else
	  v = ((l[1] & LITTLENUM_MASK) << LITTLENUM_NUMBER_OF_BITS)
	    |  (l[0] & LITTLENUM_MASK);
#endif
	}
      else
	v = inst.reloc.exp.X_add_number;

      if (!inst.operands[i].issingle)
	{
	  if (thumb_p)
	    {
	      /* LDR should not use lead in a flag-setting instruction being
		 chosen so we do not check whether movs can be used.  */





	    }
	  else if (arm_p)
	    {
	      int value = encode_arm_immediate (v);

	      if (value != FAIL)
		{
		  /* This can be done with a mov instruction.  */
		  inst.instruction &= LITERAL_MASK;
		  inst.instruction |= INST_IMMEDIATE | (OPCODE_MOV << DATA_OP_SHIFT);
		  inst.instruction |= value & 0xfff;
		  return TRUE;
		}

	      value = encode_arm_immediate (~ v);
	      if (value != FAIL)
		{
		  /* This can be done with a mvn instruction.  */
		  inst.instruction &= LITERAL_MASK;
		  inst.instruction |= INST_IMMEDIATE | (OPCODE_MVN << DATA_OP_SHIFT);
		  inst.instruction |= value & 0xfff;
		  return TRUE;
		}
	    }
	}

      if (t == CONST_VEC)
	{
	}
    }
  if (add_to_lit_pool ((!inst.operands[i].isvec
      || inst.operands[i].issingle) ? 4 : 8) == FAIL)
    return TRUE;

  inst.operands[1].reg = REG_PC;
  inst.operands[1].isreg = 1;
  inst.operands[1].preind = 1;
  inst.reloc.pc_rel = 1;
  inst.reloc.type = (thumb_p
		     ? BFD_RELOC_ARM_THUMB_OFFSET
		     : (mode_3
			? BFD_RELOC_ARM_HWLITERAL
			: BFD_RELOC_ARM_LITERAL));
  return FALSE;
}

/* inst.operands[i] was set up by parse_address.  Encode it into an
   ARM-format instruction.  Reject all forms which cannot be encoded
   into a coprocessor load/store instruction.  If wb_ok is false,
   reject use of writeback; if unind_ok is false, reject use of
   unindexed addressing.  If reloc_override is not 0, use it instead
   of BFD_ARM_CP_OFF_IMM, unless the initial relocation is a group one
   (in which case it is preserved).  */

static int
encode_arm_cp_address (int i, int wb_ok, int unind_ok, int reloc_override)
{
  if (!inst.operands[i].isreg)
    {
      /* PR 18256 */
      if (! inst.operands[0].isvec)
	{
	  inst.error = _("invalid co-processor operand");
	  return FAIL;
	}
      if (move_or_literal_pool (0, CONST_VEC, /*mode_3=*/FALSE))
	return SUCCESS;
    }

  inst.instruction |= inst.operands[i].reg << 16;

  gas_assert (!(inst.operands[i].preind && inst.operands[i].postind));

  if (!inst.operands[i].preind && !inst.operands[i].postind) /* unindexed */
    {
      gas_assert (!inst.operands[i].writeback);
      if (!unind_ok)
	{
	  inst.error = _("instruction does not support unindexed addressing");
	  return FAIL;
	}
      inst.instruction |= inst.operands[i].imm;
      inst.instruction |= INDEX_UP;
      return SUCCESS;
    }

  if (inst.operands[i].preind)
    inst.instruction |= PRE_INDEX;

  if (inst.operands[i].writeback)
    {
      if (inst.operands[i].reg == REG_PC)
	{
	  inst.error = _("pc may not be used with write-back");
	  return FAIL;
	}
      if (!wb_ok)
	{
	  inst.error = _("instruction does not support writeback");
	  return FAIL;
	}
      inst.instruction |= WRITE_BACK;
    }

  if (reloc_override)
    inst.reloc.type = (bfd_reloc_code_real_type) reloc_override;
  else if ((inst.reloc.type < BFD_RELOC_ARM_ALU_PC_G0_NC
	    || inst.reloc.type > BFD_RELOC_ARM_LDC_SB_G2)
	   && inst.reloc.type != BFD_RELOC_ARM_LDR_PC_G0)
    {
      if (thumb_mode)
	inst.reloc.type = BFD_RELOC_ARM_T32_CP_OFF_IMM;
      else
	inst.reloc.type = BFD_RELOC_ARM_CP_OFF_IMM;
    }

  /* Prefer + for zero encoded value.  */
  if (!inst.operands[i].negative)
    inst.instruction |= INDEX_UP;

  return SUCCESS;
}

/* Functions for instruction encoding, sorted by sub-architecture.
   First some generics; their names are taken from the conventional
   bit positions for register arguments in ARM format instructions.  */

static void
do_rd_rn (void)
{
  inst.instruction |= inst.operands[0].reg << 12;
  inst.instruction |= inst.operands[1].reg << 16;
}

static void
do_rd_rm_rn (void)
{
  unsigned Rn = inst.operands[2].reg;
  /* Enforce restrictions on SWP instruction.  */
  if ((inst.instruction & 0x0fbfffff) == 0x01000090)
    {
      constraint (Rn == inst.operands[0].reg || Rn == inst.operands[1].reg,
		  _("Rn must not overlap other operands"));

      /* SWP{b} is obsolete for ARMv8-A, and deprecated for ARMv6* and ARMv7.
       */
    }

  inst.instruction |= inst.operands[0].reg << 12;
  inst.instruction |= inst.operands[1].reg;
  inst.instruction |= Rn << 16;
}

/* ARM instructions, in alphabetical order by function name (except
   that wrapper functions appear immediately after the function they
   wrap).  */

/* This is a pseudo-op of the form "adr rd, label" to be converted
   into a relative address of the form "add rd, pc, #label-.-8".  */

static void
do_adr (void)
{
  inst.instruction |= (inst.operands[0].reg << 12);  /* Rd */

  /* Frag hacking will turn this into a sub instruction if the offset turns
     out to be negative.  */
  inst.reloc.type = BFD_RELOC_ARM_IMMEDIATE;
  inst.reloc.pc_rel = 1;
  inst.reloc.exp.X_add_number -= 8;

  if (support_interwork
      && inst.reloc.exp.X_op == O_symbol
      && inst.reloc.exp.X_add_symbol != NULL
      && S_IS_DEFINED (inst.reloc.exp.X_add_symbol)
      && THUMB_IS_FUNC (inst.reloc.exp.X_add_symbol))
    inst.reloc.exp.X_add_number |= 1;
}

/* This is a pseudo-op of the form "adrl rd, label" to be converted
   into a relative address of the form:
   add rd, pc, #low(label-.-8)"
   add rd, rd, #high(label-.-8)"  */

static void
do_adrl (void)
{
  inst.instruction |= (inst.operands[0].reg << 12);  /* Rd */

  /* Frag hacking will turn this into a sub instruction if the offset turns
     out to be negative.  */
  inst.reloc.type	       = BFD_RELOC_ARM_ADRL_IMMEDIATE;
  inst.reloc.pc_rel	       = 1;
  inst.size		       = INSN_SIZE * 2;
  inst.reloc.exp.X_add_number -= 8;

  if (support_interwork
      && inst.reloc.exp.X_op == O_symbol
      && inst.reloc.exp.X_add_symbol != NULL
      && S_IS_DEFINED (inst.reloc.exp.X_add_symbol)
      && THUMB_IS_FUNC (inst.reloc.exp.X_add_symbol))
    inst.reloc.exp.X_add_number |= 1;
}

static void
do_arit (void)
{
  constraint (inst.reloc.type >= BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
	      && inst.reloc.type <= BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC ,
	      THUMB1_RELOC_ONLY);
  if (!inst.operands[1].present)
    inst.operands[1].reg = inst.operands[0].reg;
  inst.instruction |= inst.operands[0].reg << 12;
  inst.instruction |= inst.operands[1].reg << 16;
  encode_arm_shifter_operand (2);
}

/* ARM V5 breakpoint instruction (argument parse)
     BKPT <16 bit unsigned immediate>
     Instruction is not conditional.
	The bit pattern given in insns[] has the COND_ALWAYS condition,
	and it is an error if the caller tried to override that.  */

static void
do_bkpt (void)
{
  /* Top 12 of 16 bits to bits 19:8.  */
  inst.instruction |= (inst.operands[0].imm & 0xfff0) << 4;

  /* Bottom 4 of 16 bits to bits 3:0.  */
  inst.instruction |= inst.operands[0].imm & 0xf;
}

static void
encode_branch (int default_reloc)
{
  if (inst.operands[0].hasreloc)
    {
      constraint (inst.operands[0].imm != BFD_RELOC_ARM_PLT32
		  && inst.operands[0].imm != BFD_RELOC_ARM_TLS_CALL,
		  _("the only valid suffixes here are '(plt)' and '(tlscall)'"));
      inst.reloc.type = inst.operands[0].imm == BFD_RELOC_ARM_PLT32
	? BFD_RELOC_ARM_PLT32
	: thumb_mode ? BFD_RELOC_ARM_THM_TLS_CALL : BFD_RELOC_ARM_TLS_CALL;
    }
  else
    inst.reloc.type = (bfd_reloc_code_real_type) default_reloc;
  inst.reloc.pc_rel = 1;
}

static void
do_branch (void)
{
  if (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4)
    encode_branch (BFD_RELOC_ARM_PCREL_JUMP);
  else
    encode_branch (BFD_RELOC_ARM_PCREL_BRANCH);
}

static void
do_bl (void)
{
  if (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4)
    {
      if (inst.cond == COND_ALWAYS)
	encode_branch (BFD_RELOC_ARM_PCREL_CALL);
      else
	encode_branch (BFD_RELOC_ARM_PCREL_JUMP);
    }
  else
    encode_branch (BFD_RELOC_ARM_PCREL_BRANCH);
}

static void
do_bx (void)
{
  bfd_boolean want_reloc;

  if (inst.operands[0].reg == REG_PC)
    as_tsktsk (_("use of r15 in bx in ARM mode is not really useful"));

  inst.instruction |= inst.operands[0].reg;
  /* Output R_ARM_V4BX relocations if is an EABI object that looks like
     it is for ARMv4t or earlier.  */
  want_reloc = TRUE;
  if (!ARM_FEATURE_ZERO (selected_object_arch))
    want_reloc = TRUE;
  if (EF_ARM_EABI_VERSION (meabi_flags) < EF_ARM_EABI_VER4)
    want_reloc = FALSE;

  if (want_reloc)
    inst.reloc.type = BFD_RELOC_ARM_V4BX;
}

/* Co-processor data operation:
      CDP{cond} <coproc>, <opcode_1>, <CRd>, <CRn>, <CRm>{, <opcode_2>}
      CDP2	<coproc>, <opcode_1>, <CRd>, <CRn>, <CRm>{, <opcode_2>}	 */
static void
do_cdp (void)
{
  inst.instruction |= inst.operands[0].reg << 8;
  inst.instruction |= inst.operands[1].imm << 20;
  inst.instruction |= inst.operands[2].reg << 12;
  inst.instruction |= inst.operands[3].reg << 16;
  inst.instruction |= inst.operands[4].reg;
  inst.instruction |= inst.operands[5].imm << 5;
}

static void
do_cmp (void)
{
  inst.instruction |= inst.operands[0].reg << 16;
  encode_arm_shifter_operand (1);
}

static void
do_co_reg (void)
{
  unsigned Rd;

  Rd = inst.operands[2].reg;
  if (thumb_mode)
    {
      if (inst.instruction == 0xee000010
	  || inst.instruction == 0xfe000010)
	/* MCR, MCR2  */
	reject_bad_reg (Rd);
    }
  else
    {
      /* MCR */
      if (inst.instruction == 0xe000010)
	constraint (Rd == REG_PC, BAD_PC);
    }

  inst.instruction |= inst.operands[0].reg << 8;
  inst.instruction |= inst.operands[1].imm << 21;
  inst.instruction |= Rd << 12;
  inst.instruction |= inst.operands[3].reg << 16;
  inst.instruction |= inst.operands[4].reg;
  inst.instruction |= inst.operands[5].imm << 5;
}

/* If there is only one register in the register list,
   then return its register number.  Otherwise return -1.  */
static int
only_one_reg_in_list (int range)
{
  int i = ffs (range) - 1;
  return (i > 15 || range != (1 << i)) ? -1 : i;
}

static void
encode_ldmstm(int from_push_pop_mnem)
{
  int base_reg = inst.operands[0].reg;
  int range = inst.operands[1].imm;
  int one_reg;

  inst.instruction |= base_reg << 16;
  inst.instruction |= range;

  if (inst.operands[1].writeback)
    inst.instruction |= LDM_TYPE_2_OR_3;

  if (inst.operands[0].writeback)
    {
      inst.instruction |= WRITE_BACK;
      /* Check for unpredictable uses of writeback.  */
      if (inst.instruction & LOAD_BIT)
	{
	  /* Not allowed in LDM type 2.	 */
	  if ((inst.instruction & LDM_TYPE_2_OR_3)
	      && ((range & (1 << REG_PC)) == 0))
	    as_warn (_("writeback of base register is UNPREDICTABLE"));
	  /* Only allowed if base reg not in list for other types.  */
	  else if (range & (1 << base_reg))
	    as_warn (_("writeback of base register when in register list is UNPREDICTABLE"));
	}
      else /* STM.  */
	{
	  /* Not allowed for type 2.  */
	  if (inst.instruction & LDM_TYPE_2_OR_3)
	    as_warn (_("writeback of base register is UNPREDICTABLE"));
	  /* Only allowed if base reg not in list, or first in list.  */
	  else if ((range & (1 << base_reg))
		   && (range & ((1 << base_reg) - 1)))
	    as_warn (_("if writeback register is in list, it must be the lowest reg in the list"));
	}
    }

  /* If PUSH/POP has only one register, then use the A2 encoding.  */
  one_reg = only_one_reg_in_list (range);
  if (from_push_pop_mnem && one_reg >= 0)
    {
      int is_push = (inst.instruction & A_PUSH_POP_OP_MASK) == A1_OPCODE_PUSH;

      if (is_push && one_reg == 13 /* SP */)
	/* PR 22483: The A2 encoding cannot be used when
	   pushing the stack pointer as this is UNPREDICTABLE.  */
	return;

      inst.instruction &= A_COND_MASK;
      inst.instruction |= is_push ? A2_OPCODE_PUSH : A2_OPCODE_POP;
      inst.instruction |= one_reg << 12;
    }
}

static void
do_ldmstm (void)
{
  encode_ldmstm (/*from_push_pop_mnem=*/FALSE);
}

/* In both ARM and thumb state 'ldr pc, #imm'  with an immediate
   which is not a multiple of four is UNPREDICTABLE.  */
static void
check_ldr_r15_aligned (void)
{
  constraint (!(inst.operands[1].immisreg)
	      && (inst.operands[0].reg == REG_PC
	      && inst.operands[1].reg == REG_PC
	      && (inst.reloc.exp.X_add_number & 0x3)),
	      _("ldr to register 15 must be 4-byte aligned"));
}

static void
do_ldst (void)
{
  inst.instruction |= inst.operands[0].reg << 12;
  if (!inst.operands[1].isreg)
    if (move_or_literal_pool (0, CONST_ARM, /*mode_3=*/FALSE))
      return;
  encode_arm_addr_mode_2 (1, /*is_t=*/FALSE);
  check_ldr_r15_aligned ();
}

static void
do_ldstt (void)
{
  /* ldrt/strt always use post-indexed addressing.  Turn [Rn] into [Rn]! and
     reject [Rn,...].  */
  if (inst.operands[1].preind)
    {
      constraint (inst.reloc.exp.X_op != O_constant
		  || inst.reloc.exp.X_add_number != 0,
		  _("this instruction requires a post-indexed address"));

      inst.operands[1].preind = 0;
      inst.operands[1].postind = 1;
      inst.operands[1].writeback = 1;
    }
  inst.instruction |= inst.operands[0].reg << 12;
  encode_arm_addr_mode_2 (1, /*is_t=*/TRUE);
}

/* Halfword and signed-byte load/store operations.  */

static void
do_ldstv4 (void)
{
  constraint (inst.operands[0].reg == REG_PC, BAD_PC);
  inst.instruction |= inst.operands[0].reg << 12;
  if (!inst.operands[1].isreg)
    if (move_or_literal_pool (0, CONST_ARM, /*mode_3=*/TRUE))
      return;
  encode_arm_addr_mode_3 (1, /*is_t=*/FALSE);
}

/* Co-processor register load/store.
   Format: <LDC|STC>{cond}[L] CP#,CRd,<address>	 */
static void
do_lstc (void)
{
  inst.instruction |= inst.operands[0].reg << 8;
  inst.instruction |= inst.operands[1].reg << 12;
  encode_arm_cp_address (2, TRUE, TRUE, 0);
}

static void
do_mlas (void)
{
  /* This restriction does not apply to mls (nor to mla in v6 or later).  */
  if (inst.operands[0].reg == inst.operands[1].reg
      && !(inst.instruction & 0x00400000))
    as_tsktsk (_("Rd and Rm should be different in mla"));

  inst.instruction |= inst.operands[0].reg << 16;
  inst.instruction |= inst.operands[1].reg;
  inst.instruction |= inst.operands[2].reg << 8;
  inst.instruction |= inst.operands[3].reg << 12;
}

static void
do_mov (void)
{
  constraint (inst.reloc.type >= BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
	      && inst.reloc.type <= BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC ,
	      THUMB1_RELOC_ONLY);
  inst.instruction |= inst.operands[0].reg << 12;
  encode_arm_shifter_operand (1);
}

static int
do_vfp_nsyn_mrs (void)
{
  if (inst.operands[0].isvec)
    {
      if (inst.operands[1].reg != 1)
	first_error (_("operand 1 must be FPSCR"));
      memset (&inst.operands[0], '\0', sizeof (inst.operands[0]));
      memset (&inst.operands[1], '\0', sizeof (inst.operands[1]));
      do_vfp_nsyn_opcode ("fmstat");
    }
  else if (inst.operands[1].isvec)
    do_vfp_nsyn_opcode ("fmrx");
  else
    return FAIL;

  return SUCCESS;
}

static int
do_vfp_nsyn_msr (void)
{
  if (inst.operands[0].isvec)
    do_vfp_nsyn_opcode ("fmxr");
  else
    return FAIL;

  return SUCCESS;
}

static void
do_mrs (void)
{
  unsigned br;

  if (do_vfp_nsyn_mrs () == SUCCESS)
    return;

  constraint (inst.operands[0].reg == REG_PC, BAD_PC);
  inst.instruction |= inst.operands[0].reg << 12;

  if (inst.operands[1].isreg)
    {
      br = inst.operands[1].reg;
      if (((br & 0x200) == 0) && ((br & 0xf0000) != 0xf0000))
	as_bad (_("bad register for mrs"));
    }
  else
    {
      /* mrs only accepts CPSR/SPSR/CPSR_all/SPSR_all.  */
      constraint ((inst.operands[1].imm & (PSR_c|PSR_x|PSR_s|PSR_f))
		  != (PSR_c|PSR_f),
		  _("'APSR', 'CPSR' or 'SPSR' expected"));
      br = (15<<16) | (inst.operands[1].imm & SPSR_BIT);
    }

  inst.instruction |= br;
}

/* Two possible forms:
      "{C|S}PSR_<field>, Rm",
      "{C|S}PSR_f, #expression".  */

static void
do_msr (void)
{
  if (do_vfp_nsyn_msr () == SUCCESS)
    return;

  inst.instruction |= inst.operands[0].imm;
  if (inst.operands[1].isreg)
    inst.instruction |= inst.operands[1].reg;
  else
    {
      inst.instruction |= INST_IMMEDIATE;
      inst.reloc.type = BFD_RELOC_ARM_IMMEDIATE;
      inst.reloc.pc_rel = 0;
    }
}

static void
do_mul (void)
{
  constraint (inst.operands[2].reg == REG_PC, BAD_PC);

  if (!inst.operands[2].present)
    inst.operands[2].reg = inst.operands[0].reg;
  inst.instruction |= inst.operands[0].reg << 16;
  inst.instruction |= inst.operands[1].reg;
  inst.instruction |= inst.operands[2].reg << 8;

  if (inst.operands[0].reg == inst.operands[1].reg)
    as_tsktsk (_("Rd and Rm should be different in mul"));
}

/* Long Multiply Parser
   UMULL RdLo, RdHi, Rm, Rs
   SMULL RdLo, RdHi, Rm, Rs
   UMLAL RdLo, RdHi, Rm, Rs
   SMLAL RdLo, RdHi, Rm, Rs.  */

static void
do_mull (void)
{
  inst.instruction |= inst.operands[0].reg << 12;
  inst.instruction |= inst.operands[1].reg << 16;
  inst.instruction |= inst.operands[2].reg;
  inst.instruction |= inst.operands[3].reg << 8;

  /* rdhi and rdlo must be different.  */
  if (inst.operands[0].reg == inst.operands[1].reg)
    as_tsktsk (_("rdhi and rdlo must be different"));

  /* rdhi, rdlo and rm must all be different before armv6.  */
  if ((inst.operands[0].reg == inst.operands[2].reg
      || inst.operands[1].reg == inst.operands[2].reg))
    as_tsktsk (_("rdhi, rdlo and rm must all be different"));
}

static void
do_nop (void)
{
  if (inst.operands[0].present)
    {
      /* Architectural NOP hints are CPSR sets with no bits selected.  */
      inst.instruction &= 0xf0000000;
      inst.instruction |= 0x0320f000;
      if (inst.operands[0].present)
	inst.instruction |= inst.operands[0].imm;
    }
}

static void
do_push_pop (void)
{
  constraint (inst.operands[0].writeback,
	      _("push/pop do not support {reglist}^"));
  inst.operands[1] = inst.operands[0];
  memset (&inst.operands[0], 0, sizeof inst.operands[0]);
  inst.operands[0].isreg = 1;
  inst.operands[0].writeback = 1;
  inst.operands[0].reg = REG_SP;
  encode_ldmstm (/*from_push_pop_mnem=*/TRUE);
}

static void
do_shift (void)
{
  unsigned int Rm = (inst.operands[1].present
		     ? inst.operands[1].reg
		     : inst.operands[0].reg);

  inst.instruction |= inst.operands[0].reg << 12;
  inst.instruction |= Rm;
  if (inst.operands[2].isreg)  /* Rd, {Rm,} Rs */
    {
      inst.instruction |= inst.operands[2].reg << 8;
      inst.instruction |= SHIFT_BY_REG;
      /* PR 12854: Error on extraneous shifts.  */
      constraint (inst.operands[2].shifted,
		  _("extraneous shift as part of operand to shift insn"));
    }
  else
    inst.reloc.type = BFD_RELOC_ARM_SHIFT_IMM;
}

static void
do_swi (void)
{
  inst.reloc.type = BFD_RELOC_ARM_SWI;
  inst.reloc.pc_rel = 0;
}


/* Encoding functions relevant only to Thumb.  */

/* inst.operands[i] is a shifted-register operand; encode
   it into inst.instruction in the format used by Thumb32.  */

static void
encode_thumb32_shifted_operand (int i)
{
  unsigned int value = inst.reloc.exp.X_add_number;
  unsigned int shift = inst.operands[i].shift_kind;

  constraint (inst.operands[i].immisreg,
	      _("shift by register not allowed in thumb mode"));
  inst.instruction |= inst.operands[i].reg;
  if (shift == SHIFT_RRX)
    inst.instruction |= SHIFT_ROR << 4;
  else
    {
      constraint (inst.reloc.exp.X_op != O_constant,
		  _("expression too complex"));

      constraint (value > 32
		  || (value == 32 && (shift == SHIFT_LSL
				      || shift == SHIFT_ROR)),
		  _("shift expression is too large"));

      if (value == 0)
	shift = SHIFT_LSL;
      else if (value == 32)
	value = 0;

      inst.instruction |= shift << 4;
      inst.instruction |= (value & 0x1c) << 10;
      inst.instruction |= (value & 0x03) << 6;
    }
}


/* inst.operands[i] was set up by parse_address.  Encode it into a
   Thumb32 format load or store instruction.  Reject forms that cannot
   be used with such instructions.  If is_t is true, reject forms that
   cannot be used with a T instruction; if is_d is true, reject forms
   that cannot be used with a D instruction.  If it is a store insn,
   reject PC in Rn.  */

static void
encode_thumb32_addr_mode (int i, bfd_boolean is_t, bfd_boolean is_d)
{
  const bfd_boolean is_pc = (inst.operands[i].reg == REG_PC);

  constraint (!inst.operands[i].isreg,
	      _("Instruction does not support =N addresses"));

  inst.instruction |= inst.operands[i].reg << 16;
  if (inst.operands[i].immisreg)
    {
      constraint (is_pc, BAD_PC_ADDRESSING);
      constraint (is_t || is_d, _("cannot use register index with this instruction"));
      constraint (inst.operands[i].negative,
		  _("Thumb does not support negative register indexing"));
      constraint (inst.operands[i].postind,
		  _("Thumb does not support register post-indexing"));
      constraint (inst.operands[i].writeback,
		  _("Thumb does not support register indexing with writeback"));
      constraint (inst.operands[i].shifted && inst.operands[i].shift_kind != SHIFT_LSL,
		  _("Thumb supports only LSL in shifted register indexing"));

      inst.instruction |= inst.operands[i].imm;
      if (inst.operands[i].shifted)
	{
	  constraint (inst.reloc.exp.X_op != O_constant,
		      _("expression too complex"));
	  constraint (inst.reloc.exp.X_add_number < 0
		      || inst.reloc.exp.X_add_number > 3,
		      _("shift out of range"));
	  inst.instruction |= inst.reloc.exp.X_add_number << 4;
	}
      inst.reloc.type = BFD_RELOC_UNUSED;
    }
  else if (inst.operands[i].preind)
    {
      constraint (is_pc && inst.operands[i].writeback, BAD_PC_WRITEBACK);
      constraint (is_t && inst.operands[i].writeback,
		  _("cannot use writeback with this instruction"));
      constraint (is_pc && ((inst.instruction & THUMB2_LOAD_BIT) == 0),
		  BAD_PC_ADDRESSING);

      if (is_d)
	{
	  inst.instruction |= 0x01000000;
	  if (inst.operands[i].writeback)
	    inst.instruction |= 0x00200000;
	}
      else
	{
	  inst.instruction |= 0x00000c00;
	  if (inst.operands[i].writeback)
	    inst.instruction |= 0x00000100;
	}
      inst.reloc.type = BFD_RELOC_ARM_T32_OFFSET_IMM;
    }
  else if (inst.operands[i].postind)
    {
      gas_assert (inst.operands[i].writeback);
      constraint (is_pc, _("cannot use post-indexing with PC-relative addressing"));
      constraint (is_t, _("cannot use post-indexing with this instruction"));

      if (is_d)
	inst.instruction |= 0x00200000;
      else
	inst.instruction |= 0x00000900;
      inst.reloc.type = BFD_RELOC_ARM_T32_OFFSET_IMM;
    }
  else /* unindexed - only for coprocessor */
    inst.error = _("instruction does not accept unindexed addressing");
}

/* Table of Thumb instructions which exist in both 16- and 32-bit
   encodings (the latter only in post-V6T2 cores).  The index is the
   value used in the insns table below.  When there is more than one
   possible 16-bit encoding for the instruction, this table always
   holds variant (1).
   Also contains several pseudo-instructions used during relaxation.  */
#define T16_32_TAB				\
  X(_adc,   4140, eb400000),			\
  X(_adcs,  4140, eb500000),			\
  X(_add,   1c00, eb000000),			\
  X(_adds,  1c00, eb100000),			\
  X(_addi,  0000, f1000000),			\
  X(_addis, 0000, f1100000),			\
  X(_add_pc,000f, f20f0000),			\
  X(_add_sp,000d, f10d0000),			\
  X(_adr,   000f, f20f0000),			\
  X(_and,   4000, ea000000),			\
  X(_ands,  4000, ea100000),			\
  X(_asr,   1000, fa40f000),			\
  X(_asrs,  1000, fa50f000),			\
  X(_b,     e000, f000b000),			\
  X(_bcond, d000, f0008000),			\
  X(_bic,   4380, ea200000),			\
  X(_bics,  4380, ea300000),			\
  X(_cmn,   42c0, eb100f00),			\
  X(_cmp,   2800, ebb00f00),			\
  X(_cpsie, b660, f3af8400),			\
  X(_cpsid, b670, f3af8600),			\
  X(_cpy,   4600, ea4f0000),			\
  X(_dec_sp,80dd, f1ad0d00),			\
  X(_eor,   4040, ea800000),			\
  X(_eors,  4040, ea900000),			\
  X(_inc_sp,00dd, f10d0d00),			\
  X(_ldmia, c800, e8900000),			\
  X(_ldr,   6800, f8500000),			\
  X(_ldrb,  7800, f8100000),			\
  X(_ldrh,  8800, f8300000),			\
  X(_ldrsb, 5600, f9100000),			\
  X(_ldrsh, 5e00, f9300000),			\
  X(_ldr_pc,4800, f85f0000),			\
  X(_ldr_pc2,4800, f85f0000),			\
  X(_ldr_sp,9800, f85d0000),			\
  X(_lsl,   0000, fa00f000),			\
  X(_lsls,  0000, fa10f000),			\
  X(_lsr,   0800, fa20f000),			\
  X(_lsrs,  0800, fa30f000),			\
  X(_mov,   2000, ea4f0000),			\
  X(_movs,  2000, ea5f0000),			\
  X(_mul,   4340, fb00f000),                     \
  X(_muls,  4340, ffffffff), /* no 32b muls */	\
  X(_mvn,   43c0, ea6f0000),			\
  X(_mvns,  43c0, ea7f0000),			\
  X(_neg,   4240, f1c00000), /* rsb #0 */	\
  X(_negs,  4240, f1d00000), /* rsbs #0 */	\
  X(_orr,   4300, ea400000),			\
  X(_orrs,  4300, ea500000),			\
  X(_pop,   bc00, e8bd0000), /* ldmia sp!,... */	\
  X(_push,  b400, e92d0000), /* stmdb sp!,... */	\
  X(_rev,   ba00, fa90f080),			\
  X(_rev16, ba40, fa90f090),			\
  X(_revsh, bac0, fa90f0b0),			\
  X(_ror,   41c0, fa60f000),			\
  X(_rors,  41c0, fa70f000),			\
  X(_sbc,   4180, eb600000),			\
  X(_sbcs,  4180, eb700000),			\
  X(_stmia, c000, e8800000),			\
  X(_str,   6000, f8400000),			\
  X(_strb,  7000, f8000000),			\
  X(_strh,  8000, f8200000),			\
  X(_str_sp,9000, f84d0000),			\
  X(_sub,   1e00, eba00000),			\
  X(_subs,  1e00, ebb00000),			\
  X(_subi,  8000, f1a00000),			\
  X(_subis, 8000, f1b00000),			\
  X(_sxtb,  b240, fa4ff080),			\
  X(_sxth,  b200, fa0ff080),			\
  X(_tst,   4200, ea100f00),			\
  X(_uxtb,  b2c0, fa5ff080),			\
  X(_uxth,  b280, fa1ff080),			\
  X(_nop,   bf00, f3af8000),			\
  X(_yield, bf10, f3af8001),			\
  X(_wfe,   bf20, f3af8002),			\
  X(_wfi,   bf30, f3af8003),			\
  X(_sev,   bf40, f3af8004),                    \
  X(_sevl,  bf50, f3af8005),			\
  X(_udf,   de00, f7f0a000)

/* To catch errors in encoding functions, the codes are all offset by
   0xF800, putting them in one of the 32-bit prefix ranges, ergo undefined
   as 16-bit instructions.  */
#define X(a,b,c) T_MNEM##a
enum t16_32_codes { T16_32_OFFSET = 0xF7FF, T16_32_TAB };
#undef X

#define X(a,b,c) 0x##b
static const unsigned short thumb_op16[] = { T16_32_TAB };
#define THUMB_OP16(n) (thumb_op16[(n) - (T16_32_OFFSET + 1)])
#undef X

#define X(a,b,c) 0x##c
static const unsigned int thumb_op32[] = { T16_32_TAB };
#define THUMB_OP32(n)        (thumb_op32[(n) - (T16_32_OFFSET + 1)])
#define THUMB_SETS_FLAGS(n)  (THUMB_OP32 (n) & 0x00100000)
#undef X
#undef T16_32_TAB

/* Thumb instruction encoders, in alphabetical order.  */

/* Parse an add or subtract instruction.  We get here with inst.instruction
   equaling any of THUMB_OPCODE_add, adds, sub, or subs.  */

static void
do_t_add_sub (void)
{
  int Rd, Rs, Rn;

  Rd = inst.operands[0].reg;
  Rs = (inst.operands[1].present
	? inst.operands[1].reg    /* Rd, Rs, foo */
	: inst.operands[0].reg);  /* Rd, foo -> Rd, Rd, foo */

  if (Rd == REG_PC)
    set_it_insn_type_last ();

  if (unified_syntax)
    {
      bfd_boolean flags;
      bfd_boolean narrow;
      int opcode;

      flags = (inst.instruction == T_MNEM_adds
	       || inst.instruction == T_MNEM_subs);
      if (flags)
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();
      if (!inst.operands[2].isreg)
	{
	  int add;

	    constraint (Rd == REG_SP && Rs != REG_SP, BAD_SP);

	  add = (inst.instruction == T_MNEM_add
		 || inst.instruction == T_MNEM_adds);
	  opcode = 0;
	  if (inst.size_req != 4)
	    {
	      /* Attempt to use a narrow opcode, with relaxation if
		 appropriate.  */
	      if (Rd == REG_SP && Rs == REG_SP && !flags)
		opcode = add ? T_MNEM_inc_sp : T_MNEM_dec_sp;
	      else if (Rd <= 7 && Rs == REG_SP && add && !flags)
		opcode = T_MNEM_add_sp;
	      else if (Rd <= 7 && Rs == REG_PC && add && !flags)
		opcode = T_MNEM_add_pc;
	      else if (Rd <= 7 && Rs <= 7 && narrow)
		{
		  if (flags)
		    opcode = add ? T_MNEM_addis : T_MNEM_subis;
		  else
		    opcode = add ? T_MNEM_addi : T_MNEM_subi;
		}
	      if (opcode)
		{
		  inst.instruction = THUMB_OP16(opcode);
		  inst.instruction |= (Rd << 4) | Rs;
		  if (inst.reloc.type < BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
		      || inst.reloc.type > BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC)
		  {
		    if (inst.size_req == 2)
		      inst.reloc.type = BFD_RELOC_ARM_THUMB_ADD;
		    else
		      inst.relax = opcode;
		  }
		}
	      else
		constraint (inst.size_req == 2, BAD_HIREG);
	    }
	  if (inst.size_req == 4
	      || (inst.size_req != 2 && !opcode))
	    {
	      constraint (inst.reloc.type >= BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
			  && inst.reloc.type <= BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC ,
			  THUMB1_RELOC_ONLY);
	      if (Rd == REG_PC)
		{
		  constraint (add, BAD_PC);
		  constraint (Rs != REG_LR || inst.instruction != T_MNEM_subs,
			     _("only SUBS PC, LR, #const allowed"));
		  constraint (inst.reloc.exp.X_op != O_constant,
			      _("expression too complex"));
		  constraint (inst.reloc.exp.X_add_number < 0
			      || inst.reloc.exp.X_add_number > 0xff,
			     _("immediate value out of range"));
		  inst.instruction = T2_SUBS_PC_LR
				     | inst.reloc.exp.X_add_number;
		  inst.reloc.type = BFD_RELOC_UNUSED;
		  return;
		}
	      else if (Rs == REG_PC)
		{
		  /* Always use addw/subw.  */
		  inst.instruction = add ? 0xf20f0000 : 0xf2af0000;
		  inst.reloc.type = BFD_RELOC_ARM_T32_IMM12;
		}
	      else
		{
		  inst.instruction = THUMB_OP32 (inst.instruction);
		  inst.instruction = (inst.instruction & 0xe1ffffff)
				     | 0x10000000;
		  if (flags)
		    inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
		  else
		    inst.reloc.type = BFD_RELOC_ARM_T32_ADD_IMM;
		}
	      inst.instruction |= Rd << 8;
	      inst.instruction |= Rs << 16;
	    }
	}
      else
	{
	  unsigned int value = inst.reloc.exp.X_add_number;
	  unsigned int shift = inst.operands[2].shift_kind;

	  Rn = inst.operands[2].reg;
	  /* See if we can do this with a 16-bit instruction.  */
	  if (!inst.operands[2].shifted && inst.size_req != 4)
	    {
	      if (Rd > 7 || Rs > 7 || Rn > 7)
		narrow = FALSE;

	      if (narrow)
		{
		  inst.instruction = ((inst.instruction == T_MNEM_adds
				       || inst.instruction == T_MNEM_add)
				      ? T_OPCODE_ADD_R3
				      : T_OPCODE_SUB_R3);
		  inst.instruction |= Rd | (Rs << 3) | (Rn << 6);
		  return;
		}

	      if (inst.instruction == T_MNEM_add && (Rd == Rs || Rd == Rn))
		{
		  /* Thumb-1 cores (except v6-M) require at least one high
		     register in a narrow non flag setting add.  */
		  if (Rd > 7 || Rn >7 || ARM_CPU_HAS_FEATURE (selected_cpu, arm_ext_msr))
		    {
		      if (Rd == Rn)
			{
			  Rn = Rs;
			  Rs = Rd;
			}
		      inst.instruction = T_OPCODE_ADD_HI;
		      inst.instruction |= (Rd & 8) << 4;
		      inst.instruction |= (Rd & 7);
		      inst.instruction |= Rn << 3;
		      return;
		    }
		}
	    }

	  constraint (Rd == REG_PC, BAD_PC);
	    constraint (Rd == REG_SP && Rs != REG_SP, BAD_SP);
	  constraint (Rs == REG_PC, BAD_PC);
	  reject_bad_reg (Rn);

	  /* If we get here, it can't be done in 16 bits.  */
	  constraint (inst.operands[2].shifted && inst.operands[2].immisreg,
		      _("shift must be constant"));
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction |= Rd << 8;
	  inst.instruction |= Rs << 16;
	  constraint (Rd == REG_SP && Rs == REG_SP && value > 3,
		      _("shift value over 3 not allowed in thumb mode"));
	  constraint (Rd == REG_SP && Rs == REG_SP && shift != SHIFT_LSL,
		      _("only LSL shift allowed in thumb mode"));
	  encode_thumb32_shifted_operand (2);
	}
    }
  else
    {
      constraint (inst.instruction == T_MNEM_adds
		  || inst.instruction == T_MNEM_subs,
		  BAD_THUMB32);

      if (!inst.operands[2].isreg) /* Rd, Rs, #imm */
	{
	  constraint ((Rd > 7 && (Rd != REG_SP || Rs != REG_SP))
		      || (Rs > 7 && Rs != REG_SP && Rs != REG_PC),
		      BAD_HIREG);

	  inst.instruction = (inst.instruction == T_MNEM_add
			      ? 0x0000 : 0x8000);
	  inst.instruction |= (Rd << 4) | Rs;
	  inst.reloc.type = BFD_RELOC_ARM_THUMB_ADD;
	  return;
	}

      Rn = inst.operands[2].reg;
      constraint (inst.operands[2].shifted, _("unshifted register required"));

      /* We now have Rd, Rs, and Rn set to registers.  */
      if (Rd > 7 || Rs > 7 || Rn > 7)
	{
	  /* Can't do this for SUB.	 */
	  constraint (inst.instruction == T_MNEM_sub, BAD_HIREG);
	  inst.instruction = T_OPCODE_ADD_HI;
	  inst.instruction |= (Rd & 8) << 4;
	  inst.instruction |= (Rd & 7);
	  if (Rs == Rd)
	    inst.instruction |= Rn << 3;
	  else if (Rn == Rd)
	    inst.instruction |= Rs << 3;
	  else
	    constraint (1, _("dest must overlap one source register"));
	}
      else
	{
	  inst.instruction = (inst.instruction == T_MNEM_add
			      ? T_OPCODE_ADD_R3 : T_OPCODE_SUB_R3);
	  inst.instruction |= Rd | (Rs << 3) | (Rn << 6);
	}
    }
}

static void
do_t_adr (void)
{
  unsigned Rd;

  Rd = inst.operands[0].reg;
  reject_bad_reg (Rd);

  if (unified_syntax && inst.size_req == 0 && Rd <= 7)
    {
      /* Defer to section relaxation.  */
      inst.relax = inst.instruction;
      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= Rd << 4;
    }
  else if (unified_syntax && inst.size_req != 2)
    {
      /* Generate a 32-bit opcode.  */
      inst.instruction = THUMB_OP32 (inst.instruction);
      inst.instruction |= Rd << 8;
      inst.reloc.type = BFD_RELOC_ARM_T32_ADD_PC12;
      inst.reloc.pc_rel = 1;
    }
  else
    {
      /* Generate a 16-bit opcode.  */
      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.reloc.type = BFD_RELOC_ARM_THUMB_ADD;
      inst.reloc.exp.X_add_number -= 4; /* PC relative adjust.  */
      inst.reloc.pc_rel = 1;
      inst.instruction |= Rd << 4;
    }

  if (inst.reloc.exp.X_op == O_symbol
      && inst.reloc.exp.X_add_symbol != NULL
      && S_IS_DEFINED (inst.reloc.exp.X_add_symbol)
      && THUMB_IS_FUNC (inst.reloc.exp.X_add_symbol))
    inst.reloc.exp.X_add_number += 1;
}

/* Arithmetic instructions for which there is just one 16-bit
   instruction encoding, and it allows only two low registers.
   For maximal compatibility with ARM syntax, we allow three register
   operands even when Thumb-32 instructions are not available, as long
   as the first two are identical.  For instance, both "sbc r0,r1" and
   "sbc r0,r0,r1" are allowed.  */
static void
do_t_arit3 (void)
{
  int Rd, Rs, Rn;

  Rd = inst.operands[0].reg;
  Rs = (inst.operands[1].present
	? inst.operands[1].reg    /* Rd, Rs, foo */
	: inst.operands[0].reg);  /* Rd, foo -> Rd, Rd, foo */
  Rn = inst.operands[2].reg;

  reject_bad_reg (Rd);
  reject_bad_reg (Rs);
  if (inst.operands[2].isreg)
    reject_bad_reg (Rn);

  if (unified_syntax)
    {
      if (!inst.operands[2].isreg)
	{
	  /* For an immediate, we always generate a 32-bit opcode;
	     section relaxation will shrink it later if possible.  */
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction = (inst.instruction & 0xe1ffffff) | 0x10000000;
	  inst.instruction |= Rd << 8;
	  inst.instruction |= Rs << 16;
	  inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
      else
	{
	  bfd_boolean narrow;

	  /* See if we can do this with a 16-bit instruction.  */
	  if (THUMB_SETS_FLAGS (inst.instruction))
	    narrow = !in_it_block ();
	  else
	    narrow = in_it_block ();

	  if (Rd > 7 || Rn > 7 || Rs > 7)
	    narrow = FALSE;
	  if (inst.operands[2].shifted)
	    narrow = FALSE;
	  if (inst.size_req == 4)
	    narrow = FALSE;

	  if (narrow
	      && Rd == Rs)
	    {
	      inst.instruction = THUMB_OP16 (inst.instruction);
	      inst.instruction |= Rd;
	      inst.instruction |= Rn << 3;
	      return;
	    }

	  /* If we get here, it can't be done in 16 bits.  */
	  constraint (inst.operands[2].shifted
		      && inst.operands[2].immisreg,
		      _("shift must be constant"));
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction |= Rd << 8;
	  inst.instruction |= Rs << 16;
	  encode_thumb32_shifted_operand (2);
	}
    }
  else
    {
      /* On its face this is a lie - the instruction does set the
	 flags.  However, the only supported mnemonic in this mode
	 says it doesn't.  */
      constraint (THUMB_SETS_FLAGS (inst.instruction), BAD_THUMB32);

      constraint (!inst.operands[2].isreg || inst.operands[2].shifted,
		  _("unshifted register required"));
      constraint (Rd > 7 || Rs > 7 || Rn > 7, BAD_HIREG);
      constraint (Rd != Rs,
		  _("dest and source1 must be the same register"));

      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= Rd;
      inst.instruction |= Rn << 3;
    }
}

/* Similarly, but for instructions where the arithmetic operation is
   commutative, so we can allow either of them to be different from
   the destination operand in a 16-bit instruction.  For instance, all
   three of "adc r0,r1", "adc r0,r0,r1", and "adc r0,r1,r0" are
   accepted.  */
static void
do_t_arit3c (void)
{
  int Rd, Rs, Rn;

  Rd = inst.operands[0].reg;
  Rs = (inst.operands[1].present
	? inst.operands[1].reg    /* Rd, Rs, foo */
	: inst.operands[0].reg);  /* Rd, foo -> Rd, Rd, foo */
  Rn = inst.operands[2].reg;

  reject_bad_reg (Rd);
  reject_bad_reg (Rs);
  if (inst.operands[2].isreg)
    reject_bad_reg (Rn);

  if (unified_syntax)
    {
      if (!inst.operands[2].isreg)
	{
	  /* For an immediate, we always generate a 32-bit opcode;
	     section relaxation will shrink it later if possible.  */
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction = (inst.instruction & 0xe1ffffff) | 0x10000000;
	  inst.instruction |= Rd << 8;
	  inst.instruction |= Rs << 16;
	  inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
      else
	{
	  bfd_boolean narrow;

	  /* See if we can do this with a 16-bit instruction.  */
	  if (THUMB_SETS_FLAGS (inst.instruction))
	    narrow = !in_it_block ();
	  else
	    narrow = in_it_block ();

	  if (Rd > 7 || Rn > 7 || Rs > 7)
	    narrow = FALSE;
	  if (inst.operands[2].shifted)
	    narrow = FALSE;
	  if (inst.size_req == 4)
	    narrow = FALSE;

	  if (narrow)
	    {
	      if (Rd == Rs)
		{
		  inst.instruction = THUMB_OP16 (inst.instruction);
		  inst.instruction |= Rd;
		  inst.instruction |= Rn << 3;
		  return;
		}
	      if (Rd == Rn)
		{
		  inst.instruction = THUMB_OP16 (inst.instruction);
		  inst.instruction |= Rd;
		  inst.instruction |= Rs << 3;
		  return;
		}
	    }

	  /* If we get here, it can't be done in 16 bits.  */
	  constraint (inst.operands[2].shifted
		      && inst.operands[2].immisreg,
		      _("shift must be constant"));
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction |= Rd << 8;
	  inst.instruction |= Rs << 16;
	  encode_thumb32_shifted_operand (2);
	}
    }
  else
    {
      /* On its face this is a lie - the instruction does set the
	 flags.  However, the only supported mnemonic in this mode
	 says it doesn't.  */
      constraint (THUMB_SETS_FLAGS (inst.instruction), BAD_THUMB32);

      constraint (!inst.operands[2].isreg || inst.operands[2].shifted,
		  _("unshifted register required"));
      constraint (Rd > 7 || Rs > 7 || Rn > 7, BAD_HIREG);

      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= Rd;

      if (Rd == Rs)
	inst.instruction |= Rn << 3;
      else if (Rd == Rn)
	inst.instruction |= Rs << 3;
      else
	constraint (1, _("dest must overlap one source register"));
    }
}

/* ARM V5 Thumb BLX (argument parse)
	BLX <target_addr>	which is BLX(1)
	BLX <Rm>		which is BLX(2)
   Unfortunately, there are two different opcodes for this mnemonic.
   So, the insns[].value is not used, and the code here zaps values
	into inst.instruction.

   ??? How to take advantage of the additional two bits of displacement
   available in Thumb32 mode?  Need new relocation?  */

static void
do_t_blx (void)
{
  set_it_insn_type_last ();

  if (inst.operands[0].isreg)
    {
      constraint (inst.operands[0].reg == REG_PC, BAD_PC);
      /* We have a register, so this is BLX(2).  */
      inst.instruction |= inst.operands[0].reg << 3;
    }
  else
    {
      /* No register.  This must be BLX(1).  */
      inst.instruction = 0xf000e800;
      encode_branch (BFD_RELOC_THUMB_PCREL_BLX);
    }
}

static void
do_t_branch (void)
{
  int opcode;
  int cond;
  bfd_reloc_code_real_type reloc;

  cond = inst.cond;
  set_it_insn_type (IF_INSIDE_IT_LAST_INSN);

  if (in_it_block ())
    {
      /* Conditional branches inside IT blocks are encoded as unconditional
	 branches.  */
      cond = COND_ALWAYS;
    }
  else
    cond = inst.cond;

  if (cond != COND_ALWAYS)
    opcode = T_MNEM_bcond;
  else
    opcode = inst.instruction;

  if (unified_syntax
      && (inst.size_req == 4
	  || (inst.size_req != 2
	      && (inst.operands[0].hasreloc
		  || inst.reloc.exp.X_op == O_constant))))
    {
      inst.instruction = THUMB_OP32(opcode);
      if (cond == COND_ALWAYS)
	reloc = BFD_RELOC_THUMB_PCREL_BRANCH25;
      else
	{
	  constraint (TRUE,
		      _("selected architecture does not support "
			"wide conditional branch instruction"));
	}
    }
  else
    {
      inst.instruction = THUMB_OP16(opcode);
      if (cond == COND_ALWAYS)
	reloc = BFD_RELOC_THUMB_PCREL_BRANCH12;
      else
	{
	  inst.instruction |= cond << 8;
	  reloc = BFD_RELOC_THUMB_PCREL_BRANCH9;
	}
      /* Allow section relaxation.  */
      if (unified_syntax && inst.size_req != 2)
	inst.relax = opcode;
    }
  inst.reloc.type = reloc;
  inst.reloc.pc_rel = 1;
}

/* Actually do the work for Thumb state bkpt and hlt.  The only difference
   between the two is the maximum immediate allowed - which is passed in
   RANGE.  */
static void
do_t_branch23 (void)
{
  set_it_insn_type_last ();
  encode_branch (BFD_RELOC_THUMB_PCREL_BRANCH23);

  /* md_apply_fix blows up with 'bl foo(PLT)' where foo is defined in
     this file.  We used to simply ignore the PLT reloc type here --
     the branch encoding is now needed to deal with TLSCALL relocs.
     So if we see a PLT reloc now, put it back to how it used to be to
     keep the preexisting behaviour.  */
  if (inst.reloc.type == BFD_RELOC_ARM_PLT32)
    inst.reloc.type = BFD_RELOC_THUMB_PCREL_BRANCH23;
}

static void
do_t_bx (void)
{
  set_it_insn_type_last ();
  inst.instruction |= inst.operands[0].reg << 3;
  /* ??? FIXME: Should add a hacky reloc here if reg is REG_PC.	 The reloc
     should cause the alignment to be checked once it is known.	 This is
     because BX PC only works if the instruction is word aligned.  */
}

/* THUMB CPY instruction (argument parse).  */

static void
do_t_cpy (void)
{
  if (inst.size_req == 4)
    {
      inst.instruction = THUMB_OP32 (T_MNEM_mov);
      inst.instruction |= inst.operands[0].reg << 8;
      inst.instruction |= inst.operands[1].reg;
    }
  else
    {
      inst.instruction |= (inst.operands[0].reg & 0x8) << 4;
      inst.instruction |= (inst.operands[0].reg & 0x7);
      inst.instruction |= inst.operands[1].reg << 3;
    }
}

/* Helper function used for both push/pop and ldm/stm.  */
static void
encode_thumb2_ldmstm (int base, unsigned mask, bfd_boolean writeback)
{
  bfd_boolean load;

  load = (inst.instruction & (1 << 20)) != 0;

  if (mask & (1 << 13))
    inst.error =  _("SP not allowed in register list");

  if ((mask & (1 << base)) != 0
      && writeback)
    inst.error = _("having the base register in the register list when "
		   "using write back is UNPREDICTABLE");

  if (load)
    {
      if (mask & (1 << 15))
	{
	  if (mask & (1 << 14))
	    inst.error = _("LR and PC should not both be in register list");
	  else
	    set_it_insn_type_last ();
	}
    }
  else
    {
      if (mask & (1 << 15))
	inst.error = _("PC not allowed in register list");
    }

  if ((mask & (mask - 1)) == 0)
    {
      /* Single register transfers implemented as str/ldr.  */
      if (writeback)
	{
	  if (inst.instruction & (1 << 23))
	    inst.instruction = 0x00000b04; /* ia! -> [base], #4 */
	  else
	    inst.instruction = 0x00000d04; /* db! -> [base, #-4]! */
	}
      else
	{
	  if (inst.instruction & (1 << 23))
	    inst.instruction = 0x00800000; /* ia -> [base] */
	  else
	    inst.instruction = 0x00000c04; /* db -> [base, #-4] */
	}

      inst.instruction |= 0xf8400000;
      if (load)
	inst.instruction |= 0x00100000;

      mask = ffs (mask) - 1;
      mask <<= 12;
    }
  else if (writeback)
    inst.instruction |= WRITE_BACK;

  inst.instruction |= mask;
  inst.instruction |= base << 16;
}

static void
do_t_ldmstm (void)
{
  /* This really doesn't seem worth it.  */
  constraint (inst.reloc.type != BFD_RELOC_UNUSED,
	      _("expression too complex"));
  constraint (inst.operands[1].writeback,
	      _("Thumb load/store multiple does not support {reglist}^"));

  if (unified_syntax)
    {
      bfd_boolean narrow;
      unsigned mask;

      narrow = FALSE;
      /* See if we can use a 16-bit instruction.  */
      if (inst.instruction < 0xffff /* not ldmdb/stmdb */
	  && inst.size_req != 4
	  && !(inst.operands[1].imm & ~0xff))
	{
	  mask = 1 << inst.operands[0].reg;

	  if (inst.operands[0].reg <= 7)
	    {
	      if (inst.instruction == T_MNEM_stmia
		  ? inst.operands[0].writeback
		  : (inst.operands[0].writeback
		     == !(inst.operands[1].imm & mask)))
		{
		  if (inst.instruction == T_MNEM_stmia
		      && (inst.operands[1].imm & mask)
		      && (inst.operands[1].imm & (mask - 1)))
		    as_warn (_("value stored for r%d is UNKNOWN"),
			     inst.operands[0].reg);

		  inst.instruction = THUMB_OP16 (inst.instruction);
		  inst.instruction |= inst.operands[0].reg << 8;
		  inst.instruction |= inst.operands[1].imm;
		  narrow = TRUE;
		}
	      else if ((inst.operands[1].imm & (inst.operands[1].imm-1)) == 0)
		{
		  /* This means 1 register in reg list one of 3 situations:
		     1. Instruction is stmia, but without writeback.
		     2. lmdia without writeback, but with Rn not in
			reglist.
		     3. ldmia with writeback, but with Rn in reglist.
		     Case 3 is UNPREDICTABLE behaviour, so we handle
		     case 1 and 2 which can be converted into a 16-bit
		     str or ldr. The SP cases are handled below.  */
		  unsigned long opcode;
		  /* First, record an error for Case 3.  */
		  if (inst.operands[1].imm & mask
		      && inst.operands[0].writeback)
		    inst.error =
			_("having the base register in the register list when "
			  "using write back is UNPREDICTABLE");

		  opcode = (inst.instruction == T_MNEM_stmia ? T_MNEM_str
							     : T_MNEM_ldr);
		  inst.instruction = THUMB_OP16 (opcode);
		  inst.instruction |= inst.operands[0].reg << 3;
		  inst.instruction |= (ffs (inst.operands[1].imm)-1);
		  narrow = TRUE;
		}
	    }
	  else if (inst.operands[0] .reg == REG_SP)
	    {
	      if (inst.operands[0].writeback)
		{
		  inst.instruction =
			THUMB_OP16 (inst.instruction == T_MNEM_stmia
				    ? T_MNEM_push : T_MNEM_pop);
		  inst.instruction |= inst.operands[1].imm;
		  narrow = TRUE;
		}
	      else if ((inst.operands[1].imm & (inst.operands[1].imm-1)) == 0)
		{
		  inst.instruction =
			THUMB_OP16 (inst.instruction == T_MNEM_stmia
				    ? T_MNEM_str_sp : T_MNEM_ldr_sp);
		  inst.instruction |= ((ffs (inst.operands[1].imm)-1) << 8);
		  narrow = TRUE;
		}
	    }
	}

      if (!narrow)
	{
	  if (inst.instruction < 0xffff)
	    inst.instruction = THUMB_OP32 (inst.instruction);

	  encode_thumb2_ldmstm (inst.operands[0].reg, inst.operands[1].imm,
				inst.operands[0].writeback);
	}
    }
  else
    {
      constraint (inst.operands[0].reg > 7
		  || (inst.operands[1].imm & ~0xff), BAD_HIREG);
      constraint (inst.instruction != T_MNEM_ldmia
		  && inst.instruction != T_MNEM_stmia,
		  _("Thumb-2 instruction only valid in unified syntax"));
      if (inst.instruction == T_MNEM_stmia)
	{
	  if (!inst.operands[0].writeback)
	    as_warn (_("this instruction will write back the base register"));
	  if ((inst.operands[1].imm & (1 << inst.operands[0].reg))
	      && (inst.operands[1].imm & ((1 << inst.operands[0].reg) - 1)))
	    as_warn (_("value stored for r%d is UNKNOWN"),
		     inst.operands[0].reg);
	}
      else
	{
	  if (!inst.operands[0].writeback
	      && !(inst.operands[1].imm & (1 << inst.operands[0].reg)))
	    as_warn (_("this instruction will write back the base register"));
	  else if (inst.operands[0].writeback
		   && (inst.operands[1].imm & (1 << inst.operands[0].reg)))
	    as_warn (_("this instruction will not write back the base register"));
	}

      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= inst.operands[0].reg << 8;
      inst.instruction |= inst.operands[1].imm;
    }
}

static void
do_t_ldst (void)
{
  unsigned long opcode;
  int Rn;

  if (inst.operands[0].isreg
      && !inst.operands[0].preind
      && inst.operands[0].reg == REG_PC)
    set_it_insn_type_last ();

  opcode = inst.instruction;
  if (unified_syntax)
    {
      if (!inst.operands[1].isreg)
	{
	  if (opcode <= 0xffff)
	    inst.instruction = THUMB_OP32 (opcode);
	  if (move_or_literal_pool (0, CONST_THUMB, /*mode_3=*/FALSE))
	    return;
	}
      if (inst.operands[1].isreg
	  && !inst.operands[1].writeback
	  && !inst.operands[1].shifted && !inst.operands[1].postind
	  && !inst.operands[1].negative && inst.operands[0].reg <= 7
	  && opcode <= 0xffff
	  && inst.size_req != 4)
	{
	  /* Insn may have a 16-bit form.  */
	  Rn = inst.operands[1].reg;
	  if (inst.operands[1].immisreg)
	    {
	      inst.instruction = THUMB_OP16 (opcode);
	      /* [Rn, Rik] */
	      if (Rn <= 7 && inst.operands[1].imm <= 7)
		goto op16;
	      else if (opcode != T_MNEM_ldr && opcode != T_MNEM_str)
		reject_bad_reg (inst.operands[1].imm);
	    }
	  else if ((Rn <= 7 && opcode != T_MNEM_ldrsh
		    && opcode != T_MNEM_ldrsb)
		   || ((Rn == REG_PC || Rn == REG_SP) && opcode == T_MNEM_ldr)
		   || (Rn == REG_SP && opcode == T_MNEM_str))
	    {
	      /* [Rn, #const] */
	      if (Rn > 7)
		{
		  if (Rn == REG_PC)
		    {
		      if (inst.reloc.pc_rel)
			opcode = T_MNEM_ldr_pc2;
		      else
			opcode = T_MNEM_ldr_pc;
		    }
		  else
		    {
		      if (opcode == T_MNEM_ldr)
			opcode = T_MNEM_ldr_sp;
		      else
			opcode = T_MNEM_str_sp;
		    }
		  inst.instruction = inst.operands[0].reg << 8;
		}
	      else
		{
		  inst.instruction = inst.operands[0].reg;
		  inst.instruction |= inst.operands[1].reg << 3;
		}
	      inst.instruction |= THUMB_OP16 (opcode);
	      if (inst.size_req == 2)
		inst.reloc.type = BFD_RELOC_ARM_THUMB_OFFSET;
	      else
		inst.relax = opcode;
	      return;
	    }
	}
      /* Definitely a 32-bit variant.  */

      /* Warning for Erratum 752419.  */
      if (opcode == T_MNEM_ldr
	  && inst.operands[0].reg == REG_SP
	  && inst.operands[1].writeback == 1
	  && !inst.operands[1].immisreg)
	{
	  if (no_cpu_selected ())
	    as_warn (_("This instruction may be unpredictable "
		       "if executed on M-profile cores "
		       "with interrupts enabled."));
	}

      /* Do some validations regarding addressing modes.  */
      if (inst.operands[1].immisreg)
	reject_bad_reg (inst.operands[1].imm);

      constraint (inst.operands[1].writeback == 1
		  && inst.operands[0].reg == inst.operands[1].reg,
		  BAD_OVERLAP);

      inst.instruction = THUMB_OP32 (opcode);
      inst.instruction |= inst.operands[0].reg << 12;
      encode_thumb32_addr_mode (1, /*is_t=*/FALSE, /*is_d=*/FALSE);
      check_ldr_r15_aligned ();
      return;
    }

  constraint (inst.operands[0].reg > 7, BAD_HIREG);

  if (inst.instruction == T_MNEM_ldrsh || inst.instruction == T_MNEM_ldrsb)
    {
      /* Only [Rn,Rm] is acceptable.  */
      constraint (inst.operands[1].reg > 7 || inst.operands[1].imm > 7, BAD_HIREG);
      constraint (!inst.operands[1].isreg || !inst.operands[1].immisreg
		  || inst.operands[1].postind || inst.operands[1].shifted
		  || inst.operands[1].negative,
		  _("Thumb does not support this addressing mode"));
      inst.instruction = THUMB_OP16 (inst.instruction);
      goto op16;
    }

  inst.instruction = THUMB_OP16 (inst.instruction);
  if (!inst.operands[1].isreg)
    if (move_or_literal_pool (0, CONST_THUMB, /*mode_3=*/FALSE))
      return;

  constraint (!inst.operands[1].preind
	      || inst.operands[1].shifted
	      || inst.operands[1].writeback,
	      _("Thumb does not support this addressing mode"));
  if (inst.operands[1].reg == REG_PC || inst.operands[1].reg == REG_SP)
    {
      constraint (inst.instruction & 0x0600,
		  _("byte or halfword not valid for base register"));
      constraint (inst.operands[1].reg == REG_PC
		  && !(inst.instruction & THUMB_LOAD_BIT),
		  _("r15 based store not allowed"));
      constraint (inst.operands[1].immisreg,
		  _("invalid base register for register offset"));

      if (inst.operands[1].reg == REG_PC)
	inst.instruction = T_OPCODE_LDR_PC;
      else if (inst.instruction & THUMB_LOAD_BIT)
	inst.instruction = T_OPCODE_LDR_SP;
      else
	inst.instruction = T_OPCODE_STR_SP;

      inst.instruction |= inst.operands[0].reg << 8;
      inst.reloc.type = BFD_RELOC_ARM_THUMB_OFFSET;
      return;
    }

  constraint (inst.operands[1].reg > 7, BAD_HIREG);
  if (!inst.operands[1].immisreg)
    {
      /* Immediate offset.  */
      inst.instruction |= inst.operands[0].reg;
      inst.instruction |= inst.operands[1].reg << 3;
      inst.reloc.type = BFD_RELOC_ARM_THUMB_OFFSET;
      return;
    }

  /* Register offset.  */
  constraint (inst.operands[1].imm > 7, BAD_HIREG);
  constraint (inst.operands[1].negative,
	      _("Thumb does not support this addressing mode"));

 op16:
  switch (inst.instruction)
    {
    case T_OPCODE_STR_IW: inst.instruction = T_OPCODE_STR_RW; break;
    case T_OPCODE_STR_IH: inst.instruction = T_OPCODE_STR_RH; break;
    case T_OPCODE_STR_IB: inst.instruction = T_OPCODE_STR_RB; break;
    case T_OPCODE_LDR_IW: inst.instruction = T_OPCODE_LDR_RW; break;
    case T_OPCODE_LDR_IH: inst.instruction = T_OPCODE_LDR_RH; break;
    case T_OPCODE_LDR_IB: inst.instruction = T_OPCODE_LDR_RB; break;
    case 0x5600 /* ldrsb */:
    case 0x5e00 /* ldrsh */: break;
    default: abort ();
    }

  inst.instruction |= inst.operands[0].reg;
  inst.instruction |= inst.operands[1].reg << 3;
  inst.instruction |= inst.operands[1].imm << 6;
}

static void
do_t_ldstt (void)
{
  inst.instruction |= inst.operands[0].reg << 12;
  encode_thumb32_addr_mode (1, /*is_t=*/TRUE, /*is_d=*/FALSE);
}

static void
do_t_mla (void)
{
  unsigned Rd, Rn, Rm, Ra;

  Rd = inst.operands[0].reg;
  Rn = inst.operands[1].reg;
  Rm = inst.operands[2].reg;
  Ra = inst.operands[3].reg;

  reject_bad_reg (Rd);
  reject_bad_reg (Rn);
  reject_bad_reg (Rm);
  reject_bad_reg (Ra);

  inst.instruction |= Rd << 8;
  inst.instruction |= Rn << 16;
  inst.instruction |= Rm;
  inst.instruction |= Ra << 12;
}

static void
do_t_mov_cmp (void)
{
  unsigned Rn, Rm;

  Rn = inst.operands[0].reg;
  Rm = inst.operands[1].reg;

  if (Rn == REG_PC)
    set_it_insn_type_last ();

  if (unified_syntax)
    {
      int r0off = (inst.instruction == T_MNEM_mov
		   || inst.instruction == T_MNEM_movs) ? 8 : 16;
      unsigned long opcode;
      bfd_boolean narrow;
      bfd_boolean low_regs;

      low_regs = (Rn <= 7 && Rm <= 7);
      opcode = inst.instruction;
      if (in_it_block ())
	narrow = opcode != T_MNEM_movs;
      else
	narrow = opcode != T_MNEM_movs || low_regs;
      if (inst.size_req == 4
	  || inst.operands[1].shifted)
	narrow = FALSE;

      /* MOVS PC, LR is encoded as SUBS PC, LR, #0.  */
      if (opcode == T_MNEM_movs && inst.operands[1].isreg
	  && !inst.operands[1].shifted
	  && Rn == REG_PC
	  && Rm == REG_LR)
	{
	  inst.instruction = T2_SUBS_PC_LR;
	  return;
	}

      if (opcode == T_MNEM_cmp)
	{
	  constraint (Rn == REG_PC, BAD_PC);
	  if (narrow)
	    {
	      /* In the Thumb-2 ISA, use of R13 as Rm is deprecated,
		 but valid.  */
	      warn_deprecated_sp (Rm);
	      /* R15 was documented as a valid choice for Rm in ARMv6,
		 but as UNPREDICTABLE in ARMv7.  ARM's proprietary
		 tools reject R15, so we do too.  */
	      constraint (Rm == REG_PC, BAD_PC);
	    }
	  else
	    reject_bad_reg (Rm);
	}
      else if (opcode == T_MNEM_mov
	       || opcode == T_MNEM_movs)
	{
	  if (inst.operands[1].isreg)
	    {
	      if (opcode == T_MNEM_movs)
		{
		  reject_bad_reg (Rn);
		  reject_bad_reg (Rm);
		}
	      else if (narrow)
		{
		  /* This is mov.n.  */
		  if ((Rn == REG_SP || Rn == REG_PC)
		      && (Rm == REG_SP || Rm == REG_PC))
		    {
		      as_tsktsk (_("Use of r%u as a source register is "
				 "deprecated when r%u is the destination "
				 "register."), Rm, Rn);
		    }
		}
	      else
		{
		  /* This is mov.w.  */
		  constraint (Rn == REG_PC, BAD_PC);
		  constraint (Rm == REG_PC, BAD_PC);
		    constraint (Rn == REG_SP && Rm == REG_SP, BAD_SP);
		}
	    }
	  else
	    reject_bad_reg (Rn);
	}

      if (!inst.operands[1].isreg)
	{
	  /* Immediate operand.  */
	  if (!in_it_block () && opcode == T_MNEM_mov)
	    narrow = 0;
	  if (low_regs && narrow)
	    {
	      inst.instruction = THUMB_OP16 (opcode);
	      inst.instruction |= Rn << 8;
	      if (inst.reloc.type < BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
		  || inst.reloc.type > BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC)
		{
		  if (inst.size_req == 2)
		    inst.reloc.type = BFD_RELOC_ARM_THUMB_IMM;
		  else
		    inst.relax = opcode;
		}
	    }
	  else
	    {
	      constraint (inst.reloc.type >= BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
			  && inst.reloc.type <= BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC ,
			  THUMB1_RELOC_ONLY);

	      inst.instruction = THUMB_OP32 (inst.instruction);
	      inst.instruction = (inst.instruction & 0xe1ffffff) | 0x10000000;
	      inst.instruction |= Rn << r0off;
	      inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
	    }
	}
      else if (inst.operands[1].shifted && inst.operands[1].immisreg
	       && (inst.instruction == T_MNEM_mov
		   || inst.instruction == T_MNEM_movs))
	{
	  /* Register shifts are encoded as separate shift instructions.  */
	  bfd_boolean flags = (inst.instruction == T_MNEM_movs);

	  if (in_it_block ())
	    narrow = !flags;
	  else
	    narrow = flags;

	  if (inst.size_req == 4)
	    narrow = FALSE;

	  if (!low_regs || inst.operands[1].imm > 7)
	    narrow = FALSE;

	  if (Rn != Rm)
	    narrow = FALSE;

	  switch (inst.operands[1].shift_kind)
	    {
	    case SHIFT_LSL:
	      opcode = narrow ? T_OPCODE_LSL_R : THUMB_OP32 (T_MNEM_lsl);
	      break;
	    case SHIFT_ASR:
	      opcode = narrow ? T_OPCODE_ASR_R : THUMB_OP32 (T_MNEM_asr);
	      break;
	    case SHIFT_LSR:
	      opcode = narrow ? T_OPCODE_LSR_R : THUMB_OP32 (T_MNEM_lsr);
	      break;
	    case SHIFT_ROR:
	      opcode = narrow ? T_OPCODE_ROR_R : THUMB_OP32 (T_MNEM_ror);
	      break;
	    default:
	      abort ();
	    }

	  inst.instruction = opcode;
	  if (narrow)
	    {
	      inst.instruction |= Rn;
	      inst.instruction |= inst.operands[1].imm << 3;
	    }
	  else
	    {
	      if (flags)
		inst.instruction |= CONDS_BIT;

	      inst.instruction |= Rn << 8;
	      inst.instruction |= Rm << 16;
	      inst.instruction |= inst.operands[1].imm;
	    }
	}
      else if (!narrow)
	{
	  /* Some mov with immediate shift have narrow variants.
	     Register shifts are handled above.  */
	  if (low_regs && inst.operands[1].shifted
	      && (inst.instruction == T_MNEM_mov
		  || inst.instruction == T_MNEM_movs))
	    {
	      if (in_it_block ())
		narrow = (inst.instruction == T_MNEM_mov);
	      else
		narrow = (inst.instruction == T_MNEM_movs);
	    }

	  if (narrow)
	    {
	      switch (inst.operands[1].shift_kind)
		{
		case SHIFT_LSL: inst.instruction = T_OPCODE_LSL_I; break;
		case SHIFT_LSR: inst.instruction = T_OPCODE_LSR_I; break;
		case SHIFT_ASR: inst.instruction = T_OPCODE_ASR_I; break;
		default: narrow = FALSE; break;
		}
	    }

	  if (narrow)
	    {
	      inst.instruction |= Rn;
	      inst.instruction |= Rm << 3;
	      inst.reloc.type = BFD_RELOC_ARM_THUMB_SHIFT;
	    }
	  else
	    {
	      inst.instruction = THUMB_OP32 (inst.instruction);
	      inst.instruction |= Rn << r0off;
	      encode_thumb32_shifted_operand (1);
	    }
	}
      else
	switch (inst.instruction)
	  {
	  case T_MNEM_mov:
	    /* In v4t or v5t a move of two lowregs produces unpredictable
	       results. Don't allow this.  */
	    if (low_regs)
	      {
		constraint (TRUE,
			    "MOV Rd, Rs with two low registers is not "
			    "permitted on this architecture");
	      }

	    inst.instruction = T_OPCODE_MOV_HR;
	    inst.instruction |= (Rn & 0x8) << 4;
	    inst.instruction |= (Rn & 0x7);
	    inst.instruction |= Rm << 3;
	    break;

	  case T_MNEM_movs:
	    /* We know we have low registers at this point.
	       Generate LSLS Rd, Rs, #0.  */
	    inst.instruction = T_OPCODE_LSL_I;
	    inst.instruction |= Rn;
	    inst.instruction |= Rm << 3;
	    break;

	  case T_MNEM_cmp:
	    if (low_regs)
	      {
		inst.instruction = T_OPCODE_CMP_LR;
		inst.instruction |= Rn;
		inst.instruction |= Rm << 3;
	      }
	    else
	      {
		inst.instruction = T_OPCODE_CMP_HR;
		inst.instruction |= (Rn & 0x8) << 4;
		inst.instruction |= (Rn & 0x7);
		inst.instruction |= Rm << 3;
	      }
	    break;
	  }
      return;
    }

  inst.instruction = THUMB_OP16 (inst.instruction);

  /* PR 10443: Do not silently ignore shifted operands.  */
  constraint (inst.operands[1].shifted,
	      _("shifts in CMP/MOV instructions are only supported in unified syntax"));

  if (inst.operands[1].isreg)
    {
      if (Rn < 8 && Rm < 8)
	{
	  /* A move of two lowregs is encoded as ADD Rd, Rs, #0
	     since a MOV instruction produces unpredictable results.  */
	  if (inst.instruction == T_OPCODE_MOV_I8)
	    inst.instruction = T_OPCODE_ADD_I3;
	  else
	    inst.instruction = T_OPCODE_CMP_LR;

	  inst.instruction |= Rn;
	  inst.instruction |= Rm << 3;
	}
      else
	{
	  if (inst.instruction == T_OPCODE_MOV_I8)
	    inst.instruction = T_OPCODE_MOV_HR;
	  else
	    inst.instruction = T_OPCODE_CMP_HR;
	  do_t_cpy ();
	}
    }
  else
    {
      constraint (Rn > 7,
		  _("only lo regs allowed with immediate"));
      inst.instruction |= Rn << 8;
      inst.reloc.type = BFD_RELOC_ARM_THUMB_IMM;
    }
}

static void
do_t_mvn_tst (void)
{
  unsigned Rn, Rm;

  Rn = inst.operands[0].reg;
  Rm = inst.operands[1].reg;

  if (inst.instruction == T_MNEM_cmp
      || inst.instruction == T_MNEM_cmn)
    constraint (Rn == REG_PC, BAD_PC);
  else
    reject_bad_reg (Rn);
  reject_bad_reg (Rm);

  if (unified_syntax)
    {
      int r0off = (inst.instruction == T_MNEM_mvn
		   || inst.instruction == T_MNEM_mvns) ? 8 : 16;
      bfd_boolean narrow;

      if (inst.size_req == 4
	  || inst.instruction > 0xffff
	  || inst.operands[1].shifted
	  || Rn > 7 || Rm > 7)
	narrow = FALSE;
      else if (inst.instruction == T_MNEM_cmn
	       || inst.instruction == T_MNEM_tst)
	narrow = TRUE;
      else if (THUMB_SETS_FLAGS (inst.instruction))
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();

      if (!inst.operands[1].isreg)
	{
	  /* For an immediate, we always generate a 32-bit opcode;
	     section relaxation will shrink it later if possible.  */
	  if (inst.instruction < 0xffff)
	    inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction = (inst.instruction & 0xe1ffffff) | 0x10000000;
	  inst.instruction |= Rn << r0off;
	  inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
      else
	{
	  /* See if we can do this with a 16-bit instruction.  */
	  if (narrow)
	    {
	      inst.instruction = THUMB_OP16 (inst.instruction);
	      inst.instruction |= Rn;
	      inst.instruction |= Rm << 3;
	    }
	  else
	    {
	      constraint (inst.operands[1].shifted
			  && inst.operands[1].immisreg,
			  _("shift must be constant"));
	      if (inst.instruction < 0xffff)
		inst.instruction = THUMB_OP32 (inst.instruction);
	      inst.instruction |= Rn << r0off;
	      encode_thumb32_shifted_operand (1);
	    }
	}
    }
  else
    {
      constraint (inst.instruction > 0xffff
		  || inst.instruction == T_MNEM_mvns, BAD_THUMB32);
      constraint (!inst.operands[1].isreg || inst.operands[1].shifted,
		  _("unshifted register required"));
      constraint (Rn > 7 || Rm > 7,
		  BAD_HIREG);

      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= Rn;
      inst.instruction |= Rm << 3;
    }
}

static void
do_t_mrs (void)
{
  unsigned Rd;

  if (do_vfp_nsyn_mrs () == SUCCESS)
    return;

  Rd = inst.operands[0].reg;
  reject_bad_reg (Rd);
  inst.instruction |= Rd << 8;

  if (inst.operands[1].isreg)
    {
      unsigned br = inst.operands[1].reg;
      if (((br & 0x200) == 0) && ((br & 0xf000) != 0xf000))
	as_bad (_("bad register for mrs"));

      inst.instruction |= br & (0xf << 16);
      inst.instruction |= (br & 0x300) >> 4;
      inst.instruction |= (br & SPSR_BIT) >> 2;
    }
  else
    {
      int flags = inst.operands[1].imm & (PSR_c|PSR_x|PSR_s|PSR_f|SPSR_BIT);

	/* mrs only accepts APSR/CPSR/SPSR/CPSR_all/SPSR_all (for non-M profile
	   devices).  */
      constraint ((flags & ~SPSR_BIT) != (PSR_c|PSR_f),
		    _("'APSR', 'CPSR' or 'SPSR' expected"));

      inst.instruction |= (flags & SPSR_BIT) >> 2;
      inst.instruction |= inst.operands[1].imm & 0xff;
      inst.instruction |= 0xf0000;
    }
}

static void
do_t_msr (void)
{
  int flags;
  unsigned Rn;

  if (do_vfp_nsyn_msr () == SUCCESS)
    return;

  constraint (!inst.operands[1].isreg,
	      _("Thumb encoding does not support an immediate here"));

  if (inst.operands[0].isreg)
    flags = (int)(inst.operands[0].reg);
  else
    flags = inst.operands[0].imm;

 constraint ((flags & 0xff) != 0, _("selected processor does not support "
		 "requested special purpose register"));

  Rn = inst.operands[1].reg;
  reject_bad_reg (Rn);

  inst.instruction |= (flags & SPSR_BIT) >> 2;
  inst.instruction |= (flags & 0xf0000) >> 8;
  inst.instruction |= (flags & 0x300) >> 4;
  inst.instruction |= (flags & 0xff);
  inst.instruction |= Rn << 16;
}

static void
do_t_mul (void)
{
  bfd_boolean narrow;
  unsigned Rd, Rn, Rm;

  if (!inst.operands[2].present)
    inst.operands[2].reg = inst.operands[0].reg;

  Rd = inst.operands[0].reg;
  Rn = inst.operands[1].reg;
  Rm = inst.operands[2].reg;

  if (unified_syntax)
    {
      if (inst.size_req == 4
	  || (Rd != Rn
	      && Rd != Rm)
	  || Rn > 7
	  || Rm > 7)
	narrow = FALSE;
      else if (inst.instruction == T_MNEM_muls)
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();
    }
  else
    {
      constraint (inst.instruction == T_MNEM_muls, BAD_THUMB32);
      constraint (Rn > 7 || Rm > 7,
		  BAD_HIREG);
      narrow = TRUE;
    }

  if (narrow)
    {
      /* 16-bit MULS/Conditional MUL.  */
      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= Rd;

      if (Rd == Rn)
	inst.instruction |= Rm << 3;
      else if (Rd == Rm)
	inst.instruction |= Rn << 3;
      else
	constraint (1, _("dest must overlap one source register"));
    }
  else
    {
      constraint (inst.instruction != T_MNEM_mul,
		  _("Thumb-2 MUL must not set flags"));
      /* 32-bit MUL.  */
      inst.instruction = THUMB_OP32 (inst.instruction);
      inst.instruction |= Rd << 8;
      inst.instruction |= Rn << 16;
      inst.instruction |= Rm << 0;

      reject_bad_reg (Rd);
      reject_bad_reg (Rn);
      reject_bad_reg (Rm);
    }
}

static void
do_t_mull (void)
{
  unsigned RdLo, RdHi, Rn, Rm;

  RdLo = inst.operands[0].reg;
  RdHi = inst.operands[1].reg;
  Rn = inst.operands[2].reg;
  Rm = inst.operands[3].reg;

  reject_bad_reg (RdLo);
  reject_bad_reg (RdHi);
  reject_bad_reg (Rn);
  reject_bad_reg (Rm);

  inst.instruction |= RdLo << 12;
  inst.instruction |= RdHi << 8;
  inst.instruction |= Rn << 16;
  inst.instruction |= Rm;

 if (RdLo == RdHi)
    as_tsktsk (_("rdhi and rdlo must be different"));
}

static void
do_t_nop (void)
{
  set_it_insn_type (NEUTRAL_IT_INSN);

  if (unified_syntax)
    {
      if (inst.size_req == 4 || inst.operands[0].imm > 15)
	{
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction |= inst.operands[0].imm;
	}
      else
	{
	  /* PR9722: Check for Thumb2 availability before
	     generating a thumb2 nop instruction.  */
	    inst.instruction = 0x46c0;
	}
    }
  else
    {
      constraint (inst.operands[0].present,
		  _("Thumb does not support NOP with hints"));
      inst.instruction = 0x46c0;
    }
}

static void
do_t_neg (void)
{
  if (unified_syntax)
    {
      bfd_boolean narrow;

      if (THUMB_SETS_FLAGS (inst.instruction))
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();
      if (inst.operands[0].reg > 7 || inst.operands[1].reg > 7)
	narrow = FALSE;
      if (inst.size_req == 4)
	narrow = FALSE;

      if (!narrow)
	{
	  inst.instruction = THUMB_OP32 (inst.instruction);
	  inst.instruction |= inst.operands[0].reg << 8;
	  inst.instruction |= inst.operands[1].reg << 16;
	}
      else
	{
	  inst.instruction = THUMB_OP16 (inst.instruction);
	  inst.instruction |= inst.operands[0].reg;
	  inst.instruction |= inst.operands[1].reg << 3;
	}
    }
  else
    {
      constraint (inst.operands[0].reg > 7 || inst.operands[1].reg > 7,
		  BAD_HIREG);
      constraint (THUMB_SETS_FLAGS (inst.instruction), BAD_THUMB32);

      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= inst.operands[0].reg;
      inst.instruction |= inst.operands[1].reg << 3;
    }
}

static void
do_t_push_pop (void)
{
  unsigned mask;

  constraint (inst.operands[0].writeback,
	      _("push/pop do not support {reglist}^"));
  constraint (inst.reloc.type != BFD_RELOC_UNUSED,
	      _("expression too complex"));

  mask = inst.operands[0].imm;
  if (inst.size_req != 4 && (mask & ~0xff) == 0)
    inst.instruction = THUMB_OP16 (inst.instruction) | mask;
  else if (inst.size_req != 4
	   && (mask & ~0xff) == (1U << (inst.instruction == T_MNEM_push
				       ? REG_LR : REG_PC)))
    {
      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= THUMB_PP_PC_LR;
      inst.instruction |= mask & 0xff;
    }
  else if (unified_syntax)
    {
      inst.instruction = THUMB_OP32 (inst.instruction);
      encode_thumb2_ldmstm (13, mask, TRUE);
    }
  else
    {
      inst.error = _("invalid register list to push/pop instruction");
      return;
    }
}

static void
do_t_rsb (void)
{
  unsigned Rd, Rs;

  Rd = inst.operands[0].reg;
  Rs = (inst.operands[1].present
	? inst.operands[1].reg    /* Rd, Rs, foo */
	: inst.operands[0].reg);  /* Rd, foo -> Rd, Rd, foo */

  reject_bad_reg (Rd);
  reject_bad_reg (Rs);
  if (inst.operands[2].isreg)
    reject_bad_reg (inst.operands[2].reg);

  inst.instruction |= Rd << 8;
  inst.instruction |= Rs << 16;
  if (!inst.operands[2].isreg)
    {
      bfd_boolean narrow;

      if ((inst.instruction & 0x00100000) != 0)
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();

      if (Rd > 7 || Rs > 7)
	narrow = FALSE;

      if (inst.size_req == 4 || !unified_syntax)
	narrow = FALSE;

      if (inst.reloc.exp.X_op != O_constant
	  || inst.reloc.exp.X_add_number != 0)
	narrow = FALSE;

      /* Turn rsb #0 into 16-bit neg.  We should probably do this via
	 relaxation, but it doesn't seem worth the hassle.  */
      if (narrow)
	{
	  inst.reloc.type = BFD_RELOC_UNUSED;
	  inst.instruction = THUMB_OP16 (T_MNEM_negs);
	  inst.instruction |= Rs << 3;
	  inst.instruction |= Rd;
	}
      else
	{
	  inst.instruction = (inst.instruction & 0xe1ffffff) | 0x10000000;
	  inst.reloc.type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
    }
  else
    encode_thumb32_shifted_operand (2);
}

static void
do_t_shift (void)
{
  if (!inst.operands[1].present)
    inst.operands[1].reg = inst.operands[0].reg;

  if (unified_syntax)
    {
      bfd_boolean narrow;
      int shift_kind;

      switch (inst.instruction)
	{
	case T_MNEM_asr:
	case T_MNEM_asrs: shift_kind = SHIFT_ASR; break;
	case T_MNEM_lsl:
	case T_MNEM_lsls: shift_kind = SHIFT_LSL; break;
	case T_MNEM_lsr:
	case T_MNEM_lsrs: shift_kind = SHIFT_LSR; break;
	case T_MNEM_ror:
	case T_MNEM_rors: shift_kind = SHIFT_ROR; break;
	default: abort ();
	}

      if (THUMB_SETS_FLAGS (inst.instruction))
	narrow = !in_it_block ();
      else
	narrow = in_it_block ();
      if (inst.operands[0].reg > 7 || inst.operands[1].reg > 7)
	narrow = FALSE;
      if (!inst.operands[2].isreg && shift_kind == SHIFT_ROR)
	narrow = FALSE;
      if (inst.operands[2].isreg
	  && (inst.operands[1].reg != inst.operands[0].reg
	      || inst.operands[2].reg > 7))
	narrow = FALSE;
      if (inst.size_req == 4)
	narrow = FALSE;

      reject_bad_reg (inst.operands[0].reg);
      reject_bad_reg (inst.operands[1].reg);

      if (!narrow)
	{
	  if (inst.operands[2].isreg)
	    {
	      reject_bad_reg (inst.operands[2].reg);
	      inst.instruction = THUMB_OP32 (inst.instruction);
	      inst.instruction |= inst.operands[0].reg << 8;
	      inst.instruction |= inst.operands[1].reg << 16;
	      inst.instruction |= inst.operands[2].reg;

	      /* PR 12854: Error on extraneous shifts.  */
	      constraint (inst.operands[2].shifted,
			  _("extraneous shift as part of operand to shift insn"));
	    }
	  else
	    {
	      inst.operands[1].shifted = 1;
	      inst.operands[1].shift_kind = shift_kind;
	      inst.instruction = THUMB_OP32 (THUMB_SETS_FLAGS (inst.instruction)
					     ? T_MNEM_movs : T_MNEM_mov);
	      inst.instruction |= inst.operands[0].reg << 8;
	      encode_thumb32_shifted_operand (1);
	      /* Prevent the incorrect generation of an ARM_IMMEDIATE fixup.  */
	      inst.reloc.type = BFD_RELOC_UNUSED;
	    }
	}
      else
	{
	  if (inst.operands[2].isreg)
	    {
	      switch (shift_kind)
		{
		case SHIFT_ASR: inst.instruction = T_OPCODE_ASR_R; break;
		case SHIFT_LSL: inst.instruction = T_OPCODE_LSL_R; break;
		case SHIFT_LSR: inst.instruction = T_OPCODE_LSR_R; break;
		case SHIFT_ROR: inst.instruction = T_OPCODE_ROR_R; break;
		default: abort ();
		}

	      inst.instruction |= inst.operands[0].reg;
	      inst.instruction |= inst.operands[2].reg << 3;

	      /* PR 12854: Error on extraneous shifts.  */
	      constraint (inst.operands[2].shifted,
			  _("extraneous shift as part of operand to shift insn"));
	    }
	  else
	    {
	      switch (shift_kind)
		{
		case SHIFT_ASR: inst.instruction = T_OPCODE_ASR_I; break;
		case SHIFT_LSL: inst.instruction = T_OPCODE_LSL_I; break;
		case SHIFT_LSR: inst.instruction = T_OPCODE_LSR_I; break;
		default: abort ();
		}
	      inst.reloc.type = BFD_RELOC_ARM_THUMB_SHIFT;
	      inst.instruction |= inst.operands[0].reg;
	      inst.instruction |= inst.operands[1].reg << 3;
	    }
	}
    }
  else
    {
      constraint (inst.operands[0].reg > 7
		  || inst.operands[1].reg > 7, BAD_HIREG);
      constraint (THUMB_SETS_FLAGS (inst.instruction), BAD_THUMB32);

      if (inst.operands[2].isreg)  /* Rd, {Rs,} Rn */
	{
	  constraint (inst.operands[2].reg > 7, BAD_HIREG);
	  constraint (inst.operands[0].reg != inst.operands[1].reg,
		      _("source1 and dest must be same register"));

	  switch (inst.instruction)
	    {
	    case T_MNEM_asr: inst.instruction = T_OPCODE_ASR_R; break;
	    case T_MNEM_lsl: inst.instruction = T_OPCODE_LSL_R; break;
	    case T_MNEM_lsr: inst.instruction = T_OPCODE_LSR_R; break;
	    case T_MNEM_ror: inst.instruction = T_OPCODE_ROR_R; break;
	    default: abort ();
	    }

	  inst.instruction |= inst.operands[0].reg;
	  inst.instruction |= inst.operands[2].reg << 3;

	  /* PR 12854: Error on extraneous shifts.  */
	  constraint (inst.operands[2].shifted,
		      _("extraneous shift as part of operand to shift insn"));
	}
      else
	{
	  switch (inst.instruction)
	    {
	    case T_MNEM_asr: inst.instruction = T_OPCODE_ASR_I; break;
	    case T_MNEM_lsl: inst.instruction = T_OPCODE_LSL_I; break;
	    case T_MNEM_lsr: inst.instruction = T_OPCODE_LSR_I; break;
	    case T_MNEM_ror: inst.error = _("ror #imm not supported"); return;
	    default: abort ();
	    }
	  inst.reloc.type = BFD_RELOC_ARM_THUMB_SHIFT;
	  inst.instruction |= inst.operands[0].reg;
	  inst.instruction |= inst.operands[1].reg << 3;
	}
    }
}

static void
do_t_swi (void)
{
  inst.reloc.type = BFD_RELOC_ARM_SWI;
}

static void
do_t_udf (void)
{
  if (!inst.operands[0].present)
    inst.operands[0].imm = 0;

  if ((unsigned int) inst.operands[0].imm > 255 || inst.size_req == 4)
    {
      constraint (inst.size_req == 2,
                  _("immediate value out of range"));
      inst.instruction = THUMB_OP32 (inst.instruction);
      inst.instruction |= (inst.operands[0].imm & 0xf000u) << 4;
      inst.instruction |= (inst.operands[0].imm & 0x0fffu) << 0;
    }
  else
    {
      inst.instruction = THUMB_OP16 (inst.instruction);
      inst.instruction |= inst.operands[0].imm;
    }

  set_it_insn_type (NEUTRAL_IT_INSN);
}

/* Neon instruction encoder helpers.  */

/* Encodings for the different types for various Neon opcodes.  */

#define check_neon_suffixes						\
  do									\
    {									\
      if (!inst.error && inst.vectype.elems > 0 && !inst.is_neon)	\
	{								\
	  as_bad (_("invalid neon suffix for non neon instruction"));	\
	  return;							\
	}								\
    }									\
  while (0)

/* Look up and encode a simple mnemonic, for use as a helper function for the
   Neon-style VFP syntax.  This avoids duplication of bits of the insns table,
   etc.  It is assumed that operand parsing has already been done, and that the
   operands are in the form expected by the given opcode (this isn't necessarily
   the same as the form in which they were parsed, hence some massaging must
   take place before this function is called).
   Checks current arch version against that in the looked-up opcode.  */

static void
do_vfp_nsyn_opcode (const char *opname)
{
  const struct asm_opcode *opcode;

  opcode = (const struct asm_opcode *) hash_find (arm_ops_hsh, opname);

  if (!opcode)
    abort ();

  constraint (!ARM_CPU_HAS_FEATURE (cpu_variant,
		thumb_mode ? *opcode->tvariant : *opcode->avariant),
	      _(BAD_FPU));

  inst.is_neon = 1;

  if (thumb_mode)
    {
      inst.instruction = opcode->tvalue;
      opcode->tencode ();
    }
  else
    {
      inst.instruction = (inst.cond << 28) | opcode->avalue;
      opcode->aencode ();
    }
}

#define LOW4(R) ((R) & 0xf)
#define HI1(R) (((R) >> 4) & 1)

/* Overall per-instruction processing.	*/

/* We need to be able to fix up arbitrary expressions in some statements.
   This is so that we can handle symbols that are an arbitrary distance from
   the pc.  The most common cases are of the form ((+/-sym -/+ . - 8) & mask),
   which returns part of an address in a form which will be valid for
   a data instruction.	We do this by pushing the expression into a symbol
   in the expr_section, and creating a fix for that.  */

static void
fix_new_arm (fragS *	   frag,
	     int	   where,
	     short int	   size,
	     expressionS * exp,
	     int	   pc_rel,
	     int	   reloc)
{
  fixS *	   new_fix;

  switch (exp->X_op)
    {
    case O_constant:
      if (pc_rel)
	{
	  /* Create an absolute valued symbol, so we have something to
	     refer to in the object file.  Unfortunately for us, gas's
	     generic expression parsing will already have folded out
	     any use of .set foo/.type foo %function that may have
	     been used to set type information of the target location,
	     that's being specified symbolically.  We have to presume
	     the user knows what they are doing.  */
	  char name[16 + 8];
	  symbolS *symbol;

	  sprintf (name, "*ABS*0x%lx", (unsigned long)exp->X_add_number);

	  symbol = symbol_find_or_make (name);
	  S_SET_SEGMENT (symbol, absolute_section);
	  symbol_set_frag (symbol, &zero_address_frag);
	  S_SET_VALUE (symbol, exp->X_add_number);
	  exp->X_op = O_symbol;
	  exp->X_add_symbol = symbol;
	  exp->X_add_number = 0;
	}
      /* FALLTHROUGH */
    case O_symbol:
    case O_add:
    case O_subtract:
      new_fix = fix_new_exp (frag, where, size, exp, pc_rel,
			     (enum bfd_reloc_code_real) reloc);
      break;

    default:
      new_fix = (fixS *) fix_new (frag, where, size, make_expr_symbol (exp), 0,
				  pc_rel, (enum bfd_reloc_code_real) reloc);
      break;
    }

  /* Mark whether the fix is to a THUMB instruction, or an ARM
     instruction.  */
  new_fix->tc_fix_data = thumb_mode;
}

/* Create a frg for an instruction requiring relaxation.  */
static void
output_relax_insn (void)
{
  char * to;
  symbolS *sym;
  int offset;

  /* The size of the instruction is unknown, so tie the debug info to the
     start of the instruction.  */
  dwarf2_emit_insn (0);

  switch (inst.reloc.exp.X_op)
    {
    case O_symbol:
      sym = inst.reloc.exp.X_add_symbol;
      offset = inst.reloc.exp.X_add_number;
      break;
    case O_constant:
      sym = NULL;
      offset = inst.reloc.exp.X_add_number;
      break;
    default:
      sym = make_expr_symbol (&inst.reloc.exp);
      offset = 0;
      break;
  }
  to = frag_var (rs_machine_dependent, INSN_SIZE, THUMB_SIZE,
		 inst.relax, sym, offset, NULL/*offset, opcode*/);
  md_number_to_chars (to, inst.instruction, THUMB_SIZE);
}

/* Write a 32-bit thumb instruction to buf.  */
static void
put_thumb32_insn (char * buf, unsigned long insn)
{
  md_number_to_chars (buf, insn >> 16, THUMB_SIZE);
  md_number_to_chars (buf + THUMB_SIZE, insn, THUMB_SIZE);
}

static void
output_inst (const char * str)
{
  char * to = NULL;

  if (inst.error)
    {
      as_bad ("%s -- `%s'", inst.error, str);
      return;
    }
  if (inst.relax)
    {
      output_relax_insn ();
      return;
    }
  if (inst.size == 0)
    return;

  to = frag_more (inst.size);
  /* PR 9814: Record the thumb mode into the current frag so that we know
     what type of NOP padding to use, if necessary.  We override any previous
     setting so that if the mode has changed then the NOPS that we use will
     match the encoding of the last instruction in the frag.  */
  frag_now->tc_frag_data.thumb_mode = thumb_mode | MODE_RECORDED;

  if (thumb_mode && (inst.size > THUMB_SIZE))
    {
      gas_assert (inst.size == (2 * THUMB_SIZE));
      put_thumb32_insn (to, inst.instruction);
    }
  else if (inst.size > INSN_SIZE)
    {
      gas_assert (inst.size == (2 * INSN_SIZE));
      md_number_to_chars (to, inst.instruction, INSN_SIZE);
      md_number_to_chars (to + INSN_SIZE, inst.instruction, INSN_SIZE);
    }
  else
    md_number_to_chars (to, inst.instruction, inst.size);

  if (inst.reloc.type != BFD_RELOC_UNUSED)
    fix_new_arm (frag_now, to - frag_now->fr_literal,
		 inst.size, & inst.reloc.exp, inst.reloc.pc_rel,
		 inst.reloc.type);

  dwarf2_emit_insn (inst.size);
}

static char *
output_it_inst (int cond, int mask, char * to)
{
  unsigned long instruction = 0xbf00;

  mask &= 0xf;
  instruction |= mask;
  instruction |= cond << 4;

  if (to == NULL)
    {
      to = frag_more (2);
      dwarf2_emit_insn (2);
    }

  md_number_to_chars (to, instruction, 2);

  return to;
}

/* Tag values used in struct asm_opcode's tag field.  */
enum opcode_tag
{
  OT_unconditional,	/* Instruction cannot be conditionalized.
			   The ARM condition field is still 0xE.  */
  OT_unconditionalF,	/* Instruction cannot be conditionalized
			   and carries 0xF in its ARM condition field.  */
  OT_csuffix,		/* Instruction takes a conditional suffix.  */
  OT_csuffixF,		/* Some forms of the instruction take a conditional
			   suffix, others place 0xF where the condition field
			   would be.  */
  OT_cinfix3,		/* Instruction takes a conditional infix,
			   beginning at character index 3.  (In
			   unified mode, it becomes a suffix.)  */
  OT_cinfix3_deprecated, /* The same as OT_cinfix3.  This is used for
			    tsts, cmps, cmns, and teqs. */
  OT_cinfix3_legacy,	/* Legacy instruction takes a conditional infix at
			   character index 3, even in unified mode.  Used for
			   legacy instructions where suffix and infix forms
			   may be ambiguous.  */
  OT_csuf_or_in3,	/* Instruction takes either a conditional
			   suffix or an infix at character index 3.  */
  OT_odd_infix_unc,	/* This is the unconditional variant of an
			   instruction that takes a conditional infix
			   at an unusual position.  In unified mode,
			   this variant will accept a suffix.  */
  OT_odd_infix_0	/* Values greater than or equal to OT_odd_infix_0
			   are the conditional variants of instructions that
			   take conditional infixes in unusual positions.
			   The infix appears at character index
			   (tag - OT_odd_infix_0).  These are not accepted
			   in unified mode.  */
};

/* Subroutine of md_assemble, responsible for looking up the primary
   opcode from the mnemonic the user wrote.  STR points to the
   beginning of the mnemonic.

   This is not simply a hash table lookup, because of conditional
   variants.  Most instructions have conditional variants, which are
   expressed with a _conditional affix_ to the mnemonic.  If we were
   to encode each conditional variant as a literal string in the opcode
   table, it would have approximately 20,000 entries.

   Most mnemonics take this affix as a suffix, and in unified syntax,
   'most' is upgraded to 'all'.  However, in the divided syntax, some
   instructions take the affix as an infix, notably the s-variants of
   the arithmetic instructions.  Of those instructions, all but six
   have the infix appear after the third character of the mnemonic.

   Accordingly, the algorithm for looking up primary opcodes given
   an identifier is:

   1. Look up the identifier in the opcode table.
      If we find a match, go to step U.

   2. Look up the last two characters of the identifier in the
      conditions table.  If we find a match, look up the first N-2
      characters of the identifier in the opcode table.  If we
      find a match, go to step CE.

   3. Look up the fourth and fifth characters of the identifier in
      the conditions table.  If we find a match, extract those
      characters from the identifier, and look up the remaining
      characters in the opcode table.  If we find a match, go
      to step CM.

   4. Fail.

   U. Examine the tag field of the opcode structure, in case this is
      one of the six instructions with its conditional infix in an
      unusual place.  If it is, the tag tells us where to find the
      infix; look it up in the conditions table and set inst.cond
      accordingly.  Otherwise, this is an unconditional instruction.
      Again set inst.cond accordingly.  Return the opcode structure.

  CE. Examine the tag field to make sure this is an instruction that
      should receive a conditional suffix.  If it is not, fail.
      Otherwise, set inst.cond from the suffix we already looked up,
      and return the opcode structure.

  CM. Examine the tag field to make sure this is an instruction that
      should receive a conditional infix after the third character.
      If it is not, fail.  Otherwise, undo the edits to the current
      line of input and proceed as for case CE.  */

static const struct asm_opcode *
opcode_lookup (char **str)
{
  char *end, *base;
  char *affix;
  const struct asm_opcode *opcode;
  const struct asm_cond *cond;
  char save[2];

  /* Scan up to the end of the mnemonic, which must end in white space,
     '.' (in unified mode, or for Neon/VFP instructions), or end of string.  */
  for (base = end = *str; *end != '\0'; end++)
    if (*end == ' ' || *end == '.')
      break;

  if (end == base)
    return NULL;

  /* Handle a possible width suffix and/or Neon type suffix.  */
  if (end[0] == '.')
    {
      int offset = 2;

      /* The .w and .n suffixes are only valid if the unified syntax is in
	 use.  */
      if (unified_syntax && end[1] == 'w')
	inst.size_req = 4;
      else if (unified_syntax && end[1] == 'n')
	inst.size_req = 2;
      else
	offset = 0;

      inst.vectype.elems = 0;

      *str = end + offset;

      if (end[offset] == '.')
	{
	  /* See if we have a Neon type suffix (possible in either unified or
	     non-unified ARM syntax mode).  */
	  if (parse_neon_type (&inst.vectype, str) == FAIL)
	    return NULL;
	}
      else if (end[offset] != '\0' && end[offset] != ' ')
	return NULL;
    }
  else
    *str = end;

  /* Look for unaffixed or special-case affixed mnemonic.  */
  opcode = (const struct asm_opcode *) hash_find_n (arm_ops_hsh, base,
						    end - base);
  if (opcode)
    {
      /* step U */
      if (opcode->tag < OT_odd_infix_0)
	{
	  inst.cond = COND_ALWAYS;
	  return opcode;
	}

      if (warn_on_deprecated && unified_syntax)
	as_tsktsk (_("conditional infixes are deprecated in unified syntax"));
      affix = base + (opcode->tag - OT_odd_infix_0);
      cond = (const struct asm_cond *) hash_find_n (arm_cond_hsh, affix, 2);
      gas_assert (cond);

      inst.cond = cond->value;
      return opcode;
    }

  /* Cannot have a conditional suffix on a mnemonic of less than two
     characters.  */
  if (end - base < 3)
    return NULL;

  /* Look for suffixed mnemonic.  */
  affix = end - 2;
  cond = (const struct asm_cond *) hash_find_n (arm_cond_hsh, affix, 2);
  opcode = (const struct asm_opcode *) hash_find_n (arm_ops_hsh, base,
						    affix - base);
  if (opcode && cond)
    {
      /* step CE */
      switch (opcode->tag)
	{
	case OT_cinfix3_legacy:
	  /* Ignore conditional suffixes matched on infix only mnemonics.  */
	  break;

	case OT_cinfix3:
	case OT_cinfix3_deprecated:
	case OT_odd_infix_unc:
	  if (!unified_syntax)
	    return NULL;
	  /* Fall through.  */

	case OT_csuffix:
	case OT_csuffixF:
	case OT_csuf_or_in3:
	  inst.cond = cond->value;
	  return opcode;

	case OT_unconditional:
	case OT_unconditionalF:
	  if (thumb_mode)
	    inst.cond = cond->value;
	  else
	    {
	      /* Delayed diagnostic.  */
	      inst.error = BAD_COND;
	      inst.cond = COND_ALWAYS;
	    }
	  return opcode;

	default:
	  return NULL;
	}
    }

  /* Cannot have a usual-position infix on a mnemonic of less than
     six characters (five would be a suffix).  */
  if (end - base < 6)
    return NULL;

  /* Look for infixed mnemonic in the usual position.  */
  affix = base + 3;
  cond = (const struct asm_cond *) hash_find_n (arm_cond_hsh, affix, 2);
  if (!cond)
    return NULL;

  memcpy (save, affix, 2);
  memmove (affix, affix + 2, (end - affix) - 2);
  opcode = (const struct asm_opcode *) hash_find_n (arm_ops_hsh, base,
						    (end - base) - 2);
  memmove (affix + 2, affix, (end - affix) - 2);
  memcpy (affix, save, 2);

  if (opcode
      && (opcode->tag == OT_cinfix3
	  || opcode->tag == OT_cinfix3_deprecated
	  || opcode->tag == OT_csuf_or_in3
	  || opcode->tag == OT_cinfix3_legacy))
    {
      /* Step CM.  */
      if (warn_on_deprecated && unified_syntax
	  && (opcode->tag == OT_cinfix3
	      || opcode->tag == OT_cinfix3_deprecated))
	as_tsktsk (_("conditional infixes are deprecated in unified syntax"));

      inst.cond = cond->value;
      return opcode;
    }

  return NULL;
}

/* This function generates an initial IT instruction, leaving its block
   virtually open for the new instructions. Eventually,
   the mask will be updated by now_it_add_mask () each time
   a new instruction needs to be included in the IT block.
   Finally, the block is closed with close_automatic_it_block ().
   The block closure can be requested either from md_assemble (),
   a tencode (), or due to a label hook.  */

static void
new_automatic_it_block (int cond)
{
  now_it.state = AUTOMATIC_IT_BLOCK;
  now_it.mask = 0x18;
  now_it.cc = cond;
  now_it.block_length = 1;
  mapping_state (MAP_THUMB);
  now_it.insn = output_it_inst (cond, now_it.mask, NULL);
  now_it.warn_deprecated = FALSE;
  now_it.insn_cond = TRUE;
}

/* Close an automatic IT block.
   See comments in new_automatic_it_block ().  */

static void
close_automatic_it_block (void)
{
  now_it.mask = 0x10;
  now_it.block_length = 0;
}

/* Update the mask of the current automatically-generated IT
   instruction. See comments in new_automatic_it_block ().  */

static void
now_it_add_mask (int cond)
{
#define CLEAR_BIT(value, nbit)  ((value) & ~(1 << (nbit)))
#define SET_BIT_VALUE(value, bitvalue, nbit)  (CLEAR_BIT (value, nbit) \
					      | ((bitvalue) << (nbit)))
  const int resulting_bit = (cond & 1);

  now_it.mask &= 0xf;
  now_it.mask = SET_BIT_VALUE (now_it.mask,
				   resulting_bit,
				  (5 - now_it.block_length));
  now_it.mask = SET_BIT_VALUE (now_it.mask,
				   1,
				   ((5 - now_it.block_length) - 1) );
  output_it_inst (now_it.cc, now_it.mask, now_it.insn);

#undef CLEAR_BIT
#undef SET_BIT_VALUE
}

/* The IT blocks handling machinery is accessed through the these functions:
     it_fsm_pre_encode ()               from md_assemble ()
     set_it_insn_type ()                optional, from the tencode functions
     set_it_insn_type_last ()           ditto
     in_it_block ()                     ditto
     it_fsm_post_encode ()              from md_assemble ()
     force_automatic_it_block_close ()  from label handling functions

   Rationale:
     1) md_assemble () calls it_fsm_pre_encode () before calling tencode (),
	initializing the IT insn type with a generic initial value depending
	on the inst.condition.
     2) During the tencode function, two things may happen:
	a) The tencode function overrides the IT insn type by
	   calling either set_it_insn_type (type) or set_it_insn_type_last ().
	b) The tencode function queries the IT block state by
	   calling in_it_block () (i.e. to determine narrow/not narrow mode).

	Both set_it_insn_type and in_it_block run the internal FSM state
	handling function (handle_it_state), because: a) setting the IT insn
	type may incur in an invalid state (exiting the function),
	and b) querying the state requires the FSM to be updated.
	Specifically we want to avoid creating an IT block for conditional
	branches, so it_fsm_pre_encode is actually a guess and we can't
	determine whether an IT block is required until the tencode () routine
	has decided what type of instruction this actually it.
	Because of this, if set_it_insn_type and in_it_block have to be used,
	set_it_insn_type has to be called first.

	set_it_insn_type_last () is a wrapper of set_it_insn_type (type), that
	determines the insn IT type depending on the inst.cond code.
	When a tencode () routine encodes an instruction that can be
	either outside an IT block, or, in the case of being inside, has to be
	the last one, set_it_insn_type_last () will determine the proper
	IT instruction type based on the inst.cond code. Otherwise,
	set_it_insn_type can be called for overriding that logic or
	for covering other cases.

	Calling handle_it_state () may not transition the IT block state to
	OUTSIDE_IT_BLOCK immediately, since the (current) state could be
	still queried. Instead, if the FSM determines that the state should
	be transitioned to OUTSIDE_IT_BLOCK, a flag is marked to be closed
	after the tencode () function: that's what it_fsm_post_encode () does.

	Since in_it_block () calls the state handling function to get an
	updated state, an error may occur (due to invalid insns combination).
	In that case, inst.error is set.
	Therefore, inst.error has to be checked after the execution of
	the tencode () routine.

     3) Back in md_assemble(), it_fsm_post_encode () is called to commit
	any pending state change (if any) that didn't take place in
	handle_it_state () as explained above.  */

static void
it_fsm_pre_encode (void)
{
  if (inst.cond != COND_ALWAYS)
    inst.it_insn_type = INSIDE_IT_INSN;
  else
    inst.it_insn_type = OUTSIDE_IT_INSN;

  now_it.state_handled = 0;
}

/* IT state FSM handling function.  */

static int
handle_it_state (void)
{
  now_it.state_handled = 1;
  now_it.insn_cond = FALSE;

  switch (now_it.state)
    {
    case OUTSIDE_IT_BLOCK:
      switch (inst.it_insn_type)
	{
	case OUTSIDE_IT_INSN:
	  break;

	case INSIDE_IT_INSN:
	case INSIDE_IT_LAST_INSN:
	  if (thumb_mode == 0)
	    {
	      if (unified_syntax
		  && !(implicit_it_mode & IMPLICIT_IT_MODE_ARM))
		as_tsktsk (_("Warning: conditional outside an IT block"\
			     " for Thumb."));
	    }
	  else
	    {
		  inst.error = BAD_OUT_IT;
		  return FAIL;
	    }
	  break;

	case IF_INSIDE_IT_LAST_INSN:
	case NEUTRAL_IT_INSN:
	  break;

	case IT_INSN:
	  now_it.state = MANUAL_IT_BLOCK;
	  now_it.block_length = 0;
	  break;
	}
      break;

    case AUTOMATIC_IT_BLOCK:
      /* Three things may happen now:
	 a) We should increment current it block size;
	 b) We should close current it block (closing insn or 4 insns);
	 c) We should close current it block and start a new one (due
	 to incompatible conditions or
	 4 insns-length block reached).  */

      switch (inst.it_insn_type)
	{
	case OUTSIDE_IT_INSN:
	  /* The closure of the block shall happen immediately,
	     so any in_it_block () call reports the block as closed.  */
	  force_automatic_it_block_close ();
	  break;

	case INSIDE_IT_INSN:
	case INSIDE_IT_LAST_INSN:
	case IF_INSIDE_IT_LAST_INSN:
	  now_it.block_length++;

	  if (now_it.block_length > 4
	      || !now_it_compatible (inst.cond))
	    {
	      force_automatic_it_block_close ();
	      if (inst.it_insn_type != IF_INSIDE_IT_LAST_INSN)
		new_automatic_it_block (inst.cond);
	    }
	  else
	    {
	      now_it.insn_cond = TRUE;
	      now_it_add_mask (inst.cond);
	    }

	  if (now_it.state == AUTOMATIC_IT_BLOCK
	      && (inst.it_insn_type == INSIDE_IT_LAST_INSN
		  || inst.it_insn_type == IF_INSIDE_IT_LAST_INSN))
	    close_automatic_it_block ();
	  break;

	case NEUTRAL_IT_INSN:
	  now_it.block_length++;
	  now_it.insn_cond = TRUE;

	  if (now_it.block_length > 4)
	    force_automatic_it_block_close ();
	  else
	    now_it_add_mask (now_it.cc & 1);
	  break;

	case IT_INSN:
	  close_automatic_it_block ();
	  now_it.state = MANUAL_IT_BLOCK;
	  break;
	}
      break;

    case MANUAL_IT_BLOCK:
      {
	/* Check conditional suffixes.  */
	const int cond = now_it.cc ^ ((now_it.mask >> 4) & 1) ^ 1;
	int is_last;
	now_it.mask <<= 1;
	now_it.mask &= 0x1f;
	is_last = (now_it.mask == 0x10);
	now_it.insn_cond = TRUE;

	switch (inst.it_insn_type)
	  {
	  case OUTSIDE_IT_INSN:
	    inst.error = BAD_NOT_IT;
	    return FAIL;

	  case INSIDE_IT_INSN:
	    if (cond != inst.cond)
	      {
		inst.error = BAD_IT_COND;
		return FAIL;
	      }
	    break;

	  case INSIDE_IT_LAST_INSN:
	  case IF_INSIDE_IT_LAST_INSN:
	    if (cond != inst.cond)
	      {
		inst.error = BAD_IT_COND;
		return FAIL;
	      }
	    if (!is_last)
	      {
		inst.error = BAD_BRANCH;
		return FAIL;
	      }
	    break;

	  case NEUTRAL_IT_INSN:
	    /* The BKPT instruction is unconditional even in an IT block.  */
	    break;

	  case IT_INSN:
	    inst.error = BAD_IT_IT;
	    return FAIL;
	  }
      }
      break;
    }

  return SUCCESS;
}

struct depr_insn_mask
{
  unsigned long pattern;
  unsigned long mask;
  const char* description;
};

static void
it_fsm_post_encode (void)
{
  int is_last;

  if (!now_it.state_handled)
    handle_it_state ();
  is_last = (now_it.mask == 0x10);
  if (is_last)
    {
      now_it.state = OUTSIDE_IT_BLOCK;
      now_it.mask = 0;
    }
}

static void
force_automatic_it_block_close (void)
{
  if (now_it.state == AUTOMATIC_IT_BLOCK)
    {
      close_automatic_it_block ();
      now_it.state = OUTSIDE_IT_BLOCK;
      now_it.mask = 0;
    }
}

static int
in_it_block (void)
{
  if (!now_it.state_handled)
    handle_it_state ();

  return now_it.state != OUTSIDE_IT_BLOCK;
}

static bfd_boolean
known_t32_only_insn (const struct asm_opcode *opcode)
{
  /* Original Thumb-1 wide instruction.  */
  if (opcode->tencode == do_t_blx
      || opcode->tencode == do_t_branch23
      || ARM_CPU_HAS_FEATURE (*opcode->tvariant, arm_ext_msr))

    return TRUE;

  /* Wide-only instruction added to ARMv8-M Baseline.  */

  return FALSE;
}

/* Whether wide instruction variant can be used if available for a valid OPCODE
   in ARCH.  */

static bfd_boolean
t32_insn_ok (arm_feature_set arch, const struct asm_opcode *opcode)
{
  if (known_t32_only_insn (opcode))
    return TRUE;

  return FALSE;
}

void
md_assemble (char *str)
{
  char *p = str;
  const struct asm_opcode * opcode;

  /* Align the previous label if needed.  */
  if (last_label_seen != NULL)
    {
      symbol_set_frag (last_label_seen, frag_now);
      S_SET_VALUE (last_label_seen, (valueT) frag_now_fix ());
      S_SET_SEGMENT (last_label_seen, now_seg);
    }

  memset (&inst, '\0', sizeof (inst));
  inst.reloc.type = BFD_RELOC_UNUSED;

  opcode = opcode_lookup (&p);
  if (!opcode)
    {
      /* It wasn't an instruction, but it might be a register alias of
	 the form alias .req reg, or a Neon .dn/.qn directive.  */
      if (! create_register_alias (str, p)
	  && ! create_neon_reg_alias (str, p))
	as_bad (_("bad instruction `%s'"), str);

      return;
    }

  if (warn_on_deprecated && opcode->tag == OT_cinfix3_deprecated)
    as_tsktsk (_("s suffix on comparison instruction is deprecated"));

  /* The value which unconditional instructions should have in place of the
     condition field.  */
  inst.uncond_value = (opcode->tag == OT_csuffixF) ? 0xf : -1;

  if (thumb_mode)
    {
      arm_feature_set variant;

      variant = cpu_variant;
      /* Only allow coprocessor instructions on Thumb-2 capable devices.  */
      /* Check that this instruction is supported for this CPU.  */
      if (!opcode->tvariant
	  || (thumb_mode == 1
	      && !ARM_CPU_HAS_FEATURE (variant, *opcode->tvariant)))
	{
	  if (opcode->tencode == do_t_swi)
	    as_bad (_("SVC is not permitted on this architecture"));
	  else
	    as_bad (_("selected processor does not support `%s' in Thumb mode"), str);
	  return;
	}
      if (inst.cond != COND_ALWAYS && !unified_syntax
	  && opcode->tencode != do_t_branch)
	{
	  as_bad (_("Thumb does not support conditional execution"));
	  return;
	}

      /* Two things are addressed here:
	 1) Implicit require narrow instructions on Thumb-1.
	    This avoids relaxation accidentally introducing Thumb-2
	    instructions.
	 2) Reject wide instructions in non Thumb-2 cores.

	 Only instructions with narrow and wide variants need to be handled
	 but selecting all non wide-only instructions is easier.  */
      if (!t32_insn_ok (variant, opcode))
	{
	  if (inst.size_req == 0)
	    inst.size_req = 2;
	  else if (inst.size_req == 4)
	    {
		as_bad (_("selected processor does not support `%s' in "
			  "Thumb-2 mode"), str);
	      return;
	    }
	}

      inst.instruction = opcode->tvalue;

      if (!parse_operands (p, opcode->operands, /*thumb=*/TRUE))
	{
	  /* Prepare the it_insn_type for those encodings that don't set
	     it.  */
	  it_fsm_pre_encode ();

	  opcode->tencode ();

	  it_fsm_post_encode ();
	}

      if (!(inst.error || inst.relax))
	{
	  gas_assert (inst.instruction < 0xe800 || inst.instruction > 0xffff);
	  inst.size = (inst.instruction > 0xffff ? 4 : 2);
	  if (inst.size_req && inst.size_req != inst.size)
	    {
	      as_bad (_("cannot honor width suffix -- `%s'"), str);
	      return;
	    }
	}

      /* Something has gone badly wrong if we try to relax a fixed size
	 instruction.  */
      gas_assert (inst.size_req == 0 || !inst.relax);

      ARM_MERGE_FEATURE_SETS (thumb_arch_used, thumb_arch_used,
			      *opcode->tvariant);
      /* Many Thumb-2 instructions also have Thumb-1 variants, so explicitly
	 set those bits when Thumb-2 32-bit instructions are seen.  The impact
	 of relaxable instructions will be considered later after we finish all
	 relaxation.  */
      if (ARM_FEATURE_CORE_EQUAL (cpu_variant, arm_arch_any))
	variant = arm_arch_none;
      else
	variant = cpu_variant;

      check_neon_suffixes;

      if (!inst.error)
	{
	  mapping_state (MAP_THUMB);
	}
    }
  else if (ARM_CPU_HAS_FEATURE (cpu_variant, arm_ext_v1))
    {
      bfd_boolean is_bx;

      /* bx is allowed on v5 cores, and sometimes on v4 cores.  */
      is_bx = (opcode->aencode == do_bx);

      /* Check that this instruction is supported for this CPU.  */
      if (!(is_bx && fix_v4bx)
	  && !(opcode->avariant &&
	       ARM_CPU_HAS_FEATURE (cpu_variant, *opcode->avariant)))
	{
	  as_bad (_("selected processor does not support `%s' in ARM mode"), str);
	  return;
	}
      if (inst.size_req)
	{
	  as_bad (_("width suffixes are invalid in ARM mode -- `%s'"), str);
	  return;
	}

      inst.instruction = opcode->avalue;
      if (opcode->tag == OT_unconditionalF)
	inst.instruction |= 0xFU << 28;
      else
	inst.instruction |= inst.cond << 28;
      inst.size = INSN_SIZE;
      if (!parse_operands (p, opcode->operands, /*thumb=*/FALSE))
	{
	  it_fsm_pre_encode ();
	  opcode->aencode ();
	  it_fsm_post_encode ();
	}
      /* Arm mode bx is marked as both v4T and v5 because it's still required
	 on a hypothetical non-thumb v5 core.  */
      if (is_bx)
	ARM_MERGE_FEATURE_SETS (arm_arch_used, arm_arch_used, arm_ext_v4t);
      else
	ARM_MERGE_FEATURE_SETS (arm_arch_used, arm_arch_used,
				*opcode->avariant);

      check_neon_suffixes;

      if (!inst.error)
	{
	  mapping_state (MAP_ARM);
	}
    }
  else
    {
      as_bad (_("attempt to use an ARM instruction on a Thumb-only processor "
		"-- `%s'"), str);
      return;
    }
  output_inst (str);
}

static void
check_it_blocks_finished (void)
{
  asection *sect;

  for (sect = stdoutput->sections; sect != NULL; sect = sect->next)
    if (seg_info (sect)->tc_segment_info_data.current_it.state
	== MANUAL_IT_BLOCK)
      {
	as_warn (_("section '%s' finished with an open IT block."),
		 sect->name);
      }
}

/* Various frobbings of labels and their addresses.  */

void
arm_start_line_hook (void)
{
  last_label_seen = NULL;
}

void
arm_frob_label (symbolS * sym)
{
  last_label_seen = sym;

  ARM_SET_THUMB (sym, thumb_mode);

  ARM_SET_INTERWORK (sym, support_interwork);

  force_automatic_it_block_close ();

  /* Note - do not allow local symbols (.Lxxx) to be labelled
     as Thumb functions.  This is because these labels, whilst
     they exist inside Thumb code, are not the entry points for
     possible ARM->Thumb calls.	 Also, these labels can be used
     as part of a computed goto or switch statement.  eg gcc
     can generate code that looks like this:

		ldr  r2, [pc, .Laaa]
		lsl  r3, r3, #2
		ldr  r2, [r3, r2]
		mov  pc, r2

       .Lbbb:  .word .Lxxx
       .Lccc:  .word .Lyyy
       ..etc...
       .Laaa:	.word Lbbb

     The first instruction loads the address of the jump table.
     The second instruction converts a table index into a byte offset.
     The third instruction gets the jump address out of the table.
     The fourth instruction performs the jump.

     If the address stored at .Laaa is that of a symbol which has the
     Thumb_Func bit set, then the linker will arrange for this address
     to have the bottom bit set, which in turn would mean that the
     address computation performed by the third instruction would end
     up with the bottom bit set.  Since the ARM is capable of unaligned
     word loads, the instruction would then load the incorrect address
     out of the jump table, and chaos would ensue.  */
  if (label_is_thumb_function_name
      && (S_GET_NAME (sym)[0] != '.' || S_GET_NAME (sym)[1] != 'L')
      && (bfd_get_section_flags (stdoutput, now_seg) & SEC_CODE) != 0)
    {
      /* When the address of a Thumb function is taken the bottom
	 bit of that address should be set.  This will allow
	 interworking between Arm and Thumb functions to work
	 correctly.  */

      THUMB_SET_FUNC (sym, 1);

      label_is_thumb_function_name = FALSE;
    }

  dwarf2_emit_label (sym);
}

bfd_boolean
arm_data_in_code (void)
{
  if (thumb_mode && ! strncmp (input_line_pointer + 1, "data:", 5))
    {
      *input_line_pointer = '/';
      input_line_pointer += 5;
      *input_line_pointer = 0;
      return TRUE;
    }

  return FALSE;
}

char *
arm_canonicalize_symbol_name (char * name)
{
  int len;

  if (thumb_mode && (len = strlen (name)) > 5
      && streq (name + len - 5, "/data"))
    *(name + len - 5) = 0;

  return name;
}

/* Table of all register names defined by default.  The user can
   define additional names with .req.  Note that all register names
   should appear in both upper and lowercase variants.	Some registers
   also have mixed-case names.	*/

#define REGDEF(s,n,t) { #s, n, REG_TYPE_##t, TRUE, 0 }
#define REGNUM(p,n,t) REGDEF(p##n, n, t)
#define REGNUM2(p,n,t) REGDEF(p##n, 2 * n, t)
#define REGSET(p,t) \
  REGNUM(p, 0,t), REGNUM(p, 1,t), REGNUM(p, 2,t), REGNUM(p, 3,t), \
  REGNUM(p, 4,t), REGNUM(p, 5,t), REGNUM(p, 6,t), REGNUM(p, 7,t), \
  REGNUM(p, 8,t), REGNUM(p, 9,t), REGNUM(p,10,t), REGNUM(p,11,t), \
  REGNUM(p,12,t), REGNUM(p,13,t), REGNUM(p,14,t), REGNUM(p,15,t)
#define REGSETH(p,t) \
  REGNUM(p,16,t), REGNUM(p,17,t), REGNUM(p,18,t), REGNUM(p,19,t), \
  REGNUM(p,20,t), REGNUM(p,21,t), REGNUM(p,22,t), REGNUM(p,23,t), \
  REGNUM(p,24,t), REGNUM(p,25,t), REGNUM(p,26,t), REGNUM(p,27,t), \
  REGNUM(p,28,t), REGNUM(p,29,t), REGNUM(p,30,t), REGNUM(p,31,t)
#define REGSET2(p,t) \
  REGNUM2(p, 0,t), REGNUM2(p, 1,t), REGNUM2(p, 2,t), REGNUM2(p, 3,t), \
  REGNUM2(p, 4,t), REGNUM2(p, 5,t), REGNUM2(p, 6,t), REGNUM2(p, 7,t), \
  REGNUM2(p, 8,t), REGNUM2(p, 9,t), REGNUM2(p,10,t), REGNUM2(p,11,t), \
  REGNUM2(p,12,t), REGNUM2(p,13,t), REGNUM2(p,14,t), REGNUM2(p,15,t)
#define SPLRBANK(base,bank,t) \
  REGDEF(lr_##bank, 768|((base+0)<<16), t), \
  REGDEF(sp_##bank, 768|((base+1)<<16), t), \
  REGDEF(spsr_##bank, 768|(base<<16)|SPSR_BIT, t), \
  REGDEF(LR_##bank, 768|((base+0)<<16), t), \
  REGDEF(SP_##bank, 768|((base+1)<<16), t), \
  REGDEF(SPSR_##bank, 768|(base<<16)|SPSR_BIT, t)

static const struct reg_entry reg_names[] =
{
  /* ARM integer registers.  */
  REGSET(r, RN), REGSET(R, RN),

  /* ATPCS synonyms.  */
  REGDEF(a1,0,RN), REGDEF(a2,1,RN), REGDEF(a3, 2,RN), REGDEF(a4, 3,RN),
  REGDEF(v1,4,RN), REGDEF(v2,5,RN), REGDEF(v3, 6,RN), REGDEF(v4, 7,RN),
  REGDEF(v5,8,RN), REGDEF(v6,9,RN), REGDEF(v7,10,RN), REGDEF(v8,11,RN),

  REGDEF(A1,0,RN), REGDEF(A2,1,RN), REGDEF(A3, 2,RN), REGDEF(A4, 3,RN),
  REGDEF(V1,4,RN), REGDEF(V2,5,RN), REGDEF(V3, 6,RN), REGDEF(V4, 7,RN),
  REGDEF(V5,8,RN), REGDEF(V6,9,RN), REGDEF(V7,10,RN), REGDEF(V8,11,RN),

  /* Well-known aliases.  */
  REGDEF(wr, 7,RN), REGDEF(sb, 9,RN), REGDEF(sl,10,RN), REGDEF(fp,11,RN),
  REGDEF(ip,12,RN), REGDEF(sp,13,RN), REGDEF(lr,14,RN), REGDEF(pc,15,RN),

  REGDEF(WR, 7,RN), REGDEF(SB, 9,RN), REGDEF(SL,10,RN), REGDEF(FP,11,RN),
  REGDEF(IP,12,RN), REGDEF(SP,13,RN), REGDEF(LR,14,RN), REGDEF(PC,15,RN),

  /* Coprocessor numbers.  */
  REGSET(p, CP), REGSET(P, CP),

  /* Coprocessor register numbers.  The "cr" variants are for backward
     compatibility.  */
  REGSET(c,  CN), REGSET(C, CN),
  REGSET(cr, CN), REGSET(CR, CN),

  /* ARM banked registers.  */
  REGDEF(R8_usr,512|(0<<16),RNB), REGDEF(r8_usr,512|(0<<16),RNB),
  REGDEF(R9_usr,512|(1<<16),RNB), REGDEF(r9_usr,512|(1<<16),RNB),
  REGDEF(R10_usr,512|(2<<16),RNB), REGDEF(r10_usr,512|(2<<16),RNB),
  REGDEF(R11_usr,512|(3<<16),RNB), REGDEF(r11_usr,512|(3<<16),RNB),
  REGDEF(R12_usr,512|(4<<16),RNB), REGDEF(r12_usr,512|(4<<16),RNB),
  REGDEF(SP_usr,512|(5<<16),RNB), REGDEF(sp_usr,512|(5<<16),RNB),
  REGDEF(LR_usr,512|(6<<16),RNB), REGDEF(lr_usr,512|(6<<16),RNB),

  REGDEF(R8_fiq,512|(8<<16),RNB), REGDEF(r8_fiq,512|(8<<16),RNB),
  REGDEF(R9_fiq,512|(9<<16),RNB), REGDEF(r9_fiq,512|(9<<16),RNB),
  REGDEF(R10_fiq,512|(10<<16),RNB), REGDEF(r10_fiq,512|(10<<16),RNB),
  REGDEF(R11_fiq,512|(11<<16),RNB), REGDEF(r11_fiq,512|(11<<16),RNB),
  REGDEF(R12_fiq,512|(12<<16),RNB), REGDEF(r12_fiq,512|(12<<16),RNB),
  REGDEF(SP_fiq,512|(13<<16),RNB), REGDEF(sp_fiq,512|(13<<16),RNB),
  REGDEF(LR_fiq,512|(14<<16),RNB), REGDEF(lr_fiq,512|(14<<16),RNB),
  REGDEF(SPSR_fiq,512|(14<<16)|SPSR_BIT,RNB), REGDEF(spsr_fiq,512|(14<<16)|SPSR_BIT,RNB),

  SPLRBANK(0,IRQ,RNB), SPLRBANK(0,irq,RNB),
  SPLRBANK(2,SVC,RNB), SPLRBANK(2,svc,RNB),
  SPLRBANK(4,ABT,RNB), SPLRBANK(4,abt,RNB),
  SPLRBANK(6,UND,RNB), SPLRBANK(6,und,RNB),
  SPLRBANK(12,MON,RNB), SPLRBANK(12,mon,RNB),
  REGDEF(elr_hyp,768|(14<<16),RNB), REGDEF(ELR_hyp,768|(14<<16),RNB),
  REGDEF(sp_hyp,768|(15<<16),RNB), REGDEF(SP_hyp,768|(15<<16),RNB),
  REGDEF(spsr_hyp,768|(14<<16)|SPSR_BIT,RNB),
  REGDEF(SPSR_hyp,768|(14<<16)|SPSR_BIT,RNB),

  /* FPA registers.  */
  REGNUM(f,0,FN), REGNUM(f,1,FN), REGNUM(f,2,FN), REGNUM(f,3,FN),
  REGNUM(f,4,FN), REGNUM(f,5,FN), REGNUM(f,6,FN), REGNUM(f,7, FN),

  REGNUM(F,0,FN), REGNUM(F,1,FN), REGNUM(F,2,FN), REGNUM(F,3,FN),
  REGNUM(F,4,FN), REGNUM(F,5,FN), REGNUM(F,6,FN), REGNUM(F,7, FN),

  /* VFP SP registers.	*/
  REGSET(s,VFS),  REGSET(S,VFS),
  REGSETH(s,VFS), REGSETH(S,VFS),

  /* VFP DP Registers.	*/
  REGSET(d,VFD),  REGSET(D,VFD),
  /* Extra Neon DP registers.  */
  REGSETH(d,VFD), REGSETH(D,VFD),

  /* Neon QP registers.  */
  REGSET2(q,NQ),  REGSET2(Q,NQ),

  /* VFP control registers.  */
  REGDEF(fpsid,0,VFC), REGDEF(fpscr,1,VFC), REGDEF(fpexc,8,VFC),
  REGDEF(FPSID,0,VFC), REGDEF(FPSCR,1,VFC), REGDEF(FPEXC,8,VFC),
  REGDEF(fpinst,9,VFC), REGDEF(fpinst2,10,VFC),
  REGDEF(FPINST,9,VFC), REGDEF(FPINST2,10,VFC),
  REGDEF(mvfr0,7,VFC), REGDEF(mvfr1,6,VFC),
  REGDEF(MVFR0,7,VFC), REGDEF(MVFR1,6,VFC),
  REGDEF(mvfr2,5,VFC), REGDEF(MVFR2,5,VFC),

  /* Maverick DSP coprocessor registers.  */
  REGSET(mvf,MVF),  REGSET(mvd,MVD),  REGSET(mvfx,MVFX),  REGSET(mvdx,MVDX),
  REGSET(MVF,MVF),  REGSET(MVD,MVD),  REGSET(MVFX,MVFX),  REGSET(MVDX,MVDX),

  REGNUM(mvax,0,MVAX), REGNUM(mvax,1,MVAX),
  REGNUM(mvax,2,MVAX), REGNUM(mvax,3,MVAX),
  REGDEF(dspsc,0,DSPSC),

  REGNUM(MVAX,0,MVAX), REGNUM(MVAX,1,MVAX),
  REGNUM(MVAX,2,MVAX), REGNUM(MVAX,3,MVAX),
  REGDEF(DSPSC,0,DSPSC),

  /* iWMMXt data registers - p0, c0-15.	 */
  REGSET(wr,MMXWR), REGSET(wR,MMXWR), REGSET(WR, MMXWR),

  /* iWMMXt control registers - p1, c0-3.  */
  REGDEF(wcid,	0,MMXWC),  REGDEF(wCID,	 0,MMXWC),  REGDEF(WCID,  0,MMXWC),
  REGDEF(wcon,	1,MMXWC),  REGDEF(wCon,	 1,MMXWC),  REGDEF(WCON,  1,MMXWC),
  REGDEF(wcssf, 2,MMXWC),  REGDEF(wCSSF, 2,MMXWC),  REGDEF(WCSSF, 2,MMXWC),
  REGDEF(wcasf, 3,MMXWC),  REGDEF(wCASF, 3,MMXWC),  REGDEF(WCASF, 3,MMXWC),

  /* iWMMXt scalar (constant/offset) registers - p1, c8-11.  */
  REGDEF(wcgr0, 8,MMXWCG),  REGDEF(wCGR0, 8,MMXWCG),  REGDEF(WCGR0, 8,MMXWCG),
  REGDEF(wcgr1, 9,MMXWCG),  REGDEF(wCGR1, 9,MMXWCG),  REGDEF(WCGR1, 9,MMXWCG),
  REGDEF(wcgr2,10,MMXWCG),  REGDEF(wCGR2,10,MMXWCG),  REGDEF(WCGR2,10,MMXWCG),
  REGDEF(wcgr3,11,MMXWCG),  REGDEF(wCGR3,11,MMXWCG),  REGDEF(WCGR3,11,MMXWCG),

  /* XScale accumulator registers.  */
  REGNUM(acc,0,XSCALE), REGNUM(ACC,0,XSCALE),
};
#undef REGDEF
#undef REGNUM
#undef REGSET

/* Table of all PSR suffixes.  Bare "CPSR" and "SPSR" are handled
   within psr_required_here.  */
static const struct asm_psr psrs[] =
{
  /* Backward compatibility notation.  Note that "all" is no longer
     truly all possible PSR bits.  */
  {"all",  PSR_c | PSR_f},
  {"flg",  PSR_f},
  {"ctl",  PSR_c},

  /* Individual flags.	*/
  {"f",	   PSR_f},
  {"c",	   PSR_c},
  {"x",	   PSR_x},
  {"s",	   PSR_s},

  /* Combinations of flags.  */
  {"fs",   PSR_f | PSR_s},
  {"fx",   PSR_f | PSR_x},
  {"fc",   PSR_f | PSR_c},
  {"sf",   PSR_s | PSR_f},
  {"sx",   PSR_s | PSR_x},
  {"sc",   PSR_s | PSR_c},
  {"xf",   PSR_x | PSR_f},
  {"xs",   PSR_x | PSR_s},
  {"xc",   PSR_x | PSR_c},
  {"cf",   PSR_c | PSR_f},
  {"cs",   PSR_c | PSR_s},
  {"cx",   PSR_c | PSR_x},
  {"fsx",  PSR_f | PSR_s | PSR_x},
  {"fsc",  PSR_f | PSR_s | PSR_c},
  {"fxs",  PSR_f | PSR_x | PSR_s},
  {"fxc",  PSR_f | PSR_x | PSR_c},
  {"fcs",  PSR_f | PSR_c | PSR_s},
  {"fcx",  PSR_f | PSR_c | PSR_x},
  {"sfx",  PSR_s | PSR_f | PSR_x},
  {"sfc",  PSR_s | PSR_f | PSR_c},
  {"sxf",  PSR_s | PSR_x | PSR_f},
  {"sxc",  PSR_s | PSR_x | PSR_c},
  {"scf",  PSR_s | PSR_c | PSR_f},
  {"scx",  PSR_s | PSR_c | PSR_x},
  {"xfs",  PSR_x | PSR_f | PSR_s},
  {"xfc",  PSR_x | PSR_f | PSR_c},
  {"xsf",  PSR_x | PSR_s | PSR_f},
  {"xsc",  PSR_x | PSR_s | PSR_c},
  {"xcf",  PSR_x | PSR_c | PSR_f},
  {"xcs",  PSR_x | PSR_c | PSR_s},
  {"cfs",  PSR_c | PSR_f | PSR_s},
  {"cfx",  PSR_c | PSR_f | PSR_x},
  {"csf",  PSR_c | PSR_s | PSR_f},
  {"csx",  PSR_c | PSR_s | PSR_x},
  {"cxf",  PSR_c | PSR_x | PSR_f},
  {"cxs",  PSR_c | PSR_x | PSR_s},
  {"fsxc", PSR_f | PSR_s | PSR_x | PSR_c},
  {"fscx", PSR_f | PSR_s | PSR_c | PSR_x},
  {"fxsc", PSR_f | PSR_x | PSR_s | PSR_c},
  {"fxcs", PSR_f | PSR_x | PSR_c | PSR_s},
  {"fcsx", PSR_f | PSR_c | PSR_s | PSR_x},
  {"fcxs", PSR_f | PSR_c | PSR_x | PSR_s},
  {"sfxc", PSR_s | PSR_f | PSR_x | PSR_c},
  {"sfcx", PSR_s | PSR_f | PSR_c | PSR_x},
  {"sxfc", PSR_s | PSR_x | PSR_f | PSR_c},
  {"sxcf", PSR_s | PSR_x | PSR_c | PSR_f},
  {"scfx", PSR_s | PSR_c | PSR_f | PSR_x},
  {"scxf", PSR_s | PSR_c | PSR_x | PSR_f},
  {"xfsc", PSR_x | PSR_f | PSR_s | PSR_c},
  {"xfcs", PSR_x | PSR_f | PSR_c | PSR_s},
  {"xsfc", PSR_x | PSR_s | PSR_f | PSR_c},
  {"xscf", PSR_x | PSR_s | PSR_c | PSR_f},
  {"xcfs", PSR_x | PSR_c | PSR_f | PSR_s},
  {"xcsf", PSR_x | PSR_c | PSR_s | PSR_f},
  {"cfsx", PSR_c | PSR_f | PSR_s | PSR_x},
  {"cfxs", PSR_c | PSR_f | PSR_x | PSR_s},
  {"csfx", PSR_c | PSR_s | PSR_f | PSR_x},
  {"csxf", PSR_c | PSR_s | PSR_x | PSR_f},
  {"cxfs", PSR_c | PSR_x | PSR_f | PSR_s},
  {"cxsf", PSR_c | PSR_x | PSR_s | PSR_f},
};

/* Table of all shift-in-operand names.	 */
static const struct asm_shift_name shift_names [] =
{
  { "asl", SHIFT_LSL },	 { "ASL", SHIFT_LSL },
  { "lsl", SHIFT_LSL },	 { "LSL", SHIFT_LSL },
  { "lsr", SHIFT_LSR },	 { "LSR", SHIFT_LSR },
  { "asr", SHIFT_ASR },	 { "ASR", SHIFT_ASR },
  { "ror", SHIFT_ROR },	 { "ROR", SHIFT_ROR },
  { "rrx", SHIFT_RRX },	 { "RRX", SHIFT_RRX }
};

/* Table of all explicit relocation names.  */
static struct reloc_entry reloc_names[] =
{
  { "got",     BFD_RELOC_ARM_GOT32   },	 { "GOT",     BFD_RELOC_ARM_GOT32   },
  { "gotoff",  BFD_RELOC_ARM_GOTOFF  },	 { "GOTOFF",  BFD_RELOC_ARM_GOTOFF  },
  { "plt",     BFD_RELOC_ARM_PLT32   },	 { "PLT",     BFD_RELOC_ARM_PLT32   },
  { "target1", BFD_RELOC_ARM_TARGET1 },	 { "TARGET1", BFD_RELOC_ARM_TARGET1 },
  { "target2", BFD_RELOC_ARM_TARGET2 },	 { "TARGET2", BFD_RELOC_ARM_TARGET2 },
  { "sbrel",   BFD_RELOC_ARM_SBREL32 },	 { "SBREL",   BFD_RELOC_ARM_SBREL32 },
  { "tlsgd",   BFD_RELOC_ARM_TLS_GD32},  { "TLSGD",   BFD_RELOC_ARM_TLS_GD32},
  { "tlsldm",  BFD_RELOC_ARM_TLS_LDM32}, { "TLSLDM",  BFD_RELOC_ARM_TLS_LDM32},
  { "tlsldo",  BFD_RELOC_ARM_TLS_LDO32}, { "TLSLDO",  BFD_RELOC_ARM_TLS_LDO32},
  { "gottpoff",BFD_RELOC_ARM_TLS_IE32},  { "GOTTPOFF",BFD_RELOC_ARM_TLS_IE32},
  { "tpoff",   BFD_RELOC_ARM_TLS_LE32},  { "TPOFF",   BFD_RELOC_ARM_TLS_LE32},
  { "got_prel", BFD_RELOC_ARM_GOT_PREL}, { "GOT_PREL", BFD_RELOC_ARM_GOT_PREL},
  { "tlsdesc", BFD_RELOC_ARM_TLS_GOTDESC},
	{ "TLSDESC", BFD_RELOC_ARM_TLS_GOTDESC},
  { "tlscall", BFD_RELOC_ARM_TLS_CALL},
	{ "TLSCALL", BFD_RELOC_ARM_TLS_CALL},
  { "tlsdescseq", BFD_RELOC_ARM_TLS_DESCSEQ},
	{ "TLSDESCSEQ", BFD_RELOC_ARM_TLS_DESCSEQ},
  { "gotfuncdesc", BFD_RELOC_ARM_GOTFUNCDESC },
	{ "GOTFUNCDESC", BFD_RELOC_ARM_GOTFUNCDESC },
  { "gotofffuncdesc", BFD_RELOC_ARM_GOTOFFFUNCDESC },
	{ "GOTOFFFUNCDESC", BFD_RELOC_ARM_GOTOFFFUNCDESC },
  { "funcdesc", BFD_RELOC_ARM_FUNCDESC },
	{ "FUNCDESC", BFD_RELOC_ARM_FUNCDESC },
   { "tlsgd_fdpic", BFD_RELOC_ARM_TLS_GD32_FDPIC },      { "TLSGD_FDPIC", BFD_RELOC_ARM_TLS_GD32_FDPIC },
   { "tlsldm_fdpic", BFD_RELOC_ARM_TLS_LDM32_FDPIC },    { "TLSLDM_FDPIC", BFD_RELOC_ARM_TLS_LDM32_FDPIC },
   { "gottpoff_fdpic", BFD_RELOC_ARM_TLS_IE32_FDPIC },   { "GOTTPOFF_FDIC", BFD_RELOC_ARM_TLS_IE32_FDPIC },
};

/* Table of all conditional affixes.  0xF is not defined as a condition code.  */
static const struct asm_cond conds[] =
{
  {"eq", 0x0},
  {"ne", 0x1},
  {"cs", 0x2}, {"hs", 0x2},
  {"cc", 0x3}, {"ul", 0x3}, {"lo", 0x3},
  {"mi", 0x4},
  {"pl", 0x5},
  {"vs", 0x6},
  {"vc", 0x7},
  {"hi", 0x8},
  {"ls", 0x9},
  {"ge", 0xa},
  {"lt", 0xb},
  {"gt", 0xc},
  {"le", 0xd},
  {"al", 0xe}
};
/*
#define UL_BARRIER(L,U,CODE,FEAT) \
  { L, CODE, ARM_FEATURE_CORE_LOW (FEAT) }, \
  { U, CODE, ARM_FEATURE_CORE_LOW (FEAT) }

static struct asm_barrier_opt barrier_opt_names[] =
{
  UL_BARRIER ("sy",	"SY",	 0xf, ARM_EXT_BARRIER),
  UL_BARRIER ("st",	"ST",	 0xe, ARM_EXT_BARRIER),
  UL_BARRIER ("ish",	"ISH",	 0xb, ARM_EXT_BARRIER),
  UL_BARRIER ("sh",	"SH",	 0xb, ARM_EXT_BARRIER),
  UL_BARRIER ("ishst",	"ISHST", 0xa, ARM_EXT_BARRIER),
  UL_BARRIER ("shst",	"SHST",	 0xa, ARM_EXT_BARRIER),
  UL_BARRIER ("un",	"UN",	 0x7, ARM_EXT_BARRIER),
  UL_BARRIER ("nsh",	"NSH",	 0x7, ARM_EXT_BARRIER),
  UL_BARRIER ("unst",	"UNST",	 0x6, ARM_EXT_BARRIER),
  UL_BARRIER ("nshst",	"NSHST", 0x6, ARM_EXT_BARRIER),
  UL_BARRIER ("osh",	"OSH",	 0x3, ARM_EXT_BARRIER),
  UL_BARRIER ("oshst",	"OSHST", 0x2, ARM_EXT_BARRIER),
};

#undef UL_BARRIER
*/
/* Table of ARM-format instructions.	*/

/* Macros for gluing together operand strings.  N.B. In all cases
   other than OPS0, the trailing OP_stop comes from default
   zero-initialization of the unspecified elements of the array.  */
#define OPS0()		  { OP_stop, }
#define OPS1(a)		  { OP_##a, }
#define OPS2(a,b)	  { OP_##a,OP_##b, }
#define OPS3(a,b,c)	  { OP_##a,OP_##b,OP_##c, }
#define OPS4(a,b,c,d)	  { OP_##a,OP_##b,OP_##c,OP_##d, }
#define OPS5(a,b,c,d,e)	  { OP_##a,OP_##b,OP_##c,OP_##d,OP_##e, }
#define OPS6(a,b,c,d,e,f) { OP_##a,OP_##b,OP_##c,OP_##d,OP_##e,OP_##f, }

/* These macros are similar to the OPSn, but do not prepend the OP_ prefix.
   This is useful when mixing operands for ARM and THUMB, i.e. using the
   MIX_ARM_THUMB_OPERANDS macro.
   In order to use these macros, prefix the number of operands with _
   e.g. _3.  */
#define OPS_1(a)	   { a, }
#define OPS_2(a,b)	   { a,b, }
#define OPS_3(a,b,c)	   { a,b,c, }
#define OPS_4(a,b,c,d)	   { a,b,c,d, }
#define OPS_5(a,b,c,d,e)   { a,b,c,d,e, }
#define OPS_6(a,b,c,d,e,f) { a,b,c,d,e,f, }

/* These macros abstract out the exact format of the mnemonic table and
   save some repeated characters.  */

/* The normal sort of mnemonic; has a Thumb variant; takes a conditional suffix.  */
#define TxCE(mnem, op, top, nops, ops, ae, te) \
  { mnem, OPS##nops ops, OT_csuffix, 0x##op, top, ARM_VARIANT, \
    THUMB_VARIANT, do_##ae, do_##te }

/* Two variants of the above - TCE for a numeric Thumb opcode, tCE for
   a T_MNEM_xyz enumerator.  */
#define TCE(mnem, aop, top, nops, ops, ae, te) \
      TxCE (mnem, aop, 0x##top, nops, ops, ae, te)
#define tCE(mnem, aop, top, nops, ops, ae, te) \
      TxCE (mnem, aop, T_MNEM##top, nops, ops, ae, te)

/* Second most common sort of mnemonic: has a Thumb variant, takes a conditional
   infix after the third character.  */
#define TxC3(mnem, op, top, nops, ops, ae, te) \
  { mnem, OPS##nops ops, OT_cinfix3, 0x##op, top, ARM_VARIANT, \
    THUMB_VARIANT, do_##ae, do_##te }
#define TxC3w(mnem, op, top, nops, ops, ae, te) \
  { mnem, OPS##nops ops, OT_cinfix3_deprecated, 0x##op, top, ARM_VARIANT, \
    THUMB_VARIANT, do_##ae, do_##te }
#define TC3(mnem, aop, top, nops, ops, ae, te) \
      TxC3 (mnem, aop, 0x##top, nops, ops, ae, te)
#define TC3w(mnem, aop, top, nops, ops, ae, te) \
      TxC3w (mnem, aop, 0x##top, nops, ops, ae, te)
#define tC3(mnem, aop, top, nops, ops, ae, te) \
      TxC3 (mnem, aop, T_MNEM##top, nops, ops, ae, te)
#define tC3w(mnem, aop, top, nops, ops, ae, te) \
      TxC3w (mnem, aop, T_MNEM##top, nops, ops, ae, te)

/* ARM-only variants of all the above.  */
#define CE(mnem,  op, nops, ops, ae)	\
  { mnem, OPS##nops ops, OT_csuffix, 0x##op, 0x0, ARM_VARIANT, 0, do_##ae, NULL }

#define C3(mnem, op, nops, ops, ae)	\
  { #mnem, OPS##nops ops, OT_cinfix3, 0x##op, 0x0, ARM_VARIANT, 0, do_##ae, NULL }

/* Legacy mnemonics that always have conditional infix after the third
   character.  */
#define CL(mnem, op, nops, ops, ae)	\
  { mnem, OPS##nops ops, OT_cinfix3_legacy, \
    0x##op, 0x0, ARM_VARIANT, 0, do_##ae, NULL }

#define xCM_(m1, m2, m3, op, nops, ops, ae)	\
  { m1 #m2 m3, OPS##nops ops, \
    sizeof (#m2) == 1 ? OT_odd_infix_unc : OT_odd_infix_0 + sizeof (m1) - 1, \
    0x##op, 0x0, ARM_VARIANT, 0, do_##ae, NULL }

#define CM(m1, m2, op, nops, ops, ae)	\
  xCM_ (m1,   , m2, op, nops, ops, ae),	\
  xCM_ (m1, eq, m2, op, nops, ops, ae),	\
  xCM_ (m1, ne, m2, op, nops, ops, ae),	\
  xCM_ (m1, cs, m2, op, nops, ops, ae),	\
  xCM_ (m1, hs, m2, op, nops, ops, ae),	\
  xCM_ (m1, cc, m2, op, nops, ops, ae),	\
  xCM_ (m1, ul, m2, op, nops, ops, ae),	\
  xCM_ (m1, lo, m2, op, nops, ops, ae),	\
  xCM_ (m1, mi, m2, op, nops, ops, ae),	\
  xCM_ (m1, pl, m2, op, nops, ops, ae),	\
  xCM_ (m1, vs, m2, op, nops, ops, ae),	\
  xCM_ (m1, vc, m2, op, nops, ops, ae),	\
  xCM_ (m1, hi, m2, op, nops, ops, ae),	\
  xCM_ (m1, ls, m2, op, nops, ops, ae),	\
  xCM_ (m1, ge, m2, op, nops, ops, ae),	\
  xCM_ (m1, lt, m2, op, nops, ops, ae),	\
  xCM_ (m1, gt, m2, op, nops, ops, ae),	\
  xCM_ (m1, le, m2, op, nops, ops, ae),	\
  xCM_ (m1, al, m2, op, nops, ops, ae)
#define do_0 0

static const struct asm_opcode insns[] =
{
#define ARM_VARIANT    & arm_ext_v1 /* Core ARM Instructions.  */
#define THUMB_VARIANT  & arm_ext_v4t
 tCE("and",	0000000, _and,     3, (RR, oRR, SH), arit, t_arit3c),
 tC3("ands",	0100000, _ands,	   3, (RR, oRR, SH), arit, t_arit3c),
 tCE("eor",	0200000, _eor,	   3, (RR, oRR, SH), arit, t_arit3c),
 tC3("eors",	0300000, _eors,	   3, (RR, oRR, SH), arit, t_arit3c),
 tCE("sub",	0400000, _sub,	   3, (RR, oRR, SH), arit, t_add_sub),
 tC3("subs",	0500000, _subs,	   3, (RR, oRR, SH), arit, t_add_sub),
 tCE("add",	0800000, _add,	   3, (RR, oRR, SHG), arit, t_add_sub),
 tC3("adds",	0900000, _adds,	   3, (RR, oRR, SHG), arit, t_add_sub),
 tCE("adc",	0a00000, _adc,	   3, (RR, oRR, SH), arit, t_arit3c),
 tC3("adcs",	0b00000, _adcs,	   3, (RR, oRR, SH), arit, t_arit3c),
 tCE("sbc",	0c00000, _sbc,	   3, (RR, oRR, SH), arit, t_arit3),
 tC3("sbcs",	0d00000, _sbcs,	   3, (RR, oRR, SH), arit, t_arit3),
 tCE("orr",	1800000, _orr,	   3, (RR, oRR, SH), arit, t_arit3c),
 tC3("orrs",	1900000, _orrs,	   3, (RR, oRR, SH), arit, t_arit3c),
 tCE("bic",	1c00000, _bic,	   3, (RR, oRR, SH), arit, t_arit3),
 tC3("bics",	1d00000, _bics,	   3, (RR, oRR, SH), arit, t_arit3),

 /* The p-variants of tst/cmp/cmn/teq (below) are the pre-V6 mechanism
    for setting PSR flag bits.  They are obsolete in V6 and do not
    have Thumb equivalents. */
 tCE("tst",	1100000, _tst,	   2, (RR, SH),      cmp,  t_mvn_tst),
 tC3w("tsts",	1100000, _tst,	   2, (RR, SH),      cmp,  t_mvn_tst),
  CL("tstp",	110f000,     	   2, (RR, SH),      cmp),
 tCE("cmp",	1500000, _cmp,	   2, (RR, SH),      cmp,  t_mov_cmp),
 tC3w("cmps",	1500000, _cmp,	   2, (RR, SH),      cmp,  t_mov_cmp),
  CL("cmpp",	150f000,     	   2, (RR, SH),      cmp),
 tCE("cmn",	1700000, _cmn,	   2, (RR, SH),      cmp,  t_mvn_tst),
 tC3w("cmns",	1700000, _cmn,	   2, (RR, SH),      cmp,  t_mvn_tst),
  CL("cmnp",	170f000,     	   2, (RR, SH),      cmp),

 tCE("mov",	1a00000, _mov,	   2, (RR, SH),      mov,  t_mov_cmp),
 tC3("movs",	1b00000, _movs,	   2, (RR, SHG),     mov,  t_mov_cmp),
 tCE("mvn",	1e00000, _mvn,	   2, (RR, SH),      mov,  t_mvn_tst),
 tC3("mvns",	1f00000, _mvns,	   2, (RR, SH),      mov,  t_mvn_tst),

 tCE("ldr",	4100000, _ldr,	   2, (RR, ADDRGLDR),ldst, t_ldst),
 tC3("ldrb",	4500000, _ldrb,	   2, (RRnpc_npcsp, ADDRGLDR),ldst, t_ldst),
 tCE("str",	4000000, _str,	   _2, (MIX_ARM_THUMB_OPERANDS (OP_RR,
								OP_RRnpc),
					OP_ADDRGLDR),ldst, t_ldst),
 tC3("strb",	4400000, _strb,	   2, (RRnpc_npcsp, ADDRGLDR),ldst, t_ldst),

 tCE("stm",	8800000, _stmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),
 tC3("stmia",	8800000, _stmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),
 tC3("stmea",	8800000, _stmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),
 tCE("ldm",	8900000, _ldmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),
 tC3("ldmia",	8900000, _ldmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),
 tC3("ldmfd",	8900000, _ldmia,    2, (RRw, REGLST), ldmstm, t_ldmstm),

 tCE("b",	a000000, _b,	   1, (EXPr),	     branch, t_branch),
 TCE("bl",	b000000, f000f800, 1, (EXPr),	     bl, t_branch23),

  /* Pseudo ops.  */
 tCE("adr",	28f0000, _adr,	   2, (RR, EXP),     adr,  t_adr),
  C3(adrl,	28f0000,           2, (RR, EXP),     adrl),
 tCE("nop",	1a00000, _nop,	   1, (oI255c),	     nop,  t_nop),
 tCE("udf",	7f000f0, _udf,     1, (oIffffb),     bkpt, t_udf),

  /* Thumb-compatibility pseudo ops.  */
 tCE("lsl",	1a00000, _lsl,	   3, (RR, oRR, SH), shift, t_shift),
 tC3("lsls",	1b00000, _lsls,	   3, (RR, oRR, SH), shift, t_shift),
 tCE("lsr",	1a00020, _lsr,	   3, (RR, oRR, SH), shift, t_shift),
 tC3("lsrs",	1b00020, _lsrs,	   3, (RR, oRR, SH), shift, t_shift),
 tCE("asr",	1a00040, _asr,	   3, (RR, oRR, SH), shift, t_shift),
 tC3("asrs",      1b00040, _asrs,     3, (RR, oRR, SH), shift, t_shift),
 tCE("ror",	1a00060, _ror,	   3, (RR, oRR, SH), shift, t_shift),
 tC3("rors",	1b00060, _rors,	   3, (RR, oRR, SH), shift, t_shift),
 tCE("neg",	2600000, _neg,	   2, (RR, RR),      rd_rn, t_neg),
 tC3("negs",	2700000, _negs,	   2, (RR, RR),      rd_rn, t_neg),
 tCE("push",	92d0000, _push,     1, (REGLST),	     push_pop, t_push_pop),
 tCE("pop",	8bd0000, _pop,	   1, (REGLST),	     push_pop, t_push_pop),

 /* These may simplify to neg.  */
 TCE("rsb",	0600000, ebc00000, 3, (RR, oRR, SH), arit, t_rsb),
 TC3("rsbs",	0700000, ebd00000, 3, (RR, oRR, SH), arit, t_rsb),

#undef THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_os

 TCE("swi",	f000000, df00,     1, (EXPi),        swi, t_swi),
 TCE("svc",	f000000, df00,     1, (EXPi),        swi, t_swi),

 /* V1 instructions with no Thumb analogue prior to V6T2.  */
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 TCE("teq",	1300000, ea900f00, 2, (RR, SH),      cmp,  t_mvn_tst),
 TC3w("teqs",	1300000, ea900f00, 2, (RR, SH),      cmp,  t_mvn_tst),
  CL("teqp",	130f000,           2, (RR, SH),      cmp),

 TC3("ldrt",	4300000, f8500e00, 2, (RRnpc_npcsp, ADDR),ldstt, t_ldstt),
 TC3("ldrbt",	4700000, f8100e00, 2, (RRnpc_npcsp, ADDR),ldstt, t_ldstt),
 TC3("strt",	4200000, f8400e00, 2, (RR_npcsp, ADDR),   ldstt, t_ldstt),
 TC3("strbt",	4600000, f8000e00, 2, (RRnpc_npcsp, ADDR),ldstt, t_ldstt),

 TC3("stmdb",	9000000, e9000000, 2, (RRw, REGLST), ldmstm, t_ldmstm),
 TC3("stmfd",     9000000, e9000000, 2, (RRw, REGLST), ldmstm, t_ldmstm),

 TC3("ldmdb",	9100000, e9100000, 2, (RRw, REGLST), ldmstm, t_ldmstm),
 TC3("ldmea",	9100000, e9100000, 2, (RRw, REGLST), ldmstm, t_ldmstm),

 /* V1 instructions with no Thumb analogue at all.  */
  CE("rsc",	0e00000,	   3, (RR, oRR, SH), arit),
  C3(rscs,	0f00000,	   3, (RR, oRR, SH), arit),

  C3(stmib,	9800000,	   2, (RRw, REGLST), ldmstm),
  C3(stmfa,	9800000,	   2, (RRw, REGLST), ldmstm),
  C3(stmda,	8000000,	   2, (RRw, REGLST), ldmstm),
  C3(stmed,	8000000,	   2, (RRw, REGLST), ldmstm),
  C3(ldmib,	9900000,	   2, (RRw, REGLST), ldmstm),
  C3(ldmed,	9900000,	   2, (RRw, REGLST), ldmstm),
  C3(ldmda,	8100000,	   2, (RRw, REGLST), ldmstm),
  C3(ldmfa,	8100000,	   2, (RRw, REGLST), ldmstm),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v2	/* ARM 2 - multiplies.	*/
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 tCE("mul",	0000090, _mul,	   3, (RRnpc, RRnpc, oRR), mul, t_mul),
 tC3("muls",	0100090, _muls,	   3, (RRnpc, RRnpc, oRR), mul, t_mul),

#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 TCE("mla",	0200090, fb000000, 4, (RRnpc, RRnpc, RRnpc, RRnpc), mlas, t_mla),
  C3(mlas,	0300090,           4, (RRnpc, RRnpc, RRnpc, RRnpc), mlas),

  /* Generic coprocessor instructions.	*/
 TCE("cdp",	e000000, ee000000, 6, (RCP, I15b, RCN, RCN, RCN, oI7b), cdp,    cdp),
 TCE("ldc",	c100000, ec100000, 3, (RCP, RCN, ADDRGLDC),	        lstc,   lstc),
 TC3("ldcl",	c500000, ec500000, 3, (RCP, RCN, ADDRGLDC),	        lstc,   lstc),
 TCE("stc",	c000000, ec000000, 3, (RCP, RCN, ADDRGLDC),	        lstc,   lstc),
 TC3("stcl",	c400000, ec400000, 3, (RCP, RCN, ADDRGLDC),	        lstc,   lstc),
 TCE("mcr",	e000010, ee000010, 6, (RCP, I7b, RR, RCN, RCN, oI7b),   co_reg, co_reg),
 TCE("mrc",	e100010, ee100010, 6, (RCP, I7b, APSR_RR, RCN, RCN, oI7b),   co_reg, co_reg),

#undef  ARM_VARIANT
#define ARM_VARIANT  & arm_ext_v2s /* ARM 3 - swp instructions.  */

  CE("swp",	1000090,           3, (RRnpc, RRnpc, RRnpcb), rd_rm_rn),
  C3(swpb,	1400090,           3, (RRnpc, RRnpc, RRnpcb), rd_rm_rn),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v3	/* ARM 6 Status register instructions.	*/
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_msr

 TCE("mrs",	1000000, f3e08000, 2, (RRnpc, rPSR), mrs, t_mrs),
 TCE("msr",	120f000, f3808000, 2, (wPSR, RR_EXi), msr, t_msr),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v3m	 /* ARM 7M long multiplies.  */
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 TCE("smull",	0c00090, fb800000, 4, (RRnpc, RRnpc, RRnpc, RRnpc), mull, t_mull),
  CM("smull","s",	0d00090,           4, (RRnpc, RRnpc, RRnpc, RRnpc), mull),
 TCE("umull",	0800090, fba00000, 4, (RRnpc, RRnpc, RRnpc, RRnpc), mull, t_mull),
  CM("umull","s",	0900090,           4, (RRnpc, RRnpc, RRnpc, RRnpc), mull),
 TCE("smlal",	0e00090, fbc00000, 4, (RRnpc, RRnpc, RRnpc, RRnpc), mull, t_mull),
  CM("smlal","s",	0f00090,           4, (RRnpc, RRnpc, RRnpc, RRnpc), mull),
 TCE("umlal",	0a00090, fbe00000, 4, (RRnpc, RRnpc, RRnpc, RRnpc), mull, t_mull),
  CM("umlal","s",	0b00090,           4, (RRnpc, RRnpc, RRnpc, RRnpc), mull),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v4	/* ARM Architecture 4.	*/
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 tC3("ldrh",	01000b0, _ldrh,     2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("strh",	00000b0, _strh,     2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldrsh",	01000f0, _ldrsh,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldrsb",	01000d0, _ldrsb,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldsh",	01000f0, _ldrsh,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldsb",	01000d0, _ldrsb,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),

#undef  ARM_VARIANT
#define ARM_VARIANT  & arm_ext_v4t

  /* ARM Architecture 4T.  */
  /* Note: bx (and blx) are required on V5, even if the processor does
     not support Thumb.	 */
 TCE("bx",	12fff10, 4700, 1, (RR),	bx, t_bx),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v2	/* ARM 2 - multiplies.	*/
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 tCE("mul",	0000090, _mul,	   3, (RRnpc, RRnpc, oRR), mul, t_mul),
 tC3("muls",	0100090, _muls,	   3, (RRnpc, RRnpc, oRR), mul, t_mul),

#undef  ARM_VARIANT
#define ARM_VARIANT  & arm_ext_v2s /* ARM 3 - swp instructions.  */

  CE("swp",	1000090,           3, (RRnpc, RRnpc, RRnpcb), rd_rm_rn),
  C3(swpb,	1400090,           3, (RRnpc, RRnpc, RRnpcb), rd_rm_rn),

#undef  ARM_VARIANT
#define ARM_VARIANT    & arm_ext_v4	/* ARM Architecture 4.	*/
#undef  THUMB_VARIANT
#define THUMB_VARIANT  & arm_ext_v4t

 tC3("ldrh",	01000b0, _ldrh,     2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("strh",	00000b0, _strh,     2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldrsh",	01000f0, _ldrsh,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldrsb",	01000d0, _ldrsb,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldsh",	01000f0, _ldrsh,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),
 tC3("ldsb",	01000d0, _ldrsb,    2, (RRnpc_npcsp, ADDRGLDRS), ldstv4, t_ldst),

  /* ARM Architecture 4T.  */
  /* Note: bx (and blx) are required on V5, even if the processor does
     not support Thumb.	 */
 TCE("bx",	12fff10, 4700, 1, (RR),	bx, t_bx),
};
#undef ARM_VARIANT
#undef THUMB_VARIANT
#undef TCE
#undef TUE
#undef TUF
#undef TCC
#undef cCE
#undef cCL
#undef C3E
#undef CE
#undef CM
#undef UE
#undef UF
#undef UT
#undef NUF
#undef nUF
#undef NCE
#undef nCE
#undef OPS0
#undef OPS1
#undef OPS2
#undef OPS3
#undef OPS4
#undef OPS5
#undef OPS6
#undef do_0

/* MD interface: bits in the object file.  */

/* Turn an integer of n bytes (in val) into a stream of bytes appropriate
   for use in the a.out file, and stores them in the array pointed to by buf.
   This knows about the endian-ness of the target machine and does
   THE RIGHT THING, whatever it is.  Possible values for n are 1 (byte)
   2 (short) and 4 (long)  Floating numbers are put out as a series of
   LITTLENUMS (shorts, here at least).	*/

void
md_number_to_chars (char * buf, valueT val, int n)
{
  if (target_big_endian)
    number_to_chars_bigendian (buf, val, n);
  else
    number_to_chars_littleendian (buf, val, n);
}

static valueT
md_chars_to_number (char * buf, int n)
{
  valueT result = 0;
  unsigned char * where = (unsigned char *) buf;

  if (target_big_endian)
    {
      while (n--)
	{
	  result <<= 8;
	  result |= (*where++ & 255);
	}
    }
  else
    {
      while (n--)
	{
	  result <<= 8;
	  result |= (where[n] & 255);
	}
    }

  return result;
}

/* MD interface: Sections.  */

/* Calculate the maximum variable size (i.e., excluding fr_fix)
   that an rs_machine_dependent frag may reach.  */

unsigned int
arm_frag_max_var (fragS *fragp)
{
  /* We only use rs_machine_dependent for variable-size Thumb instructions,
     which are either THUMB_SIZE (2) or INSN_SIZE (4).

     Note that we generate relaxable instructions even for cases that don't
     really need it, like an immediate that's a trivial constant.  So we're
     overestimating the instruction size for some of those cases.  Rather
     than putting more intelligence here, it would probably be better to
     avoid generating a relaxation frag in the first place when it can be
     determined up front that a short instruction will suffice.  */

  gas_assert (fragp->fr_type == rs_machine_dependent);
  return INSN_SIZE;
}

/* Estimate the size of a frag before relaxing.  Assume everything fits in
   2 bytes.  */

int
md_estimate_size_before_relax (fragS * fragp,
			       segT    segtype ATTRIBUTE_UNUSED)
{
  fragp->fr_var = 2;
  return 2;
}

/* Convert a machine dependent frag.  */

void
md_convert_frag (bfd *abfd, segT asec ATTRIBUTE_UNUSED, fragS *fragp)
{
  unsigned long insn;
  unsigned long old_op;
  char *buf;
  expressionS exp;
  fixS *fixp;
  int reloc_type;
  int pc_rel;
  int opcode;

  buf = fragp->fr_literal + fragp->fr_fix;

  old_op = bfd_get_16(abfd, buf);
  if (fragp->fr_symbol)
    {
      exp.X_op = O_symbol;
      exp.X_add_symbol = fragp->fr_symbol;
    }
  else
    {
      exp.X_op = O_constant;
    }
  exp.X_add_number = fragp->fr_offset;
  opcode = fragp->fr_subtype;
  switch (opcode)
    {
    case T_MNEM_ldr_pc:
    case T_MNEM_ldr_pc2:
    case T_MNEM_ldr_sp:
    case T_MNEM_str_sp:
    case T_MNEM_ldr:
    case T_MNEM_ldrb:
    case T_MNEM_ldrh:
    case T_MNEM_str:
    case T_MNEM_strb:
    case T_MNEM_strh:
      if (fragp->fr_var == 4)
	{
	  insn = THUMB_OP32 (opcode);
	  if ((old_op >> 12) == 4 || (old_op >> 12) == 9)
	    {
	      insn |= (old_op & 0x700) << 4;
	    }
	  else
	    {
	      insn |= (old_op & 7) << 12;
	      insn |= (old_op & 0x38) << 13;
	    }
	  insn |= 0x00000c00;
	  put_thumb32_insn (buf, insn);
	  reloc_type = BFD_RELOC_ARM_T32_OFFSET_IMM;
	}
      else
	{
	  reloc_type = BFD_RELOC_ARM_THUMB_OFFSET;
	}
      pc_rel = (opcode == T_MNEM_ldr_pc2);
      break;
    case T_MNEM_adr:
      if (fragp->fr_var == 4)
	{
	  insn = THUMB_OP32 (opcode);
	  insn |= (old_op & 0xf0) << 4;
	  put_thumb32_insn (buf, insn);
	  reloc_type = BFD_RELOC_ARM_T32_ADD_PC12;
	}
      else
	{
	  reloc_type = BFD_RELOC_ARM_THUMB_ADD;
	  exp.X_add_number -= 4;
	}
      pc_rel = 1;
      break;
    case T_MNEM_mov:
    case T_MNEM_movs:
    case T_MNEM_cmp:
    case T_MNEM_cmn:
      if (fragp->fr_var == 4)
	{
	  int r0off = (opcode == T_MNEM_mov
		       || opcode == T_MNEM_movs) ? 0 : 8;
	  insn = THUMB_OP32 (opcode);
	  insn = (insn & 0xe1ffffff) | 0x10000000;
	  insn |= (old_op & 0x700) << r0off;
	  put_thumb32_insn (buf, insn);
	  reloc_type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
      else
	{
	  reloc_type = BFD_RELOC_ARM_THUMB_IMM;
	}
      pc_rel = 0;
      break;
    case T_MNEM_b:
      if (fragp->fr_var == 4)
	{
	  insn = THUMB_OP32(opcode);
	  put_thumb32_insn (buf, insn);
	  reloc_type = BFD_RELOC_THUMB_PCREL_BRANCH25;
	}
      else
	reloc_type = BFD_RELOC_THUMB_PCREL_BRANCH12;
      pc_rel = 1;
      break;
    case T_MNEM_bcond:
      if (fragp->fr_var == 4)
	{
	  insn = THUMB_OP32(opcode);
	  insn |= (old_op & 0xf00) << 14;
	  put_thumb32_insn (buf, insn);
	  reloc_type = BFD_RELOC_THUMB_PCREL_BRANCH20;
	}
      else
	reloc_type = BFD_RELOC_THUMB_PCREL_BRANCH9;
      pc_rel = 1;
      break;
    case T_MNEM_add_sp:
    case T_MNEM_add_pc:
    case T_MNEM_inc_sp:
    case T_MNEM_dec_sp:
      if (fragp->fr_var == 4)
	{
	  /* ??? Choose between add and addw.  */
	  insn = THUMB_OP32 (opcode);
	  insn |= (old_op & 0xf0) << 4;
	  put_thumb32_insn (buf, insn);
	  if (opcode == T_MNEM_add_pc)
	    reloc_type = BFD_RELOC_ARM_T32_IMM12;
	  else
	    reloc_type = BFD_RELOC_ARM_T32_ADD_IMM;
	}
      else
	reloc_type = BFD_RELOC_ARM_THUMB_ADD;
      pc_rel = 0;
      break;

    case T_MNEM_addi:
    case T_MNEM_addis:
    case T_MNEM_subi:
    case T_MNEM_subis:
      if (fragp->fr_var == 4)
	{
	  insn = THUMB_OP32 (opcode);
	  insn |= (old_op & 0xf0) << 4;
	  insn |= (old_op & 0xf) << 16;
	  put_thumb32_insn (buf, insn);
	  if (insn & (1 << 20))
	    reloc_type = BFD_RELOC_ARM_T32_ADD_IMM;
	  else
	    reloc_type = BFD_RELOC_ARM_T32_IMMEDIATE;
	}
      else
	reloc_type = BFD_RELOC_ARM_THUMB_ADD;
      pc_rel = 0;
      break;
    default:
      abort ();
    }
  fixp = fix_new_exp (fragp, fragp->fr_fix, fragp->fr_var, &exp, pc_rel,
		      (enum bfd_reloc_code_real) reloc_type);
  fixp->fx_file = fragp->fr_file;
  fixp->fx_line = fragp->fr_line;
  fragp->fr_fix += fragp->fr_var;

}

/* Return the size of a relaxable immediate operand instruction.
   SHIFT and SIZE specify the form of the allowable immediate.  */
static int
relax_immediate (fragS *fragp, int size, int shift)
{
  offsetT offset;
  offsetT mask;
  offsetT low;

  /* ??? Should be able to do better than this.  */
  if (fragp->fr_symbol)
    return 4;

  low = (1 << shift) - 1;
  mask = (1 << (shift + size)) - (1 << shift);
  offset = fragp->fr_offset;
  /* Force misaligned offsets to 32-bit variant.  */
  if (offset & low)
    return 4;
  if (offset & ~mask)
    return 4;
  return 2;
}

/* Get the address of a symbol during relaxation.  */
static addressT
relaxed_symbol_addr (fragS *fragp, long stretch)
{
  fragS *sym_frag;
  addressT addr;
  symbolS *sym;

  sym = fragp->fr_symbol;
  sym_frag = symbol_get_frag (sym);
  know (S_GET_SEGMENT (sym) != absolute_section
	|| sym_frag == &zero_address_frag);
  addr = S_GET_VALUE (sym) + fragp->fr_offset;

  /* If frag has yet to be reached on this pass, assume it will
     move by STRETCH just as we did.  If this is not so, it will
     be because some frag between grows, and that will force
     another pass.  */

  if (stretch != 0
      && sym_frag->relax_marker != fragp->relax_marker)
    {
      fragS *f;

      /* Adjust stretch for any alignment frag.  Note that if have
	 been expanding the earlier code, the symbol may be
	 defined in what appears to be an earlier frag.  FIXME:
	 This doesn't handle the fr_subtype field, which specifies
	 a maximum number of bytes to skip when doing an
	 alignment.  */
      for (f = fragp; f != NULL && f != sym_frag; f = f->fr_next)
	{
	  if (f->fr_type == rs_align || f->fr_type == rs_align_code)
	    {
	      if (stretch < 0)
		stretch = - ((- stretch)
			     & ~ ((1 << (int) f->fr_offset) - 1));
	      else
		stretch &= ~ ((1 << (int) f->fr_offset) - 1);
	      if (stretch == 0)
		break;
	    }
	}
      if (f != NULL)
	addr += stretch;
    }

  return addr;
}

/* Return the size of a relaxable adr pseudo-instruction or PC-relative
   load.  */
static int
relax_adr (fragS *fragp, asection *sec, long stretch)
{
  addressT addr;
  offsetT val;

  /* Assume worst case for symbols not known to be in the same section.  */
  if (fragp->fr_symbol == NULL
      || !S_IS_DEFINED (fragp->fr_symbol)
      || sec != S_GET_SEGMENT (fragp->fr_symbol)
      || S_IS_WEAK (fragp->fr_symbol))
    return 4;

  val = relaxed_symbol_addr (fragp, stretch);
  addr = fragp->fr_address + fragp->fr_fix;
  addr = (addr + 4) & ~3;
  /* Force misaligned targets to 32-bit variant.  */
  if (val & 3)
    return 4;
  val -= addr;
  if (val < 0 || val > 1020)
    return 4;
  return 2;
}

/* Return the size of a relaxable add/sub immediate instruction.  */
static int
relax_addsub (fragS *fragp, asection *sec)
{
  char *buf;
  int op;

  buf = fragp->fr_literal + fragp->fr_fix;
  op = bfd_get_16(sec->owner, buf);
  if ((op & 0xf) == ((op >> 4) & 0xf))
    return relax_immediate (fragp, 8, 0);
  else
    return relax_immediate (fragp, 3, 0);
}

/* Return TRUE iff the definition of symbol S could be pre-empted
   (overridden) at link or load time.  */
static bfd_boolean
symbol_preemptible (symbolS *s)
{
  /* Weak symbols can always be pre-empted.  */
  if (S_IS_WEAK (s))
    return TRUE;

  /* Non-global symbols cannot be pre-empted. */
  if (! S_IS_EXTERNAL (s))
    return FALSE;

  /* In ELF, a global symbol can be marked protected, or private.  In that
     case it can't be pre-empted (other definitions in the same link unit
     would violate the ODR).  */
  if (ELF_ST_VISIBILITY (S_GET_OTHER (s)) > STV_DEFAULT)
    return FALSE;

  /* Other global symbols might be pre-empted.  */
  return TRUE;
}

/* Return the size of a relaxable branch instruction.  BITS is the
   size of the offset field in the narrow instruction.  */

static int
relax_branch (fragS *fragp, asection *sec, int bits, long stretch)
{
  addressT addr;
  offsetT val;
  offsetT limit;

  /* Assume worst case for symbols not known to be in the same section.  */
  if (!S_IS_DEFINED (fragp->fr_symbol)
      || sec != S_GET_SEGMENT (fragp->fr_symbol)
      || S_IS_WEAK (fragp->fr_symbol))
    return 4;

  /* A branch to a function in ARM state will require interworking.  */
  if (S_IS_DEFINED (fragp->fr_symbol)
      && ARM_IS_FUNC (fragp->fr_symbol))
      return 4;

  if (symbol_preemptible (fragp->fr_symbol))
    return 4;

  val = relaxed_symbol_addr (fragp, stretch);
  addr = fragp->fr_address + fragp->fr_fix + 4;
  val -= addr;

  /* Offset is a signed value *2 */
  limit = 1 << bits;
  if (val >= limit || val < -limit)
    return 4;
  return 2;
}


/* Relax a machine dependent frag.  This returns the amount by which
   the current size of the frag should change.  */

int
arm_relax_frag (asection *sec, fragS *fragp, long stretch)
{
  int oldsize;
  int newsize;

  oldsize = fragp->fr_var;
  switch (fragp->fr_subtype)
    {
    case T_MNEM_ldr_pc2:
      newsize = relax_adr (fragp, sec, stretch);
      break;
    case T_MNEM_ldr_pc:
    case T_MNEM_ldr_sp:
    case T_MNEM_str_sp:
      newsize = relax_immediate (fragp, 8, 2);
      break;
    case T_MNEM_ldr:
    case T_MNEM_str:
      newsize = relax_immediate (fragp, 5, 2);
      break;
    case T_MNEM_ldrh:
    case T_MNEM_strh:
      newsize = relax_immediate (fragp, 5, 1);
      break;
    case T_MNEM_ldrb:
    case T_MNEM_strb:
      newsize = relax_immediate (fragp, 5, 0);
      break;
    case T_MNEM_adr:
      newsize = relax_adr (fragp, sec, stretch);
      break;
    case T_MNEM_mov:
    case T_MNEM_movs:
    case T_MNEM_cmp:
    case T_MNEM_cmn:
      newsize = relax_immediate (fragp, 8, 0);
      break;
    case T_MNEM_b:
      newsize = relax_branch (fragp, sec, 11, stretch);
      break;
    case T_MNEM_bcond:
      newsize = relax_branch (fragp, sec, 8, stretch);
      break;
    case T_MNEM_add_sp:
    case T_MNEM_add_pc:
      newsize = relax_immediate (fragp, 8, 2);
      break;
    case T_MNEM_inc_sp:
    case T_MNEM_dec_sp:
      newsize = relax_immediate (fragp, 7, 2);
      break;
    case T_MNEM_addi:
    case T_MNEM_addis:
    case T_MNEM_subi:
    case T_MNEM_subis:
      newsize = relax_addsub (fragp, sec);
      break;
    default:
      abort ();
    }

  fragp->fr_var = newsize;
  /* Freeze wide instructions that are at or before the same location as
     in the previous pass.  This avoids infinite loops.
     Don't freeze them unconditionally because targets may be artificially
     misaligned by the expansion of preceding frags.  */
  if (stretch <= 0 && newsize > 2)
    {
      md_convert_frag (sec->owner, sec, fragp);
      frag_wane (fragp);
    }

  return newsize - oldsize;
}

/* Round up a section size to the appropriate boundary.	 */

valueT
md_section_align (segT	 segment ATTRIBUTE_UNUSED,
		  valueT size)
{
  return size;
}

/* This is called from HANDLE_ALIGN in write.c.	 Fill in the contents
   of an rs_align_code fragment.  */

void
arm_handle_align (fragS * fragP)
{
  static unsigned char const arm_noop[2][2][4] =
    {
      {  /* ARMv1 */
	{0x00, 0x00, 0xa0, 0xe1},  /* LE */
	{0xe1, 0xa0, 0x00, 0x00},  /* BE */
      },
      {  /* ARMv6k */
	{0x00, 0xf0, 0x20, 0xe3},  /* LE */
	{0xe3, 0x20, 0xf0, 0x00},  /* BE */
      },
    };
  static unsigned char const thumb_noop[2][2][2] =
    {
      {  /* Thumb-1 */
	{0xc0, 0x46},  /* LE */
	{0x46, 0xc0},  /* BE */
      },
      {  /* Thumb-2 */
	{0x00, 0xbf},  /* LE */
	{0xbf, 0x00}   /* BE */
      }
    };

  unsigned bytes, fix, noop_size;
  char * p;
  const unsigned char * noop;
  const unsigned char *narrow_noop = NULL;
  enum mstate state;

  if (fragP->fr_type != rs_align_code)
    return;

  bytes = fragP->fr_next->fr_address - fragP->fr_address - fragP->fr_fix;
  p = fragP->fr_literal + fragP->fr_fix;
  fix = 0;

  if (bytes > MAX_MEM_FOR_RS_ALIGN_CODE)
    bytes &= MAX_MEM_FOR_RS_ALIGN_CODE;

  gas_assert ((fragP->tc_frag_data.thumb_mode & MODE_RECORDED) != 0);

  if (fragP->tc_frag_data.thumb_mode & (~ MODE_RECORDED))
    {
	noop = thumb_noop[0][target_big_endian];
      noop_size = 2;
      state = MAP_THUMB;
    }
  else
    {
      noop = arm_noop[0][target_big_endian];
      noop_size = 4;
      state = MAP_ARM;
    }

  fragP->fr_var = noop_size;

  if (bytes & (noop_size - 1))
    {
      fix = bytes & (noop_size - 1);
      insert_data_mapping_symbol (state, fragP->fr_fix, fragP, fix);
      memset (p, 0, fix);
      p += fix;
      bytes -= fix;
    }

  if (narrow_noop)
    {
      if (bytes & noop_size)
	{
	  /* Insert a narrow noop.  */
	  memcpy (p, narrow_noop, noop_size);
	  p += noop_size;
	  bytes -= noop_size;
	  fix += noop_size;
	}

      /* Use wide noops for the remainder */
      noop_size = 4;
    }

  while (bytes >= noop_size)
    {
      memcpy (p, noop, noop_size);
      p += noop_size;
      bytes -= noop_size;
      fix += noop_size;
    }

  fragP->fr_fix += fix;
}

/* Called from md_do_align.  Used to create an alignment
   frag in a code section.  */

void
arm_frag_align_code (int n, int max)
{
  char * p;

  /* We assume that there will never be a requirement
     to support alignments greater than MAX_MEM_FOR_RS_ALIGN_CODE bytes.  */
  if (max > MAX_MEM_FOR_RS_ALIGN_CODE)
    {
      char err_msg[128];

      sprintf (err_msg,
	_("alignments greater than %d bytes not supported in .text sections."),
	MAX_MEM_FOR_RS_ALIGN_CODE + 1);
      as_fatal ("%s", err_msg);
    }

  p = frag_var (rs_align_code,
		MAX_MEM_FOR_RS_ALIGN_CODE,
		1,
		(relax_substateT) max,
		(symbolS *) NULL,
		(offsetT) n,
		(char *) NULL);
  *p = 0;
}

/* Perform target specific initialisation of a frag.
   Note - despite the name this initialisation is not done when the frag
   is created, but only when its type is assigned.  A frag can be created
   and used a long time before its type is set, so beware of assuming that
   this initialisation is performed first.  */

void
arm_init_frag (fragS * fragP, int max_chars)
{
  bfd_boolean frag_thumb_mode;

  /* If the current ARM vs THUMB mode has not already
     been recorded into this frag then do so now.  */
  if ((fragP->tc_frag_data.thumb_mode & MODE_RECORDED) == 0)
    fragP->tc_frag_data.thumb_mode = thumb_mode | MODE_RECORDED;

  /* PR 21809: Do not set a mapping state for debug sections
     - it just confuses other tools.  */
  if (bfd_get_section_flags (NULL, now_seg) & SEC_DEBUGGING)
    return;

  frag_thumb_mode = fragP->tc_frag_data.thumb_mode ^ MODE_RECORDED;

  /* Record a mapping symbol for alignment frags.  We will delete this
     later if the alignment ends up empty.  */
  switch (fragP->fr_type)
    {
    case rs_align:
    case rs_align_test:
    case rs_fill:
      mapping_state_2 (MAP_DATA, max_chars);
      break;
    case rs_align_code:
      mapping_state_2 (frag_thumb_mode ? MAP_THUMB : MAP_ARM, max_chars);
      break;
    default:
      break;
    }
}

/* When we change sections we need to issue a new mapping symbol.  */

void
arm_elf_change_section (void)
{
  /* Link an unlinked unwind index table section to the .text section.	*/
  if (elf_section_type (now_seg) == SHT_ARM_EXIDX
      && elf_linked_to_section (now_seg) == NULL)
    elf_linked_to_section (now_seg) = text_section;
}

int
arm_elf_section_type (const char * str, size_t len)
{
  if (len == 5 && strncmp (str, "exidx", 5) == 0)
    return SHT_ARM_EXIDX;

  return -1;
}

/* Code to deal with unwinding tables.	*/

static void add_unwind_adjustsp (offsetT);

/* Generate any deferred unwind frame offset.  */

static void
flush_pending_unwind (void)
{
  offsetT offset;

  offset = unwind.pending_offset;
  unwind.pending_offset = 0;
  if (offset != 0)
    add_unwind_adjustsp (offset);
}

/* Add an opcode to this list for this function.  Two-byte opcodes should
   be passed as op[0] << 8 | op[1].  The list of opcodes is built in reverse
   order.  */

static void
add_unwind_opcode (valueT op, int length)
{
  /* Add any deferred stack adjustment.	 */
  if (unwind.pending_offset)
    flush_pending_unwind ();

  unwind.sp_restored = 0;

  if (unwind.opcode_count + length > unwind.opcode_alloc)
    {
      unwind.opcode_alloc += ARM_OPCODE_CHUNK_SIZE;
      if (unwind.opcodes)
	unwind.opcodes = XRESIZEVEC (unsigned char, unwind.opcodes,
				     unwind.opcode_alloc);
      else
	unwind.opcodes = XNEWVEC (unsigned char, unwind.opcode_alloc);
    }
  while (length > 0)
    {
      length--;
      unwind.opcodes[unwind.opcode_count] = op & 0xff;
      op >>= 8;
      unwind.opcode_count++;
    }
}

/* Add unwind opcodes to adjust the stack pointer.  */

static void
add_unwind_adjustsp (offsetT offset)
{
  valueT op;

  if (offset > 0x200)
    {
      /* We need at most 5 bytes to hold a 32-bit value in a uleb128.  */
      char bytes[5];
      int n;
      valueT o;

      /* Long form: 0xb2, uleb128.  */
      /* This might not fit in a word so add the individual bytes,
	 remembering the list is built in reverse order.  */
      o = (valueT) ((offset - 0x204) >> 2);
      if (o == 0)
	add_unwind_opcode (0, 1);

      /* Calculate the uleb128 encoding of the offset.	*/
      n = 0;
      while (o)
	{
	  bytes[n] = o & 0x7f;
	  o >>= 7;
	  if (o)
	    bytes[n] |= 0x80;
	  n++;
	}
      /* Add the insn.	*/
      for (; n; n--)
	add_unwind_opcode (bytes[n - 1], 1);
      add_unwind_opcode (0xb2, 1);
    }
  else if (offset > 0x100)
    {
      /* Two short opcodes.  */
      add_unwind_opcode (0x3f, 1);
      op = (offset - 0x104) >> 2;
      add_unwind_opcode (op, 1);
    }
  else if (offset > 0)
    {
      /* Short opcode.	*/
      op = (offset - 4) >> 2;
      add_unwind_opcode (op, 1);
    }
  else if (offset < 0)
    {
      offset = -offset;
      while (offset > 0x100)
	{
	  add_unwind_opcode (0x7f, 1);
	  offset -= 0x100;
	}
      op = ((offset - 4) >> 2) | 0x40;
      add_unwind_opcode (op, 1);
    }
}

/* Finish the list of unwind opcodes for this function.	 */

static void
finish_unwind_opcodes (void)
{
  valueT op;

  if (unwind.fp_used)
    {
      /* Adjust sp as necessary.  */
      unwind.pending_offset += unwind.fp_offset - unwind.frame_size;
      flush_pending_unwind ();

      /* After restoring sp from the frame pointer.  */
      op = 0x90 | unwind.fp_reg;
      add_unwind_opcode (op, 1);
    }
  else
    flush_pending_unwind ();
}


/* Start an exception table entry.  If idx is nonzero this is an index table
   entry.  */

static void
start_unwind_section (const segT text_seg, int idx)
{
  const char * text_name;
  const char * prefix;
  const char * prefix_once;
  const char * group_name;
  char * sec_name;
  int type;
  int flags;
  int linkonce;

  if (idx)
    {
      prefix = ELF_STRING_ARM_unwind;
      prefix_once = ELF_STRING_ARM_unwind_once;
      type = SHT_ARM_EXIDX;
    }
  else
    {
      prefix = ELF_STRING_ARM_unwind_info;
      prefix_once = ELF_STRING_ARM_unwind_info_once;
      type = SHT_PROGBITS;
    }

  text_name = segment_name (text_seg);
  if (streq (text_name, ".text"))
    text_name = "";

  if (strncmp (text_name, ".gnu.linkonce.t.",
	       strlen (".gnu.linkonce.t.")) == 0)
    {
      prefix = prefix_once;
      text_name += strlen (".gnu.linkonce.t.");
    }

  sec_name = concat (prefix, text_name, (char *) NULL);

  flags = SHF_ALLOC;
  linkonce = 0;
  group_name = 0;

  /* Handle COMDAT group.  */
  if (prefix != prefix_once && (text_seg->flags & SEC_LINK_ONCE) != 0)
    {
      group_name = elf_group_name (text_seg);
      if (group_name == NULL)
	{
	  as_bad (_("Group section `%s' has no group signature"),
		  segment_name (text_seg));
	  ignore_rest_of_line ();
	  return;
	}
      flags |= SHF_GROUP;
      linkonce = 1;
    }

  obj_elf_change_section (sec_name, type, 0, flags, 0, group_name,
			  linkonce, 0);

  /* Set the section link for index tables.  */
  if (idx)
    elf_linked_to_section (now_seg) = text_seg;
}


/* Start an unwind table entry.	 HAVE_DATA is nonzero if we have additional
   personality routine data.  Returns zero, or the index table value for
   an inline entry.  */

static valueT
create_unwind_entry (int have_data)
{
  int size;
  addressT where;
  char *ptr;
  /* The current word of data.	*/
  valueT data;
  /* The number of bytes left in this word.  */
  int n;

  finish_unwind_opcodes ();

  /* Remember the current text section.	 */
  unwind.saved_seg = now_seg;
  unwind.saved_subseg = now_subseg;

  start_unwind_section (now_seg, 0);

  if (unwind.personality_routine == NULL)
    {
      if (unwind.personality_index == -2)
	{
	  if (have_data)
	    as_bad (_("handlerdata in cantunwind frame"));
	  return 1; /* EXIDX_CANTUNWIND.  */
	}

      /* Use a default personality routine if none is specified.  */
      if (unwind.personality_index == -1)
	{
	  if (unwind.opcode_count > 3)
	    unwind.personality_index = 1;
	  else
	    unwind.personality_index = 0;
	}

      /* Space for the personality routine entry.  */
      if (unwind.personality_index == 0)
	{
	  if (unwind.opcode_count > 3)
	    as_bad (_("too many unwind opcodes for personality routine 0"));

	  if (!have_data)
	    {
	      /* All the data is inline in the index table.  */
	      data = 0x80;
	      n = 3;
	      while (unwind.opcode_count > 0)
		{
		  unwind.opcode_count--;
		  data = (data << 8) | unwind.opcodes[unwind.opcode_count];
		  n--;
		}

	      /* Pad with "finish" opcodes.  */
	      while (n--)
		data = (data << 8) | 0xb0;

	      return data;
	    }
	  size = 0;
	}
      else
	/* We get two opcodes "free" in the first word.	 */
	size = unwind.opcode_count - 2;
    }
  else
    {
      /* PR 16765: Missing or misplaced unwind directives can trigger this.  */
      if (unwind.personality_index != -1)
	{
	  as_bad (_("attempt to recreate an unwind entry"));
	  return 1;
	}

      /* An extra byte is required for the opcode count.	*/
      size = unwind.opcode_count + 1;
    }

  size = (size + 3) >> 2;
  if (size > 0xff)
    as_bad (_("too many unwind opcodes"));

  frag_align (2, 0, 0);
  record_alignment (now_seg, 2);
  unwind.table_entry = expr_build_dot ();

  /* Allocate the table entry.	*/
  ptr = frag_more ((size << 2) + 4);
  /* PR 13449: Zero the table entries in case some of them are not used.  */
  memset (ptr, 0, (size << 2) + 4);
  where = frag_now_fix () - ((size << 2) + 4);

  switch (unwind.personality_index)
    {
    case -1:
      /* ??? Should this be a PLT generating relocation?  */
      /* Custom personality routine.  */
      fix_new (frag_now, where, 4, unwind.personality_routine, 0, 1,
	       BFD_RELOC_ARM_PREL31);

      where += 4;
      ptr += 4;

      /* Set the first byte to the number of additional words.	*/
      data = size > 0 ? size - 1 : 0;
      n = 3;
      break;

    /* ABI defined personality routines.  */
    case 0:
      /* Three opcodes bytes are packed into the first word.  */
      data = 0x80;
      n = 3;
      break;

    case 1:
    case 2:
      /* The size and first two opcode bytes go in the first word.  */
      data = ((0x80 + unwind.personality_index) << 8) | size;
      n = 2;
      break;

    default:
      /* Should never happen.  */
      abort ();
    }

  /* Pack the opcodes into words (MSB first), reversing the list at the same
     time.  */
  while (unwind.opcode_count > 0)
    {
      if (n == 0)
	{
	  md_number_to_chars (ptr, data, 4);
	  ptr += 4;
	  n = 4;
	  data = 0;
	}
      unwind.opcode_count--;
      n--;
      data = (data << 8) | unwind.opcodes[unwind.opcode_count];
    }

  /* Finish off the last word.	*/
  if (n < 4)
    {
      /* Pad with "finish" opcodes.  */
      while (n--)
	data = (data << 8) | 0xb0;

      md_number_to_chars (ptr, data, 4);
    }

  if (!have_data)
    {
      /* Add an empty descriptor if there is no user-specified data.   */
      ptr = frag_more (4);
      md_number_to_chars (ptr, 0, 4);
    }

  return 0;
}


/* Initialize the DWARF-2 unwind information for this procedure.  */

void
tc_arm_frame_initial_instructions (void)
{
  cfi_add_CFA_def_cfa (REG_SP, 0);
}

/* Convert REGNAME to a DWARF-2 register number.  */

int
tc_arm_regname_to_dw2regnum (char *regname)
{
  int reg = arm_reg_parse (&regname, REG_TYPE_RN);
  if (reg != FAIL)
    return reg;

  /* PR 16694: Allow VFP registers as well.  */
  reg = arm_reg_parse (&regname, REG_TYPE_VFS);
  if (reg != FAIL)
    return 64 + reg;

  reg = arm_reg_parse (&regname, REG_TYPE_VFD);
  if (reg != FAIL)
    return reg + 256;

  return FAIL;
}

/* MD interface: Symbol and relocation handling.  */

/* Return the address within the segment that a PC-relative fixup is
   relative to.  For ARM, PC-relative fixups applied to instructions
   are generally relative to the location of the fixup plus 8 bytes.
   Thumb branches are offset by 4, and Thumb loads relative to PC
   require special handling.  */

long
md_pcrel_from_section (fixS * fixP, segT seg)
{
  offsetT base = fixP->fx_where + fixP->fx_frag->fr_address;

  /* If this is pc-relative and we are going to emit a relocation
     then we just want to put out any pipeline compensation that the linker
     will need.  Otherwise we want to use the calculated base.
     For WinCE we skip the bias for externals as well, since this
     is how the MS ARM-CE assembler behaves and we want to be compatible.  */
  if (fixP->fx_pcrel
      && ((fixP->fx_addsy && S_GET_SEGMENT (fixP->fx_addsy) != seg)
	  || (arm_force_relocation (fixP))))
    base = 0;


  switch (fixP->fx_r_type)
    {
      /* PC relative addressing on the Thumb is slightly odd as the
	 bottom two bits of the PC are forced to zero for the
	 calculation.  This happens *after* application of the
	 pipeline offset.  However, Thumb adrl already adjusts for
	 this, so we need not do it again.  */
    case BFD_RELOC_ARM_THUMB_ADD:
      return base & ~3;

    case BFD_RELOC_ARM_THUMB_OFFSET:
    case BFD_RELOC_ARM_T32_OFFSET_IMM:
    case BFD_RELOC_ARM_T32_ADD_PC12:
    case BFD_RELOC_ARM_T32_CP_OFF_IMM:
      return (base + 4) & ~3;

      /* Thumb branches are simply offset by +4.  */
    case BFD_RELOC_THUMB_PCREL_BRANCH7:
    case BFD_RELOC_THUMB_PCREL_BRANCH9:
    case BFD_RELOC_THUMB_PCREL_BRANCH12:
    case BFD_RELOC_THUMB_PCREL_BRANCH20:
    case BFD_RELOC_THUMB_PCREL_BRANCH25:
      return base + 4;

    case BFD_RELOC_THUMB_PCREL_BRANCH23:
       return base + 4;

      /* BLX is like branches above, but forces the low two bits of PC to
	 zero.  */
    case BFD_RELOC_THUMB_PCREL_BLX:
    case BFD_RELOC_ARM_PCREL_BLX:
    case BFD_RELOC_ARM_PCREL_CALL:
    case BFD_RELOC_ARM_PCREL_BRANCH:
    case BFD_RELOC_ARM_PCREL_JUMP:
    case BFD_RELOC_ARM_PLT32:
      return base + 8;

      /* ARM mode loads relative to PC are also offset by +8.  Unlike
	 branches, the Windows CE loader *does* expect the relocation
	 to take this into account.  */
    case BFD_RELOC_ARM_OFFSET_IMM:
    case BFD_RELOC_ARM_OFFSET_IMM8:
    case BFD_RELOC_ARM_HWLITERAL:
    case BFD_RELOC_ARM_LITERAL:
    case BFD_RELOC_ARM_CP_OFF_IMM:
      return base + 8;


      /* Other PC-relative relocations are un-offset.  */
    default:
      return base;
    }
}

static bfd_boolean flag_warn_syms = TRUE;

bfd_boolean
arm_tc_equal_in_insn (int c ATTRIBUTE_UNUSED, char * name)
{
  /* PR 18347 - Warn if the user attempts to create a symbol with the same
     name as an ARM instruction.  Whilst strictly speaking it is allowed, it
     does mean that the resulting code might be very confusing to the reader.
     Also this warning can be triggered if the user omits an operand before
     an immediate address, eg:

       LDR =foo

     GAS treats this as an assignment of the value of the symbol foo to a
     symbol LDR, and so (without this code) it will not issue any kind of
     warning or error message.

     Note - ARM instructions are case-insensitive but the strings in the hash
     table are all stored in lower case, so we must first ensure that name is
     lower case too.  */
  if (flag_warn_syms && arm_ops_hsh)
    {
      char * nbuf = strdup (name);
      char * p;

      for (p = nbuf; *p; p++)
	*p = TOLOWER (*p);
      if (hash_find (arm_ops_hsh, nbuf) != NULL)
	{
	  static struct hash_control * already_warned = NULL;

	  if (already_warned == NULL)
	    already_warned = hash_new ();
	  /* Only warn about the symbol once.  To keep the code
	     simple we let hash_insert do the lookup for us.  */
	  if (hash_insert (already_warned, name, NULL) == NULL)
	    as_warn (_("[-mwarn-syms]: Assignment makes a symbol match an ARM instruction: %s"), name);
	}
      else
	free (nbuf);
    }

  return FALSE;
}

/* Under ELF we need to default _GLOBAL_OFFSET_TABLE.
   Otherwise we have no need to default values of symbols.  */

symbolS *
md_undefined_symbol (char * name ATTRIBUTE_UNUSED)
{
  if (name[0] == '_' && name[1] == 'G'
      && streq (name, GLOBAL_OFFSET_TABLE_NAME))
    {
      if (!GOT_symbol)
	{
	  if (symbol_find (name))
	    as_bad (_("GOT already in the symbol table"));

	  GOT_symbol = symbol_new (name, undefined_section,
				   (valueT) 0, & zero_address_frag);
	}

      return GOT_symbol;
    }

  return NULL;
}

/* Subroutine of md_apply_fix.	 Check to see if an immediate can be
   computed as two separate immediate values, added together.  We
   already know that this value cannot be computed by just one ARM
   instruction.	 */

static unsigned int
validate_immediate_twopart (unsigned int   val,
			    unsigned int * highpart)
{
  unsigned int a;
  unsigned int i;

  for (i = 0; i < 32; i += 2)
    if (((a = rotate_left (val, i)) & 0xff) != 0)
      {
	if (a & 0xff00)
	  {
	    if (a & ~ 0xffff)
	      continue;
	    * highpart = (a  >> 8) | ((i + 24) << 7);
	  }
	else if (a & 0xff0000)
	  {
	    if (a & 0xff000000)
	      continue;
	    * highpart = (a >> 16) | ((i + 16) << 7);
	  }
	else
	  {
	    gas_assert (a & 0xff000000);
	    * highpart = (a >> 24) | ((i + 8) << 7);
	  }

	return (a & 0xff) | (i << 7);
      }

  return FAIL;
}

static int
validate_offset_imm (unsigned int val, int hwse)
{
  if ((hwse && val > 255) || val > 4095)
    return FAIL;
  return val;
}

/* Subroutine of md_apply_fix.	 Do those data_ops which can take a
   negative immediate constant by altering the instruction.  A bit of
   a hack really.
	MOV <-> MVN
	AND <-> BIC
	ADC <-> SBC
	by inverting the second operand, and
	ADD <-> SUB
	CMP <-> CMN
	by negating the second operand.	 */

static int
negate_data_op (unsigned long * instruction,
		unsigned long	value)
{
  int op, new_inst;
  unsigned long negated, inverted;

  negated = encode_arm_immediate (-value);
  inverted = encode_arm_immediate (~value);

  op = (*instruction >> DATA_OP_SHIFT) & 0xf;
  switch (op)
    {
      /* First negates.	 */
    case OPCODE_SUB:		 /* ADD <-> SUB	 */
      new_inst = OPCODE_ADD;
      value = negated;
      break;

    case OPCODE_ADD:
      new_inst = OPCODE_SUB;
      value = negated;
      break;

    case OPCODE_CMP:		 /* CMP <-> CMN	 */
      new_inst = OPCODE_CMN;
      value = negated;
      break;

    case OPCODE_CMN:
      new_inst = OPCODE_CMP;
      value = negated;
      break;

      /* Now Inverted ops.  */
    case OPCODE_MOV:		 /* MOV <-> MVN	 */
      new_inst = OPCODE_MVN;
      value = inverted;
      break;

    case OPCODE_MVN:
      new_inst = OPCODE_MOV;
      value = inverted;
      break;

    case OPCODE_AND:		 /* AND <-> BIC	 */
      new_inst = OPCODE_BIC;
      value = inverted;
      break;

    case OPCODE_BIC:
      new_inst = OPCODE_AND;
      value = inverted;
      break;

    case OPCODE_ADC:		  /* ADC <-> SBC  */
      new_inst = OPCODE_SBC;
      value = inverted;
      break;

    case OPCODE_SBC:
      new_inst = OPCODE_ADC;
      value = inverted;
      break;

      /* We cannot do anything.	 */
    default:
      return FAIL;
    }

  if (value == (unsigned) FAIL)
    return FAIL;

  *instruction &= OPCODE_MASK;
  *instruction |= new_inst << DATA_OP_SHIFT;
  return value;
}

/* Like negate_data_op, but for Thumb-2.   */

static unsigned int
thumb32_negate_data_op (offsetT *instruction, unsigned int value)
{
  int op, new_inst;
  int rd;
  unsigned int negated, inverted;

  negated = encode_thumb32_immediate (-value);
  inverted = encode_thumb32_immediate (~value);

  rd = (*instruction >> 8) & 0xf;
  op = (*instruction >> T2_DATA_OP_SHIFT) & 0xf;
  switch (op)
    {
      /* ADD <-> SUB.  Includes CMP <-> CMN.  */
    case T2_OPCODE_SUB:
      new_inst = T2_OPCODE_ADD;
      value = negated;
      break;

    case T2_OPCODE_ADD:
      new_inst = T2_OPCODE_SUB;
      value = negated;
      break;

      /* ORR <-> ORN.  Includes MOV <-> MVN.  */
    case T2_OPCODE_ORR:
      new_inst = T2_OPCODE_ORN;
      value = inverted;
      break;

    case T2_OPCODE_ORN:
      new_inst = T2_OPCODE_ORR;
      value = inverted;
      break;

      /* AND <-> BIC.  TST has no inverted equivalent.  */
    case T2_OPCODE_AND:
      new_inst = T2_OPCODE_BIC;
      if (rd == 15)
	value = FAIL;
      else
	value = inverted;
      break;

    case T2_OPCODE_BIC:
      new_inst = T2_OPCODE_AND;
      value = inverted;
      break;

      /* ADC <-> SBC  */
    case T2_OPCODE_ADC:
      new_inst = T2_OPCODE_SBC;
      value = inverted;
      break;

    case T2_OPCODE_SBC:
      new_inst = T2_OPCODE_ADC;
      value = inverted;
      break;

      /* We cannot do anything.	 */
    default:
      return FAIL;
    }

  if (value == (unsigned int)FAIL)
    return FAIL;

  *instruction &= T2_OPCODE_MASK;
  *instruction |= new_inst << T2_DATA_OP_SHIFT;
  return value;
}

/* Read a 32-bit thumb instruction from buf.  */

static unsigned long
get_thumb32_insn (char * buf)
{
  unsigned long insn;
  insn = md_chars_to_number (buf, THUMB_SIZE) << 16;
  insn |= md_chars_to_number (buf + THUMB_SIZE, THUMB_SIZE);

  return insn;
}

/* We usually want to set the low bit on the address of thumb function
   symbols.  In particular .word foo - . should have the low bit set.
   Generic code tries to fold the difference of two symbols to
   a constant.  Prevent this and force a relocation when the first symbols
   is a thumb function.  */

bfd_boolean
arm_optimize_expr (expressionS *l, operatorT op, expressionS *r)
{
  if (op == O_subtract
      && l->X_op == O_symbol
      && r->X_op == O_symbol
      && THUMB_IS_FUNC (l->X_add_symbol))
    {
      l->X_op = O_subtract;
      l->X_op_symbol = r->X_add_symbol;
      l->X_add_number -= r->X_add_number;
      return TRUE;
    }

  /* Process as normal.  */
  return FALSE;
}

/* Encode Thumb2 unconditional branches and calls. The encoding
   for the 2 are identical for the immediate values.  */

static void
encode_thumb2_b_bl_offset (char * buf, offsetT value)
{
#define T2I1I2MASK  ((1 << 13) | (1 << 11))
  offsetT newval;
  offsetT newval2;
  addressT S, I1, I2, lo, hi;

  S = (value >> 24) & 0x01;
  I1 = (value >> 23) & 0x01;
  I2 = (value >> 22) & 0x01;
  hi = (value >> 12) & 0x3ff;
  lo = (value >> 1) & 0x7ff;
  newval   = md_chars_to_number (buf, THUMB_SIZE);
  newval2  = md_chars_to_number (buf + THUMB_SIZE, THUMB_SIZE);
  newval  |= (S << 10) | hi;
  newval2 &=  ~T2I1I2MASK;
  newval2 |= (((I1 ^ S) << 13) | ((I2 ^ S) << 11) | lo) ^ T2I1I2MASK;
  md_number_to_chars (buf, newval, THUMB_SIZE);
  md_number_to_chars (buf + THUMB_SIZE, newval2, THUMB_SIZE);
}

void
md_apply_fix (fixS *	fixP,
	       valueT * valP,
	       segT	seg)
{
  offsetT	 value = * valP;
  offsetT	 newval;
  unsigned int	 newimm;
  unsigned long	 temp;
  int		 sign;
  char *	 buf = fixP->fx_where + fixP->fx_frag->fr_literal;

  gas_assert (fixP->fx_r_type <= BFD_RELOC_UNUSED);

  /* Note whether this will delete the relocation.  */

  if (fixP->fx_addsy == 0 && !fixP->fx_pcrel)
    fixP->fx_done = 1;

  /* On a 64-bit host, silently truncate 'value' to 32 bits for
     consistency with the behaviour on 32-bit hosts.  Remember value
     for emit_reloc.  */
  value &= 0xffffffff;
  value ^= 0x80000000;
  value -= 0x80000000;

  *valP = value;
  fixP->fx_addnumber = value;

  /* Same treatment for fixP->fx_offset.  */
  fixP->fx_offset &= 0xffffffff;
  fixP->fx_offset ^= 0x80000000;
  fixP->fx_offset -= 0x80000000;

  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_NONE:
      /* This will need to go in the object file.  */
      fixP->fx_done = 0;
      break;

    case BFD_RELOC_ARM_IMMEDIATE:
      /* We claim that this fixup has been processed here,
	 even if in fact we generate an error because we do
	 not have a reloc for it, so tc_gen_reloc will reject it.  */
      fixP->fx_done = 1;

      if (fixP->fx_addsy)
	{
	  const char *msg = 0;

	  if (! S_IS_DEFINED (fixP->fx_addsy))
	    msg = _("undefined symbol %s used as an immediate value");
	  else if (S_GET_SEGMENT (fixP->fx_addsy) != seg)
	    msg = _("symbol %s is in a different section");
	  else if (S_IS_WEAK (fixP->fx_addsy))
	    msg = _("symbol %s is weak and may be overridden later");

	  if (msg)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    msg, S_GET_NAME (fixP->fx_addsy));
	      break;
	    }
	}

      temp = md_chars_to_number (buf, INSN_SIZE);

      /* If the offset is negative, we should use encoding A2 for ADR.  */
      if ((temp & 0xfff0000) == 0x28f0000 && value < 0)
	newimm = negate_data_op (&temp, value);
      else
	{
	  newimm = encode_arm_immediate (value);

	  /* If the instruction will fail, see if we can fix things up by
	     changing the opcode.  */
	  if (newimm == (unsigned int) FAIL)
	    newimm = negate_data_op (&temp, value);
	}

      if (newimm == (unsigned int) FAIL)
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("invalid constant (%lx) after fixup"),
			(unsigned long) value);
	  break;
	}

      newimm |= (temp & 0xfffff000);
      md_number_to_chars (buf, (valueT) newimm, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_ADRL_IMMEDIATE:
      {
	unsigned int highpart = 0;
	unsigned int newinsn  = 0xe1a00000; /* nop.  */

	if (fixP->fx_addsy)
	  {
	    const char *msg = 0;

	    if (! S_IS_DEFINED (fixP->fx_addsy))
	      msg = _("undefined symbol %s used as an immediate value");
	    else if (S_GET_SEGMENT (fixP->fx_addsy) != seg)
	      msg = _("symbol %s is in a different section");
	    else if (S_IS_WEAK (fixP->fx_addsy))
	      msg = _("symbol %s is weak and may be overridden later");

	    if (msg)
	      {
		as_bad_where (fixP->fx_file, fixP->fx_line,
			      msg, S_GET_NAME (fixP->fx_addsy));
		break;
	      }
	  }

	newimm = encode_arm_immediate (value);
	temp = md_chars_to_number (buf, INSN_SIZE);

	/* If the instruction will fail, see if we can fix things up by
	   changing the opcode.	 */
	if (newimm == (unsigned int) FAIL
	    && (newimm = negate_data_op (& temp, value)) == (unsigned int) FAIL)
	  {
	    /* No ?  OK - try using two ADD instructions to generate
	       the value.  */
	    newimm = validate_immediate_twopart (value, & highpart);

	    /* Yes - then make sure that the second instruction is
	       also an add.  */
	    if (newimm != (unsigned int) FAIL)
	      newinsn = temp;
	    /* Still No ?  Try using a negated value.  */
	    else if ((newimm = validate_immediate_twopart (- value, & highpart)) != (unsigned int) FAIL)
	      temp = newinsn = (temp & OPCODE_MASK) | OPCODE_SUB << DATA_OP_SHIFT;
	    /* Otherwise - give up.  */
	    else
	      {
		as_bad_where (fixP->fx_file, fixP->fx_line,
			      _("unable to compute ADRL instructions for PC offset of 0x%lx"),
			      (long) value);
		break;
	      }

	    /* Replace the first operand in the 2nd instruction (which
	       is the PC) with the destination register.  We have
	       already added in the PC in the first instruction and we
	       do not want to do it again.  */
	    newinsn &= ~ 0xf0000;
	    newinsn |= ((newinsn & 0x0f000) << 4);
	  }

	newimm |= (temp & 0xfffff000);
	md_number_to_chars (buf, (valueT) newimm, INSN_SIZE);

	highpart |= (newinsn & 0xfffff000);
	md_number_to_chars (buf + INSN_SIZE, (valueT) highpart, INSN_SIZE);
      }
      break;

    case BFD_RELOC_ARM_OFFSET_IMM:
      if (!fixP->fx_done && seg->use_rela_p)
	value = 0;
      /* Fall through.  */

    case BFD_RELOC_ARM_LITERAL:
      sign = value > 0;

      if (value < 0)
	value = - value;

      if (validate_offset_imm (value, 0) == FAIL)
	{
	  if (fixP->fx_r_type == BFD_RELOC_ARM_LITERAL)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid literal constant: pool needs to be closer"));
	  else
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad immediate value for offset (%ld)"),
			  (long) value);
	  break;
	}

      newval = md_chars_to_number (buf, INSN_SIZE);
      if (value == 0)
	newval &= 0xfffff000;
      else
	{
	  newval &= 0xff7ff000;
	  newval |= value | (sign ? INDEX_UP : 0);
	}
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_OFFSET_IMM8:
    case BFD_RELOC_ARM_HWLITERAL:
      sign = value > 0;

      if (value < 0)
	value = - value;

      if (validate_offset_imm (value, 1) == FAIL)
	{
	  if (fixP->fx_r_type == BFD_RELOC_ARM_HWLITERAL)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid literal constant: pool needs to be closer"));
	  else
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad immediate value for 8-bit offset (%ld)"),
			  (long) value);
	  break;
	}

      newval = md_chars_to_number (buf, INSN_SIZE);
      if (value == 0)
	newval &= 0xfffff0f0;
      else
	{
	  newval &= 0xff7ff0f0;
	  newval |= ((value >> 4) << 8) | (value & 0xf) | (sign ? INDEX_UP : 0);
	}
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_T32_OFFSET_U8:
      if (value < 0 || value > 1020 || value % 4 != 0)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("bad immediate value for offset (%ld)"), (long) value);
      value /= 4;

      newval = md_chars_to_number (buf+2, THUMB_SIZE);
      newval |= value;
      md_number_to_chars (buf+2, newval, THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_T32_OFFSET_IMM:
      /* This is a complicated relocation used for all varieties of Thumb32
	 load/store instruction with immediate offset:

	 1110 100P u1WL NNNN XXXX YYYY iiii iiii - +/-(U) pre/post(P) 8-bit,
						   *4, optional writeback(W)
						   (doubleword load/store)

	 1111 100S uTTL 1111 XXXX iiii iiii iiii - +/-(U) 12-bit PC-rel
	 1111 100S 0TTL NNNN XXXX 1Pu1 iiii iiii - +/-(U) pre/post(P) 8-bit
	 1111 100S 0TTL NNNN XXXX 1110 iiii iiii - positive 8-bit (T instruction)
	 1111 100S 1TTL NNNN XXXX iiii iiii iiii - positive 12-bit
	 1111 100S 0TTL NNNN XXXX 1100 iiii iiii - negative 8-bit

	 Uppercase letters indicate bits that are already encoded at
	 this point.  Lowercase letters are our problem.  For the
	 second block of instructions, the secondary opcode nybble
	 (bits 8..11) is present, and bit 23 is zero, even if this is
	 a PC-relative operation.  */
      newval = md_chars_to_number (buf, THUMB_SIZE);
      newval <<= 16;
      newval |= md_chars_to_number (buf+THUMB_SIZE, THUMB_SIZE);

      if ((newval & 0xf0000000) == 0xe0000000)
	{
	  /* Doubleword load/store: 8-bit offset, scaled by 4.  */
	  if (value >= 0)
	    newval |= (1 << 23);
	  else
	    value = -value;
	  if (value % 4 != 0)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset not a multiple of 4"));
	      break;
	    }
	  value /= 4;
	  if (value > 0xff)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset out of range"));
	      break;
	    }
	  newval &= ~0xff;
	}
      else if ((newval & 0x000f0000) == 0x000f0000)
	{
	  /* PC-relative, 12-bit offset.  */
	  if (value >= 0)
	    newval |= (1 << 23);
	  else
	    value = -value;
	  if (value > 0xfff)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset out of range"));
	      break;
	    }
	  newval &= ~0xfff;
	}
      else if ((newval & 0x00000100) == 0x00000100)
	{
	  /* Writeback: 8-bit, +/- offset.  */
	  if (value >= 0)
	    newval |= (1 << 9);
	  else
	    value = -value;
	  if (value > 0xff)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset out of range"));
	      break;
	    }
	  newval &= ~0xff;
	}
      else if ((newval & 0x00000f00) == 0x00000e00)
	{
	  /* T-instruction: positive 8-bit offset.  */
	  if (value < 0 || value > 0xff)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset out of range"));
	      break;
	    }
	  newval &= ~0xff;
	  newval |= value;
	}
      else
	{
	  /* Positive 12-bit or negative 8-bit offset.  */
	  int limit;
	  if (value >= 0)
	    {
	      newval |= (1 << 23);
	      limit = 0xfff;
	    }
	  else
	    {
	      value = -value;
	      limit = 0xff;
	    }
	  if (value > limit)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("offset out of range"));
	      break;
	    }
	  newval &= ~limit;
	}

      newval |= value;
      md_number_to_chars (buf, (newval >> 16) & 0xffff, THUMB_SIZE);
      md_number_to_chars (buf + THUMB_SIZE, newval & 0xffff, THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_SHIFT_IMM:
      newval = md_chars_to_number (buf, INSN_SIZE);
      if (((unsigned long) value) > 32
	  || (value == 32
	      && (((newval & 0x60) == 0) || (newval & 0x60) == 0x60)))
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("shift expression is too large"));
	  break;
	}

      if (value == 0)
	/* Shifts of zero must be done as lsl.	*/
	newval &= ~0x60;
      else if (value == 32)
	value = 0;
      newval &= 0xfffff07f;
      newval |= (value & 0x1f) << 7;
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_T32_IMMEDIATE:
    case BFD_RELOC_ARM_T32_ADD_IMM:
    case BFD_RELOC_ARM_T32_IMM12:
    case BFD_RELOC_ARM_T32_ADD_PC12:
      /* We claim that this fixup has been processed here,
	 even if in fact we generate an error because we do
	 not have a reloc for it, so tc_gen_reloc will reject it.  */
      fixP->fx_done = 1;

      if (fixP->fx_addsy
	  && ! S_IS_DEFINED (fixP->fx_addsy))
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("undefined symbol %s used as an immediate value"),
			S_GET_NAME (fixP->fx_addsy));
	  break;
	}

      newval = md_chars_to_number (buf, THUMB_SIZE);
      newval <<= 16;
      newval |= md_chars_to_number (buf+2, THUMB_SIZE);

      newimm = FAIL;
      if (fixP->fx_r_type == BFD_RELOC_ARM_T32_ADD_IMM)
	{
	  newimm = encode_thumb32_immediate (value);
	  if (newimm == (unsigned int) FAIL)
	    newimm = thumb32_negate_data_op (&newval, value);
	}
      if (newimm == (unsigned int) FAIL)
	{
	  if (fixP->fx_r_type != BFD_RELOC_ARM_T32_IMMEDIATE)
	    {
	      /* Turn add/sum into addw/subw.  */
	      if (fixP->fx_r_type == BFD_RELOC_ARM_T32_ADD_IMM)
		newval = (newval & 0xfeffffff) | 0x02000000;
	      /* No flat 12-bit imm encoding for addsw/subsw.  */
	      if ((newval & 0x00100000) == 0)
		{
		  /* 12 bit immediate for addw/subw.  */
		  if (value < 0)
		    {
		      value = -value;
		      newval ^= 0x00a00000;
		    }
		  if (value > 0xfff)
		    newimm = (unsigned int) FAIL;
		  else
		    newimm = value;
		}
	    }
	}

      if (newimm == (unsigned int)FAIL)
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("invalid constant (%lx) after fixup"),
			(unsigned long) value);
	  break;
	}

      newval |= (newimm & 0x800) << 15;
      newval |= (newimm & 0x700) << 4;
      newval |= (newimm & 0x0ff);

      md_number_to_chars (buf,   (valueT) ((newval >> 16) & 0xffff), THUMB_SIZE);
      md_number_to_chars (buf+2, (valueT) (newval & 0xffff), THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_SMC:
      if (((unsigned long) value) > 0xffff)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid smc expression"));
      newval = md_chars_to_number (buf, INSN_SIZE);
      newval |= (value & 0xf) | ((value & 0xfff0) << 4);
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_HVC:
      if (((unsigned long) value) > 0xffff)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid hvc expression"));
      newval = md_chars_to_number (buf, INSN_SIZE);
      newval |= (value & 0xf) | ((value & 0xfff0) << 4);
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_SWI:
      if (fixP->tc_fix_data != 0)
	{
	  if (((unsigned long) value) > 0xff)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid swi expression"));
	  newval = md_chars_to_number (buf, THUMB_SIZE);
	  newval |= value;
	  md_number_to_chars (buf, newval, THUMB_SIZE);
	}
      else
	{
	  if (((unsigned long) value) > 0x00ffffff)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid swi expression"));
	  newval = md_chars_to_number (buf, INSN_SIZE);
	  newval |= value;
	  md_number_to_chars (buf, newval, INSN_SIZE);
	}
      break;

    case BFD_RELOC_ARM_MULTI:
      if (((unsigned long) value) > 0xffff)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid expression in load/store multiple"));
      newval = value | md_chars_to_number (buf, INSN_SIZE);
      md_number_to_chars (buf, newval, INSN_SIZE);
      break;

    case BFD_RELOC_ARM_PCREL_CALL:
	temp = 3;
      goto arm_branch_common;

    case BFD_RELOC_ARM_PCREL_JUMP:
    case BFD_RELOC_ARM_PLT32:
    case BFD_RELOC_ARM_PCREL_BRANCH:
      temp = 3;
      goto arm_branch_common;

    case BFD_RELOC_ARM_PCREL_BLX:

      temp = 1;

       if (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4)
	 fixP->fx_r_type = BFD_RELOC_ARM_PCREL_CALL;

    arm_branch_common:
      /* We are going to store value (shifted right by two) in the
	 instruction, in a 24 bit, signed field.  Bits 26 through 32 either
	 all clear or all set and bit 0 must be clear.  For B/BL bit 1 must
	 also be clear.  */
      if (value & temp)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("misaligned branch destination"));
      if ((value & (offsetT)0xfe000000) != (offsetT)0
	  && (value & (offsetT)0xfe000000) != (offsetT)0xfe000000)
	as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);

      if (fixP->fx_done || !seg->use_rela_p)
	{
	  newval = md_chars_to_number (buf, INSN_SIZE);
	  newval |= (value >> 2) & 0x00ffffff;
	  /* Set the H bit on BLX instructions.  */
	  if (temp == 1)
	    {
	      if (value & 2)
		newval |= 0x01000000;
	      else
		newval &= ~0x01000000;
	    }
	  md_number_to_chars (buf, newval, INSN_SIZE);
	}
      break;

    case BFD_RELOC_THUMB_PCREL_BRANCH7: /* CBZ */
      /* CBZ can only branch forward.  */

      /* Attempts to use CBZ to branch to the next instruction
	 (which, strictly speaking, are prohibited) will be turned into
	 no-ops.

	 FIXME: It may be better to remove the instruction completely and
	 perform relaxation.  */
      if (value == -2)
	{
	  newval = md_chars_to_number (buf, THUMB_SIZE);
	  newval = 0xbf00; /* NOP encoding T1 */
	  md_number_to_chars (buf, newval, THUMB_SIZE);
	}
      else
	{
	  if (value & ~0x7e)
	    as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);

	  if (fixP->fx_done || !seg->use_rela_p)
	    {
	      newval = md_chars_to_number (buf, THUMB_SIZE);
	      newval |= ((value & 0x3e) << 2) | ((value & 0x40) << 3);
	      md_number_to_chars (buf, newval, THUMB_SIZE);
	    }
	}
      break;

    case BFD_RELOC_THUMB_PCREL_BRANCH9: /* Conditional branch.	*/
      if ((value & ~0xff) && ((value & ~0xff) != ~0xff))
	as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);

      if (fixP->fx_done || !seg->use_rela_p)
	{
	  newval = md_chars_to_number (buf, THUMB_SIZE);
	  newval |= (value & 0x1ff) >> 1;
	  md_number_to_chars (buf, newval, THUMB_SIZE);
	}
      break;

    case BFD_RELOC_THUMB_PCREL_BRANCH12: /* Unconditional branch.  */
      if ((value & ~0x7ff) && ((value & ~0x7ff) != ~0x7ff))
	as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);

      if (fixP->fx_done || !seg->use_rela_p)
	{
	  newval = md_chars_to_number (buf, THUMB_SIZE);
	  newval |= (value & 0xfff) >> 1;
	  md_number_to_chars (buf, newval, THUMB_SIZE);
	}
      break;

    case BFD_RELOC_THUMB_PCREL_BRANCH20:
      if ((value & ~0x1fffff) && ((value & ~0x0fffff) != ~0x0fffff))
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("conditional branch out of range"));

      if (fixP->fx_done || !seg->use_rela_p)
	{
	  offsetT newval2;
	  addressT S, J1, J2, lo, hi;

	  S  = (value & 0x00100000) >> 20;
	  J2 = (value & 0x00080000) >> 19;
	  J1 = (value & 0x00040000) >> 18;
	  hi = (value & 0x0003f000) >> 12;
	  lo = (value & 0x00000ffe) >> 1;

	  newval   = md_chars_to_number (buf, THUMB_SIZE);
	  newval2  = md_chars_to_number (buf + THUMB_SIZE, THUMB_SIZE);
	  newval  |= (S << 10) | hi;
	  newval2 |= (J1 << 13) | (J2 << 11) | lo;
	  md_number_to_chars (buf, newval, THUMB_SIZE);
	  md_number_to_chars (buf + THUMB_SIZE, newval2, THUMB_SIZE);
	}
      break;

    case BFD_RELOC_THUMB_PCREL_BLX:
      /* If there is a blx from a thumb state function to
	 another thumb function flip this to a bl and warn
	 about it.  */

      if (fixP->fx_addsy
	  && !S_FORCE_RELOC (fixP->fx_addsy, TRUE)
	  && (S_GET_SEGMENT (fixP->fx_addsy) == seg)
	  && THUMB_IS_FUNC (fixP->fx_addsy))
	{
	  const char *name = S_GET_NAME (fixP->fx_addsy);
	  as_warn_where (fixP->fx_file, fixP->fx_line,
			 _("blx to Thumb func '%s' from Thumb ISA state changed to bl"),
			 name);
	  newval = md_chars_to_number (buf + THUMB_SIZE, THUMB_SIZE);
	  newval = newval | 0x1000;
	  md_number_to_chars (buf+THUMB_SIZE, newval, THUMB_SIZE);
	  fixP->fx_r_type = BFD_RELOC_THUMB_PCREL_BRANCH23;
	  fixP->fx_done = 1;
	}


      goto thumb_bl_common;

    case BFD_RELOC_THUMB_PCREL_BRANCH23:

    thumb_bl_common:

      if (fixP->fx_r_type == BFD_RELOC_THUMB_PCREL_BLX)
	/* For a BLX instruction, make sure that the relocation is rounded up
	   to a word boundary.  This follows the semantics of the instruction
	   which specifies that bit 1 of the target address will come from bit
	   1 of the base address.  */
	value = (value + 3) & ~ 3;

       if (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4
	   && fixP->fx_r_type == BFD_RELOC_THUMB_PCREL_BLX)
	 fixP->fx_r_type = BFD_RELOC_THUMB_PCREL_BRANCH23;

      if ((value & ~0x3fffff) && ((value & ~0x3fffff) != ~0x3fffff))
	{
	    as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);
	}

      if (fixP->fx_done || !seg->use_rela_p)
	encode_thumb2_b_bl_offset (buf, value);

      break;

    case BFD_RELOC_THUMB_PCREL_BRANCH25:
      if ((value & ~0x0ffffff) && ((value & ~0x0ffffff) != ~0x0ffffff))
	as_bad_where (fixP->fx_file, fixP->fx_line, BAD_RANGE);

      if (fixP->fx_done || !seg->use_rela_p)
	  encode_thumb2_b_bl_offset (buf, value);

      break;

    case BFD_RELOC_8:
      if (fixP->fx_done || !seg->use_rela_p)
	*buf = value;
      break;

    case BFD_RELOC_16:
      if (fixP->fx_done || !seg->use_rela_p)
	md_number_to_chars (buf, value, 2);
      break;

    case BFD_RELOC_ARM_TLS_CALL:
    case BFD_RELOC_ARM_THM_TLS_CALL:
    case BFD_RELOC_ARM_TLS_DESCSEQ:
    case BFD_RELOC_ARM_THM_TLS_DESCSEQ:
    case BFD_RELOC_ARM_TLS_GOTDESC:
    case BFD_RELOC_ARM_TLS_GD32:
    case BFD_RELOC_ARM_TLS_LE32:
    case BFD_RELOC_ARM_TLS_IE32:
    case BFD_RELOC_ARM_TLS_LDM32:
    case BFD_RELOC_ARM_TLS_LDO32:
      S_SET_THREAD_LOCAL (fixP->fx_addsy);
      break;

      /* Same handling as above, but with the arm_fdpic guard.  */
    case BFD_RELOC_ARM_TLS_GD32_FDPIC:
    case BFD_RELOC_ARM_TLS_IE32_FDPIC:
    case BFD_RELOC_ARM_TLS_LDM32_FDPIC:
      if (arm_fdpic)
	{
	  S_SET_THREAD_LOCAL (fixP->fx_addsy);
	}
      else
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("Relocation supported only in FDPIC mode"));
	}
      break;

    case BFD_RELOC_ARM_GOT32:
    case BFD_RELOC_ARM_GOTOFF:
      break;

    case BFD_RELOC_ARM_GOT_PREL:
      if (fixP->fx_done || !seg->use_rela_p)
	md_number_to_chars (buf, value, 4);
      break;

    case BFD_RELOC_ARM_TARGET2:
      /* TARGET2 is not partial-inplace, so we need to write the
	 addend here for REL targets, because it won't be written out
	 during reloc processing later.  */
      if (fixP->fx_done || !seg->use_rela_p)
	md_number_to_chars (buf, fixP->fx_offset, 4);
      break;

      /* Relocations for FDPIC.  */
    case BFD_RELOC_ARM_GOTFUNCDESC:
    case BFD_RELOC_ARM_GOTOFFFUNCDESC:
    case BFD_RELOC_ARM_FUNCDESC:
      if (arm_fdpic)
	{
	  if (fixP->fx_done || !seg->use_rela_p)
	    md_number_to_chars (buf, 0, 4);
	}
      else
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("Relocation supported only in FDPIC mode"));
      }
      break;

    case BFD_RELOC_RVA:
    case BFD_RELOC_32:
    case BFD_RELOC_ARM_TARGET1:
    case BFD_RELOC_ARM_ROSEGREL32:
    case BFD_RELOC_ARM_SBREL32:
    case BFD_RELOC_32_PCREL:
      if (fixP->fx_done || !seg->use_rela_p)
	  md_number_to_chars (buf, value, 4);
      break;

    case BFD_RELOC_ARM_PREL31:
      if (fixP->fx_done || !seg->use_rela_p)
	{
	  newval = md_chars_to_number (buf, 4) & 0x80000000;
	  if ((value ^ (value >> 1)) & 0x40000000)
	    {
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("rel31 relocation overflow"));
	    }
	  newval |= value & 0x7fffffff;
	  md_number_to_chars (buf, newval, 4);
	}
      break;

    case BFD_RELOC_ARM_CP_OFF_IMM:
    case BFD_RELOC_ARM_T32_CP_OFF_IMM:
      if (fixP->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM)
	newval = md_chars_to_number (buf, INSN_SIZE);
      else
	newval = get_thumb32_insn (buf);
      if ((newval & 0x0f200f00) == 0x0d000900)
	{
	  /* This is a fp16 vstr/vldr.  The immediate offset in the mnemonic
	     has permitted values that are multiples of 2, in the range 0
	     to 510.  */
	  if (value < -510 || value > 510 || (value & 1))
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("co-processor offset out of range"));
	}
      else if (value < -1023 || value > 1023 || (value & 3))
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("co-processor offset out of range"));
    cp_off_common:
      sign = value > 0;
      if (value < 0)
	value = -value;
      if (fixP->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM
	  || fixP->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM_S2)
	newval = md_chars_to_number (buf, INSN_SIZE);
      else
	newval = get_thumb32_insn (buf);
      if (value == 0)
	newval &= 0xffffff00;
      else
	{
	  newval &= 0xff7fff00;
	  if ((newval & 0x0f200f00) == 0x0d000900)
	    {
	      /* This is a fp16 vstr/vldr.

		 It requires the immediate offset in the instruction is shifted
		 left by 1 to be a half-word offset.

		 Here, left shift by 1 first, and later right shift by 2
		 should get the right offset.  */
	      value <<= 1;
	    }
	  newval |= (value >> 2) | (sign ? INDEX_UP : 0);
	}
      if (fixP->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM
	  || fixP->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM_S2)
	md_number_to_chars (buf, newval, INSN_SIZE);
      else
	put_thumb32_insn (buf, newval);
      break;

    case BFD_RELOC_ARM_CP_OFF_IMM_S2:
    case BFD_RELOC_ARM_T32_CP_OFF_IMM_S2:
      if (value < -255 || value > 255)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("co-processor offset out of range"));
      value *= 4;
      goto cp_off_common;

    case BFD_RELOC_ARM_THUMB_OFFSET:
      newval = md_chars_to_number (buf, THUMB_SIZE);
      /* Exactly what ranges, and where the offset is inserted depends
	 on the type of instruction, we can establish this from the
	 top 4 bits.  */
      switch (newval >> 12)
	{
	case 4: /* PC load.  */
	  /* Thumb PC loads are somewhat odd, bit 1 of the PC is
	     forced to zero for these loads; md_pcrel_from has already
	     compensated for this.  */
	  if (value & 3)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, target not word aligned (0x%08lX)"),
			  (((unsigned long) fixP->fx_frag->fr_address
			    + (unsigned long) fixP->fx_where) & ~3)
			  + (unsigned long) value);

	  if (value & ~0x3fc)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, value too big (0x%08lX)"),
			  (long) value);

	  newval |= value >> 2;
	  break;

	case 9: /* SP load/store.  */
	  if (value & ~0x3fc)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, value too big (0x%08lX)"),
			  (long) value);
	  newval |= value >> 2;
	  break;

	case 6: /* Word load/store.  */
	  if (value & ~0x7c)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, value too big (0x%08lX)"),
			  (long) value);
	  newval |= value << 4; /* 6 - 2.  */
	  break;

	case 7: /* Byte load/store.  */
	  if (value & ~0x1f)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, value too big (0x%08lX)"),
			  (long) value);
	  newval |= value << 6;
	  break;

	case 8: /* Halfword load/store.	 */
	  if (value & ~0x3e)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("invalid offset, value too big (0x%08lX)"),
			  (long) value);
	  newval |= value << 5; /* 6 - 1.  */
	  break;

	default:
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			"Unable to process relocation for thumb opcode: %lx",
			(unsigned long) newval);
	  break;
	}
      md_number_to_chars (buf, newval, THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_THUMB_ADD:
      /* This is a complicated relocation, since we use it for all of
	 the following immediate relocations:

	    3bit ADD/SUB
	    8bit ADD/SUB
	    9bit ADD/SUB SP word-aligned
	   10bit ADD PC/SP word-aligned

	 The type of instruction being processed is encoded in the
	 instruction field:

	   0x8000  SUB
	   0x00F0  Rd
	   0x000F  Rs
      */
      newval = md_chars_to_number (buf, THUMB_SIZE);
      {
	int rd = (newval >> 4) & 0xf;
	int rs = newval & 0xf;
	int subtract = !!(newval & 0x8000);

	/* Check for HI regs, only very restricted cases allowed:
	   Adjusting SP, and using PC or SP to get an address.	*/
	if ((rd > 7 && (rd != REG_SP || rs != REG_SP))
	    || (rs > 7 && rs != REG_SP && rs != REG_PC))
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("invalid Hi register with immediate"));

	/* If value is negative, choose the opposite instruction.  */
	if (value < 0)
	  {
	    value = -value;
	    subtract = !subtract;
	    if (value < 0)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("immediate value out of range"));
	  }

	if (rd == REG_SP)
	  {
 	    if (value & ~0x1fc)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("invalid immediate for stack address calculation"));
	    newval = subtract ? T_OPCODE_SUB_ST : T_OPCODE_ADD_ST;
	    newval |= value >> 2;
	  }
	else if (rs == REG_PC || rs == REG_SP)
	  {
	    /* PR gas/18541.  If the addition is for a defined symbol
	       within range of an ADR instruction then accept it.  */
	    if (subtract
		&& value == 4
		&& fixP->fx_addsy != NULL)
	      {
		subtract = 0;

		if (! S_IS_DEFINED (fixP->fx_addsy)
		    || S_GET_SEGMENT (fixP->fx_addsy) != seg
		    || S_IS_WEAK (fixP->fx_addsy))
		  {
		    as_bad_where (fixP->fx_file, fixP->fx_line,
				  _("address calculation needs a strongly defined nearby symbol"));
		  }
		else
		  {
		    offsetT v = fixP->fx_where + fixP->fx_frag->fr_address;

		    /* Round up to the next 4-byte boundary.  */
		    if (v & 3)
		      v = (v + 3) & ~ 3;
		    else
		      v += 4;
		    v = S_GET_VALUE (fixP->fx_addsy) - v;

		    if (v & ~0x3fc)
		      {
			as_bad_where (fixP->fx_file, fixP->fx_line,
				      _("symbol too far away"));
		      }
		    else
		      {
			fixP->fx_done = 1;
			value = v;
		      }
		  }
	      }

	    if (subtract || value & ~0x3fc)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("invalid immediate for address calculation (value = 0x%08lX)"),
			    (unsigned long) (subtract ? - value : value));
	    newval = (rs == REG_PC ? T_OPCODE_ADD_PC : T_OPCODE_ADD_SP);
	    newval |= rd << 8;
	    newval |= value >> 2;
	  }
	else if (rs == rd)
	  {
	    if (value & ~0xff)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("immediate value out of range"));
	    newval = subtract ? T_OPCODE_SUB_I8 : T_OPCODE_ADD_I8;
	    newval |= (rd << 8) | value;
	  }
	else
	  {
	    if (value & ~0x7)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			    _("immediate value out of range"));
	    newval = subtract ? T_OPCODE_SUB_I3 : T_OPCODE_ADD_I3;
	    newval |= rd | (rs << 3) | (value << 6);
	  }
      }
      md_number_to_chars (buf, newval, THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_THUMB_IMM:
      newval = md_chars_to_number (buf, THUMB_SIZE);
      if (value < 0 || value > 255)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid immediate: %ld is out of range"),
		      (long) value);
      newval |= value;
      md_number_to_chars (buf, newval, THUMB_SIZE);
      break;

    case BFD_RELOC_ARM_THUMB_SHIFT:
      /* 5bit shift value (0..32).  LSL cannot take 32.	 */
      newval = md_chars_to_number (buf, THUMB_SIZE) & 0xf83f;
      temp = newval & 0xf800;
      if (value < 0 || value > 32 || (value == 32 && temp == T_OPCODE_LSL_I))
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid shift value: %ld"), (long) value);
      /* Shifts of zero must be encoded as LSL.	 */
      if (value == 0)
	newval = (newval & 0x003f) | T_OPCODE_LSL_I;
      /* Shifts of 32 are encoded as zero.  */
      else if (value == 32)
	value = 0;
      newval |= value << 6;
      md_number_to_chars (buf, newval, THUMB_SIZE);
      break;

    case BFD_RELOC_VTABLE_INHERIT:
    case BFD_RELOC_VTABLE_ENTRY:
      fixP->fx_done = 0;
      return;

    case BFD_RELOC_ARM_MOVW:
    case BFD_RELOC_ARM_MOVT:
    case BFD_RELOC_ARM_THUMB_MOVW:
    case BFD_RELOC_ARM_THUMB_MOVT:
      if (fixP->fx_done || !seg->use_rela_p)
	{
	  /* REL format relocations are limited to a 16-bit addend.  */
	  if (!fixP->fx_done)
	    {
	      if (value < -0x8000 || value > 0x7fff)
		  as_bad_where (fixP->fx_file, fixP->fx_line,
				_("offset out of range"));
	    }
	  else if (fixP->fx_r_type == BFD_RELOC_ARM_MOVT
		   || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVT)
	    {
	      value >>= 16;
	    }

	  if (fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVW
	      || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVT)
	    {
	      newval = get_thumb32_insn (buf);
	      newval &= 0xfbf08f00;
	      newval |= (value & 0xf000) << 4;
	      newval |= (value & 0x0800) << 15;
	      newval |= (value & 0x0700) << 4;
	      newval |= (value & 0x00ff);
	      put_thumb32_insn (buf, newval);
	    }
	  else
	    {
	      newval = md_chars_to_number (buf, 4);
	      newval &= 0xfff0f000;
	      newval |= value & 0x0fff;
	      newval |= (value & 0xf000) << 4;
	      md_number_to_chars (buf, newval, 4);
	    }
	}
      return;

   case BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC:
   case BFD_RELOC_ARM_THUMB_ALU_ABS_G1_NC:
   case BFD_RELOC_ARM_THUMB_ALU_ABS_G2_NC:
   case BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC:
      gas_assert (!fixP->fx_done);
      {
	bfd_vma insn;
	bfd_boolean is_mov;
	bfd_vma encoded_addend = value;

	/* Check that addend can be encoded in instruction.  */
	if (!seg->use_rela_p && (value < 0 || value > 255))
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("the offset 0x%08lX is not representable"),
			(unsigned long) encoded_addend);

	/* Extract the instruction.  */
	insn = md_chars_to_number (buf, THUMB_SIZE);
	is_mov = (insn & 0xf800) == 0x2000;

	/* Encode insn.  */
	if (is_mov)
	  {
	    if (!seg->use_rela_p)
	      insn |= encoded_addend;
	  }
	else
	  {
	    int rd, rs;

	    /* Extract the instruction.  */
	     /* Encoding is the following
		0x8000  SUB
		0x00F0  Rd
		0x000F  Rs
	     */
	     /* The following conditions must be true :
		- ADD
		- Rd == Rs
		- Rd <= 7
	     */
	    rd = (insn >> 4) & 0xf;
	    rs = insn & 0xf;
	    if ((insn & 0x8000) || (rd != rs) || rd > 7)
	      as_bad_where (fixP->fx_file, fixP->fx_line,
			_("Unable to process relocation for thumb opcode: %lx"),
			(unsigned long) insn);

	    /* Encode as ADD immediate8 thumb 1 code.  */
	    insn = 0x3000 | (rd << 8);

	    /* Place the encoded addend into the first 8 bits of the
	       instruction.  */
	    if (!seg->use_rela_p)
	      insn |= encoded_addend;
	  }

	/* Update the instruction.  */
	md_number_to_chars (buf, insn, THUMB_SIZE);
      }
      break;

   case BFD_RELOC_ARM_ALU_PC_G0_NC:
   case BFD_RELOC_ARM_ALU_PC_G0:
   case BFD_RELOC_ARM_ALU_PC_G1_NC:
   case BFD_RELOC_ARM_ALU_PC_G1:
   case BFD_RELOC_ARM_ALU_PC_G2:
   case BFD_RELOC_ARM_ALU_SB_G0_NC:
   case BFD_RELOC_ARM_ALU_SB_G0:
   case BFD_RELOC_ARM_ALU_SB_G1_NC:
   case BFD_RELOC_ARM_ALU_SB_G1:
   case BFD_RELOC_ARM_ALU_SB_G2:
     gas_assert (!fixP->fx_done);
     if (!seg->use_rela_p)
       {
	 bfd_vma insn;
	 bfd_vma encoded_addend;
	 bfd_vma addend_abs = labs (value);

	 /* Check that the absolute value of the addend can be
	    expressed as an 8-bit constant plus a rotation.  */
	 encoded_addend = encode_arm_immediate (addend_abs);
	 if (encoded_addend == (unsigned int) FAIL)
	   as_bad_where (fixP->fx_file, fixP->fx_line,
			 _("the offset 0x%08lX is not representable"),
			 (unsigned long) addend_abs);

	 /* Extract the instruction.  */
	 insn = md_chars_to_number (buf, INSN_SIZE);

	 /* If the addend is positive, use an ADD instruction.
	    Otherwise use a SUB.  Take care not to destroy the S bit.  */
	 insn &= 0xff1fffff;
	 if (value < 0)
	   insn |= 1 << 22;
	 else
	   insn |= 1 << 23;

	 /* Place the encoded addend into the first 12 bits of the
	    instruction.  */
	 insn &= 0xfffff000;
	 insn |= encoded_addend;

	 /* Update the instruction.  */
	 md_number_to_chars (buf, insn, INSN_SIZE);
       }
     break;

    case BFD_RELOC_ARM_LDR_PC_G0:
    case BFD_RELOC_ARM_LDR_PC_G1:
    case BFD_RELOC_ARM_LDR_PC_G2:
    case BFD_RELOC_ARM_LDR_SB_G0:
    case BFD_RELOC_ARM_LDR_SB_G1:
    case BFD_RELOC_ARM_LDR_SB_G2:
      gas_assert (!fixP->fx_done);
      if (!seg->use_rela_p)
	{
	  bfd_vma insn;
	  bfd_vma addend_abs = labs (value);

	  /* Check that the absolute value of the addend can be
	     encoded in 12 bits.  */
	  if (addend_abs >= 0x1000)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad offset 0x%08lX (only 12 bits available for the magnitude)"),
			  (unsigned long) addend_abs);

	  /* Extract the instruction.  */
	  insn = md_chars_to_number (buf, INSN_SIZE);

	  /* If the addend is negative, clear bit 23 of the instruction.
	     Otherwise set it.  */
	  if (value < 0)
	    insn &= ~(1 << 23);
	  else
	    insn |= 1 << 23;

	  /* Place the absolute value of the addend into the first 12 bits
	     of the instruction.  */
	  insn &= 0xfffff000;
	  insn |= addend_abs;

	  /* Update the instruction.  */
	  md_number_to_chars (buf, insn, INSN_SIZE);
	}
      break;

    case BFD_RELOC_ARM_LDRS_PC_G0:
    case BFD_RELOC_ARM_LDRS_PC_G1:
    case BFD_RELOC_ARM_LDRS_PC_G2:
    case BFD_RELOC_ARM_LDRS_SB_G0:
    case BFD_RELOC_ARM_LDRS_SB_G1:
    case BFD_RELOC_ARM_LDRS_SB_G2:
      gas_assert (!fixP->fx_done);
      if (!seg->use_rela_p)
	{
	  bfd_vma insn;
	  bfd_vma addend_abs = labs (value);

	  /* Check that the absolute value of the addend can be
	     encoded in 8 bits.  */
	  if (addend_abs >= 0x100)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad offset 0x%08lX (only 8 bits available for the magnitude)"),
			  (unsigned long) addend_abs);

	  /* Extract the instruction.  */
	  insn = md_chars_to_number (buf, INSN_SIZE);

	  /* If the addend is negative, clear bit 23 of the instruction.
	     Otherwise set it.  */
	  if (value < 0)
	    insn &= ~(1 << 23);
	  else
	    insn |= 1 << 23;

	  /* Place the first four bits of the absolute value of the addend
	     into the first 4 bits of the instruction, and the remaining
	     four into bits 8 .. 11.  */
	  insn &= 0xfffff0f0;
	  insn |= (addend_abs & 0xf) | ((addend_abs & 0xf0) << 4);

	  /* Update the instruction.  */
	  md_number_to_chars (buf, insn, INSN_SIZE);
	}
      break;

    case BFD_RELOC_ARM_LDC_PC_G0:
    case BFD_RELOC_ARM_LDC_PC_G1:
    case BFD_RELOC_ARM_LDC_PC_G2:
    case BFD_RELOC_ARM_LDC_SB_G0:
    case BFD_RELOC_ARM_LDC_SB_G1:
    case BFD_RELOC_ARM_LDC_SB_G2:
      gas_assert (!fixP->fx_done);
      if (!seg->use_rela_p)
	{
	  bfd_vma insn;
	  bfd_vma addend_abs = labs (value);

	  /* Check that the absolute value of the addend is a multiple of
	     four and, when divided by four, fits in 8 bits.  */
	  if (addend_abs & 0x3)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad offset 0x%08lX (must be word-aligned)"),
			  (unsigned long) addend_abs);

	  if ((addend_abs >> 2) > 0xff)
	    as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("bad offset 0x%08lX (must be an 8-bit number of words)"),
			  (unsigned long) addend_abs);

	  /* Extract the instruction.  */
	  insn = md_chars_to_number (buf, INSN_SIZE);

	  /* If the addend is negative, clear bit 23 of the instruction.
	     Otherwise set it.  */
	  if (value < 0)
	    insn &= ~(1 << 23);
	  else
	    insn |= 1 << 23;

	  /* Place the addend (divided by four) into the first eight
	     bits of the instruction.  */
	  insn &= 0xfffffff0;
	  insn |= addend_abs >> 2;

	  /* Update the instruction.  */
	  md_number_to_chars (buf, insn, INSN_SIZE);
	}
      break;

    case BFD_RELOC_ARM_V4BX:
      /* This will need to go in the object file.  */
      fixP->fx_done = 0;
      break;

    case BFD_RELOC_UNUSED:
    default:
      as_bad_where (fixP->fx_file, fixP->fx_line,
		    _("bad relocation fixup type (%d)"), fixP->fx_r_type);
    }
}

/* Translate internal representation of relocation info to BFD target
   format.  */

arelent *
tc_gen_reloc (asection *section, fixS *fixp)
{
  arelent * reloc;
  bfd_reloc_code_real_type code;

  reloc = XNEW (arelent);

  reloc->sym_ptr_ptr = XNEW (asymbol *);
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;

  if (fixp->fx_pcrel)
    {
      if (section->use_rela_p)
	fixp->fx_offset -= md_pcrel_from_section (fixp, section);
      else
	fixp->fx_offset = reloc->address;
    }
  reloc->addend = fixp->fx_offset;

  switch (fixp->fx_r_type)
    {
    case BFD_RELOC_8:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_8_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_16:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_16_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_32:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_32_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_ARM_MOVW:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_ARM_MOVW_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_ARM_MOVT:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_ARM_MOVT_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_ARM_THUMB_MOVW:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_ARM_THUMB_MOVW_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_ARM_THUMB_MOVT:
      if (fixp->fx_pcrel)
	{
	  code = BFD_RELOC_ARM_THUMB_MOVT_PCREL;
	  break;
	}
      /* Fall through.  */

    case BFD_RELOC_NONE:
    case BFD_RELOC_ARM_PCREL_BRANCH:
    case BFD_RELOC_ARM_PCREL_BLX:
    case BFD_RELOC_RVA:
    case BFD_RELOC_THUMB_PCREL_BRANCH7:
    case BFD_RELOC_THUMB_PCREL_BRANCH9:
    case BFD_RELOC_THUMB_PCREL_BRANCH12:
    case BFD_RELOC_THUMB_PCREL_BRANCH20:
    case BFD_RELOC_THUMB_PCREL_BRANCH23:
    case BFD_RELOC_THUMB_PCREL_BRANCH25:
    case BFD_RELOC_VTABLE_ENTRY:
    case BFD_RELOC_VTABLE_INHERIT:
      code = fixp->fx_r_type;
      break;

    case BFD_RELOC_THUMB_PCREL_BLX:
      if (EF_ARM_EABI_VERSION (meabi_flags) >= EF_ARM_EABI_VER4)
	code = BFD_RELOC_THUMB_PCREL_BRANCH23;
      else
	code = BFD_RELOC_THUMB_PCREL_BLX;
      break;

    case BFD_RELOC_ARM_LITERAL:
    case BFD_RELOC_ARM_HWLITERAL:
      /* If this is called then the a literal has
	 been referenced across a section boundary.  */
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("literal referenced across section boundary"));
      return NULL;

    case BFD_RELOC_ARM_TLS_CALL:
    case BFD_RELOC_ARM_THM_TLS_CALL:
    case BFD_RELOC_ARM_TLS_DESCSEQ:
    case BFD_RELOC_ARM_THM_TLS_DESCSEQ:
    case BFD_RELOC_ARM_GOT32:
    case BFD_RELOC_ARM_GOTOFF:
    case BFD_RELOC_ARM_GOT_PREL:
    case BFD_RELOC_ARM_PLT32:
    case BFD_RELOC_ARM_TARGET1:
    case BFD_RELOC_ARM_ROSEGREL32:
    case BFD_RELOC_ARM_SBREL32:
    case BFD_RELOC_ARM_PREL31:
    case BFD_RELOC_ARM_TARGET2:
    case BFD_RELOC_ARM_TLS_LDO32:
    case BFD_RELOC_ARM_PCREL_CALL:
    case BFD_RELOC_ARM_PCREL_JUMP:
    case BFD_RELOC_ARM_ALU_PC_G0_NC:
    case BFD_RELOC_ARM_ALU_PC_G0:
    case BFD_RELOC_ARM_ALU_PC_G1_NC:
    case BFD_RELOC_ARM_ALU_PC_G1:
    case BFD_RELOC_ARM_ALU_PC_G2:
    case BFD_RELOC_ARM_LDR_PC_G0:
    case BFD_RELOC_ARM_LDR_PC_G1:
    case BFD_RELOC_ARM_LDR_PC_G2:
    case BFD_RELOC_ARM_LDRS_PC_G0:
    case BFD_RELOC_ARM_LDRS_PC_G1:
    case BFD_RELOC_ARM_LDRS_PC_G2:
    case BFD_RELOC_ARM_LDC_PC_G0:
    case BFD_RELOC_ARM_LDC_PC_G1:
    case BFD_RELOC_ARM_LDC_PC_G2:
    case BFD_RELOC_ARM_ALU_SB_G0_NC:
    case BFD_RELOC_ARM_ALU_SB_G0:
    case BFD_RELOC_ARM_ALU_SB_G1_NC:
    case BFD_RELOC_ARM_ALU_SB_G1:
    case BFD_RELOC_ARM_ALU_SB_G2:
    case BFD_RELOC_ARM_LDR_SB_G0:
    case BFD_RELOC_ARM_LDR_SB_G1:
    case BFD_RELOC_ARM_LDR_SB_G2:
    case BFD_RELOC_ARM_LDRS_SB_G0:
    case BFD_RELOC_ARM_LDRS_SB_G1:
    case BFD_RELOC_ARM_LDRS_SB_G2:
    case BFD_RELOC_ARM_LDC_SB_G0:
    case BFD_RELOC_ARM_LDC_SB_G1:
    case BFD_RELOC_ARM_LDC_SB_G2:
    case BFD_RELOC_ARM_V4BX:
    case BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC:
    case BFD_RELOC_ARM_THUMB_ALU_ABS_G1_NC:
    case BFD_RELOC_ARM_THUMB_ALU_ABS_G2_NC:
    case BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC:
    case BFD_RELOC_ARM_GOTFUNCDESC:
    case BFD_RELOC_ARM_GOTOFFFUNCDESC:
    case BFD_RELOC_ARM_FUNCDESC:
      code = fixp->fx_r_type;
      break;

    case BFD_RELOC_ARM_TLS_GOTDESC:
    case BFD_RELOC_ARM_TLS_GD32:
    case BFD_RELOC_ARM_TLS_GD32_FDPIC:
    case BFD_RELOC_ARM_TLS_LE32:
    case BFD_RELOC_ARM_TLS_IE32:
    case BFD_RELOC_ARM_TLS_IE32_FDPIC:
    case BFD_RELOC_ARM_TLS_LDM32:
    case BFD_RELOC_ARM_TLS_LDM32_FDPIC:
      /* BFD will include the symbol's address in the addend.
	 But we don't want that, so subtract it out again here.  */
      if (!S_IS_COMMON (fixp->fx_addsy))
	reloc->addend -= (*reloc->sym_ptr_ptr)->value;
      code = fixp->fx_r_type;
      break;

    case BFD_RELOC_ARM_IMMEDIATE:
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("internal relocation (type: IMMEDIATE) not fixed up"));
      return NULL;

    case BFD_RELOC_ARM_ADRL_IMMEDIATE:
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("ADRL used for a symbol not defined in the same file"));
      return NULL;

    case BFD_RELOC_ARM_OFFSET_IMM:
      if (section->use_rela_p)
	{
	  code = fixp->fx_r_type;
	  break;
	}

      if (fixp->fx_addsy != NULL
	  && !S_IS_DEFINED (fixp->fx_addsy)
	  && S_IS_LOCAL (fixp->fx_addsy))
	{
	  as_bad_where (fixp->fx_file, fixp->fx_line,
			_("undefined local label `%s'"),
			S_GET_NAME (fixp->fx_addsy));
	  return NULL;
	}

      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("internal_relocation (type: OFFSET_IMM) not fixed up"));
      return NULL;

    default:
      {
	const char * type;

	switch (fixp->fx_r_type)
	  {
	  case BFD_RELOC_NONE:		   type = "NONE";	  break;
	  case BFD_RELOC_ARM_OFFSET_IMM8:  type = "OFFSET_IMM8";  break;
	  case BFD_RELOC_ARM_SHIFT_IMM:	   type = "SHIFT_IMM";	  break;
	  case BFD_RELOC_ARM_SMC:	   type = "SMC";	  break;
	  case BFD_RELOC_ARM_SWI:	   type = "SWI";	  break;
	  case BFD_RELOC_ARM_MULTI:	   type = "MULTI";	  break;
	  case BFD_RELOC_ARM_CP_OFF_IMM:   type = "CP_OFF_IMM";	  break;
	  case BFD_RELOC_ARM_T32_OFFSET_IMM: type = "T32_OFFSET_IMM"; break;
	  case BFD_RELOC_ARM_T32_CP_OFF_IMM: type = "T32_CP_OFF_IMM"; break;
	  case BFD_RELOC_ARM_THUMB_ADD:	   type = "THUMB_ADD";	  break;
	  case BFD_RELOC_ARM_THUMB_SHIFT:  type = "THUMB_SHIFT";  break;
	  case BFD_RELOC_ARM_THUMB_IMM:	   type = "THUMB_IMM";	  break;
	  case BFD_RELOC_ARM_THUMB_OFFSET: type = "THUMB_OFFSET"; break;
	  default:			   type = _("<unknown>"); break;
	  }
	as_bad_where (fixp->fx_file, fixp->fx_line,
		      _("cannot represent %s relocation in this object file format"),
		      type);
	return NULL;
      }
    }

  if ((code == BFD_RELOC_32_PCREL || code == BFD_RELOC_32)
      && GOT_symbol
      && fixp->fx_addsy == GOT_symbol)
    {
      code = BFD_RELOC_ARM_GOTPC;
      reloc->addend = fixp->fx_offset = reloc->address;
    }

  reloc->howto = bfd_reloc_type_lookup (stdoutput, code);

  if (reloc->howto == NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("cannot represent %s relocation in this object file format"),
		    bfd_get_reloc_code_name (code));
      return NULL;
    }

  /* HACK: Since arm ELF uses Rel instead of Rela, encode the
     vtable entry to be used in the relocation's section offset.  */
  if (fixp->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    reloc->address = fixp->fx_offset;

  return reloc;
}

/* This fix_new is called by cons via TC_CONS_FIX_NEW.	*/

void
cons_fix_new_arm (fragS *	frag,
		  int		where,
		  int		size,
		  expressionS * exp,
		  bfd_reloc_code_real_type reloc)
{
  int pcrel = 0;

  /* Pick a reloc.
     FIXME: @@ Should look at CPU word size.  */
  switch (size)
    {
    case 1:
      reloc = BFD_RELOC_8;
      break;
    case 2:
      reloc = BFD_RELOC_16;
      break;
    case 4:
    default:
      reloc = BFD_RELOC_32;
      break;
    case 8:
      reloc = BFD_RELOC_64;
      break;
    }

  fix_new_exp (frag, where, size, exp, pcrel, reloc);
}


int
arm_force_relocation (struct fix * fixp)
{

  /* In case we have a call or a branch to a function in ARM ISA mode from
     a thumb function or vice-versa force the relocation. These relocations
     are cleared off for some cores that might have blx and simple transformations
     are possible.  */

  switch (fixp->fx_r_type)
    {
    case BFD_RELOC_ARM_PCREL_JUMP:
    case BFD_RELOC_ARM_PCREL_CALL:
    case BFD_RELOC_THUMB_PCREL_BLX:
      if (THUMB_IS_FUNC (fixp->fx_addsy))
	return 1;
      break;

    case BFD_RELOC_ARM_PCREL_BLX:
    case BFD_RELOC_THUMB_PCREL_BRANCH25:
    case BFD_RELOC_THUMB_PCREL_BRANCH20:
    case BFD_RELOC_THUMB_PCREL_BRANCH23:
      if (ARM_IS_FUNC (fixp->fx_addsy))
	return 1;
      break;

    default:
      break;
    }

  /* Resolve these relocations even if the symbol is extern or weak.
     Technically this is probably wrong due to symbol preemption.
     In practice these relocations do not have enough range to be useful
     at dynamic link time, and some code (e.g. in the Linux kernel)
     expects these references to be resolved.  */
  if (fixp->fx_r_type == BFD_RELOC_ARM_IMMEDIATE
      || fixp->fx_r_type == BFD_RELOC_ARM_OFFSET_IMM
      || fixp->fx_r_type == BFD_RELOC_ARM_OFFSET_IMM8
      || fixp->fx_r_type == BFD_RELOC_ARM_ADRL_IMMEDIATE
      || fixp->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM
      || fixp->fx_r_type == BFD_RELOC_ARM_CP_OFF_IMM_S2
      || fixp->fx_r_type == BFD_RELOC_ARM_THUMB_OFFSET
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_ADD_IMM
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_IMMEDIATE
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_IMM12
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_OFFSET_IMM
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_ADD_PC12
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_CP_OFF_IMM
      || fixp->fx_r_type == BFD_RELOC_ARM_T32_CP_OFF_IMM_S2)
    return 0;

  /* Always leave these relocations for the linker.  */
  if ((fixp->fx_r_type >= BFD_RELOC_ARM_ALU_PC_G0_NC
       && fixp->fx_r_type <= BFD_RELOC_ARM_LDC_SB_G2)
      || fixp->fx_r_type == BFD_RELOC_ARM_LDR_PC_G0)
    return 1;

  /* Always generate relocations against function symbols.  */
  if (fixp->fx_r_type == BFD_RELOC_32
      && fixp->fx_addsy
      && (symbol_get_bfdsym (fixp->fx_addsy)->flags & BSF_FUNCTION))
    return 1;

  return generic_force_reloc (fixp);
}

/* Relocations against function names must be left unadjusted,
   so that the linker can use this information to generate interworking
   stubs.  The MIPS version of this function
   also prevents relocations that are mips-16 specific, but I do not
   know why it does this.

   FIXME:
   There is one other problem that ought to be addressed here, but
   which currently is not:  Taking the address of a label (rather
   than a function) and then later jumping to that address.  Such
   addresses also ought to have their bottom bit set (assuming that
   they reside in Thumb code), but at the moment they will not.	 */

bfd_boolean
arm_fix_adjustable (fixS * fixP)
{
  if (fixP->fx_addsy == NULL)
    return 1;

  /* Preserve relocations against symbols with function type.  */
  if (symbol_get_bfdsym (fixP->fx_addsy)->flags & BSF_FUNCTION)
    return FALSE;

  if (THUMB_IS_FUNC (fixP->fx_addsy)
      && fixP->fx_subsy == NULL)
    return FALSE;

  /* We need the symbol name for the VTABLE entries.  */
  if (	 fixP->fx_r_type == BFD_RELOC_VTABLE_INHERIT
      || fixP->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    return FALSE;

  /* Don't allow symbols to be discarded on GOT related relocs.	 */
  if (fixP->fx_r_type == BFD_RELOC_ARM_PLT32
      || fixP->fx_r_type == BFD_RELOC_ARM_GOT32
      || fixP->fx_r_type == BFD_RELOC_ARM_GOTOFF
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_GD32
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_GD32_FDPIC
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_LE32
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_IE32
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_IE32_FDPIC
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_LDM32
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_LDM32_FDPIC
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_LDO32
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_GOTDESC
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_CALL
      || fixP->fx_r_type == BFD_RELOC_ARM_THM_TLS_CALL
      || fixP->fx_r_type == BFD_RELOC_ARM_TLS_DESCSEQ
      || fixP->fx_r_type == BFD_RELOC_ARM_THM_TLS_DESCSEQ
      || fixP->fx_r_type == BFD_RELOC_ARM_TARGET2)
    return FALSE;

  /* Similarly for group relocations.  */
  if ((fixP->fx_r_type >= BFD_RELOC_ARM_ALU_PC_G0_NC
       && fixP->fx_r_type <= BFD_RELOC_ARM_LDC_SB_G2)
      || fixP->fx_r_type == BFD_RELOC_ARM_LDR_PC_G0)
    return FALSE;

  /* MOVW/MOVT REL relocations have limited offsets, so keep the symbols.  */
  if (fixP->fx_r_type == BFD_RELOC_ARM_MOVW
      || fixP->fx_r_type == BFD_RELOC_ARM_MOVT
      || fixP->fx_r_type == BFD_RELOC_ARM_MOVW_PCREL
      || fixP->fx_r_type == BFD_RELOC_ARM_MOVT_PCREL
      || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVW
      || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVT
      || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVW_PCREL
      || fixP->fx_r_type == BFD_RELOC_ARM_THUMB_MOVT_PCREL)
    return FALSE;

  /* BFD_RELOC_ARM_THUMB_ALU_ABS_Gx_NC relocations have VERY limited
     offsets, so keep these symbols.  */
  if (fixP->fx_r_type >= BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC
      && fixP->fx_r_type <= BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC)
    return FALSE;

  return TRUE;
}

const char *
elf32_arm_target_format (void)
{
  if (arm_fdpic)
    {
      if (target_big_endian)
	return "elf32-bigarm-fdpic";
      else
	return "elf32-littlearm-fdpic";
    }
  else
    {
      if (target_big_endian)
	return "elf32-bigarm";
      else
	return "elf32-littlearm";
    }
}

void
armelf_frob_symbol (symbolS * symp,
		    int *     puntp)
{
  elf_frob_symbol (symp, puntp);
}

/* MD interface: Finalization.	*/

void
arm_cleanup (void)
{
  literal_pool * pool;

  /* Ensure that all the IT blocks are properly closed.  */
  check_it_blocks_finished ();

  for (pool = list_of_pools; pool; pool = pool->next)
    {
      /* Put it at the end of the relevant section.  */
      subseg_set (pool->section, pool->sub_section);
      arm_elf_change_section ();
      s_ltorg (0);
    }
}

/* Remove any excess mapping symbols generated for alignment frags in
   SEC.  We may have created a mapping symbol before a zero byte
   alignment; remove it if there's a mapping symbol after the
   alignment.  */
static void
check_mapping_symbols (bfd *abfd ATTRIBUTE_UNUSED, asection *sec,
		       void *dummy ATTRIBUTE_UNUSED)
{
  segment_info_type *seginfo = seg_info (sec);
  fragS *fragp;

  if (seginfo == NULL || seginfo->frchainP == NULL)
    return;

  for (fragp = seginfo->frchainP->frch_root;
       fragp != NULL;
       fragp = fragp->fr_next)
    {
      symbolS *sym = fragp->tc_frag_data.last_map;
      fragS *next = fragp->fr_next;

      /* Variable-sized frags have been converted to fixed size by
	 this point.  But if this was variable-sized to start with,
	 there will be a fixed-size frag after it.  So don't handle
	 next == NULL.  */
      if (sym == NULL || next == NULL)
	continue;

      if (S_GET_VALUE (sym) < next->fr_address)
	/* Not at the end of this frag.  */
	continue;
      know (S_GET_VALUE (sym) == next->fr_address);

      do
	{
	  if (next->tc_frag_data.first_map != NULL)
	    {
	      /* Next frag starts with a mapping symbol.  Discard this
		 one.  */
	      symbol_remove (sym, &symbol_rootP, &symbol_lastP);
	      break;
	    }

	  if (next->fr_next == NULL)
	    {
	      /* This mapping symbol is at the end of the section.  Discard
		 it.  */
	      know (next->fr_fix == 0 && next->fr_var == 0);
	      symbol_remove (sym, &symbol_rootP, &symbol_lastP);
	      break;
	    }

	  /* As long as we have empty frags without any mapping symbols,
	     keep looking.  */
	  /* If the next frag is non-empty and does not start with a
	     mapping symbol, then this mapping symbol is required.  */
	  if (next->fr_address != next->fr_next->fr_address)
	    break;

	  next = next->fr_next;
	}
      while (next != NULL);
    }
}

/* Adjust the symbol table.  This marks Thumb symbols as distinct from
   ARM ones.  */

void
arm_adjust_symtab (void)
{
  symbolS * sym;
  char	    bind;

  for (sym = symbol_rootP; sym != NULL; sym = symbol_next (sym))
    {
      if (ARM_IS_THUMB (sym))
	{
	  elf_symbol_type * elf_sym;

	  elf_sym = elf_symbol (symbol_get_bfdsym (sym));
	  bind = ELF_ST_BIND (elf_sym->internal_elf_sym.st_info);

	  if (! bfd_is_arm_special_symbol_name (elf_sym->symbol.name,
		BFD_ARM_SPECIAL_SYM_TYPE_ANY))
	    {
	      /* If it's a .thumb_func, declare it as so,
		 otherwise tag label as .code 16.  */
	      if (THUMB_IS_FUNC (sym))
		ARM_SET_SYM_BRANCH_TYPE (elf_sym->internal_elf_sym.st_target_internal,
					 ST_BRANCH_TO_THUMB);
	      else if (EF_ARM_EABI_VERSION (meabi_flags) < EF_ARM_EABI_VER4)
		elf_sym->internal_elf_sym.st_info =
		  ELF_ST_INFO (bind, STT_ARM_16BIT);
	    }
	}
    }

  /* Remove any overlapping mapping symbols generated by alignment frags.  */
  bfd_map_over_sections (stdoutput, check_mapping_symbols, (char *) 0);
  /* Now do generic ELF adjustments.  */
  elf_adjust_symtab ();
}

/* MD interface: Initialization.  */

static void
set_constant_flonums (void)
{
  int i;

  for (i = 0; i < NUM_FLOAT_VALS; i++)
    if (atof_ieee ((char *) fp_const[i], 'x', fp_values[i]) == NULL)
      abort ();
}

/* Auto-select Thumb mode if it's the only available instruction set for the
   given architecture.  */

static void
autoselect_thumb_from_cpu_variant (void)
{
  if (!ARM_CPU_HAS_FEATURE (cpu_variant, arm_ext_v1))
    opcode_select (16);
}

void
md_begin (void)
{
  unsigned mach;
  unsigned int i;

  if (	 (arm_ops_hsh = hash_new ()) == NULL
      || (arm_cond_hsh = hash_new ()) == NULL
      || (arm_shift_hsh = hash_new ()) == NULL
      || (arm_psr_hsh = hash_new ()) == NULL
      || (arm_reg_hsh = hash_new ()) == NULL
      || (arm_reloc_hsh = hash_new ()) == NULL
      || (arm_barrier_opt_hsh = hash_new ()) == NULL)
    as_fatal (_("virtual memory exhausted"));

  for (i = 0; i < sizeof (insns) / sizeof (struct asm_opcode); i++)
    hash_insert (arm_ops_hsh, insns[i].template_name, (void *) (insns + i));
  for (i = 0; i < sizeof (conds) / sizeof (struct asm_cond); i++)
    hash_insert (arm_cond_hsh, conds[i].template_name, (void *) (conds + i));
  for (i = 0; i < sizeof (shift_names) / sizeof (struct asm_shift_name); i++)
    hash_insert (arm_shift_hsh, shift_names[i].name, (void *) (shift_names + i));
  for (i = 0; i < sizeof (psrs) / sizeof (struct asm_psr); i++)
    hash_insert (arm_psr_hsh, psrs[i].template_name, (void *) (psrs + i));
  for (i = 0; i < sizeof (reg_names) / sizeof (struct reg_entry); i++)
    hash_insert (arm_reg_hsh, reg_names[i].name, (void *) (reg_names + i));
  for (i = 0; i < ARRAY_SIZE (reloc_names); i++)
    {
      struct reloc_entry * entry = reloc_names + i;

      if (arm_is_eabi() && entry->reloc == BFD_RELOC_ARM_PLT32)
	/* This makes encode_branch() use the EABI versions of this relocation.  */
	entry->reloc = BFD_RELOC_UNUSED;

      hash_insert (arm_reloc_hsh, entry->name, (void *) entry);
    }

  set_constant_flonums ();

  /* Set the cpu variant based on the command-line options.  We prefer
     -mcpu= over -march= if both are set (as for GCC); and we prefer
     -mfpu= over any other way of setting the floating point unit.
     Use of legacy options with new options are faulted.  */
  if (legacy_cpu)
    {
      if (mcpu_cpu_opt || march_cpu_opt)
	as_bad (_("use of old and new-style options to set CPU type"));

      selected_arch = *legacy_cpu;
    }
  else if (mcpu_cpu_opt)
    {
      selected_arch = *mcpu_cpu_opt;
      selected_ext = *mcpu_ext_opt;
    }
  else if (march_cpu_opt)
    {
      selected_arch = *march_cpu_opt;
      selected_ext = *march_ext_opt;
    }
  ARM_MERGE_FEATURE_SETS (selected_cpu, selected_arch, selected_ext);

  if (legacy_fpu)
    {
      if (mfpu_opt)
	as_bad (_("use of old and new-style options to set FPU type"));

      selected_fpu = *legacy_fpu;
    }
  else if (mfpu_opt)
    selected_fpu = *mfpu_opt;
  else
    {
      selected_fpu = fpu_default;
    }

  if (ARM_FEATURE_ZERO (selected_fpu))
    {
      if (!no_cpu_selected ())
	selected_fpu = fpu_default;
      else
	selected_fpu = fpu_arch_fpa;
    }

#ifdef CPU_DEFAULT
  if (ARM_FEATURE_ZERO (selected_arch))
    {
      selected_arch = cpu_default;
      selected_cpu = selected_arch;
    }
  ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);
#else
  /*  Autodection of feature mode: allow all features in cpu_variant but leave
      selected_cpu unset.  It will be set in aeabi_set_public_attributes ()
      after all instruction have been processed and we can decide what CPU
      should be selected.  */
  if (ARM_FEATURE_ZERO (selected_arch))
    ARM_MERGE_FEATURE_SETS (cpu_variant, arm_arch_any, selected_fpu);
  else
    ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);
#endif

  autoselect_thumb_from_cpu_variant ();

  arm_arch_used = thumb_arch_used = arm_arch_none;

  {
    unsigned int flags = 0;

    flags = meabi_flags;

    switch (meabi_flags)
      {
      case EF_ARM_EABI_UNKNOWN:
	/* Set the flags in the private structure.  */
	if (uses_apcs_26)      flags |= F_APCS26;
	if (support_interwork) flags |= F_INTERWORK;
	if (uses_apcs_float)   flags |= F_APCS_FLOAT;
	if (pic_code)	       flags |= F_PIC;

	switch (mfloat_abi_opt)
	  {
	  case ARM_FLOAT_ABI_SOFT:
	  case ARM_FLOAT_ABI_SOFTFP:
	    flags |= F_SOFT_FLOAT;
	    break;

	  case ARM_FLOAT_ABI_HARD:
	    if (flags & F_SOFT_FLOAT)
	      as_bad (_("hard-float conflicts with specified fpu"));
	    break;
	  }

	break;

      case EF_ARM_EABI_VER4:
      case EF_ARM_EABI_VER5:
	/* No additional flags to set.	*/
	break;

      default:
	abort ();
      }
    bfd_set_private_flags (stdoutput, flags);

    /* We have run out flags in the COFF header to encode the
       status of ATPCS support, so instead we create a dummy,
       empty, debug section called .arm.atpcs.	*/
    if (atpcs)
      {
	asection * sec;

	sec = bfd_make_section (stdoutput, ".arm.atpcs");

	if (sec != NULL)
	  {
	    bfd_set_section_flags
	      (stdoutput, sec, SEC_READONLY | SEC_DEBUGGING /* | SEC_HAS_CONTENTS */);
	    bfd_set_section_size (stdoutput, sec, 0);
	    bfd_set_section_contents (stdoutput, sec, NULL, 0, 0);
	  }
      }
  }

  /* Record the CPU type as well.  */
      if (ARM_CPU_HAS_FEATURE (cpu_variant, arm_ext_v4t))
	mach = bfd_mach_arm_4T;
      else
	mach = bfd_mach_arm_4;

  bfd_set_arch_mach (stdoutput, TARGET_ARCH, mach);
}

/* Command line processing.  */

/* md_parse_option
      Invocation line includes a switch not recognized by the base assembler.
      See if it's a processor-specific option.

      This routine is somewhat complicated by the need for backwards
      compatibility (since older releases of gcc can't be changed).
     The new options try to make the interface as compatible as
      possible with GCC.

      New options (supported) are:

	      -mcpu=<cpu name>		 Assemble for selected processor
	      -march=<architecture name> Assemble for selected architecture
	      -mfpu=<fpu architecture>	 Assemble for selected FPU.
	      -EB/-mbig-endian		 Big-endian
	      -EL/-mlittle-endian	 Little-endian
	      -k			 Generate PIC code
	      -mthumb			 Start in Thumb mode
	      -mthumb-interwork		 Code supports ARM/Thumb interworking

	      -m[no-]warn-deprecated     Warn about deprecated features
	      -m[no-]warn-syms		 Warn when symbols match instructions

      For now we will also provide support for:

	      -mapcs-32			 32-bit Program counter
	      -mapcs-26			 26-bit Program counter
	      -macps-float		 Floats passed in FP registers
	      -mapcs-reentrant		 Reentrant code
	      -matpcs
      (sometime these will probably be replaced with -mapcs=<list of options>
      and -matpcs=<list of options>)

      The remaining options are only supported for back-wards compatibility.
      Cpu variants, the arm part is optional:
	      -m[arm]1		      Currently not supported.
	      -m[arm]2, -m[arm]250    Arm 2 and Arm 250 processor
	      -m[arm]3		      Arm 3 processor
	      -m[arm]6[xx],	      Arm 6 processors
	      -m[arm]7[xx][t][[d]m]   Arm 7 processors
	      -m[arm]8[10]	      Arm 8 processors
	      -m[arm]9[20][tdmi]      Arm 9 processors
	      -mstrongarm[110[0]]     StrongARM processors
	      -mxscale		      XScale processors
	      -m[arm]v[2345[t[e]]]    Arm architectures
	      -mall		      All (except the ARM1)
      FP variants:
	      -mfpa10, -mfpa11	      FPA10 and 11 co-processor instructions
	      -mfpe-old		      (No float load/store multiples)
	      -mvfpxd		      VFP Single precision
	      -mvfp		      All VFP
	      -mno-fpu		      Disable all floating point instructions

      The following CPU names are recognized:
	      arm1, arm2, arm250, arm3, arm6, arm600, arm610, arm620,
	      arm7, arm7m, arm7d, arm7dm, arm7di, arm7dmi, arm70, arm700,
	      arm700i, arm710 arm710t, arm720, arm720t, arm740t, arm710c,
	      arm7100, arm7500, arm7500fe, arm7tdmi, arm8, arm810, arm9,
	      arm920, arm920t, arm940t, arm946, arm966, arm9tdmi, arm9e,
	      arm10t arm10e, arm1020t, arm1020e, arm10200e,
	      strongarm, strongarm110, strongarm1100, strongarm1110, xscale.

      */

const char * md_shortopts = "m:k";

#define OPTION_EL (OPTION_MD_BASE + 1)
#define OPTION_FIX_V4BX (OPTION_MD_BASE + 2)
#define OPTION_FDPIC (OPTION_MD_BASE + 3)

struct option md_longopts[] =
{
  {"EL", no_argument, NULL, OPTION_EL},
  {"fix-v4bx", no_argument, NULL, OPTION_FIX_V4BX},
  {"fdpic", no_argument, NULL, OPTION_FDPIC},
  {NULL, no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

struct arm_option_table
{
  const char *  option;		/* Option name to match.  */
  const char *  help;		/* Help information.  */
  int *         var;		/* Variable to change.	*/
  int	        value;		/* What to change it to.  */
  const char *  deprecated;	/* If non-null, print this message.  */
};

struct arm_option_table arm_opts[] =
{
  {"k",	     N_("generate PIC code"),	   &pic_code,	 1, NULL},
  {"mthumb", N_("assemble Thumb code"),	   &thumb_mode,	 1, NULL},
  {"mthumb-interwork", N_("support ARM/Thumb interworking"),
   &support_interwork, 1, NULL},
  {"mapcs-32", N_("code uses 32-bit program counter"), &uses_apcs_26, 0, NULL},
  {"mapcs-26", N_("code uses 26-bit program counter"), &uses_apcs_26, 1, NULL},
  {"mapcs-float", N_("floating point args are in fp regs"), &uses_apcs_float,
   1, NULL},
  {"mapcs-reentrant", N_("re-entrant code"), &pic_code, 1, NULL},
  {"matpcs", N_("code is ATPCS conformant"), &atpcs, 1, NULL},
  {"mlittle-endian", N_("assemble for little-endian"), &target_big_endian, 0,
   NULL},

  /* These are recognized by the assembler, but have no affect on code.	 */
  {"mapcs-frame", N_("use frame pointer"), NULL, 0, NULL},
  {"mapcs-stack-check", N_("use stack size checking"), NULL, 0, NULL},

  {"mwarn-deprecated", NULL, &warn_on_deprecated, 1, NULL},
  {"mno-warn-deprecated", N_("do not warn on use of deprecated feature"),
   &warn_on_deprecated, 0, NULL},
  {"mwarn-syms", N_("warn about symbols that match instruction names [default]"), (int *) (& flag_warn_syms), TRUE, NULL},
  {"mno-warn-syms", N_("disable warnings about symobls that match instructions"), (int *) (& flag_warn_syms), FALSE, NULL},
  {NULL, NULL, NULL, 0, NULL}
};

struct arm_legacy_option_table
{
  const char *              option;		/* Option name to match.  */
  const arm_feature_set	**  var;		/* Variable to change.	*/
  const arm_feature_set	    value;		/* What to change it to.  */
  const char *              deprecated;		/* If non-null, print this message.  */
};

const struct arm_legacy_option_table arm_legacy_opts[] =
{
  {NULL, NULL, ARM_ARCH_NONE, NULL}
};

struct arm_cpu_option_table
{
  const char *           name;
  size_t                 name_len;
  const arm_feature_set	 value;
  const arm_feature_set	 ext;
  /* For some CPUs we assume an FPU unless the user explicitly sets
     -mfpu=...	*/
  const arm_feature_set	 default_fpu;
  /* The canonical name of the CPU, or NULL to use NAME converted to upper
     case.  */
  const char *           canonical_name;
};

/* This list should, at a minimum, contain all the cpu names
   recognized by GCC.  */
#define ARM_CPU_OPT(N, CN, V, E, DF) { N, sizeof (N) - 1, V, E, DF, CN }

static const struct arm_cpu_option_table arm_cpus[] =
{
  ARM_CPU_OPT ("all",		  NULL,		       ARM_ANY,
	       ARM_ARCH_NONE,
	       FPU_ARCH_FPA),
  ARM_CPU_OPT ("arm7t",		  NULL,		       ARM_ARCH_V4T,
	       ARM_ARCH_NONE,
	       FPU_ARCH_FPA),
  ARM_CPU_OPT ("arm7tdmi",	  NULL,		       ARM_ARCH_V4T,
	       ARM_ARCH_NONE,
	       FPU_ARCH_FPA),
  ARM_CPU_OPT ("arm7tdmi-s",	  NULL,		       ARM_ARCH_V4T,
	       ARM_ARCH_NONE,
	       FPU_ARCH_FPA),

  { NULL, 0, ARM_ARCH_NONE, ARM_ARCH_NONE, ARM_ARCH_NONE, NULL }
};
#undef ARM_CPU_OPT

struct arm_arch_option_table
{
  const char *           name;
  size_t                 name_len;
  const arm_feature_set	 value;
  const arm_feature_set	 default_fpu;
};

/* This list should, at a minimum, contain all the architecture names
   recognized by GCC.  */
#define ARM_ARCH_OPT(N, V, DF) { N, sizeof (N) - 1, V, DF }

static const struct arm_arch_option_table arm_archs[] =
{
  ARM_ARCH_OPT ("all",		ARM_ANY,	 FPU_ARCH_FPA),
  ARM_ARCH_OPT ("armv4",	ARM_ARCH_V4,	 FPU_ARCH_FPA),
  ARM_ARCH_OPT ("armv4xm",	ARM_ARCH_V4xM,	 FPU_ARCH_FPA),
  ARM_ARCH_OPT ("armv4t",	ARM_ARCH_V4T,	 FPU_ARCH_FPA),
  ARM_ARCH_OPT ("armv4txm",	ARM_ARCH_V4TxM,	 FPU_ARCH_FPA),
  { NULL, 0, ARM_ARCH_NONE, ARM_ARCH_NONE }
};
#undef ARM_ARCH_OPT

/* ISA extensions in the co-processor and main instruction set space.  */

struct arm_option_extension_value_table
{
  const char *           name;
  size_t                 name_len;
  const arm_feature_set  merge_value;
  const arm_feature_set  clear_value;
  /* List of architectures for which an extension is available.  ARM_ARCH_NONE
     indicates that an extension is available for all architectures while
     ARM_ANY marks an empty entry.  */
  const arm_feature_set  allowed_archs[2];
};

/* The following table must be in alphabetical order with a NULL last entry.  */

static const struct arm_option_extension_value_table arm_extensions[] =
{
  /* Duplicate entry for the purpose of allowing ARMv7 to match in presence of
     Thumb divide instruction.  Due to this having the same name as the
     previous entry, this will be ignored when doing command-line parsing and
     only considered by build attribute selection code.  */
  { NULL, 0, ARM_ARCH_NONE, ARM_ARCH_NONE, { ARM_ARCH_NONE, ARM_ARCH_NONE } }
};

/* ISA floating-point and Advanced SIMD extensions.  */
struct arm_option_fpu_value_table
{
  const char *           name;
  const arm_feature_set  value;
};

/* This list should, at a minimum, contain all the fpu names
   recognized by GCC.  */
static const struct arm_option_fpu_value_table arm_fpus[] =
{
  {"softfpa",		FPU_NONE},
  {"fpe",		FPU_ARCH_FPE},
  {"fpe2",		FPU_ARCH_FPE},
  {"fpe3",		FPU_ARCH_FPA},	/* Third release supports LFM/SFM.  */
  {"fpa",		FPU_ARCH_FPA},
  {"fpa10",		FPU_ARCH_FPA},
  {"fpa11",		FPU_ARCH_FPA},
  {"arm7500fe",		FPU_ARCH_FPA},
  {"softvfp",		FPU_ARCH_VFP},
  {NULL,		ARM_ARCH_NONE}
};

struct arm_option_value_table
{
  const char *name;
  long value;
};

/* We only know how to output GNU and ver 4/5 (AAELF) formats.  */
static const struct arm_option_value_table arm_eabis[] =
{
  {"gnu",	EF_ARM_EABI_UNKNOWN},
  {"4",		EF_ARM_EABI_VER4},
  {"5",		EF_ARM_EABI_VER5},
  {NULL,	0}
};

struct arm_long_option_table
{
  const char * option;			/* Substring to match.	*/
  const char * help;			/* Help information.  */
  int (* func) (const char * subopt);	/* Function to decode sub-option.  */
  const char * deprecated;		/* If non-null, print this message.  */
};

static bfd_boolean
arm_parse_extension (const char *str, const arm_feature_set *opt_set,
		     arm_feature_set *ext_set)
{
  /* We insist on extensions being specified in alphabetical order, and with
     extensions being added before being removed.  We achieve this by having
     the global ARM_EXTENSIONS table in alphabetical order, and using the
     ADDING_VALUE variable to indicate whether we are adding an extension (1)
     or removing it (0) and only allowing it to change in the order
     -1 -> 1 -> 0.  */
  const struct arm_option_extension_value_table * opt = NULL;
  const arm_feature_set arm_any = ARM_ANY;
  int adding_value = -1;

  while (str != NULL && *str != 0)
    {
      const char *ext;
      size_t len;

      if (*str != '+')
	{
	  as_bad (_("invalid architectural extension"));
	  return FALSE;
	}

      str++;
      ext = strchr (str, '+');

      if (ext != NULL)
	len = ext - str;
      else
	len = strlen (str);

      if (len >= 2 && strncmp (str, "no", 2) == 0)
	{
	  if (adding_value != 0)
	    {
	      adding_value = 0;
	      opt = arm_extensions;
	    }

	  len -= 2;
	  str += 2;
	}
      else if (len > 0)
	{
	  if (adding_value == -1)
	    {
	      adding_value = 1;
	      opt = arm_extensions;
	    }
	  else if (adding_value != 1)
	    {
	      as_bad (_("must specify extensions to add before specifying "
			"those to remove"));
	      return FALSE;
	    }
	}

      if (len == 0)
	{
	  as_bad (_("missing architectural extension"));
	  return FALSE;
	}

      gas_assert (adding_value != -1);
      gas_assert (opt != NULL);

      /* Scan over the options table trying to find an exact match. */
      for (; opt->name != NULL; opt++)
	if (opt->name_len == len && strncmp (opt->name, str, len) == 0)
	  {
	    int i, nb_allowed_archs =
	      sizeof (opt->allowed_archs) / sizeof (opt->allowed_archs[0]);
	    /* Check we can apply the extension to this architecture.  */
	    for (i = 0; i < nb_allowed_archs; i++)
	      {
		/* Empty entry.  */
		if (ARM_FEATURE_EQUAL (opt->allowed_archs[i], arm_any))
		  continue;
		if (ARM_FSET_CPU_SUBSET (opt->allowed_archs[i], *opt_set))
		  break;
	      }
	    if (i == nb_allowed_archs)
	      {
		as_bad (_("extension does not apply to the base architecture"));
		return FALSE;
	      }

	    /* Add or remove the extension.  */
	    if (adding_value)
	      ARM_MERGE_FEATURE_SETS (*ext_set, *ext_set, opt->merge_value);
	    else
	      ARM_CLEAR_FEATURE (*ext_set, *ext_set, opt->clear_value);

	    /* Allowing Thumb division instructions for ARMv7 in autodetection
	       rely on this break so that duplicate extensions (extensions
	       with the same name as a previous extension in the list) are not
	       considered for command-line parsing.  */
	    break;
	  }

      if (opt->name == NULL)
	{
	  /* Did we fail to find an extension because it wasn't specified in
	     alphabetical order, or because it does not exist?  */

	  for (opt = arm_extensions; opt->name != NULL; opt++)
	    if (opt->name_len == len && strncmp (opt->name, str, len) == 0)
	      break;

	  if (opt->name == NULL)
	    as_bad (_("unknown architectural extension `%s'"), str);
	  else
	    as_bad (_("architectural extensions must be specified in "
		      "alphabetical order"));

	  return FALSE;
	}
      else
	{
	  /* We should skip the extension we've just matched the next time
	     round.  */
	  opt++;
	}

      str = ext;
    };

  return TRUE;
}

static bfd_boolean
arm_parse_cpu (const char *str)
{
  const struct arm_cpu_option_table *opt;
  const char *ext = strchr (str, '+');
  size_t len;

  if (ext != NULL)
    len = ext - str;
  else
    len = strlen (str);

  if (len == 0)
    {
      as_bad (_("missing cpu name `%s'"), str);
      return FALSE;
    }

  for (opt = arm_cpus; opt->name != NULL; opt++)
    if (opt->name_len == len && strncmp (opt->name, str, len) == 0)
      {
	mcpu_cpu_opt = &opt->value;
	if (mcpu_ext_opt == NULL)
	  mcpu_ext_opt = XNEW (arm_feature_set);
	*mcpu_ext_opt = opt->ext;
	mcpu_fpu_opt = &opt->default_fpu;
	if (opt->canonical_name)
	  {
	    gas_assert (sizeof selected_cpu_name > strlen (opt->canonical_name));
	    strcpy (selected_cpu_name, opt->canonical_name);
	  }
	else
	  {
	    size_t i;

	    if (len >= sizeof selected_cpu_name)
	      len = (sizeof selected_cpu_name) - 1;

	    for (i = 0; i < len; i++)
	      selected_cpu_name[i] = TOUPPER (opt->name[i]);
	    selected_cpu_name[i] = 0;
	  }

	if (ext != NULL)
	  return arm_parse_extension (ext, mcpu_cpu_opt, mcpu_ext_opt);

	return TRUE;
      }

  as_bad (_("unknown cpu `%s'"), str);
  return FALSE;
}

static bfd_boolean
arm_parse_arch (const char *str)
{
  const struct arm_arch_option_table *opt;
  const char *ext = strchr (str, '+');
  size_t len;

  if (ext != NULL)
    len = ext - str;
  else
    len = strlen (str);

  if (len == 0)
    {
      as_bad (_("missing architecture name `%s'"), str);
      return FALSE;
    }

  for (opt = arm_archs; opt->name != NULL; opt++)
    if (opt->name_len == len && strncmp (opt->name, str, len) == 0)
      {
	march_cpu_opt = &opt->value;
	if (march_ext_opt == NULL)
	  march_ext_opt = XNEW (arm_feature_set);
	*march_ext_opt = arm_arch_none;
	march_fpu_opt = &opt->default_fpu;
	strcpy (selected_cpu_name, opt->name);

	if (ext != NULL)
	  return arm_parse_extension (ext, march_cpu_opt, march_ext_opt);

	return TRUE;
      }

  as_bad (_("unknown architecture `%s'\n"), str);
  return FALSE;
}

static bfd_boolean
arm_parse_eabi (const char * str)
{
  const struct arm_option_value_table *opt;

  for (opt = arm_eabis; opt->name != NULL; opt++)
    if (streq (opt->name, str))
      {
	meabi_flags = opt->value;
	return TRUE;
      }
  as_bad (_("unknown EABI `%s'\n"), str);
  return FALSE;
}

static bfd_boolean
arm_parse_it_mode (const char * str)
{
  bfd_boolean ret = TRUE;

  if (streq ("arm", str))
    implicit_it_mode = IMPLICIT_IT_MODE_ARM;
  else if (streq ("thumb", str))
    implicit_it_mode = IMPLICIT_IT_MODE_THUMB;
  else if (streq ("always", str))
    implicit_it_mode = IMPLICIT_IT_MODE_ALWAYS;
  else if (streq ("never", str))
    implicit_it_mode = IMPLICIT_IT_MODE_NEVER;
  else
    {
      as_bad (_("unknown implicit IT mode `%s', should be "\
		"arm, thumb, always, or never."), str);
      ret = FALSE;
    }

  return ret;
}

static bfd_boolean
arm_ccs_mode (const char * unused ATTRIBUTE_UNUSED)
{
  codecomposer_syntax = TRUE;
  arm_comment_chars[0] = ';';
  arm_line_separator_chars[0] = 0;
  return TRUE;
}

struct arm_long_option_table arm_long_opts[] =
{
  {"mcpu=", N_("<cpu name>\t  assemble for CPU <cpu name>"),
   arm_parse_cpu, NULL},
  {"march=", N_("<arch name>\t  assemble for architecture <arch name>"),
   arm_parse_arch, NULL},
  {"meabi=", N_("<ver>\t\t  assemble for eabi version <ver>"),
   arm_parse_eabi, NULL},
  {"mimplicit-it=", N_("<mode>\t  controls implicit insertion of IT instructions"),
   arm_parse_it_mode, NULL},
  {"mccs", N_("\t\t\t  TI CodeComposer Studio syntax compatibility mode"),
   arm_ccs_mode, NULL},
  {NULL, NULL, 0, NULL}
};

int
md_parse_option (int c, const char * arg)
{
  struct arm_option_table *opt;
  const struct arm_legacy_option_table *fopt;
  struct arm_long_option_table *lopt;

  switch (c)
    {
    case OPTION_EL:
      target_big_endian = 0;
      break;

    case OPTION_FIX_V4BX:
      fix_v4bx = TRUE;
      break;

    case OPTION_FDPIC:
      arm_fdpic = TRUE;
      break;

    case 'a':
      /* Listing option.  Just ignore these, we don't support additional
	 ones.	*/
      return 0;

    default:
      for (opt = arm_opts; opt->option != NULL; opt++)
	{
	  if (c == opt->option[0]
	      && ((arg == NULL && opt->option[1] == 0)
		  || streq (arg, opt->option + 1)))
	    {
	      /* If the option is deprecated, tell the user.  */
	      if (warn_on_deprecated && opt->deprecated != NULL)
		as_tsktsk (_("option `-%c%s' is deprecated: %s"), c,
			   arg ? arg : "", _(opt->deprecated));

	      if (opt->var != NULL)
		*opt->var = opt->value;

	      return 1;
	    }
	}

      for (fopt = arm_legacy_opts; fopt->option != NULL; fopt++)
	{
	  if (c == fopt->option[0]
	      && ((arg == NULL && fopt->option[1] == 0)
		  || streq (arg, fopt->option + 1)))
	    {
	      /* If the option is deprecated, tell the user.  */
	      if (warn_on_deprecated && fopt->deprecated != NULL)
		as_tsktsk (_("option `-%c%s' is deprecated: %s"), c,
			   arg ? arg : "", _(fopt->deprecated));

	      if (fopt->var != NULL)
		*fopt->var = &fopt->value;

	      return 1;
	    }
	}

      for (lopt = arm_long_opts; lopt->option != NULL; lopt++)
	{
	  /* These options are expected to have an argument.  */
	  if (c == lopt->option[0]
	      && arg != NULL
	      && strncmp (arg, lopt->option + 1,
			  strlen (lopt->option + 1)) == 0)
	    {
	      /* If the option is deprecated, tell the user.  */
	      if (warn_on_deprecated && lopt->deprecated != NULL)
		as_tsktsk (_("option `-%c%s' is deprecated: %s"), c, arg,
			   _(lopt->deprecated));

	      /* Call the sup-option parser.  */
	      return lopt->func (arg + strlen (lopt->option) - 1);
	    }
	}

      return 0;
    }

  return 1;
}

void
md_show_usage (FILE * fp)
{
  struct arm_option_table *opt;
  struct arm_long_option_table *lopt;

  fprintf (fp, _(" ARM-specific assembler options:\n"));

  for (opt = arm_opts; opt->option != NULL; opt++)
    if (opt->help != NULL)
      fprintf (fp, "  -%-23s%s\n", opt->option, _(opt->help));

  for (lopt = arm_long_opts; lopt->option != NULL; lopt++)
    if (lopt->help != NULL)
      fprintf (fp, "  -%s%s\n", lopt->option, _(lopt->help));

  fprintf (fp, _("\
  -EL                     assemble code for a little-endian cpu\n"));

  fprintf (fp, _("\
  --fix-v4bx              Allow BX in ARMv4 code\n"));

  fprintf (fp, _("\
  --fdpic                 generate an FDPIC object file\n"));
}

typedef struct
{
  int val;
  arm_feature_set flags;
} cpu_arch_ver_table;

/* Mapping from CPU features to EABI CPU arch values.  Table must be sorted
   chronologically for architectures, with an exception for ARMv6-M and
   ARMv6S-M due to legacy reasons.  No new architecture should have a
   special case.  This allows for build attribute selection results to be
   stable when new architectures are added.  */
static const cpu_arch_ver_table cpu_arch_ver[] =
{
    {0, ARM_ARCH_V1},
    {0, ARM_ARCH_V2},
    {0, ARM_ARCH_V2S},
    {0, ARM_ARCH_V3},
    {0, ARM_ARCH_V3M},
    {1, ARM_ARCH_V4xM},
    {1, ARM_ARCH_V4},
    {2, ARM_ARCH_V4TxM},
    {2, ARM_ARCH_V4T},
    {-1, ARM_ARCH_NONE}
};

/* Set an attribute if it has not already been set by the user.  */

static void
aeabi_set_attribute_int (int tag, int value)
{
  if (tag < 1
      || tag >= NUM_KNOWN_OBJ_ATTRIBUTES
      || !attributes_set_explicitly[tag])
    bfd_elf_add_proc_attr_int (stdoutput, tag, value);
}

static void
aeabi_set_attribute_string (int tag, const char *value)
{
  if (tag < 1
      || tag >= NUM_KNOWN_OBJ_ATTRIBUTES
      || !attributes_set_explicitly[tag])
    bfd_elf_add_proc_attr_string (stdoutput, tag, value);
}

/* Return whether features in the *NEEDED feature set are available via
   extensions for the architecture whose feature set is *ARCH_FSET.  */

static bfd_boolean
have_ext_for_needed_feat_p (const arm_feature_set *arch_fset,
			    const arm_feature_set *needed)
{
  int i, nb_allowed_archs;
  arm_feature_set ext_fset;
  const struct arm_option_extension_value_table *opt;

  ext_fset = arm_arch_none;
  for (opt = arm_extensions; opt->name != NULL; opt++)
    {
      /* Extension does not provide any feature we need.  */
      if (!ARM_CPU_HAS_FEATURE (*needed, opt->merge_value))
	continue;

      nb_allowed_archs =
	sizeof (opt->allowed_archs) / sizeof (opt->allowed_archs[0]);
      for (i = 0; i < nb_allowed_archs; i++)
	{
	  /* Empty entry.  */
	  if (ARM_FEATURE_EQUAL (opt->allowed_archs[i], arm_arch_any))
	    break;

	  /* Extension is available, add it.  */
	  if (ARM_FSET_CPU_SUBSET (opt->allowed_archs[i], *arch_fset))
	    ARM_MERGE_FEATURE_SETS (ext_fset, ext_fset, opt->merge_value);
	}
    }

  /* Can we enable all features in *needed?  */
  return ARM_FSET_CPU_SUBSET (*needed, ext_fset);
}

/* Select value for Tag_CPU_arch and Tag_CPU_arch_profile build attributes for
   a given architecture feature set *ARCH_EXT_FSET including extension feature
   set *EXT_FSET.  Selection logic used depend on EXACT_MATCH:
   - if true, check for an exact match of the architecture modulo extensions;
   - otherwise, select build attribute value of the first superset
     architecture released so that results remains stable when new architectures
     are added.
   For -march/-mcpu=all the build attribute value of the most featureful
   architecture is returned.  Tag_CPU_arch_profile result is returned in
   PROFILE.  */

static int
get_aeabi_cpu_arch_from_fset (const arm_feature_set *arch_ext_fset,
			      const arm_feature_set *ext_fset,
			      char *profile, int exact_match)
{
  arm_feature_set arch_fset;
  const cpu_arch_ver_table *p_ver, *p_ver_ret = NULL;

  ARM_CLEAR_FEATURE (arch_fset, *arch_ext_fset, *ext_fset);

  for (p_ver = cpu_arch_ver; p_ver->val != -1; p_ver++)
    {
      arm_feature_set known_arch_fset;

      ARM_CLEAR_FEATURE (known_arch_fset, p_ver->flags, fpu_any);
      if (exact_match)
	{
	  /* Base architecture match user-specified architecture and
	     extensions, eg. ARMv6S-M matching -march=armv6-m+os.  */
	  if (ARM_FEATURE_EQUAL (*arch_ext_fset, known_arch_fset))
	    {
	      p_ver_ret = p_ver;
	      goto found;
	    }
	  /* Base architecture match user-specified architecture only
	     (eg. ARMv6-M in the same case as above).  Record it in case we
	     find a match with above condition.  */
	  else if (p_ver_ret == NULL
		   && ARM_FEATURE_EQUAL (arch_fset, known_arch_fset))
	    p_ver_ret = p_ver;
	}
      else
	{

	  /* Architecture has all features wanted.  */
	  if (ARM_FSET_CPU_SUBSET (arch_fset, known_arch_fset))
	    {
	      arm_feature_set added_fset;

	      /* Compute features added by this architecture over the one
		 recorded in p_ver_ret.  */
	      if (p_ver_ret != NULL)
		ARM_CLEAR_FEATURE (added_fset, known_arch_fset,
				   p_ver_ret->flags);
	      /* First architecture that match incl. with extensions, or the
		 only difference in features over the recorded match is
		 features that were optional and are now mandatory.  */
	      if (p_ver_ret == NULL
		  || ARM_FSET_CPU_SUBSET (added_fset, arch_fset))
		{
		  p_ver_ret = p_ver;
		  goto found;
		}
	    }
	  else if (p_ver_ret == NULL)
	    {
	      arm_feature_set needed_ext_fset;

	      ARM_CLEAR_FEATURE (needed_ext_fset, arch_fset, known_arch_fset);

	      /* Architecture has all features needed when using some
		 extensions.  Record it and continue searching in case there
		 exist an architecture providing all needed features without
		 the need for extensions (eg. ARMv6S-M Vs ARMv6-M with
		 OS extension).  */
	      if (have_ext_for_needed_feat_p (&known_arch_fset,
					      &needed_ext_fset))
		p_ver_ret = p_ver;
	    }
	}
    }

  if (p_ver_ret == NULL)
    return -1;

found:
  /* Tag_CPU_arch_profile.  */
  *profile = '\0';
  return p_ver_ret->val;
}

/* Set the public EABI object attributes.  */

static void
aeabi_set_public_attributes (void)
{
  char profile = '\0';
  int arch = -1;
  int virt_sec = 0;
  int skip_exact_match = 0;
  arm_feature_set flags, flags_arch, flags_ext;

  /* Autodetection mode, choose the architecture based the instructions
     actually used.  */
  if (no_cpu_selected ())
    {
      ARM_MERGE_FEATURE_SETS (flags, arm_arch_used, thumb_arch_used);

      if (ARM_CPU_HAS_FEATURE (arm_arch_used, arm_arch_any))
	ARM_MERGE_FEATURE_SETS (flags, flags, arm_ext_v1);

      if (ARM_CPU_HAS_FEATURE (thumb_arch_used, arm_arch_any))
	ARM_MERGE_FEATURE_SETS (flags, flags, arm_ext_v4t);

      /* Code run during relaxation relies on selected_cpu being set.  */
      ARM_CLEAR_FEATURE (flags_arch, flags, fpu_any);
      flags_ext = arm_arch_none;
      ARM_CLEAR_FEATURE (selected_arch, flags_arch, flags_ext);
      selected_ext = flags_ext;
      selected_cpu = flags;
    }
  /* Otherwise, choose the architecture based on the capabilities of the
     requested cpu.  */
  else
    {
      ARM_MERGE_FEATURE_SETS (flags_arch, selected_arch, selected_ext);
      ARM_CLEAR_FEATURE (flags_arch, flags_arch, fpu_any);
      flags_ext = selected_ext;
      flags = selected_cpu;
    }
  ARM_MERGE_FEATURE_SETS (flags, flags, selected_fpu);

  /* Allow the user to override the reported architecture.  */
  if (!ARM_FEATURE_ZERO (selected_object_arch))
    {
      ARM_CLEAR_FEATURE (flags_arch, selected_object_arch, fpu_any);
      flags_ext = arm_arch_none;
    }
  else
    skip_exact_match = ARM_FEATURE_EQUAL (selected_cpu, arm_arch_any);

  /* When this function is run again after relaxation has happened there is no
     way to determine whether an architecture or CPU was specified by the user:
     - selected_cpu is set above for relaxation to work;
     - march_cpu_opt is not set if only -mcpu or .cpu is used;
     - mcpu_cpu_opt is set to arm_arch_any for autodetection.
     Therefore, if not in -march=all case we first try an exact match and fall
     back to autodetection.  */
  if (!skip_exact_match)
    arch = get_aeabi_cpu_arch_from_fset (&flags_arch, &flags_ext, &profile, 1);
  if (arch == -1)
    arch = get_aeabi_cpu_arch_from_fset (&flags_arch, &flags_ext, &profile, 0);
  if (arch == -1)
    as_bad (_("no architecture contains all the instructions used\n"));

  /* Tag_CPU_name.  */
  if (selected_cpu_name[0])
    {
      char *q;

      q = selected_cpu_name;
      if (strncmp (q, "armv", 4) == 0)
	{
	  int i;

	  q += 4;
	  for (i = 0; q[i]; i++)
	    q[i] = TOUPPER (q[i]);
	}
      aeabi_set_attribute_string (Tag_CPU_name, q);
    }

  /* Tag_CPU_arch.  */
  aeabi_set_attribute_int (Tag_CPU_arch, arch);

  /* Tag_CPU_arch_profile.  */
  if (profile != '\0')
    aeabi_set_attribute_int (Tag_CPU_arch_profile, profile);

  ARM_CLEAR_FEATURE (flags_arch, flags, fpu_any);
  /* Tag_ARM_ISA_use.  */
  if (ARM_CPU_HAS_FEATURE (flags, arm_ext_v1)
      || ARM_FEATURE_ZERO (flags_arch))
    aeabi_set_attribute_int (Tag_ARM_ISA_use, 1);

  /* Tag_THUMB_ISA_use.  */
  if (ARM_CPU_HAS_FEATURE (flags, arm_ext_v4t)
      || ARM_FEATURE_ZERO (flags_arch))
    {
      int thumb_isa_use;

	thumb_isa_use = 1;
      aeabi_set_attribute_int (Tag_THUMB_ISA_use, thumb_isa_use);
    }

  gas_assert (arch <= MAX_TAG_CPU_ARCH);

  if (virt_sec != 0)
    aeabi_set_attribute_int (Tag_Virtualization_use, virt_sec);
}

/* Post relaxation hook.  Recompute ARM attributes now that relaxation is
   finished and free extension feature bits which will not be used anymore.  */

void
arm_md_post_relax (void)
{
  aeabi_set_public_attributes ();
  XDELETE (mcpu_ext_opt);
  mcpu_ext_opt = NULL;
  XDELETE (march_ext_opt);
  march_ext_opt = NULL;
}

/* Add the default contents for the .ARM.attributes section.  */

void
arm_md_end (void)
{
  if (EF_ARM_EABI_VERSION (meabi_flags) < EF_ARM_EABI_VER4)
    return;

  aeabi_set_public_attributes ();
}

/* Parse a .cpu directive.  */

static void
s_arm_cpu (int ignored ATTRIBUTE_UNUSED)
{
  const struct arm_cpu_option_table *opt;
  char *name;
  char saved_char;

  name = input_line_pointer;
  while (*input_line_pointer && !ISSPACE (*input_line_pointer))
    input_line_pointer++;
  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  /* Skip the first "all" entry.  */
  for (opt = arm_cpus + 1; opt->name != NULL; opt++)
    if (streq (opt->name, name))
      {
	selected_arch = opt->value;
	selected_ext = opt->ext;
	ARM_MERGE_FEATURE_SETS (selected_cpu, selected_arch, selected_ext);
	if (opt->canonical_name)
	  strcpy (selected_cpu_name, opt->canonical_name);
	else
	  {
	    int i;
	    for (i = 0; opt->name[i]; i++)
	      selected_cpu_name[i] = TOUPPER (opt->name[i]);

	    selected_cpu_name[i] = 0;
	  }
	ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);

	*input_line_pointer = saved_char;
	demand_empty_rest_of_line ();
	return;
      }
  as_bad (_("unknown cpu `%s'"), name);
  *input_line_pointer = saved_char;
  ignore_rest_of_line ();
}

/* Parse a .arch directive.  */

static void
s_arm_arch (int ignored ATTRIBUTE_UNUSED)
{
  const struct arm_arch_option_table *opt;
  char saved_char;
  char *name;

  name = input_line_pointer;
  while (*input_line_pointer && !ISSPACE (*input_line_pointer))
    input_line_pointer++;
  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  /* Skip the first "all" entry.  */
  for (opt = arm_archs + 1; opt->name != NULL; opt++)
    if (streq (opt->name, name))
      {
	selected_arch = opt->value;
	selected_ext = arm_arch_none;
	selected_cpu = selected_arch;
	strcpy (selected_cpu_name, opt->name);
	ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);
	*input_line_pointer = saved_char;
	demand_empty_rest_of_line ();
	return;
      }

  as_bad (_("unknown architecture `%s'\n"), name);
  *input_line_pointer = saved_char;
  ignore_rest_of_line ();
}

/* Parse a .object_arch directive.  */

static void
s_arm_object_arch (int ignored ATTRIBUTE_UNUSED)
{
  const struct arm_arch_option_table *opt;
  char saved_char;
  char *name;

  name = input_line_pointer;
  while (*input_line_pointer && !ISSPACE (*input_line_pointer))
    input_line_pointer++;
  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  /* Skip the first "all" entry.  */
  for (opt = arm_archs + 1; opt->name != NULL; opt++)
    if (streq (opt->name, name))
      {
	selected_object_arch = opt->value;
	*input_line_pointer = saved_char;
	demand_empty_rest_of_line ();
	return;
      }

  as_bad (_("unknown architecture `%s'\n"), name);
  *input_line_pointer = saved_char;
  ignore_rest_of_line ();
}

/* Parse a .arch_extension directive.  */

static void
s_arm_arch_extension (int ignored ATTRIBUTE_UNUSED)
{
  const struct arm_option_extension_value_table *opt;
  char saved_char;
  char *name;
  int adding_value = 1;

  name = input_line_pointer;
  while (*input_line_pointer && !ISSPACE (*input_line_pointer))
    input_line_pointer++;
  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  if (strlen (name) >= 2
      && strncmp (name, "no", 2) == 0)
    {
      adding_value = 0;
      name += 2;
    }

  for (opt = arm_extensions; opt->name != NULL; opt++)
    if (streq (opt->name, name))
      {
	int i, nb_allowed_archs =
	  sizeof (opt->allowed_archs) / sizeof (opt->allowed_archs[i]);
	for (i = 0; i < nb_allowed_archs; i++)
	  {
	    /* Empty entry.  */
	    if (ARM_CPU_IS_ANY (opt->allowed_archs[i]))
	      continue;
	    if (ARM_FSET_CPU_SUBSET (opt->allowed_archs[i], selected_arch))
	      break;
	  }

	if (i == nb_allowed_archs)
	  {
	    as_bad (_("architectural extension `%s' is not allowed for the "
		      "current base architecture"), name);
	    break;
	  }

	if (adding_value)
	  ARM_MERGE_FEATURE_SETS (selected_ext, selected_ext,
				  opt->merge_value);
	else
	  ARM_CLEAR_FEATURE (selected_ext, selected_ext, opt->clear_value);

	ARM_MERGE_FEATURE_SETS (selected_cpu, selected_arch, selected_ext);
	ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);
	*input_line_pointer = saved_char;
	demand_empty_rest_of_line ();
	/* Allowing Thumb division instructions for ARMv7 in autodetection rely
	   on this return so that duplicate extensions (extensions with the
	   same name as a previous extension in the list) are not considered
	   for command-line parsing.  */
	return;
      }

  if (opt->name == NULL)
    as_bad (_("unknown architecture extension `%s'\n"), name);

  *input_line_pointer = saved_char;
  ignore_rest_of_line ();
}

/* Parse a .fpu directive.  */

static void
s_arm_fpu (int ignored ATTRIBUTE_UNUSED)
{
  const struct arm_option_fpu_value_table *opt;
  char saved_char;
  char *name;

  name = input_line_pointer;
  while (*input_line_pointer && !ISSPACE (*input_line_pointer))
    input_line_pointer++;
  saved_char = *input_line_pointer;
  *input_line_pointer = 0;

  for (opt = arm_fpus; opt->name != NULL; opt++)
    if (streq (opt->name, name))
      {
	selected_fpu = opt->value;
#ifndef CPU_DEFAULT
	if (no_cpu_selected ())
	  ARM_MERGE_FEATURE_SETS (cpu_variant, arm_arch_any, selected_fpu);
	else
#endif
	  ARM_MERGE_FEATURE_SETS (cpu_variant, selected_cpu, selected_fpu);
	*input_line_pointer = saved_char;
	demand_empty_rest_of_line ();
	return;
      }

  as_bad (_("unknown floating point format `%s'\n"), name);
  *input_line_pointer = saved_char;
  ignore_rest_of_line ();
}

/* Copy symbol information.  */

void
arm_copy_symbol_attributes (symbolS *dest, symbolS *src)
{
  ARM_GET_FLAG (dest) = ARM_GET_FLAG (src);
}

/* Given a symbolic attribute NAME, return the proper integer value.
   Returns -1 if the attribute is not known.  */

int
arm_convert_symbolic_attribute (const char *name)
{
  static const struct
  {
    const char * name;
    const int    tag;
  }
  attribute_table[] =
    {
      /* When you modify this table you should
	 also modify the list in doc/c-arm.texi.  */
#define T(tag) {#tag, tag}
      T (Tag_CPU_raw_name),
      T (Tag_CPU_name),
      T (Tag_CPU_arch),
      T (Tag_CPU_arch_profile),
      T (Tag_ARM_ISA_use),
      T (Tag_THUMB_ISA_use),
      T (Tag_FP_arch),
      T (Tag_VFP_arch),
      T (Tag_WMMX_arch),
      T (Tag_Advanced_SIMD_arch),
      T (Tag_PCS_config),
      T (Tag_ABI_PCS_R9_use),
      T (Tag_ABI_PCS_RW_data),
      T (Tag_ABI_PCS_RO_data),
      T (Tag_ABI_PCS_GOT_use),
      T (Tag_ABI_PCS_wchar_t),
      T (Tag_ABI_FP_rounding),
      T (Tag_ABI_FP_denormal),
      T (Tag_ABI_FP_exceptions),
      T (Tag_ABI_FP_user_exceptions),
      T (Tag_ABI_FP_number_model),
      T (Tag_ABI_align_needed),
      T (Tag_ABI_align8_needed),
      T (Tag_ABI_align_preserved),
      T (Tag_ABI_align8_preserved),
      T (Tag_ABI_enum_size),
      T (Tag_ABI_HardFP_use),
      T (Tag_ABI_VFP_args),
      T (Tag_ABI_WMMX_args),
      T (Tag_ABI_optimization_goals),
      T (Tag_ABI_FP_optimization_goals),
      T (Tag_compatibility),
      T (Tag_CPU_unaligned_access),
      T (Tag_FP_HP_extension),
      T (Tag_VFP_HP_extension),
      T (Tag_ABI_FP_16bit_format),
      T (Tag_MPextension_use),
      T (Tag_DIV_use),
      T (Tag_nodefaults),
      T (Tag_also_compatible_with),
      T (Tag_conformance),
      T (Tag_T2EE_use),
      T (Tag_Virtualization_use),
      T (Tag_DSP_extension),
      /* We deliberately do not include Tag_MPextension_use_legacy.  */
#undef T
    };
  unsigned int i;

  if (name == NULL)
    return -1;

  for (i = 0; i < ARRAY_SIZE (attribute_table); i++)
    if (streq (name, attribute_table[i].name))
      return attribute_table[i].tag;

  return -1;
}
