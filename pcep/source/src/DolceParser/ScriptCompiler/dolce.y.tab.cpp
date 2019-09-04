/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 34 "dolce.y" /* yacc.c:339  */
 
	#define DEBUG //XXX Remove
	
	#include <assert.h>
	#include <stdio.h>
	
	
	#include "../../Types/TPosLiteral.h"
	#include "../../Types/TAreaLiteral.h"
	#include "../../Types/TTimeLiteral.h"
	#include "../../Types/TDurationLiteral.h"
	
	#include "ASTree.h"
	#include "Parser.h"
	#include "SymbolTable.h"

	#include "ParserContext.h"
	#define scanner scContext->scanner

	using namespace std;

	// Compiled as C++
	// Prototypes to keep the compiler happy
	union YYSTYPE;
	struct YYLTYPE;
	extern int yylex(YYSTYPE* lval, YYLTYPE* loc, void* yyscanner);
	extern void yyerror(YYLTYPE* loc, ParserContext* _context, const char *error);
	

	TDurationLiteral parseDuration(const int _count, const TDurationUnit _unit);
	TDurationLiteral durationUnitAsSeconds(const TDurationUnit _unit);

	
	

#line 102 "dolce.y.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "dolce.y.tab.hpp".  */
#ifndef YY_YY_DOLCE_Y_TAB_HPP_INCLUDED
# define YY_YY_DOLCE_Y_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    POS = 258,
    AREA = 259,
    CHAR = 260,
    INT = 261,
    FLOAT = 262,
    STRING = 263,
    TIME = 264,
    DURATION = 265,
    AND_EOP = 266,
    OR_EOP = 267,
    AFTER_EOP = 268,
    DURING_EOP = 269,
    ABSENCE_UNARY_EOP = 270,
    EXTERNAL = 271,
    EVENT = 272,
    COMPLEX_EVENT = 273,
    MAP = 274,
    ACCEPT = 275,
    USE = 276,
    LASTS = 277,
    UNTIL = 278,
    PAYLOAD = 279,
    GROUP = 280,
    DETECT = 281,
    WHERE = 282,
    IN = 283,
    SUM_FUNC = 284,
    AVG_FUNC = 285,
    COUNT_FUNC = 286,
    DIFF_FUNC = 287,
    INAREA_FUNC = 288,
    ABS_FUNC = 289,
    FFT_FUNC = 290,
    TRAJ_FUNC = 291,
    COLL_FUNC = 292,
    IDENTIFIER = 293,
    INTEGER_CONSTANT = 294,
    FLOAT_CONSTANT = 295,
    AREA_CONSTANT = 296,
    STRING_LITERAL = 297,
    CHARACTER_CONSTANT = 298,
    POS_CONSTANT = 299,
    TIME_CONSTANT = 300,
    YEARS = 301,
    MONTHS = 302,
    WEEKS = 303,
    DAYS = 304,
    HOURS = 305,
    MINUTES = 306,
    SECONDS = 307,
    EQ_OP = 308,
    NE_OP = 309,
    GE_OP = 310,
    GT_OP = 311,
    LE_OP = 312,
    LT_OP = 313,
    OR_OP = 314,
    AND_OP = 315,
    ADD_OP = 316,
    SUB_OP = 317,
    MULT_OP = 318,
    DIV_OP = 319
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 107 "dolce.y" /* yacc.c:355  */

	unsigned long a_number;
	float a_float;
	TAreaLiteral a_area;
	TPosLiteral a_pos;
	TTimeLiteral a_time;
	char a_char;
	char* a_string;
	TDurationUnit a_duration_unit;
	ASTree* a_tree;

#line 219 "dolce.y.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (ParserContext* scContext);

