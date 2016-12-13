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
    list->types = (Type**)malloc(sizeof(Type*)*4);
    return list;
}

void InsertEntry(SymbolTable* table, const char* name, const char* kind, int level, Type* type, Attr *attr){
    TableEntry* tmp = (TableEntry*)malloc(sizeof(TableEntry));
    //char* tmpName = strdup(name);
    //char* tmpKind = strdup(kind);
    strcpy(tmp->name, name);
    strcpy(tmp->kind, kind);
    tmp->level = level;
    tmp->type = type;
    tmp->attr = attr;
    
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

    table->entry[table->position++] = tmp;
}

void PrintSymbolTable(SymbolTable* table){
    /* tmp print for debug*/
    int i;
    TableEntry* tmp;
    printf("\n");
    for(i=0; i<table->position; i++){
        tmp = table->entry[i];
        printf("name: %s\tkind: %s\tlevel: %d\ttype: %s\n", tmp->name, tmp->kind, tmp->level, tmp->type);
    }
    printf("\n");
}


void InsertListToTable(SymbolTable* table, List* list, const char* kind,Type* type, Attr* attr){
    int i;
    for(i=0; i<list->position; i++){
        InsertEntry(table, list->list[i], kind, table->currentLevel, type, attr);
        if(list->types[i]!=NULL){
            Array* cur = list->types[i]->arr;
            while(cur!=NULL){
                printf("dim: %d\n\n", cur->size);
                cur = cur->higherDim;
            }
        }
    }

    for(i=list->position-1; i>=0; i--){
        free(list->list[i]);
        if(list->types[i]!=NULL)
            free(list->types[i]);
    }
    list->position = 0;
    list->capacity = 4;
    list->list = (char**)malloc(sizeof(char*) * 4);
    list->types = (Type**)malloc(sizeof(Type*) * 4);
}

void AddIdToList(List* list, char* id){
    char* tmp = strdup(id);
    printf("addid: %s\n", tmp);
    if(list->capacity <= list->position){
        char** idtran = list->list;
        Type** typetran = list->types;
        list->capacity *= 2;
        
        list->list = (char**)malloc(sizeof(char*) * list->capacity);
        list->types = (Type**)malloc(sizeof(Type*) * list->capacity);       

        int pos = 0;
        for(pos = 0; pos < list->position; pos++){
            list->list[pos] = idtran[pos];
            list->types[pos] = typetran[pos];

        }
        free(idtran);
        free(typetran);
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

void AddDim(List* list, int size){
    printf("test\n");
    int pos = list->position;
    if(list->types[pos] == NULL){
        Type* tmp = (Type*)malloc(sizeof(Type));
        tmp->arr = NULL;
        list->types[pos] = tmp;
    }

    if(list->capacity <= list->position){
        char** idtran = list->list;
        Type** typetran = list->types;
        list->capacity *= 2;
        
        list->list = (char**)malloc(sizeof(char*) * list->capacity);
        list->types = (Type**)malloc(sizeof(Type*) * list->capacity);       

        int pos = 0;
        for(pos = 0; pos < list->position; pos++){
            list->list[pos] = idtran[pos];
            list->types[pos] = typetran[pos];

        }
        free(idtran);
        free(typetran);
    }

    Array* current = list->types[pos]->arr;
    Array* tmp = (Array*)malloc(sizeof(Array));
    tmp->size = size;
    tmp->higherDim = NULL;
    if(current == NULL){
        list->types[pos]->arr = tmp;
    } else {
        while(current->higherDim != NULL){
            current = current->higherDim;
        }
        current->higherDim = tmp;
    }

}
