#include "shell.h"

int main() {

     char line[MAX_LINE];
    char *args[MAX_ARGS];

    while(1){
        printf("zck> ");

        // Read input string
        if(!fgets(line, MAX_LINE, stdin))break;

        // Tokenize (split by space)
        parse_line(line, args);

        // Empty input
        if (args[0] == NULL) continue;
        if (strcmp(args[0], "exit") == 0) break;
        if(handle_builtin(args)) continue;

        execute_command(args);
    }

    return 0;
}