/* Top level of GNU C compiler
   Copyright (C) 1987, 88, 89, 92-8, 1999 Free Software Foundation, Inc.

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

/* This is the top level of cc1/c++.
   It parses command args, opens files, invokes the various passes
   in the proper order, and counts the time used by each.
   Error messages and low-level interface to malloc also handled here.  */

#include "config.h"
#include "system.h"
#include <signal.h>
#include <setjmp.h>
#include "input.h"
#include "tree.h"
#include "c-tree.h"
#include "rtl.h"
#include "flags.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "except.h"
#include "toplev.h"
#include "expr.h"
#include "unistd.h"

#if defined (DWARF2_DEBUGGING_INFO)
#include "dwarf2out.h"
#endif

extern int rtx_equal_function_value_matters;

extern char **environ;
extern char *version_string, *language_string;

/* Carry information from ASM_DECLARE_OBJECT_NAME
   to ASM_FINISH_DECLARE_OBJECT.  */

extern int size_directive_output;
extern tree last_assemble_variable_decl;

extern void check_line_directive();

extern char *init_parse (char *);
extern void finish_parse ();
extern void init_decl_processing ();
extern void init_obstacks ();
extern void init_tree_codes ();
extern void init_rtl ();
extern void init_regs ();
extern void init_optabs ();
extern void init_stmt ();
extern void init_reg_sets ();
extern void dump_flow_info ();
extern void dump_local_alloc ();
extern void regset_release_memory ();

extern void print_rtl ();
extern void print_rtl_with_bb (FILE *, rtx);

void rest_of_decl_compilation ();
void error_with_file_and_line (char *file, int line, char *s, ...);
void error_with_decl (tree decl, char *s, ...);
void error (char *s, ...);
void fatal (char *s, ...);
void warning_with_file_and_line (char *file, int line, char *s, ...);
void warning_with_decl (tree decl, char *s, ...);
void warning (char *s, ...);
void pedwarn (char *s, ...);
void pedwarn_with_decl (tree decl, char *s, ...);
void pedwarn_with_file_and_line (char *file, int line, char *s, ...);
void sorry (char *s, ...);
static void set_target_switch (char *);
static char *decl_name (tree, int);
static void vmessage (char *, char *, va_list);
static void v_message_with_file_and_line (char *, int, char *,
                                          char *, va_list);
static void v_message_with_decl (tree, char *, char *, va_list);
static void file_and_line_for_asm (rtx, char **, int *);
static void v_error_with_file_and_line (char *, int, char *, va_list);
static void v_error_with_decl (tree, char *, va_list);
static void v_error_for_asm (rtx, char *, va_list);
static void verror (char *, va_list);
static void vfatal (char *, va_list) ATTRIBUTE_NORETURN;
static void v_warning_with_file_and_line (char *, int, char *, va_list);
static void v_warning_with_decl (tree, char *, va_list);
static void v_warning_for_asm (rtx, char *, va_list);
static void vwarning (char *, va_list);
static void vpedwarn (char *, va_list);
static void v_pedwarn_with_decl (tree, char *, va_list);
static void v_pedwarn_with_file_and_line (char *, int, char *, va_list);
static void vsorry (char *, va_list);
static void v_really_sorry (char *, va_list) ATTRIBUTE_NORETURN;
static void float_signal (int) ATTRIBUTE_NORETURN;
static void pipe_closed (int) ATTRIBUTE_NORETURN;
static void open_dump_file (char *, char *);
static void close_dump_file (void (*)(FILE *, rtx), rtx);
static void dump_rtl (char *, tree, void (*)(FILE *, rtx), rtx);
static void clean_dump_file (char *);
static void compile_file (char *);
static void display_help (void);

static void print_version (FILE *, char *);
static int print_single_switch (FILE *, int, int, char *, char *, char *,
                                char *, char *);
static void print_switch_values (FILE *, int, int, char *, char *,
                                 char *);

void print_rtl_graph_with_bb (const char *, const char *, rtx);
void clean_graph_dump_file (const char *, const char *);
void finish_graph_dump_file (const char *, const char *);
/* Length of line when printing switch values.  */
#define MAX_LINE 75

/* Name of program invoked, sans directories.  */

char *progname;

/* Copy of arguments to main.  */
int save_argc;
char **save_argv;

/* Name of current original source file (what was input to cpp).
   This comes from each #-command in the actual input.  */

char *input_filename;

/* Name of top-level original source file (what was input to cpp).
   This comes from the #-command at the beginning of the actual input.
   If there isn't any there, then this is the cc1 input file name.  */

char *main_input_filename;

/* Current line number in real source file.  */

int lineno;

/* Nonzero if it is unsafe to create any new pseudo registers.  */
int no_new_pseudos;

/* Stack of currently pending input files.  */

struct file_stack *input_file_stack;

/* Incremented on each change to input_file_stack.  */
int input_file_stack_tick;

/* FUNCTION_DECL for function now being parsed or compiled.  */

extern tree current_function_decl;

/* Name to use as base of names for dump output files.  */

char *dump_base_name;

/* Bit flags that specify the machine subtype we are compiling for.
   Bits are tested using macros TARGET_... defined in the tm.h file
   and set by `-m...' switches.  Must be defined in rtlanal.c.  */

extern int target_flags;

/* Flags saying which kinds of debugging dump have been requested.  */

int rtl_dump = 0;
int rtl_dump_and_exit = 0;
int jump_opt_dump = 0;
int addressof_dump = 0;
int cse_dump = 0;
int gcse_dump = 0;
int loop_dump = 0;
int cse2_dump = 0;
int flow_dump = 0;
int combine_dump = 0;
int regmove_dump = 0;
int local_reg_dump = 0;
int global_reg_dump = 0;
int jump2_opt_dump = 0;
int flag_print_asm_name = 0;
int mach_dep_reorg_dump = 0;
enum graph_dump_types graph_dump_format;

/* Name for output file of assembly code, specified with -o.  */

char *asm_file_name;

/* Type(s) of debugging information we are producing (if any).
   See flags.h for the definitions of the different possible
   types of debugging information.  */
enum debug_info_type write_symbols = NO_DEBUG;

/* Level of debugging information we are producing.  See flags.h
   for the definitions of the different possible levels.  */
enum debug_info_level debug_info_level = DINFO_LEVEL_NONE;

/* Nonzero means do optimizations.  -O.
   Particular numeric values stand for particular amounts of optimization;
   thus, -O2 stores 2 here.  However, the optimizations beyond the basic
   ones are not controlled directly by this variable.  Instead, they are
   controlled by individual `flag_...' variables that are defaulted
   based on this variable.  */

int optimize = 0;

/* Nonzero means optimize for size.  -Os.
   The only valid values are zero and non-zero. When optimize_size is
   non-zero, optimize defaults to 2, but certain individual code
   bloating optimizations are disabled.  */

int optimize_size = 0;

/* Number of error messages and warning messages so far.  */

int errorcount = 0;
int warningcount = 0;
int sorrycount = 0;

/* Pointer to function to compute the name to use to print a declaration.
   DECL is the declaration in question.
   VERBOSITY determines what information will be printed:
     0: DECL_NAME, demangled as necessary.
     1: and scope information.
     2: and any other information that might be interesting, such as function
        parameter types in C++.  */

char *(*decl_printable_name)            (tree, int);

/* Pointer to function to compute rtl for a language-specific tree code.  */

typedef rtx (*lang_expand_expr_t)
    (union tree_node *, rtx, enum machine_mode,
    enum expand_modifier modifier);

lang_expand_expr_t lang_expand_expr = 0;

/* Pointer to function to finish handling an incomplete decl at the
   end of compilation.  */

void (*incomplete_decl_finalize_hook) (tree) = 0;

/* Nonzero for -pedantic switch: warn about anything
   that standard spec forbids.  */

int pedantic = 0;

/* Temporarily suppress certain warnings.
   This is set while reading code from a system header file.  */

int in_system_header = 0;

/* Nonzero means do stupid register allocation.
   Currently, this is 1 if `optimize' is 0.  */

int obey_regdecls = 0;

/* Print functions as they are compiled
   and print times taken by the various passes.  -loud.  */

int loud_flag = 0;

/* -f flags.  */

/* Nonzero means give an enum type only as many bytes as it needs.  */

int flag_short_enums;

/* Nonzero for -fcaller-saves: allocate values in regs that need to
   be saved across function calls, if that produces overall better code.
   Optional now, so people can test it.  */

#ifdef DEFAULT_CALLER_SAVES
int flag_caller_saves = 1;
#else
int flag_caller_saves = 0;
#endif

/* Nonzero if structures and unions should be returned in memory.

   This should only be defined if compatibility with another compiler or
   with an ABI is needed, because it results in slower code.  */

#ifndef DEFAULT_PCC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 1
#endif

/* Nonzero for -fpcc-struct-return: return values the same way PCC does.  */

int flag_pcc_struct_return = DEFAULT_PCC_STRUCT_RETURN;

/* Nonzero for -fforce-mem: load memory value into a register
   before arithmetic on it.  This makes better cse but slower compilation.  */

int flag_force_mem = 0;

/* Nonzero for -fforce-addr: load memory address into a register before
   reference to memory.  This makes better cse but slower compilation.  */

int flag_force_addr = 0;

/* Nonzero for -fdefer-pop: don't pop args after each function call;
   instead save them up to pop many calls' args with one insns.  */

int flag_defer_pop = 0;

/* Nonzero for -ffloat-store: don't allocate floats and doubles
   in extended-precision registers.  */

int flag_float_store = 0;

/* Nonzero for -fcse-follow-jumps:
   have cse follow jumps to do a more extensive job.  */

int flag_cse_follow_jumps;

/* Nonzero for -fcse-skip-blocks:
   have cse follow a branch around a block.  */
int flag_cse_skip_blocks;

/* Nonzero for -fexpensive-optimizations:
   perform miscellaneous relatively-expensive optimizations.  */
int flag_expensive_optimizations;

/* Nonzero for -fthread-jumps:
   have jump optimize output of loop.  */

int flag_thread_jumps;

/* Nonzero enables strength-reduction in loop.c.  */

int flag_strength_reduce = 0;

/* Nonzero enables loop unrolling in unroll.c.  Only loops for which the
   number of iterations can be calculated at compile-time (UNROLL_COMPLETELY,
   UNROLL_MODULO) or at run-time (preconditioned to be UNROLL_MODULO) are
   unrolled.  */

int flag_unroll_loops;

/* Nonzero enables loop unrolling in unroll.c.  All loops are unrolled.
   This is generally not a win.  */

int flag_unroll_all_loops;

/* Nonzero forces all invariant computations in loops to be moved
   outside the loop. */

int flag_move_all_movables = 0;

/* Nonzero forces all general induction variables in loops to be
   strength reduced. */

int flag_reduce_all_givs = 0;

/* Nonzero to perform full register move optimization passes.  This is the
   default for -O2.  */

int flag_regmove = 0;

/* Nonzero for -fwritable-strings:
   store string constants in data segment and don't uniquize them.  */

int flag_writable_strings = 0;

/* Nonzero means don't put addresses of constant functions in registers.
   Used for compiling the Unix kernel, where strange substitutions are
   done on the assembly output.  */

int flag_no_function_cse = 0;

/* Nonzero for -fomit-frame-pointer:
   don't make a frame pointer in simple functions that don't require one.  */

int flag_omit_frame_pointer = 0;

/* Nonzero means place each function into its own section on those platforms
   which support arbitrary section names and unlimited numbers of sections.  */

int flag_function_sections = 0;

/* ... and similar for data.  */

int flag_data_sections = 0;

/* Nonzero to inhibit use of define_optimization peephole opts.  */

int flag_no_peephole = 0;

/* Nonzero allows GCC to violate some IEEE or ANSI rules regarding math
   operations in the interest of optimization.  For example it allows
   GCC to assume arguments to sqrt are nonnegative numbers, allowing
   faster code for sqrt to be generated.  */

int flag_fast_math = 0;

/* Nonzero means all references through pointers are volatile.  */

int flag_volatile;

/* Nonzero means treat all global and extern variables as global.  */

int flag_volatile_global;

/* Nonzero means just do syntax checking; don't output anything.  */

int flag_syntax_only = 0;

/* Nonzero means perform global cse.  */

static int flag_gcse;

/* Nonzero means to rerun cse after loop optimization.  This increases
   compilation time about 20% and picks up a few more common expressions.  */

static int flag_rerun_cse_after_loop;

/* Nonzero means to run loop optimizations twice.  */

int flag_rerun_loop_opt;

/* Nonzero for -finline-functions: ok to inline functions that look like
   good inline candidates.  */

int flag_inline_functions;

/* Nonzero for -fkeep-inline-functions: even if we make a function
   go inline everywhere, keep its definition around for debugging
   purposes.  */

int flag_keep_inline_functions;

/* Nonzero means that functions will not be inlined.  */

int flag_no_inline;

/* Nonzero means that we should emit static const variables
   regardless of whether or not optimization is turned on.  */

int flag_keep_static_consts = 1;

/* Nonzero means we should be saving declaration info into a .X file.  */

int flag_gen_aux_info = 0;

/* Specified name of aux-info file.  */

static char *aux_info_file_name;

/* CYGNUS LOCAL v850/law */
/* Nonzero means we will be outputting struct member offsets to a .s file */

int flag_gen_offset_info = 0;

/* Specified name of offset-info file.  */

static char *offset_info_file_name;
/* END CYGNUS LOCAL */

/* Nonzero means generate extra code for exception handling and enable
   exception handling.  */

int flag_exceptions;

/* Nonzero means use the new model for exception handling. Replaces
   -DNEW_EH_MODEL as a compile option. */

int flag_new_exceptions = 0;

/* Nonzero means don't place uninitialized global data in common storage
   by default.  */

int flag_no_common;

/* Nonzero means pretend it is OK to examine bits of target floats,
   even if that isn't true.  The resulting code will have incorrect constants,
   but the same series of instructions that the native compiler would make.  */

int flag_pretend_float;

/* Nonzero means change certain warnings into errors.
   Usually these are warnings about failure to conform to some standard.  */

int flag_pedantic_errors = 0;


/* flag_on_branch_count_reg means try to replace add-1,compare,branch tupple
   by a cheaper branch, on a count register. */
int flag_branch_on_count_reg;

/* END CYGNUS LOCAL meissner/nortel */
int flag_optimize_comparisons = 0;
/* END CYGNUS LOCAL meissner/nortel */

/* -finhibit-size-directive inhibits output of .size for ELF.
   This is used only for compiling crtstuff.c,
   and it may be extended to other effects
   needed for crtstuff.c on other systems.  */
int flag_inhibit_size_directive = 0;

/* -fverbose-asm causes extra commentary information to be produced in
   the generated assembly code (to make it more readable).  This option
   is generally only of use to those who actually need to read the
   generated assembly code (perhaps while debugging the compiler itself).
   -fno-verbose-asm, the default, causes the extra information
   to be omitted and is useful when comparing two assembler files.  */

int flag_verbose_asm = 0;

/* -dA causes debug commentary information to be produced in
   the generated assembly code (to make it more readable).  This option
   is generally only of use to those who actually need to read the
   generated assembly code (perhaps while debugging the compiler itself).
   Currently, this switch is only used by dwarfout.c; however, it is intended
   to be a catchall for printing debug information in the assembler file.  */

