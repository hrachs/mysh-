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

void history() {
    int fd = open("history.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening history file");
        return;
    }
    char buffer[101];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, 100)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(fd);
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

void exit_shell() {
    for (int i = 0; i < var_count; i++) {
        free(variables[i].key);
        free(variables[i].value);
    }
    remove("history.txt");
    exit(0);
}