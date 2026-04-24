#include "shell.h"

int handle_builtin(char **args){
    // cd exection 
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "zck: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("zck: ");
            }
        }
        return 1;
    }

    return 0;
}