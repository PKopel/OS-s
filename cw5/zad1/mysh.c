#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct node{
    struct node* next;
    char* val;
} node_c;

typedef struct node{
    struct node* next;
    int val;
} node_i;

node_c* init_node_c(){
    node_c* new = (node_c*)malloc(sizeof(node_c));
    new->val = (char*)calloc(100,sizeof(char));
    return new;
}

node_i* init_node_i(){
    node_i* new = (node_i*)malloc(sizeof(node_i));
    return new;
}

node_c* read_line(FILE* file){
    node_c *head = init_node_c(),
         *current = head;
    for(char c = fgetc(file); c != '\n' && c != EOF; c = fgetc(file)){
        fseek(file,-1,1);
        fscanf(file, " %s ",&(current->val));
        current->next = init_node_c();
        current = current->next;
    }
    return head;
}

int execute_one(node_c** head,node_i** children,int in){
    int i = 0;
    for (node_c* c = (*head); c != NULL && c->val[0] != '|'; c = c->next) i++;
    char** args = 
        (char**)calloc(i,sizeof(char*));
    i = 0;
    for (node_c* c = (*head); c != NULL; c = (*head)){
        (*head) = (*head)->next;
        args[i++] = c->val;
        free((void*)c);
    }
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]); 
        dup2(fd[1],STDOUT_FILENO);
        dup2(in,STDIN_FILENO);
        execvp(args[0], args);
    } else {
        (*children) = init_node_i();
        (*children)->val = pid;
        (*children) = (*children)->next;
        for(int j = 0;j<i;j++)
            free((void*)args[j]);
        free((void*)args);
        return fd[0];
    }
}

int execute_line(node_c* head){
    node_i* children = NULL, *current = children;
    for(int i = STDIN_FILENO; head != NULL; i = execute_one(&head,&current,i));
    for(current = children; current != NULL; current = children){
        children = children->next;
        waitpid(current->val, NULL, 0);
        free((void*)current);
    }
    return 0;
}

int execute_file(FILE* file){
    for(char c = fgetc(file); c != EOF; c = fgetc(file)){
        fseek(file,-1,1);
        execute_line(read_line(file));
    }
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2) return 22;
    return 0;
}