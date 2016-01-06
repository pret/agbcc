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
     SELFNAME = 260,
     PFUNCNAME = 261,
     SCSPEC = 262,
     TYPESPEC = 263,
     CV_QUALIFIER = 264,
     CONSTANT = 265,
     STRING = 266,
     ELLIPSIS = 267,
     SIZEOF = 268,
     ENUM = 269,
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
     SIGOF = 285,
     ATTRIBUTE = 286,
     EXTENSION = 287,
     LABEL = 288,
     REALPART = 289,
     IMAGPART = 290,
     AGGR = 291,
     VISSPEC = 292,
     DELETE = 293,
     NEW = 294,
     THIS = 295,
     OPERATOR = 296,
     CXX_TRUE = 297,
     CXX_FALSE = 298,
     NAMESPACE = 299,
     TYPENAME_KEYWORD = 300,
     USING = 301,
     LEFT_RIGHT = 302,
     TEMPLATE = 303,
     TYPEID = 304,
     DYNAMIC_CAST = 305,
     STATIC_CAST = 306,
     REINTERPRET_CAST = 307,
     CONST_CAST = 308,
     SCOPE = 309,
     EMPTY = 310,
     NSNAME = 311,
     PTYPENAME = 312,
     THROW = 313,
     ASSIGN = 314,
     OROR = 315,
     ANDAND = 316,
     MIN_MAX = 317,
     EQCOMPARE = 318,
     ARITHCOMPARE = 319,
     RSHIFT = 320,
     LSHIFT = 321,
     DOT_STAR = 322,
     POINTSAT_STAR = 323,
     MINUSMINUS = 324,
     PLUSPLUS = 325,
     UNARY = 326,
     HYPERUNARY = 327,
     PAREN_STAR_PAREN = 328,
     POINTSAT = 329,
     CATCH = 330,
     TRY = 331,
     PRE_PARSED_FUNCTION_DECL = 332,
     EXTERN_LANG_STRING = 333,
     ALL = 334,
     PRE_PARSED_CLASS_DECL = 335,
     DEFARG = 336,
     DEFARG_MARKER = 337,
     TYPENAME_DEFN = 338,
     IDENTIFIER_DEFN = 339,
     PTYPENAME_DEFN = 340,
     END_OF_LINE = 341,
     END_OF_SAVED_INPUT = 342
   };
#endif
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




/* Copy the first part of user declarations.  */
#line 29 "../../../gbagnu/gcc/cp/parse.y"

/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG 1

#include "config.h"

#include "system.h"

#include "tree.h"
#include "input.h"
#include "flags.h"
#include "lex.h"
#include "cp-tree.h"
#include "output.h"
#include "except.h"
#include "toplev.h"

/* Since parsers are distinct for each language, put the language string
   definition here.  (fnf) */
char *language_string = "GNU C++";

extern tree void_list_node;
extern struct obstack permanent_obstack;

extern int end_of_file;

/* Like YYERROR but do call yyerror.  */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

#define OP0(NODE) (TREE_OPERAND (NODE, 0))
#define OP1(NODE) (TREE_OPERAND (NODE, 1))

/* Contains the statement keyword (if/while/do) to include in an
   error message if the user supplies an empty conditional expression.  */
static char *cond_stmt_keyword;

static tree empty_parms PROTO((void));

/* Nonzero if we have an `extern "C"' acting as an extern specifier.  */
int have_extern_spec;
int used_extern_spec;

/* Cons up an empty parameter list.  */
#ifdef __GNUC__
__inline
#endif
static tree
empty_parms ()
{
  tree parms;

  if (strict_prototype
      || current_class_type != NULL)
    parms = void_list_node;
  else
    parms = NULL_TREE;
  return parms;
}



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
#line 92 "../../../gbagnu/gcc/cp/parse.y"
{long itype; tree ttype; char *strtype; enum tree_code code; flagged_type_tree ftype; }
/* Line 187 of yacc.c.  */
#line 333 "parse.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 284 "../../../gbagnu/gcc/cp/parse.y"

/* List of types and structure classes of the current declaration.  */
static tree current_declspecs;

/* List of prefix attributes in effect.
   Prefix attributes are parsed by the reserved_declspecs and declmods
   rules.  They create a list that contains *both* declspecs and attrs.  */
/* ??? It is not clear yet that all cases where an attribute can now appear in
   a declspec list have been updated.  */
static tree prefix_attributes;

/* When defining an aggregate, this is the kind of the most recent one
   being defined.  (For example, this might be class_type_node.)  */
static tree current_aggr;

/* When defining an enumeration, this is the type of the enumeration.  */
static tree current_enum_type;

/* Tell yyparse how to print a token's value, if yydebug is set.  */

#define YYPRINT(FILE,YYCHAR,YYLVAL) yyprint(FILE,YYCHAR,YYLVAL)
extern void yyprint			PROTO((FILE *, int, YYSTYPE));
extern tree combine_strings		PROTO((tree));

static int
parse_decl(declarator, specs_attrs, attributes, initialized, decl)
  tree declarator;
  tree specs_attrs;
  tree attributes;
  int initialized;
  tree* decl;
{
  int  sm;

  split_specs_attrs (specs_attrs, &current_declspecs, &prefix_attributes);
  if (current_declspecs
      && TREE_CODE (current_declspecs) != TREE_LIST)
    current_declspecs = get_decl_list (current_declspecs);
  if (have_extern_spec && !used_extern_spec)
    {
      current_declspecs = decl_tree_cons (NULL_TREE, 
					  get_identifier ("extern"), 
					  current_declspecs);
      used_extern_spec = 1;
    }
  sm = suspend_momentary ();
  *decl = start_decl (declarator, current_declspecs, initialized,
		      attributes, prefix_attributes);
  return sm;
}


/* Line 216 of yacc.c.  */
#line 397 "parse.c"

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
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
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
#define YYLAST   11289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  112
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  289
/* YYNRULES -- Number of rules.  */
#define YYNRULES  879
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1630

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   342

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   110,     2,     2,     2,    82,    70,     2,
      92,   108,    80,    78,    59,    79,    91,    81,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    62,    60,
      73,    63,    74,    65,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    93,     2,   111,    69,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,    68,   109,    85,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    61,    64,    66,    67,    71,    72,    75,
      76,    77,    83,    84,    86,    87,    88,    89,    90,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     7,    10,    13,    15,    16,
      17,    18,    20,    22,    23,    26,    29,    31,    33,    39,
      44,    50,    55,    56,    63,    64,    70,    72,    75,    77,
      80,    81,    88,    91,    95,    99,   103,   107,   112,   113,
     119,   122,   126,   128,   130,   133,   136,   138,   141,   142,
     148,   152,   154,   158,   160,   161,   164,   167,   171,   173,
     177,   179,   183,   185,   189,   192,   195,   198,   200,   202,
     208,   213,   216,   219,   223,   227,   230,   233,   237,   241,
     244,   247,   250,   253,   256,   258,   260,   262,   263,   265,
     268,   269,   271,   276,   280,   284,   285,   294,   300,   301,
     311,   318,   319,   328,   334,   335,   345,   352,   355,   358,
     360,   363,   365,   372,   377,   384,   389,   392,   394,   397,
     400,   402,   405,   407,   410,   413,   418,   421,   425,   426,
     427,   429,   433,   436,   440,   442,   447,   450,   455,   458,
     463,   466,   468,   470,   472,   474,   476,   478,   480,   482,
     484,   486,   488,   489,   496,   497,   504,   505,   511,   512,
     518,   519,   527,   528,   536,   537,   544,   545,   552,   553,
     554,   560,   566,   568,   570,   576,   582,   583,   585,   587,
     588,   590,   592,   596,   598,   600,   602,   604,   606,   608,
     610,   612,   614,   616,   618,   622,   624,   628,   629,   631,
     633,   634,   642,   644,   646,   650,   655,   659,   660,   664,
     666,   670,   674,   678,   682,   684,   686,   688,   691,   694,
     697,   700,   703,   706,   709,   714,   717,   722,   725,   729,
     733,   738,   744,   751,   758,   766,   769,   774,   780,   783,
     786,   788,   789,   794,   799,   803,   805,   809,   812,   816,
     821,   823,   826,   832,   834,   838,   842,   846,   850,   854,
     858,   862,   866,   870,   874,   878,   882,   886,   890,   894,
     898,   902,   906,   910,   916,   920,   924,   926,   929,   933,
     937,   939,   941,   943,   945,   947,   948,   954,   960,   966,
     972,   978,   980,   982,   984,   986,   989,   991,   994,   997,
    1001,  1006,  1011,  1013,  1015,  1017,  1021,  1023,  1025,  1027,
    1029,  1033,  1037,  1041,  1042,  1047,  1052,  1055,  1060,  1063,
    1068,  1071,  1074,  1076,  1081,  1083,  1091,  1099,  1107,  1115,
    1120,  1125,  1128,  1131,  1134,  1136,  1141,  1144,  1147,  1153,
    1157,  1160,  1163,  1169,  1173,  1179,  1183,  1188,  1195,  1198,
    1200,  1203,  1205,  1208,  1210,  1212,  1214,  1217,  1218,  1221,
    1224,  1228,  1232,  1236,  1239,  1242,  1245,  1247,  1249,  1251,
    1254,  1257,  1260,  1263,  1265,  1267,  1269,  1271,  1274,  1277,
    1281,  1285,  1289,  1294,  1296,  1299,  1302,  1305,  1307,  1309,
    1311,  1314,  1317,  1320,  1322,  1324,  1327,  1330,  1334,  1336,
    1339,  1341,  1343,  1345,  1350,  1355,  1360,  1365,  1367,  1369,
    1371,  1373,  1377,  1379,  1383,  1385,  1389,  1390,  1395,  1396,
    1403,  1407,  1408,  1413,  1415,  1419,  1423,  1424,  1429,  1433,
    1434,  1436,  1438,  1441,  1448,  1450,  1454,  1455,  1457,  1462,
    1469,  1474,  1476,  1478,  1480,  1482,  1484,  1488,  1489,  1492,
    1494,  1497,  1501,  1506,  1508,  1510,  1514,  1519,  1523,  1529,
    1531,  1536,  1540,  1544,  1545,  1549,  1553,  1557,  1558,  1561,
    1564,  1565,  1573,  1578,  1579,  1586,  1590,  1593,  1596,  1599,
    1600,  1601,  1611,  1613,  1614,  1616,  1617,  1619,  1621,  1624,
    1627,  1630,  1633,  1636,  1639,  1642,  1645,  1648,  1652,  1657,
    1661,  1664,  1668,  1670,  1671,  1675,  1676,  1680,  1683,  1685,
    1687,  1688,  1691,  1695,  1697,  1702,  1704,  1708,  1710,  1712,
    1717,  1722,  1725,  1728,  1732,  1736,  1738,  1739,  1741,  1744,
    1748,  1751,  1754,  1756,  1759,  1762,  1765,  1768,  1771,  1774,
    1777,  1779,  1782,  1785,  1789,  1792,  1795,  1800,  1805,  1808,
    1810,  1816,  1821,  1823,  1824,  1826,  1830,  1831,  1833,  1837,
    1839,  1841,  1843,  1845,  1850,  1855,  1860,  1865,  1870,  1874,
    1879,  1884,  1889,  1894,  1898,  1900,  1904,  1906,  1910,  1913,
    1915,  1923,  1924,  1927,  1929,  1932,  1933,  1936,  1941,  1946,
    1949,  1954,  1958,  1962,  1965,  1968,  1972,  1974,  1976,  1979,
    1981,  1983,  1986,  1989,  1994,  1999,  2003,  2007,  2010,  2012,
    2014,  2017,  2021,  2025,  2028,  2031,  2035,  2037,  2041,  2045,
    2048,  2051,  2055,  2057,  2062,  2066,  2071,  2075,  2077,  2080,
    2083,  2086,  2089,  2092,  2094,  2097,  2102,  2107,  2110,  2112,
    2114,  2116,  2118,  2121,  2126,  2129,  2132,  2135,  2138,  2140,
    2143,  2146,  2149,  2152,  2156,  2158,  2161,  2165,  2170,  2173,
    2176,  2179,  2182,  2185,  2188,  2193,  2196,  2198,  2201,  2204,
    2208,  2210,  2214,  2217,  2221,  2224,  2227,  2231,  2233,  2237,
    2242,  2246,  2249,  2252,  2254,  2258,  2261,  2264,  2266,  2269,
    2273,  2275,  2279,  2281,  2288,  2293,  2298,  2302,  2308,  2312,
    2316,  2320,  2323,  2325,  2327,  2330,  2333,  2336,  2337,  2339,
    2341,  2344,  2348,  2350,  2353,  2354,  2358,  2359,  2360,  2366,
    2368,  2369,  2372,  2374,  2376,  2378,  2381,  2382,  2387,  2389,
    2390,  2391,  2397,  2398,  2399,  2407,  2408,  2409,  2410,  2411,
    2424,  2425,  2426,  2434,  2435,  2441,  2442,  2450,  2451,  2456,
    2459,  2462,  2465,  2469,  2476,  2485,  2496,  2509,  2514,  2518,
    2521,  2524,  2526,  2528,  2530,  2532,  2534,  2535,  2536,  2543,
    2544,  2545,  2551,  2553,  2556,  2557,  2558,  2564,  2566,  2568,
    2572,  2576,  2579,  2582,  2585,  2588,  2591,  2593,  2596,  2597,
    2599,  2600,  2602,  2604,  2605,  2607,  2609,  2613,  2618,  2620,
    2624,  2625,  2627,  2629,  2631,  2634,  2637,  2640,  2642,  2645,
    2648,  2649,  2653,  2655,  2657,  2659,  2662,  2665,  2668,  2673,
    2676,  2679,  2682,  2685,  2688,  2691,  2693,  2696,  2698,  2701,
    2703,  2705,  2706,  2707,  2709,  2710,  2715,  2718,  2720,  2722,
    2726,  2727,  2731,  2735,  2739,  2741,  2744,  2747,  2750,  2753,
    2756,  2759,  2762,  2765,  2768,  2771,  2774,  2777,  2780,  2783,
    2786,  2789,  2792,  2795,  2798,  2801,  2804,  2807,  2810,  2814,
    2817,  2820,  2823,  2826,  2830,  2833,  2836,  2841,  2846,  2850
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     113,     0,    -1,    -1,   114,    -1,    -1,   115,   121,    -1,
     114,   121,    -1,   114,    -1,    -1,    -1,    -1,    32,    -1,
      27,    -1,    -1,   122,   123,    -1,   149,   148,    -1,   145,
      -1,   142,    -1,   120,    92,   220,   108,    60,    -1,   134,
      58,   116,   109,    -1,   134,   117,   149,   118,   148,    -1,
     134,   117,   145,   118,    -1,    -1,    44,   164,    58,   124,
     116,   109,    -1,    -1,    44,    58,   125,   116,   109,    -1,
     126,    -1,   128,    60,    -1,   130,    -1,   119,   123,    -1,
      -1,    44,   164,    63,   127,   133,    60,    -1,    46,   315,
      -1,    46,   329,   315,    -1,    46,   329,   210,    -1,    46,
     132,   164,    -1,    46,   329,   164,    -1,    46,   329,   132,
     164,    -1,    -1,    46,    44,   131,   133,    60,    -1,    56,
      54,    -1,   132,    56,    54,    -1,   210,    -1,   315,    -1,
     329,   315,    -1,   329,   210,    -1,    98,    -1,   134,    98,
      -1,    -1,    48,    73,   136,   137,    74,    -1,    48,    73,
      74,    -1,   141,    -1,   137,    59,   141,    -1,   164,    -1,
      -1,   269,   138,    -1,    45,   138,    -1,   135,   269,   138,
      -1,   139,    -1,   139,    63,   226,    -1,   392,    -1,   392,
      63,   205,    -1,   140,    -1,   140,    63,   185,    -1,   135,
     143,    -1,   135,     1,    -1,   149,   148,    -1,   144,    -1,
     142,    -1,   134,   117,   149,   118,   148,    -1,   134,   117,
     144,   118,    -1,   119,   143,    -1,   237,    60,    -1,   230,
     236,    60,    -1,   227,   235,    60,    -1,   262,    60,    -1,
     237,    60,    -1,   230,   236,    60,    -1,   227,   235,    60,
      -1,   230,    60,    -1,   167,    60,    -1,   227,    60,    -1,
       1,    60,    -1,     1,   109,    -1,    60,    -1,   221,    -1,
     160,    -1,    -1,   159,    -1,   159,    60,    -1,    -1,   107,
      -1,   155,   147,   146,   339,    -1,   155,   147,   363,    -1,
     155,   147,     1,    -1,    -1,   320,     5,    92,   151,   383,
     108,   301,   395,    -1,   320,     5,    47,   301,   395,    -1,
      -1,   329,   320,     5,    92,   152,   383,   108,   301,   395,
      -1,   329,   320,     5,    47,   301,   395,    -1,    -1,   320,
     180,    92,   153,   383,   108,   301,   395,    -1,   320,   180,
      47,   301,   395,    -1,    -1,   329,   320,   180,    92,   154,
     383,   108,   301,   395,    -1,   329,   320,   180,    47,   301,
     395,    -1,   227,   224,    -1,   230,   312,    -1,   312,    -1,
     230,   150,    -1,   150,    -1,     5,    92,   383,   108,   301,
     395,    -1,     5,    47,   301,   395,    -1,   180,    92,   383,
     108,   301,   395,    -1,   180,    47,   301,   395,    -1,   230,
     156,    -1,   156,    -1,   227,   224,    -1,   230,   312,    -1,
     312,    -1,   230,   150,    -1,   150,    -1,    25,     3,    -1,
     158,   254,    -1,   158,    92,   197,   108,    -1,   158,    47,
      -1,    62,   161,   162,    -1,    -1,    -1,   163,    -1,   162,
      59,   163,    -1,   162,     1,    -1,    92,   197,   108,    -1,
      47,    -1,   165,    92,   197,   108,    -1,   165,    47,    -1,
     307,    92,   197,   108,    -1,   307,    47,    -1,   322,    92,
     197,   108,    -1,   322,    47,    -1,     3,    -1,     4,    -1,
       5,    -1,    57,    -1,    56,    -1,     3,    -1,    57,    -1,
      56,    -1,   104,    -1,   103,    -1,   105,    -1,    -1,    48,
     176,   233,    60,   168,   177,    -1,    -1,    48,   176,   227,
     224,   169,   177,    -1,    -1,    48,   176,   312,   170,   177,
      -1,    -1,    48,   176,   150,   171,   177,    -1,    -1,     7,
      48,   176,   233,    60,   172,   177,    -1,    -1,     7,    48,
     176,   227,   224,   173,   177,    -1,    -1,     7,    48,   176,
     312,   174,   177,    -1,    -1,     7,    48,   176,   150,   175,
     177,    -1,    -1,    -1,    57,    73,   183,   182,   181,    -1,
       4,    73,   183,   182,   181,    -1,   180,    -1,   178,    -1,
     164,    73,   183,    74,   181,    -1,     5,    73,   183,   182,
     181,    -1,    -1,    74,    -1,    76,    -1,    -1,   184,    -1,
     185,    -1,   184,    59,   185,    -1,   226,    -1,    57,    -1,
     205,    -1,    79,    -1,    78,    -1,    87,    -1,    86,    -1,
     110,    -1,   196,    -1,   205,    -1,    47,    -1,    92,   187,
     108,    -1,    47,    -1,    92,   191,   108,    -1,    -1,   191,
      -1,     1,    -1,    -1,   373,   224,   238,   247,    63,   192,
     255,    -1,   187,    -1,   109,    -1,   336,   334,   109,    -1,
     336,   334,     1,   109,    -1,   336,     1,   109,    -1,    -1,
      58,   195,   193,    -1,   348,    -1,   205,    59,   205,    -1,
     205,    59,     1,    -1,   196,    59,   205,    -1,   196,    59,
       1,    -1,   205,    -1,   196,    -1,   215,    -1,   119,   204,
      -1,    80,   204,    -1,    70,   204,    -1,    85,   204,    -1,
     186,   204,    -1,    67,   164,    -1,    13,   198,    -1,    13,
      92,   226,   108,    -1,    29,   198,    -1,    29,    92,   226,
     108,    -1,   217,   300,    -1,   217,   300,   202,    -1,   217,
     201,   300,    -1,   217,   201,   300,   202,    -1,   217,    92,
     200,   226,   199,    -1,   217,    92,   200,   226,   199,   202,
      -1,   217,   201,    92,   200,   226,   199,    -1,   217,   201,
      92,   200,   226,   199,   202,    -1,   218,   204,    -1,   218,
      93,   111,   204,    -1,   218,    93,   187,   111,   204,    -1,
      34,   204,    -1,    35,   204,    -1,   108,    -1,    -1,    92,
     200,   197,   108,    -1,    58,   200,   197,   109,    -1,    92,
     197,   108,    -1,    47,    -1,    92,   233,   108,    -1,    63,
     255,    -1,    92,   226,   108,    -1,   203,    92,   226,   108,
      -1,   198,    -1,   203,   198,    -1,   203,    58,   256,   267,
     109,    -1,   204,    -1,   205,    84,   205,    -1,   205,    83,
     205,    -1,   205,    78,   205,    -1,   205,    79,   205,    -1,
     205,    80,   205,    -1,   205,    81,   205,    -1,   205,    82,
     205,    -1,   205,    77,   205,    -1,   205,    76,   205,    -1,
     205,    75,   205,    -1,   205,    73,   205,    -1,   205,    74,
     205,    -1,   205,    72,   205,    -1,   205,    71,   205,    -1,
     205,    70,   205,    -1,   205,    68,   205,    -1,   205,    69,
     205,    -1,   205,    67,   205,    -1,   205,    66,   205,    -1,
     205,    65,   378,    62,   205,    -1,   205,    63,   205,    -1,
     205,    64,   205,    -1,    61,    -1,    61,   205,    -1,    85,
     393,   164,    -1,    85,   393,   178,    -1,   208,    -1,   400,
      -1,     3,    -1,    57,    -1,    56,    -1,    -1,     6,    73,
     207,   183,   182,    -1,   400,    73,   207,   183,   182,    -1,
      48,   164,    73,   183,   182,    -1,    48,     6,    73,   183,
     182,    -1,    48,   400,    73,   183,   182,    -1,   206,    -1,
       4,    -1,     5,    -1,   212,    -1,   248,   212,    -1,   206,
      -1,    80,   211,    -1,    70,   211,    -1,    92,   211,   108,
      -1,     3,    73,   183,   182,    -1,    56,    73,   184,   182,
      -1,   314,    -1,   206,    -1,   213,    -1,    92,   211,   108,
      -1,   206,    -1,    10,    -1,   219,    -1,   220,    -1,    92,
     187,   108,    -1,    92,   211,   108,    -1,    92,     1,   108,
      -1,    -1,    92,   216,   340,   108,    -1,   206,    92,   197,
     108,    -1,   206,    47,    -1,   215,    92,   197,   108,    -1,
     215,    47,    -1,   215,    93,   187,   111,    -1,   215,    87,
      -1,   215,    86,    -1,    40,    -1,     9,    92,   197,   108,
      -1,   318,    -1,    50,    73,   226,    74,    92,   187,   108,
      -1,    51,    73,   226,    74,    92,   187,   108,    -1,    52,
      73,   226,    74,    92,   187,   108,    -1,    53,    73,   226,
      74,    92,   187,   108,    -1,    49,    92,   187,   108,    -1,
      49,    92,   226,   108,    -1,   329,     3,    -1,   329,   208,
      -1,   329,   400,    -1,   317,    -1,   317,    92,   197,   108,
      -1,   317,    47,    -1,   222,   209,    -1,   222,   209,    92,
     197,   108,    -1,   222,   209,    47,    -1,   222,   210,    -1,
     222,   317,    -1,   222,   210,    92,   197,   108,    -1,   222,
     210,    47,    -1,   222,   317,    92,   197,   108,    -1,   222,
     317,    47,    -1,   222,    85,     8,    47,    -1,   222,     8,
      54,    85,     8,    47,    -1,   222,     1,    -1,    39,    -1,
     329,    39,    -1,    38,    -1,   329,   218,    -1,    42,    -1,
      43,    -1,    11,    -1,   220,    11,    -1,    -1,   215,    91,
      -1,   215,    94,    -1,   233,   235,    60,    -1,   227,   235,
      60,    -1,   230,   236,    60,    -1,   227,    60,    -1,   230,
      60,    -1,   119,   223,    -1,   306,    -1,   312,    -1,    47,
      -1,   225,    47,    -1,   231,   332,    -1,   302,   332,    -1,
     233,   332,    -1,   231,    -1,   302,    -1,   231,    -1,   228,
      -1,   230,   233,    -1,   233,   229,    -1,   233,   232,   229,
      -1,   230,   233,   229,    -1,   230,   233,   232,    -1,   230,
     233,   232,   229,    -1,     7,    -1,   229,   234,    -1,   229,
       7,    -1,   229,   248,    -1,   248,    -1,   302,    -1,     7,
      -1,   230,     9,    -1,   230,     7,    -1,   230,   248,    -1,
     248,    -1,   233,    -1,   302,   233,    -1,   233,   232,    -1,
     302,   233,   232,    -1,   234,    -1,   232,   234,    -1,   262,
      -1,     8,    -1,   308,    -1,    28,    92,   187,   108,    -1,
      28,    92,   226,   108,    -1,    30,    92,   187,   108,    -1,
      30,    92,   226,   108,    -1,     8,    -1,     9,    -1,   262,
      -1,   243,    -1,   235,    59,   239,    -1,   244,    -1,   236,
      59,   239,    -1,   245,    -1,   237,    59,   239,    -1,    -1,
     120,    92,   220,   108,    -1,    -1,   224,   238,   247,    63,
     240,   255,    -1,   224,   238,   247,    -1,    -1,   247,    63,
     242,   255,    -1,   247,    -1,   224,   238,   241,    -1,   312,
     238,   241,    -1,    -1,   312,   238,   246,   241,    -1,   150,
     238,   247,    -1,    -1,   248,    -1,   249,    -1,   248,   249,
      -1,    31,    92,    92,   250,   108,   108,    -1,   251,    -1,
     250,    59,   251,    -1,    -1,   252,    -1,   252,    92,     3,
     108,    -1,   252,    92,     3,    59,   197,   108,    -1,   252,
      92,   197,   108,    -1,   164,    -1,     7,    -1,     8,    -1,
       9,    -1,   164,    -1,   253,    59,   164,    -1,    -1,    63,
     255,    -1,   205,    -1,    58,   109,    -1,    58,   256,   109,
      -1,    58,   256,    59,   109,    -1,     1,    -1,   255,    -1,
     256,    59,   255,    -1,    93,   205,   111,   255,    -1,   164,
      62,   255,    -1,   256,    59,   164,    62,   255,    -1,    97,
      -1,   257,   147,   146,   339,    -1,   257,   147,   363,    -1,
     257,   147,     1,    -1,    -1,   259,   258,   148,    -1,   102,
     205,   107,    -1,   102,     1,   107,    -1,    -1,   261,   260,
      -1,   261,     1,    -1,    -1,    14,   164,    58,   263,   298,
     268,   109,    -1,    14,   164,    58,   109,    -1,    -1,    14,
      58,   264,   298,   268,   109,    -1,    14,    58,   109,    -1,
      14,   164,    -1,    14,   327,    -1,    45,   322,    -1,    -1,
      -1,   277,   283,   285,   109,   247,   265,   261,   266,   259,
      -1,   277,    -1,    -1,    59,    -1,    -1,    59,    -1,    36,
      -1,   269,     7,    -1,   269,     8,    -1,   269,     9,    -1,
     269,    36,    -1,   269,   248,    -1,   269,   164,    -1,   269,
     166,    -1,   270,    58,    -1,   270,    62,    -1,   269,   320,
     164,    -1,   269,   329,   320,   164,    -1,   269,   329,   164,
      -1,   269,   179,    -1,   269,   320,   179,    -1,   270,    -1,
      -1,   271,   274,   278,    -1,    -1,   272,   275,   278,    -1,
     269,    58,    -1,   276,    -1,   273,    -1,    -1,    62,   393,
      -1,    62,   393,   279,    -1,   280,    -1,   279,    59,   393,
     280,    -1,   281,    -1,   282,   393,   281,    -1,   322,    -1,
     307,    -1,    30,    92,   187,   108,    -1,    30,    92,   226,
     108,    -1,    37,   393,    -1,     7,   393,    -1,   282,    37,
     393,    -1,   282,     7,   393,    -1,    58,    -1,    -1,   284,
      -1,   284,   287,    -1,   285,   286,   287,    -1,   285,   286,
      -1,    37,    62,    -1,   288,    -1,   287,   288,    -1,   289,
      60,    -1,   289,   109,    -1,   157,    62,    -1,   157,    96,
      -1,   157,    25,    -1,   157,    58,    -1,    60,    -1,   119,
     288,    -1,   135,   288,    -1,   135,   227,    60,    -1,   227,
     290,    -1,   230,   291,    -1,   312,   238,   247,   254,    -1,
     150,   238,   247,   254,    -1,    62,   205,    -1,     1,    -1,
     230,   156,   238,   247,   254,    -1,   156,   238,   247,   254,
      -1,   128,    -1,    -1,   292,    -1,   290,    59,   293,    -1,
      -1,   295,    -1,   291,    59,   297,    -1,   294,    -1,   295,
      -1,   296,    -1,   297,    -1,   306,   238,   247,   254,    -1,
       4,    62,   205,   247,    -1,   312,   238,   247,   254,    -1,
     150,   238,   247,   254,    -1,     3,    62,   205,   247,    -1,
      62,   205,   247,    -1,   306,   238,   247,   254,    -1,     4,
      62,   205,   247,    -1,   312,   238,   247,   254,    -1,     3,
      62,   205,   247,    -1,    62,   205,   247,    -1,   299,    -1,
     298,    59,   299,    -1,   164,    -1,   164,    63,   205,    -1,
     373,   330,    -1,   373,    -1,    92,   200,   226,   199,    93,
     187,   111,    -1,    -1,   301,     9,    -1,     9,    -1,   302,
       9,    -1,    -1,   303,   187,    -1,   303,    92,   197,   108,
      -1,   303,    92,   383,   108,    -1,   303,    47,    -1,   303,
      92,     1,   108,    -1,    80,   302,   306,    -1,    70,   302,
     306,    -1,    80,   306,    -1,    70,   306,    -1,   328,   301,
     306,    -1,   310,    -1,   319,    -1,   329,   319,    -1,   307,
      -1,   309,    -1,   329,   309,    -1,   320,   319,    -1,   310,
     305,   301,   395,    -1,   310,    93,   304,   111,    -1,   310,
      93,   111,    -1,    92,   306,   108,    -1,   320,   319,    -1,
     319,    -1,   312,    -1,   248,   312,    -1,    80,   302,   311,
      -1,    70,   302,   311,    -1,    80,   311,    -1,    70,   311,
      -1,   328,   301,   311,    -1,   214,    -1,    80,   302,   311,
      -1,    70,   302,   311,    -1,    80,   313,    -1,    70,   313,
      -1,   328,   301,   311,    -1,   314,    -1,   214,   305,   301,
     395,    -1,    92,   313,   108,    -1,   214,    93,   304,   111,
      -1,   214,    93,   111,    -1,   316,    -1,   320,   213,    -1,
     320,   210,    -1,   320,   209,    -1,   320,   206,    -1,   320,
     209,    -1,   316,    -1,   329,   316,    -1,   233,    92,   197,
     108,    -1,   233,    92,   211,   108,    -1,   233,   225,    -1,
       4,    -1,     5,    -1,   178,    -1,   321,    -1,   320,   321,
      -1,   320,    48,   326,    54,    -1,     4,    54,    -1,     5,
      54,    -1,    56,    54,    -1,   178,    54,    -1,   323,    -1,
     329,   323,    -1,   324,   164,    -1,   324,   178,    -1,   324,
     326,    -1,   324,    48,   326,    -1,   325,    -1,   324,   325,
      -1,   324,   326,    54,    -1,   324,    48,   326,    54,    -1,
       4,    54,    -1,     5,    54,    -1,   178,    54,    -1,    57,
      54,    -1,     3,    54,    -1,    56,    54,    -1,   164,    73,
     183,   182,    -1,   329,   319,    -1,   309,    -1,   329,   309,
      -1,   320,    80,    -1,   329,   320,    80,    -1,    54,    -1,
      80,   301,   330,    -1,    80,   301,    -1,    70,   301,   330,
      -1,    70,   301,    -1,   328,   301,    -1,   328,   301,   330,
      -1,   331,    -1,    93,   187,   111,    -1,   331,    93,   304,
     111,    -1,    80,   302,   332,    -1,    80,   332,    -1,    80,
     302,    -1,    80,    -1,    70,   302,   332,    -1,    70,   332,
      -1,    70,   302,    -1,    70,    -1,   328,   301,    -1,   328,
     301,   332,    -1,   333,    -1,    92,   332,   108,    -1,    90,
      -1,   333,    92,   383,   108,   301,   395,    -1,   333,    47,
     301,   395,    -1,   333,    93,   304,   111,    -1,   333,    93,
     111,    -1,    92,   384,   108,   301,   395,    -1,   203,   301,
     395,    -1,   225,   301,   395,    -1,    93,   304,   111,    -1,
      93,   111,    -1,   347,    -1,   335,    -1,   334,   347,    -1,
     334,   335,    -1,     1,    60,    -1,    -1,   337,    -1,   338,
      -1,   337,   338,    -1,    33,   253,    60,    -1,   340,    -1,
       1,   340,    -1,    -1,    58,   341,   193,    -1,    -1,    -1,
      15,   343,   189,   344,   345,    -1,   340,    -1,    -1,   346,
     348,    -1,   340,    -1,   348,    -1,   223,    -1,   187,    60,
      -1,    -1,   342,    16,   349,   345,    -1,   342,    -1,    -1,
      -1,    17,   350,   189,   351,   194,    -1,    -1,    -1,    18,
     352,   345,    17,   353,   188,    60,    -1,    -1,    -1,    -1,
      -1,    19,   354,    92,   376,   355,   190,    60,   356,   378,
     108,   357,   194,    -1,    -1,    -1,    20,   358,    92,   191,
     108,   359,   345,    -1,    -1,    21,   205,    62,   360,   347,
      -1,    -1,    21,   205,    12,   205,    62,   361,   347,    -1,
      -1,    22,    62,   362,   347,    -1,    23,    60,    -1,    24,
      60,    -1,    25,    60,    -1,    25,   187,    60,    -1,   120,
     377,    92,   220,   108,    60,    -1,   120,   377,    92,   220,
      62,   379,   108,    60,    -1,   120,   377,    92,   220,    62,
     379,    62,   379,   108,    60,    -1,   120,   377,    92,   220,
      62,   379,    62,   379,    62,   382,   108,    60,    -1,    26,
      80,   187,    60,    -1,    26,   164,    60,    -1,   375,   347,
      -1,   375,   109,    -1,    60,    -1,   366,    -1,   130,    -1,
     129,    -1,   126,    -1,    -1,    -1,    96,   364,   146,   340,
     365,   369,    -1,    -1,    -1,    96,   367,   340,   368,   369,
      -1,   370,    -1,   369,   370,    -1,    -1,    -1,    95,   371,
     374,   372,   340,    -1,   231,    -1,   302,    -1,    92,    12,
     108,    -1,    92,   392,   108,    -1,     3,    62,    -1,    57,
      62,    -1,     4,    62,    -1,     5,    62,    -1,   378,    60,
      -1,   223,    -1,    58,   193,    -1,    -1,     9,    -1,    -1,
     187,    -1,     1,    -1,    -1,   380,    -1,   381,    -1,   380,
      59,   381,    -1,    11,    92,   187,   108,    -1,    11,    -1,
     382,    59,    11,    -1,    -1,   384,    -1,   226,    -1,   388,
      -1,   389,    12,    -1,   388,    12,    -1,   226,    12,    -1,
      12,    -1,   388,    62,    -1,   226,    62,    -1,    -1,    63,
     386,   387,    -1,   101,    -1,   255,    -1,   390,    -1,   392,
     385,    -1,   389,   391,    -1,   389,   394,    -1,   389,   394,
      63,   255,    -1,   388,    59,    -1,   226,    59,    -1,   228,
     224,    -1,   231,   224,    -1,   233,   224,    -1,   228,   332,
      -1,   228,    -1,   230,   312,    -1,   392,    -1,   392,   385,
      -1,   390,    -1,   226,    -1,    -1,    -1,   312,    -1,    -1,
      61,    92,   397,   108,    -1,    61,    47,    -1,   226,    -1,
     396,    -1,   397,    59,   396,    -1,    -1,    80,   301,   398,
      -1,    70,   301,   398,    -1,   328,   301,   398,    -1,    41,
      -1,   399,    80,    -1,   399,    81,    -1,   399,    82,    -1,
     399,    78,    -1,   399,    79,    -1,   399,    70,    -1,   399,
      68,    -1,   399,    69,    -1,   399,    85,    -1,   399,    59,
      -1,   399,    75,    -1,   399,    73,    -1,   399,    74,    -1,
     399,    72,    -1,   399,    64,    -1,   399,    63,    -1,   399,
      77,    -1,   399,    76,    -1,   399,    87,    -1,   399,    86,
      -1,   399,    67,    -1,   399,    66,    -1,   399,   110,    -1,
     399,    65,    62,    -1,   399,    71,    -1,   399,    94,    -1,
     399,    84,    -1,   399,    47,    -1,   399,    93,   111,    -1,
     399,    39,    -1,   399,    38,    -1,   399,    39,    93,   111,
      -1,   399,    38,    93,   111,    -1,   399,   373,   398,    -1,
     399,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   337,   337,   339,   348,   348,   351,   356,   357,   361,
     366,   370,   376,   380,   380,   387,   389,   391,   393,   396,
     398,   401,   405,   404,   409,   408,   412,   413,   415,   416,
     422,   421,   433,   435,   437,   442,   444,   446,   452,   451,
     466,   472,   481,   482,   483,   485,   490,   492,   500,   499,
     503,   509,   511,   516,   519,   522,   524,   529,   541,   543,
     545,   547,   549,   551,   565,   567,   572,   574,   576,   578,
     581,   584,   589,   590,   592,   594,   600,   601,   603,   605,
     607,   608,   615,   616,   617,   621,   623,   627,   629,   630,
     633,   635,   639,   641,   647,   653,   652,   656,   661,   660,
     664,   669,   668,   672,   677,   676,   680,   687,   690,   693,
     696,   699,   705,   707,   709,   711,   718,   729,   732,   737,
     742,   745,   750,   756,   765,   767,   769,   774,   788,   808,
     809,   811,   812,   816,   822,   828,   830,   832,   834,   836,
     839,   845,   846,   847,   848,   849,   853,   854,   855,   859,
     860,   861,   866,   865,   870,   869,   874,   873,   877,   876,
     880,   879,   885,   883,   889,   888,   892,   891,   897,   900,
     907,   910,   913,   917,   918,   923,   929,   938,   939,   949,
     950,   954,   956,   961,   963,   965,   969,   971,   973,   975,
     977,   982,   984,   988,   992,   997,  1001,  1007,  1008,  1009,
    1015,  1014,  1039,  1043,  1044,  1045,  1046,  1051,  1050,  1054,
    1059,  1062,  1065,  1067,  1072,  1074,  1078,  1081,  1084,  1086,
    1088,  1090,  1093,  1097,  1099,  1102,  1104,  1110,  1113,  1116,
    1119,  1131,  1136,  1140,  1144,  1149,  1151,  1155,  1159,  1161,
    1171,  1175,  1178,  1180,  1186,  1188,  1190,  1198,  1212,  1216,
    1223,  1224,  1226,  1240,  1242,  1244,  1246,  1248,  1250,  1252,
    1254,  1256,  1258,  1260,  1262,  1264,  1266,  1268,  1270,  1272,
    1274,  1276,  1278,  1280,  1282,  1286,  1288,  1290,  1308,  1310,
    1312,  1313,  1314,  1315,  1316,  1320,  1332,  1334,  1339,  1341,
    1343,  1349,  1350,  1351,  1355,  1356,  1365,  1366,  1368,  1370,
    1375,  1377,  1382,  1385,  1386,  1387,  1392,  1399,  1400,  1401,
    1415,  1417,  1420,  1423,  1422,  1438,  1440,  1442,  1444,  1446,
    1448,  1450,  1453,  1455,  1472,  1473,  1477,  1481,  1485,  1489,
    1491,  1495,  1497,  1499,  1507,  1509,  1511,  1513,  1517,  1519,
    1521,  1523,  1528,  1530,  1532,  1534,  1537,  1539,  1541,  1586,
    1588,  1593,  1595,  1600,  1602,  1608,  1609,  1615,  1627,  1629,
    1637,  1643,  1648,  1650,  1655,  1657,  1665,  1666,  1671,  1674,
    1681,  1684,  1687,  1690,  1693,  1704,  1705,  1709,  1712,  1715,
    1718,  1721,  1724,  1731,  1736,  1738,  1743,  1745,  1755,  1757,
    1759,  1762,  1768,  1770,  1781,  1784,  1787,  1790,  1796,  1798,
    1807,  1808,  1810,  1812,  1815,  1818,  1833,  1853,  1855,  1857,
    1861,  1862,  1867,  1868,  1873,  1874,  1880,  1881,  1887,  1886,
    1892,  1906,  1905,  1914,  1921,  1925,  1931,  1930,  1936,  1946,
    1947,  1952,  1954,  1959,  1964,  1966,  1972,  1973,  1975,  1977,
    1979,  1987,  1988,  1989,  1990,  1995,  1997,  2002,  2004,  2011,
    2012,  2015,  2018,  2021,  2028,  2030,  2033,  2035,  2037,  2042,
    2048,  2055,  2062,  2066,  2068,  2074,  2076,  2080,  2081,  2083,
    2089,  2088,  2099,  2104,  2103,  2114,  2118,  2121,  2124,  2132,
    2142,  2130,  2147,  2167,  2169,  2172,  2174,  2180,  2181,  2183,
    2185,  2187,  2189,  2194,  2202,  2204,  2206,  2211,  2216,  2221,
    2226,  2228,  2233,  2236,  2235,  2246,  2245,  2271,  2277,  2278,
    2282,  2284,  2286,  2291,  2292,  2301,  2305,  2312,  2314,  2315,
    2336,  2360,  2361,  2365,  2376,  2391,  2397,  2403,  2404,  2405,
    2406,  2410,  2425,  2429,  2436,  2437,  2442,  2444,  2446,  2448,
    2450,  2452,  2455,  2465,  2476,  2497,  2503,  2506,  2509,  2511,
    2522,  2527,  2530,  2537,  2538,  2545,  2557,  2558,  2565,  2576,
    2577,  2581,  2582,  2586,  2592,  2601,  2607,  2613,  2619,  2628,
    2631,  2637,  2640,  2643,  2653,  2654,  2659,  2661,  2667,  2670,
    2676,  2688,  2690,  2695,  2698,  2708,  2712,  2718,  2720,  2722,
    2724,  2731,  2733,  2735,  2737,  2739,  2743,  2747,  2764,  2775,
    2776,  2777,  2782,  2787,  2789,  2791,  2793,  2795,  2799,  2806,
    2807,  2816,  2818,  2820,  2822,  2824,  2828,  2832,  2834,  2836,
    2838,  2840,  2844,  2848,  2850,  2852,  2854,  2856,  2858,  2866,
    2869,  2875,  2878,  2884,  2885,  2890,  2892,  2894,  2898,  2899,
    2900,  2904,  2905,  2907,  2914,  2928,  2934,  2940,  2955,  2956,
    2961,  2974,  2976,  2978,  2983,  2988,  3001,  3003,  3008,  3020,
    3026,  3028,  3029,  3030,  3039,  3044,  3052,  3053,  3058,  3060,
    3067,  3073,  3075,  3077,  3079,  3081,  3085,  3089,  3094,  3096,
    3102,  3104,  3106,  3108,  3110,  3112,  3114,  3116,  3118,  3122,
    3126,  3131,  3134,  3135,  3137,  3139,  3141,  3143,  3145,  3147,
    3149,  3151,  3160,  3161,  3162,  3163,  3167,  3172,  3174,  3180,
    3181,  3185,  3199,  3201,  3206,  3205,  3213,  3218,  3212,  3224,
    3225,  3225,  3231,  3233,  3237,  3239,  3242,  3241,  3248,  3251,
    3256,  3250,  3260,  3262,  3259,  3269,  3271,  3273,  3275,  3268,
    3279,  3281,  3278,  3285,  3284,  3288,  3287,  3291,  3290,  3293,
    3295,  3297,  3299,  3301,  3307,  3313,  3316,  3319,  3325,  3327,
    3329,  3333,  3335,  3336,  3337,  3339,  3344,  3350,  3343,  3362,
    3364,  3361,  3370,  3371,  3376,  3378,  3375,  3384,  3385,  3389,
    3405,  3412,  3419,  3421,  3423,  3428,  3430,  3431,  3441,  3443,
    3449,  3450,  3451,  3459,  3460,  3464,  3465,  3470,  3475,  3477,
    3489,  3492,  3493,  3501,  3503,  3506,  3508,  3511,  3513,  3523,
    3539,  3538,  3545,  3546,  3551,  3554,  3557,  3560,  3562,  3567,
    3568,  3578,  3582,  3585,  3588,  3592,  3596,  3603,  3606,  3612,
    3613,  3617,  3622,  3627,  3639,  3641,  3643,  3648,  3653,  3654,
    3662,  3664,  3666,  3668,  3675,  3680,  3682,  3684,  3686,  3688,
    3690,  3692,  3694,  3696,  3698,  3700,  3702,  3704,  3706,  3708,
    3710,  3712,  3714,  3716,  3718,  3720,  3722,  3724,  3726,  3728,
    3730,  3732,  3734,  3736,  3738,  3740,  3742,  3744,  3747,  3749
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "TYPENAME", "SELFNAME",
  "PFUNCNAME", "SCSPEC", "TYPESPEC", "CV_QUALIFIER", "CONSTANT", "STRING",
  "ELLIPSIS", "SIZEOF", "ENUM", "IF", "ELSE", "WHILE", "DO", "FOR",
  "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN", "GOTO",
  "ASM_KEYWORD", "TYPEOF", "ALIGNOF", "SIGOF", "ATTRIBUTE", "EXTENSION",
  "LABEL", "REALPART", "IMAGPART", "AGGR", "VISSPEC", "DELETE", "NEW",
  "THIS", "OPERATOR", "CXX_TRUE", "CXX_FALSE", "NAMESPACE",
  "TYPENAME_KEYWORD", "USING", "LEFT_RIGHT", "TEMPLATE", "TYPEID",
  "DYNAMIC_CAST", "STATIC_CAST", "REINTERPRET_CAST", "CONST_CAST", "SCOPE",
  "EMPTY", "NSNAME", "PTYPENAME", "'{'", "','", "';'", "THROW", "':'",
  "'='", "ASSIGN", "'?'", "OROR", "ANDAND", "'|'", "'^'", "'&'", "MIN_MAX",
  "EQCOMPARE", "'<'", "'>'", "ARITHCOMPARE", "RSHIFT", "LSHIFT", "'+'",
  "'-'", "'*'", "'/'", "'%'", "DOT_STAR", "POINTSAT_STAR", "'~'",
  "MINUSMINUS", "PLUSPLUS", "UNARY", "HYPERUNARY", "PAREN_STAR_PAREN",
  "'.'", "'('", "'['", "POINTSAT", "CATCH", "TRY",
  "PRE_PARSED_FUNCTION_DECL", "EXTERN_LANG_STRING", "ALL",
  "PRE_PARSED_CLASS_DECL", "DEFARG", "DEFARG_MARKER", "TYPENAME_DEFN",
  "IDENTIFIER_DEFN", "PTYPENAME_DEFN", "END_OF_LINE", "END_OF_SAVED_INPUT",
  "')'", "'}'", "'!'", "']'", "$accept", "program", "extdefs", "@1",
  "extdefs_opt", ".hush_warning", ".warning_ok", "extension",
  "asm_keyword", "lang_extdef", "@2", "extdef", "@3", "@4",
  "namespace_alias", "@5", "using_decl", "namespace_using_decl",
  "using_directive", "@6", "namespace_qualifier", "any_id",
  "extern_lang_string", "template_header", "@7", "template_parm_list",
  "maybe_identifier", "template_type_parm", "template_template_parm",
  "template_parm", "template_def", "template_extdef", "template_datadef",
  "datadef", "ctor_initializer_opt", "maybe_return_init",
  "eat_saved_input", "fndef", "constructor_declarator", "@8", "@9", "@10",
  "@11", "fn.def1", "component_constructor_declarator", "fn.def2",
  "return_id", "return_init", "base_init", ".set_base_init",
  "member_init_list", "member_init", "identifier", "notype_identifier",
  "identifier_defn", "explicit_instantiation", "@12", "@13", "@14", "@15",
  "@16", "@17", "@18", "@19", "begin_explicit_instantiation",
  "end_explicit_instantiation", "template_type", "apparent_template_type",
  "self_template_type", ".finish_template_type", "template_close_bracket",
  "template_arg_list_opt", "template_arg_list", "template_arg", "unop",
  "expr", "paren_expr_or_null", "paren_cond_or_null", "xcond", "condition",
  "@20", "compstmtend", "already_scoped_stmt", "@21",
  "nontrivial_exprlist", "nonnull_exprlist", "unary_expr",
  ".finish_new_placement", ".begin_new_placement", "new_placement",
  "new_initializer", "regcast_or_absdcl", "cast_expr", "expr_no_commas",
  "notype_unqualified_id", "do_id", "template_id", "object_template_id",
  "unqualified_id", "expr_or_declarator_intern", "expr_or_declarator",
  "notype_template_declarator", "direct_notype_declarator", "primary",
  "@22", "new", "delete", "boolean.literal", "string", "nodecls", "object",
  "decl", "declarator", "fcast_or_absdcl", "type_id", "typed_declspecs",
  "typed_declspecs1", "reserved_declspecs", "declmods", "typed_typespecs",
  "reserved_typespecquals", "typespec", "typespecqual_reserved",
  "initdecls", "notype_initdecls", "nomods_initdecls", "maybeasm",
  "initdcl", "@23", "initdcl0_innards", "@24", "initdcl0",
  "notype_initdcl0", "nomods_initdcl0", "@25", "maybe_attribute",
  "attributes", "attribute", "attribute_list", "attrib", "any_word",
  "identifiers_or_typenames", "maybe_init", "init", "initlist",
  "fn.defpen", "pending_inline", "pending_inlines", "defarg_again",
  "pending_defargs", "structsp", "@26", "@27", "@28", "@29", "maybecomma",
  "maybecomma_warn", "aggr", "named_class_head_sans_basetype",
  "named_class_head_sans_basetype_defn",
  "named_complex_class_head_sans_basetype", "named_class_head", "@30",
  "@31", "unnamed_class_head", "class_head", "maybe_base_class_list",
  "base_class_list", "base_class", "base_class.1",
  "base_class_access_list", "left_curly", "self_reference",
  "opt.component_decl_list", "access_specifier", "component_decl_list",
  "component_decl", "component_decl_1", "components", "notype_components",
  "component_declarator0", "component_declarator",
  "after_type_component_declarator0", "notype_component_declarator0",
  "after_type_component_declarator", "notype_component_declarator",
  "enumlist", "enumerator", "new_type_id", "cv_qualifiers",
  "nonempty_cv_qualifiers", "suspend_mom", "nonmomentary_expr",
  "maybe_parmlist", "after_type_declarator", "nonnested_type",
  "complete_type_name", "nested_type", "direct_after_type_declarator",
  "notype_declarator_intern", "notype_declarator",
  "complex_notype_declarator", "complex_direct_notype_declarator",
  "qualified_id", "notype_qualified_id", "overqualified_id",
  "functional_cast", "type_name", "nested_name_specifier",
  "nested_name_specifier_1", "typename_sub", "typename_sub0",
  "typename_sub1", "typename_sub2", "explicit_template_type",
  "complex_type_name", "ptr_to_mem", "global_scope", "new_declarator",
  "direct_new_declarator", "absdcl", "direct_abstract_declarator", "stmts",
  "errstmt", "maybe_label_decls", "label_decls", "label_decl",
  "compstmt_or_error", "compstmt", "@32", "simple_if", "@33", "@34",
  "implicitly_scoped_stmt", "@35", "stmt", "simple_stmt", "@36", "@37",
  "@38", "@39", "@40", "@41", "@42", "@43", "@44", "@45", "@46", "@47",
  "@48", "@49", "function_try_block", "@50", "@51", "try_block", "@52",
  "@53", "handler_seq", "handler", "@54", "@55", "type_specifier_seq",
  "handler_args", "label_colon", "for.init.statement",
  "maybe_cv_qualifier", "xexpr", "asm_operands", "nonnull_asm_operands",
  "asm_operand", "asm_clobbers", "parmlist", "complex_parmlist", "defarg",
  "@56", "defarg1", "parms", "parms_comma", "named_parm", "full_parm",
  "parm", "see_typename", "bad_parm", "exception_specification_opt",
  "ansi_raise_identifier", "ansi_raise_identifiers",
  "conversion_declarator", "operator", "operator_name", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   123,    44,
      59,   313,    58,    61,   314,    63,   315,   316,   124,    94,
      38,   317,   318,    60,    62,   319,   320,   321,    43,    45,
      42,    47,    37,   322,   323,   126,   324,   325,   326,   327,
     328,    46,    40,    91,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,    41,   125,
      33,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   112,   113,   113,   115,   114,   114,   116,   116,   117,
     118,   119,   120,   122,   121,   123,   123,   123,   123,   123,
     123,   123,   124,   123,   125,   123,   123,   123,   123,   123,
     127,   126,   128,   128,   128,   129,   129,   129,   131,   130,
     132,   132,   133,   133,   133,   133,   134,   134,   136,   135,
     135,   137,   137,   138,   138,   139,   139,   140,   141,   141,
     141,   141,   141,   141,   142,   142,   143,   143,   143,   143,
     143,   143,   144,   144,   144,   144,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   146,   146,   147,   147,   147,
     148,   148,   149,   149,   149,   151,   150,   150,   152,   150,
     150,   153,   150,   150,   154,   150,   150,   155,   155,   155,
     155,   155,   156,   156,   156,   156,   157,   157,   157,   157,
     157,   157,   157,   158,   159,   159,   159,   160,   161,   162,
     162,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   164,   164,   164,   164,   164,   165,   165,   165,   166,
     166,   166,   168,   167,   169,   167,   170,   167,   171,   167,
     172,   167,   173,   167,   174,   167,   175,   167,   176,   177,
     178,   178,   178,   179,   179,   180,   181,   182,   182,   183,
     183,   184,   184,   185,   185,   185,   186,   186,   186,   186,
     186,   187,   187,   188,   188,   189,   189,   190,   190,   190,
     192,   191,   191,   193,   193,   193,   193,   195,   194,   194,
     196,   196,   196,   196,   197,   197,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     199,   200,   201,   201,   202,   202,   202,   202,   203,   203,
     204,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   206,   206,
     206,   206,   206,   206,   206,   207,   208,   208,   209,   209,
     209,   210,   210,   210,   211,   211,   212,   212,   212,   212,
     213,   213,   214,   214,   214,   214,   215,   215,   215,   215,
     215,   215,   215,   216,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   217,
     217,   218,   218,   219,   219,   220,   220,   221,   222,   222,
     223,   223,   223,   223,   223,   223,   224,   224,   225,   225,
     226,   226,   226,   226,   226,   227,   227,   228,   228,   228,
     228,   228,   228,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   230,   230,   231,   231,   231,   231,   232,   232,
     233,   233,   233,   233,   233,   233,   233,   234,   234,   234,
     235,   235,   236,   236,   237,   237,   238,   238,   240,   239,
     239,   242,   241,   241,   243,   244,   246,   245,   245,   247,
     247,   248,   248,   249,   250,   250,   251,   251,   251,   251,
     251,   252,   252,   252,   252,   253,   253,   254,   254,   255,
     255,   255,   255,   255,   256,   256,   256,   256,   256,   257,
     258,   258,   258,   259,   259,   260,   260,   261,   261,   261,
     263,   262,   262,   264,   262,   262,   262,   262,   262,   265,
     266,   262,   262,   267,   267,   268,   268,   269,   269,   269,
     269,   269,   269,   270,   271,   271,   271,   272,   272,   272,
     272,   272,   273,   274,   273,   275,   273,   276,   277,   277,
     278,   278,   278,   279,   279,   280,   280,   281,   281,   281,
     281,   282,   282,   282,   282,   283,   284,   285,   285,   285,
     285,   286,   287,   287,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   290,   290,   290,   291,   291,   291,   292,
     292,   293,   293,   294,   294,   295,   295,   295,   295,   296,
     296,   297,   297,   297,   298,   298,   299,   299,   300,   300,
     300,   301,   301,   302,   302,   303,   304,   305,   305,   305,
     305,   306,   306,   306,   306,   306,   306,   307,   307,   308,
     308,   308,   309,   310,   310,   310,   310,   310,   310,   311,
     311,   312,   312,   312,   312,   312,   312,   313,   313,   313,
     313,   313,   313,   314,   314,   314,   314,   314,   314,   315,
     315,   316,   316,   317,   317,   318,   318,   318,   319,   319,
     319,   320,   320,   320,   321,   321,   321,   321,   322,   322,
     323,   323,   323,   323,   324,   324,   324,   324,   325,   325,
     325,   325,   325,   325,   326,   327,   327,   327,   328,   328,
     329,   330,   330,   330,   330,   330,   330,   330,   331,   331,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   334,   334,   334,   334,   335,   336,   336,   337,
     337,   338,   339,   339,   341,   340,   343,   344,   342,   345,
     346,   345,   347,   347,   348,   348,   349,   348,   348,   350,
     351,   348,   352,   353,   348,   354,   355,   356,   357,   348,
     358,   359,   348,   360,   348,   361,   348,   362,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   364,   365,   363,   367,
     368,   366,   369,   369,   371,   372,   370,   373,   373,   374,
     374,   375,   375,   375,   375,   376,   376,   376,   377,   377,
     378,   378,   378,   379,   379,   380,   380,   381,   382,   382,
     383,   383,   383,   384,   384,   384,   384,   384,   384,   384,
     386,   385,   387,   387,   388,   388,   388,   388,   388,   389,
     389,   390,   390,   390,   390,   390,   390,   391,   391,   392,
     392,   393,   394,   394,   395,   395,   395,   396,   397,   397,
     398,   398,   398,   398,   399,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     0,     2,     2,     1,     0,     0,
       0,     1,     1,     0,     2,     2,     1,     1,     5,     4,
       5,     4,     0,     6,     0,     5,     1,     2,     1,     2,
       0,     6,     2,     3,     3,     3,     3,     4,     0,     5,
       2,     3,     1,     1,     2,     2,     1,     2,     0,     5,
       3,     1,     3,     1,     0,     2,     2,     3,     1,     3,
       1,     3,     1,     3,     2,     2,     2,     1,     1,     5,
       4,     2,     2,     3,     3,     2,     2,     3,     3,     2,
       2,     2,     2,     2,     1,     1,     1,     0,     1,     2,
       0,     1,     4,     3,     3,     0,     8,     5,     0,     9,
       6,     0,     8,     5,     0,     9,     6,     2,     2,     1,
       2,     1,     6,     4,     6,     4,     2,     1,     2,     2,
       1,     2,     1,     2,     2,     4,     2,     3,     0,     0,
       1,     3,     2,     3,     1,     4,     2,     4,     2,     4,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     6,     0,     6,     0,     5,     0,     5,
       0,     7,     0,     7,     0,     6,     0,     6,     0,     0,
       5,     5,     1,     1,     5,     5,     0,     1,     1,     0,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     0,     1,     1,
       0,     7,     1,     1,     3,     4,     3,     0,     3,     1,
       3,     3,     3,     3,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     4,     2,     3,     3,
       4,     5,     6,     6,     7,     2,     4,     5,     2,     2,
       1,     0,     4,     4,     3,     1,     3,     2,     3,     4,
       1,     2,     5,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     3,     3,     1,     2,     3,     3,
       1,     1,     1,     1,     1,     0,     5,     5,     5,     5,
       5,     1,     1,     1,     1,     2,     1,     2,     2,     3,
       4,     4,     1,     1,     1,     3,     1,     1,     1,     1,
       3,     3,     3,     0,     4,     4,     2,     4,     2,     4,
       2,     2,     1,     4,     1,     7,     7,     7,     7,     4,
       4,     2,     2,     2,     1,     4,     2,     2,     5,     3,
       2,     2,     5,     3,     5,     3,     4,     6,     2,     1,
       2,     1,     2,     1,     1,     1,     2,     0,     2,     2,
       3,     3,     3,     2,     2,     2,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     4,     1,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     1,     1,     2,     2,     3,     1,     2,
       1,     1,     1,     4,     4,     4,     4,     1,     1,     1,
       1,     3,     1,     3,     1,     3,     0,     4,     0,     6,
       3,     0,     4,     1,     3,     3,     0,     4,     3,     0,
       1,     1,     2,     6,     1,     3,     0,     1,     4,     6,
       4,     1,     1,     1,     1,     1,     3,     0,     2,     1,
       2,     3,     4,     1,     1,     3,     4,     3,     5,     1,
       4,     3,     3,     0,     3,     3,     3,     0,     2,     2,
       0,     7,     4,     0,     6,     3,     2,     2,     2,     0,
       0,     9,     1,     0,     1,     0,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     4,     3,
       2,     3,     1,     0,     3,     0,     3,     2,     1,     1,
       0,     2,     3,     1,     4,     1,     3,     1,     1,     4,
       4,     2,     2,     3,     3,     1,     0,     1,     2,     3,
       2,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       1,     2,     2,     3,     2,     2,     4,     4,     2,     1,
       5,     4,     1,     0,     1,     3,     0,     1,     3,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     3,     4,
       4,     4,     4,     3,     1,     3,     1,     3,     2,     1,
       7,     0,     2,     1,     2,     0,     2,     4,     4,     2,
       4,     3,     3,     2,     2,     3,     1,     1,     2,     1,
       1,     2,     2,     4,     4,     3,     3,     2,     1,     1,
       2,     3,     3,     2,     2,     3,     1,     3,     3,     2,
       2,     3,     1,     4,     3,     4,     3,     1,     2,     2,
       2,     2,     2,     1,     2,     4,     4,     2,     1,     1,
       1,     1,     2,     4,     2,     2,     2,     2,     1,     2,
       2,     2,     2,     3,     1,     2,     3,     4,     2,     2,
       2,     2,     2,     2,     4,     2,     1,     2,     2,     3,
       1,     3,     2,     3,     2,     2,     3,     1,     3,     4,
       3,     2,     2,     1,     3,     2,     2,     1,     2,     3,
       1,     3,     1,     6,     4,     4,     3,     5,     3,     3,
       3,     2,     1,     1,     2,     2,     2,     0,     1,     1,
       2,     3,     1,     2,     0,     3,     0,     0,     5,     1,
       0,     2,     1,     1,     1,     2,     0,     4,     1,     0,
       0,     5,     0,     0,     7,     0,     0,     0,     0,    12,
       0,     0,     7,     0,     5,     0,     7,     0,     4,     2,
       2,     2,     3,     6,     8,    10,    12,     4,     3,     2,
       2,     1,     1,     1,     1,     1,     0,     0,     6,     0,
       0,     5,     1,     2,     0,     0,     5,     1,     1,     3,
       3,     2,     2,     2,     2,     2,     1,     2,     0,     1,
       0,     1,     1,     0,     1,     1,     3,     4,     1,     3,
       0,     1,     1,     1,     2,     2,     2,     1,     2,     2,
       0,     3,     1,     1,     1,     2,     2,     2,     4,     2,
       2,     2,     2,     2,     2,     1,     2,     1,     2,     1,
       1,     0,     0,     1,     0,     4,     2,     1,     1,     3,
       0,     3,     3,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
       2,     2,     2,     3,     2,     2,     4,     4,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,    13,    13,     1,     6,     0,     5,     0,   282,
     638,   639,     0,   389,   401,   583,     0,    12,     0,     0,
       0,    11,   487,   844,     0,     0,     0,   168,   670,   284,
     283,    84,     0,     0,   831,     0,    46,     0,     0,    14,
      26,     0,    28,     9,     0,    17,    16,    90,   111,    87,
       0,   640,   172,   303,   280,   304,   616,     0,   376,     0,
     375,   394,     0,   414,   393,   431,   400,     0,   502,   503,
     505,   509,   508,   482,   388,   599,   402,   600,   109,   302,
     627,   597,     0,   641,   581,     0,     0,   281,    82,    83,
     179,   644,   179,   645,   179,   285,   168,   141,   142,   143,
     145,   144,   473,   476,     0,   666,     0,   477,     0,     0,
       0,     0,   142,   143,   145,   144,    24,     0,     0,     0,
       0,     0,     0,     0,   478,   648,     0,   654,     0,     0,
       0,    38,     0,     0,    32,     0,     0,    48,     0,   646,
       0,   179,     0,     0,   614,   609,     0,     0,     0,   613,
       0,     0,     0,     0,   303,     0,   294,   585,     0,     0,
     302,   581,    29,     0,    27,     4,    47,     0,    65,   389,
       0,     0,     9,    68,    64,    67,    90,     0,     0,     0,
     400,    91,    15,     0,   429,     0,     0,   447,    88,    80,
     647,   585,     0,   581,    81,     0,     0,     0,   107,     0,
     410,   366,   596,   367,   608,     0,   581,   391,   390,    79,
     110,   377,     0,   412,   392,   108,   383,   407,   408,   378,
     396,   398,   387,   409,     0,    76,   432,   488,   489,   490,
     491,   507,   150,   149,   151,   493,   494,   173,   500,   492,
       0,     0,   495,   496,   510,   510,   525,   526,   584,   395,
       0,   426,   639,     0,   668,   172,   631,   632,   628,   602,
     642,     0,   601,   598,     0,   879,   875,   874,   872,   854,
     860,   859,     0,   866,   865,   851,   852,   850,   869,   858,
     856,   857,   855,   862,   861,   848,   849,   845,   846,   847,
     871,   853,   864,   863,     0,   870,   867,   777,   394,   778,
     840,   285,   282,   583,   307,   355,     0,     0,     0,     0,
     351,   349,   322,   353,   354,     0,     0,     0,     0,     0,
     284,   283,   276,     0,     0,   187,   186,     0,     0,   189,
     188,     0,   190,     0,     0,   180,   181,     0,   250,     0,
     253,   185,   306,   216,     0,     0,   308,   309,     0,   183,
     373,   394,   374,   633,   334,   324,     0,     0,     0,     0,
     179,     0,   475,     0,   470,     0,   667,   665,     0,   191,
     192,     0,     0,     0,   436,     4,    22,    30,   662,   658,
     659,   663,   661,   660,   141,   142,   143,     0,   145,   144,
     650,   651,   655,   652,   649,     0,   292,   293,   291,   630,
     629,    34,    33,    50,     0,   158,     0,     0,   394,   156,
       0,     0,   610,   612,     0,   611,   142,   143,   278,   279,
     298,     0,   620,   297,     0,   619,     0,   305,   284,   283,
       0,     0,     0,   296,   295,   624,     0,     0,    13,     0,
     168,    10,    10,    71,     0,    66,     0,     0,    72,    75,
       0,   428,   430,   123,    94,   128,   766,     0,    86,    85,
      93,   126,     0,     0,   124,    89,   626,     0,     0,   589,
       0,   834,     0,   594,     0,   593,     0,     0,     0,     0,
     581,   429,     0,    78,   585,   581,   607,     0,   380,   381,
       0,    77,   429,   385,   384,   386,   379,   399,   416,   415,
     179,   497,   501,   499,     0,   831,   504,   506,     0,     0,
     397,   429,   581,    95,     0,     0,     0,     0,   581,   101,
     582,   615,   639,   669,   172,     0,     0,   868,   873,   396,
     581,   581,     0,   581,   878,   179,     0,     0,     0,   223,
       0,     0,   225,   238,   239,     0,     0,     0,     0,     0,
     277,   222,   219,   218,   220,     0,     0,     0,     0,     0,
     306,     0,     0,     0,   217,   177,   178,   300,     0,   221,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   316,     0,   318,   321,   320,
     358,     0,     0,   359,   241,   241,     0,   227,   579,     0,
     235,   356,   348,     0,     0,   831,   337,   340,   341,     0,
       0,   368,   687,   683,   692,     0,   585,   581,   581,   581,
     370,   690,     0,   637,   372,     0,     0,   371,   336,     0,
     331,   350,   332,   352,   634,     0,   333,   176,   176,     0,
     166,     0,   394,   164,   576,   485,   574,   472,     0,     0,
     403,     0,     0,   404,   405,   406,   442,   443,   444,   441,
       0,   434,   437,     0,     4,     0,   653,   179,   656,     0,
      42,    43,     0,    54,     0,     0,    58,    62,    51,   830,
     825,     0,   373,   394,    54,   374,   829,    60,   169,   154,
     152,   169,   301,   176,   618,   617,   305,     0,   621,     0,
      19,    21,    90,    10,    10,    74,    73,     0,   129,   357,
       0,   714,    92,   712,   453,     0,   449,   448,   215,     0,
     214,   586,   625,     0,   807,     0,   802,   394,     0,   801,
     803,   832,   814,     0,     0,   623,   592,   591,     0,     0,
     606,     0,   424,   423,   411,   605,     0,   834,   595,   382,
     413,   425,   429,     0,   498,   511,   549,   639,     0,   540,
       0,     0,   552,     0,   122,   117,     0,   172,   553,   556,
       0,   532,     0,   120,     0,   429,     0,   427,   834,   800,
     179,   179,   643,   179,   834,   800,   581,    98,   581,   104,
     877,   876,   840,   840,   840,     0,     0,     0,     0,   637,
       0,     0,     0,     0,   394,     0,     0,     0,   312,     0,
     310,   311,     0,   248,   182,   282,   638,   639,   284,   283,
       0,     0,   454,   483,     0,   274,   275,   792,   791,     0,
     272,   271,   269,   270,   268,   267,   266,   264,   265,   263,
     262,   261,   256,   257,   258,   259,   260,   255,   254,     0,
       0,     0,     0,     0,   241,   229,   245,     0,     0,   228,
     581,   581,     0,   581,   578,   677,     0,     0,     0,     0,
       0,   339,     0,   343,     0,   345,     0,   686,   685,   682,
     681,   830,     0,     0,   701,     0,     0,   834,   369,   834,
     688,   581,   800,   585,   687,   683,     0,     0,   581,     0,
     393,     0,     0,     0,     0,   171,   175,   286,   169,   162,
     160,   169,     0,   486,     0,   485,   213,   212,   211,   210,
     436,     0,     0,    25,     0,     0,   657,     0,    39,    45,
      44,    56,    53,    54,     0,    49,     0,     0,   687,   683,
       0,   821,   581,   824,   826,     0,   822,   823,    55,   493,
       0,   159,   169,   169,   157,   170,   299,    18,    20,    70,
      90,   417,   146,   638,   639,   134,   148,   147,     0,   127,
     130,     0,   640,     0,     0,     0,     0,   713,   707,   450,
       0,   125,   590,   587,   806,   820,   809,     0,   588,   805,
     819,   808,   804,   833,   816,   827,   817,   810,   815,   836,
       0,   421,   604,   603,   420,   176,   831,     0,   831,   512,
     513,   515,   831,   518,   517,   581,   800,   548,   541,   553,
     542,   429,   429,   538,   539,   536,   537,   581,   800,   282,
     638,     0,   416,   118,   544,   554,   559,   560,   416,   416,
       0,     0,   416,   116,   545,   557,   416,   533,   534,   535,
     429,   531,   479,     0,    97,     0,     0,     0,     0,   103,
       0,   834,   800,   834,   800,   842,   841,   843,   287,   323,
     224,   226,   329,   330,     0,     0,     0,     0,   311,   314,
       0,     0,     0,     0,   249,     0,   315,   317,   319,     0,
       0,     0,     0,   230,   247,     0,     0,   674,   672,     0,
     675,   585,   236,     0,     0,   179,   346,     0,     0,     0,
     684,   680,   691,   581,   700,   698,   699,   689,   834,     0,
     696,     0,   635,   636,     0,   335,   167,   169,   169,   165,
     577,   575,   474,     0,   435,   433,   282,     0,    23,    31,
     664,    57,    52,    59,    63,   686,   682,   687,   683,     0,
     597,     0,   581,   688,    61,   155,   153,    69,     0,   132,
       0,   136,     0,   138,     0,   140,     0,   767,     0,   203,
     715,     0,   708,   709,     0,   451,   687,   683,     0,   306,
       0,   633,   828,     0,     0,   837,   838,     0,     0,   418,
     174,   522,     0,   521,   831,   831,   831,     0,   834,     0,
     543,   447,   447,   834,     0,     0,     0,   429,   429,     0,
     429,   429,     0,   429,     0,   447,   467,   581,   289,   288,
     290,   581,   100,     0,   106,     0,     0,     0,     0,     0,
       0,   457,     0,   455,   252,   273,   243,   242,   240,   231,
       0,   244,   246,   673,   671,   678,   676,     0,   237,     0,
       0,   338,   342,   344,   834,   694,   581,   695,   163,   161,
     471,     0,   438,   440,   686,   682,   602,   688,   133,   131,
       0,     0,     0,     0,   445,     0,     0,   282,   638,   639,
     716,   729,   732,   735,   740,     0,     0,     0,     0,     0,
       0,     0,     0,   283,   761,   769,     0,   788,   765,   764,
     763,     0,   724,     0,     0,   394,     0,   703,   722,   728,
     702,   723,   762,     0,   710,   452,     0,   636,   818,   812,
     813,   811,     0,   835,   422,     0,     0,     0,     0,   524,
     523,   516,   113,   581,   547,   551,   115,   581,   429,   429,
     568,   447,   282,   638,     0,   555,   561,   562,   416,   416,
     447,   447,     0,   447,   558,   546,     0,   834,   834,   581,
     581,     0,     0,     0,     0,   456,     0,     0,   232,   233,
     679,   347,   288,   697,   834,     0,   135,   137,   139,   774,
     768,   772,     0,   711,   706,   206,   781,   783,   784,     0,
       0,   720,     0,     0,     0,   747,   749,   750,   751,     0,
       0,     0,     0,     0,     0,     0,   782,     0,   365,   789,
       0,   725,   363,   416,     0,   364,     0,   416,     0,     0,
       0,   204,   705,   704,   726,   760,   759,   311,   839,   419,
     519,   520,   514,   834,   834,   567,   564,   566,     0,     0,
     429,   429,   429,   563,   565,   550,   469,     0,   468,   463,
      96,   102,   834,   834,   325,   326,   327,   328,   458,     0,
     234,   693,   439,     0,   773,   446,   195,     0,   717,   730,
     719,     0,     0,     0,     0,     0,   743,     0,   752,     0,
     758,    40,   145,    35,   145,     0,    36,   770,     0,   361,
     362,     0,     0,     0,   360,   205,   720,   112,   114,   429,
     429,   573,   447,   447,     0,     0,   481,    99,   105,   580,
       0,   775,   202,     0,   394,     0,   720,     0,   733,   721,
     707,   786,   736,     0,     0,     0,     0,   748,   757,    41,
      37,     0,     0,   727,   572,   570,   569,   571,   466,   465,
     459,    87,    90,     0,     0,     0,   196,   416,   718,   207,
     731,   209,     0,   787,     0,   785,   741,   745,   744,   771,
     793,     0,     0,   464,   779,   780,   776,   429,   707,   193,
       0,     0,   199,     0,   198,   720,     0,     0,     0,   794,
     795,   753,   462,     0,   461,     0,   208,     0,   734,   737,
     742,   746,     0,   793,     0,     0,   460,   200,   194,     0,
       0,     0,   754,   796,     0,     0,   797,     0,     0,   201,
     738,   798,     0,   755,     0,     0,     0,   739,   799,   756
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   438,     3,   439,   167,   711,   333,   183,     5,
       6,    39,   674,   375,  1308,   675,   772,  1309,  1310,   395,
    1414,   679,    43,   773,   404,   685,   941,   686,   687,   688,
      45,   174,   175,    46,   457,   186,   182,    47,    48,   789,
    1072,   795,  1074,    49,   775,   776,   187,   188,   458,   718,
     979,   980,   654,   981,   236,    50,   963,   962,   701,   698,
    1138,  1137,   921,   918,   138,   961,    51,   238,    52,   915,
     567,   334,   335,   336,   337,  1311,  1581,  1478,  1583,  1523,
    1614,  1180,  1560,  1578,   369,   907,   338,  1249,   862,   606,
     869,   339,   340,   370,   342,   360,    54,   257,   680,   420,
     156,    55,    56,   343,   562,   344,   345,   346,   347,   459,
     348,  1312,   498,   628,   349,  1313,    58,   219,   691,   350,
     220,   540,   221,   199,   212,    62,   481,   499,  1335,   752,
    1198,   200,   213,    63,   511,   753,    64,    65,   670,   671,
     672,  1285,   464,   832,   833,  1551,  1552,  1516,  1458,  1366,
      66,   658,   363,  1226,  1459,  1093,   924,    67,    68,    69,
      70,    71,   244,   245,    72,    73,   506,  1019,  1020,  1021,
    1022,   247,   508,   509,   786,   780,   781,   782,  1044,  1054,
    1045,  1355,  1046,  1047,  1356,  1357,   655,   656,   607,   897,
     352,   467,   468,   193,   201,    75,    76,    77,   202,   144,
     145,   159,    79,   134,   353,   354,   355,    81,   356,    83,
    1024,   125,   126,   127,   516,   107,    84,   357,   874,   875,
     892,   631,  1316,  1317,  1181,  1182,  1183,   722,  1318,   988,
    1319,  1399,  1526,  1481,  1482,  1320,  1321,  1506,  1400,  1527,
    1401,  1562,  1402,  1564,  1609,  1624,  1403,  1585,  1536,  1586,
    1487,   460,   719,  1283,  1322,  1417,  1541,  1390,  1391,  1473,
    1555,  1525,  1521,  1323,  1532,  1420,   839,  1588,  1589,  1590,
    1622,   738,   739,  1008,  1194,  1331,   740,   741,   742,  1004,
     743,   150,  1006,   745,  1196,  1197,   534,    86,    87
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1415
static const yytype_int16 yypact[] =
{
     149,   229,   265, -1415, -1415, -1415, 10477, -1415,    58,    39,
     219,   332,   138,   239, -1415, -1415,   744, -1415,   253,   285,
     295, -1415, -1415, -1415,  1130,  1379,   889,   352, -1415,   620,
     393, -1415,  1802,  1802, -1415,  4055, -1415, 10477,   406, -1415,
   -1415,   470, -1415,   210, 10535, -1415, -1415,   497,   383,   526,
     516,   571, -1415, -1415, -1415, -1415,   384,  2547, -1415,  5045,
   -1415,  2065,   311, -1415,   619, -1415, -1415,  1389,   462, -1415,
   -1415, -1415, -1415,   595,  3778, -1415, -1415, -1415,   564, -1415,
   -1415, -1415,   697, -1415, -1415,   841,  7934,   585, -1415, -1415,
    9553, -1415,  9553, -1415,  9553, -1415, -1415, -1415,   219,   332,
     624,   393,   574,   658,   571, -1415,   589, -1415,   841,  9638,
    9638,   650, -1415, -1415, -1415, -1415, -1415,    82,   657,   636,
     653,   696,   673,   704, -1415, -1415,  1705, -1415,  1050,   219,
     332, -1415,   624,   393, -1415,  1619,   966,   754, 10828, -1415,
    9553,  9553,  4289,  3931, -1415, -1415,   807,   925,  3931, -1415,
    1205,  4309,  4309,  4055,   662,   722, -1415,   745,   737,   734,
     743, -1415, -1415,   849, -1415,   759, -1415, 10593, -1415, -1415,
     352,  5201,   774, -1415, -1415, -1415,   497,   625, 10886,   467,
     814, -1415, -1415,   784,   619,   878,    94,   519,   825, -1415,
   -1415,   793,   192, -1415, -1415,  4422,  4422,  4542,   564,   893,
   -1415, -1415,   567, -1415, -1415,  1098, -1415, -1415, -1415, -1415,
   -1415,  2065,  1000, -1415,   619,   564, -1415, -1415, -1415,  2404,
    2065, -1415,   619, -1415,   625, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415,   833, -1415,   571, -1415,   619,
    1770,  1242, -1415, -1415,   846,   846, -1415, -1415, -1415,  1183,
     841, -1415,   828,  1592, -1415,   326, -1415, -1415, -1415, -1415,
   -1415,  4611, -1415, -1415,   501, -1415,   824,   832, -1415, -1415,
   -1415, -1415,   853, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415,   826, -1415, -1415, -1415,  1183,  3778,
    1030, -1415, -1415,   856, -1415, -1415, 10239, 10324, 10409, 10409,
   -1415, -1415, -1415, -1415, -1415,   863,   886,   891,   901,   905,
     624,   999,  9723,  1265, 10409, -1415, -1415, 10409, 10409, -1415,
   -1415,  3227, -1415, 10409,   419,   903, -1415, 10409, -1415,  9808,
   -1415, 11139,   353,  1941,  2775,  9893, -1415,   979,  3627, -1415,
    1035,  2243,  2707, -1415,   422, -1415,  1921,  2346,   419,   419,
    9553, 10828, -1415,  1265,   883,  1265, -1415, -1415,   890,   941,
   11072,   895,   897,   902,  1650,   759, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415,   657,   636,   653,  1265,   696,   673,
     939,   704, -1415,   961, -1415,  2113,   219,   332, -1415, -1415,
   -1415, -1415, -1415, -1415,  6202, -1415,   625,  6328,  2214, -1415,
     705,   419, -1415, -1415,   616, -1415,   948,   951, -1415, -1415,
   -1415,  3931, -1415, -1415,  3931, -1415,   919, -1415, -1415, -1415,
     737,   737,   737, -1415, -1415, -1415,  4611,    54,   922,   927,
   -1415, -1415, -1415, -1415, 10828, -1415,  1072,  1101, -1415, -1415,
     849, -1415,   619, -1415, -1415, -1415, -1415,    96, -1415, -1415,
   -1415, -1415,  7597,  9723, -1415, -1415, -1415,  9723,   932, -1415,
    6839,   121,  4874, -1415,  4874, -1415,  5823,  5823,  4542,   956,
   -1415,   619,   625, -1415,   938, -1415, -1415,  5878,  2404,  2065,
     625, -1415,   619, -1415, -1415,   619,  2404, -1415,  1029, -1415,
    9553,   833, -1415, -1415,  1770, -1415, -1415, -1415,  3869,    68,
    1183,   619, -1415, -1415,   994,  1001,  1024,  1022, -1415, -1415,
   -1415, -1415,   885, -1415,   444,  1002,  1005, -1415, -1415,  1183,
   -1415, -1415,   822, -1415, -1415,  9553,  9723,   856,  3227, -1415,
     469,  3227, -1415, -1415, -1415,  9638,  3937,  3937,  3937,  3937,
   11117, -1415, -1415, -1415, -1415,  1010,  9984,  9984,  3227,  1016,
     222,  1021,  1079,  1033, -1415, -1415, -1415, -1415,  9553, -1415,
    7140,  3227, -1415,  9723,  9723,  7687,  9723,  9723,  9723,  9723,
    9723,  9723,  9723,  9723,  9723,  9723,  9723,  9723,  9723,  9723,
    9723,  9723,  9723,  9723,  9723, -1415,  9723, -1415, -1415, -1415,
   -1415,  9723,  9723, -1415, -1415, -1415,  3641,   592,   542,  8293,
   -1415, -1415, -1415,  1089,  1592,  1140,   594,   605,   644,  2005,
     925, -1415,  1370,  1370, -1415,  3036,  1040,  1064,  1122, -1415,
   -1415,   641,  8844,   360, -1415,  1013,   841, -1415, -1415,  9723,
   -1415, -1415, -1415, -1415, -1415,   109,   585, -1415, -1415,   419,
   -1415,   625,  2642, -1415,  1108,  1117, -1415, -1415,  1265,   939,
   -1415,  8028,  8118, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
     169, -1415,  1097,  1086,   759,  2113,  1127,  9553, -1415,  1145,
   -1415, -1415,   966,  1385,  1154,   329,  1143,  1149, -1415, -1415,
    2183, 10886,  2183,  4191,  1389,  5667, -1415,  1159, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415, -1415,  1116,  1121, -1415,  1165,
   -1415, -1415,   497, -1415, -1415, -1415, -1415,    70,   752,  1168,
    1079, -1415, -1415, -1415, -1415,  7047, 11117, -1415,   941,  1128,
   11072, -1415, -1415,  1131, -1415,  1144,   299,  5377,  1147, -1415,
     237, 10652,  1191,  1201,   647, -1415, -1415, -1415,  4874,  4874,
   -1415,  5878, -1415,  1211, -1415, -1415,  1162,   121, -1415,  2404,
   -1415, -1415,   619,  1202, -1415,  2961, -1415,   907,   931, -1415,
    9723,  3137, -1415,  3137,   348,   348,   438,   652,  4748, 10710,
    5499, -1415,   174,   348,  1213,   619,  6087, -1415,   121, 10997,
    9553,  9553, -1415,  9553,   121, 10997, -1415, -1415, -1415, -1415,
   -1415, -1415,   767,   767,   767,   419,  1172,  1174,  9208,  1122,
    1175,  1177,  1178,  1215,  3331,  1219,  1232,  1235, -1415,  1179,
   -1415, -1415,  1203, -1415, -1415,  1253,   312,   603,   151,   437,
    9723,  1254, -1415,  1259,  1216, 11117, 11117, -1415, -1415,  1263,
   11157, 11174, 11190, 11205,  4770,  3406,  3812,  2123,  2123,  2123,
    1331,  1331,  1221,  1221,  1081,  1081,  1081, -1415, -1415,  1223,
    1224,  1226,  9723,  9638, -1415,   592, -1415,  7597,  9723, -1415,
   -1415, -1415,  9723, -1415, -1415,  1241, 10409,  1231,  1260,  1271,
    1299, -1415,  9723, -1415,  9723, -1415,  9723,  1678, -1415,  1678,
   -1415,   112,  1252,  1255, -1415,  1239,  3937,   121, -1415,   121,
    2316, -1415, 10997,  1251,  9026,  9026,  6261,  1256,  9808,  1258,
     737,  1894,  2346,  1042,  1261, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415,  9723,  1265,  1262,  1117, -1415, 11117, -1415, 11117,
    1650,  1264, 10069, -1415,  1267,  1310, -1415,   419, -1415, -1415,
   -1415, -1415, -1415,  2881,  6202, -1415,  3937,  9553,  1900,  1900,
    3463, -1415, -1415, -1415, -1415,  1098, -1415, -1415, -1415,   989,
    9723, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
     497, -1415,   657,   636,   653, -1415,   696,   673,  9723,  1319,
   -1415,   667,   704,   728,   757,  1344,  1079, -1415,    66, -1415,
      49, -1415, -1415, -1415, -1415, -1415, -1415,  8935, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415,  1201,  1328, -1415, -1415, -1415,
    3937, -1415, -1415, -1415,  1340, -1415, -1415,  1326, -1415,  1360,
   -1415, -1415,   578, -1415, -1415, -1415, 10997, 11117, -1415,  2477,
   -1415,   619,   619, -1415, -1415, -1415, -1415, -1415, 10997,   524,
     781,  9723,  1029, -1415,  1362, -1415, -1415, -1415,   198,   366,
     697,   925,   377,   348,  1363, -1415,   423, -1415, -1415, -1415,
     619, -1415, -1415,  8361, -1415,  1322,   419,   419,   419, -1415,
    1329,   121, 10997,   121, 10997, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415,  1346,  1352,  1356,  1359,  1116, -1415,
   11005,  7597,  7233,  1343, -1415,  9723, -1415, -1415, -1415,  1347,
    1345,  1353,  3937, -1415, -1415,  1364,   255,   205,   205,  1357,
     205, -1415, -1415, 10409,  1447,  9553, -1415,  1365,  1366,  1371,
   -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415,   121,  1372,
   -1415,  1374, -1415, -1415,  1911, -1415, -1415, -1415, -1415, -1415,
   11117, -1415, -1415,  1358, -1415, -1415,   245,  1375, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415,  2851,  2851,  2966,  2966,  3463,
   -1415,  1098, -1415,  3526, 11139, -1415, -1415, -1415,  1381, -1415,
     752, -1415,  9723, -1415,  9723, -1415,  9723, -1415,  1265, -1415,
   -1415,  6637,  1437, -1415,  7323, -1415,  9117,  9117,  6745,   140,
    1387,   425, -1415,  7597,  7413, -1415, -1415,   259,  7597, -1415,
   -1415, -1415,  9638, -1415, -1415, -1415, -1415,  1351,   121,  1388,
   -1415,  1419,  1419,   121,  1390,  9723,  9723, 10916,   619,  5724,
     619,   619,  1210,   619,  5938,  1419, -1415, -1415, -1415,  1433,
   -1415, -1415, -1415,  1397, -1415,  1399,  9723,  9723,  9723,  9723,
    7597, -1415,  1448, -1415, -1415, 11117, -1415, -1415, -1415,   569,
    1353, -1415, -1415, -1415, -1415, -1415, -1415,  1401, -1415,  1466,
     419, -1415, -1415, -1415,   121, -1415, -1415, -1415, -1415, -1415,
   -1415,  9723, -1415, -1415,  2851,  2851, -1415,  3526, -1415, -1415,
    1407,  1408,  1410,  1428, -1415,  1107,   318,  1462,   837,   851,
   -1415, -1415, -1415, -1415, -1415,  9723,  1474,  1477,  1478,  9293,
     451,  1265,   430,   656, -1415, -1415,  9383,  1530, -1415, -1415,
   -1415,  1481, -1415,  5599, 10770,  4975,  6527, -1415, -1415,  1527,
   -1415, -1415, -1415,  8454, -1415, -1415,  1443,   420, -1415, -1415,
   -1415, -1415,  3937, -1415, -1415,  7597,  1444,  1449,  2961, -1415,
   -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, 10916, 10916,
   -1415,  1419,   794,   869,  9723, -1415, -1415, -1415,  1029,  1029,
    1419,  1419,   885,  1419, -1415, -1415,  6395,   121,   121, -1415,
   -1415,  1455,  1456,  1465,  1467, -1415,  7597,  9723, -1415,   569,
   -1415, -1415, -1415, -1415,   121,  1468, -1415, -1415, -1415, -1415,
    1428, -1415,  1265, -1415, -1415, -1415, -1415, -1415, -1415,   792,
     792,  1079,  1453,  1463,  5091, -1415, -1415, -1415, -1415,  1496,
    9723,  1510,  1511,  1524,  1545,  1693, -1415,  1079, -1415, -1415,
    1487, -1415, -1415,  1029,  1120, -1415,  1141,  1029,  9468,  1157,
     324, -1415, -1415, -1415, -1415, -1415, -1415,   525, -1415, -1415,
   -1415, -1415, -1415,   121,   121, -1415, -1415, -1415,  9723,  9723,
   10916,   619,   619, -1415, -1415, -1415, -1415,  8208, -1415, -1415,
   -1415, -1415,   121,   121, -1415, -1415, -1415, -1415, -1415,  1480,
   -1415, -1415, -1415,  1497, -1415, -1415, -1415,  9638, -1415, -1415,
   -1415,  1583,  8750,  6949,  9638,  9723, -1415,  8562, -1415,  1544,
   -1415, -1415,  1551, -1415,  1524,  1545, -1415, -1415,   849, -1415,
   -1415, 10154, 10154,  7507, -1415, -1415,  1079, -1415, -1415, 10916,
   10916, -1415,  1419,  1419,  1500, 11027,  1513, -1415, -1415, -1415,
   11010, -1415, -1415,  1503,   190,   625,  1079,  8656, -1415, -1415,
      66, -1415, -1415,  1548,  1504, 11095,  8562, -1415, -1415, -1415,
   -1415,  1428,    81, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
   -1415,   526,   497,  1505,  1506,  1079, -1415,  1029, -1415, -1415,
   -1415, -1415,   811, -1415,  7777, -1415, -1415, -1415, -1415,  1428,
    1605,  1557,   193, -1415, -1415, -1415, -1415,   619,    66, -1415,
    9723,  1559, -1415,  1561, -1415,  1079,  8562,  1534,    51,  1570,
   -1415, -1415, -1415,    96, -1415,  1569, -1415,  1526, -1415, -1415,
   -1415, -1415,  9723,  1605,  1575,  1605, -1415, -1415, -1415,  7867,
    1531,    95, -1415, -1415,  7597,  1532, -1415,  1627,  1585, -1415,
   -1415, -1415,   382, -1415,  8656,  1632,  1587, -1415, -1415, -1415
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1415, -1415,  1652, -1415,  -333,  1484,  -394,    17,     4,  1663,
   -1415,  1631, -1415, -1415,   114, -1415,   264, -1415,   275, -1415,
     256,   995,    20,    13, -1415, -1415,  -638, -1415, -1415,   735,
      47,  1507,  1236,  1517,  -706,   134,  -175,    -7,   148, -1415,
   -1415, -1415, -1415, -1415,   911, -1415, -1415, -1415, -1415, -1415,
   -1415,   521,  1146, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415,  1593,  -582,  5825,  1454,   -49,  -602,
    -101,   -70,  1555,  -538, -1415,   682, -1415,   300, -1415, -1394,
   -1415, -1397,    77, -1415,  1218,  1294,  -272,   452,  -535, -1415,
    -829,  1686,     7,  1552,  3607,  1411,  -329,   -72,   -76,   680,
    -141,   -67,    91, -1415, -1415, -1415,  -328, -1415,  -155, -1415,
   -1415, -1230,   -50,  -340,  4852,   136,   910,  -113,    79,   100,
    -205,    -6,  -146,  -168,  -174,    14,   -23,   120, -1415,  -370,
   -1415, -1415, -1415, -1415, -1415,   211,  1306,     8, -1415,   785,
   -1415, -1415, -1097,  -379,   986, -1415, -1415, -1415, -1415, -1415,
     -18, -1415, -1415, -1415, -1415, -1415,   797,  -352, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415, -1415,  1472, -1415,   380,   512,
   -1415, -1415, -1415, -1415, -1415,   937,  -671, -1415, -1415, -1415,
   -1415, -1415, -1415,   947, -1415,   504,  1071,   808,  1133,  4227,
      45,    19,  -457,  1538,  1269,  -669, -1415,     2, -1415,   820,
    4043,  -149,   171,  -104,  4531,  1396, -1415,  4888,  2552,    73,
     -20,  -116, -1415,  1607,   -81, -1415,  4712,  3373,   -31, -1415,
    1976, -1415, -1415,   435, -1415, -1415,   570,   162,  -441, -1415,
   -1415, -1415, -1415, -1358, -1415, -1187, -1388, -1415, -1415, -1415,
   -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415, -1415,
   -1415,   187, -1415, -1415, -1415, -1415, -1415,   224, -1324, -1415,
   -1415,   -47, -1415, -1415, -1415, -1415, -1414,   160, -1415,   161,
   -1415,  -678,  -585,   762, -1415, -1415, -1415, -1415,  -390, -1415,
    -384,  -443, -1415,  1611,   440, -1415,  -214, -1415,  -232
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -831
static const yytype_int16 yytable[] =
{
      61,   445,   422,   425,   447,   124,   489,   198,   437,   446,
      38,   633,   394,   986,   696,   258,   723,   434,   105,    44,
     697,   517,   358,    37,   359,   184,   180,   756,   642,   643,
     824,    61,   402,   255,   539,   542,  1103,   176,    61,   300,
     893,    38,   673,   223,   510,   393,   916,   893,   712,   983,
      44,    74,   694,   211,    37,   251,   958,    44,   179,   400,
     401,   171,   765,   399,   172,   611,  1474,   572,   249,  1533,
     863,   411,   226,   494,   497,   192,  1418,   143,   148,   258,
     298,   611,    74,   727,   351,    59,   351,   262,   351,    74,
    1534,   173,   611,   529,  1529,   454,  1023,   720,   488,  1178,
    1028,   965,  1030,   351,   351,   784,    60,   496,  1184,  1057,
     366,  1065,    90,  1603,  1344,  1345,    59,  1070,    88,   964,
      40,   822,   761,   178,   994,   646,   157,   198,  1365,  1433,
     520,   299,   408,  1563,   351,   351,  1436,    60,   258,  1561,
     376,   787,    57,  1570,    60,   377,   529,   310,  1543,    -2,
     226,    40,  -357,   180,   721,   260,   455,  1617,  1185,  1604,
     442,    61,   709,    28,   176,    61,   226,    89,  1558,   895,
    1584,   995,   211,    57,   996,  1179,   192,   785,   971,   260,
     177,  1596,   744,    74,    44,   179,   297,  -303,   171,  1571,
     456,   172,   492,   223,  1592,  1615,   421,   424,   217,   218,
     809,   223,   223,  1618,    16,   139,   160,   210,   260,   129,
     130,    95,    74,  -145,   520,   524,    74,   407,   173,   260,
     823,   192,   226,  -366,  1129,    17,    22,  1600,   930,     4,
     226,   223,  -303,  -303,  1058,    25,  1561,   621,    60,   469,
     472,   474,   157,   157,   157,  1474,    59,   226,  -296,   999,
     178,  -357,   262,  1531,  1447,   455,  -366,   647,   648,    28,
    -366,   132,   133,  1453,  1454,    -3,  1455,    60,   165,   595,
      41,    60,   617,    91,   406,   870,   616,   931,   260,   987,
     223,    42,   808,  1059,   470,   871,   405,    96,   157,   456,
     649,   681,    92,   249,  -366,   717,  1000,   608,   872,  1001,
    1537,    41,   621,    57,  1271,  1151,   676,   177,   166,   702,
     703,   994,    42,   260,   596,   543,   544,   643,  1332,   969,
     970,   893,   160,   160,   160,   351,   210,   422,   425,  1102,
    -296,   552,   943,   223,   553,   554,  1136,   260,   298,  1139,
     564,   934,   494,   497,   569,   109,   249,   808,  1209,  1568,
     494,   696,   610,  1272,   351,   652,   699,  1005,   995,   262,
    1214,   996,  -830,  1252,   497,   893,    91,  1333,   160,  -581,
     224,   225,  -581,   518,  -142,    17,   759,   110,  1394,  -416,
    1165,  1166,   517,   497,  1394,    92,    93,   111,   944,   299,
     223,  -367,  1057,    17,  1233,   451,  1235,   633,   693,  1601,
     595,   211,  -121,   945,    17,    94,    74,   898,  -416,  1154,
      17,  -416,   893,  1200,  -416,  1546,  1547,   684,   519,  -581,
    1378,  -581,  -581,  -581,  -367,   137,   180,  1395,  -367,   260,
     763,  -585,  -305,  1505,  -581,  -121,  -581,   714,    61,  -121,
     407,  1625,  -416,  -416,   297,   596,  1131,  -305,  -119,   695,
      17,  -305,  -581,  -581,    97,   112,   113,  -416,   179,   777,
     226,    60,  -367,  1033,   737,   805,   141,  -305,  -581,   638,
     223,   223,  -627,  -121,   131,   762,  -585,   191,   223,  -305,
    -305,  -119,  -305,  -305,    28,  -119,  1413,   260,  1104,    74,
    1626,   798,   223,   565,   351,   566,  1034,   651,   163,  -144,
    1035,   983,    61,   226,   692,    10,   522,   114,   115,   650,
     141,   223,  -305,  -305,   639,   695,   621,  -627,  -627,  -119,
     242,   748,   749,   178,   243,   771,   224,   448,  -305,   351,
     164,  1410,   351,  -627,  1036,   351,   799,   968,  1023,   351,
     814,   814,   814,   814,    60,  1177,   129,   130,   917,   365,
    1470,   185,   351,    74,   696,  1268,  1269,   132,   133,   608,
     697,   808,   351,   552,   553,   351,   461,   157,   157,   157,
     692,   681,  -305,  1201,   893,  1203,   189,   260,   940,  1207,
     177,   523,   462,   642,   643,  1205,  1215,   779,  1075,  1076,
    1077,    17,   694,    10,    11,  -416,    28,    90,   132,   133,
     298,   919,   754,   893,   181,   260,   939,   937,    60,   529,
     760,   463,   870,   494,  -585,  1206,   866,  -305,  -305,   693,
     129,   130,   871,  -416,  -416,   190,   737,  -416,     9,    10,
      11,    12,   867,  -299,   223,   872,   572,   365,   262,   866,
     951,   881,   956,   957,   778,   132,   133,   160,   160,   160,
      20,   299,   883,   246,  1257,   867,   774,    93,   301,  -585,
     484,   868,  1377,   124,   365,  -143,    23,   887,   889,  1023,
     695,   351,   132,   133,   139,   223,    94,   695,   139,    28,
     646,    29,    30,   362,   868,   211,   882,   957,   901,   249,
     379,   885,   260,   140,  1009,   195,   523,   884,   984,  1037,
       9,    10,   252,    12,  1078,   196,   297,   380,   260,    92,
      34,   378,  1241,  1243,  1171,   155,   364,   197,  1416,   223,
    1066,  1067,   777,  1068,   777,   692,    94,   382,  1043,   141,
     777,   777,   692,   902,   903,   693,   886,   777,    23,  1010,
     302,   223,   374,    12,  1038,   253,   141,    97,    98,    99,
     381,  1031,  1032,    29,    30,   972,   973,   974,   383,  1172,
    1060,  1338,  1339,  1340,   568,    61,   809,    61,    20,   434,
    -296,   129,   130,   211,    61,  1173,   520,   254,    23,   565,
      61,   566,    34,   693,   351,   351,   695,   351,   771,   693,
     771,   368,   372,   428,   429,  1167,   223,   771,    28,   975,
     100,   101,   102,   771,  1175,  1243,    28,   430,   976,   977,
       9,   129,   130,    12,  1328,  1330,    74,   431,    74,  1334,
    1174,    28,    34,   132,   133,    74,   129,   130,   403,   432,
     427,    74,   423,   426,   695,    91,  1150,   530,   191,  1476,
     695,   692,   435,  1216,   978,    10,    11,   531,    23,  1176,
     779,  -622,   779,   149,    92,   253,  1448,   351,  1579,   779,
     305,  1375,  1106,    29,    30,   779,  1593,    90,    -8,   394,
     365,    60,   166,    60,   449,   512,   450,   155,   132,   133,
      60,   453,    93,  1112,  1477,   465,    60,   254,   258,   692,
     814,    91,    34,   129,   130,   692,   693,   132,   133,  1397,
     737,    94,   254,  1580,   466,    93,   500,   778,   505,  1029,
      92,   552,   553,  1398,   262,   527,   778,   525,   226,   774,
     513,   774,   778,    91,    94,   526,  1042,  1052,   774,   129,
     130,  1449,   796,   131,   774,   129,   130,   528,   693,    93,
     814,   351,    92,    28,   693,   132,   133,   695,   536,   887,
     889,   695,   482,   483,  1025,   545,  1439,   684,    94,   546,
    1480,    93,   568,   413,   547,  1228,  1229,  1230,   415,   302,
     396,   397,    12,  1014,   548,   809,  1497,   797,   549,  1043,
      94,   132,   133,   258,   260,    28,   260,   132,   133,   695,
     611,   737,   657,  1155,  1156,   695,  1062,  1468,   660,  1026,
     661,   255,   692,   663,   814,   664,   692,    23,   422,   425,
     665,   561,   677,   559,   777,   678,   149,    10,    11,  1218,
     693,    92,   320,    30,    94,  1220,  1221,   706,   260,  1218,
    1223,    -7,   693,  1221,   129,   130,   710,   422,   425,   129,
     130,   157,   695,   732,   692,  1260,    10,    11,   -53,   755,
     692,    34,   -53,   118,   119,   120,    17,    61,  -184,   490,
     491,   365,   500,   -53,   750,  1480,   693,   790,   693,   132,
     133,   695,   141,  -184,   791,  -184,  1253,  1254,   792,  1256,
     771,   521,   621,   695,    28,  1480,   132,   133,   157,    28,
     365,   132,   133,   254,   258,   793,   814,   692,   132,   133,
     530,     9,    10,    11,    12,   622,   121,   122,    74,   351,
     531,   423,   707,   800,  1576,   623,   801,   695,   818,   695,
    1258,   160,   523,   260,   820,   624,   692,   625,   626,   821,
     696,   482,   715,    97,   112,   113,  1554,   721,   692,    23,
    1426,   823,   779,   878,  1480,  1424,   253,  1429,   880,   731,
     984,   894,   723,   693,    29,    30,   896,   423,   426,  1382,
     490,   716,   103,    60,   593,   594,  1392,  1393,   160,   898,
     117,   922,   692,   524,   692,  1315,   923,  1042,   254,   482,
    1499,   936,   737,    34,   809,  1307,   114,   115,   116,   932,
      22,   217,   218,   552,   553,   933,   351,    16,  1306,   778,
     490,  1500,  1274,  1275,   695,   938,   946,   260,    97,   416,
     417,   774,   947,   235,   129,  1362,   482,  1504,   561,    22,
     559,   561,   960,   559,  -299,   967,    74,   811,    25,   966,
     455,  1274,  1275,   695,   260,  1619,   991,   423,   819,   992,
     559,   704,  1211,  1212,   705,    97,    98,    99,   157,   157,
     157,   561,   993,   559,  -829,   998,   708,   838,   365,   692,
    1314,   114,   101,  1423,  1007,  1423,   132,   133,    97,   112,
     113,  1225,   390,  1012,  1011,  1061,  1015,   157,   157,   157,
    1079,    60,  1080,  1081,   861,  1082,  1083,  1088,   692,  1084,
     523,   877,   413,  1085,   415,   260,   418,   223,   100,   101,
    1315,   590,   591,   592,   593,   594,  1086,   521,   211,  1087,
    1315,  1089,   909,   564,   690,  -141,  1091,  1315,  1092,   529,
    1307,   114,   115,  1306,  1094,  1095,   814,  1307,   160,   160,
     160,  1096,  1097,  1306,  1111,  1451,  1452,  1098,   142,   142,
    1306,   158,  1113,  1542,  1115,  1114,  1116,   118,   973,   974,
    1124,    74,   422,   425,   118,   973,   974,   160,   160,   160,
    1122,    74,  1130,  1123,  1132,   214,  1133,   222,    74,  1135,
    1149,  1142,  1145,   239,   129,   130,  1148,  1573,  1170,    15,
     690,  1017,   118,   119,   120,  1314,   501,   503,   384,   385,
     386,  1193,    97,    98,    99,  1314,   227,   228,   229,   515,
     121,   122,  1314,  1199,   492,    28,    60,   121,   122,   588,
     589,   590,   591,   592,   593,   594,    60,   621,  1202,  1204,
      20,  1219,  1224,    60,    28,   230,   132,   133,  1350,  1351,
    1227,  1360,  1361,    28,  1363,   121,   122,  1231,  1236,    28,
     622,   388,   389,    28,  1237,   100,   101,   231,  1238,   142,
     623,  1239,  1244,  1247,   142,  1259,  1246,   158,   158,   158,
     624,  1248,   625,   626,   473,   475,   479,  1270,  1255,   551,
    1178,  1524,  1251,  1261,  1262,  1557,  1315,  1315,  1524,  1263,
    1266,  1315,   462,  1273,   214,  1267,  1307,  -664,   909,  1278,
     452,  1307,   232,   233,   234,  1327,  1343,   351,  1347,  1306,
    1306,   142,   142,   158,  1306,  1369,   223,  1370,   552,   553,
    1376,   659,  1380,  1381,   693,  1386,  1387,   222,  1388,   157,
     669,  1315,   299,  1389,  1396,   495,   222,    74,    74,   299,
    1315,  1307,    74,   659,  1577,   690,  1405,  1406,  1407,  1419,
    1307,  1421,   690,  1434,  1306,  1483,   748,   749,    97,   112,
     113,  1437,  1440,  1306,  1109,  1484,  1488,  1441,  1524,  1445,
    1446,  1314,  1314,  1464,  1465,   695,  1314,   142,   704,   705,
    1490,   708,    74,  1466,   377,  1467,  1472,   297,  1491,  1498,
    1315,    74,    60,    60,   297,   423,   819,    60,   559,  1520,
    1307,  1519,   157,   157,   157,    97,   112,   113,   514,   160,
    1528,  1492,   115,  1306,  1538,  1539,  1314,  1548,  1565,   299,
    1550,  1556,  1566,  1574,  1575,  1314,  1587,  1591,  1315,  1598,
     692,  1599,   302,   396,   397,    12,  1602,    60,  1307,  1605,
     155,    74,  1607,    23,  1608,  1612,    60,   158,  1621,  1616,
    1620,  1306,   341,  1628,   341,  1623,   341,  1629,   114,   115,
     764,   690,     2,    97,   112,   113,   444,   666,   667,   668,
      23,  1511,  1512,  1513,   297,  1314,     7,   253,   162,    74,
     935,  1495,   160,   160,   160,   320,    30,  1190,   443,  1152,
     713,   728,   129,   130,   441,  1572,    60,   248,   728,   361,
    1053,  1279,   341,   341,   502,   410,    97,   112,   113,   690,
    1479,  1627,  1379,  1314,    34,   690,   114,   115,   384,   385,
     386,   990,   535,   214,   222,  1144,   831,   507,  1442,  1341,
    1544,  1545,  1143,  1063,    60,   621,  1055,   142,  1364,   925,
     142,  1141,    28,   392,   132,   133,   158,   158,   158,   865,
     485,   746,   142,   747,   618,   473,   475,   479,   622,  1494,
     115,  1432,  1324,   387,   728,  1606,   758,   729,   623,  1594,
     879,   388,   389,  1611,   735,  1569,  1613,  1192,   624,   149,
     625,   626,  1438,    97,    98,    99,     0,     0,   142,     0,
     142,     0,   158,   158,   158,     0,     0,   452,  1595,     0,
       0,     0,     0,   142,   495,   222,     0,     0,   452,     0,
       0,     0,   495,     0,     0,     9,   129,   130,    12,     0,
       0,    15,   690,     0,   728,     0,   690,   452,   365,   728,
       0,     0,     0,     0,     0,     0,   100,   101,     0,   942,
     806,     0,     0,    20,     0,     0,     0,     0,   423,   426,
     959,     0,     0,    23,   158,     0,     0,   158,     0,     0,
     728,     0,     0,     0,   690,     0,    28,   728,    29,    30,
     690,     0,   158,   158,   158,     0,     0,   423,  1326,     0,
     559,   831,    32,     0,   550,     0,     0,   158,     0,     0,
       0,     0,    33,     0,  1336,     0,     0,    34,     0,     0,
     859,     0,     0,     0,    35,   860,     0,   302,    10,    11,
      12,     0,     0,     9,    10,    11,    12,   690,     0,    15,
       0,     0,   341,     0,   302,    10,    11,    12,  1371,  1372,
    1373,  1374,     0,     0,   302,    10,    11,    12,     0,     0,
       0,    20,     0,   914,     0,    23,   690,     0,   910,     0,
       0,    23,   253,     0,     0,     0,     0,   621,   690,     0,
     320,    30,    23,     0,    28,     0,    29,    30,   222,   253,
       0,     0,    23,     0,     0,     0,     0,   320,    30,   253,
     948,     0,     0,     0,   254,   413,   415,   320,    30,    34,
     949,  1409,   690,   521,   690,    34,     0,     0,   597,     0,
     624,   523,   950,   626,     0,     0,    34,   214,     0,   222,
     239,     0,     0,     0,     0,     0,    34,     0,   302,   129,
     130,    12,     0,     0,   726,   730,     0,   746,   747,     0,
     758,     0,   730,     0,     0,     0,   728,   598,   599,     0,
       0,     0,   600,   601,   602,   603,   627,   627,   627,     0,
       0,     0,     0,   222,     0,     0,    23,  1048,     0,     0,
       0,     0,   341,   253,   142,   142,     0,   142,     0,  1469,
       0,   320,    30,     0,     0,   495,     0,     0,   452,   690,
       0,     0,   216,   217,   218,     0,   669,     0,     0,    16,
     728,   728,     0,     0,     0,   214,   728,   341,   730,   942,
      34,   452,  1489,     0,   704,   705,    20,   708,   690,     0,
     728,    22,   728,     0,   728,     0,     0,     0,  1190,     0,
      25,     0,     0,     0,   158,     0,   302,   396,   397,    12,
     341,     0,   726,     0,     0,   835,   836,     0,   840,   841,
     842,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,   853,   854,   855,   856,   857,   858,     0,   730,     0,
     728,     0,     0,   730,    23,     0,  1099,  1100,     0,  1522,
       0,     0,  1105,     0,     0,   838,  1522,    28,     0,   320,
      30,     0,     0,     0,     0,     0,  1117,     0,  1118,     0,
    1119,     0,   423,  1326,   730,   559,     9,    10,    11,    12,
       0,   730,     0,     0,     0,     0,   728,     0,    34,   586,
     587,   588,   589,   590,   591,   592,   593,   594,     0,     0,
     158,   158,   910,   927,   929,   728,     0,   473,   475,   479,
       0,   216,   217,   218,    23,     0,  1147,     0,    16,   341,
     621,     0,     0,     0,     0,     0,     0,    28,  1242,    29,
      30,     0,     0,     0,     0,    20,  1522,   129,   130,   239,
      22,   217,   218,   948,   142,   142,   910,    16,     0,    25,
       0,     0,  1597,   949,     0,     0,   479,     0,    34,     0,
       0,     0,  1168,   624,   700,   950,   626,   726,     0,    22,
       0,     0,     0,     0,  1610,     0,     0,     0,    25,     0,
     621,   838,     0,     0,     0,     0,     0,    28,  1048,   132,
     133,     0,     0,   910,     0,     0,     0,     0,   627,   627,
       0,   627,     0,   622,     0,     0,     0,     0,   908,     0,
     129,   130,  1027,   623,  1284,   520,   630,   634,   637,     0,
    1242,     0,     0,   624,     0,   632,   626,   452,   452,     0,
       0,     0,   341,   341,     0,   341,     0,     0,     0,   640,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
     730,     0,     0,   621,     0,     0,   452,     0,  1013,     0,
      28,     0,   132,   133,     0,     0,   627,     0,   627,   627,
       0,   627,  1090,     0,   310,   641,   622,    23,     0,     0,
     728,     0,   728,     0,   728,     0,   623,     0,     0,  1064,
      28,     0,   132,   133,     0,  1069,   624,     0,   625,   626,
       0,   493,   217,   218,   730,   730,     0,     0,    16,   726,
     730,     0,     0,   627,   746,   747,   473,   475,   479,     0,
     690,     0,   758,     0,   730,    20,   730,     0,   730,     0,
      22,     0,     0,     0,     0,     0,  1411,  1412,     0,    25,
       0,     0,     0,     0,     0,   473,   475,   479,     0,     0,
       0,   142,   142,   158,   158,   910,  1280,     0,  1281,   142,
    1282,     0,     0,     0,  1140,     0,     0,     0,     0,     0,
    1039,  1040,    11,    12,   730,     0,     0,     0,  1358,   728,
       0,     0,   158,   158,   910,     0,     0,     0,     0,   341,
     627,     0,     0,     0,     0,     0,     0,     0,  1125,     0,
    1126,     0,  1164,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     0,   452,   452,     0,   452,   452,     0,   452,
     730,    28,     0,    29,    30,     0,     0,  1210,  1475,  1041,
       0,     0,     0,   746,   747,     0,   758,   195,     0,   730,
       9,    10,    11,    12,     0,     0,     0,   196,    82,     0,
    1493,  1496,    34,     0,     0,  1385,     0,     0,   106,   197,
       0,     0,     0,   627,     0,   627,     0,     0,   135,     0,
     142,   142,     0,   142,   146,   146,   627,   146,    23,    82,
     908,   908,   908,  1217,     0,     0,    82,     0,   888,   890,
       0,    28,     0,    29,    30,     0,     0,   194,     0,   205,
       0,    82,     0,     0,     0,     0,     0,   195,     0,   240,
     214,   222,     0,     0,     0,     0,   106,   196,     0,     0,
       0,     0,    34,     0,   627,   627,   627,   264,   106,   197,
       0,  1540,     0,   726,   726,     0,   728,  1245,     0,   216,
     217,   218,     0,     0,   452,   452,    16,     0,     0,     0,
     106,     0,     0,     0,     0,     0,   953,   341,   630,   634,
       0,   637,     0,    20,     0,     0,     0,     0,    22,     0,
       0,     0,  1232,   908,  1234,     0,     0,    25,   135,     0,
      82,     0,     0,     0,   146,   146,     0,   479,     0,   414,
     146,     0,   920,   146,   146,   146,     0,     0,     0,     0,
       0,    10,    11,   634,     0,    14,   248,     0,     0,    82,
       0,    16,     0,    82,   730,     0,   730,     0,   730,   205,
      82,     0,     0,     0,   158,    18,   726,    19,     0,  1265,
       0,     0,     0,    22,     0,   726,   726,   205,   205,   205,
     726,     0,    25,     0,   621,     0,   452,   452,   452,     0,
       0,    28,     0,   132,   133,     0,     0,  1348,  1349,     0,
     473,   475,   479,     0,     0,     0,   205,   622,     0,    10,
      11,     0,     0,    14,    15,     0,     0,   623,     0,    16,
     634,     0,   726,   504,     0,     0,     0,   624,     0,   625,
     626,     0,   106,    18,     0,    19,     0,   158,   158,   158,
       0,    22,     0,   146,     0,   452,   452,     0,     0,  1342,
      25,     0,     0,   730,  1346,     0,     0,     0,     0,    28,
       0,   132,   133,   604,     0,     0,     0,     0,     0,     0,
       0,   627,   627,   627,   627,   627,     0,  1404,     0,   627,
       0,   106,   532,     0,     9,    10,    11,    12,     0,     0,
     248,     0,     0,  1120,     0,  1121,     0,   605,     0,     0,
       0,     0,   908,   908,   908,  1383,  1127,     0,     0,     0,
     888,   890,    20,   452,    97,   112,   113,   726,   227,   228,
     229,     0,    23,     0,     0,     0,   106,     0,   621,     0,
     619,     0,   532,   532,   635,    28,  1450,    29,    30,     0,
       0,     0,    20,    82,     0,     0,     0,   230,     0,     0,
       0,   948,     0,     0,   888,   890,     0,     0,   726,     0,
       0,   949,     0,     0,     0,     0,    34,   114,   115,     0,
       0,   624,     0,   950,   626,     0,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,   205,   106,
     627,   627,     0,   627,   118,   973,   974,     0,  1016,     9,
      10,    11,    12,   146,     0,    15,   146,     0,  1460,  1461,
     730,     0,     0,     0,     0,     0,     0,     0,   146,     0,
       0,  1017,     0,     0,     0,  1471,    82,    20,  1018,     0,
    1509,  1510,     0,     0,     0,     0,     0,    23,     0,  1515,
       0,     0,     0,   621,     0,    28,     0,   121,   122,     0,
      28,     0,    29,    30,   205,     0,   205,     0,   205,   205,
     205,     0,     0,     0,   205,     0,  1157,  1535,     0,   205,
      10,    11,   205,   169,    14,    15,  1158,     0,   734,     0,
      16,    34,     0,     0,  1507,  1508,   624,     0,  1159,   626,
      82,     0,     0,     0,    18,     0,    19,    20,     0,     0,
       0,     0,    22,  1517,  1518,     0,     0,     0,     0,     0,
       0,    25,     0,   621,     0,     0,     0,     0,     0,     0,
      28,     0,   132,   133,     0,     0,     0,     0,   106,   106,
     106,   106,     0,     0,     0,     0,   622,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   623,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   624,     0,   625,   626,
       0,  1120,  1121,   888,   890,     0,     0,     0,   766,  1127,
       9,    10,   767,    12,   169,    14,    15,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,   106,     0,
     532,     0,   888,   890,     0,    18,   726,    19,    20,    21,
       0,     0,   619,    22,   532,   532,     0,   635,    23,     0,
       0,     0,    25,   768,   911,   170,     0,     0,   913,     0,
       0,    28,     0,    29,    30,     0,     0,   769,     0,   770,
       0,     0,     0,   205,     0,     0,     0,    32,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    34,     0,     0,     0,     0,   135,   555,    35,
     302,    10,    11,    12,   135,    14,   303,   304,   305,     0,
     306,    16,   205,   955,   205,   205,   240,   635,     0,     0,
    1120,  1121,     0,  1127,     0,    18,   307,    19,    20,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,    30,  -313,     0,     0,   322,   205,
       0,     0,     0,   955,   323,     0,     0,   556,     0,     0,
     205,   205,     0,   205,     0,   325,   326,   557,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,     0,   558,
     135,     0,     0,    82,     0,    82,     0,     0,     0,     0,
    1050,    82,    82,     0,     0,   129,   130,   332,    82,   217,
     218,   106,     0,     0,     0,    16,     0,   106,     0,     0,
       0,     0,     0,     0,   532,   532,   532,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   532,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,   621,    85,
       0,     0,     0,     0,     0,    28,     0,   132,   133,   108,
       0,     0,     0,     0,     0,     0,     0,     0,   128,   136,
       0,   622,     0,     0,     0,   147,   147,     0,   147,     0,
      85,   623,     0,     0,     0,     0,     0,    85,     0,     0,
       0,   624,     0,   625,   626,     0,     0,     0,     0,     0,
     147,     0,    85,     0,     0,     0,     0,     0,     0,   532,
     241,   532,     0,     0,     0,     0,     0,   250,   106,     0,
       0,     0,   532,     0,   106,     0,   911,   911,   911,   250,
       0,     0,     0,     0,  1134,     0,     9,    10,    11,    12,
     169,    14,    15,     0,     0,   734,     0,    16,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,    18,     0,    19,    20,     0,   106,     0,   106,    22,
     205,   205,  1161,     0,    23,     0,     0,     0,    25,     0,
     621,    85,     0,     0,     0,   147,   147,    28,     0,    29,
      30,   147,     0,     0,   147,   147,   147,     0,     0,     9,
      10,    11,    12,  1157,     0,   520,     0,     0,     0,     0,
      85,     0,     0,  1158,    85,     0,     0,     0,    34,  1161,
     147,    85,     0,   624,     0,  1159,   626,    20,     0,     0,
       0,     0,   106,     0,     0,     0,     0,    23,   147,   147,
     147,     0,     0,   621,     0,     0,     0,     0,   106,     0,
      28,  1050,    29,    30,     0,     0,     0,     0,     0,     0,
     106,     0,     0,     0,     0,     0,   948,   147,     0,     0,
       0,     0,     0,  1222,     0,     0,   949,     0,     0,     0,
       0,    34,     0,    53,     0,    82,   624,     0,   950,   626,
       0,     0,     0,     0,   106,     0,   106,     0,   612,     0,
     302,   396,   397,    12,   147,   613,     0,     0,     0,    53,
      53,     0,   154,     0,    53,    10,    11,     0,     0,    14,
      15,    53,     0,     0,   106,    16,     0,     0,     0,   532,
     532,     0,   532,     0,    53,     0,    53,     0,    23,    18,
       0,    19,   250,   147,     0,   614,     0,    22,     0,     0,
       0,    28,     0,   320,    30,     0,    25,     0,     0,   256,
       0,     0,     0,     0,     0,    28,     0,   132,   133,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   205,   205,
     205,  1161,   615,     0,     0,   205,     0,   250,     0,     0,
       0,   620,     0,   147,   147,   636,     0,     0,     0,     0,
     645,     0,     0,   864,    85,     0,     0,     0,  1161,  1161,
    1161,     0,   398,   398,     0,    53,     0,     0,     0,    53,
      53,     0,     0,   256,     0,    53,     0,     0,   154,   154,
     154,     0,     0,     0,     0,   433,     0,     0,   682,     0,
       0,   205,     0,     0,    53,     0,   146,   250,    53,   147,
     250,     0,    10,    11,    53,    53,    14,   248,     0,     0,
       0,     0,    16,     0,   147,     0,     0,   147,     0,     0,
       0,     0,    53,    53,   154,     0,    18,     0,    19,   147,
       0,     0,   256,     0,    22,     0,     0,    85,     0,     0,
       0,     0,     0,    25,     0,     0,   205,   205,     0,   205,
       0,    53,    28,     0,   132,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,   147,     0,   147,
     147,   147,     0,     0,     0,   147,     0,     0,     0,     0,
     147,     0,     0,   147,     0,   205,   955,   205,    53,     0,
     766,     0,     9,    10,   767,    12,   169,    14,    15,     0,
       0,    85,     0,    16,   106,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,    18,     0,    19,
      20,    21,     0,     0,     0,    22,  -527,     0,     0,     0,
      23,     0,     0,     0,    25,   768,     0,   170,     0,   250,
     250,   250,   250,    28,     0,    29,    30,     0,     0,   769,
       0,   770,     0,     0,     9,   129,   130,    12,   560,    32,
     248,    10,    11,     0,     0,    14,    15,     0,     0,    33,
       0,    16,     0,     0,    34,   398,     0,     0,     0,     0,
       0,    35,    20,   256,     0,    18,     0,    19,    53,     0,
       0,     0,    23,    22,     0,     0,     0,     0,  -527,   250,
    1161,   147,    25,     0,     0,    28,     0,    29,    30,     0,
       0,    28,     0,   132,   133,   147,   147,     0,   636,     0,
       0,    32,   398,     0,     0,   912,     0,     0,     0,     0,
       0,    33,     0,    53,     0,     0,    34,     0,   645,     0,
       0,     0,     0,    35,   147,     0,     0,     0,    53,     0,
       0,    53,     0,     0,     0,     0,     0,   433,   433,   433,
       0,     0,     0,    53,     0,     0,     0,     0,   682,    78,
       0,    53,     0,  1161,  1161,  1161,   128,     0,     9,   129,
     130,    12,     0,   147,   636,   147,   147,   241,   636,     0,
       0,     0,   106,     0,     0,     0,     0,   205,     0,    53,
      78,    53,     0,   154,   154,   154,    20,    78,     0,    53,
       0,   985,     0,     0,    53,     0,    23,    53,     0,     0,
     203,     0,   215,     0,     0,     0,     0,     0,     0,    28,
     147,    29,    30,     0,   636,    53,     0,     0,     0,     0,
       0,   147,   147,     0,   147,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,   985,     0,
      34,   136,     0,     0,    85,   560,    85,   153,   560,     0,
       0,  1051,    85,    85,     0,     0,     0,     0,     0,    85,
       0,     0,   250,   560,   560,   560,     0,     0,   250,     0,
       0,     0,     0,     0,     0,   147,   147,   147,   560,     0,
       0,   409,     0,     0,     0,   412,     0,   147,     0,     0,
       0,     0,     0,     0,     9,    10,    11,    12,   216,   217,
     218,     0,     0,     0,     0,    16,     0,     0,     0,     0,
      78,     0,     0,     0,    78,     0,     0,     0,     0,     0,
     203,   215,    20,     0,     0,     0,   256,    22,     0,     0,
       0,     0,    23,     0,     0,     0,    25,     0,   621,   560,
       0,     0,     0,     0,     0,    28,     0,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
     147,   948,   147,     0,     0,     0,     0,   203,     0,   250,
       0,   949,     0,   147,     0,   250,    34,   912,   912,   912,
       0,   624,   398,   950,   626,   645,     0,     0,     0,   398,
       0,     0,     9,   129,   130,    12,     0,    53,    53,    53,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     9,   129,   130,    12,     0,   250,    15,   250,
      20,   147,   147,   636,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,    28,    53,    29,    30,     0,    53,     0,
      23,     0,     0,     0,     0,    53,    53,     0,    53,    32,
       0,     0,     0,    28,     0,    29,    30,     0,     0,    33,
     912,     0,     0,     0,    34,     0,     0,     0,    53,   151,
      53,    35,     0,   250,     0,    53,    53,    53,   436,   152,
       0,     0,     0,    53,    34,     0,     0,     0,     0,   250,
       0,   153,  1051,     0,   653,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,   560,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     9,    10,    11,    12,     0,
       0,    15,     0,   487,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,   250,     0,   250,     0,   203,
       0,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,    28,     0,    29,    30,
     147,   147,     0,   147,     0,     0,     0,    78,     0,     0,
       0,     0,   195,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,    34,     0,     0,
       0,   560,   560,   560,   197,     0,     0,   433,   256,     0,
       0,     0,     0,     0,     0,   203,     0,     0,   147,   147,
     147,   147,   636,   203,     0,     0,   147,    80,     0,     0,
       0,     0,     0,   985,     0,     9,    10,    11,    12,     0,
       0,   783,     0,     0,     0,    53,    53,   154,     0,   912,
     912,   912,   256,    80,    80,     0,    80,     0,    80,     0,
       0,     0,     0,    20,     0,    80,     0,     0,     0,     0,
     985,     0,     0,    23,     0,     0,     0,     0,    80,     0,
      80,     0,   147,     0,     0,     0,    28,   147,    29,    30,
       0,     0,     0,     0,  1189,     0,     0,     0,     0,     0,
       0,     0,   476,     0,     9,   129,   130,    12,     0,     0,
     520,     0,   477,     0,     0,     0,     0,    34,     0,     0,
       0,     0,     0,     0,   478,     0,    53,     0,     0,     0,
       0,     0,    20,     0,     0,     0,     0,   147,   147,     0,
     147,     0,    23,     0,     0,     0,     0,   256,     0,     0,
       0,     0,     0,     0,     0,    28,     0,    29,    30,    80,
      53,     0,     0,    80,    80,  1415,     0,     0,     0,    80,
       0,    32,    80,    80,    80,     0,   147,   636,   147,     0,
       0,    33,     0,     0,   203,     0,    34,     0,    80,     0,
       0,     0,    80,    35,     0,   250,     0,   751,    80,    80,
       0,   985,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    80,    80,     0,
       0,     0,     0,   203,   954,   203,   203,     0,     0,   788,
       0,   256,     0,     0,     0,   794,     0,   161,     0,     0,
       0,  1039,  1040,    11,    12,    80,     0,   802,   803,     0,
     804,     0,    53,    53,   154,   154,   154,     0,   256,   206,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     203,     0,     0,     0,  1003,     0,     0,     0,     0,    23,
       0,     0,    80,  1189,  1189,  1189,     0,     0,     0,     0,
       0,   912,    28,     0,    29,    30,     0,     0,     0,     0,
    1041,     0,     0,     0,   783,     0,   783,     0,   195,     0,
       0,  1049,  1056,   783,     0,     0,    53,     0,   196,   783,
       0,    53,     0,    34,     0,     0,     0,     0,     0,     0,
     197,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   899,   900,     0,     0,     0,
     899,     0,     0,   161,   161,   161,     0,     0,     0,     0,
       0,     0,     0,     0,   912,   912,   912,     9,    10,    11,
      12,    53,    53,   248,    53,     0,     0,     0,   644,   206,
       0,     0,    80,   250,     0,     0,     0,     0,   147,     0,
       0,     0,     0,     0,     0,    20,     0,   206,   206,   480,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
      53,    53,    53,     0,     0,     0,     0,     0,    28,     0,
      29,    30,     0,     0,     0,     0,   206,    80,     0,     0,
       0,     0,     0,     0,   195,   204,     0,     0,     0,     0,
       0,     0,    80,     0,   196,    80,     0,     0,     0,    34,
       0,   371,   373,     0,     0,     0,   197,    80,     0,     0,
     259,     0,     0,   263,     0,    80,     0,     0,     9,    10,
      11,    12,   216,   217,   218,     0,     0,     0,     0,    16,
       0,     0,     0,     0,   259,     0,   367,     0,     0,     0,
       0,     0,     0,    80,     0,    80,    20,    80,    80,    80,
       0,    22,   533,    80,     0,     0,    23,     0,    80,     0,
      25,    80,   621,  1071,     0,  1073,     0,     0,     0,    28,
       0,    29,    30,     0,     0,  1189,     0,     0,     0,    80,
       0,     0,     0,     0,     0,   195,     0,     0,     9,    10,
      11,    12,   207,    14,   208,   196,     0,     0,     0,    16,
      34,     0,   629,   629,   629,   204,     0,  1428,     0,     0,
       0,     0,  1049,    18,     0,    19,    20,     0,     0,     0,
       0,    22,     0,   204,   204,   204,    23,     0,     0,     0,
      25,     0,     0,   486,     0,     0,     0,  1107,  1108,    28,
    1110,    29,    30,  1485,     0,   209,   783,     0,  1189,  1189,
    1189,     0,   204,     0,     0,    32,     0,     0,   206,     0,
       0,     0,     0,     0,     0,    33,     0,     0,  1128,     0,
      34,     0,    53,     0,     0,     0,     0,    35,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   644,   259,  1486,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     589,   590,   591,   592,   593,   594,     0,     0,     0,  1163,
       0,     0,    80,   563,   206,     0,   206,     0,   480,   480,
     480,     0,     0,     0,   206,     0,     0,     0,     0,   206,
       0,     0,   206,     0,     9,    10,    11,    12,   169,    14,
      15,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,    80,    80,    80,    80,     0,     0,     0,     0,    18,
       0,    19,    20,    21,     0,     0,     0,    22,     0,     0,
       0,     0,    23,     0,   259,   263,    25,     0,     0,   170,
       0,     0,  1208,     0,     0,    28,   689,    29,    30,     0,
       0,     0,  1359,     0,  1213,     0,     0,  1359,    80,     0,
       0,    32,    80,     0,     0,     0,     0,     0,     0,    80,
      80,    33,    80,     0,     0,     0,    34,     0,     0,     0,
       0,     0,     0,    35,   204,     0,     0,     0,     0,    36,
       0,     0,    80,     0,    80,     0,     0,     0,     0,    80,
      80,    80,     0,     0,     0,     0,     0,    80,     0,     0,
     873,     0,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   629,   629,     0,   629,     0,     0,
       0,     0,     0,     0,   629,     0,     0,     0,     0,     0,
    1264,     0,     0,     0,     0,     0,   203,  1427,   203,     0,
     204,     0,   204,   206,   204,   204,   204,     0,     0,     0,
     204,     0,     0,     0,     0,   204,     0,     0,   204,     0,
       9,    10,    11,    12,   216,   217,   218,     0,     0,  1277,
     807,    16,     0,   810,     0,     0,     0,   812,   813,   815,
     816,   817,   952,     0,   952,   952,     0,   629,    20,     0,
     563,     0,     0,    22,     0,     0,     0,     0,    23,     0,
       0,     0,    25,   834,   621,     0,     0,     0,     0,     0,
       0,    28,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,   644,     0,     0,     0,   948,     0,   952,
       0,     0,     0,     0,  1367,     0,     0,   949,  1368,     0,
     206,   206,    34,   206,     0,     0,     0,   624,     0,   997,
     626,     0,     0,     0,     0,     0,     0,   891,     0,    80,
      80,    80,     0,     0,   891,     0,     0,     0,     0,     0,
     206,     0,     0,  1384,     0,     0,     0,     0,     0,     0,
     766,     0,     9,    10,   767,    12,   169,    14,    15,     0,
       0,     0,     0,    16,   533,   533,   533,     0,     0,     0,
       0,     0,     0,   259,   263,     0,   629,    18,  1191,    19,
      20,    21,     0,     0,     0,    22,  -528,     0,     0,   204,
      23,     0,     0,     0,    25,   768,     0,   170,     0,     0,
       0,     0,     0,    28,     0,    29,    30,     0,     0,   769,
      80,   770,     0,     0,     0,     0,     0,     0,   203,    32,
    1443,     0,     0,     0,  1444,     0,     0,     0,   204,    33,
     204,   204,     0,     0,    34,     0,     0,     0,     0,     0,
       0,    35,     0,   689,    80,     0,  1462,  1463,     0,   629,
       0,   629,     9,    10,    11,    12,     0,     0,  -528,     0,
       0,     0,   629,     0,     0,     0,   629,   629,   629,     0,
       0,     0,     0,     0,     0,   204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   204,   204,     0,   204,
      23,   736,     0,     0,     0,     0,     0,   736,     0,     0,
       0,     0,     0,    28,     0,    29,    30,     0,     0,  1422,
     952,   952,  1162,     0,     0,     0,   204,     0,     0,   195,
    -388,    10,    11,  -388,  -388,    14,   248,     0,     0,   196,
       0,    16,     0,     0,    34,     0,    80,    80,    80,    80,
      80,   197,     0,     0,    80,    18,     0,    19,  -388,     0,
       0,     0,     0,    22,     0,     0,     0,     0,  -388,  1162,
       0,     0,    25,     0,   621,  1101,     0,  1191,  1191,  1191,
       0,    28,     0,   132,   133,     0,     0,  1352,  1353,    11,
      12,     0,     0,     0,     0,     0,     0,   622,     0,     0,
       0,   206,     0,     0,     0,     0,     0,   623,   834,     0,
      80,     0,  -388,     0,   736,    80,     0,   624,   891,   625,
     626,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
      29,    30,     0,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,     0,   195,     0,   689,     0,  1153,   259,
     263,   259,   891,     0,   196,    80,    80,     0,    80,    34,
       0,     0,     0,     0,     0,     0,   197,     0,     0,   873,
     873,     0,   873,     0,     0,     0,     9,    10,    11,    12,
       0,     0,    15,     0,     0,     0,   204,   204,  1160,     0,
       0,   104,     0,   259,    80,    80,    80,     0,     0,   891,
     123,   104,     0,     0,    20,     0,     0,   104,   104,     0,
     104,     0,  1195,     0,    23,     0,     0,   952,   952,  1162,
    1162,  1162,     0,   263,     0,   952,     0,    28,   736,    29,
      30,     9,    10,    11,    12,  1160,     0,   520,     0,     0,
     736,     0,   237,   476,     0,     0,     0,     0,  1162,  1162,
    1162,     0,     0,   477,     0,     0,     0,     0,    34,    20,
       0,     0,     0,     0,     0,   478,     0,   204,     0,    23,
       0,     0,     0,     0,   736,     0,   736,     0,     0,     0,
       0,   206,    28,     0,    29,    30,     0,     0,   486,     0,
       0,  1352,   129,   130,    12,     0,     0,     0,   195,     0,
       0,   391,     0,   123,  1250,     0,     0,     0,   196,  1191,
     104,   104,     0,    34,     0,     0,     0,   104,   104,     0,
     197,   104,   104,   104,     0,   419,   104,   104,   104,    23,
       0,     0,     0,     0,     0,     0,   952,   952,     0,   952,
       0,     0,    28,     0,    29,    30,     0,     0,     0,     0,
    1354,     0,     0,     0,     0,     0,     0,     0,    32,     0,
       0,   891,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,   259,    34,     0,   206,     0,   206,     0,     0,
      35,     0,  1191,  1191,  1191,     0,     0,     0,     0,     0,
     891,     0,     0,   204,   204,   204,   204,  1160,     0,  1276,
       0,   204,     0,     0,  1337,     0,    80,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   104,     0,     0,     0,
       0,     0,     0,     0,  1160,  1160,  1160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,   766,     0,
       9,    10,   767,    12,   169,    14,    15,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,   204,     0,     0,
       0,     0,     0,     0,     0,    18,     0,    19,    20,    21,
       0,     0,     0,    22,  -530,   104,     0,     0,    23,     0,
       0,     0,    25,   768,     0,   170,     0,     0,     0,     0,
     480,    28,     0,    29,    30,     0,     0,   769,     0,   770,
       0,     0,     0,     0,     0,     0,     0,    32,     0,     0,
       0,     0,   204,   204,     0,   204,     0,    33,     0,     0,
       0,     0,    34,   104,     0,   104,   104,     0,     0,    35,
       0,     0,     0,     0,  1195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -530,     0,     0,     0,
       0,   204,     0,   204,     0,     0,    10,    11,     0,   169,
      14,    15,     0,   480,   480,   480,    16,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,    19,    20,     0,     0,     0,   206,    22,   104,
       0,     0,     0,     0,     0,     0,   104,   683,     0,   104,
     170,     0,     0,     0,     0,     0,    28,     0,   132,   133,
       0,   104,   555,     0,   302,    10,    11,    12,   169,    14,
     303,   304,   305,   734,   306,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
     307,    19,    20,    21,     0,   308,   309,    22,     0,   310,
     311,   312,    23,   313,   314,     0,    25,     0,   621,     0,
     315,   316,   317,   318,   319,    28,  1160,   320,    30,  -313,
       0,     0,   322,     0,     0,     0,     0,     0,   323,   104,
       0,   904,    10,    11,     0,   207,    14,   208,     0,   325,
     326,   905,    16,     0,     0,     0,   328,   329,   330,     0,
       0,   624,     0,   906,   626,   563,    18,   104,    19,    20,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
       0,   332,   689,    25,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   132,   133,     0,     0,     0,  1160,
    1160,  1160,     0,     0,     0,     0,  1456,     0,  -480,  -480,
    -480,  -480,  -480,  -480,  -480,     0,     0,  -480,     0,  -480,
       0,     0,     0,   204,     0,     0,     0,     0,     0,     0,
    -480,     0,  -480,     0,     0,     0,  -480,     0,     0,     0,
       0,  -480,     0,   104,     0,     0,  -480,     0,     0,     0,
    -480,     0,  -480,     0,   104,   104,     0,   104,   104,  -480,
       0,  -480,  -480,  -480,  -480,  -480,     0,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,  -480,     0,     0,  -480,  -480,  -480,  -480,  -480,
       0,  -480,  -480,     0,     0,     0,     0,  1457,     0,     0,
     104,     0,  -480,  -480,  -480,     0,  -480,   104,   123,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,  1430,     0,
    1287,  1288,  1289,    12,   169,    14,   303,   304,   305,     0,
     306,    16,  1290,   982,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1300,    17,    18,   307,    19,    20,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,  1301,    25,  1302,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,  1303,   721,     0,  1304,   322,     0,
     982,     0,     0,   104,   323,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,     0,   331,
       0,     0,     0,  1305,     0,     0,     0,   104,   104,   104,
       0,     0,     0,     0,     0,     0,  1431,   332,  1286,   104,
    1287,  1288,  1289,    12,   169,    14,   303,   304,   305,     0,
     306,    16,  1290,     0,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1300,    17,    18,   307,    19,    20,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,  1301,    25,  1302,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,  1303,   721,     0,  1304,   322,     0,
       0,     0,     0,     0,   323,     0,     0,   324,     0,     0,
       0,     0,   104,     0,   104,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,   104,     0,     0,     0,   331,
       0,     0,     0,  1305,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,   332,     9,    10,
      11,    12,   169,    14,   303,   304,   305,   734,   306,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,   307,    19,    20,    21,     0,   308,
     309,    22,     0,   310,   311,   312,    23,   313,   314,     0,
      25,     0,   621,     0,   315,   316,   317,   318,   319,    28,
       0,    29,    30,  -313,     0,     0,   322,     0,     0,     0,
     982,     0,   323,     0,     0,  1186,     0,     0,     0,     0,
       0,     0,     0,   325,   326,  1187,     0,     0,     0,     0,
     328,   329,   330,     0,     0,   624,     0,  1188,   626,     0,
     733,     0,   302,    10,    11,    12,   169,    14,   303,   304,
     305,   734,   306,    16,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,   307,    19,
      20,    21,     0,   308,   309,    22,   104,   310,   311,   312,
      23,   313,   314,     0,    25,     0,     0,     0,   315,   316,
     317,   318,   319,    28,     0,   320,    30,     0,     0,     0,
     322,     0,     0,     0,     0,     0,   323,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,     0,     0,     0,   328,   329,   330,     0,     0,     0,
       0,   331,   104,   104,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -800,     0,   332,
     837,     0,   302,    10,    11,    12,   169,    14,   303,   304,
     305,     0,   306,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,   307,    19,
      20,    21,     0,   308,   309,    22,     0,   310,   311,   312,
      23,   313,   314,     0,    25,   982,     0,     0,   315,   316,
     317,   318,   319,    28,     0,   320,    30,  1530,     0,  -790,
     322,     0,     0,     0,     0,     0,   323,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,     0,   982,     0,   328,   329,   330,     0,     0,     0,
       0,   331,     0,     0,     0,     0,     0,   104,   724,   104,
     825,   826,   827,    12,     0,    14,   537,   304,   305,   332,
     306,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,     0,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   828,   829,   725,     0,     0,   322,     0,
       0,     0,     0,     0,   323,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,     0,   331,
     830,   724,     0,   825,   826,   827,    12,     0,    14,   537,
     304,   305,     0,   306,    16,     0,   989,   332,     0,     0,
       0,     0,     0,   982,     0,     0,     0,     0,    18,   307,
      19,     0,    21,     0,   308,   309,    22,     0,   310,   311,
     312,    23,   313,   314,     0,    25,     0,     0,     0,   315,
     316,   317,   318,   319,    28,     0,   828,   829,   725,     0,
       0,   322,     0,     0,     0,     0,     0,   323,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,   325,   326,
     327,     0,     0,     0,     0,   328,   329,   330,     0,     0,
       0,     0,   331,   830,   724,     0,   825,   826,   827,    12,
       0,    14,   537,   304,   305,     0,   306,    16,     0,     0,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   828,
     829,   725,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     0,     0,     0,   724,   331,   825,   826,   827,    12,
       0,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,  -484,   332,     0,     0,     0,     0,     0,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   828,
     829,   725,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     0,     0,     0,   724,   331,   302,    10,    11,    12,
       0,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,  1325,   332,     0,     0,     0,     0,     0,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,   725,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     0,     0,     0,     0,   331,     0,     0,   555,     0,
       9,    10,    11,    12,  1329,    14,   303,   304,   305,     0,
     306,    16,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,    20,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,    29,    30,  -313,     0,     0,   322,     0,
       0,     0,     0,     0,   323,     0,     0,  1501,     0,     0,
       0,     0,     0,     0,     0,   325,   326,  1502,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,   724,  1503,
     302,    10,    11,    12,     0,    14,   537,   304,   305,     0,
     306,    16,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,     0,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,    30,   725,     0,     0,   322,     0,
       0,     0,     0,     0,   323,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,   837,   331,
     302,    10,    11,    12,     0,    14,   537,   304,   305,     0,
     306,    16,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,     0,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,    30,     0,     0,     0,   322,  -790,
       0,     0,     0,     0,   323,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,  1582,   331,
     302,    10,    11,    12,     0,    14,   303,   304,   305,     0,
     306,    16,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,     0,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,    30,     0,     0,  -197,   322,     0,
       0,     0,     0,     0,   323,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,   325,   326,   327,     0,     0,
       0,     0,   328,   329,   330,     0,     0,     0,   837,   331,
     302,    10,    11,    12,     0,    14,   537,   304,   305,     0,
     306,    16,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,     0,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,     0,     0,   315,   316,   317,   318,
     319,    28,     0,   320,    30,     0,     0,     0,   322,     0,
       0,     0,     0,     0,   323,   265,     0,   324,    10,    11,
       0,     0,    14,    15,     0,   325,   326,   327,    16,     0,
       0,     0,   328,   329,   330,     0,     0,     0,     0,   331,
       0,     0,    18,     0,    19,     0,     0,     0,     0,     0,
      22,     0,   266,   267,     0,  -790,     0,   332,     0,    25,
       0,   268,     0,     0,     0,     0,     0,     0,    28,     0,
     132,   133,     0,   269,     0,     0,     0,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,   293,     0,     0,     0,     0,     0,   294,   295,   926,
       0,   302,    10,    11,    12,     0,    14,   537,   304,   305,
       0,   306,    16,     0,   296,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,   307,    19,     0,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
       0,     0,     0,   328,   329,   330,     0,     0,     0,   928,
     331,   302,    10,    11,    12,     0,    14,   537,   304,   305,
       0,   306,    16,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,    18,   307,    19,     0,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
       0,     0,     0,   328,   329,   330,     0,     0,     0,  1514,
     331,   302,    10,    11,    12,     0,    14,   537,   304,   305,
       0,   306,    16,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,    18,   307,    19,     0,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
       0,     0,     0,   328,   329,   330,   302,    10,    11,    12,
     331,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,     0,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,   766,   324,     9,    10,   767,    12,   169,    14,
      15,   325,   326,   327,     0,    16,     0,     0,   328,   329,
     330,     0,     0,     0,     0,   331,     0,     0,     0,    18,
       0,    19,    20,    21,     0,     0,     0,    22,  -529,     0,
       0,     0,    23,   332,   876,     0,    25,   768,     0,   170,
       0,     0,     0,     0,     0,    28,     0,    29,    30,     0,
       0,   769,     0,   770,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     0,     0,    34,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,  1287,  1288,  1289,
      12,   169,    14,   303,   304,   305,     0,   306,    16,  1290,
    -529,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,    17,    18,   307,    19,    20,    21,     0,   308,   309,
      22,     0,   310,   311,   312,    23,   313,   314,  1301,    25,
    1302,     0,     0,   315,   316,   317,   318,   319,    28,     0,
     320,  1303,   721,     0,  1304,   322,     0,     0,     0,     0,
       0,   323,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   327,     0,     0,     0,     0,   328,
     329,   330,     0,     0,     0,     0,   331,     0,     0,     0,
    1305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1435,   332,  1287,  1288,  1289,    12,   169,
      14,   303,   304,   305,     0,   306,    16,  1290,     0,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,    17,
      18,   307,    19,    20,    21,     0,   308,   309,    22,     0,
     310,   311,   312,    23,   313,   314,  1301,    25,  1302,     0,
       0,   315,   316,   317,   318,   319,    28,     0,   320,  1303,
     721,     0,  1304,   322,     0,     0,     0,     0,     0,   323,
       0,     0,   324,     0,     0,     0,     0,     0,     0,     0,
     325,   326,   327,     0,     0,     0,     0,   328,   329,   330,
       0,     0,     0,     0,   331,     0,     0,     0,  1305,  1287,
    1288,  1289,    12,   169,    14,   303,   304,   305,     0,   306,
      16,  1290,   332,  1291,  1292,  1293,  1294,  1295,  1296,  1297,
    1298,  1299,  1300,    17,    18,   307,    19,    20,    21,     0,
     308,   309,    22,     0,   310,   311,   312,    23,   313,   314,
    1301,    25,  1302,     0,     0,   315,   316,   317,   318,   319,
      28,     0,   320,  1303,  1559,     0,  1304,   322,     0,     0,
       0,     0,     0,   323,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,   325,   326,   327,     0,     0,     0,
       0,   328,   329,   330,     0,     0,     0,     0,   331,     0,
       0,     0,  1305,  1287,  1288,  1289,    12,   169,    14,   303,
     304,   305,     0,   306,    16,  1290,   332,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,    17,    18,   307,
      19,    20,    21,     0,   308,   309,    22,     0,   310,   311,
     312,    23,   313,   314,  1301,    25,  1302,     0,     0,   315,
     316,   317,   318,   319,    28,     0,   320,  1303,     0,     0,
    1304,   322,     0,     0,     0,     0,     0,   323,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,   325,   326,
     327,     0,     0,     0,     0,   328,   329,   330,     0,     0,
       0,     0,   331,     0,     0,     0,  1305,   302,    10,    11,
      12,   169,    14,   303,   304,   305,   734,   306,    16,     0,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,   307,    19,    20,    21,     0,   308,   309,
      22,     0,   310,   311,   312,    23,   313,   314,     0,    25,
       0,   621,     0,   315,   316,   317,   318,   319,    28,     0,
     320,    30,     0,     0,     0,   322,     0,     0,     0,     0,
       0,   323,     0,     0,   904,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   905,     0,     0,     0,     0,   328,
     329,   330,     0,     0,   624,     0,   906,   626,     9,    10,
      11,    12,   169,    14,   303,   304,   305,   734,   306,    16,
       0,     0,     0,     0,   332,     0,     0,     0,     0,     0,
       0,     0,     0,    18,   307,    19,    20,    21,     0,   308,
     309,    22,     0,   310,   311,   312,    23,   313,   314,     0,
      25,     0,   621,     0,   315,   316,   317,   318,   319,    28,
       0,    29,    30,     0,     0,     0,   322,     0,     0,     0,
       0,     0,   323,     0,     0,  1186,     0,     0,     0,     0,
       0,     0,     0,   325,   326,  1187,     0,     0,     0,     0,
     328,   329,   330,     0,     0,   624,     0,  1188,   626,   302,
      10,    11,    12,     0,    14,   303,   304,   305,     0,   306,
      16,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,    18,   307,    19,    20,    21,     0,
     308,   309,    22,     0,   310,   311,   312,    23,   313,   314,
       0,    25,     0,   621,     0,   315,   316,   317,   318,   319,
      28,     0,   320,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,   904,     0,     0,     0,
       0,     0,     0,     0,   325,   326,   905,     0,     0,     0,
       0,   328,   329,   330,     0,     0,   624,     0,   906,   626,
       9,    10,    11,    12,     0,    14,   303,   304,   305,     0,
     306,    16,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,    18,   307,    19,    20,    21,
       0,   308,   309,    22,     0,   310,   311,   312,    23,   313,
     314,     0,    25,     0,   621,     0,   315,   316,   317,   318,
     319,    28,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,  1186,     0,     0,
       0,     0,     0,     0,     0,   325,   326,  1187,     0,     0,
       0,     0,   328,   329,   330,     0,     0,   624,     0,  1188,
     626,   302,    10,    11,    12,     0,    14,   537,   304,   305,
       0,   306,    16,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,    18,   307,    19,    20,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   556,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   557,     0,
       0,     0,     0,   328,   329,   330,   302,    10,    11,    12,
     558,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,     0,     0,  1408,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     0,     0,     0,     0,   331,   302,    10,    11,    12,
     169,    14,   303,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,    18,   307,    19,    20,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     9,    10,    11,    12,   331,    14,   537,   304,   305,
       0,   306,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,    18,   307,    19,    20,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,    29,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,  1501,     0,
       0,     0,     0,     0,     0,     0,   325,   326,  1502,     0,
       0,     0,     0,   328,   329,   330,   302,    10,    11,    12,
    1503,    14,   303,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
     321,     0,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,   302,    10,    11,    12,   331,    14,   303,   304,   305,
       0,   306,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,    18,   307,    19,     0,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
       0,     0,     0,   328,   329,   330,   302,    10,    11,    12,
     331,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,     0,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,   302,    10,    11,    12,   331,    14,   537,   304,   305,
       0,   306,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,    18,   307,    19,     0,
      21,     0,   308,   309,    22,     0,   310,   311,   312,    23,
     313,   314,     0,    25,     0,     0,     0,   315,   316,   317,
     318,   319,    28,     0,   320,    30,   570,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
       0,     0,     0,   328,   329,   330,   302,    10,    11,    12,
     571,    14,   537,   304,   305,     0,   306,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,    18,   307,    19,     0,    21,     0,   308,   309,    22,
       0,   310,   311,   312,    23,   313,   314,     0,    25,     0,
       0,     0,   315,   316,   317,   318,   319,    28,     0,   320,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,     0,     0,     0,   328,   329,
     330,     0,     0,     0,     0,   331,   609,   302,    10,    11,
      12,     0,    14,   537,   304,   305,     0,   306,    16,     0,
       0,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,    18,   307,    19,    20,    21,     0,   308,   309,
      22,     0,   310,   311,   312,    23,   313,   314,     0,    25,
       0,     0,     0,   315,   316,   317,   318,   319,    28,     0,
     320,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,   556,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   557,     0,     0,     0,     0,   328,
     329,   330,  1146,    10,    11,    12,   558,    14,   537,   304,
     305,     0,   306,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,    18,   307,    19,
       0,    21,     0,   308,   309,    22,     0,   310,   311,   312,
      23,   313,   314,     0,    25,     0,     0,     0,   315,   316,
     317,   318,   319,    28,     0,   320,    30,     0,     0,     0,
     322,     0,     0,     0,     0,     0,   323,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,     0,     0,     0,   328,   329,   330,     9,    10,    11,
      12,   331,    14,   303,   304,   305,     0,   306,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,    18,   307,    19,    20,    21,     0,   308,   309,
      22,     0,   310,   311,   312,    23,   313,   314,     0,    25,
       0,     0,     0,   315,   316,   317,   318,   319,    28,     0,
      29,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,  1501,     0,     0,     0,     0,     0,
       0,     0,   325,   326,  1502,     0,     0,     0,     0,   328,
     329,   330,   302,    10,    11,    12,  1503,    14,   537,   304,
     305,     0,   306,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,    18,   307,    19,
       0,    21,     0,   308,   309,    22,     0,   310,   311,   312,
      23,   313,   314,     0,    25,     0,     0,     0,   315,   316,
     317,   318,   319,    28,     0,   320,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,     0,     0,     0,   328,   329,   330,   302,    10,    11,
      12,   538,    14,   537,   304,   305,     0,   306,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,    18,   307,    19,     0,    21,     0,   308,   309,
      22,     0,   310,   311,   312,    23,   313,   314,     0,    25,
       0,     0,     0,   315,   316,   317,   318,   319,    28,     0,
     320,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   327,     0,     0,     0,     0,   328,
     329,   330,   302,    10,    11,    12,   541,    14,   537,   304,
     305,     0,   306,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,     0,    18,   307,    19,
       0,    21,     0,   308,   309,    22,     0,   310,   311,   312,
      23,   313,   314,     0,    25,     0,     0,     0,   315,   316,
     317,   318,   319,    28,     0,   320,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     8,   324,
       9,    10,    11,    12,    13,    14,    15,   325,   326,   327,
       0,    16,     0,     0,   328,   329,   330,     0,     0,     0,
       0,   331,     0,     0,    17,    18,     0,    19,    20,    21,
       0,     0,     0,    22,     0,     0,     0,     0,    23,   332,
       0,    24,    25,    26,     0,    27,     0,     0,     0,     0,
       0,    28,     0,    29,    30,     0,   168,    31,     9,    10,
      11,    12,   169,    14,    15,     0,     0,    32,     0,    16,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    34,    18,     0,    19,    20,    21,     0,    35,
       0,    22,     0,     0,     0,    36,    23,     0,     0,     0,
      25,     0,     0,   170,     0,     0,     0,     0,     0,    28,
       0,    29,    30,     0,     8,     0,     9,    10,    11,    12,
      13,    14,    15,     0,     0,    32,     0,    16,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
      34,    18,     0,    19,    20,     0,     0,    35,     0,    22,
       0,     0,     0,    36,    23,     0,     0,     0,    25,     0,
       0,   440,     0,     0,     0,     0,     0,    28,     0,    29,
      30,     0,     0,    31,     0,     9,    10,    11,    12,   169,
      14,    15,     0,    32,  1002,     0,    16,     0,     0,     0,
       0,     0,     0,    33,     0,     0,     0,     0,    34,     0,
      18,     0,    19,    20,     0,    35,     0,     0,    22,     0,
       0,     0,     0,    23,     0,     0,     0,    25,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,    29,    30,
       0,     0,     0,  1039,    10,   767,    12,   207,    14,   208,
       0,     0,    32,     0,    16,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,    34,    18,     0,
      19,    20,     0,     0,    35,     0,    22,     0,     0,     0,
       0,    23,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,    29,    30,     0,     0,
       0,     0,  1041,     9,    10,    11,    12,   207,    14,   208,
      32,     0,     0,     0,    16,     0,     0,     0,     0,     0,
      33,     0,     0,     0,     0,    34,     0,     0,    18,     0,
      19,    20,    35,     0,     0,     0,    22,     0,     0,     0,
       0,    23,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,    29,    30,     0,     0,
    1425,     9,    10,    11,    12,   169,    14,    15,     0,     0,
      32,     0,    16,     0,     0,     0,     0,     0,     0,     0,
      33,     0,     0,     0,     0,    34,    18,     0,    19,    20,
       0,     0,    35,     0,    22,     0,     0,     0,     0,    23,
       0,     0,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    29,    30,     0,     0,     0,     9,
      10,    11,    12,   207,    14,   208,     0,     0,    32,     0,
      16,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    34,    18,     0,    19,    20,     0,     0,
      35,     0,    22,     0,     0,     0,     0,    23,     0,     0,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,    29,    30,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,     0,     0,     0,
       0,    34,     0,     0,     0,     0,     0,     0,    35,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,    10,    11,     0,   169,    14,    15,     0,     0,   734,
       0,    16,     0,     0,    10,    11,     0,   169,    14,    15,
       0,     0,  1553,     0,    16,    18,     0,    19,    20,     0,
       0,     0,     0,    22,     0,     0,     0,     0,    18,     0,
      19,    20,    25,     0,     0,     0,    22,     0,     0,     0,
       0,    28,     0,   132,   133,    25,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   132,   133,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,     0,     0,     0,     0,  1240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   662,     0,     0,  1549,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,  1567,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,     0,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   578,   579,   580,   581,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594
};

static const yytype_int16 yycheck[] =
{
       6,   176,   151,   152,   178,    25,   211,    57,   163,   177,
       6,   351,   128,   719,   404,    82,   457,   158,    16,     6,
     404,   253,    92,     6,    94,    48,    44,   484,   357,   357,
     568,    37,   136,    82,   306,   307,   865,    44,    44,    86,
     625,    37,   375,    61,   249,   126,   648,   632,   442,   718,
      37,     6,   404,    59,    37,    78,   694,    44,    44,   135,
     136,    44,   505,   135,    44,    11,  1390,   339,    74,  1483,
     605,   141,    64,   219,   220,    56,  1306,    32,    33,   146,
      86,    11,    37,   462,    90,     6,    92,    85,    94,    44,
    1484,    44,    11,   298,  1482,     1,   765,     1,   211,    33,
     771,   703,   773,   109,   110,    37,     6,   220,    59,   780,
     108,   789,    73,    62,  1211,  1212,    37,   795,    60,   701,
       6,   562,   492,    44,    12,   357,    35,   177,  1225,  1316,
       9,    86,   138,  1530,   140,   141,  1323,    37,   205,  1527,
      58,   511,     6,    62,    44,    63,   351,    38,  1506,     0,
     142,    37,    58,   171,    58,    82,    62,    62,   109,   108,
     167,   167,   108,    54,   171,   171,   158,   109,  1526,   626,
    1564,    59,   178,    37,    62,   109,   157,   109,   108,   106,
      44,  1578,    61,   138,   171,   171,    86,    47,   171,   108,
      96,   171,   215,   211,     1,  1609,   151,   152,     8,     9,
     540,   219,   220,   108,    14,    54,    35,    59,   135,     4,
       5,    73,   167,    62,     9,   264,   171,   138,   171,   146,
     108,   202,   214,    25,   902,    27,    36,  1585,    59,     0,
     222,   249,    92,    93,    60,    45,  1624,    47,   138,    47,
     195,   196,   151,   152,   153,  1569,   167,   239,   108,    12,
     171,    58,   250,  1483,  1351,    62,    58,   358,   359,    54,
      62,    56,    57,  1360,  1361,     0,  1363,   167,    58,    47,
       6,   171,   348,    54,   138,    70,   348,   108,   205,   720,
     298,     6,    92,   109,    92,    80,   138,    48,   197,    96,
     360,   395,    73,   299,    96,   450,    59,   344,    93,    62,
    1487,    37,    47,   167,    59,   943,   387,   171,    98,   410,
     411,    12,    37,   240,    92,   308,   309,   645,    59,   713,
     714,   906,   151,   152,   153,   331,   178,   476,   477,   864,
     108,   324,   684,   351,   327,   328,   918,   264,   344,   921,
     333,   674,   488,   489,   337,    92,   352,    92,  1026,  1536,
     496,   741,   345,   108,   360,   361,   406,   741,    59,   357,
    1038,    62,    63,   108,   510,   950,    54,   108,   197,     9,
      59,    60,    12,    47,    62,    27,   489,    92,    60,    31,
     962,   963,   614,   529,    60,    73,    54,    92,    59,   344,
     408,    25,  1063,    27,  1072,   184,  1074,   737,   404,  1586,
      47,   407,    25,    74,    27,    73,   361,    47,    60,   947,
      27,    63,   997,  1015,    31,  1512,  1513,   404,    92,    59,
    1249,    61,    62,    63,    58,    73,   444,   109,    62,   356,
     500,    47,    12,   109,    74,    58,    76,   444,   444,    62,
     361,    59,    59,    60,   344,    92,   903,    27,    25,   404,
      27,    31,    92,    93,     3,     4,     5,   109,   444,   508,
     452,   361,    96,    25,   470,   535,    73,    47,   108,    47,
     488,   489,    47,    96,    44,   498,    92,    93,   496,    59,
      60,    58,    62,    63,    54,    62,    56,   414,   867,   444,
     108,    47,   510,    74,   500,    76,    58,   361,    92,    62,
      62,  1170,   508,   495,   404,     4,     5,    56,    57,   361,
      73,   529,    92,    93,    92,   470,    47,    92,    93,    96,
      58,   476,   477,   444,    62,   508,    59,    60,   108,   535,
      60,    80,   538,   108,    96,   541,    92,   712,  1207,   545,
     546,   547,   548,   549,   444,   986,     4,     5,   649,    48,
    1379,    25,   558,   508,   944,  1137,  1138,    56,    57,   606,
     944,    92,   568,   556,   557,   571,    47,   476,   477,   478,
     470,   675,    47,  1016,  1159,  1018,    60,   504,   682,  1022,
     444,    80,    63,   912,   912,     7,    62,   508,   802,   803,
     804,    27,   944,     4,     5,    31,    54,    73,    56,    57,
     606,   651,   482,  1188,   107,   532,   682,   677,   508,   814,
     490,    92,    70,   759,    47,    37,    47,    92,    93,   625,
       4,     5,    80,    59,    60,    54,   632,    63,     3,     4,
       5,     6,    63,   108,   652,    93,   908,    48,   636,    47,
     690,    47,   692,   693,   508,    56,    57,   476,   477,   478,
      31,   606,    47,    58,  1111,    63,   508,    54,    73,    92,
      93,    92,    93,   683,    48,    62,    41,   622,   623,  1338,
     625,   677,    56,    57,    54,   693,    73,   632,    54,    54,
     912,    56,    57,   109,    92,   691,    92,   737,    47,   695,
      54,    47,   619,    73,    47,    70,    80,    92,   718,    47,
       3,     4,     5,     6,   805,    80,   606,    54,   635,    73,
      85,    54,  1091,  1092,    47,    35,    58,    92,    62,   737,
     790,   791,   771,   793,   773,   625,    73,    54,   778,    73,
     779,   780,   632,    92,    93,   741,    92,   786,    41,    92,
       3,   759,    92,     6,    92,    48,    73,     3,     4,     5,
      54,   774,   775,    56,    57,     3,     4,     5,    54,    92,
     783,  1204,  1205,  1206,    59,   771,  1106,   773,    31,   910,
     108,     4,     5,   779,   780,    47,     9,    80,    41,    74,
     786,    76,    85,   789,   790,   791,   741,   793,   771,   795,
     773,   109,   110,    56,    57,   970,   814,   780,    54,    47,
      56,    57,    58,   786,    47,  1184,    54,    70,    56,    57,
       3,     4,     5,     6,  1193,  1194,   771,    80,   773,  1198,
      92,    54,    85,    56,    57,   780,     4,     5,    74,    92,
     108,   786,   152,   153,   789,    54,   937,    70,    93,    47,
     795,   741,   108,    62,    92,     4,     5,    80,    41,    92,
     771,   108,   773,    33,    73,    48,    62,   863,    47,   780,
      11,  1240,   868,    56,    57,   786,  1572,    73,   109,   985,
      48,   771,    98,   773,    60,    47,    92,   197,    56,    57,
     780,     3,    54,   876,    92,    60,   786,    80,   955,   789,
     896,    54,    85,     4,     5,   795,   902,    56,    57,    62,
     906,    73,    80,    92,   111,    54,    73,   771,    62,   773,
      73,   904,   905,    62,   912,    62,   780,    93,   910,   771,
      92,   773,   786,    54,    73,    93,   778,   779,   780,     4,
       5,    62,    47,    44,   786,     4,     5,   111,   944,    54,
     946,   947,    73,    54,   950,    56,    57,   902,    92,   904,
     905,   906,    59,    60,    47,    92,  1335,   944,    73,    73,
    1401,    54,    59,   143,    73,  1066,  1067,  1068,   148,     3,
       4,     5,     6,   762,    73,  1315,  1417,    92,    73,  1029,
      73,    56,    57,  1050,   911,    54,   913,    56,    57,   944,
      11,   997,   109,   948,   949,   950,   785,  1376,   108,    92,
      59,  1050,   902,   108,  1010,   108,   906,    41,  1157,  1158,
     108,   331,    73,   331,  1063,    54,   196,     4,     5,  1042,
    1026,    73,    56,    57,    73,  1048,  1049,   108,   955,  1052,
    1053,   109,  1038,  1056,     4,     5,   109,  1186,  1187,     4,
       5,   950,   997,   111,   944,  1115,     4,     5,    59,   111,
     950,    85,    63,     3,     4,     5,    27,  1063,    59,    59,
      60,    48,    73,    74,   108,  1506,  1072,    73,  1074,    56,
      57,  1026,    73,    74,    73,    76,  1107,  1108,    54,  1110,
    1063,   261,    47,  1038,    54,  1526,    56,    57,   997,    54,
      48,    56,    57,    80,  1161,    73,  1102,   997,    56,    57,
      70,     3,     4,     5,     6,    70,    56,    57,  1063,  1115,
      80,   431,   432,   111,  1555,    80,   111,  1072,   108,  1074,
    1113,   950,    80,  1050,   108,    90,  1026,    92,    93,   108,
    1520,    59,    60,     3,     4,     5,  1520,    58,  1038,    41,
    1314,   108,  1063,    54,  1585,  1313,    48,  1315,     8,   467,
    1170,   111,  1593,  1159,    56,    57,    92,   477,   478,  1260,
      59,    60,    16,  1063,    83,    84,    59,    60,   997,    47,
      24,    63,  1072,  1222,  1074,  1181,    59,  1029,    80,    59,
      60,    54,  1188,    85,  1524,  1181,    56,    57,    58,    92,
      36,     8,     9,  1186,  1187,   109,  1202,    14,  1181,  1063,
      59,    60,  1157,  1158,  1159,    60,    63,  1134,     3,     4,
       5,  1063,    63,    67,     4,     5,    59,    60,   538,    36,
     538,   541,    63,   541,   108,    60,  1181,   545,    45,   108,
      62,  1186,  1187,  1188,  1161,  1614,   108,   557,   558,   108,
     558,   421,  1031,  1032,   424,     3,     4,     5,  1157,  1158,
    1159,   571,   108,   571,    63,   108,   436,   575,    48,  1159,
    1181,    56,    57,  1313,    63,  1315,    56,    57,     3,     4,
       5,  1060,   126,   111,    63,    62,    74,  1186,  1187,  1188,
     108,  1181,   108,   108,   602,   108,   108,   108,  1188,    74,
      80,   609,   472,    74,   474,  1222,   150,  1315,    56,    57,
    1306,    80,    81,    82,    83,    84,    74,   487,  1314,    74,
    1316,   108,   632,  1306,   404,    62,    62,  1323,    59,  1524,
    1316,    56,    57,  1306,   108,    62,  1332,  1323,  1157,  1158,
    1159,   108,   108,  1316,    93,  1358,  1359,   111,    32,    33,
    1323,    35,   111,  1498,    73,    85,    47,     3,     4,     5,
     111,  1306,  1501,  1502,     3,     4,     5,  1186,  1187,  1188,
     108,  1316,   111,   108,   108,    59,   108,    61,  1323,   108,
      60,   109,   108,    67,     4,     5,   109,  1552,    59,     9,
     470,    30,     3,     4,     5,  1306,   240,   241,     3,     4,
       5,    63,     3,     4,     5,  1316,     7,     8,     9,   253,
      56,    57,  1323,    63,  1427,    54,  1306,    56,    57,    78,
      79,    80,    81,    82,    83,    84,  1316,    47,    92,    59,
      31,    59,    59,  1323,    54,    36,    56,    57,  1217,  1218,
     108,  1220,  1221,    54,  1223,    56,    57,   108,    92,    54,
      70,    56,    57,    54,    92,    56,    57,    58,    92,   143,
      80,    92,   109,   108,   148,     8,   109,   151,   152,   153,
      90,   108,    92,    93,   195,   196,   197,   109,   111,   323,
      33,  1477,   108,   108,   108,  1525,  1482,  1483,  1484,   108,
     108,  1487,    63,   108,   178,   111,  1482,    54,   808,   108,
     184,  1487,   103,   104,   105,   108,   108,  1503,   108,  1482,
    1483,   195,   196,   197,  1487,   108,  1524,   108,  1501,  1502,
      62,   365,   111,    47,  1520,   108,   108,   211,   108,  1428,
     374,  1527,  1477,    95,    62,   219,   220,  1482,  1483,  1484,
    1536,  1527,  1487,   387,  1557,   625,    62,    60,    60,     9,
    1536,    60,   632,    16,  1527,    92,  1501,  1502,     3,     4,
       5,   108,   108,  1536,   872,    92,    60,   108,  1564,  1348,
    1349,  1482,  1483,   108,   108,  1520,  1487,   261,   748,   749,
      60,   751,  1527,   108,    63,   108,   108,  1477,    54,    92,
    1586,  1536,  1482,  1483,  1484,   905,   906,  1487,   906,    92,
    1586,   111,  1501,  1502,  1503,     3,     4,     5,     6,  1428,
      17,    56,    57,  1586,    60,    54,  1527,   107,    60,  1564,
      97,   108,   108,   108,   108,  1536,    11,    60,  1624,    60,
    1520,    60,     3,     4,     5,     6,    92,  1527,  1624,    59,
     950,  1586,    63,    41,   108,    60,  1536,   331,    11,   108,
     108,  1624,    90,    11,    92,    60,    94,    60,    56,    57,
     504,   741,     0,     3,     4,     5,   172,     7,     8,     9,
      41,  1450,  1451,  1452,  1564,  1586,     3,    48,    37,  1624,
     675,  1415,  1501,  1502,  1503,    56,    57,   997,   171,   944,
     444,   463,     4,     5,   167,  1551,  1586,     9,   470,    96,
     779,  1170,   140,   141,   240,   140,     3,     4,     5,   789,
    1400,  1624,  1250,  1624,    85,   795,    56,    57,     3,     4,
       5,   725,   301,   407,   408,   930,   570,   245,  1338,  1207,
    1509,  1510,   925,   786,  1624,    47,   779,   421,  1224,   658,
     424,   923,    54,   126,    56,    57,   430,   431,   432,   606,
     202,   472,   436,   474,   348,   476,   477,   478,    70,    56,
      57,  1316,  1182,    48,   536,  1593,   487,   463,    80,  1572,
     614,    56,    57,  1603,   470,  1541,  1605,  1005,    90,   949,
      92,    93,  1332,     3,     4,     5,    -1,    -1,   472,    -1,
     474,    -1,   476,   477,   478,    -1,    -1,   481,  1577,    -1,
      -1,    -1,    -1,   487,   488,   489,    -1,    -1,   492,    -1,
      -1,    -1,   496,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,     9,   902,    -1,   596,    -1,   906,   511,    48,   601,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,   683,
     536,    -1,    -1,    31,    -1,    -1,    -1,    -1,  1158,  1159,
     694,    -1,    -1,    41,   538,    -1,    -1,   541,    -1,    -1,
     632,    -1,    -1,    -1,   944,    -1,    54,   639,    56,    57,
     950,    -1,   556,   557,   558,    -1,    -1,  1187,  1188,    -1,
    1188,   725,    70,    -1,   322,    -1,    -1,   571,    -1,    -1,
      -1,    -1,    80,    -1,  1202,    -1,    -1,    85,    -1,    -1,
     596,    -1,    -1,    -1,    92,   601,    -1,     3,     4,     5,
       6,    -1,    -1,     3,     4,     5,     6,   997,    -1,     9,
      -1,    -1,   360,    -1,     3,     4,     5,     6,  1236,  1237,
    1238,  1239,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    31,    -1,   639,    -1,    41,  1026,    -1,   632,    -1,
      -1,    41,    48,    -1,    -1,    -1,    -1,    47,  1038,    -1,
      56,    57,    41,    -1,    54,    -1,    56,    57,   652,    48,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    56,    57,    48,
      70,    -1,    -1,    -1,    80,  1155,  1156,    56,    57,    85,
      80,  1299,  1072,  1163,  1074,    85,    -1,    -1,    47,    -1,
      90,    80,    92,    93,    -1,    -1,    85,   691,    -1,   693,
     694,    -1,    -1,    -1,    -1,    -1,    85,    -1,     3,     4,
       5,     6,    -1,    -1,   462,   463,    -1,   748,   749,    -1,
     751,    -1,   470,    -1,    -1,    -1,   808,    86,    87,    -1,
      -1,    -1,    91,    92,    93,    94,   350,   351,   352,    -1,
      -1,    -1,    -1,   737,    -1,    -1,    41,   778,    -1,    -1,
      -1,    -1,   500,    48,   748,   749,    -1,   751,    -1,  1377,
      -1,    56,    57,    -1,    -1,   759,    -1,    -1,   762,  1159,
      -1,    -1,     7,     8,     9,    -1,   930,    -1,    -1,    14,
     862,   863,    -1,    -1,    -1,   779,   868,   535,   536,   943,
      85,   785,  1410,    -1,  1274,  1275,    31,  1277,  1188,    -1,
     882,    36,   884,    -1,   886,    -1,    -1,    -1,  1428,    -1,
      45,    -1,    -1,    -1,   808,    -1,     3,     4,     5,     6,
     568,    -1,   570,    -1,    -1,   573,   574,    -1,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,    -1,   596,    -1,
     932,    -1,    -1,   601,    41,    -1,   862,   863,    -1,  1477,
      -1,    -1,   868,    -1,    -1,  1483,  1484,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,   882,    -1,   884,    -1,
     886,    -1,  1502,  1503,   632,  1503,     3,     4,     5,     6,
      -1,   639,    -1,    -1,    -1,    -1,   978,    -1,    85,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    -1,
     904,   905,   906,   661,   662,   997,    -1,   948,   949,   950,
      -1,     7,     8,     9,    41,    -1,   932,    -1,    14,   677,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    54,  1092,    56,
      57,    -1,    -1,    -1,    -1,    31,  1564,     4,     5,   943,
      36,     8,     9,    70,   948,   949,   950,    14,    -1,    45,
      -1,    -1,  1580,    80,    -1,    -1,   997,    -1,    85,    -1,
      -1,    -1,   978,    90,    60,    92,    93,   725,    -1,    36,
      -1,    -1,    -1,    -1,  1602,    -1,    -1,    -1,    45,    -1,
      47,  1609,    -1,    -1,    -1,    -1,    -1,    54,  1029,    56,
      57,    -1,    -1,   997,    -1,    -1,    -1,    -1,   622,   623,
      -1,   625,    -1,    70,    -1,    -1,    -1,    -1,   632,    -1,
       4,     5,   770,    80,  1178,     9,   350,   351,   352,    -1,
    1184,    -1,    -1,    90,    -1,    92,    93,  1031,  1032,    -1,
      -1,    -1,   790,   791,    -1,   793,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     808,    -1,    -1,    47,    -1,    -1,  1060,    -1,   757,    -1,
      54,    -1,    56,    57,    -1,    -1,   690,    -1,   692,   693,
      -1,   695,   830,    -1,    38,    39,    70,    41,    -1,    -1,
    1172,    -1,  1174,    -1,  1176,    -1,    80,    -1,    -1,   788,
      54,    -1,    56,    57,    -1,   794,    90,    -1,    92,    93,
      -1,     7,     8,     9,   862,   863,    -1,    -1,    14,   867,
     868,    -1,    -1,   737,  1155,  1156,  1157,  1158,  1159,    -1,
    1520,    -1,  1163,    -1,   882,    31,   884,    -1,   886,    -1,
      36,    -1,    -1,    -1,    -1,    -1,  1300,  1301,    -1,    45,
      -1,    -1,    -1,    -1,    -1,  1186,  1187,  1188,    -1,    -1,
      -1,  1155,  1156,  1157,  1158,  1159,  1172,    -1,  1174,  1163,
    1176,    -1,    -1,    -1,   922,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,   932,    -1,    -1,    -1,  1219,  1271,
      -1,    -1,  1186,  1187,  1188,    -1,    -1,    -1,    -1,   947,
     814,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   897,    -1,
     899,    -1,   960,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,  1217,  1218,    -1,  1220,  1221,    -1,  1223,
     978,    54,    -1,    56,    57,    -1,    -1,    60,  1392,    62,
      -1,    -1,    -1,  1274,  1275,    -1,  1277,    70,    -1,   997,
       3,     4,     5,     6,    -1,    -1,    -1,    80,     6,    -1,
    1414,  1415,    85,    -1,    -1,  1271,    -1,    -1,    16,    92,
      -1,    -1,    -1,   887,    -1,   889,    -1,    -1,    26,    -1,
    1274,  1275,    -1,  1277,    32,    33,   900,    35,    41,    37,
     904,   905,   906,  1041,    -1,    -1,    44,    -1,   622,   623,
      -1,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    70,    -1,    67,
    1314,  1315,    -1,    -1,    -1,    -1,    74,    80,    -1,    -1,
      -1,    -1,    85,    -1,   948,   949,   950,    85,    86,    92,
      -1,  1495,    -1,  1091,  1092,    -1,  1428,  1095,    -1,     7,
       8,     9,    -1,    -1,  1348,  1349,    14,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,   690,  1115,   692,   693,
      -1,   695,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,  1071,   997,  1073,    -1,    -1,    45,   136,    -1,
     138,    -1,    -1,    -1,   142,   143,    -1,  1428,    -1,   147,
     148,    -1,    60,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,     4,     5,   737,    -1,     8,     9,    -1,    -1,   167,
      -1,    14,    -1,   171,  1172,    -1,  1174,    -1,  1176,   177,
     178,    -1,    -1,    -1,  1428,    28,  1184,    30,    -1,  1128,
      -1,    -1,    -1,    36,    -1,  1193,  1194,   195,   196,   197,
    1198,    -1,    45,    -1,    47,    -1,  1450,  1451,  1452,    -1,
      -1,    54,    -1,    56,    57,    -1,    -1,  1215,  1216,    -1,
    1501,  1502,  1503,    -1,    -1,    -1,   224,    70,    -1,     4,
       5,    -1,    -1,     8,     9,    -1,    -1,    80,    -1,    14,
     814,    -1,  1240,   241,    -1,    -1,    -1,    90,    -1,    92,
      93,    -1,   250,    28,    -1,    30,    -1,  1501,  1502,  1503,
      -1,    36,    -1,   261,    -1,  1509,  1510,    -1,    -1,  1208,
      45,    -1,    -1,  1271,  1213,    -1,    -1,    -1,    -1,    54,
      -1,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1155,  1156,  1157,  1158,  1159,    -1,  1295,    -1,  1163,
      -1,   299,   300,    -1,     3,     4,     5,     6,    -1,    -1,
       9,    -1,    -1,   887,    -1,   889,    -1,    92,    -1,    -1,
      -1,    -1,  1186,  1187,  1188,  1264,   900,    -1,    -1,    -1,
     904,   905,    31,  1577,     3,     4,     5,  1335,     7,     8,
       9,    -1,    41,    -1,    -1,    -1,   344,    -1,    47,    -1,
     348,    -1,   350,   351,   352,    54,  1354,    56,    57,    -1,
      -1,    -1,    31,   361,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    70,    -1,    -1,   948,   949,    -1,    -1,  1376,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    85,    56,    57,    -1,
      -1,    90,    -1,    92,    93,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   404,    -1,   406,   407,
    1274,  1275,    -1,  1277,     3,     4,     5,    -1,     7,     3,
       4,     5,     6,   421,    -1,     9,   424,    -1,  1367,  1368,
    1428,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   436,    -1,
      -1,    30,    -1,    -1,    -1,  1384,   444,    31,    37,    -1,
    1448,  1449,    -1,    -1,    -1,    -1,    -1,    41,    -1,  1457,
      -1,    -1,    -1,    47,    -1,    54,    -1,    56,    57,    -1,
      54,    -1,    56,    57,   472,    -1,   474,    -1,   476,   477,
     478,    -1,    -1,    -1,   482,    -1,    70,  1485,    -1,   487,
       4,     5,   490,     7,     8,     9,    80,    -1,    12,    -1,
      14,    85,    -1,    -1,  1443,  1444,    90,    -1,    92,    93,
     508,    -1,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    36,  1462,  1463,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,   546,   547,
     548,   549,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    92,    93,
      -1,  1155,  1156,  1157,  1158,    -1,    -1,    -1,     1,  1163,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,   606,    -1,
     608,    -1,  1186,  1187,    -1,    28,  1614,    30,    31,    32,
      -1,    -1,   620,    36,   622,   623,    -1,   625,    41,    -1,
      -1,    -1,    45,    46,   632,    48,    -1,    -1,   636,    -1,
      -1,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    62,
      -1,    -1,    -1,   651,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,   675,     1,    92,
       3,     4,     5,     6,   682,     8,     9,    10,    11,    -1,
      13,    14,   690,   691,   692,   693,   694,   695,    -1,    -1,
    1274,  1275,    -1,  1277,    -1,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    -1,    61,   737,
      -1,    -1,    -1,   741,    67,    -1,    -1,    70,    -1,    -1,
     748,   749,    -1,   751,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,    92,
     768,    -1,    -1,   771,    -1,   773,    -1,    -1,    -1,    -1,
     778,   779,   780,    -1,    -1,     4,     5,   110,   786,     8,
       9,   789,    -1,    -1,    -1,    14,    -1,   795,    -1,    -1,
      -1,    -1,    -1,    -1,   802,   803,   804,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   814,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,     6,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    70,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    80,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    90,    -1,    92,    93,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,   887,
      67,   889,    -1,    -1,    -1,    -1,    -1,    74,   896,    -1,
      -1,    -1,   900,    -1,   902,    -1,   904,   905,   906,    86,
      -1,    -1,    -1,    -1,   912,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    -1,    14,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    28,    -1,    30,    31,    -1,   944,    -1,   946,    36,
     948,   949,   950,    -1,    41,    -1,    -1,    -1,    45,    -1,
      47,   138,    -1,    -1,    -1,   142,   143,    54,    -1,    56,
      57,   148,    -1,    -1,   151,   152,   153,    -1,    -1,     3,
       4,     5,     6,    70,    -1,     9,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    80,   171,    -1,    -1,    -1,    85,   997,
     177,   178,    -1,    90,    -1,    92,    93,    31,    -1,    -1,
      -1,    -1,  1010,    -1,    -1,    -1,    -1,    41,   195,   196,
     197,    -1,    -1,    47,    -1,    -1,    -1,    -1,  1026,    -1,
      54,  1029,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
    1038,    -1,    -1,    -1,    -1,    -1,    70,   224,    -1,    -1,
      -1,    -1,    -1,  1051,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    85,    -1,     6,    -1,  1063,    90,    -1,    92,    93,
      -1,    -1,    -1,    -1,  1072,    -1,  1074,    -1,     1,    -1,
       3,     4,     5,     6,   261,     8,    -1,    -1,    -1,    32,
      33,    -1,    35,    -1,    37,     4,     5,    -1,    -1,     8,
       9,    44,    -1,    -1,  1102,    14,    -1,    -1,    -1,  1107,
    1108,    -1,  1110,    -1,    57,    -1,    59,    -1,    41,    28,
      -1,    30,   299,   300,    -1,    48,    -1,    36,    -1,    -1,
      -1,    54,    -1,    56,    57,    -1,    45,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1155,  1156,  1157,
    1158,  1159,    85,    -1,    -1,  1163,    -1,   344,    -1,    -1,
      -1,   348,    -1,   350,   351,   352,    -1,    -1,    -1,    -1,
     357,    -1,    -1,    92,   361,    -1,    -1,    -1,  1186,  1187,
    1188,    -1,   135,   136,    -1,   138,    -1,    -1,    -1,   142,
     143,    -1,    -1,   146,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,    -1,   395,    -1,
      -1,  1219,    -1,    -1,   167,    -1,  1224,   404,   171,   406,
     407,    -1,     4,     5,   177,   178,     8,     9,    -1,    -1,
      -1,    -1,    14,    -1,   421,    -1,    -1,   424,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,    28,    -1,    30,   436,
      -1,    -1,   205,    -1,    36,    -1,    -1,   444,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,  1274,  1275,    -1,  1277,
      -1,   224,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   472,    -1,   474,    -1,   476,
     477,   478,    -1,    -1,    -1,   482,    -1,    -1,    -1,    -1,
     487,    -1,    -1,   490,    -1,  1313,  1314,  1315,   261,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,   508,    -1,    14,  1332,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    28,    -1,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,   546,
     547,   548,   549,    54,    -1,    56,    57,    -1,    -1,    60,
      -1,    62,    -1,    -1,     3,     4,     5,     6,   331,    70,
       9,     4,     5,    -1,    -1,     8,     9,    -1,    -1,    80,
      -1,    14,    -1,    -1,    85,   348,    -1,    -1,    -1,    -1,
      -1,    92,    31,   356,    -1,    28,    -1,    30,   361,    -1,
      -1,    -1,    41,    36,    -1,    -1,    -1,    -1,   109,   606,
    1428,   608,    45,    -1,    -1,    54,    -1,    56,    57,    -1,
      -1,    54,    -1,    56,    57,   622,   623,    -1,   625,    -1,
      -1,    70,   395,    -1,    -1,   632,    -1,    -1,    -1,    -1,
      -1,    80,    -1,   406,    -1,    -1,    85,    -1,   645,    -1,
      -1,    -1,    -1,    92,   651,    -1,    -1,    -1,   421,    -1,
      -1,   424,    -1,    -1,    -1,    -1,    -1,   430,   431,   432,
      -1,    -1,    -1,   436,    -1,    -1,    -1,    -1,   675,     6,
      -1,   444,    -1,  1501,  1502,  1503,   683,    -1,     3,     4,
       5,     6,    -1,   690,   691,   692,   693,   694,   695,    -1,
      -1,    -1,  1520,    -1,    -1,    -1,    -1,  1525,    -1,   472,
      37,   474,    -1,   476,   477,   478,    31,    44,    -1,   482,
      -1,   718,    -1,    -1,   487,    -1,    41,   490,    -1,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    54,
     737,    56,    57,    -1,   741,   508,    -1,    -1,    -1,    -1,
      -1,   748,   749,    -1,   751,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,   765,    -1,
      85,   768,    -1,    -1,   771,   538,   773,    92,   541,    -1,
      -1,   778,   779,   780,    -1,    -1,    -1,    -1,    -1,   786,
      -1,    -1,   789,   556,   557,   558,    -1,    -1,   795,    -1,
      -1,    -1,    -1,    -1,    -1,   802,   803,   804,   571,    -1,
      -1,   138,    -1,    -1,    -1,   142,    -1,   814,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
     177,   178,    31,    -1,    -1,    -1,   619,    36,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    45,    -1,    47,   632,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   651,    -1,
     887,    70,   889,    -1,    -1,    -1,    -1,   224,    -1,   896,
      -1,    80,    -1,   900,    -1,   902,    85,   904,   905,   906,
      -1,    90,   675,    92,    93,   912,    -1,    -1,    -1,   682,
      -1,    -1,     3,     4,     5,     6,    -1,   690,   691,   692,
     693,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,     3,     4,     5,     6,    -1,   944,     9,   946,
      31,   948,   949,   950,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    54,   737,    56,    57,    -1,   741,    -1,
      41,    -1,    -1,    -1,    -1,   748,   749,    -1,   751,    70,
      -1,    -1,    -1,    54,    -1,    56,    57,    -1,    -1,    80,
     997,    -1,    -1,    -1,    85,    -1,    -1,    -1,   771,    70,
     773,    92,    -1,  1010,    -1,   778,   779,   780,   161,    80,
      -1,    -1,    -1,   786,    85,    -1,    -1,    -1,    -1,  1026,
      -1,    92,  1029,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,  1038,    -1,    -1,    -1,   808,    -1,    -1,    -1,    -1,
     193,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,     9,    -1,   206,    -1,    -1,  1063,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1072,    -1,  1074,    -1,   406,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1102,    54,    -1,    56,    57,
    1107,  1108,    -1,  1110,    -1,    -1,    -1,   444,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,   904,   905,   906,    92,    -1,    -1,   910,   911,    -1,
      -1,    -1,    -1,    -1,    -1,   482,    -1,    -1,  1155,  1156,
    1157,  1158,  1159,   490,    -1,    -1,  1163,     6,    -1,    -1,
      -1,    -1,    -1,  1170,    -1,     3,     4,     5,     6,    -1,
      -1,   508,    -1,    -1,    -1,   948,   949,   950,    -1,  1186,
    1187,  1188,   955,    32,    33,    -1,    35,    -1,    37,    -1,
      -1,    -1,    -1,    31,    -1,    44,    -1,    -1,    -1,    -1,
    1207,    -1,    -1,    41,    -1,    -1,    -1,    -1,    57,    -1,
      59,    -1,  1219,    -1,    -1,    -1,    54,  1224,    56,    57,
      -1,    -1,    -1,    -1,   997,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    -1,     3,     4,     5,     6,    -1,    -1,
       9,    -1,    80,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,  1029,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,  1274,  1275,    -1,
    1277,    -1,    41,    -1,    -1,    -1,    -1,  1050,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,   138,
    1063,    -1,    -1,   142,   143,  1302,    -1,    -1,    -1,   148,
      -1,    70,   151,   152,   153,    -1,  1313,  1314,  1315,    -1,
      -1,    80,    -1,    -1,   651,    -1,    85,    -1,   167,    -1,
      -1,    -1,   171,    92,    -1,  1332,    -1,   480,   177,   178,
      -1,  1338,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
      -1,    -1,    -1,   690,   691,   692,   693,    -1,    -1,   512,
      -1,  1134,    -1,    -1,    -1,   518,    -1,    35,    -1,    -1,
      -1,     3,     4,     5,     6,   224,    -1,   530,   531,    -1,
     533,    -1,  1155,  1156,  1157,  1158,  1159,    -1,  1161,    57,
    1163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     737,    -1,    -1,    -1,   741,    -1,    -1,    -1,    -1,    41,
      -1,    -1,   261,  1186,  1187,  1188,    -1,    -1,    -1,    -1,
      -1,  1428,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,   771,    -1,   773,    -1,    70,    -1,
      -1,   778,   779,   780,    -1,    -1,  1219,    -1,    80,   786,
      -1,  1224,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,   628,   629,    -1,    -1,    -1,
     633,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1501,  1502,  1503,     3,     4,     5,
       6,  1274,  1275,     9,  1277,    -1,    -1,    -1,   357,   177,
      -1,    -1,   361,  1520,    -1,    -1,    -1,    -1,  1525,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,   195,   196,   197,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
    1313,  1314,  1315,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,   224,   406,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    57,    -1,    -1,    -1,    -1,
      -1,    -1,   421,    -1,    80,   424,    -1,    -1,    -1,    85,
      -1,   109,   110,    -1,    -1,    -1,    92,   436,    -1,    -1,
      82,    -1,    -1,    85,    -1,   444,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,   472,    -1,   474,    31,   476,   477,   478,
      -1,    36,   300,   482,    -1,    -1,    41,    -1,   487,    -1,
      45,   490,    47,   796,    -1,   798,    -1,    -1,    -1,    54,
      -1,    56,    57,    -1,    -1,  1428,    -1,    -1,    -1,   508,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    80,    -1,    -1,    -1,    14,
      85,    -1,   350,   351,   352,   177,    -1,    92,    -1,    -1,
      -1,    -1,  1029,    28,    -1,    30,    31,    -1,    -1,    -1,
      -1,    36,    -1,   195,   196,   197,    41,    -1,    -1,    -1,
      45,    -1,    -1,   205,    -1,    -1,    -1,   870,   871,    54,
     873,    56,    57,    12,    -1,    60,  1063,    -1,  1501,  1502,
    1503,    -1,   224,    -1,    -1,    70,    -1,    -1,   406,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,   901,    -1,
      85,    -1,  1525,    -1,    -1,    -1,    -1,    92,   250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   620,   264,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,   952,
      -1,    -1,   651,   331,   472,    -1,   474,    -1,   476,   477,
     478,    -1,    -1,    -1,   482,    -1,    -1,    -1,    -1,   487,
      -1,    -1,   490,    -1,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,   690,   691,   692,   693,    -1,    -1,    -1,    -1,    28,
      -1,    30,    31,    32,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,    -1,   356,   357,    45,    -1,    -1,    48,
      -1,    -1,  1025,    -1,    -1,    54,   404,    56,    57,    -1,
      -1,    -1,  1219,    -1,  1037,    -1,    -1,  1224,   737,    -1,
      -1,    70,   741,    -1,    -1,    -1,    -1,    -1,    -1,   748,
     749,    80,   751,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,   406,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   771,    -1,   773,    -1,    -1,    -1,    -1,   778,
     779,   780,    -1,    -1,    -1,    -1,    -1,   786,    -1,    -1,
     608,    -1,   470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   622,   623,    -1,   625,    -1,    -1,
      -1,    -1,    -1,    -1,   632,    -1,    -1,    -1,    -1,    -1,
    1123,    -1,    -1,    -1,    -1,    -1,  1313,  1314,  1315,    -1,
     472,    -1,   474,   651,   476,   477,   478,    -1,    -1,    -1,
     482,    -1,    -1,    -1,    -1,   487,    -1,    -1,   490,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,  1162,
     538,    14,    -1,   541,    -1,    -1,    -1,   545,   546,   547,
     548,   549,   690,    -1,   692,   693,    -1,   695,    31,    -1,
     558,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    45,   571,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   912,    -1,    -1,    -1,    70,    -1,   737,
      -1,    -1,    -1,    -1,  1227,    -1,    -1,    80,  1231,    -1,
     748,   749,    85,   751,    -1,    -1,    -1,    90,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   625,    -1,   948,
     949,   950,    -1,    -1,   632,    -1,    -1,    -1,    -1,    -1,
     778,    -1,    -1,  1266,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    14,   802,   803,   804,    -1,    -1,    -1,
      -1,    -1,    -1,   635,   636,    -1,   814,    28,   997,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    -1,    -1,   651,
      41,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    56,    57,    -1,    -1,    60,
    1029,    62,    -1,    -1,    -1,    -1,    -1,    -1,  1525,    70,
    1343,    -1,    -1,    -1,  1347,    -1,    -1,    -1,   690,    80,
     692,   693,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,   741,  1063,    -1,  1369,  1370,    -1,   887,
      -1,   889,     3,     4,     5,     6,    -1,    -1,   109,    -1,
      -1,    -1,   900,    -1,    -1,    -1,   904,   905,   906,    -1,
      -1,    -1,    -1,    -1,    -1,   737,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   748,   749,    -1,   751,
      41,   789,    -1,    -1,    -1,    -1,    -1,   795,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    56,    57,    -1,    -1,    60,
     948,   949,   950,    -1,    -1,    -1,   778,    -1,    -1,    70,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    80,
      -1,    14,    -1,    -1,    85,    -1,  1155,  1156,  1157,  1158,
    1159,    92,    -1,    -1,  1163,    28,    -1,    30,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,   997,
      -1,    -1,    45,    -1,    47,   863,    -1,  1186,  1187,  1188,
      -1,    54,    -1,    56,    57,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,  1029,    -1,    -1,    -1,    -1,    -1,    80,   896,    -1,
    1219,    -1,    85,    -1,   902,  1224,    -1,    90,   906,    92,
      93,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,   944,    -1,   946,   911,
     912,   913,   950,    -1,    80,  1274,  1275,    -1,  1277,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,  1107,
    1108,    -1,  1110,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,     9,    -1,    -1,    -1,   948,   949,   950,    -1,
      -1,    16,    -1,   955,  1313,  1314,  1315,    -1,    -1,   997,
      25,    26,    -1,    -1,    31,    -1,    -1,    32,    33,    -1,
      35,    -1,  1010,    -1,    41,    -1,    -1,  1155,  1156,  1157,
    1158,  1159,    -1,   985,    -1,  1163,    -1,    54,  1026,    56,
      57,     3,     4,     5,     6,   997,    -1,     9,    -1,    -1,
    1038,    -1,    67,    70,    -1,    -1,    -1,    -1,  1186,  1187,
    1188,    -1,    -1,    80,    -1,    -1,    -1,    -1,    85,    31,
      -1,    -1,    -1,    -1,    -1,    92,    -1,  1029,    -1,    41,
      -1,    -1,    -1,    -1,  1072,    -1,  1074,    -1,    -1,    -1,
      -1,  1219,    54,    -1,    56,    57,    -1,    -1,  1050,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    70,    -1,
      -1,   126,    -1,   128,  1102,    -1,    -1,    -1,    80,  1428,
     135,   136,    -1,    85,    -1,    -1,    -1,   142,   143,    -1,
      92,   146,   147,   148,    -1,   150,   151,   152,   153,    41,
      -1,    -1,    -1,    -1,    -1,    -1,  1274,  1275,    -1,  1277,
      -1,    -1,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      -1,  1159,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,  1134,    85,    -1,  1313,    -1,  1315,    -1,    -1,
      92,    -1,  1501,  1502,  1503,    -1,    -1,    -1,    -1,    -1,
    1188,    -1,    -1,  1155,  1156,  1157,  1158,  1159,    -1,  1161,
      -1,  1163,    -1,    -1,  1202,    -1,  1525,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   240,   241,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1186,  1187,  1188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   261,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,  1219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    30,    31,    32,
      -1,    -1,    -1,    36,    37,   300,    -1,    -1,    41,    -1,
      -1,    -1,    45,    46,    -1,    48,    -1,    -1,    -1,    -1,
    1428,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,  1274,  1275,    -1,  1277,    -1,    80,    -1,    -1,
      -1,    -1,    85,   348,    -1,   350,   351,    -1,    -1,    92,
      -1,    -1,    -1,    -1,  1332,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,  1313,    -1,  1315,    -1,    -1,     4,     5,    -1,     7,
       8,     9,    -1,  1501,  1502,  1503,    14,    -1,    -1,    -1,
     395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    30,    31,    -1,    -1,    -1,  1525,    36,   414,
      -1,    -1,    -1,    -1,    -1,    -1,   421,    45,    -1,   424,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,
      -1,   436,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    30,    31,    32,    -1,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    47,    -1,
      49,    50,    51,    52,    53,    54,  1428,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,   504,
      -1,    70,     4,     5,    -1,     7,     8,     9,    -1,    78,
      79,    80,    14,    -1,    -1,    -1,    85,    86,    87,    -1,
      -1,    90,    -1,    92,    93,  1503,    28,   532,    30,    31,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,   110,  1520,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    57,    -1,    -1,    -1,  1501,
    1502,  1503,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    12,    -1,    14,
      -1,    -1,    -1,  1525,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,   608,    -1,    -1,    41,    -1,    -1,    -1,
      45,    -1,    47,    -1,   619,   620,    -1,   622,   623,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
     675,    -1,   107,   108,   109,    -1,   111,   682,   683,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   694,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,   718,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    61,    -1,
     765,    -1,    -1,   768,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    -1,   802,   803,   804,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,     1,   814,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,   887,    -1,   889,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,   900,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,   110,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
     985,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,
      85,    86,    87,    -1,    -1,    90,    -1,    92,    93,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,  1051,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    -1,    -1,    -1,    85,    86,    87,    -1,    -1,    -1,
      -1,    92,  1107,  1108,    -1,  1110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    -1,    45,  1170,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    -1,  1207,    -1,    85,    86,    87,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,  1222,     1,  1224,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,   110,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,    92,
      93,     1,    -1,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    -1,    13,    14,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,  1338,    -1,    -1,    -1,    -1,    28,    29,
      30,    -1,    32,    -1,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
      80,    -1,    -1,    -1,    -1,    85,    86,    87,    -1,    -1,
      -1,    -1,    92,    93,     1,    -1,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,     1,    92,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,     1,    92,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,    -1,    92,    -1,    -1,     1,    -1,
       3,     4,     5,     6,   101,     8,     9,    10,    11,    -1,
      13,    14,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,    92,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,    92,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,    62,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,    92,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,    92,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,     1,    -1,    70,     4,     5,
      -1,    -1,     8,     9,    -1,    78,    79,    80,    14,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    28,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,   108,    -1,   110,    -1,    45,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      56,    57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    93,    94,     1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      -1,    13,    14,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,
      92,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,    -1,    -1,    -1,     1,
      92,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,     3,     4,     5,     6,
      92,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,     1,    70,     3,     4,     5,     6,     7,     8,
       9,    78,    79,    80,    -1,    14,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    28,
      -1,    30,    31,    32,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    41,   110,   111,    -1,    45,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,    -1,
      -1,    60,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    14,    15,
     109,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,
      86,    87,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,    87,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    96,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,   110,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,    -1,
      -1,    85,    86,    87,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    96,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    14,    15,   110,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
      80,    -1,    -1,    -1,    -1,    85,    86,    87,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    96,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,
      86,    87,    -1,    -1,    90,    -1,    92,    93,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,
      85,    86,    87,    -1,    -1,    90,    -1,    92,    93,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    -1,    13,
      14,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      -1,    45,    -1,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,    -1,
      -1,    85,    86,    87,    -1,    -1,    90,    -1,    92,    93,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    -1,
      13,    14,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    -1,
      -1,    -1,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      93,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    31,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,     3,     4,     5,     6,
      92,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,    -1,    92,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,     3,     4,     5,     6,    92,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,    31,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,     3,     4,     5,     6,
      92,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,     3,     4,     5,     6,    92,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,     3,     4,     5,     6,
      92,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,     3,     4,     5,     6,    92,     8,     9,    10,    11,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      -1,    -1,    -1,    85,    86,    87,     3,     4,     5,     6,
      92,     8,     9,    10,    11,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,    86,
      87,    -1,    -1,    -1,    -1,    92,    93,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    -1,    13,    14,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,
      86,    87,     3,     4,     5,     6,    92,     8,     9,    10,
      11,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    -1,    -1,    -1,    85,    86,    87,     3,     4,     5,
       6,    92,     8,     9,    10,    11,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,
      86,    87,     3,     4,     5,     6,    92,     8,     9,    10,
      11,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    -1,    -1,    -1,    85,    86,    87,     3,     4,     5,
       6,    92,     8,     9,    10,    11,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    28,    29,    30,    -1,    32,    -1,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    85,
      86,    87,     3,     4,     5,     6,    92,     8,     9,    10,
      11,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,     1,    70,
       3,     4,     5,     6,     7,     8,     9,    78,    79,    80,
      -1,    14,    -1,    -1,    85,    86,    87,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    27,    28,    -1,    30,    31,    32,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,   110,
      -1,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    57,    -1,     1,    60,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    70,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    85,    28,    -1,    30,    31,    32,    -1,    92,
      -1,    36,    -1,    -1,    -1,    98,    41,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    56,    57,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    70,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      85,    28,    -1,    30,    31,    -1,    -1,    92,    -1,    36,
      -1,    -1,    -1,    98,    41,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,
      57,    -1,    -1,    60,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    70,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    85,    -1,
      28,    -1,    30,    31,    -1,    92,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    70,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    85,    28,    -1,
      30,    31,    -1,    -1,    92,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    57,    -1,    -1,
      -1,    -1,    62,     3,     4,     5,     6,     7,     8,     9,
      70,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    85,    -1,    -1,    28,    -1,
      30,    31,    92,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    57,    -1,    -1,
      60,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      70,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    85,    28,    -1,    30,    31,
      -1,    -1,    92,    -1,    36,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    57,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    70,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    85,    28,    -1,    30,    31,    -1,    -1,
      92,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    56,    57,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,     4,     5,    -1,     7,     8,     9,    -1,    -1,    12,
      -1,    14,    -1,    -1,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    12,    -1,    14,    28,    -1,    30,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    28,    -1,
      30,    31,    45,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    54,    -1,    56,    57,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    57,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,   107,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   113,   114,   115,     0,   121,   122,   121,     1,     3,
       4,     5,     6,     7,     8,     9,    14,    27,    28,    30,
      31,    32,    36,    41,    44,    45,    46,    48,    54,    56,
      57,    60,    70,    80,    85,    92,    98,   119,   120,   123,
     126,   128,   130,   134,   135,   142,   145,   149,   150,   155,
     167,   178,   180,   206,   208,   213,   214,   227,   228,   230,
     231,   233,   237,   245,   248,   249,   262,   269,   270,   271,
     272,   273,   276,   277,   302,   307,   308,   309,   312,   314,
     316,   319,   320,   321,   328,   329,   399,   400,    60,   109,
      73,    54,    73,    54,    73,    73,    48,     3,     4,     5,
      56,    57,    58,   164,   178,   309,   320,   327,   329,    92,
      92,    92,     4,     5,    56,    57,    58,   164,     3,     4,
       5,    56,    57,   178,   322,   323,   324,   325,   329,     4,
       5,    44,    56,    57,   315,   320,   329,    73,   176,    54,
      73,    73,   248,   302,   311,   312,   320,   329,   302,   311,
     393,    70,    80,    92,   206,   211,   212,   214,   248,   313,
     314,   328,   123,    92,    60,    58,    98,   117,     1,     7,
      48,   119,   134,   142,   143,   144,   149,   227,   230,   237,
     262,   107,   148,   120,   238,    25,   147,   158,   159,    60,
      54,    93,   303,   305,    60,    70,    80,    92,   224,   235,
     243,   306,   310,   312,   319,   320,   328,     7,     9,    60,
     150,   233,   236,   244,   248,   312,     7,     8,     9,   229,
     232,   234,   248,   262,    59,    60,   249,     7,     8,     9,
      36,    58,   103,   104,   105,   164,   166,   178,   179,   248,
     320,   329,    58,    62,   274,   275,    58,   283,     9,   233,
     329,   238,     5,    48,    80,   180,   206,   209,   213,   319,
     321,   301,   309,   319,   320,     1,    38,    39,    47,    59,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      84,    85,    86,    87,    93,    94,   110,   231,   233,   302,
     373,    73,     3,     9,    10,    11,    13,    29,    34,    35,
      38,    39,    40,    42,    43,    49,    50,    51,    52,    53,
      56,    57,    61,    67,    70,    78,    79,    80,    85,    86,
      87,    92,   110,   119,   183,   184,   185,   186,   198,   203,
     204,   205,   206,   215,   217,   218,   219,   220,   222,   226,
     231,   233,   302,   316,   317,   318,   320,   329,   183,   183,
     207,   176,   109,   264,    58,    48,   309,   319,   187,   196,
     205,   226,   187,   226,    92,   125,    58,    63,    54,    54,
      54,    54,    54,    54,     3,     4,     5,    48,    56,    57,
     164,   178,   325,   326,   323,   131,     4,     5,   206,   209,
     210,   210,   315,    74,   136,   150,   227,   230,   233,   312,
     184,   183,   312,   311,   320,   311,     4,     5,   164,   178,
     211,   302,   313,   211,   302,   313,   211,   108,    56,    57,
      70,    80,    92,   206,   212,   108,   301,   220,   114,   116,
      48,   145,   149,   143,   117,   148,   235,   236,    60,    60,
      92,   247,   248,     3,     1,    62,    96,   146,   160,   221,
     363,    47,    63,    92,   254,    60,   111,   303,   304,    47,
      92,   301,   302,   306,   302,   306,    70,    80,    92,   306,
     328,   238,    59,    60,    93,   305,   319,   301,   229,   232,
      59,    60,   238,     7,   234,   248,   229,   234,   224,   239,
      73,   164,   179,   164,   320,    62,   278,   278,   284,   285,
     232,   246,    47,    92,     6,   164,   326,   400,    47,    92,
       9,   311,     5,    80,   180,    93,    93,    62,   111,   232,
      70,    80,   320,   328,   398,   207,    92,     9,    92,   198,
     233,    92,   198,   204,   204,    92,    73,    73,    73,    73,
     205,   164,   204,   204,   204,     1,    70,    80,    92,   187,
     206,   211,   216,   226,   204,    74,    76,   182,    59,   204,
      58,    92,   198,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    47,    92,    47,    86,    87,
      91,    92,    93,    94,    58,    92,   201,   300,   373,    93,
     204,    11,     1,     8,    48,    85,   209,   210,   317,   320,
     329,    47,    70,    80,    90,    92,    93,   203,   225,   328,
     332,   333,    92,   225,   332,   320,   329,   332,    47,    92,
       3,    39,   208,   218,   316,   329,   400,   182,   182,   183,
     150,   227,   233,   312,   164,   298,   299,   109,   263,   164,
     108,    59,    59,   108,   108,   108,     7,     8,     9,   164,
     250,   251,   252,   116,   124,   127,   326,    73,    54,   133,
     210,   315,   329,    45,   135,   137,   139,   140,   141,   226,
     228,   230,   231,   233,   269,   302,   390,   392,   171,   224,
      60,   170,   182,   182,   311,   311,   108,   211,   311,   108,
     109,   118,   118,   144,   149,    60,    60,   220,   161,   364,
       1,    58,   339,   340,     1,    58,   205,   255,   196,   197,
     205,   187,   111,     1,    12,   197,   226,   233,   383,   384,
     388,   389,   390,   392,    61,   395,   306,   306,   302,   302,
     108,   301,   241,   247,   239,   111,   304,   301,   306,   229,
     239,   241,   238,   183,   164,   393,     1,     5,    46,    60,
      62,   119,   128,   135,   150,   156,   157,   180,   227,   230,
     287,   288,   289,   312,    37,   109,   286,   241,   301,   151,
      73,    73,    54,    73,   301,   153,    47,    92,    47,    92,
     111,   111,   301,   301,   301,   183,   197,   226,    92,   225,
     226,   187,   226,   226,   233,   226,   226,   226,   108,   211,
     108,   108,   340,   108,   185,     3,     4,     5,    56,    57,
      93,   164,   255,   256,   226,   205,   205,     1,   187,   378,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   197,
     197,   187,   200,   200,    92,   300,    47,    63,    92,   202,
      70,    80,    93,   328,   330,   331,   111,   187,    54,   164,
       8,    47,    92,    47,    92,    47,    92,   302,   332,   302,
     332,   226,   332,   384,   111,   304,    92,   301,    47,   301,
     301,    47,    92,    93,    70,    80,    92,   197,   203,   211,
     248,   320,   329,   320,   197,   181,   181,   182,   175,   224,
      60,   174,    63,    59,   268,   298,     1,   205,     1,   205,
      59,   108,    92,   109,   116,   133,    54,   183,    60,   210,
     315,   138,   164,   269,    59,    74,    63,    63,    70,    80,
      92,   224,   328,   332,   312,   320,   224,   224,   138,   164,
      63,   177,   169,   168,   177,   181,   108,    60,   148,   118,
     118,   108,     3,     4,     5,    47,    56,    57,    92,   162,
     163,   165,   178,   307,   322,   329,   146,   340,   341,   109,
     256,   108,   108,   108,    12,    59,    62,    92,   108,    12,
      59,    62,    12,   312,   391,   392,   394,    63,   385,    47,
      92,    63,   111,   395,   247,    74,     7,    30,    37,   279,
     280,   281,   282,   307,   322,    47,    92,   205,   288,   227,
     288,   238,   238,    25,    58,    62,    96,    47,    92,     3,
       4,    62,   150,   224,   290,   292,   294,   295,   306,   312,
     320,   329,   150,   156,   291,   295,   312,   288,    60,   109,
     238,    62,   247,   287,   395,   383,   183,   183,   183,   395,
     383,   301,   152,   301,   154,   398,   398,   398,   182,   108,
     108,   108,   108,   108,    74,    74,    74,    74,   108,   108,
     205,    62,    59,   267,   108,    62,   108,   108,   111,   197,
     197,   226,   200,   202,   255,   197,   233,   301,   301,   187,
     301,    93,   204,   111,    85,    73,    47,   197,   197,   197,
     332,   332,   108,   108,   111,   395,   395,   332,   301,   383,
     111,   304,   108,   108,   320,   108,   177,   173,   172,   177,
     205,   299,   109,   268,   251,   108,     3,   197,   109,    60,
     182,   138,   141,   226,   185,   302,   302,    70,    80,    92,
     319,   320,   328,   301,   205,   177,   177,   148,   197,     1,
      59,    47,    92,    47,    92,    47,    92,   340,    33,   109,
     193,   336,   337,   338,    59,   109,    70,    80,    92,   206,
     211,   316,   385,    63,   386,   226,   396,   397,   242,    63,
     181,   393,    92,   393,    59,     7,    37,   393,   301,   383,
      60,   247,   247,   301,   383,    62,    62,   205,   238,    59,
     238,   238,   320,   238,    59,   247,   265,   108,   182,   182,
     182,   108,   395,   383,   395,   383,    92,    92,    92,    92,
     111,   255,   164,   255,   109,   205,   109,   108,   108,   199,
     226,   108,   108,   330,   330,   111,   330,   304,   204,     8,
     183,   108,   108,   108,   301,   395,   108,   111,   177,   177,
     109,    59,   108,   108,   302,   302,   319,   301,   108,   163,
     197,   197,   197,   365,   164,   253,     1,     3,     4,     5,
      15,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    44,    46,    57,    60,    96,   119,   120,   126,   129,
     130,   187,   223,   227,   230,   233,   334,   335,   340,   342,
     347,   348,   366,   375,   338,   109,   211,   108,   255,   101,
     255,   387,    59,   108,   255,   240,   187,   226,   393,   393,
     393,   281,   395,   108,   254,   254,   395,   108,   205,   205,
     247,   247,     3,     4,    62,   293,   296,   297,   306,   312,
     247,   247,     5,   247,   297,   254,   261,   301,   301,   108,
     108,   187,   187,   187,   187,   255,    62,    93,   202,   199,
     111,    47,   182,   395,   301,   197,   108,   108,   108,    95,
     369,   370,    59,    60,    60,   109,    62,    62,    62,   343,
     350,   352,   354,   358,   205,    62,    60,    60,    60,   187,
      80,   164,   164,    56,   132,   329,    62,   367,   223,     9,
     377,    60,    60,   224,   235,    60,   236,   312,    92,   235,
       1,   109,   335,   347,    16,   109,   347,   108,   396,   255,
     108,   108,   280,   301,   301,   247,   247,   254,    62,    62,
     205,   238,   238,   254,   254,   254,     1,   102,   260,   266,
     395,   395,   301,   301,   108,   108,   108,   108,   255,   187,
     202,   395,   108,   371,   370,   164,    47,    92,   189,   189,
     340,   345,   346,    92,    92,    12,    62,   362,    60,   187,
      60,    54,    56,   164,    56,   132,   164,   340,    92,    60,
      60,    70,    80,    92,    60,   109,   349,   395,   395,   205,
     205,   247,   247,   247,     1,   205,   259,   395,   395,   111,
      92,   374,   187,   191,   233,   373,   344,   351,    17,   348,
      58,   223,   376,   378,   191,   205,   360,   347,    60,    54,
     164,   368,   220,   345,   247,   247,   254,   254,   107,   107,
      97,   257,   258,    12,   392,   372,   108,   224,   345,    58,
     194,   348,   353,   193,   355,    60,   108,    62,   347,   369,
      62,   108,   147,   148,   108,   108,   340,   238,   195,    47,
      92,   188,     1,   190,   191,   359,   361,    11,   379,   380,
     381,    60,     1,   146,   363,   247,   193,   187,    60,    60,
     345,   347,    92,    62,   108,    59,   339,    63,   108,   356,
     187,   379,    60,   381,   192,   378,   108,    62,   108,   255,
     108,    11,   382,    60,   357,    59,   108,   194,    11,    60
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

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
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
        case 3:
#line 340 "../../../gbagnu/gcc/cp/parse.y"
    { finish_translation_unit (); ;}
    break;

  case 4:
#line 348 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 5:
#line 350 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 6:
#line 352 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 9:
#line 361 "../../../gbagnu/gcc/cp/parse.y"
    { have_extern_spec = 1;
		  used_extern_spec = 0;
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 10:
#line 366 "../../../gbagnu/gcc/cp/parse.y"
    { have_extern_spec = 0; ;}
    break;

  case 11:
#line 371 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = pedantic;
		  pedantic = 0; ;}
    break;

  case 13:
#line 380 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_lang_change) do_pending_lang_change(); ;}
    break;

  case 14:
#line 382 "../../../gbagnu/gcc/cp/parse.y"
    { if (! toplevel_bindings_p () && ! pseudo_global_level_p())
		  pop_everything (); ;}
    break;

  case 15:
#line 388 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 16:
#line 390 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 17:
#line 392 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 18:
#line 394 "../../../gbagnu/gcc/cp/parse.y"
    { if (TREE_CHAIN ((yyvsp[(3) - (5)].ttype))) (yyvsp[(3) - (5)].ttype) = combine_strings ((yyvsp[(3) - (5)].ttype));
		  assemble_asm ((yyvsp[(3) - (5)].ttype)); ;}
    break;

  case 19:
#line 397 "../../../gbagnu/gcc/cp/parse.y"
    { pop_lang_context (); ;}
    break;

  case 20:
#line 399 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines ();
		  pop_lang_context (); ;}
    break;

  case 21:
#line 402 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines ();
		  pop_lang_context (); ;}
    break;

  case 22:
#line 405 "../../../gbagnu/gcc/cp/parse.y"
    { push_namespace ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 23:
#line 407 "../../../gbagnu/gcc/cp/parse.y"
    { pop_namespace (); ;}
    break;

  case 24:
#line 409 "../../../gbagnu/gcc/cp/parse.y"
    { push_namespace (NULL_TREE); ;}
    break;

  case 25:
#line 411 "../../../gbagnu/gcc/cp/parse.y"
    { pop_namespace (); ;}
    break;

  case 27:
#line 414 "../../../gbagnu/gcc/cp/parse.y"
    { do_toplevel_using_decl ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 29:
#line 417 "../../../gbagnu/gcc/cp/parse.y"
    { pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 30:
#line 422 "../../../gbagnu/gcc/cp/parse.y"
    { begin_only_namespace_names (); ;}
    break;

  case 31:
#line 424 "../../../gbagnu/gcc/cp/parse.y"
    {
		  end_only_namespace_names ();
		  if (lastiddecl)
		    (yyvsp[(5) - (6)].ttype) = lastiddecl;
		  do_namespace_alias ((yyvsp[(2) - (6)].ttype), (yyvsp[(5) - (6)].ttype));
		;}
    break;

  case 32:
#line 434 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 33:
#line 436 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 34:
#line 438 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 35:
#line 443 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 36:
#line 445 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (SCOPE_REF, global_namespace, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 37:
#line 447 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 38:
#line 452 "../../../gbagnu/gcc/cp/parse.y"
    { begin_only_namespace_names (); ;}
    break;

  case 39:
#line 454 "../../../gbagnu/gcc/cp/parse.y"
    {
		  end_only_namespace_names ();
		  /* If no declaration was found, the using-directive is
		     invalid. Since that was not reported, we need the
		     identifier for the error message. */
		  if (TREE_CODE ((yyvsp[(4) - (5)].ttype)) == IDENTIFIER_NODE && lastiddecl)
		    (yyvsp[(4) - (5)].ttype) = lastiddecl;
		  do_using_directive ((yyvsp[(4) - (5)].ttype));
		;}
    break;

  case 40:
#line 467 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyval.ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype);
		;}
    break;

  case 41:
#line 473 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = (yyvsp[(2) - (3)].ttype);
		  if (TREE_CODE ((yyval.ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype);
		;}
    break;

  case 44:
#line 484 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 45:
#line 486 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 46:
#line 491 "../../../gbagnu/gcc/cp/parse.y"
    { push_lang_context ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 47:
#line 493 "../../../gbagnu/gcc/cp/parse.y"
    { if (current_lang_name != (yyvsp[(2) - (2)].ttype))
		    cp_error ("use of linkage spec `%D' is different from previous spec `%D'", (yyvsp[(2) - (2)].ttype), current_lang_name);
		  pop_lang_context (); push_lang_context ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 48:
#line 500 "../../../gbagnu/gcc/cp/parse.y"
    { begin_template_parm_list (); ;}
    break;

  case 49:
#line 502 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = end_template_parm_list ((yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 50:
#line 504 "../../../gbagnu/gcc/cp/parse.y"
    { begin_specialization(); 
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 51:
#line 510 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = process_template_parm (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 52:
#line 512 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = process_template_parm ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 53:
#line 517 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 54:
#line 519 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 55:
#line 523 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_template_type_parm ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 56:
#line 525 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_template_type_parm (class_type_node, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 57:
#line 530 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_template_template_parm ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 58:
#line 542 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 59:
#line 544 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (groktypename ((yyvsp[(3) - (3)].ftype).t), (yyvsp[(1) - (3)].ttype)); ;}
    break;

  case 60:
#line 546 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ftype).t); ;}
    break;

  case 61:
#line 548 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ftype).t); ;}
    break;

  case 62:
#line 550 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 63:
#line 552 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(3) - (3)].ttype)) != TEMPLATE_DECL
		      && TREE_CODE ((yyvsp[(3) - (3)].ttype)) != TEMPLATE_TEMPLATE_PARM
		      && TREE_CODE ((yyvsp[(3) - (3)].ttype)) != TYPE_DECL)
		    {
		      error ("invalid default template argument");
		      (yyvsp[(3) - (3)].ttype) = error_mark_node;
		    }
		  (yyval.ttype) = build_tree_list ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ttype));
		;}
    break;

  case 64:
#line 566 "../../../gbagnu/gcc/cp/parse.y"
    { finish_template_decl ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 65:
#line 568 "../../../gbagnu/gcc/cp/parse.y"
    { finish_template_decl ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 66:
#line 573 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 67:
#line 575 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 68:
#line 577 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines (); ;}
    break;

  case 69:
#line 579 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines ();
		  pop_lang_context (); ;}
    break;

  case 70:
#line 582 "../../../gbagnu/gcc/cp/parse.y"
    { if (pending_inlines) do_pending_inlines ();
		  pop_lang_context (); ;}
    break;

  case 71:
#line 585 "../../../gbagnu/gcc/cp/parse.y"
    { pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 73:
#line 591 "../../../gbagnu/gcc/cp/parse.y"
    {;}
    break;

  case 74:
#line 593 "../../../gbagnu/gcc/cp/parse.y"
    { note_list_got_semicolon ((yyvsp[(1) - (3)].ftype).t); ;}
    break;

  case 75:
#line 595 "../../../gbagnu/gcc/cp/parse.y"
    { maybe_process_partial_specialization ((yyvsp[(1) - (2)].ftype).t);
		  note_got_semicolon ((yyvsp[(1) - (2)].ftype).t); ;}
    break;

  case 77:
#line 602 "../../../gbagnu/gcc/cp/parse.y"
    {;}
    break;

  case 78:
#line 604 "../../../gbagnu/gcc/cp/parse.y"
    { note_list_got_semicolon ((yyvsp[(1) - (3)].ftype).t); ;}
    break;

  case 79:
#line 606 "../../../gbagnu/gcc/cp/parse.y"
    { pedwarn ("empty declaration"); ;}
    break;

  case 81:
#line 609 "../../../gbagnu/gcc/cp/parse.y"
    {
		  tree t, attrs;
		  split_specs_attrs ((yyvsp[(1) - (2)].ftype).t, &t, &attrs);
		  shadow_tag (t);
		  note_list_got_semicolon ((yyvsp[(1) - (2)].ftype).t);
		;}
    break;

  case 85:
#line 622 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 86:
#line 624 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 1; ;}
    break;

  case 92:
#line 640 "../../../gbagnu/gcc/cp/parse.y"
    { finish_function (lineno, (int)(yyvsp[(3) - (4)].itype), 0); ;}
    break;

  case 93:
#line 642 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  int nested = (hack_decl_function_context
				(current_function_decl) != NULL_TREE);
		  finish_function (lineno, (int)(yyvsp[(3) - (3)].itype), nested); 
		;}
    break;

  case 94:
#line 648 "../../../gbagnu/gcc/cp/parse.y"
    { ;}
    break;

  case 95:
#line 653 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 96:
#line 655 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(4) - (8)].ttype), (yyvsp[(5) - (8)].ttype), (yyvsp[(7) - (8)].ttype), (yyvsp[(8) - (8)].ttype)); ;}
    break;

  case 97:
#line 657 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(1) - (5)].ttype), (yyvsp[(2) - (5)].ttype)); 
		  (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), (yyvsp[(4) - (5)].ttype), (yyvsp[(5) - (5)].ttype));
		;}
    break;

  case 98:
#line 661 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 99:
#line 663 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(5) - (9)].ttype), (yyvsp[(6) - (9)].ttype), (yyvsp[(8) - (9)].ttype), (yyvsp[(9) - (9)].ttype)); ;}
    break;

  case 100:
#line 665 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(2) - (6)].ttype), (yyvsp[(3) - (6)].ttype));
		  (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), (yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype));
		;}
    break;

  case 101:
#line 669 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 102:
#line 671 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(4) - (8)].ttype), (yyvsp[(5) - (8)].ttype), (yyvsp[(7) - (8)].ttype), (yyvsp[(8) - (8)].ttype)); ;}
    break;

  case 103:
#line 673 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(1) - (5)].ttype), (yyvsp[(2) - (5)].ttype));
		  (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), (yyvsp[(4) - (5)].ttype), (yyvsp[(5) - (5)].ttype));
		;}
    break;

  case 104:
#line 677 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 105:
#line 679 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(5) - (9)].ttype), (yyvsp[(6) - (9)].ttype), (yyvsp[(8) - (9)].ttype), (yyvsp[(9) - (9)].ttype)); ;}
    break;

  case 106:
#line 681 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_constructor_declarator ((yyvsp[(2) - (6)].ttype), (yyvsp[(3) - (6)].ttype)); 
		  (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), (yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype));
		;}
    break;

  case 107:
#line 688 "../../../gbagnu/gcc/cp/parse.y"
    { if (!begin_function_definition ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)))
		    YYERROR1; ;}
    break;

  case 108:
#line 691 "../../../gbagnu/gcc/cp/parse.y"
    { if (!begin_function_definition ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)))
		    YYERROR1; ;}
    break;

  case 109:
#line 694 "../../../gbagnu/gcc/cp/parse.y"
    { if (!begin_function_definition (NULL_TREE, (yyvsp[(1) - (1)].ttype)))
		    YYERROR1; ;}
    break;

  case 110:
#line 697 "../../../gbagnu/gcc/cp/parse.y"
    { if (!begin_function_definition ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)))
		    YYERROR1; ;}
    break;

  case 111:
#line 700 "../../../gbagnu/gcc/cp/parse.y"
    { if (!begin_function_definition (NULL_TREE, (yyvsp[(1) - (1)].ttype)))
		    YYERROR1; ;}
    break;

  case 112:
#line 706 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(1) - (6)].ttype), (yyvsp[(3) - (6)].ttype), (yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype)); ;}
    break;

  case 113:
#line 708 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(1) - (4)].ttype), empty_parms (), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 114:
#line 710 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(1) - (6)].ttype), (yyvsp[(3) - (6)].ttype), (yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype)); ;}
    break;

  case 115:
#line 712 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyvsp[(1) - (4)].ttype), empty_parms (), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 116:
#line 719 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs, attrs;
		  split_specs_attrs ((yyvsp[(1) - (2)].ttype), &specs, &attrs);
		  attrs = build_tree_list (attrs, NULL_TREE);
		  (yyval.ttype) = start_method (specs, (yyvsp[(2) - (2)].ttype), attrs);
		 rest_of_mdef:
		  if (! (yyval.ttype))
		    YYERROR1;
		  if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  reinit_parse_for_method (yychar, (yyval.ttype)); ;}
    break;

  case 117:
#line 730 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = start_method (NULL_TREE, (yyvsp[(1) - (1)].ttype), NULL_TREE); 
		  goto rest_of_mdef; ;}
    break;

  case 118:
#line 733 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs, attrs;
		  split_specs_attrs ((yyvsp[(1) - (2)].ftype).t, &specs, &attrs);
		  attrs = build_tree_list (attrs, NULL_TREE);
		  (yyval.ttype) = start_method (specs, (yyvsp[(2) - (2)].ttype), attrs); goto rest_of_mdef; ;}
    break;

  case 119:
#line 738 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs, attrs;
		  split_specs_attrs ((yyvsp[(1) - (2)].ttype), &specs, &attrs);
		  attrs = build_tree_list (attrs, NULL_TREE);
		  (yyval.ttype) = start_method (specs, (yyvsp[(2) - (2)].ttype), attrs); goto rest_of_mdef; ;}
    break;

  case 120:
#line 743 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = start_method (NULL_TREE, (yyval.ttype), NULL_TREE); 
		  goto rest_of_mdef; ;}
    break;

  case 121:
#line 746 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs, attrs;
		  split_specs_attrs ((yyvsp[(1) - (2)].ttype), &specs, &attrs);
		  attrs = build_tree_list (attrs, NULL_TREE);
		  (yyval.ttype) = start_method (specs, (yyvsp[(2) - (2)].ttype), attrs); goto rest_of_mdef; ;}
    break;

  case 122:
#line 751 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = start_method (NULL_TREE, (yyval.ttype), NULL_TREE); 
		  goto rest_of_mdef; ;}
    break;

  case 123:
#line 757 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (! current_function_parms_stored)
		    store_parm_decls ();
		  (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		;}
    break;

  case 124:
#line 766 "../../../gbagnu/gcc/cp/parse.y"
    { store_return_init ((yyval.ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 125:
#line 768 "../../../gbagnu/gcc/cp/parse.y"
    { store_return_init ((yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 126:
#line 770 "../../../gbagnu/gcc/cp/parse.y"
    { store_return_init ((yyval.ttype), NULL_TREE); ;}
    break;

  case 127:
#line 775 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if ((yyvsp[(3) - (3)].itype) == 0)
		    error ("no base initializers given following ':'");
		  setup_vtbl_ptr ();
		  /* Always keep the BLOCK node associated with the outermost
		     pair of curley braces of a function.  These are needed
		     for correct operation of dwarfout.c.  */
		  keep_next_level ();
		;}
    break;

  case 128:
#line 788 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (! current_function_parms_stored)
		    store_parm_decls ();

		  if (DECL_CONSTRUCTOR_P (current_function_decl))
		    {
		      /* Make a contour for the initializer list.  */
		      pushlevel (0);
		      clear_last_expr ();
		      expand_start_bindings (0);
		    }
		  else if (current_class_type == NULL_TREE)
		    error ("base initializers not allowed for non-member functions");
		  else if (! DECL_CONSTRUCTOR_P (current_function_decl))
		    error ("only constructors take base initializers");
		;}
    break;

  case 129:
#line 808 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 130:
#line 810 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 1; ;}
    break;

  case 133:
#line 817 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (current_class_name)
		    pedwarn ("anachronistic old style base class initializer");
		  expand_member_init (current_class_ref, NULL_TREE, (yyvsp[(2) - (3)].ttype));
		;}
    break;

  case 134:
#line 823 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (current_class_name)
		    pedwarn ("anachronistic old style base class initializer");
		  expand_member_init (current_class_ref, NULL_TREE, void_type_node);
		;}
    break;

  case 135:
#line 829 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 136:
#line 831 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, (yyvsp[(1) - (2)].ttype), void_type_node); ;}
    break;

  case 137:
#line 833 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 138:
#line 835 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, (yyvsp[(1) - (2)].ttype), void_type_node); ;}
    break;

  case 139:
#line 837 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, TYPE_MAIN_DECL ((yyvsp[(1) - (4)].ttype)),
				      (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 140:
#line 840 "../../../gbagnu/gcc/cp/parse.y"
    { expand_member_init (current_class_ref, TYPE_MAIN_DECL ((yyvsp[(1) - (2)].ttype)),
				      void_type_node); ;}
    break;

  case 152:
#line 866 "../../../gbagnu/gcc/cp/parse.y"
    { do_type_instantiation ((yyvsp[(3) - (4)].ftype).t, NULL_TREE);
		  yyungetc (';', 1); ;}
    break;

  case 154:
#line 870 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(3) - (4)].ftype).t);
		  do_decl_instantiation (specs, (yyvsp[(4) - (4)].ttype), NULL_TREE); ;}
    break;

  case 156:
#line 874 "../../../gbagnu/gcc/cp/parse.y"
    { do_decl_instantiation (NULL_TREE, (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 158:
#line 877 "../../../gbagnu/gcc/cp/parse.y"
    { do_decl_instantiation (NULL_TREE, (yyvsp[(3) - (3)].ttype), NULL_TREE); ;}
    break;

  case 160:
#line 880 "../../../gbagnu/gcc/cp/parse.y"
    { do_type_instantiation ((yyvsp[(4) - (5)].ftype).t, (yyvsp[(1) - (5)].ttype));
		  yyungetc (';', 1); ;}
    break;

  case 162:
#line 885 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(4) - (5)].ftype).t);
		  do_decl_instantiation (specs, (yyvsp[(5) - (5)].ttype), (yyvsp[(1) - (5)].ttype)); ;}
    break;

  case 164:
#line 889 "../../../gbagnu/gcc/cp/parse.y"
    { do_decl_instantiation (NULL_TREE, (yyvsp[(4) - (4)].ttype), (yyvsp[(1) - (4)].ttype)); ;}
    break;

  case 166:
#line 892 "../../../gbagnu/gcc/cp/parse.y"
    { do_decl_instantiation (NULL_TREE, (yyvsp[(4) - (4)].ttype), (yyvsp[(1) - (4)].ttype)); ;}
    break;

  case 168:
#line 897 "../../../gbagnu/gcc/cp/parse.y"
    { begin_explicit_instantiation(); ;}
    break;

  case 169:
#line 900 "../../../gbagnu/gcc/cp/parse.y"
    { end_explicit_instantiation(); ;}
    break;

  case 170:
#line 909 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(5) - (5)].ttype); ;}
    break;

  case 171:
#line 912 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(5) - (5)].ttype); ;}
    break;

  case 174:
#line 920 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(5) - (5)].ttype); ;}
    break;

  case 175:
#line 925 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(5) - (5)].ttype); ;}
    break;

  case 176:
#line 929 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  if (yychar == YYEMPTY)
		    yychar = YYLEX;

		  (yyval.ttype) = finish_template_type ((yyvsp[(-3) - (0)].ttype), (yyvsp[(-1) - (0)].ttype), 
					     yychar == SCOPE);
		;}
    break;

  case 178:
#line 940 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* Handle `Class<Class<Type>>' without space in the `>>' */
		  pedwarn ("`>>' should be `> >' in template class name");
		  yyungetc ('>', 1);
		;}
    break;

  case 179:
#line 949 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 181:
#line 955 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyval.ttype)); ;}
    break;

  case 182:
#line 957 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyval.ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 183:
#line 962 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = groktypename ((yyvsp[(1) - (1)].ftype).t); ;}
    break;

  case 184:
#line 964 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lastiddecl; ;}
    break;

  case 186:
#line 970 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.code) = NEGATE_EXPR; ;}
    break;

  case 187:
#line 972 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.code) = CONVERT_EXPR; ;}
    break;

  case 188:
#line 974 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.code) = PREINCREMENT_EXPR; ;}
    break;

  case 189:
#line 976 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.code) = PREDECREMENT_EXPR; ;}
    break;

  case 190:
#line 978 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.code) = TRUTH_NOT_EXPR; ;}
    break;

  case 191:
#line 983 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_compound_expr ((yyval.ttype)); ;}
    break;

  case 193:
#line 989 "../../../gbagnu/gcc/cp/parse.y"
    { error ("ANSI C++ forbids an empty condition for `%s'",
			 cond_stmt_keyword);
		  (yyval.ttype) = integer_zero_node; ;}
    break;

  case 194:
#line 993 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 195:
#line 998 "../../../gbagnu/gcc/cp/parse.y"
    { error ("ANSI C++ forbids an empty condition for `%s'",
			 cond_stmt_keyword);
		  (yyval.ttype) = integer_zero_node; ;}
    break;

  case 196:
#line 1002 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 197:
#line 1007 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 199:
#line 1010 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 200:
#line 1015 "../../../gbagnu/gcc/cp/parse.y"
    { {
		  tree d;
		  for (d = getdecls (); d; d = TREE_CHAIN (d))
		    if (TREE_CODE (d) == TYPE_DECL) {
		      tree s = TREE_TYPE (d);
		      if (TREE_CODE (s) == RECORD_TYPE)
			cp_error ("definition of class `%T' in condition", s);
		      else if (TREE_CODE (s) == ENUMERAL_TYPE)
			cp_error ("definition of enum `%T' in condition", s);
		    }
		  }
		  current_declspecs = (yyvsp[(1) - (5)].ftype).t;
		  (yyvsp[(5) - (5)].itype) = suspend_momentary ();
		  (yyval.ttype) = start_decl ((yyvsp[(2) - (5)].ttype), current_declspecs, 1,
					  (yyvsp[(4) - (5)].ttype), /*prefix_attributes*/ NULL_TREE);
		;}
    break;

  case 201:
#line 1032 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  cp_finish_decl ((yyvsp[(6) - (7)].ttype), (yyvsp[(7) - (7)].ttype), (yyvsp[(4) - (7)].ttype), 1, LOOKUP_ONLYCONVERTING);
		  resume_momentary ((yyvsp[(5) - (7)].itype));
		  (yyval.ttype) = convert_from_reference ((yyvsp[(6) - (7)].ttype)); 
		  if (TREE_CODE (TREE_TYPE ((yyval.ttype))) == ARRAY_TYPE)
		    cp_error ("definition of array `%#D' in condition", (yyval.ttype)); 
		;}
    break;

  case 207:
#line 1051 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_compound_stmt (1); ;}
    break;

  case 208:
#line 1053 "../../../gbagnu/gcc/cp/parse.y"
    { finish_compound_stmt (1, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 210:
#line 1060 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = expr_tree_cons (NULL_TREE, (yyval.ttype), 
		                  build_expr_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 211:
#line 1063 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = expr_tree_cons (NULL_TREE, (yyval.ttype), 
		                  build_expr_list (NULL_TREE, error_mark_node)); ;}
    break;

  case 212:
#line 1066 "../../../gbagnu/gcc/cp/parse.y"
    { chainon ((yyval.ttype), build_expr_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 213:
#line 1068 "../../../gbagnu/gcc/cp/parse.y"
    { chainon ((yyval.ttype), build_expr_list (NULL_TREE, error_mark_node)); ;}
    break;

  case 214:
#line 1073 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_expr_list (NULL_TREE, (yyval.ttype)); ;}
    break;

  case 216:
#line 1079 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 217:
#line 1082 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 218:
#line 1085 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_indirect_ref ((yyvsp[(2) - (2)].ttype), "unary *"); ;}
    break;

  case 219:
#line 1087 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_unary_op (ADDR_EXPR, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 220:
#line 1089 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_unary_op (BIT_NOT_EXPR, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 221:
#line 1091 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_unary_op_expr ((yyvsp[(1) - (2)].code), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 222:
#line 1094 "../../../gbagnu/gcc/cp/parse.y"
    { if (pedantic)
		    pedwarn ("ANSI C++ forbids `&&'");
  		  (yyval.ttype) = finish_label_address_expr ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 223:
#line 1098 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = expr_sizeof ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 224:
#line 1100 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = c_sizeof (groktypename ((yyvsp[(3) - (4)].ftype).t));
		  check_for_new_type ("sizeof", (yyvsp[(3) - (4)].ftype)); ;}
    break;

  case 225:
#line 1103 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grok_alignof ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 226:
#line 1105 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = c_alignof (groktypename ((yyvsp[(3) - (4)].ftype).t)); 
		  check_for_new_type ("alignof", (yyvsp[(3) - (4)].ftype)); ;}
    break;

  case 227:
#line 1111 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new (NULL_TREE, (yyvsp[(2) - (2)].ftype).t, NULL_TREE, (yyvsp[(1) - (2)].itype)); 
		  check_for_new_type ("new", (yyvsp[(2) - (2)].ftype)); ;}
    break;

  case 228:
#line 1114 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new (NULL_TREE, (yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].itype)); 
		  check_for_new_type ("new", (yyvsp[(2) - (3)].ftype)); ;}
    break;

  case 229:
#line 1117 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ftype).t, NULL_TREE, (yyvsp[(1) - (3)].itype)); 
		  check_for_new_type ("new", (yyvsp[(3) - (3)].ftype)); ;}
    break;

  case 230:
#line 1120 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new ((yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ftype).t, (yyvsp[(4) - (4)].ttype), (yyvsp[(1) - (4)].itype)); 
		  check_for_new_type ("new", (yyvsp[(3) - (4)].ftype)); ;}
    break;

  case 231:
#line 1133 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new (NULL_TREE, groktypename((yyvsp[(4) - (5)].ftype).t),
				  NULL_TREE, (yyvsp[(1) - (5)].itype)); 
		  check_for_new_type ("new", (yyvsp[(4) - (5)].ftype)); ;}
    break;

  case 232:
#line 1138 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new (NULL_TREE, groktypename((yyvsp[(4) - (6)].ftype).t), (yyvsp[(6) - (6)].ttype), (yyvsp[(1) - (6)].itype)); 
		  check_for_new_type ("new", (yyvsp[(4) - (6)].ftype)); ;}
    break;

  case 233:
#line 1142 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new ((yyvsp[(2) - (6)].ttype), groktypename((yyvsp[(5) - (6)].ftype).t), NULL_TREE, (yyvsp[(1) - (6)].itype)); 
		  check_for_new_type ("new", (yyvsp[(5) - (6)].ftype)); ;}
    break;

  case 234:
#line 1146 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_new ((yyvsp[(2) - (7)].ttype), groktypename((yyvsp[(5) - (7)].ftype).t), (yyvsp[(7) - (7)].ttype), (yyvsp[(1) - (7)].itype)); 
		  check_for_new_type ("new", (yyvsp[(5) - (7)].ftype)); ;}
    break;

  case 235:
#line 1150 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = delete_sanity ((yyvsp[(2) - (2)].ttype), NULL_TREE, 0, (yyvsp[(1) - (2)].itype)); ;}
    break;

  case 236:
#line 1152 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = delete_sanity ((yyvsp[(4) - (4)].ttype), NULL_TREE, 1, (yyvsp[(1) - (4)].itype));
		  if (yychar == YYEMPTY)
		    yychar = YYLEX; ;}
    break;

  case 237:
#line 1156 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = delete_sanity ((yyvsp[(5) - (5)].ttype), (yyvsp[(3) - (5)].ttype), 2, (yyvsp[(1) - (5)].itype));
		  if (yychar == YYEMPTY)
		    yychar = YYLEX; ;}
    break;

  case 238:
#line 1160 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_unary_op (REALPART_EXPR, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 239:
#line 1162 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_unary_op (IMAGPART_EXPR, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 240:
#line 1172 "../../../gbagnu/gcc/cp/parse.y"
    { finish_new_placement (NULL_TREE, (yyvsp[(-1) - (1)].itype)); ;}
    break;

  case 241:
#line 1175 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = begin_new_placement (); ;}
    break;

  case 242:
#line 1179 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_new_placement ((yyvsp[(3) - (4)].ttype), (yyvsp[(2) - (4)].itype)); ;}
    break;

  case 243:
#line 1181 "../../../gbagnu/gcc/cp/parse.y"
    { cp_pedwarn ("old style placement syntax, use () instead");
		  (yyval.ttype) = finish_new_placement ((yyvsp[(3) - (4)].ttype), (yyvsp[(2) - (4)].itype)); ;}
    break;

  case 244:
#line 1187 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 245:
#line 1189 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 246:
#line 1191 "../../../gbagnu/gcc/cp/parse.y"
    {
		  cp_error ("`%T' is not a valid expression", (yyvsp[(2) - (3)].ftype).t);
		  (yyval.ttype) = error_mark_node;
		;}
    break;

  case 247:
#line 1199 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (pedantic)
		    pedwarn ("ANSI C++ forbids initialization of new expression with `='");
		  if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) != TREE_LIST
		      && TREE_CODE ((yyvsp[(2) - (2)].ttype)) != CONSTRUCTOR)
		    (yyval.ttype) = build_expr_list (NULL_TREE, (yyvsp[(2) - (2)].ttype));
		  else
		    (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		;}
    break;

  case 248:
#line 1213 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(2) - (3)].ftype).t = finish_parmlist (build_tree_list (NULL_TREE, (yyvsp[(2) - (3)].ftype).t), 0);
		  (yyval.ttype) = make_call_declarator (NULL_TREE, (yyvsp[(2) - (3)].ftype).t, NULL_TREE, NULL_TREE);
		  check_for_new_type ("cast", (yyvsp[(2) - (3)].ftype)); ;}
    break;

  case 249:
#line 1217 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(3) - (4)].ftype).t = finish_parmlist (build_tree_list (NULL_TREE, (yyvsp[(3) - (4)].ftype).t), 0); 
		  (yyval.ttype) = make_call_declarator ((yyval.ttype), (yyvsp[(3) - (4)].ftype).t, NULL_TREE, NULL_TREE);
		  check_for_new_type ("cast", (yyvsp[(3) - (4)].ftype)); ;}
    break;

  case 251:
#line 1225 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = reparse_absdcl_as_casts ((yyval.ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 252:
#line 1227 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  tree init = build_nt (CONSTRUCTOR, NULL_TREE,
					nreverse ((yyvsp[(3) - (5)].ttype))); 
		  if (pedantic)
		    pedwarn ("ANSI C++ forbids constructor-expressions");
		  /* Indicate that this was a GNU C constructor expression.  */
		  TREE_HAS_CONSTRUCTOR (init) = 1;

		  (yyval.ttype) = reparse_absdcl_as_casts ((yyval.ttype), init);
		;}
    break;

  case 254:
#line 1243 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op (MEMBER_REF, (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 255:
#line 1245 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_m_component_ref ((yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 256:
#line 1247 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 257:
#line 1249 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 258:
#line 1251 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 259:
#line 1253 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 260:
#line 1255 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 261:
#line 1257 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 262:
#line 1259 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 263:
#line 1261 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 264:
#line 1263 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op (LT_EXPR, (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 265:
#line 1265 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op (GT_EXPR, (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 266:
#line 1267 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 267:
#line 1269 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 268:
#line 1271 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 269:
#line 1273 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 270:
#line 1275 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op ((yyvsp[(2) - (3)].code), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 271:
#line 1277 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op (TRUTH_ANDIF_EXPR, (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 272:
#line 1279 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_binary_op (TRUTH_ORIF_EXPR, (yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 273:
#line 1281 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_conditional_expr ((yyval.ttype), (yyvsp[(3) - (5)].ttype), (yyvsp[(5) - (5)].ttype)); ;}
    break;

  case 274:
#line 1283 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_modify_expr ((yyval.ttype), NOP_EXPR, (yyvsp[(3) - (3)].ttype));
		  if ((yyval.ttype) != error_mark_node)
                    C_SET_EXP_ORIGINAL_CODE ((yyval.ttype), MODIFY_EXPR); ;}
    break;

  case 275:
#line 1287 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_modify_expr ((yyval.ttype), (yyvsp[(2) - (3)].code), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 276:
#line 1289 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_throw (NULL_TREE); ;}
    break;

  case 277:
#line 1291 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_throw ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 278:
#line 1309 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (BIT_NOT_EXPR, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 279:
#line 1311 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (BIT_NOT_EXPR, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 285:
#line 1320 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* If lastiddecl is a TREE_LIST, it's a baselink, which
		     means that we're in an expression like S::f<int>, so
		     don't do_identifier; we only do that for unqualified
		     identifiers.  */
		  if (lastiddecl && TREE_CODE (lastiddecl) != TREE_LIST)
		    (yyval.ttype) = do_identifier ((yyvsp[(-1) - (0)].ttype), 1, NULL_TREE);
		  else
		    (yyval.ttype) = (yyvsp[(-1) - (0)].ttype);
		;}
    break;

  case 286:
#line 1333 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(3) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 287:
#line 1335 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(3) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 288:
#line 1340 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 289:
#line 1342 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 290:
#line 1345 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 295:
#line 1357 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* Provide support for '(' attributes '*' declarator ')'
		     etc */
		  (yyval.ttype) = decl_tree_cons ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype), NULL_TREE);
		;}
    break;

  case 297:
#line 1367 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (INDIRECT_REF, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 298:
#line 1369 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ADDR_EXPR, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 299:
#line 1371 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 300:
#line 1376 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 301:
#line 1378 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = lookup_template_function ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 305:
#line 1388 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_decl_parsing ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 306:
#line 1393 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (1)].ttype)) == BIT_NOT_EXPR)
		    (yyval.ttype) = build_x_unary_op (BIT_NOT_EXPR, TREE_OPERAND ((yyvsp[(1) - (1)].ttype), 0));
		  else 
		    (yyval.ttype) = finish_id_expr ((yyvsp[(1) - (1)].ttype));
		;}
    break;

  case 309:
#line 1402 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (processing_template_decl)
		    push_obstacks (&permanent_obstack, &permanent_obstack);
		  (yyval.ttype) = combine_strings ((yyval.ttype));
		  /* combine_strings doesn't set up TYPE_MAIN_VARIANT of
		     a const array the way we want, so fix it.  */
		  if (flag_const_strings)
		    TREE_TYPE ((yyval.ttype)) = build_cplus_array_type
		      (TREE_TYPE (TREE_TYPE ((yyval.ttype))),
		       TYPE_DOMAIN (TREE_TYPE ((yyval.ttype))));
		  if (processing_template_decl)
		    pop_obstacks ();
		;}
    break;

  case 310:
#line 1416 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parenthesized_expr ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 311:
#line 1418 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(2) - (3)].ttype) = reparse_decl_as_expr (NULL_TREE, (yyvsp[(2) - (3)].ttype));
		  (yyval.ttype) = finish_parenthesized_expr ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 312:
#line 1421 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = error_mark_node; ;}
    break;

  case 313:
#line 1423 "../../../gbagnu/gcc/cp/parse.y"
    { tree scope = current_scope ();
		  if (!scope || TREE_CODE (scope) != FUNCTION_DECL)
		    {
		      error ("braced-group within expression allowed only inside a function");
		      YYERROR;
		    }
		  if (pedantic)
		    pedwarn ("ANSI C++ forbids braced-groups within expressions");  
		  (yyval.ttype) = begin_stmt_expr (); 
		;}
    break;

  case 314:
#line 1434 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_stmt_expr ((yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 315:
#line 1439 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_call_expr ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype), 1); ;}
    break;

  case 316:
#line 1441 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_call_expr ((yyvsp[(1) - (2)].ttype), NULL_TREE, 1); ;}
    break;

  case 317:
#line 1443 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_call_expr ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype), 0); ;}
    break;

  case 318:
#line 1445 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_call_expr ((yyvsp[(1) - (2)].ttype), NULL_TREE, 0); ;}
    break;

  case 319:
#line 1447 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grok_array_decl ((yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 320:
#line 1449 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_increment_expr ((yyvsp[(1) - (2)].ttype), POSTINCREMENT_EXPR); ;}
    break;

  case 321:
#line 1451 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_increment_expr ((yyvsp[(1) - (2)].ttype), POSTDECREMENT_EXPR); ;}
    break;

  case 322:
#line 1454 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_this_expr (); ;}
    break;

  case 323:
#line 1456 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* This is a C cast in C++'s `functional' notation
		     using the "implicit int" extension so that:
		     `const (3)' is equivalent to `const int (3)'.  */
		  tree type;

		  if ((yyvsp[(3) - (4)].ttype) == error_mark_node)
		    {
		      (yyval.ttype) = error_mark_node;
		      break;
		    }

		  type = cp_build_qualified_type (integer_type_node,
						  cp_type_qual_from_rid ((yyvsp[(1) - (4)].ttype)));
		  (yyval.ttype) = build_c_cast (type, build_compound_expr ((yyvsp[(3) - (4)].ttype)));
		;}
    break;

  case 325:
#line 1474 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (7)].ftype).t);
		  check_for_new_type ("dynamic_cast", (yyvsp[(3) - (7)].ftype));
		  (yyval.ttype) = build_dynamic_cast (type, (yyvsp[(6) - (7)].ttype)); ;}
    break;

  case 326:
#line 1478 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (7)].ftype).t);
		  check_for_new_type ("static_cast", (yyvsp[(3) - (7)].ftype));
		  (yyval.ttype) = build_static_cast (type, (yyvsp[(6) - (7)].ttype)); ;}
    break;

  case 327:
#line 1482 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (7)].ftype).t);
		  check_for_new_type ("reinterpret_cast", (yyvsp[(3) - (7)].ftype));
		  (yyval.ttype) = build_reinterpret_cast (type, (yyvsp[(6) - (7)].ttype)); ;}
    break;

  case 328:
#line 1486 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (7)].ftype).t);
		  check_for_new_type ("const_cast", (yyvsp[(3) - (7)].ftype));
		  (yyval.ttype) = build_const_cast (type, (yyvsp[(6) - (7)].ttype)); ;}
    break;

  case 329:
#line 1490 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_typeid ((yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 330:
#line 1492 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (4)].ftype).t);
		  check_for_new_type ("typeid", (yyvsp[(3) - (4)].ftype));
		  (yyval.ttype) = get_typeid (TYPE_MAIN_VARIANT (type)); ;}
    break;

  case 331:
#line 1496 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = do_scoped_id ((yyvsp[(2) - (2)].ttype), 1); ;}
    break;

  case 332:
#line 1498 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 333:
#line 1500 "../../../gbagnu/gcc/cp/parse.y"
    {
		  got_scope = NULL_TREE;
		  if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = do_scoped_id ((yyvsp[(2) - (2)].ttype), 1);
		  else
		    (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		;}
    break;

  case 334:
#line 1508 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_offset_ref (OP0 ((yyval.ttype)), OP1 ((yyval.ttype))); ;}
    break;

  case 335:
#line 1510 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_qualified_call_expr ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 336:
#line 1512 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_qualified_call_expr ((yyvsp[(1) - (2)].ttype), NULL_TREE); ;}
    break;

  case 337:
#line 1514 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  (yyval.ttype) = build_x_component_ref ((yyval.ttype), (yyvsp[(2) - (2)].ttype), NULL_TREE, 1); 
		;}
    break;

  case 338:
#line 1518 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_object_call_expr ((yyvsp[(2) - (5)].ttype), (yyvsp[(1) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 339:
#line 1520 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_object_call_expr ((yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 340:
#line 1522 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_x_component_ref ((yyval.ttype), (yyvsp[(2) - (2)].ttype), NULL_TREE, 1); ;}
    break;

  case 341:
#line 1524 "../../../gbagnu/gcc/cp/parse.y"
    { if (processing_template_decl)
		    (yyval.ttype) = build_min_nt (COMPONENT_REF, (yyvsp[(1) - (2)].ttype), copy_to_permanent ((yyvsp[(2) - (2)].ttype)));
		  else
		    (yyval.ttype) = build_object_ref ((yyval.ttype), OP0 ((yyvsp[(2) - (2)].ttype)), OP1 ((yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 342:
#line 1529 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_object_call_expr ((yyvsp[(2) - (5)].ttype), (yyvsp[(1) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 343:
#line 1531 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_object_call_expr ((yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 344:
#line 1533 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_qualified_object_call_expr ((yyvsp[(2) - (5)].ttype), (yyvsp[(1) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 345:
#line 1535 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_qualified_object_call_expr ((yyvsp[(2) - (3)].ttype), (yyvsp[(1) - (3)].ttype), NULL_TREE); ;}
    break;

  case 346:
#line 1538 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_pseudo_destructor_call_expr ((yyvsp[(1) - (4)].ttype), NULL_TREE, (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 347:
#line 1540 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_pseudo_destructor_call_expr ((yyvsp[(1) - (6)].ttype), (yyvsp[(2) - (6)].ttype), (yyvsp[(5) - (6)].ttype)); ;}
    break;

  case 348:
#line 1542 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = error_mark_node;
		;}
    break;

  case 349:
#line 1587 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 350:
#line 1589 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE; (yyval.itype) = 1; ;}
    break;

  case 351:
#line 1594 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 352:
#line 1596 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE; (yyval.itype) = 1; ;}
    break;

  case 353:
#line 1601 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = boolean_true_node; ;}
    break;

  case 354:
#line 1603 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = boolean_false_node; ;}
    break;

  case 356:
#line 1610 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyval.ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 357:
#line 1615 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (! current_function_parms_stored)
		    store_parm_decls ();
		  setup_vtbl_ptr ();
		  /* Always keep the BLOCK node associated with the outermost
		     pair of curley braces of a function.  These are needed
		     for correct operation of dwarfout.c.  */
		  keep_next_level ();
		;}
    break;

  case 358:
#line 1628 "../../../gbagnu/gcc/cp/parse.y"
    { got_object = TREE_TYPE ((yyval.ttype)); ;}
    break;

  case 359:
#line 1630 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = build_x_arrow ((yyval.ttype)); 
		  got_object = TREE_TYPE ((yyval.ttype));
		;}
    break;

  case 360:
#line 1638 "../../../gbagnu/gcc/cp/parse.y"
    {
		  resume_momentary ((yyvsp[(2) - (3)].itype));
		  if ((yyvsp[(1) - (3)].ftype).t && IS_AGGR_TYPE_CODE (TREE_CODE ((yyvsp[(1) - (3)].ftype).t)))
		    note_got_semicolon ((yyvsp[(1) - (3)].ftype).t);
		;}
    break;

  case 361:
#line 1644 "../../../gbagnu/gcc/cp/parse.y"
    {
		  resume_momentary ((yyvsp[(2) - (3)].itype));
		  note_list_got_semicolon ((yyvsp[(1) - (3)].ftype).t);
		;}
    break;

  case 362:
#line 1649 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((yyvsp[(2) - (3)].itype)); ;}
    break;

  case 363:
#line 1651 "../../../gbagnu/gcc/cp/parse.y"
    {
		  shadow_tag ((yyvsp[(1) - (2)].ftype).t);
		  note_list_got_semicolon ((yyvsp[(1) - (2)].ftype).t);
		;}
    break;

  case 364:
#line 1656 "../../../gbagnu/gcc/cp/parse.y"
    { warning ("empty declaration"); ;}
    break;

  case 365:
#line 1658 "../../../gbagnu/gcc/cp/parse.y"
    { pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 368:
#line 1672 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator (NULL_TREE, empty_parms (),
					     NULL_TREE, NULL_TREE); ;}
    break;

  case 369:
#line 1675 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), NULL_TREE,
					     NULL_TREE); ;}
    break;

  case 370:
#line 1682 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 371:
#line 1685 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 372:
#line 1688 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list (get_decl_list ((yyvsp[(1) - (2)].ftype).t), (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 373:
#line 1691 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (1)].ftype).t, NULL_TREE);
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag;  ;}
    break;

  case 374:
#line 1694 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (1)].ftype).t, NULL_TREE); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag; ;}
    break;

  case 377:
#line 1710 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ftype).t, (yyvsp[(1) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(2) - (2)].ftype).new_type_flag; ;}
    break;

  case 378:
#line 1713 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 379:
#line 1716 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(1) - (3)].ftype).t, chainon ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype))); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (3)].ftype).new_type_flag; ;}
    break;

  case 380:
#line 1719 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ftype).t, chainon ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ttype))); 
		  (yyval.ftype).new_type_flag = (yyvsp[(2) - (3)].ftype).new_type_flag; ;}
    break;

  case 381:
#line 1722 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ftype).t, chainon ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ttype))); 
		  (yyval.ftype).new_type_flag = (yyvsp[(2) - (3)].ftype).new_type_flag; ;}
    break;

  case 382:
#line 1725 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (4)].ftype).t,
					 chainon ((yyvsp[(3) - (4)].ttype), chainon ((yyvsp[(4) - (4)].ttype), (yyvsp[(1) - (4)].ttype)))); 
		  (yyval.ftype).new_type_flag = (yyvsp[(2) - (4)].ftype).new_type_flag; ;}
    break;

  case 383:
#line 1732 "../../../gbagnu/gcc/cp/parse.y"
    { if (extra_warnings)
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyval.ttype)));
		  (yyval.ttype) = build_decl_list (NULL_TREE, (yyval.ttype)); ;}
    break;

  case 384:
#line 1737 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ftype).t, (yyval.ttype)); ;}
    break;

  case 385:
#line 1739 "../../../gbagnu/gcc/cp/parse.y"
    { if (extra_warnings)
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype)));
		  (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyval.ttype)); ;}
    break;

  case 386:
#line 1744 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons ((yyvsp[(2) - (2)].ttype), NULL_TREE, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 387:
#line 1746 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons ((yyvsp[(1) - (1)].ttype), NULL_TREE, NULL_TREE); ;}
    break;

  case 388:
#line 1756 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ftype).t; TREE_STATIC ((yyval.ttype)) = 1; ;}
    break;

  case 389:
#line 1758 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = IDENTIFIER_AS_LIST ((yyval.ttype)); ;}
    break;

  case 390:
#line 1760 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyval.ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; ;}
    break;

  case 391:
#line 1763 "../../../gbagnu/gcc/cp/parse.y"
    { if (extra_warnings && TREE_STATIC ((yyval.ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype)));
		  (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyval.ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 392:
#line 1769 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons ((yyvsp[(2) - (2)].ttype), NULL_TREE, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 393:
#line 1771 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons ((yyvsp[(1) - (1)].ttype), NULL_TREE, NULL_TREE); ;}
    break;

  case 394:
#line 1782 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = get_decl_list ((yyvsp[(1) - (1)].ftype).t); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag; ;}
    break;

  case 395:
#line 1785 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ftype).t, (yyvsp[(1) - (2)].ftype).t); 
		  (yyval.ftype).new_type_flag = (yyvsp[(2) - (2)].ftype).new_type_flag; ;}
    break;

  case 396:
#line 1788 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 397:
#line 1791 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (3)].ftype).t, chainon ((yyvsp[(3) - (3)].ttype), (yyvsp[(1) - (3)].ftype).t)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (3)].ftype).new_type_flag; ;}
    break;

  case 398:
#line 1797 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_decl_list (NULL_TREE, (yyvsp[(1) - (1)].ftype).t); ;}
    break;

  case 399:
#line 1799 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ftype).t, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 401:
#line 1809 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(1) - (1)].ttype); (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 402:
#line 1811 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(1) - (1)].ttype); (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 403:
#line 1813 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = finish_typeof ((yyvsp[(3) - (4)].ttype));
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 404:
#line 1816 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = groktypename ((yyvsp[(3) - (4)].ftype).t);
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 405:
#line 1819 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = TREE_TYPE ((yyvsp[(3) - (4)].ttype));

                  (yyval.ftype).new_type_flag = 0;
		  if (IS_AGGR_TYPE (type))
		    {
		      sorry ("sigof type specifier");
		      (yyval.ftype).t = type;
		    }
		  else
		    {
		      error ("`sigof' applied to non-aggregate expression");
		      (yyval.ftype).t = error_mark_node;
		    }
		;}
    break;

  case 406:
#line 1834 "../../../gbagnu/gcc/cp/parse.y"
    { tree type = groktypename ((yyvsp[(3) - (4)].ftype).t);

                  (yyval.ftype).new_type_flag = 0;
		  if (IS_AGGR_TYPE (type))
		    {
		      sorry ("sigof type specifier");
		      (yyval.ftype).t = type;
		    }
		  else
		    {
		      error("`sigof' applied to non-aggregate type");
		      (yyval.ftype).t = error_mark_node;
		    }
		;}
    break;

  case 407:
#line 1854 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(1) - (1)].ttype); (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 408:
#line 1856 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(1) - (1)].ttype); (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 411:
#line 1863 "../../../gbagnu/gcc/cp/parse.y"
    { check_multiple_declarators (); ;}
    break;

  case 413:
#line 1869 "../../../gbagnu/gcc/cp/parse.y"
    { check_multiple_declarators (); ;}
    break;

  case 415:
#line 1875 "../../../gbagnu/gcc/cp/parse.y"
    { check_multiple_declarators (); ;}
    break;

  case 416:
#line 1880 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 417:
#line 1882 "../../../gbagnu/gcc/cp/parse.y"
    { if (TREE_CHAIN ((yyvsp[(3) - (4)].ttype))) (yyvsp[(3) - (4)].ttype) = combine_strings ((yyvsp[(3) - (4)].ttype)); (yyval.ttype) = (yyvsp[(3) - (4)].ttype); ;}
    break;

  case 418:
#line 1887 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = start_decl ((yyvsp[(1) - (4)].ttype), current_declspecs, 1,
					  (yyvsp[(3) - (4)].ttype), prefix_attributes); ;}
    break;

  case 419:
#line 1891 "../../../gbagnu/gcc/cp/parse.y"
    { cp_finish_decl ((yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype), (yyvsp[(2) - (6)].ttype), 1, LOOKUP_ONLYCONVERTING); ;}
    break;

  case 420:
#line 1893 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = start_decl ((yyvsp[(1) - (3)].ttype), current_declspecs, 0,
					  (yyvsp[(3) - (3)].ttype), prefix_attributes);
		  cp_finish_decl ((yyval.ttype), NULL_TREE, (yyvsp[(2) - (3)].ttype), 1, 0); ;}
    break;

  case 421:
#line 1906 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(2) - (2)].itype) = parse_decl ((yyvsp[(-1) - (2)].ttype), (yyvsp[(-2) - (2)].ttype), 
					   (yyvsp[(1) - (2)].ttype), 1, &(yyval.ttype)); ;}
    break;

  case 422:
#line 1911 "../../../gbagnu/gcc/cp/parse.y"
    { cp_finish_decl ((yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype), (yyvsp[(0) - (4)].ttype), 1,
				  LOOKUP_ONLYCONVERTING);
		  (yyval.itype) = (yyvsp[(2) - (4)].itype); ;}
    break;

  case 423:
#line 1915 "../../../gbagnu/gcc/cp/parse.y"
    { tree d;
		  (yyval.itype) = parse_decl ((yyvsp[(-1) - (1)].ttype), (yyvsp[(-2) - (1)].ttype), (yyvsp[(1) - (1)].ttype), 0, &d);
		  cp_finish_decl (d, NULL_TREE, (yyvsp[(0) - (1)].ttype), 1, 0); ;}
    break;

  case 424:
#line 1922 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = (yyvsp[(3) - (3)].itype); ;}
    break;

  case 425:
#line 1926 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = (yyvsp[(3) - (3)].itype); ;}
    break;

  case 426:
#line 1931 "../../../gbagnu/gcc/cp/parse.y"
    { /* Set things up as initdcl0_innards expects.  */
	      (yyvsp[(2) - (2)].ttype) = (yyvsp[(1) - (2)].ttype); 
              (yyvsp[(1) - (2)].ttype) = NULL_TREE; ;}
    break;

  case 427:
#line 1935 "../../../gbagnu/gcc/cp/parse.y"
    {;}
    break;

  case 428:
#line 1937 "../../../gbagnu/gcc/cp/parse.y"
    { tree d;
		  parse_decl((yyvsp[(1) - (3)].ttype), NULL_TREE, (yyvsp[(3) - (3)].ttype), 0, &d);
		  cp_finish_decl (d, NULL_TREE, (yyvsp[(2) - (3)].ttype), 1, 0); ;}
    break;

  case 429:
#line 1946 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 430:
#line 1948 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 431:
#line 1953 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 432:
#line 1955 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 433:
#line 1960 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(4) - (6)].ttype); ;}
    break;

  case 434:
#line 1965 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(1) - (1)].ttype); ;}
    break;

  case 435:
#line 1967 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 436:
#line 1972 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 437:
#line 1974 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (1)].ttype), NULL_TREE); ;}
    break;

  case 438:
#line 1976 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (4)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (4)].ttype))); ;}
    break;

  case 439:
#line 1978 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (6)].ttype), tree_cons (NULL_TREE, (yyvsp[(3) - (6)].ttype), (yyvsp[(5) - (6)].ttype))); ;}
    break;

  case 440:
#line 1980 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 445:
#line 1996 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 446:
#line 1998 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyvsp[(1) - (3)].ttype), build_tree_list (NULL_TREE, (yyvsp[(3) - (3)].ttype))); ;}
    break;

  case 447:
#line 2003 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 448:
#line 2005 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 450:
#line 2013 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_nt (CONSTRUCTOR, NULL_TREE, NULL_TREE);
		  TREE_HAS_CONSTRUCTOR ((yyval.ttype)) = 1; ;}
    break;

  case 451:
#line 2016 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_nt (CONSTRUCTOR, NULL_TREE, nreverse ((yyvsp[(2) - (3)].ttype)));
		  TREE_HAS_CONSTRUCTOR ((yyval.ttype)) = 1; ;}
    break;

  case 452:
#line 2019 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_nt (CONSTRUCTOR, NULL_TREE, nreverse ((yyvsp[(2) - (4)].ttype)));
		  TREE_HAS_CONSTRUCTOR ((yyval.ttype)) = 1; ;}
    break;

  case 453:
#line 2022 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 454:
#line 2029 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list (NULL_TREE, (yyval.ttype)); ;}
    break;

  case 455:
#line 2031 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = expr_tree_cons (NULL_TREE, (yyvsp[(3) - (3)].ttype), (yyval.ttype)); ;}
    break;

  case 456:
#line 2034 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_expr_list ((yyvsp[(2) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 457:
#line 2036 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_expr_list ((yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 458:
#line 2038 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = expr_tree_cons ((yyvsp[(3) - (5)].ttype), (yyvsp[(5) - (5)].ttype), (yyval.ttype)); ;}
    break;

  case 459:
#line 2043 "../../../gbagnu/gcc/cp/parse.y"
    { start_function (NULL_TREE, TREE_VALUE ((yyvsp[(1) - (1)].ttype)),
				  NULL_TREE, 2);
		  reinit_parse_for_function (); ;}
    break;

  case 460:
#line 2049 "../../../gbagnu/gcc/cp/parse.y"
    {
		  int nested = (hack_decl_function_context
				(current_function_decl) != NULL_TREE);
		  finish_function (lineno, (int)(yyvsp[(3) - (4)].itype) | 2, nested);
		  process_next_inline ((yyvsp[(1) - (4)].ttype));
		;}
    break;

  case 461:
#line 2056 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  int nested = (hack_decl_function_context
				(current_function_decl) != NULL_TREE);
		  finish_function (lineno, (int)(yyvsp[(3) - (3)].itype) | 2, nested); 
                  process_next_inline ((yyvsp[(1) - (3)].ttype));
		;}
    break;

  case 462:
#line 2063 "../../../gbagnu/gcc/cp/parse.y"
    { process_next_inline ((yyvsp[(1) - (3)].ttype)); ;}
    break;

  case 465:
#line 2075 "../../../gbagnu/gcc/cp/parse.y"
    { replace_defarg ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 466:
#line 2077 "../../../gbagnu/gcc/cp/parse.y"
    { replace_defarg ((yyvsp[(1) - (3)].ttype), error_mark_node); ;}
    break;

  case 468:
#line 2082 "../../../gbagnu/gcc/cp/parse.y"
    { do_pending_defargs (); ;}
    break;

  case 469:
#line 2084 "../../../gbagnu/gcc/cp/parse.y"
    { do_pending_defargs (); ;}
    break;

  case 470:
#line 2089 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(3) - (3)].itype) = suspend_momentary ();
		  (yyval.ttype) = current_enum_type;
		  current_enum_type = start_enum ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 471:
#line 2093 "../../../gbagnu/gcc/cp/parse.y"
    { TYPE_VALUES (current_enum_type) = (yyvsp[(5) - (7)].ttype);
		  (yyval.ftype).t = finish_enum (current_enum_type);
		  (yyval.ftype).new_type_flag = 1;
		  current_enum_type = (yyvsp[(4) - (7)].ttype);
		  resume_momentary ((int) (yyvsp[(3) - (7)].itype));
		  check_for_missing_semicolon ((yyval.ftype).t); ;}
    break;

  case 472:
#line 2100 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = finish_enum (start_enum ((yyvsp[(2) - (4)].ttype)));
		  (yyval.ftype).new_type_flag = 1;
		  check_for_missing_semicolon ((yyval.ftype).t); ;}
    break;

  case 473:
#line 2104 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(2) - (2)].itype) = suspend_momentary ();
		  (yyval.ttype) = current_enum_type;
		  current_enum_type = start_enum (make_anon_name ()); ;}
    break;

  case 474:
#line 2108 "../../../gbagnu/gcc/cp/parse.y"
    { TYPE_VALUES (current_enum_type) = (yyvsp[(4) - (6)].ttype);
		  (yyval.ftype).t = finish_enum (current_enum_type);
		  (yyval.ftype).new_type_flag = 1;
		  current_enum_type = (yyvsp[(3) - (6)].ttype);
		  resume_momentary ((int) (yyvsp[(1) - (6)].itype));
		  check_for_missing_semicolon ((yyval.ftype).t); ;}
    break;

  case 475:
#line 2115 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = finish_enum (start_enum (make_anon_name()));
		  (yyval.ftype).new_type_flag = 1;
		  check_for_missing_semicolon ((yyval.ftype).t); ;}
    break;

  case 476:
#line 2119 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = xref_tag (enum_type_node, (yyvsp[(2) - (2)].ttype), 1); 
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 477:
#line 2122 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = xref_tag (enum_type_node, (yyvsp[(2) - (2)].ttype), 1); 
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 478:
#line 2125 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(2) - (2)].ttype);
		  (yyval.ftype).new_type_flag = 0; 
		  if (!processing_template_decl)
		    cp_pedwarn ("using `typename' outside of template"); ;}
    break;

  case 479:
#line 2132 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  int semi;

		  if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  semi = yychar == ';';

		  (yyval.ttype) = finish_class_definition ((yyvsp[(1) - (5)].ttype), (yyvsp[(5) - (5)].ttype), semi); 
		;}
    break;

  case 480:
#line 2142 "../../../gbagnu/gcc/cp/parse.y"
    { finish_default_args (); ;}
    break;

  case 481:
#line 2144 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = (yyvsp[(6) - (9)].ttype);
		  (yyval.ftype).new_type_flag = 1; 
		  begin_inline_definitions (); ;}
    break;

  case 482:
#line 2148 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ftype).new_type_flag = 0;
		  if (TYPE_BINFO ((yyvsp[(1) - (1)].ttype)) == NULL_TREE)
		    {
		      cp_error ("%T is not a class type", (yyvsp[(1) - (1)].ttype));
		      (yyval.ftype).t = error_mark_node;
		    } 
		  else
		    {
		      (yyval.ftype).t = (yyvsp[(1) - (1)].ttype);
		      /* struct B: public A; is not accepted by the WP grammar.  */
		      if (TYPE_BINFO_BASETYPES ((yyval.ftype).t) && !TYPE_SIZE ((yyval.ftype).t)
			  && ! TYPE_BEING_DEFINED ((yyval.ftype).t))
			cp_error ("base clause without member specification for `%#T'",
				  (yyval.ftype).t);
		    }
		;}
    break;

  case 486:
#line 2175 "../../../gbagnu/gcc/cp/parse.y"
    { if (pedantic && !in_system_header)
		    pedwarn ("comma at end of enumerator list"); ;}
    break;

  case 488:
#line 2182 "../../../gbagnu/gcc/cp/parse.y"
    { error ("storage class specifier `%s' not allowed after struct or class", IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 489:
#line 2184 "../../../gbagnu/gcc/cp/parse.y"
    { error ("type specifier `%s' not allowed after struct or class", IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 490:
#line 2186 "../../../gbagnu/gcc/cp/parse.y"
    { error ("type qualifier `%s' not allowed after struct or class", IDENTIFIER_POINTER ((yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 491:
#line 2188 "../../../gbagnu/gcc/cp/parse.y"
    { error ("no body nor ';' separates two class, struct or union declarations"); ;}
    break;

  case 492:
#line 2190 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_decl_list ((yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 493:
#line 2195 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  current_aggr = (yyvsp[(1) - (2)].ttype); 
		  (yyval.ttype) = (yyvsp[(2) - (2)].ttype); 
		;}
    break;

  case 494:
#line 2203 "../../../gbagnu/gcc/cp/parse.y"
    { current_aggr = (yyval.ttype); (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 495:
#line 2205 "../../../gbagnu/gcc/cp/parse.y"
    { yyungetc ('{', 1); ;}
    break;

  case 496:
#line 2207 "../../../gbagnu/gcc/cp/parse.y"
    { yyungetc (':', 1); ;}
    break;

  case 497:
#line 2212 "../../../gbagnu/gcc/cp/parse.y"
    {
		  current_aggr = (yyvsp[(1) - (3)].ttype);
		  (yyval.ttype) = handle_class_head ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		;}
    break;

  case 498:
#line 2217 "../../../gbagnu/gcc/cp/parse.y"
    {
		  current_aggr = (yyvsp[(1) - (4)].ttype);
		  (yyval.ttype) = handle_class_head ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype));
		;}
    break;

  case 499:
#line 2222 "../../../gbagnu/gcc/cp/parse.y"
    {
		  current_aggr = (yyvsp[(1) - (3)].ttype);
		  (yyval.ttype) = handle_class_head ((yyvsp[(1) - (3)].ttype), NULL_TREE, (yyvsp[(3) - (3)].ttype));
		;}
    break;

  case 500:
#line 2227 "../../../gbagnu/gcc/cp/parse.y"
    { current_aggr = (yyval.ttype); (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 501:
#line 2229 "../../../gbagnu/gcc/cp/parse.y"
    { current_aggr = (yyval.ttype); (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 502:
#line 2234 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = xref_tag (current_aggr, (yyvsp[(1) - (1)].ttype), 1); ;}
    break;

  case 503:
#line 2236 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = xref_tag (current_aggr, (yyvsp[(1) - (1)].ttype), 0); ;}
    break;

  case 504:
#line 2240 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  (yyval.ttype) = (yyvsp[(2) - (3)].ttype);
		  if ((yyvsp[(3) - (3)].ttype))
                    xref_basetypes (current_aggr, (yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); 
		;}
    break;

  case 505:
#line 2246 "../../../gbagnu/gcc/cp/parse.y"
    { push_scope (CP_DECL_CONTEXT ((yyvsp[(1) - (1)].ttype))); ;}
    break;

  case 506:
#line 2248 "../../../gbagnu/gcc/cp/parse.y"
    { 
                  pop_scope (CP_DECL_CONTEXT ((yyvsp[(1) - (3)].ttype)));
		  (yyval.ttype) = TREE_TYPE ((yyvsp[(1) - (3)].ttype));
		  if (current_aggr == union_type_node
		      && TREE_CODE ((yyval.ttype)) != UNION_TYPE)
		    cp_pedwarn ("`union' tag used in declaring `%#T'", (yyval.ttype));
		  else if (TREE_CODE ((yyval.ttype)) == UNION_TYPE
			   && current_aggr != union_type_node)
		    cp_pedwarn ("non-`union' tag used in declaring `%#T'", (yyval.ttype));
		  else if (TREE_CODE ((yyval.ttype)) == RECORD_TYPE)
		    /* We might be specializing a template with a different
		       class-key; deal.  */
		    CLASSTYPE_DECLARED_CLASS ((yyval.ttype)) = (current_aggr
						     == class_type_node);
		  if ((yyvsp[(3) - (3)].ttype))
		    {
		      maybe_process_partial_specialization ((yyval.ttype));
		      xref_basetypes (current_aggr, (yyvsp[(1) - (3)].ttype), (yyval.ttype), (yyvsp[(3) - (3)].ttype)); 
		    }
		;}
    break;

  case 507:
#line 2272 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = xref_tag ((yyval.ttype), make_anon_name (), 0);
		  yyungetc ('{', 1); ;}
    break;

  case 510:
#line 2283 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 511:
#line 2285 "../../../gbagnu/gcc/cp/parse.y"
    { yyungetc(':', 1); (yyval.ttype) = NULL_TREE; ;}
    break;

  case 512:
#line 2287 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 514:
#line 2293 "../../../gbagnu/gcc/cp/parse.y"
    { /* CYGNUS LOCAL Embedded C++ */
		  if (flag_embedded_cxx)
		    pedwarn ("Embedded C++ prohibits multiple inheritance");
		  /* END CYGNUS LOCAL Embedded C++ */
		  (yyval.ttype) = chainon ((yyval.ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 515:
#line 2302 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_base_specifier (access_default_node, (yyvsp[(1) - (1)].ttype),
					      current_aggr 
					      == signature_type_node); ;}
    break;

  case 516:
#line 2306 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_base_specifier ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype), 
					      current_aggr 
					      == signature_type_node); ;}
    break;

  case 517:
#line 2313 "../../../gbagnu/gcc/cp/parse.y"
    { if ((yyval.ttype) != error_mark_node) (yyval.ttype) = TYPE_MAIN_DECL ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 519:
#line 2316 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (current_aggr == signature_type_node)
		    {
		      if (IS_AGGR_TYPE (TREE_TYPE ((yyvsp[(3) - (4)].ttype))))
			{
			  sorry ("`sigof' as base signature specifier");
			  (yyval.ttype) = TREE_TYPE ((yyvsp[(3) - (4)].ttype));
			}
		      else
			{
			  error ("`sigof' applied to non-aggregate expression");
			  (yyval.ttype) = error_mark_node;
			}
		    }
		  else
		    {
		      error ("`sigof' in struct or class declaration");
		      (yyval.ttype) = error_mark_node;
		    }
		;}
    break;

  case 520:
#line 2337 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (current_aggr == signature_type_node)
		    {
		      if (IS_AGGR_TYPE (groktypename ((yyvsp[(3) - (4)].ftype).t)))
			{
			  sorry ("`sigof' as base signature specifier");
			  (yyval.ttype) = groktypename ((yyvsp[(3) - (4)].ftype).t);
			}
		      else
			{
			  error ("`sigof' applied to non-aggregate expression");
			  (yyval.ttype) = error_mark_node;
			}
		    }
		  else
		    {
		      error ("`sigof' in struct or class declaration");
		      (yyval.ttype) = error_mark_node;
		    }
		;}
    break;

  case 522:
#line 2362 "../../../gbagnu/gcc/cp/parse.y"
    { if ((yyvsp[(1) - (2)].ttype) != ridpointers[(int)RID_VIRTUAL])
		    cp_error ("`%D' access", (yyvsp[(1) - (2)].ttype));
		  (yyval.ttype) = access_default_virtual_node; ;}
    break;

  case 523:
#line 2366 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if ((yyvsp[(1) - (3)].ttype) != access_default_virtual_node)
		    error ("multiple access specifiers");
		  else if ((yyvsp[(2) - (3)].ttype) == access_public_node)
		    (yyval.ttype) = access_public_virtual_node;
		  else if ((yyvsp[(2) - (3)].ttype) == access_protected_node)
		    (yyval.ttype) = access_protected_virtual_node;
		  else /* $2 == access_private_node */
		    (yyval.ttype) = access_private_virtual_node;
		;}
    break;

  case 524:
#line 2377 "../../../gbagnu/gcc/cp/parse.y"
    { if ((yyvsp[(2) - (3)].ttype) != ridpointers[(int)RID_VIRTUAL])
		    cp_error ("`%D' access", (yyvsp[(2) - (3)].ttype));
		  else if ((yyval.ttype) == access_public_node)
		    (yyval.ttype) = access_public_virtual_node;
		  else if ((yyval.ttype) == access_protected_node)
		    (yyval.ttype) = access_protected_virtual_node;
		  else if ((yyval.ttype) == access_private_node)
		    (yyval.ttype) = access_private_virtual_node;
		  else
		    error ("multiple `virtual' specifiers");
		;}
    break;

  case 525:
#line 2392 "../../../gbagnu/gcc/cp/parse.y"
    { (yyvsp[(0) - (1)].ttype) = begin_class_definition ((yyvsp[(0) - (1)].ttype)); ;}
    break;

  case 526:
#line 2397 "../../../gbagnu/gcc/cp/parse.y"
    {
		  finish_member_declaration (build_self_reference ());
		;}
    break;

  case 531:
#line 2411 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (current_aggr == signature_type_node)
		    {
		      error ("access specifier not allowed in signature");
		      (yyvsp[(1) - (2)].ttype) = access_public_node;
		    }

		  current_access_specifier = (yyvsp[(1) - (2)].ttype);
                ;}
    break;

  case 532:
#line 2426 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  finish_member_declaration ((yyvsp[(1) - (1)].ttype));
		;}
    break;

  case 533:
#line 2430 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  finish_member_declaration ((yyvsp[(2) - (2)].ttype));
		;}
    break;

  case 535:
#line 2438 "../../../gbagnu/gcc/cp/parse.y"
    { error ("missing ';' before right brace");
		  yyungetc ('}', 0); ;}
    break;

  case 536:
#line 2443 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_method ((yyval.ttype)); ;}
    break;

  case 537:
#line 2445 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_method ((yyval.ttype)); ;}
    break;

  case 538:
#line 2447 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_method ((yyval.ttype)); ;}
    break;

  case 539:
#line 2449 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_method ((yyval.ttype)); ;}
    break;

  case 540:
#line 2451 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 541:
#line 2453 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  pedantic = (yyvsp[(1) - (2)].itype); ;}
    break;

  case 542:
#line 2456 "../../../gbagnu/gcc/cp/parse.y"
    {  
		  if ((yyvsp[(2) - (2)].ttype))
		    (yyval.ttype) = finish_member_template_decl ((yyvsp[(2) - (2)].ttype));
		  else
		    /* The component was already processed.  */
		    (yyval.ttype) = NULL_TREE;

		  finish_template_decl ((yyvsp[(1) - (2)].ttype));
		;}
    break;

  case 543:
#line 2466 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  (yyval.ttype) = finish_member_class_template ((yyvsp[(2) - (3)].ftype).t); 
		  finish_template_decl ((yyvsp[(1) - (3)].ttype));
		;}
    break;

  case 544:
#line 2477 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* Most of the productions for component_decl only
		     allow the creation of one new member, so we call
		     finish_member_declaration in component_decl_list.
		     For this rule and the next, however, there can be
		     more than one member, e.g.:

		       int i, j;

		     and we need the first member to be fully
		     registered before the second is processed.
		     Therefore, the rules for components take care of
		     this processing.  To avoid registering the
		     components more than once, we send NULL_TREE up
		     here; that lets finish_member_declaration now
		     that there is nothing to do.  */
		  if (!(yyvsp[(2) - (2)].itype))
		    grok_x_components ((yyvsp[(1) - (2)].ftype).t);
		  (yyval.ttype) = NULL_TREE;
		;}
    break;

  case 545:
#line 2498 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  if (!(yyvsp[(2) - (2)].itype))
		    grok_x_components ((yyvsp[(1) - (2)].ttype));
		  (yyval.ttype) = NULL_TREE; 
		;}
    break;

  case 546:
#line 2504 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokfield ((yyval.ttype), NULL_TREE, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), NULL_TREE)); ;}
    break;

  case 547:
#line 2507 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokfield ((yyval.ttype), NULL_TREE, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), NULL_TREE)); ;}
    break;

  case 548:
#line 2510 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokbitfield (NULL_TREE, NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 549:
#line 2512 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 550:
#line 2523 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs, attrs;
		  split_specs_attrs ((yyvsp[(1) - (5)].ttype), &specs, &attrs);
		  (yyval.ttype) = grokfield ((yyvsp[(2) - (5)].ttype), specs, (yyvsp[(5) - (5)].ttype), (yyvsp[(3) - (5)].ttype),
				  build_tree_list ((yyvsp[(4) - (5)].ttype), attrs)); ;}
    break;

  case 551:
#line 2528 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokfield ((yyval.ttype), NULL_TREE, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), NULL_TREE)); ;}
    break;

  case 552:
#line 2531 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = do_class_using_decl ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 553:
#line 2537 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 554:
#line 2539 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  if (PROCESSING_REAL_TEMPLATE_DECL_P ())
		    (yyvsp[(1) - (1)].ttype) = finish_member_template_decl ((yyvsp[(1) - (1)].ttype));
		  finish_member_declaration ((yyvsp[(1) - (1)].ttype)); 
		  (yyval.itype) = 1;
		;}
    break;

  case 555:
#line 2546 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  check_multiple_declarators ();
		  if (PROCESSING_REAL_TEMPLATE_DECL_P ())
		    (yyvsp[(3) - (3)].ttype) = finish_member_template_decl ((yyvsp[(3) - (3)].ttype));
		  finish_member_declaration ((yyvsp[(3) - (3)].ttype));
		  (yyval.itype) = 2;
		;}
    break;

  case 556:
#line 2557 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = 0; ;}
    break;

  case 557:
#line 2559 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  if (PROCESSING_REAL_TEMPLATE_DECL_P ())
		    (yyvsp[(1) - (1)].ttype) = finish_member_template_decl ((yyvsp[(1) - (1)].ttype));
		  finish_member_declaration ((yyvsp[(1) - (1)].ttype));
		  (yyval.itype) = 1;
		;}
    break;

  case 558:
#line 2566 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  check_multiple_declarators ();
		  if (PROCESSING_REAL_TEMPLATE_DECL_P ())
		    (yyvsp[(3) - (3)].ttype) = finish_member_template_decl ((yyvsp[(3) - (3)].ttype));
		  finish_member_declaration ((yyvsp[(3) - (3)].ttype)); 
		  (yyval.itype) = 2;
		;}
    break;

  case 563:
#line 2587 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (4)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (4)].ttype) = current_declspecs;
		  (yyval.ttype) = grokfield ((yyval.ttype), current_declspecs, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), prefix_attributes)); ;}
    break;

  case 564:
#line 2593 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (4)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (4)].ttype) = current_declspecs;
		  (yyval.ttype) = grokbitfield ((yyval.ttype), current_declspecs, (yyvsp[(3) - (4)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(4) - (4)].ttype), prefix_attributes); ;}
    break;

  case 565:
#line 2602 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (4)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (4)].ttype) = current_declspecs;
		  (yyval.ttype) = grokfield ((yyval.ttype), current_declspecs, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), prefix_attributes)); ;}
    break;

  case 566:
#line 2608 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (4)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (4)].ttype) = current_declspecs;
		  (yyval.ttype) = grokfield ((yyval.ttype), current_declspecs, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), prefix_attributes)); ;}
    break;

  case 567:
#line 2614 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (4)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (4)].ttype) = current_declspecs;
		  (yyval.ttype) = grokbitfield ((yyval.ttype), current_declspecs, (yyvsp[(3) - (4)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(4) - (4)].ttype), prefix_attributes); ;}
    break;

  case 568:
#line 2620 "../../../gbagnu/gcc/cp/parse.y"
    { split_specs_attrs ((yyvsp[(0) - (3)].ttype), &current_declspecs,
				     &prefix_attributes);
		  (yyvsp[(0) - (3)].ttype) = current_declspecs;
		  (yyval.ttype) = grokbitfield (NULL_TREE, current_declspecs, (yyvsp[(2) - (3)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(3) - (3)].ttype), prefix_attributes); ;}
    break;

  case 569:
#line 2629 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokfield ((yyval.ttype), current_declspecs, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), prefix_attributes)); ;}
    break;

  case 570:
#line 2632 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokbitfield ((yyval.ttype), current_declspecs, (yyvsp[(3) - (4)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(4) - (4)].ttype), prefix_attributes); ;}
    break;

  case 571:
#line 2638 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokfield ((yyval.ttype), current_declspecs, (yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype),
				  build_tree_list ((yyvsp[(3) - (4)].ttype), prefix_attributes)); ;}
    break;

  case 572:
#line 2641 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokbitfield ((yyval.ttype), current_declspecs, (yyvsp[(3) - (4)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(4) - (4)].ttype), prefix_attributes); ;}
    break;

  case 573:
#line 2644 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokbitfield (NULL_TREE, current_declspecs, (yyvsp[(2) - (3)].ttype));
		  cplus_decl_attributes ((yyval.ttype), (yyvsp[(3) - (3)].ttype), prefix_attributes); ;}
    break;

  case 575:
#line 2655 "../../../gbagnu/gcc/cp/parse.y"
    { TREE_CHAIN ((yyvsp[(3) - (3)].ttype)) = (yyval.ttype); (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 576:
#line 2660 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_enumerator ((yyval.ttype), NULL_TREE, current_enum_type); ;}
    break;

  case 577:
#line 2662 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_enumerator ((yyval.ttype), (yyvsp[(3) - (3)].ttype), current_enum_type); ;}
    break;

  case 578:
#line 2668 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 579:
#line 2671 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_decl_list ((yyvsp[(1) - (1)].ftype).t, NULL_TREE); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag; ;}
    break;

  case 580:
#line 2678 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (pedantic)
		    pedwarn ("ANSI C++ forbids array dimensions with parenthesized type in new");
		  (yyval.ftype).t = build_parse_node (ARRAY_REF, TREE_VALUE ((yyvsp[(3) - (7)].ftype).t), (yyvsp[(6) - (7)].ttype));
		  (yyval.ftype).t = build_decl_list (TREE_PURPOSE ((yyvsp[(3) - (7)].ftype).t), (yyval.ftype).t);
		  (yyval.ftype).new_type_flag = (yyvsp[(3) - (7)].ftype).new_type_flag;
		;}
    break;

  case 581:
#line 2689 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 582:
#line 2691 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyval.ttype)); ;}
    break;

  case 583:
#line 2696 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = IDENTIFIER_AS_LIST ((yyvsp[(1) - (1)].ttype)); 
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 584:
#line 2699 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = decl_tree_cons (NULL_TREE, (yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ftype).t); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 585:
#line 2708 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.itype) = suspend_momentary (); ;}
    break;

  case 586:
#line 2713 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((int) (yyvsp[(1) - (2)].itype)); (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 587:
#line 2719 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((int) (yyvsp[(1) - (4)].itype)); (yyval.ttype) = (yyvsp[(3) - (4)].ttype); ;}
    break;

  case 588:
#line 2721 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((int) (yyvsp[(1) - (4)].itype)); (yyval.ttype) = (yyvsp[(3) - (4)].ttype); ;}
    break;

  case 589:
#line 2723 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((int) (yyvsp[(1) - (2)].itype)); (yyval.ttype) = empty_parms (); ;}
    break;

  case 590:
#line 2725 "../../../gbagnu/gcc/cp/parse.y"
    { resume_momentary ((int) (yyvsp[(1) - (4)].itype)); (yyval.ttype) = NULL_TREE; ;}
    break;

  case 591:
#line 2732 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 592:
#line 2734 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 593:
#line 2736 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 594:
#line 2738 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 595:
#line 2740 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 597:
#line 2748 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (1)].ttype)) == IDENTIFIER_NODE)
		    {
		      (yyval.ttype) = lookup_name ((yyvsp[(1) - (1)].ttype), 1);
		      if (current_class_type
			  && TYPE_BEING_DEFINED (current_class_type)
			  && ! IDENTIFIER_CLASS_VALUE ((yyvsp[(1) - (1)].ttype)))
			{
			  /* Remember that this name has been used in the class
			     definition, as per [class.scope0] */
			  pushdecl_class_level ((yyval.ttype));
			}
		    }
		  else
		    (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
		;}
    break;

  case 598:
#line 2765 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = IDENTIFIER_GLOBAL_VALUE ((yyvsp[(2) - (2)].ttype));
		  else
		    (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  got_scope = NULL_TREE;
		;}
    break;

  case 601:
#line 2778 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 602:
#line 2783 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = get_type_decl ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 603:
#line 2788 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyval.ttype), (yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 604:
#line 2790 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 605:
#line 2792 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), NULL_TREE); ;}
    break;

  case 606:
#line 2794 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 607:
#line 2796 "../../../gbagnu/gcc/cp/parse.y"
    { push_nested_class ((yyvsp[(1) - (2)].ttype), 3);
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyval.ttype), (yyvsp[(2) - (2)].ttype));
		  TREE_COMPLEXITY ((yyval.ttype)) = current_class_depth; ;}
    break;

  case 610:
#line 2808 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* Provide support for '(' attributes '*' declarator ')'
		     etc */
		  (yyval.ttype) = decl_tree_cons ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype), NULL_TREE);
		;}
    break;

  case 611:
#line 2817 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 612:
#line 2819 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 613:
#line 2821 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 614:
#line 2823 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 615:
#line 2825 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 617:
#line 2833 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 618:
#line 2835 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 619:
#line 2837 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 620:
#line 2839 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 621:
#line 2841 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 623:
#line 2849 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyval.ttype), (yyvsp[(2) - (4)].ttype), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 624:
#line 2851 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 625:
#line 2853 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 626:
#line 2855 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), NULL_TREE); ;}
    break;

  case 627:
#line 2857 "../../../gbagnu/gcc/cp/parse.y"
    { enter_scope_of ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 628:
#line 2859 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE;
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype));
		  enter_scope_of ((yyval.ttype));
		;}
    break;

  case 629:
#line 2867 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE;
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyval.ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 630:
#line 2870 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE;
 		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 631:
#line 2876 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE;
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyval.ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 632:
#line 2879 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE;
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 634:
#line 2886 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 635:
#line 2891 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_functional_cast ((yyvsp[(1) - (4)].ftype).t, (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 636:
#line 2893 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = reparse_decl_as_expr ((yyvsp[(1) - (4)].ftype).t, (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 637:
#line 2895 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = reparse_absdcl_as_expr ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 642:
#line 2906 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 643:
#line 2908 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = (yyval.ttype) = make_typename_type ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 644:
#line 2915 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (2)].ttype)) == IDENTIFIER_NODE)
		    {
		      (yyval.ttype) = lastiddecl;
		      /* Remember that this name has been used in the class
			 definition, as per [class.scope0] */
		      if (current_class_type
			  && TYPE_BEING_DEFINED (current_class_type)
			  && ! IDENTIFIER_CLASS_VALUE ((yyvsp[(1) - (2)].ttype)))
			pushdecl_class_level ((yyval.ttype));
		    }
		  got_scope = (yyval.ttype) = TYPE_MAIN_VARIANT (TREE_TYPE ((yyval.ttype)));
		;}
    break;

  case 645:
#line 2929 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (2)].ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype) = TREE_TYPE ((yyval.ttype));
		;}
    break;

  case 646:
#line 2935 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyval.ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype);
		;}
    break;

  case 647:
#line 2941 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = (yyval.ttype) = complete_type (TREE_TYPE ((yyvsp[(1) - (2)].ttype))); ;}
    break;

  case 649:
#line 2957 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 650:
#line 2962 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE_CLASS (TREE_CODE ((yyvsp[(1) - (2)].ttype))) == 't')
		    (yyval.ttype) = make_typename_type ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype));
		  else if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == IDENTIFIER_NODE)
		    cp_error ("`%T' is not a class or namespace", (yyvsp[(2) - (2)].ttype));
		  else
		    {
		      (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		      if (TREE_CODE ((yyval.ttype)) == TYPE_DECL)
			(yyval.ttype) = TREE_TYPE ((yyval.ttype));
		    }
		;}
    break;

  case 651:
#line 2975 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = TREE_TYPE ((yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 652:
#line 2977 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_typename_type ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 653:
#line 2979 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_typename_type ((yyvsp[(1) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 654:
#line 2984 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (1)].ttype)) == IDENTIFIER_NODE)
		    cp_error ("`%T' is not a class or namespace", (yyvsp[(1) - (1)].ttype));
		;}
    break;

  case 655:
#line 2989 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE_CLASS (TREE_CODE ((yyvsp[(1) - (2)].ttype))) == 't')
		    (yyval.ttype) = make_typename_type ((yyvsp[(1) - (2)].ttype), (yyvsp[(2) - (2)].ttype));
		  else if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == IDENTIFIER_NODE)
		    cp_error ("`%T' is not a class or namespace", (yyvsp[(2) - (2)].ttype));
		  else
		    {
		      (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		      if (TREE_CODE ((yyval.ttype)) == TYPE_DECL)
			(yyval.ttype) = TREE_TYPE ((yyval.ttype));
		    }
		;}
    break;

  case 656:
#line 3002 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = (yyval.ttype) = make_typename_type ((yyvsp[(1) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 657:
#line 3004 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = (yyval.ttype) = make_typename_type ((yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 658:
#line 3009 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (2)].ttype)) != IDENTIFIER_NODE)
		    (yyvsp[(1) - (2)].ttype) = lastiddecl;

		  /* Retrieve the type for the identifier, which might involve
		     some computation. */
		  got_scope = (yyval.ttype) = complete_type (IDENTIFIER_TYPE_VALUE ((yyvsp[(1) - (2)].ttype)));

		  if ((yyval.ttype) == error_mark_node)
		    cp_error ("`%T' is not a class or namespace", (yyvsp[(1) - (2)].ttype));
		;}
    break;

  case 659:
#line 3021 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(1) - (2)].ttype)) != IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype) = complete_type (TREE_TYPE ((yyval.ttype)));
		;}
    break;

  case 660:
#line 3027 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = (yyval.ttype) = complete_type (TREE_TYPE ((yyval.ttype))); ;}
    break;

  case 663:
#line 3031 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyval.ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = lastiddecl;
		  got_scope = (yyval.ttype);
		;}
    break;

  case 664:
#line 3040 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_min_nt (TEMPLATE_ID_EXPR, (yyvsp[(1) - (4)].ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 665:
#line 3045 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (TREE_CODE ((yyvsp[(2) - (2)].ttype)) == IDENTIFIER_NODE)
		    (yyval.ttype) = IDENTIFIER_GLOBAL_VALUE ((yyvsp[(2) - (2)].ttype));
		  else
		    (yyval.ttype) = (yyvsp[(2) - (2)].ttype);
		  got_scope = NULL_TREE;
		;}
    break;

  case 667:
#line 3054 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (2)].ttype); ;}
    break;

  case 668:
#line 3059 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE; ;}
    break;

  case 669:
#line 3061 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); got_scope = NULL_TREE; ;}
    break;

  case 670:
#line 3068 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = void_type_node; ;}
    break;

  case 671:
#line 3074 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 672:
#line 3076 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (2)].ttype), NULL_TREE); ;}
    break;

  case 673:
#line 3078 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 674:
#line 3080 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (2)].ttype), NULL_TREE); ;}
    break;

  case 675:
#line 3082 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (2)].ttype), NULL_TREE);
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (2)].ttype), arg);
		;}
    break;

  case 676:
#line 3086 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 678:
#line 3095 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, NULL_TREE, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 679:
#line 3097 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 680:
#line 3103 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 681:
#line 3105 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 682:
#line 3107 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (2)].ftype).t, NULL_TREE); ;}
    break;

  case 683:
#line 3109 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator (NULL_TREE, NULL_TREE); ;}
    break;

  case 684:
#line 3111 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 685:
#line 3113 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator (NULL_TREE, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 686:
#line 3115 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (2)].ftype).t, NULL_TREE); ;}
    break;

  case 687:
#line 3117 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator (NULL_TREE, NULL_TREE); ;}
    break;

  case 688:
#line 3119 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (2)].ttype), NULL_TREE);
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (2)].ttype), arg);
		;}
    break;

  case 689:
#line 3123 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 691:
#line 3132 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(2) - (3)].ttype); ;}
    break;

  case 693:
#line 3136 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyval.ttype), (yyvsp[(3) - (6)].ttype), (yyvsp[(5) - (6)].ttype), (yyvsp[(6) - (6)].ttype)); ;}
    break;

  case 694:
#line 3138 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator ((yyval.ttype), empty_parms (), (yyvsp[(3) - (4)].ttype), (yyvsp[(4) - (4)].ttype)); ;}
    break;

  case 695:
#line 3140 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 696:
#line 3142 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, (yyval.ttype), NULL_TREE); ;}
    break;

  case 697:
#line 3144 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_call_declarator (NULL_TREE, (yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype), (yyvsp[(5) - (5)].ttype)); ;}
    break;

  case 698:
#line 3146 "../../../gbagnu/gcc/cp/parse.y"
    { set_quals_and_spec ((yyval.ttype), (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 699:
#line 3148 "../../../gbagnu/gcc/cp/parse.y"
    { set_quals_and_spec ((yyval.ttype), (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 700:
#line 3150 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, NULL_TREE, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 701:
#line 3152 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_parse_node (ARRAY_REF, NULL_TREE, NULL_TREE); ;}
    break;

  case 708:
#line 3175 "../../../gbagnu/gcc/cp/parse.y"
    { if (pedantic)
		    pedwarn ("ANSI C++ forbids label declarations"); ;}
    break;

  case 711:
#line 3186 "../../../gbagnu/gcc/cp/parse.y"
    { tree link;
		  for (link = (yyvsp[(2) - (3)].ttype); link; link = TREE_CHAIN (link))
		    {
		      tree label = shadow_label (TREE_VALUE (link));
		      C_DECLARED_LABEL_FLAG (label) = 1;
		      declare_nonlocal_label (label);
		    }
		;}
    break;

  case 712:
#line 3200 "../../../gbagnu/gcc/cp/parse.y"
    {;}
    break;

  case 714:
#line 3206 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_compound_stmt (0); ;}
    break;

  case 715:
#line 3208 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_compound_stmt (0, (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 716:
#line 3213 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = begin_if_stmt ();
		  cond_stmt_keyword = "if";
		;}
    break;

  case 717:
#line 3218 "../../../gbagnu/gcc/cp/parse.y"
    { finish_if_stmt_cond ((yyvsp[(3) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 718:
#line 3220 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_then_clause ((yyvsp[(2) - (5)].ttype)); ;}
    break;

  case 720:
#line 3225 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_compound_stmt (0); ;}
    break;

  case 721:
#line 3227 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_compound_stmt (0, (yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 722:
#line 3232 "../../../gbagnu/gcc/cp/parse.y"
    {;}
    break;

  case 724:
#line 3238 "../../../gbagnu/gcc/cp/parse.y"
    { finish_stmt (); ;}
    break;

  case 725:
#line 3240 "../../../gbagnu/gcc/cp/parse.y"
    { finish_expr_stmt ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 726:
#line 3242 "../../../gbagnu/gcc/cp/parse.y"
    { begin_else_clause (); ;}
    break;

  case 727:
#line 3244 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  finish_else_clause ((yyvsp[(1) - (4)].ttype)); 
		  finish_if_stmt ();
		;}
    break;

  case 728:
#line 3249 "../../../gbagnu/gcc/cp/parse.y"
    { finish_if_stmt (); ;}
    break;

  case 729:
#line 3251 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = begin_while_stmt ();
		  cond_stmt_keyword = "while";
		;}
    break;

  case 730:
#line 3256 "../../../gbagnu/gcc/cp/parse.y"
    { finish_while_stmt_cond ((yyvsp[(3) - (3)].ttype), (yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 731:
#line 3258 "../../../gbagnu/gcc/cp/parse.y"
    { finish_while_stmt ((yyvsp[(2) - (5)].ttype)); ;}
    break;

  case 732:
#line 3260 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_do_stmt (); ;}
    break;

  case 733:
#line 3262 "../../../gbagnu/gcc/cp/parse.y"
    {
		  finish_do_body ((yyvsp[(2) - (4)].ttype));
		  cond_stmt_keyword = "do";
		;}
    break;

  case 734:
#line 3267 "../../../gbagnu/gcc/cp/parse.y"
    { finish_do_stmt ((yyvsp[(6) - (7)].ttype), (yyvsp[(2) - (7)].ttype)); ;}
    break;

  case 735:
#line 3269 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_for_stmt (); ;}
    break;

  case 736:
#line 3271 "../../../gbagnu/gcc/cp/parse.y"
    { finish_for_init_stmt ((yyvsp[(2) - (4)].ttype)); ;}
    break;

  case 737:
#line 3273 "../../../gbagnu/gcc/cp/parse.y"
    { finish_for_cond ((yyvsp[(6) - (7)].ttype), (yyvsp[(2) - (7)].ttype)); ;}
    break;

  case 738:
#line 3275 "../../../gbagnu/gcc/cp/parse.y"
    { finish_for_expr ((yyvsp[(9) - (10)].ttype), (yyvsp[(2) - (10)].ttype)); ;}
    break;

  case 739:
#line 3277 "../../../gbagnu/gcc/cp/parse.y"
    { finish_for_stmt ((yyvsp[(9) - (12)].ttype), (yyvsp[(2) - (12)].ttype)); ;}
    break;

  case 740:
#line 3279 "../../../gbagnu/gcc/cp/parse.y"
    { begin_switch_stmt (); ;}
    break;

  case 741:
#line 3281 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_switch_cond ((yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 742:
#line 3283 "../../../gbagnu/gcc/cp/parse.y"
    { finish_switch_stmt ((yyvsp[(4) - (7)].ttype), (yyvsp[(6) - (7)].ttype)); ;}
    break;

  case 743:
#line 3285 "../../../gbagnu/gcc/cp/parse.y"
    { finish_case_label ((yyvsp[(2) - (3)].ttype), NULL_TREE); ;}
    break;

  case 745:
#line 3288 "../../../gbagnu/gcc/cp/parse.y"
    { finish_case_label ((yyvsp[(2) - (5)].ttype), (yyvsp[(4) - (5)].ttype)); ;}
    break;

  case 747:
#line 3291 "../../../gbagnu/gcc/cp/parse.y"
    { finish_case_label (NULL_TREE, NULL_TREE); ;}
    break;

  case 749:
#line 3294 "../../../gbagnu/gcc/cp/parse.y"
    { finish_break_stmt (); ;}
    break;

  case 750:
#line 3296 "../../../gbagnu/gcc/cp/parse.y"
    { finish_continue_stmt (); ;}
    break;

  case 751:
#line 3298 "../../../gbagnu/gcc/cp/parse.y"
    { finish_return_stmt (NULL_TREE); ;}
    break;

  case 752:
#line 3300 "../../../gbagnu/gcc/cp/parse.y"
    { finish_return_stmt ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 753:
#line 3302 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  finish_asm_stmt ((yyvsp[(2) - (6)].ttype), (yyvsp[(4) - (6)].ttype), NULL_TREE, NULL_TREE,
				   NULL_TREE); 
		;}
    break;

  case 754:
#line 3308 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  finish_asm_stmt ((yyvsp[(2) - (8)].ttype), (yyvsp[(4) - (8)].ttype), (yyvsp[(6) - (8)].ttype), NULL_TREE,
				   NULL_TREE); 
		;}
    break;

  case 755:
#line 3314 "../../../gbagnu/gcc/cp/parse.y"
    { finish_asm_stmt ((yyvsp[(2) - (10)].ttype), (yyvsp[(4) - (10)].ttype), (yyvsp[(6) - (10)].ttype), (yyvsp[(8) - (10)].ttype), NULL_TREE); ;}
    break;

  case 756:
#line 3318 "../../../gbagnu/gcc/cp/parse.y"
    { finish_asm_stmt ((yyvsp[(2) - (12)].ttype), (yyvsp[(4) - (12)].ttype), (yyvsp[(6) - (12)].ttype), (yyvsp[(8) - (12)].ttype), (yyvsp[(10) - (12)].ttype)); ;}
    break;

  case 757:
#line 3320 "../../../gbagnu/gcc/cp/parse.y"
    { 
		  if (pedantic)
		    pedwarn ("ANSI C++ forbids computed gotos");
		  finish_goto_stmt ((yyvsp[(3) - (4)].ttype));
		;}
    break;

  case 758:
#line 3326 "../../../gbagnu/gcc/cp/parse.y"
    { finish_goto_stmt ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 759:
#line 3328 "../../../gbagnu/gcc/cp/parse.y"
    { finish_stmt (); ;}
    break;

  case 760:
#line 3330 "../../../gbagnu/gcc/cp/parse.y"
    { error ("label must be followed by statement");
		  yyungetc ('}', 0);
		  finish_stmt (); ;}
    break;

  case 761:
#line 3334 "../../../gbagnu/gcc/cp/parse.y"
    { finish_stmt (); ;}
    break;

  case 764:
#line 3338 "../../../gbagnu/gcc/cp/parse.y"
    { do_local_using_decl ((yyvsp[(1) - (1)].ttype)); ;}
    break;

  case 766:
#line 3344 "../../../gbagnu/gcc/cp/parse.y"
    {
		  if (! current_function_parms_stored)
		    store_parm_decls ();
		  expand_start_early_try_stmts ();
		;}
    break;

  case 767:
#line 3350 "../../../gbagnu/gcc/cp/parse.y"
    { 
                  expand_start_all_catch (); 
                ;}
    break;

  case 768:
#line 3354 "../../../gbagnu/gcc/cp/parse.y"
    {
		  expand_end_all_catch ();
		  (yyval.itype) = (yyvsp[(3) - (6)].itype);
		;}
    break;

  case 769:
#line 3362 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_try_block (); ;}
    break;

  case 770:
#line 3364 "../../../gbagnu/gcc/cp/parse.y"
    { finish_try_block ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 771:
#line 3366 "../../../gbagnu/gcc/cp/parse.y"
    { finish_handler_sequence ((yyvsp[(2) - (5)].ttype)); ;}
    break;

  case 774:
#line 3376 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = begin_handler(); ;}
    break;

  case 775:
#line 3378 "../../../gbagnu/gcc/cp/parse.y"
    { finish_handler_parms ((yyvsp[(2) - (3)].ttype)); ;}
    break;

  case 776:
#line 3380 "../../../gbagnu/gcc/cp/parse.y"
    { finish_handler ((yyvsp[(2) - (5)].ttype)); ;}
    break;

  case 779:
#line 3390 "../../../gbagnu/gcc/cp/parse.y"
    { expand_start_catch_block (NULL_TREE, NULL_TREE); ;}
    break;

  case 780:
#line 3406 "../../../gbagnu/gcc/cp/parse.y"
    { check_for_new_type ("inside exception declarations", (yyvsp[(2) - (3)].ftype));
		  expand_start_catch_block (TREE_PURPOSE ((yyvsp[(2) - (3)].ftype).t),
					    TREE_VALUE ((yyvsp[(2) - (3)].ftype).t)); ;}
    break;

  case 781:
#line 3413 "../../../gbagnu/gcc/cp/parse.y"
    { tree label;
		do_label:
		  label = define_label (input_filename, lineno, (yyvsp[(1) - (2)].ttype));
		  if (label && ! minimal_parse_mode)
		    expand_label (label);
		;}
    break;

  case 782:
#line 3420 "../../../gbagnu/gcc/cp/parse.y"
    { goto do_label; ;}
    break;

  case 783:
#line 3422 "../../../gbagnu/gcc/cp/parse.y"
    { goto do_label; ;}
    break;

  case 784:
#line 3424 "../../../gbagnu/gcc/cp/parse.y"
    { goto do_label; ;}
    break;

  case 785:
#line 3429 "../../../gbagnu/gcc/cp/parse.y"
    { if ((yyvsp[(1) - (2)].ttype)) cplus_expand_expr_stmt ((yyvsp[(1) - (2)].ttype)); ;}
    break;

  case 787:
#line 3432 "../../../gbagnu/gcc/cp/parse.y"
    { if (pedantic)
		    pedwarn ("ANSI C++ forbids compound statements inside for initializations");
		;}
    break;

  case 788:
#line 3441 "../../../gbagnu/gcc/cp/parse.y"
    { emit_line_note (input_filename, lineno);
		  (yyval.ttype) = NULL_TREE; ;}
    break;

  case 789:
#line 3444 "../../../gbagnu/gcc/cp/parse.y"
    { emit_line_note (input_filename, lineno); ;}
    break;

  case 790:
#line 3449 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 792:
#line 3452 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 793:
#line 3459 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 796:
#line 3466 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyval.ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 797:
#line 3471 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_tree_list ((yyval.ttype), (yyvsp[(3) - (4)].ttype)); ;}
    break;

  case 798:
#line 3476 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = tree_cons (NULL_TREE, (yyval.ttype), NULL_TREE); ;}
    break;

  case 799:
#line 3478 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[(3) - (3)].ttype), (yyval.ttype)); ;}
    break;

  case 800:
#line 3489 "../../../gbagnu/gcc/cp/parse.y"
    {
		  (yyval.ttype) = empty_parms();
		;}
    break;

  case 802:
#line 3494 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist (build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ftype).t), 0);
		  check_for_new_type ("inside parameter list", (yyvsp[(1) - (1)].ftype)); ;}
    break;

  case 803:
#line 3502 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist ((yyval.ttype), 0); ;}
    break;

  case 804:
#line 3504 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist ((yyvsp[(1) - (2)].ttype), 1); ;}
    break;

  case 805:
#line 3507 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist ((yyvsp[(1) - (2)].ttype), 1); ;}
    break;

  case 806:
#line 3509 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist (build_tree_list (NULL_TREE,
							 (yyvsp[(1) - (2)].ftype).t), 1); ;}
    break;

  case 807:
#line 3512 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = finish_parmlist (NULL_TREE, 1); ;}
    break;

  case 808:
#line 3514 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* This helps us recover from really nasty
		     parse errors, for example, a missing right
		     parenthesis.  */
		  yyerror ("possibly missing ')'");
		  (yyval.ttype) = finish_parmlist ((yyvsp[(1) - (2)].ttype), 0);
		  yyungetc (':', 0);
		  yychar = ')';
		;}
    break;

  case 809:
#line 3524 "../../../gbagnu/gcc/cp/parse.y"
    {
		  /* This helps us recover from really nasty
		     parse errors, for example, a missing right
		     parenthesis.  */
		  yyerror ("possibly missing ')'");
		  (yyval.ttype) = finish_parmlist (build_tree_list (NULL_TREE,
							 (yyvsp[(1) - (2)].ftype).t), 0); 
		  yyungetc (':', 0);
		  yychar = ')';
		;}
    break;

  case 810:
#line 3539 "../../../gbagnu/gcc/cp/parse.y"
    { maybe_snarf_defarg (); ;}
    break;

  case 811:
#line 3541 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(3) - (3)].ttype); ;}
    break;

  case 814:
#line 3552 "../../../gbagnu/gcc/cp/parse.y"
    { check_for_new_type ("in a parameter list", (yyvsp[(1) - (1)].ftype));
		  (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ftype).t); ;}
    break;

  case 815:
#line 3555 "../../../gbagnu/gcc/cp/parse.y"
    { check_for_new_type ("in a parameter list", (yyvsp[(1) - (2)].ftype));
		  (yyval.ttype) = build_tree_list ((yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ftype).t); ;}
    break;

  case 816:
#line 3558 "../../../gbagnu/gcc/cp/parse.y"
    { check_for_new_type ("in a parameter list", (yyvsp[(2) - (2)].ftype));
		  (yyval.ttype) = chainon ((yyval.ttype), (yyvsp[(2) - (2)].ftype).t); ;}
    break;

  case 817:
#line 3561 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyval.ttype), build_tree_list (NULL_TREE, (yyvsp[(2) - (2)].ttype))); ;}
    break;

  case 818:
#line 3563 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = chainon ((yyval.ttype), build_tree_list ((yyvsp[(4) - (4)].ttype), (yyvsp[(2) - (4)].ttype))); ;}
    break;

  case 820:
#line 3569 "../../../gbagnu/gcc/cp/parse.y"
    { check_for_new_type ("in a parameter list", (yyvsp[(1) - (2)].ftype));
		  (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[(1) - (2)].ftype).t); ;}
    break;

  case 821:
#line 3579 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(1) - (2)].ftype).t);
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag;
		  (yyval.ftype).t = build_tree_list (specs, (yyvsp[(2) - (2)].ttype)); ;}
    break;

  case 822:
#line 3583 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_tree_list ((yyvsp[(1) - (2)].ftype).t, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 823:
#line 3586 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_tree_list (get_decl_list ((yyvsp[(1) - (2)].ftype).t), (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 824:
#line 3589 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(1) - (2)].ftype).t);
		  (yyval.ftype).t = build_tree_list (specs, (yyvsp[(2) - (2)].ttype));
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag; ;}
    break;

  case 825:
#line 3593 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(1) - (1)].ftype).t);
		  (yyval.ftype).t = build_tree_list (specs, NULL_TREE); 
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag; ;}
    break;

  case 826:
#line 3597 "../../../gbagnu/gcc/cp/parse.y"
    { tree specs = strip_attrs ((yyvsp[(1) - (2)].ttype));
		  (yyval.ftype).t = build_tree_list (specs, (yyvsp[(2) - (2)].ttype)); 
		  (yyval.ftype).new_type_flag = 0; ;}
    break;

  case 827:
#line 3604 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_tree_list (NULL_TREE, (yyvsp[(1) - (1)].ftype).t);
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (1)].ftype).new_type_flag;  ;}
    break;

  case 828:
#line 3607 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ftype).t = build_tree_list ((yyvsp[(2) - (2)].ttype), (yyvsp[(1) - (2)].ftype).t);
		  (yyval.ftype).new_type_flag = (yyvsp[(1) - (2)].ftype).new_type_flag;  ;}
    break;

  case 831:
#line 3618 "../../../gbagnu/gcc/cp/parse.y"
    { see_typename (); ;}
    break;

  case 832:
#line 3623 "../../../gbagnu/gcc/cp/parse.y"
    {
		  error ("type specifier omitted for parameter");
		  (yyval.ttype) = build_tree_list (integer_type_node, NULL_TREE);
		;}
    break;

  case 833:
#line 3628 "../../../gbagnu/gcc/cp/parse.y"
    {
		  error ("type specifier omitted for parameter");
		  if (TREE_CODE ((yyval.ttype)) == SCOPE_REF
		      && (TREE_CODE (TREE_OPERAND ((yyval.ttype), 0)) == TEMPLATE_TYPE_PARM
			  || TREE_CODE (TREE_OPERAND ((yyval.ttype), 0)) == TEMPLATE_TEMPLATE_PARM))
		    cp_error ("  perhaps you want `typename %E' to make it a type", (yyval.ttype));
		  (yyval.ttype) = build_tree_list (integer_type_node, (yyval.ttype));
		;}
    break;

  case 834:
#line 3640 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 835:
#line 3642 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = (yyvsp[(3) - (4)].ttype); ;}
    break;

  case 836:
#line 3644 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_decl_list (NULL_TREE, NULL_TREE); ;}
    break;

  case 837:
#line 3649 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = build_decl_list (NULL_TREE, groktypename((yyvsp[(1) - (1)].ftype).t)); ;}
    break;

  case 839:
#line 3655 "../../../gbagnu/gcc/cp/parse.y"
    {
		  TREE_CHAIN ((yyvsp[(3) - (3)].ttype)) = (yyval.ttype);
		  (yyval.ttype) = (yyvsp[(3) - (3)].ttype);
		;}
    break;

  case 840:
#line 3663 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = NULL_TREE; ;}
    break;

  case 841:
#line 3665 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 842:
#line 3667 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = make_reference_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 843:
#line 3669 "../../../gbagnu/gcc/cp/parse.y"
    { tree arg = make_pointer_declarator ((yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].ttype));
		  (yyval.ttype) = build_parse_node (SCOPE_REF, (yyvsp[(1) - (3)].ttype), arg);
		;}
    break;

  case 844:
#line 3676 "../../../gbagnu/gcc/cp/parse.y"
    { got_scope = NULL_TREE; ;}
    break;

  case 845:
#line 3681 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[MULT_EXPR]; ;}
    break;

  case 846:
#line 3683 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[TRUNC_DIV_EXPR]; ;}
    break;

  case 847:
#line 3685 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[TRUNC_MOD_EXPR]; ;}
    break;

  case 848:
#line 3687 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[PLUS_EXPR]; ;}
    break;

  case 849:
#line 3689 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[MINUS_EXPR]; ;}
    break;

  case 850:
#line 3691 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[BIT_AND_EXPR]; ;}
    break;

  case 851:
#line 3693 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[BIT_IOR_EXPR]; ;}
    break;

  case 852:
#line 3695 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[BIT_XOR_EXPR]; ;}
    break;

  case 853:
#line 3697 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[BIT_NOT_EXPR]; ;}
    break;

  case 854:
#line 3699 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[COMPOUND_EXPR]; ;}
    break;

  case 855:
#line 3701 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 856:
#line 3703 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[LT_EXPR]; ;}
    break;

  case 857:
#line 3705 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[GT_EXPR]; ;}
    break;

  case 858:
#line 3707 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 859:
#line 3709 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_assopname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 860:
#line 3711 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname [MODIFY_EXPR]; ;}
    break;

  case 861:
#line 3713 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 862:
#line 3715 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 863:
#line 3717 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[POSTINCREMENT_EXPR]; ;}
    break;

  case 864:
#line 3719 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[PREDECREMENT_EXPR]; ;}
    break;

  case 865:
#line 3721 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[TRUTH_ANDIF_EXPR]; ;}
    break;

  case 866:
#line 3723 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[TRUTH_ORIF_EXPR]; ;}
    break;

  case 867:
#line 3725 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[TRUTH_NOT_EXPR]; ;}
    break;

  case 868:
#line 3727 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[COND_EXPR]; ;}
    break;

  case 869:
#line 3729 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[(yyvsp[(2) - (2)].code)]; ;}
    break;

  case 870:
#line 3731 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[COMPONENT_REF]; ;}
    break;

  case 871:
#line 3733 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[MEMBER_REF]; ;}
    break;

  case 872:
#line 3735 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[CALL_EXPR]; ;}
    break;

  case 873:
#line 3737 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[ARRAY_REF]; ;}
    break;

  case 874:
#line 3739 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[NEW_EXPR]; ;}
    break;

  case 875:
#line 3741 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[DELETE_EXPR]; ;}
    break;

  case 876:
#line 3743 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[VEC_NEW_EXPR]; ;}
    break;

  case 877:
#line 3745 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[VEC_DELETE_EXPR]; ;}
    break;

  case 878:
#line 3748 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = grokoptypename ((yyvsp[(2) - (3)].ftype).t, (yyvsp[(3) - (3)].ttype)); ;}
    break;

  case 879:
#line 3750 "../../../gbagnu/gcc/cp/parse.y"
    { (yyval.ttype) = ansi_opname[ERROR_MARK]; ;}
    break;


/* Line 1267 of yacc.c.  */
#line 9589 "parse.c"
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


#line 3753 "../../../gbagnu/gcc/cp/parse.y"


#ifdef SPEW_DEBUG
const char *
debug_yytranslate (value)
    int value;
{
  return yytname[YYTRANSLATE (value)];
}

#endif

