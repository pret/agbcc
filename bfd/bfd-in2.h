/* DO NOT EDIT!  -*- buffer-read-only: t -*-  This file is automatically
   generated from "bfd-in.h", "init.c", "opncls.c", "libbfd.c",
   "bfdio.c", "bfdwin.c", "section.c", "archures.c", "reloc.c",
   "syms.c", "bfd.c", "archive.c", "corefile.c", "targets.c", "format.c",
   "linker.c", "simple.c" and "compress.c".
   Run "make headers" in your build bfd/ to regenerate.  */

/* Main header file for the bfd library -- portable access to object files.

   Copyright (C) 1990-2018 Free Software Foundation, Inc.

   Contributed by Cygnus Support.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef __BFD_H_SEEN__
#define __BFD_H_SEEN__

/* PR 14072: Ensure that config.h is included first.  */
#if !defined PACKAGE && !defined PACKAGE_VERSION
#error config.h must be included before this header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ansidecl.h"
#include "symcat.h"
#include "diagnostics.h"
#include <stdarg.h>
#include <sys/stat.h>
#include <stdint.h>


#ifndef SABER
/* This hack is to avoid a problem with some strict ANSI C preprocessors.
   The problem is, "32_" is not a valid preprocessing token, and we don't
   want extra underscores (e.g., "nlm_32_").  The XCONCAT2 macro will
   cause the inner CONCAT2 macros to be evaluated first, producing
   still-valid pp-tokens.  Then the final concatenation can be done.  */
#undef CONCAT4
#define CONCAT4(a,b,c,d) XCONCAT2(CONCAT2(a,b),CONCAT2(c,d))
#endif

/* This is a utility macro to handle the situation where the code
   wants to place a constant string into the code, followed by a
   comma and then the length of the string.  Doing this by hand
   is error prone, so using this macro is safer.  */
#define STRING_COMMA_LEN(STR) (STR), (sizeof (STR) - 1)
/* Unfortunately it is not possible to use the STRING_COMMA_LEN macro
   to create the arguments to another macro, since the preprocessor
   will mis-count the number of arguments to the outer macro (by not
   evaluating STRING_COMMA_LEN and so missing the comma).  This is a
   problem for example when trying to use STRING_COMMA_LEN to build
   the arguments to the strncmp() macro.  Hence this alternative
   definition of strncmp is provided here.

   Note - these macros do NOT work if STR2 is not a constant string.  */
#define CONST_STRNEQ(STR1,STR2) (strncmp ((STR1), (STR2), sizeof (STR2) - 1) == 0)
  /* strcpy() can have a similar problem, but since we know we are
     copying a constant string, we can use memcpy which will be faster
     since there is no need to check for a NUL byte inside STR.  We
     can also save time if we do not need to copy the terminating NUL.  */
#define LITMEMCPY(DEST,STR2) memcpy ((DEST), (STR2), sizeof (STR2) - 1)
#define LITSTRCPY(DEST,STR2) memcpy ((DEST), (STR2), sizeof (STR2))

/* The word size used by BFD on the host.  This may be 64 with a 32
   bit target if the host is 64 bit, or if other 64 bit targets have
   been selected with --enable-targets, or if --enable-64-bit-bfd.  */
#define BFD_ARCH_SIZE 32

/* The word size of the default bfd target.  */
#define BFD_DEFAULT_TARGET_SIZE @bfd_default_target_size@

#if @BFD_HOST_64_BIT_DEFINED@
#define BFD_HOST_64_BIT int64_t
#define BFD_HOST_U_64_BIT uint64_t
typedef BFD_HOST_64_BIT bfd_int64_t;
typedef BFD_HOST_U_64_BIT bfd_uint64_t;
#endif

# include <inttypes.h>


/* Declaring a type wide enough to hold a host long and a host pointer.  */
#define BFD_HOSTPTR_T @BFD_HOSTPTR_T@
typedef BFD_HOSTPTR_T bfd_hostptr_t;

/* Forward declaration.  */
typedef struct bfd bfd;

/* Boolean type used in bfd.  Too many systems define their own
   versions of "boolean" for us to safely typedef a "boolean" of
   our own.  Using an enum for "bfd_boolean" has its own set of
   problems, with strange looking casts required to avoid warnings
   on some older compilers.  Thus we just use an int.

   General rule: Functions which are bfd_boolean return TRUE on
   success and FALSE on failure (unless they're a predicate).  */

typedef int bfd_boolean;
#undef FALSE
#undef TRUE
#define FALSE 0
#define TRUE 1

/* Represent a target address.  Also used as a generic unsigned type
   which is guaranteed to be big enough to hold any arithmetic types
   we need to deal with.  */
typedef uint32_t bfd_vma;

/* A generic signed type which is guaranteed to be big enough to hold any
   arithmetic types we need to deal with.  Can be assumed to be compatible
   with bfd_vma in the same way that signed and unsigned ints are compatible
   (as parameters, in assignment, etc).  */
typedef int32_t bfd_signed_vma;

typedef uint32_t symvalue;
typedef uint32_t bfd_size_type;

/* Print a bfd_vma x on stream s.  */
#define BFD_VMA_FMT ""
#define fprintf_vma(s,x) fprintf (s, "%08" BFD_VMA_FMT "x", x)
#define sprintf_vma(s,x) sprintf (s, "%08" BFD_VMA_FMT "x", x)

#define HALF_BFD_SIZE_TYPE \
  (((bfd_size_type) 1) << (8 * sizeof (bfd_size_type) / 2))

#ifndef BFD_HOST_64_BIT
/* Fall back on a 32 bit type.  The idea is to make these types always
   available for function return types, but in the case that
   BFD_HOST_64_BIT is undefined such a function should abort or
   otherwise signal an error.  */
typedef bfd_signed_vma bfd_int64_t;
typedef bfd_vma bfd_uint64_t;
#endif

/* An offset into a file.  BFD always uses the largest possible offset
   based on the build time availability of fseek, fseeko, or fseeko64.  */
typedef @bfd_file_ptr@ file_ptr;
typedef unsigned @bfd_file_ptr@ ufile_ptr;

extern void bfd_sprintf_vma (bfd *, char *, bfd_vma);
extern void bfd_fprintf_vma (bfd *, void *, bfd_vma);

#define printf_vma(x) fprintf_vma(stdout,x)
#define bfd_printf_vma(abfd,x) bfd_fprintf_vma (abfd,stdout,x)

typedef unsigned int flagword;	/* 32 bits of flags */
typedef unsigned char bfd_byte;

/* File formats.  */

typedef enum bfd_format
{
  bfd_unknown = 0,	/* File format is unknown.  */
  bfd_object,		/* Linker/assembler/compiler output.  */
  bfd_archive,		/* Object archive file.  */
  bfd_core,		/* Core dump.  */
  bfd_type_end		/* Marks the end; don't use it!  */
}
bfd_format;

/* Symbols and relocation.  */

/* A count of carsyms (canonical archive symbols).  */
typedef uint32_t symindex;

/* How to perform a relocation.  */
typedef const struct reloc_howto_struct reloc_howto_type;

#define BFD_NO_MORE_SYMBOLS ((symindex) ~0)

/* General purpose part of a symbol X;
   target specific parts are in libcoff.h, libaout.h, etc.  */

#define bfd_get_section(x) ((x)->section)
#define bfd_get_output_section(x) ((x)->section->output_section)
#define bfd_set_section(x,y) ((x)->section) = (y)
#define bfd_asymbol_base(x) ((x)->section->vma)
#define bfd_asymbol_value(x) (bfd_asymbol_base(x) + (x)->value)
#define bfd_asymbol_name(x) ((x)->name)
/*Perhaps future: #define bfd_asymbol_bfd(x) ((x)->section->owner)*/
#define bfd_asymbol_bfd(x) ((x)->the_bfd)
#define bfd_asymbol_flavour(x)			\
  (((x)->flags & BSF_SYNTHETIC) != 0		\
   ? bfd_target_unknown_flavour			\
   : bfd_asymbol_bfd (x)->xvec->flavour)

/* A canonical archive symbol.  */
/* This is a type pun with struct ranlib on purpose!  */
typedef struct carsym
{
  char *name;
  file_ptr file_offset;	/* Look here to find the file.  */
}
carsym;			/* To make these you call a carsymogen.  */

/* Used in generating armaps (archive tables of contents).
   Perhaps just a forward definition would do?  */
struct orl		/* Output ranlib.  */
{
  char **name;		/* Symbol name.  */
  union
  {
    file_ptr pos;
    bfd *abfd;
  } u;			/* bfd* or file position.  */
  int namidx;		/* Index into string table.  */
};

/* Linenumber stuff.  */
typedef struct lineno_cache_entry
{
  unsigned int line_number;	/* Linenumber from start of function.  */
  union
  {
    struct bfd_symbol *sym;	/* Function name.  */
    bfd_vma offset;		/* Offset into section.  */
  } u;
}
alent;

/* Object and core file sections.  */
typedef struct bfd_section *sec_ptr;

#define	align_power(addr, align)	\
  (((addr) + ((bfd_vma) 1 << (align)) - 1) & (-((bfd_vma) 1 << (align))))

/* Align an address upward to a boundary, expressed as a number of bytes.
   E.g. align to an 8-byte boundary with argument of 8.  Take care never
   to wrap around if the address is within boundary-1 of the end of the
   address space.  */
#define BFD_ALIGN(this, boundary)					  \
  ((((bfd_vma) (this) + (boundary) - 1) >= (bfd_vma) (this))		  \
   ? (((bfd_vma) (this) + ((boundary) - 1)) & ~ (bfd_vma) ((boundary)-1)) \
   : ~ (bfd_vma) 0)

#define bfd_get_section_name(bfd, ptr) ((void) bfd, (ptr)->name)
#define bfd_get_section_vma(bfd, ptr) ((void) bfd, (ptr)->vma)
#define bfd_get_section_lma(bfd, ptr) ((void) bfd, (ptr)->lma)
#define bfd_get_section_alignment(bfd, ptr) ((void) bfd, \
					     (ptr)->alignment_power)
#define bfd_section_name(bfd, ptr) ((ptr)->name)
#define bfd_section_size(bfd, ptr) ((ptr)->size)
#define bfd_get_section_size(ptr) ((ptr)->size)
#define bfd_section_vma(bfd, ptr) ((ptr)->vma)
#define bfd_section_lma(bfd, ptr) ((ptr)->lma)
#define bfd_section_alignment(bfd, ptr) ((ptr)->alignment_power)
#define bfd_get_section_flags(bfd, ptr) ((void) bfd, (ptr)->flags)
#define bfd_get_section_userdata(bfd, ptr) ((void) bfd, (ptr)->userdata)

#define bfd_is_com_section(ptr) (((ptr)->flags & SEC_IS_COMMON) != 0)

#define bfd_get_section_limit_octets(bfd, sec)			\
  ((bfd)->direction != write_direction && (sec)->rawsize != 0	\
   ? (sec)->rawsize : (sec)->size)

/* Find the address one past the end of SEC.  */
#define bfd_get_section_limit(bfd, sec) \
  (bfd_get_section_limit_octets(bfd, sec) / bfd_octets_per_byte (bfd))

/* Return TRUE if input section SEC has been discarded.  */
#define discarded_section(sec)				\
  (!bfd_is_abs_section (sec)					\
   && bfd_is_abs_section ((sec)->output_section)		\
   && (sec)->sec_info_type != SEC_INFO_TYPE_MERGE		\
   && (sec)->sec_info_type != SEC_INFO_TYPE_JUST_SYMS)

typedef enum bfd_print_symbol
{
  bfd_print_symbol_name,
  bfd_print_symbol_more,
  bfd_print_symbol_all
} bfd_print_symbol_type;

/* Information about a symbol that nm needs.  */

typedef struct _symbol_info
{
  symvalue value;
  char type;
  const char *name;		/* Symbol name.  */
  unsigned char stab_type;	/* Stab type.  */
  char stab_other;		/* Stab other.  */
  short stab_desc;		/* Stab desc.  */
  const char *stab_name;	/* String for stab type.  */
} symbol_info;

/* Get the name of a stabs type code.  */

extern const char *bfd_get_stab_name (int);

/* Hash table routines.  There is no way to free up a hash table.  */

/* An element in the hash table.  Most uses will actually use a larger
   structure, and an instance of this will be the first field.  */

struct bfd_hash_entry
{
  /* Next entry for this hash code.  */
  struct bfd_hash_entry *next;
  /* String being hashed.  */
  const char *string;
  /* Hash code.  This is the full hash code, not the index into the
     table.  */
  uint32_t hash;
};

/* A hash table.  */

struct bfd_hash_table
{
  /* The hash array.  */
  struct bfd_hash_entry **table;
  /* A function used to create new elements in the hash table.  The
     first entry is itself a pointer to an element.  When this
     function is first invoked, this pointer will be NULL.  However,
     having the pointer permits a hierarchy of method functions to be
     built each of which calls the function in the superclass.  Thus
     each function should be written to allocate a new block of memory
     only if the argument is NULL.  */
  struct bfd_hash_entry *(*newfunc)
    (struct bfd_hash_entry *, struct bfd_hash_table *, const char *);
  /* An objalloc for this hash table.  This is a struct objalloc *,
     but we use void * to avoid requiring the inclusion of objalloc.h.  */
  void *memory;
  /* The number of slots in the hash table.  */
  unsigned int size;
  /* The number of entries in the hash table.  */
  unsigned int count;
  /* The size of elements.  */
  unsigned int entsize;
  /* If non-zero, don't grow the hash table.  */
  unsigned int frozen:1;
};

/* Initialize a hash table.  */
extern bfd_boolean bfd_hash_table_init
  (struct bfd_hash_table *,
   struct bfd_hash_entry *(*) (struct bfd_hash_entry *,
			       struct bfd_hash_table *,
			       const char *),
   unsigned int);

/* Initialize a hash table specifying a size.  */
extern bfd_boolean bfd_hash_table_init_n
  (struct bfd_hash_table *,
   struct bfd_hash_entry *(*) (struct bfd_hash_entry *,
			       struct bfd_hash_table *,
			       const char *),
   unsigned int, unsigned int);

/* Free up a hash table.  */
extern void bfd_hash_table_free
  (struct bfd_hash_table *);

/* Look up a string in a hash table.  If CREATE is TRUE, a new entry
   will be created for this string if one does not already exist.  The
   COPY argument must be TRUE if this routine should copy the string
   into newly allocated memory when adding an entry.  */
extern struct bfd_hash_entry *bfd_hash_lookup
  (struct bfd_hash_table *, const char *, bfd_boolean create,
   bfd_boolean copy);

/* Insert an entry in a hash table.  */
extern struct bfd_hash_entry *bfd_hash_insert
  (struct bfd_hash_table *, const char *, uint32_t);

/* Rename an entry in a hash table.  */
extern void bfd_hash_rename
  (struct bfd_hash_table *, const char *, struct bfd_hash_entry *);

/* Replace an entry in a hash table.  */
extern void bfd_hash_replace
  (struct bfd_hash_table *, struct bfd_hash_entry *old,
   struct bfd_hash_entry *nw);

/* Base method for creating a hash table entry.  */
extern struct bfd_hash_entry *bfd_hash_newfunc
  (struct bfd_hash_entry *, struct bfd_hash_table *, const char *);

/* Grab some space for a hash table entry.  */
extern void *bfd_hash_allocate
  (struct bfd_hash_table *, unsigned int);

/* Traverse a hash table in a random order, calling a function on each
   element.  If the function returns FALSE, the traversal stops.  The
   INFO argument is passed to the function.  */
extern void bfd_hash_traverse
  (struct bfd_hash_table *,
   bfd_boolean (*) (struct bfd_hash_entry *, void *),
   void *info);

/* Allows the default size of a hash table to be configured. New hash
   tables allocated using bfd_hash_table_init will be created with
   this size.  */
extern uint32_t bfd_hash_set_default_size (uint32_t);

/* Types of compressed DWARF debug sections.  We currently support
   zlib.  */
enum compressed_debug_section_type
{
  COMPRESS_DEBUG_NONE = 0,
  COMPRESS_DEBUG = 1 << 0,
  COMPRESS_DEBUG_GNU_ZLIB = COMPRESS_DEBUG | 1 << 1,
  COMPRESS_DEBUG_GABI_ZLIB = COMPRESS_DEBUG | 1 << 2
};

/* This structure is used to keep track of stabs in sections
   information while linking.  */

struct stab_info
{
  /* A hash table used to hold stabs strings.  */
  struct bfd_strtab_hash *strings;
  /* The header file hash table.  */
  struct bfd_hash_table includes;
  /* The first .stabstr section.  */
  struct bfd_section *stabstr;
};

#define COFF_SWAP_TABLE (void *) &bfd_coff_std_swap_table

/* User program access to BFD facilities.  */

/* Direct I/O routines, for programs which know more about the object
   file than BFD does.  Use higher level routines if possible.  */

extern bfd_size_type bfd_bread (void *, bfd_size_type, bfd *);
extern bfd_size_type bfd_bwrite (const void *, bfd_size_type, bfd *);
extern int bfd_seek (bfd *, file_ptr, int);
extern file_ptr bfd_tell (bfd *);
extern int bfd_flush (bfd *);
extern int bfd_stat (bfd *, struct stat *);

/* Deprecated old routines.  */
#if __GNUC__
#define bfd_read(BUF, ELTSIZE, NITEMS, ABFD)				\
  (_bfd_warn_deprecated ("bfd_read", __FILE__, __LINE__, __FUNCTION__),	\
   bfd_bread ((BUF), (ELTSIZE) * (NITEMS), (ABFD)))
