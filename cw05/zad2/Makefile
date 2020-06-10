CC = gcc -Wall
FILE = test.txt

clean:
	rm -f *.o

build:clean
	$(CC) mysort.c -o mysort.o

run: build
	./mysort.o $(FILE)