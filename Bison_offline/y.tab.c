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
using namespace std;

int ScopeTable::counter1 = 0;
int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_count;
extern int err_count;
extern int start_line;
bool infunc;
MyStack stack1;

FILE *fp;
FILE *logout;
FILE *errorout;
FILE *parseout;
FILE *checkerout;

SymbolTable *table;
SymbolInfo *temp;

string type;
void yyerror(char *s)
{

}

ParseTree* setLeft(string type, string line){
	return new ParseTree(type,line);
}

ParseTree* symbolToParse(SymbolInfo *s){
	return new ParseTree("", s->getType()+" : "+s->getName());
}

void insertParameters(){
	{
	if(infunc){
		SymbolInfo *s;
		while(!stack1.isEmpty()){
			s = stack1.pop() ;
			table->Insert(s->getName(),s->getType());
			/* delete s;*/
			s = nullptr;
		}
		infunc = false;
	}
}
}
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 58 "parser.y"
typedef union YYSTYPE {
	SymbolInfo* symbolInfo;
	ParseTree* parseTree;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 91 "y.tab.c"

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

#ifndef YYDESTRUCT_DECL
#define YYDESTRUCT_DECL() yydestruct(const char *msg, int psymb, YYSTYPE *val)
#endif
#ifndef YYDESTRUCT_CALL
#define YYDESTRUCT_CALL(msg, psymb, val) yydestruct(msg, psymb, val)
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
   11,   11,   11,   11,   11,   13,   13,   20,   20,   12,
   12,   14,   14,   15,   15,   16,   16,   17,   17,   18,
   18,   18,   19,   19,   19,   19,   19,   19,   19,   21,
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
    0,   57,    0,   27,    0,   29,    0,   25,    0,   28,
   40,    0,    0,    0,   48,   52,    0,    7,    9,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   50,
    0,   51,    0,   15,   26,   37,    0,    0,    0,    0,
    0,   58,   59,   11,   22,    0,    0,    0,   63,    0,
    0,    0,   55,    0,   35,   43,    0,    0,   49,   41,
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
  272,  262,  262,  262,  262,  271,  299,  262,  299,  305,
  307,  305,  261,  266,  298,  264,  277,  278,  279,  280,
  276,  283,  284,  261,  273,  299,  300,  299,  301,  308,
  309,  299,  263,  261,  264,  302,  303,  304,  305,  301,
  263,  300,  263,  263,  267,  273,  263,  298,  299,  298,
  301,  264,  258,  263,  298,  298,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          4,
    5,    6,   44,    8,    9,   45,   20,   46,   47,   13,
   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
   90,   91,
};
static const YYINT yysindex[] = {                       -60,
    0,    0,    0,    0,  -60,    0,    0,    0,    0, -238,
    0, -252, -152, -179, -230,    0, -227, -122, -207, -218,
 -213, -205,    0, -187,    0,    0, -107,  -60,    0, -201,
 -186, -182, -176, -250,  -87,    0,    0,    0, -160,  -87,
  -87,    0,  -87,    0, -155,    0, -161,    0, -154,    0,
    0, -149, -100, -163,    0,    0,  -44,    0,    0, -138,
 -141,  -87, -229,  -87,  -87,  -87, -127, -116, -115,    0,
  -50,    0, -117,    0,    0,    0,  -87,  -87,  -87,  -87,
  -87,    0,    0,    0,    0, -101, -229,  -99,    0,  -94,
  -91,  -82,    0,  -77,    0,    0,  -86, -163,    0,    0,
 -109,  -87, -109,    0,  -87,    0,  -68,  -58,  -36,    0,
    0,    0, -109, -109,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,  202,    0,    0,    0,    0,    0,
    0, -148,    0,    0,    0,    0,    0,    0, -208,    0,
    0, -126,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -96,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -47, -249,  -66,    0,    0,  -74,    0,    0, -170,
    0,    0,    0,    0,  -27,    0,    0,    0,    0,    0,
 -216,    0, -148,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -26,    0,    0,    0,    0,    0,  -42,  -49,    0,    0,
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
    0,  214,   11,    0,    0,    8,    0,   -1,    0,    0,
  -45,  -35,  -57,  -61,  146,  160,  162,  -34,    0,  -40,
    0,    0,
};
#define YYTABLESIZE 241
static const YYINT yytable[] = {                         67,
   71,   75,   71,   89,   69,   87,   70,   10,   72,   14,
    7,   65,   10,   44,   44,    7,   25,   44,   15,  100,
   66,   19,   12,   44,   71,   59,   86,   44,   88,  102,
   92,   34,   35,   22,   36,   60,   71,   71,   71,   71,
   71,   21,   38,  111,   27,   99,   53,   53,   28,   41,
   53,   42,   43,   26,   14,  108,   53,  110,   14,   29,
   53,   53,   53,   53,   71,   30,  109,  115,  116,   31,
   61,   32,   33,   34,   35,   62,   36,   24,   37,   63,
    1,    2,    3,   18,   38,   64,   39,   40,    1,    2,
    3,   41,   12,   42,   43,   31,   12,   32,   33,   34,
   35,   68,   36,   24,   74,   73,    1,    2,    3,   76,
   38,   16,   39,   40,   17,   23,   80,   41,   23,   42,
   43,   31,   84,   31,   31,   31,   31,   77,   31,   31,
   31,   85,   31,   31,   31,   93,   31,   21,   31,   31,
   21,   23,   24,   31,   94,   31,   31,   31,   95,   32,
   33,   34,   35,   15,   36,   24,   58,   24,    1,    2,
    3,  101,   38,  103,   39,   40,   38,   38,  104,   41,
   38,   42,   43,   34,   35,  105,   38,   78,   79,   38,
   38,   38,   38,   38,   38,  107,   38,   38,   53,   53,
  106,   41,   79,   42,   43,  112,   46,   46,   53,  113,
   46,    1,   53,   53,   53,   53,   46,    1,    2,    3,
   46,   46,   46,   47,   47,   42,   42,   47,   11,   42,
   45,   45,   96,   47,   45,   42,  114,   47,   47,   47,
   45,   81,   82,   83,   45,   61,   60,   97,   82,   83,
   98,
};
static const YYINT yycheck[] = {                         35,
   41,   47,   43,   65,   40,   63,   41,    0,   43,  262,
    0,  262,    5,  263,  264,    5,   18,  267,  271,   81,
  271,   14,  261,  273,   65,   27,   62,  277,   64,   87,
   66,  261,  262,  261,  264,   28,   77,   78,   79,   80,
   81,  272,  272,  105,  263,   80,  263,  264,  267,  279,
  267,  281,  282,  261,  263,  101,  273,  103,  267,  273,
  277,  278,  279,  280,  105,  271,  102,  113,  114,  257,
  272,  259,  260,  261,  262,  262,  264,  265,  266,  262,
  268,  269,  270,  263,  272,  262,  274,  275,  268,  269,
  270,  279,  263,  281,  282,  257,  267,  259,  260,  261,
  262,  262,  264,  265,  266,  261,  268,  269,  270,  264,
  272,  264,  274,  275,  267,  264,  280,  279,  267,  281,
  282,  257,  261,  259,  260,  261,  262,  277,  264,  265,
  266,  273,  268,  269,  270,  263,  272,  264,  274,  275,
  267,  264,  265,  279,  261,  281,  282,  257,  264,  259,
  260,  261,  262,  271,  264,  265,  264,  265,  268,  269,
  270,  263,  272,  263,  274,  275,  263,  264,  263,  279,
  267,  281,  282,  261,  262,  267,  273,  278,  279,  276,
  277,  278,  279,  280,  272,  263,  283,  284,  263,  264,
  273,  279,  279,  281,  282,  264,  263,  264,  273,  258,
  267,    0,  277,  278,  279,  280,  273,  268,  269,  270,
  277,  278,  279,  263,  264,  263,  264,  267,    5,  267,
  263,  264,   77,  273,  267,  273,  263,  277,  278,  279,
  273,  276,  283,  284,  277,  263,  263,   78,  283,  284,
   79,
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
"declaration_list","statement","expression","expression_statement",
"logic_expression","rel_expression","simple_expression","term",
"unary_expression","factor","variable","argument_list","arguments",
"illegal-symbol",
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
#line 635 "parser.y"
int main(int argc,char *argv[])
{
	infunc = false;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout= fopen("log.txt","w");
	errorout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt","w");
	checkerout = fopen("checker.txt", "w");

	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	fprintf(logout,"Total lines: %d\nTotal errors: %d\n", line_count,err_count);
	
	delete table;
	fclose(checkerout);
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}

#line 605 "y.tab.c"

/* Release memory associated with symbol. */
#if ! defined YYDESTRUCT_IS_DECLARED
static void
YYDESTRUCT_DECL()
{
    switch (psymb)
    {
	case 287:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 622 "y.tab.c"
	break;
	case 288:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 632 "y.tab.c"
	break;
	case 289:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 642 "y.tab.c"
	break;
	case 290:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 652 "y.tab.c"
	break;
	case 291:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 662 "y.tab.c"
	break;
	case 292:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 672 "y.tab.c"
	break;
	case 293:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 682 "y.tab.c"
	break;
	case 294:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 692 "y.tab.c"
	break;
	case 295:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 702 "y.tab.c"
	break;
	case 296:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 712 "y.tab.c"
	break;
	case 297:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 722 "y.tab.c"
	break;
	case 298:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 732 "y.tab.c"
	break;
	case 299:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 742 "y.tab.c"
	break;
	case 300:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 752 "y.tab.c"
	break;
	case 301:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 762 "y.tab.c"
	break;
	case 302:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 772 "y.tab.c"
	break;
	case 303:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 782 "y.tab.c"
	break;
	case 304:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 792 "y.tab.c"
	break;
	case 305:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 802 "y.tab.c"
	break;
	case 306:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 812 "y.tab.c"
	break;
	case 307:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 822 "y.tab.c"
	break;
	case 308:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 832 "y.tab.c"
	break;
	case 309:
#line 72 "parser.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 842 "y.tab.c"
	break;
    }
}
#define YYDESTRUCT_IS_DECLARED 1
#endif

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
#line 86 "parser.y"
	{
		fprintf(logout,"start : program \n");
		yyval.parseTree = setLeft("start","start : program");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setEndLine(line_count);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->printTree(yyval.parseTree,0,parseout);
	}
#line 1527 "y.tab.c"
break;
case 2:
#line 96 "parser.y"
	{
		fprintf(logout,"program : program unit \n");
		yyval.parseTree = setLeft("program","program : program unit");
		yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); 
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree); 
		yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);

	}