#define bfd_write(BUF, ELTSIZE, NITEMS, ABFD)				\
  (_bfd_warn_deprecated ("bfd_write", __FILE__, __LINE__, __FUNCTION__), \
   bfd_bwrite ((BUF), (ELTSIZE) * (NITEMS), (ABFD)))
#else
#define bfd_read(BUF, ELTSIZE, NITEMS, ABFD)				\
  (_bfd_warn_deprecated ("bfd_read", (const char *) 0, 0, (const char *) 0), \
   bfd_bread ((BUF), (ELTSIZE) * (NITEMS), (ABFD)))
#define bfd_write(BUF, ELTSIZE, NITEMS, ABFD)				\
  (_bfd_warn_deprecated ("bfd_write", (const char *) 0, 0, (const char *) 0),\
   bfd_bwrite ((BUF), (ELTSIZE) * (NITEMS), (ABFD)))
#endif
extern void _bfd_warn_deprecated (const char *, const char *, int, const char *);

/* Cast from const char * to char * so that caller can assign to
   a char * without a warning.  */
#define bfd_get_filename(abfd) ((char *) (abfd)->filename)
#define bfd_get_cacheable(abfd) ((abfd)->cacheable)
#define bfd_get_format(abfd) ((abfd)->format)
#define bfd_get_target(abfd) ((abfd)->xvec->name)
#define bfd_get_flavour(abfd) ((abfd)->xvec->flavour)
#define bfd_family_coff(abfd) \
  (bfd_get_flavour (abfd) == bfd_target_coff_flavour || \
   bfd_get_flavour (abfd) == bfd_target_xcoff_flavour)
#define bfd_big_endian(abfd) ((abfd)->xvec->byteorder == BFD_ENDIAN_BIG)
#define bfd_little_endian(abfd) ((abfd)->xvec->byteorder == BFD_ENDIAN_LITTLE)
#define bfd_header_big_endian(abfd) \
  ((abfd)->xvec->header_byteorder == BFD_ENDIAN_BIG)
#define bfd_header_little_endian(abfd) \
  ((abfd)->xvec->header_byteorder == BFD_ENDIAN_LITTLE)
#define bfd_get_file_flags(abfd) ((abfd)->flags)
#define bfd_applicable_file_flags(abfd) ((abfd)->xvec->object_flags)
#define bfd_applicable_section_flags(abfd) ((abfd)->xvec->section_flags)
#define bfd_has_map(abfd) ((abfd)->has_armap)
#define bfd_is_thin_archive(abfd) ((abfd)->is_thin_archive)

#define bfd_valid_reloc_types(abfd) ((abfd)->xvec->valid_reloc_types)
#define bfd_usrdata(abfd) ((abfd)->usrdata)

#define bfd_get_start_address(abfd) ((abfd)->start_address)
#define bfd_get_symcount(abfd) ((abfd)->symcount)
#define bfd_get_outsymbols(abfd) ((abfd)->outsymbols)
#define bfd_count_sections(abfd) ((abfd)->section_count)

#define bfd_get_dynamic_symcount(abfd) ((abfd)->dynsymcount)

#define bfd_get_symbol_leading_char(abfd) ((abfd)->xvec->symbol_leading_char)

extern bfd_boolean bfd_cache_close
  (bfd *abfd);
/* NB: This declaration should match the autogenerated one in libbfd.h.  */

extern bfd_boolean bfd_cache_close_all (void);

extern bfd_boolean bfd_record_phdr
  (bfd *, uint32_t, bfd_boolean, flagword, bfd_boolean, bfd_vma,
   bfd_boolean, bfd_boolean, unsigned int, struct bfd_section **);

/* Byte swapping routines.  */

bfd_uint64_t bfd_getb64 (const void *);
bfd_uint64_t bfd_getl64 (const void *);
bfd_int64_t bfd_getb_signed_64 (const void *);
bfd_int64_t bfd_getl_signed_64 (const void *);
bfd_vma bfd_getb32 (const void *);
bfd_vma bfd_getl32 (const void *);
bfd_signed_vma bfd_getb_signed_32 (const void *);
bfd_signed_vma bfd_getl_signed_32 (const void *);
bfd_vma bfd_getb16 (const void *);
bfd_vma bfd_getl16 (const void *);
bfd_signed_vma bfd_getb_signed_16 (const void *);
bfd_signed_vma bfd_getl_signed_16 (const void *);
void bfd_putb64 (bfd_uint64_t, void *);
void bfd_putl64 (bfd_uint64_t, void *);
void bfd_putb32 (bfd_vma, void *);
void bfd_putl32 (bfd_vma, void *);
void bfd_putb24 (bfd_vma, void *);
void bfd_putl24 (bfd_vma, void *);
void bfd_putb16 (bfd_vma, void *);
void bfd_putl16 (bfd_vma, void *);

/* Byte swapping routines which take size and endiannes as arguments.  */

bfd_uint64_t bfd_get_bits (const void *, int, bfd_boolean);
void bfd_put_bits (bfd_uint64_t, void *, int, bfd_boolean);

struct ecoff_debug_info;
struct ecoff_debug_swap;
struct ecoff_extr;
struct bfd_symbol;
struct bfd_link_info;
struct bfd_link_hash_entry;
struct bfd_section_already_linked;
struct bfd_elf_version_tree;

extern bfd_boolean bfd_section_already_linked_table_init (void);
extern void bfd_section_already_linked_table_free (void);
extern bfd_boolean _bfd_handle_already_linked
  (struct bfd_section *, struct bfd_section_already_linked *,
   struct bfd_link_info *);

/* Externally visible ECOFF routines.  */

extern bfd_boolean bfd_ecoff_set_gp_value
  (bfd *abfd, bfd_vma gp_value);
extern bfd_boolean bfd_ecoff_set_regmasks
  (bfd *abfd, uint32_t gprmask, uint32_t fprmask,
   uint32_t *cprmask);
extern void *bfd_ecoff_debug_init
  (bfd *output_bfd, struct ecoff_debug_info *output_debug,
   const struct ecoff_debug_swap *output_swap, struct bfd_link_info *);
extern void bfd_ecoff_debug_free
  (void *handle, bfd *output_bfd, struct ecoff_debug_info *output_debug,
   const struct ecoff_debug_swap *output_swap, struct bfd_link_info *);
extern bfd_boolean bfd_ecoff_debug_accumulate
  (void *handle, bfd *output_bfd, struct ecoff_debug_info *output_debug,
   const struct ecoff_debug_swap *output_swap, bfd *input_bfd,
   struct ecoff_debug_info *input_debug,
   const struct ecoff_debug_swap *input_swap, struct bfd_link_info *);
extern bfd_boolean bfd_ecoff_debug_accumulate_other
  (void *handle, bfd *output_bfd, struct ecoff_debug_info *output_debug,
   const struct ecoff_debug_swap *output_swap, bfd *input_bfd,
   struct bfd_link_info *);
extern bfd_boolean bfd_ecoff_debug_externals
  (bfd *abfd, struct ecoff_debug_info *debug,
   const struct ecoff_debug_swap *swap, bfd_boolean relocatable,
   bfd_boolean (*get_extr) (struct bfd_symbol *, struct ecoff_extr *),
   void (*set_index) (struct bfd_symbol *, bfd_size_type));
extern bfd_boolean bfd_ecoff_debug_one_external
  (bfd *abfd, struct ecoff_debug_info *debug,
   const struct ecoff_debug_swap *swap, const char *name,
   struct ecoff_extr *esym);
extern bfd_size_type bfd_ecoff_debug_size
  (bfd *abfd, struct ecoff_debug_info *debug,
   const struct ecoff_debug_swap *swap);
extern bfd_boolean bfd_ecoff_write_debug
  (bfd *abfd, struct ecoff_debug_info *debug,
   const struct ecoff_debug_swap *swap, file_ptr where);
extern bfd_boolean bfd_ecoff_write_accumulated_debug
  (void *handle, bfd *abfd, struct ecoff_debug_info *debug,
   const struct ecoff_debug_swap *swap,
   struct bfd_link_info *info, file_ptr where);

/* Externally visible ELF routines.  */

struct bfd_link_needed_list
{
  struct bfd_link_needed_list *next;
  bfd *by;
  const char *name;
};

enum dynamic_lib_link_class {
  DYN_NORMAL = 0,
  DYN_AS_NEEDED = 1,
  DYN_DT_NEEDED = 2,
  DYN_NO_ADD_NEEDED = 4,
  DYN_NO_NEEDED = 8
};

enum notice_asneeded_action {
  notice_as_needed,
  notice_not_needed,
  notice_needed
};

extern bfd_boolean bfd_elf_record_link_assignment
  (bfd *, struct bfd_link_info *, const char *, bfd_boolean,
   bfd_boolean);
extern struct bfd_link_needed_list *bfd_elf_get_needed_list
  (bfd *, struct bfd_link_info *);
extern bfd_boolean bfd_elf_get_bfd_needed_list
  (bfd *, struct bfd_link_needed_list **);
extern bfd_boolean bfd_elf_stack_segment_size (bfd *, struct bfd_link_info *,
					       const char *, bfd_vma);
extern bfd_boolean bfd_elf_size_dynamic_sections
  (bfd *, const char *, const char *, const char *, const char *, const char *,
   const char * const *, struct bfd_link_info *, struct bfd_section **);
extern bfd_boolean bfd_elf_size_dynsym_hash_dynstr
  (bfd *, struct bfd_link_info *);
extern void bfd_elf_set_dt_needed_name
  (bfd *, const char *);
extern const char *bfd_elf_get_dt_soname
  (bfd *);
extern void bfd_elf_set_dyn_lib_class
  (bfd *, enum dynamic_lib_link_class);
extern int bfd_elf_get_dyn_lib_class
  (bfd *);
extern struct bfd_link_needed_list *bfd_elf_get_runpath_list
  (bfd *, struct bfd_link_info *);
extern int bfd_elf_discard_info
  (bfd *, struct bfd_link_info *);
extern unsigned int _bfd_elf_default_action_discarded
  (struct bfd_section *);

/* Return an upper bound on the number of bytes required to store a
   copy of ABFD's program header table entries.  Return -1 if an error
   occurs; bfd_get_error will return an appropriate code.  */
extern long bfd_get_elf_phdr_upper_bound
  (bfd *abfd);

/* Copy ABFD's program header table entries to *PHDRS.  The entries
   will be stored as an array of Elf_Internal_Phdr structures, as
   defined in include/elf/internal.h.  To find out how large the
   buffer needs to be, call bfd_get_elf_phdr_upper_bound.

   Return the number of program header table entries read, or -1 if an
   error occurs; bfd_get_error will return an appropriate code.  */
extern int bfd_get_elf_phdrs
  (bfd *abfd, void *phdrs);

/* Create a new BFD as if by bfd_openr.  Rather than opening a file,
   reconstruct an ELF file by reading the segments out of remote
   memory based on the ELF file header at EHDR_VMA and the ELF program
   headers it points to.  If non-zero, SIZE is the known extent of the
   object.  If not null, *LOADBASEP is filled in with the difference
   between the VMAs from which the segments were read, and the VMAs
   the file headers (and hence BFD's idea of each section's VMA) put
   them at.

   The function TARGET_READ_MEMORY is called to copy LEN bytes from
   the remote memory at target address VMA into the local buffer at
   MYADDR; it should return zero on success or an `errno' code on
   failure.  TEMPL must be a BFD for a target with the word size and
   byte order found in the remote memory.  */
extern bfd *bfd_elf_bfd_from_remote_memory
  (bfd *templ, bfd_vma ehdr_vma, bfd_size_type size, bfd_vma *loadbasep,
   int (*target_read_memory) (bfd_vma vma, bfd_byte *myaddr,
			      bfd_size_type len));

extern struct bfd_section *_bfd_elf_tls_setup
  (bfd *, struct bfd_link_info *);

extern struct bfd_section *
_bfd_nearby_section (bfd *, struct bfd_section *, bfd_vma);

extern void _bfd_fix_excluded_sec_syms
  (bfd *, struct bfd_link_info *);

/* SunOS shared library support routines for the linker.  */

extern struct bfd_link_needed_list *bfd_sunos_get_needed_list
  (bfd *, struct bfd_link_info *);
extern bfd_boolean bfd_sunos_record_link_assignment
  (bfd *, struct bfd_link_info *, const char *);
extern bfd_boolean bfd_sunos_size_dynamic_sections
  (bfd *, struct bfd_link_info *, struct bfd_section **,
   struct bfd_section **, struct bfd_section **);

/* mmap hacks */

struct _bfd_window_internal;
typedef struct _bfd_window_internal bfd_window_internal;

typedef struct _bfd_window
{
  /* What the user asked for.  */
  void *data;
  bfd_size_type size;
  /* The actual window used by BFD.  Small user-requested read-only
     regions sharing a page may share a single window into the object
     file.  Read-write versions shouldn't until I've fixed things to
     keep track of which portions have been claimed by the
     application; don't want to give the same region back when the
     application wants two writable copies!  */
  struct _bfd_window_internal *i;
}
bfd_window;

extern void bfd_init_window
  (bfd_window *);
extern void bfd_free_window
  (bfd_window *);
extern bfd_boolean bfd_get_file_window
  (bfd *, file_ptr, bfd_size_type, bfd_window *, bfd_boolean);


/* Externally visible COFF routines.  */
struct internal_syment;
union internal_auxent;

extern void bfd_elf32_arm_init_maps
  (bfd *);

/* ARM Interworking support.  Called from linker.  */
extern bfd_boolean bfd_arm_allocate_interworking_sections
  (struct bfd_link_info *);

extern bfd_boolean bfd_arm_process_before_allocation
  (bfd *, struct bfd_link_info *, int);

extern bfd_boolean bfd_arm_get_bfd_for_interworking
  (bfd *, struct bfd_link_info *);

/* ELF ARM Interworking support.  Called from linker.  */
extern bfd_boolean bfd_elf32_arm_allocate_interworking_sections
  (struct bfd_link_info *);

extern bfd_boolean bfd_elf32_arm_process_before_allocation
  (bfd *, struct bfd_link_info *);

struct elf32_arm_params {
  char *thumb_entry_symbol;
  int byteswap_code;
  int target1_is_rel;
  char * target2_type;
  int fix_v4bx;
  int use_blx;
  int no_enum_size_warning;
  int no_wchar_size_warning;
  int pic_veneer;
  int fix_cortex_a8;
  int fix_arm1176;
  int merge_exidx_entries;
  int cmse_implib;
  bfd *in_implib_bfd;
};

void bfd_elf32_arm_set_target_params
  (bfd *, struct bfd_link_info *, struct elf32_arm_params *);

extern bfd_boolean bfd_elf32_arm_get_bfd_for_interworking
  (bfd *, struct bfd_link_info *);

extern bfd_boolean bfd_elf32_arm_add_glue_sections_to_bfd
  (bfd *, struct bfd_link_info *);

extern void bfd_elf32_arm_keep_private_stub_output_sections
  (struct bfd_link_info *);

/* ELF ARM mapping symbol support.  */
#define BFD_ARM_SPECIAL_SYM_TYPE_MAP	(1 << 0)
#define BFD_ARM_SPECIAL_SYM_TYPE_TAG	(1 << 1)
#define BFD_ARM_SPECIAL_SYM_TYPE_OTHER	(1 << 2)
#define BFD_ARM_SPECIAL_SYM_TYPE_ANY	(~0)

extern bfd_boolean bfd_is_arm_special_symbol_name
  (const char *, int);

extern void bfd_elf32_arm_set_byteswap_code
  (struct bfd_link_info *, int);

extern void bfd_elf32_arm_use_long_plt (void);

/* ARM Note section processing.  */
extern bfd_boolean bfd_arm_merge_machines
  (bfd *, bfd *);

extern bfd_boolean bfd_arm_update_notes
  (bfd *, const char *);

extern unsigned int bfd_arm_get_mach_from_notes
  (bfd *, const char *);

/* ARM stub generation support.  Called from the linker.  */
extern int elf32_arm_setup_section_lists
  (bfd *, struct bfd_link_info *);
extern void elf32_arm_next_input_section
  (struct bfd_link_info *, struct bfd_section *);
extern bfd_boolean elf32_arm_size_stubs
  (bfd *, bfd *, struct bfd_link_info *, bfd_signed_vma,
   struct bfd_section * (*) (const char *, struct bfd_section *,
			     struct bfd_section *, unsigned int),
   void (*) (void));
extern bfd_boolean elf32_arm_build_stubs
  (struct bfd_link_info *);

/* ARM unwind section editing support.  */
extern bfd_boolean elf32_arm_fix_exidx_coverage
(struct bfd_section **, unsigned int, struct bfd_link_info *, bfd_boolean);

/* Extracted from init.c.  */
void bfd_init (void);

/* Extracted from opncls.c.  */
/* Set to N to open the next N BFDs using an alternate id space.  */
extern unsigned int bfd_use_reserved_id;
bfd *bfd_fopen (const char *filename, const char *target,
    const char *mode, int fd);

bfd *bfd_openr (const char *filename, const char *target);

bfd *bfd_fdopenr (const char *filename, const char *target, int fd);

bfd *bfd_openstreamr (const char * filename, const char * target,
    void * stream);

bfd *bfd_openr_iovec (const char *filename, const char *target,
    void *(*open_func) (struct bfd *nbfd,
    void *open_closure),
    void *open_closure,
    file_ptr (*pread_func) (struct bfd *nbfd,
    void *stream,
    void *buf,
    file_ptr nbytes,
    file_ptr offset),
    int (*close_func) (struct bfd *nbfd,
    void *stream),
    int (*stat_func) (struct bfd *abfd,
    void *stream,
    struct stat *sb));

