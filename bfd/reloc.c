/* BFD support for handling relocation entries.
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

/*
SECTION
	Relocations

	BFD maintains relocations in much the same way it maintains
	symbols: they are left alone until required, then read in
	en-masse and translated into an internal form.  A common
	routine <<bfd_perform_relocation>> acts upon the
	canonical form to do the fixup.

	Relocations are maintained on a per section basis,
	while symbols are maintained on a per BFD basis.

	All that a back end has to do to fit the BFD interface is to create
	a <<struct reloc_cache_entry>> for each relocation
	in a particular section, and fill in the right bits of the structures.

@menu
@* typedef arelent::
@* howto manager::
@end menu

*/

/* DO compile in the reloc_code name table from libbfd.h.  */
#define _BFD_MAKE_TABLE_bfd_reloc_code_real

#include "sysdep.h"
#include "bfd.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "bfdver.h"
/*
DOCDD
INODE
	typedef arelent, howto manager, Relocations, Relocations
*/

/*
DESCRIPTION

	Here is a description of each of the fields within an <<arelent>>:
	*snip*
*/

/*
SUBSUBSECTION
	<<enum complain_overflow>>

	Indicates what sort of overflow checking should be done when
	performing a relocation.
*/

/*
SUBSUBSECTION
	<<reloc_howto_type>>

	The <<reloc_howto_type>> is a structure which contains all the
	information that libbfd needs to know to tie up a back end's data.

*/

/*
FUNCTION
	The HOWTO Macro

DESCRIPTION
	The HOWTO define is horrible and will go away.

.#define HOWTO(C, R, S, B, P, BI, O, SF, NAME, INPLACE, MASKSRC, MASKDST, PC) \
.  { (unsigned) C, R, S, B, P, BI, O, SF, NAME, INPLACE, MASKSRC, MASKDST, PC }
*/

/*
FUNCTION
	bfd_get_reloc_size
 */

unsigned int
bfd_get_reloc_size (reloc_howto_type *howto)
{
  switch (howto->size)
    {
    case 5: return 3;
    case 0: return 1;
    case 1: return 2;
    case 2: return 4;
    case 3: return 0;
    case 4: return 8;
    case 8: return 16;
    case -1: return 2;
    case -2: return 4;
    default: abort ();
    }
}

/*
TYPEDEF
	arelent_chain
*/

/* N_ONES produces N one bits, without overflowing machine arithmetic.  */
#define N_ONES(n) (((((bfd_vma) 1 << ((n) - 1)) - 1) << 1) | 1)

/*
FUNCTION
	bfd_check_overflow

DESCRIPTION
	Perform overflow checking on @var{relocation} which has
	@var{bitsize} significant bits and will be shifted right by
	@var{rightshift} bits, on a machine with addresses containing
	@var{addrsize} significant bits.  The result is either of
	@code{bfd_reloc_ok} or @code{bfd_reloc_overflow}.

*/

bfd_reloc_status_type
bfd_check_overflow (enum complain_overflow how,
		    unsigned int bitsize,
		    unsigned int rightshift,
		    unsigned int addrsize,
		    bfd_vma relocation)
{
  bfd_vma fieldmask, addrmask, signmask, ss, a;
  bfd_reloc_status_type flag = bfd_reloc_ok;

  /* Note: BITSIZE should always be <= ADDRSIZE, but in case it's not,
     we'll be permissive: extra bits in the field mask will
     automatically extend the address mask for purposes of the
     overflow check.  */
  fieldmask = N_ONES (bitsize);
  signmask = ~fieldmask;
  addrmask = N_ONES (addrsize) | (fieldmask << rightshift);
  a = (relocation & addrmask) >> rightshift;

  switch (how)
    {
    case complain_overflow_dont:
      break;

    case complain_overflow_signed:
      /* If any sign bits are set, all sign bits must be set.  That
	 is, A must be a valid negative address after shifting.  */
      signmask = ~ (fieldmask >> 1);
      /* Fall thru */

    case complain_overflow_bitfield:
      /* Bitfields are sometimes signed, sometimes unsigned.  We
	 explicitly allow an address wrap too, which means a bitfield
	 of n bits is allowed to store -2**n to 2**n-1.  Thus overflow
	 if the value has some, but not all, bits set outside the
	 field.  */
      ss = a & signmask;
      if (ss != 0 && ss != ((addrmask >> rightshift) & signmask))
	flag = bfd_reloc_overflow;
      break;

    case complain_overflow_unsigned:
      /* We have an overflow if the address does not fit in the field.  */
      if ((a & signmask) != 0)
	flag = bfd_reloc_overflow;
      break;

    default:
      abort ();
    }

  return flag;
}

/*
FUNCTION
	bfd_reloc_offset_in_range

DESCRIPTION
	Returns TRUE if the reloc described by @var{HOWTO} can be
	applied at @var{OFFSET} octets in @var{SECTION}.

*/