int flag_debug_asm = 0;

/* CYGNUS LOCAL unaligned-struct-hack */
/* This is a hack.  Disable the effect of SLOW_BYTE_ACCESS, so that references
   to aligned fields inside of unaligned structures can work.  That is, we
   want to always access fields with their declared size, because using a
   larger load may result in an unaligned access.  This makes some invalid
   code work at the expense of losing some optimizations.  */

int flag_unaligned_struct_hack = 0;
/* END CYGNUS LOCAL */

/* CYGNUS LOCAL unaligned-pointers */
/* Assume that pointers may have unaligned addresses, and thus treat any
   pointer indirection like a bitfield access.  */

int flag_unaligned_pointers = 0;
/* END CYGNUS LOCAL */

/* Tag all structures with __attribute__(packed) */
int flag_pack_struct = 0;

/* Emit code to check for stack overflow; also may cause large objects
   to be allocated dynamically.  */
int flag_stack_check;

/* -fcheck-memory-usage causes extra code to be generated in order to check
   memory accesses.  This is used by a detector of bad memory accesses such
   as Checker.  */
int flag_check_memory_usage = 0;

/* -fprefix-function-name causes function name to be prefixed.  This
   can be used with -fcheck-memory-usage to isolate code compiled with
   -fcheck-memory-usage.  */
int flag_prefix_function_name = 0;

/* 0 if pointer arguments may alias each other.  True in C.
   1 if pointer arguments may not alias each other but may alias
   global variables.
   2 if pointer arguments may not alias each other and may not
   alias global variables.  True in Fortran.
   This defaults to 0 for C.  */
int flag_argument_noalias = 0;

/* Nonzero if we should do (language-dependent) alias analysis.
   Typically, this analysis will assume that expressions of certain
   types do not alias expressions of certain other types.  Only used
   if alias analysis (in general) is enabled.  */
int flag_strict_aliasing = 0;

/* Instrument functions with calls at entry and exit, for profiling.  */
int flag_instrument_function_entry_exit = 0;

/* Use hex instead of decimal in ASM output.  */
int flag_hex_asm = 0;

typedef struct
{
    char *string;
    int *variable;
    int on_value;
    char *description;
}
lang_independent_options;

/* Table of language-independent -f options.
   STRING is the option name.  VARIABLE is the address of the variable.
   ON_VALUE is the value to store in VARIABLE
    if `-fSTRING' is seen as an option.
   (If `-fno-STRING' is seen as an option, the opposite value is stored.)  */

lang_independent_options f_options[] =
{
    {"float-store", &flag_float_store, 1,
     "Do not store floats in registers" },
    {"volatile", &flag_volatile, 1,
     "Consider all mem refs through pointers as volatile"},
    {"volatile-global", &flag_volatile_global, 1,
     "Consider all mem refs to global data to be volatile" },
    {"defer-pop", &flag_defer_pop, 1,
     "Defer popping functions args from stack until later" },
    {"omit-frame-pointer", &flag_omit_frame_pointer, 1,
     "When possible do not generate stack frames"},
    {"cse-follow-jumps", &flag_cse_follow_jumps, 1,
     "When running CSE, follow jumps to their targets" },
    {"cse-skip-blocks", &flag_cse_skip_blocks, 1,
     "When running CSE, follow conditional jumps" },
    {"expensive-optimizations", &flag_expensive_optimizations, 1,
     "Perform a number of minor, expensive optimisations" },
    {"thread-jumps", &flag_thread_jumps, 1,
     "Perform jump threading optimisations"},
    {"strength-reduce", &flag_strength_reduce, 1,
     "Perform strength reduction optimisations" },
    {"unroll-loops", &flag_unroll_loops, 1,
     "Perform loop unrolling when interation count is known" },
    {"unroll-all-loops", &flag_unroll_all_loops, 1,
     "Perform loop unrolling for all loops" },
    {"move-all-movables", &flag_move_all_movables, 1,
     "Force all loop invariant computations out of loops" },
    {"reduce-all-givs", &flag_reduce_all_givs, 1,
     "Strength reduce all loop general induction variables" },
    {"writable-strings", &flag_writable_strings, 1,
     "Store strings in writable data section" },
    {"peephole", &flag_no_peephole, 0,
     "Enable machine specific peephole optimisations" },
    {"force-mem", &flag_force_mem, 1,
     "Copy memory operands into registers before using" },
    {"force-addr", &flag_force_addr, 1,
     "Copy memory address constants into regs before using" },
    {"function-cse", &flag_no_function_cse, 0,
     "Allow function addresses to be held in registers" },
    {"inline-functions", &flag_inline_functions, 1,
     "Integrate simple functions into their callers" },
    {"keep-inline-functions", &flag_keep_inline_functions, 1,
     "Generate code for funcs even if they are fully inlined" },
    {"inline", &flag_no_inline, 0,
     "Pay attention to the 'inline' keyword"},
    {"keep-static-consts", &flag_keep_static_consts, 1,
     "Emit static const variables even if they are not used" },
    {"syntax-only", &flag_syntax_only, 1,
     "Check for syntax errors, then stop" },
    {"caller-saves", &flag_caller_saves, 1,
     "Enable saving registers around function calls" },
    {"pcc-struct-return", &flag_pcc_struct_return, 1,
     "Return 'short' aggregates in memory, not registers" },
    {"reg-struct-return", &flag_pcc_struct_return, 0,
     "Return 'short' aggregates in registers" },
    {"gcse", &flag_gcse, 1,
     "Perform the global common subexpression elimination" },
    {"rerun-cse-after-loop", &flag_rerun_cse_after_loop, 1,
     "Run CSE pass after loop optimisations"},
    {"rerun-loop-opt", &flag_rerun_loop_opt, 1,
     "Run the loop optimiser twice"},
    {"pretend-float", &flag_pretend_float, 1,
     "Pretend that host and target use the same FP format"},
    {"branch-count-reg",&flag_branch_on_count_reg, 1,
     "Replace add,compare,branch with branch on count reg"},
/* END CYGNUS LOCAL meissner/nortel */
    {"optimize-comparisons", &flag_optimize_comparisons, 1,
     "Make some comparison operations sequence optimizations"},
/* END CYGNUS LOCAL meissner/nortel */
    {"exceptions", &flag_exceptions, 1,
     "Enable exception handling" },
    {"new-exceptions", &flag_new_exceptions, 1,
     "Use the new model for exception handling" },
    {"sjlj-exceptions", &exceptions_via_longjmp, 1,
     "Use setjmp/longjmp to handle exceptions" },
    {"asynchronous-exceptions", &asynchronous_exceptions, 1,
     "Support asynchronous exceptions" },
    {"fast-math", &flag_fast_math, 1,
     "Improve FP speed by violating ANSI & IEEE rules" },
    {"common", &flag_no_common, 0,
     "Do not put unitialised globals in the common section" },
    {"inhibit-size-directive", &flag_inhibit_size_directive, 1,
     "Do not generate .size directives" },
    {"function-sections", &flag_function_sections, 1,
     "place each function into its own section" },
    {"data-sections", &flag_data_sections, 1,
     "place data items into their own section" },
    {"verbose-asm", &flag_verbose_asm, 1,
     "Add extra commentry to assembler output"},
    /* CYGNUS LOCAL unaligned-struct-hack */
    {"unaligned-struct-hack", &flag_unaligned_struct_hack, 1,
     "Assume structure fields may be unaligned" },
    /* END CYGNUS LOCAL */
    /* CYGNUS LOCAL unaligned-pointers */
    {"unaligned-pointers", &flag_unaligned_pointers, 1,
     "Assume all pointers might be unaligned"},
    /* END CYGNUS LOCAL */
    {"regmove", &flag_regmove, 1,
     "Enables a register move optimisation"},
    {"optimize-register-move", &flag_regmove, 1,
     "Do the full regmove optimization pass"},
    {"pack-struct", &flag_pack_struct, 1,
     "Pack structure members together without holes" },
    {"stack-check", &flag_stack_check, 1,
     "Insert stack checking code into the program" },
    {"argument-alias", &flag_argument_noalias, 0,
     "Specify that arguments may alias each other & globals"},
    {"argument-noalias", &flag_argument_noalias, 1,
     "Assume arguments may alias globals but not each other"},
    {"argument-noalias-global", &flag_argument_noalias, 2,
     "Assume arguments do not alias each other or globals" },
    {"strict-aliasing", &flag_strict_aliasing, 1,
     "Assume strict aliasing rules apply" },
    {"check-memory-usage", &flag_check_memory_usage, 1,
     "Generate code to check every memory access" },
    {"prefix-function-name", &flag_prefix_function_name, 1,
     "Add a prefix to all function names" },
    {"dump-unnumbered", &flag_dump_unnumbered, 1,
     "Suppress output of instruction numbers and line number notes in debugging dumps"},
    {"instrument-functions", &flag_instrument_function_entry_exit, 1,
     "Instrument function entry/exit with profiling calls"},
    {"hex-asm", &flag_hex_asm, 1,
     "Use hex instead of decimal in assembly output"},
};

#define NUM_ELEM(a)  (sizeof (a) / sizeof ((a)[0]))

/* Table of language-specific options.  */

static struct lang_opt
{
    char *option;
    char *description;
}
documented_lang_options[] =
{
    /* In order not to overload the --help output, the convention
       used here is to only describe those options which are not
       enabled by default.  */

    { "-ansi", "Compile just for ANSI C" },
    { "-fallow-single-precision",
      "Do not promote floats to double if using -traditional" },
    { "-std= ", "Determine language standard"},

    { "-fsigned-bitfields", "" },
    { "-funsigned-bitfields","Make bitfields by unsigned by default" },
    { "-fno-signed-bitfields", "" },
    { "-fno-unsigned-bitfields","" },

    { "-ftraditional", "" },
    { "-traditional", "Attempt to support traditional K&R style C"},
    { "-fnotraditional", "" },
    { "-fno-traditional", "" },

    { "-fasm", "" },
    { "-fno-asm", "Do not recognise the 'asm' keyword" },
    { "-fbuiltin", "" },
    { "-fno-builtin", "Do not recognise any built in functions" },
    { "-fhosted", "Assume normal C execution environment" },
    { "-fno-hosted", "" },
    { "-ffreestanding",
      "Assume that standard libraries & main might not exist" },
    { "-fno-freestanding", "" },
    { "-fcond-mismatch", "Allow different types as args of ? operator"},
    { "-fno-cond-mismatch", "" },
    { "-fident", "" },
    { "-fno-ident", "Ignore #ident directives" },
    { "-fshort-double", "Use the same size for double as for float" },
    { "-fno-short-double", "" },
    { "-fshort-enums", "Use the smallest fitting integer to hold enums"},
    { "-fno-short-enums", "" },

    { "-Wall", "Enable most warning messages" },
    { "-Wbad-function-cast",
      "Warn about casting functions to incompatible types" },
    { "-Wno-bad-function-cast", "" },
    { "-Wmissing-noreturn",
      "Warn about functions which might be candidates for attribute noreturn" },
    { "-Wno-missing-noreturn", "" },
    { "-Wcast-qual", "Warn about casts which discard qualifiers"},
    { "-Wno-cast-qual", "" },
    { "-Wchar-subscripts", "Warn about subscripts whose type is 'char'"},
    { "-Wno-char-subscripts", "" },
    { "-Wconversion", "Warn about possibly confusing type conversions" },
    { "-Wno-conversion", "" },
    { "-Wformat", "Warn about printf format anomalies" },
    { "-Wno-format", "" },
    { "-Wimplicit-function-declaration",
      "Warn about implicit function declarations" },
    { "-Wno-implicit-function-declaration", "" },
    { "-Werror-implicit-function-declaration", "" },
    { "-Wimplicit-int", "Warn when a declaration does not specify a type" },
    { "-Wno-implicit-int", "" },
    { "-Wimplicit", "" },
    { "-Wno-implicit", "" },
    { "-Wimport", "Warn about the use of the #import directive" },
    { "-Wno-import", "" },
    { "-Wlong-long","" },
    { "-Wno-long-long", "Do not warn about using 'long long' when -pedantic" },
    { "-Wmissing-braces",
      "Warn about possibly missing braces around initialisers" },
    { "-Wno-missing-braces", "" },
    { "-Wmissing-declarations",
      "Warn about global funcs without previous declarations"},
    { "-Wno-missing-declarations", "" },
    { "-Wmissing-prototypes", "Warn about global funcs without prototypes" },
    { "-Wno-missing-prototypes", "" },
    { "-Wmultichar", "Warn about use of multicharacter literals"},
    { "-Wno-multichar", "" },
    { "-Wnested-externs", "Warn about externs not at file scope level" },
    { "-Wno-nested-externs", "" },
    { "-Wparentheses", "Warn about possible missing parentheses" },
    { "-Wno-parentheses", "" },
    { "-Wpointer-arith", "Warn about function pointer arithmetic" },
    { "-Wno-pointer-arith", "" },
    { "-Wredundant-decls",
      "Warn about multiple declarations of the same object" },
    { "-Wno-redundant-decls", "" },
    { "-Wsign-compare", "Warn about signed/unsigned comparisons" },
    { "-Wno-sign-compare", "" },
    { "-Wstrict-prototypes", "Warn about non-prototyped function decls" },
    { "-Wno-strict-prototypes", "" },
    { "-Wtraditional", "Warn about constructs whose meaning change in ANSI C"},
    { "-Wno-traditional", "" },
    { "-Wwrite-strings", "Mark strings as 'const char *'"},
    { "-Wno-write-strings", "" },
};

/* Here is a table, controlled by the tm.h file, listing each -m switch
   and which bits in `target_switches' it should set or clear.
   If VALUE is positive, it is bits to set.
   If VALUE is negative, -VALUE is bits to clear.
   (The sign bit is not used so there is no confusion.)  */

struct
{
    char * name;
    int value;
    char * description;
}
target_switches [] = TARGET_SWITCHES;

/* This table is similar, but allows the switch to have a value.  */

#ifdef TARGET_OPTIONS
struct
{
    char *  prefix;
    char ** variable;
    char *  description;
}
target_options [] = TARGET_OPTIONS;
#endif

/* Options controlling warnings */

/* Don't print warning messages.  -w.  */

int inhibit_warnings = 0;

/* Print various extra warnings.  -W.  */

int extra_warnings = 0;

/* Treat warnings as errors.  -Werror.  */

int warnings_are_errors = 0;

/* Nonzero to warn about unused local variables.  */

int warn_unused;

/* Nonzero to warn about variables used before they are initialized.  */

int warn_uninitialized;

/* Nonzero means warn about all declarations which shadow others.   */

int warn_shadow;

/* Warn if a switch on an enum fails to have a case for every enum value.  */

int warn_switch;

/* Nonzero means warn about function definitions that default the return type
   or that use a null return and have a return-type other than void.  */

int warn_return_type;

/* Nonzero means warn about pointer casts that increase the required
   alignment of the target type (and might therefore lead to a crash
   due to a misaligned access).  */

int warn_cast_align;

/* Nonzero means warn about any identifiers that match in the first N
   characters.  The value N is in `id_clash_len'.  */

int warn_id_clash;
unsigned id_clash_len;

