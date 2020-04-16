CC = gcc -Wall
FILE = test.txt

clean:
	rm -f *.o

build:clean
	$(CC) mysh.c -o mysh.o

build_debug:clean
	$(CC) mysh.c -g -o mysh.o

run: build
	./mysh.o $(FILE)