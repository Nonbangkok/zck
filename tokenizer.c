#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while(1){
        printf("zck> ");

        // Read input string
        if(!fgets(line, MAX_LINE, stdin))break;

        // Tokenize (split by space)
        int i = 0;
        char *token = strtok(line, " \t\r\n");
        
        while (token != NULL) {
            args[i] = token;
            i++;
            if (i >= MAX_ARGS - 1) break;
            token = strtok(NULL, " \t\r\n");
        }
        
        args[i] = NULL;

        // Empty input
        if (args[0] == NULL) continue;
        if (strcmp(args[0], "exit") == 0) break;

        // Built-in command: cd
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "zck: expected argument to \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("zck: ");
                }
            }
            continue;
        }

        // Execute command using fork/exec
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
        } else if (pid == 0) {
            for (int j = 0; args[j] != NULL; j++) {
                if (strcmp(args[j], ">") == 0) {
                    int fd = open(args[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        perror("open failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);

                    args[j] = NULL;
                    break;
                } else if (strcmp(args[j], "<") == 0) {
                    int fd = open(args[j+1], O_RDONLY);
                    if (fd < 0) {
                        perror("open failed");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);

                    args[j] = NULL;
                    break;
                } else if (strcmp(args[j], "|") == 0) {
                    int fd[2];
                    pipe(fd);

                    int pid2 = fork();

                    if (pid2 < 0) {
                        perror("fork failed");
                    } else if (pid2 == 0) {
                        close(fd[1]);
                        dup2(fd[0], STDIN_FILENO);
                        close(fd[0]);

                        execvp(args[j+1], &args[j+1]);
                        perror("execvp failed");
                        exit(1);
                    } else {
                        close(fd[0]);
                        dup2(fd[1], STDOUT_FILENO);
                        close(fd[1]);

                        args[j] = NULL;
                        execvp(args[0], args);
                        perror("execvp failed");
                        exit(1);
                    }
                }
            }

            if (execvp(args[0], args) == -1) {
                perror("zck: ");
            }
            exit(1);
        } else {
            wait(NULL);
        }
    }

    return 0;
}