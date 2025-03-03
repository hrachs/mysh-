#include "shell.h"

void execute_builtin(char* args[]) {
    if (strcmp(args[0], "set") == 0) {
        if (args[1] == NULL) {
            printf("Usage: set NAME=VALUE\n");
            return;
        }
        char *key = strtok(args[1], "=");
        char *value = strtok(NULL, "");
        if (key && value) {
            set_variable(key, value);
        } else {
            printf("Invalid format. Use NAME=VALUE\n");
        }
    } else if (strcmp(args[0], "unset") == 0) {
        if (args[1] == NULL) {
            printf("Usage: unset NAME\n");
            return;
        }
        unset_variable(args[1]);
    } else if (strcmp(args[0], "echo") == 0) {
        if (args[1] == NULL) {
            printf("\n");
            return;
        }
        for (int i = 1; args[i] != NULL; i++) {
            if (args[i][0] == '$') {
                char *var_name = args[i] + 1;
                for (int j = 0; j < var_count; j++) {
                    if (strcmp(variables[j].key, var_name) == 0) {
                        printf("%s ", variables[j].value);
                        break;
                    }
                }
            } else {
                printf("%s ", args[i]);
            }
        }
        printf("\n");
    } else if (strcmp(args[0], "history") == 0) {
        history();
    } else if (strcmp(args[0], "pwd") == 0) {
        pwd();
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            printf("Usage: cd <directory>\n");
            return;
        }
        cd(args[1]);
    }
}