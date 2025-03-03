#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_VARIABLES 1000
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

typedef struct {
    char *key;
    char *value;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;
char command[MAX_COMMAND_LENGTH];
char *arg[MAX_ARGS];

void set_variable(char *key, char *value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].key, key) == 0) {
            variables[i].value = strdup(value);
            return;
        }
    }
    variables[var_count].key = strdup(key);
    variables[var_count].value = strdup(value);
    var_count++;
}

void unset_variable(char *key) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].key, key) == 0) {
            free(variables[i].key);
            free(variables[i].value);
            for (int j = i; j < var_count - 1; j++) {
                variables[j] = variables[j + 1];
            }
            var_count--;
            return;
        }
    }
}

void print_variables() {
    for (int i = 0; i < var_count; i++) {
        printf("%s=%s\n", variables[i].key, variables[i].value);
    }
}

void my_history(char *command) {
    int fd = open("history.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
    write(fd, command, strlen(command));
    write(fd, "\n", 1);
    close(fd);
}

int execute_builtin(char *command) {
    if (strcmp(command, "exit") == 0) {
        return 1;
    }
    if (strncmp(command, "set ", 4) == 0) {
        char *key = strtok(command + 4, "=");
        char *value = strtok(NULL, "");
        if (key && value) {
            set_variable(key, value);
        }
        return 0;
    }
    if (strncmp(command, "unset ", 6) == 0) {
        unset_variable(command + 6);
        return 0;
    }
    if (strcmp(command, "echo") == 0) {
        print_variables();
        return 0;
    }
    return 0;
}

void my_shell() {
    while (1) {
        printf("MY Shell>> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';
        my_history(command);
        
        if (execute_builtin(command)) {
            break;
        }

        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL) {
            arg[i++] = token;
            token = strtok(NULL, " ");
        }
        arg[i] = NULL;

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        if (pid == 0) {
            if (execvp(arg[0], arg) == -1) {
                perror("Command execution failed");
            }
            exit(1);
        } else {
            wait(NULL);
        }
    }
}

int main() {
    my_shell();
    return 0;
}
