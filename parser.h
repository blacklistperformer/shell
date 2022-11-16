// This code is writen by Neeraj Nikhil Roy 20/11/EC/53 and submitted to Dr. Mukesh Kumar Giluka
// OS Assignment designing a shell
// source.h
// github repo link https://github.com/blacklistperformer/shell.git

#ifndef PARSER_H

#define PARSER_H

#include "scanner.h" //struct token_s

#include "source.h" //struct source_s

struct node_s *parse_simple_command(struct token_s *tok);

#endif
