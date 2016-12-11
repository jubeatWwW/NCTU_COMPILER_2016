#include "SymbolTable.h"

SymbolTable* SymbolTableFactory(){
    printf("table built\n");
    SymbolTable* symbolTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    symbolTable->position = 0;
    symbolTable->currentLevel = 0;
    symbolTable->capacity = 4;
    symbolTable->entry = (TableEntry**)malloc(sizeof(TableEntry*)*4);
    return symbolTable;
}

List* ListFactory(){
    List* list = (List*)malloc(sizeof(List));
    list->position = 0;
    list->capacity = 4;
    list->list = (char**)malloc(sizeof(char*)*4);
    return list;
}

void InsertEntry(SymbolTable* table, const char* name, const char* kind, int level, Type* type, Attr *attr){
    TableEntry* entry = (TableEntry*)malloc(sizeof(TableEntry));
    //char* tmpName = strdup(name);
    //char* tmpKind = strdup(kind);
    strcpy(entry->name, name);
    strcpy(entry->kind, kind);
    entry->level = level;
    entry->type = type;
    entry->attr = attr;

    if(table->capacity <= table->position){
        TableEntry** entryTran = table->entry;
        table->capacity *= 2;
        table->entry = (TableEntry**)malloc(sizeof(TableEntry*) * (table->capacity));
        int pos;
        for(pos=0; pos < table->position; pos++){
            table->entry[pos] = entryTran[pos];
        }
        free(entryTran);
    }

    table->entry[table->position++] = entry;
}

void PrintSymbolTable(SymbolTable* table){
    /* tmp print for debug*/
    int i;
    TableEntry* tmp;
    for(i=0; i<table->position; i++){
        tmp = table->entry[i];
        printf("name: %s\tkind: %s\ttype: %s\n", tmp->name, tmp->kind, tmp->type);
    }
    free(tmp);
}


void InsertListToTable(SymbolTable* table, List* list, const char* kind,Type* type, Attr* attr){
    int i;
    for(i=0; i<list->position; i++){
        InsertEntry(table, list->list[i], kind, table->currentLevel, type, attr);
    }

    for(i=list->position-1; i>=0; i--){
        free(list->list[i]);
    }
    list->position = 0;
    list->capacity = 4;
    list->list = (char**)malloc(sizeof(char*) * 4);
}

void AddIdToList(List* list, char* id){
    char* tmp = strdup(id);
    printf("addid: %s\n", tmp);
    if(list->capacity <= list->position){
        char** idtran = list->list;
        list->list = (char**)malloc(sizeof(char*) * list->capacity*2);
        list->capacity *= 2;
        
        int pos = 0;
        for(pos = 0; pos < list->position; pos++){
            list->list[pos] = idtran[pos];
        }
        free(idtran);
    }

    list->list[list->position++] = tmp;
}

void PrintList(List* list){
    int i;
    for(i=0; i < list->position; i++){
        printf("pos: %d, id:%s \n", i, list->list[i]);
    }
}

Type* BuildType(const char* typeName){
    Type* type = (Type*)malloc(sizeof(type));
    strcpy(type->name, typeName);
    type->arr = NULL;
}

