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
extern int start_line;
bool infunc = false;
bool sayArray = false;
MyStack stack1, stack2;

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
	fprintf(logout,"Error at line %d: %s\n",line_count,s);
	infunc = false;	
	stack1.cleanUp();
	stack2.cleanUp();
}

FuncExtras *addExtraInFunc(string type, bool defined){
	vector<Pair *> p;
	SymbolInfo *s;
	// fprintf(checkerout,"%d",stack2.size());
	while(!stack2.isEmpty()){
		s = stack2.pop() ;
		p.push_back(new Pair(s->getName(), s->getType()));
		s = nullptr;
	}
	return new FuncExtras(type,p,defined);
}

ParseTree* setLeft(string type, string line){
	return new ParseTree(type,line,line_count);
}

ParseTree* symbolToParse(SymbolInfo *s){
	if((s->getType()=="INT" or s->getType()=="FLOAT" or s->getType()=="VOID" or s->getType()=="FUNCTION") && !(s->getName()=="int" || s->getName()=="float" || s->getName()=="void"))
		return new ParseTree("", "ID : "+s->getName(), s->getLine());
	else
		return new ParseTree("", s->getType()+" : "+s->getName(), s->getLine());
}

void insertParameters(){
	if(infunc){
		SymbolInfo *s;
		while(!stack1.isEmpty()){
			s = stack1.pop() ;
			table->Insert(s->getName(),s->getType());
			delete s;
			s = nullptr;
		}
		infunc = false;
	}
}

// error checking functions
void conflictingType(SymbolInfo *s, int line, bool isfunc, string type1, bool define){ // boolean is func is used so that if we get ID type we may use 
	// fprintf(checkerout,"%s %d %d\n",s->getType().c_str(),s->isFunc(),isfunc);
	if(s->isFunc() and !isfunc){
		if(!table->Insert(s->getName(),type)){
			fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",line,s->getName().c_str());
			err_count++;
		}
	}
	else if(!s->isFunc() and isfunc){
		// fprintf(checkerout,"%s %d %d\n",s->getType().c_str(),s->isFunc(),isfunc);
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
	else if(sayArray && table->Insert(s->getName(),type)){
		temp = table->Lookup(s->getName());
		temp->setArray();
		sayArray = false;
	}
	else if(!table->Insert(s->getName(),type)){
		fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",line,s->getName().c_str());
		err_count++;
	}
	
}
void lookForVariable(SymbolInfo *s, int line){
	if(table->Lookup(s->getName())==nullptr){
		fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",line,s->getName().c_str());
		err_count++;
	}
}
void leftRightTypeChecking(){

}

void indexIsInt(){

}

void intModint(SymbolInfo *op){

}

void functionArgumentCheck(){

}

void voidFuncCheck(){

}

void uniquenessCheck(){

}

void arrayIndexCheck(){

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
		fprintf(logout,"start : program \n");
		$$ = setLeft("start","start : program");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->printTree($$,0,parseout);
	}
	;

program : program unit {
		fprintf(logout,"program : program unit \n");
		$$ = setLeft("program","program : program unit");
		$$->addChild($1); 
		$$->addChild($2); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());

	}
	| unit{
		fprintf(logout,"program : unit \n");
		$$ = setLeft("program","program : unit");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	}
	;
	
