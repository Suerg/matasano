all: cryptlib

cryptlib: main.o cryptlib.o
	gcc main.o cryptlib.o -o cryptlib @$(HOME)/.gcc

main.o: main.c
	gcc  -c main.c @$(HOME)/.gcc

cryptlib.o: cryptlib.c
	gcc -c cryptlib.c @$(HOME)/.gcc

clean:
	trash *.o
