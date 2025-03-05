#include "shell.h"

void cd(char* path) {
    if (chdir(path) == 0) {
        set_variable("PWD", getcwd(NULL, 0));
    } else {
        perror("cd failed");
    }
}

void pwd() {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("%s\n", buffer);
    } else {
        perror("pwd failed");
    }
}

void exit_shell() {
    for (int i = 0; i < var_count; i++) {
        free(variables[i].key);
        free(variables[i].value);
    }
    remove(get_history_file_path());
    exit(0);
}

void echo(char *args[]){
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
}