CC = gcc -Wall 

clean:
	rm -f *.o
	rm -f *.a

build: clean
	$(CC) server.c -o server.o -lrt
	$(CC) client.c -o client.o -lrt

run_server: build
	./server.o

run_client: build
	./client.o