#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

// ฟังก์ชันสำหรับจัดการคำสั่ง
void parse_line(char *line, char **args);
int handle_builtin(char **args);
void execute_command(char **args);

// ฟังก์ชันย่อยใน executor (แยกออกมาให้ไม่งง)
void setup_redirection(char **args);
void handle_pipe(char **args, int pipe_index);

#endif
