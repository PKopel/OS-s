#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

int multiply(FILE* A, FILE* B,int col_a,int row_a,int col_b,int col_start,int col_end, FILE* C){
    int current, a, b, sum = 0;
    int **tmp = 
        (int**)calloc(col_end - col_start,sizeof(int*));
    char* format = 
		(char*)calloc(col_b*4-1,sizeof(char)),
		previous,swap;
	for(int i = 0; i < col_b; i++)
		if(i == col_start){
			strcat(format,"%d ");
			current = 1+i*4;
		} else strcat(format,"%*d ");
	format[col_b*4-2] = '\0';

    for(int i=col_start;i<col_end;i++){
        tmp[i] = 
            (int*)calloc(row_a,sizeof(int));
        for(int j = 0; j<row_a;j++){
            for(int k = 0; k< col_a;k++){
                fscanf(A,"%d ",&a);
                fscanf(B,format,&b);
                sum+= a*b;
            }
            tmp[i][j] = sum;
            format[current++]='*';
            previous = 'd';
            while(current < col_b*4-1 && format[current]!='d'){
                swap = format[current];
                format[current]=previous;
                previous = swap;
                current++;
            }	
            fseek(B,0,0);
        }
    }

    for(int )
    return 0;
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
    int col_a, row_a, col_b, col_start, col_end, col_new_start, current_line, i=0; 
    flock(msg_d,LOCK_EX);
    fscanf(msg,"%d %d",&current_line,&col_start);
    fseek(msg,0,0);
    do{
        fscanf(list_file,"%s %s %s",file_a,file_b,file_c);
        i++;
    } while (i<current_line && getc(list_file) != EOF);
    if(getc(list_file) != EOF){
        FILE *A = fopen(file_a,"r"),
            *B = fopen(file_b,"r"),
            *C;
        if(write_type){
            char* tmp_file = (char*)calloc(strlen(file_c)+7,sizeof(char));
            sprintf(tmp_file,"%s_%d_%d",file_c,col_start,col_end);
            fopen(tmp_file,"r+");
        } else  C = fopen(file_c,"r+");
        fscanf(A,"%d",&col_b);
        if (col_b < workers_num) col_end = col_start + 1;
        else col_end = col_start + col_b/workers_num;
        col_new_start = col_end == col_b ? 0 : col_end;
        current_line = col_new_start == 0 ? i+1 : i;
        fprintf(msg,"%d %d",current_line,col_new_start);
        fseek(msg,0,0);
        flock(msg_d,LOCK_UN);
        multiply(A,B,col_a,row_a,col_start,col_end,C);
        }
    }
    fclose(msg);
}

int run_processes(char* list,int workers_max,int time_max,int write_type){
    pid_t child_pid;
    for (int i =0; i< workers_max; i++){
        child_pid = fork();

    }
}

int main(int argc, char** argv){
    if (argc != 5) return 22;
    char* list = argv[1];
    int workers_max = atoi(argv[2]);
    int time_max = atoi(argv[3]);
    int write_type;
    if(strcmp(argv[4],"common") == 0) write_type = 0;
    else if (strcmp(argv[4],"separate") == 0) write_type = 1;
    else return 22;
    return run_processes(list,workers_max,time_max,write_type);
}