#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/file.h>

int main(int argc, char** argv){
    if (argc != 2) return 22;
    srand(time(NULL));
    int number_of_files = atoi(argv[1]);
    int no_lines, line_size;
    char* file_name = 
        (char*)calloc(20,sizeof(char)),
        current;
    for(int i = 0;i<number_of_files;i++){
        no_lines = rand()%5 + 5;
        line_size = rand()%10 + 10;
        current = rand()%93 + 33;
        sprintf(file_name,"%s_%02d","in",i);
        FILE* file = fopen(file_name,"w");
        for(int k = 0; k< no_lines;k++){
            for (int j = 1; j < line_size-1; j++){
                fputc(current,file);
            }
            fputc('\n',file);
        }
        
        fclose(file);
    }
    free((void*)file_name);
    return 0;
}