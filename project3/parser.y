%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"

extern int linenum;
extern FILE	*yyin;
extern char	*yytext;
extern int yylex(void);
extern char buf[256];

%}

%union {
    char* str;
    int num;
    float flnum;
    double dflnum;
    Type* type;
    Value* value;
    Expr* expr;
    //TableEntry* entry;
}

%token <str> ID
%token <num> INT_CONST
%token <flnum> FLOAT_CONST
%token <num> SCIENTIFIC
%token <str> STR_CONST

%token <str> LE_OP
%token <str> NE_OP
%token <str> GE_OP
%token <str> EQ_OP
%token <str> AND_OP
%token <str> OR_OP

%token	READ
%token	BOOLEAN
%token	WHILE
%token	DO
%token	IF
%token	ELSE
%token	TRUE
%token	FALSE
%token	FOR
%token <type> INT
%token	PRINT
%token <type> BOOL
%token <type> VOID
%token <type> FLOAT
%token <type> DOUBLE
%token <type> STRING
%token	CONTINUE
%token	BREAK
%token	RETURN
%token  CONST

%token <str> L_PAREN
%token <str> R_PAREN
%token <str> COMMA
%token <str> SEMICOLON
%token <str> ML_BRACE
%token <str> MR_BRACE
%token <str> L_BRACE
%token <str> R_BRACE
%token <str> ADD_OP
%token <str> SUB_OP
%token <str> MUL_OP
%token <str> DIV_OP
%token <str> MOD_OP
%token <str> ASSIGN_OP
%token <str> LT_OP
%token <str> GT_OP
%token <str> NOT_OP

%type <type> scalar_type
%type <expr> logical_expression
%type <expr> logical_term
%type <expr> logical_factor
%type <expr> relation_expression
%type <expr> arithmetic_expression
%type <expr> term
%type <expr> factor
%type <expr> variable_reference
%type <expr> array_list

%type <num> dimension

/*	Program 
	Function 
	Array 
	Const 
	IF 
	ELSE 
	RETURN 
	FOR 
	WHILE
*/
%start program
%%

program :  decl_list funct_def decl_and_def_list 
        {
            PrintSymbolTable(symbolTable);
        }
           ;

decl_list : decl_list var_decl
		  | decl_list const_decl
		  | decl_list funct_decl
		  |
		  ;


decl_and_def_list : decl_and_def_list var_decl
				  | decl_and_def_list const_decl
				  | decl_and_def_list funct_decl
				  | decl_and_def_list funct_def
				  | 
				  ;

funct_def : scalar_type ID L_PAREN R_PAREN compound_statement
          {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", $1, NULL);
          }
		  | scalar_type ID L_PAREN parameter_list R_PAREN compound_statement
          {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", $1, NULL);
          }
		  | VOID ID L_PAREN R_PAREN compound_statement
          {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", BuildType("void"), NULL);
          }
		  | VOID ID L_PAREN parameter_list R_PAREN compound_statement
          {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", BuildType("void"), NULL);
          }
          ;

funct_decl : scalar_type ID L_PAREN R_PAREN SEMICOLON
           {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", $1, NULL);
           }
	 	   | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON
           {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", $1, NULL);
           }
		   | VOID ID L_PAREN R_PAREN SEMICOLON
           {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", BuildType("void"), NULL);
           }
		   | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON
           {
                AddIdToList(list, $2);
                InsertListToTable(symbolTable, list, "function", BuildType("void"), NULL);
           }
		   ;

parameter_list : parameter_list COMMA scalar_type ID
               {
                    AddIdToList(list, $4);
                    InsertListToTable(symbolTable, list, "parameter", $3, NULL);
               }
			   | parameter_list COMMA scalar_type array_decl
               {
                    InsertListToTable(symbolTable, list, "parameter", $3, NULL);
               }
			   | scalar_type array_decl
               {
                    InsertListToTable(symbolTable, list, "parameter", $1, NULL);
               }
			   | scalar_type ID
               {
                    AddIdToList(list, $2);
                    InsertListToTable(symbolTable, list, "parameter", $1, NULL);
               }
			   ;

var_decl : scalar_type identifier_list SEMICOLON
         {
            PrintList(list);
            InsertListToTable(symbolTable, list, "variable", $1, NULL);
         }
		 ;

