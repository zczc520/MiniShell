#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// configuration of mini shell
#define MAX_LEN 255 // max length of string
#define MAX_TOKEN 40 // max num of tokens
#define MAX_CMD 5 // max num of commands which are linked by pipe
#define MAX_OPTION 10 // max num of options
#define MAX_ARG 5 // max num of arguments(not include options)
#define MAX_INPUT 1 // single source of reinput temporarily
#define MAX_OUTPUT 1 // single channel of reoutput temporarily

//symbols
#define SYMBOL_PIPE "|"
#define SYMBOL_REINPUT "<"
#define SYMBOL_REOUTPUT_COVER ">"
#define SYMBOL_REOUTPUT_APPEND ">>"

//redirection type for output
#define TYPE_UNKNOWN 0
#define TYPE_COVER 1
#define TYPE_APPEND 2

// string of commands
#define CMDSTR_LS "ls"
#define CMDSTR_CD "cd"
#define CMDSTR_EXIT "exit"

extern int execmd(const char* str);

struct Cmd{
	char name[MAX_LEN]; // command name(string)
	int optionNum; // num of options
	char options[MAX_OPTION]; // options(characters)
	int argNum; // num of arguments
	char args[MAX_ARG][MAX_LEN]; // args(strings)
	int inputNum; // num of redirection input(single)
	char reInput[MAX_INPUT][MAX_LEN]; // redirection input(single source of input)
	int outputNum; // num of redirection output(single)
	int outputType; // type of redirection output(TYPE_COVER or TYPE_APPEND)
	char reOutput[MAX_OUTPUT][MAX_LEN]; // redirection output(single channel of output)
};

#endif
