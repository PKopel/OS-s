static:clean
	gcc -Wall -c difflib.c -o difflib.o
	ar rcs libdifflib.a difflib.o

shared:clean
	gcc -fPIC -shared difflib.c -o libdifflib.so

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so