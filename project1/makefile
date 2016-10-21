OS := $(shell uname)
PROGRAM := lex.cpp
all: scanner

scanner: lex.yy.c
ifeq ($(OS),Darwin)
	gcc -o scanner lex.yy.c -ll
else
	gcc -o scanner lex.yy.c -lfl
endif
lex.yy.c:lextemplate.l
	lex lextemplate.l
run: 	
	./scanner $(PROGRAM)
clean:
	rm -f lex.yy.c
