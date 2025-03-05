#include "shell.h"

char* get_history_file_path() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char *history_path = malloc(strlen(homedir) + strlen("/history.txt") + 1);
    strcpy(history_path, homedir);
    strcat(history_path, "/history.txt");
    return history_path;
}

void history() {
    char *history_path = get_history_file_path();
    int fd = open(history_path, O_RDONLY);
    free(history_path);
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

void my_history(char* command, int fd) {
    char *history_path = get_history_file_path();
    FILE *file = fopen(history_path, "a");
    free(history_path);
    if (file == NULL) {
        perror("Failed to open history file");
        return;
    }
    fprintf(file, "%s\n", command);
    fclose(file);
}

int history_call(){
    char *history_path = get_history_file_path();
    int fd = open(history_path, O_WRONLY | O_APPEND | O_CREAT, 0666);
    free(history_path);
    if (fd < 0) {
        perror("Error opening history file");
        return -1;
    }
    return fd;
}