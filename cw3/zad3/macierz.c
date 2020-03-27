#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/resource.h>

int COMMON = 0;
int SEPARATE = 1;

struct matrix{
    int cols;
    int rows;
    int **matrix;
};

void set_limits(int max_cpu_time, int max_mem){
    struct rlimit cpu_limit;
    struct rlimit mem_limit;
    cpu_limit.rlim_max = (rlim_t) max_cpu_time;
    cpu_limit.rlim_cur = (rlim_t) max_cpu_time;
    setrlimit(RLIMIT_CPU, &cpu_limit);
    mem_limit.rlim_max = (rlim_t) max_mem*1000000;
    mem_limit.rlim_cur = (rlim_t) max_mem*1000000;
    setrlimit(RLIMIT_AS, &mem_limit);
}

long int get_time(struct timeval time){
    return (long int)time.tv_sec * 1000000 + (long int)time.tv_usec;
}

void write_usage(struct rusage not_included, struct rusage included){
    long int user = get_time(included.ru_utime) - get_time(not_included.ru_utime);
    long int system = get_time(included.ru_stime) - get_time(not_included.ru_stime);
    long int mem = included.ru_maxrss - not_included.ru_maxrss;
    printf("CPU czas użytkownika (s): %lf\n", (double)user / 1000000);
    printf("CPU czas systemu (s): %lf\n", (double)system / 1000000);
    printf("RSS (MB): %f\n",(double)mem/1000);
}

struct matrix multiply(struct matrix A, struct matrix B, int start, int end){
    int sum = 0;
    struct matrix result;
    result.rows = A.rows;
    result.cols = end - start;
    result.matrix = 
        (int**)calloc(A.rows,sizeof(int));
    for(int j = 0; j < A.rows; j++){
        result.matrix[j] = 
                (int*)calloc(end-start,sizeof(int));
        for(int i = start; i < end; i++){
            for(int k = 0; k < A.cols; k++)
                sum+= A.matrix[j][k]*B.matrix[k][i];
            result.matrix[j][i] = sum;
            sum = 0;
        }
    }
    return result;
}

void free_matrix(struct matrix M){
    for(int i = 0; i < M.rows; i++){
        free((void*)M.matrix[i]);
    }
    free((void*)M.matrix);    
}

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

void write_matrix(FILE* file, struct matrix M){
    for(int i = 0; i< M.rows;i++){
        for(int j = 0; j< M.cols;j++){
            fprintf(file,"\t%d",M.matrix[i][j]);
        }
        fprintf(file,"\n");
    }
}

void rewrite_matrix(FILE* file, struct matrix M, int col_start, int col_end){
    struct matrix current;
    if(fgetc(file) != EOF){
        current = read_matrix(file);
    } else {
        current.matrix = 
        (int**)calloc(M.rows,sizeof(int*));
        for(int i = 0; i < M.rows; i++){
            current.matrix[i] = 
                (int*)calloc(col_end,sizeof(int)); 
            for(int j = 0; j< col_end; j++){
                current.matrix[i][j] = 0;
            }
        }
    }
    for(int i = 0; i < M.rows; i++){
        for(int j = col_start; j < col_start + M.cols; j++){
            current.matrix[i][j] = M.matrix[i][j-col_start];
        }
    }
    fseek(file,0,0);
    write_matrix(file,current);
    free_matrix(current);
}

