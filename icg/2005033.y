%{
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

string processVariable(ParseTree *p){ // the ans should be at AX
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
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; //the index is at AX
			result += "\tMOV DX, 2\n\tMUL DX\n"; //the index*2 is the location of the word
			result += "\tLEA SI, "+temp->getName()+"\n\tADD SI, AX\n";
			result += "\tPUSH [SI]\n";	
		}
		else{
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+'\n'; //the index is at AX 
			result += "\tMOV DX, 2\n\tMUL DX\n"; //the index*2 is the location of the word
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
string processFactor(ParseTree *p){ // the ans should be at AX
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

string processUnaryExpression(ParseTree *p){ // the ans should be at AX
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

string processTerm(ParseTree *p){ // the ans should be at AX
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

string processSimpleExpression(ParseTree *p){ // the ans should be at AX
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
string processRelop(ParseTree *p){ // JUMP FOR TRUE
	if(p->getType() == "<") return "\tJL L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "<=") return "\tJLE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == ">") return "\tJG L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == ">=") return "\tJGE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "==") return "\tJE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else if(p->getType() == "!=") return "\tJNE L"+to_string(labelCount+1)+"\n\tPUSH 0\n\tJMP L"+to_string(labelCount+2)+'\n';
	else return "";
}
string processRelExpression(ParseTree *p){ // the ans should be at AX
	vector<ParseTree *> childs = p->getChild();
	string result = "";
	if(p->getLine() == "rel_expression : simple_expression"){
		return processSimpleExpression(p->getChild()[0]);
	}
	else if(p->getLine() == "rel_expression : simple_expression RELOP simple_expression"){
		result += processSimpleExpression(childs[2]);
		result += processSimpleExpression(childs[0]);
		result += "\tPOP AX\n\tPOP BX\n\tCMP AX, BX\t;Line "+to_string(p->getStartLine())+"\n"; // AX has the value of child[0] and BX has the value of child[2]
		result += processRelop(childs[1]);
		result += newLabel();
		result += "\tPUSH 1\n";
		result += newLabel();
		return result;
	}
	return "";
}


string processLogicExpression(ParseTree *p){ // the ans should be at AX
	vector<ParseTree *> childs = p->getChild();
	string result = "", temp = "";
	if(p->getLine() == "logic_expression : rel_expression")
		return processRelExpression(p->getChild()[0]);
	else if(p->getLine() == "logic_expression : rel_expression LOGICOP rel_expression"){
		if(childs[1]->getType() == "&&"){
			result += processRelExpression(childs[0]);
			temp = processRelExpression(childs[2]);
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; // AX has the value of child[0]
			result += "\tCMP AX, 0\n";
			result += "\tJE L"+to_string(labelCount+2)+'\n';
			result += "\tPUSH 1\n";
			result += newLabel();
			result += temp;
			result += "\tPOP AX\n"; // AX has the value of child[2]
			result += "\tPOP BX\n"; // previously pushed 1
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
			result += "\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; // AX has the value of child[0]
			result += "\tCMP AX, 0\n";
			result += "\tJG L"+to_string(labelCount+2)+'\n';
			result += "\tPUSH 0\n";
			result += newLabel();
			result += temp;
			result += "\tPOP AX\n"; // AX has the value of child[2]
			result += "\tPOP BX\n"; // previously pushed 1
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
			result += "\tPOP BX\n\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; //the index is at AX and the value is at BX
			result += "\tMOV DX, 2\n\tMUL DX\n"; //the index*2 is the location of the word
			result += "\tLEA SI, "+temp->getName()+"\n\tADD SI, AX\n";
			result += "\tMOV [SI], BX\n";	
		}
		else{
			result += "\tPOP BX\n\tPOP AX\t;Line "+to_string(p->getStartLine())+"\n"; //the index is at AX and the value is at BX
			result += "\tMOV DX, 2\n\tMUL DX\n"; //the index*2 is the location of the word
			result += "\tMOV DX, "+to_string(temp->getOffset());
			result += "\n\tSUB DX, AX\n";
			result += "\tMOV SI, DX\n";
			result += "\tNEG SI\n\tMOV [BP+SI], BX\n";	
		}
		if(doPush)
			result+= "\tPUSH BX\t;Line "+to_string(p->getStartLine())+"\n"; // not that much sure
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
		result += "\tPOP AX\n"; // not that much sure 
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
		return processVarDeclaration(childs[0]); // need to check whether pushing bp requires for all declaration
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
		result += processExpression(childs[2]) +"\t;Assignmrnt\n"; // assignment
		temp1 += processExpression(childs[3]) +"\t;Condition Check\n"; // conditional check
		temp2 += processLogicExpression(childs[4]->getChild()[0])+"\tPOP AX\n\t;INC\n"; // increment / decrement
		temp3 += processSingleStatement(childs[6]);
		result += newLabel() + temp1;
		// result += "\tPOP AX\n"; / not sure
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
	if(child->getChildCount() == 3) // checking if the compound statement is LCURL statement RCURL
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
	if(child->getChildCount() == 3) // checking if the compound statement is LCURL statement RCURL
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
//this part semantic check

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

// error checking functions
void conflictingType(SymbolInfo *s, int line, bool isfunc, string type1, bool define){ // boolean is func is used so that if we get ID type we may use 
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

%}

%union {
	SymbolInfo* symbolInfo;
	ParseTree* parseTree;
}
%token <symbolInfo> IF ELSE FOR WHILE ID LPAREN RPAREN SEMICOLON LCURL RCURL COMMA INT FLOAT VOID LTHIRD CONST_INT RTHIRD PRINTLN RETURN
ASSIGNOP LOGICOP RELOP ADDOP MULOP CONST_FLOAT NOT INCOP DECOP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%error-verbose 
%destructor {
	if($$!=nullptr){
		delete $$;
		$$ = nullptr;
	}
} <parseTree> 
%type <parseTree> start program unit var_declaration func_declaration func_definition type_specifier parameter_list compound_statement statements declaration_list statement expression expression_statement logic_expression rel_expression simple_expression term unary_expression factor variable argument_list arguments
%left 
%right
%%

start : program
	{
		// fprintf(logout,"start : program \n");
		$$ = setLeft("start","start : program");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->printTree($$,0,parseout);
		generateCode($$);
	}
	;

program : program unit {
		// fprintf(logout,"program : program unit \n");
		$$ = setLeft("program","program : program unit");
		$$->addChild($1); 
		$$->addChild($2); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());

	}
	| unit{
		// fprintf(logout,"program : unit \n");
		$$ = setLeft("program","program : unit");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	}
	;
	
unit : var_declaration{
		// fprintf(logout,"unit : var_declaration\n");
		$$ = setLeft("unit","unit : var_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	}
     | func_declaration{
		// fprintf(logout,"unit : func_declaration  \n");
		$$ = setLeft("unit","unit : func_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	 }
     | func_definition{
		// fprintf(logout,"unit : func_definition  \n");
		$$ = setLeft("unit","unit : func_definition");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	 }
     ;
	
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			// fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),false);
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild($4); $$->addChild(symbolToParse($5)); $$->addChild(symbolToParse($6));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($6->getEndLine());	
			stack1.cleanUp();
			stack2.cleanUp();
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			// fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),false);
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN{infunc = true;} compound_statement{ //
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ");
			// fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),true);
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild($4); $$->addChild(symbolToParse($5)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());
				
		}
		| type_specifier ID LPAREN RPAREN{infunc = true;} compound_statement{
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN RPAREN compound_statement ");
			// fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),true);
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild($6);		
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($6->getEndLine());			
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID{
			$$ = setLeft("parameter_list","parameter_list : parameter_list COMMA type_specifier ID");
			// fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
			temp = $4;
			if(!voidCheck(type,temp->getName(),$4->getEndLine()) && stillValid){
				stack1.push(new SymbolInfo(temp->getName(), type));
				stack2.push(new SymbolInfo(temp->getName(), type));
				validCount++;
			}			
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->addChild($3); $$->addChild(symbolToParse($4));
			$$->setStartLine($1->getStartLine());	
			$$->setEndLine($4->getEndLine());
		}
		| parameter_list COMMA type_specifier{
			$$ = setLeft("parameter_list", "parameter_list : parameter_list COMMA type_specifier");
			// fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
			if(!voidCheck(type,"",$3->getStartLine()) && stillValid){
				stack1.push(new SymbolInfo("temp", type));
				stack2.push(new SymbolInfo("temp", type));
				validCount++;
			}
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
		}
 		| type_specifier ID{
			$$ = setLeft("parameter_list","parameter_list : type_specifier ID");
			// fprintf(logout,"parameter_list  : type_specifier ID\n");
			temp = $2;
			if(!voidCheck(type,temp->getName(),$2->getEndLine()) && stillValid){
				stack1.push(new SymbolInfo(temp->getName(), type));
				stack2.push(new SymbolInfo(temp->getName(), type));
				validCount++;
			}
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
		}
		| type_specifier{
			$$ = setLeft("parameter_list","parameter_list : type_specifier");
			// fprintf(logout,"parameter_list  : type_specifier\n");
			if(!voidCheck(type,"",$1->getStartLine())){
				stack1.push(new SymbolInfo("temp", type));
				stack2.push(new SymbolInfo("temp", type));
				validCount++;
			}
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
		}
 		;

 		
compound_statement : LCURL{insertParameters();} statements RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL statements RCURL");
				// fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
				// table->printAll(logout);
				table->ExitScope();
				$$->addChild(symbolToParse($1)); $$->addChild($3); $$->addChild(symbolToParse($4));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($4->getEndLine());			
			}
 		    | LCURL{insertParameters();} RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL RCURL");
				// fprintf(logout,"compound_statement : LCURL RCURL  \n");
				// table->printAll(logout);
				table->ExitScope();
				$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($3));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($3->getEndLine());
			}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		$$ = setLeft("var_declaration","var_declaration : type_specifier declaration_list SEMICOLON");
		// fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		$$->addChild($1); $$->addChild($2); $$->addChild(symbolToParse($3)); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		}
 		 ;
 		 
type_specifier	: INT {
		$$ = setLeft("INT", "type_specifier : INT");
		// fprintf(logout,"type_specifier : INT \n"); 
		type = "INT"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		| FLOAT {
		$$ = setLeft("FLOAT", "type_specifier : FLOAT");
		// fprintf(logout,"type_specifier : FLOAT \n"); 
		type = "FLOAT"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		| VOID {
		$$ = setLeft("VOID", "type_specifier : VOID");
		// fprintf(logout,"type_specifier : VOID \n"); 
		type = "VOID"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		;
 		
declaration_list : declaration_list COMMA ID{
				$$=setLeft("declaration_list","declaration_list : declaration_list COMMA ID");
				// fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
				conflictingType($3,$1->getStartLine(),false,type,false);				
				$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($3->getEndLine());
				}
 		  		| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
				// fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
				conflictingType($3,$1->getStartLine(),false,type,true);				
				$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
				$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5)); $$->addChild(symbolToParse($6)); 
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($6->getEndLine());	
				}
 		  		| ID{
				$$=setLeft("declaration_list", "declaration_list : ID");
				// fprintf(logout,"declaration_list : ID \n");
				conflictingType($1,$1->getStartLine(),false,type,false);				
				$$->addChild(symbolToParse($1));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($1->getEndLine());
		  		}
 		  		| ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list : ID LSQUARE CONST_INT RSQUARE");
				// fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");			
				conflictingType($1,$1->getStartLine(),false,type,true);
				$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3)); $$->addChild(symbolToParse($4));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($4->getEndLine());	
		  		}	
 		  ;
 		  
statements : statement{
		$$=setLeft("statements","statements : statement");
		// fprintf(logout,"statements : statement  \n");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
	   | statements statement{
		$$=setLeft("statements","statements : statements statement");
		// fprintf(logout,"statements : statements statement  \n");
		$$->addChild($1); $$->addChild($2);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
	   }
	   ;
	   
statement : var_declaration {
			$$=setLeft("statement","statement : var_declaration");
			// fprintf(logout,"statement : var_declaration \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

			}
	  		| expression_statement{
			$$=setLeft("statement","statement : expression_statement");
			// fprintf(logout,"statement : expression_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

	  		}
	  		| compound_statement{
			$$=setLeft("statement","statement : compound_statement");
			// fprintf(logout,"statement : compound_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

	  		}
	  		| FOR LPAREN expression_statement expression_statement expression RPAREN statement{
			$$=setLeft("statement","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			// fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild($4); $$->addChild($5); $$->addChild(symbolToParse($6)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());

	  		}
	  		| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE ;{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement");
			// fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());

	  		}
	  		| IF LPAREN expression RPAREN statement ELSE statement{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement ELSE statement");
			// fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5); $$->addChild(symbolToParse($6)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());

	  		}
	  		| WHILE LPAREN expression RPAREN statement{
			$$=setLeft("statement","statement : WHILE LPAREN expression RPAREN statement");
			// fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement\n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5); 
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
	  		}
	  		| PRINTLN LPAREN ID RPAREN SEMICOLON{
			$$=setLeft("statement","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");			
			// fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
			lookForVariable($3,$1->getStartLine(), false);
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5)); 
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
	  		}
	  		| RETURN expression SEMICOLON{
			$$=setLeft("statement","statement : RETURN expression SEMICOLON");
			// fprintf(logout,"statement : RETURN expression SEMICOLON\n");
			$$->addChild(symbolToParse($1)); $$->addChild($2); $$->addChild(symbolToParse($3));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
	  		}
			;
	  
expression_statement : SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : SEMICOLON");
			// fprintf(logout,"expression_statement : SEMICOLON\n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}			
			| expression SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : expression SEMICOLON");
			// fprintf(logout,"expression_statement : expression SEMICOLON\n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			} 
			;
	  
variable : ID{
		$$=setLeft($1->getType(),"variable : ID");
		// fprintf(logout,"variable : ID\n");
		lookForVariable($1,$1->getStartLine(), false);
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		} 		
	 	| ID LTHIRD expression RTHIRD{
		$$=setLeft($1->getType(),"variable : ID LSQUARE expression RSQUARE");
		// fprintf(logout,"variable : ID LSQUARE expression RSQUARE\n");
		lookForVariable($1,$1->getStartLine(), false);
		if($1->getType()!="ID" && !$1->isArray()){fprintf(errorout,"Line# %d: '%s' is not an array\n", $1->getStartLine(),$1->getName().c_str()); err_count++;}
		else if($3->getType() != "INT") {fprintf(errorout,"Line# %d: Array subscript is not an integer\n", $3->getStartLine()); err_count++;}
		$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3); $$->addChild(symbolToParse($4));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
	 } 
	 ;
	 
expression : logic_expression{
			$$=setLeft($1->getType(),"expression : logic_expression");
			// fprintf(logout,"expression : logic_expression\n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
	 		}
	   		| variable ASSIGNOP logic_expression{ // what will be the type of $$
			// fprintf(logout,"expression : variable ASSIGNOP logic_expression \n");
			if($1->getType() == "INT" and $3->getType() == "FLOAT"){
				fprintf(errorout,"Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n", $1->getStartLine());
				err_count++;
			}
			if(checkVoidInExpression($1->getType(),$1->getStartLine())){
				$1->setType("INT");
			}
			if(checkVoidInExpression($3->getType(),$3->getStartLine())){
				$3->setType("INT");
			}
			$$=setLeft($1->getType(),"expression : variable ASSIGNOP logic_expression");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
	   		}	
	   		;
			
logic_expression : rel_expression 	{
			$$=setLeft($1->getType(),"logic_expression : rel_expression");
			// fprintf(logout,"logic_expression : rel_expression 	 \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			 | rel_expression LOGICOP rel_expression {
			$$=setLeft("INT","logic_expression : rel_expression LOGICOP rel_expression");
			// fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression\n");
			if(checkVoidInExpression($1->getType(),$1->getStartLine())){
				$1->setType("INT");
			}
			if(checkVoidInExpression($3->getType(),$3->getStartLine())){
				$3->setType("INT");
			}
			
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
		 	}
		 	;
			
rel_expression	: simple_expression {
			$$=setLeft($1->getType(),"rel_expression : simple_expression");
			// fprintf(logout,"rel_expression : simple_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

			}
			| simple_expression RELOP simple_expression	{
			$$=setLeft("INT","rel_expression : simple_expression RELOP simple_expression");
			// fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");
			if(checkVoidInExpression($1->getType(),$1->getStartLine())){
				$1->setType("INT");
			}
			if(checkVoidInExpression($3->getType(),$3->getStartLine())){
				$3->setType("INT");
			}
			
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
		}
		;
				
simple_expression : term {
			$$=setLeft($1->getType(),"simple_expression : term");
			// fprintf(logout,"simple_expression : term \n");
			$$->addChild($1);
			$$->setEndLine($1->getEndLine());

			}	
		  	| simple_expression ADDOP term {
			// fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");
			if($1->getType() == "INT" and $3->getType() == "FLOAT"){
				$$->setType("FLOAT");
			}
			if(checkVoidInExpression($1->getType(),$1->getStartLine())){
				$1->setType("INT");
			}
			if(checkVoidInExpression($3->getType(),$3->getStartLine())){
				$3->setType("INT");
			}
			$$=setLeft($1->getType(),"simple_expression : simple_expression ADDOP term");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
		  }
		  ;
					
term :	unary_expression {
			$$=setLeft($1->getType(),"term : unary_expression");
			// fprintf(logout,"term :	unary_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
     		|  term MULOP unary_expression {
			$$=setLeft($1->getType(),"term : term MULOP unary_expression");
			if($1->getType() == "INT" and $3->getType() == "FLOAT"){
				$$->setType("FLOAT");
			}
			if(($1->getType() == "FLOAT" or $3->getType() == "FLOAT") and ($2->getName()=="%")){
				err_count++;
				fprintf(errorout,"Line# %d: Operands of modulus must be integers\n", $1->getStartLine());
				$$->setType("INT");
			}
			if(checkVoidInExpression($1->getType(),$1->getStartLine())){
				$1->setType("INT");
				$$->setType("INT");
			}
			if(checkVoidInExpression($3->getType(),$3->getStartLine())){
				$3->setType("INT");
			}
			string s(yytext);
			if(s[0] == '0' and stof(s)==0 and ($2->getName()=="%" || $2->getName()=="/")){
				err_count++;
				fprintf(errorout,"Line# %d: Warning: division by zero i=0f=1Const=0\n", $1->getStartLine());
			}
			// fprintf(logout,"term : term MULOP unary_expression \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
	 		}
     		;

unary_expression : ADDOP unary_expression  {
			if(checkVoidInExpression($2->getType(),$2->getStartLine())){
				$2->setType("INT");
			}
			$$=setLeft($2->getType(),"unary_expression : ADDOP unary_expression");
			// fprintf(logout,"unary_expression : ADDOP unary_expression  \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
		 	| NOT unary_expression {
			if(checkVoidInExpression($2->getType(),$2->getStartLine())){
				$2->setType("INT");
			}
			$$=setLeft($2->getType(),"unary_expression : NOT unary_expression");
			// fprintf(logout,"unary_expression : NOT unary_expression \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
		 	}
		 	| factor {
			$$=setLeft($1->getType(),"unary_expression : factor");
			// fprintf(logout,"unary_expression : factor \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
		 	} 
		 	;
	
factor	: variable {
			$$=setLeft($1->getType(),"factor : variable");
			// fprintf(logout,"factor : variable  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| ID LPAREN argument_list RPAREN {
			string testType;
			if($1->isFunc()){
				testType = $1->getExtra()->getReturnType();				
			}
			else{
				testType = $1->getType();
			}
			$$=setLeft(testType,"factor	: ID LPAREN argument_list RPAREN");
			// fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
			lookForVariable($1,$1->getStartLine(), true);
			functionChecker($1,typeList);
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3); $$->addChild(symbolToParse($4));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($4->getEndLine());
			}
			| LPAREN expression RPAREN {
			$$=setLeft($2->getType(),"factor : LPAREN expression RPAREN");
			// fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2); $$->addChild(symbolToParse($3));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
			}
			| CONST_INT {
			$$=setLeft("INT","factor : CONST_INT");
			// fprintf(logout,"factor : CONST_INT\n");
			value = atoi($1->getName().c_str());
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| CONST_FLOAT {
			$$=setLeft("FLOAT","factor : CONST_FLOAT");
			// fprintf(logout,"factor	: CONST_FLOAT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| variable INCOP {
			$$=setLeft($1->getType(),"factor : variable INCOP");
			// fprintf(logout,"factor	: variable INCOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
			| variable DECOP {
			$$=setLeft($1->getType(),"factor : variable DECOP");
			// fprintf(logout,"factor	: variable DECOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
			;
	
argument_list : arguments{
			$$=setLeft($1->getType(),"argument_list : arguments");
			// fprintf(logout,"argument_list : arguments  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			  | {
			$$=setLeft("VOID","argument_list :");
			// fprintf(logout,"argument_list :\n");
			$$->addChild(new ParseTree("","",line_count));
			$$->setStartLine(line_count);
			$$->setEndLine(line_count);
			}
			;
	
arguments : arguments COMMA logic_expression {
			$$=setLeft($1->getType(),"arguments : arguments COMMA logic_expression");
			typeList.push_back($3->getType());
			// fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
			}
	      | logic_expression {
			typeList.push_back($1->getType());
			$$=setLeft($1->getType(),"arguments : logic_expression");
			// fprintf(logout,"arguments : logic_expression\n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
		  }
	      ;
%%
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