bfd *bfd_openw (const char *filename, const char *target);

bfd_boolean bfd_close (bfd *abfd);

bfd_boolean bfd_close_all_done (bfd *);

bfd *bfd_create (const char *filename, bfd *templ);

bfd_boolean bfd_make_writable (bfd *abfd);

bfd_boolean bfd_make_readable (bfd *abfd);

void *bfd_alloc (bfd *abfd, bfd_size_type wanted);

void *bfd_zalloc (bfd *abfd, bfd_size_type wanted);

uint32_t bfd_calc_gnu_debuglink_crc32
   (uint32_t crc, const unsigned char *buf, bfd_size_type len);

char *bfd_get_debug_link_info (bfd *abfd, uint32_t *crc32_out);

char *bfd_get_alt_debug_link_info (bfd * abfd,
    bfd_size_type *buildid_len,
    bfd_byte **buildid_out);

char *bfd_follow_gnu_debuglink (bfd *abfd, const char *dir);

char *bfd_follow_gnu_debugaltlink (bfd *abfd, const char *dir);

struct bfd_section *bfd_create_gnu_debuglink_section
   (bfd *abfd, const char *filename);

bfd_boolean bfd_fill_in_gnu_debuglink_section
   (bfd *abfd, struct bfd_section *sect, const char *filename);

char *bfd_follow_build_id_debuglink (bfd *abfd, const char *dir);

/* Extracted from libbfd.c.  */

/* Byte swapping macros for user section data.  */

#define bfd_put_8(abfd, val, ptr) \
  ((void) (*((unsigned char *) (ptr)) = (val) & 0xff))
#define bfd_put_signed_8 \
  bfd_put_8
#define bfd_get_8(abfd, ptr) \
  (*(const unsigned char *) (ptr) & 0xff)
#define bfd_get_signed_8(abfd, ptr) \
  (((*(const unsigned char *) (ptr) & 0xff) ^ 0x80) - 0x80)

#define bfd_put_16(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_putx16, ((val),(ptr)))
#define bfd_put_signed_16 \
  bfd_put_16
#define bfd_get_16(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx16, (ptr))
#define bfd_get_signed_16(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx_signed_16, (ptr))

#define bfd_put_32(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_putx32, ((val),(ptr)))
#define bfd_put_signed_32 \
  bfd_put_32
#define bfd_get_32(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx32, (ptr))
#define bfd_get_signed_32(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx_signed_32, (ptr))

#define bfd_put_64(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_putx64, ((val), (ptr)))
#define bfd_put_signed_64 \
  bfd_put_64
#define bfd_get_64(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx64, (ptr))
#define bfd_get_signed_64(abfd, ptr) \
  BFD_SEND (abfd, bfd_getx_signed_64, (ptr))

#define bfd_get(bits, abfd, ptr)                       \
  ((bits) == 8 ? (bfd_vma) bfd_get_8 (abfd, ptr)       \
   : (bits) == 16 ? bfd_get_16 (abfd, ptr)             \
   : (bits) == 32 ? bfd_get_32 (abfd, ptr)             \
   : (bits) == 64 ? bfd_get_64 (abfd, ptr)             \
   : (abort (), (bfd_vma) - 1))

#define bfd_put(bits, abfd, val, ptr)                  \
  ((bits) == 8 ? bfd_put_8  (abfd, val, ptr)           \
   : (bits) == 16 ? bfd_put_16 (abfd, val, ptr)        \
   : (bits) == 32 ? bfd_put_32 (abfd, val, ptr)        \
   : (bits) == 64 ? bfd_put_64 (abfd, val, ptr)        \
   : (abort (), (void) 0))


/* Byte swapping macros for file header data.  */

#define bfd_h_put_8(abfd, val, ptr) \
  bfd_put_8 (abfd, val, ptr)
#define bfd_h_put_signed_8(abfd, val, ptr) \
  bfd_put_8 (abfd, val, ptr)
#define bfd_h_get_8(abfd, ptr) \
  bfd_get_8 (abfd, ptr)
#define bfd_h_get_signed_8(abfd, ptr) \
  bfd_get_signed_8 (abfd, ptr)

#define bfd_h_put_16(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_h_putx16, (val, ptr))
#define bfd_h_put_signed_16 \
  bfd_h_put_16
#define bfd_h_get_16(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx16, (ptr))
#define bfd_h_get_signed_16(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx_signed_16, (ptr))

#define bfd_h_put_32(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_h_putx32, (val, ptr))
#define bfd_h_put_signed_32 \
  bfd_h_put_32
#define bfd_h_get_32(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx32, (ptr))
#define bfd_h_get_signed_32(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx_signed_32, (ptr))

#define bfd_h_put_64(abfd, val, ptr) \
  BFD_SEND (abfd, bfd_h_putx64, (val, ptr))
#define bfd_h_put_signed_64 \
  bfd_h_put_64
#define bfd_h_get_64(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx64, (ptr))
#define bfd_h_get_signed_64(abfd, ptr) \
  BFD_SEND (abfd, bfd_h_getx_signed_64, (ptr))

/* Aliases for the above, which should eventually go away.  */

#define H_PUT_64  bfd_h_put_64
#define H_PUT_32  bfd_h_put_32
#define H_PUT_16  bfd_h_put_16
#define H_PUT_8   bfd_h_put_8
#define H_PUT_S64 bfd_h_put_signed_64
#define H_PUT_S32 bfd_h_put_signed_32
#define H_PUT_S16 bfd_h_put_signed_16
#define H_PUT_S8  bfd_h_put_signed_8
#define H_GET_64  bfd_h_get_64
#define H_GET_32  bfd_h_get_32
#define H_GET_16  bfd_h_get_16
#define H_GET_8   bfd_h_get_8
#define H_GET_S64 bfd_h_get_signed_64
#define H_GET_S32 bfd_h_get_signed_32
#define H_GET_S16 bfd_h_get_signed_16
#define H_GET_S8  bfd_h_get_signed_8


/* Extracted from bfdio.c.  */
long bfd_get_mtime (bfd *abfd);

ufile_ptr bfd_get_size (bfd *abfd);

ufile_ptr bfd_get_file_size (bfd *abfd);

void *bfd_mmap (bfd *abfd, void *addr, bfd_size_type len,
    int prot, int flags, file_ptr offset,
    void **map_addr, bfd_size_type *map_len);

/* Extracted from bfdwin.c.  */
/* Extracted from section.c.  */

typedef struct bfd_section
{
  /* The name of the section; the name isn't a copy, the pointer is
     the same as that passed to bfd_make_section.  */
  const char *name;

  /* A unique sequence number.  */
  unsigned int id;

  /* Which section in the bfd; 0..n-1 as sections are created in a bfd.  */
  unsigned int index;

  /* The next section in the list belonging to the BFD, or NULL.  */
  struct bfd_section *next;

  /* The previous section in the list belonging to the BFD, or NULL.  */
  struct bfd_section *prev;

  /* The field flags contains attributes of the section. Some
     flags are read in from the object file, and some are
     synthesized from other information.  */
  flagword flags;

#define SEC_NO_FLAGS                      0x0

  /* Tells the OS to allocate space for this section when loading.
     This is clear for a section containing debug information only.  */
#define SEC_ALLOC                         0x1

  /* Tells the OS to load the section from the file when loading.
     This is clear for a .bss section.  */
#define SEC_LOAD                          0x2

  /* The section contains data still to be relocated, so there is
     some relocation information too.  */
#define SEC_RELOC                         0x4

  /* A signal to the OS that the section contains read only data.  */
#define SEC_READONLY                      0x8

  /* The section contains code only.  */
#define SEC_CODE                         0x10

  /* The section contains data only.  */
#define SEC_DATA                         0x20

  /* The section will reside in ROM.  */
#define SEC_ROM                          0x40

  /* The section contains constructor information. This section
     type is used by the linker to create lists of constructors and
     destructors used by <<g++>>. When a back end sees a symbol
     which should be used in a constructor list, it creates a new
     section for the type of name (e.g., <<__CTOR_LIST__>>), attaches
     the symbol to it, and builds a relocation. To build the lists
     of constructors, all the linker has to do is catenate all the
     sections called <<__CTOR_LIST__>> and relocate the data
     contained within - exactly the operations it would peform on
     standard data.  */
#define SEC_CONSTRUCTOR                  0x80

  /* The section has contents - a data section could be
     <<SEC_ALLOC>> | <<SEC_HAS_CONTENTS>>; a debug section could be
     <<SEC_HAS_CONTENTS>>  */
#define SEC_HAS_CONTENTS                0x100

  /* An instruction to the linker to not output the section
     even if it has information which would normally be written.  */
#define SEC_NEVER_LOAD                  0x200

  /* The section contains thread local data.  */
#define SEC_THREAD_LOCAL                0x400

  /* The section's size is fixed.  Generic linker code will not
     recalculate it and it is up to whoever has set this flag to
     get the size right.  */
#define SEC_FIXED_SIZE                  0x800

  /* The section contains common symbols (symbols may be defined
     multiple times, the value of a symbol is the amount of
     space it requires, and the largest symbol value is the one
     used).  Most targets have exactly one of these (which we
     translate to bfd_com_section_ptr), but ECOFF has two.  */
#define SEC_IS_COMMON                  0x1000

  /* The section contains only debugging information.  For
     example, this is set for ELF .debug and .stab sections.
     strip tests this flag to see if a section can be
     discarded.  */
#define SEC_DEBUGGING                  0x2000

  /* The contents of this section are held in memory pointed to
     by the contents field.  This is checked by bfd_get_section_contents,
     and the data is retrieved from memory if appropriate.  */
#define SEC_IN_MEMORY                  0x4000

  /* The contents of this section are to be excluded by the
     linker for executable and shared objects unless those
     objects are to be further relocated.  */
#define SEC_EXCLUDE                    0x8000

  /* The contents of this section are to be sorted based on the sum of
     the symbol and addend values specified by the associated relocation
     entries.  Entries without associated relocation entries will be
     appended to the end of the section in an unspecified order.  */
#define SEC_SORT_ENTRIES              0x10000

  /* When linking, duplicate sections of the same name should be
     discarded, rather than being combined into a single section as
     is usually done.  This is similar to how common symbols are
     handled.  See SEC_LINK_DUPLICATES below.  */
#define SEC_LINK_ONCE                 0x20000

  /* If SEC_LINK_ONCE is set, this bitfield describes how the linker
     should handle duplicate sections.  */
#define SEC_LINK_DUPLICATES           0xc0000

  /* This value for SEC_LINK_DUPLICATES means that duplicate
     sections with the same name should simply be discarded.  */
#define SEC_LINK_DUPLICATES_DISCARD       0x0

  /* This value for SEC_LINK_DUPLICATES means that the linker
     should warn if there are any duplicate sections, although
     it should still only link one copy.  */
#define SEC_LINK_DUPLICATES_ONE_ONLY  0x40000

  /* This value for SEC_LINK_DUPLICATES means that the linker
     should warn if any duplicate sections are a different size.  */
#define SEC_LINK_DUPLICATES_SAME_SIZE 0x80000

  /* This value for SEC_LINK_DUPLICATES means that the linker
     should warn if any duplicate sections contain different
     contents.  */
#define SEC_LINK_DUPLICATES_SAME_CONTENTS \
  (SEC_LINK_DUPLICATES_ONE_ONLY | SEC_LINK_DUPLICATES_SAME_SIZE)

  /* This section was created by the linker as part of dynamic
     relocation or other arcane processing.  It is skipped when
     going through the first-pass output, trusting that someone
     else up the line will take care of it later.  */
#define SEC_LINKER_CREATED           0x100000

  /* This section should not be subject to garbage collection.
     Also set to inform the linker that this section should not be
     listed in the link map as discarded.  */
#define SEC_KEEP                     0x200000

  /* This section contains "short" data, and should be placed
     "near" the GP.  */
#define SEC_SMALL_DATA               0x400000

  /* Attempt to merge identical entities in the section.
     Entity size is given in the entsize field.  */
#define SEC_MERGE                    0x800000

  /* If given with SEC_MERGE, entities to merge are zero terminated
     strings where entsize specifies character size instead of fixed
     size entries.  */
#define SEC_STRINGS                 0x1000000

  /* This section contains data about section groups.  */
#define SEC_GROUP                   0x2000000

  /* The section is a COFF shared library section.  This flag is
     only for the linker.  If this type of section appears in
     the input file, the linker must copy it to the output file
     without changing the vma or size.  FIXME: Although this
     was originally intended to be general, it really is COFF
     specific (and the flag was renamed to indicate this).  It
     might be cleaner to have some more general mechanism to
     allow the back end to control what the linker does with
     sections.  */
#define SEC_COFF_SHARED_LIBRARY     0x4000000

  /* This input section should be copied to output in reverse order
     as an array of pointers.  This is for ELF linker internal use
     only.  */
#define SEC_ELF_REVERSE_COPY        0x4000000

  /* This section contains data which may be shared with other
     executables or shared objects. This is for COFF only.  */
#define SEC_COFF_SHARED             0x8000000

  /* This section should be compressed.  This is for ELF linker
     internal use only.  */
#define SEC_ELF_COMPRESS            0x8000000

  /* When a section with this flag is being linked, then if the size of
     the input section is less than a page, it should not cross a page
     boundary.  If the size of the input section is one page or more,
     it should be aligned on a page boundary.  This is for TI
     TMS320C54X only.  */
#define SEC_TIC54X_BLOCK           0x10000000

  /* This section should be renamed.  This is for ELF linker
     internal use only.  */
#define SEC_ELF_RENAME             0x10000000

  /* Conditionally link this section; do not link if there are no
     references found to any symbol in the section.  This is for TI
     TMS320C54X only.  */
#define SEC_TIC54X_CLINK           0x20000000

  /* This section contains vliw code.  This is for Toshiba MeP only.  */
#define SEC_MEP_VLIW               0x20000000

  /* Indicate that section has the no read flag set. This happens
     when memory read flag isn't set. */
#define SEC_COFF_NOREAD            0x40000000

  /* Indicate that section has the purecode flag set.  */
#define SEC_ELF_PURECODE           0x80000000

  /*  End of section flags.  */

  /* Some internal packed boolean fields.  */

  /* See the vma field.  */
  unsigned int user_set_vma : 1;

  /* A mark flag used by some of the linker backends.  */
  unsigned int linker_mark : 1;

  /* Another mark flag used by some of the linker backends.  Set for
     output sections that have an input section.  */
  unsigned int linker_has_input : 1;

  /* Mark flag used by some linker backends for garbage collection.  */
  unsigned int gc_mark : 1;

  /* Section compression status.  */
  unsigned int compress_status : 2;
#define COMPRESS_SECTION_NONE    0
#define COMPRESS_SECTION_DONE    1
#define DECOMPRESS_SECTION_SIZED 2

  /* The following flags are used by the ELF linker. */

  /* Mark sections which have been allocated to segments.  */
  unsigned int segment_mark : 1;

  /* Type of sec_info information.  */
  unsigned int sec_info_type:3;
#define SEC_INFO_TYPE_NONE      0
#define SEC_INFO_TYPE_STABS     1
#define SEC_INFO_TYPE_MERGE     2
#define SEC_INFO_TYPE_EH_FRAME  3
#define SEC_INFO_TYPE_JUST_SYMS 4
#define SEC_INFO_TYPE_TARGET    5
#define SEC_INFO_TYPE_EH_FRAME_ENTRY 6

  /* Nonzero if this section uses RELA relocations, rather than REL.  */
  unsigned int use_rela_p:1;

  /* Bits used by various backends.  The generic code doesn't touch
     these fields.  */

  unsigned int sec_flg0:1;
  unsigned int sec_flg1:1;
  unsigned int sec_flg2:1;
  unsigned int sec_flg3:1;
  unsigned int sec_flg4:1;
  unsigned int sec_flg5:1;

  /* End of internal packed boolean fields.  */

  /*  The virtual memory address of the section - where it will be
      at run time.  The symbols are relocated against this.  The
      user_set_vma flag is maintained by bfd; if it's not set, the
      backend can assign addresses (for example, in <<a.out>>, where
      the default address for <<.data>> is dependent on the specific
      target and various flags).  */
  bfd_vma vma;

  /*  The load address of the section - where it would be in a
      rom image; really only used for writing section header
      information.  */
  bfd_vma lma;

  /* The size of the section in *octets*, as it will be output.
     Contains a value even if the section has no contents (e.g., the
     size of <<.bss>>).  */
  bfd_size_type size;

  /* For input sections, the original size on disk of the section, in
     octets.  This field should be set for any section whose size is
     changed by linker relaxation.  It is required for sections where
     the linker relaxation scheme doesn't cache altered section and
     reloc contents (stabs, eh_frame, SEC_MERGE, some coff relaxing
     targets), and thus the original size needs to be kept to read the
     section multiple times.  For output sections, rawsize holds the
     section size calculated on a previous linker relaxation pass.  */
  bfd_size_type rawsize;

  /* The compressed size of the section in octets.  */
  bfd_size_type compressed_size;

  /* Relaxation table. */
  struct relax_table *relax;

  /* Count of used relaxation table entries. */
  int relax_count;


  /* If this section is going to be output, then this value is the
     offset in *bytes* into the output section of the first byte in the
     input section (byte ==> smallest addressable unit on the
     target).  In most cases, if this was going to start at the
     100th octet (8-bit quantity) in the output section, this value
     would be 100.  However, if the target byte size is 16 bits
     (bfd_octets_per_byte is "2"), this value would be 50.  */
  bfd_vma output_offset;

  /* The output section through which to map on output.  */
  struct bfd_section *output_section;

  /* The alignment requirement of the section, as an exponent of 2 -
     e.g., 3 aligns to 2^3 (or 8).  */
  unsigned int alignment_power;

  /* If an input section, a pointer to a vector of relocation
     records for the data in this section.  */
  struct reloc_cache_entry *relocation;

  /* If an output section, a pointer to a vector of pointers to
     relocation records for the data in this section.  */
  struct reloc_cache_entry **orelocation;

  /* The number of relocation records in one of the above.  */
  unsigned reloc_count;

  /* Information below is back end specific - and not always used
     or updated.  */

  /* File position of section data.  */
  file_ptr filepos;

  /* File position of relocation info.  */
  file_ptr rel_filepos;

  /* File position of line data.  */
  file_ptr line_filepos;

  /* Pointer to data for applications.  */
  void *userdata;

  /* If the SEC_IN_MEMORY flag is set, this points to the actual
     contents.  */
  unsigned char *contents;

  /* Attached line number information.  */
  alent *lineno;

  /* Number of line number records.  */
  unsigned int lineno_count;

  /* Entity size for merging purposes.  */
  unsigned int entsize;

  /* Points to the kept section if this section is a link-once section,
     and is discarded.  */
  struct bfd_section *kept_section;

  /* When a section is being output, this value changes as more
     linenumbers are written out.  */
  file_ptr moving_line_filepos;

  /* What the section number is in the target world.  */
  int target_index;

  void *used_by_bfd;

  /* If this is a constructor section then here is a list of the
     relocations created to relocate items within it.  */
  struct relent_chain *constructor_chain;

  /* The BFD which owns the section.  */
  bfd *owner;

  /* A symbol which points at this section only.  */
  struct bfd_symbol *symbol;
  struct bfd_symbol **symbol_ptr_ptr;

  /* Early in the link process, map_head and map_tail are used to build
     a list of input sections attached to an output section.  Later,
     output sections use these fields for a list of bfd_link_order
     structs.  */
  union {
    struct bfd_link_order *link_order;
    struct bfd_section *s;
  } map_head, map_tail;
} asection;