identifier_list : identifier_list COMMA ID 
                {
                    AddIdToList(list, $3);
                }
                | identifier_list COMMA ID ASSIGN_OP logical_expression
                {
                    AddIdToList(list, $3);
                }
				| identifier_list COMMA array_decl ASSIGN_OP initial_array
				| identifier_list COMMA array_decl
				| array_decl ASSIGN_OP initial_array
				| array_decl
				| ID ASSIGN_OP logical_expression
                {
                    AddIdToList(list, $1);
                }
				| ID 
                {
                    AddIdToList(list, $1);
                }
				;

initial_array : L_BRACE literal_list R_BRACE
			  ;

literal_list : literal_list COMMA logical_expression
			 | logical_expression
                         | 
			 ;

const_decl : CONST scalar_type const_list SEMICOLON
           {
                InsertListToTable(symbolTable, list, "constant", $2, NULL);
           }
           ;

const_list : const_list COMMA ID ASSIGN_OP literal_const
           {
                AddIdToList(list, $3);
           }
		   | ID ASSIGN_OP literal_const
           {
                AddIdToList(list, $1);
           }
		   ;

array_decl : ID dim
           {
                AddIdToList(list, $1);
           }
		   ;

dim : dim ML_BRACE INT_CONST MR_BRACE
    {
        AddDim(list, $3);
    }
	| ML_BRACE INT_CONST MR_BRACE
    {
        AddDim(list, $2);
    }
	;

compound_statement : L_BRACE {symbolTable->currentLevel++;}
                     var_const_stmt_list 
                     R_BRACE {
                        PrintSymbolTable(symbolTable);
                        symbolTable->currentLevel--;
                     }
                   ;

var_const_stmt_list : var_const_stmt_list statement	
				    | var_const_stmt_list var_decl
					| var_const_stmt_list const_decl
				    |
				    ;

statement : compound_statement
		  | simple_statement
		  | conditional_statement
		  | while_statement
		  | for_statement
		  | function_invoke_statement
		  | jump_statement
		  ;		

simple_statement : variable_reference ASSIGN_OP logical_expression SEMICOLON
                 {
                    TypeCoercionAssign($1, $3);
                 }
				 | PRINT logical_expression SEMICOLON
				 | READ variable_reference SEMICOLON
				 ;

conditional_statement : IF L_PAREN logical_expression R_PAREN 
                        L_BRACE{symbolTable->currentLevel++;} 
                            var_const_stmt_list 
                        R_BRACE {
                            PrintSymbolTable(symbolTable);
                            symbolTable->currentLevel--;
                        }
                        conditional_statement_else
					  ;
conditional_statement_else : ELSE
					  		 L_BRACE{symbolTable->currentLevel++;} 
                                  var_const_stmt_list 
                             R_BRACE {
                                PrintSymbolTable(symbolTable);
                                symbolTable->currentLevel--;
                             }
                           |
                           ;

while_statement : WHILE L_PAREN logical_expression R_PAREN
                  L_BRACE {symbolTable->currentLevel++;} 
                      var_const_stmt_list 
                  R_BRACE {
                     PrintSymbolTable(symbolTable);
                     symbolTable->currentLevel--;
                  }
				| DO L_BRACE {symbolTable->currentLevel++;} 
					var_const_stmt_list
                  R_BRACE {
                    PrintSymbolTable(symbolTable);
                    symbolTable->currentLevel--;
                  }
                  WHILE L_PAREN logical_expression R_PAREN SEMICOLON
				;

for_statement : FOR L_PAREN initial_expression_list SEMICOLON control_expression_list SEMICOLON increment_expression_list R_PAREN 
                 L_BRACE{symbolTable->currentLevel++;} 
                      var_const_stmt_list 
                 R_BRACE {
                    PrintSymbolTable(symbolTable);
                    symbolTable->currentLevel--;
                 }
			  ;

initial_expression_list : initial_expression
				  	    |
				        ;

initial_expression : initial_expression COMMA variable_reference ASSIGN_OP logical_expression
				   | initial_expression COMMA logical_expression
				   | logical_expression
				   | variable_reference ASSIGN_OP logical_expression

control_expression_list : control_expression
				  		|
				  		;

control_expression : control_expression COMMA variable_reference ASSIGN_OP logical_expression
				   | control_expression COMMA logical_expression
				   | logical_expression
				   | variable_reference ASSIGN_OP logical_expression
				   ;

increment_expression_list : increment_expression 
						  |
						  ;

increment_expression : increment_expression COMMA variable_reference ASSIGN_OP logical_expression
					 | increment_expression COMMA logical_expression
					 | logical_expression
					 | variable_reference ASSIGN_OP logical_expression
					 ;

function_invoke_statement : ID L_PAREN logical_expression_list R_PAREN SEMICOLON
						  | ID L_PAREN R_PAREN SEMICOLON
                          {
                                SearchType(symbolTable, $1);
                          }
						  ;

