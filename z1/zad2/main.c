#include "../zad1/difflib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

struct BlockArray array;

void test_block(char** block){
    printf("number of operations:%d\n\n",operation_count(block));
    int i = 0;
    while (block[i] != NULL){
        printf("%s\n",block[i++]);
    }
}

void test(struct BlockArray array){
    printf("number of blocks:%d\n\n",array.size);
    for (int i = 0; i < array.size; i++){
        test_block(array.array[i]);
    }
    printf("\n");
}

int main(void){
    char small[] = "a.txt:b.txt c.txt:d.txt a.txt:c.txt b.txt:d.txt ";
    char medium[] = "a.txt:b.txt c.txt:d.txt e.txt:f.txt a.txt:c.txt b.txt:d.txt c.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:f.txt";
    char big[] = "a.txt:b.txt a.txt:c.txt a.txt:d.txt a.txt:e.txt a.txt:f.txt b.txt:a.txt b.txt:c.txt b.txt:d.txt b.txt:e.txt b.txt:f.txt c.txt:a.txt c.txt:b.txt c.txt:d.txt c.txt:e.txt c.txt:f.txt d.txt:a.txt d.txt:b.txt d.txt:c.txt d.txt:e.txt d.txt:f.txt e.txt:a.txt e.txt:f.txt";
    char very_similar[] = "a.txt:b.txt c.txt:d.txt e.txt:f.txt";
    char a_bit_similar[] = "a.txt:e.txt c.txt:e.txt d.txt:f.txt";
    char not_similar[] = "a.txt:c.txt a.txt:d.txt b.txt:d.txt"; 
    struct Sequence sequence = define_sequence(big);
    struct TmpFiles files = compare(sequence);
    array = create_array(sequence.size*2);
    for (int i = 0; i < files.size; i++)
    {
        printf("%d",create_block(&array,files.tmp_files[i]));
        fseek(files.tmp_files[i],0,0);
    }
    
    test(array);
    remove_operation(array.array[0],0);
    test(array);
    remove_block(&array,1);
    test(array);
    test_block(array.array[create_block(&array,files.tmp_files[0])]);
    test(array);
    test_block(array.array[create_block(&array,files.tmp_files[1])]);
    test(array);
    for(int i = 0; i<array.full;i++){
        int j = 0;
        if(array.array[i])
        while(array.array[i][j]!=NULL)
        {
            free((void*)array.array[i][j++]);
        }
        free((void*)array.array[i]);
    }
    free((void*)array.array);
}
