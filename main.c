#include <stdio.h>

#include "minishell.h"

pid_t pidex;

int count = 1;

Job *head = NULL;

pid_t job_pid;

int stat;

char prompt[] = "minishell";

char input_string[MAX_INPUT_SIZE];

char input_cpy[MAX_INPUT_SIZE];

char cwd[MAX_INPUT_SIZE];

int ps1_enabled = 0;

int main()
{
    init_shell();

    main_loop();

    return 0;
}