/* Nonzero means warn about any objects definitions whose size is larger
   than N bytes.  Also want about function definitions whose returned
   values are larger than N bytes. The value N is in `larger_than_size'.  */

int warn_larger_than;
unsigned larger_than_size;

/* Nonzero means warn if inline function is too large.  */

int warn_inline;

/* Warn if a function returns an aggregate,
   since there are often incompatible calling conventions for doing this.  */

int warn_aggregate_return;

/* Likewise for -W.  */

lang_independent_options W_options[] =
{
    {"unused", &warn_unused, 1, "Warn when a variable is unused" },
    {"error", &warnings_are_errors, 1, ""},
    {"shadow", &warn_shadow, 1, "Warn when one local variable shadows another" },
    {"switch", &warn_switch, 1,
     "Warn about enumerated switches missing a specific case" },
    {"aggregate-return", &warn_aggregate_return, 1,
     "Warn about returning structures, unions or arrays" },
    {"cast-align", &warn_cast_align, 1,
     "Warn about pointer casts which increase alignment" },
    {"uninitialized", &warn_uninitialized, 1,
     "Warn about unitialized automatic variables"},
    {"inline", &warn_inline, 1,
     "Warn when an inlined function cannot be inlined"}
};

/* Output files for assembler code (real compiler output)
   and debugging dumps.  */

FILE *asm_out_file;
FILE *aux_info_file;
FILE *rtl_dump_file = NULL;

/* CYGNUS LOCAL v850/law */
FILE *offset_info_file;
/* END CYGNUS LOCAL */

/* Time accumulators, to count the total time spent in various passes.  */

int parse_time;
int varconst_time;
int integration_time;
int jump_time;
int cse_time;
int gcse_time;
int loop_time;
int cse2_time;
int flow_time;
int combine_time;
int regmove_time;
int local_alloc_time;
int global_alloc_time;
int shorten_branch_time;
int stack_reg_time;
int final_time;
int symout_time;
int dump_time;

/* Return time used so far, in microseconds.  */

long
get_run_time()
{
    if (!loud_flag)
        return 0;

    clock_t clk = clock();

    if (clk < 0)
        return 0;

    return (clk * 1000000) / CLOCKS_PER_SEC;
}

#define TIMEVAR(VAR, BODY)    \
    do { int otime = get_run_time(); BODY; VAR += get_run_time() - otime; } while (0)

void
print_time(char *str, int total)
{
    fprintf(stderr,
            "time in %s: %d.%06d\n",
            str, total / 1000000, total % 1000000);
}

/* Count an error or warning.  Return 1 if the message should be printed.  */

int
count_error(int warningp)
{
    if (warningp && inhibit_warnings)
        return 0;

    if (warningp && !warnings_are_errors)
        warningcount++;
    else
    {
        static int warning_message = 0;

        if (warningp && !warning_message)
        {
            fprintf(stderr, "%s: warnings being treated as errors\n", progname);
            warning_message = 1;
        }
        errorcount++;
    }

    return 1;
}

/* Print a fatal error message.  NAME is the text.
   Also include a system error message based on `errno'.  */

void
pfatal_with_name(char *name)
{
    fprintf(stderr, "%s: ", progname);
    perror(name);
    exit(EXIT_FAILURE);
}

void
fatal_io_error(char *name)
{
    fprintf(stderr, "%s: %s: I/O error\n", progname, name);
    exit(EXIT_FAILURE);
}

/* Called to give a better error message for a bad insn rather than
   just calling abort().  */

void
fatal_insn(char *message, rtx insn)
{
    error(message);
    debug_rtx(insn);
    if (asm_out_file)
        fflush(asm_out_file);
    if (aux_info_file)
        fflush(aux_info_file);
    if (rtl_dump_file != NULL)
        fflush(rtl_dump_file);
    fflush(stdout);
    fflush(stderr);
    abort();
}

/* Called to give a better error message when we don't have an insn to match
   what we are looking for or if the insn's constraints aren't satisfied,
   rather than just calling abort().  */

void
fatal_insn_not_found(rtx insn)
{
    if (INSN_CODE(insn) < 0)
        fatal_insn("internal error--unrecognizable insn:", insn);
    else
        fatal_insn("internal error--insn does not satisfy its constraints:", insn);
}

/* This is the default decl_printable_name function.  */

static char *
decl_name(tree decl, int verbosity ATTRIBUTE_UNUSED)
{
    return IDENTIFIER_POINTER(DECL_NAME(decl));
}

static int need_error_newline;

/* Function of last error message;
   more generally, function such that if next error message is in it
   then we don't have to mention the function name.  */
static tree last_error_function = NULL;

/* Used to detect when input_file_stack has changed since last described.  */
static int last_error_tick;

/* Called when the start of a function definition is parsed,
   this function prints on stderr the name of the function.  */

void
announce_function(tree decl)
{
    if (loud_flag)
    {
        if (rtl_dump_and_exit)
            fprintf(stderr, "%s ", IDENTIFIER_POINTER(DECL_NAME(decl)));
        else
            fprintf(stderr, " %s", (*decl_printable_name) (decl, 2));
        fflush(stderr);
        need_error_newline = 1;
        last_error_function = current_function_decl;
    }
}

/* The default function to print out name of current function that caused
   an error.  */

void
default_print_error_function(char *file)
{
    if (last_error_function != current_function_decl)
    {
        char *kind = "function";
        if (current_function_decl != 0
            && TREE_CODE(TREE_TYPE(current_function_decl)) == METHOD_TYPE)
            kind = "method";

        if (file)
            fprintf(stderr, "%s: ", file);

        if (current_function_decl == NULL)
            fprintf(stderr, "At top level:\n");
        else
        {
            char *name = (*decl_printable_name) (current_function_decl, 2);
            fprintf(stderr, "In %s `%s':\n", kind, name);
        }

        last_error_function = current_function_decl;
    }
}

/* Called by report_error_function to print out function name.
 * Default may be overridden by language front-ends.  */

void (*print_error_function) (char *) = default_print_error_function;

/* Prints out, if necessary, the name of the current function
   that caused an error.  Called from all error and warning functions.  */

void
report_error_function(char *file)
{
    struct file_stack *p;

    if (need_error_newline)
    {
        fprintf(stderr, "\n");
        need_error_newline = 0;
    }

    (*print_error_function) (file);

    if (input_file_stack && input_file_stack->next != 0
        && input_file_stack_tick != last_error_tick
        && file == input_filename)
    {
        fprintf(stderr, "In file included");
        for (p = input_file_stack->next; p; p = p->next)
        {
            fprintf(stderr, " from %s:%d", p->name, p->line);
            if (p->next)
                fprintf(stderr, ",\n                ");
        }
        fprintf(stderr, ":\n");
        last_error_tick = input_file_stack_tick;
    }
}

/* Print a message.  */

static void
vmessage(char *prefix, char *s, va_list ap)
{
    if (prefix)
        fprintf(stderr, "%s: ", prefix);

    vfprintf(stderr, s, ap);
}

/* Print a message relevant to line LINE of file FILE.  */

static void
v_message_with_file_and_line(char *file, int line, char *prefix, char *s, va_list ap)
{
    if (file)
        fprintf(stderr, "%s:%d: ", file, line);
    else
        fprintf(stderr, "%s: ", progname);

    vmessage(prefix, s, ap);
    fputc('\n', stderr);
}

/* Print a message relevant to the given DECL.  */

static void
v_message_with_decl(tree decl, char *prefix, char *s, va_list ap)
{
    char *p;

    fprintf(stderr, "%s:%d: ",
            DECL_SOURCE_FILE(decl), DECL_SOURCE_LINE(decl));

    if (prefix)
        fprintf(stderr, "%s: ", prefix);

    /* Do magic to get around lack of varargs support for insertion
       of arguments into existing list.  We know that the decl is first;
       we ass_u_me that it will be printed with "%s".  */

    for (p = s; *p; ++p)
    {
        if (*p == '%')
        {
            if (*(p + 1) == '%')
                ++p;
            else
                break;
        }
    }

    if (p > s)                  /* Print the left-hand substring.  */
    {
        char fmt[sizeof "%.255s"];
        long width = p - s;

        if (width > 255L) width = 255L; /* arbitrary */
        sprintf(fmt, "%%.%lds", width);
        fprintf(stderr, fmt, s);
    }

    if (*p == '%')              /* Print the name.  */
    {
        char *n = (DECL_NAME(decl)
                   ? (*decl_printable_name) (decl, 2)
                   : "((anonymous))");
        fputs(n, stderr);
        while (*p)
        {
            ++p;
            if (ISALPHA(*(p - 1) & 0xFF))
                break;
        }
    }

    if (*p)                     /* Print the rest of the message.  */
        vmessage((char *)NULL, p, ap);

    fputc('\n', stderr);
}

/* Figure file and line of the given INSN.  */

static void
file_and_line_for_asm(rtx insn, char **pfile, int *pline)
{
    rtx body = PATTERN(insn);
    rtx asmop;

    /* Find the (or one of the) ASM_OPERANDS in the insn.  */
    if (GET_CODE(body) == SET && GET_CODE(SET_SRC(body)) == ASM_OPERANDS)
        asmop = SET_SRC(body);
    else if (GET_CODE(body) == ASM_OPERANDS)
        asmop = body;
    else if (GET_CODE(body) == PARALLEL
             && GET_CODE(XVECEXP(body, 0, 0)) == SET)
        asmop = SET_SRC(XVECEXP(body, 0, 0));
    else if (GET_CODE(body) == PARALLEL
             && GET_CODE(XVECEXP(body, 0, 0)) == ASM_OPERANDS)
        asmop = XVECEXP(body, 0, 0);
    else
        asmop = NULL;

    if (asmop)
    {
        *pfile = ASM_OPERANDS_SOURCE_FILE(asmop);
        *pline = ASM_OPERANDS_SOURCE_LINE(asmop);
    }
    else
    {
        *pfile = input_filename;
        *pline = lineno;
    }
}

/* Report an error at line LINE of file FILE.  */

static void
v_error_with_file_and_line(char *file, int line, char *s, va_list ap)
{
    count_error(0);
    report_error_function(file);
    v_message_with_file_and_line(file, line, (char *)NULL, s, ap);
}

void
error_with_file_and_line(char *file, int line, char *s, ...)
{
    va_list ap;

    va_start(ap, s);


    v_error_with_file_and_line(file, line, s, ap);
    va_end(ap);
}

/* Report an error at the declaration DECL.
   S is a format string which uses %s to substitute the declaration
   name; subsequent substitutions are a la printf.  */

static void
v_error_with_decl(tree decl, char *s, va_list ap)
{
    count_error(0);
    report_error_function(DECL_SOURCE_FILE(decl));
    v_message_with_decl(decl, (char *)NULL, s, ap);
}

void
error_with_decl(tree decl, char *s, ...)
{
    va_list ap;

    va_start(ap, s);


    v_error_with_decl(decl, s, ap);
    va_end(ap);
}

/* Report an error at the line number of the insn INSN.
   This is used only when INSN is an `asm' with operands,
   and each ASM_OPERANDS records its own source file and line.  */

static void
v_error_for_asm(rtx insn, char *s, va_list ap)
{
    char *file;
    int line;

    count_error(0);
    file_and_line_for_asm(insn, &file, &line);
    report_error_function(file);
    v_message_with_file_and_line(file, line, (char *)NULL, s, ap);
}

void
error_for_asm(rtx insn, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_error_for_asm(insn, s, ap);
    va_end(ap);
}

/* Report an error at the current line number.  */

static void
verror(char *s, va_list ap)
{
    v_error_with_file_and_line(input_filename, lineno, s, ap);
}

void
error(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    verror(s, ap);
    va_end(ap);
}

/* Report a fatal error at the current line number.  */

static void
vfatal(char *s, va_list ap)
{
    verror(s, ap);
    exit(EXIT_FAILURE);
}

void
fatal(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    vfatal(s, ap);
    va_end(ap);
}

/* Report a warning at line LINE of file FILE.  */

static void
v_warning_with_file_and_line(char *file, int line, char *s, va_list ap)
{
    if (count_error(1))
    {
        report_error_function(file);
        v_message_with_file_and_line(file, line, "warning", s, ap);
    }
}

void
warning_with_file_and_line(char *file, int line, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_warning_with_file_and_line(file, line, s, ap);
    va_end(ap);
}

/* Report a warning at the declaration DECL.
   S is a format string which uses %s to substitute the declaration
   name; subsequent substitutions are a la printf.  */

static void
v_warning_with_decl(tree decl, char *s, va_list ap)
{
    if (count_error(1))
    {
        report_error_function(DECL_SOURCE_FILE(decl));
        v_message_with_decl(decl, "warning", s, ap);
    }
}

void
warning_with_decl(tree decl, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_warning_with_decl(decl, s, ap);
    va_end(ap);
}

/* Report a warning at the line number of the insn INSN.
   This is used only when INSN is an `asm' with operands,
   and each ASM_OPERANDS records its own source file and line.  */

static void
v_warning_for_asm(rtx insn, char *s, va_list ap)
{
    if (count_error(1))
    {
        char *file;
        int line;

        file_and_line_for_asm(insn, &file, &line);
        report_error_function(file);
        v_message_with_file_and_line(file, line, "warning", s, ap);
    }
}

void
warning_for_asm(rtx insn, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_warning_for_asm(insn, s, ap);
    va_end(ap);
}

/* Report a warning at the current line number.  */

static void
vwarning(char *s, va_list ap)
{
    v_warning_with_file_and_line(input_filename, lineno, s, ap);
}

void
warning(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    vwarning(s, ap);
    va_end(ap);
}

/* These functions issue either warnings or errors depending on
   -pedantic-errors.  */

static void
vpedwarn(char *s, va_list ap)
{
    if (flag_pedantic_errors)
        verror(s, ap);
    else
        vwarning(s, ap);
}

void
pedwarn(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    vpedwarn(s, ap);
    va_end(ap);
}

static void
v_pedwarn_with_decl(tree decl, char *s, va_list ap)
{
    /* We don't want -pedantic-errors to cause the compilation to fail from
       "errors" in system header files.  Sometimes fixincludes can't fix what's
       broken (eg: unsigned char bitfields - fixing it may change the alignment
       which will cause programs to mysteriously fail because the C library
       or kernel uses the original layout).  There's no point in issuing a
       warning either, it's just unnecessary noise.  */

    if (!DECL_IN_SYSTEM_HEADER(decl))
    {
        if (flag_pedantic_errors)
            v_error_with_decl(decl, s, ap);
        else
            v_warning_with_decl(decl, s, ap);
    }
}

void
pedwarn_with_decl(tree decl, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_pedwarn_with_decl(decl, s, ap);
    va_end(ap);
}

static void
v_pedwarn_with_file_and_line(char *file, int line, char *s, va_list ap)
{
    if (flag_pedantic_errors)
        v_error_with_file_and_line(file, line, s, ap);
    else
        v_warning_with_file_and_line(file, line, s, ap);
}

void
pedwarn_with_file_and_line(char *file, int line, char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_pedwarn_with_file_and_line(file, line, s, ap);
    va_end(ap);
}

/* Apologize for not implementing some feature.  */