/* HOWTO describes a relocation, at offset OCTET.  Return whether the
   relocation field is within SECTION of ABFD.  */

bfd_boolean
bfd_reloc_offset_in_range (reloc_howto_type *howto,
			   bfd *abfd,
			   asection *section,
			   bfd_size_type octet)
{
  bfd_size_type octet_end = bfd_get_section_limit_octets (abfd, section);
  bfd_size_type reloc_size = bfd_get_reloc_size (howto);

  /* The reloc field must be contained entirely within the section.
     Allow zero length fields (marker relocs or NONE relocs where no
     relocation will be performed) at the end of the section.  */
  return octet <= octet_end && octet + reloc_size <= octet_end;
}

/*
FUNCTION
	bfd_perform_relocation


DESCRIPTION
	If @var{output_bfd} is supplied to this function, the
	generated image will be relocatable; the relocations are
	copied to the output file after they have been changed to
	reflect the new state of the world.

*/

bfd_reloc_status_type
bfd_perform_relocation (bfd *abfd,
			arelent *reloc_entry,
			void *data,
			asection *input_section,
			bfd *output_bfd,
			char **error_message)
{
  bfd_vma relocation;
  bfd_reloc_status_type flag = bfd_reloc_ok;
  bfd_size_type octets;
  bfd_vma output_base = 0;
  reloc_howto_type *howto = reloc_entry->howto;
  asection *reloc_target_output_section;
  asymbol *symbol;

  symbol = *(reloc_entry->sym_ptr_ptr);

  /* If we are not producing relocatable output, return an error if
     the symbol is not defined. */
  if (bfd_is_und_section (symbol->section)
      && (symbol->flags & BSF_WEAK) == 0
      && output_bfd == NULL)
    flag = bfd_reloc_undefined;

  /* If there is a function supplied to handle this relocation type,
     call it.  */
  if (howto && howto->special_function)
    {
      bfd_reloc_status_type cont;

      /* Note - we do not call bfd_reloc_offset_in_range here as the
	 reloc_entry->address field might actually be valid for the
	 backend concerned.  It is up to the special_function itself
	 to call bfd_reloc_offset_in_range if needed.  */
      cont = howto->special_function (abfd, reloc_entry, symbol, data,
				      input_section, output_bfd,
				      error_message);
      if (cont != bfd_reloc_continue)
	return cont;
    }

  if (bfd_is_abs_section (symbol->section)
      && output_bfd != NULL)
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }

  /* PR 17512: file: 0f67f69d.  */
  if (howto == NULL)
    return bfd_reloc_undefined;

  /* Is the address of the relocation really within the section?  */
  octets = reloc_entry->address * bfd_octets_per_byte (abfd);
  if (!bfd_reloc_offset_in_range (howto, abfd, input_section, octets))
    return bfd_reloc_outofrange;

  /* Work out which section the relocation is targeted at and the
     initial relocation command value.  */

  /* Get symbol value.  (Common symbols are special.)  */
  if (bfd_is_com_section (symbol->section))
    relocation = 0;
  else
    relocation = symbol->value;

  reloc_target_output_section = symbol->section->output_section;

  /* Convert input-section-relative symbol value to absolute.  */
  if ((output_bfd && ! howto->partial_inplace)
      || reloc_target_output_section == NULL)
    output_base = 0;
  else
    output_base = reloc_target_output_section->vma;

  relocation += output_base + symbol->section->output_offset;

  /* Add in supplied addend.  */
  relocation += reloc_entry->addend;

  /* Here the variable relocation holds the final address of the
     symbol we are relocating against, plus any addend.  */

  if (howto->pc_relative)
    {
      /* This is a PC relative relocation.  We want to set RELOCATION
	 to the distance between the address of the symbol and the
	 location.  RELOCATION is already the address of the symbol. */

      relocation -=
	input_section->output_section->vma + input_section->output_offset;

      if (howto->pcrel_offset)
	relocation -= reloc_entry->address;
    }

  if (output_bfd != NULL)
    {
      if (! howto->partial_inplace)
	{
	  /* This is a partial relocation, and we want to apply the relocation
	     to the reloc entry rather than the raw data. Modify the reloc
	     inplace to reflect what we now know.  */
	  reloc_entry->addend = relocation;
	  reloc_entry->address += input_section->output_offset;
	  return flag;
	}
      else
	{
	  /* This is a partial relocation, but inplace, so modify the
	     reloc record a bit.  */

	  reloc_entry->address += input_section->output_offset;

	  /* WTF?? */
	  if (abfd->xvec->flavour == bfd_target_coff_flavour
	      && strcmp (abfd->xvec->name, "coff-Intel-little") != 0
	      && strcmp (abfd->xvec->name, "coff-Intel-big") != 0)
	    {
	      /* For m68k-coff, the addend was being subtracted twice during
		 relocation with -r.  Removing the line below this comment
		 fixes that problem; see PR 2953. */
	      relocation -= reloc_entry->addend;
	      reloc_entry->addend = 0;
	    }
	  else
	    {
	      reloc_entry->addend = relocation;
	    }
	}
    }

  /* FIXME: This overflow checking is incomplete, because the value
     might have overflowed before we get here.
     FIXME: We should also do overflow checking on the result after
     adding in the value contained in the object file.  */
  if (howto->complain_on_overflow != complain_overflow_dont
      && flag == bfd_reloc_ok)
    flag = bfd_check_overflow (howto->complain_on_overflow,
			       howto->bitsize,
			       howto->rightshift,
			       bfd_arch_bits_per_address (abfd),
			       relocation);

  /* Either we are relocating all the way, or we don't want to apply
     the relocation to the reloc entry (probably because there isn't
     any room in the output format to describe addends to relocs).  */

  /* The cast to bfd_vma avoids a bug in the Alpha OSF/1 C compiler
     (OSF version 1.3, compiler version 3.11).  */

  relocation >>= (bfd_vma) howto->rightshift;

  /* Shift everything up to where it's going to be used.  */
  relocation <<= (bfd_vma) howto->bitpos;

  /* Wait for the day when all have the mask in them.  */

