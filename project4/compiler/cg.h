#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"


typedef struct SymNode SymNode;
typedef struct param_sem Param;
typedef struct PType PType;
typedef struct InstrStack InstrStack;
typedef struct expr_sem ExprSem;

extern int linenum;
extern FILE* fout;
extern int scope;
extern int hasRead;
extern struct SymTable *symbolTable;
extern char fileName[256];
extern InstrStack instrStack;


struct InstrStack{
    char* stack[1024];
    int current;
};

char instr[256];

void InstrStackPush(const char* instr);
void InstrStackClear();
void InstrStackPrint();

void ProgSt(const char* name);

void GlobalVar(const char* name, PType* ptype);
void ReadVar(ExprSem *expr);
void FuncSt(const char* name, Param* param, PType* ret);


