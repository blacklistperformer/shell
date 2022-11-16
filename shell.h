// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// shell.h file
// github repo link https://github.com/blacklistperformer/shell.git

#ifndef SHELL_H
#define SHELL_H

#include "source.h"

void print_prompt1(void);
void print_prompt2(void);

char *read_cmd(void);

int parse_and_execute(struct source_s *src);


#endif
