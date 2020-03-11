#include "difflib.h"
#include <string.h>
#include <stdlib.h>


struct Pair create_pair(char* filename_a, char* filename_b){
    struct Pair new_pair;
    new_pair.file_a = 
        (char*) calloc(strlen(filename_a), sizeof(char));
    strcpy(new_pair.file_a, filename_a);
    new_pair.file_b = 
        (char*) calloc(strlen(filename_b), sizeof(char));
    strcpy(new_pair.file_b, filename_b);
    return new_pair;
}

struct Sequence define_sequence(char* sequence){
    int length = strlen(sequence);
    struct Sequence new_sequence;
    new_sequence.size = 0;
    for (int i = 0; i < length; i ++){
        if (sequence[i]==':') new_sequence.size++;
    }
    new_sequence.sequence =
        (struct Pair*)calloc(new_sequence.size,sizeof(struct Pair));
    
    char* words = strtok_r(sequence," ",&sequence);
    for (int i = 0; words != NULL && i < new_sequence.size; i++)
    {
        new_sequence.sequence[i] = 
            create_pair(strtok_r(words,":",&words),strtok_r(NULL,":",&words));
        words = strtok_r(NULL," ",&sequence);
    }   
    return new_sequence;
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
    struct Block new_block;
    char current, previous = '\0';
    int line_size = 0, operations = 0;
    while((current = fgetc(tmp_file)) != EOF){
        printf("%c",current);
        if (current <= 57 
            && current >= 48 
            && (previous == '\n' || !previous)){
            operations++;
        }
        previous = current;
    }
    fseek(tmp_file,0,0);
    new_block.size = operations;
    new_block.operations = 
        (char**)calloc(operations,sizeof(char*));
    for(int i = 0; i < operations; i++){
        current = fgetc(tmp_file);
        //printf("%c",current);
        if (current == EOF || 
            (current <= 57 
            && current >= 48 
            && previous == '\n')){
            new_block.operations[i] = 
                (char*)calloc(line_size,sizeof(char));
            fseek(tmp_file,-line_size,1);
            fread(new_block.operations[i],sizeof(char),line_size,tmp_file);
            //printf("%s\n\n\n",new_block.operations[i]);
            line_size=0;
        } else {
            line_size++;
        }
        previous = current;
    }
    return new_block;  
}

struct BlockTable create_table(struct TmpFiles sequence){
    struct BlockTable new_table;
    new_table.table = 
        (struct Block*)calloc(sequence.size,sizeof(struct Block));
    new_table.size = sequence.size;
    for (int i = 0; i < new_table.size; i++){
        new_table.table[i] = 
            create_block(sequence.tmp_files[i]);
    }
    return new_table;
}

int operation_count(struct Block block){
    return block.size;
}

struct BlockTable remove_block(struct BlockTable table, int index){
    struct Block* new_table = 
        (struct Block*)calloc(--table.size,sizeof(struct Block));
    int removed = 0;
    for(int i = 0; i< table.size;i ++){
        removed = i - removed == index ? 1 : 0;
        if (removed == 0){
            new_table[i]=table.table[i];
        } else {
            new_table[i]=table.table[i+1];
        }
    }
    free((void*)table.table);
    table.table=new_table;
    return table;
}

struct Block remove_operation(struct Block block, int index){
    char** new_operations = 
        (char**)calloc(--block.size,sizeof(char*));
    int removed = 0;
    for(int i = 0; i< block.size;i ++){
        removed = i - removed == index ? 1 : 0;
        if (removed == 0){
            new_operations[i]=block.operations[i];
        } else {
            new_operations[i]=block.operations[i+1];
        }
    }
    free((void*)block.operations);
    block.operations=new_operations;
    return block;
}