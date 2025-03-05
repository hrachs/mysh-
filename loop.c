#include "shell.h"

int var_count = 0;
variable_t variables[MAX_VARIABLES];

void loop() {
    int PAGESIZE = getpagesize();
    signal(SIGINT, sig_handle);
    char * command = (char*)mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    char **args = (char**)malloc(MAX_ARGS * sizeof(char**));

    while (1) {
        int fd = history_call();

        promt(command, args);

        if ((token(command, args, fd)) != 0){
            exec(args, command, fd);
        }
    }
}
