#ifdef DYNAMIC
#include "../zad3a/dynamic.h"
#else
#include "../zad1/difflib.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

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

void write_time(char* name,struct tms start,struct tms end,clock_t real_start,clock_t real_end, FILE* file){
    printf("%s:\n",name);
    fprintf(file,"%s:\n",name);
    double real = (double)(real_end - real_start)/sysconf(_SC_CLK_TCK);
    double user = (double)(end.tms_utime - start.tms_utime)/sysconf(_SC_CLK_TCK);
    double system = (double)(end.tms_stime - start.tms_stime)/sysconf(_SC_CLK_TCK);
    printf("real:\t%f\tuser:\t%f\tsystem:\t%f\t\n",
            real,user,system);
    fprintf(file, "real:\t%f\tuser:\t%f\tsystem:\t%f\t\n",
            real,user,system);
}

void run(char* string,char* desc){
    struct tms start, end;
    clock_t real_start, real_end;
    FILE* report = fopen("raport.txt","a");
    printf("##########################\n%s %s\n",desc,string);
    fprintf(report,"##########################\n%s %s\n",desc,string);

    real_start = times(&start);
    struct Sequence sequence = define_sequence(string);
    struct TmpFiles files = compare(sequence);
    real_end = times(&end);
    write_time("porównanie par",start,end,real_start,real_end,report);

    real_start = times(&start);
    array = create_array(sequence.size);
    for (int i = 0; i < files.size; i++)
    {
        create_block(&array,files.tmp_files[i]);
        fseek(files.tmp_files[i],0,0);
    }
    real_end = times(&end);
    write_time("zapisanie bloków",start,end,real_start,real_end,report);
    
    real_start = times(&start);
    remove_operation(array.array[0],0);
    real_end = times(&end);
    write_time("usunięcie operacji",start,end,real_start,real_end,report);

    real_start = times(&start);
    remove_block(&array,1);
    real_end = times(&end);
    write_time("usunięcie bloku",start,end,real_start,real_end,report);

    real_start = times(&start);
    for (int i = 0; i < 100000; i++)
    {
        remove_block(&array,i%3);
        create_block(&array,files.tmp_files[i%files.size]);
        fseek(files.tmp_files[i%files.size],0,0);
    }
    real_end = times(&end);
    write_time("usuwanie i zapisanie 100000 bloków",start,end,real_start,real_end,report);
    fclose(report);
    for (int i = 0; i < files.size; i++)
    {
        fclose(files.tmp_files[i]);
    }
    for(int i = 0; i<array.full;i++){
        int j = 0;
        if(array.array[i])
        while(array.array[i][j]!=NULL)
        {
            if(array.array[i][j])
            free((void*)array.array[i][j++]);
        }
        free((void*)array.array[i]);
    }
    free((void*)array.array);
}

int main(void){

    #ifdef DYNAMIC
    init_dynamic();
    #endif

    char small[] = "a.txt:b.txt c.txt:d.txt a.txt:c.txt b.txt:d.txt ";
    char medium[] = "a.txt:b.txt c.txt:d.txt e.txt:f.txt a.txt:c.txt b.txt:d.txt c.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:f.txt";
    char big[] = "a.txt:b.txt a.txt:c.txt a.txt:d.txt a.txt:e.txt a.txt:f.txt b.txt:a.txt b.txt:c.txt b.txt:d.txt b.txt:e.txt b.txt:f.txt c.txt:a.txt c.txt:b.txt c.txt:d.txt c.txt:e.txt c.txt:f.txt d.txt:a.txt d.txt:b.txt d.txt:c.txt d.txt:e.txt d.txt:f.txt e.txt:a.txt e.txt:f.txt";
    char very_similar[] = "a.txt:b.txt c.txt:d.txt e.txt:f.txt";
    char a_bit_similar[] = "a.txt:e.txt c.txt:e.txt d.txt:f.txt";
    char not_similar[] = "a.txt:c.txt a.txt:d.txt b.txt:d.txt"; 
    run(small,"mała ilość");
    run(medium,"średnia ilość");
    run(big,"duża ilość");
    run(very_similar,"małe różnice");
    run(a_bit_similar,"średnie różnice");
    run(not_similar,"duże różnice");
}
