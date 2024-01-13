%option noyywrap

%{
#include "2005033.hpp"
#include "y.tab.h"
int line_count= 1;
int err_count = 0;

#define YYSTYPE SymbolInfo*
extern YYSTYPE yylval;
extern SymbolTable *table;
extern FILE *logout;
%}

WHITESPACE [ \t\f\r\v]* 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \r?\n
IDENTIFIER (_|{LETTER})({LETTER}|_|{DIGIT})*
CONST_INTEGER {DIGIT}+
CONST_FLOAT (({DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+))|({DIGIT}*(\.{DIGIT}+)(E[+-]?{DIGIT}+)?))
TOO_MANY_POINTS {DIGIT}*\.({DIGIT}*\.{DIGIT}*)+({DIGIT}+E[+-]?{DIGIT}+)?
ILLFORMED_NUMBER (({DIGIT}+(\.{DIGIT}+)?(E[+-]?({DIGIT}*\.{DIGIT}*)))|((\.{DIGIT}+)(E[+-]?({DIGIT}*\.{DIGIT}*))))
INVALID_ID ({CONST_FLOAT}|{CONST_INTEGER}){IDENTIFIER}

%%

{NEWLINE} {line_count++; }

{WHITESPACE} {}

"if"	{
			fprintf(logout,"Line# %d: Token <IF> Lexeme %s found\n",line_count,yytext);
			return IF;
		}
		
"else"	{	
			fprintf(logout,"Line# %d: Token <ELSE> Lexeme %s found\n",line_count,yytext);
			return ELSE;
		}

"for"	{	
			fprintf(logout,"Line# %d: Token <FOR> Lexeme %s found\n",line_count,yytext);
			return FOR;
		}

"while"	{	
			fprintf(logout,"Line# %d: Token <WHILE> Lexeme %s found\n",line_count,yytext);
			return WHILE;
		}

"int"	{	
			fprintf(logout,"Line# %d: Token <INT> Lexeme %s found\n",line_count,yytext);
			return INT;
		}

"float"	{	
			fprintf(logout,"Line# %d: Token <FLOAT> Lexeme %s found\n",line_count,yytext);
			return FLOAT;
		}
		
"void"	{	
			fprintf(logout,"Line# %d: Token <VOID> Lexeme %s found\n",line_count,yytext);
			return VOID;
		}

"return"	{	
			fprintf(logout,"Line# %d: Token <RETURN> Lexeme %s found\n",line_count,yytext);
			return RETURN;
		}

"println"	{
			fprintf(logout,"Line# %d: Token <PRINTLN> Lexeme %s found\n",line_count,yytext);
			return PRINTLN;
		}

{CONST_INTEGER} {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "CONST_INT");
			yylval = (YYSTYPE)s;	
			fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",line_count,yytext);
			return CONST_INT;
		}

{CONST_FLOAT} {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "CONST_FLOAT");
			yylval = (YYSTYPE)s; 	
			fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",line_count,yytext);
			return CONST_FLOAT;
		}

{TOO_MANY_POINTS} {	
			err_count++;
			fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",line_count,yytext);
		}

{ILLFORMED_NUMBER} {	
			err_count++;
			fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",line_count,yytext);
		}

"++" {	
			fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
			return INCOP;
		}

"--"  {	
			fprintf(logout,"Line# %d: Token <DECOP> Lexeme %s found\n",line_count,yytext);
			return DECOP;
}

[+-]	{
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "ADDOP");
			yylval = (YYSTYPE)s;	
			fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",line_count,yytext);
			return ADDOP;
		}

[*/%]	{	
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "MULOP");
			yylval = (YYSTYPE)s;
			fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",line_count,yytext);
			return MULOP;
		}

"&&"|"||" {	
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "LOGICOP");
			yylval = (YYSTYPE)s;
			fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",line_count,yytext);
			return LOGICOP;
		}

[<!>=][=]|[<>] {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "RELOP");
			yylval = (YYSTYPE)s;	
			fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",line_count,yytext);
			return RELOP;
		}		

"="	{	
			fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",line_count,yytext);
			return ASSIGNOP;
		}			

"!" {	
			fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",line_count,yytext);
			return NOT;
		}			

[(]	{	
			fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",line_count,yytext);
			return LPAREN;
		}			

[)]	{	
			fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",line_count,yytext);
			return RPAREN;
		}

[\{]	{
			table->EnterScope();
			fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",line_count,yytext);
			return LCURL;
		
	}			

[\}]	{
			table->ExitScope();
			fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",line_count,yytext);
			return RCURL;
		}

[[]	{	
			fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",line_count,yytext);
			return LTHIRD;
		}			

[]]	{	
			fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",line_count,yytext);
			return RTHIRD;
		}

[,]	{	
			fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",line_count,yytext);
			return COMMA;
		}			

[;]	{	
			fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",line_count,yytext);
			return SEMICOLON;
		}
{IDENTIFIER} {
			string str(yytext);
			SymbolInfo *s= table->Lookup(str);
			if(s==NULL)
			{					
				s = new SymbolInfo(str,"ID");
			}	
			fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",line_count,yytext);
			yylval = (YYSTYPE)s;
			return ID;
}
{INVALID_ID} {	
			err_count++;
			fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",line_count,yytext);
		}


. {
			err_count++;
			fprintf(logout,"Error at line# %d: UNRECOGNIZED_CHAR %s\n",line_count,yytext);
}
%%

