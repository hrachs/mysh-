#include "shell.h"

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