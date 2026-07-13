#include <stdio.h>
#include "minishell.h"
int promps1 = 0;

int main()
{
    init_shell();
    main_loop();
    return 0;
}






void add_job(pid_t pid, char *command)
{
    // Add a job to the job list
}

void remove_job(pid_t pid)
{
    // Remove a job from the job list
}

void list_jobs()
{
    // List all jobs
}

void bring_job_to_foreground(int job_id)
{
    // Bring a job to the foreground
}

void send_job_to_background(int job_id)
{
    // Send a job to the background
}