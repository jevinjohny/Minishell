#include "minishell.h"

void parse_command(char *input, char **args)
{
    // Tokenize the input string into arguments and  store in args

    char *temp_input = strtok(input, " ");

    int i = 0;

    while (temp_input != NULL)
    {
        args[i] = temp_input;

        i++;

        temp_input = strtok(NULL, " ");
    }
    args[i] = NULL;
}