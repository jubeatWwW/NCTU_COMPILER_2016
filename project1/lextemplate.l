%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define tokenChar(t)        { LIST; if (Opt_Token) printf("<\"%s\">\n", t); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
int isComment = 0;
char buf[MAX_LINE_LENGTH];
%}

%Start COMMENT


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

<INITIAL>[0-9]*\.[0-9]+([Ee][+-])[0-9]+ {
    tokenString("scientific", yytext);
}

<INITIAL>(0|[1-9]+)\.[0-9]+ {
    tokenString("float", yytext);
}

<INITIAL>0|[1-9]*[0-9]+ {
    tokenString("integer", yytext);
}

<INITIAL>\"(\\.|[^\\"])*\" {
    tokenString("string", yytext);
}

<INITIAL>while|do|if|else|true|false|for|int|print|const|read|boolean|bool|void|float|double|string|continue|break|return {
    tokenString("KW", yytext);
}

<INITIAL>[a-zA-Z0-9]+ {
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
    }

    exit(0);
}