#define DOIT(x) \
  x = ( (x & ~howto->dst_mask) | (((x & howto->src_mask) +  relocation) & howto->dst_mask))

  switch (howto->size)
    {
    case 5:
      {
	long x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
	x >>= 8;
	DOIT (x);
	bfd_put_16 (abfd, (bfd_vma) (x >> 8), (bfd_byte *) data + octets);
	bfd_put_8 (abfd, (x & 0xFF), (unsigned char *) data + 2 + octets);
      }
      break;

    case 0:
      {
	char x = bfd_get_8 (abfd, (char *) data + octets);
	DOIT (x);
	bfd_put_8 (abfd, x, (unsigned char *) data + octets);
      }
      break;

    case 1:
      {
	short x = bfd_get_16 (abfd, (bfd_byte *) data + octets);
	DOIT (x);
	bfd_put_16 (abfd, (bfd_vma) x, (unsigned char *) data + octets);
      }
      break;
    case 2:
      {
	long x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
	DOIT (x);
	bfd_put_32 (abfd, (bfd_vma) x, (bfd_byte *) data + octets);
      }
      break;
    case -2:
      {
	long x = bfd_get_32 (abfd, (bfd_byte *) data + octets);
	relocation = -relocation;
	DOIT (x);
	bfd_put_32 (abfd, (bfd_vma) x, (bfd_byte *) data + octets);
      }
      break;

    case -1:
      {
	long x = bfd_get_16 (abfd, (bfd_byte *) data + octets);
	relocation = -relocation;
	DOIT (x);
	bfd_put_16 (abfd, (bfd_vma) x, (bfd_byte *) data + octets);
      }
      break;

    case 3:
      /* Do nothing */
      break;

    case 4:
      abort ();
      break;
    default:
      return bfd_reloc_other;
    }

  return flag;
}

/*
FUNCTION
	bfd_install_relocation

DESCRIPTION
	This looks remarkably like <<bfd_perform_relocation>>, except it
	does not expect that the section contents have been filled in.
	I.e., it's suitable for use when creating, rather than applying
	a relocation.

	For now, this function should be considered reserved for the
	assembler.
*/

