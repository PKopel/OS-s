CC = gcc -Wall

sys:clean
	$(CC) -c sysprog.c -o sysprog.o 
	ar rcs libsysprog.a sysprog.o

lib:clean
	$(CC) -c libprog.c -o libprog.o
	ar rcs liblibprog.a libprog.o 

clean:
	rm -f *.o
	rm -f *.a
	
prog:sys lib
	$(CC) program.c -o program.o -L. -llibprog -lsysprog

gen_test:prog
	./program.o generate test.txt 100 512

cpy_sys_test:prog
	./program.o copy test.txt test_sys.txt 100 512 sys

cpy_lib_test:prog
	./program.o copy test.txt test_lib.txt 100 512 lib

srt_sys_test:prog
	cat test.txt > test_o.txt
	./program.o sort test.txt 100 512 sys

srt_lib_test:prog
	cat test.txt > test_o.txt
	./program.o sort test.txt 100 512 lib