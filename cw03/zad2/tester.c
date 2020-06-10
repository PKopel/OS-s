#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/file.h>


struct matrix{
    int cols;
    int rows;
    int **matrix;
};


struct matrix read_matrix(FILE* file){
    struct matrix new_matrix;
    new_matrix.cols = 0;
    new_matrix.rows = 0;
    char current, previous;
    do{
        fscanf(file,"\t%*d");
        current= fgetc(file);
        new_matrix.cols++;
    }while (current != '\n' && current != EOF);
    fseek(file,0,0);
    do{
        previous = current;
        current = fgetc(file);
        if(current == '\n' || (current == EOF && previous != '\n'))
            new_matrix.rows++;
    }while (current != EOF);
    fseek(file,0,0);
    new_matrix.matrix = 
        (int**)calloc(new_matrix.rows,sizeof(int*));
    for(int i = 0; i < new_matrix.rows; i++){
        new_matrix.matrix[i] = 
            (int*)calloc(new_matrix.cols,sizeof(int)); 
        for(int j = 0; j< new_matrix.cols; j++){
            fscanf(file,"\t%d",&(new_matrix.matrix[i][j]));
        }
    }
    return new_matrix;
}

void free_matrix(struct matrix M){
    for(int i = 0; i < M.rows; i++){
        free((void*)M.matrix[i]);
    }
    free((void*)M.matrix);    
}

void write_matrix(FILE* file, struct matrix M){
    for(int i = 0; i< M.rows;i++){
        for(int j = 0; j< M.cols;j++){
            fprintf(file,"\t%d",M.matrix[i][j]);
        }
        fprintf(file,"\n");
    }
}

int check_multiply(struct matrix A, struct matrix B, struct matrix C){
    int sum = 0;
    //if(C.cols != B.cols || C.rows != A.rows) return 1;
    for(int i = 0; i < A.rows; i++){
        for(int j = 0; j < B.cols; j++){
            for(int k = 0; k < A.cols; k++)
                sum+= A.matrix[i][k]*B.matrix[k][j];
            if(C.matrix[i][j] != sum) return 1;
            sum = 0;
        }
    }
    return 0;
}

int check_matrixes(char* list_name){

    char *file_a = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
         *file_b = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *file_c = 
        (char*)calloc(FILENAME_MAX,sizeof(char));
    FILE* list_file = fopen(list_name,"r");
    do{
        fscanf(list_file,"%s %s %s",file_a,file_b,file_c); 
        FILE* a = fopen(file_a,"r");
        FILE* b = fopen(file_b,"r");
        FILE* c = fopen(file_c,"r");
        struct matrix A = read_matrix(a);
        struct matrix B = read_matrix(b);
        struct matrix C = read_matrix(c);
        if(check_multiply(A,B,C)!=0) 
            printf("zły wynik w parze %s %s\n",file_a,file_b);
        else 
            printf("poprawny wyniki w parze %s %s\n",file_a, file_b);
        free_matrix(A);
        free_matrix(B);
        free_matrix(C);
    } while (getc(list_file) != EOF);
    free((void*)file_c);
    free((void*)file_b);
    free((void*)file_a);
    fclose(list_file);
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2) return 22;
    char* list = argv[1];
    return check_matrixes(list);
}