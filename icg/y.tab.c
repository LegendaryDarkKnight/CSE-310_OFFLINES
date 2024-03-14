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

#line 1 "2005033.y"

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
extern char *yytext;
bool infunc = false;
bool sayArray = false;
MyStack1 stack1, stack2;
bool stillValid = true;
int validCount = 0;
bool voidFound = false;
vector<string> typeList;
int value = 1;
FILE *fp;
FILE *logout;
FILE *errorout;
FILE *parseout;
FILE *codeout;
FILE *optcodeout;

int offset = 0;
bool outOffset = true;
int tempOffset1 = 0;
int tempOffset2 = 0;
int labelCount = 0;

SymbolTable *table;
SymbolInfo *temp;
string type;

string processExpression(ParseTree *p);
string processLogicExpression(ParseTree *p);
string processSimpleExpression(ParseTree *p);
string processVariableAssignExpression(ParseTree *p, bool doPush);
string extractIndexForArray(ParseTree *p);
string processStatements(ParseTree *p);

string newLabel(){
	labelCount++;
	return "L"+to_string(labelCount)+":\n";
}

string printFunction(){
	const char *assemblyCode = 
"new_line proc\n"
"    push ax\n"
"    push dx\n"
"    mov ah,2\n"
"    mov dl,0Dh\n"
"    int 21h\n"
"    mov ah,2\n"
"    mov dl,0Ah\n"
"    int 21h\n"
"    pop dx\n"
"    pop ax\n"
"    ret\n"
"new_line endp\n"
"\n"
"print_output proc  ;print what is in ax\n"
"    push ax\n"
"    push bx\n"
"    push cx\n"
"    push dx\n"
"    push si\n"
"    lea si,number\n"
"    mov bx,10\n"
"    add si,4\n"
"    cmp ax,0\n"
"    jnge negate\n"
"print:\n"
"    xor dx,dx\n"
"    div bx\n"
"    mov [si],dl\n"
"    add [si],'0'\n"
"    dec si\n"
"    cmp ax,0\n"
"    jne print\n"
"    inc si\n"
"    lea dx,si\n"
"    mov ah,9\n"
"    int 21h\n"
"    pop si\n"
"    pop dx\n"
"    pop cx\n"
"    pop bx\n"
"    pop ax\n"
"    ret\n"
"\n"
"negate:\n"
"    push ax\n"
"    mov ah,2\n"
"    mov dl,'-'\n"
"    int 21h\n"
"    pop ax\n"
"    neg ax\n"
"    jmp print\n"
"\n"
"print_output endp\n";
	string result(assemblyCode);
	return result;
}

void extractUnits(ParseTree *p, vector<ParseTree *> &v){
    if(p == nullptr) return;
    if(p->getType()=="unit") {v.push_back(p); return;}
    for(auto s: p->getChild()){
        extractUnits(s,v);
    }
}

string extractArrayVariables(ParseTree *p, int size){
	if(table->Insert(p->getType(), type)){
		SymbolInfo *s = table->Lookup(p->getType());
		if(outOffset){
			offset += 2*size;
			s->setOffset(offset);
		}
		else{
			tempOffset2 += 2*size;
			s->setOffset(tempOffset2);
		}
		return "\tSUB SP, " + to_string(2*size) +'\n';
	}
	return "";
}

string extractVariables(ParseTree *p){
	string result = "";
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "declaration_list : ID LSQUARE CONST_INT RSQUARE"){
		return extractArrayVariables(childs[0], stoi(childs[2]->getType().c_str()));
	} 
	else if(p->getLine() == "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE"){
		string temp = extractVariables(childs[0]) ;
		return temp+extractArrayVariables(childs[2], stoi(childs[4]->getType().c_str()));
	}
	else if(p->getLine().substr(0,2) == "ID"){
		if(table->Insert(p->getType(), type)){
			SymbolInfo *s = table->Lookup(p->getType());
			if(outOffset){
				offset += 2;
				s->setOffset(offset);
			}
			else{
				tempOffset2 += 2;
				s->setOffset(tempOffset2);
			}

			return "\tSUB SP, 2\n";
		}
	}
	for(auto v: childs)
		result += extractVariables(v);
	return result;
}

string processVarDeclaration(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	type = childs[0]->getType();
	return extractVariables(childs[1]);
}

string processVariable(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	SymbolInfo *temp = table->Lookup(childs[0]->getType());
	if(p->getLine() == "variable : ID"){
		if(temp->getOffset() == 0){ return "\tPUSH "+temp->getName()+"\t;Line "+to_string(p->getStartLine())+'\n';}
		else return "\tPUSH [BP - "+ to_string(temp->getOffset()) + "]"+"\t;Line "+to_string(p->getStartLine())+"\n";
	}
	else if(p->getLine() == "variable : ID LSQUARE expression RSQUARE"){
		result += processLogicExpression(childs[2]->getChild()[0]);
		if(temp->getOffset() == 0) {
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; /*the index is at AX*/
			result += "\tMOV DX, 2\n\tMUL DX\n"; /*the index*2 is the location of the word*/
			result += "\tLEA SI, "+temp->getName()+"\n\tADD SI, AX\n";
			result += "\tPUSH [SI]\n";	
		}
		else{
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n'; /*the index is at AX */
			result += "\tMOV DX, 2\n\tMUL DX\n"; /*the index*2 is the location of the word*/
			result += "\tMOV DX, "+to_string(temp->getOffset());
			result += "\n\tSUB DX, AX\n";
			result += "\tMOV SI, DX\n";
			result += "\tNEG SI\n\tPUSH [BP+SI]\n";	
		}
		return result;
	}
	return result;
}
string processArguments(ParseTree *p){
	string result = "";
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "arguments : arguments COMMA logic_expression"){
		result += processArguments(childs[0]);
		return processLogicExpression(childs[2]) + result;
	}
	else{
		return processLogicExpression(childs[0]);
	}
	return result;
}
string processArgumentList(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();

	if(p->getLine() == "argument_list : arguments"){
		return processArguments(childs[0]);
	}
	return "";
}
string processFactor(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "factor : variable")
		return processVariable(childs[0]);
	else if(p->getLine() == "factor : CONST_INT")
		return "\tPUSH "+childs[0]->getType()+'\n';
	else if(p->getLine() == "factor : LPAREN expression RPAREN"){
		return processLogicExpression(childs[1]->getChild()[0]);
	}
	else if(p->getLine() == "factor : variable INCOP"){
		result += processVariable(childs[0]);
		result += "\tPOP AX\n\tINC AX\t;Line "+to_string(p->getStartLine())+'\n';
		if(childs[0]->getLine() == "variable : ID"){
			result += "\tPUSH AX\n";
			result += processVariableAssignExpression(childs[0], false)+"\tDEC AX\n\tPUSH AX\n";
		}
		else{
			result += "\tMOV BX, AX\n";
			result += extractIndexForArray(childs[0]);
			result += "\tPUSH BX\n";
			result += processVariableAssignExpression(childs[0], false)+"\tDEC BX\n\tPUSH BX\n";
		}
		return result;
	}
	else if(p->getLine() == "factor : variable DECOP"){
		result += processVariable(childs[0]);
		result += "\tPOP AX\n\tDEC AX\t;Line "+to_string(p->getStartLine())+'\n';
		if(childs[0]->getLine() == "variable : ID"){
			result += "\tPUSH AX\n";
			result += processVariableAssignExpression(childs[0], false)+"\tINC AX\n\tPUSH AX\n";
		}
		else{
			result += "\tMOV BX, AX\n";
			result += extractIndexForArray(childs[0]);
			result += "\tPUSH BX\n";
			result += processVariableAssignExpression(childs[0], false)+"\tINC BX\n\tPUSH BX\n";
		}
		return result;
	}
	else if(p->getLine() == "factor	: ID LPAREN argument_list RPAREN"){
		result += processArgumentList(childs[2]);
		result += "\tCALL "+childs[0]->getType()+"\n\tPUSH AX"+"\t;Line "+to_string(p->getStartLine())+'\n';
		return result;
	}
	return result;
}

