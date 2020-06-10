CC = gcc -Wall
LENGTH = 100
BLOCK = 512
MODE = lib
FILE = test.txt
RESULT = result.txt
BACKUP = backup.txt

sys:clean
	$(CC) -c sysprog.c -o sysprog.o 
	ar rcs libsysprog.a sysprog.o

lib:clean
	$(CC) -c libprog.c -o libprog.o
	ar rcs liblibprog.a libprog.o 

clean:
	rm -f *.o
	rm -f *.a
	rm -f tmp_$(FILE)
	rm -f $(RESULT)
	rm -f $(BACKUP)
	
prog:sys lib
	$(CC) program.c -o program.o -L. -llibprog -lsysprog

test_gen:
	./program.o generate $(FILE) $(LENGTH) $(BLOCK)

test_copy:
	./program.o copy $(FILE) $(RESULT) $(LENGTH) $(BLOCK) $(MODE)

test_sort:
	cat $(FILE) > $(BACKUP)
	./program.o sort $(FILE) $(LENGTH) $(BLOCK) $(MODE)

reset_test:
	cat $(BACKUP) > $(FILE)
	rm -f tmp_$(FILE)

compare_sort:
	make test_sort BACKUP=test_srt.txt MODE=sys
	make test_sort FILE=test_srt.txt MODE=lib

compare_copy:
	make test_copy RESULT=test_copy_sys.txt MODE=sys
	make test_copy RESULT=test_copy_lib.txt MODE=lib

compare_all: prog
	echo "porównanie kopiowania\n" > wynik.txt
	make test_gen LENGTH=10000 BLOCK=8192
	make compare_copy LENGTH=10000 BLOCK=8192
	make compare_copy LENGTH=20000 BLOCK=4096
	make compare_copy LENGTH=40000 BLOCK=2048
	make compare_copy LENGTH=80000 BLOCK=1024
	make compare_copy LENGTH=160000 BLOCK=512
	make compare_copy LENGTH=20322500 BLOCK=4
	make compare_copy LENGTH=81290000 BLOCK=1
	echo "porównanie sortowania\n" >> wynik.txt
	make test_gen LENGTH=1000 BLOCK=8192
	make compare_sort LENGTH=1000 BLOCK=8192
	make compare_sort LENGTH=2000 BLOCK=4096
	make compare_sort LENGTH=4000 BLOCK=2048
	make compare_sort LENGTH=8000 BLOCK=1024
	make compare_sort LENGTH=16000 BLOCK=512
	make test_gen LENGTH=100000 BLOCK=4
	make compare_sort LENGTH=100000 BLOCK=4
	make compare_sort LENGTH=200000 BLOCK=1