#line 1540 "y.tab.c"
break;
case 3:
#line 105 "parser.y"
	{
		fprintf(logout,"program : unit \n");
		yyval.parseTree = setLeft("program","program : unit");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
	}
#line 1551 "y.tab.c"
break;
case 4:
#line 114 "parser.y"
	{
		fprintf(logout,"unit : var_declaration  \n");
		yyval.parseTree = setLeft("unit","unit : var_declaration");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
	}
#line 1562 "y.tab.c"
break;
case 5:
#line 121 "parser.y"
	{
		fprintf(logout,"unit : func_declaration  \n");
		yyval.parseTree = setLeft("unit","unit : func_declaration");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
	 }
#line 1573 "y.tab.c"
break;
case 6:
#line 128 "parser.y"
	{
		fprintf(logout,"unit : func_definition  \n");
		yyval.parseTree = setLeft("unit","unit : func_definition");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
	 }
#line 1584 "y.tab.c"
break;
case 7:
#line 137 "parser.y"
	{
			yyval.parseTree = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = yystack.l_mark[-4].symbolInfo;
			table->Insert(temp->getName(),"FUNCTION");
			yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LPAREN : ("));
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
			infunc = false;
			stack1.cleanUp();
		}
#line 1600 "y.tab.c"
break;
case 8:
#line 149 "parser.y"
	{
			yyval.parseTree = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
			temp = yystack.l_mark[-3].symbolInfo;
			table->Insert(temp->getName(),"FUNCTION");
			yyval.parseTree->addChild(yystack.l_mark[-4].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LPAREN : ("));
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(yystack.l_mark[-4].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 1614 "y.tab.c"
break;
case 9:
#line 161 "parser.y"
	{
			yyval.parseTree = setLeft("func_definition","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = yystack.l_mark[-4].symbolInfo;
			table->Insert(temp->getName(),"FUNCTION");
			yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LPAREN : ("));
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);	
		}
#line 1628 "y.tab.c"
break;
case 10:
#line 171 "parser.y"
	{
			yyval.parseTree = setLeft("func_definition","func_definition : type_specifier ID LPAREN RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
			temp = yystack.l_mark[-3].symbolInfo;
			table->Insert(temp->getName(),"FUNCTION");
			yyval.parseTree->addChild(yystack.l_mark[-4].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LPAREN : ("));
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);		
			yyval.parseTree->setStartLine(yystack.l_mark[-4].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 1642 "y.tab.c"
break;
case 11:
#line 184 "parser.y"
	{
			infunc = true;
			yyval.parseTree = setLeft("parameter_list","parameter_list : parameter_list COMMA type_specifier ID");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
			temp = yystack.l_mark[0].symbolInfo;
			stack1.push(new SymbolInfo(temp->getName(), type));
			yyval.parseTree->addChild(yystack.l_mark[-3].parseTree); yyval.parseTree->addChild(new ParseTree("","COMMA : ,"));
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-3].parseTree->getStartLine());	
			yyval.parseTree->setEndLine(line_count);
		}
#line 1657 "y.tab.c"
break;
case 12:
#line 195 "parser.y"
	{
			yyval.parseTree = setLeft("parameter_list", "parameter_list : parameter_list COMMA type_specifier");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","COMMA : ,")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 1668 "y.tab.c"
break;
case 13:
#line 202 "parser.y"
	{
			infunc = true;
			yyval.parseTree = setLeft("parameter_list","parameter_list  : type_specifier ID");
			fprintf(logout,"parameter_list  : type_specifier ID\n");
			temp = yystack.l_mark[0].symbolInfo;
			stack1.push(new SymbolInfo(temp->getName(), type));
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 1682 "y.tab.c"
break;
case 14:
#line 212 "parser.y"
	{
			yyval.parseTree = setLeft("parameter_list","parameter_list  : type_specifier");
			fprintf(logout,"parameter_list  : type_specifier\n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 1693 "y.tab.c"
break;
case 15:
#line 222 "parser.y"
	{
				yyval.parseTree = setLeft("compound_statement","compound_statement : LCURL statements RCURL");
				if(yyval.parseTree == nullptr)
				cout<<"Null ";
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				yyval.parseTree->addChild(new ParseTree("","LCURL : {")); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","RCURL : }"));
				yyval.parseTree->setStartLine(start_line);
				yyval.parseTree->setEndLine(line_count);		
			}
#line 1708 "y.tab.c"
break;
case 16:
#line 233 "parser.y"
	{
				yyval.parseTree = setLeft("compound_statement","compound_statement : LCURL RCURL");
				fprintf(logout,"compound_statement : LCURL RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				yyval.parseTree->addChild(new ParseTree("","LCURL : {")); yyval.parseTree->addChild(new ParseTree("","RCURL : }"));
				yyval.parseTree->setStartLine(start_line);
				yyval.parseTree->setEndLine(line_count);
			}
#line 1721 "y.tab.c"
break;
case 17:
#line 244 "parser.y"
	{
		yyval.parseTree = setLeft("var_declaration","var_declaration : type_specifier declaration_list SEMICOLON");
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;")); 
		yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
		}
#line 1732 "y.tab.c"
break;
case 18:
#line 253 "parser.y"
	{
		yyval.parseTree = setLeft("type_specifier", "type_specifier : INT");fprintf(logout,"type_specifier : INT \n"); type = "INT"; 
		yyval.parseTree->addChild(new ParseTree("","INT : int")); 
		yyval.parseTree->setStartLine(start_line);
		yyval.parseTree->setEndLine(line_count);
		}
#line 1742 "y.tab.c"
break;
case 19:
#line 259 "parser.y"
	{
		yyval.parseTree = setLeft("type_specifier", "type_specifier : FLOAT");fprintf(logout,"type_specifier : FLOAT \n"); type = "FLOAT"; 
		yyval.parseTree->addChild(new ParseTree("","FLOAT : float")); 
		yyval.parseTree->setStartLine(start_line);
		yyval.parseTree->setEndLine(line_count);
		}
#line 1752 "y.tab.c"
break;
case 20:
#line 265 "parser.y"
	{
		yyval.parseTree = setLeft("type_specifier", "type_specifier : VOID");fprintf(logout,"type_specifier : VOID \n"); type = "VOID"; 
		yyval.parseTree->addChild(new ParseTree("","VOID : void")); 
		yyval.parseTree->setStartLine(start_line);
		yyval.parseTree->setEndLine(line_count);
		}
#line 1762 "y.tab.c"
break;
case 21:
#line 273 "parser.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list : declaration_list COMMA ID");
				fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
				temp = yystack.l_mark[0].symbolInfo;
				table->Insert(temp->getName(),type);
				yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","COMMA : ,")); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
				yyval.parseTree->setEndLine(line_count);
				}
#line 1775 "y.tab.c"
break;
case 22:
#line 282 "parser.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
				temp = yystack.l_mark[-3].symbolInfo;
				table->Insert(temp->getName(),type);
				yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(new ParseTree("","COMMA : ,")); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo));
				yyval.parseTree->addChild(new ParseTree("","LTHIRD : [")); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","RTHIRD : ]")); 
				yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
				yyval.parseTree->setEndLine(line_count);
				}
#line 1789 "y.tab.c"
break;
case 23:
#line 292 "parser.y"
	{
				yyval.parseTree=setLeft("declaration_list", "declaration_list : ID");
				fprintf(logout,"declaration_list : ID \n");
				temp = yystack.l_mark[0].symbolInfo;
				table->Insert(temp->getName(),type);
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(start_line);
				yyval.parseTree->setEndLine(line_count);
		  		}
#line 1802 "y.tab.c"
break;
case 24:
#line 301 "parser.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list : ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
				temp = yystack.l_mark[-3].symbolInfo;
				table->Insert(temp->getName(),type);
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LTHIRD : [")); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","RTHIRD : ]"));
				yyval.parseTree->setStartLine(start_line);
				yyval.parseTree->setEndLine(line_count);
		  }
#line 1815 "y.tab.c"
break;
case 25:
#line 312 "parser.y"
	{
		yyval.parseTree=setLeft("statements","statements : statement");
		fprintf(logout,"statements : statement  \n");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
		insertParameters();
		}
#line 1827 "y.tab.c"
break;
case 26:
#line 320 "parser.y"
	{
		yyval.parseTree=setLeft("statements","statements : statements statement");
		fprintf(logout,"statements : statements statement  \n");
		yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
		yyval.parseTree->setEndLine(line_count);
		insertParameters();
	   }
#line 1839 "y.tab.c"
break;
case 27:
#line 330 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : var_declaration");
			fprintf(logout,"statement : var_declaration \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
			}
#line 1850 "y.tab.c"
break;
case 28:
#line 337 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : expression_statement");
			fprintf(logout,"statement : expression_statement  \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
	  		}
#line 1861 "y.tab.c"
break;
case 29:
#line 344 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : compound_statement");
			fprintf(logout,"statement : compound_statement  \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
	  		}
#line 1872 "y.tab.c"
break;
case 30:
#line 351 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
			yyval.parseTree->addChild(new ParseTree("","FOR : for")); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-4].parseTree);
			yyval.parseTree->addChild(yystack.l_mark[-3].parseTree); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
	  		}
#line 1884 "y.tab.c"
break;
case 31:
#line 359 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : IF LPAREN expression RPAREN statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
			yyval.parseTree->addChild(new ParseTree("","IF : if")); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree);
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
	  		}