#endif /* !YY_YY_DOLCE_Y_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 249 "dolce.y.tab.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   244

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  141
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  249

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   319

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      74,    75,     2,     2,    71,     2,    65,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    68,
       2,    67,     2,     2,    66,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    72,     2,    73,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,     2,    70,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   183,   183,   188,   198,   203,   208,   217,   226,   231,
     236,   242,   248,   254,   260,   266,   273,   275,   277,   279,
     285,   293,   301,   306,   314,   328,   332,   340,   342,   346,
     356,   361,   369,   376,   380,   388,   390,   392,   394,   400,
     405,   409,   413,   421,   425,   432,   436,   443,   455,   462,
     469,   473,   481,   490,   494,   501,   516,   518,   526,   528,
     536,   539,   544,   552,   554,   562,   564,   572,   574,   582,
     584,   592,   594,   603,   605,   607,   609,   611,   613,   615,
     617,   623,   626,   629,   632,   638,   644,   646,   655,   657,
     667,   669,   678,   680,   690,   692,   700,   702,   711,   713,
     723,   725,   729,   733,   737,   742,   747,   751,   755,   759,
     763,   770,   773,   781,   782,   783,   784,   785,   786,   794,
     795,   796,   797,   798,   799,   800,   804,   805,   806,   807,
     808,   809,   810,   811,   817,   819,   823,   828,   833,   838,
     842,   847
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "POS", "AREA", "CHAR", "INT", "FLOAT",
  "STRING", "TIME", "DURATION", "AND_EOP", "OR_EOP", "AFTER_EOP",
  "DURING_EOP", "ABSENCE_UNARY_EOP", "EXTERNAL", "EVENT", "COMPLEX_EVENT",
  "MAP", "ACCEPT", "USE", "LASTS", "UNTIL", "PAYLOAD", "GROUP", "DETECT",
  "WHERE", "IN", "SUM_FUNC", "AVG_FUNC", "COUNT_FUNC", "DIFF_FUNC",
  "INAREA_FUNC", "ABS_FUNC", "FFT_FUNC", "TRAJ_FUNC", "COLL_FUNC",
  "IDENTIFIER", "INTEGER_CONSTANT", "FLOAT_CONSTANT", "AREA_CONSTANT",
  "STRING_LITERAL", "CHARACTER_CONSTANT", "POS_CONSTANT", "TIME_CONSTANT",
  "YEARS", "MONTHS", "WEEKS", "DAYS", "HOURS", "MINUTES", "SECONDS",
  "EQ_OP", "NE_OP", "GE_OP", "GT_OP", "LE_OP", "LT_OP", "OR_OP", "AND_OP",
  "ADD_OP", "SUB_OP", "MULT_OP", "DIV_OP", "'.'", "'@'", "'='", "';'",
  "'{'", "'}'", "','", "'['", "']'", "'('", "')'", "$accept",
  "translation_unit", "postfix_identifier", "identifier",
  "integer_constant", "float_constant", "area_constant", "pos_constant",
  "time_constant", "duration_constant", "character_constant",
  "string_literal", "decl", "external_decl", "map_decl", "event_decl",
  "event_prologue", "event_units", "event_unit", "complex_event_decl",
  "complex_event_prologue", "complex_event_units", "complex_event_unit",
  "use_statement", "lasts_statement", "event_identifier_list",
  "event_attribute_list", "event_attribute", "accept_statement",
  "payload_definition", "payload_attribute_list", "payload_attribute",
  "group_statement", "detect_statement", "where_clause", "window_clause",
  "primary_event_expr", "event_expr", "event_or_expr", "event_and_expr",
  "event_during_expr", "event_after_expr", "event_absence_expr",
  "constant_expr", "primary_expr", "expression", "logical_or_expr",
  "logical_and_expr", "comparison_expr", "additive_expr",
  "subtractive_expr", "multiplicative_expr", "divisive_expr", "func_expr",
  "func_parameter", "comparison_operator", "duration_unit",
  "type_specifier", "window_range", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    46,    64,    61,    59,   123,
     125,    44,    91,    93,    40,    41
};
# endif

#define YYPACT_NINF -96

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-96)))

