#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc != 2 ) return 22;
    char *command = 
        (char*)calloc(5 + strlen(argv[1]),sizeof(char));
    strcpy(command, "sort ");
    strcat(command,argv[1]);
    FILE* sorted = popen(command, "r");
    char *line = 
        (char*)calloc(256,sizeof(char)),
        c = fgetc(sorted);
    for(int i = 0; c != EOF; (i++)%256){
        line[i] = c;
        c = fgetc(sorted);
        if (i == 255) printf("%s", line);
    }
    pclose(sorted);
}