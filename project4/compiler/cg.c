#include "cg.h"

void ProgSt(const char* name){
    fprintf(fout, "; %s\n", name);
    fprintf(fout, ".class public %s\n", name);
    fprintf(fout, ".super java/lang//Object\n");
    fprintf(fout, ".field public static _sc Ljava/util/Scanner;\n");
}

void InstrStackPush(const char* ins){
    instrStack.stack[instrStack.current++] = strdup(ins);
}

void InstrStackPrint(){
    int i;
    for(i=0; i< instrStack.current; i++){
        fprintf(fout, "%s\n", instrStack.stack[i]);
        free(instrStack.stack[i]);
    }
    instrStack.current = 0;
}

void InstrStackClear(){
    int i;
    for(i=0; i< instrStack.current; i++){
        free(instrStack.stack[i]);
    }
    instrStack.current = 0;
}

void GlobalVar(const char* name, PType* ptype){
    switch(ptype->type){
        case INTEGER_t:
            fprintf(fout, ".field public static %s %s\n", name, "I");
            break;
        case BOOLEAN_t:
            fprintf(fout, ".field public static %s %s\n", name, "Z");
            break;
        case FLOAT_t:
            fprintf(fout, ".field public static %s %s\n", name, "F");
            break;
        case DOUBLE_t:
            fprintf(fout, ".field public static %s %s\n", name, "D");
            break;
        default:
            printf("Unknown type\n");
            break;
    }
}

