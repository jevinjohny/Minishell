#include "minishell.h"

void init_shell()
{
    system("clear");
}

void main_loop()
{
    // input
    char *args[MAX_ARGS];

    init_signal_handlers();

    while (1)
    {
        if (!ps1_enabled)
        {
            printf(ANSI_COLOR_CYAN ANSI_BOLD "%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);
        }
        else
            printf("%s", prompt);

        if (!ps1_enabled)
        {
            if (getcwd(cwd, MAX_INPUT_SIZE) != NULL)
            {
                printf(ANSI_COLOR_RED ANSI_BOLD "%s main prompt" ANSI_BOLD_RESET ANSI_COLOR_RESET, cwd);
            }
            printf("$ ");
        }

        fflush(stdout);

        if (fgets(input_string, 1024, stdin) == NULL)
        {

            printf("\n");

            exit(0);
        }

        input_string[strcspn(input_string, "\n")] = '\0';

        strcpy(input_cpy, input_string);

        if (input_string[0] == '\0')
        {
            continue;
        }

        int ps1_flag = 1;

        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            char *str = input_string + 4;

            strcpy(prompt, str);

            ps1_flag = 0;

            ps1_enabled = 1;
        }
        else if (strchr(input_string, '|') != NULL)
        {
            char *ptr[MAX_INPUT_SIZE];

            int i = 0;

            char *temp = strtok(input_string, "|");

            while (temp != NULL)
            {
                ptr[i] = temp;

                i++;

                temp = strtok(NULL, "|");
            }
            ptr[i] = NULL;

            handle_redirection_and_piping(ptr);
        }
        else
        {
            parse_command(input_string, args);

            execute_command(args);
        }
    }
}