#include "minishell.h"

void add_job(pid_t pid, char *command)
{
    Job *new = malloc(sizeof(Job));

    if (!new)
    {
        printf("Failed to allocate memory\n");

        return;
    }
    new->next = NULL;

    new->pid = pid;

    new->job_id = count;

    count++;

    strcpy(new->command, command);

    if (head != NULL)
    {
        Job *temp = head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new;

        new->next = NULL;

        return;
    }

    head = new;

    new->next = NULL;
}

void remove_job(pid_t pid)
{
    Job *temp = head;

    Job *prev = NULL;

    if (temp->pid == pid)
    {
        free(temp);

        head = NULL;

        count--;

        return;
    }

    while (temp)
    {

        if (temp->pid == pid)
        {
            prev->next = NULL;

            free(temp);

            count--;

            return;
        }
        prev = temp;

        temp = temp->next;
    }
}

void list_jobs()
{
    if (head == NULL)
    {
        return;
    }
    Job *temp = head;

    while (temp)
    {
        printf("[%d]  stopped\t\t%s\n", temp->job_id, temp->command);

        temp = temp->next;
    }
}

void bring_job_to_foreground(int job_id)
{
    Job *temp = head;

    if (temp == NULL)
    {
        return;
    }

    while (temp)
    {
        if (temp->job_id == job_id)
        {
            printf("%s\n", temp->command);

            pidex = temp->pid;

            signal(SIGTSTP, SIG_IGN);
            signal(SIGINT, SIG_IGN);

            int val = kill(temp->pid, SIGCONT);

            int status;

            waitpid(temp->pid, &status, WUNTRACED);

            signal(SIGTSTP, signal_handler);
            signal(SIGINT, signal_handler);

            if (!WIFSTOPPED(status))
            {
                remove_job(temp->pid);

                pidex = 0;

                return;
            }
            else
            {
                pidex = 0;

                return;
            }
        }
        temp = temp->next;
    }
}

void send_job_to_background(int job_id)
{
    Job *temp = head;

    if (temp == NULL)
    {
        return;
    }

    while (temp)
    {

        if (temp->job_id == job_id)
        {
            printf("%s &\n", temp->command);

            pidex = temp->pid;

            kill(temp->pid, SIGCONT);

            // if(kill(temp->pid, SIGCONT))
            // {
            //     perror("kill");
                
            //     exit(1);
            // }

            pidex = 0;
        }
        temp = temp->next;
    }
}
