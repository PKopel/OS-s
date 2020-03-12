static:clean
	gcc -Wall -c ../zad1/difflib.c -o difflib.o
	ar rcs libdifflib.a difflib.o

shared:clean
	gcc -fPIC -shared ../zad1/difflib.c -o libdifflib.so

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so

test_static:static
	gcc -Wall ../zad2/main.c -o main.o -L. -ldifflib
	./main.o

test_shared:shared
	gcc -Wall ../zad2/main.c -o main.o -L. -Wl,-rpath=. -ldifflib
	./main.o