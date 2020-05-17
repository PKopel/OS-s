CC = gcc -Wall
QUEUE = 10
CHAIRS = 4

clean:
	rm -f *.o

build: clean
	$(CC) golibroda.c -o golibroda.o -lpthread

run: build
	./golibroda.o $(CHAIRS) $(QUEUE)