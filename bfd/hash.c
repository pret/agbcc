/* hash.c -- hash table routines for BFD
   Copyright (C) 1993-2018 Free Software Foundation, Inc.
   Written by Steve Chamberlain <sac@cygnus.com>

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

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "objalloc.h"
#include "libiberty.h"

/*
SECTION
	Hash Tables

@cindex Hash tables
	BFD provides a simple set of hash table functions.  Routines
	are provided to initialize a hash table, to free a hash table,
	to look up a string in a hash table and optionally create an
	entry for it, and to traverse a hash table.  There is
	currently no routine to delete an string from a hash table.
	snip
*/

/* The default number of entries to use when creating a hash table.  */
#define DEFAULT_SIZE 4051

/* The following function returns a nearest prime number which is
   greater than N, and near a power of two.  Copied from libiberty.
   Returns zero for ridiculously large N to signify an error.  */

static uint32_t
higher_prime_number (uint32_t n)
{
  /* These are primes that are near, but slightly smaller than, a
     power of two.  */
  static const uint32_t primes[] =
    {
      31U,
      61U,
      127U,
      251U,
      509U,
      1021U,
      2039U,
      4093U,
      8191U,
      16381U,
      32749U,
      65521U,
      131071U,
      262139U,
      524287U,
      1048573U,
      2097143U,
      4194301U,
      8388593U,
      16777213U,
      33554393U,
      67108859U,
      134217689U,
      268435399U,
      536870909U,
      1073741789U,
      2147483647U,
					/* 4294967291L */
      (2147483647U) + (2147483644U),
  };

  const uint32_t *low = &primes[0];
  const uint32_t *high = &primes[sizeof (primes) / sizeof (primes[0])];

  while (low != high)
    {
      const uint32_t *mid = low + (high - low) / 2;
      if (n >= *mid)
	low = mid + 1;
      else
	high = mid;
    }

  if (n >= *low)
    return 0;

  return *low;
}

static uint32_t bfd_default_hash_table_size = DEFAULT_SIZE;

/* Create a new hash table, given a number of entries.  */

bfd_boolean
bfd_hash_table_init_n (struct bfd_hash_table *table,
		       struct bfd_hash_entry *(*newfunc) (struct bfd_hash_entry *,
							  struct bfd_hash_table *,
							  const char *),
		       unsigned int entsize,
		       unsigned int size)
{
  uint32_t alloc;

  alloc = size;
  alloc *= sizeof (struct bfd_hash_entry *);
  if (alloc / sizeof (struct bfd_hash_entry *) != size)
    {
      bfd_set_error (bfd_error_no_memory);
      return FALSE;
    }

  table->memory = (void *) objalloc_create ();
  if (table->memory == NULL)
    {
      bfd_set_error (bfd_error_no_memory);
      return FALSE;
    }
  table->table = (struct bfd_hash_entry **)
      objalloc_alloc ((struct objalloc *) table->memory, alloc);
  if (table->table == NULL)
    {
      bfd_hash_table_free (table);
      bfd_set_error (bfd_error_no_memory);
      return FALSE;
    }
  memset ((void *) table->table, 0, alloc);
  table->size = size;
  table->entsize = entsize;
  table->count = 0;
  table->frozen = 0;
  table->newfunc = newfunc;
  return TRUE;
}

/* Create a new hash table with the default number of entries.  */

bfd_boolean
bfd_hash_table_init (struct bfd_hash_table *table,
		     struct bfd_hash_entry *(*newfunc) (struct bfd_hash_entry *,
							struct bfd_hash_table *,
							const char *),
		     unsigned int entsize)
{
  return bfd_hash_table_init_n (table, newfunc, entsize,
				bfd_default_hash_table_size);
}

/* Free a hash table.  */

void
bfd_hash_table_free (struct bfd_hash_table *table)
{
  objalloc_free ((struct objalloc *) table->memory);
  table->memory = NULL;
}

