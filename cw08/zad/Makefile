CC = gcc -Wall
IN = test.pgm
OUT = hist
THREADS = 4
MODE = block

clean:
	rm -f *.o

build: clean
	$(CC) histogram.c -o histogram.o -lpthread

run: build
	echo "$(MODE), $(THREADS) threads" >> Times.txt
	./histogram.o $(THREADS) $(MODE) $(IN) $(OUT).$(MODE).txt >> Times.txt

compare: build
	echo "\n block, $(THREADS) threads" >> Times.txt
	./histogram.o $(THREADS) block $(IN) $(OUT).block.txt  >> Times.txt
	echo "\n sign, $(THREADS) threads" >> Times.txt
	./histogram.o $(THREADS) sign $(IN) $(OUT).sign.txt  >> Times.txt
	echo "\n interleaved, $(THREADS) threads" >> Times.txt
	./histogram.o $(THREADS) interleaved $(IN) $(OUT).inter.txt  >> Times.txt
	echo "##################" >> Times.txt