#define YYTABLE_NINF -113

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      34,   147,   -30,   -30,   -30,    30,   -96,   -96,   -96,   -96,
     -49,   -96,   -44,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -30,   -96,   -96,   -96,   -38,   -96,   -96,    -8,     2,
     -12,     5,    -3,    -1,    40,   -96,     1,     9,   -96,   -96,
     -96,     0,   -30,    -4,   -96,    11,    12,   -96,   -96,   -96,
     -96,   119,   -96,    43,    58,   147,   -30,   120,   -96,   -96,
     -96,    55,   -96,   147,   -96,   -48,    -4,    -4,   -96,   -96,
      97,   113,   115,   114,   117,   -96,   -96,    63,   -96,   120,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,    67,   -96,    62,    64,    65,    70,
      71,    91,   100,   101,   102,    58,   -96,   -48,   -96,   -96,
     107,    78,   118,    51,   121,   122,   116,   124,   -96,   -32,
     -96,   -30,   -96,   109,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,    14,   -96,   -30,   -30,   -23,   -96,   108,
      58,   157,    -4,    -4,    -4,    -4,   -96,   -96,   -30,   -30,
     -30,    58,    58,    58,    58,    58,    58,   111,   -96,    58,
      58,   -96,   -96,   -96,   -96,   -96,   -96,    58,    58,    58,
      58,    58,   -96,   147,   -96,   -30,   -96,   147,   123,   -96,
     127,   128,   -96,   -96,   125,   -96,   115,   114,   117,   -96,
     -96,   -48,   126,   129,   130,   131,     8,   132,   134,   136,
     135,   137,   138,   142,   -96,   118,    51,   121,   122,   116,
     124,   -96,   -96,   -96,   -96,    58,   -30,   -30,    37,   -96,
     -96,   -96,    58,   -96,    58,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -29,   -30,   148,   -96,   145,   146,   149,    83,
     -96,   -96,   150,   -96,   -96,   -96,   -96,   -96,   -96
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     2,    16,    17,    18,
       0,    19,     0,   126,   127,   128,   129,   130,   131,   132,
     133,     0,     7,    24,    32,     0,     1,     3,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,    28,    29,
      27,     0,     0,     0,    31,     0,     0,    38,    35,    36,
      37,     0,     8,     0,     0,     0,     0,     0,    40,    41,
      22,     0,    25,     0,    53,    54,     0,     0,    60,    71,
      56,    62,    63,    65,    67,    69,    30,     0,    33,     8,
       9,    10,    15,    14,    11,    12,    73,    74,    76,    75,
      79,    80,    78,    77,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    82,    81,   100,
       0,    85,    86,    88,    90,    92,    94,    96,    98,     0,
      45,     0,    43,    42,   119,   120,   121,   122,   123,   124,
     125,    13,    26,     0,    50,     0,     0,     0,    72,     0,
       0,    58,     0,     0,     0,     0,    34,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,   113,   114,   115,   116,   117,   118,     0,     0,     0,
       0,     0,    39,     0,    47,     0,    49,     0,     0,     4,
       0,     0,    61,    57,     0,    55,    64,    66,    68,    70,
     112,   111,     0,     0,     0,    83,    82,     0,     0,     0,
       0,     0,     0,     0,    84,    87,    89,    91,    93,    95,
      97,    99,    46,    44,    51,     0,     0,     0,     0,   101,
     102,   103,     0,   106,     0,   107,   108,   109,   110,    52,
       6,     5,   134,   139,     0,   135,     0,     0,     0,     0,
     141,   140,     0,    59,   105,   104,   138,   137,   136
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -96,   -96,   -36,    -2,   -27,   -96,   -96,   -96,   -24,   -31,
     -96,   -96,   190,   -96,   -96,   -96,   -96,   -96,   160,   -96,
     -96,   -96,   153,   -96,   -14,   -96,   -96,    26,   -96,   -96,
     -96,    23,   -96,   -96,   -96,   -96,   -96,   155,   -96,    60,
      77,    79,   -59,   174,   -96,   -95,   -96,    68,    66,    61,
      72,    69,    59,    73,   -33,   -96,   -96,     4,   -96
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,   106,   107,    86,    87,    88,    89,    90,    91,
      92,    93,     6,     7,     8,     9,    10,    36,    37,    11,
      12,    45,    46,    38,    39,   123,   119,   120,    40,    48,
     133,   134,    49,    50,   141,   185,    69,    70,    71,    72,
      73,    74,    75,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   192,   167,   131,   121,   236
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,    24,    25,    59,    53,    21,    64,   138,    22,    22,
     157,    66,    32,    33,    34,    47,    52,   136,   137,    30,
      28,    32,    33,    34,    34,    29,    41,    42,    43,    31,
      26,    47,    58,    34,    22,    41,    42,    43,   172,   173,
      65,    68,   239,   180,    52,   183,     1,     2,     3,     4,
       1,     2,     3,     4,   122,    51,   197,   199,   200,   201,
     202,   203,    35,    56,    68,    68,    54,   135,    55,    63,
      67,    60,    44,   136,   137,    22,    79,    62,    22,    57,
      78,    76,    85,  -111,   176,   177,   189,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    22,    79,    80,    81,
      82,    83,    84,    85,   161,   162,   163,   164,   165,   166,
     181,    95,   190,   190,   190,   195,   193,   194,   198,   174,
     229,    22,    57,   132,   140,   142,   143,   237,   144,   238,
     145,   146,   105,   178,   179,   147,   148,   159,   149,   150,
      68,    68,    68,    68,   151,   152,   191,   191,   191,   196,
      13,    14,    15,    16,    17,    18,    19,    20,    79,    80,
      81,    82,    83,    84,    85,   153,   124,   125,   126,   127,
     128,   129,   130,   213,   154,   155,   156,   158,   160,   170,
     175,   135,   168,   182,   169,   184,   204,   235,   171,    57,
     215,   233,   216,   217,   234,    27,    61,   218,    77,   212,
     214,   219,   186,   222,   220,   221,  -112,   224,   247,   223,
     225,   248,   226,   227,   230,   231,   232,   228,   243,   242,
     187,   244,   139,   188,   245,    94,   206,   205,   207,   210,
     240,   241,     0,     0,     0,     0,     0,   246,   209,     0,
     208,     0,     0,     0,   211
};

