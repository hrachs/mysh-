#ifndef SHELL
#define SHELL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <pwd.h>

#define MAX_VARIABLES 100
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 100


typedef struct {
    char *key;
    char *value;
} variable_t;

extern int var_count;
extern variable_t variables[MAX_VARIABLES];


void set_variable(char* key, char* value);
void unset_variable(char* key);
void execute_builtin(char* args[]);
void my_history(char* command, int fd);
void history();
void pwd();
void cd(char* path);
void exit_shell();
void loop();
void sig_handle(int signum);
void echo(char * args[]);
int history_call();
void exec(char * args[], char* command , int fd);
void promt(char * command , char * args[]);
int token(char * command, char * args[], int fd);
char* get_history_file_path();

#endif
