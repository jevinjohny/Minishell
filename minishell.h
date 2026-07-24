#ifndef MINISHELL_H
#define MINISHELL_H

/* ================= HEADERS ================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

/* ================= COLORS ================= */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_BOLD          "\x1b[1m"
#define ANSI_BOLD_RESET    "\x1b[0m"

/* ================= CONSTANTS ================= */

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_JOBS 64

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

/* ================= DATA STRUCTURES ================= */

typedef struct Job
{
    int job_id;
    pid_t pid;
    char command[MAX_INPUT_SIZE];
    struct Job *next;

} Job;

/* ================= GLOBAL VARIABLES ================= */

extern int ps1_enabled;

extern pid_t pidex;

extern pid_t job_pid;

extern int count;

extern int stat;

extern Job *head;

extern char prompt[];

extern char input_string[MAX_INPUT_SIZE];

extern char input_cpy[MAX_INPUT_SIZE];

extern char cwd[MAX_INPUT_SIZE];

/* ================= SHELL ================= */

void init_shell(void);
void main_loop(void);

/* ================= PARSER ================= */

void parse_command(char *input, char **args);

/* ================= EXECUTION ================= */

void execute_command(char **args);
void handle_external_command(char **args);
void handle_redirection_and_piping(char **ptr);

int command_type(char **args);

/* ================= BUILTIN ================= */

void handle_builtin_command(char **args);

/* ================= JOB CONTROL ================= */

void add_job(pid_t pid, char *command);
void remove_job(pid_t pid);
void list_jobs(void);

void bring_job_to_foreground(int job_id);
void send_job_to_background(int job_id);

/* ================= SIGNALS ================= */

void init_signal_handlers(void);
void signal_handler(int sig);

#endif