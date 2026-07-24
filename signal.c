#include "minishell.h"

void init_signal_handlers()
{
    signal(SIGINT, signal_handler);

    signal(SIGTSTP, signal_handler);

    signal(SIGCHLD, signal_handler);
}

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        if (pidex == 0)
        {
            printf(ANSI_COLOR_CYAN ANSI_BOLD "\n%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);

            if (getcwd(cwd, MAX_INPUT_SIZE) != NULL)
            {
                printf(ANSI_COLOR_RED ANSI_BOLD "%s own handler$ " ANSI_BOLD_RESET ANSI_COLOR_RESET, cwd);
            }
        }
        else
        {
            printf("\n");
        }
    }
    else if (sig == SIGTSTP)
    {
        if (pidex == 0)
        {
            // printf("\n");
            printf(ANSI_COLOR_CYAN ANSI_BOLD "\n%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);

            if (getcwd(cwd, MAX_INPUT_SIZE) != NULL)
            {
                printf(ANSI_COLOR_RED ANSI_BOLD "%s stop handler$ " ANSI_BOLD_RESET ANSI_COLOR_RESET, cwd);
            }
        }
        else
        {
            printf("\n");
        }
    }
    else if (sig == SIGCHLD)
    {
        int status;

        int pid = waitpid(-1, &status, WNOHANG);

        pidex = pid;

        if (pid>0 && !WIFSTOPPED(status))
        {

            remove_job(pid);

            pidex = 0;

            return;
        }
        else
        {
            pidex = 0;

            return;
        }
    }
    fflush(stdout);
}