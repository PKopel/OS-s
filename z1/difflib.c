#include "difflib.h"
#include <string.h>
#include <stdlib.h>


struct Pair create_pair(char* filename_a, char* filename_b){
    struct Pair newPair;
    newPair.file_a = filename_a;
    newPair.file_b = filename_b;
    return newPair;
}

struct Sequence define_sequence(char* sequence){
    char* words = strtok(sequence," ");
    int length = strlen(sequence);
    
    struct Sequence newSequence;
    newSequence.size = 0;
    for (int i = 0; i< length; i ++){
        if (sequence[i]==':') newSequence.size++;
    }
    newSequence.sequence =
        (struct Pair*)calloc(newSequence.size,sizeof(struct Pair));
    
    for (int i = 0; words != NULL; i++)
    {
        newSequence.sequence[i] = 
            create_pair(strtok(words,":"),strtok(NULL,":"));
        words = strtok(NULL," ");
    }   
    return newSequence;
}

FILE* compare_pair(struct Pair pair){
    int len_a = strlen(pair.file_a);
    int len_b = strlen(pair.file_b);
    //command is "diff <file_a> <file_b> > tmp_<file_a><file_b>"
    char* command = (char*)calloc(2* (len_a + len_b+8),sizeof(char));
    char* tmp_file = (char*)calloc(4 + len_a + len_b,sizeof(char));
    //create name for tmp file
    strcpy(tmp_file,"tmp_");
    strcat(tmp_file,pair.file_a);
    strcat(tmp_file,pair.file_b);
    //create command
    strcpy(command,"diff ");
    strcat(command,pair.file_a);
    strcat(command," ");
    strcat(command,pair.file_b);
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
        files.tmp_files[i] = compare_pair(
            sequence.sequence[i]);
    }
    return files;
}

struct Block create_block(FILE* tmp_file){
    struct Block newBlock;
    FILE* start = tmp_file;
    char current, previous;
    int line_size = 0, operations = 0;
    while((current = fgetc(tmp_file)) != EOF){
        if (current <= 57 && current >= 48 && previous == '\n'){
            operations++;
        } else {
            previous = current;
        }
    }
    tmp_file = start;
    newBlock.size = operations;
    newBlock.operations = 
        (char**)calloc(operations,sizeof(char**));
    int i = 0;
    while((current = fgetc(tmp_file)) != EOF){
        if (current <= 57 && current >= 48 && previous == '\n'){
            newBlock.operations[i] = 
                (char*)calloc(line_size,sizeof(char*));
            fread(newBlock.operations[i],sizeof(char*),line_size,start);
            line_size=0;
        } else {
            previous = current;
            line_size++;
        }
    }
    return newBlock;  
}

int operation_count(struct Block block){
    return block.size;
}

void remove_block(struct BlockTable* table, int index){
    struct Block* newTable = 
        (struct Block*)calloc(--table->size,sizeof(struct Block*));
    int removed = 0;
    for(int i = 0; i< table->size-1;i ++){
        removed = i - removed == index ? 1 : 0;
        if (removed == 0){
            newTable[i]=table->table[i];
        } else {
            newTable[i]=table->table[i+1];
        }
    }
    free((void*)table->table);
    table->table=newTable;
}

void remove_operation(struct Block* block, int index){
    char** newOperations = 
        (char**)calloc(block->size,sizeof(char**));
    int removed = 0;
    for(int i = 0; i< block->size-1;i ++){
        removed = i - removed == index ? 1 : 0;
        if (removed == 0){
            newOperations[i]=block->operations[i];
        } else {
            newOperations[i]=block->operations[i+1];
        }
    }
    free((void*)block->operations);
    block->operations=newOperations;
}