unit : var_declaration{
		fprintf(logout,"unit : var_declaration  \n");
		$$ = setLeft("unit","unit : var_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	}
     | func_declaration{
		fprintf(logout,"unit : func_declaration  \n");
		$$ = setLeft("unit","unit : func_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	 }
     | func_definition{
		fprintf(logout,"unit : func_definition  \n");
		$$ = setLeft("unit","unit : func_definition");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	 }
     ;
	
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),false);
			// temp = $2;
			// table->Insert(temp->getName(),"FUNCTION", addExtraInFunc($1->getType(),false));
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild($4); $$->addChild(symbolToParse($5)); $$->addChild(symbolToParse($6));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($6->getEndLine());	
			stack1.cleanUp();
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),false);
			// temp = $2;
			// table->Insert(temp->getName(),"FUNCTION",addExtraInFunc($1->getType(),false));
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN{infunc = true;} compound_statement{ //
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),true);
			// temp = $2;
			// table->Insert(temp->getName(),"FUNCTION",addExtraInFunc($1->getType(),true));
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild($4); $$->addChild(symbolToParse($5)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());
				
		}
		| type_specifier ID LPAREN RPAREN{infunc = true;} compound_statement{
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
			conflictingType($2,$1->getStartLine(),true,$1->getType(),true);
			// temp = $2;
			// table->Insert(temp->getName(),"FUNCTION",addExtraInFunc($1->getType(),true));
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild($6);		
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($6->getEndLine());			
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID{
			$$ = setLeft("parameter_list","parameter_list : parameter_list COMMA type_specifier ID");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
			temp = $4;
			stack1.push(new SymbolInfo(temp->getName(), type));
			stack2.push(new SymbolInfo(temp->getName(), type));
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->addChild($3); $$->addChild(symbolToParse($4));
			$$->setStartLine($1->getStartLine());	
			$$->setEndLine($4->getEndLine());
		}
		| parameter_list COMMA type_specifier{
			$$ = setLeft("parameter_list", "parameter_list : parameter_list COMMA type_specifier");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
			stack1.push(new SymbolInfo("", type));
			stack2.push(new SymbolInfo("", type));
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
		}
 		| type_specifier ID{
			$$ = setLeft("parameter_list","parameter_list  : type_specifier ID");
			fprintf(logout,"parameter_list  : type_specifier ID\n");
			temp = $2;
			stack1.push(new SymbolInfo(temp->getName(), type));
			stack2.push(new SymbolInfo(temp->getName(), type));
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
		}
		| type_specifier{
			$$ = setLeft("parameter_list","parameter_list  : type_specifier");
			fprintf(logout,"parameter_list  : type_specifier\n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
		}
 		;

 		
compound_statement : LCURL{insertParameters();} statements RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL statements RCURL");
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				$$->addChild(symbolToParse($1)); $$->addChild($3); $$->addChild(symbolToParse($4));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($4->getEndLine());			
			}
 		    | LCURL{insertParameters();} RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL RCURL");
				fprintf(logout,"compound_statement : LCURL RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($3));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($3->getEndLine());
			}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		$$ = setLeft("var_declaration","var_declaration : type_specifier declaration_list SEMICOLON");
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		$$->addChild($1); $$->addChild($2); $$->addChild(symbolToParse($3)); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		}
 		 ;
 		 
type_specifier	: INT {
		$$ = setLeft("INT", "type_specifier : INT");fprintf(logout,"type_specifier : INT \n"); type = "INT"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		| FLOAT {
		$$ = setLeft("FLOAT", "type_specifier : FLOAT");fprintf(logout,"type_specifier : FLOAT \n"); type = "FLOAT"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		| VOID {
		$$ = setLeft("VOID", "type_specifier : VOID");fprintf(logout,"type_specifier : VOID \n"); type = "VOID"; 
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
 		;
 		
declaration_list : declaration_list COMMA ID{
				$$=setLeft("declaration_list","declaration_list : declaration_list COMMA ID");
				fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
				conflictingType($3,$1->getStartLine(),false,type,false);				
				// temp = $3;
				// table->Insert(temp->getName(),type);
				$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($3->getEndLine());
				}
 		  		| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
				conflictingType($3,$1->getStartLine(),false,type,false);				
				// temp = $3;
				// table->Insert(temp->getName(),type);
				$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
				$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5)); $$->addChild(symbolToParse($6)); 
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($6->getEndLine());	
				}
 		  		| ID{
				$$=setLeft("declaration_list", "declaration_list : ID");
				fprintf(logout,"declaration_list : ID \n");
				conflictingType($1,$1->getStartLine(),false,type,false);				
				// temp = $1;
				// table->Insert(temp->getName(),type);
				$$->addChild(symbolToParse($1));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($1->getEndLine());
	
		  		}
 		  		| ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list : ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
				sayArray = true;				
				conflictingType($1,$1->getStartLine(),false,type,false);
				// temp = $1;
				// table->Insert(temp->getName(),type);
				$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3)); $$->addChild(symbolToParse($4));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine($4->getEndLine());	
		  		}	
 		  ;
 		  
