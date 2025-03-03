#include "shell.h"

int var_count = 0;
variable_t variables[MAX_VARIABLES];

void loop() {
    signal(SIGINT, sig_handle);
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        printf("MY SHELL> ");
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin)) {
            printf("\n");
            break;
        }
        
        command[strcspn(command, "\n")] = '\0';

        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;

        if (strcmp(args[0], "exit") == 0) {
            exit_shell();
        } else if (strcmp(args[0], "set") == 0 || strcmp(args[0], "unset") == 0 || 
                   strcmp(args[0], "echo") == 0 || strcmp(args[0], "history") == 0 || 
                   strcmp(args[0], "pwd") == 0 || strcmp(args[0], "cd") == 0) {
            execute_builtin(args);
            my_history(command);
        } else {
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(1);
            }
            if (pid == 0) {
                if (execvp(args[0], args) == 0) {
                    my_history(command);
                }else {
                    perror("Command execution failed");
                    exit(1);
                    }
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}
