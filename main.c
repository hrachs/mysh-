#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


char * foo(char * buff){
	char* token = strtok(buff, " ");
    return token;
}

int main(){
    char exit_word[] = {"exit_word"};
    int j = 0;
    char arg[50] = {};
    char* buff = (char *)malloc(100 * sizeof(char));
    do{
        printf("mysh>");
        scanf("%s", buff);
        char * command = foo(buff);
        int i = strcmp(exit_word, command);
        if (i == 0){
            return 0;
        }else {
            pid_t pid = fork();
            if(pid > 0){
                wait(NULL);
            }
            else if (pid == 0){
                for(int i = 0; command[i] != '\0'; i++){
                    if(command[i] == '-'){
                       for(int i = 0; command[i] != ' '; i++){ 
                        arg[j] = command[i];
                        j++;
                        
                       }
                    }
                    
                    
                }
                execlp(command, command, arg, NULL);
            }
        }


    
    
    }while(1);
}