#line 1896 "y.tab.c"
break;
case 32:
#line 367 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : IF LPAREN expression RPAREN statement ELSE statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");
			yyval.parseTree->addChild(new ParseTree("","IF : if")); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-4].parseTree);
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","ELSE : else")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
	  		}
#line 1908 "y.tab.c"
break;
case 33:
#line 375 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : WHILE LPAREN expression RPAREN statement");
			fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement  \n");
			yyval.parseTree->addChild(new ParseTree("","WHILE : while")); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree);
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

	  		}
#line 1921 "y.tab.c"
break;
case 34:
#line 384 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");			
			fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
			yyval.parseTree->addChild(new ParseTree("","PRINTLN : println")); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo));
			yyval.parseTree->addChild(new ParseTree("","RPAREN : )")); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

	  		}
#line 1934 "y.tab.c"
break;
case 35:
#line 393 "parser.y"
	{
			yyval.parseTree=setLeft("statement","statement : RETURN expression SEMICOLON");
			fprintf(logout,"statement : RETURN expression SEMICOLON  \n");
			yyval.parseTree->addChild(new ParseTree("","RETURN : return")); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

	  		}
#line 1946 "y.tab.c"
break;
case 36:
#line 403 "parser.y"
	{
			yyval.parseTree=setLeft("expression_statement","expression_statement : SEMICOLON");
			fprintf(logout,"expression_statement : SEMICOLON\n");
			yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
			}
