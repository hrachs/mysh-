#ifndef SHELL
#define SHELL

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int my_init();
int build_in();
int my_exit();
int history(char *command);


#endif