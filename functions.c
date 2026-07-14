#include <stdio.h>
#include "minishell.h"

char prompt[] = "minishell";

char cwd[MAX_INPUT_SIZE];

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

        if (pid == 0)
        {

            // printf("%d\n",pidno[i]);

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
    }

    for (int i = 0; i < count - 1; i++)
    {
        close(fd[i][0]);

        close(fd[i][1]);
    }

    // for (int i = 0; i < count; i++)
    // {
    //     wait(NULL);
    // }

    for (int i = 0; i < count; i++)
    {
        waitpid(pidno[i], &status, WUNTRACED);
    }
}
pid_t pidex;

void handle_external_command(char **args)
{
    pidex = fork();

    if (pidex == 0)
    {
        signal(SIGINT, SIG_DFL);

        signal(SIGTSTP, SIG_DFL);

        execvp(args[0], args);

        perror("execvp");

        exit(1);
    }
    else
    {
        waitpid(pidex, NULL, WUNTRACED);
        pidex = 0; // after executing one command  and cntrl+z is pressed main prompt will print and pidex will be having the child pid
        // which is >0 so if u press again the cntrl+z it will call ownhandler and pidex==0 will be false and ownhandler prompt will
        // not print so u need to reset the pidex=0;
    }
}

void handle_builtin_command(char **args)
{
    if (strcmp(args[0], "cd") == 0)
    {
        chdir(args[1]);

        getcwd(cwd, 1024);

        printf("%s\n", cwd);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        getcwd(cwd, 1024);

        printf("%s\n", cwd);
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        if (args[1] == NULL)
        {
            printf("\n");
        }
        else if (strcmp(args[1], "$$") == 0)
        {
            pid_t pid;

            pid = getpid();

            printf("%d\n", pid);
        }
        else if (strcmp(args[1], "$SHELL") == 0)
        {
            char *shell_path = getenv("SHELL");

            if (shell_path != NULL)
            {
                strcpy(cwd, shell_path);

                printf("%s\n", cwd);
            }
        }
        else
            printf("%s\n", args[1]);
    }
}

void init_signal_handlers()
{
    signal(SIGINT, signal_handler);

    signal(SIGTSTP, signal_handler);
}

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        if (pidex == 0)
        {
            printf(ANSI_COLOR_CYAN ANSI_BOLD "%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);
            
            if (getcwd(cwd, MAX_INPUT_SIZE) != NULL)
            {
                printf(ANSI_COLOR_RED ANSI_BOLD "%s own handler$ " ANSI_BOLD_RESET ANSI_COLOR_RESET, cwd);
            }
        }
    }
    else if (sig == SIGTSTP)
    {
        printf("\n");
        if (pidex == 0)
        {
            printf(ANSI_COLOR_CYAN ANSI_BOLD "%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);

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
    fflush(stdout);

}

void main_loop()
{
    //input
    char input_string[MAX_INPUT_SIZE];

    char *args[MAX_ARGS];

    init_signal_handlers();

    while (1)
    {
        if (!promps1)
        {
            printf(ANSI_COLOR_CYAN ANSI_BOLD "%s:" ANSI_BOLD_RESET ANSI_COLOR_RESET, prompt);
        }
        else
            printf("%s", prompt);

        if (!promps1)
        {
            if (getcwd(cwd, MAX_INPUT_SIZE) != NULL)
            {
                printf(ANSI_COLOR_RED ANSI_BOLD "%s main prompt" ANSI_BOLD_RESET ANSI_COLOR_RESET, cwd);
            }
            printf("$ ");
        }

        fflush(stdout);

        if ( fgets(input_string,1024, stdin) == NULL)
        {

            printf("\n");

            exit(0);
        }

        input_string[strcspn(input_string,"\n")]='\0';

        if (input_string[0]=='\0')
        {
            continue;
        }


        int ps1_flag = 1;

        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            char *str = input_string + 4;

            strcpy(prompt, str);

            ps1_flag = 0;

            promps1 = 1;
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

void execute_command(char **args)
{
    int ret = is_builtin_command(args);

    if (ret == BUILTIN)
    {
        handle_builtin_command(args);
    }
    else if (ret == EXTERNAL)
    {
        handle_external_command(args);
    }
    else
    {
        printf("command not found\n");
    }
}

int is_builtin_command(char **args)
{
    // Check if the command is a built-in command (e.g., cd, exit, fg, bg)
    char *builtins[] = {"echo", "printf", "read", "cd", /*"pwd" */ "pushd", "popd", "dirs", "let", "eval",
                        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(builtins[i], args[0]) == 0)
        {
            return BUILTIN;
        }
    }

    char *external[] = {"clear","wc", "bash", "bunzip2", "busybox", "bzcat", "bzcmp", "bzdiff", "bzegrep", "bzexe",
                        "bzfgrep", "bzgrep", "bzip2", "bzip2recover", "bzless", "bzmore", "cat", "chacl",
                        "chgrp", "chmod", "chown", "chvt", "cp", "cpio", "dash", "date", "dbus-cleanup-sockets",
                        "dbus-daemon", "dbus-uuidgen", "dd", "df", "dir", "dmesg", "dnsdomainname", "domainname",
                        "dumpkeys", "echo", "ed", "egrep", "false", "fgconsole", "fgrep", "findmnt", "fuser",
                        "fusermount", "getfacl", "grep", "gunzip", "gzexe", "gzip", "hostname", "ip", "kbd_mode",
                        "kill", "kmod", "less", "lessecho", "lessfile", "lesskey", "lesspipe", "ln", "loadkeys",
                        "login", "loginctl", "lowntfs-3g", "ls", "lsblk", "lsmod", "mkdir", "mknod", "mktemp", "more",
                        "mount", "mountpoint", "mt", "mt-gnu", "mv", "nano", "nc", "nc.openbsd", "netcat", "netstat",
                        "nisdomainname", "ntfs-3g", "ntfs-3g.probe", "ntfs-3g.secaudit", "ntfs-3g.usermap", "ntfscat",
                        "ntfsck", "ntfscluster", "ntfscmp", "ntfsdump_logfile", "ntfsfix", "ntfsinfo", "ntfsls", "ntfsmftalloc",
                        "ntfsmove", "ntfstruncate", "ntfswipe", "open", "openvt", "pidof", "ping", "ping6", "plymouth",
                        "plymouth-upstart-bridge", "ps", "pwd", "rbash", "readlink", "red", "rm", "rmdir", "rnano",
                        "running-in-container", "run-parts", "sed", "setfacl", "setfont", "setupcon", "sh", "sh.distrib",
                        "sleep", "ss", "static-sh", "stty", "su", "sync", "tailf", "tar", "tempfile", "touch", "true",
                        "udevadm", "ulockmgr_server", "umount", "uname", "uncompress", "unicode_start", "vdir", "vmmouse_detect",
                        "which", "whiptail", "ypdomainname", "cat", "zcmp", "zdiff", "zegrep", "zfgrep", "zforce", "zgrep", "zless",
                        "zmore", "znew", NULL};

    for (int i = 0; external[i] != NULL; i++)
    {
        if (strcmp(external[i], args[0]) == 0)
        {
            return EXTERNAL;
        }
    }
    return NO_COMMAND;
}

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

void init_shell()
{
    system("clear");
    // Initialize the shell (e.g., set up environment, signal handlers)
    // init_signal_handlers();
}

// ctrl+c,fg ,bg,jobs,ctrl+d