string processUnaryExpression(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "unary_expression : factor")
		return processFactor(childs[0]);
	else if(p->getLine() == "unary_expression : ADDOP unary_expression"){
		result += processUnaryExpression(childs[1]);
		result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n';
		if(childs[0]->getType()=="-") result+="\tNEG AX\n";
		result += "\tPUSH AX\n";
		return result;
	}
	else if(p->getLine() == "unary_expression : NOT unary_expression"){
		result += processUnaryExpression(childs[1]);
		result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n';
		result += "\tCMP AX, 0\n\tJE L"+to_string(labelCount+1)+'\n';
		result += "\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
		result += newLabel();
		result += "\tPUSH 1\n";
		result += newLabel();
		return result;
	}
	return "";
}

string processTerm(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "term : unary_expression")
		return processUnaryExpression(childs[0]);
	else if(p->getLine() == "term : term MULOP unary_expression"){
		result += processUnaryExpression(childs[2]);
		result += processTerm(childs[0]);
		result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n'+"\tPOP BX\n\tMOV DX, 0\n";
		if(childs[1]->getType()=="*"){
			result+="\tMUL BX\n\tPUSH AX\n";
		}
		if(childs[1]->getType()=="/"){
			result+="\tDIV BX\n\tPUSH AX\n";
		}
		if(childs[1]->getType()=="%"){
			result+="\tDIV BX\n\tPUSH DX\n";
		}
		return result;
	}
	return "";
}

string processSimpleExpression(ParseTree *p){ /* the ans should be at AX*/
	string result = "";
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "simple_expression : term")
		return processTerm(p->getChild()[0]); 
	else if(p->getLine() == "simple_expression : simple_expression ADDOP term"){
		result += processTerm(childs[2]);
		result += processSimpleExpression(childs[0]);
		result += "\tPOP AX\n\tPOP BX\t;Line "+to_string(p->getStartLine())+"\n";
		if(childs[1]->getType()=="+"){
			result+="\tADD AX, BX\n\tPUSH AX\n";
		}
		if(childs[1]->getType()=="-"){
			result+="\tSUB AX, BX\n\tPUSH AX\n";
		}
		return result;
	}
	return "";
}
string processRelop(ParseTree *p){ /* JUMP FOR TRUE*/
	if(p->getType() == "<") return "\tJL L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "<=") return "\tJLE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == ">") return "\tJG L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == ">=") return "\tJGE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "==") return "\tJE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "!=") return "\tJNE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else return "";
}
string processRelExpression(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "rel_expression : simple_expression"){
		return processSimpleExpression(p->getChild()[0]);
	}
	else if(p->getLine() == "rel_expression : simple_expression RELOP simple_expression"){
		result += processSimpleExpression(childs[2]);
		result += processSimpleExpression(childs[0]);
		result += "\tPOP AX\n\tPOP BX\n\tCMP AX, BX\t;Line "+to_string(p->getStartLine())+"\n"; /* AX has the value of child[0] and BX has the value of child[2]*/
		result += processRelop(childs[1]);
		result += newLabel();
		result += "\tPUSH 1\n";
		result += newLabel();
		return result;
	}
	return "";
}


string processLogicExpression(ParseTree *p){ /* the ans should be at AX*/
	vector<ParseTree *> childs = p->getChild();
	string result = "", temp = "";
	if(p->getLine() == "logic_expression : rel_expression")
		return processRelExpression(p->getChild()[0]);
	else if(p->getLine() == "logic_expression : rel_expression LOGICOP rel_expression"){
		if(childs[1]->getType() == "&&"){
			result += processRelExpression(childs[0]);
			temp = processRelExpression(childs[2]);
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; /* AX has the value of child[0]*/
			result += "\tCMP AX, 0\n";
			result += "\tJE L"+to_string(labelCount+2)+'\n';
			result += "\tPUSH 1\n";
			result += newLabel();
			result += temp;
			result += "\tPOP AX\n"; /* AX has the value of child[2]*/
			result += "\tPOP BX\n"; /* previously pushed 1*/
			result += "\tCMP AX, 0\n";
			result += "\tJG L"+to_string(labelCount+2)+'\n';
			result += newLabel();
			result += "\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
			result += newLabel();
			result += "\tPUSH 1\n";
			result += newLabel();
		}
		else{
			result += processRelExpression(childs[0]);
			temp = processRelExpression(childs[2]);
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; /* AX has the value of child[0]*/
			result += "\tCMP AX, 0\n";
			result += "\tJG L"+to_string(labelCount+2)+'\n';
			result += "\tPUSH 0\n";
			result += newLabel();
			result += temp;
			result += "\tPOP AX\n"; /* AX has the value of child[2]*/
			result += "\tPOP BX\n"; /* previously pushed 1*/
			result += "\tCMP AX, 0\n";
			result += "\tJE L"+to_string(labelCount+2)+'\n';
			result += newLabel();
			result += "\tPUSH 1\n\tJMP L"+to_string(labelCount+2)+'\n';
			result += newLabel();
			result += "\tPUSH 0\n";
			result += newLabel();
		}
		return result;
	}
	return "";
}

string extractIndexForArray(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine()  == "variable : ID LSQUARE expression RSQUARE"){
		result += processLogicExpression(childs[2]->getChild()[0]);
	}
	return result;
}

string processVariableAssignExpression(ParseTree *p, bool doPush){
	vector<ParseTree *> childs = p->getChild();
	SymbolInfo *temp = table->Lookup(childs[0]->getType());
	string result = "";
	if(p->getLine()  == "variable : ID"){
		result = "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n";
		if(temp->getOffset() == 0) return result+"\tMOV "+temp->getName()+", AX\n";
		else return result+"\tMOV [BP-"+to_string(temp->getOffset())+"], AX\n";
	}
	else{
		if(temp->getOffset() == 0) {
			result += "\tPOP BX\n\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; /*the index is at AX and the value is at BX*/
			result += "\tMOV DX, 2\n\tMUL DX\n"; /*the index*2 is the location of the word*/
			result += "\tLEA SI, "+temp->getName()+"\n\tADD SI, AX\n";
			result += "\tMOV [SI], BX\n";	
		}
		else{
			result += "\tPOP BX\n\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; /*the index is at AX and the value is at BX*/
			result += "\tMOV DX, 2\n\tMUL DX\n"; /*the index*2 is the location of the word*/
			result += "\tMOV DX, "+to_string(temp->getOffset());
			result += "\n\tSUB DX, AX\n";
			result += "\tMOV SI, DX\n";
			result += "\tNEG SI\n\tMOV [BP+SI], BX\n";	
		}
		if(doPush)
			result+= "\tPUSH BX\t;Line "+to_string(p->getStartLine())+"\n"; /* not that much sure*/
		return result;
	}
}

string processExpression(ParseTree *p){
	string result = "";
	vector<ParseTree *> childs = p->getChild();
	if(childs.size()<2) return "";
	ParseTree *temp = childs[0];
	if(temp->getLine() == "expression : variable ASSIGNOP logic_expression"){
		result += extractIndexForArray(temp->getChild()[0]);
		result += processLogicExpression(temp->getChild()[2]);
		result += processVariableAssignExpression(temp->getChild()[0], true);
		
	}
	else if(temp->getLine() == "expression : logic_expression"){
		result += processLogicExpression(temp->getChild()[0]);
		result += "\tPOP AX\n"; /* not that much sure */
	}
	return result;
}

