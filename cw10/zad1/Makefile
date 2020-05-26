CC = gcc -Wall

PORT = 8888
US_PATH = abcde

CLIENT_NAME = client
CONNECTION = unix
ADDRESS = localhost:8888 

clean:
	rm -f *.o

build: clean
	$(CC) server.c ../server_common.c ../common.c -o server.o -lpthread
	$(CC) client.c ../client_common.c ../common.c -o client.o

run_server: build
	./server.o $(PORT) $(US_PATH)

run_client: build
	./client.o $(CLIENT_NAME) $(CONNECTION) $(ADDRESS)