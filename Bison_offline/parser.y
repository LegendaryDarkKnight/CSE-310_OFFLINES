%{
#include "2005033.hpp"
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#define YYSTYPE SymbolInfo*
using namespace std;


int ScopeTable::counter1 = 0;
int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_count;
extern int err_count;

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

}

%}

%token IF ELSE FOR WHILE ID LPAREN RPAREN SEMICOLON LCURL RCURL COMMA INT FLOAT VOID LTHIRD CONST_INT RTHIRD PRINTLN RETURN
ASSIGNOP LOGICOP RELOP ADDOP MULOP CONST_FLOAT NOT INCOP DECOP

%left 
%right

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start : program
	{
		fprintf(logout,"start : program \n");
	}
	;

program : program unit {
		fprintf(logout,"program : program unit \n");
	}
	| unit{
		fprintf(logout,"program : unit \n");
	}
	;
	
unit : var_declaration{
		fprintf(logout,"unit : var_declaration  \n");
	}
     | func_declaration{
		fprintf(logout,"unit : func_declaration  \n");
	 }
     | func_definition{
		fprintf(logout,"unit : func_definition  \n");
	 }
     ;
	
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON{
			fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN{infunc = true;} compound_statement{
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");
			
		}
		| type_specifier ID LPAREN RPAREN compound_statement{
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
			temp = $2;
			table->Insert(temp->getName(),"FUNCTION");			
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
			temp = $4;
			stack1.push(new SymbolInfo(temp->getName(), type));
		}
		| parameter_list COMMA type_specifier{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
		}
 		| type_specifier ID{
			fprintf(logout,"parameter_list  : type_specifier ID\n");
			temp = $2;
			stack1.push(new SymbolInfo(temp->getName(), type));
		}
		| type_specifier{

		}
 		;

 		
compound_statement : LCURL{
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
} statements RCURL{
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
				table->printAll(logout);
				table->ExitScope();
			}
 		    | LCURL RCURL{
				fprintf(logout,"compound_statement : LCURL RCURL\n");
				table->printAll(logout);
				table->ExitScope();
			}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		}
 		 ;
 		 
type_specifier	: INT {fprintf(logout,"type_specifier : INT \n"); type = "INT";}
 		| FLOAT {fprintf(logout,"type_specifier : FLOAT \n"); type = "FLOAT";}
 		| VOID {fprintf(logout,"type_specifier : VOID \n"); type = "VOID"; }
 		;
 		
declaration_list : declaration_list COMMA ID{
					fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
					temp = $3;
					table->Insert(temp->getName(),type);
					}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD{
			fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
			temp = $3;
			table->Insert(temp->getName(),type);
			}
 		  | ID{
			fprintf(logout,"declaration_list : ID \n");
			temp = $1;
			table->Insert(temp->getName(),type);
		  }
 		  | ID LTHIRD CONST_INT RTHIRD{
			fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
			temp = $1;
			table->Insert(temp->getName(),type);
		  }
 		  ;
 		  
statements : statement{
			fprintf(logout,"statements : statement  \n");
		}
	   | statements statement{
			fprintf(logout,"statements : statements statement  \n");
	   }
	   ;
	   
statement : var_declaration {
			fprintf(logout,"statement : var_declaration \n");
		}
	  | expression_statement{
			fprintf(logout,"statement : expression_statement  \n");
	  }
	  | compound_statement{
			
	  }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement{
			fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement  \n");
	  }
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE ;{
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement  \n");
	  }
	  | IF LPAREN expression RPAREN statement ELSE statement{
			fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement  \n");
	  }
	  | WHILE LPAREN expression RPAREN statement{
			fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement  \n");
	  }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON{
			fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
	  }
	  | RETURN expression SEMICOLON{
			fprintf(logout,"statement : RETURN expression SEMICOLON  \n");
	  }
	  ;
	  
expression_statement : SEMICOLON{
				fprintf(logout,"expression_statement : SEMICOLON\n");
			}			
			| expression SEMICOLON{
				fprintf(logout,"expression_statement : expression SEMICOLON\n");
			} 
			;
	  
variable : ID{
		fprintf(logout,"variable : ID 	 \n");
	} 		
	 | ID LTHIRD expression RTHIRD{
		fprintf(logout,"variable : ID LSQUARE expression RSQUARE  	 \n");
	 } 
	 ;
	 
expression : logic_expression{
			fprintf(logout,"expression 	: logic_expression	 \n");
	 	}
	   | variable ASSIGNOP logic_expression{
			fprintf(logout,"expression 	: variable ASSIGNOP logic_expression 		 \n");
	   }	
	   ;
			
logic_expression : rel_expression 	{
			fprintf(logout,"logic_expression : rel_expression 	 \n");
		}
		 | rel_expression LOGICOP rel_expression {
			fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
		 }
		 ;
			
rel_expression	: simple_expression {
		fprintf(logout,"rel_expression	: simple_expression \n");
		}
		| simple_expression RELOP simple_expression	{
		fprintf(logout,"rel_expression	: simple_expression RELOP simple_expression	\n");

		}
		;
				
simple_expression : term {
		fprintf(logout,"simple_expression : term \n");

		}
		  | simple_expression ADDOP term {
		fprintf(logout,"simple_expression :	simple_expression ADDOP term \n");

		  }
		  ;
					
term :	unary_expression {
		fprintf(logout,"term :	unary_expression \n");
	}
     |  term MULOP unary_expression {
		fprintf(logout,"term : term MULOP unary_expression \n");
	 }
     ;

unary_expression : ADDOP unary_expression  {
			fprintf(logout,"factor : variable  \n");
		}
		 | NOT unary_expression {
			fprintf(logout,"unary_expression : NOT unary_expression \n");
		 }
		 | factor {
			fprintf(logout,"unary_expression : factor \n");
		 } 
		 ;
	
factor	: variable {
	fprintf(logout,"factor : variable  \n");
	}
	| ID LPAREN argument_list RPAREN {
		fprintf(logout,"factor	: ID LPAREN argument_list RPAREN  \n");
	}
	| LPAREN expression RPAREN {
		fprintf(logout,"factor	: LPAREN expression RPAREN   \n");
	}
	| CONST_INT {
		fprintf(logout,"factor	: CONST_INT   \n");
	}
	| CONST_FLOAT {
		fprintf(logout,"factor	: CONST_FLOAT   \n");
	}
	| variable INCOP {
		fprintf(logout,"factor	: variable INCOP  \n");
	}
	| variable DECOP {
		fprintf(logout,"factor	: variable DECOP  \n");
	}
	;
	
argument_list : arguments{
			fprintf(logout,"argument_list : arguments  \n");
			}
			  |
			  ;
	
arguments : arguments COMMA logic_expression {
			fprintf(logout,"arguments : arguments COMMA logic_expression \n");
			}
	      | logic_expression {
			fprintf(logout,"arguments : logic_expression\n");
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
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}

