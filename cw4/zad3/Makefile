CC = gcc -Wall

clean:
	rm -f *.o 

clean_all:clean
	rm -f raport*

build:clean
	$(CC) siginfo.c -o siginfo.o

run:build
	./siginfo.o