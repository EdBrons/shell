CFLAGS=-Wall -pedantic -g

mysh: mysh.o 
	gcc -g -o mysh mysh.o 

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f mysh mysh.o 
