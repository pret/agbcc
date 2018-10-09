/* Implements exception handling.
   Copyright (C) 1989, 92-97, 1998 Free Software Foundation, Inc.
   Contributed by Mike Stump <mrs@cygnus.com>.

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
#include "eh-common.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "flags.h"
#include "except.h"
#include "function.h"
#include "insn-flags.h"
#include "expr.h"
#include "insn-codes.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "toplev.h"
#include "obstack.h"

/* One to use setjmp/longjmp method of generating code for exception
   handling.  */

int exceptions_via_longjmp = 2;

/* One to enable asynchronous exception support.  */

int asynchronous_exceptions = 0;

/* One to protect cleanup actions with a handler that calls
   __terminate, zero otherwise.  */

int protect_cleanup_actions_with_terminate;

/* A list of labels used for exception handlers.  Created by
   find_exception_handler_labels for the optimization passes.  */

rtx exception_handler_labels;

/* The EH context.  Nonzero if the function has already
   fetched a pointer to the EH context  for exception handling.  */

rtx current_function_ehc;

/* A stack used for keeping track of the currently active exception
   handling region.  As each exception region is started, an entry
   describing the region is pushed onto this stack.  The current
   region can be found by looking at the top of the stack, and as we
   exit regions, the corresponding entries are popped.

   Entries cannot overlap; they can be nested. So there is only one
   entry at most that corresponds to the current instruction, and that
   is the entry on the top of the stack.  */

static struct eh_stack ehstack;


/* This stack is used to represent what the current eh region is
   for the catch blocks beings processed */

static struct eh_stack catchstack;

/* A queue used for tracking which exception regions have closed but
   whose handlers have not yet been expanded. Regions are emitted in
   groups in an attempt to improve paging performance.

   As we exit a region, we enqueue a new entry. The entries are then
   dequeued during expand_leftover_cleanups and expand_start_all_catch,

   We should redo things so that we either take RTL for the handler,
   or we expand the handler expressed as a tree immediately at region
   end time.  */

static struct eh_queue ehqueue;

/* Insns for all of the exception handlers for the current function.
   They are currently emitted by the frontend code.  */

rtx catch_clauses;

/* A TREE_CHAINed list of handlers for regions that are not yet
   closed. The TREE_VALUE of each entry contains the handler for the
   corresponding entry on the ehstack.  */

static tree protect_list;

/* Stacks to keep track of various labels.  */

/* Keeps track of the label to resume to should one want to resume
   normal control flow out of a handler (instead of, say, returning to
   the caller of the current function or exiting the program).  */

struct label_node *caught_return_label_stack = NULL;

/* A random data area for the front end's own use.  */

struct label_node *false_label_stack = NULL;

/* Pseudos used to hold exception return data in the interim between
   __builtin_eh_return and the end of the function.  */

static rtx eh_return_context;
static rtx eh_return_stack_adjust;
static rtx eh_return_handler;

/* Used to mark the eh return stub for flow, so that the Right Thing
   happens with the values for the hardregs therin.  */

rtx eh_return_stub_label;

/* This is used for targets which can call rethrow with an offset instead
   of an address. This is subtracted from the rethrow label we are
   interested in. */

static rtx first_rethrow_symbol = NULL_RTX;
static rtx final_rethrow = NULL_RTX;
static rtx last_rethrow_symbol = NULL_RTX;


/* Prototypes for local functions.  */

static void push_eh_entry(struct eh_stack *);
static struct eh_entry *pop_eh_entry(struct eh_stack *);
static void enqueue_eh_entry(struct eh_queue *, struct eh_entry *);
static struct eh_entry *dequeue_eh_entry(struct eh_queue *);
static rtx call_get_eh_context(void);
static void start_dynamic_handler(void);
static void expand_rethrow(rtx);
static void output_exception_table_entry(FILE *, int);
static int can_throw(rtx);
static rtx scan_region(rtx, int, int *);
static void eh_regs(rtx *, rtx *, rtx *, int);
static void set_insn_eh_region(rtx *, int);
#ifdef DONT_USE_BUILTIN_SETJMP
static void jumpif_rtx(rtx, rtx);
#endif

rtx expand_builtin_return_addr(enum built_in_function, int, rtx);

/* Various support routines to manipulate the various data structures
   used by the exception handling code.  */

extern struct obstack permanent_obstack;

/* Generate a SYMBOL_REF for rethrow to use */
static rtx create_rethrow_ref(int region_num)
{
    rtx def;
    char *ptr;
    char buf[60];

    push_obstacks_nochange();
    end_temporary_allocation();

    ASM_GENERATE_INTERNAL_LABEL(buf, "LRTH", region_num);
    ptr = (char *)obstack_copy0(&permanent_obstack, buf, strlen(buf));
    def = gen_rtx_SYMBOL_REF(Pmode, ptr);
    SYMBOL_REF_NEED_ADJUST(def) = 1;

    pop_obstacks();
    return def;
}

/* get an exception label. These must be on the permanent obstack */

rtx gen_exception_label(void)
{
    rtx lab;
    lab = gen_label_rtx();
    return lab;
}

/* Push a new eh_node entry onto STACK.  */

static void push_eh_entry(struct eh_stack *stack)
{
    struct eh_node *node = (struct eh_node *)xmalloc(sizeof(struct eh_node));
    struct eh_entry *entry = (struct eh_entry *)xmalloc(sizeof(struct eh_entry));

    rtx rlab = gen_exception_label();
    entry->finalization = NULL_TREE;
    entry->label_used = 0;
    entry->exception_handler_label = rlab;
    entry->false_label = NULL_RTX;
    if (!flag_new_exceptions)
        entry->outer_context = gen_label_rtx();
    else
        entry->outer_context = create_rethrow_ref(CODE_LABEL_NUMBER(rlab));
    entry->rethrow_label = entry->outer_context;

    node->entry = entry;
    node->chain = stack->top;
    stack->top = node;
}

/* Pop an entry from the given STACK.  */

static struct eh_entry *pop_eh_entry(struct eh_stack *stack)
{
    struct eh_node *tempnode;
    struct eh_entry *tempentry;

    tempnode = stack->top;
    tempentry = tempnode->entry;
    stack->top = stack->top->chain;
    free(tempnode);

    return tempentry;
}

/* Enqueue an ENTRY onto the given QUEUE.  */

static void enqueue_eh_entry(struct eh_queue *queue, struct eh_entry *entry)
{
    struct eh_node *node = (struct eh_node *)xmalloc(sizeof(struct eh_node));

    node->entry = entry;
    node->chain = NULL;

    if (queue->head == NULL)
    {
        queue->head = node;
    }
    else
    {
        queue->tail->chain = node;
    }
    queue->tail = node;
}

/* Dequeue an entry from the given QUEUE.  */

static struct eh_entry *dequeue_eh_entry(struct eh_queue *queue)
{
    struct eh_node *tempnode;
    struct eh_entry *tempentry;

    if (queue->head == NULL)
        return NULL;

    tempnode = queue->head;
    queue->head = queue->head->chain;

    tempentry = tempnode->entry;
    free(tempnode);

    return tempentry;
}