/* Relax table contains information about instructions which can
   be removed by relaxation -- replacing a long address with a
   short address.  */
struct relax_table {
  /* Address where bytes may be deleted. */
  bfd_vma addr;

  /* Number of bytes to be deleted.  */
  int size;
};

/* Note: the following are provided as inline functions rather than macros
   because not all callers use the return value.  A macro implementation
   would use a comma expression, eg: "((ptr)->foo = val, TRUE)" and some
   compilers will complain about comma expressions that have no effect.  */
static inline bfd_boolean
bfd_set_section_userdata (bfd * abfd ATTRIBUTE_UNUSED, asection * ptr,
                          void * val)
{
  ptr->userdata = val;
  return TRUE;
}

static inline bfd_boolean
bfd_set_section_vma (bfd * abfd ATTRIBUTE_UNUSED, asection * ptr, bfd_vma val)
{
  ptr->vma = ptr->lma = val;
  ptr->user_set_vma = TRUE;
  return TRUE;
}

static inline bfd_boolean
bfd_set_section_alignment (bfd * abfd ATTRIBUTE_UNUSED, asection * ptr,
                           unsigned int val)
{
  ptr->alignment_power = val;
  return TRUE;
}

/* These sections are global, and are managed by BFD.  The application
   and target back end are not permitted to change the values in
   these sections.  */
extern asection _bfd_std_section[4];

#define BFD_ABS_SECTION_NAME "*ABS*"
#define BFD_UND_SECTION_NAME "*UND*"
#define BFD_COM_SECTION_NAME "*COM*"
#define BFD_IND_SECTION_NAME "*IND*"

/* Pointer to the common section.  */
#define bfd_com_section_ptr (&_bfd_std_section[0])
/* Pointer to the undefined section.  */
#define bfd_und_section_ptr (&_bfd_std_section[1])
/* Pointer to the absolute section.  */
#define bfd_abs_section_ptr (&_bfd_std_section[2])
/* Pointer to the indirect section.  */
#define bfd_ind_section_ptr (&_bfd_std_section[3])

#define bfd_is_und_section(sec) ((sec) == bfd_und_section_ptr)
#define bfd_is_abs_section(sec) ((sec) == bfd_abs_section_ptr)
#define bfd_is_ind_section(sec) ((sec) == bfd_ind_section_ptr)

#define bfd_is_const_section(SEC)              \
 (   ((SEC) == bfd_abs_section_ptr)            \
  || ((SEC) == bfd_und_section_ptr)            \
  || ((SEC) == bfd_com_section_ptr)            \
  || ((SEC) == bfd_ind_section_ptr))

/* Macros to handle insertion and deletion of a bfd's sections.  These
   only handle the list pointers, ie. do not adjust section_count,
   target_index etc.  */
#define bfd_section_list_remove(ABFD, S) \
  do                                                   \
    {                                                  \
      asection *_s = S;                                \
      asection *_next = _s->next;                      \
      asection *_prev = _s->prev;                      \
      if (_prev)                                       \
        _prev->next = _next;                           \
      else                                             \
        (ABFD)->sections = _next;                      \
      if (_next)                                       \
        _next->prev = _prev;                           \
      else                                             \
        (ABFD)->section_last = _prev;                  \
    }                                                  \
  while (0)
#define bfd_section_list_append(ABFD, S) \
  do                                                   \
    {                                                  \
      asection *_s = S;                                \
      bfd *_abfd = ABFD;                               \
      _s->next = NULL;                                 \
      if (_abfd->section_last)                         \
        {                                              \
          _s->prev = _abfd->section_last;              \
          _abfd->section_last->next = _s;              \
        }                                              \
      else                                             \
        {                                              \
          _s->prev = NULL;                             \
          _abfd->sections = _s;                        \
        }                                              \
      _abfd->section_last = _s;                        \
    }                                                  \
  while (0)
#define bfd_section_list_prepend(ABFD, S) \
  do                                                   \
    {                                                  \
      asection *_s = S;                                \
      bfd *_abfd = ABFD;                               \
      _s->prev = NULL;                                 \
      if (_abfd->sections)                             \
        {                                              \
          _s->next = _abfd->sections;                  \
          _abfd->sections->prev = _s;                  \
        }                                              \
      else                                             \
        {                                              \
          _s->next = NULL;                             \
          _abfd->section_last = _s;                    \
        }                                              \
      _abfd->sections = _s;                            \
    }                                                  \
  while (0)
#define bfd_section_list_insert_after(ABFD, A, S) \
  do                                                   \
    {                                                  \
      asection *_a = A;                                \
      asection *_s = S;                                \
      asection *_next = _a->next;                      \
      _s->next = _next;                                \
      _s->prev = _a;                                   \
      _a->next = _s;                                   \
      if (_next)                                       \
        _next->prev = _s;                              \
      else                                             \
        (ABFD)->section_last = _s;                     \
    }                                                  \
  while (0)
#define bfd_section_list_insert_before(ABFD, B, S) \
  do                                                   \
    {                                                  \
      asection *_b = B;                                \
      asection *_s = S;                                \
      asection *_prev = _b->prev;                      \
      _s->prev = _prev;                                \
      _s->next = _b;                                   \
      _b->prev = _s;                                   \
      if (_prev)                                       \
        _prev->next = _s;                              \
      else                                             \
        (ABFD)->sections = _s;                         \
    }                                                  \
  while (0)
#define bfd_section_removed_from_list(ABFD, S) \
  ((S)->next == NULL ? (ABFD)->section_last != (S) : (S)->next->prev != (S))

#define BFD_FAKE_SECTION(SEC, SYM, NAME, IDX, FLAGS)                   \
  /* name, id,  index, next, prev, flags, user_set_vma,            */  \
  {  NAME, IDX, 0,     NULL, NULL, FLAGS, 0,                           \
                                                                       \
  /* linker_mark, linker_has_input, gc_mark, decompress_status,    */  \
     0,           0,                1,       0,                        \
                                                                       \
  /* segment_mark, sec_info_type, use_rela_p,                      */  \
     0,            0,             0,                                   \
                                                                       \
  /* sec_flg0, sec_flg1, sec_flg2, sec_flg3, sec_flg4, sec_flg5,   */  \
     0,        0,        0,        0,        0,        0,              \
                                                                       \
  /* vma, lma, size, rawsize, compressed_size, relax, relax_count, */  \
     0,   0,   0,    0,       0,               0,     0,               \
                                                                       \
  /* output_offset, output_section, alignment_power,               */  \
     0,             &SEC,           0,                                 \
                                                                       \
  /* relocation, orelocation, reloc_count, filepos, rel_filepos,   */  \
     NULL,       NULL,        0,           0,       0,                 \
                                                                       \
  /* line_filepos, userdata, contents, lineno, lineno_count,       */  \
     0,            NULL,     NULL,     NULL,   0,                      \
                                                                       \
  /* entsize, kept_section, moving_line_filepos,                    */ \
     0,       NULL,          0,                                        \
                                                                       \
  /* target_index, used_by_bfd, constructor_chain, owner,          */  \
     0,            NULL,        NULL,              NULL,               \
                                                                       \
  /* symbol,                    symbol_ptr_ptr,                    */  \
     (struct bfd_symbol *) SYM, &SEC.symbol,                           \
                                                                       \
  /* map_head, map_tail                                            */  \
     { NULL }, { NULL }                                                \
    }

/* We use a macro to initialize the static asymbol structures because
   traditional C does not permit us to initialize a union member while
   gcc warns if we don't initialize it.
   the_bfd, name, value, attr, section [, udata]  */
#define GLOBAL_SYM_INIT(NAME, SECTION) \
  { 0, NAME, 0, BSF_SECTION_SYM, SECTION, { 0 }}

void bfd_section_list_clear (bfd *);

asection *bfd_get_section_by_name (bfd *abfd, const char *name);

asection *bfd_get_next_section_by_name (bfd *ibfd, asection *sec);

asection *bfd_get_linker_section (bfd *abfd, const char *name);

asection *bfd_get_section_by_name_if
   (bfd *abfd,
    const char *name,
    bfd_boolean (*func) (bfd *abfd, asection *sect, void *obj),
    void *obj);

char *bfd_get_unique_section_name
   (bfd *abfd, const char *templat, int *count);

asection *bfd_make_section_old_way (bfd *abfd, const char *name);

asection *bfd_make_section_anyway_with_flags
   (bfd *abfd, const char *name, flagword flags);

asection *bfd_make_section_anyway (bfd *abfd, const char *name);

asection *bfd_make_section_with_flags
   (bfd *, const char *name, flagword flags);

asection *bfd_make_section (bfd *, const char *name);

bfd_boolean bfd_set_section_flags
   (bfd *abfd, asection *sec, flagword flags);

void bfd_rename_section
   (bfd *abfd, asection *sec, const char *newname);

void bfd_map_over_sections
   (bfd *abfd,
    void (*func) (bfd *abfd, asection *sect, void *obj),
    void *obj);

asection *bfd_sections_find_if
   (bfd *abfd,
    bfd_boolean (*operation) (bfd *abfd, asection *sect, void *obj),
    void *obj);

bfd_boolean bfd_set_section_size
   (bfd *abfd, asection *sec, bfd_size_type val);

bfd_boolean bfd_set_section_contents
   (bfd *abfd, asection *section, const void *data,
    file_ptr offset, bfd_size_type count);

bfd_boolean bfd_get_section_contents
   (bfd *abfd, asection *section, void *location, file_ptr offset,
    bfd_size_type count);

bfd_boolean bfd_malloc_and_get_section
   (bfd *abfd, asection *section, bfd_byte **buf);

bfd_boolean bfd_copy_private_section_data
   (bfd *ibfd, asection *isec, bfd *obfd, asection *osec);

#define bfd_copy_private_section_data(ibfd, isection, obfd, osection) \
       BFD_SEND (obfd, _bfd_copy_private_section_data, \
                 (ibfd, isection, obfd, osection))
bfd_boolean bfd_generic_is_group_section (bfd *, const asection *sec);

bfd_boolean bfd_generic_discard_group (bfd *abfd, asection *group);

/* Extracted from archures.c.  */
enum bfd_architecture
{
  bfd_arch_unknown,   /* File arch not known.  */
  bfd_arch_obscure,   /* Arch known, not one of these.  */
  bfd_arch_arm,       /* Advanced Risc Machines ARM.  */
#define bfd_mach_arm_unknown   0
#define bfd_mach_arm_2         1
#define bfd_mach_arm_2a        2
#define bfd_mach_arm_3         3
#define bfd_mach_arm_3M        4
#define bfd_mach_arm_4         5
#define bfd_mach_arm_4T        6
#define bfd_mach_arm_5         7
#define bfd_mach_arm_5T        8
#define bfd_mach_arm_5TE       9
#define bfd_mach_arm_XScale    10
#define bfd_mach_arm_ep9312    11
#define bfd_mach_arm_iWMMXt    12
#define bfd_mach_arm_iWMMXt2   13
  bfd_arch_last
  };

typedef struct bfd_arch_info
{
  int bits_per_word;
  int bits_per_address;
  int bits_per_byte;
  enum bfd_architecture arch;
  uint32_t mach;
  const char *arch_name;
  const char *printable_name;
  unsigned int section_align_power;
  /* TRUE if this is the default machine for the architecture.
     The default arch should be the first entry for an arch so that
     all the entries for that arch can be accessed via <<next>>.  */
  bfd_boolean the_default;
  const struct bfd_arch_info * (*compatible) (const struct bfd_arch_info *,
                                              const struct bfd_arch_info *);

  bfd_boolean (*scan) (const struct bfd_arch_info *, const char *);

  /* Allocate via bfd_malloc and return a fill buffer of size COUNT.  If
     IS_BIGENDIAN is TRUE, the order of bytes is big endian.  If CODE is
     TRUE, the buffer contains code.  */
  void *(*fill) (bfd_size_type count, bfd_boolean is_bigendian,
                 bfd_boolean code);

  const struct bfd_arch_info *next;
}
bfd_arch_info_type;

const char *bfd_printable_name (bfd *abfd);

const bfd_arch_info_type *bfd_scan_arch (const char *string);

const char **bfd_arch_list (void);

const bfd_arch_info_type *bfd_arch_get_compatible
   (const bfd *abfd, const bfd *bbfd, bfd_boolean accept_unknowns);

void bfd_set_arch_info (bfd *abfd, const bfd_arch_info_type *arg);

bfd_boolean bfd_default_set_arch_mach
   (bfd *abfd, enum bfd_architecture arch, uint32_t mach);

enum bfd_architecture bfd_get_arch (bfd *abfd);

uint32_t bfd_get_mach (bfd *abfd);

unsigned int bfd_arch_bits_per_byte (bfd *abfd);

unsigned int bfd_arch_bits_per_address (bfd *abfd);

const bfd_arch_info_type *bfd_get_arch_info (bfd *abfd);

const bfd_arch_info_type *bfd_lookup_arch
   (enum bfd_architecture arch, uint32_t machine);

const char *bfd_printable_arch_mach
   (enum bfd_architecture arch, uint32_t machine);

unsigned int bfd_octets_per_byte (bfd *abfd);

unsigned int bfd_arch_mach_octets_per_byte
   (enum bfd_architecture arch, uint32_t machine);

/* Extracted from reloc.c.  */

typedef enum bfd_reloc_status
{
  /* No errors detected.  Note - the value 2 is used so that it
     will not be mistaken for the boolean TRUE or FALSE values.  */
  bfd_reloc_ok = 2,

  /* The relocation was performed, but there was an overflow.  */
  bfd_reloc_overflow,

  /* The address to relocate was not within the section supplied.  */
  bfd_reloc_outofrange,

  /* Used by special functions.  */
  bfd_reloc_continue,

  /* Unsupported relocation size requested.  */
  bfd_reloc_notsupported,

  /* Unused.  */
  bfd_reloc_other,

  /* The symbol to relocate against was undefined.  */
  bfd_reloc_undefined,

  /* The relocation was performed, but may not be ok.  If this type is
     returned, the error_message argument to bfd_perform_relocation
     will be set.  */
  bfd_reloc_dangerous
 }
 bfd_reloc_status_type;


typedef struct reloc_cache_entry
{
  /* A pointer into the canonical table of pointers.  */
  struct bfd_symbol **sym_ptr_ptr;

  /* offset in section.  */
  bfd_size_type address;

  /* addend for relocation value.  */
  bfd_vma addend;

  /* Pointer to how to perform the required relocation.  */
  reloc_howto_type *howto;

}
arelent;


enum complain_overflow
{
  /* Do not complain on overflow.  */
  complain_overflow_dont,

  /* Complain if the value overflows when considered as a signed
     number one bit larger than the field.  ie. A bitfield of N bits
     is allowed to represent -2**n to 2**n-1.  */
  complain_overflow_bitfield,

  /* Complain if the value overflows when considered as a signed
     number.  */
  complain_overflow_signed,

  /* Complain if the value overflows when considered as an
     unsigned number.  */
  complain_overflow_unsigned
};
struct bfd_symbol;             /* Forward declaration.  */

struct reloc_howto_struct
{
  /*  The type field has mainly a documentary use - the back end can
      do what it wants with it, though normally the back end's
      external idea of what a reloc number is stored
      in this field.  For example, a PC relative word relocation
      in a coff environment has the type 023 - because that's
      what the outside world calls a R_PCRWORD reloc.  */
  unsigned int type;

  /*  The value the final relocation is shifted right by.  This drops
      unwanted data from the relocation.  */
  unsigned int rightshift;

