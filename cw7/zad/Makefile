CC = gcc -Wall
SENDERS=3
PACKERS=3
RECEIVERS=3

clean:
	rm -f *.o

build_sv:clean
	$(CC) packer.c sv_sem.c common.c -o packer.o
	$(CC) sender.c sv_sem.c common.c -o sender.o
	$(CC) receiver.c sv_sem.c common.c -o receiver.o
	$(CC) master.c sv_sem.c common.c -o master.o

build_posix:clean
	$(CC) packer.c posix_sem.c common.c -o packer.o -lpthread -lrt
	$(CC) sender.c posix_sem.c common.c -o sender.o -lpthread -lrt
	$(CC) receiver.c posix_sem.c common.c -o receiver.o -lpthread -lrt
	$(CC) master.c posix_sem.c common.c -o master.o -lpthread -lrt

run:
	./master.o $(RECEIVERS) $(PACKERS) $(SENDERS)