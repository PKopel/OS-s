#ifndef difflib_h
#define difflib_h

#include <stdio.h>

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

struct Block{
    char** operations;
    int size;
};

struct BlockTable{
    struct Block* table;
    int size;
};

struct BlockTable create_table(struct TmpFiles sequence);
struct Sequence define_sequence(char* sequence);
struct TmpFiles compare(struct Sequence sequence);
struct Block create_block(FILE* tmp_file);
int operation_count(struct Block block);
void remove_block(struct BlockTable* table,int index);
void remove_operation(struct Block* block,int index);

#endif 