  /*  The size of the item to be relocated.  This is *not* a
      power-of-two measure.  To get the number of bytes operated
      on by a type of relocation, use bfd_get_reloc_size.  */
  int size;

  /*  The number of bits in the item to be relocated.  This is used
      when doing overflow checking.  */
  unsigned int bitsize;

  /*  The relocation is relative to the field being relocated.  */
  bfd_boolean pc_relative;

  /*  The bit position of the reloc value in the destination.
      The relocated value is left shifted by this amount.  */
  unsigned int bitpos;

  /* What type of overflow error should be checked for when
     relocating.  */
  enum complain_overflow complain_on_overflow;

  /* If this field is non null, then the supplied function is
     called rather than the normal function.  This allows really
     strange relocation methods to be accommodated.  */
  bfd_reloc_status_type (*special_function)
    (bfd *, arelent *, struct bfd_symbol *, void *, asection *,
     bfd *, char **);

  /* The textual name of the relocation type.  */
  char *name;

  /* Some formats record a relocation addend in the section contents
     rather than with the relocation.  For ELF formats this is the
     distinction between USE_REL and USE_RELA (though the code checks
     for USE_REL == 1/0).  The value of this field is TRUE if the
     addend is recorded with the section contents; when performing a
     partial link (ld -r) the section contents (the data) will be
     modified.  The value of this field is FALSE if addends are
     recorded with the relocation (in arelent.addend); when performing
     a partial link the relocation will be modified.
     All relocations for all ELF USE_RELA targets should set this field
     to FALSE (values of TRUE should be looked on with suspicion).
     However, the converse is not true: not all relocations of all ELF
     USE_REL targets set this field to TRUE.  Why this is so is peculiar
     to each particular target.  For relocs that aren't used in partial
     links (e.g. GOT stuff) it doesn't matter what this is set to.  */
  bfd_boolean partial_inplace;

  /* src_mask selects the part of the instruction (or data) to be used
     in the relocation sum.  If the target relocations don't have an
     addend in the reloc, eg. ELF USE_REL, src_mask will normally equal
     dst_mask to extract the addend from the section contents.  If
     relocations do have an addend in the reloc, eg. ELF USE_RELA, this
     field should be zero.  Non-zero values for ELF USE_RELA targets are
     bogus as in those cases the value in the dst_mask part of the
     section contents should be treated as garbage.  */
  bfd_vma src_mask;

  /* dst_mask selects which parts of the instruction (or data) are
     replaced with a relocated value.  */
  bfd_vma dst_mask;

  /* When some formats create PC relative instructions, they leave
     the value of the pc of the place being relocated in the offset
     slot of the instruction, so that a PC relative relocation can
     be made just by adding in an ordinary offset (e.g., sun3 a.out).
     Some formats leave the displacement part of an instruction
     empty (e.g., ELF); this flag signals the fact.  */
  bfd_boolean pcrel_offset;
};

#define HOWTO(C, R, S, B, P, BI, O, SF, NAME, INPLACE, MASKSRC, MASKDST, PC) \
  { (unsigned) C, R, S, B, P, BI, O, SF, NAME, INPLACE, MASKSRC, MASKDST, PC }
#define NEWHOWTO(FUNCTION, NAME, SIZE, REL, IN) \
  HOWTO (0, 0, SIZE, 0, REL, 0, complain_overflow_dont, FUNCTION, \
         NAME, FALSE, 0, 0, IN)

#define EMPTY_HOWTO(C) \
  HOWTO ((C), 0, 0, 0, FALSE, 0, complain_overflow_dont, NULL, \
         NULL, FALSE, 0, 0, FALSE)

#define HOWTO_PREPARE(relocation, symbol)              \
  {                                                    \
    if (symbol != NULL)                                \
      {                                                \
        if (bfd_is_com_section (symbol->section))      \
          {                                            \
            relocation = 0;                            \
          }                                            \
        else                                           \
          {                                            \
            relocation = symbol->value;                \
          }                                            \
      }                                                \
  }

unsigned int bfd_get_reloc_size (reloc_howto_type *);

typedef struct relent_chain
{
  arelent relent;
  struct relent_chain *next;
}
arelent_chain;

bfd_reloc_status_type bfd_check_overflow
   (enum complain_overflow how,
    unsigned int bitsize,
    unsigned int rightshift,
    unsigned int addrsize,
    bfd_vma relocation);

bfd_boolean bfd_reloc_offset_in_range
   (reloc_howto_type *howto,
    bfd *abfd,
    asection *section,
    bfd_size_type offset);

bfd_reloc_status_type bfd_perform_relocation
   (bfd *abfd,
    arelent *reloc_entry,
    void *data,
    asection *input_section,
    bfd *output_bfd,
    char **error_message);

bfd_reloc_status_type bfd_install_relocation
   (bfd *abfd,
    arelent *reloc_entry,
    void *data, bfd_vma data_start,
    asection *input_section,
    char **error_message);

enum bfd_reloc_code_real {
  _dummy_first_bfd_reloc_code_real,


/* Basic absolute relocations of N bits.  */
  BFD_RELOC_64,
  BFD_RELOC_32,
  BFD_RELOC_26,
  BFD_RELOC_24,
  BFD_RELOC_16,
  BFD_RELOC_14,
  BFD_RELOC_8,

/* PC-relative relocations.  Sometimes these are relative to the address
of the relocation itself; sometimes they are relative to the start of
the section containing the relocation.  It depends on the specific target.  */
  BFD_RELOC_64_PCREL,
  BFD_RELOC_32_PCREL,
  BFD_RELOC_24_PCREL,
  BFD_RELOC_16_PCREL,
  BFD_RELOC_12_PCREL,
  BFD_RELOC_8_PCREL,

/* Section relative relocations.  Some targets need this for DWARF2.  */
  BFD_RELOC_32_SECREL,

/* For ELF.  */
  BFD_RELOC_32_GOT_PCREL,
  BFD_RELOC_16_GOT_PCREL,
  BFD_RELOC_8_GOT_PCREL,
  BFD_RELOC_32_GOTOFF,
  BFD_RELOC_16_GOTOFF,
  BFD_RELOC_LO16_GOTOFF,
  BFD_RELOC_HI16_GOTOFF,
  BFD_RELOC_HI16_S_GOTOFF,
  BFD_RELOC_8_GOTOFF,
  BFD_RELOC_64_PLT_PCREL,
  BFD_RELOC_32_PLT_PCREL,
  BFD_RELOC_24_PLT_PCREL,
  BFD_RELOC_16_PLT_PCREL,
  BFD_RELOC_8_PLT_PCREL,
  BFD_RELOC_64_PLTOFF,
  BFD_RELOC_32_PLTOFF,
  BFD_RELOC_16_PLTOFF,
  BFD_RELOC_LO16_PLTOFF,
  BFD_RELOC_HI16_PLTOFF,
  BFD_RELOC_HI16_S_PLTOFF,
  BFD_RELOC_8_PLTOFF,

/* Size relocations.  */
  BFD_RELOC_SIZE32,
  BFD_RELOC_SIZE64,

/* For systems that allocate a Global Pointer register, these are
displacements off that register.  These relocation types are
handled specially, because the value the register will have is
decided relatively late.  */
  BFD_RELOC_GPREL16,
  BFD_RELOC_GPREL32,
  BFD_RELOC_NONE,

/* Linkage-table relative.  */
  BFD_RELOC_32_BASEREL,
  BFD_RELOC_16_BASEREL,
  BFD_RELOC_LO16_BASEREL,
  BFD_RELOC_HI16_BASEREL,
  BFD_RELOC_HI16_S_BASEREL,
  BFD_RELOC_8_BASEREL,
  BFD_RELOC_RVA,

/* The type of reloc used to build a constructor table - at the moment
probably a 32 bit wide absolute relocation, but the target can choose.
It generally does map to one of the other relocation types.  */
  BFD_RELOC_CTOR,

/* ARM 26 bit pc-relative branch.  The lowest two bits must be zero and are
not stored in the instruction.  */
  BFD_RELOC_ARM_PCREL_BRANCH,

/* ARM 26 bit pc-relative branch.  The lowest bit must be zero and is
not stored in the instruction.  The 2nd lowest bit comes from a 1 bit
field in the instruction.  */
  BFD_RELOC_ARM_PCREL_BLX,

/* Thumb 22 bit pc-relative branch.  The lowest bit must be zero and is
not stored in the instruction.  The 2nd lowest bit comes from a 1 bit
field in the instruction.  */
  BFD_RELOC_THUMB_PCREL_BLX,

/* ARM 26-bit pc-relative branch for an unconditional BL or BLX instruction.  */
  BFD_RELOC_ARM_PCREL_CALL,

/* ARM 26-bit pc-relative branch for B or conditional BL instruction.  */
  BFD_RELOC_ARM_PCREL_JUMP,

/* Thumb 7-, 9-, 12-, 20-, 23-, and 25-bit pc-relative branches.
The lowest bit must be zero and is not stored in the instruction.
Note that the corresponding ELF R_ARM_THM_JUMPnn constant has an
"nn" one smaller in all cases.  Note further that BRANCH23
corresponds to R_ARM_THM_CALL.  */
  BFD_RELOC_THUMB_PCREL_BRANCH7,
  BFD_RELOC_THUMB_PCREL_BRANCH9,
  BFD_RELOC_THUMB_PCREL_BRANCH12,
  BFD_RELOC_THUMB_PCREL_BRANCH20,
  BFD_RELOC_THUMB_PCREL_BRANCH23,
  BFD_RELOC_THUMB_PCREL_BRANCH25,

/* 12-bit immediate offset, used in ARM-format ldr and str instructions.  */
  BFD_RELOC_ARM_OFFSET_IMM,

/* 5-bit immediate offset, used in Thumb-format ldr and str instructions.  */
  BFD_RELOC_ARM_THUMB_OFFSET,

/* Pc-relative or absolute relocation depending on target.  Used for
entries in .init_array sections.  */
  BFD_RELOC_ARM_TARGET1,

/* Read-only segment base relative address.  */
  BFD_RELOC_ARM_ROSEGREL32,

/* Data segment base relative address.  */
  BFD_RELOC_ARM_SBREL32,

/* This reloc is used for references to RTTI data from exception handling
tables.  The actual definition depends on the target.  It may be a
pc-relative or some form of GOT-indirect relocation.  */
  BFD_RELOC_ARM_TARGET2,

/* 31-bit PC relative address.  */
  BFD_RELOC_ARM_PREL31,

/* Low and High halfword relocations for MOVW and MOVT instructions.  */
  BFD_RELOC_ARM_MOVW,
  BFD_RELOC_ARM_MOVT,
  BFD_RELOC_ARM_MOVW_PCREL,
  BFD_RELOC_ARM_MOVT_PCREL,
  BFD_RELOC_ARM_THUMB_MOVW,
  BFD_RELOC_ARM_THUMB_MOVT,
  BFD_RELOC_ARM_THUMB_MOVW_PCREL,
  BFD_RELOC_ARM_THUMB_MOVT_PCREL,

/* ARM FDPIC specific relocations.  */
  BFD_RELOC_ARM_GOTFUNCDESC,
  BFD_RELOC_ARM_GOTOFFFUNCDESC,
  BFD_RELOC_ARM_FUNCDESC,
  BFD_RELOC_ARM_FUNCDESC_VALUE,
  BFD_RELOC_ARM_TLS_GD32_FDPIC,
  BFD_RELOC_ARM_TLS_LDM32_FDPIC,
  BFD_RELOC_ARM_TLS_IE32_FDPIC,

/* Relocations for setting up GOTs and PLTs for shared libraries.  */
  BFD_RELOC_ARM_JUMP_SLOT,
  BFD_RELOC_ARM_GLOB_DAT,
  BFD_RELOC_ARM_GOT32,
  BFD_RELOC_ARM_PLT32,
  BFD_RELOC_ARM_RELATIVE,
  BFD_RELOC_ARM_GOTOFF,
  BFD_RELOC_ARM_GOTPC,
  BFD_RELOC_ARM_GOT_PREL,

/* ARM thread-local storage relocations.  */
  BFD_RELOC_ARM_TLS_GD32,
  BFD_RELOC_ARM_TLS_LDO32,
  BFD_RELOC_ARM_TLS_LDM32,
  BFD_RELOC_ARM_TLS_DTPOFF32,
  BFD_RELOC_ARM_TLS_DTPMOD32,
  BFD_RELOC_ARM_TLS_TPOFF32,
  BFD_RELOC_ARM_TLS_IE32,
  BFD_RELOC_ARM_TLS_LE32,
  BFD_RELOC_ARM_TLS_GOTDESC,
  BFD_RELOC_ARM_TLS_CALL,
  BFD_RELOC_ARM_THM_TLS_CALL,
  BFD_RELOC_ARM_TLS_DESCSEQ,
  BFD_RELOC_ARM_THM_TLS_DESCSEQ,
  BFD_RELOC_ARM_TLS_DESC,

/* ARM group relocations.  */
  BFD_RELOC_ARM_ALU_PC_G0_NC,
  BFD_RELOC_ARM_ALU_PC_G0,
  BFD_RELOC_ARM_ALU_PC_G1_NC,
  BFD_RELOC_ARM_ALU_PC_G1,
  BFD_RELOC_ARM_ALU_PC_G2,
  BFD_RELOC_ARM_LDR_PC_G0,
  BFD_RELOC_ARM_LDR_PC_G1,
  BFD_RELOC_ARM_LDR_PC_G2,
  BFD_RELOC_ARM_LDRS_PC_G0,
  BFD_RELOC_ARM_LDRS_PC_G1,
  BFD_RELOC_ARM_LDRS_PC_G2,
  BFD_RELOC_ARM_LDC_PC_G0,
  BFD_RELOC_ARM_LDC_PC_G1,
  BFD_RELOC_ARM_LDC_PC_G2,
  BFD_RELOC_ARM_ALU_SB_G0_NC,
  BFD_RELOC_ARM_ALU_SB_G0,
  BFD_RELOC_ARM_ALU_SB_G1_NC,
  BFD_RELOC_ARM_ALU_SB_G1,
  BFD_RELOC_ARM_ALU_SB_G2,
  BFD_RELOC_ARM_LDR_SB_G0,
  BFD_RELOC_ARM_LDR_SB_G1,
  BFD_RELOC_ARM_LDR_SB_G2,
  BFD_RELOC_ARM_LDRS_SB_G0,
  BFD_RELOC_ARM_LDRS_SB_G1,
  BFD_RELOC_ARM_LDRS_SB_G2,
  BFD_RELOC_ARM_LDC_SB_G0,
  BFD_RELOC_ARM_LDC_SB_G1,
  BFD_RELOC_ARM_LDC_SB_G2,

/* Annotation of BX instructions.  */
  BFD_RELOC_ARM_V4BX,

/* ARM support for STT_GNU_IFUNC.  */
  BFD_RELOC_ARM_IRELATIVE,

/* Thumb1 relocations to support execute-only code.  */
  BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC,
  BFD_RELOC_ARM_THUMB_ALU_ABS_G1_NC,
  BFD_RELOC_ARM_THUMB_ALU_ABS_G2_NC,
  BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC,

/* These relocs are only used within the ARM assembler.  They are not
(at present) written to any object files.  */
  BFD_RELOC_ARM_IMMEDIATE,
  BFD_RELOC_ARM_ADRL_IMMEDIATE,
  BFD_RELOC_ARM_T32_IMMEDIATE,
  BFD_RELOC_ARM_T32_ADD_IMM,
  BFD_RELOC_ARM_T32_IMM12,
  BFD_RELOC_ARM_T32_ADD_PC12,
  BFD_RELOC_ARM_SHIFT_IMM,
  BFD_RELOC_ARM_SMC,
  BFD_RELOC_ARM_HVC,
  BFD_RELOC_ARM_SWI,
  BFD_RELOC_ARM_MULTI,
  BFD_RELOC_ARM_CP_OFF_IMM,
  BFD_RELOC_ARM_CP_OFF_IMM_S2,
  BFD_RELOC_ARM_T32_CP_OFF_IMM,
  BFD_RELOC_ARM_T32_CP_OFF_IMM_S2,
  BFD_RELOC_ARM_ADR_IMM,
  BFD_RELOC_ARM_LDR_IMM,
  BFD_RELOC_ARM_LITERAL,
  BFD_RELOC_ARM_IN_POOL,
  BFD_RELOC_ARM_OFFSET_IMM8,
  BFD_RELOC_ARM_T32_OFFSET_U8,
  BFD_RELOC_ARM_T32_OFFSET_IMM,
  BFD_RELOC_ARM_HWLITERAL,
  BFD_RELOC_ARM_THUMB_ADD,
  BFD_RELOC_ARM_THUMB_IMM,
  BFD_RELOC_ARM_THUMB_SHIFT,

  BFD_RELOC_VTABLE_INHERIT,
  BFD_RELOC_VTABLE_ENTRY,
  BFD_RELOC_UNUSED };

typedef enum bfd_reloc_code_real bfd_reloc_code_real_type;
reloc_howto_type *bfd_reloc_type_lookup
   (bfd *abfd, bfd_reloc_code_real_type code);
reloc_howto_type *bfd_reloc_name_lookup
   (bfd *abfd, const char *reloc_name);

const char *bfd_get_reloc_code_name (bfd_reloc_code_real_type code);

/* Extracted from syms.c.  */