bfd_reloc_status_type
bfd_install_relocation (bfd *abfd,
			arelent *reloc_entry,
			void *data_start,
			bfd_vma data_start_offset,
			asection *input_section,
			char **error_message)
{
  bfd_vma relocation;
  bfd_reloc_status_type flag = bfd_reloc_ok;
  bfd_size_type octets;
  bfd_vma output_base = 0;
  reloc_howto_type *howto = reloc_entry->howto;
  asection *reloc_target_output_section;
  asymbol *symbol;
  bfd_byte *data;

  symbol = *(reloc_entry->sym_ptr_ptr);

  /* If there is a function supplied to handle this relocation type,
     call it.  It'll return `bfd_reloc_continue' if further processing
     can be done.  */
  if (howto && howto->special_function)
    {
      bfd_reloc_status_type cont;

      /* Note - we do not call bfd_reloc_offset_in_range here as the
	 reloc_entry->address field might actually be valid for the
	 backend concerned.  */
      /* XXX - The special_function calls haven't been fixed up to deal
	 with creating new relocations and section contents.  */
      cont = howto->special_function (abfd, reloc_entry, symbol,
				      /* XXX - Non-portable! */
				      ((bfd_byte *) data_start
				       - data_start_offset),
				      input_section, abfd, error_message);
      if (cont != bfd_reloc_continue)
	return cont;
    }

  if (bfd_is_abs_section (symbol->section))
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }

  /* No need to check for howto != NULL if !bfd_is_abs_section as
     it will have been checked in `bfd_perform_relocation already'.  */

  /* Is the address of the relocation really within the section?  */
  octets = reloc_entry->address * bfd_octets_per_byte (abfd);
  if (!bfd_reloc_offset_in_range (howto, abfd, input_section, octets))
    return bfd_reloc_outofrange;

  /* Work out which section the relocation is targeted at and the
     initial relocation command value.  */

  /* Get symbol value.  (Common symbols are special.)  */
  if (bfd_is_com_section (symbol->section))
    relocation = 0;
  else
    relocation = symbol->value;

  reloc_target_output_section = symbol->section->output_section;

  /* Convert input-section-relative symbol value to absolute.  */
  if (! howto->partial_inplace)
    output_base = 0;
  else
    output_base = reloc_target_output_section->vma;

  relocation += output_base + symbol->section->output_offset;

  /* Add in supplied addend.  */
  relocation += reloc_entry->addend;

  /* Here the variable relocation holds the final address of the
     symbol we are relocating against, plus any addend.  */

  if (howto->pc_relative)
    {
      /* This is a PC relative relocation.  We want to set RELOCATION
	 to the distance between the address of the symbol and the
	 location.  RELOCATION is already the address of the symbol. */

      relocation -=
	input_section->output_section->vma + input_section->output_offset;

      if (howto->pcrel_offset && howto->partial_inplace)
	relocation -= reloc_entry->address;
    }

  if (! howto->partial_inplace)
    {
      /* This is a partial relocation, and we want to apply the relocation
	 to the reloc entry rather than the raw data. Modify the reloc
	 inplace to reflect what we now know.  */
      reloc_entry->addend = relocation;
      reloc_entry->address += input_section->output_offset;
      return flag;
    }
  else
    {
      /* This is a partial relocation, but inplace, so modify the
	 reloc record a bit.

	 If we've relocated with a symbol with a section, change
	 into a ref to the section belonging to the symbol.  */
      reloc_entry->address += input_section->output_offset;

      /* WTF?? */
      if (abfd->xvec->flavour == bfd_target_coff_flavour
	  && strcmp (abfd->xvec->name, "coff-Intel-little") != 0
	  && strcmp (abfd->xvec->name, "coff-Intel-big") != 0)
	{

	  /* For m68k-coff, the addend was being subtracted twice during
	     relocation with -r.  Removing the line below this comment
	     fixes that problem; see PR 2953.b */
	  relocation -= reloc_entry->addend;
	  /* FIXME: There should be no target specific code here...  */
	  if (strcmp (abfd->xvec->name, "coff-z8k") != 0)
	    reloc_entry->addend = 0;
	}
      else
	{
	  reloc_entry->addend = relocation;
	}
    }

  /* FIXME: This overflow checking is incomplete, because the value
     might have overflowed before we get here.  For a correct check we
     need to compute the value in a size larger than bitsize, but we
     can't reasonably do that for a reloc the same size as a host
     machine word.
     FIXME: We should also do overflow checking on the result after
     adding in the value contained in the object file.  */
  if (howto->complain_on_overflow != complain_overflow_dont)
    flag = bfd_check_overflow (howto->complain_on_overflow,
			       howto->bitsize,
			       howto->rightshift,
			       bfd_arch_bits_per_address (abfd),
			       relocation);

  /* Either we are relocating all the way, or we don't want to apply
     the relocation to the reloc entry (probably because there isn't
     any room in the output format to describe addends to relocs).  */

  /* The cast to bfd_vma avoids a bug in the Alpha OSF/1 C compiler
     (OSF version 1.3, compiler version 3.11).  */

  relocation >>= (bfd_vma) howto->rightshift;

  /* Shift everything up to where it's going to be used.  */
  relocation <<= (bfd_vma) howto->bitpos;

  /* Wait for the day when all have the mask in them.  */

#define DOIT(x) \
  x = ( (x & ~howto->dst_mask) | (((x & howto->src_mask) +  relocation) & howto->dst_mask))

  data = (bfd_byte *) data_start + (octets - data_start_offset);

  switch (howto->size)
    {
    case 0:
      {
	char x = bfd_get_8 (abfd, data);
	DOIT (x);
	bfd_put_8 (abfd, x, data);
      }
      break;

    case 1:
      {
	short x = bfd_get_16 (abfd, data);
	DOIT (x);
	bfd_put_16 (abfd, (bfd_vma) x, data);
      }
      break;
    case 2:
      {
	long x = bfd_get_32 (abfd, data);
	DOIT (x);
	bfd_put_32 (abfd, (bfd_vma) x, data);
      }
      break;
    case -2:
      {
	long x = bfd_get_32 (abfd, data);
	relocation = -relocation;
	DOIT (x);
	bfd_put_32 (abfd, (bfd_vma) x, data);
      }
      break;

    case 3:
      /* Do nothing */
      break;

    case 4:
      {
	bfd_vma x = bfd_get_64 (abfd, data);
	DOIT (x);
	bfd_put_64 (abfd, x, data);
      }
      break;
    default:
      return bfd_reloc_other;
    }

  return flag;
}