int proces(char* list, int workers_num, int max_time, int write_type){
    time_t start_time = time(NULL);
    int counter = 0;
    FILE* list_file = fopen(list,"r");
    FILE* msg = fopen("msg","r+");
    //FILE* log = fopen("log","a+");
    int msg_d = fileno(msg);
    char *file_a = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
         *file_b = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *file_c = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *file_tmp = 
        (char*)calloc(FILENAME_MAX,sizeof(char)),
        *msg_line = 
        (char*)calloc(10,sizeof(char));
    int col_start, col_end, col_new_start, current_line, i=0; 
    struct matrix A,B,C;
    while((time(NULL) - start_time) < max_time){
        int result = flock(msg_d,LOCK_EX | LOCK_NB);
        if(result == 0){
            fgets(msg_line,10,msg);
            sscanf(msg_line,"%d\t%d",&current_line,&col_start);
            fseek(msg,0,0);
            if(current_line >= 0){
                //fprintf(log,"before %d %d %d\n",getpid(), current_line,col_new_start);
                //fflush(log);
                do{
                    fscanf(list_file,"%s %s %s",file_a,file_b,file_c);
                    i++;
                } while (i<current_line && getc(list_file) != EOF);
                i = current_line;
                FILE* a = fopen(file_a,"r");
                FILE* b = fopen(file_b,"r");
                A = read_matrix(a);
                B = read_matrix(b);
                if (B.cols < workers_num) col_end = col_start + 1;
                else col_end = col_start + B.cols/workers_num;
                col_new_start = col_end < B.cols ? col_end : 0;
                current_line = col_new_start == 0 ? i+1 : i;
                if(fgetc(list_file) == EOF 
                && col_new_start == 0) current_line = -1;
                fprintf(msg,"%d\t%d",current_line,col_new_start);
                fflush(msg);
                //fprintf(log,"after %d %d %d\n",getpid(), current_line,col_new_start);
                //fflush(log);
                fseek(msg,0,0);
                flock(msg_d,LOCK_UN);
                C = multiply(A,B,col_start,col_end);
                if(write_type == SEPARATE){
                    sprintf(file_tmp,"%s_%03d",file_c,col_start);
                    FILE* tmp = fopen(file_tmp,"a+");
                    write_matrix(tmp,C);
                    fclose(tmp);
                } else {
                    FILE* c = fopen(file_c,"a+");
                    int c_d = fileno(c);
                    flock(c_d,LOCK_EX);
                    rewrite_matrix(c,C,col_start,B.cols);
                    flock(c_d,LOCK_UN);
                    fclose(c);
                }
                counter++;
                free_matrix(A);
                free_matrix(B);
                free_matrix(C);
                fclose(a);
                fclose(b);
            } else flock(msg_d,LOCK_UN);
        }
    }
    free((void*)file_c);
    free((void*)file_b);
    free((void*)file_a);
    free((void*)msg_line);
    fclose(log);
    fclose(msg);
    fclose(list_file);
    return counter;
}

int run_processes(char* list,int max_workers,int max_time,int write_type, int max_cpu_time, int max_mem){
    pid_t *children = 
        (pid_t*)calloc(max_workers,sizeof(pid_t));
    pid_t child_pid;
    for (int i =0; i< max_workers; i++){
        child_pid = fork();
        if(child_pid == 0){
            set_limits(max_cpu_time,max_mem);
            int result = proces(list,max_workers,max_time,write_type);
            exit(result);
        } else {
            children[i] = child_pid;
        }
    }
    int result;
    for (int i =0; i< max_workers; i++){
        struct rusage not_included;
        struct rusage included;
        getrusage(RUSAGE_CHILDREN,&not_included);
        waitpid(children[i], &result, 0);
        getrusage(RUSAGE_CHILDREN,&included);
        printf("Proces %d wykonał %d mnożeń\n", children[i],WEXITSTATUS(result));
        write_usage(not_included,included);
    }
    remove("msg");
    //remove("log");
    if(write_type){
        char* cmd = 
            (char*)calloc(FILENAME_MAX,sizeof(char)),
            *file_c = 
            (char*)calloc(FILENAME_MAX,sizeof(char));
        FILE* list_file = fopen(list,"r");
        do{
            fscanf(list_file,"%*s %*s %s",file_c);
            sprintf(cmd,"paste %s_* > %s; rm -f %s_*",file_c, file_c,file_c);
            child_pid = fork();
            if(child_pid == 0){
                execlp("bash","bash","-c",cmd,NULL);
            }
        } while (getc(list_file) != EOF);
        free((void*)file_c);
        free((void*)cmd);
        fclose(list_file);
    }
    return 0;
}

int main(int argc, char** argv){
    if (argc != 7) return 22;
    char* list = argv[1];
    int max_workers = atoi(argv[2]);
    int max_time = atoi(argv[3]);
    int max_cpu_time = atoi(argv[5]);
    int max_mem = atoi(argv[6]);
    int write_type;
    if(strcmp(argv[4],"c") == 0) write_type = COMMON;
    else if (strcmp(argv[4],"s") == 0) write_type = SEPARATE;
    else return 22;
    FILE* msg = fopen("msg","w");
    fprintf(msg,"0\t0");
    fclose(msg);
    FILE* log = fopen("log","a+");
    fprintf(log,"%d %d\n",max_workers, max_time);
    fflush(log);
    return run_processes(list,max_workers,max_time,write_type,max_cpu_time, max_mem);
}