%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%token ADD SUB MUL DIV MOD
%token ID
%token ASSIGN
%token LT LE GT GE EQ NEQ AND OR NOT
%token SEMI
%token READ WHILE DO IF ELSE TRUE FALSE FOR CONST 
%token PRINT BOOL CONTINUE BREAK RETURN
%token BOOLEAN INT VOID FLOAT DOUBLE STRING
%token CONST_INT CONST_FLOAT CONST_SCI CONST_STR

%left NEG
%left MUL DIV ADD SUB
%left NOT AND OR
%left LT GT
%left PARENTHESES

%%

program : programBodies
	    ;

programBodies : programBodies programBody
              | programBody
              ;

programBody : function
            | declartions SEMI
            ;

/* function */
type : INT 
     | FLOAT
     | DOUBLE
     | STRING
     | BOOL
     | VOID
     ; 

nonVoidType : INT 
            | FLOAT
            | DOUBLE
            | STRING
            | BOOL
            ; 

return : NonVoidreturn
       | voidReturn
       ;

NonVoidreturn : RETURN expressions SEMI
            ;

voidReturn : RETURN SEMI
           ;

function : nonVoidType ID '(' arguments ')' SEMI
         | VOID ID '(' arguments ')' SEMI
         | nonVoidType ID '(' arguments ')' compound
         | VOID ID '(' arguments ')' voidcompound

/* argument */
arguments : /* empty */  
          | ExistArguments
          ;

argument : type ID
         ;

ExistArguments : ExistArguments ',' argument
               | argument
               ;

compound : '{' statements '}'
voidcompound : '{' voidstatements '}'
/* every statement */
statements : /* empty */
           | ExistStatement
           ;

voidstatements : /* empty */
               | voidExistStatement
               ;

ExistStatement : ExistStatement statement
               | statement
               ;

voidExistStatement : voidExistStatement voidstatement
                   | voidstatement
                   ;

statement : declartions SEMI
          | simple SEMI
          | conditional
          | while
          | for
          | jump
          ;

voidstatement : declartions SEMI
              | simple SEMI
              | conditional
              | while
              | for
              | voidjump
              ;


number : CONST_INT
       | CONST_FLOAT
       | CONST_SCI
       ;

const_bool : TRUE
           | FALSE
           ;

/* declartion */
declartions : CONST declartion
            | declartion 
            ;

declartion : nonVoidType varNames
           | nonVoidType var
           ;

varNames : varNames ',' var
         | var
         ;

var : ID ASSIGN expression
    | ID
    | array
    | array ASSIGN '{' arrayInit '}'
    ;

/* array */
array : ID dimensions
      ;

dimensions : dimensions dimension
           | dimension
           ;

dimension : '[' CONST_INT ']'
          ;

arrayInit : arrayInit ',' expression
          | expression

/* simple */

arrayRef : ID dims

dims : dims dim
     | dim
     ;

dim : '[' expressions ']'
    ;

variableRef : ID
            | arrayRef
            ;

simple : variableRef ASSIGN expressions
       | PRINT variableRef
       | PRINT CONST_STR
       | PRINT expressions
       | READ variableRef
       ;

/* expression */

expressions : expression
            | boolCalc
            ;

expression : number
           | boolCalc
           | variableRef
           | SUB expression %prec NEG
           | expression ADD expression
           | expression SUB expression
           | expression MUL expression
           | expression DIV expression
           | expression MOD expression %prec MUL
           | '(' expression ')' %prec PARENTHESES
           | ID '(' functionUsageArg ')'
           ;

boolCalc : const_bool
         | expression LT expression
         | expression GT expression
         | expression LE expression %prec GT
         | expression GE expression %prec GT
         | expression EQ expression
         | expression NEQ expression %prec GT
         | expression AND expression %prec AND
         | expression OR expression %prec OR
         | NOT expression %prec NOT
         ;

functionUsageArg : /* empty */  
                 | expression
                 | functionUsageArg ',' expression
                 ;

/* conditional */

conditional : IF '(' boolCalc ')' '{' statements '}' elseCond
            ;

elseCond : /* empty */
         | ELSE '{' statements '}'
         ;

/* while */

while : WHILE '(' expressions ')' '{' statements '}'
      | DO '{' statements '}' WHILE '(' expressions ')' SEMI

/* for */

initialExpr : declartion
            | simple
            ;

for : FOR '(' initialExpr SEMI expressions SEMI simple')' '{' statements '}'
    ;

/* jump */

jump : return 
     | BREAK SEMI
     | CONTINUE SEMI
     ;

voidjump : voidReturn
         | BREAK SEMI
         | CONTINUE SEMI
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
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}