static void
vsorry(char *s, va_list ap)
{
    sorrycount++;
    if (input_filename)
        fprintf(stderr, "%s:%d: ", input_filename, lineno);
    else
        fprintf(stderr, "%s: ", progname);
    vmessage("sorry, not implemented", s, ap);
    fputc('\n', stderr);
}

void
sorry(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    vsorry(s, ap);
    va_end(ap);
}

/* Apologize for not implementing some feature, then quit.  */

static void
v_really_sorry(char *s, va_list ap)
{
    sorrycount++;
    if (input_filename)
        fprintf(stderr, "%s:%d: ", input_filename, lineno);
    else
        fprintf(stderr, "%s: ", progname);
    vmessage("sorry, not implemented", s, ap);
    fatal(" (fatal)\n");
}

void
really_sorry(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    v_really_sorry(s, ap);
    va_end(ap);
}

/* More 'friendly' abort that prints the line and file.
   config.h can #define abort fancy_abort if you like that sort of thing.

   I don't think this is actually a good idea.
   Other sorts of crashes will look a certain way.
   It is a good thing if crashes from calling abort look the same way.
     -- RMS  */

void
fancy_abort()
{
    fatal("internal gcc abort");
}

/* This calls abort and is used to avoid problems when abort if a macro.
   It is used when we need to pass the address of abort.  */

void
do_abort()
{
    abort();
}

/* Same as `malloc' but report error if no memory available.  */

void *
xmalloc(size_t size)
{
    void *value;

    if (size == 0)
        size = 1;

    value = malloc(size);
    if (value == 0)
        fatal("virtual memory exhausted");
    return value;
}

/* Same as `calloc' but report error if no memory available.  */

void *
xcalloc(size_t size1, size_t size2)
{
    void *value;

    if (size1 == 0 || size2 == 0)
        size1 = size2 = 1;

    value = calloc(size1, size2);
    if (value == 0)
        fatal("virtual memory exhausted");
    return value;
}


/* Same as `realloc' but report error if no memory available.
   Also handle null PTR even if the vendor realloc gets it wrong.  */

void *
xrealloc(void *ptr, size_t size)
{
    void *result;

    if (size == 0)
        size = 1;

    result = ptr ? realloc(ptr, size) : malloc(size);

    if (!result)
        fatal("virtual memory exhausted");

    return result;
}

/* Same as `strdup' but report error if no memory available.  */

char *
xstrdup(const char *s)
{
    char *result = (char *) malloc(strlen(s) + 1);

    if (!result)
        fatal("virtual memory exhausted");
    strcpy(result, s);
    return result;
}

/* Return the logarithm of X, base 2, considering X unsigned,
   if X is a power of 2.  Otherwise, returns -1.

   This should be used via the `exact_log2' macro.  */

int
exact_log2_wide(HOST_WIDE_UINT x)
{
    int log = 0;
    /* Test for 0 or a power of 2.  */
    if (x == 0 || x != (x & -x))
        return -1;
    while ((x >>= 1) != 0)
        log++;
    return log;
}

/* Given X, an unsigned number, return the largest int Y such that 2**Y <= X.
   If X is 0, return -1.

   This should be used via the floor_log2 macro.  */

int
floor_log2_wide(HOST_WIDE_UINT x)
{
    int log = -1;
    while (x != 0)
        log++,
        x >>= 1;
    return log;
}

static int float_handler_set;
int float_handled;
jmp_buf float_handler;

/* Signals actually come here.  */

static void
float_signal(int signo ATTRIBUTE_UNUSED)
{
    if (float_handled == 0)
        abort();
#if defined (USG) || defined (hpux)
    signal(SIGFPE, float_signal); /* re-enable the signal catcher */
#endif
    float_handled = 0;
    signal(SIGFPE, float_signal);
    longjmp(float_handler, 1);
}

/* Specify where to longjmp to when a floating arithmetic error happens.
   If HANDLER is 0, it means don't handle the errors any more.  */

void
set_float_handler(jmp_buf handler)
{
    float_handled = (handler != 0);
    if (handler)
        copy_memory((char *) handler, (char *) float_handler, sizeof (float_handler));

    if (float_handled && !float_handler_set)
    {
        signal(SIGFPE, float_signal);
        float_handler_set = 1;
    }
}

/* Specify, in HANDLER, where to longjmp to when a floating arithmetic
   error happens, pushing the previous specification into OLD_HANDLER.
   Return an indication of whether there was a previous handler in effect.  */

int
push_float_handler(jmp_buf handler, jmp_buf old_handler)
{
    int was_handled = float_handled;

    float_handled = 1;
    if (was_handled)
        memcpy((char *) old_handler, (char *) float_handler,
               sizeof (float_handler));

    memcpy((char *) float_handler, (char *) handler, sizeof (float_handler));
    return was_handled;
}

/* Restore the previous specification of whether and where to longjmp to
   when a floating arithmetic error happens.  */

void
pop_float_handler(int handled, jmp_buf handler)
{
    float_handled = handled;
    if (handled)
        copy_memory((char *) handler, (char *) float_handler, sizeof (float_handler));
}

/* Handler for SIGPIPE.  */

static void
pipe_closed(int signo ATTRIBUTE_UNUSED)
{
    fatal("output pipe has been closed");
}

/* Strip off a legitimate source ending from the input string NAME of
   length LEN.  Rather than having to know the names used by all of
   our front ends, we strip off an ending of a period followed by
   up to five characters.  (Java uses ".class".) */

void
strip_off_ending(char *name, int len)
{
    int i;
    for (i = 2; i < 6 && len > i; i++)
    {
        if (name[len - i] == '.')
        {
            name[len - i] = '\0';
            break;
        }
    }
}

/* Routine to open a dump file.  */
static void
open_dump_file(char *suffix, char *function_name)
{
    char *dumpname;

    TIMEVAR
        (dump_time,
        {
            dumpname = (char *) xmalloc(strlen(dump_base_name) + strlen(suffix) + 1);

            if (rtl_dump_file != NULL)
                fclose(rtl_dump_file);

            strcpy(dumpname, dump_base_name);
            strcat(dumpname, suffix);

            rtl_dump_file = fopen(dumpname, "a");

            if (rtl_dump_file == NULL)
                pfatal_with_name(dumpname);

            free(dumpname);

            if (function_name)
                fprintf(rtl_dump_file, "\n;; Function %s\n\n", function_name);
        });

    return;
}

/* Routine to close a dump file.  */
static void
close_dump_file(void (*func) (FILE *, rtx), rtx insns)
{
    TIMEVAR
        (dump_time,
        {
            if (func)
                func(rtl_dump_file, insns);

            fflush(rtl_dump_file);
            fclose(rtl_dump_file);

            rtl_dump_file = NULL;
        });

    return;
}

/* Routine to dump rtl into a file.  */
static void
dump_rtl(char *suffix, tree decl, void (*func) (FILE *, rtx), rtx insns)
{
    open_dump_file(suffix, decl_printable_name(decl, 2));
    close_dump_file(func, insns);
}

/* Routine to empty a dump file.  */
static void
clean_dump_file(char *suffix)
{
    char *dumpname;

    dumpname = (char *) xmalloc(strlen(dump_base_name) + strlen(suffix) + 1);

    strcpy(dumpname, dump_base_name);
    strcat(dumpname, suffix);

    rtl_dump_file = fopen(dumpname, "w");

    if (rtl_dump_file == NULL)
        pfatal_with_name(dumpname);

    free(dumpname);

    fclose(rtl_dump_file);
    rtl_dump_file = NULL;

    return;
}


/* Compile an entire file of output from cpp, named NAME.
   Write a file of assembly output and various debugging dumps.  */

static void
compile_file(char *name)
{
    tree globals;
    int start_time;

    int name_specified = name != 0;

    if (dump_base_name == 0)
        dump_base_name = name ? name : "gccdump";

    parse_time = 0;
    varconst_time = 0;
    integration_time = 0;
    jump_time = 0;
    cse_time = 0;
    gcse_time = 0;
    loop_time = 0;
    cse2_time = 0;
    flow_time = 0;
    combine_time = 0;
    regmove_time = 0;
    local_alloc_time = 0;
    global_alloc_time = 0;
    shorten_branch_time = 0;
    stack_reg_time = 0;
    final_time = 0;
    symout_time = 0;
    dump_time = 0;

    /* Initialize data in various passes.  */

    init_obstacks();
    init_tree_codes();
    name = init_parse(name);
    init_rtl();
    init_emit_once(debug_info_level == DINFO_LEVEL_NORMAL
                   || debug_info_level == DINFO_LEVEL_VERBOSE);
    init_regs();
    init_decl_processing();
    init_optabs();
    init_stmt();
    init_expmed();
    init_expr_once();
    init_loop();
    init_reload();
    init_alias_once();

    if (flag_caller_saves)
        init_caller_save();

    /* If auxiliary info generation is desired, open the output file.
       This goes in the same directory as the source file--unlike
       all the other output files.  */
    if (flag_gen_aux_info)
    {
        aux_info_file = fopen(aux_info_file_name, "w");
        if (aux_info_file == 0)
            pfatal_with_name(aux_info_file_name);
    }

    /* CYGNUS LOCAL v850/law */
    if (flag_gen_offset_info)
    {
        offset_info_file = fopen(offset_info_file_name, "w");
        if (offset_info_file == 0)
            pfatal_with_name(offset_info_file_name);
    }
    /* END CYGNUS LOCAL */

    /* Clear the dump files.  */
    if (rtl_dump)
        clean_dump_file(".rtl");
    if (jump_opt_dump)
    {
        clean_dump_file(".jump");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".jump");
    }
    if (addressof_dump)
    {
        clean_dump_file(".addressof");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".addressof");
    }
    if (cse_dump)
    {
        clean_dump_file(".cse");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".cse");
    }
    if (loop_dump)
    {
        clean_dump_file(".loop");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".loop");
    }
    if (cse2_dump)
    {
        clean_dump_file(".cse2");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".cse2");
    }
    if (flow_dump)
    {
        clean_dump_file(".flow");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".flow");
    }
    if (combine_dump)
    {
        clean_dump_file(".combine");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".combine");
    }
    if (regmove_dump)
    {
        clean_dump_file(".regmove");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".regmove");
    }
    if (local_reg_dump)
    {
        clean_dump_file(".lreg");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".lreg");
    }
    if (global_reg_dump)
    {
        clean_dump_file(".greg");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".greg");
    }
    if (jump2_opt_dump)
    {
        clean_dump_file(".jump2");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".jump2");
    }
    if (gcse_dump)
    {
        clean_dump_file(".gcse");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".gcse");
    }
    if (mach_dep_reorg_dump)
    {
        clean_dump_file(".mach");
        if (graph_dump_format != no_graph)
            clean_graph_dump_file(dump_base_name, ".mach");
    }

    /* Open assembler code output file.  */

    if (flag_syntax_only)
        asm_out_file = NULL;
    else
    {
        if (!name_specified && asm_file_name == 0)
            asm_out_file = stdout;
        else
        {
            int len = strlen(dump_base_name);
            char *dumpname = (char *) xmalloc(len + 6);
            strcpy(dumpname, dump_base_name);
            strip_off_ending(dumpname, len);
            strcat(dumpname, ".s");
            if (asm_file_name == 0)
            {
                asm_file_name = (char *) xmalloc(strlen(dumpname) + 1);
                strcpy(asm_file_name, dumpname);
            }
            if (!strcmp(asm_file_name, "-"))
                asm_out_file = stdout;
            else
                asm_out_file = fopen(asm_file_name, "w");
            if (asm_out_file == 0)
                pfatal_with_name(asm_file_name);
        }

#ifdef IO_BUFFER_SIZE
        setvbuf(asm_out_file, (char *) xmalloc(IO_BUFFER_SIZE),
                _IOFBF, IO_BUFFER_SIZE);
#endif
    }

    input_filename = name;

    /* Put an entry on the input file stack for the main input file.  */
    input_file_stack
        = (struct file_stack *) xmalloc(sizeof (struct file_stack));
    input_file_stack->next = 0;
    input_file_stack->name = input_filename;

    /* This may set main_input_filename.  */
    check_line_directive();

    /* If the input doesn't start with a #line, use the input name
       as the official input file name.  */
    if (main_input_filename == 0)
        main_input_filename = name;

    if (flag_syntax_only)
    {
        write_symbols = NO_DEBUG;
    }
    else
    {
        ASM_FILE_START(asm_out_file);

#ifdef ASM_COMMENT_START
        if (flag_verbose_asm)
        {
            /* Print the list of options in effect.  */
            print_version(asm_out_file, ASM_COMMENT_START);
            print_switch_values(asm_out_file, 0, MAX_LINE,
                                ASM_COMMENT_START, " ", "\n");
            /* Add a blank line here so it appears in assembler output but not
               screen output.  */
            fprintf(asm_out_file, "\n");
        }
#endif

        /* Output something to inform GDB that this compilation was by GCC.  */
#ifndef ASM_IDENTIFY_GCC
        fprintf(asm_out_file, "gcc2_compiled.:\n");
#else
        ASM_IDENTIFY_GCC(asm_out_file);
#endif

        /* Output something to identify which front-end produced this file.  */
#ifdef ASM_IDENTIFY_LANGUAGE
        ASM_IDENTIFY_LANGUAGE(asm_out_file);
#endif
    } /* ! flag_syntax_only */

#ifndef ASM_OUTPUT_SECTION_NAME
    if (flag_function_sections)
    {
        warning("-ffunction-sections not supported for this target.");
        flag_function_sections = 0;
    }
    if (flag_data_sections)
    {
        warning("-fdata-sections not supported for this target.");
        flag_data_sections = 0;
    }
#endif

#ifndef OBJECT_FORMAT_ELF
    if (flag_function_sections && write_symbols != NO_DEBUG)
        warning("-ffunction-sections may affect debugging on some targets.");
#endif

    /* Initialize DWARF-2.  */

#ifdef DWARF2_DEBUGGING_INFO
    if (write_symbols == DWARF2_DEBUG)
        TIMEVAR(symout_time, dwarf2out_init(asm_out_file, main_input_filename));
