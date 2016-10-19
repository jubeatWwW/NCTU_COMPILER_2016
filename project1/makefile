scanner: lex.yy.c
	gcc -o scanner lex.yy.c -ll
lex.yy.c:lextemplate.l
	lex lextemplate.l
run:
	./scanner test.cpp
clean:
	rm -f lex.yy.c
