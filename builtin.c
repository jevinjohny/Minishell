#include "minishell.h"


int command_type(char **args)
{
    // Check if the command is a built-in command (e.g., cd, exit, fg, bg)
    char *builtins[] = {"fg", "bg", "jobs", "echo", "printf", "read", "cd", /*"pwd" */ "pushd", "popd", "dirs", "let", "eval",
                        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(builtins[i], args[0]) == 0)
        {
            return BUILTIN;
        }
    }

    char *external[] = {"clear", "wc", "bash", "bunzip2", "busybox", "bzcat", "bzcmp", "bzdiff", "bzegrep", "bzexe",
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
    else if (strcmp(args[0], "jobs") == 0)
    {
        list_jobs();
    }
    else if (strcmp(args[0], "fg") == 0)
    {
        Job *temp = head;

        if (temp == NULL)
        {
            return;
        }

        while (temp->next)
        {
            temp = temp->next;
        }
        int id = temp->job_id;

        bring_job_to_foreground(id);
    }
    else if (strcmp(args[0], "bg") == 0)
    {
        Job *temp = head;

        if (temp == NULL)
        {
            return;
        }

        while (temp->next)
        {
            temp = temp->next;
        }
        int id = temp->job_id;

        send_job_to_background(id);
    }
}