typedef struct bfd_symbol
{
  /* A pointer to the BFD which owns the symbol. This information
     is necessary so that a back end can work out what additional
     information (invisible to the application writer) is carried
     with the symbol.

     This field is *almost* redundant, since you can use section->owner
     instead, except that some symbols point to the global sections
     bfd_{abs,com,und}_section.  This could be fixed by making
     these globals be per-bfd (or per-target-flavor).  FIXME.  */
  struct bfd *the_bfd; /* Use bfd_asymbol_bfd(sym) to access this field.  */

  /* The text of the symbol. The name is left alone, and not copied; the
     application may not alter it.  */
  const char *name;

  /* The value of the symbol.  This really should be a union of a
     numeric value with a pointer, since some flags indicate that
     a pointer to another symbol is stored here.  */
  symvalue value;

  /* Attributes of a symbol.  */
#define BSF_NO_FLAGS            0

  /* The symbol has local scope; <<static>> in <<C>>. The value
     is the offset into the section of the data.  */
#define BSF_LOCAL               (1 << 0)

  /* The symbol has global scope; initialized data in <<C>>. The
     value is the offset into the section of the data.  */
#define BSF_GLOBAL              (1 << 1)

  /* The symbol has global scope and is exported. The value is
     the offset into the section of the data.  */
#define BSF_EXPORT              BSF_GLOBAL /* No real difference.  */

  /* A normal C symbol would be one of:
     <<BSF_LOCAL>>, <<BSF_UNDEFINED>> or <<BSF_GLOBAL>>.  */

  /* The symbol is a debugging record. The value has an arbitrary
     meaning, unless BSF_DEBUGGING_RELOC is also set.  */
#define BSF_DEBUGGING           (1 << 2)

  /* The symbol denotes a function entry point.  Used in ELF,
     perhaps others someday.  */
#define BSF_FUNCTION            (1 << 3)

  /* Used by the linker.  */
#define BSF_KEEP                (1 << 5)

  /* An ELF common symbol.  */
#define BSF_ELF_COMMON          (1 << 6)

  /* A weak global symbol, overridable without warnings by
     a regular global symbol of the same name.  */
#define BSF_WEAK                (1 << 7)

  /* This symbol was created to point to a section, e.g. ELF's
     STT_SECTION symbols.  */
#define BSF_SECTION_SYM         (1 << 8)

  /* The symbol used to be a common symbol, but now it is
     allocated.  */
#define BSF_OLD_COMMON          (1 << 9)

  /* In some files the type of a symbol sometimes alters its
     location in an output file - ie in coff a <<ISFCN>> symbol
     which is also <<C_EXT>> symbol appears where it was
     declared and not at the end of a section.  This bit is set
     by the target BFD part to convey this information.  */
#define BSF_NOT_AT_END          (1 << 10)

  /* Signal that the symbol is the label of constructor section.  */
#define BSF_CONSTRUCTOR         (1 << 11)

  /* Signal that the symbol is a warning symbol.  The name is a
     warning.  The name of the next symbol is the one to warn about;
     if a reference is made to a symbol with the same name as the next
     symbol, a warning is issued by the linker.  */
#define BSF_WARNING             (1 << 12)

  /* Signal that the symbol is indirect.  This symbol is an indirect
     pointer to the symbol with the same name as the next symbol.  */
#define BSF_INDIRECT            (1 << 13)

  /* BSF_FILE marks symbols that contain a file name.  This is used
     for ELF STT_FILE symbols.  */
#define BSF_FILE                (1 << 14)

  /* Symbol is from dynamic linking information.  */
#define BSF_DYNAMIC             (1 << 15)

  /* The symbol denotes a data object.  Used in ELF, and perhaps
     others someday.  */
#define BSF_OBJECT              (1 << 16)

  /* This symbol is a debugging symbol.  The value is the offset
     into the section of the data.  BSF_DEBUGGING should be set
     as well.  */
#define BSF_DEBUGGING_RELOC     (1 << 17)

  /* This symbol is thread local.  Used in ELF.  */
#define BSF_THREAD_LOCAL        (1 << 18)

  /* This symbol represents a complex relocation expression,
     with the expression tree serialized in the symbol name.  */
#define BSF_RELC                (1 << 19)

  /* This symbol represents a signed complex relocation expression,
     with the expression tree serialized in the symbol name.  */
#define BSF_SRELC               (1 << 20)

  /* This symbol was created by bfd_get_synthetic_symtab.  */
#define BSF_SYNTHETIC           (1 << 21)

  /* This symbol is an indirect code object.  Unrelated to BSF_INDIRECT.
     The dynamic linker will compute the value of this symbol by
     calling the function that it points to.  BSF_FUNCTION must
     also be also set.  */
#define BSF_GNU_INDIRECT_FUNCTION (1 << 22)
  /* This symbol is a globally unique data object.  The dynamic linker
     will make sure that in the entire process there is just one symbol
     with this name and type in use.  BSF_OBJECT must also be set.  */
#define BSF_GNU_UNIQUE          (1 << 23)

  flagword flags;

  /* A pointer to the section to which this symbol is
     relative.  This will always be non NULL, there are special
     sections for undefined and absolute symbols.  */
  struct bfd_section *section;

  /* Back end special data.  */
  union
    {
      void *p;
      bfd_vma i;
    }
  udata;
}
asymbol;

#define bfd_get_symtab_upper_bound(abfd) \
       BFD_SEND (abfd, _bfd_get_symtab_upper_bound, (abfd))

bfd_boolean bfd_is_local_label (bfd *abfd, asymbol *sym);

bfd_boolean bfd_is_local_label_name (bfd *abfd, const char *name);

#define bfd_is_local_label_name(abfd, name) \
       BFD_SEND (abfd, _bfd_is_local_label_name, (abfd, name))

bfd_boolean bfd_is_target_special_symbol (bfd *abfd, asymbol *sym);

#define bfd_is_target_special_symbol(abfd, sym) \
       BFD_SEND (abfd, _bfd_is_target_special_symbol, (abfd, sym))

#define bfd_canonicalize_symtab(abfd, location) \
       BFD_SEND (abfd, _bfd_canonicalize_symtab, (abfd, location))

bfd_boolean bfd_set_symtab
   (bfd *abfd, asymbol **location, unsigned int count);

void bfd_print_symbol_vandf (bfd *abfd, void *file, asymbol *symbol);

#define bfd_make_empty_symbol(abfd) \
       BFD_SEND (abfd, _bfd_make_empty_symbol, (abfd))

asymbol *_bfd_generic_make_empty_symbol (bfd *);

#define bfd_make_debug_symbol(abfd,ptr,size) \
       BFD_SEND (abfd, _bfd_make_debug_symbol, (abfd, ptr, size))

int bfd_decode_symclass (asymbol *symbol);

bfd_boolean bfd_is_undefined_symclass (int symclass);

void bfd_symbol_info (asymbol *symbol, symbol_info *ret);

bfd_boolean bfd_copy_private_symbol_data
   (bfd *ibfd, asymbol *isym, bfd *obfd, asymbol *osym);

#define bfd_copy_private_symbol_data(ibfd, isymbol, obfd, osymbol) \
       BFD_SEND (obfd, _bfd_copy_private_symbol_data, \
                 (ibfd, isymbol, obfd, osymbol))

/* Extracted from bfd.c.  */

enum bfd_direction
  {
    no_direction = 0,
    read_direction = 1,
    write_direction = 2,
    both_direction = 3
  };

enum bfd_plugin_format
  {
    bfd_plugin_unknown = 0,
    bfd_plugin_yes = 1,
    bfd_plugin_no = 2
  };

struct bfd_build_id
  {
    bfd_size_type size;
    bfd_byte data[1];
  };

struct bfd
{
  /* The filename the application opened the BFD with.  */
  const char *filename;

  /* A pointer to the target jump table.  */
  const struct bfd_target *xvec;

  /* The IOSTREAM, and corresponding IO vector that provide access
     to the file backing the BFD.  */
  void *iostream;
  const struct bfd_iovec *iovec;

  /* The caching routines use these to maintain a
     least-recently-used list of BFDs.  */
  struct bfd *lru_prev, *lru_next;

  /* Track current file position (or current buffer offset for
     in-memory BFDs).  When a file is closed by the caching routines,
     BFD retains state information on the file here.  */
  ufile_ptr where;

  /* File modified time, if mtime_set is TRUE.  */
  long mtime;

  /* A unique identifier of the BFD  */
  unsigned int id;

  /* The format which belongs to the BFD. (object, core, etc.)  */
  ENUM_BITFIELD (bfd_format) format : 3;

  /* The direction with which the BFD was opened.  */
  ENUM_BITFIELD (bfd_direction) direction : 2;

  /* Format_specific flags.  */
  flagword flags : 20;

  /* Values that may appear in the flags field of a BFD.  These also
     appear in the object_flags field of the bfd_target structure, where
     they indicate the set of flags used by that backend (not all flags
     are meaningful for all object file formats) (FIXME: at the moment,
     the object_flags values have mostly just been copied from backend
     to another, and are not necessarily correct).  */

#define BFD_NO_FLAGS                0x0

  /* BFD contains relocation entries.  */
#define HAS_RELOC                   0x1

  /* BFD is directly executable.  */
#define EXEC_P                      0x2

  /* BFD has line number information (basically used for F_LNNO in a
     COFF header).  */
#define HAS_LINENO                  0x4

  /* BFD has debugging information.  */
#define HAS_DEBUG                  0x08

  /* BFD has symbols.  */
#define HAS_SYMS                   0x10

  /* BFD has local symbols (basically used for F_LSYMS in a COFF
     header).  */
#define HAS_LOCALS                 0x20

  /* BFD is a dynamic object.  */
#define DYNAMIC                    0x40

  /* Text section is write protected (if D_PAGED is not set, this is
     like an a.out NMAGIC file) (the linker sets this by default, but
     clears it for -r or -N).  */
#define WP_TEXT                    0x80

  /* BFD is dynamically paged (this is like an a.out ZMAGIC file) (the
     linker sets this by default, but clears it for -r or -n or -N).  */
#define D_PAGED                   0x100

  /* BFD is relaxable (this means that bfd_relax_section may be able to
     do something) (sometimes bfd_relax_section can do something even if
     this is not set).  */
#define BFD_IS_RELAXABLE          0x200

  /* This may be set before writing out a BFD to request using a
     traditional format.  For example, this is used to request that when
     writing out an a.out object the symbols not be hashed to eliminate
     duplicates.  */
#define BFD_TRADITIONAL_FORMAT    0x400

  /* This flag indicates that the BFD contents are actually cached
     in memory.  If this is set, iostream points to a bfd_in_memory
     struct.  */
#define BFD_IN_MEMORY             0x800

  /* This BFD has been created by the linker and doesn't correspond
     to any input file.  */
#define BFD_LINKER_CREATED       0x1000

  /* This may be set before writing out a BFD to request that it
     be written using values for UIDs, GIDs, timestamps, etc. that
     will be consistent from run to run.  */
#define BFD_DETERMINISTIC_OUTPUT 0x2000

  /* Compress sections in this BFD.  */
#define BFD_COMPRESS             0x4000

  /* Decompress sections in this BFD.  */
#define BFD_DECOMPRESS           0x8000

  /* BFD is a dummy, for plugins.  */
#define BFD_PLUGIN              0x10000

  /* Compress sections in this BFD with SHF_COMPRESSED from gABI.  */
#define BFD_COMPRESS_GABI       0x20000

  /* Convert ELF common symbol type to STT_COMMON or STT_OBJECT in this
     BFD.  */
#define BFD_CONVERT_ELF_COMMON  0x40000

  /* Use the ELF STT_COMMON type in this BFD.  */
#define BFD_USE_ELF_STT_COMMON  0x80000

  /* Flags bits to be saved in bfd_preserve_save.  */
#define BFD_FLAGS_SAVED \
  (BFD_IN_MEMORY | BFD_COMPRESS | BFD_DECOMPRESS | BFD_LINKER_CREATED \
   | BFD_PLUGIN | BFD_COMPRESS_GABI | BFD_CONVERT_ELF_COMMON \
   | BFD_USE_ELF_STT_COMMON)

  /* Flags bits which are for BFD use only.  */
#define BFD_FLAGS_FOR_BFD_USE_MASK \
  (BFD_IN_MEMORY | BFD_COMPRESS | BFD_DECOMPRESS | BFD_LINKER_CREATED \
   | BFD_PLUGIN | BFD_TRADITIONAL_FORMAT | BFD_DETERMINISTIC_OUTPUT \
   | BFD_COMPRESS_GABI | BFD_CONVERT_ELF_COMMON | BFD_USE_ELF_STT_COMMON)

  /* Is the file descriptor being cached?  That is, can it be closed as
     needed, and re-opened when accessed later?  */
  unsigned int cacheable : 1;

  /* Marks whether there was a default target specified when the
     BFD was opened. This is used to select which matching algorithm
     to use to choose the back end.  */
  unsigned int target_defaulted : 1;

  /* ... and here: (``once'' means at least once).  */
  unsigned int opened_once : 1;

  /* Set if we have a locally maintained mtime value, rather than
     getting it from the file each time.  */
  unsigned int mtime_set : 1;

  /* Flag set if symbols from this BFD should not be exported.  */
  unsigned int no_export : 1;

  /* Remember when output has begun, to stop strange things
     from happening.  */
  unsigned int output_has_begun : 1;

  /* Have archive map.  */
  unsigned int has_armap : 1;

  /* Set if this is a thin archive.  */
  unsigned int is_thin_archive : 1;

  /* Set if only required symbols should be added in the link hash table for
     this object.  Used by VMS linkers.  */
  unsigned int selective_search : 1;

  /* Set if this is the linker output BFD.  */
  unsigned int is_linker_output : 1;

  /* Set if this is the linker input BFD.  */
  unsigned int is_linker_input : 1;

  /* If this is an input for a compiler plug-in library.  */
  ENUM_BITFIELD (bfd_plugin_format) plugin_format : 2;

  /* Set if this is a plugin output file.  */
  unsigned int lto_output : 1;

  /* Set to dummy BFD created when claimed by a compiler plug-in
     library.  */
  bfd *plugin_dummy_bfd;

  /* Currently my_archive is tested before adding origin to
     anything. I believe that this can become always an add of
     origin, with origin set to 0 for non archive files.  */
  ufile_ptr origin;

  /* The origin in the archive of the proxy entry.  This will
     normally be the same as origin, except for thin archives,
     when it will contain the current offset of the proxy in the
     thin archive rather than the offset of the bfd in its actual
     container.  */
  ufile_ptr proxy_origin;

  /* A hash table for section names.  */
  struct bfd_hash_table section_htab;

  /* Pointer to linked list of sections.  */
  struct bfd_section *sections;

  /* The last section on the section list.  */
  struct bfd_section *section_last;

  /* The number of sections.  */
  unsigned int section_count;

  /* A field used by _bfd_generic_link_add_archive_symbols.  This will
     be used only for archive elements.  */
  int archive_pass;

  /* Stuff only useful for object files:
     The start address.  */
  bfd_vma start_address;

  /* Symbol table for output BFD (with symcount entries).
     Also used by the linker to cache input BFD symbols.  */
  struct bfd_symbol  **outsymbols;

  /* Used for input and output.  */
  unsigned int symcount;

  /* Used for slurped dynamic symbol tables.  */
  unsigned int dynsymcount;

  /* Pointer to structure which contains architecture information.  */
  const struct bfd_arch_info *arch_info;

  /* Stuff only useful for archives.  */
  void *arelt_data;
  struct bfd *my_archive;      /* The containing archive BFD.  */
  struct bfd *archive_next;    /* The next BFD in the archive.  */
  struct bfd *archive_head;    /* The first BFD in the archive.  */
  struct bfd *nested_archives; /* List of nested archive in a flattened
                                  thin archive.  */

  union {
    /* For input BFDs, a chain of BFDs involved in a link.  */
    struct bfd *next;
    /* For output BFD, the linker hash table.  */
    struct bfd_link_hash_table *hash;
  } link;

  /* Used by the back end to hold private data.  */
  union
    {
      struct aout_data_struct *aout_data;
      struct artdata *aout_ar_data;
      struct coff_tdata *coff_obj_data;
      struct pe_tdata *pe_obj_data;
      struct xcoff_tdata *xcoff_obj_data;
      struct ecoff_tdata *ecoff_obj_data;
      struct srec_data_struct *srec_data;
      struct verilog_data_struct *verilog_data;
      struct ihex_data_struct *ihex_data;
      struct tekhex_data_struct *tekhex_data;
      struct elf_obj_tdata *elf_obj_data;
      struct mmo_data_struct *mmo_data;
      struct sun_core_struct *sun_core_data;
      struct sco5_core_struct *sco5_core_data;
      struct trad_core_struct *trad_core_data;
      struct som_data_struct *som_data;
      struct hpux_core_struct *hpux_core_data;
      struct hppabsd_core_struct *hppabsd_core_data;
      struct sgi_core_struct *sgi_core_data;
      struct lynx_core_struct *lynx_core_data;
      struct osf_core_struct *osf_core_data;
      struct cisco_core_struct *cisco_core_data;
      struct versados_data_struct *versados_data;
      struct netbsd_core_struct *netbsd_core_data;
      struct mach_o_data_struct *mach_o_data;
      struct mach_o_fat_data_struct *mach_o_fat_data;
      struct plugin_data_struct *plugin_data;
      struct bfd_pef_data_struct *pef_data;
      struct bfd_pef_xlib_data_struct *pef_xlib_data;
      struct bfd_sym_data_struct *sym_data;
      void *any;
    }
  tdata;

  /* Used by the application to hold private data.  */
  void *usrdata;

  /* Where all the allocated stuff under this BFD goes.  This is a
     struct objalloc *, but we use void * to avoid requiring the inclusion
     of objalloc.h.  */
  void *memory;