/* This relocation routine is used by some of the backend linkers.
   They do not construct asymbol or arelent structures, so there is no
   reason for them to use bfd_perform_relocation.  Also,
   bfd_perform_relocation is so hacked up it is easier to write a new
   function than to try to deal with it. */

bfd_reloc_status_type
_bfd_final_link_relocate (reloc_howto_type *howto,
			  bfd *input_bfd,
			  asection *input_section,
			  bfd_byte *contents,
			  bfd_vma address,
			  bfd_vma value,
			  bfd_vma addend)
{
  bfd_vma relocation;
  bfd_size_type octets = address * bfd_octets_per_byte (input_bfd);

  /* Sanity check the address.  */
  if (!bfd_reloc_offset_in_range (howto, input_bfd, input_section, octets))
    return bfd_reloc_outofrange;

  /* This function assumes that we are dealing with a basic relocation
     against a symbol. */
  relocation = value + addend;

  /* If the relocation is PC relative, we want to set RELOCATION to
     the distance between the symbol (currently in RELOCATION) and the
     location we are relocating.  */
  if (howto->pc_relative)
    {
      relocation -= (input_section->output_section->vma
		     + input_section->output_offset);
      if (howto->pcrel_offset)
	relocation -= address;
    }

  return _bfd_relocate_contents (howto, input_bfd, relocation,
				 contents
				 + address * bfd_octets_per_byte (input_bfd));
}

/* Relocate a given location using a given value and howto.  */

bfd_reloc_status_type
_bfd_relocate_contents (reloc_howto_type *howto,
			bfd *input_bfd,
			bfd_vma relocation,
			bfd_byte *location)
{
  int size;
  bfd_vma x = 0;
  bfd_reloc_status_type flag;
  unsigned int rightshift = howto->rightshift;
  unsigned int bitpos = howto->bitpos;

  /* If the size is negative, negate RELOCATION.  This isn't very
     general.  */
  if (howto->size < 0)
    relocation = -relocation;

  /* Get the value we are going to relocate.  */
  size = bfd_get_reloc_size (howto);
  switch (size)
    {
    default:
      abort ();
    case 0:
      return bfd_reloc_ok;
    case 1:
      x = bfd_get_8 (input_bfd, location);
      break;
    case 2:
      x = bfd_get_16 (input_bfd, location);
      break;
    case 4:
      x = bfd_get_32 (input_bfd, location);
      break;
    case 8:
      abort ();
      break;
    }

  /* Check for overflow.  FIXME: We may drop bits during the addition
     which we don't check for.  We must either check at every single
     operation, which would be tedious, or we must do the computations
     in a type larger than bfd_vma, which would be inefficient.  */
  flag = bfd_reloc_ok;
  if (howto->complain_on_overflow != complain_overflow_dont)
    {
      bfd_vma addrmask, fieldmask, signmask, ss;
      bfd_vma a, b, sum;

      /* Get the values to be added together.  */
      fieldmask = N_ONES (howto->bitsize);
      signmask = ~fieldmask;
      addrmask = (N_ONES (bfd_arch_bits_per_address (input_bfd))
		  | (fieldmask << rightshift));
      a = (relocation & addrmask) >> rightshift;
      b = (x & howto->src_mask & addrmask) >> bitpos;
      addrmask >>= rightshift;

      switch (howto->complain_on_overflow)
	{
	case complain_overflow_signed:
	  /* If any sign bits are set, all sign bits must be set. */
	  signmask = ~(fieldmask >> 1);
	  /* Fall thru */

	case complain_overflow_bitfield:
	  /* Much like the signed check, but for a field one bit
	     wider.  */
	  ss = a & signmask;
	  if (ss != 0 && ss != (addrmask & signmask))
	    flag = bfd_reloc_overflow;

	  /* We only need this next bit of code if the sign bit of B
	     is below the sign bit of A.  */
	  ss = ((~howto->src_mask) >> 1) & howto->src_mask;
	  ss >>= bitpos;

	  /* Set all the bits above the sign bit.  */
	  b = (b ^ ss) - ss;

	  /* Now we can do the addition.  */
	  sum = a + b;

	  /* See if the result has the correct sign. */
	  if (((~(a ^ b)) & (a ^ sum)) & signmask & addrmask)
	    flag = bfd_reloc_overflow;
	  break;

	case complain_overflow_unsigned:
	  /* Checking for an unsigned overflow is relatively easy:
	     trim the addresses and add, and trim the result as well.*/
	  sum = (a + b) & addrmask;
	  if ((a | b | sum) & signmask)
	    flag = bfd_reloc_overflow;
	  break;

	default:
	  abort ();
	}
    }

  /* Put RELOCATION in the right bits.  */
  relocation >>= (bfd_vma) rightshift;
  relocation <<= (bfd_vma) bitpos;

  /* Add RELOCATION to the right bits of X.  */
  x = ((x & ~howto->dst_mask)
       | (((x & howto->src_mask) + relocation) & howto->dst_mask));

  /* Put the relocated value back in the object file.  */
  switch (size)
    {
    default:
      abort ();
    case 1:
      bfd_put_8 (input_bfd, x, location);
      break;
    case 2:
      bfd_put_16 (input_bfd, x, location);
      break;
    case 4:
      bfd_put_32 (input_bfd, x, location);
      break;
    case 8:
      abort ();
      break;
    }

  return flag;
}

