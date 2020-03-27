#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/file.h>

void generate_matrix(FILE* file, int col, int row){
    for(int i = 0; i< row;i++){
        for(int j = 0; j< col;j++){
            fprintf(file,"\t%d",rand()%200 - 100);
        }
        fprintf(file,"\n");
    }
}

int main(int argc, char** argv){
    if (argc != 4) return 22;
    int number_of_files = atoi(argv[1]);
    int min_size = atoi(argv[2]);
    int max_size = atoi(argv[3]);
    srand(time(NULL));
    char* file_name = 
        (char*)calloc(20,sizeof(char));
    for(int i = 0;i<number_of_files;i++){
        int row_a, col_b, row_b_col_a;
        row_a = rand()%(max_size-min_size)+min_size;
        col_b = rand()%(max_size-min_size)+min_size;
        row_b_col_a = rand()%(max_size-min_size)+min_size;
        sprintf(file_name,"%s%03d","inA",i);
        FILE* file = fopen(file_name,"w");
        generate_matrix(file,row_b_col_a,row_a);
        fclose(file);
        sprintf(file_name,"%s%03d","inB",i);
        file = fopen(file_name,"w");
        generate_matrix(file,col_b,row_b_col_a);
        fclose(file);
    }
    free((void*)file_name);
    return 0;
}