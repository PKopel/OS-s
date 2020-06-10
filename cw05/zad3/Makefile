CC = gcc -Wall
FILENUMBER = 3

clean:
	rm -f *.o

build:clean
	$(CC) producer.c -o producer.o
	$(CC) consumer.c -o consumer.o
	$(CC) generator.c -o generator.o
	$(CC) starter.c -o starter.o

generate: build
	./generator.o $(FILENUMBER)

run: build
	./generator.o 5
	./starter.o