static void receive_exception_label(rtx handler_label)
{
    emit_label(handler_label);

#ifdef HAVE_exception_receiver
    if (!exceptions_via_longjmp)
        if (HAVE_exception_receiver)
            emit_insn(gen_exception_receiver());
#endif

#ifdef HAVE_nonlocal_goto_receiver
    if (!exceptions_via_longjmp)
        if (HAVE_nonlocal_goto_receiver)
            emit_insn(gen_nonlocal_goto_receiver());
#endif
}


struct func_eh_entry
{
    int range_number;  /* EH region number from EH NOTE insn's */
    rtx rethrow_label; /* Label for rethrow */
    struct handler_info *handlers;
};


/* table of function eh regions */
static struct func_eh_entry *function_eh_regions = NULL;
static int num_func_eh_entries = 0;
static int current_func_eh_entry = 0;

#define SIZE_FUNC_EH(X) (sizeof(struct func_eh_entry) * X)

/* Add a new eh_entry for this function, and base it off of the information
   in the EH_ENTRY parameter. A NULL parameter is invalid.
   OUTER_CONTEXT is a label which is used for rethrowing. The number
   returned is an number which uniquely identifies this exception range. */

static int new_eh_region_entry(int note_eh_region, rtx rethrow)
{
    if (current_func_eh_entry == num_func_eh_entries)
    {
        if (num_func_eh_entries == 0)
        {
            function_eh_regions = (struct func_eh_entry *)malloc(SIZE_FUNC_EH(50));
            num_func_eh_entries = 50;
        }
        else
        {
            num_func_eh_entries = num_func_eh_entries * 3 / 2;
            function_eh_regions = (struct func_eh_entry *)realloc(
                function_eh_regions, SIZE_FUNC_EH(num_func_eh_entries));
        }
    }
    function_eh_regions[current_func_eh_entry].range_number = note_eh_region;
    if (rethrow == NULL_RTX)
        function_eh_regions[current_func_eh_entry].rethrow_label
            = create_rethrow_ref(note_eh_region);
    else
        function_eh_regions[current_func_eh_entry].rethrow_label = rethrow;
    function_eh_regions[current_func_eh_entry].handlers = NULL;

    return current_func_eh_entry++;
}

/* Add new handler information to an exception range. The  first parameter
   specifies the range number (returned from new_eh_entry()). The second
   parameter specifies the handler.  By default the handler is inserted at
   the end of the list. A handler list may contain only ONE NULL_TREE
   typeinfo entry. Regardless where it is positioned, a NULL_TREE entry
   is always output as the LAST handler in the exception table for a region. */

void add_new_handler(int region, struct handler_info *newhandler)
{
    struct handler_info *last;

    newhandler->next = NULL;
    last = function_eh_regions[region].handlers;
    if (last == NULL)
        function_eh_regions[region].handlers = newhandler;
    else
    {
        for (;; last = last->next)
        {
            if (last->type_info == CATCH_ALL_TYPE)
                pedwarn("additional handler after ...");
            if (last->next == NULL)
                break;
        }
        last->next = newhandler;
    }
}

/* Remove a handler label. The handler label is being deleted, so all
   regions which reference this handler should have it removed from their
   list of possible handlers. Any region which has the final handler
   removed can be deleted. */

void remove_handler(rtx removing_label)
{
    struct handler_info *handler, *last;
    int x;
    for (x = 0; x < current_func_eh_entry; ++x)
    {
        last = NULL;
        handler = function_eh_regions[x].handlers;
        for (; handler; last = handler, handler = handler->next)
            if (handler->handler_label == removing_label)
            {
                if (last)
                {
                    last->next = handler->next;
                    handler = last;
                }
                else
                    function_eh_regions[x].handlers = handler->next;
            }
    }
}

/* Create a new handler structure initialized with the handler label and
   typeinfo fields passed in. */

struct handler_info *get_new_handler(rtx handler, void *typeinfo)
{
    struct handler_info *ptr;
    ptr = (struct handler_info *)malloc(sizeof(struct handler_info));
    ptr->handler_label = handler;
    ptr->handler_number = CODE_LABEL_NUMBER(handler);
    ptr->type_info = typeinfo;
    ptr->next = NULL;

    return ptr;
}


/* Find the index in function_eh_regions associated with a NOTE region. If
   the region cannot be found, a -1 is returned. This should never happen! */

int find_func_region(int insn_region)
{
    int x;
    for (x = 0; x < current_func_eh_entry; x++)
        if (function_eh_regions[x].range_number == insn_region)
            return x;

    return -1;
}

/* Get a pointer to the first handler in an exception region's list. */

struct handler_info *get_first_handler(int region)
{
    return function_eh_regions[find_func_region(region)].handlers;
}

/* Clean out the function_eh_region table and free all memory */

static void clear_function_eh_region(void)
{
    int x;
    struct handler_info *ptr, *next;
    for (x = 0; x < current_func_eh_entry; x++)
        for (ptr = function_eh_regions[x].handlers; ptr != NULL; ptr = next)
        {
            next = ptr->next;
            free(ptr);
        }
    free(function_eh_regions);
    num_func_eh_entries = 0;
    current_func_eh_entry = 0;
}

/* Make a duplicate of an exception region by copying all the handlers
   for an exception region. Return the new handler index. The final
   parameter is a routine which maps old labels to new ones. */

int duplicate_eh_handlers(int old_note_eh_region, int new_note_eh_region, rtx (*map)(rtx))
{
    struct handler_info *ptr, *new_ptr;
    int new_region, region;
    rtx tmp ATTRIBUTE_UNUSED;

    region = find_func_region(old_note_eh_region);
    if (region == -1)
        fatal("Cannot duplicate non-existant exception region.");

    /* duplicate_eh_handlers may have been called during a symbol remap. */
    new_region = find_func_region(new_note_eh_region);
    if (new_region != -1)
        return (new_region);

    new_region = new_eh_region_entry(new_note_eh_region, NULL_RTX);

    ptr = function_eh_regions[region].handlers;

    for (; ptr; ptr = ptr->next)
    {
        new_ptr = get_new_handler(map(ptr->handler_label), ptr->type_info);
        add_new_handler(new_region, new_ptr);
    }

    return new_region;
}


/* Given a rethrow symbol, find the EH region number this is for. */
int eh_region_from_symbol(rtx sym)
{
    int x;
    if (sym == last_rethrow_symbol)
        return 1;
    for (x = 0; x < current_func_eh_entry; x++)
        if (function_eh_regions[x].rethrow_label == sym)
            return function_eh_regions[x].range_number;
    return -1;
}


/* When inlining/unrolling, we have to map the symbols passed to
   __rethrow as well. This performs the remap. If a symbol isn't foiund,
   the original one is returned. This is not an efficient routine,
   so don't call it on everything!! */
