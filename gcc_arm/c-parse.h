/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#define IDENTIFIER 258
#define TYPENAME 259
#define SCSPEC 260
#define TYPESPEC 261
#define TYPE_QUAL 262
#define CONSTANT 263
#define STRING 264
#define ELLIPSIS 265
#define SIZEOF 266
#define ENUM 267
#define STRUCT 268
#define UNION 269
#define IF 270
#define ELSE 271
#define WHILE 272
#define DO 273
#define FOR 274
#define SWITCH 275
#define CASE 276
#define DEFAULT 277
#define BREAK 278
#define CONTINUE 279
#define RETURN 280
#define GOTO 281
#define ASM_KEYWORD 282
#define TYPEOF 283
#define ALIGNOF 284
#define ATTRIBUTE 285
#define EXTENSION 286
#define LABEL 287
#define REALPART 288
#define IMAGPART 289
#define ASSIGN 290
#define OROR 291
#define ANDAND 292
#define EQCOMPARE 293
#define ARITHCOMPARE 294
#define RSHIFT 295
#define LSHIFT 296
#define MINUSMINUS 297
#define PLUSPLUS 298
#define UNARY 299
#define HYPERUNARY 300
#define POINTSAT 301
#define INTERFACE 302
#define IMPLEMENTATION 303
#define END 304
#define SELECTOR 305
#define DEFS 306
#define ENCODE 307
#define CLASSNAME 308
#define PUBLIC 309
#define PRIVATE 310
#define PROTECTED 311
#define PROTOCOL 312
#define OBJECTNAME 313
#define CLASS 314
#define ALIAS 315
#define OBJC_STRING 316




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 87 "c-parse.y"
{long itype; tree ttype; enum tree_code code;
	char *filename; int lineno; int ends_in_label; }
/* Line 1489 of yacc.c.  */
#line 174 "c-parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

