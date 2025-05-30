%option noyywrap

%{
#include "2005033.hpp"
#include "y.tab.h"
int line_count= 1;
int err_count = 0;
int start_line = 1;
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
			// fprintf(logout,"Line# %d: Token <IF> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("if","IF",nullptr,line_count);	
			return IF;
		}
		
"else"	{	
			// fprintf(logout,"Line# %d: Token <ELSE> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("else","ELSE",nullptr,line_count);
			return ELSE;
		}

"for"	{	
			// fprintf(logout,"Line# %d: Token <FOR> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("for","FOR",nullptr,line_count);	
			 return FOR;
		}

"while"	{	
			// fprintf(logout,"Line# %d: Token <WHILE> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("while","WHILE",nullptr,line_count);	
			return WHILE;
		}

"int"	{	
			// fprintf(logout,"Line# %d: Token <INT> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("int","INT",nullptr,line_count);	
			 return INT;
		}

"float"	{	
			// fprintf(logout,"Line# %d: Token <FLOAT> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("float","FLOAT",nullptr,line_count);	
			return FLOAT;
		}
		
"void"	{	
			// fprintf(logout,"Line# %d: Token <VOID> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("void","VOID",nullptr,line_count);	
			return VOID;
		}

"return"	{	
			// fprintf(logout,"Line# %d: Token <RETURN> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("return","RETURN",nullptr,line_count);	
			return RETURN;
		}

"println"	{
			// fprintf(logout,"Line# %d: Token <PRINTLN> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("println","PRINTLN",nullptr,line_count);	
			return PRINTLN;
		}

{CONST_INTEGER} {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "CONST_INT", nullptr, line_count);
			yylval.symbolInfo = s;	
			// fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",line_count,yytext);
			 return CONST_INT;
		}

{CONST_FLOAT} {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "CONST_FLOAT", nullptr, line_count);
			yylval.symbolInfo = s; 	
			// fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",line_count,yytext);
			 return CONST_FLOAT;
		}

{TOO_MANY_POINTS} {	
			err_count++;
			// fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",line_count,yytext);

		}

{ILLFORMED_NUMBER} {	
			err_count++;
			// fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",line_count,yytext);

		}

"++" {	
			// fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("++","INCOP",nullptr,line_count);	
			return INCOP;
		}

"--"  {	
			// fprintf(logout,"Line# %d: Token <DECOP> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("--","DECOP",nullptr,line_count);	
			 return DECOP;
}

[+-]	{
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "ADDOP", nullptr, line_count);
			yylval.symbolInfo = s;	
			// fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",line_count,yytext);
			 return ADDOP;
		}

[*/%]	{	
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "MULOP", nullptr, line_count);
			yylval.symbolInfo = s;
			// fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",line_count,yytext);
			return MULOP;
		}

"&&"|"||" {	
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "LOGICOP", nullptr, line_count);
			yylval.symbolInfo = s;
			// fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",line_count,yytext);
			return LOGICOP;
		}

[<!>=][=]|[<>] {
			string str(yytext);
			SymbolInfo *s= new  SymbolInfo(str, "RELOP", nullptr, line_count);
			yylval.symbolInfo = s;	
			// fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",line_count,yytext);
			return RELOP;
		}		

"="	{	
			// fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("=","ASSIGNOP",nullptr,line_count);	
			return ASSIGNOP;
		}			

"!" {	
			// fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("!","NOT",nullptr,line_count);	
			return NOT;
		}			

[(]	{	
			// fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("(","LPAREN",nullptr,line_count);	
			return LPAREN;
		}			

[)]	{	
			// fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo(")","RPAREN",nullptr,line_count);	
			return RPAREN;
		}

[\{]	{
			table->EnterScope();
			// fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("{","LCURL",nullptr,line_count);	
			return LCURL;	
	}			

[\}]	{
			// fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("}","RCURL",nullptr,line_count);	
			return RCURL;
		}

[[]	{	
			// fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("[","LSQUARE",nullptr,line_count);	
			return LTHIRD;
		}			

[]]	{	
			// fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo("]","RSQUARE",nullptr,line_count);	
			return RTHIRD;
		}

[,]	{	
			// fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo(",","COMMA",nullptr,line_count);	
			return COMMA;
		}			

[;]	{	
			// fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = new SymbolInfo(";","SEMICOLON",nullptr,line_count);	
			return SEMICOLON;
		}
{IDENTIFIER} {
			string str(yytext);
			SymbolInfo *s= table->Lookup(str);
			if(s==NULL)
			{					
				s = new SymbolInfo(str,"ID",nullptr,line_count);
			}
			s->setLine(line_count);	
			// fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",line_count,yytext);
			yylval.symbolInfo = s;
			return ID;
}
{INVALID_ID} {	
			err_count++;
			// fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",line_count,yytext);

		}
. {
			err_count++;
			// fprintf(logout,"Error at line# %d: UNRECOGNIZED_CHAR %s\n",line_count,yytext);

}
%%

