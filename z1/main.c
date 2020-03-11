#include "difflib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test(struct BlockTable table){
    printf("%d\n\n",table.size);
    for (int i = 0; i < table.size; i++)
    {
        printf("%d\n\n",operation_count(table.table[i]));
        for(int j = 0; j< table.table[i].size;j++){
            printf("%s\n",table.table[i].operations[j]);
        }
    }
    printf("\n");
}

int main(void){
    char string[] = "a.txt:b.txt c.txt:d.txt a.txt:c.txt b.txt:d.txt";
    struct Sequence sequence = define_sequence(string);
    struct TmpFiles files = compare(sequence);
    struct BlockTable table = create_table(files);
    test(table);
    table.table[0] = remove_operation(table.table[0],0);
    test(table);
    table = remove_block(table,0);
    test(table);
    free((void*)table.table);
}