string processPrint(ParseTree *p){
	SymbolInfo *temp = table->Lookup(p->getType());
	if(temp->getOffset() == 0){
		return "\tMOV AX, "+p->getType()+"\t;Line "+to_string(p->getStartLine())+"\n\tCALL print_output\n\tCALL new_line\n";
	}
	else {
		return "\tMOV AX, [BP - "+to_string(temp->getOffset())+"]\t;Line "+to_string(p->getStartLine())+"\n\tCALL print_output\n\tCALL new_line\n";
	}
}
string processSingleStatement(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	string result = "", temp1 = "", temp2 = "", temp3 = "", temp4 = "";
	if(p->getLine() == "statement : var_declaration")
		return processVarDeclaration(childs[0]); /* need to check whether pushing bp requires for all declaration*/
	else if(p->getLine() == "statement : expression_statement")
		return newLabel()+processExpression(childs[0]);
	else if(p->getLine() == "statement : compound_statement"){
		table->EnterScope();
		if(childs[0]->getChildCount()>2){
			result+=processStatements(childs[0]->getChild()[1]);
		}
		table->ExitScope();
		table->printAll(logout);
		return newLabel()+result;
	}
	else if(p->getLine() == "statement : IF LPAREN expression RPAREN statement"){
		temp1 = processSingleStatement(childs[4]);
		result = newLabel()+processLogicExpression(childs[2]->getChild()[0]);
		result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n';
		result += "\tCMP AX, 0\n\tJE L"+to_string(labelCount+1)+'\n';
		result += temp1;
		result += newLabel();
		return result;
	}
	else if(p->getLine() == "statement : IF LPAREN expression RPAREN statement ELSE statement"){
		temp1 = processSingleStatement(childs[4]);
		temp2 = processSingleStatement(childs[6]);
		result = newLabel()+processLogicExpression(childs[2]->getChild()[0]);
		result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n';
		result += "\tCMP AX, 0\n\tJE L"+to_string(labelCount+1)+'\n';
		result += temp1;
		result += "\tJMP L"+to_string(labelCount+2)+'\n';
		result += newLabel();
		result += temp2;
		result += newLabel();
		return result;
	}
	else if(p->getLine() == "statement : WHILE LPAREN expression RPAREN statement"){
		temp1 = processSingleStatement(childs[4]);
		result = newLabel()+processLogicExpression(childs[2]->getChild()[0]);
		result += "\tPOP AX\n";
		result += "\tCMP AX, 0\n\tJE L"+to_string(labelCount+1)+"\t;Line "+to_string(p->getStartLine())+'\n';
		result += temp1;
		result += "\tJMP L"+to_string(labelCount)+'\n';
		result += newLabel();
		return result;
	}
	else if(p->getLine() == "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement"){
		result += processExpression(childs[2]) +"\t;Assignmrnt\n"; /* assignment*/
		temp1 += processExpression(childs[3]) +"\t;Condition Check\n"; /* conditional check*/
		temp2 += processLogicExpression(childs[4]->getChild()[0])+"\tPOP AX\n\t;INC\n"; /* increment / decrement*/
		temp3 += processSingleStatement(childs[6]);
		result += newLabel() + temp1;
		/* result += "\tPOP AX\n"; / not sure*/
		result += "\tCMP AX, 0\n\tJE L"+to_string(labelCount+1)+"\t;Line "+to_string(p->getStartLine())+'\n';
		result += temp3;
		result += temp2;
		result += "\tJMP L"+to_string(labelCount)+'\n';	
		result += newLabel();
		return result;
	}
	else if(p->getLine() == "statement : PRINTLN LPAREN ID RPAREN SEMICOLON")
		return newLabel()+processPrint(childs[2]);
	else if(p->getLine() == "statement : RETURN expression SEMICOLON"){
		temp1 = processLogicExpression(childs[1]->getChild()[0])+"\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n';
		if(outOffset)
		return newLabel()+temp1+"\tADD SP, "+to_string(offset)+"\n\tPOP BP\n";
		else
		return newLabel()+temp1+"\tADD SP, "+to_string(tempOffset2)+"\n\tPOP BP\n\tRET "+to_string(tempOffset1*-1 - 2)+'\n';
	}
	else
		return "";
}

string processStatements(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "statements : statement"){
		return processSingleStatement(childs[0]);
	}
	else{
		string temp = processStatements(childs[0]);
		return temp + processSingleStatement(childs[1]);
	}
}

string buildMain(ParseTree *p){
	table->EnterScope();
	string result = "\nmain PROC\n\tMOV AX, @DATA\n\tMOV DS, AX\n";
	result+="\tPUSH BP\n\tMOV BP, SP\n";
	ParseTree *child = p->getChild().back();
	if(child->getChildCount() == 3) /* checking if the compound statement is LCURL statement RCURL*/
		result += processStatements(child->getChild()[1]);
	result+="\tMOV AX,4CH\n\tINT 21H\n";
	result+= "main ENDP\n"+printFunction();
	table->ExitScope();
	table->printAll(logout);
	return result;
}

string addParameter(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "parameter_list : parameter_list COMMA type_specifier ID"){
		string temp = addParameter(childs[0]);
		return temp + addParameter(childs[3]);
	}
	else if(p->getLine() == "parameter_list : type_specifier ID"){
		return  addParameter(childs[1]);
	}
	else if(p->getLine().substr(0,2) == "ID"){
		if(table->Insert(p->getType(), type)){
			SymbolInfo *s = table->Lookup(p->getType());
			tempOffset1 -= 2;
			s->setOffset(tempOffset1);
		}
	}

	return result;
}

string buildFunction(ParseTree *p){
	table->EnterScope();
	vector<ParseTree *> childs = p->getChild();
	outOffset = false;
	string result = childs[1]->getType()+"  PROC\n";
	result+="\tPUSH BP\n\tMOV BP, SP\n";
	tempOffset1 = -2;
	if(childs.size() == 6){
		result+=addParameter(childs[3]);
	}
	ParseTree *child = p->getChild().back();
	if(child->getChildCount() == 3) /* checking if the compound statement is LCURL statement RCURL*/
		result += processStatements(child->getChild()[1]);
	if(childs[0]->getChild()[0]->getType()=="void"){
		result+="\tADD SP, "+to_string(tempOffset2)+"\n\tPOP BP\n\tRET 0\n";
	}
	result+=childs[1]->getType()+" ENDP\n";
	tempOffset1 = tempOffset2= 0;
	outOffset = true;
	table->ExitScope();
	table->printAll(logout);
	return result;
}

string addVariable(ParseTree *p){
	if(p == nullptr) return "";
	vector<ParseTree *> childs = p->getChild();
	string result;
	if(p->getLine() == "declaration_list : ID LSQUARE CONST_INT RSQUARE"){
		return "\t"+ childs[0]->getType() + " DW " + childs[2]->getType() + " DUP (000H)\n";
	}
	else if(p->getLine() == "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE"){
		return addVariable(childs[0]) +"\t"+ childs[2]->getType() + " DW " + childs[4]->getType() + " DUP (000H)\n";
	}
	else if(p->getLine() == "declaration_list : ID"){
		return "\t"+childs[0]->getType() + " DW 1 DUP (000H)\n";
	}
	else if(p->getLine() == "declaration_list : declaration_list COMMA ID"){
		return addVariable(childs[0]) + "\t"+childs[2]->getType() + " DW 1 DUP (000H)\n";
	}
	for(auto v:childs){
		result += addVariable(v);
	}
	return result;
}

std::string removePushPop(const std::string& input) {
    std::string result;
    size_t pos = 0;
    size_t found;

    while ((found = input.find("PUSH AX\n\tPOP AX\n", pos)) != std::string::npos) {
        result += input.substr(pos, found - pos);
        pos = found + 15; 
    }
    while ((found = input.find("PUSH BX\n\tPOP BX\n", pos)) != std::string::npos) {
        result += input.substr(pos, found - pos);
        pos = found + 15; 
    }

    result += input.substr(pos);
    return result;
}

string optimizedCode(string code){
	return removePushPop(code);
}
void generateCode(ParseTree *p){
	
	string ans =".MODEL SMALL\n.STACK 1000H\n.DATA\n\tnumber DB \"00000$\"\n";
	string temp ="";
    fprintf(codeout,".MODEL SMALL\n.STACK 1000H\n.DATA\n\tnumber DB \"00000$\"\n");
    vector<ParseTree *> units;
    extractUnits(p,units);
    for(auto v: units){
		if(v->getLine()=="unit : var_declaration"){
			temp = addVariable(v);
			fprintf(codeout,"%s\n", temp.c_str());
			ans+= temp;
			temp = "";
		}
		
    }
	fprintf(codeout, ".CODE\n");
	ans +=  ".CODE\n";
    for(auto v: units){
		if(v->getLine()=="unit : func_definition"){
			if(v->getChild()[0]->getChild()[1]->getType()=="main"){
				temp = buildMain(v->getChild()[0]);
				fprintf(codeout,"%s\n", temp.c_str());
				ans+= temp;
				temp = "";
			}
		}
    }
    for(auto v: units){
		if(v->getLine()=="unit : func_definition"){
			if(v->getChild()[0]->getChild()[1]->getType()!="main"){
				temp = buildFunction(v->getChild()[0]);
				fprintf(codeout,"%s\n", temp.c_str());
				ans+= temp;
				temp = "";
			}
		}
    }
	string opCode = optimizedCode(ans);
	fprintf(optcodeout,"%s",opCode.c_str());
}
/*this part semantic check*/

void yyerror(char *s)
{
	fprintf(logout,"Error at line %d: %s\n",line_count,s);
	err_count++;
	infunc = false;	
	stillValid = true;
	validCount = 0;
	stack1.cleanUp();
	stack2.cleanUp();
	typeList.clear();
}

