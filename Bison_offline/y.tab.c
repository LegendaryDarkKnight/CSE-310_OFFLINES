/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20220114

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
#include "2005033.hpp"
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#define YYSTYPE SymbolInfo*
using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_count;
extern int err_count;

FILE *fp;
FILE *logout;
FILE *errorout;
FILE *parseout;

SymbolTable *table;

void yyerror(char *s)
{

}

#line 51 "y.tab.c"

#if ! defined(YYSTYPE) && ! defined(YYSTYPE_IS_DECLARED)
/* Default: YYSTYPE is the semantic value type. */
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define IF 257
#define ELSE 258
#define FOR 259
#define WHILE 260
#define ID 261
#define LPAREN 262
#define RPAREN 263
#define SEMICOLON 264
#define LCURL 265
#define RCURL 266
#define COMMA 267
#define INT 268
#define FLOAT 269
#define VOID 270
#define LTHIRD 271
#define CONST_INT 272
#define RTHIRD 273
#define PRINTLN 274
#define RETURN 275
#define ASSIGNOP 276
#define LOGICOP 277
#define RELOP 278
#define ADDOP 279
#define MULOP 280
#define CONST_FLOAT 281
#define NOT 282
#define INCOP 283
#define DECOP 284
#define LOWER_THAN_ELSE 285
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    2,    2,    2,    4,    4,    5,    5,
    7,    7,    7,    7,    8,    8,    3,    6,    6,    6,
   10,   10,   10,   10,    9,    9,   11,   11,   11,   11,
   11,   11,   11,   11,   11,   12,   12,   14,   14,   13,
   13,   15,   15,   16,   16,   17,   17,   18,   18,   19,
   19,   19,   20,   20,   20,   20,   20,   20,   20,   21,
   21,   22,   22,
};
static const YYINT yylen[] = {                            2,
    1,    2,    1,    1,    1,    1,    6,    5,    6,    5,
    4,    3,    2,    1,    3,    2,    3,    1,    1,    1,
    3,    6,    1,    4,    1,    2,    1,    1,    1,    7,
    5,    7,    5,    5,    3,    1,    2,    1,    4,    1,
    3,    1,    3,    1,    3,    1,    3,    1,    3,    2,
    2,    1,    1,    4,    3,    1,    1,    2,    2,    1,
    0,    3,    1,
};
static const YYINT yydefred[] = {                         0,
   18,   19,   20,    0,    0,    3,    4,    5,    6,    0,
    2,    0,    0,    0,    0,   17,    0,    0,    0,    0,
    0,    0,    8,    0,   10,   13,    0,    0,   24,    0,
    0,    0,    0,    0,    0,   36,   16,   56,    0,    0,
    0,   57,    0,   27,    0,   29,    0,   25,   28,    0,
    0,   40,    0,    0,    0,   48,   52,    7,    9,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   50,   51,    0,   15,   26,   37,    0,   58,   59,    0,
    0,    0,    0,   11,   22,    0,    0,    0,   63,    0,
    0,    0,   55,    0,   35,   41,   43,    0,    0,   49,
    0,    0,    0,   54,    0,   39,    0,    0,    0,   33,
   62,   34,    0,    0,   32,   30,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  268,  269,  270,  287,  288,  289,  290,  291,  292,  293,
  289,  261,  297,  262,  271,  264,  267,  263,  293,  294,
  272,  261,  264,  265,  295,  261,  263,  267,  273,  271,
  257,  259,  260,  261,  262,  264,  266,  272,  274,  275,
  279,  281,  282,  290,  293,  295,  296,  298,  299,  300,
  301,  302,  303,  304,  305,  306,  307,  264,  295,  293,
  272,  262,  262,  262,  262,  271,  300,  262,  300,  301,
  306,  306,  261,  266,  298,  264,  276,  283,  284,  277,
  278,  279,  280,  261,  273,  300,  299,  300,  302,  308,
  309,  300,  263,  261,  264,  302,  303,  304,  305,  306,
  263,  299,  263,  263,  267,  273,  263,  298,  300,  298,
  302,  264,  258,  263,  298,  298,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          4,
    5,    6,   44,    8,    9,   45,   20,   46,   47,   13,
   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
   90,   91,
};
static const YYINT yysindex[] = {                      -127,
    0,    0,    0,    0, -127,    0,    0,    0,    0, -233,
    0, -250, -249, -179, -256,    0, -231, -148, -223, -206,
 -202, -195,    0, -187,    0,    0, -107, -127,    0, -192,
 -176, -169, -165, -245, -227,    0,    0,    0, -160, -227,
 -227,    0, -227,    0, -155,    0, -161,    0,    0, -154,
  -74,    0, -158,  -60, -157,    0,    0,    0,    0, -133,
 -141, -227,  -86, -227, -227, -227, -125, -116, -128,  -62,
    0,    0, -122,    0,    0,    0, -227,    0,    0, -227,
 -227, -227, -227,    0,    0, -101,  -86,  -99,    0,  -94,
 -113,  -88,    0,  -89,    0,    0,    0, -100, -157,    0,
 -109, -227, -109,    0, -227,    0,  -75,  -66,  -52,    0,
    0,    0, -109, -109,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,  197,    0,    0,    0,    0,    0,
    0, -216,    0,    0,    0,    0,    0,    0, -203,    0,
    0, -152,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -96,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -65,    0,  -22, -253,  -47,    0,    0,    0,    0, -201,
    0,    0,    0,    0,  -36,    0,    0,    0,    0,  -73,
    0,    0, -216,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -34,    0,    0,    0,    0,    0,    0, -214,  -39,    0,
    0,    0,    0,    0,    0,    0,    0, -135,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  196,   39,    0,    0,    8,    0,    5,    0,    0,
  -45,  -54,  -35,  -40,  -58,  123,  142,  151,  -37,    0,
    0,    0,
};
#define YYTABLESIZE 251
static const YYINT yytable[] = {                         67,
   70,   75,   70,   71,   69,   72,   89,   10,   87,   44,
   44,   14,   10,   44,   16,   21,   65,   17,   96,   44,
   15,   19,   25,   44,   70,   66,   86,   12,   88,   22,
   92,   59,  102,   34,   35,   60,   70,   26,    7,   70,
   70,   70,   70,    7,   38,  100,  111,   23,   45,   45,
   23,   41,   45,   42,   43,  108,   27,  110,   45,   14,
   28,   12,   45,   14,   70,   12,  109,  115,  116,   31,
   29,   32,   33,   34,   35,   30,   36,   24,   37,   61,
    1,    2,    3,   18,   38,   62,   39,   40,    1,    2,
    3,   41,   63,   42,   43,   31,   64,   32,   33,   34,
   35,   68,   36,   24,   74,   73,    1,    2,    3,   76,
   38,   21,   39,   40,   21,   23,   24,   41,   80,   42,
   43,   31,   83,   31,   31,   31,   31,   84,   31,   31,
   31,   85,   31,   31,   31,   95,   31,   93,   31,   31,
    1,    2,    3,   31,   94,   31,   31,   31,   15,   32,
   33,   34,   35,  105,   36,   24,   58,   24,    1,    2,
    3,  101,   38,  103,   39,   40,   38,   38,  104,   41,
   38,   42,   43,  107,   34,   35,   38,   36,   82,   38,
   38,   38,   38,   38,  106,   38,   38,   38,  112,   53,
   53,  113,   41,   53,   42,   43,    1,   53,   53,   53,
   11,   77,   97,   53,   53,   53,   53,   53,   78,   79,
  114,   53,   53,   53,   53,   46,   46,   81,   82,   46,
   78,   79,   98,   47,   47,   46,   61,   47,   60,   46,
   46,   46,   99,   47,    0,    0,    0,   47,   47,   47,
   42,   42,    0,    0,   42,    0,    0,    0,    0,    0,
   42,
};
static const YYINT yycheck[] = {                         35,
   41,   47,   43,   41,   40,   43,   65,    0,   63,  263,
  264,  262,    5,  267,  264,  272,  262,  267,   77,  273,
  271,   14,   18,  277,   65,  271,   62,  261,   64,  261,
   66,   27,   87,  261,  262,   28,   77,  261,    0,   80,
   81,   82,   83,    5,  272,   83,  105,  264,  263,  264,
  267,  279,  267,  281,  282,  101,  263,  103,  273,  263,
  267,  263,  277,  267,  105,  267,  102,  113,  114,  257,
  273,  259,  260,  261,  262,  271,  264,  265,  266,  272,
  268,  269,  270,  263,  272,  262,  274,  275,  268,  269,
  270,  279,  262,  281,  282,  257,  262,  259,  260,  261,
  262,  262,  264,  265,  266,  261,  268,  269,  270,  264,
  272,  264,  274,  275,  267,  264,  265,  279,  277,  281,
  282,  257,  280,  259,  260,  261,  262,  261,  264,  265,
  266,  273,  268,  269,  270,  264,  272,  263,  274,  275,
  268,  269,  270,  279,  261,  281,  282,  257,  271,  259,
  260,  261,  262,  267,  264,  265,  264,  265,  268,  269,
  270,  263,  272,  263,  274,  275,  263,  264,  263,  279,
  267,  281,  282,  263,  261,  262,  273,  264,  279,  276,
  277,  278,  279,  280,  273,  272,  283,  284,  264,  263,
  264,  258,  279,  267,  281,  282,    0,  263,  264,  273,
    5,  276,   80,  277,  278,  279,  280,  273,  283,  284,
  263,  277,  278,  279,  280,  263,  264,  278,  279,  267,
  283,  284,   81,  263,  264,  273,  263,  267,  263,  277,
  278,  279,   82,  273,   -1,   -1,   -1,  277,  278,  279,
  263,  264,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
  273,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 285
#define YYUNDFTOKEN 310
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","IF","ELSE","FOR","WHILE","ID",
"LPAREN","RPAREN","SEMICOLON","LCURL","RCURL","COMMA","INT","FLOAT","VOID",
"LTHIRD","CONST_INT","RTHIRD","PRINTLN","RETURN","ASSIGNOP","LOGICOP","RELOP",
"ADDOP","MULOP","CONST_FLOAT","NOT","INCOP","DECOP","LOWER_THAN_ELSE","$accept",
"start","program","unit","var_declaration","func_declaration","func_definition",
"type_specifier","parameter_list","compound_statement","statements",
"declaration_list","statement","expression_statement","expression","variable",
"logic_expression","rel_expression","simple_expression","term",
"unary_expression","factor","argument_list","arguments","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : start",
"start : program",
"program : program unit",
"program : unit",
"unit : var_declaration",
"unit : func_declaration",
"unit : func_definition",
"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON",
"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON",
"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement",
"func_definition : type_specifier ID LPAREN RPAREN compound_statement",
"parameter_list : parameter_list COMMA type_specifier ID",
"parameter_list : parameter_list COMMA type_specifier",
"parameter_list : type_specifier ID",
"parameter_list : type_specifier",
"compound_statement : LCURL statements RCURL",
"compound_statement : LCURL RCURL",
"var_declaration : type_specifier declaration_list SEMICOLON",
"type_specifier : INT",
"type_specifier : FLOAT",
"type_specifier : VOID",
"declaration_list : declaration_list COMMA ID",
"declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD",
"declaration_list : ID",
"declaration_list : ID LTHIRD CONST_INT RTHIRD",
"statements : statement",
"statements : statements statement",
"statement : var_declaration",
"statement : expression_statement",
"statement : compound_statement",
"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement",
"statement : IF LPAREN expression RPAREN statement",
"statement : IF LPAREN expression RPAREN statement ELSE statement",
"statement : WHILE LPAREN expression RPAREN statement",
"statement : PRINTLN LPAREN ID RPAREN SEMICOLON",
"statement : RETURN expression SEMICOLON",
"expression_statement : SEMICOLON",
"expression_statement : expression SEMICOLON",
"variable : ID",
"variable : ID LTHIRD expression RTHIRD",
"expression : logic_expression",
"expression : variable ASSIGNOP logic_expression",
"logic_expression : rel_expression",
"logic_expression : rel_expression LOGICOP rel_expression",
"rel_expression : simple_expression",
"rel_expression : simple_expression RELOP simple_expression",
"simple_expression : term",
"simple_expression : simple_expression ADDOP term",
"term : unary_expression",
"term : term MULOP unary_expression",
"unary_expression : ADDOP unary_expression",
"unary_expression : NOT unary_expression",
"unary_expression : factor",
"factor : variable",
"factor : ID LPAREN argument_list RPAREN",
"factor : LPAREN expression RPAREN",
"factor : CONST_INT",
"factor : CONST_FLOAT",
"factor : variable INCOP",
"factor : variable DECOP",
"argument_list : arguments",
"argument_list :",
"arguments : arguments COMMA logic_expression",
"arguments : logic_expression",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 276 "parser.y"
int main(int argc,char *argv[])
{

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout= fopen("log.txt","w");
	errorout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt","w");

	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	fprintf(logout,"Total lines: %d\nTotal errors: %d\n", line_count, err_count);
	
	delete table;
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}

#line 564 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 42 "parser.y"
	{
		fprintf(logout,"start : program \n");
	}
#line 1239 "y.tab.c"
break;
case 2:
#line 47 "parser.y"
	{
		fprintf(logout,"program : program unit \n");
	}
#line 1246 "y.tab.c"
break;
case 3:
#line 50 "parser.y"
	{
		
	}
#line 1253 "y.tab.c"
break;
case 4:
#line 55 "parser.y"
	{
		fprintf(logout,"unit : var_declaration  \n");
	}
#line 1260 "y.tab.c"
break;
case 5:
#line 58 "parser.y"
	{
		fprintf(logout,"unit : func_declaration  \n");
	 }
#line 1267 "y.tab.c"
break;
case 6:
#line 61 "parser.y"
	{
		fprintf(logout,"unit : func_definition  \n");
	 }
#line 1274 "y.tab.c"
break;
case 7:
#line 66 "parser.y"
	{
			fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
		}
#line 1281 "y.tab.c"
break;
case 8:
#line 69 "parser.y"
	{
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
		}
#line 1288 "y.tab.c"
break;
case 9:
#line 74 "parser.y"
	{
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
		}
#line 1295 "y.tab.c"
break;
case 10:
#line 77 "parser.y"
	{
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
		}
#line 1302 "y.tab.c"
break;
case 11:
#line 83 "parser.y"
	{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
		}
#line 1309 "y.tab.c"
break;
case 12:
#line 86 "parser.y"
	{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
		}
#line 1316 "y.tab.c"
break;
case 13:
#line 89 "parser.y"
	{
			fprintf(logout,"parameter_list  : type_specifier ID\n");
		}
#line 1323 "y.tab.c"
break;
case 14:
#line 92 "parser.y"
	{

		}
#line 1330 "y.tab.c"
break;
case 15:
#line 98 "parser.y"
	{
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
			}
#line 1337 "y.tab.c"
break;
case 16:
#line 101 "parser.y"
	{
				fprintf(logout,"compound_statement : LCURL RCURL\n");
			}
#line 1344 "y.tab.c"
break;
case 17:
#line 106 "parser.y"
	{
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		}
#line 1351 "y.tab.c"
break;
case 18:
#line 111 "parser.y"
	{fprintf(logout,"type_specifier : INT \n");}
#line 1356 "y.tab.c"
break;
case 19:
#line 112 "parser.y"
	{fprintf(logout,"type_specifier : FLOAT \n");}
#line 1361 "y.tab.c"
break;
case 20:
#line 113 "parser.y"
	{fprintf(logout,"type_specifier : VOID \n");}
#line 1366 "y.tab.c"
break;
case 21:
#line 116 "parser.y"
	{
					fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
					}
#line 1373 "y.tab.c"
break;
case 22:
#line 119 "parser.y"
	{
			fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
			}
#line 1380 "y.tab.c"
break;
case 23:
#line 122 "parser.y"
	{
			fprintf(logout,"declaration_list : ID \n");
		  }
#line 1387 "y.tab.c"
break;
case 24:
#line 125 "parser.y"
	{
			fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
		  }
#line 1394 "y.tab.c"
break;
case 25:
#line 130 "parser.y"
	{
			fprintf(logout,"statements : statement  \n");
		}
#line 1401 "y.tab.c"
break;
case 26:
#line 133 "parser.y"
	{
			fprintf(logout,"statements : statements statement  \n");
	   }
#line 1408 "y.tab.c"
break;
case 27:
#line 138 "parser.y"
	{
			fprintf(logout,"statement : var_declaration \n");
		}
#line 1415 "y.tab.c"
break;
case 28:
#line 141 "parser.y"
	{
			fprintf(logout,"statement : expression_statement  \n");
	  }
#line 1422 "y.tab.c"
break;
case 29:
#line 144 "parser.y"
	{
			
	  }
#line 1429 "y.tab.c"
break;
case 30:
#line 147 "parser.y"
	{
			fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
	  }
#line 1436 "y.tab.c"
break;
case 31:
#line 150 "parser.y"
	{
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
	  }
#line 1443 "y.tab.c"
break;
case 32:
#line 153 "parser.y"
	{
			fprintf(logout,"IF LPAREN expression RPAREN statement ELSE statement  \n");
	  }
#line 1450 "y.tab.c"
break;
case 33:
#line 156 "parser.y"
	{
			fprintf(logout,"WHILE LPAREN expression RPAREN statement  \n");
	  }
#line 1457 "y.tab.c"
break;
case 34:
#line 159 "parser.y"
	{
			fprintf(logout,"PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
	  }
#line 1464 "y.tab.c"
break;
case 35:
#line 162 "parser.y"
	{
			fprintf(logout,"RETURN expression SEMICOLON  \n");
	  }
#line 1471 "y.tab.c"
break;
case 36:
#line 167 "parser.y"
	{

			}
#line 1478 "y.tab.c"
break;
case 37:
#line 170 "parser.y"
	{

			}
#line 1485 "y.tab.c"
break;
case 38:
#line 175 "parser.y"
	{
		fprintf(logout,"variable : ID 	 \n");
	}
#line 1492 "y.tab.c"
break;
case 39:
#line 178 "parser.y"
	{
		fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
	 }
#line 1499 "y.tab.c"
break;
case 40:
#line 183 "parser.y"
	{
			fprintf(logout,"expression 	: logic_expression	 \n");
	 	}
#line 1506 "y.tab.c"
break;
case 41:
#line 186 "parser.y"
	{
			fprintf(logout,"expression 	: variable ASSIGNOP logic_expression 		 \n");
	   }
#line 1513 "y.tab.c"
break;
case 42:
#line 191 "parser.y"
	{
			fprintf(logout,"logic_expression : rel_expression 	 \n");
		}
#line 1520 "y.tab.c"
break;
case 43:
#line 194 "parser.y"
	{
			fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
		 }
#line 1527 "y.tab.c"
break;
case 44:
#line 199 "parser.y"
	{
		fprintf(logout,"rel_expression	: simple_expression \n");
		}
#line 1534 "y.tab.c"
break;
case 45:
#line 202 "parser.y"
	{
		fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");

		}
#line 1542 "y.tab.c"
break;
case 46:
#line 208 "parser.y"
	{
		fprintf(logout,"simple_expression : term \n");

		}
#line 1550 "y.tab.c"
break;
case 47:
#line 212 "parser.y"
	{
		fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");

		  }
#line 1558 "y.tab.c"
break;
case 48:
#line 218 "parser.y"
	{
		fprintf(logout,"term :	unary_expression \n");
	}
#line 1565 "y.tab.c"
break;
case 49:
#line 221 "parser.y"
	{
		fprintf(logout,"term : term MULOP unary_expression \n");
	 }
#line 1572 "y.tab.c"
break;
case 50:
#line 226 "parser.y"
	{
			fprintf(logout,"factor : variable  \n");
		}
#line 1579 "y.tab.c"
break;
case 51:
#line 229 "parser.y"
	{
			fprintf(logout,"unary_expression : NOT unary_expression \n");
		 }
#line 1586 "y.tab.c"
break;
case 52:
#line 232 "parser.y"
	{
			fprintf(logout,"unary_expression : factor \n");
		 }
#line 1593 "y.tab.c"
break;
case 53:
#line 237 "parser.y"
	{
	fprintf(logout,"factor : variable  \n");
	}
#line 1600 "y.tab.c"
break;
case 54:
#line 240 "parser.y"
	{
		fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
	}
#line 1607 "y.tab.c"
break;
case 55:
#line 243 "parser.y"
	{
		fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
	}
#line 1614 "y.tab.c"
break;
case 56:
#line 246 "parser.y"
	{
		fprintf(logout,"factor	: CONST_INT   \n");
	}
#line 1621 "y.tab.c"
break;
case 57:
#line 249 "parser.y"
	{
		fprintf(logout,"factor	: CONST_FLOAT   \n");
	}
#line 1628 "y.tab.c"
break;
case 58:
#line 252 "parser.y"
	{
		fprintf(logout,"factor	: variable INCOP  \n");
	}
#line 1635 "y.tab.c"
break;
case 59:
#line 255 "parser.y"
	{
		fprintf(logout,"factor	: variable DECOP  \n");
	}
#line 1642 "y.tab.c"
break;
case 60:
#line 260 "parser.y"
	{
			fprintf(logout,"argument_list : arguments  \n");
			}
#line 1649 "y.tab.c"
break;
case 62:
#line 266 "parser.y"
	{
			fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			}
#line 1656 "y.tab.c"
break;
case 63:
#line 269 "parser.y"
	{
			fprintf(logout,"arguments : logic_expression\n");
		  }
#line 1663 "y.tab.c"
break;
#line 1665 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
