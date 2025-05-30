%option noyywrap

%{
#include "2005033.hpp"
int line_count= 1;
int err_count = 0;
int warning_count = 0;
int requiredTab = 0;
int tabCount = 0;
int spaceCount = 0;
int comment_start = 1;
int string_start = 1;
bool lineStarted = true;
string commentText;
string stringText;

FILE *logout;
FILE *tokenout;
SymbolTable *symboltable;

void checkIndentation() {
    if (spaceCount > 0) {
        fprintf(logout, "Line# %d: Warning, tab required but got space.\n", line_count);
		warning_count++;
    } else if (tabCount != requiredTab) {
        fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n", line_count, requiredTab, tabCount);
		warning_count++;
    }
    tabCount = spaceCount = 0;
}

char getEscapeChar(char escapeSeq) {
    switch (escapeSeq) {
        case 'n': return '\n';
        case 't': return '\t';
		case 'a': return '\a';
		case 'f': return '\f';
        case 'r': return '\r';
		case 'b': return '\b';
		case 'v': return '\v';
		case '0': return '\0';
		 
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
	string str1 = "";
	for(int i=0; i<str.length(); i++){
		if(str[i]!='\n' and str[i]!='\r')
			str1+=str[i];
	}
	string s = replaceEscapeSequences(str1);
	return s.substr(1,s.length()-2);
}

string truncateNewline(string str){
	string s = str;
	if(str[str.length()-1]=='\n'){
	 	return str.substr(0,str.length() - 2);
	}	
	return str; 
}

void startLine(){
	lineStarted = true; spaceCount = tabCount = 0;
}
%}

WHITESPACE [ \t\f\r\v]* 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \r?\n
UNSPECIFIED [^a-zA-Z0-9\r\n \t\f\r\v\'\"]
IDENTIFIER (_|{LETTER})({LETTER}|_|{DIGIT})*
CONST_INTEGER {DIGIT}+
CONST_FLOAT (({DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+))|({DIGIT}*(\.{DIGIT}+)(E[+-]?{DIGIT}+)?))
TOO_MANY_POINTS {DIGIT}*\.({DIGIT}*\.{DIGIT}*)+({DIGIT}+E[+-]?{DIGIT}+)?
ILLFORMED_NUMBER (({DIGIT}+(\.{DIGIT}+)?(E[+-]?({DIGIT}*\.{DIGIT}*)))|((\.{DIGIT}+)(E[+-]?({DIGIT}*\.{DIGIT}*))))
EMPTY_CONST_CHAR "''"
CONST_CHAR \'{CHARACTER}\'
UNFINISHED_CONST_CHAR \'({CHARACTER}|\\\'|\r?\n)
MULTICHAR_CONST_CHAR \'{CHARACTER}{CHARACTER}+\'
CHARACTER [^\'\"\\\f\r\v\r\n]|\\[\"\'\\ntafrbv0]
INVALID_ID ({CONST_FLOAT}|{CONST_INTEGER}){IDENTIFIER}


%x IN_COMMENT IN_STRING IN_SINGLE_COMMENT

%%

{NEWLINE} {line_count++; startLine();}

{WHITESPACE} {
	if(lineStarted){
		for(int i=0; i<strlen(yytext); i++){
			if(yytext[i]=='\t')
				tabCount++;
			else if(yytext[i]==' ')
				spaceCount++;
		}
	}
}
"if"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}			
			fprintf(tokenout,"<IF, if>\n");
			fprintf(logout,"Line# %d: Token <IF> Lexeme %s found\n",line_count,yytext);
		}
		
"else"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<ELSE, else>\n");
			fprintf(logout,"Line# %d: Token <ELSE> Lexeme %s found\n",line_count,yytext);
		}

"for"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<FOR, for>\n");
			fprintf(logout,"Line# %d: Token <FOR> Lexeme %s found\n",line_count,yytext);
		}

"while"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<WHILE, while>\n");
			fprintf(logout,"Line# %d: Token <WHILE> Lexeme %s found\n",line_count,yytext);
		}
		
"do"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<DO, do>\n");
			fprintf(logout,"Line# %d: Token <DO> Lexeme %s found\n",line_count,yytext);
		}

"break"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<BREAK, break>\n");
			fprintf(logout,"Line# %d: Token <BREAK> Lexeme %s found\n",line_count,yytext);
		}	

"int"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<INT, int>\n");
			fprintf(logout,"Line# %d: Token <INT> Lexeme %s found\n",line_count,yytext);
		}
		
"char"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<CHAR, char>\n");
			fprintf(logout,"Line# %d: Token <CHAR> Lexeme %s found\n",line_count,yytext);
		}

"float"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<FLOAT, float>\n");
			fprintf(logout,"Line# %d: Token <FLOAT> Lexeme %s found\n",line_count,yytext);
		}
			
"double"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<DOUBLE, double>\n");
			fprintf(logout,"Line# %d: Token <DOUBLE> Lexeme %s found\n",line_count,yytext);
		}
		
"void"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<VOID, void>\n");
			fprintf(logout,"Line# %d: Token <VOID> Lexeme %s found\n",line_count,yytext);
		}

"return"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<RETURN, return>\n");
			fprintf(logout,"Line# %d: Token <RETURN> Lexeme %s found\n",line_count,yytext);
		}

"switch"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<SWITCH, switch>\n");
			fprintf(logout,"Line# %d: Token <SWITCH> Lexeme %s found\n",line_count,yytext);
		}