/* Clear a given location using a given howto, by applying a fixed relocation
   value and discarding any in-place addend. */

void
_bfd_clear_contents (reloc_howto_type *howto,
		     bfd *input_bfd,
		     asection *input_section,
		     bfd_byte *location)
{
  int size;
  bfd_vma x = 0;

  /* Get the value we are going to relocate.  */
  size = bfd_get_reloc_size (howto);
  switch (size)
    {
    default:
      abort ();
    case 0:
      return;
    case 1:
      x = bfd_get_8 (input_bfd, location);
      break;
    case 2:
      x = bfd_get_16 (input_bfd, location);
      break;
    case 4:
      x = bfd_get_32 (input_bfd, location);
      break;
    case 8:
      abort ();
      break;
    }

  /* Zero out the unwanted bits of X.  */
  x &= ~howto->dst_mask;

  /* For a range list, use 1 instead of 0 as placeholder.  0
     would terminate the list, hiding any later entries.  */
  if (strcmp (bfd_get_section_name (input_bfd, input_section),
	      ".debug_ranges") == 0
      && (howto->dst_mask & 1) != 0)
    x |= 1;

  /* Put the relocated value back in the object file.  */
  switch (size)
    {
    default:
    case 0:
      abort ();
    case 1:
      bfd_put_8 (input_bfd, x, location);
      break;
    case 2:
      bfd_put_16 (input_bfd, x, location);
      break;
    case 4:
      bfd_put_32 (input_bfd, x, location);
      break;
    case 8:
      abort ();
      break;
    }
}

/*
DOCDD
INODE
	howto manager,  , typedef arelent, Relocations

SUBSECTION
	The howto manager
*/

/*
TYPEDEF
	bfd_reloc_code_type
	*mega snip*
*/

/*
FUNCTION
	bfd_reloc_type_lookup
	bfd_reloc_name_lookup
DESCRIPTION
	Return a pointer to a howto structure which, when
	invoked, will perform the relocation @var{code} on data from the
	architecture noted.

*/

reloc_howto_type *
bfd_reloc_type_lookup (bfd *abfd, bfd_reloc_code_real_type code)
{
  return BFD_SEND (abfd, reloc_type_lookup, (abfd, code));
}

reloc_howto_type *
bfd_reloc_name_lookup (bfd *abfd, const char *reloc_name)
{
  return BFD_SEND (abfd, reloc_name_lookup, (abfd, reloc_name));
}

static reloc_howto_type bfd_howto_32 =
HOWTO (0, 00, 2, 32, FALSE, 0, complain_overflow_dont, 0, "VRT32", FALSE, 0xffffffff, 0xffffffff, TRUE);

/*
INTERNAL_FUNCTION
	bfd_default_reloc_type_lookup

DESCRIPTION
	Provides a default relocation lookup routine for any architecture.

*/

reloc_howto_type *
bfd_default_reloc_type_lookup (bfd *abfd, bfd_reloc_code_real_type code)
{
  switch (code)
    {
    case BFD_RELOC_CTOR:
      /* The type of reloc used in a ctor, which will be as wide as the
	 address - so either a 64, 32, or 16 bitter.  */
      switch (bfd_arch_bits_per_address (abfd))
	{
	case 64:
	  BFD_FAIL ();
	  break;
	case 32:
	  return &bfd_howto_32;
	case 16:
	  BFD_FAIL ();
	  break;
	default:
	  BFD_FAIL ();
	}
      break;
    default:
      BFD_FAIL ();
    }
  return NULL;
}

/*
FUNCTION
	bfd_get_reloc_code_name

DESCRIPTION
	Provides a printable name for the supplied relocation code.
	Useful mainly for printing error messages.
*/

