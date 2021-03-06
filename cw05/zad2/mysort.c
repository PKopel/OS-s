#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char** argv){
    if(argc != 2 ) return 22;
    char *command = 
        (char*)calloc(5 + strlen(argv[1]),sizeof(char));
    strcpy(command, "sort ");
    strcat(command,argv[1]);
    FILE* sorted = popen(command, "r");
    if(sorted == NULL){
        perror("open sorted");
        return errno;
    } 
    while(!feof(sorted)){
        putchar(fgetc(sorted));
    }
    pclose(sorted);
}