bool voidCheck(string type1, string name, int line){
	if(type1=="VOID"){
		fprintf(errorout,"Line# %d: Variable or field '%s' declared void\n",line,name.c_str());
		err_count++;
		stillValid = false;
		return true;
	}
	return false;
}
FuncExtras *addExtraInFunc(string type, bool defined){
	vector<Pair *> p;
	SymbolInfo *s;
	stillValid = true;
	while(!stack2.isEmpty() && validCount>0){
		s = stack2.pop();
		p.push_back(new Pair(s->getName(), s->getType()));
		s = nullptr;
		validCount--;
	}
	validCount = 0;
	stack2.cleanUp();
	return new FuncExtras(type,p,defined);
}

ParseTree* setLeft(string type, string line){
	return new ParseTree(type,line,line_count);
}

ParseTree* symbolToParse(SymbolInfo *s){
	if((s->getType()=="INT" or s->getType()=="FLOAT" or s->getType()=="VOID" or s->getType()=="FUNCTION") && !(s->getName()=="int" || s->getName()=="float" || s->getName()=="void"))
		return new ParseTree(s->getName(), "ID : "+s->getName(), s->getLine());
	else
		return new ParseTree(s->getName(), s->getType()+" : "+s->getName(), s->getLine());
}

void insertParameters(){
	if(infunc){
		SymbolInfo *s;	
		int valid = 0;	
		stillValid = true;
		while(!stack1.isEmpty()){
			s = stack1.pop() ;
			if(!table->Insert(s->getName(),s->getType())){	
				fprintf(errorout,"Line# %d: Redefinition of parameter '%s'\n",s->getStartLine(),s->getName().c_str());
				err_count++;
				delete s;
				break;
			}
			valid++;
		}
		validCount = valid;
		stack1.cleanUp();
		infunc = false;
	}
}

bool redefinitionCheck(string name, string type, int line){
	temp = table->Lookup(name);
	if(temp!= nullptr){
		if(temp->getType()==type){
		fprintf(errorout,"Line# %d: Redeclaration of variable '%s'\n",line,name.c_str());
		err_count++;
		return true;
		}
	}
	return false;
}
bool redefinitionCheckFunc(string name, string type1, int line){
	temp = table->Lookup(name);
	if(temp!= nullptr){
		if(temp->getExtra()->getReturnType()==type1){
		fprintf(errorout,"Line# %d: Redefinition of Function '%s'\n",line,name.c_str());
		err_count++;
		return true;
		}
	}
	return false;
}

/* error checking functions*/
void conflictingType(SymbolInfo *s, int line, bool isfunc, string type1, bool define){ /* boolean is func is used so that if we get ID type we may use */
	bool  reclare ;
	if(!(s->isFunc() or isfunc))
		reclare = redefinitionCheck(s->getName(),type1,line);
	if(!isfunc and voidCheck(type1,s->getName(),line)){		
		return;
	}
	if(s->isFunc() and !isfunc){
		if(!table->Insert(s->getName(),type)){
			fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",line,s->getName().c_str());
			err_count++;
		}
	}
	else if(!s->isFunc() and isfunc){
		if(!table->Insert(s->getName(),"FUNCTION",addExtraInFunc(type1,define))){
			fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",line,s->getName().c_str());
			err_count++;
		}
	}
	else if(s->isFunc()){
		if(s->getExtra()->getDefinition()||s->getExtra()->getReturnType()!= type1 ||s->getExtra()->getNumber()!=stack2.size()){
				fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
				err_count++;		
		}
		else if(s->getExtra()){
			vector<Pair *> v2 = s->getExtra()->getParameterList();
			vector<string> v3 = stack2.getList();
			for(int i=0; i<v3.size(); i++){
				if(v2[i]->getType() != v3[i]){
					fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
					err_count++;
					return;
				}
			}
		}
		else if(!table->Insert(s->getName(),"FUNCTION",addExtraInFunc(type1,define)) && !define){
			fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
			err_count++;
		}
	}
	else if(isfunc){
		if(!table->Insert(s->getName(),"FUNCTION",addExtraInFunc(type1,define))){
			fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
			err_count++;
		}
	}
	else if(!reclare && define && table->Insert(s->getName(),type) ){
		temp = table->Lookup(s->getName());
		temp->setArray();
	}
	else if(!reclare && !table->Insert(s->getName(),type) ){
		fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
		err_count++;
	}
	
}
void lookForVariable(SymbolInfo *s, int line, bool func){
	if(table->Lookup(s->getName())==nullptr){
		string s1;
		if(func) s1 = "function";
		else s1 = "variable";
		fprintf(errorout,"Line# %d: Undeclared %s '%s'\n",line,s1.c_str(),s->getName().c_str());
		err_count++;
	}
}
bool checkVoidInExpression(string testType, int line){
	if(testType == "VOID"){
		fprintf(errorout,"Line# %d: Void cannot be used in expression\n", line);
		err_count++;
		return true;
	}
	return false;
}

