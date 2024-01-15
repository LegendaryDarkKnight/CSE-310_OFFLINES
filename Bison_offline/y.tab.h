#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
	SymbolInfo* symbolInfo;
	ParseTree* parseTree;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
