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
extern bool checkStart;
bool infunc;
MyStack stack1;

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
	fprintf(errorout,"Error at line %d: %s\n",line_count,s);
	infunc = false;
	stack1.cleanUp();
}

// FuncExtras *addExtraInFunc(string type, bool defined){
// 	vector<string> p;
// 	SymbolInfo *s;
// 	while(!stack1.isEmpty()){
// 		s = stack1.pop() ;
// 		p.push_back(s->getType());
// 		s = nullptr;
// 	}
// 	return new FuncExtras(type,p,defined);
// }

ParseTree* setLeft(string type, string line){
	return new ParseTree(type,line);
}

ParseTree* symbolToParse(SymbolInfo *s){
	return new ParseTree("", s->getType()+" : "+s->getName());
}

void insertParameters(){
	{
	if(infunc){
		SymbolInfo *s;
		while(!stack1.isEmpty()){
			s = stack1.pop() ;
			table->Insert(s->getName(),s->getType());
			// delete s;
			s = nullptr;
		}
		infunc = false;
	}
}
}

// error checking functions
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
		$$->setEndLine(line_count);
		checkStart = true;
		$$->printTree($$,0,parseout);
	}
	;

program : program unit {
		fprintf(logout,"program : program unit \n");
		$$ = setLeft("program","program : program unit");
		$$->addChild($1); 
		$$->addChild($2); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;

	}
	| unit{
		fprintf(logout,"program : unit \n");
		$$ = setLeft("program","program : unit");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
	}
	;
	
