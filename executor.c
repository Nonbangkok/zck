#include "shell.h"

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        for (int j = 0; args[j] != NULL; j++) {
            if (strcmp(args[j], ">") == 0) {
                int fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);

                args[j] = NULL;
                break;
            } else if (strcmp(args[j], "<") == 0) {
                int fd = open(args[j + 1], O_RDONLY);
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

                    execvp(args[j + 1], &args[j + 1]);
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