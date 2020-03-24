#include <stdio.h>
#include <string.h>

int run_processes(char* list,int workers_no,int max_s,int result_type){
    return 0;
}

int main(int argc, char** argv){
    if (argc != 5) return 22;
    char* list = argv[1];
    int workers_no = atoi(argv[2]);
    int max_s = atoi(argv[3]);
    int result_type;
    if(strcmp(argv[4],"common") == 0) result_type = 0;
    else if (strcmp(argv[4],"separate") == 0) result_type = 1;
    else return 22;
    return run_processes(list,workers_no,max_s,result_type);
}