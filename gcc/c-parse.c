/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     TYPENAME = 259,
     SCSPEC = 260,
     TYPESPEC = 261,
     TYPE_QUAL = 262,
     CONSTANT = 263,
     STRING = 264,
     ELLIPSIS = 265,
     SIZEOF = 266,
     ENUM = 267,
     STRUCT = 268,
     UNION = 269,
     IF = 270,
     ELSE = 271,
     WHILE = 272,
     DO = 273,
     FOR = 274,
     SWITCH = 275,
     CASE = 276,
     DEFAULT = 277,
     BREAK = 278,
     CONTINUE = 279,
     RETURN = 280,
     GOTO = 281,
     ASM_KEYWORD = 282,
     TYPEOF = 283,
     ALIGNOF = 284,
     ATTRIBUTE = 285,
     EXTENSION = 286,
     LABEL = 287,
     REALPART = 288,
     IMAGPART = 289,
     ASSIGN = 290,
     OROR = 291,
     ANDAND = 292,
     EQCOMPARE = 293,
     ARITHCOMPARE = 294,
     RSHIFT = 295,
     LSHIFT = 296,
     MINUSMINUS = 297,
     PLUSPLUS = 298,
     UNARY = 299,
     HYPERUNARY = 300,
     POINTSAT = 301,
     INTERFACE = 302,
     IMPLEMENTATION = 303,
     END = 304,
     SELECTOR = 305,
     DEFS = 306,
     ENCODE = 307,
     CLASSNAME = 308,
     PUBLIC = 309,
     PRIVATE = 310,
     PROTECTED = 311,
     PROTOCOL = 312,
     OBJECTNAME = 313,
     CLASS = 314,
     ALIAS = 315,
     OBJC_STRING = 316
   };
#endif
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




/* Copy the first part of user declarations.  */


#include "config.h"
#include "system.h"
#include <setjmp.h>

#include "tree.h"
#include "input.h"
#include "c-lex.h"
#include "c-tree.h"
#include "flags.h"
#include "output.h"
#include "toplev.h"


/* Since parsers are distinct for each language, put the language string
   definition here.  */
char *language_string = "GNU C";

/* Like YYERROR but do call yyerror.  */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG 1


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{long itype; tree ttype; enum tree_code code;
	char *filename; int lineno; int ends_in_label; }
/* Line 187 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Number of statements (loosely speaking) and compound statements 
   seen so far.  */
static int stmt_count;
static int compstmt_count;
  
/* Input file and line number of the end of the body of last simple_if;
   used by the stmt-rule immediately after simple_if returns.  */
static char *if_stmt_file;
static int if_stmt_line;

/* List of types and structure classes of the current declaration.  */
static tree current_declspecs = NULL_TREE;
static tree prefix_attributes = NULL_TREE;

/* Stack of saved values of current_declspecs and prefix_attributes.  */
static tree declspec_stack;

/* 1 if we explained undeclared var errors.  */
static int undeclared_variable_notice;


/* Tell yyparse how to print a token's value, if yydebug is set.  */

#define YYPRINT(FILE,YYCHAR,YYLVAL) yyprint(FILE,YYCHAR,YYLVAL)
extern void yyprint			(FILE *, int, YYSTYPE);


/* Line 216 of yacc.c.  */


