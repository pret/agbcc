/* sysdep.h -- handle host dependencies for the BFD library
   Copyright (C) 1995-2018 Free Software Foundation, Inc.
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

#ifndef BFD_SYSDEP_H
#define BFD_SYSDEP_H

#ifdef PACKAGE
#error sysdep.h must be included in lieu of config.h
#endif

#include "config.h"

#include "ansidecl.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#if !(defined(errno) || defined(_MSC_VER) && defined(_INC_ERRNO))
extern int errno;
#endif

#ifdef STRING_WITH_STRINGS
#include <string.h>
#include <strings.h>
#else
#include <string.h>
#endif
#include <stdlib.h>

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif /* HAVE_SYS_RESOURCE_H */

#include "fopen-bin.h"

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#else
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#endif

#ifndef O_RDONLY
#define O_RDONLY 0
#endif
#ifndef O_WRONLY
#define O_WRONLY 1
#endif
#ifndef O_RDWR
#define O_RDWR 2
#endif
#ifndef O_ACCMODE
#define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
#endif
/* Systems that don't already define this, don't need it.  */
#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#include "filenames.h"

#if !HAVE_DECL_FFS
extern int ffs (int);
#endif

#ifdef HAVE_FTELLO
#if !HAVE_DECL_FTELLO
extern off_t ftello (FILE *stream);
#endif
#endif

#ifdef HAVE_FTELLO64
#if !HAVE_DECL_FTELLO64
extern off64_t ftello64 (FILE *stream);
#endif
#endif

#ifdef HAVE_FSEEKO
#if !HAVE_DECL_FSEEKO
extern int fseeko (FILE *stream, off_t offset, int whence);
#endif
#endif

#ifdef HAVE_FSEEKO64
#if !HAVE_DECL_FSEEKO64
extern int fseeko64 (FILE *stream, off64_t offset, int whence);
#endif
#endif

/* Define offsetof for those systems which lack it */

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

# define gettext(Msgid) (Msgid)
# define dgettext(Domainname, Msgid) (Msgid)
# define dcgettext(Domainname, Msgid, Category) (Msgid)
# define ngettext(Msgid1, Msgid2, n) \
  (n == 1 ? Msgid1 : Msgid2)
# define dngettext(Domainname, Msgid1, Msgid2, n) \
  (n == 1 ? Msgid1 : Msgid2)
# define dcngettext(Domainname, Msgid1, Msgid2, n, Category) \
  (n == 1 ? Msgid1 : Msgid2)
# define textdomain(Domainname) do {} while (0)
# define bindtextdomain(Domainname, Dirname) do {} while (0)
# define _(String) (String)
# define N_(String) (String)

#endif /* ! defined (BFD_SYSDEP_H) */
