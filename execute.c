#include "minishell.h"

void execute_command(char **args)
{
    int ret = command_type(args);

    if (ret == BUILTIN)
    {
        handle_builtin_command(args);
    }
    else if (ret == EXTERNAL)
    {
        handle_external_command(args);
        // pidex=0;
    }
    else
    {
        printf("command not found\n");
    }
}

void handle_external_command(char **args)
{
    pidex = fork();

    if (pidex == 0)
    {
        signal(SIGINT, SIG_DFL);

        signal(SIGTSTP, SIG_DFL);

        signal(SIGCHLD, SIG_DFL);

        execvp(args[0], args);

        perror("execvp");

        exit(1);
    }
    else
    {
        job_pid = pidex;

        waitpid(pidex, &stat, WUNTRACED);

        if (WIFSTOPPED(stat))
        {
            add_job(job_pid, input_cpy);
        }

        pidex = 0; // after executing one command  and cntrl+z is pressed main prompt will print and pidex will be having the child pid
        // which is >0 so if u press again the cntrl+z it will call ownhandler and pidex==0 will be false and ownhandler prompt will
        // not print so u need to reset the pidex=0;
    }
}