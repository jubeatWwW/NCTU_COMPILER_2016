TARGET = parser
OBJECT = SymbolTable.o lex.yy.c y.tab.c y.tab.o lex.yy.o y.output parser y.tab.h main.o
CC = gcc -g
LEX = flex
YACC = yacc -v
YACCFLAG = -d
LIBS = -lfl

all: $(TARGET)

parser:  SymbolTable.o y.tab.o lex.yy.o main.o	
	$(CC) -o $(TARGET) SymbolTable.o y.tab.o lex.yy.o main.o  $(LIBS)

y.tab.o: y.tab.c
	$(CC) -c y.tab.c

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

lex.yy.c: lex.l
	$(LEX) lex.l

main.o: main.c
	$(CC) -c main.c

SymbolTable.o: SymbolTable.c
	$(CC) -c SymbolTable.c

clean:
	rm -f $(TARGET) $(OBJECT)

