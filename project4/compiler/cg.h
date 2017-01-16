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
typedef struct varDeclParam varDeclParam;

extern int linenum;
extern FILE* fout;
extern int scope;
extern int hasRead;
extern struct SymTable *symbolTable;
extern char fileName[256];


//specical instr
void ProgSt(const char* name);
void ProgEnd();
void MainFunc();
void MainEnd();



void GlobalVar(const char* name, PType* ptype);

void DeclInit();
void DeclPush(varDeclParam* var);
void DeclPop(PType* type);

//simple
void ReadVar(ExprSem *expr);
void PrintVarPre();
void PrintVar(ExprSem* expr);
void AssignToVar(ExprSem* var, ExprSem* booleanExp);


//if statement
void ConditionSt();
void ConditionEnd();
void ConditionElse();
void ConditionElseEnd();

//for loop
void ForBegin();
void ForLogical();
void ForExit();

//while loop (with do while)
void WhileBegin();
void WhileExit();
void WhileEnd();

//function declaration
void FuncSt(const char* name, Param* param, PType* ret);
void FuncEnd(PType* ret);
void FuncReturn(ExprSem* ret, int isMain);

//normal calculation and relation
void FunctionCall(const char* name, int isNeg);
void ConstExpr(ConstAttr* constattr);
void IdExpr(ExprSem* expr);
void Oper(ExprSem* e1, OPERATOR op, ExprSem* e2);
void Relation(ExprSem* e1, OPERATOR op, ExprSem* e2);
void Boolean(OPERATOR op);
void Negative(ExprSem* exp);
