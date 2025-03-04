#include "shell.h"

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

void my_history(char* command , int fd) {
    write(fd, command, strlen(command));
    write(fd, "\n", 1);
    close(fd);
}

int history_call(){
    int fd = open("history.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd < 0) {
        perror("Error opening history file");
        return -1;
    }
    return fd;
}