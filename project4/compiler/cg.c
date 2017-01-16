#include "cg.h"

int label = 0;
int labelStack[256];
int stTop = 0;

void ProgSt(const char* name){
    fprintf(fout, "; %s\n", name);
    fprintf(fout, ".class public %s\n", name);
    fprintf(fout, ".super java/lang/Object\n");
    fprintf(fout, ".field public static _sc Ljava/util/Scanner;\n");
}

void MainFunc(){
    fprintf(fout, ".method public static main([Ljava/lang/String;)V\n");
    fprintf(fout, ".limit stack 100\n");
    fprintf(fout, ".limit locals 100\n");
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

void PrintVarPre(){
    fprintf(fout, "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
}

void PrintVar(ExprSem* expr){
    switch(expr->pType->type){
        case INTEGER_t:
            fprintf(fout, "invokevirtual java/io/PrintStream/print(I)V\n");
            break;
        case FLOAT_t:
            fprintf(fout, "invokevirtual java/io/PrintStream/print(F)V\n");
            break;
        case DOUBLE_t:
            fprintf(fout, "invokevirtual java/io/PrintStream/print(D)V\n");
            break;
        case BOOLEAN_t:
            fprintf(fout, "invokevirtual java/io/PrintStream/print(Z)V\n");
            break;
        case STRING_t:
            fprintf(fout, "invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
            break;
    }
}

void AssignToVar(ExprSem* var, ExprSem* booleanExp){
    if(0 == var->varRef)
        return;
    char* name = var->varRef->id;
    SEMTYPE t = var->pType->type;

    SymNode* node = lookupSymbol(symbolTable, name, scope, __FALSE);

    if(0 == node)
        return;
    
    if(VARIABLE_t == node->category){
        if(0 == node->scope){
            switch(t){
                case INTEGER_t:
                    fprintf(fout, "putstatic %s/%s I\n", fileName, node->name);
                    break;
                case FLOAT_t:
                    if(INTEGER_t == booleanExp->pType->type)
                        fprintf(fout, "i2f\nputstatic %s/%s F\n", fileName, node->name);
                    else
                        fprintf(fout, "putstatic %s/%s F\n", fileName, node->name);
                    break;
                case DOUBLE_t:
                    if(INTEGER_t == booleanExp->pType->type)
                        fprintf(fout, "i2f\nputstatic %s/%s D\n", fileName, node->name);
                    else
                        fprintf(fout, "putstatic %s/%s D\n", fileName, node->name);
                    break;
                case BOOLEAN_t:
                    fprintf(fout, "putstatic %s/%s Z\n", fileName, node->name);
                    break;

            }
        } else {
            switch(t){
                case INTEGER_t:
                    fprintf(fout, "istore %d\n", node->attribute->varNo);
                    break;
                case FLOAT_t:
                    if(INTEGER_t == booleanExp->pType->type)
                        fprintf(fout, "i2f\nfstore %d\n", node->attribute->varNo);
                    else
                        fprintf(fout, "fstore %d\n", node->attribute->varNo);
                    break;
                case DOUBLE_t:
                    if(INTEGER_t == booleanExp->pType->type)
                        fprintf(fout, "i2f\nfstore %d\n", node->attribute->varNo);
                    else
                        fprintf(fout, "fstore %d\n", node->attribute->varNo);
                    break;
                case BOOLEAN_t:
                    fprintf(fout, "istore %d\n", node->attribute->varNo);
                    break;
            }
        }
    }
}

void ConditionSt(){
    label++;
    stTop++;
    labelStack[stTop] = label;
    fprintf(fout, "ifeq LFalse_%d\n", label);
}

void ConditionEnd(){
    fprintf(fout, "LFalse_%d:\n", labelStack[stTop--]);
}

void ConditionElse(){
    fprintf(fout, "goto LTrue_%d\n", labelStack[stTop]);
    fprintf(fout, "LFalse_%d:\n", labelStack[stTop]);
}

void ConditionElseEnd(){
    fprintf(fout, "LTrue_%d:\n", labelStack[stTop--]);
}

//i = 0;
void ForBegin(){
    label++;
    stTop++;
    labelStack[stTop] = label;
    fprintf(fout, "iconst_0\n"); //First loop flag
    fprintf(fout, "LRe_%d:\n", labelStack[stTop]);
}

//i<10;
void ForLogical(){
    fprintf(fout, "ifeq LFalse_%d\n", labelStack[stTop]);  //logical expression result check

    fprintf(fout, "ifeq LFirst_%d\n", labelStack[stTop]);  //jump the first loop calc
    fprintf(fout, "goto LBody_%d\n", labelStack[stTop]);

    fprintf(fout, "LBegin_%d:\n", labelStack[stTop]);

    
}

//i = i+1
void ForFirst(){
    fprintf(fout, "goto LRe_%d\n", labelStack[stTop]);
    fprintf(fout, "LFirst_%d:\n", labelStack[stTop]);
    fprintf(fout, "LBody_%d:\n", labelStack[stTop]);
}

//{   ...
void ForExit(){
    fprintf(fout, "iconst_1\n");
    fprintf(fout, "goto LBegin_%d\n", labelStack[stTop]);
    fprintf(fout, "LFalse_%d:\n", labelStack[stTop]);
    stTop--;
}

void WhileBegin(){
    label++;
    stTop++;
    labelStack[stTop] = label;
    fprintf(fout, "LWhile_%d:\n", labelStack[stTop]);
}

void WhileExit(){
    fprintf(fout, "ifeq LWhileExit_%d\n", labelStack[stTop]);
}

void WhileEnd(){
    fprintf(fout, "goto LWhile_%d\n", labelStack[stTop]);
    fprintf(fout, "LWhileExit_%d:\n", labelStack[stTop]);

}

void FuncSt(const char* name, Param* param, PType* ret){
    char funcdecl[128];
    snprintf(funcdecl, sizeof(funcdecl), ".method public static %s(", name);
    
    Param* list = param;
    NodeSem* node;
    while(0 != list){
        node = list->idlist;
        while(0 != node){
            switch(list->pType->type){
                case INTEGER_t:
                    strcat(funcdecl, "I");
                    break;
                case FLOAT_t:
                    strcat(funcdecl, "F");
                    break;
                case DOUBLE_t:
                    strcat(funcdecl, "D");
                    break;
                case BOOLEAN_t:
                    strcat(funcdecl, "Z");
                    break;
            }
            node = node->next;
        }
        list = list->next;
    }

    switch(ret->type){
        case INTEGER_t:
            strcat(funcdecl, ")I\n");
            break;
        case FLOAT_t:
            strcat(funcdecl, ")F\n");
            break;
        case DOUBLE_t:
            strcat(funcdecl, ")D\n");
            break;
        case BOOLEAN_t:
            strcat(funcdecl, ")Z\n");
            break;
        case VOID_t:
            strcat(funcdecl, ")V\n");
            break;
    }

    fprintf(fout, "%s", funcdecl);
    fprintf(fout, ".limit stack 100\n");
    fprintf(fout, ".limit locals 100\n");
}

void FuncEnd(PType* ret){
    if(INTEGER_t == ret->type || BOOLEAN_t == ret->type){
        fprintf(fout, "ireturn\n");
    } else if(FLOAT_t == ret->type || DOUBLE_t == ret->type){
        fprintf(fout, "freturn\n");
    } else{
        fprintf(fout, "return\n");
    }

    fprintf(fout, ".end method\n");
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

void FuncReturn(ExprSem* ret, int isMain){
    SEMTYPE t = ret->pType->type;
    if(1 == isMain){
        fprintf(fout, "return\n");
    } else if(INTEGER_t == t || BOOLEAN_t == t){
        fprintf(fout, "ireturn\n");
    } else if(FLOAT_t == t || DOUBLE_t == t){
        fprintf(fout, "freturn\n");
    } else {
        fprintf(fout, "return\n");
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

void Oper(ExprSem* e1, OPERATOR op, ExprSem* e2){
    if(e1 && e2){
        SEMTYPE t1 = e1->pType->type;
        SEMTYPE t2 = e2->pType->type;
        if(INTEGER_t == t1 && (FLOAT_t == t2 || DOUBLE_t == t2)){
            fprintf(fout, "i2f\n");
            e1->pType->type = t2;
        } else if(INTEGER_t == t2 && (FLOAT_t == t1 || DOUBLE_t == t1)){
            fprintf(fout, "i2f\n");
        }

        if(INTEGER_t == t1){
            switch(op){
                case ADD_t:
                    fprintf(fout, "iadd\n");
                    break;
                case SUB_t:
                    fprintf(fout, "isub\n");
                    break;
                case MUL_t:
                    fprintf(fout, "imul\n");
                    break;
                case DIV_t:
                    fprintf(fout, "idiv\n");
                    break;
                case MOD_t:
                    fprintf(fout, "irem\n");
                    break;
            }
        } else{
            switch(op){
                case ADD_t:
                    fprintf(fout, "fadd\n");
                    break;
                case SUB_t:
                    fprintf(fout, "fsub\n");
                    break;
                case MUL_t:
                    fprintf(fout, "fmul\n");
                    break;
                case DIV_t:
                    fprintf(fout, "fdiv\n");
                    break;
                case MOD_t:
                    fprintf(fout, "irem\n");
                    break;
            }
            
        }
        
    }
    
}

void Relation(ExprSem* e1, OPERATOR op, ExprSem* e2){
    SEMTYPE t1 = e1->pType->type;
    SEMTYPE t2 = e2->pType->type;

    if(INTEGER_t == t1){
        fprintf(fout, "isub\n");
    } else if(FLOAT_t == t1 || DOUBLE_t == t1){
        fprintf(fout, "fcmpl\n");
    }
    
    switch(op){
        case LT_t:
            fprintf(fout, "iflt Ltrue_%d\n", label);
            break;
        case LE_t:
            fprintf(fout, "ifle Ltrue_%d\n", label);
            break;
        case GT_t:
            fprintf(fout, "ifgt Ltrue_%d\n", label);
            break;
        case GE_t:
            fprintf(fout, "ifge Ltrue_%d\n", label);
            break;
        case EQ_t:
            fprintf(fout, "ifeq Ltrue_%d\n", label);
            break;
        case NE_t:
            fprintf(fout, "ifne Ltrue_%d\n", label);
            break;

    }

    fprintf(fout, "iconst_0\n");
    fprintf(fout, "goto Lfalse_%d\n", label);
    fprintf(fout, "Ltrue_%d:\n", label);
    fprintf(fout, "iconst_1\n");
    fprintf(fout, "Lfalse_%d:\n", label);
    label++;

}

void Boolean(OPERATOR op){
    switch(op){
        case AND_t:
            fprintf(fout, "iand\n");
            break;
        case OR_t:
            fprintf(fout, "ior\n");
            break;
        case NOT_t:
            fprintf(fout, "iconst\nnixor\n");
            break;
    }
}

void Negative(ExprSem* exp){
    if(INTEGER_t == exp->pType->type){
        fprintf(fout, "ineg\n");
    } else if(FLOAT_t == exp->pType->type || DOUBLE_t == exp->pType->type){
        fprintf(fout, "fneg\n");
    }
}