static const yytype_int16 yycheck[] =
{
       2,     3,     4,    34,    31,     1,    42,    66,    38,    38,
     105,    15,    20,    21,    22,    29,    39,    65,    66,    21,
      69,    20,    21,    22,    22,    69,    24,    25,    26,    67,
       0,    45,    34,    22,    38,    24,    25,    26,    70,    71,
      42,    43,    71,    66,    39,   140,    16,    17,    18,    19,
      16,    17,    18,    19,    56,    67,   151,   152,   153,   154,
     155,   156,    70,    23,    66,    67,    69,    63,    69,    69,
      74,    70,    70,    65,    66,    38,    39,    68,    38,    39,
      68,    70,    45,    75,    70,    71,   145,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    53,    54,    55,    56,    57,    58,
     137,    68,   148,   149,   150,   151,   149,   150,   151,   121,
     215,    38,    39,    68,    27,    12,    11,   222,    14,   224,
      13,    68,    74,   135,   136,    68,    74,    59,    74,    74,
     142,   143,   144,   145,    74,    74,   148,   149,   150,   151,
       3,     4,     5,     6,     7,     8,     9,    10,    39,    40,
      41,    42,    43,    44,    45,    74,    46,    47,    48,    49,
      50,    51,    52,   175,    74,    74,    74,    70,    60,    63,
      71,   177,    61,    75,    62,    28,    75,   218,    64,    39,
      67,   218,    65,    65,   218,     5,    36,    72,    45,   173,
     177,    75,   142,    71,    75,    75,    75,    71,   239,    75,
      75,   242,    75,    75,   216,   217,   218,    75,    73,    71,
     143,    75,    67,   144,    75,    51,   160,   159,   167,   170,
     232,   233,    -1,    -1,    -1,    -1,    -1,   239,   169,    -1,
     168,    -1,    -1,    -1,   171
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    16,    17,    18,    19,    77,    88,    89,    90,    91,
      92,    95,    96,     3,     4,     5,     6,     7,     8,     9,
      10,   133,    38,    79,    79,    79,     0,    88,    69,    69,
      79,    67,    20,    21,    22,    70,    93,    94,    99,   100,
     104,    24,    25,    26,    70,    97,    98,   100,   105,   108,
     109,    67,    39,    80,    69,    69,    23,    39,    79,    85,
      70,    94,    68,    69,    78,    79,    15,    74,    79,   112,
     113,   114,   115,   116,   117,   118,    70,    98,    68,    39,
      40,    41,    42,    43,    44,    45,    80,    81,    82,    83,
      84,    85,    86,    87,   119,    68,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    74,    78,    79,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   102,
     103,   133,    79,   101,    46,    47,    48,    49,    50,    51,
      52,   132,    68,   106,   107,   133,    65,    66,   118,   113,
      27,   110,    12,    11,    14,    13,    68,    68,    74,    74,
      74,    74,    74,    74,    74,    74,    74,   121,    70,    59,
      60,    53,    54,    55,    56,    57,    58,   131,    61,    62,
      63,    64,    70,    71,    79,    71,    70,    71,    79,    79,
      66,    80,    75,   121,    28,   111,   115,   116,   117,   118,
      78,    79,   130,   130,   130,    78,    79,   121,   130,   121,
     121,   121,   121,   121,    75,   123,   124,   125,   126,   127,
     128,   129,   103,    79,   107,    67,    65,    65,    72,    75,
      75,    75,    71,    75,    71,    75,    75,    75,    75,   121,
      79,    79,    79,    80,    84,    85,   134,   121,   121,    71,
      79,    79,    71,    73,    75,    75,    79,    85,    85
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    77,    78,    78,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    88,    88,    88,
      89,    90,    91,    91,    92,    93,    93,    94,    94,    94,
      95,    95,    96,    97,    97,    98,    98,    98,    98,    99,
     100,   100,   100,   101,   101,   102,   102,   103,   104,   105,
     106,   106,   107,   108,   108,   109,   110,   110,   111,   111,
     112,   112,   113,   114,   114,   115,   115,   116,   116,   117,
     117,   118,   118,   119,   119,   119,   119,   119,   119,   119,
     119,   120,   120,   120,   120,   121,   122,   122,   123,   123,
     124,   124,   125,   125,   126,   126,   127,   127,   128,   128,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   130,   130,   131,   131,   131,   131,   131,   131,   132,
     132,   132,   132,   132,   132,   132,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   134,   134,   134,   134,
     134,   134
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     5,     5,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       6,     5,     4,     3,     2,     2,     3,     1,     1,     1,
       4,     3,     2,     2,     3,     1,     1,     1,     1,     4,
       2,     2,     3,     1,     3,     1,     3,     2,     4,     4,
       1,     3,     4,     2,     2,     4,     0,     2,     0,     4,
       1,     3,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     4,     4,     4,     6,     6,     4,     4,     4,     4,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     1,
       2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, scContext, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scContext); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* scContext)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scContext);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* scContext)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scContext);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParserContext* scContext)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scContext);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scContext); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* scContext)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scContext);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParserContext* scContext)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 184 "dolce.y" /* yacc.c:1646  */
    {
			
			scContext->parser->getASTree()->addChild((yyvsp[0].a_tree));
		}
