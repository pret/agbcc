/* Symbol concatenation utilities.

   Copyright (C) 1998-2018 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef SYM_CAT_H
#define SYM_CAT_H

#define CONCAT2(a,b)	 a##b
#define CONCAT3(a,b,c)	 a##b##c
#define CONCAT4(a,b,c,d) a##b##c##d
#define CONCAT5(a,b,c,d,e) a##b##c##d##e
#define CONCAT6(a,b,c,d,e,f) a##b##c##d##e##f
#define STRINGX(s) #s

#define XCONCAT2(a,b)     CONCAT2(a,b)
#define XCONCAT3(a,b,c)   CONCAT3(a,b,c)
#define XCONCAT4(a,b,c,d) CONCAT4(a,b,c,d)
#define XCONCAT5(a,b,c,d,e) CONCAT5(a,b,c,d,e)
#define XCONCAT6(a,b,c,d,e,f) CONCAT6(a,b,c,d,e,f)

/* Note the layer of indirection here is typically used to allow
   stringification of the expansion of macros.  I.e. "#define foo
   bar", "XSTRING(foo)", to yield "bar".  Be aware that this only
   works for __STDC__, not for traditional C which will still resolve
   to "foo".  */
#define XSTRING(s) STRINGX(s) 

#endif /* SYM_CAT_H */
