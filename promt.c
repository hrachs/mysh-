#include "shell.h"

void promt(char *command, char *args[]) {
    printf("MY SHELL>> ");
    if (!fgets(command, MAX_COMMAND_LENGTH, stdin)) {
        printf("\n");
        exit_shell();
    }
    
    command[strcspn(command, "\n")] = '\0';

    for (int i = 0; i < MAX_ARGS; i++) {
        args[i] = NULL;
    }

    if (strcmp(command, "exit") == 0) {
        exit_shell();
    }
}