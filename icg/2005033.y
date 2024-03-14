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
FILE *checkerout;
FILE *codeout;
int offset = 0;
int wordSize = 0;
SymbolTable *table;
SymbolInfo *temp;

string type;

string printFunction(){
	const char *str =
	"print_output PROC NEAR\n"
    "\tPUSH BP\n"
    "\tMOV BP, SP\n" 
    "\tMOV BX, 0\n"
	"PRINT_1:\n"
    "\tMOV DX, 0\n"
    "\tMOV CX, 10\n"
    "\tDIV CX\n"
    "\tPUSH DX\n"
    "\tINC BX\n"
    "\tCMP AX, 0\n"
    "\tJG PRINT_1\n"
	"PRINT_2:\n"
    "\tPOP DX\n"
    "\tADD DX, 30H\n"
    "\tDEC BX\n"
    "\tMOV AL, 0\n" 
    "\tMOV AH, 2\n"
    "\tINT 21H\n"
    "\tCMP BX, 0\n"
    "\tJG PRINT_2\n"
    "\tPOP BP\n"
    "\tRET 0\n";
	string result(str);
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
		offset += 2*size;
		cout<<p->getType()<<" "<<offset<<endl;
		s->setOffset(offset);
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
			offset += 2;
			cout<<p->getType()<<" "<<offset<<endl;
			s->setOffset(offset);
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
	return "";
}
string processFactor(ParseTree *p){ // the ans should be at AX
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "factor : variable")
		return processVariable(childs[0]);
	else if(p->getLine() == "factor : CONST_INT")
		return "\tMOV AX, "+childs[0]->getType()+'\n';
	return "";
}

string processUnaryExpression(ParseTree *p){ // the ans should be at AX
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "unary_expression : factor")
		return processFactor(childs[0]);
	return "";
}

string processTerm(ParseTree *p){ // the ans should be at AX
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "term : unary_expression")
		return processUnaryExpression(childs[0]);
	return "";
}

string processSimpleExpression(ParseTree *p){ // the ans should be at AX
	if(p->getLine() == "simple_expression : term")
		return processTerm(p->getChild()[0]);
	return "";
}

string processRelExpression(ParseTree *p){ // the ans should be at AX
	if(p->getLine() == "rel_expression : simple_expression")
		return processSimpleExpression(p->getChild()[0]);
	return "";
}

string processLogicExpression(ParseTree *p){ // the ans should be at AX
	if(p->getLine() == "logic_expression : rel_expression")
		return processRelExpression(p->getChild()[0]);
	return "";
}

string extractIndexForArray(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	SymbolInfo *temp = table->Lookup(childs[0]->getType());
	string result = "";
	if(p->getLine()  == "variable : ID LSQUARE expression RSQUARE"){
		result += processLogicExpression(childs[2]);
		result +="\tPUSH AX\n";
	}
	return result;
}

string processVariableAssignExpression(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	SymbolInfo *temp = table->Lookup(childs[0]->getType());
	string result = "";
	if(p->getLine()  == "variable : ID"){
		if(temp->getOffset() == 0) return "\tMOV "+temp->getName()+", AX\n";
		else return "\tMOV [BP-"+to_string(temp->getOffset())+"], AX\n";
	}
	else{
		cout<<temp->getName()<<"-->"<<temp->getOffset()<<endl;
		if(temp->getOffset() == 0) {
			result += "\tPOP BX\n\tPUSH AX\n"; //the index is at BX and the value is at AX
			result += "\tMOV AX, 2\n\tMUL BX\n"; //the index*2 is the location of the word
			result += "\tPOP AX\n\tMOV "+temp->getName()+"[BX], AX\n";	
		}
		else{
			// here array[0] is at offset, so I have to calculate accordingly
			result += "\tPOP BX\n\tPUSH AX\n"; //the index is at BX and the value is at AX
			result += "\tMOV AX, 2\n\tMUL BX\n"; //the index*2 is the location of the word
			result += "\tMOV BX, AX\n\tMOV AX, "+to_string(temp->getOffset());
			result += "\n\tSUB AX, BX\n\tMOV BX, AX\n";
			result += "\tPOP AX\n\tMOV SI, BX\n";
			result += "\tNEG SI\n\tMOV [BP+SI], AX\n";	
		}
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
		result += processVariableAssignExpression(temp->getChild()[0]);
	}
	return result;
}

string processPrint(ParseTree *p){
	SymbolInfo *temp = table->Lookup(p->getType());
	if(temp->getOffset() == 0){
		return "\tMOV AX, "+p->getType()+"\n\tCALL print_output\n";
	}
	else {
		return "\tMOV AX, [BP - "+to_string(temp->getOffset())+"]\n\tCALL print_output\n";
	}
}
string processSingleStatement(ParseTree *p){
	vector<ParseTree *> childs = p->getChild();
	if(p->getLine() == "statement : var_declaration")
		return processVarDeclaration(childs[0]); // need to check whether pushing bp requires for all declaration
	else if(p->getLine() == "statement : expression_statement")
		return processExpression(childs[0]);
	else if(p->getLine() == "statement : PRINTLN LPAREN ID RPAREN SEMICOLON")
		return processPrint(childs[2]);
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
	string result = ".CODE\nmain PROC\n\tMOV AX, @DATA\n\tMOV DS, AX\n";
	result+="\tPUSH BP\n\tMOV BP, SP\n";
	ParseTree *child = p->getChild().back();
	if(child->getChildCount() == 3) // checking if the compound statement is LCURL statement RCURL
		result += processStatements(child->getChild()[1]);
	result+="\tADD SP, "+to_string(offset)+"\n\tPOP BP\n";
	result+="\tMOV AX,4CH\n\tINT 21H\n";
	result+= "main ENDP\n"+printFunction();
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

void generateCode(ParseTree *p){
    fprintf(codeout,".MODEL SMALL\n.STACK 1000H\n.DATA\n\tnumber DB \"00000$\"\n");
    vector<ParseTree *> units;
    extractUnits(p,units);
	//for this time I am considering main to be only func definition
    for(auto v: units){
		if(v->getLine()=="unit : var_declaration"){
			fprintf(codeout,"%s\n", addVariable(v).c_str());
		}
		else if(v->getLine()=="unit : func_definition"){
			fprintf(codeout,"%s\n", buildMain(v->getChild()[0]).c_str());
		}
    }
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
		// cout<<valid<<endl;
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
	// fprintf(checkerout,"%d %s %d\n", line_count, s->getName().c_str(),define); // define will be also used as
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
	// cout<<s->getType()<<endl;
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
			$$ = setLeft("parameter_list","parameter_list  : type_specifier ID");
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
			// cout<<s;
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
			$$=setLeft($2->getType(),"factor	: LPAREN expression RPAREN");
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
			$$=setLeft("FLOAT","factor	: CONST_FLOAT");
			// fprintf(logout,"factor	: CONST_FLOAT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| variable INCOP {
			$$=setLeft($1->getType(),"factor	: variable INCOP");
			// fprintf(logout,"factor	: variable INCOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
			| variable DECOP {
			$$=setLeft($1->getType(),"factor	: variable DECOP");
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
	
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout= fopen("log.txt","w");
	errorout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt","w");
	codeout = fopen("code.txt", "w");
	
	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	/* fprintf(logout,"Total Lines: %d\nTotal Errors: %d\n", line_count,err_count); */
	
	delete table;
	fclose(codeout);
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}