#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

typedef struct node_c{
    struct node_c* next;
    char* val;
} node_c;

typedef struct node_i{
    struct node_i* next;
    int val;
} node_i;

int error(char* msg){
    perror(msg);
    exit(errno);
}

node_c* init_node_c(){
    node_c* new = (node_c*)malloc(sizeof(node_c));
    new->next = NULL;
    new->val = (char*)calloc(100,sizeof(char));
    return new;
}

node_i* init_node_i(){
    node_i* new = (node_i*)malloc(sizeof(node_i));
    new->next = NULL;
    new->val = 0;
    return new;
}

node_c* read_line(FILE* file){
    node_c *head = init_node_c(),
         *current = head;
    for(char c = fgetc(file); c != '\n' && c != EOF; c = fgetc(file)){
        if(c !=' '){
            fseek(file,-1,1);
            fscanf(file, " %s",current->val);
            current->next = init_node_c();
            current = current->next;
        }
    }
    fgetc(file);
    return head;
}

int execute_one(node_c** head,node_i** children,int in){
    int i = 0, not_last = 1;
    for (node_c* c = (*head); c != NULL && c->val[0] != '|'; c = c->next){
        i++;
        if (c->next == NULL) not_last = 0;
    }
    char** args = 
        (char**)calloc(i+1,sizeof(char*));
    i = 0;
    for (node_c* c = (*head); c != NULL && c->val[0] != '|'; c = (*head)){
        (*head) = (*head)->next;
        args[i++] = c->val[0] ? c->val : NULL;
        free((void*)c);
    }
    args[i] = NULL;
    if((*head) != NULL){
        (*head) = (*head)->next;
    }
    int fd[2];
    if(pipe(fd) < 0) error("pipe");
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]); 
        if (not_last) if(dup2(fd[1],STDOUT_FILENO) < 0) error("dup2 stdout");
        if(dup2(in,STDIN_FILENO) < 0) error("dup2 stdin");
        if(execvp(args[0], args) < 0) error("exec");
        exit(0);
    } else if(pid > 0){
        close(fd[1]);
        (*children)->val = pid;
        (*children)->next = init_node_i();
        (*children) = (*children)->next;
        for(int j = 0;j<i;j++)
            free((void*)args[j]);
        free((void*)args);
        return fd[0];
    } else return -1;
}

int execute_line(node_c* head){
    node_i* children = init_node_i(), *current = children;
    for(int i = STDIN_FILENO; head != NULL; i = execute_one(&head,&current,i));
    for(current = children; current != NULL; current = children){
        children = children->next->val > 0 ? children->next : NULL;
        if( waitpid(current->val, NULL, 0) < 0) error("wait");
        free((void*)current);
    }
    return 0;
}

int execute_file(FILE* file){
    while(!feof(file)){
        fseek(file,-1,1);
        node_c* head = read_line(file);
        execute_line(head);
    }
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2) return 22;
    FILE* file = fopen(argv[1],"r");
    if(file == NULL) error("open file");
    execute_file(file);
    fclose(file);
    return 0;
}