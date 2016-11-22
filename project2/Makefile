all: y.tab.c
	gcc scanner.c y.tab.c -ly -lfl

y.tab.c:
	yacc yacctemplate.y -d -v

clean:
	rm -rf y.tab.c y.tab.h y.output
