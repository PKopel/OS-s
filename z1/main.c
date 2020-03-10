#include "difflib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char string[] = "a.txt:b.txt c.txt:d.txt a.txt:c.txt b.txt:d.txt";
    struct Sequence sequence = define_sequence(string);
    struct TmpFiles files = compare(sequence);
    struct BlockTable table = create_table(files);
    printf("%d\n",operation_count(table.table[0]));
    printf("%d\n",table.size);
    table.table[0] = remove_operation(table.table[0],0);
    printf("%d\n",operation_count(table.table[0]));
    printf("%d\n",table.size);
    table = remove_block(table,0);
    printf("%d\n",table.size);
    free((void*)table.table);
}
