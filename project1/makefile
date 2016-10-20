scanner: lex.yy.c
	gcc -o scanner lex.yy.c -ll
lex.yy.c:lextemplate.l
	lex lextemplate.l
run: $(PROGRAM)
	./scanner lex.cpp
clean:
	rm -f lex.yy.c