rtx rethrow_symbol_map(rtx sym, rtx (*map)(rtx))
{
    int x, y;
    for (x = 0; x < current_func_eh_entry; x++)
        if (function_eh_regions[x].rethrow_label == sym)
        {
            /* We've found the original region, now lets determine which region
               this now maps to. */
            rtx l1 = function_eh_regions[x].handlers->handler_label;
            rtx l2 = map(l1);
            y = CODE_LABEL_NUMBER(l2); /* This is the new region number */
            x = find_func_region(y);   /* Get the new permanent region */
            if (x == -1)               /* Hmm, Doesn't exist yet */
            {
                x = duplicate_eh_handlers(CODE_LABEL_NUMBER(l1), y, map);
                /* Since we're mapping it, it must be used. */
                SYMBOL_REF_USED(function_eh_regions[x].rethrow_label) = 1;
            }
            return function_eh_regions[x].rethrow_label;
        }
    return sym;
}

int rethrow_used(int region)
{
    if (flag_new_exceptions)
    {
        rtx lab = function_eh_regions[find_func_region(region)].rethrow_label;
        return (SYMBOL_REF_USED(lab));
    }
    return 0;
}


/* Routine to see if exception handling is turned on.
   DO_WARN is non-zero if we want to inform the user that exception
   handling is turned off.

   This is used to ensure that -fexceptions has been specified if the
   compiler tries to use any exception-specific functions.  */

int doing_eh(int do_warn)
{
    if (!flag_exceptions)
    {
        static int warned = 0;
        if (!warned && do_warn)
        {
            error("exception handling disabled, use -fexceptions to enable");
            warned = 1;
        }
        return 0;
    }
    return 1;
}

/* Given a return address in ADDR, determine the address we should use
   to find the corresponding EH region.  */

rtx eh_outer_context(rtx addr)
{
    /* First mask out any unwanted bits.  */
#ifdef MASK_RETURN_ADDR
    expand_and(addr, MASK_RETURN_ADDR, addr);
#endif

    /* Then adjust to find the real return address.  */
#if defined(RETURN_ADDR_OFFSET)
    addr = plus_constant(addr, RETURN_ADDR_OFFSET);
#endif

    return addr;
}

/* Emit code to get EH context to current function.  */

static rtx call_get_eh_context(void)
{
    static tree fn;
    tree expr;

    if (fn == NULL_TREE)
    {
        tree fntype;
        fn = get_identifier("__get_eh_context");
        push_obstacks_nochange();
        end_temporary_allocation();
        fntype = build_pointer_type(build_pointer_type(build_pointer_type(void_type_node)));
        fntype = build_function_type(fntype, NULL_TREE);
        fn = build_decl(FUNCTION_DECL, fn, fntype);
        DECL_EXTERNAL(fn) = 1;
        TREE_PUBLIC(fn) = 1;
        DECL_ARTIFICIAL(fn) = 1;
        TREE_READONLY(fn) = 1;
        make_decl_rtl(fn, NULL, 1);
        assemble_external(fn);
        pop_obstacks();
    }

    expr = build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(fn)), fn);
    expr = build(CALL_EXPR, TREE_TYPE(TREE_TYPE(fn)), expr, NULL_TREE, NULL_TREE);
    TREE_SIDE_EFFECTS(expr) = 1;

    return copy_to_reg(expand_expr(expr, NULL_RTX, VOIDmode, EXPAND_NORMAL));
}

/* Get a reference to the EH context.
   We will only generate a register for the current function EH context here,
   and emit a USE insn to mark that this is a EH context register.

   Later, emit_eh_context will emit needed call to __get_eh_context
   in libgcc2, and copy the value to the register we have generated. */

rtx get_eh_context(void)
{
    if (current_function_ehc == 0)
    {
        rtx insn;

        current_function_ehc = gen_reg_rtx(Pmode);

        insn = gen_rtx_USE(GET_MODE(current_function_ehc), current_function_ehc);
        insn = emit_insn_before(insn, get_first_nonparm_insn());

        REG_NOTES(insn) = gen_rtx_EXPR_LIST(REG_EH_CONTEXT, current_function_ehc, REG_NOTES(insn));
    }
    return current_function_ehc;
}

/* Get a reference to the dynamic handler chain.  It points to the
   pointer to the next element in the dynamic handler chain.  It ends
   when there are no more elements in the dynamic handler chain, when
   the value is &top_elt from libgcc2.c.  Immediately after the
   pointer, is an area suitable for setjmp/longjmp when
   DONT_USE_BUILTIN_SETJMP is defined, and an area suitable for
   __builtin_setjmp/__builtin_longjmp when DONT_USE_BUILTIN_SETJMP
   isn't defined. */

rtx get_dynamic_handler_chain(void)
{
    rtx ehc, dhc, result;

    ehc = get_eh_context();

    /* This is the offset of dynamic_handler_chain in the eh_context struct
       declared in eh-common.h. If its location is change, change this offset */
    dhc = plus_constant(ehc, POINTER_SIZE / BITS_PER_UNIT);

    result = copy_to_reg(dhc);

    /* We don't want a copy of the dcc, but rather, the single dcc.  */
    return gen_rtx_MEM(Pmode, result);
}

/* Get a reference to the dynamic cleanup chain.  It points to the
   pointer to the next element in the dynamic cleanup chain.
   Immediately after the pointer, are two Pmode variables, one for a
   pointer to a function that performs the cleanup action, and the
   second, the argument to pass to that function.  */

rtx get_dynamic_cleanup_chain(void)
{
    rtx dhc, dcc, result;

    dhc = get_dynamic_handler_chain();
    dcc = plus_constant(dhc, POINTER_SIZE / BITS_PER_UNIT);

    result = copy_to_reg(dcc);

    /* We don't want a copy of the dcc, but rather, the single dcc.  */
    return gen_rtx_MEM(Pmode, result);
}

#ifdef DONT_USE_BUILTIN_SETJMP
/* Generate code to evaluate X and jump to LABEL if the value is nonzero.
   LABEL is an rtx of code CODE_LABEL, in this function.  */

static void jumpif_rtx(rtx x, rtx label)
{
    jumpif(make_tree(type_for_mode(GET_MODE(x), 0), x), label);
}
#endif

/* Emit RTL to start a dynamic handler on the EH runtime dynamic
   handler stack.  This should only be used by expand_eh_region_start
   or expand_eh_region_start_tree.  */