  /* For input BFDs, the build ID, if the object has one. */
  const struct bfd_build_id *build_id;
};

/* See note beside bfd_set_section_userdata.  */
static inline bfd_boolean
bfd_set_cacheable (bfd * abfd, bfd_boolean val)
{
  abfd->cacheable = val;
  return TRUE;
}


typedef enum bfd_error
{
  bfd_error_no_error = 0,
  bfd_error_system_call,
  bfd_error_invalid_target,
  bfd_error_wrong_format,
  bfd_error_wrong_object_format,
  bfd_error_invalid_operation,
  bfd_error_no_memory,
  bfd_error_no_symbols,
  bfd_error_no_armap,
  bfd_error_no_more_archived_files,
  bfd_error_malformed_archive,
  bfd_error_missing_dso,
  bfd_error_file_not_recognized,
  bfd_error_file_ambiguously_recognized,
  bfd_error_no_contents,
  bfd_error_nonrepresentable_section,
  bfd_error_no_debug_section,
  bfd_error_bad_value,
  bfd_error_file_truncated,
  bfd_error_file_too_big,
  bfd_error_on_input,
  bfd_error_invalid_error_code
}
bfd_error_type;

bfd_error_type bfd_get_error (void);

void bfd_set_error (bfd_error_type error_tag);

void bfd_set_input_error (bfd *input, bfd_error_type error_tag);

const char *bfd_errmsg (bfd_error_type error_tag);

void bfd_perror (const char *message);


typedef void (*bfd_error_handler_type) (const char *, va_list);

void _bfd_error_handler (const char *fmt, ...) ATTRIBUTE_PRINTF_1;

bfd_error_handler_type bfd_set_error_handler (bfd_error_handler_type);

void bfd_set_error_program_name (const char *);


typedef void (*bfd_assert_handler_type) (const char *bfd_formatmsg,
                                         const char *bfd_version,
                                         const char *bfd_file,
                                         int bfd_line);

bfd_assert_handler_type bfd_set_assert_handler (bfd_assert_handler_type);

long bfd_get_reloc_upper_bound (bfd *abfd, asection *sect);

long bfd_canonicalize_reloc
   (bfd *abfd, asection *sec, arelent **loc, asymbol **syms);

void bfd_set_reloc
   (bfd *abfd, asection *sec, arelent **rel, unsigned int count);

#define bfd_set_reloc(abfd, asect, location, count) \
       BFD_SEND (abfd, _bfd_set_reloc, (abfd, asect, location, count))
bfd_boolean bfd_set_file_flags (bfd *abfd, flagword flags);

int bfd_get_arch_size (bfd *abfd);

int bfd_get_sign_extend_vma (bfd *abfd);

bfd_boolean bfd_set_start_address (bfd *abfd, bfd_vma vma);

unsigned int bfd_get_gp_size (bfd *abfd);

void bfd_set_gp_size (bfd *abfd, unsigned int i);

bfd_vma bfd_scan_vma (const char *string, const char **end, int base);

bfd_boolean bfd_copy_private_header_data (bfd *ibfd, bfd *obfd);

#define bfd_copy_private_header_data(ibfd, obfd) \
       BFD_SEND (obfd, _bfd_copy_private_header_data, \
                 (ibfd, obfd))
bfd_boolean bfd_copy_private_bfd_data (bfd *ibfd, bfd *obfd);

#define bfd_copy_private_bfd_data(ibfd, obfd) \
       BFD_SEND (obfd, _bfd_copy_private_bfd_data, \
                 (ibfd, obfd))
bfd_boolean bfd_set_private_flags (bfd *abfd, flagword flags);

#define bfd_set_private_flags(abfd, flags) \
       BFD_SEND (abfd, _bfd_set_private_flags, (abfd, flags))
#define bfd_sizeof_headers(abfd, info) \
       BFD_SEND (abfd, _bfd_sizeof_headers, (abfd, info))

#define bfd_find_nearest_line(abfd, sec, syms, off, file, func, line) \
       BFD_SEND (abfd, _bfd_find_nearest_line, \
                 (abfd, syms, sec, off, file, func, line, NULL))

#define bfd_find_nearest_line_discriminator(abfd, sec, syms, off, file, func, \
                                           line, disc) \
       BFD_SEND (abfd, _bfd_find_nearest_line, \
                 (abfd, syms, sec, off, file, func, line, disc))

#define bfd_find_line(abfd, syms, sym, file, line) \
       BFD_SEND (abfd, _bfd_find_line, \
                 (abfd, syms, sym, file, line))

#define bfd_find_inliner_info(abfd, file, func, line) \
       BFD_SEND (abfd, _bfd_find_inliner_info, \
                 (abfd, file, func, line))

#define bfd_debug_info_start(abfd) \
       BFD_SEND (abfd, _bfd_debug_info_start, (abfd))

#define bfd_debug_info_end(abfd) \
       BFD_SEND (abfd, _bfd_debug_info_end, (abfd))

#define bfd_debug_info_accumulate(abfd, section) \
       BFD_SEND (abfd, _bfd_debug_info_accumulate, (abfd, section))

#define bfd_stat_arch_elt(abfd, stat) \
       BFD_SEND (abfd, _bfd_stat_arch_elt,(abfd, stat))

#define bfd_update_armap_timestamp(abfd) \
       BFD_SEND (abfd, _bfd_update_armap_timestamp, (abfd))

#define bfd_set_arch_mach(abfd, arch, mach)\
       BFD_SEND ( abfd, _bfd_set_arch_mach, (abfd, arch, mach))

#define bfd_relax_section(abfd, section, link_info, again) \
       BFD_SEND (abfd, _bfd_relax_section, (abfd, section, link_info, again))

#define bfd_gc_sections(abfd, link_info) \
       BFD_SEND (abfd, _bfd_gc_sections, (abfd, link_info))

#define bfd_lookup_section_flags(link_info, flag_info, section) \
       BFD_SEND (abfd, _bfd_lookup_section_flags, (link_info, flag_info, section))

#define bfd_merge_sections(abfd, link_info) \
       BFD_SEND (abfd, _bfd_merge_sections, (abfd, link_info))

#define bfd_is_group_section(abfd, sec) \
       BFD_SEND (abfd, _bfd_is_group_section, (abfd, sec))

#define bfd_discard_group(abfd, sec) \
       BFD_SEND (abfd, _bfd_discard_group, (abfd, sec))

#define bfd_link_hash_table_create(abfd) \
       BFD_SEND (abfd, _bfd_link_hash_table_create, (abfd))

#define bfd_link_add_symbols(abfd, info) \
       BFD_SEND (abfd, _bfd_link_add_symbols, (abfd, info))

#define bfd_link_just_syms(abfd, sec, info) \
       BFD_SEND (abfd, _bfd_link_just_syms, (sec, info))

#define bfd_final_link(abfd, info) \
       BFD_SEND (abfd, _bfd_final_link, (abfd, info))

#define bfd_free_cached_info(abfd) \
       BFD_SEND (abfd, _bfd_free_cached_info, (abfd))

#define bfd_get_dynamic_symtab_upper_bound(abfd) \
       BFD_SEND (abfd, _bfd_get_dynamic_symtab_upper_bound, (abfd))

#define bfd_print_private_bfd_data(abfd, file)\
       BFD_SEND (abfd, _bfd_print_private_bfd_data, (abfd, file))

#define bfd_canonicalize_dynamic_symtab(abfd, asymbols) \
       BFD_SEND (abfd, _bfd_canonicalize_dynamic_symtab, (abfd, asymbols))

#define bfd_get_synthetic_symtab(abfd, count, syms, dyncount, dynsyms, ret) \
       BFD_SEND (abfd, _bfd_get_synthetic_symtab, (abfd, count, syms, \
                                                   dyncount, dynsyms, ret))

#define bfd_get_dynamic_reloc_upper_bound(abfd) \
       BFD_SEND (abfd, _bfd_get_dynamic_reloc_upper_bound, (abfd))

#define bfd_canonicalize_dynamic_reloc(abfd, arels, asyms) \
       BFD_SEND (abfd, _bfd_canonicalize_dynamic_reloc, (abfd, arels, asyms))

extern bfd_byte *bfd_get_relocated_section_contents
  (bfd *, struct bfd_link_info *, struct bfd_link_order *, bfd_byte *,
   bfd_boolean, asymbol **);

bfd_boolean bfd_alt_mach_code (bfd *abfd, int alternative);

bfd_vma bfd_emul_get_maxpagesize (const char *);

void bfd_emul_set_maxpagesize (const char *, bfd_vma);

bfd_vma bfd_emul_get_commonpagesize (const char *, bfd_boolean);

void bfd_emul_set_commonpagesize (const char *, bfd_vma);

char *bfd_demangle (bfd *, const char *, int);

void bfd_update_compression_header
   (bfd *abfd, bfd_byte *contents, asection *sec);

bfd_boolean bfd_check_compression_header
   (bfd *abfd, bfd_byte *contents, asection *sec,
    bfd_size_type *uncompressed_size);

int bfd_get_compression_header_size (bfd *abfd, asection *sec);

bfd_size_type bfd_convert_section_size
   (bfd *ibfd, asection *isec, bfd *obfd, bfd_size_type size);

bfd_boolean bfd_convert_section_contents
   (bfd *ibfd, asection *isec, bfd *obfd,
    bfd_byte **ptr, bfd_size_type *ptr_size);

/* Extracted from archive.c.  */
symindex bfd_get_next_mapent
   (bfd *abfd, symindex previous, carsym **sym);

bfd_boolean bfd_set_archive_head (bfd *output, bfd *new_head);

bfd *bfd_openr_next_archived_file (bfd *archive, bfd *previous);

/* Extracted from corefile.c.  */
const char *bfd_core_file_failing_command (bfd *abfd);

int bfd_core_file_failing_signal (bfd *abfd);

int bfd_core_file_pid (bfd *abfd);

bfd_boolean core_file_matches_executable_p
   (bfd *core_bfd, bfd *exec_bfd);

bfd_boolean generic_core_file_matches_executable_p
   (bfd *core_bfd, bfd *exec_bfd);

/* Extracted from targets.c.  */
#define BFD_SEND(bfd, message, arglist) \
  ((*((bfd)->xvec->message)) arglist)

#ifdef DEBUG_BFD_SEND
#undef BFD_SEND
#define BFD_SEND(bfd, message, arglist) \
  (((bfd) && (bfd)->xvec && (bfd)->xvec->message) ? \
    ((*((bfd)->xvec->message)) arglist) : \
    (bfd_assert (__FILE__,__LINE__), NULL))
#endif
#define BFD_SEND_FMT(bfd, message, arglist) \
  (((bfd)->xvec->message[(int) ((bfd)->format)]) arglist)

#ifdef DEBUG_BFD_SEND
#undef BFD_SEND_FMT
#define BFD_SEND_FMT(bfd, message, arglist) \
  (((bfd) && (bfd)->xvec && (bfd)->xvec->message) ? \
   (((bfd)->xvec->message[(int) ((bfd)->format)]) arglist) : \
   (bfd_assert (__FILE__,__LINE__), NULL))
#endif

enum bfd_flavour
{
  /* N.B. Update bfd_flavour_name if you change this.  */
  bfd_target_unknown_flavour,
  bfd_target_aout_flavour,
  bfd_target_coff_flavour,
  bfd_target_ecoff_flavour,
  bfd_target_xcoff_flavour,
  bfd_target_elf_flavour,
  bfd_target_tekhex_flavour,
  bfd_target_srec_flavour,
  bfd_target_verilog_flavour,
  bfd_target_ihex_flavour,
  bfd_target_som_flavour,
  bfd_target_os9k_flavour,
  bfd_target_versados_flavour,
  bfd_target_msdos_flavour,
  bfd_target_ovax_flavour,
  bfd_target_evax_flavour,
  bfd_target_mmo_flavour,
  bfd_target_mach_o_flavour,
  bfd_target_pef_flavour,
  bfd_target_pef_xlib_flavour,
  bfd_target_sym_flavour
};

enum bfd_endian { BFD_ENDIAN_BIG, BFD_ENDIAN_LITTLE, BFD_ENDIAN_UNKNOWN };

/* Forward declaration.  */
typedef struct bfd_link_info _bfd_link_info;

/* Forward declaration.  */
typedef struct flag_info flag_info;

