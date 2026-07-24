#include "minishell.h"

void handle_redirection_and_piping(char **ptr)
{
    int count = 0;

    int status;

    for (int i = 0; ptr[i] != NULL; i++)
    {
        count++;
    }

    int fd[count - 1][2];

    for (int i = 0; i < count - 1; i++)
    {
        pipe(fd[i]);
    }

    pid_t pidno[count];

    for (int i = 0; i < count; i++)
    {
        pid_t pid = fork();

        pidex = pid;

        if (pid == 0)
        {
            char *args[MAX_ARGS];

            parse_command(ptr[i], args);

            if (i != 0)
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            if (i != count - 1)
            {
                dup2(fd[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < count - 1; j++)
            {
                close(fd[j][0]);

                close(fd[j][1]);
            }
            execvp(args[0], args);
        }
        else
            pidno[i] = pid;
        // piidex=0;
    }

    for (int i = 0; i < count - 1; i++)
    {
        close(fd[i][0]);

        close(fd[i][1]);
    }
    for (int i = 0; i < count; i++)
    {
        waitpid(pidno[i], &status, WUNTRACED);
    }
}