static void start_dynamic_handler(void)
{
    rtx dhc, dcc;
    rtx x ATTRIBUTE_UNUSED, arg, buf;
    int size;

#ifndef DONT_USE_BUILTIN_SETJMP
    /* The number of Pmode words for the setjmp buffer, when using the
       builtin setjmp/longjmp, see expand_builtin, case
       BUILT_IN_LONGJMP.  */
    size = 5;
#else
#ifdef JMP_BUF_SIZE
    size = JMP_BUF_SIZE;
#else
    /* Should be large enough for most systems, if it is not,
       JMP_BUF_SIZE should be defined with the proper value.  It will
       also tend to be larger than necessary for most systems, a more
       optimal port will define JMP_BUF_SIZE.  */
    size = FIRST_PSEUDO_REGISTER + 2;
#endif
#endif
    /* XXX, FIXME: The stack space allocated this way is too long lived,
       but there is no allocation routine that allocates at the level of
       the last binding contour.  */
    arg = assign_stack_local(BLKmode, GET_MODE_SIZE(Pmode) * (size + 1), 0);

    arg = change_address(arg, Pmode, NULL_RTX);

    /* Store dhc into the first word of the newly allocated buffer.  */

    dhc = get_dynamic_handler_chain();
    dcc = gen_rtx_MEM(Pmode, plus_constant(XEXP(arg, 0), GET_MODE_SIZE(Pmode)));
    emit_move_insn(arg, dhc);

    /* Zero out the start of the cleanup chain.  */
    emit_move_insn(dcc, const0_rtx);

    /* The jmpbuf starts two words into the area allocated.  */
    buf = plus_constant(XEXP(arg, 0), GET_MODE_SIZE(Pmode) * 2);

#ifdef DONT_USE_BUILTIN_SETJMP
    x = emit_library_call_value(setjmp_libfunc, NULL_RTX, 1, SImode, 1, buf, Pmode);
    /* If we come back here for a catch, transfer control to the handler.  */
    jumpif_rtx(x, ehstack.top->entry->exception_handler_label);
#else
    {
        /* A label to continue execution for the no exception case.  */
        rtx noex = gen_label_rtx();
        x = expand_builtin_setjmp(buf, NULL_RTX, noex, ehstack.top->entry->exception_handler_label);
        emit_label(noex);
    }
#endif

    /* We are committed to this, so update the handler chain.  */

    emit_move_insn(dhc, XEXP(arg, 0));
}

/* Just like expand_eh_region_start, except if a cleanup action is
   entered on the cleanup chain, the TREE_PURPOSE of the element put
   on the chain is DECL.  DECL should be the associated VAR_DECL, if
   any, otherwise it should be NULL_TREE.  */

void expand_eh_region_start_for_decl(tree decl)
{
    rtx note;

    /* This is the old code.  */
    if (!doing_eh(0))
        return;

    if (exceptions_via_longjmp)
    {
        /* We need a new block to record the start and end of the
       dynamic handler chain.  We could always do this, but we
       really want to permit jumping into such a block, and we want
       to avoid any errors or performance impact in the SJ EH code
       for now.  */
        expand_start_bindings(0);

        /* But we don't need or want a new temporary level.  */
        pop_temp_slots();

        /* Mark this block as created by expand_eh_region_start.  This
       is so that we can pop the block with expand_end_bindings
       automatically.  */
        mark_block_as_eh_region();

        /* Arrange for returns and gotos to pop the entry we make on the
       dynamic handler stack.  */
        expand_dhc_cleanup(decl);
    }

    push_eh_entry(&ehstack);
    note = emit_note(NULL, NOTE_INSN_EH_REGION_BEG);
    NOTE_BLOCK_NUMBER(note) = CODE_LABEL_NUMBER(ehstack.top->entry->exception_handler_label);
    if (exceptions_via_longjmp)
        start_dynamic_handler();
}

/* Start an exception handling region.  All instructions emitted after
   this point are considered to be part of the region until
   expand_eh_region_end is invoked.  */

void expand_eh_region_start(void)
{
    expand_eh_region_start_for_decl(NULL_TREE);
}

/* End an exception handling region.  The information about the region
   is found on the top of ehstack.

   HANDLER is either the cleanup for the exception region, or if we're
   marking the end of a try block, HANDLER is integer_zero_node.

   HANDLER will be transformed to rtl when expand_leftover_cleanups
   is invoked.  */

void expand_eh_region_end(tree handler)
{
    struct eh_entry *entry;
    rtx note;
    int ret ATTRIBUTE_UNUSED;
    int r ATTRIBUTE_UNUSED;

    if (!doing_eh(0))
        return;

    entry = pop_eh_entry(&ehstack);

    note = emit_note(NULL, NOTE_INSN_EH_REGION_END);
    ret = NOTE_BLOCK_NUMBER(note) = CODE_LABEL_NUMBER(entry->exception_handler_label);
    if (exceptions_via_longjmp == 0
        && !flag_new_exceptions
        /* We share outer_context between regions; only emit it once.  */
        && INSN_UID(entry->outer_context) == 0)
    {
        rtx label;

        label = gen_label_rtx();
        emit_jump(label);

        /* Emit a label marking the end of this exception region that
       is used for rethrowing into the outer context.  */
        emit_label(entry->outer_context);
        expand_internal_throw();

        emit_label(label);
    }

    entry->finalization = handler;

    /* create region entry in final exception table */
    r = new_eh_region_entry(NOTE_BLOCK_NUMBER(note), entry->rethrow_label);

    enqueue_eh_entry(&ehqueue, entry);

    /* If we have already started ending the bindings, don't recurse.
       This only happens when exceptions_via_longjmp is true.  */
    if (is_eh_region())
    {
        /* Because we don't need or want a new temporary level and
       because we didn't create one in expand_eh_region_start,
       create a fake one now to avoid removing one in
       expand_end_bindings.  */
        push_temp_slots();

        mark_block_as_not_eh_region();

        /* Maybe do this to prevent jumping in and so on...  */
        expand_end_bindings(NULL_TREE, 0, 0);
    }
}

/* End the EH region for a goto fixup.  We only need them in the region-based
   EH scheme.  */

void expand_fixup_region_start(void)
{
    if (!doing_eh(0) || exceptions_via_longjmp)
        return;

    expand_eh_region_start();
}

/* End the EH region for a goto fixup.  CLEANUP is the cleanup we just
   expanded; to avoid running it twice if it throws, we look through the
   ehqueue for a matching region and rethrow from its outer_context.  */

void expand_fixup_region_end(tree cleanup)
{
    struct eh_node *node;
    int dont_issue;

    if (!doing_eh(0) || exceptions_via_longjmp)
        return;

    for (node = ehstack.top; node && node->entry->finalization != cleanup;)
        node = node->chain;
    if (node == 0)
        for (node = ehqueue.head; node && node->entry->finalization != cleanup;)
            node = node->chain;
    if (node == 0)
        abort();

    /* If the outer context label has not been issued yet, we don't want
       to issue it as a part of this region, unless this is the
       correct region for the outer context. If we did, then the label for
       the outer context will be WITHIN the begin/end labels,
       and we could get an infinte loop when it tried to rethrow, or just
       generally incorrect execution following a throw. */

    dont_issue
        = ((INSN_UID(node->entry->outer_context) == 0) && (ehstack.top->entry != node->entry));

    ehstack.top->entry->outer_context = node->entry->outer_context;

    /* Since we are rethrowing to the OUTER region, we know we don't need
       a jump around sequence for this region, so we'll pretend the outer
       context label has been issued by setting INSN_UID to 1, then clearing
       it again afterwards. */

    if (dont_issue)
        INSN_UID(node->entry->outer_context) = 1;

    /* Just rethrow.  size_zero_node is just a NOP.  */
    expand_eh_region_end(size_zero_node);

    if (dont_issue)
        INSN_UID(node->entry->outer_context) = 0;
}

/* If we are using the setjmp/longjmp EH codegen method, we emit a
   call to __sjthrow.

   Otherwise, we emit a call to __throw and note that we threw
   something, so we know we need to generate the necessary code for
   __throw.

   Before invoking throw, the __eh_pc variable must have been set up
   to contain the PC being thrown from. This address is used by
   __throw to determine which exception region (if any) is
   responsible for handling the exception.  */