#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
#define YY_(msgid) msgid
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2427

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  158
/* YYNRULES -- Number of rules.  */
#define YYNRULES  404
/* YYNRULES -- Number of states.  */
#define YYNSTATES  689

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   316

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,     2,     2,    52,    43,     2,
      58,    76,    50,    48,    81,    49,    57,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    77,
       2,    35,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    83,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    82,    41,    78,    79,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      36,    39,    40,    44,    45,    46,    47,    53,    54,    55,
      56,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     7,    10,    11,    15,    17,
      19,    25,    28,    32,    37,    42,    45,    48,    51,    54,
      56,    57,    58,    66,    71,    72,    73,    81,    86,    87,
      88,    95,    99,   101,   103,   105,   107,   109,   111,   113,
     115,   117,   119,   120,   122,   124,   128,   130,   133,   136,
     139,   142,   145,   150,   153,   158,   161,   164,   166,   168,
     170,   175,   176,   184,   186,   190,   194,   198,   202,   206,
     210,   214,   218,   222,   226,   230,   234,   235,   240,   241,
     246,   247,   248,   256,   257,   263,   267,   271,   273,   275,
     277,   281,   285,   286,   291,   296,   301,   305,   309,   312,
     315,   317,   320,   321,   323,   326,   330,   332,   334,   337,
     340,   345,   350,   353,   356,   360,   362,   364,   367,   370,
     371,   372,   377,   382,   386,   390,   393,   396,   399,   402,
     406,   407,   410,   413,   416,   419,   423,   424,   427,   430,
     432,   434,   437,   440,   442,   444,   447,   450,   453,   457,
     458,   461,   463,   465,   467,   472,   477,   479,   481,   483,
     485,   489,   491,   495,   496,   501,   502,   509,   513,   514,
     521,   525,   526,   528,   530,   533,   540,   542,   546,   547,
     549,   554,   561,   566,   568,   570,   572,   574,   576,   577,
     582,   584,   585,   588,   590,   594,   598,   601,   602,   607,
     609,   610,   615,   617,   619,   621,   624,   627,   633,   637,
     638,   639,   645,   646,   647,   653,   655,   657,   661,   665,
     670,   674,   678,   682,   684,   688,   693,   698,   702,   706,
     710,   712,   716,   720,   724,   729,   734,   738,   742,   744,
     746,   749,   751,   754,   756,   759,   760,   768,   774,   777,
     778,   786,   792,   795,   796,   805,   806,   814,   817,   818,
     820,   821,   823,   825,   828,   829,   833,   836,   840,   842,
     846,   848,   850,   853,   855,   859,   864,   871,   877,   879,
     883,   885,   887,   891,   894,   897,   898,   900,   902,   905,
     906,   909,   913,   917,   920,   924,   929,   933,   936,   940,
     943,   945,   947,   950,   953,   954,   956,   959,   960,   961,
     963,   965,   968,   972,   974,   977,   979,   982,   989,   995,
    1001,  1004,  1007,  1012,  1013,  1018,  1019,  1020,  1024,  1029,
    1033,  1035,  1037,  1039,  1041,  1044,  1045,  1050,  1052,  1056,
    1057,  1058,  1066,  1072,  1075,  1076,  1077,  1078,  1091,  1092,
    1099,  1102,  1105,  1108,  1112,  1119,  1128,  1139,  1152,  1156,
    1161,  1163,  1165,  1166,  1173,  1177,  1183,  1186,  1190,  1191,
    1193,  1194,  1196,  1197,  1199,  1201,  1205,  1210,  1212,  1216,
    1217,  1220,  1223,  1224,  1229,  1232,  1233,  1235,  1237,  1241,
    1243,  1247,  1252,  1257,  1262,  1267,  1272,  1273,  1276,  1278,
    1281,  1283,  1287,  1289,  1293
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      85,     0,    -1,    -1,    86,    -1,    -1,    87,    89,    -1,
      -1,    86,    88,    89,    -1,    91,    -1,    90,    -1,    27,
      58,   100,    76,    77,    -1,   241,    89,    -1,   123,   137,
      77,    -1,   130,   123,   137,    77,    -1,   126,   123,   136,
      77,    -1,   130,    77,    -1,   126,    77,    -1,     1,    77,
      -1,     1,    78,    -1,    77,    -1,    -1,    -1,   126,   123,
     165,    92,   117,    93,   199,    -1,   126,   123,   165,     1,
      -1,    -1,    -1,   130,   123,   168,    94,   117,    95,   199,
      -1,   130,   123,   168,     1,    -1,    -1,    -1,   123,   168,
      96,   117,    97,   199,    -1,   123,   168,     1,    -1,     3,
      -1,     4,    -1,    43,    -1,    49,    -1,    48,    -1,    54,
      -1,    53,    -1,    79,    -1,    80,    -1,   102,    -1,    -1,
     102,    -1,   108,    -1,   102,    81,   108,    -1,   114,    -1,
      50,   106,    -1,   241,   106,    -1,    99,   106,    -1,    40,
      98,    -1,   104,   103,    -1,   104,    58,   186,    76,    -1,
     105,   103,    -1,   105,    58,   186,    76,    -1,    33,   106,
      -1,    34,   106,    -1,    11,    -1,    29,    -1,   103,    -1,
      58,   186,    76,   106,    -1,    -1,    58,   186,    76,    82,
     107,   151,    78,    -1,   106,    -1,   108,    48,   108,    -1,
     108,    49,   108,    -1,   108,    50,   108,    -1,   108,    51,
     108,    -1,   108,    52,   108,    -1,   108,    47,   108,    -1,
     108,    46,   108,    -1,   108,    45,   108,    -1,   108,    44,
     108,    -1,   108,    43,   108,    -1,   108,    41,   108,    -1,
     108,    42,   108,    -1,    -1,   108,    40,   109,   108,    -1,
      -1,   108,    39,   110,   108,    -1,    -1,    -1,   108,    37,
     111,   100,    38,   112,   108,    -1,    -1,   108,    37,   113,
      38,   108,    -1,   108,    35,   108,    -1,   108,    36,   108,
      -1,     3,    -1,     8,    -1,   116,    -1,    58,   100,    76,
      -1,    58,     1,    76,    -1,    -1,    58,   115,   201,    76,
      -1,   114,    58,   101,    76,    -1,   114,    59,   100,    83,
      -1,   114,    57,    98,    -1,   114,    60,    98,    -1,   114,
      54,    -1,   114,    53,    -1,     9,    -1,   116,     9,    -1,
      -1,   119,    -1,   119,    10,    -1,   206,   207,   120,    -1,
     118,    -1,   194,    -1,   119,   118,    -1,   118,   194,    -1,
     128,   123,   136,    77,    -1,   131,   123,   137,    77,    -1,
     128,    77,    -1,   131,    77,    -1,   206,   207,   125,    -1,
     121,    -1,   194,    -1,   122,   121,    -1,   121,   194,    -1,
      -1,    -1,   126,   123,   136,    77,    -1,   130,   123,   137,
      77,    -1,   126,   123,   159,    -1,   130,   123,   162,    -1,
     126,    77,    -1,   130,    77,    -1,   241,   125,    -1,   134,
     127,    -1,   130,   134,   127,    -1,    -1,   127,   135,    -1,
     127,     5,    -1,   127,   144,    -1,   134,   129,    -1,   131,
     134,   129,    -1,    -1,   129,   135,    -1,   129,     5,    -1,
     131,    -1,   144,    -1,   130,   131,    -1,   130,   144,    -1,
       7,    -1,     5,    -1,   131,     7,    -1,   131,     5,    -1,
     134,   133,    -1,   188,   134,   133,    -1,    -1,   133,   135,
      -1,     6,    -1,   172,    -1,     4,    -1,    28,    58,   100,
      76,    -1,    28,    58,   186,    76,    -1,     6,    -1,     7,
      -1,   172,    -1,   139,    -1,   136,    81,   139,    -1,   141,
      -1,   137,    81,   139,    -1,    -1,    27,    58,   116,    76,
      -1,    -1,   165,   138,   143,    35,   140,   149,    -1,   165,
     138,   143,    -1,    -1,   168,   138,   143,    35,   142,   149,
      -1,   168,   138,   143,    -1,    -1,   144,    -1,   145,    -1,
     144,   145,    -1,    30,    58,    58,   146,    76,    76,    -1,
     147,    -1,   146,    81,   147,    -1,    -1,   148,    -1,   148,
      58,     3,    76,    -1,   148,    58,     3,    81,   102,    76,
      -1,   148,    58,   101,    76,    -1,    98,    -1,     5,    -1,
       6,    -1,     7,    -1,   108,    -1,    -1,    82,   150,   151,
      78,    -1,     1,    -1,    -1,   152,   177,    -1,   153,    -1,
     152,    81,   153,    -1,   157,    35,   155,    -1,   158,   155,
      -1,    -1,    98,    38,   154,   155,    -1,   155,    -1,    -1,
      82,   156,   151,    78,    -1,   108,    -1,     1,    -1,   158,
      -1,   157,   158,    -1,    57,    98,    -1,    59,   108,    10,
     108,    83,    -1,    59,   108,    83,    -1,    -1,    -1,   165,
     160,   117,   161,   201,    -1,    -1,    -1,   168,   163,   117,
     164,   201,    -1,   166,    -1,   168,    -1,    58,   166,    76,
      -1,   166,    58,   236,    -1,   166,    59,   100,    83,    -1,
     166,    59,    83,    -1,    50,   189,   166,    -1,   144,   124,
     166,    -1,     4,    -1,   167,    58,   236,    -1,   167,    59,
      50,    83,    -1,   167,    59,   100,    83,    -1,   167,    59,
      83,    -1,    50,   189,   167,    -1,   144,   124,   167,    -1,
       4,    -1,   168,    58,   236,    -1,    58,   168,    76,    -1,
      50,   189,   168,    -1,   168,    59,    50,    83,    -1,   168,
      59,   100,    83,    -1,   168,    59,    83,    -1,   144,   124,
     168,    -1,     3,    -1,    13,    -1,    13,   144,    -1,    14,
      -1,    14,   144,    -1,    12,    -1,    12,   144,    -1,    -1,
     169,    98,    82,   173,   179,    78,   143,    -1,   169,    82,
     179,    78,   143,    -1,   169,    98,    -1,    -1,   170,    98,
      82,   174,   179,    78,   143,    -1,   170,    82,   179,    78,
     143,    -1,   170,    98,    -1,    -1,   171,    98,    82,   175,
     184,   178,    78,   143,    -1,    -1,   171,    82,   176,   184,
     178,    78,   143,    -1,   171,    98,    -1,    -1,    81,    -1,
      -1,    81,    -1,   180,    -1,   180,   181,    -1,    -1,   180,
     181,    77,    -1,   180,    77,    -1,   132,   123,   182,    -1,
     132,    -1,   188,   123,   182,    -1,   188,    -1,     1,    -1,
     241,   181,    -1,   183,    -1,   182,    81,   183,    -1,   206,
     207,   165,   143,    -1,   206,   207,   165,    38,   108,   143,
      -1,   206,   207,    38,   108,   143,    -1,   185,    -1,   184,
      81,   185,    -1,     1,    -1,    98,    -1,    98,    35,   108,
      -1,   132,   187,    -1,   188,   187,    -1,    -1,   190,    -1,
       7,    -1,   188,     7,    -1,    -1,   189,     7,    -1,    58,
     190,    76,    -1,    50,   189,   190,    -1,    50,   189,    -1,
     190,    58,   229,    -1,   190,    59,   100,    83,    -1,   190,
      59,    83,    -1,    58,   229,    -1,    59,   100,    83,    -1,
      59,    83,    -1,   192,    -1,   209,    -1,   192,   209,    -1,
     192,   194,    -1,    -1,   191,    -1,     1,    77,    -1,    -1,
      -1,   197,    -1,   198,    -1,   197,   198,    -1,    32,   240,
      77,    -1,   201,    -1,     1,   201,    -1,    82,    -1,   200,
      78,    -1,   200,   195,   196,   122,   193,    78,    -1,   200,
     195,   196,     1,    78,    -1,   200,   195,   196,   191,    78,
      -1,   203,   208,    -1,   203,     1,    -1,    15,    58,   100,
      76,    -1,    -1,    18,   205,   208,    17,    -1,    -1,    -1,
     206,   207,   211,    -1,   206,   207,   222,   208,    -1,   206,
     207,   210,    -1,   211,    -1,   222,    -1,   201,    -1,   219,
      -1,   100,    77,    -1,    -1,   202,    16,   212,   208,    -1,
     202,    -1,   202,    16,     1,    -1,    -1,    -1,    17,   213,
      58,   100,    76,   214,   208,    -1,   204,    58,   100,    76,
      77,    -1,   204,     1,    -1,    -1,    -1,    -1,    19,    58,
     224,    77,   215,   224,    77,   216,   224,    76,   217,   208,
      -1,    -1,    20,    58,   100,    76,   218,   208,    -1,    23,
      77,    -1,    24,    77,    -1,    25,    77,    -1,    25,   100,
      77,    -1,    27,   223,    58,   100,    76,    77,    -1,    27,
     223,    58,   100,    38,   225,    76,    77,    -1,    27,   223,
      58,   100,    38,   225,    38,   225,    76,    77,    -1,    27,
     223,    58,   100,    38,   225,    38,   225,    38,   228,    76,
      77,    -1,    26,    98,    77,    -1,    26,    50,   100,    77,
      -1,    77,    -1,   220,    -1,    -1,    19,    58,   114,    76,
     221,   208,    -1,    21,   108,    38,    -1,    21,   108,    10,
     108,    38,    -1,    22,    38,    -1,    98,    38,   143,    -1,
      -1,     7,    -1,    -1,   100,    -1,    -1,   226,    -1,   227,
      -1,   226,    81,   227,    -1,     9,    58,   100,    76,    -1,
     116,    -1,   228,    81,   116,    -1,    -1,   230,   231,    -1,
     233,    76,    -1,    -1,   234,    77,   232,   231,    -1,     1,
      76,    -1,    -1,    10,    -1,   234,    -1,   234,    81,    10,
      -1,   235,    -1,   234,    81,   235,    -1,   126,   123,   167,
     143,    -1,   126,   123,   168,   143,    -1,   126,   123,   187,
     143,    -1,   130,   123,   168,   143,    -1,   130,   123,   187,
     143,    -1,    -1,   237,   238,    -1,   231,    -1,   239,    76,
      -1,     3,    -1,   239,    81,     3,    -1,    98,    -1,   240,
      81,    98,    -1,    31,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   234,   234,   238,   253,   253,   254,   254,   258,   259,
     260,   268,   273,   283,   288,   293,   295,   297,   298,   299,
     306,   311,   305,   318,   324,   329,   323,   336,   342,   347,
     341,   354,   362,   363,   366,   368,   370,   372,   374,   376,
     378,   382,   388,   389,   393,   395,   400,   401,   404,   407,
     411,   439,   445,   448,   451,   454,   456,   461,   465,   469,
     470,   474,   473,   505,   506,   508,   510,   512,   514,   516,
     518,   520,   522,   524,   526,   528,   531,   530,   537,   536,
     543,   546,   542,   552,   551,   561,   564,   571,   669,   670,
     672,   678,   681,   680,   717,   719,   721,   725,   731,   733,
     739,   740,   745,   747,   748,   759,   764,   765,   766,   767,
     775,   780,   785,   788,   797,   802,   803,   804,   805,   813,
     824,   828,   833,   838,   843,   848,   850,   852,   862,   864,
     869,   870,   872,   877,   882,   884,   890,   891,   893,   906,
     908,   910,   912,   917,   920,   922,   925,   939,   941,   946,
     947,   955,   956,   957,   961,   963,   969,   970,   971,   975,
     976,   980,   981,   986,   987,   995,   994,  1002,  1011,  1010,
    1019,  1028,  1029,  1034,  1036,  1041,  1046,  1048,  1054,  1055,
    1057,  1059,  1061,  1069,  1070,  1071,  1072,  1078,  1080,  1079,
    1092,  1099,  1101,  1105,  1106,  1112,  1113,  1115,  1114,  1117,
    1122,  1121,  1125,  1127,  1131,  1132,  1136,  1141,  1143,  1149,
    1158,  1148,  1172,  1181,  1171,  1197,  1198,  1204,  1206,  1211,
    1213,  1215,  1222,  1224,  1233,  1238,  1243,  1245,  1247,  1254,
    1256,  1263,  1268,  1270,  1272,  1277,  1279,  1286,  1288,  1292,
    1294,  1299,  1301,  1306,  1308,  1314,  1313,  1319,  1323,  1326,
    1325,  1329,  1333,  1336,  1335,  1342,  1341,  1347,  1351,  1353,
    1356,  1358,  1364,  1366,  1372,  1373,  1375,  1390,  1396,  1401,
    1407,  1412,  1414,  1420,  1421,  1426,  1429,  1433,  1444,  1445,
    1450,  1456,  1458,  1463,  1465,  1471,  1472,  1476,  1478,  1484,
    1485,  1490,  1493,  1495,  1497,  1499,  1501,  1503,  1505,  1507,
    1518,  1526,  1527,  1529,  1533,  1535,  1538,  1542,  1552,  1554,
    1560,  1561,  1565,  1579,  1581,  1584,  1586,  1588,  1596,  1604,
    1616,  1620,  1624,  1639,  1638,  1651,  1655,  1659,  1664,  1669,
    1674,  1676,  1682,  1684,  1685,  1703,  1702,  1710,  1722,  1725,
    1735,  1724,  1745,  1753,  1758,  1770,  1773,  1756,  1800,  1799,
    1813,  1818,  1823,  1827,  1831,  1842,  1849,  1856,  1863,  1874,
    1880,  1884,  1890,  1889,  1945,  1976,  2007,  2022,  2038,  2040,
    2046,  2047,  2053,  2054,  2058,  2059,  2064,  2069,  2071,  2078,
    2078,  2088,  2090,  2089,  2099,  2106,  2107,  2117,  2119,  2124,
    2126,  2133,  2142,  2151,  2160,  2170,  2185,  2185,  2195,  2196,
    2206,  2208,  2214,  2216,  2221
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "TYPENAME", "SCSPEC",
  "TYPESPEC", "TYPE_QUAL", "CONSTANT", "STRING", "ELLIPSIS", "SIZEOF",
  "ENUM", "STRUCT", "UNION", "IF", "ELSE", "WHILE", "DO", "FOR", "SWITCH",
  "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN", "GOTO", "ASM_KEYWORD",
  "TYPEOF", "ALIGNOF", "ATTRIBUTE", "EXTENSION", "LABEL", "REALPART",
  "IMAGPART", "'='", "ASSIGN", "'?'", "':'", "OROR", "ANDAND", "'|'",
  "'^'", "'&'", "EQCOMPARE", "ARITHCOMPARE", "RSHIFT", "LSHIFT", "'+'",
  "'-'", "'*'", "'/'", "'%'", "MINUSMINUS", "PLUSPLUS", "UNARY",
  "HYPERUNARY", "'.'", "'('", "'['", "POINTSAT", "INTERFACE",
  "IMPLEMENTATION", "END", "SELECTOR", "DEFS", "ENCODE", "CLASSNAME",
  "PUBLIC", "PRIVATE", "PROTECTED", "PROTOCOL", "OBJECTNAME", "CLASS",
  "ALIAS", "OBJC_STRING", "')'", "';'", "'}'", "'~'", "'!'", "','", "'{'",
  "']'", "$accept", "program", "extdefs", "@1", "@2", "extdef", "datadef",
  "fndef", "@3", "@4", "@5", "@6", "@7", "@8", "identifier", "unop",
  "expr", "exprlist", "nonnull_exprlist", "unary_expr", "sizeof",
  "alignof", "cast_expr", "@9", "expr_no_commas", "@10", "@11", "@12",
  "@13", "@14", "primary", "@15", "string", "old_style_parm_decls",
  "lineno_datadecl", "datadecls", "datadecl", "lineno_decl", "decls",
  "setspecs", "setattrs", "decl", "typed_declspecs", "reserved_declspecs",
  "typed_declspecs_no_prefix_attr", "reserved_declspecs_no_prefix_attr",
  "declmods", "declmods_no_prefix_attr", "typed_typespecs",
  "reserved_typespecquals", "typespec", "typespecqual_reserved",
  "initdecls", "notype_initdecls", "maybeasm", "initdcl", "@16",
  "notype_initdcl", "@17", "maybe_attribute", "attributes", "attribute",
  "attribute_list", "attrib", "any_word", "init", "@18",
  "initlist_maybe_comma", "initlist1", "initelt", "@19", "initval", "@20",
  "designator_list", "designator", "nested_function", "@21", "@22",
  "notype_nested_function", "@23", "@24", "declarator",
  "after_type_declarator", "parm_declarator", "notype_declarator",
  "struct_head", "union_head", "enum_head", "structsp", "@25", "@26",
  "@27", "@28", "maybecomma", "maybecomma_warn", "component_decl_list",
  "component_decl_list2", "component_decl", "components",
  "component_declarator", "enumlist", "enumerator", "typename", "absdcl",
  "nonempty_type_quals", "type_quals", "absdcl1", "stmts",
  "lineno_stmt_or_labels", "xstmts", "errstmt", "pushlevel",
  "maybe_label_decls", "label_decls", "label_decl", "compstmt_or_error",
  "compstmt_start", "compstmt", "simple_if", "if_prefix", "do_stmt_start",
  "@29", "save_filename", "save_lineno", "lineno_labeled_stmt",
  "lineno_stmt_or_label", "stmt_or_label", "stmt", "@30", "@31", "@32",
  "@33", "@34", "@35", "@36", "all_iter_stmt", "all_iter_stmt_simple",
  "@37", "label", "maybe_type_qual", "xexpr", "asm_operands",
  "nonnull_asm_operands", "asm_operand", "asm_clobbers", "parmlist", "@38",
  "parmlist_1", "@39", "parmlist_2", "parms", "parm",
  "parmlist_or_identifiers", "@40", "parmlist_or_identifiers_1",
  "identifiers", "identifiers_or_typenames", "extension", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    61,   290,    63,    58,   291,
     292,   124,    94,    38,   293,   294,   295,   296,    43,    45,
      42,    47,    37,   297,   298,   299,   300,    46,    40,    91,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,    41,    59,   125,   126,
      33,    44,   123,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    84,    85,    85,    87,    86,    88,    86,    89,    89,
      89,    89,    90,    90,    90,    90,    90,    90,    90,    90,
      92,    93,    91,    91,    94,    95,    91,    91,    96,    97,
      91,    91,    98,    98,    99,    99,    99,    99,    99,    99,
      99,   100,   101,   101,   102,   102,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   104,   105,   106,
     106,   107,   106,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   108,   110,   108,
     111,   112,   108,   113,   108,   108,   108,   114,   114,   114,
     114,   114,   115,   114,   114,   114,   114,   114,   114,   114,
     116,   116,   117,   117,   117,   118,   119,   119,   119,   119,
     120,   120,   120,   120,   121,   122,   122,   122,   122,   123,
     124,   125,   125,   125,   125,   125,   125,   125,   126,   126,
     127,   127,   127,   127,   128,   128,   129,   129,   129,   130,
     130,   130,   130,   131,   131,   131,   131,   132,   132,   133,
     133,   134,   134,   134,   134,   134,   135,   135,   135,   136,
     136,   137,   137,   138,   138,   140,   139,   139,   142,   141,
     141,   143,   143,   144,   144,   145,   146,   146,   147,   147,
     147,   147,   147,   148,   148,   148,   148,   149,   150,   149,
     149,   151,   151,   152,   152,   153,   153,   154,   153,   153,
     156,   155,   155,   155,   157,   157,   158,   158,   158,   160,
     161,   159,   163,   164,   162,   165,   165,   166,   166,   166,
     166,   166,   166,   166,   167,   167,   167,   167,   167,   167,
     167,   168,   168,   168,   168,   168,   168,   168,   168,   169,
     169,   170,   170,   171,   171,   173,   172,   172,   172,   174,
     172,   172,   172,   175,   172,   176,   172,   172,   177,   177,
     178,   178,   179,   179,   180,   180,   180,   181,   181,   181,
     181,   181,   181,   182,   182,   183,   183,   183,   184,   184,
     184,   185,   185,   186,   186,   187,   187,   188,   188,   189,
     189,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     191,   192,   192,   192,   193,   193,   194,   195,   196,   196,
     197,   197,   198,   199,   199,   200,   201,   201,   201,   201,
     202,   202,   203,   205,   204,   206,   207,   208,   208,   209,
     210,   210,   211,   211,   211,   212,   211,   211,   211,   213,
     214,   211,   211,   211,   215,   216,   217,   211,   218,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   219,   221,   220,   222,   222,   222,   222,   223,   223,
     224,   224,   225,   225,   226,   226,   227,   228,   228,   230,
     229,   231,   232,   231,   231,   233,   233,   233,   233,   234,
     234,   235,   235,   235,   235,   235,   237,   236,   238,   238,
     239,   239,   240,   240,   241
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     0,     2,     0,     3,     1,     1,
       5,     2,     3,     4,     4,     2,     2,     2,     2,     1,
       0,     0,     7,     4,     0,     0,     7,     4,     0,     0,
       6,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     3,     1,     2,     2,     2,
       2,     2,     4,     2,     4,     2,     2,     1,     1,     1,
       4,     0,     7,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     0,     4,     0,     4,
       0,     0,     7,     0,     5,     3,     3,     1,     1,     1,
       3,     3,     0,     4,     4,     4,     3,     3,     2,     2,
       1,     2,     0,     1,     2,     3,     1,     1,     2,     2,
       4,     4,     2,     2,     3,     1,     1,     2,     2,     0,
       0,     4,     4,     3,     3,     2,     2,     2,     2,     3,
       0,     2,     2,     2,     2,     3,     0,     2,     2,     1,
       1,     2,     2,     1,     1,     2,     2,     2,     3,     0,
       2,     1,     1,     1,     4,     4,     1,     1,     1,     1,
       3,     1,     3,     0,     4,     0,     6,     3,     0,     6,
       3,     0,     1,     1,     2,     6,     1,     3,     0,     1,
       4,     6,     4,     1,     1,     1,     1,     1,     0,     4,
       1,     0,     2,     1,     3,     3,     2,     0,     4,     1,
       0,     4,     1,     1,     1,     2,     2,     5,     3,     0,
       0,     5,     0,     0,     5,     1,     1,     3,     3,     4,
       3,     3,     3,     1,     3,     4,     4,     3,     3,     3,
       1,     3,     3,     3,     4,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     2,     0,     7,     5,     2,     0,
       7,     5,     2,     0,     8,     0,     7,     2,     0,     1,
       0,     1,     1,     2,     0,     3,     2,     3,     1,     3,
       1,     1,     2,     1,     3,     4,     6,     5,     1,     3,
       1,     1,     3,     2,     2,     0,     1,     1,     2,     0,
       2,     3,     3,     2,     3,     4,     3,     2,     3,     2,
       1,     1,     2,     2,     0,     1,     2,     0,     0,     1,
       1,     2,     3,     1,     2,     1,     2,     6,     5,     5,
       2,     2,     4,     0,     4,     0,     0,     3,     4,     3,
       1,     1,     1,     1,     2,     0,     4,     1,     3,     0,
       0,     7,     5,     2,     0,     0,     0,    12,     0,     6,
       2,     2,     2,     3,     6,     8,    10,    12,     3,     4,
       1,     1,     0,     6,     3,     5,     2,     3,     0,     1,
       0,     1,     0,     1,     1,     3,     4,     1,     3,     0,
       2,     2,     0,     4,     2,     0,     1,     1,     3,     1,
       3,     4,     4,     4,     4,     4,     0,     2,     1,     2,
       1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     6,     0,     1,     0,     0,   153,   144,   151,
     143,   243,   239,   241,     0,     0,     0,   404,    19,     5,
       9,     8,     0,   119,   119,   139,   130,   140,   173,     0,
       0,     0,   152,     0,     7,    17,    18,   244,   240,   242,
       0,     0,     0,   238,   289,     0,     0,   161,   120,     0,
      16,     0,    15,     0,   141,   130,   142,   146,   145,   128,
     174,    32,    33,   264,   248,   264,   252,   255,   257,    11,
      87,    88,   100,    57,    58,     0,     0,     0,    34,    36,
      35,     0,    38,    37,     0,    39,    40,     0,     0,    41,
      59,     0,     0,    63,    44,    46,    89,     0,   287,     0,
     285,   149,     0,   285,   178,     0,     0,    12,     0,     0,
      31,     0,   396,     0,     0,   171,   223,   289,     0,     0,
     159,   120,     0,   215,   216,     0,     0,   129,   132,   156,
     157,   131,   133,   158,     0,     0,   245,     0,   249,     0,
     253,    55,    56,    50,    47,     0,     0,     0,     0,    49,
       0,     0,     0,    51,     0,    53,     0,     0,    80,    78,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,    98,     0,    42,     0,     0,   101,
      48,   154,   289,   379,     0,   283,   286,   147,   155,   288,
     149,   284,   184,   185,   186,   183,     0,   176,   179,   290,
     233,   232,   162,   163,   237,     0,   231,     0,     0,   236,
       0,     0,    29,     0,   325,   107,   326,   170,   172,     0,
       0,    14,     0,     0,    23,     0,   171,   396,     0,    13,
      27,     0,   171,   271,   266,   119,   263,   119,     0,   264,
     171,   264,   280,   281,   260,   278,     0,    91,    90,   315,
     307,     0,     0,    10,    45,     0,     0,    85,    86,     0,
       0,     0,     0,    74,    75,    73,    72,    71,    70,    69,
      64,    65,    66,    67,    68,    96,     0,    43,     0,    97,
     293,     0,   297,     0,   299,     0,   379,     0,   150,   148,
       0,   178,    42,     0,     0,   400,   386,   119,   119,   398,
       0,   387,   389,   397,     0,   234,   235,   306,     0,   109,
     104,   108,     0,   168,   221,   217,   160,   222,    21,   167,
     218,   220,     0,    25,   247,   325,   265,   325,   272,     0,
     251,     0,     0,   261,     0,   260,   316,   308,    93,    61,
      60,    52,    54,     0,     0,    79,    77,    94,    95,   292,
     291,   380,   298,   294,   296,     0,   175,   177,    87,     0,
     164,   384,   285,   285,   381,   382,     0,   399,     0,     0,
      30,   313,   105,   119,   119,   136,     0,     0,   165,   219,
       0,   267,   273,   326,   269,   171,   171,   282,   279,   171,
       0,     0,     0,   309,   310,     0,    81,    84,   295,   180,
       0,   182,   230,   289,   379,   120,   171,   171,   171,   289,
     171,   171,     0,   388,   390,   401,   314,   112,     0,   113,
       0,   136,   134,   190,   188,   187,   169,    22,     0,    26,
     325,     0,   246,   250,   256,   171,   402,     0,     0,     0,
     325,     0,     0,   116,   326,   301,   311,   203,    87,     0,
       0,   200,     0,   202,     0,   258,   193,   199,     0,     0,
       0,     0,   293,     0,   396,     0,   391,   392,   393,   293,
     394,   395,   383,     0,     0,   163,   135,   138,   137,     0,
     166,   274,     0,   171,   254,   312,     0,   318,   118,   117,
     305,     0,   319,   303,   326,   302,     0,   206,     0,     0,
     197,    62,     0,   192,     0,   205,   196,    82,   181,   228,
     289,   229,   224,     0,   227,     0,   110,   111,     0,   171,
       0,   275,   403,   317,     0,   153,     0,   339,   323,     0,
       0,     0,     0,     0,     0,     0,     0,   368,   360,     0,
       0,   114,   119,   119,   332,   337,     0,     0,   329,   330,
     333,   361,   331,     0,     0,   208,     0,     0,   194,   195,
       0,   225,   226,   189,   277,   171,     0,     0,   325,   370,
       0,     0,   366,   350,   351,   352,     0,     0,     0,   369,
       0,   171,   334,   125,     0,   126,     0,     0,   321,   326,
     320,   343,     0,   127,     0,   201,   198,   276,     0,     0,
       0,   371,    46,     0,     0,     0,   364,   353,     0,   358,
       0,   367,     0,   123,   209,     0,   124,   212,   338,   325,
       0,     0,   207,   322,     0,   324,   362,   344,   348,     0,
     359,     0,   121,     0,   122,     0,   336,   327,   325,     0,
     340,   325,   370,   325,   365,   372,     0,   210,   213,   328,
     342,   325,   363,     0,   349,     0,     0,   373,   374,   354,
       0,     0,   341,   345,     0,   372,     0,     0,   211,   214,
     370,     0,     0,   355,   375,     0,   376,     0,     0,   346,
     377,     0,   356,   325,     0,     0,   347,   357,   378
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,     5,    19,    20,    21,   225,   377,
     231,   380,   114,   308,   452,    87,   146,   276,    89,    90,
      91,    92,    93,   395,    94,   262,   261,   259,   460,   260,
      95,   147,    96,   212,   213,   214,   372,   439,   440,    22,
     109,   541,   297,    59,   373,   422,   298,    25,   100,   187,
      26,   131,   119,    46,   115,   120,   428,    47,   376,   217,
     218,    28,   196,   197,   198,   426,   479,   454,   455,   456,
     557,   457,   499,   458,   459,   613,   633,   660,   616,   635,
     661,   203,   123,   509,   124,    29,    30,    31,    32,   239,
     241,   246,   139,   503,   334,   134,   135,   236,   381,   382,
     244,   245,   102,   185,   103,   105,   186,   441,   442,   491,
     215,   337,   392,   393,   394,   370,   250,   371,   545,   546,
     547,   568,   589,   312,   590,   445,   548,   549,   619,   567,
     651,   642,   670,   683,   643,   550,   551,   641,   552,   580,
     603,   656,   657,   658,   681,   282,   283,   299,   412,   300,
     301,   302,   206,   207,   303,   304,   437,    97
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -470
static const yytype_int16 yypact[] =
{
      72,    86,    93,  2287,  -470,  2287,   212,  -470,  -470,  -470,
    -470,   149,   149,   149,    67,   101,   135,  -470,  -470,  -470,
    -470,  -470,   429,   141,   209,   228,  -470,   149,  -470,    57,
      60,    66,  -470,  2287,  -470,  -470,  -470,   149,   149,   149,
    2113,  2047,   138,  -470,  -470,   429,    99,  -470,   149,  1376,
    -470,   378,  -470,   429,   228,  -470,   149,  -470,  -470,   695,
    -470,  -470,  -470,  -470,   143,  -470,   159,  -470,   170,  -470,
    -470,  -470,  -470,  -470,  -470,  2113,  2113,   298,  -470,  -470,
    -470,  2113,  -470,  -470,  1080,  -470,  -470,  2113,   182,   186,
    -470,  2165,  2198,  -470,  2375,  1369,   268,  2113,  -470,   203,
     153,  -470,   239,   575,   538,   459,   116,  -470,   378,   429,
    -470,   246,  -470,  1451,   771,   149,  -470,  -470,   378,   118,
    -470,   149,   328,   280,   387,   129,  1438,   695,  -470,  -470,
    -470,  -470,   149,  -470,   267,  1577,  -470,   272,  -470,   436,
    -470,  -470,  -470,  -470,  -470,   283,   290,   318,   302,  -470,
     303,  2113,  1080,  -470,  1080,  -470,  2113,  2113,   350,  -470,
    -470,  2113,  2113,  2113,  2113,  2113,  2113,  2113,  2113,  2113,
    2113,  2113,  2113,  -470,  -470,   298,  2113,  2113,   298,  -470,
    -470,  -470,  -470,   153,  1513,  -470,   405,   313,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,   110,  -470,   363,  -470,
     387,  -470,  -470,   399,   387,   420,  -470,  1628,  1566,  -470,
     351,   380,  -470,   468,    52,  -470,  -470,   403,   149,   178,
     217,  -470,   378,   378,  -470,   771,   149,  -470,  1619,  -470,
    -470,   771,   149,  -470,  -470,   437,   384,   340,  1736,  -470,
     149,  -470,  -470,   430,   390,  -470,   436,  -470,  -470,  -470,
     398,   392,  1991,  -470,  2375,   407,   412,  2375,  2375,  2113,
     452,  2113,  2113,  2280,   757,   888,  1280,  1161,   547,   547,
     364,   364,  -470,  -470,  -470,  -470,   417,   186,   416,  -470,
     119,   241,  -470,  1681,  -470,   418,  -470,  1672,  -470,   313,
     435,   538,  2231,    78,   440,  -470,  -470,  -470,  1144,  -470,
     451,   150,  -470,  -470,   162,  -470,  -470,  -470,    56,  -470,
    -470,  -470,  1345,  -470,   280,  -470,  -470,   280,  -470,   478,
    -470,  -470,   445,  -470,  -470,  -470,  -470,  -470,  -470,   460,
    -470,   470,  2113,   298,   471,   390,  -470,   486,  -470,  -470,
    -470,  -470,  -470,   487,  2113,  1963,  2136,  -470,  -470,   405,
    -470,  -470,  -470,  -470,  -470,   472,  -470,  -470,   168,   475,
    -470,  -470,   278,   361,  -470,  -470,   667,  -470,   550,   318,
    -470,  -470,  -470,   479,  1003,  -470,  1313,    56,  -470,  -470,
      56,   483,  -470,  -470,   483,   149,   149,  2375,  -470,   149,
     490,   298,   715,   486,  -470,  1136,  -470,  1751,  -470,  -470,
    2113,  -470,  -470,  -470,   361,   149,    49,    53,   149,  -470,
      53,   149,  1681,  -470,  -470,  -470,  -470,  -470,   378,  -470,
     429,  -470,   579,  -470,  -470,  2375,  -470,  -470,  1313,  -470,
    -470,   293,  -470,  -470,  -470,   149,  -470,   232,   443,   635,
     491,   493,   809,  -470,  -470,  -470,  -470,  -470,   535,   298,
    2113,  -470,   536,  2375,   497,   496,  -470,  -470,   167,  1254,
    2113,   193,   394,   447,  -470,  1725,  -470,  -470,  -470,   354,
    -470,  -470,  -470,   233,   296,    61,   579,  -470,  -470,  1136,
    -470,  -470,  2113,    37,  -470,  -470,   298,  -470,  -470,  -470,
    -470,   500,  -470,  -470,  -470,  -470,  1859,  -470,  2311,  1136,
    -470,  -470,  1195,  -470,  1367,  -470,  -470,  1751,  -470,   465,
    -470,   465,  -470,  1778,  -470,   507,  -470,  -470,   523,  2356,
    2113,  -470,  -470,  -470,  1939,   566,   573,  -470,  -470,   574,
     577,  2113,   570,   560,   590,  2080,    77,   644,  -470,   632,
     599,  -470,   605,   899,  -470,   670,   941,    65,  -470,  -470,
    -470,  -470,  -470,   867,  2113,  -470,   609,  1367,  -470,  -470,
     372,  -470,  -470,  -470,  -470,  2356,  2113,   633,  -470,  2113,
    2113,  1803,  -470,  -470,  -470,  -470,   613,  2113,   615,  -470,
     636,   149,  -470,  -470,   378,  -470,   429,  1024,  -470,  -470,
    -470,  -470,  2113,  -470,  2330,  -470,  -470,  -470,   620,  2113,
     681,  -470,   569,   622,   627,  2113,  -470,  -470,   628,  -470,
    2113,  -470,   306,  -470,   477,   326,  -470,  1041,  -470,  -470,
    1939,   630,  -470,  -470,   655,  -470,  -470,  -470,  -470,  1883,
    -470,    39,  -470,   771,  -470,   771,  -470,  -470,  -470,   673,
    -470,  -470,  2113,  -470,  -470,   738,   674,  -470,  -470,  -470,
    -470,  -470,  -470,   675,  -470,   646,    54,   672,  -470,  -470,
     318,   318,  -470,  -470,  2113,   738,   677,   738,  -470,  -470,
    2113,   680,    95,  -470,  -470,   683,  -470,   420,   684,  -470,
     268,   197,  -470,  -470,   685,   420,  -470,  -470,   268
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -470,  -470,  -470,  -470,  -470,   157,  -470,  -470,  -470,  -470,
    -470,  -470,  -470,  -470,   -26,  -470,   -40,   474,  -128,   442,
    -470,  -470,     9,  -470,   449,  -470,  -470,  -470,  -470,  -470,
     188,  -470,  -183,  -202,   546,  -470,  -470,   327,  -470,    -3,
    -102,   218,     4,   719,  -470,   349,     7,    -7,   -77,   589,
      18,  -154,  -377,   -51,  -106,   -56,  -470,  -470,  -470,  -123,
      23,    62,  -470,   489,  -470,   358,  -470,  -363,  -470,   285,
    -470,  -410,  -470,  -470,   324,  -470,  -470,  -470,  -470,  -470,
    -470,   -37,   -63,  -312,   -14,  -470,  -470,  -470,   -29,  -470,
    -470,  -470,  -470,  -470,   453,   -41,  -470,   551,   463,   366,
     545,   481,   -30,   -92,   -70,  -111,  -151,   371,  -470,  -470,
    -188,  -470,  -470,  -470,   422,  -237,  -470,  -129,  -470,  -470,
    -470,  -470,   -68,  -339,  -454,   356,  -470,   196,  -470,  -470,
    -470,  -470,  -470,  -470,  -470,  -470,  -470,  -470,   199,  -470,
    -469,   156,  -470,   155,  -470,   537,  -470,  -245,  -470,  -470,
    -470,   473,  -200,  -470,  -470,  -470,  -470,    10
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -386
static const yytype_int16 yytable[] =
{
      88,    99,   125,    64,    66,    68,   219,    23,    49,    23,
      24,   191,    24,    33,   122,    33,   226,    54,   251,   223,
      51,    53,   293,   318,   137,   309,    27,   320,    27,   323,
     133,   106,   281,   288,    37,    38,    39,    23,   351,   126,
      24,   473,    55,    33,   431,    48,   216,    56,   277,   506,
     406,   143,   202,  -103,   148,   220,    27,   369,   235,   101,
      61,    62,   310,    61,    62,   237,   591,    16,    48,    61,
      62,   280,    -2,   210,   121,   520,    48,   645,   195,    16,
      61,    62,   132,    16,   141,   142,     4,   179,   111,    60,
     144,   200,   665,    -3,   559,   204,   149,   226,   133,    60,
      60,    60,   101,   319,   106,   496,   180,   464,   465,   324,
      60,   112,   113,   243,   600,   646,   518,   330,    60,   112,
     113,   190,   255,   592,   256,    40,   199,   577,    48,   349,
     666,   121,    48,   677,  -103,   288,   556,   278,   249,    63,
     427,   121,    65,   429,   285,   238,   216,   596,    67,   275,
     132,   511,   279,   101,   360,   524,   314,   216,   133,    41,
     317,   235,    34,   216,   277,   636,   316,   472,   237,   182,
     101,   678,   101,   653,   112,   113,   107,   183,   184,    16,
     108,    43,   116,    60,   649,   199,   290,   652,   322,   654,
      69,   291,   201,    42,    60,   221,   104,   662,   329,   222,
     331,   675,   504,   182,   443,   200,   229,   612,    16,   204,
     108,   183,   184,     7,     8,     9,    10,   144,    50,   343,
     243,    11,    12,    13,   449,   136,   450,   365,   117,   686,
      27,   366,   325,    57,   327,    58,   118,    15,   367,    16,
     416,   138,   121,   368,   399,   121,   121,   355,   238,   400,
     620,   488,   140,   281,   493,   190,   101,   383,   150,   383,
     133,   340,   432,   433,   512,   195,   434,   151,   478,   508,
     408,   411,   461,   684,   151,   227,   228,   179,   685,   181,
      60,    43,   402,   466,   467,   468,    52,   470,   471,    35,
      36,    54,   462,   315,   362,   363,    43,   116,   469,   286,
     287,    61,    62,   463,   205,   374,    27,   243,    16,   485,
     516,   349,   484,   486,   222,   188,    55,   350,   349,   129,
     130,    56,   478,    16,   444,    11,    12,    13,   403,   224,
     375,   482,   -20,   -20,   -20,   -20,   404,   184,   227,   228,
     -20,   -20,   -20,   117,     7,   232,     9,   189,   407,   410,
     240,   118,    11,    12,    13,   111,   -20,    43,  -163,   247,
     521,   199,   383,  -163,    43,   436,   248,   544,    15,   474,
     418,   420,   444,   517,   494,    43,   402,   108,   252,   199,
     253,    43,   116,   632,    16,   405,    48,   222,   -83,    27,
     106,    16,   421,   133,   483,   544,   564,    43,   402,   560,
     249,   199,    16,   634,   409,  -163,   475,   108,    16,  -163,
     -20,   409,   404,   184,   170,   171,   172,  -270,  -270,   404,
     184,   292,   510,   497,    16,   515,   111,    48,   117,    72,
      45,   647,    43,   648,   306,    27,   118,   242,   313,    61,
      62,   121,   597,    48,   403,   112,   113,   133,   200,   204,
      43,   402,   404,   184,   121,   200,   540,   307,   611,    16,
     522,   326,    43,   286,   287,   332,   199,    60,   338,   211,
     539,   333,  -106,  -106,  -106,  -106,   336,    16,  -106,    44,
    -106,  -106,  -106,   341,   540,   405,   405,    45,   342,    16,
     344,   544,    48,   347,   680,   576,  -106,   510,   539,   348,
     542,   352,   688,   543,   111,    45,   553,  -163,   226,    44,
     578,   356,  -163,   378,  -268,  -268,   361,    45,   391,    27,
     307,   487,   144,   464,   465,   396,   598,   364,   379,   601,
     604,   668,   669,   153,   155,   615,    54,   608,   385,   584,
     586,    61,    62,   192,   193,   194,   200,   614,   386,   389,
    -106,   401,   621,   415,  -163,   398,   417,   542,  -163,   624,
     543,    55,   180,   553,   430,   216,    56,   216,   435,  -304,
     631,   492,   617,   -32,   500,   501,    27,   502,   523,     7,
     540,     9,   189,   405,   477,   129,   130,    11,    12,    13,
     562,    11,    12,    13,   539,   168,   169,   170,   171,   172,
     254,   563,   601,    15,   -33,   257,   258,   121,   572,    48,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   173,   174,   671,   182,   175,   176,   177,   178,
     601,   566,   569,   183,   184,   570,   211,   573,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,   626,  -115,  -115,  -115,  -115,
    -115,   579,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,   574,  -115,  -115,
     581,     7,     8,     9,    10,  -115,   582,   413,  -115,    11,
      12,    13,   583,  -115,  -115,  -115,   587,   595,  -115,  -115,
     607,   599,   609,  -115,   610,    15,   623,    16,   625,   627,
     128,   129,   130,   628,   664,   630,   639,    11,    12,    13,
     345,   346,  -115,  -115,  -115,  -115,   438,  -115,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,    16,  -325,  -325,  -325,  -325,
    -325,   640,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,   655,  -325,  -325,
     650,   659,   663,   667,   673,  -325,   676,   602,  -325,   679,
     311,   682,   687,  -325,  -325,  -325,   359,   489,  -325,  -325,
     476,   593,   211,  -325,   127,  -325,  -325,  -325,  -325,   289,
     357,   387,   505,  -325,  -325,  -325,   480,   558,   390,   328,
     384,   335,  -325,   397,  -325,  -325,   481,  -325,   495,  -325,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     211,   490,  -325,  -325,   388,   446,   637,  -325,  -325,   638,
    -325,   672,   674,   353,  -325,   425,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,     0,  -325,   414,
    -325,     0,  -325,  -325,   453,     0,     0,     0,     0,  -325,
       0,     0,  -325,  -102,     0,     0,     0,  -325,  -325,  -325,
       0,     0,  -325,  -325,     0,     0,     0,  -325,     0,     0,
      70,     7,     8,     9,    10,    71,    72,   425,    73,    11,
      12,    13,     0,     0,     0,     0,  -325,  -300,  -325,  -325,
       0,  -325,     0,     0,     0,    15,    74,    16,    17,   498,
      75,    76,     0,     7,     8,     9,    10,    77,   453,   507,
      78,    11,    12,    13,     0,    79,    80,    81,     0,     0,
      82,    83,     0,     0,     0,    84,     0,    15,   453,    16,
       0,   519,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,   588,     0,  -325,  -325,    85,    86,   453,  -325,
    -325,   453,  -325,   453,     0,     0,  -325,     0,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,   565,
    -325,     0,  -325,     0,  -325,  -325,   585,     0,     0,     0,
     571,  -325,     0,     0,  -325,     0,     0,     0,     0,  -325,
    -325,  -325,     0,     0,  -325,  -325,     0,     0,     0,  -325,
       0,     0,     0,   594,     0,     0,   453,     7,    57,     9,
      58,     0,     0,     0,     0,    11,    12,    13,  -325,     0,
    -325,  -325,     0,  -325,     0,   618,     0,  -335,  -335,     0,
       0,    15,  -335,  -335,     0,  -335,     0,     0,     0,  -335,
       0,  -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,  -335,
    -335,  -335,     0,  -335,   629,  -335,     0,  -335,  -335,     0,
       0,     0,     0,     0,  -335,     0,     0,  -335,   111,     0,
       0,  -163,  -335,  -335,  -335,     0,  -163,  -335,  -335,     0,
     419,   145,  -335,    70,     7,     0,     9,    98,    71,    72,
       0,    73,    11,    12,    13,     0,     0,     0,     0,   112,
     113,  -335,     0,  -335,  -335,     0,  -335,     0,    15,    74,
       0,    17,     0,    75,    76,     0,     0,     0,  -163,     0,
      77,     0,  -163,    78,     0,     0,     0,     0,    79,    80,
      81,     0,     0,    82,    83,     0,     0,   447,    84,   448,
      62,     0,     0,     0,    71,    72,     0,    73,     7,     8,
       9,    10,     0,     0,     0,     0,    11,    12,    13,    85,
      86,     0,   -92,     0,     0,    74,     0,    17,     0,    75,
      76,     0,    15,     0,    16,     0,    77,     0,     0,    78,
       0,     0,     0,     0,    79,    80,    81,     0,     0,    82,
      83,     0,     0,   449,    84,   450,   447,     0,   448,    62,
       0,     0,     0,    71,    72,     0,    73,   166,   167,   168,
     169,   170,   171,   172,  -191,    85,    86,     0,   451,     0,
       0,     0,     0,     0,    74,     0,    17,     0,    75,    76,
       0,     0,     0,     0,     0,    77,     0,     0,    78,     0,
       0,     0,     0,    79,    80,    81,     0,     0,    82,    83,
       0,     0,   449,    84,   450,   447,     0,    70,     0,     0,
       0,     0,    71,    72,     0,    73,     0,     0,     0,     0,
       0,     0,     0,  -259,    85,    86,     0,   451,     0,     0,
       0,     0,     0,    74,     0,    17,     0,    75,    76,  -204,
       0,     0,     0,     0,    77,     0,     0,    78,     0,     0,
       0,     0,    79,    80,    81,     0,     0,    82,    83,     0,
       0,  -204,    84,  -204,   423,     0,    70,     0,     0,     0,
       0,    71,    72,     0,    73,   165,   166,   167,   168,   169,
     170,   171,   172,    85,    86,     0,   451,     0,     0,     0,
       0,     0,    74,     0,    17,     0,    75,    76,     0,     7,
       8,     9,    10,    77,     0,     0,    78,    11,    12,    13,
       0,    79,    80,    81,     0,     0,    82,    83,   447,     0,
      70,    84,     0,    15,     0,    71,    72,   110,    73,     0,
     -28,   -28,   -28,   -28,     0,     0,     0,     0,   -28,   -28,
     -28,     0,    85,    86,     0,   424,    74,     0,    17,     0,
      75,    76,     0,   111,   -28,     0,  -163,    77,     0,     0,
      78,  -163,     0,     0,     0,    79,    80,    81,     0,     0,
      82,    83,   173,   174,     0,    84,   175,   176,   177,   178,
       0,     0,     0,     0,   112,   113,     0,     0,     0,   230,
       0,     0,   -24,   -24,   -24,   -24,    85,    86,     0,   451,
     -24,   -24,   -24,  -163,    70,     0,     0,  -163,   -28,    71,
      72,     0,    73,     0,     0,   111,   -24,     0,  -163,     0,
       0,     0,     0,  -163,     0,     0,     0,     0,     0,     0,
      74,     0,    17,     0,    75,    76,     0,     0,     0,     0,
       0,    77,     0,     0,    78,     0,   112,   113,     0,    79,
      80,   208,     0,     0,    82,    83,     0,     0,     0,    84,
       0,     0,     0,     0,     0,  -163,    70,     0,     0,  -163,
     -24,    71,    72,     0,    73,     0,     0,     0,     0,     0,
      85,    86,     0,     0,   209,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    17,     0,    75,    76,     0,     0,
       0,     0,     0,    77,     0,     0,    78,     0,     0,     0,
       0,    79,    80,    81,     0,     0,    82,    83,     0,    70,
       0,    84,     0,     0,    71,    72,     0,    73,   233,     0,
       0,     7,     0,     9,    98,     0,     0,     0,     0,    11,
      12,    13,    85,    86,     0,    74,   284,    17,     0,    75,
      76,     0,     0,     0,     0,    15,    77,     0,    17,    78,
       0,     0,     0,     0,    79,    80,    81,     0,     0,    82,
      83,     0,    70,     0,    84,     0,     0,    71,    72,   294,
      73,   295,     7,     8,     9,    10,     0,     0,   296,     0,
      11,    12,    13,     0,     0,    85,    86,     0,    74,   305,
      17,     0,    75,    76,   234,  -262,    15,     0,    16,    77,
       0,     0,    78,     0,     0,     0,     0,    79,    80,    81,
       0,     0,    82,    83,     0,    70,     0,    84,     0,     0,
      71,    72,   294,    73,     0,     7,     8,     9,    10,     0,
       0,   296,     0,    11,    12,    13,     0,     0,    85,    86,
       0,    74,   321,    17,  -385,    75,    76,     0,     0,    15,
       0,    16,    77,     0,     0,    78,     0,     0,     0,     0,
      79,    80,    81,     0,     0,    82,    83,     0,    70,     0,
      84,     0,     0,    71,    72,     0,    73,   233,     0,     0,
       7,     0,     9,    98,     0,     0,     0,     0,    11,    12,
      13,    85,    86,     0,    74,   354,    17,  -385,    75,    76,
       0,     0,     0,     0,    15,    77,     0,    17,    78,     0,
       0,     0,     0,    79,    80,   513,     0,     0,    82,    83,
       0,    70,     0,    84,     0,     0,    71,    72,   158,    73,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    85,    86,     0,    74,   514,    17,
       0,    75,    76,   605,     0,     0,     0,     0,    77,     0,
       0,    78,     0,     0,     0,     0,    79,    80,    81,     0,
       0,    82,    83,     0,     0,     0,    84,     0,   156,   157,
     158,   606,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,     0,    85,    86,     0,
       0,   561,   448,   525,     8,     9,    10,    71,    72,     0,
      73,    11,    12,    13,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,    15,    74,    16,
      17,     0,    75,    76,     0,     0,     0,     0,     0,    77,
       0,     0,    78,     0,     0,     0,     0,    79,    80,    81,
       0,     0,    82,    83,     0,     0,     0,    84,   156,   157,
     158,   644,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   538,     0,    85,    86,
       0,   249,   448,    62,     0,     0,     0,    71,    72,     0,
      73,     0,     0,     0,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,    74,     0,
      17,     0,    75,    76,     0,     0,     0,     0,     0,    77,
       0,     0,    78,     0,     0,     0,     0,    79,    80,    81,
       0,     0,    82,    83,    70,     0,     0,    84,     0,    71,
      72,     0,    73,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   538,     0,    85,    86,
      74,   249,    17,     0,    75,    76,     0,     0,     0,     0,
       0,    77,     0,     0,    78,     0,     0,     0,     0,    79,
      80,    81,     0,     0,    82,    83,     0,     0,     0,    84,
      70,     7,     0,     9,    98,    71,    72,     0,    73,    11,
      12,    13,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,     0,   339,     0,    15,    74,     0,    17,     0,
      75,    76,     0,    70,     0,     0,     0,    77,    71,    72,
      78,    73,     0,     0,     0,    79,    80,    81,     0,     0,
      82,    83,     0,     0,     0,    84,     0,     0,     0,    74,
       0,    17,     0,    75,    76,     0,    70,     0,     0,     0,
      77,    71,    72,    78,    73,     0,    85,    86,    79,    80,
      81,     0,     0,    82,    83,     0,     0,     0,    84,     0,
       0,     0,    74,     0,    17,     0,    75,    76,     0,     0,
       0,     0,     0,    77,     0,     0,    78,   575,     0,    85,
      86,    79,    80,    81,     0,     0,    82,    83,    70,     0,
       0,    84,     0,    71,    72,     0,    73,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,    85,    86,    74,     0,    17,     0,    75,    76,
       0,    70,     0,     0,     0,    77,    71,    72,    78,    73,
       0,     0,     0,    79,    80,    81,     0,     0,    82,    83,
       0,     0,     0,   152,     0,     0,     0,    74,     0,    17,
       0,    75,    76,     0,   358,     0,     0,     0,    77,    71,
      72,    78,    73,     0,    85,    86,    79,    80,    81,     0,
       0,    82,    83,     0,     0,     0,   154,     0,     0,     0,
      74,     0,    17,     0,    75,    76,     0,     0,     0,     0,
       0,    77,     0,     0,    78,     0,     0,    85,    86,    79,
      80,    81,     0,     0,    82,    83,     0,     0,     6,    84,
    -119,     7,     8,     9,    10,     0,     0,     0,     0,    11,
      12,    13,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,     0,     0,    14,    15,     0,    16,    17,     0,
       0,   554,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,  -119,     0,     0,
       0,     0,     0,     0,     0,  -119,   156,   157,   158,     0,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    18,   156,   157,   158,     0,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,    16,     0,     0,     0,
       0,   156,   157,   158,   555,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
     156,   157,   158,   622,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172
};

static const yytype_int16 yycheck[] =
{
      40,    41,    53,    29,    30,    31,   117,     3,    22,     5,
       3,   103,     5,     3,    51,     5,   122,    24,   147,   121,
      23,    24,   205,   225,    65,   213,     3,   227,     5,   231,
      59,    45,   183,   187,    11,    12,    13,    33,   283,    53,
      33,   418,    24,    33,   383,    22,   114,    24,   176,   459,
     362,    77,   108,     1,    84,   118,    33,     1,   135,    41,
       3,     4,    10,     3,     4,   135,     1,    30,    45,     3,
       4,   182,     0,   113,    51,    38,    53,    38,   104,    30,
       3,     4,    59,    30,    75,    76,     0,     9,    27,    27,
      81,   105,    38,     0,   504,   109,    87,   203,   127,    37,
      38,    39,    84,   226,   118,   444,    97,    58,    59,   232,
      48,    58,    59,   139,   568,    76,   479,   240,    56,    58,
      59,   103,   152,    58,   154,    58,     7,    50,   105,   280,
      76,   108,   109,    38,    82,   289,   499,   177,    82,    82,
     377,   118,    82,   380,   184,   135,   214,   557,    82,   175,
     127,   463,   178,   135,    76,   494,   219,   225,   187,    58,
     223,   238,     5,   231,   292,   619,   222,   412,   238,    50,
     152,    76,   154,   642,    58,    59,    77,    58,    59,    30,
      81,     3,     4,   121,   638,     7,    76,   641,   228,   643,
      33,    81,    76,    58,   132,    77,    58,   651,   239,    81,
     241,   670,    35,    50,   392,   219,    77,   584,    30,   223,
      81,    58,    59,     4,     5,     6,     7,   208,    77,   259,
     246,    12,    13,    14,    57,    82,    59,    77,    50,   683,
     207,    81,   235,     5,   237,     7,    58,    28,    76,    30,
     369,    82,   219,    81,    76,   222,   223,   287,   238,    81,
     589,   439,    82,   404,   442,   237,   238,   325,    76,   327,
     289,   252,   385,   386,   464,   291,   389,    81,   422,    76,
     362,   363,   400,    76,    81,    58,    59,     9,    81,    76,
     218,     3,     4,   406,   407,   408,    77,   410,   411,    77,
      78,   298,   403,    76,   297,   298,     3,     4,   409,    58,
      59,     3,     4,   405,    58,   312,   283,   333,    30,    77,
      77,   462,   435,    81,    81,    76,   298,    76,   469,     6,
       7,   298,   476,    30,   392,    12,    13,    14,    50,     1,
     312,    38,     4,     5,     6,     7,    58,    59,    58,    59,
      12,    13,    14,    50,     4,    78,     6,     7,   362,   363,
      78,    58,    12,    13,    14,    27,    28,     3,    30,    76,
     483,     7,   430,    35,     3,   391,    76,   496,    28,   420,
     373,   374,   440,    77,   442,     3,     4,    81,    76,     7,
      77,     3,     4,    77,    30,   362,   363,    81,    38,   366,
     404,    30,   374,   422,   431,   524,   519,     3,     4,   510,
      82,     7,    30,    77,    50,    77,   420,    81,    30,    81,
      82,    50,    58,    59,    50,    51,    52,    77,    78,    58,
      59,    58,    50,   449,    30,   465,    27,   404,    50,     9,
      58,   633,     3,   635,    83,   412,    58,     1,    35,     3,
       4,   418,   565,   420,    50,    58,    59,   476,   462,   463,
       3,     4,    58,    59,   431,   469,   496,    77,   581,    30,
     486,    77,     3,    58,    59,    35,     7,   405,    76,     1,
     496,    81,     4,     5,     6,     7,    78,    30,    10,    50,
      12,    13,    14,    76,   524,   462,   463,    58,    76,    30,
      38,   620,   469,    76,   677,   535,    28,    50,   524,    83,
     496,    83,   685,   496,    27,    58,   496,    30,   614,    50,
     536,    76,    35,    35,    77,    78,    76,    58,    32,   496,
      77,    78,   513,    58,    59,    38,   566,    76,    83,   569,
     570,   660,   661,    91,    92,   586,   543,   577,    78,   542,
     543,     3,     4,     5,     6,     7,   560,   584,    78,    78,
      82,    76,   592,     3,    77,    83,    77,   553,    81,   599,
     553,   543,   553,   553,    81,   633,   543,   635,    78,    78,
     610,    78,   586,    38,    38,    78,   553,    81,    78,     4,
     620,     6,     7,   560,     5,     6,     7,    12,    13,    14,
      83,    12,    13,    14,   620,    48,    49,    50,    51,    52,
     151,    78,   642,    28,    38,   156,   157,   584,    38,   586,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    53,    54,   664,    50,    57,    58,    59,    60,
     670,    58,    58,    58,    59,    58,     1,    77,     3,     4,
       5,     6,     7,     8,     9,    76,    11,    12,    13,    14,
      15,     7,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    77,    33,    34,
      38,     4,     5,     6,     7,    40,    77,    10,    43,    12,
      13,    14,    77,    48,    49,    50,    16,    78,    53,    54,
      77,    58,    77,    58,    58,    28,    76,    30,    17,    77,
       5,     6,     7,    76,    58,    77,    76,    12,    13,    14,
     261,   262,    77,    78,    79,    80,     1,    82,     3,     4,
       5,     6,     7,     8,     9,    30,    11,    12,    13,    14,
      15,    76,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     9,    33,    34,
      77,    77,    77,    81,    77,    40,    76,   569,    43,    76,
     214,    77,    77,    48,    49,    50,   292,   440,    53,    54,
     421,   553,     1,    58,    55,     4,     5,     6,     7,   190,
     291,   332,   458,    12,    13,    14,   428,   502,   335,   238,
     327,   246,    77,   344,    79,    80,   430,    82,   442,    28,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       1,   440,     3,     4,   333,   393,   620,     8,     9,   620,
      11,   665,   667,   286,    15,   376,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,   366,
      31,    -1,    33,    34,   395,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    43,    82,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    54,    -1,    -1,    -1,    58,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,   428,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    28,    29,    30,    31,   450,
      33,    34,    -1,     4,     5,     6,     7,    40,   459,   460,
      43,    12,    13,    14,    -1,    48,    49,    50,    -1,    -1,
      53,    54,    -1,    -1,    -1,    58,    -1,    28,   479,    30,
      -1,   482,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,     1,    -1,     3,     4,    79,    80,   499,     8,
       9,   502,    11,   504,    -1,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,   520,
      29,    -1,    31,    -1,    33,    34,    77,    -1,    -1,    -1,
     531,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    -1,    53,    54,    -1,    -1,    -1,    58,
      -1,    -1,    -1,   554,    -1,    -1,   557,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    12,    13,    14,    77,    -1,
      79,    80,    -1,    82,    -1,     1,    -1,     3,     4,    -1,
      -1,    28,     8,     9,    -1,    11,    -1,    -1,    -1,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,   605,    31,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    43,    27,    -1,
      -1,    30,    48,    49,    50,    -1,    35,    53,    54,    -1,
      77,     1,    58,     3,     4,    -1,     6,     7,     8,     9,
      -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,    58,
      59,    77,    -1,    79,    80,    -1,    82,    -1,    28,    29,
      -1,    31,    -1,    33,    34,    -1,    -1,    -1,    77,    -1,
      40,    -1,    81,    43,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    -1,    53,    54,    -1,    -1,     1,    58,     3,
       4,    -1,    -1,    -1,     8,     9,    -1,    11,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    79,
      80,    -1,    82,    -1,    -1,    29,    -1,    31,    -1,    33,
      34,    -1,    28,    -1,    30,    -1,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,
      54,    -1,    -1,    57,    58,    59,     1,    -1,     3,     4,
      -1,    -1,    -1,     8,     9,    -1,    11,    46,    47,    48,
      49,    50,    51,    52,    78,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    31,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    54,
      -1,    -1,    57,    58,    59,     1,    -1,     3,    -1,    -1,
      -1,    -1,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    31,    -1,    33,    34,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    54,    -1,
      -1,    57,    58,    59,     1,    -1,     3,    -1,    -1,    -1,
      -1,     8,     9,    -1,    11,    45,    46,    47,    48,    49,
      50,    51,    52,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    -1,    33,    34,    -1,     4,
       5,     6,     7,    40,    -1,    -1,    43,    12,    13,    14,
      -1,    48,    49,    50,    -1,    -1,    53,    54,     1,    -1,
       3,    58,    -1,    28,    -1,     8,     9,     1,    11,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    -1,    79,    80,    -1,    82,    29,    -1,    31,    -1,
      33,    34,    -1,    27,    28,    -1,    30,    40,    -1,    -1,
      43,    35,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      53,    54,    53,    54,    -1,    58,    57,    58,    59,    60,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,     1,
      -1,    -1,     4,     5,     6,     7,    79,    80,    -1,    82,
      12,    13,    14,    77,     3,    -1,    -1,    81,    82,     8,
       9,    -1,    11,    -1,    -1,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    31,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    58,    59,    -1,    48,
      49,    50,    -1,    -1,    53,    54,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    81,
      82,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    -1,    53,    54,    -1,     3,
      -1,    58,    -1,    -1,     8,     9,    -1,    11,     1,    -1,
      -1,     4,    -1,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    79,    80,    -1,    29,    83,    31,    -1,    33,
      34,    -1,    -1,    -1,    -1,    28,    40,    -1,    31,    43,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,
      54,    -1,     3,    -1,    58,    -1,    -1,     8,     9,     1,
      11,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    79,    80,    -1,    29,    83,
      31,    -1,    33,    34,    77,    78,    28,    -1,    30,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    54,    -1,     3,    -1,    58,    -1,    -1,
       8,     9,     1,    11,    -1,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    12,    13,    14,    -1,    -1,    79,    80,
      -1,    29,    83,    31,    76,    33,    34,    -1,    -1,    28,
      -1,    30,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    53,    54,    -1,     3,    -1,
      58,    -1,    -1,     8,     9,    -1,    11,     1,    -1,    -1,
       4,    -1,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    79,    80,    -1,    29,    83,    31,    76,    33,    34,
      -1,    -1,    -1,    -1,    28,    40,    -1,    31,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    54,
      -1,     3,    -1,    58,    -1,    -1,     8,     9,    37,    11,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    79,    80,    -1,    29,    83,    31,
      -1,    33,    34,    10,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    58,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    -1,    79,    80,    -1,
      -1,    83,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    54,    -1,    -1,    -1,    58,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    77,    -1,    79,    80,
      -1,    82,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      11,    -1,    -1,    -1,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    -1,
      31,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    54,     3,    -1,    -1,    58,    -1,     8,
       9,    -1,    11,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    77,    -1,    79,    80,
      29,    82,    31,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    -1,    53,    54,    -1,    -1,    -1,    58,
       3,     4,    -1,     6,     7,     8,     9,    -1,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    82,    -1,    28,    29,    -1,    31,    -1,
      33,    34,    -1,     3,    -1,    -1,    -1,    40,     8,     9,
      43,    11,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      53,    54,    -1,    -1,    -1,    58,    -1,    -1,    -1,    29,
      -1,    31,    -1,    33,    34,    -1,     3,    -1,    -1,    -1,
      40,     8,     9,    43,    11,    -1,    79,    80,    48,    49,
      50,    -1,    -1,    53,    54,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    29,    -1,    31,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    43,    77,    -1,    79,
      80,    48,    49,    50,    -1,    -1,    53,    54,     3,    -1,
      -1,    58,    -1,     8,     9,    -1,    11,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,    -1,    79,    80,    29,    -1,    31,    -1,    33,    34,
      -1,     3,    -1,    -1,    -1,    40,     8,     9,    43,    11,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    54,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    29,    -1,    31,
      -1,    33,    34,    -1,     3,    -1,    -1,    -1,    40,     8,
       9,    43,    11,    -1,    79,    80,    48,    49,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      29,    -1,    31,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    -1,    79,    80,    48,
      49,    50,    -1,    -1,    53,    54,    -1,    -1,     1,    58,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    27,    28,    -1,    30,    31,    -1,
      -1,    10,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    35,    36,    37,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    77,    35,    36,    37,    -1,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    37,    83,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      35,    36,    37,    83,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    85,    86,    87,     0,    88,     1,     4,     5,     6,
       7,    12,    13,    14,    27,    28,    30,    31,    77,    89,
      90,    91,   123,   126,   130,   131,   134,   144,   145,   169,
     170,   171,   172,   241,    89,    77,    78,   144,   144,   144,
      58,    58,    58,     3,    50,    58,   137,   141,   144,   168,
      77,   123,    77,   123,   131,   134,   144,     5,     7,   127,
     145,     3,     4,    82,    98,    82,    98,    82,    98,    89,
       3,     8,     9,    11,    29,    33,    34,    40,    43,    48,
      49,    50,    53,    54,    58,    79,    80,    99,   100,   102,
     103,   104,   105,   106,   108,   114,   116,   241,     7,   100,
     132,   134,   186,   188,    58,   189,   168,    77,    81,   124,
       1,    27,    58,    59,    96,   138,     4,    50,    58,   136,
     139,   144,   165,   166,   168,   137,   168,   127,     5,     6,
       7,   135,   144,   172,   179,   180,    82,   179,    82,   176,
      82,   106,   106,    98,   106,     1,   100,   115,   186,   106,
      76,    81,    58,   103,    58,   103,    35,    36,    37,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    57,    58,    59,    60,     9,
     106,    76,    50,    58,    59,   187,   190,   133,    76,     7,
     134,   187,     5,     6,     7,    98,   146,   147,   148,     7,
     168,    76,   139,   165,   168,    58,   236,   237,    50,    83,
     100,     1,   117,   118,   119,   194,   206,   143,   144,   189,
     166,    77,    81,   124,     1,    92,   138,    58,    59,    77,
       1,    94,    78,     1,    77,   132,   181,   188,   241,   173,
      78,   174,     1,    98,   184,   185,   175,    76,    76,    82,
     200,   201,    76,    77,   108,   186,   186,   108,   108,   111,
     113,   110,   109,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,    98,   101,   102,   100,    98,
     189,   190,   229,   230,    83,   100,    58,    59,   135,   133,
      76,    81,    58,   116,     1,     3,    10,   126,   130,   231,
     233,   234,   235,   238,   239,    83,    83,    77,    97,   194,
      10,   118,   207,    35,   166,    76,   139,   166,   117,   143,
     236,    83,   100,   117,   143,   123,    77,   123,   181,   179,
     143,   179,    35,    81,   178,   184,    78,   195,    76,    82,
     106,    76,    76,   100,    38,   108,   108,    76,    83,   190,
      76,   231,    83,   229,    83,   100,    76,   147,     3,   101,
      76,    76,   123,   123,    76,    77,    81,    76,    81,     1,
     199,   201,   120,   128,   131,   134,   142,    93,    35,    83,
      95,   182,   183,   206,   182,    78,    78,   108,   185,    78,
     178,    32,   196,   197,   198,   107,    38,   108,    83,    76,
      81,    76,     4,    50,    58,   144,   167,   168,   187,    50,
     168,   187,   232,    10,   235,     3,   201,    77,   123,    77,
     123,   134,   129,     1,    82,   108,   149,   199,   140,   199,
      81,   207,   143,   143,   143,    78,    98,   240,     1,   121,
     122,   191,   192,   194,   206,   209,   198,     1,     3,    57,
      59,    82,    98,   108,   151,   152,   153,   155,   157,   158,
     112,   102,   189,   124,    58,    59,   143,   143,   143,   189,
     143,   143,   231,   136,   137,   168,   129,     5,   135,   150,
     149,   183,    38,   165,   143,    77,    81,    78,   194,   121,
     191,   193,    78,   194,   206,   209,   207,    98,   108,   156,
      38,    78,    81,   177,    35,   158,   155,   108,    76,   167,
      50,   167,   236,    50,    83,   100,    77,    77,   151,   108,
      38,   143,    98,    78,   207,     4,    15,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    77,    98,
     100,   125,   126,   130,   201,   202,   203,   204,   210,   211,
     219,   220,   222,   241,    10,    83,   151,   154,   153,   155,
     189,    83,    83,    78,   143,   108,    58,   213,   205,    58,
      58,   108,    38,    77,    77,    77,   100,    50,    98,     7,
     223,    38,    77,    77,   123,    77,   123,    16,     1,   206,
     208,     1,    58,   125,   108,    78,   155,   143,   100,    58,
     208,   100,   114,   224,   100,    10,    38,    77,   100,    77,
      58,   143,   136,   159,   165,   137,   162,   168,     1,   212,
     207,   100,    83,    76,   100,    17,    76,    77,    76,   108,
      77,   100,    77,   160,    77,   163,   208,   211,   222,    76,
      76,   221,   215,   218,    38,    38,    76,   117,   117,   208,
      77,   214,   208,   224,   208,     9,   225,   226,   227,    77,
     161,   164,   208,    77,    58,    38,    76,    81,   201,   201,
     216,   100,   225,    77,   227,   224,    76,    38,    76,    76,
     116,   228,    77,   217,    76,    81,   208,    77,   116
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { if (pedantic)
		    pedwarn ("ANSI C forbids an empty source file");
		;}
    break;

  case 3:

    {
		  /* In case there were missing closebraces,
		     get us back to the global binding level.  */
		  while (! global_bindings_p ())
		    poplevel (0, 0, 0);
		;}
    break;

  case 4:

    {(yyval.ttype) = NULL_TREE; ;}
    break;

  case 6:

    {(yyval.ttype) = NULL_TREE; ;}
    break;

  case 10:

    { STRIP_NOPS ((yyvsp[(3) - (5)].ttype));
		  if ((TREE_CODE ((yyvsp[(3) - (5)].ttype)) == ADDR_EXPR
		       && TREE_CODE (TREE_OPERAND ((yyvsp[(3) - (5)].ttype), 0)) == STRING_CST)
		      || TREE_CODE ((yyvsp[(3) - (5)].ttype)) == STRING_CST)
		    assemble_asm ((yyvsp[(3) - (5)].ttype));
		  else
		    error ("argument of `asm' is not a constant string"); ;}
    break;

  case 11:

    { pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 12:

    { if (pedantic)
		    error ("ANSI C forbids data definition with no type or storage class");
		  else if (!flag_traditional)
		    warning ("data definition has no type or storage class"); 

		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(1) - (3)].itype)); ;}
    break;

  case 13:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 14:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype));  ;}
    break;

  case 15:

    { pedwarn ("empty declaration"); ;}
    break;

  case 16:

    { shadow_tag ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 19:

    { if (pedantic)
		    pedwarn ("ANSI C does not allow extra `;' outside of a function"); ;}
    break;

  case 20:

    { if (! start_function (current_declspecs, (yyvsp[(3) - (3)].ttype),
					prefix_attributes, NULL_TREE, 0))
		    YYERROR1;
		  reinit_parse_for_function (); ;}
    break;

  case 21:

    { store_parm_decls (); ;}
    break;

  case 22:

    { finish_function (0); 
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (7)].itype)); ;}
    break;

  case 23:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 24:

    { if (! start_function (current_declspecs, (yyvsp[(3) - (3)].ttype),
					prefix_attributes, NULL_TREE, 0))
		    YYERROR1;
		  reinit_parse_for_function (); ;}
    break;

  case 25:

    { store_parm_decls (); ;}
    break;

  case 26:

    { finish_function (0); 
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (7)].itype)); ;}
    break;

  case 27:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 28:

    { if (! start_function (NULL_TREE, (yyvsp[(2) - (2)].ttype),
					prefix_attributes, NULL_TREE, 0))
		    YYERROR1;
		  reinit_parse_for_function (); ;}
    break;

  case 29:

    { store_parm_decls (); ;}
    break;

  case 30:

    { finish_function (0); 
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(1) - (6)].itype)); ;}
    break;

  case 31:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(1) - (3)].itype)); ;}
    break;

  case 34:

    { (yyval.code) = ADDR_EXPR; ;}
    break;

  case 35:

    { (yyval.code) = NEGATE_EXPR; ;}
    break;

  case 36:

    { (yyval.code) = CONVERT_EXPR; ;}
    break;

  case 37:

    { (yyval.code) = PREINCREMENT_EXPR; ;}
    break;

  case 38:

    { (yyval.code) = PREDECREMENT_EXPR; ;}
    break;

  case 39:

    { (yyval.code) = BIT_NOT_EXPR; ;}
    break;

  case 40:

    { (yyval.code) = TRUTH_NOT_EXPR; ;}
    break;

  case 41:

    { (yyval.ttype) = build_compound_expr ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 42:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 44:

    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 45:

    { chainon ((yyvsp[(1) - (3)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 47:

    { (yyval.ttype) = build_indirect_ref ((yyvsp[(2) - (2)].ttype), "unary *"); ;}
    break;

  case 48:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 49:

    { (yyval.ttype) = build_unary_op ((yyvsp[(1) - (2)].code), (yyvsp[(2) - (2)].ttype), 0);
		  overflow_warning ((yyval.ttype)); ;}
    break;

  case 50:

    { tree label = lookup_label ((yyvsp[(2) - (2)].ttype));
		  if (pedantic)
		    pedwarn ("ANSI C forbids `&&'");
		  if (label == 0)
		    (yyval.ttype) = null_pointer_node;
		  else
		    {
		      TREE_USED (label) = 1;
		      (yyval.ttype) = build1 (ADDR_EXPR, ptr_type_node, label);
		      TREE_CONSTANT ((yyval.ttype)) = 1;
		    }
		;}
    break;

  case 51:

    { skip_evaluation--;
		  if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND ((yyvsp[(2) - (2)].ttype), 1)))
		    error ("`sizeof' applied to a bit-field");
		  (yyval.ttype) = c_sizeof (TREE_TYPE ((yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 52:

    { skip_evaluation--;
		  (yyval.ttype) = c_sizeof (groktypename ((yyvsp[(3) - (4)].ttype))); ;}
    break;

  case 53:

    { skip_evaluation--;
		  (yyval.ttype) = c_alignof_expr ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 54:

    { skip_evaluation--;
		  (yyval.ttype) = c_alignof (groktypename ((yyvsp[(3) - (4)].ttype))); ;}
    break;

  case 55:

    { (yyval.ttype) = build_unary_op (REALPART_EXPR, (yyvsp[(2) - (2)].ttype), 0); ;}
    break;

  case 56:

    { (yyval.ttype) = build_unary_op (IMAGPART_EXPR, (yyvsp[(2) - (2)].ttype), 0); ;}
    break;

  case 57:

    { skip_evaluation++; ;}
    break;

  case 58:

    { skip_evaluation++; ;}
    break;

  case 60:

    { tree type = groktypename ((yyvsp[(2) - (4)].ttype));
		  (yyval.ttype) = build_c_cast (type, (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 61:

    { start_init (NULL_TREE, NULL, 0);
		  (yyvsp[(2) - (4)].ttype) = groktypename ((yyvsp[(2) - (4)].ttype));
		  really_start_incremental_init ((yyvsp[(2) - (4)].ttype)); ;}
    break;

  case 62:

    { char *name;
		  tree result = pop_init_level (0);
		  tree type = (yyvsp[(2) - (7)].ttype);
		  finish_init ();

		  if (pedantic && ! flag_isoc9x)
		    pedwarn ("ANSI C forbids constructor expressions");
		  if (TYPE_NAME (type) != 0)
		    {
		      if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
			name = IDENTIFIER_POINTER (TYPE_NAME (type));
		      else
			name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
		    }
		  else
		    name = "";
		  (yyval.ttype) = result;
		  if (TREE_CODE (type) == ARRAY_TYPE && TYPE_SIZE (type) == 0)
		    {
		      int failure = complete_array_type (type, (yyval.ttype), 1);
		      if (failure)
			abort ();
		    }
		;}
    break;

  case 64:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 65:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 66:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 67:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 68:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 69:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 70:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 71:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 72:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 73:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 74:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 75:

    { (yyval.ttype) = parser_build_binary_op ((yyvsp[(2) - (3)].code), (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 76:

    { (yyvsp[(1) - (2)].ttype) = truthvalue_conversion (default_conversion ((yyvsp[(1) - (2)].ttype)));
		  skip_evaluation += (yyvsp[(1) - (2)].ttype) == boolean_false_node; ;}
    break;

  case 77:

    { skip_evaluation -= (yyvsp[(1) - (4)].ttype) == boolean_false_node;
		  (yyval.ttype) = parser_build_binary_op (TRUTH_ANDIF_EXPR, (yyvsp[(1) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 78:

    { (yyvsp[(1) - (2)].ttype) = truthvalue_conversion (default_conversion ((yyvsp[(1) - (2)].ttype)));
		  skip_evaluation += (yyvsp[(1) - (2)].ttype) == boolean_true_node; ;}
    break;

  case 79:

    { skip_evaluation -= (yyvsp[(1) - (4)].ttype) == boolean_true_node;
		  (yyval.ttype) = parser_build_binary_op (TRUTH_ORIF_EXPR, (yyvsp[(1) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 80:

    { (yyvsp[(1) - (2)].ttype) = truthvalue_conversion (default_conversion ((yyvsp[(1) - (2)].ttype)));
		  skip_evaluation += (yyvsp[(1) - (2)].ttype) == boolean_false_node; ;}
    break;

  case 81:

    { skip_evaluation += (((yyvsp[(1) - (5)].ttype) == boolean_true_node)
				      - ((yyvsp[(1) - (5)].ttype) == boolean_false_node)); ;}
    break;

  case 82:

    { skip_evaluation -= (yyvsp[(1) - (7)].ttype) == boolean_true_node;
		  (yyval.ttype) = build_conditional_expr ((yyvsp[(1) - (7)].ttype), (yyvsp[(4) - (7)].ttype), (yyvsp[(7) - (7)].ttype)); ;}
    break;

  case 83:

    { if (pedantic)
		    pedwarn ("ANSI C forbids omitting the middle term of a ?: expression");
		  /* Make sure first operand is calculated only once.  */
		  (yyvsp[(2) - (2)].ttype) = save_expr ((yyvsp[(1) - (2)].ttype));
		  (yyvsp[(1) - (2)].ttype) = truthvalue_conversion (default_conversion ((yyvsp[(2) - (2)].ttype)));
		  skip_evaluation += (yyvsp[(1) - (2)].ttype) == boolean_true_node; ;}
    break;

  case 84:

    { skip_evaluation -= (yyvsp[(1) - (5)].ttype) == boolean_true_node;
		  (yyval.ttype) = build_conditional_expr ((yyvsp[(1) - (5)].ttype), (yyvsp[(2) - (5)].ttype), (yyvsp[(5) - (5)].ttype)); ;}
    break;

  case 85:

    { (yyval.ttype) = build_modify_expr ((yyvsp[(1) - (3)].ttype), NOP_EXPR, (yyvsp[(3) - (3)].ttype));
		  C_SET_EXP_ORIGINAL_CODE ((yyval.ttype), MODIFY_EXPR); ;}
    break;

  case 86:

    { (yyval.ttype) = build_modify_expr ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].code), (yyvsp[(3) - (3)].ttype));
		  /* This inhibits warnings in truthvalue_conversion.  */
		  C_SET_EXP_ORIGINAL_CODE ((yyval.ttype), ERROR_MARK); ;}
    break;

  case 87:

    {
		  (yyval.ttype) = lastiddecl;
		  if (!(yyval.ttype) || (yyval.ttype) == error_mark_node)
		    {
		      if (yychar == YYEMPTY)
			yychar = YYLEX;
		      if (yychar == '(')
			{
			    {
			      /* Ordinary implicit function declaration.  */
			      (yyval.ttype) = implicitly_declare ((yyvsp[(1) - (1)].ttype));
			      assemble_external ((yyval.ttype));
			      TREE_USED ((yyval.ttype)) = 1;
			    }
			}
		      else if (current_function_decl == 0)
			{
			  error ("`%s' undeclared here (not in a function)",
				 IDENTIFIER_POINTER ((yyvsp[(1) - (1)].ttype)));
			  (yyval.ttype) = error_mark_node;
			}
		      else
			{
			    {
			      if (IDENTIFIER_GLOBAL_VALUE ((yyvsp[(1) - (1)].ttype)) != error_mark_node
				  || IDENTIFIER_ERROR_LOCUS ((yyvsp[(1) - (1)].ttype)) != current_function_decl)
				{
				  error ("`%s' undeclared (first use in this function)",
					 IDENTIFIER_POINTER ((yyvsp[(1) - (1)].ttype)));

				  if (! undeclared_variable_notice)
				    {
				      error ("(Each undeclared identifier is reported only once");
				      error ("for each function it appears in.)");
				      undeclared_variable_notice = 1;
				    }
				}
			      (yyval.ttype) = error_mark_node;
			      /* Prevent repeated error messages.  */
			      IDENTIFIER_GLOBAL_VALUE ((yyvsp[(1) - (1)].ttype)) = error_mark_node;
			      IDENTIFIER_ERROR_LOCUS ((yyvsp[(1) - (1)].ttype)) = current_function_decl;
			    }
			}
		    }
		  else if (TREE_TYPE ((yyval.ttype)) == error_mark_node)
		    (yyval.ttype) = error_mark_node;
		  else if (C_DECL_ANTICIPATED ((yyval.ttype)))
		    {
		      /* The first time we see a build-in function used,
			 if it has not been declared.  */
		      C_DECL_ANTICIPATED ((yyval.ttype)) = 0;
		      if (yychar == YYEMPTY)
			yychar = YYLEX;
		      if (yychar == '(')
			{
			  /* Omit the implicit declaration we
			     would ordinarily do, so we don't lose
			     the actual built in type.
			     But print a diagnostic for the mismatch.  */
			    if (TREE_CODE ((yyval.ttype)) != FUNCTION_DECL)
			      error ("`%s' implicitly declared as function",
				     IDENTIFIER_POINTER (DECL_NAME ((yyval.ttype))));
			  else if ((TYPE_MODE (TREE_TYPE (TREE_TYPE ((yyval.ttype))))
				    != TYPE_MODE (integer_type_node))
				   && (TREE_TYPE (TREE_TYPE ((yyval.ttype)))
				       != void_type_node))
			    pedwarn ("type mismatch in implicit declaration for built-in function `%s'",
				     IDENTIFIER_POINTER (DECL_NAME ((yyval.ttype))));
			  /* If it really returns void, change that to int.  */
			  if (TREE_TYPE (TREE_TYPE ((yyval.ttype))) == void_type_node)
			    TREE_TYPE ((yyval.ttype))
			      = build_function_type (integer_type_node,
						     TYPE_ARG_TYPES (TREE_TYPE ((yyval.ttype))));
			}
		      else
			pedwarn ("built-in function `%s' used without declaration",
				 IDENTIFIER_POINTER (DECL_NAME ((yyval.ttype))));

		      /* Do what we would ordinarily do when a fn is used.  */
		      assemble_external ((yyval.ttype));
		      TREE_USED ((yyval.ttype)) = 1;
		    }
		  else
		    {
		      assemble_external ((yyval.ttype));
		      TREE_USED ((yyval.ttype)) = 1;
		    }

		  if (TREE_CODE ((yyval.ttype)) == CONST_DECL)
		    {
		      (yyval.ttype) = DECL_INITIAL ((yyval.ttype));
		      /* This is to prevent an enum whose value is 0
			 from being considered a null pointer constant.  */
		      (yyval.ttype) = build1 (NOP_EXPR, TREE_TYPE ((yyval.ttype)), (yyval.ttype));
		      TREE_CONSTANT ((yyval.ttype)) = 1;
		    }
		;}
    break;

  case 89:

    { (yyval.ttype) = combine_strings ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 90:

    { char class = TREE_CODE_CLASS (TREE_CODE ((yyvsp[(2) - (3)].ttype)));
		  if (class == 'e' || class == '1'
		      || class == '2' || class == '<')
		    C_SET_EXP_ORIGINAL_CODE ((yyvsp[(2) - (3)].ttype), ERROR_MARK);
		  (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 91:

    { (yyval.ttype) = error_mark_node; ;}
    break;

  case 92:

    { if (current_function_decl == 0)
		    {
		      error ("braced-group within expression allowed only inside a function");
		      YYERROR;
		    }
		  /* We must force a BLOCK for this level
		     so that, if it is not expanded later,
		     there is a way to turn off the entire subtree of blocks
		     that are contained in it.  */
		  keep_next_level ();
		  push_iterator_stack ();
		  push_label_level ();
		  (yyval.ttype) = expand_start_stmt_expr (); ;}
    break;

  case 93:

    { tree rtl_exp;
		  if (pedantic)
		    pedwarn ("ANSI C forbids braced-groups within expressions");
		  pop_iterator_stack ();
		  pop_label_level ();
		  rtl_exp = expand_end_stmt_expr ((yyvsp[(2) - (4)].ttype));
		  /* The statements have side effects, so the group does.  */
		  TREE_SIDE_EFFECTS (rtl_exp) = 1;

		  if (TREE_CODE ((yyvsp[(3) - (4)].ttype)) == BLOCK)
		    {
		      /* Make a BIND_EXPR for the BLOCK already made.  */
		      (yyval.ttype) = build (BIND_EXPR, TREE_TYPE (rtl_exp),
				  NULL_TREE, rtl_exp, (yyvsp[(3) - (4)].ttype));
		      /* Remove the block from the tree at this point.
			 It gets put back at the proper place
			 when the BIND_EXPR is expanded.  */
		      delete_block ((yyvsp[(3) - (4)].ttype));
		    }
		  else
		    (yyval.ttype) = (yyvsp[(3) - (4)].ttype);
		;}
    break;

  case 94:

    { (yyval.ttype) = build_function_call ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 95:

    { (yyval.ttype) = build_array_ref ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 96:

    {
		    (yyval.ttype) = build_component_ref ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		;}
    break;

  case 97:

    {
                  tree expr = build_indirect_ref ((yyvsp[(1) - (3)].ttype), "->");

                    (yyval.ttype) = build_component_ref (expr, (yyvsp[(3) - (3)].ttype));
		;}
    break;

  case 98:

    { (yyval.ttype) = build_unary_op (POSTINCREMENT_EXPR, (yyvsp[(1) - (2)].ttype), 0); ;}
    break;

  case 99:

    { (yyval.ttype) = build_unary_op (POSTDECREMENT_EXPR, (yyvsp[(1) - (2)].ttype), 0); ;}
    break;

  case 101:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 104:

    { c_mark_varargs ();
		  if (pedantic)
		    pedwarn ("ANSI C does not permit use of `varargs.h'"); ;}
    break;

  case 105:

    { ;}
    break;

  case 110:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 111:

    { current_declspecs = TREE_VALUE (declspec_stack);	
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 112:

    { shadow_tag_warned ((yyvsp[(1) - (2)].ttype), 1);
		  pedwarn ("empty declaration"); ;}
    break;

  case 113:

    { pedwarn ("empty declaration"); ;}
    break;

  case 114:

    { ;}
    break;

  case 119:

    { (yyval.itype) = suspend_momentary ();
		  pending_xref_error ();
		  declspec_stack = tree_cons (prefix_attributes,
					      current_declspecs,
					      declspec_stack);
		  split_specs_attrs ((yyvsp[(0) - (0)].ttype),
				     &current_declspecs, &prefix_attributes); ;}
    break;

  case 120:

    { prefix_attributes = chainon (prefix_attributes, (yyvsp[(0) - (0)].ttype)); ;}
    break;

  case 121:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 122:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 123:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (3)].itype)); ;}
    break;

  case 124:

    { current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (3)].itype)); ;}
    break;

  case 125:

    { shadow_tag ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 126:

    { pedwarn ("empty declaration"); ;}
    break;

  case 127:

    { pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 128:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 129:

    { (yyval.ttype) = chainon ((yyvsp[(3) - (3)].ttype), tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype))); ;}
    break;

  case 130:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 131:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 132:

    { if (extra_warnings)
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 133:

    { (yyval.ttype) = tree_cons ((yyvsp[(2) - (2)].ttype), NULL_TREE, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 134:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 135:

    { (yyval.ttype) = chainon ((yyvsp[(3) - (3)].ttype), tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype))); ;}
    break;

  case 136:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 137:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 138:

    { if (extra_warnings)
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 139:

    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 140:

    { (yyval.ttype) = tree_cons ((yyvsp[(1) - (1)].ttype), NULL_TREE, NULL_TREE); ;}
    break;

  case 141:

    { (yyval.ttype) = chainon ((yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 142:

    { (yyval.ttype) = tree_cons ((yyvsp[(2) - (2)].ttype), NULL_TREE, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 143:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (1)].ttype), NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 1; ;}
    break;

  case 144:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (1)].ttype), NULL_TREE); ;}
    break;

  case 145:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; ;}
    break;

  case 146:

    { if (extra_warnings && TREE_STATIC ((yyvsp[(1) - (2)].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 147:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 148:

    { (yyval.ttype) = chainon ((yyvsp[(3) - (3)].ttype), tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype))); ;}
    break;

  case 149:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 150:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 153:

    { /* For a typedef name, record the meaning, not the name.
		     In case of `foo foo, bar;'.  */
		  (yyval.ttype) = lookup_name ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 154:

    { (yyval.ttype) = TREE_TYPE ((yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 155:

    { (yyval.ttype) = groktypename ((yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 163:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 164:

    { if (TREE_CHAIN ((yyvsp[(3) - (4)].ttype))) (yyvsp[(3) - (4)].ttype) = combine_strings ((yyvsp[(3) - (4)].ttype));
		  (yyval.ttype) = (yyvsp[(3) - (4)].ttype);
		;}
    break;

  case 165:

    { (yyval.ttype) = start_decl ((yyvsp[(1) - (4)].ttype), current_declspecs, 1,
					  (yyvsp[(3) - (4)].ttype), prefix_attributes);
		  start_init ((yyval.ttype), (yyvsp[(2) - (4)].ttype), global_bindings_p ()); ;}
    break;

  case 166:

    { finish_init ();
		  finish_decl ((yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype), (yyvsp[(2) - (6)].ttype)); ;}
    break;

  case 167:

    { tree d = start_decl ((yyvsp[(1) - (3)].ttype), current_declspecs, 0,
				       (yyvsp[(3) - (3)].ttype), prefix_attributes);
		  finish_decl (d, NULL_TREE, (yyvsp[(2) - (3)].ttype)); 
                ;}
    break;

  case 168:

    { (yyval.ttype) = start_decl ((yyvsp[(1) - (4)].ttype), current_declspecs, 1,
					  (yyvsp[(3) - (4)].ttype), prefix_attributes);
		  start_init ((yyval.ttype), (yyvsp[(2) - (4)].ttype), global_bindings_p ()); ;}
    break;

  case 169:

    { finish_init ();
		  decl_attributes ((yyvsp[(5) - (6)].ttype), (yyvsp[(3) - (6)].ttype), prefix_attributes);
		  finish_decl ((yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype), (yyvsp[(2) - (6)].ttype)); ;}
    break;

  case 170:

    { tree d = start_decl ((yyvsp[(1) - (3)].ttype), current_declspecs, 0,
				       (yyvsp[(3) - (3)].ttype), prefix_attributes);
		  finish_decl (d, NULL_TREE, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 171:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 172:

    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 173:

    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 174:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 175:

    { (yyval.ttype) = (yyvsp[(4) - (6)].ttype); ;}
    break;

  case 176:

    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 177:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 178:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 179:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (1)].ttype), NULL_TREE); ;}
    break;

  case 180:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (4)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (4)].ttype))); ;}
    break;

  case 181:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (6)].ttype), tree_cons (NULL_TREE, (yyvsp[(3) - (6)].ttype), (yyvsp[(5) - (6)].ttype))); ;}
    break;

  case 182:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 188:

    { really_start_incremental_init (NULL_TREE);
		  /* Note that the call to clear_momentary
		     is in process_init_element.  */
		  push_momentary (); ;}
    break;

  case 189:

    { (yyval.ttype) = pop_init_level (0);
		  if ((yyval.ttype) == error_mark_node
		      && ! (yychar == STRING || yychar == CONSTANT))
		    pop_momentary ();
		  else
		    pop_momentary_nofree (); ;}
    break;

  case 190:

    { (yyval.ttype) = error_mark_node; ;}
    break;

  case 191:

    { if (pedantic)
		    pedwarn ("ANSI C forbids empty initializer braces"); ;}
    break;

  case 197:

    { set_init_label ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 200:

    { push_init_level (0); ;}
    break;

  case 201:

    { process_init_element (pop_init_level (0)); ;}
    break;

  case 202:

    { process_init_element ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 206:

    { set_init_label ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 207:

    { set_init_index ((yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 208:

    { set_init_index ((yyvsp[(2) - (3)].ttype), NULL_TREE); ;}
    break;

  case 209:

    { push_c_function_context ();
		  if (! start_function (current_declspecs, (yyvsp[(1) - (1)].ttype),
					prefix_attributes, NULL_TREE, 1))
		    {
		      pop_c_function_context ();
		      YYERROR1;
		    }
		  reinit_parse_for_function (); ;}
    break;

  case 210:

    { store_parm_decls (); ;}
    break;

  case 211:

    { finish_function (1);
		  pop_c_function_context (); ;}
    break;

  case 212:

    { push_c_function_context ();
		  if (! start_function (current_declspecs, (yyvsp[(1) - (1)].ttype),
					prefix_attributes, NULL_TREE, 1))
		    {
		      pop_c_function_context ();
		      YYERROR1;
		    }
		  reinit_parse_for_function (); ;}
    break;

  case 213:

    { store_parm_decls (); ;}
    break;

  case 214:

    { finish_function (1);
		  pop_c_function_context (); ;}
    break;

  case 217:

    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 218:

    { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 219:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 220:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 221:

    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 222:

    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 224:

    { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 225:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), NULL_TREE);
		  if (! flag_isoc9x)
		    error ("`[*]' in parameter declaration only allowed in ISO C 9x");
		;}
    break;

  case 226:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 227:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 228:

    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 229:

    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 231:

    { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 232:

    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 233:

    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 234:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), NULL_TREE);
		  if (! flag_isoc9x)
		    error ("`[*]' in parameter declaration only allowed in ISO C 9x");
		;}
    break;

  case 235:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 236:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 237:

    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 239:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 240:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 241:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 242:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 243:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 244:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 245:

    { (yyval.ttype) = start_struct (RECORD_TYPE, (yyvsp[(2) - (3)].ttype));
		  /* Start scope of tag before parsing components.  */
		;}
    break;

  case 246:

    { (yyval.ttype) = finish_struct ((yyvsp[(4) - (7)].ttype), (yyvsp[(5) - (7)].ttype), chainon ((yyvsp[(1) - (7)].ttype), (yyvsp[(7) - (7)].ttype))); ;}
    break;

  case 247:

    { (yyval.ttype) = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
				      (yyvsp[(3) - (5)].ttype), chainon ((yyvsp[(1) - (5)].ttype), (yyvsp[(5) - (5)].ttype)));
		;}
    break;

  case 248:

    { (yyval.ttype) = xref_tag (RECORD_TYPE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 249:

    { (yyval.ttype) = start_struct (UNION_TYPE, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 250:

    { (yyval.ttype) = finish_struct ((yyvsp[(4) - (7)].ttype), (yyvsp[(5) - (7)].ttype), chainon ((yyvsp[(1) - (7)].ttype), (yyvsp[(7) - (7)].ttype))); ;}
    break;

  case 251:

    { (yyval.ttype) = finish_struct (start_struct (UNION_TYPE, NULL_TREE),
				      (yyvsp[(3) - (5)].ttype), chainon ((yyvsp[(1) - (5)].ttype), (yyvsp[(5) - (5)].ttype)));
		;}
    break;

  case 252:

    { (yyval.ttype) = xref_tag (UNION_TYPE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 253:

    { (yyvsp[(3) - (3)].itype) = suspend_momentary ();
		  (yyval.ttype) = start_enum ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 254:

    { (yyval.ttype)= finish_enum ((yyvsp[(4) - (8)].ttype), nreverse ((yyvsp[(5) - (8)].ttype)), chainon ((yyvsp[(1) - (8)].ttype), (yyvsp[(8) - (8)].ttype)));
		  resume_momentary ((yyvsp[(3) - (8)].itype)); ;}
    break;

  case 255:

    { (yyvsp[(2) - (2)].itype) = suspend_momentary ();
		  (yyval.ttype) = start_enum (NULL_TREE); ;}
    break;

  case 256:

    { (yyval.ttype)= finish_enum ((yyvsp[(3) - (7)].ttype), nreverse ((yyvsp[(4) - (7)].ttype)), chainon ((yyvsp[(1) - (7)].ttype), (yyvsp[(7) - (7)].ttype)));
		  resume_momentary ((yyvsp[(2) - (7)].itype)); ;}
    break;

  case 257:

    { (yyval.ttype) = xref_tag (ENUMERAL_TYPE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 261:

    { if (pedantic && ! flag_isoc9x)
		    pedwarn ("comma at end of enumerator list"); ;}
    break;

  case 262:

    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 263:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype));
		  pedwarn ("no semicolon at end of struct or union"); ;}
    break;

  case 264:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 265:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 266:

    { if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified"); ;}
    break;

  case 267:

    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype);
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (3)].itype)); ;}
    break;

  case 268:

    { if (pedantic)
		    pedwarn ("ANSI C forbids member declarations with no members");
		  shadow_tag((yyvsp[(1) - (1)].ttype));
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 269:

    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype);
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (3)].itype)); ;}
    break;

  case 270:

    { if (pedantic)
		    pedwarn ("ANSI C forbids member declarations with no members");
		  shadow_tag((yyvsp[(1) - (1)].ttype));
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 271:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 272:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 274:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 275:

    { (yyval.ttype) = grokfield ((yyvsp[(1) - (4)].filename), (yyvsp[(2) - (4)].lineno), (yyvsp[(3) - (4)].ttype), current_declspecs, NULL_TREE);
		  decl_attributes ((yyval.ttype), (yyvsp[(4) - (4)].ttype), prefix_attributes); ;}
    break;

  case 276:

    { (yyval.ttype) = grokfield ((yyvsp[(1) - (6)].filename), (yyvsp[(2) - (6)].lineno), (yyvsp[(3) - (6)].ttype), current_declspecs, (yyvsp[(5) - (6)].ttype));
		  decl_attributes ((yyval.ttype), (yyvsp[(6) - (6)].ttype), prefix_attributes); ;}
    break;

  case 277:

    { (yyval.ttype) = grokfield ((yyvsp[(1) - (5)].filename), (yyvsp[(2) - (5)].lineno), NULL_TREE, current_declspecs, (yyvsp[(4) - (5)].ttype));
		  decl_attributes ((yyval.ttype), (yyvsp[(5) - (5)].ttype), prefix_attributes); ;}
    break;

  case 279:

    { if ((yyvsp[(1) - (3)].ttype) == error_mark_node)
		    (yyval.ttype) = (yyvsp[(1) - (3)].ttype);
		  else
		    (yyval.ttype) = chainon ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ttype)); ;}
    break;

  case 280:

    { (yyval.ttype) = error_mark_node; ;}
    break;

  case 281:

    { (yyval.ttype) = build_enumerator ((yyvsp[(1) - (1)].ttype), NULL_TREE); ;}
    break;

  case 282:

    { (yyval.ttype) = build_enumerator ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 283:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 284:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 285:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 287:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(1) - (1)].ttype), NULL_TREE); ;}
    break;

  case 288:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 289:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 290:

    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 291:

    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 292:

    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 293:

    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (2)].ttype), NULL_TREE); ;}
    break;

  case 294:

    { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 295:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 296:

    { (yyval.ttype) = build_nt (ARRAY_REF, (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 297:

    { (yyval.ttype) = build_nt (CALL_EXPR, NULL_TREE, (yyvsp[(2) - (2)].ttype), NULL_TREE); ;}
    break;

  case 298:

    { (yyval.ttype) = build_nt (ARRAY_REF, NULL_TREE, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 299:

    { (yyval.ttype) = build_nt (ARRAY_REF, NULL_TREE, NULL_TREE); ;}
    break;

  case 300:

    {
		  if (pedantic && (yyvsp[(1) - (1)].ends_in_label))
		    pedwarn ("ANSI C forbids label at end of compound statement");
		;}
    break;

  case 302:

    { (yyval.ends_in_label) = (yyvsp[(2) - (2)].ends_in_label); ;}
    break;

  case 303:

    { (yyval.ends_in_label) = 0; ;}
    break;

  case 307:

    { emit_line_note (input_filename, lineno);
		  pushlevel (0);
		  clear_last_expr ();
		  push_momentary ();
		  expand_start_bindings (0);
		;}
    break;

  case 309:

    { if (pedantic)
		    pedwarn ("ANSI C forbids label declarations"); ;}
    break;

  case 312:

    { tree link;
		  for (link = (yyvsp[(2) - (3)].ttype); link; link = TREE_CHAIN (link))
		    {
		      tree label = shadow_label (TREE_VALUE (link));
		      C_DECLARED_LABEL_FLAG (label) = 1;
		      declare_nonlocal_label (label);
		    }
		;}
    break;

  case 313:

    {;}
    break;

  case 315:

    { compstmt_count++; ;}
    break;

  case 316:

    { (yyval.ttype) = convert (void_type_node, integer_zero_node); ;}
    break;

  case 317:

    { emit_line_note (input_filename, lineno);
		  expand_end_bindings (getdecls (), 1, 0);
		  (yyval.ttype) = poplevel (1, 1, 0);
		  if (yychar == CONSTANT || yychar == STRING)
		    pop_momentary_nofree ();
		  else
		    pop_momentary (); ;}
    break;

  case 318:

    { emit_line_note (input_filename, lineno);
		  expand_end_bindings (getdecls (), kept_level_p (), 0);
		  (yyval.ttype) = poplevel (kept_level_p (), 0, 0);
		  if (yychar == CONSTANT || yychar == STRING)
		    pop_momentary_nofree ();
		  else
		    pop_momentary (); ;}
    break;

  case 319:

    { emit_line_note (input_filename, lineno);
		  expand_end_bindings (getdecls (), kept_level_p (), 0);
		  (yyval.ttype) = poplevel (kept_level_p (), 0, 0);
		  if (yychar == CONSTANT || yychar == STRING)
		    pop_momentary_nofree ();
		  else
		    pop_momentary (); ;}
    break;

  case 322:

    { emit_line_note ((yyvsp[(-1) - (4)].filename), (yyvsp[(0) - (4)].lineno));
		  c_expand_start_cond (truthvalue_conversion ((yyvsp[(3) - (4)].ttype)), 0, 
				       compstmt_count);
		  (yyval.itype) = stmt_count;
		  if_stmt_file = (yyvsp[(-1) - (4)].filename);
		  if_stmt_line = (yyvsp[(0) - (4)].lineno);
		  position_after_white_space (); ;}
    break;

  case 323:

    { stmt_count++;
		  compstmt_count++;
		  emit_line_note ((yyvsp[(-1) - (1)].filename), (yyvsp[(0) - (1)].lineno));
		  /* See comment in `while' alternative, above.  */
		  emit_nop ();
		  expand_start_loop_continue_elsewhere (1);
		  position_after_white_space (); ;}
    break;

  case 324:

    { expand_loop_continue_here (); ;}
    break;

  case 325:

    { (yyval.filename) = input_filename; ;}
    break;

  case 326:

    { (yyval.lineno) = lineno; ;}
    break;

  case 327:

    { ;}
    break;

  case 328:

    { ;}
    break;

  case 329:

    { (yyval.ends_in_label) = (yyvsp[(3) - (3)].ends_in_label); ;}
    break;

  case 330:

    { (yyval.ends_in_label) = 0; ;}
    break;

  case 331:

    { (yyval.ends_in_label) = 1; ;}
    break;

  case 332:

    { stmt_count++; ;}
    break;

  case 334:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (2)].filename), (yyvsp[(0) - (2)].lineno));
/* It appears that this should not be done--that a non-lvalue array
   shouldn't get an error if the value isn't used.
   Section 3.2.2.1 says that an array lvalue gets converted to a pointer
   if it appears as a top-level expression,
   but says nothing about non-lvalue arrays.  */
#if 0
		  /* Call default_conversion to get an error
		     on referring to a register array if pedantic.  */
		  if (TREE_CODE (TREE_TYPE ((yyvsp[(1) - (2)].ttype))) == ARRAY_TYPE
		      || TREE_CODE (TREE_TYPE ((yyvsp[(1) - (2)].ttype))) == FUNCTION_TYPE)
		    (yyvsp[(1) - (2)].ttype) = default_conversion ((yyvsp[(1) - (2)].ttype));
#endif
		  iterator_expand ((yyvsp[(1) - (2)].ttype));
		  clear_momentary (); ;}
    break;

  case 335:

    { c_expand_start_else ();
		  (yyvsp[(1) - (2)].itype) = stmt_count;
		  position_after_white_space (); ;}
    break;

  case 336:

    { c_expand_end_cond ();
		  if (extra_warnings && stmt_count == (yyvsp[(1) - (4)].itype))
		    warning ("empty body in an else-statement"); ;}
    break;

  case 337:

    { c_expand_end_cond ();
		  /* This warning is here instead of in simple_if, because we
		     do not want a warning if an empty if is followed by an
		     else statement.  Increment stmt_count so we don't
		     give a second error if this is a nested `if'.  */
		  if (extra_warnings && stmt_count++ == (yyvsp[(1) - (1)].itype))
		    warning_with_file_and_line (if_stmt_file, if_stmt_line,
						"empty body in an if-statement"); ;}
    break;

  case 338:

    { c_expand_end_cond (); ;}
    break;

  case 339:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (1)].filename), (yyvsp[(0) - (1)].lineno));
		  /* The emit_nop used to come before emit_line_note,
		     but that made the nop seem like part of the preceding line.
		     And that was confusing when the preceding line was
		     inside of an if statement and was not really executed.
		     I think it ought to work to put the nop after the line number.
		     We will see.  --rms, July 15, 1991.  */
		  emit_nop (); ;}
    break;

  case 340:

    { /* Don't start the loop till we have succeeded
		     in parsing the end test.  This is to make sure
		     that we end every loop we start.  */
		  expand_start_loop (1);
		  emit_line_note (input_filename, lineno);
		  expand_exit_loop_if_false (NULL,
					     truthvalue_conversion ((yyvsp[(4) - (5)].ttype)));
		  position_after_white_space (); ;}
    break;

  case 341:

    { expand_end_loop (); ;}
    break;

  case 342:

    { emit_line_note (input_filename, lineno);
		  expand_exit_loop_if_false (NULL,
					     truthvalue_conversion ((yyvsp[(3) - (5)].ttype)));
		  expand_end_loop ();
		  clear_momentary (); ;}
    break;

  case 343:

    { expand_end_loop ();
		  clear_momentary (); ;}
    break;

  case 344:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (4)].filename), (yyvsp[(0) - (4)].lineno));
		  /* See comment in `while' alternative, above.  */
		  emit_nop ();
		  if ((yyvsp[(3) - (4)].ttype)) c_expand_expr_stmt ((yyvsp[(3) - (4)].ttype));
		  /* Next step is to call expand_start_loop_continue_elsewhere,
		     but wait till after we parse the entire for (...).
		     Otherwise, invalid input might cause us to call that
		     fn without calling expand_end_loop.  */
		;}
    break;

  case 345:

    { (yyvsp[(7) - (7)].lineno) = lineno;
		  (yyval.filename) = input_filename; ;}
    break;

  case 346:

    { 
		  /* Start the loop.  Doing this after parsing
		     all the expressions ensures we will end the loop.  */
		  expand_start_loop_continue_elsewhere (1);
		  /* Emit the end-test, with a line number.  */
		  emit_line_note ((yyvsp[(8) - (10)].filename), (yyvsp[(7) - (10)].lineno));
		  if ((yyvsp[(6) - (10)].ttype))
		    expand_exit_loop_if_false (NULL,
					       truthvalue_conversion ((yyvsp[(6) - (10)].ttype)));
		  /* Don't let the tree nodes for $9 be discarded by
		     clear_momentary during the parsing of the next stmt.  */
		  push_momentary ();
		  (yyvsp[(7) - (10)].lineno) = lineno;
		  (yyvsp[(8) - (10)].filename) = input_filename;
		  position_after_white_space (); ;}
    break;

  case 347:

    { /* Emit the increment expression, with a line number.  */
		  emit_line_note ((yyvsp[(8) - (12)].filename), (yyvsp[(7) - (12)].lineno));
		  expand_loop_continue_here ();
		  if ((yyvsp[(9) - (12)].ttype))
		    c_expand_expr_stmt ((yyvsp[(9) - (12)].ttype));
		  if (yychar == CONSTANT || yychar == STRING)
		    pop_momentary_nofree ();
		  else
		    pop_momentary ();
		  expand_end_loop (); ;}
    break;

  case 348:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (4)].filename), (yyvsp[(0) - (4)].lineno));
		  c_expand_start_case ((yyvsp[(3) - (4)].ttype));
		  /* Don't let the tree nodes for $3 be discarded by
		     clear_momentary during the parsing of the next stmt.  */
		  push_momentary ();
		  position_after_white_space (); ;}
    break;

  case 349:

    { expand_end_case ((yyvsp[(3) - (6)].ttype));
		  if (yychar == CONSTANT || yychar == STRING)
		    pop_momentary_nofree ();
		  else
		    pop_momentary (); ;}
    break;

  case 350:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (2)].filename), (yyvsp[(0) - (2)].lineno));
		  if ( ! expand_exit_something ())
		    error ("break statement not within loop or switch"); ;}
    break;

  case 351:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (2)].filename), (yyvsp[(0) - (2)].lineno));
		  if (! expand_continue_loop (NULL))
		    error ("continue statement not within a loop"); ;}
    break;

  case 352:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (2)].filename), (yyvsp[(0) - (2)].lineno));
		  c_expand_return (NULL_TREE); ;}
    break;

  case 353:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (3)].filename), (yyvsp[(0) - (3)].lineno));
		  c_expand_return ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 354:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (6)].filename), (yyvsp[(0) - (6)].lineno));
		  STRIP_NOPS ((yyvsp[(4) - (6)].ttype));
		  if ((TREE_CODE ((yyvsp[(4) - (6)].ttype)) == ADDR_EXPR
		       && TREE_CODE (TREE_OPERAND ((yyvsp[(4) - (6)].ttype), 0)) == STRING_CST)
		      || TREE_CODE ((yyvsp[(4) - (6)].ttype)) == STRING_CST)
		    expand_asm ((yyvsp[(4) - (6)].ttype));
		  else
		    error ("argument of `asm' is not a constant string"); ;}
    break;

  case 355:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (8)].filename), (yyvsp[(0) - (8)].lineno));
		  c_expand_asm_operands ((yyvsp[(4) - (8)].ttype), (yyvsp[(6) - (8)].ttype), NULL_TREE, NULL_TREE,
					 (yyvsp[(2) - (8)].ttype) == ridpointers[(int)RID_VOLATILE],
					 input_filename, lineno); ;}
    break;

  case 356:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (10)].filename), (yyvsp[(0) - (10)].lineno));
		  c_expand_asm_operands ((yyvsp[(4) - (10)].ttype), (yyvsp[(6) - (10)].ttype), (yyvsp[(8) - (10)].ttype), NULL_TREE,
					 (yyvsp[(2) - (10)].ttype) == ridpointers[(int)RID_VOLATILE],
					 input_filename, lineno); ;}
    break;

  case 357:

    { stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (12)].filename), (yyvsp[(0) - (12)].lineno));
		  c_expand_asm_operands ((yyvsp[(4) - (12)].ttype), (yyvsp[(6) - (12)].ttype), (yyvsp[(8) - (12)].ttype), (yyvsp[(10) - (12)].ttype),
					 (yyvsp[(2) - (12)].ttype) == ridpointers[(int)RID_VOLATILE],
					 input_filename, lineno); ;}
    break;

  case 358:

    { tree decl;
		  stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (3)].filename), (yyvsp[(0) - (3)].lineno));
		  decl = lookup_label ((yyvsp[(2) - (3)].ttype));
		  if (decl != 0)
		    {
		      TREE_USED (decl) = 1;
		      expand_goto (decl);
		    }
		;}
    break;

  case 359:

    { if (pedantic)
		    pedwarn ("ANSI C forbids `goto *expr;'");
		  stmt_count++;
		  emit_line_note ((yyvsp[(-1) - (4)].filename), (yyvsp[(0) - (4)].lineno));
		  expand_computed_goto (convert (ptr_type_node, (yyvsp[(3) - (4)].ttype))); ;}
    break;

  case 362:

    {
	    /* The value returned by this action is  */
	    /*      1 if everything is OK */ 
	    /*      0 in case of error or already bound iterator */

	    (yyval.itype) = 0;
	    if (TREE_CODE ((yyvsp[(3) - (4)].ttype)) != VAR_DECL)
	      error ("invalid `for (ITERATOR)' syntax");
	    else if (! ITERATOR_P ((yyvsp[(3) - (4)].ttype)))
	      error ("`%s' is not an iterator",
		     IDENTIFIER_POINTER (DECL_NAME ((yyvsp[(3) - (4)].ttype))));
	    else if (ITERATOR_BOUND_P ((yyvsp[(3) - (4)].ttype)))
	      error ("`for (%s)' inside expansion of same iterator",
		     IDENTIFIER_POINTER (DECL_NAME ((yyvsp[(3) - (4)].ttype))));
	    else
	      {
		(yyval.itype) = 1;
		iterator_for_loop_start ((yyvsp[(3) - (4)].ttype));
	      }
	  ;}
    break;

  case 363:

    {
	    if ((yyvsp[(5) - (6)].itype))
	      iterator_for_loop_end ((yyvsp[(3) - (6)].ttype));
	  ;}
    break;

  case 364:

    { register tree value = check_case_value ((yyvsp[(2) - (3)].ttype));
		  register tree label
		    = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);

		  stmt_count++;

		  if (value != error_mark_node)
		    {
		      tree duplicate;
		      int success;

		      if (pedantic && ! INTEGRAL_TYPE_P (TREE_TYPE (value)))
			pedwarn ("label must have integral type in ANSI C");

		      success = pushcase (value, convert_and_check,
					  label, &duplicate);

		      if (success == 1)
			error ("case label not within a switch statement");
		      else if (success == 2)
			{
			  error ("duplicate case value");
			  error_with_decl (duplicate, "this is the first entry for that value");
			}
		      else if (success == 3)
			warning ("case value out of range");
		      else if (success == 5)
			error ("case label within scope of cleanup or variable array");
		    }
		  position_after_white_space (); ;}
    break;

  case 365:

    { register tree value1 = check_case_value ((yyvsp[(2) - (5)].ttype));
		  register tree value2 = check_case_value ((yyvsp[(4) - (5)].ttype));
		  register tree label
		    = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);

		  if (pedantic)
		    pedwarn ("ANSI C forbids case ranges");
		  stmt_count++;

		  if (value1 != error_mark_node && value2 != error_mark_node)
		    {
		      tree duplicate;
		      int success = pushcase_range (value1, value2,
						    convert_and_check, label,
						    &duplicate);
		      if (success == 1)
			error ("case label not within a switch statement");
		      else if (success == 2)
			{
			  error ("duplicate case value");
			  error_with_decl (duplicate, "this is the first entry for that value");
			}
		      else if (success == 3)
			warning ("case value out of range");
		      else if (success == 4)
			warning ("empty case range");
		      else if (success == 5)
			error ("case label within scope of cleanup or variable array");
		    }
		  position_after_white_space (); ;}
    break;

  case 366:

    {
		  tree duplicate;
		  register tree label
		    = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
		  int success = pushcase (NULL_TREE, 0, label, &duplicate);
		  stmt_count++;
		  if (success == 1)
		    error ("default label not within a switch statement");
		  else if (success == 2)
		    {
		      error ("multiple default labels in one switch");
		      error_with_decl (duplicate, "this is the first default label");
		    }
		  position_after_white_space (); ;}
    break;

  case 367:

    { tree label = define_label (input_filename, lineno, (yyvsp[(1) - (3)].ttype));
		  stmt_count++;
		  emit_nop ();
		  if (label)
		    {
		      expand_label (label);
		      decl_attributes (label, (yyvsp[(3) - (3)].ttype), NULL_TREE);
		    }
		  position_after_white_space (); ;}
    break;

  case 368:

    { emit_line_note (input_filename, lineno);
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 369:

    { emit_line_note (input_filename, lineno); ;}
    break;

  case 370:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 372:

    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 375:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 376:

    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 377:

    { (yyval.ttype) = tree_cons (NULL_TREE, combine_strings ((yyvsp[(1) - (1)].ttype)), NULL_TREE); ;}
    break;

  case 378:

    { (yyval.ttype) = tree_cons (NULL_TREE, combine_strings ((yyvsp[(3) - (3)].ttype)), (yyvsp[(1) - (3)].ttype)); ;}
    break;

  case 379:

    { pushlevel (0);
		  clear_parm_order ();
		  declare_parm_level (0); ;}
    break;

  case 380:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  parmlist_tags_warning ();
		  poplevel (0, 0, 0); ;}
    break;

  case 382:

    { tree parm;
		  if (pedantic)
		    pedwarn ("ANSI C forbids forward parameter declarations");
		  /* Mark the forward decls as such.  */
		  for (parm = getdecls (); parm; parm = TREE_CHAIN (parm))
		    TREE_ASM_WRITTEN (parm) = 1;
		  clear_parm_order (); ;}
    break;

  case 383:

    { (yyval.ttype) = (yyvsp[(4) - (4)].ttype); ;}
    break;

  case 384:

    { (yyval.ttype) = tree_cons (NULL_TREE, NULL_TREE, NULL_TREE); ;}
    break;

  case 385:

    { (yyval.ttype) = get_parm_info (0); ;}
    break;

  case 386:

    { (yyval.ttype) = get_parm_info (0);
		  /* Gcc used to allow this as an extension.  However, it does
		     not work for all targets, and thus has been disabled.
		     Also, since func (...) and func () are indistinguishable,
		     it caused problems with the code in expand_builtin which
		     tries to verify that BUILT_IN_NEXT_ARG is being used
		     correctly.  */
		  error ("ANSI C requires a named argument before `...'");
		;}
    break;

  case 387:

    { (yyval.ttype) = get_parm_info (1); ;}
    break;

  case 388:

    { (yyval.ttype) = get_parm_info (0); ;}
    break;

  case 389:

    { push_parm_decl ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 390:

    { push_parm_decl ((yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 391:

    { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[(3) - (4)].ttype)),
					build_tree_list (prefix_attributes,
							 (yyvsp[(4) - (4)].ttype)));
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 392:

    { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[(3) - (4)].ttype)),
					build_tree_list (prefix_attributes,
							 (yyvsp[(4) - (4)].ttype))); 
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 393:

    { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[(3) - (4)].ttype)),
					build_tree_list (prefix_attributes,
							 (yyvsp[(4) - (4)].ttype)));
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype)); ;}
    break;

  case 394:

    { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[(3) - (4)].ttype)),
					build_tree_list (prefix_attributes,
							 (yyvsp[(4) - (4)].ttype)));
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype));  ;}
    break;

  case 395:

    { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[(3) - (4)].ttype)),
					build_tree_list (prefix_attributes,
							 (yyvsp[(4) - (4)].ttype)));
		  current_declspecs = TREE_VALUE (declspec_stack);
		  prefix_attributes = TREE_PURPOSE (declspec_stack);
		  declspec_stack = TREE_CHAIN (declspec_stack);
		  resume_momentary ((yyvsp[(2) - (4)].itype));  ;}
    break;

  case 396:

    { pushlevel (0);
		  clear_parm_order ();
		  declare_parm_level (1); ;}
    break;

  case 397:

    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  parmlist_tags_warning ();
		  poplevel (0, 0, 0); ;}
    break;

  case 399:

    { tree t;
		  for (t = (yyvsp[(1) - (2)].ttype); t; t = TREE_CHAIN (t))
		    if (TREE_VALUE (t) == NULL_TREE)
		      error ("`...' in old-style identifier list");
		  (yyval.ttype) = tree_cons (NULL_TREE, NULL_TREE, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 400:

    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 401:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 402:

    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 403:

    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 404:

    { (yyval.itype) = pedantic;
		  pedantic = 0; ;}
    break;


/* Line 1267 of yacc.c.  */

      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}