void functionChecker(SymbolInfo *s, vector<string> &v){
	if(!s->isFunc() ){
		if(s->getType() != "ID"){
		fprintf(errorout,"Line# %d: '%s' is not a function\n", s->getStartLine(), s->getName().c_str());
		err_count++;
		}
	}
    else if(v.size()<s->getExtra()->getNumber()){
        fprintf(errorout,"Line# %d: Too few arguments to function '%s'\n", s->getStartLine(), s->getName().c_str());
		err_count++;
    }
    else if(v.size() > s->getExtra()->getNumber()){
        fprintf(errorout,"Line# %d: Too many arguments to function '%s'\n", s->getStartLine(), s->getName().c_str());
		err_count++;
    }
    else {
        vector<Pair *> temp = s->getExtra()->getParameterList();
        for(int i=0; i<v.size(); i++){
            if(v[i]!=temp[i]->getType()){
                fprintf(errorout,"Line# %d: Type mismatch for argument %d of '%s'\n", s->getStartLine(), i+1, s->getName().c_str());
				err_count++;
            }
        }

    }
	v.clear();
}

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 907 "2005033.y"
typedef union YYSTYPE {
	SymbolInfo* symbolInfo;
	ParseTree* parseTree;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 941 "y.tab.c"

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
    0,    1,    1,    2,    2,    2,    4,    4,   23,    5,
   24,    5,    7,    7,    7,    7,   25,    8,   26,    8,
    3,    6,    6,    6,   10,   10,   10,   10,    9,    9,
   11,   11,   11,   11,   11,   11,   11,   11,   11,   13,
   13,   20,   20,   12,   12,   14,   14,   15,   15,   16,
   16,   17,   17,   18,   18,   18,   19,   19,   19,   19,
   19,   19,   19,   21,   21,   22,   22,
};
static const YYINT yylen[] = {                            2,
    1,    2,    1,    1,    1,    1,    6,    5,    0,    7,
    0,    6,    4,    3,    2,    1,    0,    4,    0,    3,
    3,    1,    1,    1,    3,    6,    1,    4,    1,    2,
    1,    1,    1,    7,    5,    7,    5,    5,    3,    1,
    2,    1,    4,    1,    3,    1,    3,    1,    3,    1,
    3,    1,    3,    2,    2,    1,    1,    4,    3,    1,
    1,    2,    2,    1,    0,    3,    1,
};
static const YYINT yydefred[] = {                         0,
   22,   23,   24,    0,    0,    3,    4,    5,    6,    0,
    2,    0,    0,    0,    0,   21,    0,    0,    0,    0,
    0,    0,    8,    0,   15,    0,    0,   28,    0,    0,
   12,    7,    0,    0,    0,    0,    0,   10,   13,   26,
    0,    0,    0,    0,    0,   40,   60,    0,    0,    0,
   61,    0,   31,    0,   33,    0,   29,    0,   32,   44,
    0,    0,    0,   52,   56,    0,   20,    0,    0,    0,
    0,    0,    0,    0,    0,   54,    0,   55,    0,   18,
   30,   41,    0,    0,    0,    0,    0,   62,   63,    0,
    0,    0,   67,    0,    0,    0,   59,    0,   39,   47,
    0,    0,   53,   45,    0,    0,    0,   58,    0,   43,
    0,    0,    0,   37,   66,   38,    0,    0,   36,   34,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  268,  269,  270,  287,  288,  289,  290,  291,  292,  293,
  289,  261,  297,  262,  271,  264,  267,  263,  293,  294,
  272,  261,  264,  311,  261,  263,  267,  273,  271,  265,
  295,  264,  310,  293,  272,  312,  313,  295,  261,  273,
  257,  259,  260,  261,  262,  264,  272,  274,  275,  279,
  281,  282,  290,  293,  295,  296,  298,  299,  300,  301,
  302,  303,  304,  305,  306,  307,  266,  262,  262,  262,
  262,  271,  299,  262,  299,  305,  307,  305,  261,  266,
  298,  264,  277,  278,  279,  280,  276,  283,  284,  299,
  300,  299,  301,  308,  309,  299,  263,  261,  264,  302,
  303,  304,  305,  301,  263,  300,  263,  263,  267,  273,
  263,  298,  299,  298,  301,  264,  258,  263,  298,  298,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          4,
    5,    6,   53,    8,    9,   54,   20,   55,   56,   13,
   57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
   94,   95,   33,   24,   36,   37,
};
static const YYINT yysindex[] = {                      -211,
    0,    0,    0,    0, -211,    0,    0,    0,    0, -240,
    0, -231, -132, -185, -207,    0, -191, -190, -181, -176,
 -177, -162,    0, -154,    0, -151, -211,    0, -155,    0,
    0,    0, -154, -139, -147, -115, -136,    0,    0,    0,
 -125, -119, -110, -216, -229,    0,    0, -106, -229, -229,
    0, -229,    0, -103,    0, -193,    0, -101,    0,    0,
 -109,  -66, -107,    0,    0,  -80,    0, -229,  -92, -229,
 -229, -229,  -84,  -78,  -73,    0,  -68,    0,  -85,    0,
    0,    0, -229, -229, -229, -229, -229,    0,    0,  -58,
  -92,  -44,    0,  -41,  -72,  -76,    0,  -40,    0,    0,
  -51, -107,    0,    0, -115, -229, -115,    0, -229,    0,
  -35,  -28,  -29,    0,    0,    0, -115, -115,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,  235,    0,    0,    0,    0,    0,
    0, -128,    0,    0,    0,    0,    0,  -27, -163,    0,
    0, -116,    0,    0,    0,  -26,    0,    0,    0, -167,
    0,    0,    0, -157,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -102,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -219, -254,  -53,    0,    0,  -71,    0,    0,    0,    0,
  -23,    0,    0,    0,    0,    0,  -79,    0, -128,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -22,    0,    0,    0,    0,    0,
 -249,  -46,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -141,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  231,   11,    0,    0,   12,    0,   23,    0,    0,
  -56,  -43,  -61,  -67,  154,  158,  159,  -45,    0,  -49,
    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 244
static const YYINT yytable[] = {                         81,
   77,   73,   77,   93,   76,   75,   78,   91,   48,   48,
    7,   10,   48,   49,   49,    7,   10,   49,   48,  104,
   12,   77,   48,   49,   90,   19,   92,   49,   96,  106,
   14,   44,   45,   77,   77,   77,   77,   77,   34,   15,
  103,  115,   47,   46,   46,   71,   31,   46,  112,   50,
  114,   51,   52,   46,   72,   38,    1,    2,    3,   77,
  119,  120,  113,   41,   21,   42,   43,   44,   45,   22,
   46,   30,   80,   23,    1,    2,    3,   18,   47,   25,
   48,   49,    1,    2,    3,   50,   26,   51,   52,   17,
   27,   17,   17,   17,   17,   28,   17,   17,   19,   16,
   17,   17,   17,   16,   17,   14,   17,   17,   29,   14,
   30,   17,   32,   17,   17,   35,   35,   35,   35,   35,
   35,   39,   35,   35,   35,   40,   35,   35,   35,   67,
   35,   16,   35,   35,   17,   27,   68,   35,   27,   35,
   35,   41,   69,   42,   43,   44,   45,   25,   46,   30,
   25,   70,    1,    2,    3,   74,   47,   79,   48,   49,
   42,   42,   82,   50,   42,   51,   52,   83,   44,   45,
   42,   46,   86,   42,   42,   42,   42,   42,   97,   47,
   42,   42,   98,   57,   57,   15,   50,   57,   51,   52,
   99,   57,   57,   57,  109,   87,  110,   57,   57,   57,
   57,   57,   88,   89,  105,   57,   57,   57,   57,   50,
   50,   84,   85,   50,   88,   89,   51,   51,  107,   50,
   51,  108,  111,   50,   50,   50,   51,   85,  116,  117,
   51,   51,   51,  118,    1,   11,  100,   11,    9,   65,
   64,  101,    0,  102,
};
static const YYINT yycheck[] = {                         56,
   50,   45,   52,   71,   50,   49,   52,   69,  263,  264,
    0,    0,  267,  263,  264,    5,    5,  267,  273,   87,
  261,   71,  277,  273,   68,   14,   70,  277,   72,   91,
  262,  261,  262,   83,   84,   85,   86,   87,   27,  271,
   86,  109,  272,  263,  264,  262,   24,  267,  105,  279,
  107,  281,  282,  273,  271,   33,  268,  269,  270,  109,
  117,  118,  106,  257,  272,  259,  260,  261,  262,  261,
  264,  265,  266,  264,  268,  269,  270,  263,  272,  261,
  274,  275,  268,  269,  270,  279,  263,  281,  282,  257,
  267,  259,  260,  261,  262,  273,  264,  265,  266,  263,
  268,  269,  270,  267,  272,  263,  274,  275,  271,  267,
  265,  279,  264,  281,  282,  257,  272,  259,  260,  261,
  262,  261,  264,  265,  266,  273,  268,  269,  270,  266,
  272,  264,  274,  275,  267,  264,  262,  279,  267,  281,
  282,  257,  262,  259,  260,  261,  262,  264,  264,  265,
  267,  262,  268,  269,  270,  262,  272,  261,  274,  275,
  263,  264,  264,  279,  267,  281,  282,  277,  261,  262,
  273,  264,  280,  276,  277,  278,  279,  280,  263,  272,
  283,  284,  261,  263,  264,  271,  279,  267,  281,  282,
  264,  263,  264,  273,  267,  276,  273,  277,  278,  279,
  280,  273,  283,  284,  263,  277,  278,  279,  280,  263,
  264,  278,  279,  267,  283,  284,  263,  264,  263,  273,
  267,  263,  263,  277,  278,  279,  273,  279,  264,  258,
  277,  278,  279,  263,    0,    5,   83,  265,  265,  263,
  263,   84,   -1,   85,
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
   -1,   -1,   -1,
};
#endif
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 285
#define YYUNDFTOKEN 314
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
"unary_expression","factor","variable","argument_list","arguments","$$1","$$2",
"$$3","$$4","illegal-symbol",
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
"$$1 :",
"func_definition : type_specifier ID LPAREN parameter_list RPAREN $$1 compound_statement",
"$$2 :",
"func_definition : type_specifier ID LPAREN RPAREN $$2 compound_statement",
"parameter_list : parameter_list COMMA type_specifier ID",
"parameter_list : parameter_list COMMA type_specifier",
"parameter_list : type_specifier ID",
"parameter_list : type_specifier",
"$$3 :",
"compound_statement : LCURL $$3 statements RCURL",
"$$4 :",
"compound_statement : LCURL $$4 RCURL",
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
#line 1554 "2005033.y"

int main(int argc,char *argv[])
{
	if((fp=fopen(argv[1],"r"))==NULL){
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout= fopen("log.txt","w");
	errorout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt","w");
	codeout = fopen("2005033_code.asm", "w");
	optcodeout = fopen("2005033_optimizedCode.asm","w");
	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	delete table;
	fclose(optcodeout);
	fclose(codeout);
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}
#line 1457 "y.tab.c"

/* Release memory associated with symbol. */
#if ! defined YYDESTRUCT_IS_DECLARED
static void
YYDESTRUCT_DECL()
{
    switch (psymb)
    {
	case 287:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1474 "y.tab.c"
	break;
	case 288:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1484 "y.tab.c"
	break;
	case 289:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1494 "y.tab.c"
	break;
	case 290:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1504 "y.tab.c"
	break;
	case 291:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1514 "y.tab.c"
	break;
	case 292:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1524 "y.tab.c"
	break;
	case 293:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1534 "y.tab.c"
	break;
	case 294:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1544 "y.tab.c"
	break;
	case 295:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1554 "y.tab.c"
	break;
	case 296:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1564 "y.tab.c"
	break;
	case 297:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1574 "y.tab.c"
	break;
	case 298:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1584 "y.tab.c"
	break;
	case 299:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1594 "y.tab.c"
	break;
	case 300:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1604 "y.tab.c"
	break;
	case 301:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1614 "y.tab.c"
	break;
	case 302:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1624 "y.tab.c"
	break;
	case 303:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1634 "y.tab.c"
	break;
	case 304:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1644 "y.tab.c"
	break;
	case 305:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1654 "y.tab.c"
	break;
	case 306:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1664 "y.tab.c"
	break;
	case 307:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1674 "y.tab.c"
	break;
	case 308:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1684 "y.tab.c"
	break;
	case 309:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1694 "y.tab.c"
	break;
	case 310:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1704 "y.tab.c"
	break;
	case 311:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1714 "y.tab.c"
	break;
	case 312:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1724 "y.tab.c"
	break;
	case 313:
#line 916 "2005033.y"
	{
	if((*val).parseTree!=nullptr){
		delete (*val).parseTree;
		(*val).parseTree = nullptr;
	}
}
#line 1734 "y.tab.c"
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
#line 928 "2005033.y"
	{
		/* fprintf(logout,"start : program \n");*/
		yyval.parseTree = setLeft("start","start : program");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		yyval.parseTree->printTree(yyval.parseTree,0,parseout);
		generateCode(yyval.parseTree);
	}
#line 2420 "y.tab.c"
break;
case 2:
#line 939 "2005033.y"
	{
		/* fprintf(logout,"program : program unit \n");*/
		yyval.parseTree = setLeft("program","program : program unit");
		yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); 
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree); 
		yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	}
#line 2433 "y.tab.c"
break;
case 3:
#line 948 "2005033.y"
	{
		/* fprintf(logout,"program : unit \n");*/
		yyval.parseTree = setLeft("program","program : unit");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	}
#line 2444 "y.tab.c"
break;
case 4:
#line 957 "2005033.y"
	{
		/* fprintf(logout,"unit : var_declaration\n");*/
		yyval.parseTree = setLeft("unit","unit : var_declaration");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	}
#line 2455 "y.tab.c"
break;
case 5:
#line 964 "2005033.y"
	{
		/* fprintf(logout,"unit : func_declaration  \n");*/
		yyval.parseTree = setLeft("unit","unit : func_declaration");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	 }
#line 2466 "y.tab.c"
break;
case 6:
#line 971 "2005033.y"
	{
		/* fprintf(logout,"unit : func_definition  \n");*/
		yyval.parseTree = setLeft("unit","unit : func_definition");
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	 }
#line 2477 "y.tab.c"
break;
case 7:
#line 980 "2005033.y"
	{
			yyval.parseTree = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			/* fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON \n");*/
			conflictingType(yystack.l_mark[-4].symbolInfo,yystack.l_mark[-5].parseTree->getStartLine(),true,yystack.l_mark[-5].parseTree->getType(),false);
			yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo));
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());	
			stack1.cleanUp();
			stack2.cleanUp();
		}
