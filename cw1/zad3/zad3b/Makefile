CC = gcc -Wall
SRC_PATH = ../zad2
LIB_PATH = ../zad1
OPT = O

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
	$(CC) $(SRC_PATH)/main.c -o main.o -L. -ldifflib -$(OPT)
	./main.o
	cat raport.txt >> results3b.txt

test_shared:shared
	$(CC) $(SRC_PATH)/main.c -o main.o -L. -Wl,-rpath=. -ldifflib -$(OPT)
	./main.o
	cat raport.txt >> results3b.txt

test_dynamic:shared
	$(CC) $(SRC_PATH)/main.c -o main.o -ldl -D DYNAMIC -$(OPT)
	./main.o
	cat raport.txt >> results3b.txt

test_all:
	echo "static\n" >> results3b.txt
	make test_static OPT=O
	echo "shared\n" >> results3b.txt
	make test_shared OPT=O
	make clean

test_all_02:
	echo "static O2\n" >> results3b.txt
	make test_static OPT=O2
	echo "shared O2\n" >> results3b.txt
	make test_shared OPT=O2
	make clean

test_all_0s:
	echo "static Os\n" >> results3b.txt
	make test_static OPT=Os
	echo "shared Os\n" >> results3b.txt
	make test_shared OPT=Os
	make clean

test_all_all:
	make test_all
	make test_all_02
	make test_all_0s