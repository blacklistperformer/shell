// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// executor.h file
// github repo link https://github.com/blacklistperformer/shell.git

#ifndef BACKEND_H
#define BACKEND_H

#include "node.h"

char *search_path(char *file);
int do_exec_cmd(int argc, char **argv);
int do_simple_command(struct node_s *node);

#endif