#line 1632 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 189 "dolce.y" /* yacc.c:1646  */
    {
			scContext->parser->getASTree()->addChild((yyvsp[0].a_tree));
		}
#line 1640 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 199 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PPostfixExpression);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
		}
#line 1649 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 204 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PWindowOffsetPostfixExpression);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-4].a_tree))->addChild((yyvsp[-2].a_tree));
		}
#line 1658 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 209 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PWindowOffsetPostfixExpression);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-4].a_tree))->addChild(new ASTree(
									new TValue((int)0)));
		}
#line 1668 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 218 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(scContext->parser->getSymbolTable()->add(
									(yyvsp[0].a_string), 
									scContext->line, 
									scContext->col));
		}
#line 1678 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 227 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((int)(yyvsp[0].a_number)));
		}
#line 1685 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 232 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_float)));
		}
#line 1692 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 237 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_area)));
		}
#line 1699 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 243 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_pos)));
		}
#line 1706 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 249 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_time)));
		}
#line 1713 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 255 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue(parseDuration((yyvsp[-1].a_number), (yyvsp[0].a_duration_unit))));
		}
#line 1720 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 261 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_char)));
		}
#line 1727 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 267 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(new TValue((yyvsp[0].a_string)));
		}
#line 1734 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 274 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 1740 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 276 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 1746 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 278 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 1752 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 280 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); 	}
#line 1758 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 286 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PExternal); 
			(yyval.a_tree)->addChild((yyvsp[-4].a_tree))->addChild((yyvsp[-3].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1767 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 294 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PMap); 
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1776 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 302 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PEvent);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree));
		}
