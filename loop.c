#include "shell.h"

int var_count = 0;
variable_t variables[MAX_VARIABLES];

void loop() {
    signal(SIGINT, sig_handle);
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        int fd = history_call();

        promt(command, args);

        if ((token(command, args, fd)) != 0){
            exec(args, command, fd);
        }
    }
}
