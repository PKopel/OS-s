#include "difflib.h"
#include <string.h>


struct Pair create_pair(char* filename_a, char* filename_b){
    struct Pair newPair;
    newPair.file_a = fopen(filename_a,"r");
    newPair.file_b = fopen(filename_b,"r");
    return newPair;
}

struct Sequence define_sequence(char* sequence){
    char* words = strtok(sequence," ");
    int length = strlen(sequence);
    
    struct Sequence newSequence;
    newSequence.size = 0;
    for (int i = 0; i< length; i ++){
        if (sequence[i]==":") newSequence.size++;
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

