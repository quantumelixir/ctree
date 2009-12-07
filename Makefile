example : example.o ctree.o
	gcc -Wall example.o ctree.o -lncurses -o example

debug : example.c ctree.c ctree.h
	gcc -g -Wall example.c ctree.c -lncurses -o debug

example.o : example.c
	gcc -Wall -c example.c

ctree.o : ctree.c ctree.h
	gcc -Wall -c ctree.c