#endif

    /* Initialize yet another pass.  */

    init_final(main_input_filename);

    start_time = get_run_time();

    /* Call the parser, which parses the entire file
       (calling rest_of_compilation for each function).  */

    if (yyparse() != 0)
    {
        if (errorcount == 0)
            fprintf(stderr, "Errors detected in input file (your bison.simple is out of date)");

        /* In case there were missing closebraces,
           get us back to the global binding level.  */
        while (!global_bindings_p())
            poplevel(0, 0, 0);
    }

    /* Compilation is now finished except for writing
       what's left of the symbol table output.  */

    parse_time += get_run_time() - start_time;

    parse_time -= integration_time;
    parse_time -= varconst_time;

    if (flag_syntax_only)
        goto finish_syntax;

    globals = getdecls();

    /* Really define vars that have had only a tentative definition.
       Really output inline functions that must actually be callable
       and have not been output so far.  */

    {
        int len = list_length(globals);
        tree *vec = (tree *) alloca(sizeof (tree) * len);
        int i;
        tree decl;
        int reconsider = 1;

        /* Process the decls in reverse order--earliest first.
           Put them into VEC from back to front, then take out from front.  */

        for (i = 0, decl = globals; i < len; i++, decl = TREE_CHAIN(decl))
            vec[len - i - 1] = decl;

        for (i = 0; i < len; i++)
        {
            decl = vec[i];

            /* We're not deferring this any longer.  */
            DECL_DEFER_OUTPUT(decl) = 0;

            if (TREE_CODE(decl) == VAR_DECL && DECL_SIZE(decl) == 0
                && incomplete_decl_finalize_hook != 0)
                (*incomplete_decl_finalize_hook) (decl);
        }

        /* Now emit any global variables or functions that we have been putting
           off.  We need to loop in case one of the things emitted here
           references another one which comes earlier in the list.  */
        while (reconsider)
        {
            reconsider = 0;
            for (i = 0; i < len; i++)
            {
                decl = vec[i];

                if (TREE_ASM_WRITTEN(decl) || DECL_EXTERNAL(decl))
                    continue;

                /* Don't write out static consts, unless we still need them.

                   We also keep static consts if not optimizing (for debugging),
                   unless the user specified -fno-keep-static-consts.
                   ??? They might be better written into the debug information.
                   This is possible when using DWARF.

                   A language processor that wants static constants to be always
                   written out (even if it is not used) is responsible for
                   calling rest_of_decl_compilation itself.  E.g. the C front-end
                   calls rest_of_decl_compilation from finish_decl.
                   One motivation for this is that is conventional in some
                   environments to write things like:
                       static const char rcsid[] = "... version string ...";
                   intending to force the string to be in the executable.

                   A language processor that would prefer to have unneeded
                   static constants "optimized away" would just defer writing
                   them out until here.  E.g. C++ does this, because static
                   constants are often defined in header files.

                   ??? A tempting alternative (for both C and C++) would be
                   to force a constant to be written if and only if it is
                   defined in a main file, as opposed to an include file.  */

                if (TREE_CODE(decl) == VAR_DECL && TREE_STATIC(decl)
                    && (!TREE_READONLY(decl)
                        || TREE_PUBLIC(decl)
                        || (!optimize && flag_keep_static_consts)
                        || TREE_SYMBOL_REFERENCED(DECL_ASSEMBLER_NAME(decl))))
                {
                    reconsider = 1;
                    rest_of_decl_compilation(decl, NULL, 1, 1);
                }

                if (TREE_CODE(decl) == FUNCTION_DECL
                    && DECL_INITIAL(decl) != 0
                    && DECL_SAVED_INSNS(decl) != 0
                    && (flag_keep_inline_functions
                        || TREE_PUBLIC(decl)
                        || TREE_SYMBOL_REFERENCED(DECL_ASSEMBLER_NAME(decl))))
                {
                    reconsider = 1;
                    temporary_allocation();
                    output_inline_function(decl);
                    permanent_allocation(1);
                }
            }
        }

        /* Now that all possible functions have been output, we can dump
           the exception table.  */

        output_exception_table();

        for (i = 0; i < len; i++)
        {
            decl = vec[i];

            if (TREE_CODE(decl) == VAR_DECL && TREE_STATIC(decl)
                && !TREE_ASM_WRITTEN(decl))
                /* Cancel the RTL for this decl so that, if debugging info
                   output for global variables is still to come,
                   this one will be omitted.  */
                DECL_RTL(decl) = NULL;

            /* Warn about any function
               declared static but not defined.
               We don't warn about variables,
               because many programs have static variables
               that exist only to get some text into the object file.  */
            if (TREE_CODE(decl) == FUNCTION_DECL
                && (warn_unused
                    || TREE_SYMBOL_REFERENCED(DECL_ASSEMBLER_NAME(decl)))
                && DECL_INITIAL(decl) == 0
                && DECL_EXTERNAL(decl)
                && !DECL_ARTIFICIAL(decl)
                && !TREE_PUBLIC(decl))
            {
                if (TREE_SYMBOL_REFERENCED(DECL_ASSEMBLER_NAME(decl)))
                    pedwarn_with_decl(decl,
                                      "`%s' used but never defined");
                else
                    warning_with_decl(decl,
                                      "`%s' declared `static' but never defined");
                /* This symbol is effectively an "extern" declaration now.  */
                TREE_PUBLIC(decl) = 1;
                assemble_external(decl);
            }

            /* Warn about static fns or vars defined but not used,
               but not about inline functions or static consts
               since defining those in header files is normal practice.  */
            if (warn_unused
                && ((TREE_CODE(decl) == FUNCTION_DECL && !DECL_INLINE(decl))
                    || (TREE_CODE(decl) == VAR_DECL && !TREE_READONLY(decl)))
                && !DECL_IN_SYSTEM_HEADER(decl)
                && !DECL_EXTERNAL(decl)
                && !TREE_PUBLIC(decl)
                && !TREE_USED(decl)
                && (TREE_CODE(decl) == FUNCTION_DECL || !DECL_REGISTER(decl))
                /* The TREE_USED bit for file-scope decls
                   is kept in the identifier, to handle multiple
                   external decls in different scopes.  */
                && !TREE_USED(DECL_NAME(decl)))
                warning_with_decl(decl, "`%s' defined but not used");

#ifdef DWARF2_DEBUGGING_INFO
            /* Output DWARF2 information for file-scope tentative data object
               declarations, file-scope (extern) function declarations (which
               had no corresponding body) and file-scope tagged type declarations
               and definitions which have not yet been forced out.  */

            if (write_symbols == DWARF2_DEBUG
                && (TREE_CODE(decl) != FUNCTION_DECL || !DECL_INITIAL(decl)))
                TIMEVAR(symout_time, dwarf2out_decl(decl));
#endif
        }
    }

    /* Write out any pending weak symbol declarations.  */

    weak_finish();


#ifdef DWARF2_DEBUGGING_INFO
    if (write_symbols == DWARF2_DEBUG)
        TIMEVAR(symout_time,
                {
                    dwarf2out_finish();
                });
#endif

    /* Output some stuff at end of file if nec.  */

    end_final(dump_base_name);

#ifdef ASM_FILE_END
    ASM_FILE_END(asm_out_file);
#endif

finish_syntax:

    /* Close the dump files.  */

    if (flag_gen_aux_info)
    {
        fclose(aux_info_file);
        if (errorcount)
            unlink(aux_info_file_name);
    }

    /* CYGNUS LOCAL v850/law */
    if (flag_gen_offset_info)
    {
        fclose(offset_info_file);
        if (errorcount)
            unlink(offset_info_file_name);
    }
    /* END CYGNUS LOCAL */

    if (combine_dump)
    {
        open_dump_file(".combine", NULL);
        TIMEVAR(dump_time, dump_combine_total_stats(rtl_dump_file));
        close_dump_file(NULL, NULL_RTX);
    }

    /* Close non-debugging input and output files.  Take special care to note
       whether fclose returns an error, since the pages might still be on the
       buffer chain while the file is open.  */

    finish_parse();

    if (!flag_syntax_only
        && (ferror(asm_out_file) != 0 || fclose(asm_out_file) != 0))
        fatal_io_error(asm_file_name);

    /* Do whatever is necessary to finish printing the graphs.  */
    if (graph_dump_format != no_graph)
    {
        if (jump_opt_dump)
            finish_graph_dump_file(dump_base_name, ".jump");
        if (addressof_dump)
            finish_graph_dump_file(dump_base_name, ".addressof");
        if (cse_dump)
            finish_graph_dump_file(dump_base_name, ".cse");
        if (loop_dump)
            finish_graph_dump_file(dump_base_name, ".loop");
        if (cse2_dump)
            finish_graph_dump_file(dump_base_name, ".cse2");
        if (flow_dump)
            finish_graph_dump_file(dump_base_name, ".flow");
        if (combine_dump)
            finish_graph_dump_file(dump_base_name, ".combine");
        if (regmove_dump)
            finish_graph_dump_file(dump_base_name, ".regmove");
        if (local_reg_dump)
            finish_graph_dump_file(dump_base_name, ".lreg");
        if (global_reg_dump)
            finish_graph_dump_file(dump_base_name, ".greg");
        if (jump2_opt_dump)
            finish_graph_dump_file(dump_base_name, ".jump2");
        if (gcse_dump)
            finish_graph_dump_file(dump_base_name, ".gcse");
        if (mach_dep_reorg_dump)
            finish_graph_dump_file(dump_base_name, ".mach");
    }

    /* Free up memory for the benefit of leak detectors.  */
    free_reg_info();

    /* Print the times.  */

    if (loud_flag)
    {
        fprintf(stderr,"\n");
        print_time("parse", parse_time);
        print_time("integration", integration_time);
        print_time("jump", jump_time);
        print_time("cse", cse_time);
        print_time("gcse", gcse_time);
        print_time("loop", loop_time);
        print_time("cse2", cse2_time);
        print_time("flow", flow_time);
        print_time("combine", combine_time);
        print_time("regmove", regmove_time);
        print_time("local-alloc", local_alloc_time);
        print_time("global-alloc", global_alloc_time);
        print_time("shorten-branch", shorten_branch_time);
        print_time("stack-reg", stack_reg_time);
        print_time("final", final_time);
        print_time("varconst", varconst_time);
        print_time("symout", symout_time);
        print_time("dump", dump_time);
    }
}

/* This is called from various places for FUNCTION_DECL, VAR_DECL,
   and TYPE_DECL nodes.

   This does nothing for local (non-static) variables.
   Otherwise, it sets up the RTL and outputs any assembler code
   (label definition, storage allocation and initialization).

   DECL is the declaration.  If ASMSPEC is nonzero, it specifies
   the assembler symbol name to be used.  TOP_LEVEL is nonzero
   if this declaration is not within a function.  */

void
rest_of_decl_compilation(tree decl, char *asmspec, int top_level, int at_end)
{
    /* Declarations of variables, and of functions defined elsewhere.  */

/* The most obvious approach, to put an #ifndef around where
   this macro is used, doesn't work since it's inside a macro call.  */
#ifndef ASM_FINISH_DECLARE_OBJECT
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP, END)
#endif

    /* Forward declarations for nested functions are not "external",
       but we need to treat them as if they were.  */
    if (TREE_STATIC(decl) || DECL_EXTERNAL(decl)
        || TREE_CODE(decl) == FUNCTION_DECL)
        TIMEVAR(varconst_time,
                {
                    make_decl_rtl(decl, asmspec, top_level);
                    /* Initialized extern variable exists to be replaced
                       with its value, or represents something that will be
                       output in another file.  */
                    if (!(TREE_CODE(decl) == VAR_DECL
                          && DECL_EXTERNAL(decl) && TREE_READONLY(decl)
                          && DECL_INITIAL(decl) != 0
                          && DECL_INITIAL(decl) != error_mark_node))
                        /* Don't output anything
                           when a tentative file-scope definition is seen.
                           But at end of compilation, do output code for them.  */
                        if (!(!at_end && top_level
                              && (DECL_INITIAL(decl) == 0
                                  || DECL_INITIAL(decl) == error_mark_node)))
                            assemble_variable(decl, top_level, at_end, 0);
                    if (decl == last_assemble_variable_decl)
                    {
                        ASM_FINISH_DECLARE_OBJECT(asm_out_file, decl,
                                                  top_level, at_end);
                    }
                });
    else if (DECL_REGISTER(decl) && asmspec != 0)
    {
        if (decode_reg_name(asmspec) >= 0)
        {
            DECL_RTL(decl) = 0;
            make_decl_rtl(decl, asmspec, top_level);
        }
        else
            error("invalid register name `%s' for register variable", asmspec);
    }
}

/* Called after finishing a record, union or enumeral type.  */

void
rest_of_type_compilation(tree type ATTRIBUTE_UNUSED, int toplev ATTRIBUTE_UNUSED)
{
}

/* This is called from finish_function (within yyparse)
   after each top-level definition is parsed.
   It is supposed to compile that function or variable
   and output the assembler code for it.
   After we return, the tree storage is freed.  */