void emit_throw(void)
{
    if (exceptions_via_longjmp)
    {
        emit_library_call(sjthrow_libfunc, 0, VOIDmode, 0);
    }
    else
    {
#ifdef JUMP_TO_THROW
        emit_indirect_jump(throw_libfunc);
#else
        emit_library_call(throw_libfunc, 0, VOIDmode, 0);
#endif
    }
    emit_barrier();
}

/* Throw the current exception.  If appropriate, this is done by jumping
   to the next handler.  */

void expand_internal_throw(void)
{
    emit_throw();
}

/* Called from expand_exception_blocks and expand_end_catch_block to
   emit any pending handlers/cleanups queued from expand_eh_region_end.  */

void expand_leftover_cleanups(void)
{
    struct eh_entry *entry;

    while ((entry = dequeue_eh_entry(&ehqueue)) != 0)
    {
        rtx prev;

        /* A leftover try block. Shouldn't be one here.  */
        if (entry->finalization == integer_zero_node)
            abort();

        /* Output the label for the start of the exception handler.  */

        receive_exception_label(entry->exception_handler_label);

        /* register a handler for this cleanup region */
        add_new_handler(find_func_region(CODE_LABEL_NUMBER(entry->exception_handler_label)),
            get_new_handler(entry->exception_handler_label, NULL));

        /* And now generate the insns for the handler.  */
        expand_expr(entry->finalization, const0_rtx, VOIDmode, EXPAND_NORMAL);

        prev = get_last_insn();
        if (prev == NULL || GET_CODE(prev) != BARRIER)
            /* Emit code to throw to the outer context if we fall off
               the end of the handler.  */
            expand_rethrow(entry->outer_context);

        do_pending_stack_adjust();
        free(entry);
    }
}

/* Rethrow from the outer context LABEL.  */

static void expand_rethrow(rtx label)
{
    if (exceptions_via_longjmp)
        emit_throw();
    else if (flag_new_exceptions)
    {
        rtx insn, val;
        if (label == NULL_RTX)
            label = last_rethrow_symbol;
        emit_library_call(rethrow_libfunc, 0, VOIDmode, 1, label, Pmode);
        SYMBOL_REF_USED(label) = 1;
        insn = get_last_insn();
        val = GEN_INT(eh_region_from_symbol(label));
        /* Mark the label/symbol on the call. */
        REG_NOTES(insn) = gen_rtx_EXPR_LIST(REG_EH_RETHROW, val, REG_NOTES(insn));
        emit_barrier();
    }
    else
        emit_jump(label);
}

/* Arrange for __terminate to be called if there is an unhandled throw
   from within E.  */

tree protect_with_terminate(tree e)
{
    /* We only need to do this when using setjmp/longjmp EH and the
       language requires it, as otherwise we protect all of the handlers
       at once, if we need to.  */
    if (exceptions_via_longjmp && protect_cleanup_actions_with_terminate)
    {
        tree handler, result;

        /* All cleanups must be on the function_obstack.  */
        push_obstacks_nochange();
        resume_temporary_allocation();

        handler = make_node(RTL_EXPR);
        TREE_TYPE(handler) = void_type_node;
        RTL_EXPR_RTL(handler) = const0_rtx;
        TREE_SIDE_EFFECTS(handler) = 1;
        start_sequence_for_rtl_expr(handler);

        emit_library_call(terminate_libfunc, 0, VOIDmode, 0);
        emit_barrier();

        RTL_EXPR_SEQUENCE(handler) = get_insns();
        end_sequence();

        result = build(TRY_CATCH_EXPR, TREE_TYPE(e), e, handler);
        TREE_SIDE_EFFECTS(result) = TREE_SIDE_EFFECTS(e);
        TREE_THIS_VOLATILE(result) = TREE_THIS_VOLATILE(e);
        TREE_READONLY(result) = TREE_READONLY(e);

        pop_obstacks();

        e = result;
    }

    return e;
}

/* The exception table that we build that is used for looking up and
   dispatching exceptions, the current number of entries, and its
   maximum size before we have to extend it.

   The number in eh_table is the code label number of the exception
   handler for the region. This is added by add_eh_table_entry and
   used by output_exception_table_entry.  */

static int *eh_table = NULL;
static int eh_table_size = 0;
static int eh_table_max_size = 0;

/* Note the need for an exception table entry for region N.  If we
   don't need to output an explicit exception table, avoid all of the
   extra work.

   Called from final_scan_insn when a NOTE_INSN_EH_REGION_BEG is seen.
   (Or NOTE_INSN_EH_REGION_END sometimes)
   N is the NOTE_BLOCK_NUMBER of the note, which comes from the code
   label number of the exception handler for the region.  */

void add_eh_table_entry(int n)
{
#ifndef OMIT_EH_TABLE
    if (eh_table_size >= eh_table_max_size)
    {
        if (eh_table)
        {
            eh_table_max_size += eh_table_max_size >> 1;

            if (eh_table_max_size < 0)
                abort();

            eh_table = (int *)xrealloc(eh_table, eh_table_max_size * sizeof(int));
        }
        else
        {
            eh_table_max_size = 252;
            eh_table = (int *)xmalloc(eh_table_max_size * sizeof(int));
        }
    }
    eh_table[eh_table_size++] = n;
#endif
}

/* Output the entry of the exception table corresponding to the
   exception region numbered N to file FILE.

   N is the code label number corresponding to the handler of the
   region.  */

static void output_exception_table_entry(FILE *file, int n)
{
    char buf[256];
    rtx sym;
    struct handler_info *handler = get_first_handler(n);
    int index = find_func_region(n);
    rtx rethrow;

    /* form and emit the rethrow label, if needed  */
    rethrow = function_eh_regions[index].rethrow_label;
    if (rethrow != NULL_RTX && !flag_new_exceptions)
        rethrow = NULL_RTX;
    if (rethrow != NULL_RTX && handler == NULL)
        if (!SYMBOL_REF_USED(rethrow))
            rethrow = NULL_RTX;


    for (; handler != NULL || rethrow != NULL_RTX; handler = handler->next)
    {
        /* rethrow label should indicate the LAST entry for a region */
        if (rethrow != NULL_RTX && (handler == NULL || handler->next == NULL))
        {
            ASM_GENERATE_INTERNAL_LABEL(buf, "LRTH", n);
            assemble_label(buf);
            rethrow = NULL_RTX;
        }

        ASM_GENERATE_INTERNAL_LABEL(buf, "LEHB", n);
        sym = gen_rtx_SYMBOL_REF(Pmode, buf);
        assemble_integer(sym, POINTER_SIZE / BITS_PER_UNIT, 1);

        ASM_GENERATE_INTERNAL_LABEL(buf, "LEHE", n);
        sym = gen_rtx_SYMBOL_REF(Pmode, buf);
        assemble_integer(sym, POINTER_SIZE / BITS_PER_UNIT, 1);

        if (handler == NULL)
            assemble_integer(GEN_INT(0), POINTER_SIZE / BITS_PER_UNIT, 1);
        else
        {
            ASM_GENERATE_INTERNAL_LABEL(buf, "L", handler->handler_number);
            sym = gen_rtx_SYMBOL_REF(Pmode, buf);
            assemble_integer(sym, POINTER_SIZE / BITS_PER_UNIT, 1);
        }

        if (flag_new_exceptions)
        {
            if (handler == NULL || handler->type_info == NULL)
                assemble_integer(const0_rtx, POINTER_SIZE / BITS_PER_UNIT, 1);
            else if (handler->type_info == CATCH_ALL_TYPE)
                assemble_integer(GEN_INT(CATCH_ALL_TYPE), POINTER_SIZE / BITS_PER_UNIT, 1);
            else
                output_constant((tree)(handler->type_info), POINTER_SIZE / BITS_PER_UNIT);
        }
        putc('\n', file); /* blank line */
        /* We only output the first label under the old scheme */
        if (!flag_new_exceptions || handler == NULL)
            break;
    }
}
static int language_code = 0;
static int version_code = 0;

