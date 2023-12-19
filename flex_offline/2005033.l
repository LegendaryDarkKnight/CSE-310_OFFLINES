%option noyywrap

%{
#include "2005033_SymbolTable.hpp"
#include <algorithm>
int line_count= 1;
int err_count = 0;
int warning_count = 0;
int comment_start = 1;
int string_start = 1;
string commentText;
string stringText;

FILE *logout;
FILE *tokenout;
SymbolTable *symboltable;

char getEscapeChar(char escapeSeq) {
    switch (escapeSeq) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        // Add more escape sequences as needed
        default: return escapeSeq; // Treat unknown escape sequences as the literal character
    }
}

string replaceEscapeSequences(string input) {
	string result = "";
    bool escape = false;

    for (char c : input) {
        if (escape) {
            result += getEscapeChar(c);
            escape = false;
        } else {
            if (c == '\\') {
                escape = true;
            } else {
                result += c;
            }
        }
    }

    return result;
}

string processString(string str){
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	string s = replaceEscapeSequences(str);
	return s.substr(1,s.length()-2);
}

string truncateNewline(string str){
	string s = str;
	if(str[str.length()-1]=='\n'){
	 	return str.substr(0,str.length() - 2);
	}	
	return str; 
}

%}

WHITESPACE [ \t\f\r\v]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \r?\n
UNSPECIFIED [^a-zA-Z0-9\r\n \t\f\r\v\'\"]
CONST_INTEGER {DIGIT}+
CONST_FLOAT {DIGIT}*(\.{DIGIT}+)?(E[+-]?{DIGIT}+)?
TOO_MANY_POINTS {DIGIT}*\.({DIGIT}*\.{DIGIT}*)+(E[+-]?{DIGIT}+)?
ILLFORMED_NUMBER {DIGIT}*(\.{DIGIT}*)?E[+-]?({DIGIT}*\.{DIGIT}*)?
INVALID_ID {DIGIT}+{LETTER}+
CHARACTER [^\'\"\\\t\f\r\v\r\n]|\\[^ \t\f\r\v\r\n]
EMPTY_CONST_CHAR "''"
CONST_CHAR \'{CHARACTER}\'
UNFINISHED_CONST_CHAR \'({CHARACTER}|\\\'|\r?\n)
MULTICHAR_CONST_CHAR \'{CHARACTER}{CHARACTER}+\'

SINGLE_LINE_STRING \"([^\\\r\n\"]*|\\[^ \t\f\r\v\r\n])*\"
MULTI_LINE_STRING_PREFIX \"([^\\\r\n\"]*|\\([^ \t\f\r\v]+|\r?\n))*
SINGLE_COMMENT1 \/\/([^\\\r\n\"]*|\\([^ \t\f\r\v]+|\r?\n))*(\r?\n)
SINGLE_COMMENT2 \/\/[^\r\n]*(\r?\n)
MULTI_COMMENT \/[*]([*]*)?([^*]*[*]+[^*/])*[^*]*[*]+\/

%x IN_COMMENT IN_STRING IN_SINGLE_COMMENT

%%

{NEWLINE} {line_count++;}

{WHITESPACE} {

}
"if"	{
			fprintf(tokenout,"<IF, if>\n");
			fprintf(logout,"Line# %d: Token <IF> Lexeme %s found\n",line_count,yytext);
		}
		
"else"	{
			fprintf(tokenout,"<ELSE, else>\n");
			fprintf(logout,"Line# %d: Token <ELSE> Lexeme %s found\n",line_count,yytext);
		}

"for"	{
			fprintf(tokenout,"<FOR, for>\n");
			fprintf(logout,"Line# %d: Token <FOR> Lexeme %s found\n",line_count,yytext);
		}

"while"	{
			fprintf(tokenout,"<WHILE, while>\n");
			fprintf(logout,"Line# %d: Token <WHILE> Lexeme %s found\n",line_count,yytext);
		}
		
"do"	{
			fprintf(tokenout,"<DO, do>\n");
			fprintf(logout,"Line# %d: Token <DO> Lexeme %s found\n",line_count,yytext);
		}

"break"	{
			fprintf(tokenout,"<BREAK, break>\n");
			fprintf(logout,"Line# %d: Token <BREAK> Lexeme %s found\n",line_count,yytext);
		}	

"int"	{
			fprintf(tokenout,"<INT, int>\n");
			fprintf(logout,"Line# %d: Token <INT> Lexeme %s found\n",line_count,yytext);
		}
		
"char"	{
			fprintf(tokenout,"<CHAR, char>\n");
			fprintf(logout,"Line# %d: Token <CHAR> Lexeme %s found\n",line_count,yytext);
		}

"float"	{
			fprintf(tokenout,"<FLOAT, float>\n");
			fprintf(logout,"Line# %d: Token <FLOAT> Lexeme %s found\n",line_count,yytext);
		}
			
"double"	{
			fprintf(tokenout,"<DOUBLE, double>\n");
			fprintf(logout,"Line# %d: Token <DOUBLE> Lexeme %s found\n",line_count,yytext);
		}
		
"void"	{
			fprintf(tokenout,"<VOID, void>\n");
			fprintf(logout,"Line# %d: Token <VOID> Lexeme %s found\n",line_count,yytext);
		}

"return"	{
			fprintf(tokenout,"<RETURN, return>\n");
			fprintf(logout,"Line# %d: Token <RETURN> Lexeme %s found\n",line_count,yytext);
		}

"switch"	{
			fprintf(tokenout,"<SWITCH, switch>\n");
			fprintf(logout,"Line# %d: Token <SWITCH> Lexeme %s found\n",line_count,yytext);
		}

"case"	{
			fprintf(tokenout,"<CASE, case>\n");
			fprintf(logout,"Line# %d: Token <CASE> Lexeme %s found\n",line_count,yytext);
		}

"default"	{
			fprintf(tokenout,"<DEFAULT, default>\n");
			fprintf(logout,"Line# %d: Token <DEFAULT> Lexeme %s found\n",line_count,yytext);
		}

"continue" {
			fprintf(tokenout,"<CONTINUE, continue>\n");
			fprintf(logout,"Line# %d: Token <CONTINUE> Lexeme %s found\n",line_count,yytext);
		}

{CONST_INTEGER} { 
			fprintf(tokenout,"<CONST_INT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",line_count,yytext);
		}

{CONST_FLOAT} { 
			fprintf(tokenout,"<CONST_FLOAT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",line_count,yytext);
		}

{TOO_MANY_POINTS} {
			err_count++;
			fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",line_count,yytext);
		}

{ILLFORMED_NUMBER} {
			err_count++;
			fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",line_count,yytext);
		}

{INVALID_ID} {
			err_count++;
			fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",line_count,yytext);
		}

{CONST_CHAR} {
			string str(yytext);
			fprintf(tokenout,"<CONST_CHAR, %s>\n",processString(str).c_str());
			fprintf(logout,"Line# %d: Token <CONST_CHAR> Lexeme %s found\n",line_count,processString(str).c_str());
		}

{EMPTY_CONST_CHAR} {
			err_count++;
			fprintf(logout,"Error at line# %d: EMPTY_CONST_CHAR %s\n",line_count,yytext);
		}

{UNFINISHED_CONST_CHAR} {
			err_count++;
			string str(yytext);
			fprintf(logout,"Error at line# %d: UNFINISHED_CONST_CHAR %s\n",line_count,truncateNewline(str).c_str());
			if(str[str.length() - 1] == '\n') line_count++; 
		}

{MULTICHAR_CONST_CHAR} {
			err_count++;
			fprintf(logout,"Error at line# %d: MULTICHAR_CONST_CHAR %s\n",line_count,yytext);
		}


"++"|"--" {
			fprintf(tokenout,"<INCOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
		}

[+-]	{
			fprintf(tokenout,"<ADDOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",line_count,yytext);
		}

[*\/%]	{
			fprintf(tokenout,"<MULOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",line_count,yytext);
		}

"&&"|"||" {
			fprintf(tokenout,"<LOGICOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",line_count,yytext);
		}

[&|^]|"<<"|">>"	{
			fprintf(tokenout,"<BITOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <BITOP> Lexeme %s found\n",line_count,yytext);
		}

[<!>=][=]|[<>] {
			fprintf(tokenout,"<RELOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",line_count,yytext);
		}		

"="	{
			fprintf(tokenout,"<ASSIGNOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",line_count,yytext);
		}			

"!" {
			fprintf(tokenout,"<NOT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",line_count,yytext);
		}			

[(]	{
			fprintf(tokenout,"<LPAREN, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",line_count,yytext);
		}			

[)]	{
			fprintf(tokenout,"<RPAREN, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",line_count,yytext);
		}

[\{]	{
			symboltable->EnterScope();
			fprintf(tokenout,"<LCURL, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",line_count,yytext);
		
	}			

[\}]	{
			symboltable->ExitScope();
			fprintf(tokenout,"<RCURL, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",line_count,yytext);
		}

[[]	{
			fprintf(tokenout,"<LSQUARE, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",line_count,yytext);
		}			

[]]	{
			fprintf(tokenout,"<RSQUARE, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",line_count,yytext);
		}

[,]	{
			fprintf(tokenout,"<COMMA, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",line_count,yytext);
		}			

[;]	{
			fprintf(tokenout,"<SEMICOLON, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",line_count,yytext);
		}

(_|{LETTER})({LETTER}|_|{DIGIT})* {
			fprintf(tokenout,"<ID, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",line_count,yytext);
			string s(yytext);
			if(symboltable->Insert(s, "ID"))
				symboltable->printAll(logout);
			else
				fprintf(logout,"\t%s already exists in the current ScopeTable\n",yytext);	
		}
\" {
	string_start = line_count;
	stringText = "";
	string temp(yytext);
    stringText+=temp;
    BEGIN(IN_STRING);
} 
<IN_STRING>{
	[^\\]["] {
		string temp(yytext);
        stringText+=temp;
		if(string_start == line_count){
			fprintf(tokenout,"<SINGLE LINE STRING, %s>\n", processString(stringText).c_str());
			fprintf(logout,"Line# %d: Token <SINGLE LINE STRING> Lexeme %s found\n",string_start,stringText.c_str());
		}
		else{
			fprintf(tokenout,"<MULTI LINE STRING, %s>\n", processString(stringText).c_str());
			fprintf(logout,"Line# %d: Token <MULTI LINE STRING> Lexeme %s found\n",string_start,stringText.c_str());
		}
        BEGIN(INITIAL);
	}
	[^\r\n"] {
		string temp(yytext);
		stringText+=temp;
	}
	\\(\r?\n) {
		string temp(yytext);
		stringText+=temp;
		line_count++;
	}
	[^\\](\r?\n) {
		string temp(yytext);
        stringText+=temp;
		string str(stringText);
		fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,truncateNewline(str).c_str());
		err_count++;
		line_count++;
		BEGIN(INITIAL);
	}
	<<EOF>> {
		string temp(yytext);
        stringText+=temp;
		err_count++;
		fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,stringText.c_str());
		BEGIN(INITIAL);
	}

}

"/*"    {
	comment_start = line_count;
	commentText = "";
	string temp(yytext);
    commentText+=temp;
    BEGIN(IN_COMMENT);
}

<IN_COMMENT>{
    "*/" {
		string temp(yytext);
        commentText+=temp;
		fprintf(logout,"Line# %d: Token <MULTI LINE COMMENT> Lexeme %s found\n",comment_start,commentText.c_str());
		BEGIN(INITIAL);
    }
    \r?\n {
		string temp(yytext);
        commentText+=temp;
        line_count++;
    }
    .  {
		string temp(yytext);
		commentText+=temp;
    }
	<<EOF>> {
		string temp(yytext);
        commentText+=temp;
		err_count++;
		fprintf(logout,"Error at line# %d: UNFINISHED_COMMENT %s\n",line_count,commentText.c_str());
        BEGIN(INITIAL);
	}
}

\/\/ {
	comment_start = line_count;
	commentText = "";
	string temp(yytext);
    commentText+=temp;
    BEGIN(IN_SINGLE_COMMENT);
}

<IN_SINGLE_COMMENT>{
    [^\\](\r?\n) {
		string temp(yytext);
        commentText+=temp;
		string str(commentText);
		fprintf(logout,"Line# %d: Token <SINGLE LINE COMMENT> Lexeme %s found\n",comment_start,truncateNewline(str).c_str());
		line_count++;
		BEGIN(INITIAL);
    }
	\\(\r?\n) {
		string temp(yytext);
		commentText+=temp;
		line_count++;
	}
    .  {
		string temp(yytext);
		commentText+=temp;
    }
	<<EOF>> {
		string temp(yytext);
        commentText+=temp;
		fprintf(logout,"Line# %d: Token <SINGLE LINE COMMENT> Lexeme %s found\n",comment_start,commentText.c_str());
        BEGIN(INITIAL);
	}
}
 
{UNSPECIFIED} {
			err_count++;
			fprintf(logout,"Error at line# %d: UNRECOGNIZED_CHAR %s\n",line_count,yytext);
		}
%%

int main(int argc,char *argv[]){
	
	 if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	} 
	
	logout= fopen("2005033_log.txt","w");
	tokenout= fopen("2005033_token.txt","w");
	symboltable = new SymbolTable(10);

	yyin= fin;
	yylex();
	symboltable->printAll(logout);
	fprintf(logout,"Total lines: %d\nTotal errors: %d\nTotal warnings: %d\n", line_count, err_count, warning_count);
	delete symboltable;
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	
	return 0;
}