#line 1785 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 307 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PEvent);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
		}
#line 1793 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 315 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree);
		}
#line 1800 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 329 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventUnits);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1808 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 333 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-2].a_tree);	
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1816 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 341 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = (yyvsp[0].a_tree); }
#line 1822 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 343 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PUse);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1830 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 347 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = (yyvsp[0].a_tree); }
#line 1836 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 357 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PComplexEvent);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree));
		}
#line 1845 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 362 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PComplexEvent);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
		}
#line 1853 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 370 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree);
		}
#line 1860 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 377 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PComplexEventUnits);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1868 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 381 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-2].a_tree);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1876 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 389 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 1882 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 391 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 1888 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 393 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 1894 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 395 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); 	}
#line 1900 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 401 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[-1].a_tree);	}
#line 1906 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 406 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PLasts);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1914 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 410 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PLasts);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1922 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 414 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PLasts);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1930 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 422 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PEventIdentifierList);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1938 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 426 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-2].a_tree);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1946 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 433 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PEventAttributeList);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1954 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 437 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-2].a_tree);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1962 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 444 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventAttribute);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree))->addChild((yyvsp[0].a_tree));
		}
#line 1970 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 456 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PAccept);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 1978 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 463 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-1].a_tree);
		}
#line 1985 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 470 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PPayload);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 1993 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 474 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-2].a_tree);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2001 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 482 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PPayloadAttribute);
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree))->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2010 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 491 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PGroup); 
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2018 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 495 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PGroup); 
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2026 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 502 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PDetect);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			
			if((yyvsp[-1].a_tree))
				(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
				
			if((yyvsp[0].a_tree))
				(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2040 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 516 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = 0;
		}
#line 2047 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 519 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PDetectWhere);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2055 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 526 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = 0;
		}
#line 2062 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 529 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PDetectWindow);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2070 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 537 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	
		}
#line 2077 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 540 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-1].a_tree);	}
#line 2083 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 545 "dolce.y" /* yacc.c:1646  */
    {	
			(yyval.a_tree) = new ASTree(ASTree::PDetectEventExpression);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2092 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 553 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2098 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 555 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventOpOr);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2107 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 563 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2113 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 565 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventOpAnd);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2122 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 573 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2128 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 575 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventOpDuring);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2137 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 583 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2143 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 585 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventOpAfter);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2152 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 593 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2158 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 595 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PEventOpAbsence);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2166 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 604 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2172 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 606 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); 	}
#line 2178 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 608 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2184 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 610 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2190 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 612 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2196 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 614 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2202 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 616 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2208 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 618 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2214 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 624 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); 
		}
#line 2221 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 627 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); 
		}
#line 2228 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 630 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); 
		}
#line 2235 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 633 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-1].a_tree); 
		}
