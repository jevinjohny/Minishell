#include <stdio.h>
#include "minishell.h"
int promps1 = 0;

int main()
{
    init_shell();

    main_loop();

    return 0;
}






void send_job_to_background(int job_id)
{
    // Send a job to the background
}