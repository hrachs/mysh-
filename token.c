#include "shell.h"

int token(char *command, char *args[], int fd) {
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) return 0;

    if (strcmp(args[0], "set") == 0 || strcmp(args[0], "unset") == 0 || 
        strcmp(args[0], "echo") == 0 || strcmp(args[0], "history") == 0 || 
        strcmp(args[0], "pwd") == 0 || strcmp(args[0], "cd") == 0) {
        execute_builtin(args);
        my_history(command, fd);
        return 0;
    }
    return 1;
}
