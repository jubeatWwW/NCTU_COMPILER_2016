#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"
#include "cg.h"

extern int yyparse();
extern FILE* yyin;

extern struct SymTable *symbolTable;
extern struct PType *funcReturn;
extern char fileName[256];

extern __BOOLEAN semError; 

FILE *fout;

int  main( int argc, char **argv )
{
	if( argc == 1 )
	{
		yyin = stdin;
	}
	else if( argc == 2 )
	{
		FILE *fp = fopen( argv[1], "r" );
		if( fp == NULL ) {
				fprintf( stderr, "Open file error\n" );
				exit(-1);
		}
 
        /*strcpy(fileName, argv[1]);
        int len = strlen(fileName);
        char *ext = fileName + len;
        while(*ext-- != '.');
        *(ext+1) = '\0';

        while('/' != *ext && ext >= fileName){
            ext--;
        }

        ext++;
        char* tmp = strdup(ext);
        strcpy(fileName, tmp);
        

        char newfilename[256];*/

        strcpy(fileName, "output");
        char* newfilename = strdup("output.j");
        

        //snprintf(newfilename, sizeof(newfilename), "%s.j", fileName);
        fout = fopen(newfilename, "w");
        
		yyin = fp;
	}
	else
	{
	  	fprintf( stderr, "Usage: ./parser [filename]\n" );
   		exit(0);
 	} 

	symbolTable = (struct SymTable *)malloc(sizeof(struct SymTable));
	initSymTab( symbolTable );

	// initial function return recoder

	yyparse();	/* primary procedure of parser */

	if(semError == __TRUE){	
		fprintf( stdout, "\n|--------------------------------|\n" );
		fprintf( stdout, "|  There is no syntactic error!  |\n" );
		fprintf( stdout, "|--------------------------------|\n" );
	}
	else{
		fprintf( stdout, "\n|-------------------------------------------|\n" );
		fprintf( stdout, "| There is no syntactic and semantic error! |\n" );
		fprintf( stdout, "|-------------------------------------------|\n" );
	}
    
    fclose(fout);
	exit(0);
}

