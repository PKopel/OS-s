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
	rm -f raport.txt tmp_a.txtb.txt tmp_a.txtc.txt tmp_a.txtd.txt tmp_a.txte.txt tmp_b.txta.txt tmp_b.txtc.txt tmp_b.txtd.txt tmp_c.txta.txt tmp_c.txtb.txt tmp_c.txtd.txt tmp_d.txta.txt tmp_d.txtb.txt tmp_d.txtc.txt tmp_e.txta.txt tmp_e.txtb.txt tmp_e.txtc.txt tmp_e.txtd.txt tmp_f.txta.txt tmp_f.txtb.txt tmp_f.txtc.txt tmp_f.txtd.txt tmp_f.txte.txt

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