static inline uint32_t
bfd_hash_hash (const char *string, unsigned int *lenp)
{
  const unsigned char *s;
  uint32_t hash;
  unsigned int len;
  unsigned int c;

  BFD_ASSERT (string != NULL);
  hash = 0;
  len = 0;
  s = (const unsigned char *) string;
  while ((c = *s++) != '\0')
    {
      hash += c + (c << 17);
      hash ^= hash >> 2;
    }
  len = (s - (const unsigned char *) string) - 1;
  hash += len + (len << 17);
  hash ^= hash >> 2;
  if (lenp != NULL)
    *lenp = len;
  return hash;
}

/* Look up a string in a hash table.  */

struct bfd_hash_entry *
bfd_hash_lookup (struct bfd_hash_table *table,
		 const char *string,
		 bfd_boolean create,
		 bfd_boolean copy)
{
  uint32_t hash;
  struct bfd_hash_entry *hashp;
  unsigned int len;
  unsigned int _index;

  hash = bfd_hash_hash (string, &len);
  _index = hash % table->size;
  for (hashp = table->table[_index];
       hashp != NULL;
       hashp = hashp->next)
    {
      if (hashp->hash == hash
	  && strcmp (hashp->string, string) == 0)
	return hashp;
    }

  if (! create)
    return NULL;

  if (copy)
    {
      char *new_string;

      new_string = (char *) objalloc_alloc ((struct objalloc *) table->memory,
					    len + 1);
      if (!new_string)
	{
	  bfd_set_error (bfd_error_no_memory);
	  return NULL;
	}
      memcpy (new_string, string, len + 1);
      string = new_string;
    }

  return bfd_hash_insert (table, string, hash);
}

/* Insert an entry in a hash table.  */

struct bfd_hash_entry *
bfd_hash_insert (struct bfd_hash_table *table,
		 const char *string,
		 uint32_t hash)
{
  struct bfd_hash_entry *hashp;
  unsigned int _index;

  hashp = (*table->newfunc) (NULL, table, string);
  if (hashp == NULL)
    return NULL;
  hashp->string = string;
  hashp->hash = hash;
  _index = hash % table->size;
  hashp->next = table->table[_index];
  table->table[_index] = hashp;
  table->count++;

  if (!table->frozen && table->count > table->size * 3 / 4)
    {
      uint32_t newsize = higher_prime_number (table->size);
      struct bfd_hash_entry **newtable;
      unsigned int hi;
      uint32_t alloc = newsize * sizeof (struct bfd_hash_entry *);

      /* If we can't find a higher prime, or we can't possibly alloc
	 that much memory, don't try to grow the table.  */
      if (newsize == 0 || alloc / sizeof (struct bfd_hash_entry *) != newsize)
	{
	  table->frozen = 1;
	  return hashp;
	}

      newtable = ((struct bfd_hash_entry **)
		  objalloc_alloc ((struct objalloc *) table->memory, alloc));
      if (newtable == NULL)
	{
	  table->frozen = 1;
	  return hashp;
	}
      memset (newtable, 0, alloc);

      for (hi = 0; hi < table->size; hi ++)
	while (table->table[hi])
	  {
	    struct bfd_hash_entry *chain = table->table[hi];
	    struct bfd_hash_entry *chain_end = chain;

	    while (chain_end->next && chain_end->next->hash == chain->hash)
	      chain_end = chain_end->next;

	    table->table[hi] = chain_end->next;
	    _index = chain->hash % newsize;
	    chain_end->next = newtable[_index];
	    newtable[_index] = chain;
	  }
      table->table = newtable;
      table->size = newsize;
    }

  return hashp;
}

/* Rename an entry in a hash table.  */

void
bfd_hash_rename (struct bfd_hash_table *table,
		 const char *string,
		 struct bfd_hash_entry *ent)
{
  unsigned int _index;
  struct bfd_hash_entry **pph;

  _index = ent->hash % table->size;
  for (pph = &table->table[_index]; *pph != NULL; pph = &(*pph)->next)
    if (*pph == ent)
      break;
  if (*pph == NULL)
    abort ();

