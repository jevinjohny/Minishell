#include <stdio.h>
#include "minishell.h"
int promps1 = 0;

int main()
{
    init_shell();

    main_loop();

    return 0;
}

void remove_job(pid_t pid)
{
    // Remove a job from the job list
}



void bring_job_to_foreground(int job_id)
{
    // Bring a job to the foreground
}

void send_job_to_background(int job_id)
{
    // Send a job to the background
}