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
	rm -f raport.txt tmp_b.txta.txt 

test_static:static
	$(CC) $(SRC_PATH)/main.c -o main.o -L. -ldifflib
	echo static > raport.txt
	./main.o
	cat raport.txt >> raport2.txt
	make clean

test_shared:shared
	$(CC) $(SRC_PATH)/main.c -o main.o -L. -Wl,-rpath=. -ldifflib
	echo shared >> raport.txt
	./main.o
	cat raport.txt >> raport2.txt
	make clean

test_all:
	make test_static
	make test_shared
	make clean 
