#include "shell.h"
#include <stdio.h>
#define MAX_VARIABLES 1000

typedef struct{
    char *key;
    char *value;
}Variable;

Variable variables[MAX_VARIABLES];

char command[100];
char *arg[10];
    int status;
    
int my_init() {
    char  *arr[] = {"pwd", "cd", "exit", "set", "unset", "echo", "help", "history"};
    do {
        printf("MY Shell>> ");
        fgets(command, sizeof(command), stdin);
        int fd = ("text.txt", O_RDWR|O_CREAT, 0666);
        
        write(fd, command, strlen(command));
        
        

        command[strcspn(command, "\n")] = '\0';

        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            arg[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        arg[i] = NULL;  

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);

        } 
        int t = build_in(command);
        if (t == 1){
        return -1;
        break;
    }    
        if (pid == 0) {
            
            if (execvp(arg[0], arg) == -1) {

                perror("Command execution failed");
            }

            exit(1);
        } else {
            wait(&status);
        }
        char command[] = {0};

    } while (1);
}
int build_in(char * command){
    int a = strcmp(command, "exit");if(a == 0){int r = my_exit();return r;}
    // int b = strcmp(command, "history");if(b == 0){int r = myhistory(command);return r;}
    // int c = strcmp(command, "cd");if(c == 0){int r = mycd(command);return r;}
    // int d = strcmp(command, "pwd");if(d == 0){int r = mypwd(command);return r;}
    // int e = strcmp(command, "set");if(e == 0){int r = myset(command);return r;}
    // int f = strcmp(command, "unset");if(f == 0){int r = myunset(command);return r;}
    // int g  = strcmp(command, "history");if(g == 0){int r = myhistory(command);return r;}
    // int h = strcmp(command, "echo");if(h == 0){int r = myecho(command);return r;}
    // int i = strcmp(command, "help");if(i == 0){int r = myhelp(command);return r;}
    return 0; 
}




int myhistory(char *command, int fd) {
    
 
    
    
    size_t len = strlen(command);
    command[len] = '\n'; 
    command[len + 1] = '\0'; 
    write(fd, command, len + 1); 
}

void set(char *arg){
    
    char *key = strtok(arg, "=");
    char *value = strtok(NULL, "\n");
    
}

int my_exit(){
    
    
    return 1;
}