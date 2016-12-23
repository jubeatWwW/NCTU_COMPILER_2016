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
        //free(entryTran);
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
        if(tmp->level == table->currentLevel || (table->currentLevel == 0 && strcmp(tmp->kind, "parameter")==0) ){
            printf("name: %s\tkind: %s\tlevel: %d\t", tmp->name, tmp->kind, tmp->level);
            PrintType(tmp->type);
            printf("\n");
        }
    }
    printf("\n");

    RemoveFromTable(table);
}

void RemoveFromTable(SymbolTable* table){
    int i;
    TableEntry* tmp;
    for(i = table->position-1;i>=0; i--){
        tmp = table->entry[i];
        if(strcmp(tmp->kind, "variable") == 0 && tmp->level == table->currentLevel){
            free(tmp);
            table->position--;
        } else {
            break;
        }
    }
}

void PrintType(Type* type){
    Array* cur = type->arr;
    printf("%s", type->name);
    if(type->hasArr >= 1){
        while(cur!=NULL){
            printf("[%d]", cur->size);
            cur = cur->higherDim;
        }
    }
}

void InsertListToTable(SymbolTable* table, List* list, const char* kind,Type* type, Attr* attr){
    int i;
    for(i=0; i<list->position; i++){
        int level = strcmp(kind, "parameter")==0 ? table->currentLevel+1 : table->currentLevel;
        printf("insert %s to table\n", list->list[i]);
        if(list->types[i]!=NULL){
            /*Array* cur = list->types[i]->arr;
            while(cur!=NULL){
                printf("dim: %d\n\n", cur->size);
                cur = cur->higherDim;
            }*/
            strcpy(list->types[i]->name, type->name);
            InsertEntry(table, list->list[i], kind, level, list->types[i], attr);
        } else {
            type->arr = NULL;
            InsertEntry(table, list->list[i], kind, level, type, attr);
        }

    }

    for(i=list->position-1; i>=0; i--){
        free(list->list[i]);
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

Expr* BuildExpr(const char* kind, const char* id, Type* type, int dim){
    printf("build expr\n");
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    strcpy(expr->kind, kind);
    strcpy(expr->id, id);
    expr->type = type;
    expr->dim = dim;
    return expr;
}

Type* BuildType(const char* typeName){
    Type* type = (Type*)malloc(sizeof(Type));
    strcpy(type->name, typeName);
    type->arr = NULL;
    type->hasArr = 0;
    return type;
}

void AddDim(List* list, int size){
    printf("test\n");
    int pos = list->position;
    if(list->types[pos] == NULL){
        Type* tmp = (Type*)malloc(sizeof(Type));
        tmp->arr = NULL;
        tmp->hasArr = 1;
        list->types[pos] = tmp;
    } else {
        list->types[pos]->hasArr++;
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

Type* SearchType(SymbolTable* table, char* id){
    printf("Search for %s\n", id);
    int i;
    TableEntry* tmp;
    for(i=0; i<table->position; i++){
        tmp = table->entry[i];
        if(tmp->level <= table->currentLevel && strcmp(id, tmp->name)==0){
            printf("found %s, type: %s\n", id, tmp->type->name);
            return tmp->type;
        }
    }
    return BuildType("None");
}

Expr* TypeCoercion(Expr* exp1, Expr* exp2){
    printf("type cor, type1: %s, type2: %s\n", exp1->type->name, exp2->type->name);
    if(exp1->dim != exp1->type->hasArr || exp2->dim != exp2->type->hasArr){
        printf("Illegal: array arithmetic\n");
    }
    if(strcmp(exp1->type->name, "double") == 0){
        if(strcmp(exp2->type->name, "float") == 0 || strcmp(exp2->type->name, "int") == 0){
            return BuildExpr("const", "const", BuildType("double"), 0);
        }
    }
    if(strcmp(exp2->type->name, "double") == 0){
        if(strcmp(exp1->type->name, "float") == 0 || strcmp(exp1->type->name, "int") == 0){
            return BuildExpr("const", "const", BuildType("double"), 0);
        }
    }
    if(strcmp(exp1->type->name, "float") == 0){
        if(strcmp(exp2->type->name, "int") == 0){
            return BuildExpr("const", "const", BuildType("float"), 0);
        }
    }
    if(strcmp(exp2->type->name, "float") == 0){
        if(strcmp(exp1->type->name, "int") == 0){
            return BuildExpr("const", "const", BuildType("float"), 0);
        }
    }
}

void TypeCoercionAssign(Expr* exp1, Expr* exp2){
    printf("type cor assign, type1: %s, type2: %s\n", exp1->type->name, exp2->type->name);
    if(strcmp(exp2->type->name, "double") == 0){
        if(strcmp(exp1->type->name, "float") == 0 || strcmp(exp1->type->name, "int") == 0){
            printf("Illegal cor\n");
        }
    }
    if(strcmp(exp2->type->name, "float") == 0){
        if(strcmp(exp1->type->name, "int") == 0){
            printf("Illegal cor\n");
        }
    }

    if(exp1->dim != exp1->type->hasArr || exp2->dim != exp2->type->hasArr){
        printf("Illegal: array assignment\n");
    }
}