void
rest_of_compilation(tree decl)
{
    rtx insns;
    int start_time = get_run_time();
    int tem;
    /* Nonzero if we have saved the original DECL_INITIAL of the function,
       to be restored after we finish compiling the function
       (for use when compiling inline calls to this function).  */
    tree saved_block_tree = 0;
    /* Likewise, for DECL_ARGUMENTS.  */
    tree saved_arguments = 0;
    int failure = 0;

    /* If we are reconsidering an inline function
       at the end of compilation, skip the stuff for making it inline.  */

    if (DECL_SAVED_INSNS(decl) == 0)
    {
        int inlinable = 0;
        char *lose;

        /* If requested, consider whether to make this function inline.  */
        if (DECL_INLINE(decl) || flag_inline_functions)
            TIMEVAR(integration_time,
                    {
                        lose = function_cannot_inline_p(decl);
                        if (lose || !optimize)
                        {
                            if (warn_inline && DECL_INLINE(decl))
                                warning_with_decl(decl, lose);
                            DECL_ABSTRACT_ORIGIN(decl) = 0;
                            /* Don't really compile an extern inline function.
                               If we can't make it inline, pretend
                               it was only declared.  */
                            if (DECL_EXTERNAL(decl))
                            {
                                DECL_INITIAL(decl) = 0;
                                goto exit_rest_of_compilation;
                            }
                        }
                        else
                            /* ??? Note that this has the effect of making it look
                               like "inline" was specified for a function if we choose
                               to inline it.  This isn't quite right, but it's
                               probably not worth the trouble to fix.  */
                            inlinable = DECL_INLINE(decl) = 1;
                    });

        insns = get_insns();

        /* Dump the rtl code if we are dumping rtl.  */
        if (rtl_dump)
        {
            open_dump_file(".rtl", decl_printable_name(decl, 2));

            if (DECL_SAVED_INSNS(decl))
                fprintf(rtl_dump_file, ";; (integrable)\n\n");

            close_dump_file(print_rtl, insns);
        }

        /* If we can, defer compiling inlines until EOF.
           save_for_inline_copying can be extremely expensive.  */
        if (inlinable && !decl_function_context(decl))
            DECL_DEFER_OUTPUT(decl) = 1;

        /* If function is inline, and we don't yet know whether to
           compile it by itself, defer decision till end of compilation.
           finish_compilation will call rest_of_compilation again
           for those functions that need to be output.  Also defer those
           functions that we are supposed to defer.  We cannot defer
           functions containing nested functions since the nested function
           data is in our non-saved obstack.  We cannot defer nested
           functions for the same reason.  */

        /* If this is a nested inline, remove ADDRESSOF now so we can
           finish compiling ourselves.  Otherwise, wait until EOF.
           We have to do this because the purge_addressof transformation
           changes the DECL_RTL for many variables, which confuses integrate.  */
        if (inlinable)
        {
            if (decl_function_context(decl))
                purge_addressof(insns);
            else
                DECL_DEFER_OUTPUT(decl) = 1;
        }

        if (!current_function_contains_functions
            && (DECL_DEFER_OUTPUT(decl)
                || (DECL_INLINE(decl)
                    && ((!TREE_PUBLIC(decl) && !TREE_ADDRESSABLE(decl)
                         && !flag_keep_inline_functions)
                        || DECL_EXTERNAL(decl)))))
        {
            DECL_DEFER_OUTPUT(decl) = 1;

            /* If -Wreturn-type, we have to do a bit of compilation.
               However, if we just fall through we will call
               save_for_inline_copying() which results in excessive
               memory use.  Instead, we just want to call
               jump_optimize() to figure out whether or not we can fall
               off the end of the function; we do the minimum amount of
               work necessary to make that safe.  And, we set optimize
               to zero to keep jump_optimize from working too hard.  */
            if (warn_return_type)
            {
                int saved_optimize = optimize;
                optimize = 0;
                find_exception_handler_labels();
                jump_optimize(get_insns(), !JUMP_CROSS_JUMP, !JUMP_NOOP_MOVES,
                              !JUMP_AFTER_REGSCAN);
                optimize = saved_optimize;
            }

#ifdef DWARF2_DEBUGGING_INFO
            /* Generate the DWARF2 info for the "abstract" instance
               of a function which we may later generate inlined and/or
               out-of-line instances of.  */
            if (write_symbols == DWARF2_DEBUG)
            {
                set_decl_abstract_flags(decl, 1);
                TIMEVAR(symout_time, dwarf2out_decl(decl));
                set_decl_abstract_flags(decl, 0);
            }
#endif
            TIMEVAR(integration_time, save_for_inline_nocopy(decl));
            RTX_INTEGRATED_P(DECL_SAVED_INSNS(decl)) = inlinable;
            goto exit_rest_of_compilation;
        }

        /* If we have to compile the function now, save its rtl and subdecls
           so that its compilation will not affect what others get.  */
        if (inlinable || DECL_DEFER_OUTPUT(decl))
        {
#ifdef DWARF2_DEBUGGING_INFO
            /* Generate the DWARF2 info for the "abstract" instance of
               a function which we will generate an out-of-line instance
               of almost immediately (and which we may also later generate
               various inlined instances of).  */
            if (write_symbols == DWARF2_DEBUG)
            {
                set_decl_abstract_flags(decl, 1);
                TIMEVAR(symout_time, dwarf2out_decl(decl));
                set_decl_abstract_flags(decl, 0);
            }
#endif
            saved_block_tree = DECL_INITIAL(decl);
            saved_arguments = DECL_ARGUMENTS(decl);
            TIMEVAR(integration_time, save_for_inline_copying(decl));
            RTX_INTEGRATED_P(DECL_SAVED_INSNS(decl)) = inlinable;
        }

        /* If specified extern inline but we aren't inlining it, we are
           done.  This goes for anything that gets here with DECL_EXTERNAL
           set, not just things with DECL_INLINE.  */
        if (DECL_EXTERNAL(decl))
            goto exit_rest_of_compilation;
    }

    if (!DECL_DEFER_OUTPUT(decl))
        TREE_ASM_WRITTEN(decl) = 1;

    /* Now that integrate will no longer see our rtl, we need not distinguish
       between the return value of this function and the return value of called
       functions.  */
    rtx_equal_function_value_matters = 0;

    /* Don't return yet if -Wreturn-type; we need to do jump_optimize.  */
    if ((rtl_dump_and_exit || flag_syntax_only) && !warn_return_type)
    {
        goto exit_rest_of_compilation;
    }

    /* Emit code to get eh context, if needed. */
    emit_eh_context();

    /* From now on, allocate rtl in current_obstack, not in saveable_obstack.
       Note that that may have been done above, in save_for_inline_copying.
       The call to resume_temporary_allocation near the end of this function
       goes back to the usual state of affairs.  This must be done after
       we've built up any unwinders for exception handling, and done
       the FINALIZE_PIC work, if necessary.  */

    rtl_in_current_obstack();

    insns = get_insns();

    /* Copy any shared structure that should not be shared.  */

    unshare_all_rtl(insns);

#ifdef SETJMP_VIA_SAVE_AREA
    /* This must be performed before virutal register instantiation.  */
    if (current_function_calls_alloca)
        optimize_save_area_alloca(insns);
#endif

    /* Instantiate all virtual registers.  */

    instantiate_virtual_regs(current_function_decl, get_insns());

    /* See if we have allocated stack slots that are not directly addressable.
       If so, scan all the insns and create explicit address computation
       for all references to such slots.  */
/*   fixup_stack_slots (); */

    /* Find all the EH handlers.  */
    find_exception_handler_labels();

    /* Always do one jump optimization pass to ensure that JUMP_LABEL fields
       are initialized and to compute whether control can drop off the end
       of the function.  */
    TIMEVAR(jump_time, reg_scan(insns, max_reg_num(), 0));
    TIMEVAR(jump_time, jump_optimize(insns, !JUMP_CROSS_JUMP, !JUMP_NOOP_MOVES,
                                     JUMP_AFTER_REGSCAN));

    /* CYGNUS LOCAL law */
    if (optimize > 1)
    {
        TIMEVAR(jump_time, delete_null_pointer_checks(get_insns()));
        TIMEVAR(jump_time, merge_blocks(insns));
    }
    /* END CYGNUS LOCAL */

    /* Now is when we stop if -fsyntax-only and -Wreturn-type.  */
    if (rtl_dump_and_exit || flag_syntax_only || DECL_DEFER_OUTPUT(decl))
        goto exit_rest_of_compilation;

    /* Dump rtl code after jump, if we are doing that.  */

    if (jump_opt_dump)
        dump_rtl(".jump", decl, print_rtl, insns);

    /* Perform common subexpression elimination.
       Nonzero value from `cse_main' means that jumps were simplified
       and some code may now be unreachable, so do
       jump optimization again.  */

    if (optimize > 0)
    {
        if (cse_dump)
            open_dump_file(".cse", decl_printable_name(decl, 2));

        TIMEVAR(cse_time, reg_scan(insns, max_reg_num(), 1));

        if (flag_thread_jumps)
            /* Hacks by tiemann & kenner.  */
            TIMEVAR(jump_time, thread_jumps(insns, max_reg_num(), 1));

        TIMEVAR(cse_time, tem = cse_main(insns, max_reg_num(),
                                         0, rtl_dump_file));
        TIMEVAR(cse_time, delete_trivially_dead_insns(insns, max_reg_num()));

        /* CYGNUS LOCAL law */
        if (tem || optimize > 1)
        {
            TIMEVAR(jump_time, jump_optimize(insns, !JUMP_CROSS_JUMP,
                                             !JUMP_NOOP_MOVES,
                                             !JUMP_AFTER_REGSCAN));
            if (optimize > 1)
            {
                TIMEVAR(jump_time, delete_null_pointer_checks(get_insns()));
                TIMEVAR(jump_time, merge_blocks(insns));
            }
        }
        /* END CYGNUS LOCAL */

        /* Dump rtl code after cse, if we are doing that.  */

        if (cse_dump)
        {
            close_dump_file(print_rtl, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".cse", insns);
        }
    }

    purge_addressof(insns);
    reg_scan(insns, max_reg_num(), 1);

    if (addressof_dump)
    {
        dump_rtl(".addressof", decl, print_rtl, insns);
        if (graph_dump_format != no_graph)
            print_rtl_graph_with_bb(dump_base_name, ".addressof", insns);
    }

    /* Perform global cse.  */

    if (optimize > 0 && flag_gcse)
    {
        if (gcse_dump)
            open_dump_file(".gcse", IDENTIFIER_POINTER(DECL_NAME(decl)));

        /* CYGNUS LOCAL edge splitting/law */
        TIMEVAR(gcse_time, tem = gcse_main(insns, rtl_dump_file));

        /* If gcse altered any jumps, rerun jump optimizations to clean
           things up.  */
        if (tem)
        {
            TIMEVAR(jump_time, jump_optimize(insns, !JUMP_CROSS_JUMP,
                                             !JUMP_NOOP_MOVES,
                                             !JUMP_AFTER_REGSCAN));
            if (optimize > 1)
                TIMEVAR(jump_time, merge_blocks(insns));
        }
        /* END CYGNUS LOCAL */


        if (gcse_dump)
        {
            close_dump_file(print_rtl, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".gcse", insns);
        }
    }

    /* Move constant computations out of loops.  */

    if (optimize > 0)
    {
        if (loop_dump)
            open_dump_file(".loop", decl_printable_name(decl, 2));

        TIMEVAR
            (loop_time,
            {
                if (flag_rerun_loop_opt)
                {
                    /* We only want to perform unrolling once.  */

                    loop_optimize(insns, rtl_dump_file, 0, 0);


                    /* The first call to loop_optimize makes some instructions
                       trivially dead.  We delete those instructions now in the
                       hope that doing so will make the heuristics in loop work
                       better and possibly speed up compilation.  */
                    delete_trivially_dead_insns(insns, max_reg_num());

                    /* The regscan pass is currently necessary as the alias
                       analysis code depends on this information.  */
                    reg_scan(insns, max_reg_num(), 1);
                }
                loop_optimize(insns, rtl_dump_file, flag_unroll_loops, 1);
            });

        /* Dump rtl code after loop opt, if we are doing that.  */

        if (loop_dump)
        {
            close_dump_file(print_rtl, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".loop", insns);
        }
    }

    if (optimize > 0)
    {
        if (cse2_dump)
            open_dump_file(".cse2", decl_printable_name(decl, 2));

        if (flag_rerun_cse_after_loop)
        {
            /* Running another jump optimization pass before the second
               cse pass sometimes simplifies the RTL enough to allow
               the second CSE pass to do a better job.  Jump_optimize can change
               max_reg_num so we must rerun reg_scan afterwards.
               ??? Rework to not call reg_scan so often.  */
            TIMEVAR(jump_time, reg_scan(insns, max_reg_num(), 0));
            TIMEVAR(jump_time, jump_optimize(insns, !JUMP_CROSS_JUMP,
                                             !JUMP_NOOP_MOVES,
                                             JUMP_AFTER_REGSCAN));

            TIMEVAR(cse2_time, reg_scan(insns, max_reg_num(), 0));
            TIMEVAR(cse2_time, tem = cse_main(insns, max_reg_num(),
                                              1, rtl_dump_file));
            /* CYGNUS LOCAL law */
            if (tem)
            {
                TIMEVAR(jump_time, jump_optimize(insns, !JUMP_CROSS_JUMP,
                                                 !JUMP_NOOP_MOVES,
                                                 !JUMP_AFTER_REGSCAN));
                if (optimize > 1)
                    TIMEVAR(jump_time, merge_blocks(insns));
            }
            /* END CYGNUS LOCAL */
        }

        if (flag_thread_jumps)
        {
            /* This pass of jump threading straightens out code
               that was kinked by loop optimization.  */
            TIMEVAR(jump_time, reg_scan(insns, max_reg_num(), 0));
            TIMEVAR(jump_time, thread_jumps(insns, max_reg_num(), 0));
        }

        /* Dump rtl code after cse, if we are doing that.  */

        if (cse2_dump)
        {
            close_dump_file(print_rtl, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".cse2", insns);
        }
    }

    /* We are no longer anticipating cse in this function, at least.  */

    cse_not_expected = 1;

    /* Now we choose between stupid (pcc-like) register allocation
       (if we got the -noreg switch and not -opt)
       and smart register allocation.  */

    if (optimize > 0)           /* Stupid allocation probably won't work */
        obey_regdecls = 0;      /* if optimizations being done.  */

    regclass_init();

    /* Print function header into flow dump now
       because doing the flow analysis makes some of the dump.  */
    if (flow_dump)
        open_dump_file(".flow", decl_printable_name(decl, 2));

    if (obey_regdecls)
    {
        TIMEVAR(flow_time,
                {
                    regclass(insns, max_reg_num());
                    stupid_life_analysis(insns, max_reg_num(),
                                         rtl_dump_file);
                });
    }
    else
    {
        /* Do control and data flow analysis,
           and write some of the results to dump file.  */

        TIMEVAR
            (flow_time,
            {
                find_basic_blocks(insns, max_reg_num(), rtl_dump_file);
                life_analysis(insns, max_reg_num(), rtl_dump_file);
            });

        if (warn_uninitialized)
        {
            uninitialized_vars_warning(DECL_INITIAL(decl));
            setjmp_args_warning();
        }
    }

    /* Dump rtl after flow analysis.  */
    if (flow_dump)
    {
        close_dump_file(print_rtl_with_bb, insns);
        if (graph_dump_format != no_graph)
            print_rtl_graph_with_bb(dump_base_name, ".flow", insns);
    }

    /* The first life analysis pass has finished.  From now on we can not
       generate any new pseudos.  */
    no_new_pseudos = 1;

    /* If -opt, try combining insns through substitution.  */

    if (optimize > 0)
    {
        TIMEVAR(combine_time, combine_instructions(insns, max_reg_num()));

        /* Dump rtl code after insn combination.  */

        if (combine_dump)
        {
            dump_rtl(".combine", decl, print_rtl_with_bb, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".combine", insns);
        }
    }

    /* Register allocation pre-pass, to reduce number of moves
       necessary for two-address machines.  */
    if (optimize > 0 && (flag_regmove || flag_expensive_optimizations))
    {
        if (regmove_dump)
            open_dump_file(".regmove", decl_printable_name(decl, 2));

        TIMEVAR(regmove_time, regmove_optimize(insns, max_reg_num(),
                                               rtl_dump_file));

        if (regmove_dump)
        {
            close_dump_file(print_rtl_with_bb, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".regmove", insns);
        }
    }

    /* Unless we did stupid register allocation,
       allocate pseudo-regs that are used only within 1 basic block.  */

    if (!obey_regdecls)
        TIMEVAR(local_alloc_time,
                {
                    recompute_reg_usage(insns, !optimize_size);
                    regclass(insns, max_reg_num());
                    local_alloc();
                });

    /* Dump rtl code after allocating regs within basic blocks.  */

    if (local_reg_dump)
    {
        open_dump_file(".lreg", decl_printable_name(decl, 2));

        TIMEVAR(dump_time, dump_flow_info(rtl_dump_file));
        TIMEVAR(dump_time, dump_local_alloc(rtl_dump_file));

        close_dump_file(print_rtl_with_bb, insns);
        if (graph_dump_format != no_graph)
            print_rtl_graph_with_bb(dump_base_name, ".lreg", insns);
    }

    if (global_reg_dump)
        open_dump_file(".greg", decl_printable_name(decl, 2));

    /* Unless we did stupid register allocation,
       allocate remaining pseudo-regs, then do the reload pass
       fixing up any insns that are invalid.  */

    TIMEVAR(global_alloc_time,
            {
                if (!obey_regdecls)
                    failure = global_alloc(rtl_dump_file);
                else
                    failure = reload(insns, 0, rtl_dump_file);
            });


    if (failure)
        goto exit_rest_of_compilation;

    /* Do a very simple CSE pass over just the hard registers.  */
    if (optimize > 0)
        reload_cse_regs(insns);

    /* Re-create the death notes which were deleted during reload.  */
    if (optimize)
        TIMEVAR
            (flow_time,
            {
                find_basic_blocks(insns, max_reg_num(), rtl_dump_file);
                life_analysis(insns, max_reg_num(), rtl_dump_file);
            });

    flow2_completed = 1;

    /* On some machines, the prologue and epilogue code, or parts thereof,
       can be represented as RTL.  Doing so lets us schedule insns between
       it and the rest of the code and also allows delayed branch
       scheduling to operate in the epilogue.  */

    thread_prologue_and_epilogue_insns(insns);

    if (global_reg_dump)
    {
        TIMEVAR(dump_time, dump_global_regs(rtl_dump_file));
        close_dump_file(print_rtl_with_bb, insns);
        if (graph_dump_format != no_graph)
            print_rtl_graph_with_bb(dump_base_name, ".greg", insns);
    }

    /* One more attempt to remove jumps to .+1
       left by dead-store-elimination.
       Also do cross-jumping this time
       and delete no-op move insns.  */

    if (optimize > 0)
    {
        TIMEVAR(jump_time, jump_optimize(insns, JUMP_CROSS_JUMP,
                                         JUMP_NOOP_MOVES,
                                         !JUMP_AFTER_REGSCAN));

        /* Dump rtl code after jump, if we are doing that.  */

        if (jump2_opt_dump)
        {
            dump_rtl(".jump2", decl, print_rtl_with_bb, insns);
            if (graph_dump_format != no_graph)
                print_rtl_graph_with_bb(dump_base_name, ".jump2", insns);
        }
    }

    /* Do machine-dependent reorganization.  */
    MACHINE_DEPENDENT_REORG(insns);

    if (mach_dep_reorg_dump)
    {
        dump_rtl(".mach", decl, print_rtl_with_bb, insns);
        if (graph_dump_format != no_graph)
            print_rtl_graph_with_bb(dump_base_name, ".mach", insns);
    }

    /* Shorten branches.  */
    TIMEVAR(shorten_branch_time,
            {
                shorten_branches(get_insns());
            });


    /* Now turn the rtl into assembler code.  */

    TIMEVAR(final_time,
            {
                rtx x;
                char *fnname;

                /* Get the function's name, as described by its RTL.
                   This may be different from the DECL_NAME name used
                   in the source file.  */

                x = DECL_RTL(decl);
                if (GET_CODE(x) != MEM)
                    abort();
                x = XEXP(x, 0);
                if (GET_CODE(x) != SYMBOL_REF)
                    abort();
                fnname = XSTR(x, 0);

                assemble_start_function(decl, fnname);
                final_start_function(insns, asm_out_file, optimize);
                final(insns, asm_out_file, optimize, 0);
                final_end_function(insns, asm_out_file, optimize);
                assemble_end_function(decl, fnname);
                if (loud_flag)
                    fflush(asm_out_file);

                /* Release all memory held by regsets now */
                regset_release_memory();
            });

    /* Write debug symbols if requested */

    /* Note that for those inline functions where we don't initially
       know for certain that we will be generating an out-of-line copy,
       the first invocation of this routine (rest_of_compilation) will
       skip over this code by doing a `goto exit_rest_of_compilation;'.
       Later on, finish_compilation will call rest_of_compilation again
       for those inline functions that need to have out-of-line copies
       generated.  During that call, we *will* be routed past here.  */

#ifdef DWARF2_DEBUGGING_INFO
    if (write_symbols == DWARF2_DEBUG)
        TIMEVAR(symout_time, dwarf2out_decl(decl));
#endif

exit_rest_of_compilation:

    free_bb_memory();

    /* In case the function was not output,
       don't leave any temporary anonymous types
       queued up for sdb output.  */

    /* Put back the tree of subblocks and list of arguments
       from before we copied them.
       Code generation and the output of debugging info may have modified
       the copy, but the original is unchanged.  */

    if (saved_block_tree != 0)
    {
        DECL_INITIAL(decl) = saved_block_tree;
        DECL_ARGUMENTS(decl) = saved_arguments;
        DECL_ABSTRACT_ORIGIN(decl) = NULL_TREE;
    }

    reload_completed = 0;
    flow2_completed = 0;
    no_new_pseudos = 0;

    TIMEVAR(final_time,
            {
                /* CYGNUS LOCAL LRS */
                /* Free storage allocated by find_basic_blocks.  */
                free_basic_block_vars(0);
                /* END CYGNUS LOCAL */

                /* Clear out the insn_length contents now that they are no
                   longer valid.  */
                init_insn_lengths();

                /* Clear out the real_constant_chain before some of the rtx's
                   it runs through become garbage.  */
                clear_const_double_mem();

                /* Cancel the effect of rtl_in_current_obstack.  */
                resume_temporary_allocation();

                /* Show no temporary slots allocated.  */
                init_temp_slots();

                /* Release all memory held by regsets.  */
                regset_release_memory();
                /* END CYGNUS LOCAL */
            });

    /* Make sure volatile mem refs aren't considered valid operands for
       arithmetic insns.  We must call this here if this is a nested inline
       function, since the above code leaves us in the init_recog state
       (from final.c), and the function context push/pop code does not
       save/restore volatile_ok.

       ??? Maybe it isn't necessary for expand_start_function to call this
       anymore if we do it here?  */

    init_recog_no_volatile();

    /* The parsing time is all the time spent in yyparse
     * except* what is spent in this function.  */

    parse_time -= get_run_time() - start_time;

    /* Reset global variables.  */
    free_basic_block_vars(0);
}