#line 2492 "y.tab.c"
break;
case 8:
#line 991 "2005033.y"
	{
			yyval.parseTree = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			/* fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");*/
			conflictingType(yystack.l_mark[-3].symbolInfo,yystack.l_mark[-4].parseTree->getStartLine(),true,yystack.l_mark[-4].parseTree->getType(),false);
			yyval.parseTree->addChild(yystack.l_mark[-4].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo));
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-4].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2505 "y.tab.c"
break;
case 9:
#line 1002 "2005033.y"
	{infunc = true;}
#line 2510 "y.tab.c"
break;
case 10:
#line 1002 "2005033.y"
	{ /**/
			yyval.parseTree = setLeft("func_definition","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ");
			/* fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");*/
			conflictingType(yystack.l_mark[-5].symbolInfo,yystack.l_mark[-6].parseTree->getStartLine(),true,yystack.l_mark[-6].parseTree->getType(),true);
			yyval.parseTree->addChild(yystack.l_mark[-6].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-5].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo));
			yyval.parseTree->addChild(yystack.l_mark[-3].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-6].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
				
		}
#line 2524 "y.tab.c"
break;
case 11:
#line 1012 "2005033.y"
	{infunc = true;}
#line 2529 "y.tab.c"
break;
case 12:
#line 1012 "2005033.y"
	{
			yyval.parseTree = setLeft("func_definition","func_definition : type_specifier ID LPAREN RPAREN compound_statement ");
			/* fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");*/
			conflictingType(yystack.l_mark[-4].symbolInfo,yystack.l_mark[-5].parseTree->getStartLine(),true,yystack.l_mark[-5].parseTree->getType(),true);
			yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo));
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);		
			yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());			
		}
#line 2542 "y.tab.c"
break;
case 13:
#line 1024 "2005033.y"
	{
			yyval.parseTree = setLeft("parameter_list","parameter_list : parameter_list COMMA type_specifier ID");
			/* fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");*/
			temp = yystack.l_mark[0].symbolInfo;
			if(!voidCheck(type,temp->getName(),yystack.l_mark[0].symbolInfo->getEndLine()) && stillValid){
				stack1.push(new SymbolInfo(temp->getName(), type));
				stack2.push(new SymbolInfo(temp->getName(), type));
				validCount++;
			}			
			yyval.parseTree->addChild(yystack.l_mark[-3].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo));
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-3].parseTree->getStartLine());	
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2560 "y.tab.c"
break;
case 14:
#line 1038 "2005033.y"
	{
			yyval.parseTree = setLeft("parameter_list", "parameter_list : parameter_list COMMA type_specifier");
			/* fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");*/
			if(!voidCheck(type,"",yystack.l_mark[0].parseTree->getStartLine()) && stillValid){
				stack1.push(new SymbolInfo("temp", type));
				stack2.push(new SymbolInfo("temp", type));
				validCount++;
			}
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		}
#line 2576 "y.tab.c"
break;
case 15:
#line 1050 "2005033.y"
	{
			yyval.parseTree = setLeft("parameter_list","parameter_list : type_specifier ID");
			/* fprintf(logout,"parameter_list  : type_specifier ID\n");*/
			temp = yystack.l_mark[0].symbolInfo;
			if(!voidCheck(type,temp->getName(),yystack.l_mark[0].symbolInfo->getEndLine()) && stillValid){
				stack1.push(new SymbolInfo(temp->getName(), type));
				stack2.push(new SymbolInfo(temp->getName(), type));
				validCount++;
			}
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2593 "y.tab.c"
break;
case 16:
#line 1063 "2005033.y"
	{
			yyval.parseTree = setLeft("parameter_list","parameter_list : type_specifier");
			/* fprintf(logout,"parameter_list  : type_specifier\n");*/
			if(!voidCheck(type,"",yystack.l_mark[0].parseTree->getStartLine())){
				stack1.push(new SymbolInfo("temp", type));
				stack2.push(new SymbolInfo("temp", type));
				validCount++;
			}
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		}
#line 2609 "y.tab.c"
break;
case 17:
#line 1078 "2005033.y"
	{insertParameters();}
#line 2614 "y.tab.c"
break;
case 18:
#line 1078 "2005033.y"
	{
				yyval.parseTree = setLeft("compound_statement","compound_statement : LCURL statements RCURL");
				/* fprintf(logout,"compound_statement : LCURL statements RCURL  \n");*/
				/* table->printAll(logout);*/
				table->ExitScope();
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[-3].symbolInfo->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());			
			}
#line 2627 "y.tab.c"
break;
case 19:
#line 1087 "2005033.y"
	{insertParameters();}
#line 2632 "y.tab.c"
break;
case 20:
#line 1087 "2005033.y"
	{
				yyval.parseTree = setLeft("compound_statement","compound_statement : LCURL RCURL");
				/* fprintf(logout,"compound_statement : LCURL RCURL  \n");*/
				/* table->printAll(logout);*/
				table->ExitScope();
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[-2].symbolInfo->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 2645 "y.tab.c"
break;
case 21:
#line 1098 "2005033.y"
	{
		yyval.parseTree = setLeft("var_declaration","var_declaration : type_specifier declaration_list SEMICOLON");
		/* fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");*/
		yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo)); 
		yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2656 "y.tab.c"