#line 2242 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 639 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2248 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 645 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2254 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 647 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PLogicalOrOp);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2263 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 656 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2269 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 658 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PLogicalAndOp);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2278 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 668 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2284 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 670 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[-1].a_tree);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2293 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 679 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2299 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 681 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::POpAdd);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2308 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 691 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2314 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 693 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::POpSub);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2323 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 701 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2329 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 703 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::POpMult);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2338 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 712 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2344 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 714 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::POpDiv);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2353 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 724 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = (yyvsp[0].a_tree); }
#line 2359 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 726 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PSumFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2367 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 730 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PAvgFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2375 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 734 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PCountFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2383 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 738 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PInAreaFuncX);
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree));
                        (yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2392 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 743 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PDiffFuncX);
			(yyval.a_tree)->addChild((yyvsp[-3].a_tree));
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2401 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 748 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PDiffFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2409 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 752 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PAbsFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2417 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 756 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PFftFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2425 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 760 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PTrajFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2433 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 764 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PCollFunc);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
		}
#line 2441 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 771 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); 
		}
#line 2448 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 774 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree); 
		}
#line 2455 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 781 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpEq); }
#line 2461 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 782 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpNe); }
#line 2467 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 783 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpGe); }
#line 2473 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 784 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpGt); }
#line 2479 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 785 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpLe); }
#line 2485 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 786 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::POpLt); }
#line 2491 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 794 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_years; }
#line 2497 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 795 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_months; }
#line 2503 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 796 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_weeks; }
#line 2509 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 797 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_days; }
#line 2515 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 798 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_hours; }
#line 2521 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 799 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_minutes; }
#line 2527 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 800 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_duration_unit) = d_seconds; }
#line 2533 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 804 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PPos); }
#line 2539 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 805 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PArea); }
#line 2545 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 806 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PChar); }
#line 2551 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 807 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PInt); }
#line 2557 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 808 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PFloat); }
#line 2563 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 809 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PString); }
#line 2569 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 810 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PTime); }
#line 2575 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 811 "dolce.y" /* yacc.c:1646  */
    { (yyval.a_tree) = new ASTree(ASTree::PDuration); }
#line 2581 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 818 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = (yyvsp[0].a_tree);	}
#line 2587 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 820 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PSlidingTimeWin);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2595 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 824 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PFixedTimeWin);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2604 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 829 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PFixedTimeWin);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2613 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 834 "dolce.y" /* yacc.c:1646  */
    { 	(yyval.a_tree) = new ASTree(ASTree::PFixedTimeWin);
			(yyval.a_tree)->addChild((yyvsp[-2].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2622 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 839 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PTupleWin);
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2630 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 843 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PTupleWin);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2639 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 848 "dolce.y" /* yacc.c:1646  */
    {	(yyval.a_tree) = new ASTree(ASTree::PTupleWin);
			(yyval.a_tree)->addChild((yyvsp[-1].a_tree));
			(yyval.a_tree)->addChild((yyvsp[0].a_tree));
		}
#line 2648 "dolce.y.tab.cpp" /* yacc.c:1646  */
    break;


#line 2652 "dolce.y.tab.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scContext, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scContext, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval, &yylloc, scContext);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
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

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scContext);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scContext, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scContext);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scContext);
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
  return yyresult;
}
#line 854 "dolce.y" /* yacc.c:1906  */



#include <stdio.h>

extern char yytext[];




TDurationLiteral parseDuration(const int _count, const TDurationUnit _unit)
{
	return (time_t)(_count * durationUnitAsSeconds(_unit));
}


//FIXME: add special prefix to the functions in order to establish a namespace  (e.g. "dolce_l_"
TDurationLiteral durationUnitAsSeconds(const TDurationUnit _unit)
{
	if(_unit == d_years) return (time_t)31536000;
	if(_unit == d_months) return (time_t)2628000; //based on month == 365/12 days.
	if(_unit == d_weeks) return (time_t)604800;
	if(_unit == d_days) return (time_t)86400;
	if(_unit == d_hours) return (time_t)3600;
	if(_unit == d_minutes) return (time_t)60;
	if(_unit == d_seconds) return (time_t)1;
	
	return (time_t)0;
}