static void
display_help()
{
    int undoc;
    unsigned long i;
    char * lang;

    printf("Usage: %s input [switches]\n", progname);
    printf("Switches:\n");
    printf("  -ffixed-<register>      Mark <register> as being unavailable to the compiler\n");
    printf("  -fcall-used-<register>  Mark <register> as being corrupted by function calls\n");
    printf("  -fcall-saved-<register> Mark <register> as being preserved across functions\n");

    for (i = NUM_ELEM(f_options); i--; )
    {
        char * description = f_options[i].description;

        if (description != NULL && *description != 0)
            printf("  -f%-21s %s\n",
                   f_options[i].string, description);
    }

    printf("  -O[number]              Set optimisation level to [number]\n");
    printf("  -Os                     Optimise for space rather than speed\n");
    printf("  -pedantic               Issue warnings needed by strict compliance to ANSI C\n");
    printf("  -pedantic-errors        Like -pedantic except that errors are produced\n");
    printf("  -w                      Suppress warnings\n");
    printf("  -W                      Enable extra warnings\n");

    for (i = NUM_ELEM(W_options); i--; )
    {
        char * description = W_options[i].description;

        if (description != NULL && *description != 0)
            printf("  -W%-21s %s\n",
                   W_options[i].string, description);
    }

    printf("  -Wid-clash-<num>        Warn if 2 identifiers have the same first <num> chars\n");
    printf("  -Wlarger-than-<number>  Warn if an object is larger than <number> bytes\n");
    printf("  -o <file>               Place output into <file> \n");
    printf("  -g                      Enable debug output\n");
    printf("  -aux-info <file>        Emit declaration info into <file>.X\n");
    /* CYGNUS LOCAL v850/law */
    printf("  -offset-info <file>     Emit structure member offsets into <file>.s\n");
    /* CYGNUS LOCAL v850/law */
    printf("  -quiet                  Do not display functions compiled or elapsed time\n");
    printf("  -version                Display the compiler's version\n");
    printf("  -d[letters]             Enable dumps from specific passes of the compiler\n");
    printf("  -dumpbase <file>        Base name to be used for dumps from specific passes\n");
    printf("  --help                  Display this information\n");

    undoc = 0;
    lang  = "language";

    /* Display descriptions of language specific options.
       If there is no description, note that there is an undocumented option.
       If the description is empty, do not display anything.  (This allows
       options to be deliberately undocumented, for whatever reason).
       If the option string is missing, then this is a marker, indicating
       that the description string is in fact the name of a language, whose
       language specific options are to follow.  */

    if (NUM_ELEM(documented_lang_options) > 1)
    {
        printf("\nLanguage specific options:\n");

        for (i = 0; i < NUM_ELEM(documented_lang_options); i++)
        {
            char * description = documented_lang_options[i].description;
            char * option      = documented_lang_options[i].option;

            if (description == NULL)
            {
                undoc = 1;

                if (extra_warnings)
                    printf("  %-23.23s [undocumented]\n", option);
            }
            else if (*description == 0)
                continue;
            else if (option == NULL)
            {
                if (undoc)
                    printf
                        ("\nThere are undocumented %s specific options as well.\n",
                        lang);
                undoc = 0;

                printf("\n Options for %s:\n", description);

                lang = description;
            }
            else
                printf("  %-23.23s %s\n", option, description);
        }
    }

    if (undoc)
        printf("\nThere are undocumented %s specific options as well.\n", lang);

    if (NUM_ELEM(target_switches) > 1
#ifdef TARGET_OPTIONS
        || NUM_ELEM(target_options) > 1
#endif
        )
    {
        int doc = 0;

        undoc = 0;

        printf("\nTarget specific options:\n");

        for (i = NUM_ELEM(target_switches); i--; )
        {
            char * option      = target_switches[i].name;
            char * description = target_switches[i].description;

            if (option == NULL || *option == 0)
                continue;
            else if (description == NULL)
            {
                undoc = 1;

                if (extra_warnings)
                    printf("  -m%-21.21s [undocumented]\n", option);
            }
            else if (*description != 0)
                doc += printf("  -m%-21.21s %s\n", option, description);
        }

#ifdef TARGET_OPTIONS
        for (i = NUM_ELEM(target_options); i--; )
        {
            char * option      = target_options[i].prefix;
            char * description = target_options[i].description;

            if (option == NULL || *option == 0)
                continue;
            else if (description == NULL)
            {
                undoc = 1;

                if (extra_warnings)
                    printf("  -m%-21.21s [undocumented]\n", option);
            }
            else if (*description != 0)
                doc += printf("  -m%-21.21s %s\n", option, description);
        }
#endif
        if (undoc)
        {
            if (doc)
                printf("\nThere are undocumented target specific options as well.\n");
            else
                printf("  They exist, but they are not documented.\n");
        }
    }
}

/* Compare the user specified 'option' with the language
   specific 'lang_option'.  Return true if they match, or
   if 'option' is a viable prefix of 'lang_option'.  */

static int
check_lang_option(char *option, char *lang_option)
{
    lang_independent_options * indep_options;
    int len;
    long k;
    char * space;

    /* Ignore NULL entries.  */
    if (option == NULL || lang_option == NULL)
        return 0;

    if ((space = strchr(lang_option, ' ')) != NULL)
        len = space - lang_option;
    else
        len = strlen(lang_option);

    /* If they do not match to the first n characters then fail.  */
    if (strncmp(option, lang_option, len) != 0)
        return 0;

    /* Do not accept a lang option, if it matches a normal -f or -W
       option.  Chill defines a -fpack, but we want to support
       -fpack-struct.  */

    /* An exact match is OK  */
    if ((int) strlen(option) == len)
        return 1;

    /* If it is not an -f or -W option allow the match */
    if (option[0] != '-')
        return 1;

    switch (option[1])
    {
    case 'f': indep_options = f_options; break;
    case 'W': indep_options = W_options; break;
    default:  return 1;
    }

    /* The option is a -f or -W option.
       Skip past the prefix and search for the remainder in the
       appropriate table of options.  */
    option += 2;

    if (option[0] == 'n' && option[1] == 'o' && option[2] == '-')
        option += 3;

    for (k = NUM_ELEM(indep_options); k--; )
    {
        if (!strcmp(option, indep_options[k].string))
        {
            /* The option matched a language independent option,
               do not allow the language specific match.  */

            return 0;
        }
    }

    /* The option matches the start of the langauge specific option
       and it is not an exact match for a language independent option.  */
    return 1;
}

/* Entry point of cc1/c++.  Decode command args, then call compile_file.
   Exit code is 35 if can't open files, 34 if fatal error,
   33 if had nonfatal errors, else success.  */

int
main(int argc, char **argv)
{
    int i;
    char *filename = 0;
    int version_flag = 0;
    char *p;

    /* save in case md file wants to emit args as a comment.  */
    save_argc = argc;
    save_argv = argv;

    p = argv[0] + strlen(argv[0]);
    while (p != argv[0] && p[-1] != '/'
#ifdef DIR_SEPARATOR
           && p[-1] != DIR_SEPARATOR
#endif
           )
        --p;
    progname = p;

    signal(SIGFPE, float_signal);

#ifdef SIGPIPE
    signal(SIGPIPE, pipe_closed);
#endif

    decl_printable_name = decl_name;
    lang_expand_expr = (lang_expand_expr_t) do_abort;

#ifdef DEFAULT_SHORT_ENUMS
    /* Initialize how much space enums occupy, by default.  */
    flag_short_enums = DEFAULT_SHORT_ENUMS;
#endif

    /* Scan to see what optimization level has been specified.  That will
       determine the default value of many flags.  */
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-O"))
        {
            optimize = 1;
            optimize_size = 0;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'O')
        {
            /* Handle -Os, -O2, -O3, -O69, ...  */
            char *p = &argv[i][2];
            int c;

            if ((p[0] == 's') && (p[1] == 0))
            {
                optimize_size = 1;

                /* Optimizing for size forces optimize to be 2. */
                optimize = 2;
            }
            else
            {
                while ((c = *p++))
                    if (!(c >= '0' && c <= '9'))
                        break;
                if (c == 0)
                {
                    optimize = atoi(&argv[i][2]);
                    optimize_size = 0;
                }
            }
        }
    }

    obey_regdecls = (optimize == 0);

    if (optimize >= 1)
    {
        flag_defer_pop = 1;
        flag_thread_jumps = 1;
#ifdef CAN_DEBUG_WITHOUT_FP
        flag_omit_frame_pointer = 1;
#endif
    }

    if (optimize >= 2)
    {
        flag_cse_follow_jumps = 1;
        flag_cse_skip_blocks = 1;
        flag_gcse = 1;
        flag_expensive_optimizations = 1;
        flag_strength_reduce = 1;
        flag_rerun_cse_after_loop = 1;
        flag_rerun_loop_opt = 1;
        flag_caller_saves = 1;
        flag_force_mem = 1;
        flag_regmove = 1;
        flag_strict_aliasing = 1;
    }

    if (optimize >= 3)
    {
        flag_inline_functions = 1;
    }

    /* Initialize target_flags before OPTIMIZATION_OPTIONS so the latter can
       modify it.  */
    target_flags = 0;
    set_target_switch("");

#ifdef OPTIMIZATION_OPTIONS
    /* Allow default optimizations to be specified on a per-machine basis.  */
    OPTIMIZATION_OPTIONS(optimize, optimize_size);