const char *
bfd_get_reloc_code_name (bfd_reloc_code_real_type code)
{
  if (code > BFD_RELOC_UNUSED)
    return 0;
  return bfd_reloc_code_real_names[code];
}

/*
INTERNAL_FUNCTION
	bfd_generic_relax_section

DESCRIPTION
	Provides default handling for relaxing for back ends which
	don't do relaxing.
*/

bfd_boolean
bfd_generic_relax_section (bfd *abfd ATTRIBUTE_UNUSED,
			   asection *section ATTRIBUTE_UNUSED,
			   struct bfd_link_info *link_info ATTRIBUTE_UNUSED,
			   bfd_boolean *again)
{
  if (bfd_link_relocatable (link_info))
    (*link_info->callbacks->einfo)
      (_("%P%F: --relax and -r may not be used together\n"));

  *again = FALSE;
  return TRUE;
}

/*
INTERNAL_FUNCTION
	bfd_generic_gc_sections

DESCRIPTION
	Provides default handling for relaxing for back ends which
	don't do section gc -- i.e., does nothing.
*/

bfd_boolean
bfd_generic_gc_sections (bfd *abfd ATTRIBUTE_UNUSED,
			 struct bfd_link_info *info ATTRIBUTE_UNUSED)
{
  return TRUE;
}

/*
INTERNAL_FUNCTION
	bfd_generic_lookup_section_flags


DESCRIPTION
	Provides default handling for section flags lookup
	-- i.e., does nothing.
	Returns FALSE if the section should be omitted, otherwise TRUE.
*/

bfd_boolean
bfd_generic_lookup_section_flags (struct bfd_link_info *info ATTRIBUTE_UNUSED,
				  struct flag_info *flaginfo,
				  asection *section ATTRIBUTE_UNUSED)
{
  if (flaginfo != NULL)
    {
      _bfd_error_handler (_("INPUT_SECTION_FLAGS are not supported"));
      return FALSE;
    }
  return TRUE;
}

/*
INTERNAL_FUNCTION
	bfd_generic_merge_sections

DESCRIPTION
	Provides default handling for SEC_MERGE section merging for back ends
	which don't have SEC_MERGE support -- i.e., does nothing.
*/

bfd_boolean
bfd_generic_merge_sections (bfd *abfd ATTRIBUTE_UNUSED,
			    struct bfd_link_info *link_info ATTRIBUTE_UNUSED)
{
  return TRUE;
}

/*
INTERNAL_FUNCTION
	bfd_generic_get_relocated_section_contents

DESCRIPTION
	Provides default handling of relocation effort for back ends
	which can't be bothered to do it efficiently.

*/

