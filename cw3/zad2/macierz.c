#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/file.h>

struct matrix{
    int cols;
    int rows;
    int **matrix;
};

struct matrix multiply(struct matrix A, struct matrix B, int start, int end){
    int sum = 0;
    struct matrix result;
    result.rows = A.rows;
    result.cols = 0;
    result.matrix = 
        (int**)calloc(end - start,sizeof(int));
    for(int j = start; j < end; j++){
        result.matrix[result.cols] = 
                (int*)calloc(A.rows,sizeof(int));
        for(int i = 0; i < A.rows; i++){
            for(int k = 0; k < A.cols; k++)
                sum+= A.matrix[i][k]*B.matrix[k][j];
            result.matrix[result.cols][i] = sum;
            sum = 0;
        }
        result.cols++;
    }
    return result;
}

struct matrix read_matrix(char** file_name){
    FILE* file = fopen(file_name,"r");
    struct matrix new_matrix;
    new_matrix.cols = 0;
    new_matrix.rows = 0;
    char t;
    do{
        fscanf(file,"%*d");
        t= fgetc(file);
        new_matrix.cols++;
    }while (t != '\n' && t != EOF);
    fseek(file,0,0);
    do{
        t= fgetc(file);
        if(t == '\n' || t == EOF)
        new_matrix.rows++;
    }while (t != EOF);
    fseek(file,0,0);
    new_matrix.matrix = 
        (int**)calloc(new_matrix.rows,sizeof(int*));
    for(int i = 0; i < new_matrix.rows; i++){
        new_matrix.matrix[i] = 
            (int*)calloc(new_matrix.cols,sizeof(int)); 
        for(int j = 0; j< new_matrix.cols; j++){
            fscanf(file,"%d",&(new_matrix.matrix[i][j]));
        }
    }
    fclose(file);
    return new_matrix;
}

void write_matrix(char** file_name, struct matrix M){
    FILE* file = fopen(file_name,"w");
    for(int i = 0; i< M.rows;i++){
        for(int j = 0; j< M.cols;j++){
            fprintf(file,"%d\t",M.matrix[j][i]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}
/*
startuje timer;
dopóki nie przekroczę czasu:
otwieram plik lista do odczytu;
wczytuję nazwy a b i c;
sprawdzam czy ilość kolumn b jest mniejsza niż ilość procesów:
tak: sprawdzam czy mój numer jest większy od ilości kolumn b
    tak: wczytuję kolejną linię z listy
    nie: zaczynam mnożyć kolumnę pod moim numerem
nie: zaczynam mnożyć fragment b od kolumny 
    (mój numer - ilość bloków które pominąłem)*ilość kolumn b / ilość wszystkich procesów 
    do (mój numer - ilość bloków które pominąłem + 1)*ilość kolumn b / ilość wszystkich procesów
*/


int proces(char* list, int workers_num, int time_max, int write_type){
    FILE* list_file = fopen(list,"r");
    FILE* msg = fopen("msg","r+");
    int msg_d = fileno(msg);
    char *file_a = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
         *file_b = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *file_c = 
        (char*)calloc(FILENAME_MAX,sizeof(char));
    int col_start, col_end, col_new_start, current_line, i=0; 
    struct matrix A,B,C;
    flock(msg_d,LOCK_EX);
    fscanf(msg,"%d %d",&current_line,&col_start);
    fseek(msg,0,0);
    do{
        fscanf(list_file,"%s %s %s",file_a,file_b,file_c);
        i++;
    } while (i<current_line && getc(list_file) != EOF);
    if(getc(list_file) != EOF){
        A = read_matrix(file_a);
        B = read_matrix(file_b);
        if (B.cols < workers_num) col_end = col_start + 1;
        else col_end = col_start + B.cols/workers_num;
        col_new_start = col_end == B.cols ? 0 : col_end;
        current_line = col_new_start == 0 ? i+1 : i;
        fprintf(msg,"%d %d",current_line,col_new_start);
        fseek(msg,0,0);
        flock(msg_d,LOCK_UN);
        C = multiply(A,B,col_start,col_end);
    }
    free((void*)A.matrix);
    free((void*)B.matrix);
    free((void*)C.matrix);
    free((void*)file_c);
    free((void*)file_b);
    free((void*)file_a);
    fclose(msg);
}

int run_processes(char* list,int workers_max,int time_max,int write_type){
    pid_t child_pid;
    for (int i =0; i< workers_max; i++){
        child_pid = fork();

    }
}

int main(int argc, char** argv){
    FILE* list_file = fopen("list","r");
char *file_a = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
         *file_b = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *file_c = 
        (char*)calloc(FILENAME_MAX,sizeof(char));
    fscanf(list_file,"%s %s %s",file_a,file_b,file_c);
    struct matrix A = read_matrix(file_a);
    struct matrix B = read_matrix(file_b);
    struct matrix C = multiply(A,B,0,B.cols);
    write_matrix(file_c,C);
    free((void*)A.matrix);
    free((void*)B.matrix);
    free((void*)C.matrix);
    free((void*)file_c);
    free((void*)file_b);
    free((void*)file_a);
    fclose(list_file);
    return 0; 
    /*
    if (argc != 5) return 22;
    char* list = argv[1];
    int workers_max = atoi(argv[2]);
    int time_max = atoi(argv[3]);
    int write_type;
    if(strcmp(argv[4],"common") == 0) write_type = 0;
    else if (strcmp(argv[4],"separate") == 0) write_type = 1;
    else return 22;
    return run_processes(list,workers_max,time_max,write_type);
    */
}