  *pph = ent->next;
  ent->string = string;
  ent->hash = bfd_hash_hash (string, NULL);
  _index = ent->hash % table->size;
  ent->next = table->table[_index];
  table->table[_index] = ent;
}

/* Replace an entry in a hash table.  */

void
bfd_hash_replace (struct bfd_hash_table *table,
		  struct bfd_hash_entry *old,
		  struct bfd_hash_entry *nw)
{
  unsigned int _index;
  struct bfd_hash_entry **pph;

  _index = old->hash % table->size;
  for (pph = &table->table[_index];
       (*pph) != NULL;
       pph = &(*pph)->next)
    {
      if (*pph == old)
	{
	  *pph = nw;
	  return;
	}
    }

  abort ();
}

/* Allocate space in a hash table.  */

void *
bfd_hash_allocate (struct bfd_hash_table *table,
		   unsigned int size)
{
  void * ret;

  ret = objalloc_alloc ((struct objalloc *) table->memory, size);
  if (ret == NULL && size != 0)
    bfd_set_error (bfd_error_no_memory);
  return ret;
}

/* Base method for creating a new hash table entry.  */

struct bfd_hash_entry *
bfd_hash_newfunc (struct bfd_hash_entry *entry,
		  struct bfd_hash_table *table,
		  const char *string ATTRIBUTE_UNUSED)
{
  if (entry == NULL)
    entry = (struct bfd_hash_entry *) bfd_hash_allocate (table,
							 sizeof (* entry));
  return entry;
}

/* Traverse a hash table.  */

void
bfd_hash_traverse (struct bfd_hash_table *table,
		   bfd_boolean (*func) (struct bfd_hash_entry *, void *),
		   void * info)
{
  unsigned int i;

  table->frozen = 1;
  for (i = 0; i < table->size; i++)
    {
      struct bfd_hash_entry *p;

      for (p = table->table[i]; p != NULL; p = p->next)
	if (! (*func) (p, info))
	  goto out;
    }
 out:
  table->frozen = 0;
}

uint32_t
bfd_hash_set_default_size (uint32_t hash_size)
{
  /* Extend this prime list if you want more granularity of hash table size.  */
  static const uint32_t hash_size_primes[] =
    {
      31, 61, 127, 251, 509, 1021, 2039, 4091, 8191, 16381, 32749, 65537
    };
  unsigned int _index;

  /* Work out best prime number near the hash_size.  */
  for (_index = 0; _index < ARRAY_SIZE (hash_size_primes) - 1; ++_index)
    if (hash_size <= hash_size_primes[_index])
      break;

  bfd_default_hash_table_size = hash_size_primes[_index];
  return bfd_default_hash_table_size;
}

/* A few different object file formats (a.out, COFF, ELF) use a string
   table.  These functions support adding strings to a string table,
   returning the byte offset, and writing out the table.

   Possible improvements:
   + look for strings matching trailing substrings of other strings
   + better data structures?  balanced trees?
   + look at reducing memory use elsewhere -- maybe if we didn't have
     to construct the entire symbol table at once, we could get by
     with smaller amounts of VM?  (What effect does that have on the
     string table reductions?)  */

/* An entry in the strtab hash table.  */

struct strtab_hash_entry
{
  struct bfd_hash_entry root;
  /* Index in string table.  */
  bfd_size_type index;
  /* Next string in strtab.  */
  struct strtab_hash_entry *next;
};

/* The strtab hash table.  */

struct bfd_strtab_hash
{
  struct bfd_hash_table table;
  /* Size of strtab--also next available index.  */
  bfd_size_type size;
  /* First string in strtab.  */
  struct strtab_hash_entry *first;
  /* Last string in strtab.  */
  struct strtab_hash_entry *last;
  /* Whether to precede strings with a two byte length, as in the
     XCOFF .debug section.  */
  bfd_boolean xcoff;
};

/* Routine to create an entry in a strtab.  */

