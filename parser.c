#include "shell.h"

void parse_line(char *line, char **args){
    int i = 0;
    char *token = strtok(line, " \t\r\n");
    
    while (token != NULL) {
        args[i] = token;
        i++;
        if (i >= MAX_ARGS - 1) break;
        token = strtok(NULL, " \t\r\n");
    }
    
    args[i] = NULL;
}