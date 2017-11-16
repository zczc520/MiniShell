#ifndef MYERROR_H
#define MYERROR_H

#include <stdio.h>
#include <errno.h>

// NOTE: Since all macros of error.h (errno) is larger than 0
//		 All custom error macros are equal with 0 or less then 0
//		 So there aren't any conflicts here

// success
#define ERR_SUCC 0 // success code

// overmuch error
#define ERR_OVER_TOKEN -0x10 // overmuch tokens
#define ERR_OVER_OPTION -0x11 // overmuch options
#define ERR_OVER_ARG -0x12 // toomuch arguments
#define ERR_OVER_INPUT -0x13 // toomuch input source
#define ERR_OVER_OUTPUT -0x14 // toomuch output channel
#define ERR_OVER_CMD -0x15 // toomuch commands

// invalid error
#define ERR_INVALID_PIPE -0x20 // invalid using of pipe symbol(blank after pipe)
#define ERR_INVALID_OPTION -0x21 // invalid option character or lack of option or redundant option
#define ERR_INVALID_NAME -0x22 // invalid command name
#define ERR_INVALID_ARG -0x23 // invalid argument or lack of argument or redundant argument
#define ERR_INVALID_INPUT -0x24 // invalid redirection input source or redundant reinput
#define ERR_INVALID_OUTPUT -0x25 // invalid redirection output channel or redundant reoutput
#define ERR_INVALID_OUTPUT_TYPE -0x26 // invalid redireciton output type
#define ERR_INVALID_JOBID -0x27 // invalid job id
#define ERR_INVALID_EXIT -0x28 // invalid exit(suspended job exist)

// C operation error
#define ERR_MEMCPY -0x30 // memcpy error
#define ERR_STRCPY -0x31 // strcpy error
#define ERR_CHDIR - 0x32 // chdir error

// unknown error
#define ERR_UNKNOWN -0x100 // unknown error

extern void printerr(int errcode);

#endif
