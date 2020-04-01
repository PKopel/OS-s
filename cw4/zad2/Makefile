CC = gcc -Wall
OPTION = handler

clean:
	rm -f *.o 

clean_all:clean
	rm -f raport*

build:clean
	$(CC) sig_inherit.c -o sig_inherit.o

run:build
	./sig_inherit.o $(OPTION)