bfd_byte *
bfd_generic_get_relocated_section_contents (bfd *abfd,
					    struct bfd_link_info *link_info,
					    struct bfd_link_order *link_order,
					    bfd_byte *data,
					    bfd_boolean relocatable,
					    asymbol **symbols)
{
  bfd *input_bfd = link_order->u.indirect.section->owner;
  asection *input_section = link_order->u.indirect.section;
  long reloc_size;
  arelent **reloc_vector;
  long reloc_count;

  reloc_size = bfd_get_reloc_upper_bound (input_bfd, input_section);
  if (reloc_size < 0)
    return NULL;

  /* Read in the section.  */
  if (!bfd_get_full_section_contents (input_bfd, input_section, &data))
    return NULL;

  if (data == NULL)
    return NULL;

  if (reloc_size == 0)
    return data;

  reloc_vector = (arelent **) bfd_malloc (reloc_size);
  if (reloc_vector == NULL)
    return NULL;

  reloc_count = bfd_canonicalize_reloc (input_bfd,
					input_section,
					reloc_vector,
					symbols);
  if (reloc_count < 0)
    goto error_return;

  if (reloc_count > 0)
    {
      arelent **parent;

      for (parent = reloc_vector; *parent != NULL; parent++)
	{
	  char *error_message = NULL;
	  asymbol *symbol;
	  bfd_reloc_status_type r;

	  symbol = *(*parent)->sym_ptr_ptr;
	  /* PR ld/19628: A specially crafted input file
	     can result in a NULL symbol pointer here.  */
	  if (symbol == NULL)
	    {
	      link_info->callbacks->einfo
		/* xgettext:c-format */
		(_("%X%P: %pB(%pA): error: relocation for offset %V has no value\n"),
		 abfd, input_section, (* parent)->address);
	      goto error_return;
	    }

	  if (symbol->section && discarded_section (symbol->section))
	    {
	      bfd_byte *p;
	      static reloc_howto_type none_howto
		= HOWTO (0, 0, 0, 0, FALSE, 0, complain_overflow_dont, NULL,
			 "unused", FALSE, 0, 0, FALSE);

	      p = data + (*parent)->address * bfd_octets_per_byte (input_bfd);
	      _bfd_clear_contents ((*parent)->howto, input_bfd, input_section,
				   p);
	      (*parent)->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
	      (*parent)->addend = 0;
	      (*parent)->howto = &none_howto;
	      r = bfd_reloc_ok;
	    }
	  else
	    r = bfd_perform_relocation (input_bfd,
					*parent,
					data,
					input_section,
					relocatable ? abfd : NULL,
					&error_message);

	  if (relocatable)
	    {
	      asection *os = input_section->output_section;

	      /* A partial link, so keep the relocs.  */
	      os->orelocation[os->reloc_count] = *parent;
	      os->reloc_count++;
	    }

	  if (r != bfd_reloc_ok)
	    {
	      switch (r)
		{
		case bfd_reloc_undefined:
		  (*link_info->callbacks->undefined_symbol)
		    (link_info, bfd_asymbol_name (*(*parent)->sym_ptr_ptr),
		     input_bfd, input_section, (*parent)->address, TRUE);
		  break;
		case bfd_reloc_dangerous:
		  BFD_ASSERT (error_message != NULL);
		  (*link_info->callbacks->reloc_dangerous)
		    (link_info, error_message,
		     input_bfd, input_section, (*parent)->address);
		  break;
		case bfd_reloc_overflow:
		  (*link_info->callbacks->reloc_overflow)
		    (link_info, NULL,
		     bfd_asymbol_name (*(*parent)->sym_ptr_ptr),
		     (*parent)->howto->name, (*parent)->addend,
		     input_bfd, input_section, (*parent)->address);
		  break;
		case bfd_reloc_outofrange:
		  /* PR ld/13730:
		     This error can result when processing some partially
		     complete binaries.  Do not abort, but issue an error
		     message instead.  */
		  link_info->callbacks->einfo
		    /* xgettext:c-format */
		    (_("%X%P: %pB(%pA): relocation \"%pR\" goes out of range\n"),
		     abfd, input_section, * parent);
		  goto error_return;

		case bfd_reloc_notsupported:
		  /* PR ld/17512
		     This error can result when processing a corrupt binary.
		     Do not abort.  Issue an error message instead.  */
		  link_info->callbacks->einfo
		    /* xgettext:c-format */
		    (_("%X%P: %pB(%pA): relocation \"%pR\" is not supported\n"),
		     abfd, input_section, * parent);
		  goto error_return;

		default:
		  /* PR 17512; file: 90c2a92e.
		     Report unexpected results, without aborting.  */
		  link_info->callbacks->einfo
		    /* xgettext:c-format */
		    (_("%X%P: %pB(%pA): relocation \"%pR\" returns an unrecognized value %x\n"),
		     abfd, input_section, * parent, r);
		  break;
		}

	    }
	}
    }

  free (reloc_vector);
  return data;

error_return:
  free (reloc_vector);
  return NULL;
}

/*
INTERNAL_FUNCTION
	_bfd_generic_set_reloc

DESCRIPTION
	Installs a new set of internal relocations in SECTION.
*/

void
_bfd_generic_set_reloc (bfd *abfd ATTRIBUTE_UNUSED,
			sec_ptr section,
			arelent **relptr,
			unsigned int count)
{
  section->orelocation = relptr;
  section->reloc_count = count;
}

/*
INTERNAL_FUNCTION
	_bfd_unrecognized_reloc

DESCRIPTION
	Reports an unrecognized reloc.
	Written as a function in order to reduce code duplication.
	Returns FALSE so that it can be called from a return statement.
*/

bfd_boolean
_bfd_unrecognized_reloc (bfd * abfd, sec_ptr section, unsigned int r_type)
{
   /* xgettext:c-format */
  _bfd_error_handler (_("%pB: unrecognized relocation type %#x in section `%pA'"),
		      abfd, r_type, section);

  /* PR 21803: Suggest the most likely cause of this error.  */
  _bfd_error_handler (_("is this version of the linker - %s - out of date ?"),
		      BFD_VERSION_STRING);

  bfd_set_error (bfd_error_bad_value);
  return FALSE;
}

reloc_howto_type *
_bfd_norelocs_bfd_reloc_type_lookup
    (bfd *abfd,
     bfd_reloc_code_real_type code ATTRIBUTE_UNUSED)
{
  return (reloc_howto_type *) _bfd_ptr_bfd_null_error (abfd);
}

reloc_howto_type *
_bfd_norelocs_bfd_reloc_name_lookup (bfd *abfd,
				     const char *reloc_name ATTRIBUTE_UNUSED)
{
  return (reloc_howto_type *) _bfd_ptr_bfd_null_error (abfd);
}

long
_bfd_nodynamic_canonicalize_dynamic_reloc (bfd *abfd,
					   arelent **relp ATTRIBUTE_UNUSED,
					   asymbol **symp ATTRIBUTE_UNUSED)
{
  return _bfd_long_bfd_n1_error (abfd);
}
