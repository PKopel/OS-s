#ifndef difflib_h
#define difflib_h

#include <stdio.h>
#include <stdbool.h>

struct Pair{
    char* file_a;
    char* file_b;
};

struct Sequence{
    struct Pair* sequence;
    int size;
};

struct TmpFiles{
    FILE** tmp_files;
    int size;
};

struct BlockArray{
    char*** array;
    int size;
    int full;
};

struct BlockArray create_array(int size);
struct Sequence define_sequence(char* sequence);
struct TmpFiles compare(struct Sequence sequence);
int create_block(struct BlockArray* array, FILE* tmp_file);
int operation_count(char** block);
void remove_block(struct BlockArray* array,int index);
void remove_operation(char** block,int index);

#endif 