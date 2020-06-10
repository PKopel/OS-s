#include "difflib.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


char** create_pair(char* filename_a, char* filename_b){
    char** pair = 
        (char**)calloc(2,sizeof(char*));
    pair[0] = 
        (char*) calloc(strlen(filename_a), sizeof(char));
    strcpy(pair[0], filename_a);
    pair[1] = 
        (char*) calloc(strlen(filename_b), sizeof(char));
    strcpy(pair[1], filename_b);
    return pair;
}

struct Sequence define_sequence(char* sequence){
    int length = strlen(sequence);
    struct Sequence new_sequence;
    new_sequence.size = 0;
    //count number of pairs in sequence
    for (int i = 0; i < length; i ++){
        if (sequence[i]==':') new_sequence.size++;
    }
    new_sequence.sequence =
        (char***)calloc(new_sequence.size,sizeof(char**));
    //split sequence to pairs "file_a:file_b"
    char* words = strtok_r(sequence," ",&sequence);
    for (int i = 0; words != NULL && i < new_sequence.size; i++)
    {
        //get filenames from pairs "file_a:file_b"
        new_sequence.sequence[i] = 
            create_pair(strtok_r(words,":",&words),strtok_r(NULL,":",&words));
        words = strtok_r(NULL," ",&sequence);
    }   
    return new_sequence;
}

FILE* compare_pair(char** pair){
    int len_a = strlen(pair[0]);
    int len_b = strlen(pair[1]);
    //command is "diff <file_a> <file_b> > tmp_<file_a><file_b>"
    char* command = (char*)calloc(2* (len_a + len_b+8),sizeof(char));
    char* tmp_file = (char*)calloc(4 + len_a + len_b,sizeof(char));
    //create name for tmp file
    strcpy(tmp_file,"tmp_");
    strcat(tmp_file,pair[0]);
    strcat(tmp_file,pair[1]);
    //create command
    strcpy(command,"diff ");
    strcat(command,pair[0]);
    strcat(command," ");
    strcat(command,pair[1]);
    strcat(command," > ");
    strcat(command,tmp_file);
    system(command);
    return fopen(tmp_file,"r");
}

struct TmpFiles compare(struct Sequence sequence){
    struct TmpFiles files;
    files.tmp_files = 
        (FILE**)calloc(sequence.size,sizeof(FILE*));
    files.size = sequence.size;
    for( int i = 0; i< sequence.size;i++){
        files.tmp_files[i] = 
            compare_pair(sequence.sequence[i]);
    }
    return files;
}

int size(char** block){
    int size = 0;
    while(block[size]!=NULL) size++;
    return size;
}

char** create_block_hidden(FILE* tmp_file){
    char current, previous = '\0';
    int line_size = 0, operations = 0;
    //count operations
    while((current = fgetc(tmp_file)) != EOF){;
        if (current <= '9' 
            && current >= '0' 
            && (previous == '\n' || !previous)){
            operations++;
        }
        previous = current;
    }
    //reset pointer and previous
    fseek(tmp_file,0,0);
    previous = '\0';
    char** new_block = 
        (char**)calloc(operations+1,sizeof(char*));
    //aditional array for sizes of operations
    int* line_sizes = 
        (int*)calloc(operations,sizeof(int));
    //count sizes of operations
    for(int i = 0; i < operations; previous = current){
        current = fgetc(tmp_file);
        if (current == EOF || 
            (current <= '9' 
            && current >= '0'
            && previous == '\n'))
        {
            line_sizes[i++] = line_size;
            line_size=0;
        }
        line_size++;
    }
    //reset pointer again
    fseek(tmp_file,0,0);
    //copy operations from file to block
    for(int i = 0; i < operations; i++){
        new_block[i] = 
            (char*)calloc(line_sizes[i],sizeof(char));
        fread(new_block[i],sizeof(char),line_sizes[i],tmp_file);
    }
    free((void*)line_sizes);
    new_block[operations]=NULL;
    return new_block;  
}

int create_block(struct BlockArray* array, FILE* tmp_file){
    if (array->full == array->size) {
        array->array = 
            (char***)realloc((void*)array->array
                        ,(++array->full)*sizeof(char**));
    }
    array->array[array->size] = create_block_hidden(tmp_file);
    return array->size++;
}

struct BlockArray create_array(int size){
    struct BlockArray new_array;
    new_array.array = 
        (char***)calloc(size,sizeof(char**));
    new_array.size = 0;
    new_array.full = size;
    return new_array;
}

int operation_count(char** block){
    return size(block);
}

void remove_block(struct BlockArray* array, int index){
    array->size--;
    for(int i = 0; i< array->size;i ++){
        //if i >= index, then one item should be omitted
        array->array[i] = 
            array->array[i >= index ? i + 1 : i];
    }
}

void remove_operation(char** block, int index){
    int bsize = size(block);
    for(int i = 0; i< bsize;i ++){
        //if i >= index, then one item should be omitted,
        // last one (block[bsize]) is NULL
        block[i] = block[i >= index ? i + 1 : i];
    }
    block=
        (char**)realloc((void*)block,bsize*sizeof(char*));
}