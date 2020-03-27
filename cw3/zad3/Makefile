CC = gcc -Wall
LIST = list
WORKERS = 1
TIME = 3
RESULT = s
CPU = 10
MEM = 10
MIN = 4
MAX = 6
FILES = 3


clean_all:
	rm -f *.o log msg out*

clean:
	rm -f log msg

build:clean
	$(CC) macierz.c -o macierz.o
	$(CC) ../zad2/generator.c -o generator.o
	$(CC) ../zad2/tester.c -o tester.o

generate:build
	./generator.o $(FILES) $(MIN) $(MAX)

run:build
	./macierz.o list $(WORKERS)  $(TIME) $(RESULT) $(CPU) $(MEM)

test:build
	./tester.o $(LIST)