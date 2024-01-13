%{
#include "2005033.hpp"
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#define YYSTYPE SymbolInfo*
using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_count;
extern int err_count;

FILE *fp;
FILE *logout;
FILE *errorout;
FILE *parseout;

SymbolTable *table;

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
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement{
			fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
		}
		| type_specifier ID LPAREN RPAREN compound_statement{
			fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID\n");
		}
		| parameter_list COMMA type_specifier{
			fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier\n");
		}
 		| type_specifier ID{
			fprintf(logout,"parameter_list  : type_specifier ID\n");
		}
		| type_specifier{

		}
 		;

 		
compound_statement : LCURL statements RCURL{
				fprintf(logout,"compound_statement : LCURL statements RCURL  \n");
			}
 		    | LCURL RCURL{
				fprintf(logout,"compound_statement : LCURL RCURL\n");
			}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
		}
 		 ;
 		 
type_specifier	: INT {fprintf(logout,"type_specifier : INT \n");}
 		| FLOAT {fprintf(logout,"type_specifier : FLOAT \n");}
 		| VOID {fprintf(logout,"type_specifier : VOID \n");}
 		;
 		
declaration_list : declaration_list COMMA ID{
					fprintf(logout,"declaration_list : declaration_list COMMA ID \n");
					}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD{
			fprintf(logout,"declaration_list :declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
			}
 		  | ID{
			fprintf(logout,"declaration_list : ID \n");
		  }
 		  | ID LTHIRD CONST_INT RTHIRD{
			fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
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
			fprintf(logout,"IF LPAREN expression RPAREN statement ELSE statement  \n");
	  }
	  | WHILE LPAREN expression RPAREN statement{
			fprintf(logout,"WHILE LPAREN expression RPAREN statement  \n");
	  }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON{
			fprintf(logout,"PRINTLN LPAREN ID RPAREN SEMICOLON  \n");
	  }
	  | RETURN expression SEMICOLON{
			fprintf(logout,"RETURN expression SEMICOLON  \n");
	  }
	  ;
	  
expression_statement 	: SEMICOLON{

			}			
			| expression SEMICOLON{

			} 
			;
	  
variable : ID{
		fprintf(logout,"variable : ID 	 \n");
	} 		
	 | ID LTHIRD expression RTHIRD{
		fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
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

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logout= fopen("log.txt","w");
	errorout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt","w");

	table = new SymbolTable(11);

	yyin=fp;
	yyparse();
	
	fprintf(logout,"Total lines: %d\nTotal errors: %d\n", line_count, err_count);
	
	delete table;
	fclose(parseout);
	fclose(errorout);
	fclose(logout);
	fclose(fp);
	return 0;
}