break;
case 22:
#line 1107 "2005033.y"
	{
		yyval.parseTree = setLeft("INT", "type_specifier : INT");
		/* fprintf(logout,"type_specifier : INT \n"); */
		type = "INT"; 
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2668 "y.tab.c"
break;
case 23:
#line 1115 "2005033.y"
	{
		yyval.parseTree = setLeft("FLOAT", "type_specifier : FLOAT");
		/* fprintf(logout,"type_specifier : FLOAT \n"); */
		type = "FLOAT"; 
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2680 "y.tab.c"
break;
case 24:
#line 1123 "2005033.y"
	{
		yyval.parseTree = setLeft("VOID", "type_specifier : VOID");
		/* fprintf(logout,"type_specifier : VOID \n"); */
		type = "VOID"; 
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2692 "y.tab.c"
break;
case 25:
#line 1133 "2005033.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list : declaration_list COMMA ID");
				/* fprintf(logout,"declaration_list : declaration_list COMMA ID \n");*/
				conflictingType(yystack.l_mark[0].symbolInfo,yystack.l_mark[-2].parseTree->getStartLine(),false,type,false);				
				yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
				}
#line 2704 "y.tab.c"
break;
case 26:
#line 1141 "2005033.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
				/* fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");*/
				conflictingType(yystack.l_mark[-3].symbolInfo,yystack.l_mark[-5].parseTree->getStartLine(),false,type,true);				
				yyval.parseTree->addChild(yystack.l_mark[-5].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo));
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo)); 
				yyval.parseTree->setStartLine(yystack.l_mark[-5].parseTree->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());	
				}
#line 2717 "y.tab.c"
break;
case 27:
#line 1150 "2005033.y"
	{
				yyval.parseTree=setLeft("declaration_list", "declaration_list : ID");
				/* fprintf(logout,"declaration_list : ID \n");*/
				conflictingType(yystack.l_mark[0].symbolInfo,yystack.l_mark[0].symbolInfo->getStartLine(),false,type,false);				
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		  		}
#line 2729 "y.tab.c"
break;
case 28:
#line 1158 "2005033.y"
	{
				yyval.parseTree=setLeft("declaration_list","declaration_list : ID LSQUARE CONST_INT RSQUARE");
				/* fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");			*/
				conflictingType(yystack.l_mark[-3].symbolInfo,yystack.l_mark[-3].symbolInfo->getStartLine(),false,type,true);
				yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
				yyval.parseTree->setStartLine(yystack.l_mark[-3].symbolInfo->getStartLine());
				yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());	
		  		}
#line 2741 "y.tab.c"
break;
case 29:
#line 1168 "2005033.y"
	{
		yyval.parseTree=setLeft("statements","statements : statement");
		/* fprintf(logout,"statements : statement  \n");*/
		yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		}
#line 2752 "y.tab.c"
break;
case 30:
#line 1175 "2005033.y"
	{
		yyval.parseTree=setLeft("statements","statements : statements statement");
		/* fprintf(logout,"statements : statements statement  \n");*/
		yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
		yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	   }
#line 2763 "y.tab.c"
break;
case 31:
#line 1184 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : var_declaration");
			/* fprintf(logout,"statement : var_declaration \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

			}
#line 2775 "y.tab.c"
break;
case 32:
#line 1192 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : expression_statement");
			/* fprintf(logout,"statement : expression_statement  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	  		}
#line 2787 "y.tab.c"
break;
case 33:
#line 1200 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : compound_statement");
			/* fprintf(logout,"statement : compound_statement  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	  		}
#line 2799 "y.tab.c"
break;
case 34:
#line 1208 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			/* fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-6].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-5].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-4].parseTree);
			yyval.parseTree->addChild(yystack.l_mark[-3].parseTree); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-6].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	  		}
#line 2812 "y.tab.c"
break;
case 35:
#line 1217 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : IF LPAREN expression RPAREN statement");
			/* fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree);
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-4].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	  		}
#line 2825 "y.tab.c"
break;
case 36:
#line 1226 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : IF LPAREN expression RPAREN statement ELSE statement");
			/* fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-6].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-5].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-4].parseTree);
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-6].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

	  		}
#line 2838 "y.tab.c"
break;
case 37:
#line 1235 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : WHILE LPAREN expression RPAREN statement");
			/* fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement\n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-2].parseTree);
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree); 
			yyval.parseTree->setStartLine(yystack.l_mark[-4].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	  		}
#line 2850 "y.tab.c"
break;
case 38:
#line 1243 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");			
			/* fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");*/
			lookForVariable(yystack.l_mark[-2].symbolInfo,yystack.l_mark[-4].symbolInfo->getStartLine(), false);
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-4].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo));
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo)); 
			yyval.parseTree->setStartLine(yystack.l_mark[-4].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
	  		}
#line 2863 "y.tab.c"
break;
case 39:
#line 1252 "2005033.y"
	{
			yyval.parseTree=setLeft("statement","statement : RETURN expression SEMICOLON");
			/* fprintf(logout,"statement : RETURN expression SEMICOLON\n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-2].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
	  		}
#line 2874 "y.tab.c"
break;
case 40:
#line 1261 "2005033.y"
	{
			yyval.parseTree=setLeft("expression_statement","expression_statement : SEMICOLON");
			/* fprintf(logout,"expression_statement : SEMICOLON\n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 2885 "y.tab.c"
break;
case 41:
#line 1268 "2005033.y"
	{
			yyval.parseTree=setLeft("expression_statement","expression_statement : expression SEMICOLON");
			/* fprintf(logout,"expression_statement : expression SEMICOLON\n");*/
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 2896 "y.tab.c"
break;
case 42:
#line 1277 "2005033.y"
	{
		yyval.parseTree=setLeft(yystack.l_mark[0].symbolInfo->getType(),"variable : ID");
		/* fprintf(logout,"variable : ID\n");*/
		lookForVariable(yystack.l_mark[0].symbolInfo,yystack.l_mark[0].symbolInfo->getStartLine(), false);
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
		}
#line 2908 "y.tab.c"
break;
case 43:
#line 1285 "2005033.y"
	{
		yyval.parseTree=setLeft(yystack.l_mark[-3].symbolInfo->getType(),"variable : ID LSQUARE expression RSQUARE");
		/* fprintf(logout,"variable : ID LSQUARE expression RSQUARE\n");*/
		lookForVariable(yystack.l_mark[-3].symbolInfo,yystack.l_mark[-3].symbolInfo->getStartLine(), false);
		if(yystack.l_mark[-3].symbolInfo->getType()!="ID" && !yystack.l_mark[-3].symbolInfo->isArray()){fprintf(errorout,"Line# %d: '%s' is not an array\n", yystack.l_mark[-3].symbolInfo->getStartLine(),yystack.l_mark[-3].symbolInfo->getName().c_str()); err_count++;}
		else if(yystack.l_mark[-1].parseTree->getType() != "INT") {fprintf(errorout,"Line# %d: Array subscript is not an integer\n", yystack.l_mark[-1].parseTree->getStartLine()); err_count++;}
		yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
		yyval.parseTree->setStartLine(yystack.l_mark[-3].symbolInfo->getStartLine());
		yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
	 }
#line 2922 "y.tab.c"
break;
case 44:
#line 1297 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"expression : logic_expression");
			/* fprintf(logout,"expression : logic_expression\n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	 		}
#line 2933 "y.tab.c"
break;
case 45:
#line 1304 "2005033.y"
	{ /* what will be the type of $$*/
			/* fprintf(logout,"expression : variable ASSIGNOP logic_expression \n");*/
			if(yystack.l_mark[-2].parseTree->getType() == "INT" and yystack.l_mark[0].parseTree->getType() == "FLOAT"){
				fprintf(errorout,"Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n", yystack.l_mark[-2].parseTree->getStartLine());
				err_count++;
			}
			if(checkVoidInExpression(yystack.l_mark[-2].parseTree->getType(),yystack.l_mark[-2].parseTree->getStartLine())){
				yystack.l_mark[-2].parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			yyval.parseTree=setLeft(yystack.l_mark[-2].parseTree->getType(),"expression : variable ASSIGNOP logic_expression");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	   		}
#line 2954 "y.tab.c"
break;
case 46:
#line 1323 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"logic_expression : rel_expression");
			/* fprintf(logout,"logic_expression : rel_expression 	 \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 2965 "y.tab.c"
break;
case 47:
#line 1330 "2005033.y"
	{
			yyval.parseTree=setLeft("INT","logic_expression : rel_expression LOGICOP rel_expression");
			/* fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression\n");*/
			if(checkVoidInExpression(yystack.l_mark[-2].parseTree->getType(),yystack.l_mark[-2].parseTree->getStartLine())){
				yystack.l_mark[-2].parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		 	}
#line 2983 "y.tab.c"
break;
case 48:
#line 1346 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"rel_expression : simple_expression");
			/* fprintf(logout,"rel_expression : simple_expression \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

			}
#line 2995 "y.tab.c"
break;
case 49:
#line 1354 "2005033.y"
	{
			yyval.parseTree=setLeft("INT","rel_expression : simple_expression RELOP simple_expression");
			/* fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");*/
			if(checkVoidInExpression(yystack.l_mark[-2].parseTree->getType(),yystack.l_mark[-2].parseTree->getStartLine())){
				yystack.l_mark[-2].parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		}
#line 3013 "y.tab.c"
break;
case 50:
#line 1370 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"simple_expression : term");
			/* fprintf(logout,"simple_expression : term \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());

			}
#line 3024 "y.tab.c"
break;
case 51:
#line 1377 "2005033.y"
	{
			/* fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");*/
			if(yystack.l_mark[-2].parseTree->getType() == "INT" and yystack.l_mark[0].parseTree->getType() == "FLOAT"){
				yyval.parseTree->setType("FLOAT");
			}
			if(checkVoidInExpression(yystack.l_mark[-2].parseTree->getType(),yystack.l_mark[-2].parseTree->getStartLine())){
				yystack.l_mark[-2].parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			yyval.parseTree=setLeft(yystack.l_mark[-2].parseTree->getType(),"simple_expression : simple_expression ADDOP term");
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		  }
#line 3044 "y.tab.c"
break;
case 52:
#line 1395 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"term : unary_expression");
			/* fprintf(logout,"term :	unary_expression \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 3055 "y.tab.c"
break;
case 53:
#line 1402 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[-2].parseTree->getType(),"term : term MULOP unary_expression");
			if(yystack.l_mark[-2].parseTree->getType() == "INT" and yystack.l_mark[0].parseTree->getType() == "FLOAT"){
				yyval.parseTree->setType("FLOAT");
			}
			if((yystack.l_mark[-2].parseTree->getType() == "FLOAT" or yystack.l_mark[0].parseTree->getType() == "FLOAT") and (yystack.l_mark[-1].symbolInfo->getName()=="%")){
				err_count++;
				fprintf(errorout,"Line# %d: Operands of modulus must be integers\n", yystack.l_mark[-2].parseTree->getStartLine());
				yyval.parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[-2].parseTree->getType(),yystack.l_mark[-2].parseTree->getStartLine())){
				yystack.l_mark[-2].parseTree->setType("INT");
				yyval.parseTree->setType("INT");
			}
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			string s(yytext);
			if(s[0] == '0' and stof(s)==0 and (yystack.l_mark[-1].symbolInfo->getName()=="%" || yystack.l_mark[-1].symbolInfo->getName()=="/")){
				err_count++;
				fprintf(errorout,"Line# %d: Warning: division by zero i=0f=1Const=0\n", yystack.l_mark[-2].parseTree->getStartLine());
			}
			/* fprintf(logout,"term : term MULOP unary_expression \n");*/
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
	 		}
