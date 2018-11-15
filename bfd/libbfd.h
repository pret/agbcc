/* DO NOT EDIT!  -*- buffer-read-only: t -*-  This file is automatically
   generated from "libbfd-in.h", "libbfd.c", "bfdio.c", "bfdwin.c",
   "cache.c", "reloc.c", "archures.c" and "linker.c".
   Run "make headers" in your build bfd/ to regenerate.  */

/* libbfd.h -- Declarations used by bfd library *implementation*.
   (This include file is not for users of the library.)

   Copyright (C) 1990-2018 Free Software Foundation, Inc.

   Written by Cygnus Support.

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
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _LIBBFD_H
#define _LIBBFD_H 1

#ifndef ATTRIBUTE_HIDDEN
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define ATTRIBUTE_HIDDEN __attribute__ ((__visibility__ ("hidden")))
#else
#define ATTRIBUTE_HIDDEN
#endif
#endif

#include "hashtab.h"

#ifdef __cplusplus
extern "C" {
#endif

/* If you want to read and write large blocks, you might want to do it
   in quanta of this amount */
#define DEFAULT_BUFFERSIZE 8192

/* Set a tdata field.  Can't use the other macros for this, since they
   do casts, and casting to the left of assignment isn't portable.  */
#define set_tdata(bfd, v) ((bfd)->tdata.any = (v))

/* If BFD_IN_MEMORY is set for a BFD, then the iostream fields points
   to an instance of this structure.  */

struct bfd_in_memory
{
  /* Size of buffer.  */
  bfd_size_type size;
  /* Buffer holding contents of BFD.  */
  bfd_byte *buffer;
};

struct section_hash_entry
{
  struct bfd_hash_entry root;
  asection section;
};

/* Unique section id.  */
extern unsigned int _bfd_section_id ATTRIBUTE_HIDDEN;

/* tdata for an archive.  For an input archive, cache
   needs to be free()'d.  For an output archive, symdefs do.  */

struct artdata
{
  file_ptr first_file_filepos;
  /* Speed up searching the armap */
  htab_t cache;
  bfd *archive_head;		/* Only interesting in output routines.  */
  carsym *symdefs;		/* The symdef entries.  */
  symindex symdef_count;	/* How many there are.  */
  char *extended_names;		/* Clever intel extension.  */
  bfd_size_type extended_names_size; /* Size of extended names.  */
  /* When more compilers are standard C, this can be a time_t.  */
  long  armap_timestamp;	/* Timestamp value written into armap.
				   This is used for BSD archives to check
				   that the timestamp is recent enough
				   for the BSD linker to not complain,
				   just before we finish writing an
				   archive.  */
  file_ptr armap_datepos;	/* Position within archive to seek to
				   rewrite the date field.  */
  void *tdata;			/* Backend specific information.  */
};

#define bfd_ardata(bfd) ((bfd)->tdata.aout_ar_data)

/* Goes in bfd's arelt_data slot */
struct areltdata
{
  char * arch_header;		/* It's actually a string.  */
  bfd_size_type parsed_size;	/* Octets of filesize not including ar_hdr.  */
  bfd_size_type extra_size;	/* BSD4.4: extra bytes after the header.  */
  char *filename;		/* Null-terminated.  */
  file_ptr origin;		/* For element of a thin archive.  */
  void *parent_cache;		/* Where and how to find this member.  */
  file_ptr key;
};

#define arelt_size(bfd) (((struct areltdata *)((bfd)->arelt_data))->parsed_size)

