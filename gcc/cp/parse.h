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
#define SELFNAME 260
#define PFUNCNAME 261
#define SCSPEC 262
#define TYPESPEC 263
#define CV_QUALIFIER 264
#define CONSTANT 265
#define STRING 266
#define ELLIPSIS 267
#define SIZEOF 268
#define ENUM 269
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
#define SIGOF 285
#define ATTRIBUTE 286
#define EXTENSION 287
#define LABEL 288
#define REALPART 289
#define IMAGPART 290
#define AGGR 291
#define VISSPEC 292
#define DELETE 293
#define NEW 294
#define THIS 295
#define OPERATOR 296
#define CXX_TRUE 297
#define CXX_FALSE 298
#define NAMESPACE 299
#define TYPENAME_KEYWORD 300
#define USING 301
#define LEFT_RIGHT 302
#define TEMPLATE 303
#define TYPEID 304
#define DYNAMIC_CAST 305
#define STATIC_CAST 306
#define REINTERPRET_CAST 307
#define CONST_CAST 308
#define SCOPE 309
#define EMPTY 310
#define NSNAME 311
#define PTYPENAME 312
#define THROW 313
#define ASSIGN 314
#define OROR 315
#define ANDAND 316
#define MIN_MAX 317
#define EQCOMPARE 318
#define ARITHCOMPARE 319
#define RSHIFT 320
#define LSHIFT 321
#define DOT_STAR 322
#define POINTSAT_STAR 323
#define MINUSMINUS 324
#define PLUSPLUS 325
#define UNARY 326
#define HYPERUNARY 327
#define PAREN_STAR_PAREN 328
#define POINTSAT 329
#define CATCH 330
#define TRY 331
#define PRE_PARSED_FUNCTION_DECL 332
#define EXTERN_LANG_STRING 333
#define ALL 334
#define PRE_PARSED_CLASS_DECL 335
#define DEFARG 336
#define DEFARG_MARKER 337
#define TYPENAME_DEFN 338
#define IDENTIFIER_DEFN 339
#define PTYPENAME_DEFN 340
#define END_OF_LINE 341
#define END_OF_SAVED_INPUT 342




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 92 "../../../gbagnu/gcc/cp/parse.y"
{long itype; tree ttype; char *strtype; enum tree_code code; flagged_type_tree ftype; }
/* Line 1489 of yacc.c.  */
#line 225 "parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#define YYEMPTY		(-2)