jump_statement : CONTINUE SEMICOLON
			   | BREAK SEMICOLON
			   | RETURN logical_expression SEMICOLON
			   ;

variable_reference : array_list
                   {
                        $$ = $1;
                   }
				   | ID
                   {
                        $$ = BuildExpr("var", $1, SearchType(symbolTable, $1), 0);
                   }
				   ;


logical_expression : logical_expression OR_OP logical_term
                   {
                        $$ = BuildExpr("const", "const", BuildType("bool"), 0);
                   }
				   | logical_term
                   {
                        $$ = $1;
                   }
				   ;

logical_term : logical_term AND_OP logical_factor
             {
                $$ = BuildExpr("const", "const", BuildType("bool"), 0);
             }
			 | logical_factor
             {
                $$ = $1;
             }
			 ;

logical_factor : NOT_OP logical_factor
               {
                    $$ = BuildExpr("const", "const", BuildType("bool"), 0);
               }
			   | relation_expression
               {
                    $$ = $1;
               }
			   ;

relation_expression : arithmetic_expression relation_operator arithmetic_expression
                    {
                        $$ = BuildExpr("const", "const", BuildType("bool"), 0);
                    }
					| arithmetic_expression
                    {
                        $$ = $1;
                    }
					;

relation_operator : LT_OP
				  | LE_OP
				  | EQ_OP
				  | GE_OP
				  | GT_OP
				  | NE_OP
				  ;

arithmetic_expression : arithmetic_expression ADD_OP term
                      {
                        $$ = TypeCoercion($1, $3);
                      }
                       | arithmetic_expression SUB_OP term
                      {
                        $$ = TypeCoercion($1, $3);
                      }
                       | relation_expression
                       | term
                       ;

term : term MUL_OP factor
     {
        $$ = TypeCoercion($1, $3);
     }
     | term DIV_OP factor
     {
        $$ = TypeCoercion($1, $3);
     }
	 | term MOD_OP factor
     {
        $$ = TypeCoercion($1, $3);
     }
	 | factor
     {
        $$ = $1;
     }
	 ;

factor : variable_reference
       {
            $$ = $1;
       }
	   | SUB_OP factor
       {
            $$ = BuildExpr("const", "const", BuildType("bool"), 0);
       }
	   | L_PAREN logical_expression R_PAREN
       {
            $$ = BuildExpr("const", "const", BuildType("bool"), 0);
       }
	   | SUB_OP L_PAREN logical_expression R_PAREN
       {
            $$ = $3;
       }
	   | ID L_PAREN logical_expression_list R_PAREN
       {
            $$ = BuildExpr("var", $1, SearchType(symbolTable, $1), 0);
       }
	   | ID L_PAREN R_PAREN
       {
            $$ = BuildExpr("var", $1, SearchType(symbolTable, $1), 0);
       }
	   | literal_const
       {
            $$ = BuildExpr("const", "const", BuildType("bool"), 0);
       }
	   | SUB_OP ID L_PAREN logical_expression R_PAREN
       {
            $$ = BuildExpr("var", $2, SearchType(symbolTable, $2), 0);
       }
	   | SUB_OP ID L_PAREN R_PAREN
       {
            $$ = BuildExpr("var", $2, SearchType(symbolTable, $2), 0);
       }
	   ;

logical_expression_list : logical_expression_list COMMA logical_expression
						| logical_expression
						;

array_list : ID dimension
           {
                $$ = BuildExpr("var", "$1", SearchType(symbolTable, $1), $2);
           }
		   ;

dimension : dimension ML_BRACE logical_expression MR_BRACE
          {
                printf("dim add 1\n");
                $$ = $1 + 1;
          }
		  | ML_BRACE logical_expression MR_BRACE
          {
              printf("has 1 dim\n");
              $$ = 1;
          }
		  ;



scalar_type : INT       {$$ = BuildType("int");}
			| DOUBLE    {$$ = BuildType("double");}
			| STRING    {$$ = BuildType("string");}
			| BOOL      {$$ = BuildType("bool");}
			| FLOAT     {$$ = BuildType("float");}
			;
 
literal_const : INT_CONST
			  | SUB_OP INT_CONST
			  | FLOAT_CONST
			  | SUB_OP FLOAT_CONST
			  | SCIENTIFIC
			  | SUB_OP SCIENTIFIC
			  | STR_CONST
			  | TRUE
			  | FALSE
			  ;
%%

int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );
	exit(-1);
	//  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}


