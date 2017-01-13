#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern char *yytext;
extern int linenum;

typedef struct SymbolTable SymbolTable;
typedef struct TableEntry TableEntry;
typedef struct Type Type;
typedef struct Types Types;
typedef struct Attr Attr;
typedef struct Array Array;
typedef struct List List;
typedef struct Value Value;
typedef struct Expr Expr;

SymbolTable* symbolTable;
List* list;
extern TableEntry* tableEntry;

struct SymbolTable {
    int position;
    int currentLevel;
    int capacity;
    TableEntry** entry;
};

struct TableEntry{
    char name[65];
    char kind[33];
    int level;
    Type* type;
    Attr* attr;
};

struct Type{
    char name[33];
    Array* arr;
    int hasArr;
};

struct Types{
    Type** types;
};

struct Attr{
    Type** types;
    int position;
    int capacity;
};

struct Array{
    int size;
    Array* higherDim;
};

struct List{
    int position;
    int capacity;
    char** list;
    Type** types;
};

struct Value{
    Type* type;
    int ival;
    float fval;
    double dval;
    char* cval;
};

struct Expr{
    char kind[33];
    char id[33];
    Type* type;
    int dim;
};


SymbolTable* SymbolTableFactory();
List* ListFactory();

void InsertListToTable(SymbolTable*, List*, const char*, Type*, Attr*);
void InsertEntry(SymbolTable*, const char*, const char*, int, Type*, Attr*);
void PrintSymbolTable(SymbolTable*);
void RemoveFromTable(SymbolTable*);
void PrintType(Type*);

Type* BuildType(const char* typeName);
Expr* BuildExpr(const char* kind, const char* id, Type* type, int dim);

void AddIdToList(List*, char*);
void PrintList(List*);
void AddDim(List*, int);

Type* SearchType(SymbolTable*, char*);
void TypeCoercionAssign(Expr* exp1, Expr* exp2);
Expr* TypeCoercion(Expr*, Expr*);
