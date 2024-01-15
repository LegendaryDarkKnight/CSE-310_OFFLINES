%option noyywrap

%{
    #include "2005033.hpp"
    #define YYSTYPE SymbolInfo*
    extern SymbolTable *table;
extern YYSTYPE yylval;
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
#include "y.tab.h"
int linecount = 1;
int totalError = 0;
int totalWarning = 0;
int scopeCount = 0;
int lineStart = 1;
int messedTab = 0;
extern FILE *logout;
// FILE *tokenout;
string comment;

%}

Exp [E]
Point [.]
Digit [0-9]
Apostrophe [\']
Underscore [_]
Letter [a-zA-Z]
Digits {Digit}+
Digit_s {Digit}*
Plus_Minus_ [+-]?
AlphaNumeric {Letter}|{Digit}
Exp_Term {Exp}{Plus_Minus_}{Digits}
KeyWord if|for|do|int|float|void|switch|default|else|while|break|char|double|return|case|continue
Identifier ({Letter}|{Underscore})({AlphaNumeric}|{Underscore})*
Integer {Digits}
Float ({Digit_s}{Point}{Digits}{Exp_Term}?)|({Digits}{Exp_Term})

WHITESPACE [ \t\f\r\v]* 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \r?\n
IDENTIFIER (_|{LETTER})({LETTER}|_|{DIGIT})*
CONST_INTEGER {DIGIT}+
CONST_FLOAT (({DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+))|({DIGIT}*(\.{DIGIT}+)(E[+-]?{DIGIT}+)?))


%%

[\r]?[\n]  {
                    linecount++;
                    lineStart=1;
                
            }

{KeyWord}   {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                fprintf(logout,"Line# %d: Token <%s> Lexeme %s found\n",linecount,yytext,s.c_str());
                lineStart = 0;
                if (yytext=="if") return IF;
                else if (yytext=="for") return FOR;
                else if (yytext=="int") return INT;
                else if (yytext=="float") return FLOAT;
                else if (yytext=="void") return VOID;
                else if (yytext=="else") return ELSE;
                else if (yytext=="while") return WHILE;
                else if (yytext=="return") return RETURN;
            }
{Integer}   {
                fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return CONST_INT;
            }
{Float}     {
                fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return CONST_FLOAT;
            }
[+-]        {
                fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return ADDOP;
            }
[*/%]       {
                fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return MULOP;
            }
"++"|"--"        {
                fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return INCOP;
            }
"<"|"<="|">"|">="|"=="|"!=" {
                fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return RELOP;
            }
"="         {
                fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return ASSIGNOP;
            }
"&&"|"||"   {
                fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return LOGICOP;
            }
"!"         {
                fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return NOT;
            }
"\("         {
                fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return LPAREN;
            }
"\)"         {
                fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return RPAREN;
            }
"\{"         {
                fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",linecount,yytext);
                scopeCount++;
                lineStart = 0;
                return LCURL;
            }
[}]         {
                scopeCount--;
                fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return RCURL;
            }
"\["         {
                fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return LTHIRD;
            }
"\]"         {
                fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return RTHIRD;
            }
[,]	{	
			fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",linecount,yytext);
			return COMMA;
		}
";"         {
                fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
                return SEMICOLON;
            }
{IDENTIFIER} {
			string str(yytext);
			SymbolInfo *s= table->Lookup(str);
			if(s==NULL)
			{					
				s = new SymbolInfo(str,"ID");
			}	
			fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",linecount,yytext);
			yylval = (YYSTYPE)s;
			return ID;
}
[ \t]*     {lineStart = 0;}
[\t ]*[\r]?[\n]        {
                linecount++;
                lineStart = 1;
            }
. {
    fprintf(logout,"Line# %d:sdsdsd",linecount,yytext);
}


%%