extern void *bfd_malloc
  (bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_realloc
  (void *, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_realloc_or_free
  (void *, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_zmalloc
  (bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_malloc2
  (bfd_size_type, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_realloc2
  (void *, bfd_size_type, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_zmalloc2
  (bfd_size_type, bfd_size_type) ATTRIBUTE_HIDDEN;

/* These routines allocate and free things on the BFD's objalloc.  */

extern void *bfd_alloc2
  (bfd *, bfd_size_type, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void *bfd_zalloc2
  (bfd *, bfd_size_type, bfd_size_type) ATTRIBUTE_HIDDEN;
extern void bfd_release
  (bfd *, void *) ATTRIBUTE_HIDDEN;

extern bfd * _bfd_create_empty_archive_element_shell
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd * _bfd_look_for_bfd_in_cache
  (bfd *, file_ptr) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_add_bfd_to_archive_cache
  (bfd *, file_ptr, bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_generic_mkarchive
  (bfd *) ATTRIBUTE_HIDDEN;
extern char *_bfd_append_relative_path
  (bfd *, char *) ATTRIBUTE_HIDDEN;
extern const bfd_target *bfd_generic_archive_p
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean bfd_slurp_armap
  (bfd *) ATTRIBUTE_HIDDEN;
#define bfd_slurp_bsd_armap bfd_slurp_armap
#define bfd_slurp_coff_armap bfd_slurp_armap
extern bfd_boolean _bfd_archive_64_bit_slurp_armap
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_archive_64_bit_write_armap
  (bfd *, unsigned int, struct orl *, unsigned int, int) ATTRIBUTE_HIDDEN;
#define _bfd_archive_64_bit_slurp_extended_name_table \
  _bfd_slurp_extended_name_table
#define _bfd_archive_64_bit_construct_extended_name_table \
  _bfd_archive_coff_construct_extended_name_table
#define _bfd_archive_64_bit_truncate_arname \
  bfd_dont_truncate_arname
#define _bfd_archive_64_bit_read_ar_hdr \
  _bfd_generic_read_ar_hdr
#define _bfd_archive_64_bit_write_ar_hdr \
  _bfd_generic_write_ar_hdr
#define _bfd_archive_64_bit_openr_next_archived_file \
  bfd_generic_openr_next_archived_file
#define _bfd_archive_64_bit_get_elt_at_index \
  _bfd_generic_get_elt_at_index
#define _bfd_archive_64_bit_generic_stat_arch_elt \
  bfd_generic_stat_arch_elt
#define _bfd_archive_64_bit_update_armap_timestamp _bfd_bool_bfd_true

extern bfd_boolean _bfd_slurp_extended_name_table
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_construct_extended_name_table
  (bfd *, bfd_boolean, char **, bfd_size_type *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_write_archive_contents
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_compute_and_write_armap
  (bfd *, unsigned int) ATTRIBUTE_HIDDEN;
extern bfd *_bfd_get_elt_at_filepos
  (bfd *, file_ptr) ATTRIBUTE_HIDDEN;
extern bfd *_bfd_generic_get_elt_at_index
  (bfd *, symindex) ATTRIBUTE_HIDDEN;
extern bfd * _bfd_new_bfd
  (void) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_free_cached_info
  (bfd *) ATTRIBUTE_HIDDEN;

extern bfd_boolean _bfd_bool_bfd_asymbol_false
  (bfd *, asymbol *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_false_error
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_true
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_link_true
  (bfd *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_bfd_true
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_uint_true
  (bfd *, unsigned int) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_asection_bfd_asection_true
  (bfd *, asection *, bfd *, asection *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_asymbol_bfd_asymbol_true
  (bfd *, asymbol *, bfd *, asymbol *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_bool_bfd_ptr_true
  (bfd *, void *) ATTRIBUTE_HIDDEN;
extern void *_bfd_ptr_bfd_null_error
  (bfd *) ATTRIBUTE_HIDDEN;
extern int _bfd_int_bfd_0
  (bfd *) ATTRIBUTE_HIDDEN;
extern unsigned int _bfd_uint_bfd_0
  (bfd *) ATTRIBUTE_HIDDEN;
extern long _bfd_long_bfd_0
  (bfd *) ATTRIBUTE_HIDDEN;
extern long _bfd_long_bfd_n1_error
  (bfd *) ATTRIBUTE_HIDDEN;
extern void _bfd_void_bfd
  (bfd *) ATTRIBUTE_HIDDEN;
extern void _bfd_void_bfd_link
  (bfd *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;
extern void _bfd_void_bfd_asection
  (bfd *, asection *) ATTRIBUTE_HIDDEN;

extern bfd *_bfd_new_bfd_contained_in
  (bfd *) ATTRIBUTE_HIDDEN;
extern const bfd_target *_bfd_dummy_target
  (bfd *) ATTRIBUTE_HIDDEN;

extern void bfd_dont_truncate_arname
  (bfd *, const char *, char *) ATTRIBUTE_HIDDEN;
extern void bfd_bsd_truncate_arname
  (bfd *, const char *, char *) ATTRIBUTE_HIDDEN;
extern void bfd_gnu_truncate_arname
  (bfd *, const char *, char *) ATTRIBUTE_HIDDEN;

extern bfd_boolean _bfd_coff_write_armap
  (bfd *, unsigned int, struct orl *, unsigned int, int) ATTRIBUTE_HIDDEN;

extern void *_bfd_generic_read_ar_hdr
  (bfd *) ATTRIBUTE_HIDDEN;
extern void _bfd_ar_spacepad
  (char *, size_t, const char *, long) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_ar_sizepad
  (char *, size_t, bfd_size_type) ATTRIBUTE_HIDDEN;

extern void *_bfd_generic_read_ar_hdr_mag
  (bfd *, const char *) ATTRIBUTE_HIDDEN;

extern bfd_boolean _bfd_generic_write_ar_hdr
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;

extern bfd * bfd_generic_openr_next_archived_file
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;

extern int bfd_generic_stat_arch_elt
  (bfd *, struct stat *) ATTRIBUTE_HIDDEN;

#define _bfd_read_ar_hdr(abfd) \
	BFD_SEND (abfd, _bfd_read_ar_hdr_fn, (abfd))
#define _bfd_write_ar_hdr(archive, abfd)	 \
	BFD_SEND (abfd, _bfd_write_ar_hdr_fn, (archive, abfd))

/* Generic routines to use for BFD_JUMP_TABLE_GENERIC.  Use
   BFD_JUMP_TABLE_GENERIC (_bfd_generic).  */

#define _bfd_generic_close_and_cleanup _bfd_archive_close_and_cleanup
extern bfd_boolean _bfd_archive_close_and_cleanup
  (bfd *) ATTRIBUTE_HIDDEN;
#define _bfd_generic_bfd_free_cached_info _bfd_bool_bfd_true
extern bfd_boolean _bfd_generic_new_section_hook
  (bfd *, asection *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_generic_get_section_contents
  (bfd *, asection *, void *, file_ptr, bfd_size_type) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_generic_get_section_contents_in_window
  (bfd *, asection *, bfd_window *, file_ptr, bfd_size_type) ATTRIBUTE_HIDDEN;

/* Generic routines to use for BFD_JUMP_TABLE_COPY.  Use
   BFD_JUMP_TABLE_COPY (_bfd_generic).  */

#define _bfd_generic_bfd_copy_private_bfd_data _bfd_bool_bfd_bfd_true
#define _bfd_generic_bfd_merge_private_bfd_data \
  _bfd_bool_bfd_link_true
#define _bfd_generic_bfd_set_private_flags _bfd_bool_bfd_uint_true
#define _bfd_generic_bfd_copy_private_section_data \
  _bfd_bool_bfd_asection_bfd_asection_true
#define _bfd_generic_bfd_copy_private_symbol_data \
  _bfd_bool_bfd_asymbol_bfd_asymbol_true
#define _bfd_generic_bfd_copy_private_header_data _bfd_bool_bfd_bfd_true
#define _bfd_generic_bfd_print_private_bfd_data _bfd_bool_bfd_ptr_true

extern bfd_boolean _bfd_generic_init_private_section_data
  (bfd *, asection *, bfd *, asection *, struct bfd_link_info *)
  ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_CORE when there is no core file
   support.  Use BFD_JUMP_TABLE_CORE (_bfd_nocore).  */

extern char *_bfd_nocore_core_file_failing_command
  (bfd *) ATTRIBUTE_HIDDEN;
extern int _bfd_nocore_core_file_failing_signal
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nocore_core_file_matches_executable_p
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;
extern int _bfd_nocore_core_file_pid
  (bfd *) ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_ARCHIVE when there is no archive
   file support.  Use BFD_JUMP_TABLE_ARCHIVE (_bfd_noarchive).  */

#define _bfd_noarchive_slurp_armap _bfd_bool_bfd_false_error
#define _bfd_noarchive_slurp_extended_name_table _bfd_bool_bfd_false_error
extern bfd_boolean _bfd_noarchive_construct_extended_name_table
  (bfd *, char **, bfd_size_type *, const char **) ATTRIBUTE_HIDDEN;
extern void _bfd_noarchive_truncate_arname
  (bfd *, const char *, char *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_noarchive_write_armap
  (bfd *, unsigned int, struct orl *, unsigned int, int) ATTRIBUTE_HIDDEN;
#define _bfd_noarchive_read_ar_hdr _bfd_ptr_bfd_null_error
extern bfd_boolean _bfd_noarchive_write_ar_hdr
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;
extern bfd *
_bfd_noarchive_openr_next_archived_file
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;
extern bfd * _bfd_noarchive_get_elt_at_index
  (bfd *, symindex) ATTRIBUTE_HIDDEN;
#define _bfd_noarchive_generic_stat_arch_elt bfd_generic_stat_arch_elt
#define _bfd_noarchive_update_armap_timestamp _bfd_bool_bfd_false_error

/* Routines to use for BFD_JUMP_TABLE_ARCHIVE to get COFF style
   archives.  Use BFD_JUMP_TABLE_ARCHIVE (_bfd_archive_coff).  */

#define _bfd_archive_coff_slurp_armap bfd_slurp_coff_armap
#define _bfd_archive_coff_slurp_extended_name_table \
  _bfd_slurp_extended_name_table
extern bfd_boolean _bfd_archive_coff_construct_extended_name_table
  (bfd *, char **, bfd_size_type *, const char **) ATTRIBUTE_HIDDEN;
#define _bfd_archive_coff_truncate_arname bfd_dont_truncate_arname
#define _bfd_archive_coff_write_armap _bfd_coff_write_armap
#define _bfd_archive_coff_read_ar_hdr _bfd_generic_read_ar_hdr
#define _bfd_archive_coff_write_ar_hdr _bfd_generic_write_ar_hdr
#define _bfd_archive_coff_openr_next_archived_file \
  bfd_generic_openr_next_archived_file
#define _bfd_archive_coff_get_elt_at_index _bfd_generic_get_elt_at_index
#define _bfd_archive_coff_generic_stat_arch_elt \
  bfd_generic_stat_arch_elt
#define _bfd_archive_coff_update_armap_timestamp _bfd_bool_bfd_true

/* Routines to use for BFD_JUMP_TABLE_ARCHIVE to get VMS style
   archives.  Use BFD_JUMP_TABLE_ARCHIVE (_bfd_vms_lib).  Some of them
   are irrelevant.  */

extern bfd_boolean _bfd_vms_lib_write_archive_contents
  (bfd *) ATTRIBUTE_HIDDEN;
#define _bfd_vms_lib_slurp_armap _bfd_noarchive_slurp_armap
#define _bfd_vms_lib_slurp_extended_name_table \
  _bfd_noarchive_slurp_extended_name_table
#define _bfd_vms_lib_construct_extended_name_table \
  _bfd_noarchive_construct_extended_name_table
#define _bfd_vms_lib_truncate_arname _bfd_noarchive_truncate_arname
#define _bfd_vms_lib_write_armap _bfd_noarchive_write_armap
#define _bfd_vms_lib_read_ar_hdr _bfd_noarchive_read_ar_hdr
#define _bfd_vms_lib_write_ar_hdr _bfd_noarchive_write_ar_hdr
extern bfd *_bfd_vms_lib_openr_next_archived_file
  (bfd *, bfd *) ATTRIBUTE_HIDDEN;
extern bfd *_bfd_vms_lib_get_elt_at_index
  (bfd *, symindex) ATTRIBUTE_HIDDEN;
extern int _bfd_vms_lib_generic_stat_arch_elt
  (bfd *, struct stat *) ATTRIBUTE_HIDDEN;
#define _bfd_vms_lib_update_armap_timestamp _bfd_bool_bfd_true

/* Extra routines for VMS style archives.  */

extern symindex _bfd_vms_lib_find_symbol
  (bfd *, const char *) ATTRIBUTE_HIDDEN;
extern bfd *_bfd_vms_lib_get_imagelib_file
  (bfd *) ATTRIBUTE_HIDDEN;
extern const bfd_target *_bfd_vms_lib_alpha_archive_p
  (bfd *) ATTRIBUTE_HIDDEN;
extern const bfd_target *_bfd_vms_lib_ia64_archive_p
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_vms_lib_alpha_mkarchive
  (bfd *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_vms_lib_ia64_mkarchive
  (bfd *) ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_SYMBOLS where there is no symbol
   support.  Use BFD_JUMP_TABLE_SYMBOLS (_bfd_nosymbols).  */

#define _bfd_nosymbols_get_symtab_upper_bound _bfd_long_bfd_n1_error
extern long _bfd_nosymbols_canonicalize_symtab
  (bfd *, asymbol **) ATTRIBUTE_HIDDEN;
#define _bfd_nosymbols_make_empty_symbol _bfd_generic_make_empty_symbol
extern void _bfd_nosymbols_print_symbol
  (bfd *, void *, asymbol *, bfd_print_symbol_type) ATTRIBUTE_HIDDEN;
extern void _bfd_nosymbols_get_symbol_info
  (bfd *, asymbol *, symbol_info *) ATTRIBUTE_HIDDEN;
extern const char * _bfd_nosymbols_get_symbol_version_string
  (bfd *, asymbol *, bfd_boolean *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nosymbols_bfd_is_local_label_name
  (bfd *, const char *) ATTRIBUTE_HIDDEN;
#define _bfd_nosymbols_bfd_is_target_special_symbol _bfd_bool_bfd_asymbol_false
extern alent *_bfd_nosymbols_get_lineno
  (bfd *, asymbol *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nosymbols_find_nearest_line
  (bfd *, asymbol **, asection *, bfd_vma,
   const char **, const char **, unsigned int *, unsigned int *)
  ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nosymbols_find_line
  (bfd *, asymbol **, asymbol *, const char **, unsigned int *)
  ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nosymbols_find_inliner_info
  (bfd *, const char **, const char **, unsigned int *) ATTRIBUTE_HIDDEN;
extern asymbol *_bfd_nosymbols_bfd_make_debug_symbol
  (bfd *, void *, uint32_t) ATTRIBUTE_HIDDEN;
extern long _bfd_nosymbols_read_minisymbols
  (bfd *, bfd_boolean, void **, unsigned int *) ATTRIBUTE_HIDDEN;
extern asymbol *_bfd_nosymbols_minisymbol_to_symbol
  (bfd *, bfd_boolean, const void *, asymbol *) ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_RELOCS when there is no reloc
   support.  Use BFD_JUMP_TABLE_RELOCS (_bfd_norelocs).  */

extern long _bfd_norelocs_get_reloc_upper_bound
  (bfd *, asection *) ATTRIBUTE_HIDDEN;
extern long _bfd_norelocs_canonicalize_reloc
  (bfd *, asection *, arelent **, asymbol **) ATTRIBUTE_HIDDEN;
extern void _bfd_norelocs_set_reloc
  (bfd *, asection *, arelent **, unsigned int) ATTRIBUTE_HIDDEN;
extern reloc_howto_type *_bfd_norelocs_bfd_reloc_type_lookup
  (bfd *, bfd_reloc_code_real_type) ATTRIBUTE_HIDDEN;
extern reloc_howto_type *_bfd_norelocs_bfd_reloc_name_lookup
  (bfd *, const char *) ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_WRITE for targets which may not
   be written.  Use BFD_JUMP_TABLE_WRITE (_bfd_nowrite).  */

extern bfd_boolean _bfd_nowrite_set_arch_mach
  (bfd *, enum bfd_architecture, uint32_t) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_nowrite_set_section_contents
  (bfd *, asection *, const void *, file_ptr, bfd_size_type) ATTRIBUTE_HIDDEN;

/* Generic routines to use for BFD_JUMP_TABLE_WRITE.  Use
   BFD_JUMP_TABLE_WRITE (_bfd_generic).  */

#define _bfd_generic_set_arch_mach bfd_default_set_arch_mach
extern bfd_boolean _bfd_generic_set_section_contents
  (bfd *, asection *, const void *, file_ptr, bfd_size_type) ATTRIBUTE_HIDDEN;

/* Routines to use for BFD_JUMP_TABLE_DYNAMIC for targets which do not
   have dynamic symbols or relocs.  Use BFD_JUMP_TABLE_DYNAMIC
   (_bfd_nodynamic).  */

#define _bfd_nodynamic_get_dynamic_symtab_upper_bound _bfd_long_bfd_n1_error
#define _bfd_nodynamic_canonicalize_dynamic_symtab \
  _bfd_nosymbols_canonicalize_symtab
extern long _bfd_nodynamic_get_synthetic_symtab
  (bfd *, long, asymbol **, long, asymbol **, asymbol **) ATTRIBUTE_HIDDEN;
#define _bfd_nodynamic_get_dynamic_reloc_upper_bound _bfd_long_bfd_n1_error
extern long _bfd_nodynamic_canonicalize_dynamic_reloc
  (bfd *, arelent **, asymbol **) ATTRIBUTE_HIDDEN;

/* Generic routine to determine of the given symbol is a local
   label.  */
extern bfd_boolean bfd_generic_is_local_label_name
  (bfd *, const char *) ATTRIBUTE_HIDDEN;

/* Generic minisymbol routines.  */
extern long _bfd_generic_read_minisymbols
  (bfd *, bfd_boolean, void **, unsigned int *) ATTRIBUTE_HIDDEN;
extern asymbol *_bfd_generic_minisymbol_to_symbol
  (bfd *, bfd_boolean, const void *, asymbol *) ATTRIBUTE_HIDDEN;

/* Find the nearest line using .stab/.stabstr sections.  */
extern bfd_boolean _bfd_stab_section_find_nearest_line
  (bfd *, asymbol **, asection *, bfd_vma, bfd_boolean *,
   const char **, const char **, unsigned int *, void **) ATTRIBUTE_HIDDEN;

/* Find the nearest line using DWARF 1 debugging information.  */
extern bfd_boolean _bfd_dwarf1_find_nearest_line
  (bfd *, asymbol **, asection *, bfd_vma,
   const char **, const char **, unsigned int *) ATTRIBUTE_HIDDEN;

struct dwarf_debug_section
{
  const char * uncompressed_name;
  const char * compressed_name;
};

/* Map of uncompressed DWARF debug section name to compressed one.  It
   is terminated by NULL uncompressed_name.  */

extern const struct dwarf_debug_section dwarf_debug_sections[] ATTRIBUTE_HIDDEN;

/* Find the nearest line using DWARF 2 debugging information.  */
extern bfd_boolean _bfd_dwarf2_find_nearest_line
  (bfd *, asymbol **, asymbol *, asection *, bfd_vma,
   const char **, const char **, unsigned int *, unsigned int *,
   const struct dwarf_debug_section *, unsigned int, void **) ATTRIBUTE_HIDDEN;

/* Find inliner info after calling bfd_find_nearest_line. */
extern bfd_boolean _bfd_dwarf2_find_inliner_info
  (bfd *, const char **, const char **, unsigned int *, void **)
  ATTRIBUTE_HIDDEN;

/* Read DWARF 2 debugging information. */
extern bfd_boolean _bfd_dwarf2_slurp_debug_info
  (bfd *, bfd *, const struct dwarf_debug_section *, asymbol **, void **,
   bfd_boolean) ATTRIBUTE_HIDDEN;

/* Clean up the data used to handle DWARF 2 debugging information. */
extern void _bfd_dwarf2_cleanup_debug_info
  (bfd *, void **) ATTRIBUTE_HIDDEN;

/* Create a new section entry.  */
extern struct bfd_hash_entry *bfd_section_hash_newfunc
  (struct bfd_hash_entry *, struct bfd_hash_table *, const char *)
  ATTRIBUTE_HIDDEN;

/* A routine to create entries for a bfd_link_hash_table.  */
extern struct bfd_hash_entry *_bfd_link_hash_newfunc
  (struct bfd_hash_entry *entry, struct bfd_hash_table *table,
   const char *string) ATTRIBUTE_HIDDEN;

/* Initialize a bfd_link_hash_table.  */
extern bfd_boolean _bfd_link_hash_table_init
  (struct bfd_link_hash_table *, bfd *,
   struct bfd_hash_entry *(*) (struct bfd_hash_entry *,
			       struct bfd_hash_table *,
			       const char *),
   unsigned int) ATTRIBUTE_HIDDEN;

/* Generic link hash table creation routine.  */
extern struct bfd_link_hash_table *_bfd_generic_link_hash_table_create
  (bfd *) ATTRIBUTE_HIDDEN;

/* Generic link hash table destruction routine.  */
extern void _bfd_generic_link_hash_table_free
  (bfd *) ATTRIBUTE_HIDDEN;

/* Generic add symbol routine.  */
extern bfd_boolean _bfd_generic_link_add_symbols
  (bfd *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;

/* Generic archive add symbol routine.  */
extern bfd_boolean _bfd_generic_link_add_archive_symbols
  (bfd *, struct bfd_link_info *,
   bfd_boolean (*) (bfd *, struct bfd_link_info *,
		    struct bfd_link_hash_entry *, const char *,
		    bfd_boolean *)) ATTRIBUTE_HIDDEN;

/* Forward declaration to avoid prototype errors.  */
typedef struct bfd_link_hash_entry _bfd_link_hash_entry;

/* Generic routine to add a single symbol.  */
extern bfd_boolean _bfd_generic_link_add_one_symbol
  (struct bfd_link_info *, bfd *, const char *name, flagword,
   asection *, bfd_vma, const char *, bfd_boolean copy,
   bfd_boolean constructor, struct bfd_link_hash_entry **) ATTRIBUTE_HIDDEN;

/* Generic routine to mark section as supplying symbols only.  */
extern void _bfd_generic_link_just_syms
  (asection *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;

/* Generic routine that does nothing.  */
extern void _bfd_generic_copy_link_hash_symbol_type
  (bfd *, struct bfd_link_hash_entry *, struct bfd_link_hash_entry *)
  ATTRIBUTE_HIDDEN;

/* Generic link routine.  */
extern bfd_boolean _bfd_generic_final_link
  (bfd *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;

extern bfd_boolean _bfd_generic_link_split_section
  (bfd *, struct bfd_section *) ATTRIBUTE_HIDDEN;

extern bfd_boolean _bfd_generic_section_already_linked
  (bfd *, asection *, struct bfd_link_info *) ATTRIBUTE_HIDDEN;

/* Generic reloc_link_order processing routine.  */
extern bfd_boolean _bfd_generic_reloc_link_order
  (bfd *, struct bfd_link_info *, asection *, struct bfd_link_order *)
  ATTRIBUTE_HIDDEN;

/* Default link order processing routine.  */
extern bfd_boolean _bfd_default_link_order
  (bfd *, struct bfd_link_info *, asection *, struct bfd_link_order *)
  ATTRIBUTE_HIDDEN;

/* Final link relocation routine.  */
extern bfd_reloc_status_type _bfd_final_link_relocate
  (reloc_howto_type *, bfd *, asection *, bfd_byte *,
   bfd_vma, bfd_vma, bfd_vma) ATTRIBUTE_HIDDEN;

/* Relocate a particular location by a howto and a value.  */
extern bfd_reloc_status_type _bfd_relocate_contents
  (reloc_howto_type *, bfd *, bfd_vma, bfd_byte *) ATTRIBUTE_HIDDEN;

/* Clear a given location using a given howto.  */
extern void _bfd_clear_contents
  (reloc_howto_type *, bfd *, asection *, bfd_byte *) ATTRIBUTE_HIDDEN;

/* Link stabs in sections in the first pass.  */

extern bfd_boolean _bfd_link_section_stabs
  (bfd *, struct stab_info *, asection *, asection *, void **,
   bfd_size_type *) ATTRIBUTE_HIDDEN;

/* Eliminate stabs for discarded functions and symbols.  */
extern bfd_boolean _bfd_discard_section_stabs
  (bfd *, asection *, void *, bfd_boolean (*) (bfd_vma, void *), void *)
  ATTRIBUTE_HIDDEN;

/* Write out the .stab section when linking stabs in sections.  */

extern bfd_boolean _bfd_write_section_stabs
  (bfd *, struct stab_info *, asection *, void **, bfd_byte *)
  ATTRIBUTE_HIDDEN;

/* Write out the .stabstr string table when linking stabs in sections.  */

extern bfd_boolean _bfd_write_stab_strings
  (bfd *, struct stab_info *) ATTRIBUTE_HIDDEN;

/* Find an offset within a .stab section when linking stabs in
   sections.  */

extern bfd_vma _bfd_stab_section_offset
  (asection *, void *, bfd_vma) ATTRIBUTE_HIDDEN;

/* Register a SEC_MERGE section as a candidate for merging.  */

extern bfd_boolean _bfd_add_merge_section
  (bfd *, void **, asection *, void **) ATTRIBUTE_HIDDEN;

/* Attempt to merge SEC_MERGE sections.  */

extern bfd_boolean _bfd_merge_sections
  (bfd *, struct bfd_link_info *, void *, void (*) (bfd *, asection *))
  ATTRIBUTE_HIDDEN;

/* Write out a merged section.  */

extern bfd_boolean _bfd_write_merged_section
  (bfd *, asection *, void *) ATTRIBUTE_HIDDEN;

/* Find an offset within a modified SEC_MERGE section.  */

extern bfd_vma _bfd_merged_section_offset
  (bfd *, asection **, void *, bfd_vma) ATTRIBUTE_HIDDEN;

/* Tidy up when done.  */

extern void _bfd_merge_sections_free (void *) ATTRIBUTE_HIDDEN;

/* Create a string table.  */
extern struct bfd_strtab_hash *_bfd_stringtab_init
  (void) ATTRIBUTE_HIDDEN;

/* Free a string table.  */
extern void _bfd_stringtab_free
  (struct bfd_strtab_hash *) ATTRIBUTE_HIDDEN;

/* Get the size of a string table.  */
extern bfd_size_type _bfd_stringtab_size
  (struct bfd_strtab_hash *) ATTRIBUTE_HIDDEN;

/* Add a string to a string table.  */
extern bfd_size_type _bfd_stringtab_add
  (struct bfd_strtab_hash *, const char *, bfd_boolean hash, bfd_boolean copy)
  ATTRIBUTE_HIDDEN;

/* Write out a string table.  */
extern bfd_boolean _bfd_stringtab_emit
  (bfd *, struct bfd_strtab_hash *) ATTRIBUTE_HIDDEN;

/* Macros to tell if bfds are read or write enabled.

   Note that bfds open for read may be scribbled into if the fd passed
   to bfd_fdopenr is actually open both for read and write
   simultaneously.  However an output bfd will never be open for
   read.  Therefore sometimes you want to check bfd_read_p or
   !bfd_read_p, and only sometimes bfd_write_p.
*/

#define	bfd_read_p(abfd) \
  ((abfd)->direction == read_direction || (abfd)->direction == both_direction)
#define	bfd_write_p(abfd) \
  ((abfd)->direction == write_direction || (abfd)->direction == both_direction)

extern void bfd_assert
  (const char*,int) ATTRIBUTE_HIDDEN;

#define BFD_ASSERT(x) \
  do { if (!(x)) bfd_assert(__FILE__,__LINE__); } while (0)

#define BFD_FAIL() \
  do { bfd_assert(__FILE__,__LINE__); } while (0)

extern void _bfd_abort
  (const char *, int, const char *) ATTRIBUTE_NORETURN ATTRIBUTE_HIDDEN;

/* if gcc >= 2.6, we can give a function name, too */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 6)
#define __PRETTY_FUNCTION__  ((char *) NULL)
#endif

#undef abort
#define abort() _bfd_abort (__FILE__, __LINE__, __PRETTY_FUNCTION__)

/* Manipulate a system FILE but using BFD's "file_ptr", rather than
   the system "off_t" or "off64_t", as the offset.  */
extern file_ptr _bfd_real_ftell
  (FILE *) ATTRIBUTE_HIDDEN;
extern int _bfd_real_fseek
  (FILE *, file_ptr, int) ATTRIBUTE_HIDDEN;
extern FILE *_bfd_real_fopen
  (const char *, const char *) ATTRIBUTE_HIDDEN;

/* List of supported target vectors, and the default vector (if
   bfd_default_vector[0] is NULL, there is no default).  */
extern const bfd_target * const *bfd_target_vector ATTRIBUTE_HIDDEN;
extern const bfd_target *bfd_default_vector[] ATTRIBUTE_HIDDEN;

/* List of associated target vectors.  */
extern const bfd_target * const *bfd_associated_vector ATTRIBUTE_HIDDEN;

/* Functions shared by the ECOFF and MIPS ELF backends, which have no
   other common header files.  */

#if defined(__STDC__) || defined(ALMOST_STDC)
struct ecoff_find_line;
#endif

extern bfd_boolean _bfd_ecoff_locate_line
  (bfd *, asection *, bfd_vma, struct ecoff_debug_info * const,
   const struct ecoff_debug_swap * const, struct ecoff_find_line *,
   const char **, const char **, unsigned int *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_ecoff_get_accumulated_pdr
  (void *, bfd_byte *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_ecoff_get_accumulated_sym
  (void *, bfd_byte *) ATTRIBUTE_HIDDEN;
extern bfd_boolean _bfd_ecoff_get_accumulated_ss
  (void *, bfd_byte *) ATTRIBUTE_HIDDEN;

extern bfd_vma _bfd_get_gp_value
  (bfd *) ATTRIBUTE_HIDDEN;
extern void _bfd_set_gp_value
  (bfd *, bfd_vma) ATTRIBUTE_HIDDEN;

/* Function shared by the COFF and ELF SH backends, which have no
   other common header files.  */

#ifndef _bfd_sh_align_load_span
extern bfd_boolean _bfd_sh_align_load_span
  (bfd *, asection *, bfd_byte *,
   bfd_boolean (*) (bfd *, asection *, void *, bfd_byte *, bfd_vma),
   void *, bfd_vma **, bfd_vma *, bfd_vma, bfd_vma, bfd_boolean *) ATTRIBUTE_HIDDEN;
#endif

/* This is the shape of the elements inside the already_linked hash
   table. It maps a name onto a list of already_linked elements with
   the same name.  */

struct bfd_section_already_linked_hash_entry
{
  struct bfd_hash_entry root;
  struct bfd_section_already_linked *entry;
};

struct bfd_section_already_linked
{
  struct bfd_section_already_linked *next;
  asection *sec;
};

extern struct bfd_section_already_linked_hash_entry *
  bfd_section_already_linked_table_lookup (const char *) ATTRIBUTE_HIDDEN;
extern bfd_boolean bfd_section_already_linked_table_insert
  (struct bfd_section_already_linked_hash_entry *, asection *)
  ATTRIBUTE_HIDDEN;
extern void bfd_section_already_linked_table_traverse
  (bfd_boolean (*) (struct bfd_section_already_linked_hash_entry *,
		    void *), void *) ATTRIBUTE_HIDDEN;

extern bfd_vma _bfd_read_unsigned_leb128
  (bfd *, bfd_byte *, unsigned int *) ATTRIBUTE_HIDDEN;
extern bfd_signed_vma _bfd_read_signed_leb128
  (bfd *, bfd_byte *, unsigned int *) ATTRIBUTE_HIDDEN;
extern bfd_vma _bfd_safe_read_leb128
  (bfd *, bfd_byte *, unsigned int *, bfd_boolean, const bfd_byte * const)
  ATTRIBUTE_HIDDEN;
/* Extracted from libbfd.c.  */
bfd_boolean bfd_write_bigendian_4byte_int (bfd *, unsigned int);

unsigned int bfd_log2 (bfd_vma x);

/* Extracted from bfdio.c.  */
struct bfd_iovec
{
  /* To avoid problems with macros, a "b" rather than "f"
     prefix is prepended to each method name.  */
  /* Attempt to read/write NBYTES on ABFD's IOSTREAM storing/fetching
     bytes starting at PTR.  Return the number of bytes actually
     transfered (a read past end-of-file returns less than NBYTES),
     or -1 (setting <<bfd_error>>) if an error occurs.  */
  file_ptr (*bread) (struct bfd *abfd, void *ptr, file_ptr nbytes);
  file_ptr (*bwrite) (struct bfd *abfd, const void *ptr,
                      file_ptr nbytes);
  /* Return the current IOSTREAM file offset, or -1 (setting <<bfd_error>>
     if an error occurs.  */
  file_ptr (*btell) (struct bfd *abfd);
  /* For the following, on successful completion a value of 0 is returned.
     Otherwise, a value of -1 is returned (and <<bfd_error>> is set).  */
  int (*bseek) (struct bfd *abfd, file_ptr offset, int whence);
  int (*bclose) (struct bfd *abfd);
  int (*bflush) (struct bfd *abfd);
  int (*bstat) (struct bfd *abfd, struct stat *sb);
  /* Mmap a part of the files. ADDR, LEN, PROT, FLAGS and OFFSET are the usual
     mmap parameter, except that LEN and OFFSET do not need to be page
     aligned.  Returns (void *)-1 on failure, mmapped address on success.
     Also write in MAP_ADDR the address of the page aligned buffer and in
     MAP_LEN the size mapped (a page multiple).  Use unmap with MAP_ADDR and
     MAP_LEN to unmap.  */
  void *(*bmmap) (struct bfd *abfd, void *addr, bfd_size_type len,
                  int prot, int flags, file_ptr offset,
                  void **map_addr, bfd_size_type *map_len);
};
extern const struct bfd_iovec _bfd_memory_iovec;
/* Extracted from bfdwin.c.  */
struct _bfd_window_internal {
  struct _bfd_window_internal *next;
  void *data;
  bfd_size_type size;
  int refcount : 31;           /* should be enough...  */
  unsigned mapped : 1;         /* 1 = mmap, 0 = malloc */
};
/* Extracted from cache.c.  */
bfd_boolean bfd_cache_init (bfd *abfd);

bfd_boolean bfd_cache_close (bfd *abfd);

FILE* bfd_open_file (bfd *abfd);

/* Extracted from reloc.c.  */
#ifdef _BFD_MAKE_TABLE_bfd_reloc_code_real

static const char *const bfd_reloc_code_real_names[] = { "@@uninitialized@@",

  "BFD_RELOC_64",
  "BFD_RELOC_32",
  "BFD_RELOC_26",
  "BFD_RELOC_24",
  "BFD_RELOC_16",
  "BFD_RELOC_14",
  "BFD_RELOC_8",
  "BFD_RELOC_64_PCREL",
  "BFD_RELOC_32_PCREL",
  "BFD_RELOC_24_PCREL",
  "BFD_RELOC_16_PCREL",
  "BFD_RELOC_12_PCREL",
  "BFD_RELOC_8_PCREL",
  "BFD_RELOC_32_SECREL",
  "BFD_RELOC_32_GOT_PCREL",
  "BFD_RELOC_16_GOT_PCREL",
  "BFD_RELOC_8_GOT_PCREL",
  "BFD_RELOC_32_GOTOFF",
  "BFD_RELOC_16_GOTOFF",
  "BFD_RELOC_LO16_GOTOFF",
  "BFD_RELOC_HI16_GOTOFF",
  "BFD_RELOC_HI16_S_GOTOFF",
  "BFD_RELOC_8_GOTOFF",
  "BFD_RELOC_64_PLT_PCREL",
  "BFD_RELOC_32_PLT_PCREL",
  "BFD_RELOC_24_PLT_PCREL",
  "BFD_RELOC_16_PLT_PCREL",
  "BFD_RELOC_8_PLT_PCREL",
  "BFD_RELOC_64_PLTOFF",
  "BFD_RELOC_32_PLTOFF",
  "BFD_RELOC_16_PLTOFF",
  "BFD_RELOC_LO16_PLTOFF",
  "BFD_RELOC_HI16_PLTOFF",
  "BFD_RELOC_HI16_S_PLTOFF",
  "BFD_RELOC_8_PLTOFF",
  "BFD_RELOC_SIZE32",
  "BFD_RELOC_SIZE64",
  "BFD_RELOC_GPREL16",
  "BFD_RELOC_GPREL32",
  "BFD_RELOC_NONE",
  "BFD_RELOC_32_BASEREL",
  "BFD_RELOC_16_BASEREL",
  "BFD_RELOC_LO16_BASEREL",
  "BFD_RELOC_HI16_BASEREL",
  "BFD_RELOC_HI16_S_BASEREL",
  "BFD_RELOC_8_BASEREL",
  "BFD_RELOC_RVA",
  "BFD_RELOC_CTOR",
  "BFD_RELOC_ARM_PCREL_BRANCH",
  "BFD_RELOC_ARM_PCREL_BLX",
  "BFD_RELOC_THUMB_PCREL_BLX",
  "BFD_RELOC_ARM_PCREL_CALL",
  "BFD_RELOC_ARM_PCREL_JUMP",
  "BFD_RELOC_THUMB_PCREL_BRANCH7",
  "BFD_RELOC_THUMB_PCREL_BRANCH9",
  "BFD_RELOC_THUMB_PCREL_BRANCH12",
  "BFD_RELOC_THUMB_PCREL_BRANCH20",
  "BFD_RELOC_THUMB_PCREL_BRANCH23",
  "BFD_RELOC_THUMB_PCREL_BRANCH25",
  "BFD_RELOC_ARM_OFFSET_IMM",
  "BFD_RELOC_ARM_THUMB_OFFSET",
  "BFD_RELOC_ARM_TARGET1",
  "BFD_RELOC_ARM_ROSEGREL32",
  "BFD_RELOC_ARM_SBREL32",
  "BFD_RELOC_ARM_TARGET2",
  "BFD_RELOC_ARM_PREL31",
  "BFD_RELOC_ARM_MOVW",
  "BFD_RELOC_ARM_MOVT",
  "BFD_RELOC_ARM_MOVW_PCREL",
  "BFD_RELOC_ARM_MOVT_PCREL",
  "BFD_RELOC_ARM_THUMB_MOVW",
  "BFD_RELOC_ARM_THUMB_MOVT",
  "BFD_RELOC_ARM_THUMB_MOVW_PCREL",
  "BFD_RELOC_ARM_THUMB_MOVT_PCREL",
  "BFD_RELOC_ARM_GOTFUNCDESC",
  "BFD_RELOC_ARM_GOTOFFFUNCDESC",
  "BFD_RELOC_ARM_FUNCDESC",
  "BFD_RELOC_ARM_FUNCDESC_VALUE",
  "BFD_RELOC_ARM_TLS_GD32_FDPIC",
  "BFD_RELOC_ARM_TLS_LDM32_FDPIC",
  "BFD_RELOC_ARM_TLS_IE32_FDPIC",
  "BFD_RELOC_ARM_JUMP_SLOT",
  "BFD_RELOC_ARM_GLOB_DAT",
  "BFD_RELOC_ARM_GOT32",
  "BFD_RELOC_ARM_PLT32",
  "BFD_RELOC_ARM_RELATIVE",
  "BFD_RELOC_ARM_GOTOFF",
  "BFD_RELOC_ARM_GOTPC",
  "BFD_RELOC_ARM_GOT_PREL",
  "BFD_RELOC_ARM_TLS_GD32",
  "BFD_RELOC_ARM_TLS_LDO32",
  "BFD_RELOC_ARM_TLS_LDM32",
  "BFD_RELOC_ARM_TLS_DTPOFF32",
  "BFD_RELOC_ARM_TLS_DTPMOD32",
  "BFD_RELOC_ARM_TLS_TPOFF32",
  "BFD_RELOC_ARM_TLS_IE32",
  "BFD_RELOC_ARM_TLS_LE32",
  "BFD_RELOC_ARM_TLS_GOTDESC",
  "BFD_RELOC_ARM_TLS_CALL",
  "BFD_RELOC_ARM_THM_TLS_CALL",
  "BFD_RELOC_ARM_TLS_DESCSEQ",
  "BFD_RELOC_ARM_THM_TLS_DESCSEQ",
  "BFD_RELOC_ARM_TLS_DESC",
  "BFD_RELOC_ARM_ALU_PC_G0_NC",
  "BFD_RELOC_ARM_ALU_PC_G0",
  "BFD_RELOC_ARM_ALU_PC_G1_NC",
  "BFD_RELOC_ARM_ALU_PC_G1",
  "BFD_RELOC_ARM_ALU_PC_G2",
  "BFD_RELOC_ARM_LDR_PC_G0",
  "BFD_RELOC_ARM_LDR_PC_G1",
  "BFD_RELOC_ARM_LDR_PC_G2",
  "BFD_RELOC_ARM_LDRS_PC_G0",
  "BFD_RELOC_ARM_LDRS_PC_G1",
  "BFD_RELOC_ARM_LDRS_PC_G2",
  "BFD_RELOC_ARM_LDC_PC_G0",
  "BFD_RELOC_ARM_LDC_PC_G1",
  "BFD_RELOC_ARM_LDC_PC_G2",
  "BFD_RELOC_ARM_ALU_SB_G0_NC",
  "BFD_RELOC_ARM_ALU_SB_G0",
  "BFD_RELOC_ARM_ALU_SB_G1_NC",
  "BFD_RELOC_ARM_ALU_SB_G1",
  "BFD_RELOC_ARM_ALU_SB_G2",
  "BFD_RELOC_ARM_LDR_SB_G0",
  "BFD_RELOC_ARM_LDR_SB_G1",
  "BFD_RELOC_ARM_LDR_SB_G2",
  "BFD_RELOC_ARM_LDRS_SB_G0",
  "BFD_RELOC_ARM_LDRS_SB_G1",
  "BFD_RELOC_ARM_LDRS_SB_G2",
  "BFD_RELOC_ARM_LDC_SB_G0",
  "BFD_RELOC_ARM_LDC_SB_G1",
  "BFD_RELOC_ARM_LDC_SB_G2",
  "BFD_RELOC_ARM_V4BX",
  "BFD_RELOC_ARM_IRELATIVE",
  "BFD_RELOC_ARM_THUMB_ALU_ABS_G0_NC",
  "BFD_RELOC_ARM_THUMB_ALU_ABS_G1_NC",
  "BFD_RELOC_ARM_THUMB_ALU_ABS_G2_NC",
  "BFD_RELOC_ARM_THUMB_ALU_ABS_G3_NC",
  "BFD_RELOC_ARM_IMMEDIATE",
  "BFD_RELOC_ARM_ADRL_IMMEDIATE",
  "BFD_RELOC_ARM_T32_IMMEDIATE",
  "BFD_RELOC_ARM_T32_ADD_IMM",
  "BFD_RELOC_ARM_T32_IMM12",
  "BFD_RELOC_ARM_T32_ADD_PC12",
  "BFD_RELOC_ARM_SHIFT_IMM",
  "BFD_RELOC_ARM_SMC",
  "BFD_RELOC_ARM_HVC",
  "BFD_RELOC_ARM_SWI",
  "BFD_RELOC_ARM_MULTI",
  "BFD_RELOC_ARM_CP_OFF_IMM",
  "BFD_RELOC_ARM_CP_OFF_IMM_S2",
  "BFD_RELOC_ARM_T32_CP_OFF_IMM",
  "BFD_RELOC_ARM_T32_CP_OFF_IMM_S2",
  "BFD_RELOC_ARM_ADR_IMM",
  "BFD_RELOC_ARM_LDR_IMM",
  "BFD_RELOC_ARM_LITERAL",
  "BFD_RELOC_ARM_IN_POOL",
  "BFD_RELOC_ARM_OFFSET_IMM8",
  "BFD_RELOC_ARM_T32_OFFSET_U8",
  "BFD_RELOC_ARM_T32_OFFSET_IMM",
  "BFD_RELOC_ARM_HWLITERAL",
  "BFD_RELOC_ARM_THUMB_ADD",
  "BFD_RELOC_ARM_THUMB_IMM",
  "BFD_RELOC_ARM_THUMB_SHIFT",
  "BFD_RELOC_VTABLE_INHERIT",
  "BFD_RELOC_VTABLE_ENTRY",
 "@@overflow: BFD_RELOC_UNUSED@@",
};
#endif

reloc_howto_type *bfd_default_reloc_type_lookup
   (bfd *abfd, bfd_reloc_code_real_type  code);

bfd_boolean bfd_generic_relax_section
   (bfd *abfd,
    asection *section,
    struct bfd_link_info *,
    bfd_boolean *);

bfd_boolean bfd_generic_gc_sections
   (bfd *, struct bfd_link_info *);

bfd_boolean bfd_generic_lookup_section_flags
   (struct bfd_link_info *, struct flag_info *, asection *);

bfd_boolean bfd_generic_merge_sections
   (bfd *, struct bfd_link_info *);

bfd_byte *bfd_generic_get_relocated_section_contents
   (bfd *abfd,
    struct bfd_link_info *link_info,
    struct bfd_link_order *link_order,
    bfd_byte *data,
    bfd_boolean relocatable,
    asymbol **symbols);

void _bfd_generic_set_reloc
   (bfd *abfd,
    sec_ptr section,
    arelent **relptr,
    unsigned int count);

bfd_boolean _bfd_unrecognized_reloc
   (bfd * abfd,
    sec_ptr section,
    unsigned int r_type);

/* Extracted from archures.c.  */
extern const bfd_arch_info_type bfd_default_arch_struct;
const bfd_arch_info_type *bfd_default_compatible
   (const bfd_arch_info_type *a, const bfd_arch_info_type *b);

bfd_boolean bfd_default_scan
   (const struct bfd_arch_info *info, const char *string);

void *bfd_arch_default_fill (bfd_size_type count,
    bfd_boolean is_bigendian,
    bfd_boolean code);

/* Extracted from linker.c.  */
bfd_boolean _bfd_generic_verify_endian_match
   (bfd *ibfd, struct bfd_link_info *info);

#ifdef __cplusplus
}
#endif
#endif