/* Output the exception table if we have and need one.  */

void output_exception_table(void)
{
    int i;
    char buf[256];
    extern FILE *asm_out_file;

    if (!doing_eh(0) || !eh_table)
        return;

    exception_section();

    /* Beginning marker for table.  */
    assemble_align(GET_MODE_ALIGNMENT(ptr_mode));
    assemble_label("__EXCEPTION_TABLE__");

    if (flag_new_exceptions)
    {
        assemble_integer(GEN_INT(NEW_EH_RUNTIME), POINTER_SIZE / BITS_PER_UNIT, 1);
        assemble_integer(GEN_INT(language_code), 2, 1);
        assemble_integer(GEN_INT(version_code), 2, 1);

        /* Add enough padding to make sure table aligns on a pointer boundry. */
        i = GET_MODE_ALIGNMENT(ptr_mode) / BITS_PER_UNIT - 4;
        for (; i < 0; i = i + GET_MODE_ALIGNMENT(ptr_mode) / BITS_PER_UNIT)
            ;
        if (i != 0)
            assemble_integer(const0_rtx, i, 1);

        /* Generate the label for offset calculations on rethrows */
        ASM_GENERATE_INTERNAL_LABEL(buf, "LRTH", 0);
        assemble_label(buf);
    }

    for (i = 0; i < eh_table_size; ++i)
        output_exception_table_entry(asm_out_file, eh_table[i]);

    free(eh_table);
    clear_function_eh_region();

    /* Ending marker for table.  */
    /* Generate the label for end of table. */
    ASM_GENERATE_INTERNAL_LABEL(buf, "LRTH", CODE_LABEL_NUMBER(final_rethrow));
    assemble_label(buf);
    assemble_integer(constm1_rtx, POINTER_SIZE / BITS_PER_UNIT, 1);

    /* for binary compatability, the old __throw checked the second
       position for a -1, so we should output at least 2 -1's */
    if (!flag_new_exceptions)
        assemble_integer(constm1_rtx, POINTER_SIZE / BITS_PER_UNIT, 1);

    putc('\n', asm_out_file); /* blank line */
}

/* Emit code to get EH context.

   We have to scan thru the code to find possible EH context registers.
   Inlined functions may use it too, and thus we'll have to be able
   to change them too.

   This is done only if using exceptions_via_longjmp. */

void emit_eh_context(void)
{
    rtx insn;
    rtx ehc = 0;

    if (!doing_eh(0))
        return;

    for (insn = get_insns(); insn; insn = NEXT_INSN(insn))
        if (GET_CODE(insn) == INSN && GET_CODE(PATTERN(insn)) == USE)
        {
            rtx reg = find_reg_note(insn, REG_EH_CONTEXT, 0);
            if (reg)
            {
                rtx insns;

                start_sequence();

                /* If this is the first use insn, emit the call here.  This
                   will always be at the top of our function, because if
                   expand_inline_function notices a REG_EH_CONTEXT note, it
                   adds a use insn to this function as well.  */
                if (ehc == 0)
                    ehc = call_get_eh_context();

                emit_move_insn(XEXP(reg, 0), ehc);
                insns = get_insns();
                end_sequence();

                emit_insns_before(insns, insn);

                /* At -O0, we must make the context register stay alive so
                   that the stupid.c register allocator doesn't get confused. */
                if (obey_regdecls != 0)
                {
                    insns = gen_rtx_USE(GET_MODE(XEXP(reg, 0)), XEXP(reg, 0));
                    emit_insn_before(insns, get_last_insn());
                }
            }
        }
}

/* Scan the current insns and build a list of handler labels. The
   resulting list is placed in the global variable exception_handler_labels.

   It is called after the last exception handling region is added to
   the current function (when the rtl is almost all built for the
   current function) and before the jump optimization pass.  */

void find_exception_handler_labels(void)
{
    rtx insn;

    exception_handler_labels = NULL_RTX;

    /* If we aren't doing exception handling, there isn't much to check.  */
    if (!doing_eh(0))
        return;

    /* For each start of a region, add its label to the list.  */

    for (insn = get_insns(); insn; insn = NEXT_INSN(insn))
    {
        struct handler_info *ptr;
        if (GET_CODE(insn) == NOTE && NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_BEG)
        {
            ptr = get_first_handler(NOTE_BLOCK_NUMBER(insn));
            for (; ptr; ptr = ptr->next)
            {
                /* make sure label isn't in the list already */
                rtx x;
                for (x = exception_handler_labels; x; x = XEXP(x, 1))
                    if (XEXP(x, 0) == ptr->handler_label)
                        break;
                if (!x)
                    exception_handler_labels
                        = gen_rtx_EXPR_LIST(VOIDmode, ptr->handler_label, exception_handler_labels);
            }
        }
    }
}

/* Perform sanity checking on the exception_handler_labels list.

   Can be called after find_exception_handler_labels is called to
   build the list of exception handlers for the current function and
   before we finish processing the current function.  */

void check_exception_handler_labels(void)
{
    rtx insn, insn2;

    /* If we aren't doing exception handling, there isn't much to check.  */
    if (!doing_eh(0))
        return;

    /* Make sure there is no more than 1 copy of a label */
    for (insn = exception_handler_labels; insn; insn = XEXP(insn, 1))
    {
        int count = 0;
        for (insn2 = exception_handler_labels; insn2; insn2 = XEXP(insn2, 1))
            if (XEXP(insn, 0) == XEXP(insn2, 0))
                count++;
        if (count != 1)
            warning("Counted %d copies of EH region %d in list.\n", count, CODE_LABEL_NUMBER(insn));
    }
}

/* This group of functions initializes the exception handling data
   structures at the start of the compilation, initializes the data
   structures at the start of a function, and saves and restores the
   exception handling data structures for the start/end of a nested
   function.  */

/* Toplevel initialization for EH things.  */

void init_eh(void)
{
    first_rethrow_symbol = create_rethrow_ref(0);
    final_rethrow = gen_exception_label();
    last_rethrow_symbol = create_rethrow_ref(CODE_LABEL_NUMBER(final_rethrow));
}

/* Initialize the per-function EH information.  */