unit : var_declaration{
		fprintf(logout,"unit : var_declaration  \n");
		$$ = setLeft("unit","unit : var_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
	}
     | func_declaration{
		fprintf(logout,"unit : func_declaration  \n");
		$$ = setLeft("unit","unit : func_declaration");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
	 }
     | func_definition{
		fprintf(logout,"unit : func_definition  \n");
		$$ = setLeft("unit","unit : func_definition");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
	 }
     ;
	
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(new ParseTree("","LPAREN : ("));
			$$->addChild($4); $$->addChild(new ParseTree("","RPAREN : )")); $$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			infunc = false;
			stack1.cleanUp();
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			$$ = setLeft("func_declaration","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(new ParseTree("","LPAREN : ("));
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement{
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(new ParseTree("","LPAREN : ("));
			$$->addChild($4); $$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($6);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			infunc = false;	
		}
		| type_specifier ID LPAREN RPAREN compound_statement{
			$$ = setLeft("func_definition","func_definition : type_specifier ID LPAREN RPAREN compound_statement ");
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild(new ParseTree("","LPAREN : ("));
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($5);		
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			infunc = false;
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID{
			infunc = true;
			$$ = setLeft("parameter_list","parameter_list : parameter_list COMMA type_specifier ID");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
			temp = $4;
			stack1.push(new SymbolInfo(temp->getName(), type));
			$$->addChild($1); $$->addChild(new ParseTree("","COMMA : ,"));
			$$->addChild($3); $$->addChild(symbolToParse($4));
			$$->setStartLine($1->getStartLine());	
			$$->setEndLine(line_count);
			checkStart = true;
		}
		| parameter_list COMMA type_specifier{
			$$ = setLeft("parameter_list", "parameter_list : parameter_list COMMA type_specifier");
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
			stack1.push(new SymbolInfo("", type));
			$$->addChild($1); $$->addChild(new ParseTree("","COMMA : ,")); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		}
 		| type_specifier ID{
			infunc = true;
			$$ = setLeft("parameter_list","parameter_list  : type_specifier ID");
			fprintf(logout,"parameter_list  : type_specifier ID\n");
			temp = $2;
			stack1.push(new SymbolInfo(temp->getName(), type));
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		}
		| type_specifier{
			$$ = setLeft("parameter_list","parameter_list  : type_specifier");
			fprintf(logout,"parameter_list  : type_specifier\n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		}
 		;

 		
compound_statement : LCURL statements RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL statements RCURL");
				if($$ == nullptr)
				cout<<"Null ";
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				$$->addChild(new ParseTree("","LCURL : {")); $$->addChild($2); $$->addChild(new ParseTree("","RCURL : }"));
				$$->setStartLine(start_line);
				$$->setEndLine(line_count);
				checkStart = true;		
			}
 		    | LCURL RCURL{
				$$ = setLeft("compound_statement","compound_statement : LCURL RCURL");
				fprintf(logout,"compound_statement : LCURL RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
				$$->addChild(new ParseTree("","LCURL : {")); $$->addChild(new ParseTree("","RCURL : }"));
				$$->setStartLine(start_line);
				$$->setEndLine(line_count);
				checkStart = true;
			}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		$$ = setLeft("var_declaration","var_declaration : type_specifier declaration_list SEMICOLON");
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		$$->addChild($1); $$->addChild($2); $$->addChild(new ParseTree("","SEMICOLON : ;")); 
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
		}
 		 ;
 		 
type_specifier	: INT {
		$$ = setLeft("type_specifier", "type_specifier : INT");fprintf(logout,"type_specifier : INT \n"); type = "INT"; 
		$$->addChild(new ParseTree("INT","INT : int"));
		$$->setStartLine(start_line);
		$$->setEndLine(line_count);
		checkStart = true;
		}
 		| FLOAT {
		$$ = setLeft("type_specifier", "type_specifier : FLOAT");fprintf(logout,"type_specifier : FLOAT \n"); type = "FLOAT"; 
		$$->addChild(new ParseTree("INT","FLOAT : float"));
		$$->setStartLine(start_line);
		$$->setEndLine(line_count);
		checkStart = true;
		}
 		| VOID {
		$$ = setLeft("type_specifier", "type_specifier : VOID");fprintf(logout,"type_specifier : VOID \n"); type = "VOID"; 
		$$->addChild(new ParseTree("VOID","VOID : void"));
		$$->setStartLine(start_line);
		$$->setEndLine(line_count);
		checkStart = true;
		}
 		;
 		
declaration_list : declaration_list COMMA ID{
				$$=setLeft("declaration_list","declaration_list : declaration_list COMMA ID");
				fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
				temp = $3;
				table->Insert(temp->getName(),type);
				$$->addChild($1); $$->addChild(new ParseTree("","COMMA : ,")); $$->addChild(symbolToParse($3));
				$$->setStartLine($1->getStartLine());
				$$->setEndLine(line_count);
				checkStart = true;
				}
 		  		| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
				temp = $3;
				table->Insert(temp->getName(),type);
				$$->addChild($1); $$->addChild(new ParseTree("","COMMA : ,")); $$->addChild(symbolToParse($3));
				$$->addChild(new ParseTree("","LTHIRD : [")); $$->addChild(symbolToParse($5)); $$->addChild(new ParseTree("","RTHIRD : ]")); 
				$$->setStartLine($1->getStartLine());
				$$->setEndLine(line_count);
				checkStart = true;
				}
 		  		| ID{
				$$=setLeft("declaration_list", "declaration_list : ID");
				fprintf(logout,"declaration_list : ID \n");
				temp = $1;
				table->Insert(temp->getName(),type);
				$$->addChild(symbolToParse($1));
				$$->setStartLine(start_line);
				$$->setEndLine(line_count);
				checkStart = true;
		  		}
 		  		| ID LTHIRD CONST_INT RTHIRD{
				$$=setLeft("declaration_list","declaration_list : ID LSQUARE CONST_INT RSQUARE");
				fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
				temp = $1;
				table->Insert(temp->getName(),type);
				$$->addChild(symbolToParse($1)); $$->addChild(new ParseTree("","LTHIRD : [")); $$->addChild(symbolToParse($2)); $$->addChild(new ParseTree("","RTHIRD : ]"));
				$$->setStartLine(start_line);
				$$->setEndLine(line_count);
				checkStart = true;
		  }
 		  ;
 		  
statements : statement{
		$$=setLeft("statements","statements : statement");
		fprintf(logout,"statements : statement  \n");
		$$->addChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
		insertParameters();
		}
	   | statements statement{
		$$=setLeft("statements","statements : statements statement");
		fprintf(logout,"statements : statements statement  \n");
		$$->addChild($1); $$->addChild($2);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine(line_count);
		checkStart = true;
		insertParameters();
	   }
	   ;
	   
statement : var_declaration {
			$$=setLeft("statement","statement : var_declaration");
			fprintf(logout,"statement : var_declaration \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			}
	  		| expression_statement{
			$$=setLeft("statement","statement : expression_statement");
			fprintf(logout,"statement : expression_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
	  		}
	  		| compound_statement{
			$$=setLeft("statement","statement : compound_statement");
			fprintf(logout,"statement : compound_statement  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
	  		}
	  		| FOR LPAREN expression_statement expression_statement expression RPAREN statement{
			$$=setLeft("statement","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
			$$->addChild(new ParseTree("","FOR : for")); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($3);
			$$->addChild($4); $$->addChild($5); $$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($7);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
	  		}
	  		| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE ;{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
			$$->addChild(new ParseTree("","IF : if")); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($3);
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($5);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
	  		}
	  		| IF LPAREN expression RPAREN statement ELSE statement{
			$$=setLeft("statement","statement : IF LPAREN expression RPAREN statement ELSE statement");
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");
			$$->addChild(new ParseTree("","IF : if")); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($3);
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($5); $$->addChild(new ParseTree("","ELSE : else")); $$->addChild($7);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
	  		}
	  		| WHILE LPAREN expression RPAREN statement{
			$$=setLeft("statement","statement : WHILE LPAREN expression RPAREN statement");
			fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement  \n");
			$$->addChild(new ParseTree("","WHILE : while")); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($3);
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild($5);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

	  		}
	  		| PRINTLN LPAREN ID RPAREN SEMICOLON{
			$$=setLeft("statement","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");			
			fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
			$$->addChild(new ParseTree("","PRINTLN : println")); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild(symbolToParse($3));
			$$->addChild(new ParseTree("","RPAREN : )")); $$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

	  		}
	  		| RETURN expression SEMICOLON{
			$$=setLeft("statement","statement : RETURN expression SEMICOLON");
			fprintf(logout,"statement : RETURN expression SEMICOLON  \n");
			$$->addChild(new ParseTree("","RETURN : return")); $$->addChild($2); $$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

	  		}
			;
	  
expression_statement : SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : SEMICOLON");
			fprintf(logout,"expression_statement : SEMICOLON\n");
			$$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
			}			
			| expression SEMICOLON{
			$$=setLeft("expression_statement","expression_statement : expression SEMICOLON");
			fprintf(logout,"expression_statement : expression SEMICOLON\n");
			$$->addChild($1); $$->addChild(new ParseTree("","SEMICOLON : ;"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
			} 
			;
	  
variable : ID{
		$$=setLeft("variable","variable : ID");
		fprintf(logout,"variable : ID 	 \n");
		$$->addChild(symbolToParse($1));
		$$->setStartLine(start_line);
		$$->setEndLine(line_count);
		checkStart = true;
		} 		
	 	| ID LTHIRD expression RTHIRD{
		$$=setLeft("variable","variable : ID LSQUARE expression RSQUARE");
		fprintf(logout,"variable : ID LSQUARE expression RSQUARE  	 \n");
		$$->addChild(symbolToParse($1)); $$->addChild(new ParseTree("","LTHIRD : [")); $$->addChild($3); $$->addChild(new ParseTree("","RTHIRD : ]"));
		$$->setStartLine(start_line);
		$$->setEndLine(line_count);
		checkStart = true;
	 } 
	 ;
	 
expression : logic_expression{
			$$=setLeft("expression","expression 	: logic_expression");
			fprintf(logout,"expression 	: logic_expression	 \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
	 		}
	   		| variable ASSIGNOP logic_expression{
			$$=setLeft("expression","expression 	: variable ASSIGNOP logic_expression");
			fprintf(logout,"expression 	: variable ASSIGNOP logic_expression 		 \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
	   		}	
	   		;
			
logic_expression : rel_expression 	{
			$$=setLeft("logic_expression","logic_expression : rel_expression");
			fprintf(logout,"logic_expression : rel_expression 	 \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			}
			 | rel_expression LOGICOP rel_expression {
			$$=setLeft("logic_expression","logic_expression : rel_expression LOGICOP rel_expression");
			fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		 	}
		 	;
			
rel_expression	: simple_expression {
			$$=setLeft("rel_expression","rel_expression	: simple_expression");
			fprintf(logout,"rel_expression	: simple_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
			}
			| simple_expression RELOP simple_expression	{
			$$=setLeft("rel_expression","rel_expression	: simple_expression RELOP simple_expression");
			fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		}
		;
				
simple_expression : term {
			$$=setLeft("simple_expression","simple_expression : term");
			fprintf(logout,"simple_expression : term \n");
			$$->addChild($1);
			$$->setEndLine(line_count);
			checkStart = true;
			}	
		  	| simple_expression ADDOP term {
			$$=setLeft("simple_expression","simple_expression :	simple_expression ADDOP term");
			fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
		  }
		  ;
					
term :	unary_expression {
			$$=setLeft("term","term :	unary_expression");
			fprintf(logout,"term :	unary_expression \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
     		|  term MULOP unary_expression {
			$$=setLeft("term","term : term MULOP unary_expression");
			fprintf(logout,"term : term MULOP unary_expression \n");
			$$->addChild($1); $$->addChild(symbolToParse($2)); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;
	 }
     ;

unary_expression : ADDOP unary_expression  {
			$$=setLeft("unary_expression","unary_expression : ADDOP unary_expression");
			fprintf(logout,"unary_expression : ADDOP unary_expression  \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

			}
		 	| NOT unary_expression {
			$$=setLeft("unary_expression","unary_expression : NOT unary_expression");
			fprintf(logout,"unary_expression : NOT unary_expression \n");
			$$->addChild(symbolToParse($1)); $$->addChild($2);
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

		 	}
		 	| factor {
			$$=setLeft("unary_expression","unary_expression : factor");
			fprintf(logout,"unary_expression : factor \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

		 	} 
		 	;
	
factor	: variable {
			$$=setLeft("factor","factor : variable");
			fprintf(logout,"factor : variable  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
			| ID LPAREN argument_list RPAREN {
			$$=setLeft("factor","factor	: ID LPAREN argument_list RPAREN");
			fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
			$$->addChild(symbolToParse($1)); $$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($3); $$->addChild(new ParseTree("","RPAREN : )"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
			}
			| LPAREN expression RPAREN {
			$$=setLeft("factor","factor	: LPAREN expression RPAREN");
			fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
			$$->addChild(new ParseTree("","LPAREN : (")); $$->addChild($2); $$->addChild(new ParseTree("","RPAREN : )"));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;
			}
			| CONST_INT {
			$$=setLeft("factor","factor	: CONST_INT");
			fprintf(logout,"factor	: CONST_INT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

			}
			| CONST_FLOAT {
			$$=setLeft("factor","factor	: CONST_FLOAT");
			fprintf(logout,"factor	: CONST_FLOAT   \n");
			$$->addChild(symbolToParse($1));
			$$->setStartLine(start_line);
			$$->setEndLine(line_count);
			checkStart = true;

			}
			| variable INCOP {
			$$=setLeft("factor","factor	: variable INCOP");
			fprintf(logout,"factor	: variable INCOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
			| variable DECOP {
			$$=setLeft("factor","factor	: variable DECOP");
			fprintf(logout,"factor	: variable DECOP  \n");
			$$->addChild($1); $$->addChild(symbolToParse($2));
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
			;
	
argument_list : arguments{
			$$=setLeft("argument_list","argument_list : arguments");
			fprintf(logout,"argument_list : arguments  \n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
			  | {
			$$=setLeft("argument_list","argument_list :");
			fprintf(logout,"argument_list :\n");
			$$->addChild(new ParseTree("",""));
			$$->setStartLine(line_count);
			$$->setEndLine(line_count);
			checkStart = true;

			}
			;
	
arguments : arguments COMMA logic_expression {
			$$=setLeft("arguments","arguments : arguments COMMA logic_expression");
			fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			$$->addChild($1); $$->addChild(new ParseTree("","COMMA : ,")); $$->addChild($3);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

			}
	      | logic_expression {
			$$=setLeft("arguments","arguments : logic_expression");
			fprintf(logout,"arguments : logic_expression\n");
			$$->addChild($1);
			$$->setStartLine($1->getStartLine());
			$$->setEndLine(line_count);
			checkStart = true;

		  }
	      ;
 

%%
int main(int argc,char *argv[])
{
	infunc = false;
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
	
	fprintf(logout,"Total lines: %d\nTotal errors: %d\n", line_count,err_count);
	
	delete table;
	fclose(checkerout);
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}

