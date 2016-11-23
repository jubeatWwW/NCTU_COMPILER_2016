LEX = lex
LEXFN = lex.l
CC = gcc
CFLAGS = -O2
LIBS = -lfl
RM = rm -f
SCANNERN = scanner
SCANNERSN = scanner.c
TESTFN = whole.cm
all: scanner
	$(CC) $(SCANNERSN) y.tab.c -ly -lfl -o parser

scanner: lex
	$(CC) -o $(SCANNERN) $(SCANNERSN) $(LIBS)

lex: y.tab.c
	$(LEX) -o $(SCANNERSN) $(LEXFN) 

y.tab.c:
	yacc yacctemplate.y -d -v

test:
	./$(SCANNERN) $(TESTFN) 

clean:
	$(RM) $(SCANNERN)
	$(RM) $(SCANNERSN)
	$(RM) y.tab.c y.tab.h y.output

