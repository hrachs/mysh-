#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_VARIABLES 100
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

struct Variable {
    char* key;
    char* value;
};

int var_count = 0;
struct Variable variables[MAX_VARIABLES];

void set_variable(char* key, char* value);
void unset_variable(char* key);
void execute_builtin(char* args[]);
void my_history(char* command);
void history();
void pwd();
void cd(char* path);
void exit_shell();

void loop() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        printf("mysh> ");
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin)) {
            printf("\n");
            break;
        }
        
        command[strcspn(command, "\n")] = '\0';  // Remove newline
        my_history(command); // Save command to history

        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue; // Ignore empty input

        if (strcmp(args[0], "exit") == 0) {
            exit_shell();
        } else if (strcmp(args[0], "set") == 0 || strcmp(args[0], "unset") == 0 || 
                   strcmp(args[0], "echo") == 0 || strcmp(args[0], "history") == 0 || 
                   strcmp(args[0], "pwd") == 0 || strcmp(args[0], "cd") == 0) {
            execute_builtin(args);
        } else {
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(1);
            }
            if (pid == 0) {
                if (execvp(args[0], args) == -1) {
                    perror("Command execution failed");
                }
                exit(1);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}

void my_history(char* command) {
    int fd = open("history.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd < 0) {
        perror("Error opening history file");
        return;
    }
    write(fd, command, strlen(command));
    write(fd, "\n", 1);
    close(fd);
}

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
                char *var_name = args[i] + 1; // Remove '$'
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

void set_variable(char* key, char* value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].key, key) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);
            return;
        }
    }
    if (var_count < MAX_VARIABLES) {
        variables[var_count].key = strdup(key);
        variables[var_count].value = strdup(value);
        var_count++;
    } else {
        printf("Max variable limit reached!\n");
    }
}

void unset_variable(char* key) {
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

void history() {
    int fd = open("history.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening history file");
        return;
    }
    char buffer[101];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, 100)) > 0) {
        buffer[bytes_read] = '\0'; // Ensure null termination
        printf("%s", buffer);
    }
    close(fd);
}

void pwd() {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("%s\n", buffer);
    } else {
        perror("pwd failed");
    }
}

void cd(char* path) {
    if (chdir(path) == 0) {
        set_variable("PWD", getcwd(NULL, 0));
    } else {
        perror("cd failed");
    }
}

void exit_shell() {
    for (int i = 0; i < var_count; i++) {
        free(variables[i].key);
        free(variables[i].value);
    }
    exit(0);
}

int main() {
    loop();
    return 0;
}

