CC = gcc -Wall
OPTION = handler

clean:
	rm -f *.o 

clean_all:clean
	rm -f raport*

build:clean
	$(CC) sig_inherit.c -o sig_inherit.o
	$(CC) exec_mother.c -o exec_mother.o
	$(CC) exec_child.c -o exec_child.o

run_fork:build
	./sig_inherit.o $(OPTION)

run_exec:build
	./exec_mother.o $(OPTION)

run_both:run_exec run_fork

run_all: clean_all
	make run_fork
	make run_both OPTION=mask
	make run_both OPTION=ignore
	make run_both OPTION=pending