void init_eh_for_function(void)
{
    ehstack.top = 0;
    catchstack.top = 0;
    ehqueue.head = ehqueue.tail = 0;
    catch_clauses = NULL_RTX;
    false_label_stack = 0;
    caught_return_label_stack = 0;
    protect_list = NULL_TREE;
    current_function_ehc = NULL_RTX;
    eh_return_context = NULL_RTX;
    eh_return_stack_adjust = NULL_RTX;
    eh_return_handler = NULL_RTX;
    eh_return_stub_label = NULL_RTX;
}

/* Save some of the per-function EH info into the save area denoted by
   P.

   This is currently called from save_stmt_status.  */

void save_eh_status(struct function *p)
{
    if (p == NULL)
        abort();

    p->ehstack = ehstack;
    p->catchstack = catchstack;
    p->ehqueue = ehqueue;
    p->catch_clauses = catch_clauses;
    p->false_label_stack = false_label_stack;
    p->caught_return_label_stack = caught_return_label_stack;
    p->protect_list = protect_list;
    p->ehc = current_function_ehc;

    init_eh_for_function();
}

/* Restore the per-function EH info saved into the area denoted by P.

   This is currently called from restore_stmt_status.  */

void restore_eh_status(struct function *p)
{
    if (p == NULL)
        abort();

    protect_list = p->protect_list;
    caught_return_label_stack = p->caught_return_label_stack;
    false_label_stack = p->false_label_stack;
    catch_clauses = p->catch_clauses;
    ehqueue = p->ehqueue;
    ehstack = p->ehstack;
    catchstack = p->catchstack;
    current_function_ehc = p->ehc;
}

/* This section is for the exception handling specific optimization
   pass.  First are the internal routines, and then the main
   optimization pass.  */

/* Determine if the given INSN can throw an exception.  */

static int can_throw(rtx insn)
{
    /* Calls can always potentially throw exceptions.  */
    if (GET_CODE(insn) == CALL_INSN)
        return 1;

    if (asynchronous_exceptions)
    {
        /* If we wanted asynchronous exceptions, then everything but NOTEs
       and CODE_LABELs could throw.  */
        if (GET_CODE(insn) != NOTE && GET_CODE(insn) != CODE_LABEL)
            return 1;
    }

    return 0;
}

/* Scan a exception region looking for the matching end and then
   remove it if possible. INSN is the start of the region, N is the
   region number, and DELETE_OUTER is to note if anything in this
   region can throw.

   Regions are removed if they cannot possibly catch an exception.
   This is determined by invoking can_throw on each insn within the
   region; if can_throw returns true for any of the instructions, the
   region can catch an exception, since there is an insn within the
   region that is capable of throwing an exception.

   Returns the NOTE_INSN_EH_REGION_END corresponding to this region, or
   calls abort if it can't find one.

   Can abort if INSN is not a NOTE_INSN_EH_REGION_BEGIN, or if N doesn't
   correspond to the region number, or if DELETE_OUTER is NULL.  */

static rtx scan_region(rtx insn, int n, int *delete_outer)
{
    rtx start = insn;

    /* Assume we can delete the region.  */
    int can_delete = 1;

    int r = find_func_region(n);
    /* Can't delete something which is rethrown to. */
    if (SYMBOL_REF_USED((function_eh_regions[r].rethrow_label)))
        can_delete = 0;

    if (insn == NULL_RTX || GET_CODE(insn) != NOTE
        || NOTE_LINE_NUMBER(insn) != NOTE_INSN_EH_REGION_BEG || NOTE_BLOCK_NUMBER(insn) != n
        || delete_outer == NULL)
        abort();

    insn = NEXT_INSN(insn);

    /* Look for the matching end.  */
    while (!(GET_CODE(insn) == NOTE && NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_END))
    {
        /* If anything can throw, we can't remove the region.  */
        if (can_delete &&can_throw(insn))
        {
            can_delete = 0;
        }

        /* Watch out for and handle nested regions.  */
        if (GET_CODE(insn) == NOTE && NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_BEG)
        {
            insn = scan_region(insn, NOTE_BLOCK_NUMBER(insn), &can_delete);
        }

        insn = NEXT_INSN(insn);
    }

    /* The _BEG/_END NOTEs must match and nest.  */
    if (NOTE_BLOCK_NUMBER(insn) != n)
        abort();

    /* If anything in this exception region can throw, we can throw.  */
    if (!can_delete)
        *delete_outer = 0;
    else
    {
        /* Delete the start and end of the region.  */
        delete_insn(start);
        delete_insn(insn);

        /* We no longer removed labels here, since flow will now remove any
           handler which cannot be called any more. */
    }
    return insn;
}

/* Perform various interesting optimizations for exception handling
   code.

   We look for empty exception regions and make them go (away). The
   jump optimization code will remove the handler if nothing else uses
   it.  */

void exception_optimize(void)
{
    rtx insn;
    int n;

    /* Remove empty regions.  */
    for (insn = get_insns(); insn; insn = NEXT_INSN(insn))
    {
        if (GET_CODE(insn) == NOTE && NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_BEG)
        {
            /* Since scan_region will return the NOTE_INSN_EH_REGION_END
               insn, we will indirectly skip through all the insns
               inbetween. We are also guaranteed that the value of insn
               returned will be valid, as otherwise scan_region won't
               return.  */
            insn = scan_region(insn, NOTE_BLOCK_NUMBER(insn), &n);
        }
    }
}

/* Various hooks for the DWARF 2 __throw routine.  */

/* Do any necessary initialization to access arbitrary stack frames.
   On the SPARC, this means flushing the register windows.  */

void expand_builtin_unwind_init(void)
{
    /* Set this so all the registers get saved in our frame; we need to be
       able to copy the saved values for any registers from frames we unwind. */
    current_function_has_nonlocal_label = 1;

#ifdef SETUP_FRAME_ADDRESSES
    SETUP_FRAME_ADDRESSES();
#endif
}

/* Given a value extracted from the return address register or stack slot,
   return the actual address encoded in that value.  */

rtx expand_builtin_extract_return_addr(tree addr_tree)
{
    rtx addr = expand_expr(addr_tree, NULL_RTX, Pmode, EXPAND_NORMAL);
    return eh_outer_context(addr);
}

/* Given an actual address in addr_tree, do any necessary encoding
   and return the value to be stored in the return address register or
   stack slot so the epilogue will return to that address.  */

rtx expand_builtin_frob_return_addr(tree addr_tree)
{
    rtx addr = expand_expr(addr_tree, NULL_RTX, Pmode, EXPAND_NORMAL);
#ifdef RETURN_ADDR_OFFSET
    addr = plus_constant(addr, -RETURN_ADDR_OFFSET);
#endif
    return addr;
}

/* Choose three registers for communication between the main body of
   __throw and the epilogue (or eh stub) and the exception handler.
   We must do this with hard registers because the epilogue itself
   will be generated after reload, at which point we may not reference
   pseudos at all.

   The first passes the exception context to the handler.  For this
   we use the return value register for a void*.

   The second holds the stack pointer value to be restored.  For
   this we use the static chain register if it exists and is different
   from the previous, otherwise some arbitrary call-clobbered register.

   The third holds the address of the handler itself.  Here we use
   some arbitrary call-clobbered register.  */

