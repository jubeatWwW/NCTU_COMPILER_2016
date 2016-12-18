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
    char name[33];
    char kind[33];
    int level;
    Type* type;
    Attr* attr;
};

struct Type{
    char name[33];
    Array* arr;
};

struct Types{
    Type** types;
};

struct Attr{
    Types* types;
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


SymbolTable* SymbolTableFactory();
List* ListFactory();

void InsertListToTable(SymbolTable*, List*, const char*, Type*, Attr*);
void InsertEntry(SymbolTable*, const char*, const char*, int, Type*, Attr*);
void PrintSymbolTable(SymbolTable*);
void PrintType(Type*);

Type* BuildType(const char* typeName);

void AddIdToList(List*, char*);
void PrintList(List*);
void AddDim(List*, int);