#endif

    /* Initialize register usage now so switches may override.  */
    init_reg_sets();

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            display_help();
            exit(0);
        }

        size_t j;

        /* If this is a language-specific option,
           decode it in a language-specific way.  */
        for (j = NUM_ELEM(documented_lang_options); j--; )
            if (check_lang_option(argv[i], documented_lang_options[j].option))
                break;

        if (j != (size_t)-1)
        {
            c_decode_option(argv[i]);
        }
        else if (argv[i][0] == '-' && argv[i][1] != 0)
        {
            char *str = argv[i] + 1;
            if (str[0] == 'Y')
                str++;

            if (str[0] == 'm')
                set_target_switch(&str[1]);
            else if (!strcmp(str, "dumpbase"))
            {
                dump_base_name = argv[++i];
            }
            else if (str[0] == 'd')
            {
                char *p = &str[1];
                while (*p)
                    switch (*p++)
                    {
                    case 'a':
                        combine_dump = 1;
                        flow_dump = 1;
                        global_reg_dump = 1;
                        jump_opt_dump = 1;
                        addressof_dump = 1;
                        jump2_opt_dump = 1;
                        local_reg_dump = 1;
                        loop_dump = 1;
                        regmove_dump = 1;
                        rtl_dump = 1;
                        cse_dump = 1, cse2_dump = 1;
                        gcse_dump = 1;
                        mach_dep_reorg_dump = 1;
                        break;
                    case 'A':
                        flag_debug_asm = 1;
                        break;
                    case 'c':
                        combine_dump = 1;
                        break;
                    case 'f':
                        flow_dump = 1;
                        break;
                    case 'F':
                        addressof_dump = 1;
                        break;
                    case 'g':
                        global_reg_dump = 1;
                        break;
                    case 'G':
                        gcse_dump = 1;
                        break;
                    case 'j':
                        jump_opt_dump = 1;
                        break;
                    case 'J':
                        jump2_opt_dump = 1;
                        break;
                    case 'l':
                        local_reg_dump = 1;
                        break;
                    case 'L':
                        loop_dump = 1;
                        break;
                    case 'M':
                        mach_dep_reorg_dump = 1;
                        break;
                    case 'p':
                        flag_print_asm_name = 1;
                        break;
                    case 'r':
                        rtl_dump = 1;
                        break;
                    case 's':
                        cse_dump = 1;
                        break;
                    case 't':
                        cse2_dump = 1;
                        break;
                    case 'N':
                        regmove_dump = 1;
                        break;
                    case 'v':
                        graph_dump_format = vcg;
                        break;
                    case 'y':
                        set_yydebug(1);
                        break;
                    case 'x':
                        rtl_dump_and_exit = 1;
                        break;
                    case 'D':   /* these are handled by the preprocessor */
                    case 'I':
                        break;
                    default:
                        warning("unrecognised gcc debugging option: %c", p[-1]);
                        break;
                    }
            }
            else if (str[0] == 'f')
            {
                char *p = &str[1];
                int found = 0;

                /* Some kind of -f option.
                   P's value is the option sans `-f'.
                   Search for it in the table of options.  */

                for (j = 0;
                     !found && j < sizeof (f_options) / sizeof (f_options[0]);
                     j++)
                {
                    if (!strcmp(p, f_options[j].string))
                    {
                        *f_options[j].variable = f_options[j].on_value;
                        /* A goto here would be cleaner,
                           but breaks the vax pcc.  */
                        found = 1;
                    }
                    if (p[0] == 'n' && p[1] == 'o' && p[2] == '-'
                        && !strcmp(p+3, f_options[j].string))
                    {
                        *f_options[j].variable = !f_options[j].on_value;
                        found = 1;
                    }
                }

                if (found)
                    ;
                else if (!strncmp(p, "fixed-", 6))
                    fix_register(&p[6], 1, 1);
                else if (!strncmp(p, "call-used-", 10))
                    fix_register(&p[10], 0, 1);
                else if (!strncmp(p, "call-saved-", 11))
                    fix_register(&p[11], 0, 0);
                else
                    error("Invalid option `%s'", argv[i]);
            }
            else if (str[0] == 'O')
            {
                char *p = str+1;
                if (*p == 's')
                    p++;
                else
                    while (*p && *p >= '0' && *p <= '9')
                        p++;
                if (*p == '\0')
                    ;
                else
                    error("Invalid option `%s'", argv[i]);
            }
            else if (!strcmp(str, "pedantic"))
                pedantic = 1;
            else if (!strcmp(str, "pedantic-errors"))
                flag_pedantic_errors = pedantic = 1;
            else if (!strcmp(str, "loud"))
                loud_flag = 1;
            else if (!strcmp(str, "version"))
                version_flag = 1;
            else if (!strcmp(str, "w"))
                inhibit_warnings = 1;
            else if (!strcmp(str, "W"))
            {
                extra_warnings = 1;
                /* We save the value of warn_uninitialized, since if they put
                   -Wuninitialized on the command line, we need to generate a
                   warning about not using it without also specifying -O.  */
                if (warn_uninitialized != 1)
                    warn_uninitialized = 2;
            }
            else if (str[0] == 'W')
            {
                char *p = &str[1];
                int found = 0;

                /* Some kind of -W option.
                   P's value is the option sans `-W'.
                   Search for it in the table of options.  */

                for (j = 0;
                     !found && j < sizeof (W_options) / sizeof (W_options[0]);
                     j++)
                {
                    if (!strcmp(p, W_options[j].string))
                    {
                        *W_options[j].variable = W_options[j].on_value;
                        /* A goto here would be cleaner,
                           but breaks the vax pcc.  */
                        found = 1;
                    }
                    if (p[0] == 'n' && p[1] == 'o' && p[2] == '-'
                        && !strcmp(p+3, W_options[j].string))
                    {
                        *W_options[j].variable = !W_options[j].on_value;
                        found = 1;
                    }
                }

                if (found)
                    ;
                else if (!strncmp(p, "id-clash-", 9))
                {
                    char *endp = p + 9;

                    while (*endp)
                    {
                        if (*endp >= '0' && *endp <= '9')
                            endp++;
                        else
                        {
                            error("Invalid option `%s'", argv[i]);
                            goto id_clash_lose;
                        }
                    }
                    warn_id_clash = 1;
                    id_clash_len = atoi(str + 10);
id_clash_lose:;
                }
                else if (!strncmp(p, "larger-than-", 12))
                {
                    char *endp = p + 12;

                    while (*endp)
                    {
                        if (*endp >= '0' && *endp <= '9')
                            endp++;
                        else
                        {
                            error("Invalid option `%s'", argv[i]);
                            goto larger_than_lose;
                        }
                    }
                    warn_larger_than = 1;
                    larger_than_size = atoi(str + 13);
larger_than_lose:;
                }
                else
                    error("Invalid option `%s'", argv[i]);
            }
            else if (str[0] == 'g')
            {
                unsigned level = 0;

                if (str[1] == 0)
                {
                    level = 2; /* default debugging info level */
                }
                else if (str[1] >= '0' && str[1] <= '3' && str[2] == 0)
                {
                    level = str[1] - '0';
                }
                else
                {
                    warning("invalid debug option `-%s'", str);
                }

                write_symbols = (level == 0) ? NO_DEBUG : DWARF2_DEBUG;
                debug_info_level = (enum debug_info_level)level;
            }
            else if (!strcmp(str, "o"))
            {
                asm_file_name = argv[++i];
            }
            else if (!strncmp(str, "aux-info", 8))
            {
                flag_gen_aux_info = 1;
                aux_info_file_name = (str[8] != '\0' ? str+8 : argv[++i]);
            }
            /* CYGNUS LOCAL v850/law */
            else if (!strncmp(str, "offset-info", 11))
            {
                flag_gen_offset_info = 1;
                offset_info_file_name = (str[11] != '\0' ? str+11 : argv[++i]);
            }
            /* END CYGNUS LOCAL */
            else if (!strcmp(str, "-help"))
            {
                display_help();
                exit(0);
            }
            else
                error("Invalid option `%s'", argv[i]);
        }
        else if (argv[i][0] == '+')
            error("Invalid option `%s'", argv[i]);
        else
            filename = argv[i];
    }

    /* Checker uses the frame pointer.  */
    if (flag_check_memory_usage)
        flag_omit_frame_pointer = 0;

    if (optimize == 0)
    {
        /* Inlining does not work if not optimizing,
           so force it not to be done.  */
        flag_no_inline = 1;
        warn_inline = 0;

        /* The c_decode_option and lang_decode_option functions set
           this to `2' if -Wall is used, so we can avoid giving out
           lots of errors for people who don't realize what -Wall does.  */
        if (warn_uninitialized == 1)
            warning("-Wuninitialized is not supported without -O");
    }

#ifdef OVERRIDE_OPTIONS
    /* Some machines may reject certain combinations of options.  */
    OVERRIDE_OPTIONS;
#endif

    if (exceptions_via_longjmp == 2)
    {
        exceptions_via_longjmp = 1;
    }

    /* Unrolling all loops implies that standard loop unrolling must also
       be done.  */
    if (flag_unroll_all_loops)
        flag_unroll_loops = 1;
    /* Loop unrolling requires that strength_reduction be on also.  Silently
       turn on strength reduction here if it isn't already on.  Also, the loop
       unrolling code assumes that cse will be run after loop, so that must
       be turned on also.  */
    if (flag_unroll_loops)
    {
        flag_strength_reduce = 1;
        flag_rerun_cse_after_loop = 1;
    }

    /* If we are in verbose mode, write out the version and maybe all the
       option flags in use.  */
    if (version_flag)
    {
        print_version(stderr, "");
        if (loud_flag)
            print_switch_values(stderr, 0, MAX_LINE, "", " ", "\n");
    }

    compile_file(filename);

    if (errorcount)
        exit(EXIT_FAILURE);
    if (sorrycount)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
    return 0;
}

/* Decode -m switches.  */
/* Decode the switch -mNAME.  */

static void
set_target_switch(char *name)
{
    size_t j;
    int valid = 0;

    for (j = 0; j < sizeof target_switches / sizeof target_switches[0]; j++)
        if (!strcmp(target_switches[j].name, name))
        {
            if (target_switches[j].value < 0)
                target_flags &= ~-target_switches[j].value;
            else
                target_flags |= target_switches[j].value;
            valid = 1;
        }

#ifdef TARGET_OPTIONS
    if (!valid)
        for (j = 0; j < sizeof target_options / sizeof target_options[0]; j++)
        {
            int len = strlen(target_options[j].prefix);
            if (!strncmp(target_options[j].prefix, name, len))
            {
                *target_options[j].variable = name + len;
                valid = 1;
            }
        }
#endif

    if (!valid)
        error("Invalid option `%s'", name);
}

/* Print version information to FILE.
   Each line begins with INDENT (for the case where FILE is the
   assembler output file).  */

static void
print_version(FILE *file, char *indent)
{
    fprintf(file, "%s%s%s version %s", indent, *indent != 0 ? " " : "",
            language_string, version_string);

    char *target_name = "thumb-elf";
    fprintf(file, " (%s)", target_name);
#ifdef __GNUC__
#ifndef __VERSION__
#define __VERSION__ "[unknown]"
#endif
    fprintf(file, " compiled by GNU C version %s.\n", __VERSION__);
#else
    fprintf(file, " compiled by CC.\n");
#endif
}

/* Print an option value and return the adjusted position in the line.
   ??? We don't handle error returns from fprintf (disk full); presumably
   other code will catch a disk full though.  */

static int
print_single_switch(FILE *file, int pos, int max, char *indent, char *sep, char *term, char *type, char *name)
{
    /* The ultrix fprintf returns 0 on success, so compute the result we want
       here since we need it for the following test.  */
    int len = strlen(sep) + strlen(type) + strlen(name);

    if (pos != 0
        && pos + len > max)
    {
        fprintf(file, "%s", term);
        pos = 0;
    }
    if (pos == 0)
    {
        fprintf(file, "%s", indent);
        pos = strlen(indent);
    }
    fprintf(file, "%s%s%s", sep, type, name);
    pos += len;
    return pos;
}

/* Print active target switches to FILE.
   POS is the current cursor position and MAX is the size of a "line".
   Each line begins with INDENT and ends with TERM.
   Each switch is separated from the next by SEP.  */

static void
print_switch_values(FILE *file, int pos, int max, char *indent, char *sep, char *term)
{
    size_t j;
    char **p;

    /* Print the options as passed.  */

    pos = print_single_switch(file, pos, max, indent, *indent ? " " : "", term,
                              "options passed: ", "");

    for (p = &save_argv[1]; *p != NULL; p++)
        if (**p == '-')
        {
            /* Ignore these.  */
            if (strcmp(*p, "-o") == 0)
            {
                if (p[1] != NULL)
                    p++;
                continue;
            }
            if (strcmp(*p, "-quiet") == 0)
                continue;
            if (strcmp(*p, "-version") == 0)
                continue;
            if ((*p)[1] == 'd')
                continue;

            pos = print_single_switch(file, pos, max, indent, sep, term, *p, "");
        }
    if (pos > 0)
        fprintf(file, "%s", term);

    /* Print the -f and -m options that have been enabled.
       We don't handle language specific options but printing argv
       should suffice.  */

    pos = print_single_switch(file, 0, max, indent, *indent ? " " : "", term,
                              "options enabled: ", "");

    for (j = 0; j < sizeof f_options / sizeof f_options[0]; j++)
        if (*f_options[j].variable == f_options[j].on_value)
            pos = print_single_switch(file, pos, max, indent, sep, term,
                                      "-f", f_options[j].string);

    /* Print target specific options.  */

    for (j = 0; j < sizeof target_switches / sizeof target_switches[0]; j++)
        if (target_switches[j].name[0] != '\0'
            && target_switches[j].value > 0
            && ((target_switches[j].value & target_flags)
                == target_switches[j].value))
        {
            pos = print_single_switch(file, pos, max, indent, sep, term,
                                      "-m", target_switches[j].name);
        }

#ifdef TARGET_OPTIONS
    for (j = 0; j < sizeof target_options / sizeof target_options[0]; j++)
        if (*target_options[j].variable != NULL)
        {
            char prefix[256];
            sprintf(prefix, "-m%s", target_options[j].prefix);
            pos = print_single_switch(file, pos, max, indent, sep, term,
                                      prefix, *target_options[j].variable);
        }
#endif

    fprintf(file, "%s", term);
}

/* Record the beginning of a new source file, named FILENAME.  */

void
debug_start_source_file(char *filename)
{
#ifdef DWARF2_DEBUGGING_INFO
    if (debug_info_level == DINFO_LEVEL_VERBOSE
        && write_symbols == DWARF2_DEBUG)
        dwarf2out_start_source_file(filename);
#endif /* DWARF2_DEBUGGING_INFO */
}

/* Record the resumption of a source file.  LINENO is the line number in
   the source file we are returning to.  */

void
debug_end_source_file(unsigned lineno ATTRIBUTE_UNUSED)
{
#ifdef DWARF2_DEBUGGING_INFO
    if (debug_info_level == DINFO_LEVEL_VERBOSE
        && write_symbols == DWARF2_DEBUG)
        dwarf2out_end_source_file();
#endif /* DWARF2_DEBUGGING_INFO */
}

/* Called from check_newline in c-parse.y.  The `buffer' parameter contains
   the tail part of the directive line, i.e. the part which is past the
   initial whitespace, #, whitespace, directive-name, whitespace part.  */

void
debug_define(unsigned lineno, char *buffer)
{
#ifdef DWARF2_DEBUGGING_INFO
    if (debug_info_level == DINFO_LEVEL_VERBOSE
        && write_symbols == DWARF2_DEBUG)
        dwarf2out_define(lineno, buffer);
#endif /* DWARF2_DEBUGGING_INFO */
}

/* Called from check_newline in c-parse.y.  The `buffer' parameter contains
   the tail part of the directive line, i.e. the part which is past the
   initial whitespace, #, whitespace, directive-name, whitespace part.  */

void
debug_undef(unsigned lineno, char *buffer)
{
#ifdef DWARF2_DEBUGGING_INFO
    if (debug_info_level == DINFO_LEVEL_VERBOSE
        && write_symbols == DWARF2_DEBUG)
        dwarf2out_undef(lineno, buffer);
#endif /* DWARF2_DEBUGGING_INFO */
}