static void eh_regs(rtx *pcontext, rtx *psp, rtx *pra, int outgoing)
{
    rtx rcontext, rsp, rra;
    int i;

#ifdef FUNCTION_OUTGOING_VALUE
    if (outgoing)
        rcontext
            = FUNCTION_OUTGOING_VALUE(build_pointer_type(void_type_node), current_function_decl);
    else
#endif
        rcontext = FUNCTION_VALUE(build_pointer_type(void_type_node), current_function_decl);

#ifdef STATIC_CHAIN_REGNUM
    if (outgoing)
        rsp = static_chain_incoming_rtx;
    else
        rsp = static_chain_rtx;
    if (REGNO(rsp) == REGNO(rcontext))
#endif /* STATIC_CHAIN_REGNUM */
        rsp = NULL_RTX;

    if (rsp == NULL_RTX)
    {
        for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
            if (call_used_regs[i] && !fixed_regs[i] && i != REGNO(rcontext))
                break;
        if (i == FIRST_PSEUDO_REGISTER)
            abort();

        rsp = gen_rtx_REG(Pmode, i);
    }

    for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
        if (call_used_regs[i] && !fixed_regs[i] && i != REGNO(rcontext) && i != REGNO(rsp))
            break;
    if (i == FIRST_PSEUDO_REGISTER)
        abort();

    rra = gen_rtx_REG(Pmode, i);

    *pcontext = rcontext;
    *psp = rsp;
    *pra = rra;
}

/* Set up the epilogue with the magic bits we'll need to return to the
   exception handler.  */

void expand_builtin_eh_return(tree context, tree stack, tree handler)
{
    if (eh_return_context)
        error("Duplicate call to __builtin_eh_return");

    eh_return_context = copy_to_reg(expand_expr(context, NULL_RTX, VOIDmode, EXPAND_NORMAL));
    eh_return_stack_adjust = copy_to_reg(expand_expr(stack, NULL_RTX, VOIDmode, EXPAND_NORMAL));
    eh_return_handler = copy_to_reg(expand_expr(handler, NULL_RTX, VOIDmode, EXPAND_NORMAL));
}

void expand_eh_return(void)
{
    rtx reg1, reg2, reg3;
    rtx stub_start, after_stub;
    rtx ra, tmp;

    if (!eh_return_context)
        return;

    eh_regs(&reg1, &reg2, &reg3, 1);
    emit_move_insn(reg1, eh_return_context);
    emit_move_insn(reg2, eh_return_stack_adjust);
    emit_move_insn(reg3, eh_return_handler);

    /* Talk directly to the target's epilogue code when possible.  */

#ifdef HAVE_eh_epilogue
    if (HAVE_eh_epilogue)
    {
        emit_insn(gen_eh_epilogue(reg1, reg2, reg3));
        return;
    }
#endif

    /* Otherwise, use the same stub technique we had before.  */

    eh_return_stub_label = stub_start = gen_label_rtx();
    after_stub = gen_label_rtx();

    /* Set the return address to the stub label.  */

    ra = expand_builtin_return_addr(BUILT_IN_RETURN_ADDRESS, 0, frame_pointer_rtx);
    if (GET_CODE(ra) == REG && REGNO(ra) >= FIRST_PSEUDO_REGISTER)
        abort();

    tmp = memory_address(Pmode, gen_rtx_LABEL_REF(Pmode, stub_start));
#ifdef RETURN_ADDR_OFFSET
    tmp = plus_constant(tmp, -RETURN_ADDR_OFFSET);
#endif
    tmp = force_operand(tmp, ra);
    if (tmp != ra)
        emit_move_insn(ra, tmp);

    /* Indicate that the registers are in fact used.  */
    emit_insn(gen_rtx_USE(VOIDmode, reg1));
    emit_insn(gen_rtx_USE(VOIDmode, reg2));
    emit_insn(gen_rtx_USE(VOIDmode, reg3));
    if (GET_CODE(ra) == REG)
        emit_insn(gen_rtx_USE(VOIDmode, ra));

    /* Generate the stub.  */

    emit_jump(after_stub);
    emit_label(stub_start);

    eh_regs(&reg1, &reg2, &reg3, 0);
    adjust_stack(reg2);
    emit_indirect_jump(reg3);

    emit_label(after_stub);
}


/* This contains the code required to verify whether arbitrary instructions
   are in the same exception region. */

static int *insn_eh_region = (int *)0;
static int maximum_uid;

static void set_insn_eh_region(rtx *first, int region_num)
{
    rtx insn;
    int rnum;

    for (insn = *first; insn; insn = NEXT_INSN(insn))
    {
        if ((GET_CODE(insn) == NOTE) && (NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_BEG))
        {
            rnum = NOTE_BLOCK_NUMBER(insn);
            insn_eh_region[INSN_UID(insn)] = rnum;
            insn = NEXT_INSN(insn);
            set_insn_eh_region(&insn, rnum);
            /* Upon return, insn points to the EH_REGION_END of nested region */
            continue;
        }
        insn_eh_region[INSN_UID(insn)] = region_num;
        if ((GET_CODE(insn) == NOTE) && (NOTE_LINE_NUMBER(insn) == NOTE_INSN_EH_REGION_END))
            break;
    }
    *first = insn;
}

/* Free the insn table, an make sure it cannot be used again. */

void free_insn_eh_region(void)
{
    if (!doing_eh(0))
        return;

    if (insn_eh_region)
    {
        free(insn_eh_region);
        insn_eh_region = (int *)0;
    }
}

/* Initialize the table. max_uid must be calculated and handed into
   this routine. If it is unavailable, passing a value of 0 will
   cause this routine to calculate it as well. */

void init_insn_eh_region(rtx first, int max_uid)
{
    rtx insn;

    if (!doing_eh(0))
        return;

    if (insn_eh_region)
        free_insn_eh_region();

    if (max_uid == 0)
        for (insn = first; insn; insn = NEXT_INSN(insn))
            if (INSN_UID(insn) > max_uid) /* find largest UID */
                max_uid = INSN_UID(insn);

    maximum_uid = max_uid;
    insn_eh_region = (int *)malloc((max_uid + 1) * sizeof(int));
    insn = first;
    set_insn_eh_region(&insn, 0);
}


/* Check whether 2 instructions are within the same region. */

int in_same_eh_region(rtx insn1, rtx insn2)
{
    int ret, uid1, uid2;

    /* If no exceptions, instructions are always in same region. */
    if (!doing_eh(0))
        return 1;

    /* If the table isn't allocated, assume the worst. */
    if (!insn_eh_region)
        return 0;

    uid1 = INSN_UID(insn1);
    uid2 = INSN_UID(insn2);

    /* if instructions have been allocated beyond the end, either
       the table is out of date, or this is a late addition, or
       something... Assume the worst. */
    if (uid1 > maximum_uid || uid2 > maximum_uid)
        return 0;

    ret = (insn_eh_region[uid1] == insn_eh_region[uid2]);
    return ret;
}
