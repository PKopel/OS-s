#ifndef sysprog_h
#define sysprog_h

#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int sort_sys(char* filename, int length, int size);
int copy_sys(char* file1, char*file2, int length, int size);

#endif