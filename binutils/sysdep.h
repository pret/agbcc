/* sysdep.h -- handle host dependencies for binutils
   Copyright (C) 1991-2018 Free Software Foundation, Inc.

   This file is part of GNU Binutils.

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

#ifndef _BIN_SYSDEP_H
#define _BIN_SYSDEP_H

#include "config.h"
#include "ansidecl.h"
#include <stdio.h>
#include <sys/types.h>

#include "bfdver.h"

#include <stdarg.h>

#include "fopen-bin.h"

#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef STRING_WITH_STRINGS
#include <string.h>
#include <strings.h>
#else
#include <string.h>
#endif

#include <stdlib.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "binary-io.h"

#if !HAVE_DECL_STPCPY
extern char *stpcpy (char *, const char *);
#endif

#ifndef O_RDONLY
#define O_RDONLY 0
#endif

#ifndef O_RDWR
#define O_RDWR 2
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
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

/* Used by ar.c and objcopy.c.  */
#define BUFSIZE 8192

/* For PATH_MAX.  */
#include <limits.h>
#ifndef PATH_MAX
/* For MAXPATHLEN.  */
# ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
# endif
# ifndef PATH_MAX
#  ifdef MAXPATHLEN
#   define PATH_MAX MAXPATHLEN
#  else
#   define PATH_MAX 1024
#  endif
# endif
#endif

#if SIZEOF_LONG_LONG > SIZEOF_LONG
/* We can't use any bfd types here since readelf may define BFD64 and
   objdump may not.  */
#define HOST_WIDEST_INT long long
#else
#define HOST_WIDEST_INT long
#endif

#endif /* _BIN_SYSDEP_H */