statements : statement{
		$$=setLeft("statements","statements : statement");
		fprintf(logout,"statements : statement  \n");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		}
	   | statements statement{
		$$=setLeft("statements","statements : statements statement");
		fprintf(logout,"statements : statements statement  \n");
		$$->addChild($1); $$->addChild($2);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
	   }
	   ;
	   
statement : var_declaration {
			$$=setLeft("statement","statement : var_declaration");
			fprintf(logout,"statement : var_declaration \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

			}
	  		| expression_statement{
			$$=setLeft("statement","statement : expression_statement");
			fprintf(logout,"statement : expression_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

	  		}
	  		| compound_statement{
			$$=setLeft("statement","statement : compound_statement");
			fprintf(logout,"statement : compound_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

	  		}
	  		| FOR LPAREN expression_statement expression_statement expression RPAREN statement{
			$$=setLeft("statement","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild($4); $$->addChild($5); $$->addChild(symbolToParse($6)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());

	  		}
	  		| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE ;{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());

	  		}
	  		| IF LPAREN expression RPAREN statement ELSE statement{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement ELSE statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5); $$->addChild(symbolToParse($6)); $$->addChild($7);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($7->getEndLine());

	  		}
	  		| WHILE LPAREN expression RPAREN statement{
			$$=setLeft("statement","statement : WHILE LPAREN expression RPAREN statement");
			fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->addChild(symbolToParse($4)); $$->addChild($5); 
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
	  		}
	  		| PRINTLN LPAREN ID RPAREN SEMICOLON{
			$$=setLeft("statement","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");			
			fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
			lookForVariable($3,$1->getStartLine());
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild(symbolToParse($3));
			$$->addChild(symbolToParse($4)); $$->addChild(symbolToParse($5)); 
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($5->getEndLine());
	  		}
	  		| RETURN expression SEMICOLON{
			$$=setLeft("statement","statement : RETURN expression SEMICOLON");
			fprintf(logout,"statement : RETURN expression SEMICOLON  \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2); $$->addChild(symbolToParse($3));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
	  		}
			;
	  
expression_statement : SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : SEMICOLON");
			fprintf(logout,"expression_statement : SEMICOLON\n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}			
			| expression SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : expression SEMICOLON");
			fprintf(logout,"expression_statement : expression SEMICOLON\n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			} 
			;
	  
variable : ID{
		$$=setLeft("variable","variable : ID");
		fprintf(logout,"variable : ID 	 \n");
		lookForVariable($1,$1->getStartLine());
		$$->addChild(symbolToParse($1));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		} 		
	 	| ID LTHIRD expression RTHIRD{
		$$=setLeft("variable","variable : ID LSQUARE expression RSQUARE");
		fprintf(logout,"variable : ID LSQUARE expression RSQUARE  	 \n");
		lookForVariable($1,$1->getStartLine());
		$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3); $$->addChild(symbolToParse($4));
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
	 } 
	 ;
	 
expression : logic_expression{
			$$=setLeft("expression","expression 	: logic_expression");
			fprintf(logout,"expression 	: logic_expression	 \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
	 		}
	   		| variable ASSIGNOP logic_expression{
			$$=setLeft("expression","expression 	: variable ASSIGNOP logic_expression");
			fprintf(logout,"expression 	: variable ASSIGNOP logic_expression 		 \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());

	   		}	
	   		;
			
logic_expression : rel_expression 	{
			$$=setLeft("logic_expression","logic_expression : rel_expression");
			fprintf(logout,"logic_expression : rel_expression 	 \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

			}
			 | rel_expression LOGICOP rel_expression {
			$$=setLeft("logic_expression","logic_expression : rel_expression LOGICOP rel_expression");
			fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());

		 	}
		 	;
			
rel_expression	: simple_expression {
			$$=setLeft("rel_expression","rel_expression	: simple_expression");
			fprintf(logout,"rel_expression	: simple_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());

			}
			| simple_expression RELOP simple_expression	{
			$$=setLeft("rel_expression","rel_expression	: simple_expression RELOP simple_expression");
			fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());

		}
		;
				
simple_expression : term {
			$$=setLeft("simple_expression","simple_expression : term");
			fprintf(logout,"simple_expression : term \n");
			$$->addChild($1);
			$$->setEndLine($1->getEndLine());

			}	
		  	| simple_expression ADDOP term {
			$$=setLeft("simple_expression","simple_expression :	simple_expression ADDOP term");
			fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());

		  }
		  ;
					
term :	unary_expression {
			$$=setLeft("term","term :	unary_expression");
			fprintf(logout,"term :	unary_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
     		|  term MULOP unary_expression {
			$$=setLeft("term","term : term MULOP unary_expression");
			fprintf(logout,"term : term MULOP unary_expression \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());

	 }
     ;

unary_expression : ADDOP unary_expression  {
			$$=setLeft("unary_expression","unary_expression : ADDOP unary_expression");
			fprintf(logout,"unary_expression : ADDOP unary_expression  \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
		 	| NOT unary_expression {
			$$=setLeft("unary_expression","unary_expression : NOT unary_expression");
			fprintf(logout,"unary_expression : NOT unary_expression \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
		 	}
		 	| factor {
			$$=setLeft("unary_expression","unary_expression : factor");
			fprintf(logout,"unary_expression : factor \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
		 	} 
		 	;
	
factor	: variable {
			$$=setLeft("factor","factor : variable");
			fprintf(logout,"factor : variable  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| ID LPAREN argument_list RPAREN {
			$$=setLeft("factor","factor	: ID LPAREN argument_list RPAREN");
			fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
			lookForVariable($1,$1->getStartLine());
			$$->addChild(symbolToParse($1)); $$->addChild(symbolToParse($2)); $$->addChild($3); $$->addChild(symbolToParse($4));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($4->getEndLine());
			}
			| LPAREN expression RPAREN {
			$$=setLeft("factor","factor	: LPAREN expression RPAREN");
			fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2); $$->addChild(symbolToParse($3));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
			}
			| CONST_INT {
			$$=setLeft("factor","factor	: CONST_INT");
			fprintf(logout,"factor	: CONST_INT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| CONST_FLOAT {
			$$=setLeft("factor","factor	: CONST_FLOAT");
			fprintf(logout,"factor	: CONST_FLOAT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			| variable INCOP {
			$$=setLeft("factor","factor	: variable INCOP");
			fprintf(logout,"factor	: variable INCOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
			| variable DECOP {
			$$=setLeft("factor","factor	: variable DECOP");
			fprintf(logout,"factor	: variable DECOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($2->getEndLine());
			}
			;
	
argument_list : arguments{
			$$=setLeft("argument_list","argument_list : arguments");
			fprintf(logout,"argument_list : arguments  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($1->getEndLine());
			}
			  | {
			$$=setLeft("argument_list","argument_list :");
			fprintf(logout,"argument_list :\n");
			$$->addChild(new ParseTree("","",line_count));
			$$->setStartLine(line_count);
			$$->setEndLine(line_count);
			}
			;
	
arguments : arguments COMMA logic_expression {
			$$=setLeft("arguments","arguments : arguments COMMA logic_expression");
			fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine($3->getEndLine());
			}
	      | logic_expression {
			$$=setLeft("arguments","arguments : logic_expression");
			fprintf(logout,"arguments : logic_expression\n");
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
	checkerout = fopen("checker.txt", "w");

	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	fprintf(logout,"Total Lines: %d\nTotal Errors: %d\n", line_count,err_count);
	
	delete table;
	fclose(checkerout);
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}