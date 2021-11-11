CFLAGS=-Wall -pedantic -g

.PHONY: all
all: mysh

mysh: mysh.o
	gcc $(CFLAGS) -o mysh mysh.o

mysh.o: mysh.c
	gcc $(CFLAGS) -c -o mysh.o mysh.c

.PHONY: clean
clean:
	rm -f mysh mysh.o
