CC = gcc -Wall
FILE = test.txt

clean:
	rm -f *.o

build:clean
	$(CC) mysh.c -o mysh.o

run: build
	./mysh.o $(FILE)