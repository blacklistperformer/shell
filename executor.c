// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// executort.c file, it contains execution files of the comand input bruh
// github repo link https://github.com/blacklistperformer/shell.git

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "shell.h"
#include "node.h"
#include "executor.h"

void updatePath(char *dir)
{
    char *path = getenv("PWD"), sub[3] = "..";
    int i, j, count = 0, count1 = 0;
    int l1 = strlen(dir), l2 = strlen(sub);

    if (!strcmp(dir, "."))
    {
        printf("%s\n", path);
        return;
    }

    for (i = 0; i < l1;)
    {
        j = 0;
        count = 0;

        while ((dir[i] == sub[j]))
        {
            count++;
            i++;
            j++;
        }

        if (count == l2)
        {
            count1++;
            count = 0;
        }

        else
            i++;
    }

    if (count1 || (!strcmp(dir, "..")))
    {
        if (!count1)
            count1 = 1;

        while (count1--)
        {
            char c;
            int i = 0, num[20], j = 0;

            do
            {
                c = path[i];

                if (c == '/')
                {
                    num[j] = i;
                    j++;
                }

                i++;
            }

            while (c != '\0');
            j--;
            path[num[j]] = '\0';
            setenv("PWD", path, 1);
            printf("%s\n", path);
        }

        return;
    }

    strcat(path, "/");
    strcat(path, dir);
    setenv("PWD", path, 1);
    printf("%s\n", path);
}

void echo(int argc, char **argv)
{
    for (int i = 1; argv[i] != NULL; i++)
        printf("%s ", argv[i]);
    printf("\n");
}

void pwd(int argc, char **argv)
{
    printf("%s\n", getenv("PWD"));
}

char *search_path(char *file)
{
    char *PATH = getenv("PATH");
    char *p = PATH;
    char *p2;

    while (p && *p)
    {
        p2 = p;

        while (*p2 && *p2 != ':')
        {
            p2++;
        }

        int plen = p2 - p;

        if (!plen)
        {
            plen = 1;
        }

        int alen = strlen(file);
        char path[plen + 1 + alen + 1];

        strncpy(path, p, p2 - p);
        path[p2 - p] = '\0';

        if (p2[-1] != '/')
        {
            strcat(path, "/");
        }

        strcat(path, file);

        struct stat st;

        if (stat(path, &st) == 0)
        {
            if (!S_ISREG(st.st_mode))
            {
                errno = ENOENT;
                p = p2;

                if (*p2 == ':')
                {
                    p++;
                }
                continue;
            }

            p = malloc(strlen(path) + 1);

            if (!p)
            {
                return NULL;
            }

            strcpy(p, path);
            return p;
        }

        else // file not found
        {
            p = p2;
            if (*p2 == ':')
            {
                p++;
            }
        }
    }

    errno = ENOENT;
    return NULL;
}

// exec commands
int do_exec_cmd(int argc, char **argv)
{

    if (strchr(argv[0], '/'))
    {
        execv(argv[0], argv);
    }

    else
    {
        char *path = search_path(argv[0]);
        if (!path)
        {
            return 0;
        }
        execv(path, argv);
        free(path);
    }
    return 0;
}

static inline void free_argv(int argc, char **argv)
{
    if (!argc)
    {
        return;
    }

    while (argc--)
    {
        free(argv[argc]);
    }
}

int do_simple_command(struct node_s *node)
{
    if (!node)
    {
        return 0;
    }

    struct node_s *child = node->first_child;

    if (!child)
    {
        return 0;
    }

    int argc = 0;
    long max_args = 255;
    char *argv[max_args + 1]; /* keep 1 for the terminating NULL arg */
    char *str;

    while (child)
    {
        str = child->val.str;
        argv[argc] = malloc(strlen(str) + 1);

        if (!argv[argc])
        {
            free_argv(argc, argv);
            return 0;
        }

        strcpy(argv[argc], str);

        if (++argc >= max_args)
            break;

        child = child->next_sibling;
    }

    argv[argc] = NULL;

    if (!strcmp(argv[0], "cd"))
    {
        char *direc = argv[1];

        if (!chdir(direc))
        {
            updatePath(direc);
        }
        else if (strcmp(direc, "-") == 0)
        {
            // chdir("..");
            printf("%s\n", getenv("PWD"));
        }
        else if (strcmp(direc, "~") == 0)
        {
            // chdir("..");
            // updatepath(direc);
            // printf("%s\n", getenv("PWD"));

            // printf("Hi");

            char *path1;
            char *home_directory = getenv("HOME");

            if (home_directory == NULL)
            {
                printf("Error: HOME environment variable not set\n");
                return 1;
            }
            path1 = home_directory;

            int success = chdir(path1);
            if (success == -1)
            {
                printf("Error: Unable to change directory to %s\n", path1);
                return 1;
            }
            // printf("%s\n", getenv("PWD"));
            // updatePath(home_directory);
            setenv("PWD", path1, 1);
            printf("%s\n", path1);
            return 0;
        }
        else
        {
            printf("Error: no such directory. \n");
        }

        return 1;
    }

    else if (!strcmp(argv[0], "echo"))
    {
        echo(argc, argv);
        return 1;
    }

    else if (!strcmp(argv[0], "pwd"))
    {
        pwd(argc, argv);
        return 1;
    }

    else
    {
        pid_t child_pid = 0;

        if ((child_pid = fork()) == 0)
        {
            do_exec_cmd(argc, argv);
            fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));

            if (errno == ENOEXEC)
            {
                exit(126);
            }

            else if (errno == ENOENT)
            {
                exit(127);
            }

            else
            {
                exit(EXIT_FAILURE);
            }
        }

        else if (child_pid < 0)
        {
            fprintf(stderr, "error: failed to fork command: %s\n", strerror(errno));
            return 0;
        }

        int status = 0;
        waitpid(child_pid, &status, 0);
        free_argv(argc, argv);
    }

    return 1;
}