#line 1957 "y.tab.c"
break;
case 37:
#line 410 "parser.y"
	{
			yyval.parseTree=setLeft("expression_statement","expression_statement : expression SEMICOLON");
			fprintf(logout,"expression_statement : expression SEMICOLON\n");
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","SEMICOLON : ;"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
			}
#line 1968 "y.tab.c"
break;
case 38:
#line 419 "parser.y"
	{
		yyval.parseTree=setLeft("variable","variable : ID");
		fprintf(logout,"variable : ID 	 \n");
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(start_line);
		yyval.parseTree->setEndLine(line_count);
		}
#line 1979 "y.tab.c"
break;
case 39:
#line 426 "parser.y"
	{
		yyval.parseTree=setLeft("variable","variable : ID LSQUARE expression RSQUARE");
		fprintf(logout,"variable : ID LSQUARE expression RSQUARE  	 \n");
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LTHIRD : [")); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","RTHIRD : ]"));
		yyval.parseTree->setStartLine(start_line);
		yyval.parseTree->setEndLine(line_count);
	 }
#line 1990 "y.tab.c"
break;
case 40:
#line 435 "parser.y"
	{
			yyval.parseTree=setLeft("expression","expression 	: logic_expression");
			fprintf(logout,"expression 	: logic_expression	 \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
	 		}
#line 2001 "y.tab.c"
break;
case 41:
#line 442 "parser.y"
	{
			yyval.parseTree=setLeft("expression","expression 	: variable ASSIGNOP logic_expression");
			fprintf(logout,"expression 	: variable ASSIGNOP logic_expression 		 \n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
	   		}
#line 2012 "y.tab.c"
break;
case 42:
#line 451 "parser.y"
	{
			yyval.parseTree=setLeft("logic_expression","logic_expression : rel_expression");
			fprintf(logout,"logic_expression : rel_expression 	 \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
			}
#line 2023 "y.tab.c"
break;
case 43:
#line 458 "parser.y"
	{
			yyval.parseTree=setLeft("logic_expression","logic_expression : rel_expression LOGICOP rel_expression");
			fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		 	}
#line 2034 "y.tab.c"
break;
case 44:
#line 467 "parser.y"
	{
			yyval.parseTree=setLeft("rel_expression","rel_expression	: simple_expression");
			fprintf(logout,"rel_expression	: simple_expression \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
			}
#line 2045 "y.tab.c"
break;
case 45:
#line 474 "parser.y"
	{
			yyval.parseTree=setLeft("rel_expression","rel_expression	: simple_expression RELOP simple_expression");
			fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		}
#line 2056 "y.tab.c"
break;
case 46:
#line 483 "parser.y"
	{
			yyval.parseTree=setLeft("simple_expression","simple_expression : term");
			fprintf(logout,"simple_expression : term \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setEndLine(line_count);
			}
#line 2066 "y.tab.c"
break;
case 47:
#line 489 "parser.y"
	{
			yyval.parseTree=setLeft("simple_expression","simple_expression :	simple_expression ADDOP term");
			fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
		  }
#line 2077 "y.tab.c"
break;
case 48:
#line 498 "parser.y"
	{
			yyval.parseTree=setLeft("term","term :	unary_expression");
			fprintf(logout,"term :	unary_expression \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2089 "y.tab.c"
break;
case 49:
#line 506 "parser.y"
	{
			yyval.parseTree=setLeft("term","term : term MULOP unary_expression");
			fprintf(logout,"term : term MULOP unary_expression \n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);
	 }
#line 2100 "y.tab.c"
break;
case 50:
#line 515 "parser.y"
	{
			yyval.parseTree=setLeft("unary_expression","unary_expression : ADDOP unary_expression");
			fprintf(logout,"unary_expression : ADDOP unary_expression  \n");
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

			}
#line 2112 "y.tab.c"
break;
case 51:
#line 523 "parser.y"
	{
			yyval.parseTree=setLeft("unary_expression","unary_expression : NOT unary_expression");
			fprintf(logout,"unary_expression : NOT unary_expression \n");
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

		 	}
#line 2124 "y.tab.c"
break;
case 52:
#line 531 "parser.y"
	{
			yyval.parseTree=setLeft("unary_expression","unary_expression : factor");
			fprintf(logout,"unary_expression : factor \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

		 	}
#line 2136 "y.tab.c"
break;
case 53:
#line 541 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor : variable");
			fprintf(logout,"factor : variable  \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2148 "y.tab.c"
break;
case 54:
#line 549 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: ID LPAREN argument_list RPAREN");
			fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","RPAREN : )"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
			}
#line 2159 "y.tab.c"
break;
case 55:
#line 556 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: LPAREN expression RPAREN");
			fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
			yyval.parseTree->addChild(new ParseTree("","LPAREN : (")); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(new ParseTree("","RPAREN : )"));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);
			}
#line 2170 "y.tab.c"
break;
case 56:
#line 563 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: CONST_INT");
			fprintf(logout,"factor	: CONST_INT   \n");
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

			}
#line 2182 "y.tab.c"
break;
case 57:
#line 571 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: CONST_FLOAT");
			fprintf(logout,"factor	: CONST_FLOAT   \n");
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(start_line);
			yyval.parseTree->setEndLine(line_count);

			}
#line 2194 "y.tab.c"
break;
case 58:
#line 579 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: variable INCOP");
			fprintf(logout,"factor	: variable INCOP  \n");
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2206 "y.tab.c"
break;
case 59:
#line 587 "parser.y"
	{
			yyval.parseTree=setLeft("factor","factor	: variable DECOP");
			fprintf(logout,"factor	: variable DECOP  \n");
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2218 "y.tab.c"
break;
case 60:
#line 597 "parser.y"
	{
			yyval.parseTree=setLeft("argument_list","argument_list : arguments");
			fprintf(logout,"argument_list : arguments  \n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2230 "y.tab.c"
break;
case 61:
#line 605 "parser.y"
	{
			yyval.parseTree=setLeft("argument_list","argument_list :");
			fprintf(logout,"argument_list :\n");
			yyval.parseTree->addChild(new ParseTree("",""));
			yyval.parseTree->setStartLine(line_count);
			yyval.parseTree->setEndLine(line_count);

			}
#line 2242 "y.tab.c"
break;
case 62:
#line 615 "parser.y"
	{
			yyval.parseTree=setLeft("arguments","arguments : arguments COMMA logic_expression");
			fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(new ParseTree("","COMMA : ,")); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

			}
#line 2254 "y.tab.c"
break;
case 63:
#line 623 "parser.y"
	{
			yyval.parseTree=setLeft("arguments","arguments : logic_expression");
			fprintf(logout,"arguments : logic_expression\n");
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(line_count);

		  }
#line 2266 "y.tab.c"
break;
#line 2268 "y.tab.c"
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
