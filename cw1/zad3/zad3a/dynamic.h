#ifndef dynamic_h
#define dynamic_h
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

struct BlockArray (*_create_array)(int size);
struct Sequence (*_define_sequence)(char *sequence);
struct TmpFiles (*_compare)(struct Sequence sequence);
int (*_operations_count)(char** block);
int (*_create_block)(struct BlockArray* array, FILE* tmp_file);
void (*_remove_block)(struct BlockArray* array, int idx);
void (*_remove_operation)(char** block, int indx);

void init_dynamic()
{
    void*  difflib = dlopen("difflib.so", RTLD_NOW);
    if ( difflib == NULL)
    {
        fprintf(stderr, "Error while opening dynamic library");
        return;
    }

    _create_array = (BlockArray (*)(int))dlsym( difflib, "create_array");
    _create_block = (int (*)(BlockArray*,FILE*) )dlsym( difflib, "create_block");
    _define_sequence = (Sequence (*)(char*) )dlsym( difflib, "define_sequence");
    _compare = (TmpFiles (*)(Sequence) )dlsym( difflib, "compare");
    _operations_count = (int (*)(char**) )dlsym( difflib, "operation_count");
    _remove_block = (void (*)(BlockArray*,int) )dlsym( difflib, "remove_block");
    _remove_operation = (void (*)(char**,int) )dlsym( difflib, "remove_operation");

    char *error;
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

struct BlockArray create_array(int size)
{
    return (*_create_array)(size);
}
struct Sequence define_sequence(char *sequence)
{
    return (*_define_sequence)(sequence);
}
struct TmpFiles compare(struct Sequence sequence )
{
    return (*_compare)(sequence);
}
int operations_count(char** block)
{
    return (*_operations_count)(block);
}
int create_block(struct BlockArray* array, FILE* tmp_file)
{
    return (*_create_block)(array,tmp_file);
}
void remove_block(struct BlockArray* array, int idx)
{
    (*_remove_block)(array,idx);
}
void remove_operation(char** block, int indx)
{
    (*_remove_operation)(block, indx);
}

struct Sequence{
    char** sequence;
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


#endif