#line 3086 "y.tab.c"
break;
case 54:
#line 1431 "2005033.y"
	{
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"unary_expression : ADDOP unary_expression");
			/* fprintf(logout,"unary_expression : ADDOP unary_expression  \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-1].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 3100 "y.tab.c"
break;
case 55:
#line 1441 "2005033.y"
	{
			if(checkVoidInExpression(yystack.l_mark[0].parseTree->getType(),yystack.l_mark[0].parseTree->getStartLine())){
				yystack.l_mark[0].parseTree->setType("INT");
			}
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"unary_expression : NOT unary_expression");
			/* fprintf(logout,"unary_expression : NOT unary_expression \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-1].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		 	}
#line 3114 "y.tab.c"
break;
case 56:
#line 1451 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"unary_expression : factor");
			/* fprintf(logout,"unary_expression : factor \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		 	}
#line 3125 "y.tab.c"
break;
case 57:
#line 1460 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"factor : variable");
			/* fprintf(logout,"factor : variable  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 3136 "y.tab.c"
break;
case 58:
#line 1467 "2005033.y"
	{
			string testType;
			if(yystack.l_mark[-3].symbolInfo->isFunc()){
				testType = yystack.l_mark[-3].symbolInfo->getExtra()->getReturnType();				
			}
			else{
				testType = yystack.l_mark[-3].symbolInfo->getType();
			}
			yyval.parseTree=setLeft(testType,"factor	: ID LPAREN argument_list RPAREN");
			/* fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");*/
			lookForVariable(yystack.l_mark[-3].symbolInfo,yystack.l_mark[-3].symbolInfo->getStartLine(), true);
			functionChecker(yystack.l_mark[-3].symbolInfo,typeList);
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-3].symbolInfo)); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-3].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3156 "y.tab.c"
break;
case 59:
#line 1483 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[-1].parseTree->getType(),"factor : LPAREN expression RPAREN");
			/* fprintf(logout,"factor	: LPAREN expression RPAREN   \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-2].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-2].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3167 "y.tab.c"
break;
case 60:
#line 1490 "2005033.y"
	{
			yyval.parseTree=setLeft("INT","factor : CONST_INT");
			/* fprintf(logout,"factor : CONST_INT\n");*/
			value = atoi(yystack.l_mark[0].symbolInfo->getName().c_str());
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3179 "y.tab.c"
break;
case 61:
#line 1498 "2005033.y"
	{
			yyval.parseTree=setLeft("FLOAT","factor : CONST_FLOAT");
			/* fprintf(logout,"factor	: CONST_FLOAT   \n");*/
			yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[0].symbolInfo->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3190 "y.tab.c"
break;
case 62:
#line 1505 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[-1].parseTree->getType(),"factor : variable INCOP");
			/* fprintf(logout,"factor	: variable INCOP  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3201 "y.tab.c"
break;
case 63:
#line 1512 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[-1].parseTree->getType(),"factor : variable DECOP");
			/* fprintf(logout,"factor	: variable DECOP  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[-1].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[0].symbolInfo));
			yyval.parseTree->setStartLine(yystack.l_mark[-1].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].symbolInfo->getEndLine());
			}
#line 3212 "y.tab.c"
break;
case 64:
#line 1521 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"argument_list : arguments");
			/* fprintf(logout,"argument_list : arguments  \n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 3223 "y.tab.c"
break;
case 65:
#line 1528 "2005033.y"
	{
			yyval.parseTree=setLeft("VOID","argument_list :");
			/* fprintf(logout,"argument_list :\n");*/
			yyval.parseTree->addChild(new ParseTree("","",line_count));
			yyval.parseTree->setStartLine(line_count);
			yyval.parseTree->setEndLine(line_count);
			}
#line 3234 "y.tab.c"
break;
case 66:
#line 1537 "2005033.y"
	{
			yyval.parseTree=setLeft(yystack.l_mark[-2].parseTree->getType(),"arguments : arguments COMMA logic_expression");
			typeList.push_back(yystack.l_mark[0].parseTree->getType());
			/* fprintf(logout,"arguments : arguments COMMA logic_expression \n");*/
			yyval.parseTree->addChild(yystack.l_mark[-2].parseTree); yyval.parseTree->addChild(symbolToParse(yystack.l_mark[-1].symbolInfo)); yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[-2].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
			}
#line 3246 "y.tab.c"
break;
case 67:
#line 1545 "2005033.y"
	{
			typeList.push_back(yystack.l_mark[0].parseTree->getType());
			yyval.parseTree=setLeft(yystack.l_mark[0].parseTree->getType(),"arguments : logic_expression");
			/* fprintf(logout,"arguments : logic_expression\n");*/
			yyval.parseTree->addChild(yystack.l_mark[0].parseTree);
			yyval.parseTree->setStartLine(yystack.l_mark[0].parseTree->getStartLine());
			yyval.parseTree->setEndLine(yystack.l_mark[0].parseTree->getEndLine());
		  }
#line 3258 "y.tab.c"
break;
#line 3260 "y.tab.c"
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
