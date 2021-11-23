CFLAGS=-Wall -pedantic -g

mysh: mysh.o run.o parse.o
	gcc -o mysh mysh.o run.o parse.o

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f mysh mysh.o run.o parse.o
