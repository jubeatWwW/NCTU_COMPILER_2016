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