"case"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<CASE, case>\n");
			fprintf(logout,"Line# %d: Token <CASE> Lexeme %s found\n",line_count,yytext);
		}

"default"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<DEFAULT, default>\n");
			fprintf(logout,"Line# %d: Token <DEFAULT> Lexeme %s found\n",line_count,yytext);
		}

"continue" {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<CONTINUE, continue>\n");
			fprintf(logout,"Line# %d: Token <CONTINUE> Lexeme %s found\n",line_count,yytext);
		}

{CONST_INTEGER} { 
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<CONST_INT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",line_count,yytext);
		}

{CONST_FLOAT} { 
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<CONST_FLOAT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",line_count,yytext);
		}

{TOO_MANY_POINTS} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",line_count,yytext);
		}

{ILLFORMED_NUMBER} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",line_count,yytext);
		}


{CONST_CHAR} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			string str(yytext);
			fprintf(tokenout,"<CONST_CHAR, %s>\n",processString(str).c_str());
			fprintf(logout,"Line# %d: Token <CONST_CHAR> Lexeme %s found\n",line_count,processString(str).c_str());
		}

{EMPTY_CONST_CHAR} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			fprintf(logout,"Error at line# %d: EMPTY_CONST_CHAR %s\n",line_count,yytext);
		}

{UNFINISHED_CONST_CHAR} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			string str(yytext);
			fprintf(logout,"Error at line# %d: UNFINISHED_CONST_CHAR %s\n",line_count,truncateNewline(str).c_str());
			if(str[str.length() - 1] == '\n'){ 
				line_count++;
				startLine();
			}
		}

{MULTICHAR_CONST_CHAR} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			fprintf(logout,"Error at line# %d: MULTICHAR_CONST_CHAR %s\n",line_count,yytext);
		}


"++"|"--" {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<INCOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
		}

[+-]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<ADDOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",line_count,yytext);
		}

[*\/%]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<MULOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",line_count,yytext);
		}

"&&"|"||" {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<LOGICOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",line_count,yytext);
		}

[&|^]|"<<"|">>"	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<BITOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <BITOP> Lexeme %s found\n",line_count,yytext);
		}

[<!>=][=]|[<>] {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<RELOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",line_count,yytext);
		}		

"="	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<ASSIGNOP, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",line_count,yytext);
		}			

"!" {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<NOT, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",line_count,yytext);
		}			

[(]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<LPAREN, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",line_count,yytext);
		}			

[)]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<RPAREN, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",line_count,yytext);
		}

[\{]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}
			requiredTab++;	
			symboltable->EnterScope();
			fprintf(tokenout,"<LCURL, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",line_count,yytext);
		
	}			

[\}]	{
			if(requiredTab>0) requiredTab--;
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			symboltable->ExitScope();
			fprintf(tokenout,"<RCURL, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",line_count,yytext);
		}

[[]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<LSQUARE, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",line_count,yytext);
		}			

[]]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<RSQUARE, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",line_count,yytext);
		}

[,]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<COMMA, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",line_count,yytext);
		}			

[;]	{
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<SEMICOLON, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",line_count,yytext);
		}
{IDENTIFIER} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			fprintf(tokenout,"<ID, %s>\n",yytext);
			fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",line_count,yytext);
			string s(yytext);
			if(symboltable->Insert(s, "ID"))
				symboltable->printAll(logout);
			else
				fprintf(logout,"\t%s already exists in the current ScopeTable\n",yytext);	
		}
{INVALID_ID} {
			if(lineStarted){
				checkIndentation();
				lineStarted = false;
			}	
			err_count++;
			fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",line_count,yytext);
		}

\" {
	if(lineStarted){
		checkIndentation();
		lineStarted = false;
	}	
	string_start = line_count;
	stringText = "";
	string temp(yytext);
    stringText+=temp;
    BEGIN(IN_STRING);
} 
<IN_STRING>{
	[ \t\f\r\v]* {
		if(lineStarted){
			for(int i=0; i<strlen(yytext); i++){
				if(yytext[i]=='\t')
					tabCount++;
				else if(yytext[i]==' ')
					spaceCount++;
			}
		}
		string temp(yytext);
		stringText+=temp;
	}
	[^\\\r\n\"]|(\\[ \"\'\\ntafrbv0]) {
		if(lineStarted){
			checkIndentation();
			lineStarted = false;
		}		
		string temp(yytext);
		stringText+=temp;
	}
	\\(\r?\n) {
		if(lineStarted){
			checkIndentation();
			lineStarted = false;
		}	
		string temp(yytext);
		stringText+=temp;
		line_count++;
		startLine();
	}
	[^\\\"](\r?\n) {
		if(lineStarted){
			checkIndentation();
			lineStarted = false;
		}
		
		string temp(yytext);
        stringText+=temp;
		string str(stringText);
		fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,truncateNewline(str).c_str());
		err_count++;
		line_count++;
		lineStarted = true;
		BEGIN(INITIAL);
	}
	<<EOF>> {
		string temp(yytext);
        stringText+=temp;
		err_count++;
		fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,stringText.c_str());
		BEGIN(INITIAL);
	}
	\\\; {
		string temp(yytext);
        stringText+=temp;
		err_count++;
		fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,stringText.c_str());
		BEGIN(INITIAL);
	}
	[^\\]?\" {
		if(lineStarted){
			checkIndentation();
			lineStarted = false;
		}	
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
		startLine();
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
 
. {
		if(lineStarted){
			checkIndentation();
			lineStarted = false;
	}
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
