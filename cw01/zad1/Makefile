CC = gcc -Wall
SRC_PATH = ../zad2
LIB_PATH = ../zad1

static:clean
	$(CC) -c $(LIB_PATH)/difflib.c -o difflib.o
	ar rcs libdifflib.a difflib.o

shared:clean
	$(CC) -fPIC -shared $(LIB_PATH)/difflib.c -o libdifflib.so 

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so