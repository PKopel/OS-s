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

test_gen:prog
	./program.o generate $(FILE) $(LENGTH) $(BLOCK)

test_cpy:prog
	./program.o copy $(FILE) $(RESULT) $(LENGTH) $(BLOCK) $(MODE)

test_srt:prog
	cat $(FILE) > $(BACKUP)
	./program.o sort $(FILE) $(LENGTH) $(BLOCK) $(MODE)

reset_test:
	cat $(BACKUP) > $(FILE)
	rm -f tmp_$(FILE)
