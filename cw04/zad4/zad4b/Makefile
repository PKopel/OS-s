CC = gcc -Wall

clean:
	rm -f *.o 

clean_all:clean
	rm -f raport*

build:clean
	$(CC) sender.c -o sender.o
	$(CC) catcher.c -o catcher.o