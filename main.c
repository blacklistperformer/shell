// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// main.c file, it contains repl command for shell
// github repo link https://github.com/blacklistperformer/shell.git

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "shell.h"
#include "source.h"
#include "parser.h"

//the first function prints the first promt string, this string is before entering a command
void print_prompt1(void)
{
    char *path = getenv("PWD"), *log = getenv("LOGNAME"), *user = getenv("USER");
    fprintf(stderr, "\033[1;31m%s@%s\033[0m:\033[1;34m~%s\033[0m$ ", log, user, path);
}

// the second function prints the second promt string, this string is for multiu line command
void print_prompt2(void)
{
    fprintf(stderr, "> ");
}

int main(int argc, char **argv)
{
    char *cmd;
    // this command repl or read evaluate print loop is the command shell uses to to
    // read given command execute it adn then prit or basic io stuff and repeat untill
    // exited or terinating command like shutdown
    do
    {
        // printing shell's promt
        print_prompt1();

        // reading a command
        cmd = read_cmd();

        // impleenting shell's repl loop
        // if theres an error reading  a command, exiting the shell
        if (!cmd)
        {
            exit(EXIT_SUCCESS);
        }

        //if the command is empty we skip th input and continue with the  loop
        if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
        {
            free(cmd); //freeing the command
            continue;
        }

        //if the command is exit we exit the shell
        if (strcmp(cmd, "exit\n") == 0)
        {
            free(cmd); // freeing the command
            break;
        }

        struct source_s src;
        src.buffer   = cmd;
        src.bufsize  = strlen(cmd);
        src.curpos   = INIT_SRC_POS;
        parse_and_execute(&src);

        free(cmd); //freeing the command

    }

    while (1);
    exit(EXIT_SUCCESS);
}

//Reading User Input
char *read_cmd(void)
{
    //read input from stdin in 6969 byte chunks and store the input in a buffer
    char buf[6969];
    char *ptr = NULL;
    char ptrlen = 0;

    while(fgets(buf, 6969, stdin))
    {
        int buflen = strlen(buf);

        // reading first time input and allocating/creating our buffer with malloc()
        if(!ptr)
        {
            ptr = malloc(buflen+1);
        }

        //for subsequent inputs extendind buffer with realloc()
        else
        {
            char *ptr2 = realloc(ptr, ptrlen+buflen+1);

            if(ptr2)
            {
                ptr = ptr2;
            }

            else
            {
                free(ptr);
                ptr = NULL;
            }
        }

        //if something dosent turns well returning null pointer
        if(!ptr)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n",
                    strerror(errno));
            return NULL;
        }

        //copying the input to our buffer
        strcpy(ptr+ptrlen, buf);

        if(buf[buflen-1] == '\n')
        {
            if(buflen == 1 || buf[buflen-2] != '\\')
            {
                return ptr;
            }

            ptr[ptrlen+buflen-2] = '\0';
            buflen -= 2;
            print_prompt2();
        }

        ptrlen += buflen;
    }

    return ptr;
}

//parsing and executing file/ commands code

int parse_and_execute(struct source_s *src)
{
    skip_white_spaces(src);
    struct token_s *tok = tokenize(src);
    
    if(tok == &eof_token)
    {
        return 0;
    }

    while(tok && tok != &eof_token)
    {
        struct node_s *cmd = parse_simple_command(tok);
        if(!cmd)
        {
            break;
        }
        do_simple_command(cmd);
        free_node_tree(cmd);
        tok = tokenize(src);
    }

    return 1;
}