void ReadVar(ExprSem* expr){
    if(0 == hasRead){
        fprintf(fout, "new java/util/Scanner\n");
        fprintf(fout, "dup\n");
        fprintf(fout, "getstatic java/lang/System/in Ljava/io/InputStream;\n");
        fprintf(fout, "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
        fprintf(fout, "putstatic %s/_sc Ljava/util/Scanner;\n",fileName);
        fprintf(fout, "\n");
        hasRead = 1;
    }

    fprintf(fout, "getstatic %s/_sc Ljava/util/Scanner;\n",fileName);
    if(expr->varRef){
        char* id = expr->varRef->id;
        SymNode* node = lookupSymbol(symbolTable, id, scope, __FALSE);

        if(VARIABLE_t == node->category && 0 != node->scope){
            if(0 != node->scope){
                switch(expr->pType->type){
                    case INTEGER_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextInt()I\n");
                        fprintf(fout, "istore %d\n", node->attribute->varNo);
                        break;
                    case FLOAT_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextFloat()F\n");
                        fprintf(fout, "fstore %d\n", node->attribute->varNo);
                        break;
                    case DOUBLE_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextFloat()D\n");
                        fprintf(fout, "fstore %d\n", node->attribute->varNo);
                        break;
                    case BOOLEAN_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextBoolean()Z\n");
                        fprintf(fout, "istore %d\n", node->attribute->varNo);
                        break;
                }
            } else {
                switch(expr->pType->type){
                    case INTEGER_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextInt()I\n");
                        fprintf(fout, "putstatic %s %s I\n", fileName, node->name);
                        break;
                    case FLOAT_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextFloat()F\n");
                        fprintf(fout, "putstatic %s %s F\n", fileName, node->name);
                        break;
                    case DOUBLE_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextFloat()D\n");
                        fprintf(fout, "putstatic %s %s D\n", fileName, node->name);
                        break;
                    case BOOLEAN_t:
                        fprintf(fout, "invokevirtual java/util/Scanner/nextBoolean()Z\n");
                        fprintf(fout, "putstatic %s %s Z\n", fileName, node->name);
                        break;
                }
            }
        }
        
        InstrStackClear();
        InstrStackPrint();
    }

}

void FuncSt(const char* name, Param* param, PType* ret){
    
}

void FunctionCall(const char* name){
    SymNode* node = lookupSymbol(symbolTable, name, 0, __FALSE);
    if(node){
        char funcins[128];
        snprintf(funcins, sizeof(funcins), "invokestatic %s/%s(",fileName,name);
        
        PTypeList* list = node->attribute->formalParam->params;
        while(0 != list){
            switch(list->value->type){
                case INTEGER_t:
                    strcat(funcins, "I");
                    break;
                case FLOAT_t:
                    strcat(funcins, "F");
                    break;
                case DOUBLE_t:
                    strcat(funcins, "D");
                    break;
                case BOOLEAN_t:
                    strcat(funcins, "Z");
                    break;
            }
            list = list->next;
        }

        switch(node->type->type){
            case INTEGER_t:
                strcat(funcins, ")I");
                break;
            case FLOAT_t:
                strcat(funcins, ")F");
                snprintf(funcins, sizeof(funcins), "%s%s", funcins, ")F");
                break;
            case DOUBLE_t:
                strcat(funcins, ")D");
                break;
            case BOOLEAN_t:
                strcat(funcins, ")Z");
                break;
        }
        
        fprintf(fout, "%s\n", funcins);
        
    }   
}

void ConstExpr(ConstAttr* constattr){
    switch(constattr->category){
        case STRING_t:
            fprintf(fout, "ldc \"%s\"\n", constattr->value.stringVal);
            break;
        case INTEGER_t:
            fprintf(fout, "sipush %d\n", constattr->value.integerVal);
            break;
        case FLOAT_t:
            fprintf(fout, "ldc %lf\n", constattr->value.floatVal);
            break;
        case DOUBLE_t:
            fprintf(fout, "ldc \"%s\"\n", constattr->value.doubleVal);
            break;
        case BOOLEAN_t:
            fprintf(fout, "iconst_%d\n", constattr->value.booleanVal);
            break;
    }
}

void IdExpr(ExprSem* expr){
    if(!expr || !expr->varRef)
        return;

    char* name = expr->varRef->id;
    SymNode* node = lookupSymbol(symbolTable, name, scope, __FALSE);
    
    if(node){
        SEMTYPE t = node->category;
        printf("RRRRRRRRRR\n");
        if(CONSTANT_t == t){
            switch(expr->pType->type){
                case INTEGER_t:
                    fprintf(fout, "sipush %d\n", node->attribute->constVal->value.integerVal);
                    break;
                case FLOAT_t:
                    fprintf(fout, "ldc %lf\n", node->attribute->constVal->value.floatVal);
                    break;
                case DOUBLE_t:
                    fprintf(fout, "ldc %lf\n", node->attribute->constVal->value.doubleVal);
                    break;
                case BOOLEAN_t:
                    fprintf(fout, "iconst_%d\n", node->attribute->constVal->value.booleanVal);
                    break;
            }
        } else if((VARIABLE_t == t || PARAMETER_t == t) && 0 != node->scope){
            printf("RRRRRRRRRRVAR\n");
            printf("%s\n", name);
            switch(expr->pType->type){
                case INTEGER_t:
                    fprintf(fout, "iload %d\n", node->attribute->varNo);
                    break;
                case FLOAT_t:
                    fprintf(fout, "fload %d\n", node->attribute->varNo);
                    break;
                case DOUBLE_t:
                    fprintf(fout, "iload %d\n", node->attribute->varNo);
                    break;
                case BOOLEAN_t:
                    fprintf(fout, "iload %d\n", node->attribute->varNo);
                    break;
            }
        } else if(VARIABLE_t == t && 0 == node->scope){
            switch(expr->pType->type){
                case INTEGER_t:
                    fprintf(fout, "getstatic %s/%s I\n", fileName, node->name);
                    break;
                case FLOAT_t:
                    fprintf(fout, "getstatic %s/%s F\n", fileName, node->name);
                    break;
                case DOUBLE_t:
                    fprintf(fout, "getstatic %s/%s D\n", fileName, node->name);
                    break;
                case BOOLEAN_t:
                    fprintf(fout, "getstatic %s/%s Z\n", fileName, node->name);
                    break;
            }
            
        }
    }
}

