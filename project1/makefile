OS := $(shell uname)
scanner: lex.yy.c
	ifeq ($(OS),Darwin)
		gcc -o scanner lex.yy.c -ll
	else
		gcc -o scanner lex.yy.c -ll
	endif
lex.yy.c:lextemplate.l
	lex lextemplate.l
run: $(PROGRAM)
	./scanner lex.cpp
clean:
	rm -f lex.yy.c