static struct bfd_hash_entry *
strtab_hash_newfunc (struct bfd_hash_entry *entry,
		     struct bfd_hash_table *table,
		     const char *string)
{
  struct strtab_hash_entry *ret = (struct strtab_hash_entry *) entry;

  /* Allocate the structure if it has not already been allocated by a
     subclass.  */
  if (ret == NULL)
    ret = (struct strtab_hash_entry *) bfd_hash_allocate (table,
							  sizeof (* ret));
  if (ret == NULL)
    return NULL;

  /* Call the allocation method of the superclass.  */
  ret = (struct strtab_hash_entry *)
	 bfd_hash_newfunc ((struct bfd_hash_entry *) ret, table, string);

  if (ret)
    {
      /* Initialize the local fields.  */
      ret->index = (bfd_size_type) -1;
      ret->next = NULL;
    }

  return (struct bfd_hash_entry *) ret;
}

/* Look up an entry in an strtab.  */

#define strtab_hash_lookup(t, string, create, copy) \
  ((struct strtab_hash_entry *) \
   bfd_hash_lookup (&(t)->table, (string), (create), (copy)))

/* Create a new strtab.  */

struct bfd_strtab_hash *
_bfd_stringtab_init (void)
{
  struct bfd_strtab_hash *table;
  bfd_size_type amt = sizeof (* table);

  table = (struct bfd_strtab_hash *) bfd_malloc (amt);
  if (table == NULL)
    return NULL;

  if (!bfd_hash_table_init (&table->table, strtab_hash_newfunc,
			    sizeof (struct strtab_hash_entry)))
    {
      free (table);
      return NULL;
    }

  table->size = 0;
  table->first = NULL;
  table->last = NULL;
  table->xcoff = FALSE;

  return table;
}

/* Free a strtab.  */

void
_bfd_stringtab_free (struct bfd_strtab_hash *table)
{
  bfd_hash_table_free (&table->table);
  free (table);
}

/* Get the index of a string in a strtab, adding it if it is not
   already present.  If HASH is FALSE, we don't really use the hash
   table, and we don't eliminate duplicate strings.  If COPY is true
   then store a copy of STR if creating a new entry.  */

bfd_size_type
_bfd_stringtab_add (struct bfd_strtab_hash *tab,
		    const char *str,
		    bfd_boolean hash,
		    bfd_boolean copy)
{
  struct strtab_hash_entry *entry;

  if (hash)
    {
      entry = strtab_hash_lookup (tab, str, TRUE, copy);
      if (entry == NULL)
	return (bfd_size_type) -1;
    }
  else
    {
      entry = (struct strtab_hash_entry *) bfd_hash_allocate (&tab->table,
							      sizeof (* entry));
      if (entry == NULL)
	return (bfd_size_type) -1;
      if (! copy)
	entry->root.string = str;
      else
	{
	  size_t len = strlen (str) + 1;
	  char *n;

	  n = (char *) bfd_hash_allocate (&tab->table, len);
	  if (n == NULL)
	    return (bfd_size_type) -1;
	  memcpy (n, str, len);
	  entry->root.string = n;
	}
      entry->index = (bfd_size_type) -1;
      entry->next = NULL;
    }

  if (entry->index == (bfd_size_type) -1)
    {
      entry->index = tab->size;
      tab->size += strlen (str) + 1;
      if (tab->xcoff)
	{
	  entry->index += 2;
	  tab->size += 2;
	}
      if (tab->first == NULL)
	tab->first = entry;
      else
	tab->last->next = entry;
      tab->last = entry;
    }

  return entry->index;
}

/* Get the number of bytes in a strtab.  */

bfd_size_type
_bfd_stringtab_size (struct bfd_strtab_hash *tab)
{
  return tab->size;
}

/* Write out a strtab.  ABFD must already be at the right location in
   the file.  */

bfd_boolean
_bfd_stringtab_emit (bfd *abfd, struct bfd_strtab_hash *tab)
{
  struct strtab_hash_entry *entry;

  for (entry = tab->first; entry != NULL; entry = entry->next)
    {
      const char *str;
      size_t len;

      str = entry->root.string;
      len = strlen (str) + 1;

      if (bfd_bwrite ((void *) str, (bfd_size_type) len, abfd) != len)
	return FALSE;
    }

  return TRUE;
}
