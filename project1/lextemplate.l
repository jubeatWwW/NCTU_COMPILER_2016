%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define tokenChar(t)        { LIST; if (Opt_Token) printf("<\"%s\">\n", t); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
#define true 1
#define false 0
typedef int bool;
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
char dict[256][256];
int cnt[256];
int dict_num = 0;
%}

%Start COMMENT PRAGMA


%%
<INITIAL>\/\*.*\*\/ {
    LIST;
}

<COMMENT>.*\*\/ {
    LIST;
    BEGIN INITIAL;
}

<INITIAL>\/\*.* {
    LIST;
    BEGIN COMMENT;
}

<COMMENT>. {
    LIST;
}

<INITIAL>\/\/.* {
    LIST;
}

<INITIAL>[(){};\[\],:] {
    tokenString("delim", yytext); 
}
<INITIAL>(==|<=|>=|!=|&&|\|\|) {
    tokenChar(yytext);
}

<INITIAL>[<>=!\+\-\*\/%] {
    tokenChar(yytext);
}

<INITIAL>[0-9]*\.[0-9]+[Ee][+-]?[0-9]+ {
    tokenString("scientific", yytext);
}

<INITIAL>(0|[1-9]+)\.[0-9]+ {
    tokenString("float", yytext);
}

<INITIAL>0|([1-9][0-9]*) {
    tokenString("integer", yytext);
}

<INITIAL>\"(\\.|[^\\"])*\" {
    int len = yyleng;
    char newStr[len];
    int oldIndex = 1, newIndex = 0;
    while(oldIndex < len){
        if(yytext[oldIndex]=='\\' && yytext[oldIndex+1]=='\"'){
            oldIndex++;
        }else if(yytext[oldIndex]=='\\' && yytext[oldIndex+1]=='\\'){
            oldIndex++;
        }

        newStr[newIndex++] = yytext[oldIndex++];
    }
    newStr[newIndex-1] = '\0';
    tokenString("string", newStr);
}

<INITIAL>while|do|if|else|true|false|for|int|print|const|read|boolean|bool|void|float|double|string|continue|break|return {
    tokenString("KW", yytext);
}

<INITIAL>^#pragma\40(source)\40(on|off) {
    LIST;
    if(yytext[strlen(yytext) - 1] == 'n')
        Opt_Source = 1;
    else
        Opt_Source = 0;
    BEGIN PRAGMA;
}

<INITIAL>^#pragma\40(token)\40(on|off) {
    LIST;
    if(yytext[strlen(yytext) - 1] == 'n')
        Opt_Token = 1;
    else
        Opt_Token = 0;
    BEGIN PRAGMA;
}

<INITIAL>^#pragma\40(statistic)\40(on|off) {
    LIST;
    if(yytext[strlen(yytext) - 1] == 'n')
        Opt_Statistic = 1;
    else
        Opt_Statistic = 0;
    BEGIN PRAGMA;
}

<PRAGMA>\/\/.* {
    LIST;
}

<PRAGMA>\/\*.*\*\/ {
    LIST;
}

<PRAGMA>\/\*.* {
    LIST;
    BEGIN COMMENT;
}

<PRAGMA>\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
    BEGIN INITIAL;
}

<INITIAL>[a-zA-Z][a-zA-Z0-9]* {
    if(yyleng>=MAX_ID_LENGTH){
        REJECT;
    }
    if(Opt_Statistic){
        bool flag = true;
        for(int i=0; i<dict_num; i++){
            int cursor = 0;
            bool same = true;
            while(cursor < yyleng){
                if(dict[i][cursor]!=yytext[cursor] || dict[i][yyleng] != '\0'){
                    same = false;    
                    break;    
                }
                cursor++;
            }
            if(same){
                flag = false;
                cnt[i]++;
                break;           
            }
        }
        if(flag){
            int i = 0;
            while(i<yyleng){
                dict[dict_num][i] = yytext[i];
                i++;
            }
            dict[dict_num][i] = '\0';
            cnt[dict_num] = 1;
            dict_num++;
        }
    }

    tokenString("id", yytext);
}


\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

^\40+ ;

\40 {
    LIST;
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic)
    {
        // Print frequency table
        printf("\nfrequency of identifiers:\n");
        for(int i=0; i<dict_num; i++){
            printf("%s\t%d\n", dict[i], cnt[i]);
        }
    }

    exit(0);
}
