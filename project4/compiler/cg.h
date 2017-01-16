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
typedef struct ConstAttr ConstAttr;
typedef struct PTypeList PTypeList;
typedef struct idNode_sem NodeSem;

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
void ProgEnd();
void MainFunc();
void MainEnd();

void GlobalVar(const char* name, PType* ptype);
void ReadVar(ExprSem *expr);
void PrintVarPre();
void PrintVar(ExprSem* expr);
void AssignToVar(ExprSem* var, ExprSem* booleanExp);

void ConditionSt();
void ConditionEnd();
void ConditionElse();
void ConditionElseEnd();

void ForBegin();
void ForLogical();
void ForExit();

void FuncSt(const char* name, Param* param, PType* ret);
void FuncEnd(PType* ret);
void FuncReturn(ExprSem* ret, int isMain);

void FunctionCall(const char* name);
void ConstExpr(ConstAttr* constattr);
void IdExpr(ExprSem* expr);
void Oper(ExprSem* e1, OPERATOR op, ExprSem* e2);
void Relation(ExprSem* e1, OPERATOR op, ExprSem* e2);
void Boolean(OPERATOR op);
void Negative(ExprSem* exp);
