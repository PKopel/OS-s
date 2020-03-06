#ifndef difflib_h
#define difflib_h

#include <stdio.h>

struct Pair{
    FILE* file1;
    FILE* file2;
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

struct BlockTable create_table(int pair_number);
struct Sequence define_sequence(char* sequence);
struct TmpFiles compare(struct Sequence sequence);
struct Block create_block(FILE* tmp_file);
int operation_count(struct Block block);
void remove_block(struct Block* table);
void remove_operation(struct Block* block);

#endif 