typedef struct bfd_target
{
  /* Identifies the kind of target, e.g., SunOS4, Ultrix, etc.  */
  char *name;

 /* The "flavour" of a back end is a general indication about
    the contents of a file.  */
  enum bfd_flavour flavour;

  /* The order of bytes within the data area of a file.  */
  enum bfd_endian byteorder;

 /* The order of bytes within the header parts of a file.  */
  enum bfd_endian header_byteorder;

  /* A mask of all the flags which an executable may have set -
     from the set <<BFD_NO_FLAGS>>, <<HAS_RELOC>>, ...<<D_PAGED>>.  */
  flagword object_flags;

 /* A mask of all the flags which a section may have set - from
    the set <<SEC_NO_FLAGS>>, <<SEC_ALLOC>>, ...<<SET_NEVER_LOAD>>.  */
  flagword section_flags;

 /* The character normally found at the front of a symbol.
    (if any), perhaps `_'.  */
  char symbol_leading_char;

 /* The pad character for file names within an archive header.  */
  char ar_pad_char;

  /* The maximum number of characters in an archive header.  */
  unsigned char ar_max_namelen;

  /* How well this target matches, used to select between various
     possible targets when more than one target matches.  */
  unsigned char match_priority;

  /* Entries for byte swapping for data. These are different from the
     other entry points, since they don't take a BFD as the first argument.
     Certain other handlers could do the same.  */
  bfd_uint64_t   (*bfd_getx64) (const void *);
  bfd_int64_t    (*bfd_getx_signed_64) (const void *);
  void           (*bfd_putx64) (bfd_uint64_t, void *);
  bfd_vma        (*bfd_getx32) (const void *);
  bfd_signed_vma (*bfd_getx_signed_32) (const void *);
  void           (*bfd_putx32) (bfd_vma, void *);
  bfd_vma        (*bfd_getx16) (const void *);
  bfd_signed_vma (*bfd_getx_signed_16) (const void *);
  void           (*bfd_putx16) (bfd_vma, void *);

  /* Byte swapping for the headers.  */
  bfd_uint64_t   (*bfd_h_getx64) (const void *);
  bfd_int64_t    (*bfd_h_getx_signed_64) (const void *);
  void           (*bfd_h_putx64) (bfd_uint64_t, void *);
  bfd_vma        (*bfd_h_getx32) (const void *);
  bfd_signed_vma (*bfd_h_getx_signed_32) (const void *);
  void           (*bfd_h_putx32) (bfd_vma, void *);
  bfd_vma        (*bfd_h_getx16) (const void *);
  bfd_signed_vma (*bfd_h_getx_signed_16) (const void *);
  void           (*bfd_h_putx16) (bfd_vma, void *);

  /* Format dependent routines: these are vectors of entry points
     within the target vector structure, one for each format to check.  */

  /* Check the format of a file being read.  Return a <<bfd_target *>> or zero.  */
  const struct bfd_target *
              (*_bfd_check_format[bfd_type_end]) (bfd *);

  /* Set the format of a file being written.  */
  bfd_boolean (*_bfd_set_format[bfd_type_end]) (bfd *);

  /* Write cached information into a file being written, at <<bfd_close>>.  */
  bfd_boolean (*_bfd_write_contents[bfd_type_end]) (bfd *);


  /* Generic entry points.  */
#define BFD_JUMP_TABLE_GENERIC(NAME) \
  NAME##_close_and_cleanup, \
  NAME##_bfd_free_cached_info, \
  NAME##_new_section_hook, \
  NAME##_get_section_contents, \
  NAME##_get_section_contents_in_window

  /* Called when the BFD is being closed to do any necessary cleanup.  */
  bfd_boolean (*_close_and_cleanup) (bfd *);
  /* Ask the BFD to free all cached information.  */
  bfd_boolean (*_bfd_free_cached_info) (bfd *);
  /* Called when a new section is created.  */
  bfd_boolean (*_new_section_hook) (bfd *, sec_ptr);
  /* Read the contents of a section.  */
  bfd_boolean (*_bfd_get_section_contents) (bfd *, sec_ptr, void *, file_ptr,
                                            bfd_size_type);
  bfd_boolean (*_bfd_get_section_contents_in_window) (bfd *, sec_ptr,
                                                      bfd_window *, file_ptr,
                                                      bfd_size_type);

  /* Entry points to copy private data.  */
#define BFD_JUMP_TABLE_COPY(NAME) \
  NAME##_bfd_copy_private_bfd_data, \
  NAME##_bfd_merge_private_bfd_data, \
  _bfd_generic_init_private_section_data, \
  NAME##_bfd_copy_private_section_data, \
  NAME##_bfd_copy_private_symbol_data, \
  NAME##_bfd_copy_private_header_data, \
  NAME##_bfd_set_private_flags, \
  NAME##_bfd_print_private_bfd_data

  /* Called to copy BFD general private data from one object file
     to another.  */
  bfd_boolean (*_bfd_copy_private_bfd_data) (bfd *, bfd *);
  /* Called to merge BFD general private data from one object file
     to a common output file when linking.  */
  bfd_boolean (*_bfd_merge_private_bfd_data) (bfd *, struct bfd_link_info *);
  /* Called to initialize BFD private section data from one object file
     to another.  */
#define bfd_init_private_section_data(ibfd, isec, obfd, osec, link_info) \
       BFD_SEND (obfd, _bfd_init_private_section_data, \
                 (ibfd, isec, obfd, osec, link_info))
  bfd_boolean (*_bfd_init_private_section_data) (bfd *, sec_ptr, bfd *,
                                                 sec_ptr,
                                                 struct bfd_link_info *);
  /* Called to copy BFD private section data from one object file
     to another.  */
  bfd_boolean (*_bfd_copy_private_section_data) (bfd *, sec_ptr, bfd *,
                                                 sec_ptr);
  /* Called to copy BFD private symbol data from one symbol
     to another.  */
  bfd_boolean (*_bfd_copy_private_symbol_data) (bfd *, asymbol *, bfd *,
                                                asymbol *);
  /* Called to copy BFD private header data from one object file
     to another.  */
  bfd_boolean (*_bfd_copy_private_header_data) (bfd *, bfd *);
  /* Called to set private backend flags.  */
  bfd_boolean (*_bfd_set_private_flags) (bfd *, flagword);

  /* Called to print private BFD data.  */
  bfd_boolean (*_bfd_print_private_bfd_data) (bfd *, void *);

  /* Core file entry points.  */
#define BFD_JUMP_TABLE_CORE(NAME) \
  NAME##_core_file_failing_command, \
  NAME##_core_file_failing_signal, \
  NAME##_core_file_matches_executable_p, \
  NAME##_core_file_pid

  char *      (*_core_file_failing_command) (bfd *);
  int         (*_core_file_failing_signal) (bfd *);
  bfd_boolean (*_core_file_matches_executable_p) (bfd *, bfd *);
  int         (*_core_file_pid) (bfd *);

  /* Archive entry points.  */
#define BFD_JUMP_TABLE_ARCHIVE(NAME) \
  NAME##_slurp_armap, \
  NAME##_slurp_extended_name_table, \
  NAME##_construct_extended_name_table, \
  NAME##_truncate_arname, \
  NAME##_write_armap, \
  NAME##_read_ar_hdr, \
  NAME##_write_ar_hdr, \
  NAME##_openr_next_archived_file, \
  NAME##_get_elt_at_index, \
  NAME##_generic_stat_arch_elt, \
  NAME##_update_armap_timestamp

  bfd_boolean (*_bfd_slurp_armap) (bfd *);
  bfd_boolean (*_bfd_slurp_extended_name_table) (bfd *);
  bfd_boolean (*_bfd_construct_extended_name_table) (bfd *, char **,
                                                     bfd_size_type *,
                                                     const char **);
  void        (*_bfd_truncate_arname) (bfd *, const char *, char *);
  bfd_boolean (*write_armap) (bfd *, unsigned int, struct orl *,
                              unsigned int, int);
  void *      (*_bfd_read_ar_hdr_fn) (bfd *);
  bfd_boolean (*_bfd_write_ar_hdr_fn) (bfd *, bfd *);
  bfd *       (*openr_next_archived_file) (bfd *, bfd *);
#define bfd_get_elt_at_index(b,i) \
       BFD_SEND (b, _bfd_get_elt_at_index, (b,i))
  bfd *       (*_bfd_get_elt_at_index) (bfd *, symindex);
  int         (*_bfd_stat_arch_elt) (bfd *, struct stat *);
  bfd_boolean (*_bfd_update_armap_timestamp) (bfd *);

  /* Entry points used for symbols.  */
#define BFD_JUMP_TABLE_SYMBOLS(NAME) \
  NAME##_get_symtab_upper_bound, \
  NAME##_canonicalize_symtab, \
  NAME##_make_empty_symbol, \
  NAME##_print_symbol, \
  NAME##_get_symbol_info, \
  NAME##_get_symbol_version_string, \
  NAME##_bfd_is_local_label_name, \
  NAME##_bfd_is_target_special_symbol, \
  NAME##_get_lineno, \
  NAME##_find_nearest_line, \
  NAME##_find_line, \
  NAME##_find_inliner_info, \
  NAME##_bfd_make_debug_symbol, \
  NAME##_read_minisymbols, \
  NAME##_minisymbol_to_symbol

  long        (*_bfd_get_symtab_upper_bound) (bfd *);
  long        (*_bfd_canonicalize_symtab) (bfd *, struct bfd_symbol **);
  struct bfd_symbol *
              (*_bfd_make_empty_symbol) (bfd *);
  void        (*_bfd_print_symbol) (bfd *, void *, struct bfd_symbol *,
                                    bfd_print_symbol_type);
#define bfd_print_symbol(b,p,s,e) \
       BFD_SEND (b, _bfd_print_symbol, (b,p,s,e))
  void        (*_bfd_get_symbol_info) (bfd *, struct bfd_symbol *,
                                       symbol_info *);
#define bfd_get_symbol_info(b,p,e) \
       BFD_SEND (b, _bfd_get_symbol_info, (b,p,e))
  const char *(*_bfd_get_symbol_version_string) (bfd *, struct bfd_symbol *,
                                                 bfd_boolean *);
#define bfd_get_symbol_version_string(b,s,h) \
       BFD_SEND (b, _bfd_get_symbol_version_string, (b,s,h))
  bfd_boolean (*_bfd_is_local_label_name) (bfd *, const char *);
  bfd_boolean (*_bfd_is_target_special_symbol) (bfd *, asymbol *);
  alent *     (*_get_lineno) (bfd *, struct bfd_symbol *);
  bfd_boolean (*_bfd_find_nearest_line) (bfd *, struct bfd_symbol **,
                                         struct bfd_section *, bfd_vma,
                                         const char **, const char **,
                                         unsigned int *, unsigned int *);
  bfd_boolean (*_bfd_find_line) (bfd *, struct bfd_symbol **,
                                 struct bfd_symbol *, const char **,
                                 unsigned int *);
  bfd_boolean (*_bfd_find_inliner_info)
    (bfd *, const char **, const char **, unsigned int *);
 /* Back-door to allow format-aware applications to create debug symbols
    while using BFD for everything else.  Currently used by the assembler
    when creating COFF files.  */
  asymbol *   (*_bfd_make_debug_symbol) (bfd *, void *, uint32_t size);
#define bfd_read_minisymbols(b, d, m, s) \
       BFD_SEND (b, _read_minisymbols, (b, d, m, s))
  long        (*_read_minisymbols) (bfd *, bfd_boolean, void **,
                                    unsigned int *);
#define bfd_minisymbol_to_symbol(b, d, m, f) \
       BFD_SEND (b, _minisymbol_to_symbol, (b, d, m, f))
  asymbol *   (*_minisymbol_to_symbol) (bfd *, bfd_boolean, const void *,
                                        asymbol *);

  /* Routines for relocs.  */
#define BFD_JUMP_TABLE_RELOCS(NAME) \
  NAME##_get_reloc_upper_bound, \
  NAME##_canonicalize_reloc, \
  NAME##_set_reloc, \
  NAME##_bfd_reloc_type_lookup, \
  NAME##_bfd_reloc_name_lookup

  long        (*_get_reloc_upper_bound) (bfd *, sec_ptr);
  long        (*_bfd_canonicalize_reloc) (bfd *, sec_ptr, arelent **,
                                          struct bfd_symbol **);
  void        (*_bfd_set_reloc) (bfd *, sec_ptr, arelent **, unsigned int);
  /* See documentation on reloc types.  */
  reloc_howto_type *
              (*reloc_type_lookup) (bfd *, bfd_reloc_code_real_type);
  reloc_howto_type *
              (*reloc_name_lookup) (bfd *, const char *);

  /* Routines used when writing an object file.  */
#define BFD_JUMP_TABLE_WRITE(NAME) \
  NAME##_set_arch_mach, \
  NAME##_set_section_contents

  bfd_boolean (*_bfd_set_arch_mach) (bfd *, enum bfd_architecture,
                                     uint32_t);
  bfd_boolean (*_bfd_set_section_contents) (bfd *, sec_ptr, const void *,
                                            file_ptr, bfd_size_type);

  /* Routines used by the linker.  */
#define BFD_JUMP_TABLE_LINK(NAME) \
  NAME##_sizeof_headers, \
  NAME##_bfd_get_relocated_section_contents, \
  NAME##_bfd_relax_section, \
  NAME##_bfd_link_hash_table_create, \
  NAME##_bfd_link_add_symbols, \
  NAME##_bfd_link_just_syms, \
  NAME##_bfd_copy_link_hash_symbol_type, \
  NAME##_bfd_final_link, \
  NAME##_bfd_link_split_section, \
  NAME##_bfd_link_check_relocs, \
  NAME##_bfd_gc_sections, \
  NAME##_bfd_lookup_section_flags, \
  NAME##_bfd_merge_sections, \
  NAME##_bfd_is_group_section, \
  NAME##_bfd_discard_group, \
  NAME##_section_already_linked, \
  NAME##_bfd_define_common_symbol, \
  NAME##_bfd_link_hide_symbol, \
  NAME##_bfd_define_start_stop

  int         (*_bfd_sizeof_headers) (bfd *, struct bfd_link_info *);
  bfd_byte *  (*_bfd_get_relocated_section_contents) (bfd *,
                                                      struct bfd_link_info *,
                                                      struct bfd_link_order *,
                                                      bfd_byte *, bfd_boolean,
                                                      struct bfd_symbol **);

  bfd_boolean (*_bfd_relax_section) (bfd *, struct bfd_section *,
                                     struct bfd_link_info *, bfd_boolean *);

  /* Create a hash table for the linker.  Different backends store
     different information in this table.  */
  struct bfd_link_hash_table *
              (*_bfd_link_hash_table_create) (bfd *);

  /* Add symbols from this object file into the hash table.  */
  bfd_boolean (*_bfd_link_add_symbols) (bfd *, struct bfd_link_info *);

  /* Indicate that we are only retrieving symbol values from this section.  */
  void        (*_bfd_link_just_syms) (asection *, struct bfd_link_info *);

  /* Copy the symbol type and other attributes for a linker script
     assignment of one symbol to another.  */
#define bfd_copy_link_hash_symbol_type(b, t, f) \
       BFD_SEND (b, _bfd_copy_link_hash_symbol_type, (b, t, f))
  void        (*_bfd_copy_link_hash_symbol_type) (bfd *,
                                                  struct bfd_link_hash_entry *,
                                                  struct bfd_link_hash_entry *);

  /* Do a link based on the link_order structures attached to each
     section of the BFD.  */
  bfd_boolean (*_bfd_final_link) (bfd *, struct bfd_link_info *);

  /* Should this section be split up into smaller pieces during linking.  */
  bfd_boolean (*_bfd_link_split_section) (bfd *, struct bfd_section *);

  /* Check the relocations in the bfd for validity.  */
  bfd_boolean (* _bfd_link_check_relocs)(bfd *, struct bfd_link_info *);

  /* Remove sections that are not referenced from the output.  */
  bfd_boolean (*_bfd_gc_sections) (bfd *, struct bfd_link_info *);

  /* Sets the bitmask of allowed and disallowed section flags.  */
  bfd_boolean (*_bfd_lookup_section_flags) (struct bfd_link_info *,
                                            struct flag_info *, asection *);

  /* Attempt to merge SEC_MERGE sections.  */
  bfd_boolean (*_bfd_merge_sections) (bfd *, struct bfd_link_info *);

  /* Is this section a member of a group?  */
  bfd_boolean (*_bfd_is_group_section) (bfd *, const struct bfd_section *);

  /* Discard members of a group.  */
  bfd_boolean (*_bfd_discard_group) (bfd *, struct bfd_section *);

  /* Check if SEC has been already linked during a reloceatable or
     final link.  */
  bfd_boolean (*_section_already_linked) (bfd *, asection *,
                                          struct bfd_link_info *);

  /* Define a common symbol.  */
  bfd_boolean (*_bfd_define_common_symbol) (bfd *, struct bfd_link_info *,
                                            struct bfd_link_hash_entry *);

  /* Hide a symbol.  */
  void (*_bfd_link_hide_symbol) (bfd *, struct bfd_link_info *,
                                 struct bfd_link_hash_entry *);

  /* Define a __start, __stop, .startof. or .sizeof. symbol.  */
  struct bfd_link_hash_entry *
              (*_bfd_define_start_stop) (struct bfd_link_info *, const char *,
                                         asection *);

  /* Routines to handle dynamic symbols and relocs.  */
#define BFD_JUMP_TABLE_DYNAMIC(NAME) \
  NAME##_get_dynamic_symtab_upper_bound, \
  NAME##_canonicalize_dynamic_symtab, \
  NAME##_get_synthetic_symtab, \
  NAME##_get_dynamic_reloc_upper_bound, \
  NAME##_canonicalize_dynamic_reloc

  /* Get the amount of memory required to hold the dynamic symbols.  */
  long        (*_bfd_get_dynamic_symtab_upper_bound) (bfd *);
  /* Read in the dynamic symbols.  */
  long        (*_bfd_canonicalize_dynamic_symtab) (bfd *, struct bfd_symbol **);
  /* Create synthetized symbols.  */
  long        (*_bfd_get_synthetic_symtab) (bfd *, long, struct bfd_symbol **,
                                            long, struct bfd_symbol **,
                                            struct bfd_symbol **);
  /* Get the amount of memory required to hold the dynamic relocs.  */
  long        (*_bfd_get_dynamic_reloc_upper_bound) (bfd *);
  /* Read in the dynamic relocs.  */
  long        (*_bfd_canonicalize_dynamic_reloc) (bfd *, arelent **,
                                                  struct bfd_symbol **);

  /* Opposite endian version of this target.  */
  const struct bfd_target *alternative_target;

  /* Data for use by back-end routines, which isn't
     generic enough to belong in this structure.  */
  const void *backend_data;

} bfd_target;

bfd_boolean bfd_set_default_target (const char *name);

const bfd_target *bfd_find_target (const char *target_name, bfd *abfd);

const bfd_target *bfd_get_target_info (const char *target_name,
    bfd *abfd,
    bfd_boolean *is_bigendian,
    int *underscoring,
    const char **def_target_arch);
const char ** bfd_target_list (void);

const bfd_target *bfd_iterate_over_targets
   (int (*func) (const bfd_target *, void *),
    void *data);

const char *bfd_flavour_name (enum bfd_flavour flavour);

/* Extracted from format.c.  */
bfd_boolean bfd_check_format (bfd *abfd, bfd_format format);

bfd_boolean bfd_check_format_matches
   (bfd *abfd, bfd_format format, char ***matching);

bfd_boolean bfd_set_format (bfd *abfd, bfd_format format);

const char *bfd_format_string (bfd_format format);

/* Extracted from linker.c.  */
bfd_boolean bfd_link_split_section (bfd *abfd, asection *sec);

#define bfd_link_split_section(abfd, sec) \
       BFD_SEND (abfd, _bfd_link_split_section, (abfd, sec))

bfd_boolean bfd_section_already_linked (bfd *abfd,
    asection *sec,
    struct bfd_link_info *info);

#define bfd_section_already_linked(abfd, sec, info) \
       BFD_SEND (abfd, _section_already_linked, (abfd, sec, info))

bfd_boolean bfd_generic_define_common_symbol
   (bfd *output_bfd, struct bfd_link_info *info,
    struct bfd_link_hash_entry *h);

#define bfd_define_common_symbol(output_bfd, info, h) \
       BFD_SEND (output_bfd, _bfd_define_common_symbol, (output_bfd, info, h))

void _bfd_generic_link_hide_symbol
   (bfd *output_bfd, struct bfd_link_info *info,
    struct bfd_link_hash_entry *h);

#define bfd_link_hide_symbol(output_bfd, info, h) \
       BFD_SEND (output_bfd, _bfd_link_hide_symbol, (output_bfd, info, h))

struct bfd_link_hash_entry *bfd_generic_define_start_stop
   (struct bfd_link_info *info,
    const char *symbol, asection *sec);

#define bfd_define_start_stop(output_bfd, info, symbol, sec) \
       BFD_SEND (output_bfd, _bfd_define_start_stop, (info, symbol, sec))

struct bfd_elf_version_tree * bfd_find_version_for_sym
   (struct bfd_elf_version_tree *verdefs,
    const char *sym_name, bfd_boolean *hide);

bfd_boolean bfd_hide_sym_by_version
   (struct bfd_elf_version_tree *verdefs, const char *sym_name);

bfd_boolean bfd_link_check_relocs
   (bfd *abfd, struct bfd_link_info *info);

bfd_boolean _bfd_generic_link_check_relocs
   (bfd *abfd, struct bfd_link_info *info);

bfd_boolean bfd_merge_private_bfd_data
   (bfd *ibfd, struct bfd_link_info *info);

#define bfd_merge_private_bfd_data(ibfd, info) \
       BFD_SEND ((info)->output_bfd, _bfd_merge_private_bfd_data, \
                 (ibfd, info))
/* Extracted from simple.c.  */
bfd_byte *bfd_simple_get_relocated_section_contents
   (bfd *abfd, asection *sec, bfd_byte *outbuf, asymbol **symbol_table);

/* Extracted from compress.c.  */
bfd_boolean bfd_get_full_section_contents
   (bfd *abfd, asection *section, bfd_byte **ptr);

void bfd_cache_section_contents
   (asection *sec, void *contents);

bfd_boolean bfd_is_section_compressed_with_header
   (bfd *abfd, asection *section,
    int *compression_header_size_p,
    bfd_size_type *uncompressed_size_p);

bfd_boolean bfd_is_section_compressed
   (bfd *abfd, asection *section);

bfd_boolean bfd_init_section_decompress_status
   (bfd *abfd, asection *section);

bfd_boolean bfd_init_section_compress_status
   (bfd *abfd, asection *section);

bfd_boolean bfd_compress_section
   (bfd *abfd, asection *section, bfd_byte *uncompressed_buffer);

#ifdef __cplusplus
}
#endif
#endif
