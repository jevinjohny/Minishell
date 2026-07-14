#ifndef MINSHELL_H
#define MINSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

// colours
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// bold text
#define ANSI_BOLD "\x1b[1m"
#define ANSI_BOLD_RESET "\x1b[0m"

// Define constants
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_JOBS 64
#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

// Job control structures
typedef struct Job
{
    int job_id;
    pid_t pid;
    char command[MAX_INPUT_SIZE];
    struct Job *next;
} Job;

extern int promps1;

// Function declarations
void init_shell();
void main_loop();
void parse_command(char *input, char **args);
void execute_command(char **args);
int is_builtin_command(char **args);
void handle_builtin_command(char **args);
void handle_external_command(char **args);
void handle_redirection_and_piping(char **args);
void signal_handler(int sig);
void init_signal_handlers();
void add_job(pid_t pid, char *command);
void remove_job(pid_t pid);
void list_jobs();
void bring_job_to_foreground(int job_id);
void send_job_to_background(int job_id);
#endif